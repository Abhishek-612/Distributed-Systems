
#include "ChordProtocol.h"


FingerTable::FingerTable(int my_id) {
    for (int i = 0; i < m; ++i) {
        int entry = (my_id + (1 << i)) % (1 << m);
        table.push_back({entry, nullptr});
    }
}

void FingerTable::print() {
    for (int i = 0; i < table.size(); ++i) {
        cout << "Entry: " << i << " Interval start: " << table[i].first << " Successor: " 
                    << (table[i].second ? to_string(table[i].second->id) : "None") << endl;
    }
}
