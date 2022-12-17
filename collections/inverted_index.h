#include <utility>

#include "set.h"
#include "hash_table.h"

#ifndef INVERTED_INDEX_INVERTED_INDEX_H
#define INVERTED_INDEX_INVERTED_INDEX_H

class ConcurrentInvertedIndex : ConcurrentHashTable<Set*> {
public:
    ConcurrentInvertedIndex() : ConcurrentHashTable<Set*>(6) {};

    // todo: synchronize concurrent set update
    void addToIndex(const std::string& word, std::string docId) {
        auto set = ConcurrentHashTable<Set*>::get(word, new Set());
        set->add(std::move(docId));
        ConcurrentHashTable<Set*>::set(word, set);

    }

    Set* getDocIds(const std::string& word) {
        return ConcurrentHashTable<Set*>::get(word, new Set());
    }
};


#endif
