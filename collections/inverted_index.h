#include <utility>

#include "set.h"
#include "hash_table.h"

#ifndef INVERTED_INDEX_INVERTED_INDEX_H
#define INVERTED_INDEX_INVERTED_INDEX_H

class ConcurrentInvertedIndex : ConcurrentHashTable<Set*> {
public:
    ConcurrentInvertedIndex(unsigned long initialSize) : ConcurrentHashTable<Set*>(initialSize) {};

    void addToIndex(const std::string& word, std::string docId) {
        auto set = ConcurrentHashTable<Set*>::getOrSetDefault(word, new Set());
        set->add(std::move(docId));
    }

    Set* getDocIds(const std::string& word) {
        return ConcurrentHashTable<Set*>::get(word, new Set());
    }
};


#endif
