#include <utility>
#include <vector>

#include "hash_table.h"


#ifndef INVERTED_INDEX_SET_H
#define INVERTED_INDEX_SET_H
// does not store any value
class Set : ConcurrentHashTable<bool> {
public:
    Set() : ConcurrentHashTable<bool>(8) {}

    void add(std::string key) {
        ConcurrentHashTable<bool>::set(std::move(key), true);
    }

    bool has(std::string key) {
        return ConcurrentHashTable<bool>::get(std::move(key), false);
    }

    std::vector<std::string> getItemsVector() {
        std::vector<std::string> items;
        for (int i = 0; i < ConcurrentHashTable<bool>::size; i++) {
            auto hashTableNodesVector = ConcurrentHashTable<bool>::array[i]->toVector();
            for (auto & node : hashTableNodesVector)
                items.push_back(node->key);
        }

        return items;
    }
};
#endif
