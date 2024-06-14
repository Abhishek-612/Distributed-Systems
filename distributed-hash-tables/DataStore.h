#ifndef _DATASTORE_H_
#define _DATASTORE_H_

#include "common.h"
#include <unordered_map>

class DataStore {

public:
    unordered_map<string, string> data;

    void insert(const string& key, const string& value);
    void remove(const string& key);
    string search(const string& key);

};

#endif /* _DATASTORE_H__ */