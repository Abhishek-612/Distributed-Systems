#ifndef _NODE_H_
#define _NODE_H_

#include "common.h"
#include "DataStore.h"
#include "RequestHandler.h"
#include "ChordProtocol.h"


const int m = 7;

class FingerTable;

class NodeInfo {
public:
    NodeInfo(const string& ip, int port) : ip(ip), port(port) {}

    string toString() const {
        return ip + "|" + to_string(port);
    }

    string ip;
    int port;
};

class Node {
public:
    string ip;
    int port;
    int id;
    NodeInfo nodeinfo;
    Node* predecessor;
    Node* successor;
    FingerTable* finger_table;
    DataStore data_store;
    RequestHandler request_handler;
    
    Node(const string& ip, int port);
    int hash(const string& message);

    void start();

    void serve_requests(int conn);

    string process_requests(const string& message);

    string insert_key(const string& key, const string& value);

    string delete_key(const string& key);

    string search_key(const string& key);

    string join_request_from_other_node(int node_id);

    void join(const string& node_ip, int node_port);

    string find_predecessor(int search_id);

    string find_successor(int search_id);

    Node* closest_preceding_node(int search_id);
    string send_keys(int id_of_joining_node);

    void stabilize();

    void notify(int node_id, const string& node_ip, int node_port);

    void fix_fingers();
    string get_predecessor();
    string get_successor();
    string get_id();

    pair<string, int> get_ip_port(const string& string_format);

    int get_backward_distance(int node1);

    int get_backward_distance_2nodes(int node2, int node1);

    int get_forward_distance(int nodeid);
    int get_forward_distance_2nodes(int node2, int node1);

};

#endif /* _NODE_H_ */