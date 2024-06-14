
#include "Node.h"


Node::Node(const string& ip, int port) : ip(ip), port(port), nodeinfo(ip, port), predecessor(nullptr), successor(nullptr)  {
    id = hash(nodeinfo.toString());
    finger_table = new FingerTable(id);
}

int Node::hash(const string& message) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)&message[0], message.size(), (unsigned char*)&digest);    
    unsigned long long int hash = 0;
    for(int i = 0; i < 4; i++) {
        hash = (hash << 8) | digest[i];
    }
    return hash % (1 << m);
}

void Node::start() {
    thread stabilize_thread(&Node::stabilize, this);
    thread fix_finger_thread(&Node::fix_fingers, this);
    stabilize_thread.detach();
    fix_finger_thread.detach();

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        cerr << "Socket creation failed\n";
        return;
    }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    address.sin_port = htons(port);

    if (::bind(server_sock, (struct sockaddr*)&address, sizeof(address)) == -1) {
        cerr << "Bind failed\n";
        return;
    }

    if (listen(server_sock, 3) == -1) {
        cerr << "Listen failed\n";
        return;
    }

    while (true) {
        int new_socket = accept(server_sock, NULL, NULL);
        if (new_socket == -1) {
            cerr << "Accept failed\n";
            continue;
        }
        thread(&Node::serve_requests, this, new_socket).detach();
    }
}

void Node::serve_requests(int conn) {
    char buffer[1024] = {0};
    read(conn, buffer, 1024);
    string response = process_requests(buffer);
    send(conn, response.c_str(), response.size(), 0);
    close(conn);
}

string Node::process_requests(const string& message) {
    string operation = message.substr(0, message.find("|"));
    string args = message.substr(message.find("|") + 1);
    string result = "Done";

    if (operation == "insert_server") {
        string key = args.substr(0, args.find(":"));
        string value = args.substr(args.find(":") + 1);
        data_store.insert(key, value);
        result = "Inserted";
    } else if (operation == "delete_server") {
        data_store.remove(args);
        result = "Deleted";
    } else if (operation == "search_server") {
        result = data_store.search(args);
    } else if (operation == "send_keys") {
        int id_of_joining_node = stoi(args);
        result = send_keys(id_of_joining_node);
    } else if (operation == "insert") {
        string key = args.substr(0, args.find(":"));
        string value = args.substr(args.find(":") + 1);
        result = insert_key(key, value);
    } else if (operation == "delete") {
        result = delete_key(args);
    } else if (operation == "search") {
        result = search_key(args);
    } else if (operation == "join_request") {
        result = join_request_from_other_node(stoi(args));
    } else if (operation == "find_predecessor") {
        result = find_predecessor(stoi(args));
    } else if (operation == "find_successor") {
        result = find_successor(stoi(args));
    } else if (operation == "get_successor") {
        result = get_successor();
    } else if (operation == "get_predecessor") {
        result = get_predecessor();
    } else if (operation == "get_id") {
        result = get_id();
    } else if (operation == "notify") {
        string node_ip = args.substr(args.find("|") + 1, args.rfind("|") - args.find("|") - 1);
        int node_port = stoi(args.substr(args.rfind("|") + 1));
        notify(stoi(args.substr(0, args.find("|"))), node_ip, node_port);
    }

    return result;
}

string Node::insert_key(const string& key, const string& value) {
    int id_of_key = hash(key);
    string succ = find_successor(id_of_key);
    auto[ip, port] = get_ip_port(succ);
    request_handler.send_message(ip, port, "insert_server|" + key + ":" + value);
    return "Inserted at node id " + to_string(Node(ip, port).id) + " key was " + key + " key hash was " + to_string(id_of_key);
}

string Node::delete_key(const string& key) {
    int id_of_key = hash(key);
    string succ = find_successor(id_of_key);
    auto[ip, port] = get_ip_port(succ);
    request_handler.send_message(ip, port, "delete_server|" + key);
    return "deleted at node id " + to_string(Node(ip, port).id) + " key was " + key + " key hash was " + to_string(id_of_key);
}

string Node::search_key(const string& key) {
    int id_of_key = hash(key);
    string succ = find_successor(id_of_key);
    auto[ip, port] = get_ip_port(succ);
    return request_handler.send_message(ip, port, "search_server|" + key);
}

string Node::join_request_from_other_node(int node_id) {
    return find_successor(node_id);
}

void Node::join(const string& node_ip, int node_port) {
    string data = "join_request|" + to_string(id);
    string succ = request_handler.send_message(node_ip, node_port, data);
    auto[ip, port] = get_ip_port(succ);
    successor = new Node(ip, port);
    (*finger_table).table[0].second = successor;
    predecessor = nullptr;

    if (successor->id != id) {
        string keys = request_handler.send_message(successor->ip, successor->port, "send_keys|" + to_string(id));
        vector<string> key_value_pairs;
        string key_value;
        for (const auto& key_value_pair : keys) {
            if (key_value_pair == ':') {
                auto delimiter_pos = key_value.find('|');
                data_store.insert(key_value.substr(0, delimiter_pos), key_value.substr(delimiter_pos + 1));
                key_value.clear();
            } else {
                key_value += key_value_pair;
            }
        }
    }
}

string Node::find_predecessor(int search_id) {
    if (search_id == id) {
        return nodeinfo.toString();
    }

    if (predecessor != nullptr && successor->id == id) {
        return nodeinfo.toString();
    }

    if (get_forward_distance(successor->id) > get_forward_distance(search_id)) {
        return nodeinfo.toString();
    } else {
        Node* new_node_hop = closest_preceding_node(search_id);
        if (new_node_hop == nullptr) {
            return "None";
        }

        auto[ip, port] = get_ip_port(new_node_hop->nodeinfo.toString());
        if (ip == this->ip && port == this->port) {
            return nodeinfo.toString();
        }

        return request_handler.send_message(ip, port, "find_predecessor|" + to_string(search_id));
    }
}

string Node::find_successor(int search_id) {
    if (search_id == id) {
        return nodeinfo.toString();
    }

    string predecessor = find_predecessor(search_id);
    auto[ip, port] = get_ip_port(predecessor);
    return request_handler.send_message(ip, port, "get_successor");
}

Node* Node::closest_preceding_node(int search_id) {
    Node* closest_node = nullptr;
    int min_distance = (1 << m) + 1;

    for (int i = m - 1; i >= 0; --i) {
        if ((*finger_table).table[i].second != nullptr &&
            get_forward_distance_2nodes((*finger_table).table[i].second->id, search_id) < min_distance) {
            closest_node = (*finger_table).table[i].second;
            min_distance = get_forward_distance_2nodes((*finger_table).table[i].second->id, search_id);
        }
    }

    return closest_node;
}

string Node::send_keys(int id_of_joining_node) {
    string data;
    vector<string> keys_to_be_removed;

    for (const auto& [key, value] : data_store.data) {
        int key_id = hash(key);
        if (get_forward_distance_2nodes(key_id, id_of_joining_node) < get_forward_distance_2nodes(key_id, id)) {
            data += key + "|" + value + ":";
            keys_to_be_removed.push_back(key);
        }
    }

    for (const auto& key : keys_to_be_removed) {
        data_store.remove(key);
    }

    return data;
}

void Node::stabilize() {
    while (true) {
        if (successor == nullptr) {
            this_thread::sleep_for(chrono::seconds(10));
            continue;
        }

        string data = "get_predecessor";

        if (successor->ip == ip && successor->port == port) {
            this_thread::sleep_for(chrono::seconds(10));
            continue;
        }

        string result = request_handler.send_message(successor->ip, successor->port, data);

        if (result == "None" || result.empty()) {
            request_handler.send_message(successor->ip, successor->port, "notify|" + to_string(id) + "|" + nodeinfo.toString());
            continue;
        }

        auto[ip, port] = get_ip_port(result);
        result = request_handler.send_message(ip, port, "get_id");

        if (get_backward_distance(stoi(result)) > get_backward_distance(successor->id)) {
            successor = new Node(ip, port);
            (*finger_table).table[0].second = successor;
        }

        request_handler.send_message(successor->ip, successor->port, "notify|" + to_string(id) + "|" + nodeinfo.toString());

        cout << "===============================================\n";
        cout << "STABILIZING\n";
        cout << "===============================================\n";
        cout << "ID: " << id << "\n";
        if (successor != nullptr) {
            cout << "Successor ID: " << successor->id << "\n";
        }
        if (predecessor != nullptr) {
            cout << "Predecessor ID: " << predecessor->id << "\n";
        }
        cout << "===============================================\n";
        cout << "=============== FINGER TABLE ==================\n";
        (*finger_table).print();
        cout << "===============================================\n";
        cout << "DATA STORE\n";
        cout << "===============================================\n";
        cout << "Data: \n";
        // Print the data store contents
        cout << "===============================================\n";
        cout << "+++++++++++++++ END +++++++++++++++++++++++++++\n\n\n";

        this_thread::sleep_for(chrono::seconds(10));
    }
}

void Node::notify(int node_id, const string& node_ip, int node_port) {
    if (predecessor != nullptr) {
        if (get_backward_distance(node_id) < get_backward_distance(predecessor->id)) {
            predecessor = new Node(node_ip, node_port);
            return;
        }
    }

    if (predecessor == nullptr || (node_id > predecessor->id && node_id < id) || (id == predecessor->id && node_id != id)) {
        predecessor = new Node(node_ip, node_port);
        if (id == successor->id) {
            successor = new Node(node_ip, node_port);
            (*finger_table).table[0].second = successor;
        }
    }
}

void Node::fix_fingers() {
    while (true) {
        int random_index = rand() % (m - 1) + 1;
        int finger = (*finger_table).table[random_index].first;
        string data = find_successor(finger);
        if (data == "None") {
            this_thread::sleep_for(chrono::seconds(10));
            continue;
        }
        auto[ip, port] = get_ip_port(data);
        (*finger_table).table[random_index].second = new Node(ip, port);
        this_thread::sleep_for(chrono::seconds(10));
    }
}

string Node::get_successor() {
    return successor ? successor->nodeinfo.toString() : "None";
}

string Node::get_predecessor() {
    return predecessor ? predecessor->nodeinfo.toString() : "None";
}

string Node::get_id() {
    return to_string(id);
}

pair<string, int> Node::get_ip_port(const string& string_format) {
    auto delimiter_pos = string_format.find('|');
    string ip = string_format.substr(0, delimiter_pos);
    int port = stoi(string_format.substr(delimiter_pos + 1));
    return {ip, port};
}

int Node::get_backward_distance(int node1) {
    int distance = 0;
    if (id > node1) {
        distance = id - node1;
    } else if (id == node1) {
        distance = 0;
    } else {
        distance = (1 << m) - (node1 - id);
    }
    return distance;
}

int Node::get_backward_distance_2nodes(int node2, int node1) {
    int distance = 0;
    if (node2 > node1) {
        distance = node2 - node1;
    } else if (node2 == node1) {
        distance = 0;
    } else {
        distance = (1 << m) - (node1 - node2);
    }
    return distance;
}

int Node::get_forward_distance(int nodeid) {
    return (1 << m) - get_backward_distance(nodeid);
}

int Node::get_forward_distance_2nodes(int node2, int node1) {
    return (1 << m) - get_backward_distance_2nodes(node2, node1);
}