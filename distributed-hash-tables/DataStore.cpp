
#include "DataStore.h"


void DataStore::insert(const string& key, const string& value) {
    data[key] = value;
}

void DataStore::remove(const string& key) {
    data.erase(key);
}

string DataStore::search(const string& key) {
    if (data.find(key) != data.end()) {
        return data[key];
    } else {
        return "NOT FOUND";
    }
}
