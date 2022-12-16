#include <utility>

#include "set.h"
#include "hash_table.h"

#ifndef INVERTED_INDEX_INVERTED_INDEX_H
#define INVERTED_INDEX_INVERTED_INDEX_H

class ConcurrentInvertedIndex : ConcurrentHashTable<Set*> {
public:
    ConcurrentInvertedIndex() : ConcurrentHashTable<Set*>(1) {};

    // todo: synchronize concurrent set update
    void addToIndex(const std::string& word, std::string docId) {
        //safe_print("Getting docs set for key=" + word, std::to_string(procId));
        auto set = ConcurrentHashTable<Set*>::get(word, new Set());
        //safe_print("Modifying the set", std::to_string(procId));
        set->add(docId);
        //safe_print("Setting the new set for doc", std::to_string(procId));
        ConcurrentHashTable<Set*>::set(word, set);

    }

    Set* getDocIds(const std::string& word) {
        return ConcurrentHashTable<Set*>::get(word, new Set());
    }
};


#endif
