#include <iostream>
#include <string>
#include "Application.h"

int main(int argc, char* argv[]) {
    string ip = "127.0.0.1";

    if (argc == 3) {
        cout << "JOINING RING" << endl;
        Node node(ip, stoi(argv[1]));
        node.join(ip, stoi(argv[2]));
        node.start();
    } else if (argc == 2) {
        cout << "CREATING RING" << endl;
        Node node(ip, stoi(argv[1]));
        node.predecessor = new Node(ip, stoi(argv[1]));
        node.successor = new Node(ip, stoi(argv[1]));
        node.finger_table->table[0].second = &node;
        node.start();
    } else {
        cerr << "Usage: " << argv[0] << " <port> [join_port]" << endl;
        return 1;
    }

    return 0;
}