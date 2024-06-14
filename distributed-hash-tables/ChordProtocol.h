#ifndef _CHORDPROTOCOL_H_
#define _CHORDPROTOCOL_H_

#include "common.h"
#include "Node.h"

class Node;

class FingerTable {
public:
    vector<pair<int, Node*>> table;

    FingerTable(int my_id);
    void print();

};

#endif /* _CHORDPROTOCOL_H_ */