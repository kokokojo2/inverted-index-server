#include <utility>

#include "hash_table.h"


#ifndef INVERTED_INDEX_SET_H
#define INVERTED_INDEX_SET_H
// does not store any value
// TODO: factor out implementations to set.cpp file
class Set : HashTable<bool> {
public:
    Set() : HashTable<bool>() {}

    void add(std::string key) {
        HashTable<bool>::set(std::move(key), true);
    }

    bool has(std::string key) {
        return HashTable<bool>::get(std::move(key), false);
    }
};
#endif
