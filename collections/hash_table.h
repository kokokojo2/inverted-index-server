#include <string>
#include <iostream>
#include <string>

#include "node.h"
#include "linked_list.h"

#ifndef INVERTED_INDEX_HASH_TABLE_H
#define INVERTED_INDEX_HASH_TABLE_H
template <class valueT> class HashTable {
    unsigned long size;
    unsigned long bucketsUsed;

    LinkedList<valueT>** array;

    void initializeArray(LinkedList<valueT>** arr, unsigned long arraySize) {
        for (int i = 0; i < arraySize; i++) {
            arr[i] = new LinkedList<valueT>();
        }
    }
    static unsigned long getHash(const std::string& key) {
        std::hash<std::string> hash;
        return hash(key);
    }

    unsigned long getIndex(const std::string& key) const {
        return HashTable::getHash(key) % size;
    }

    void resize() {
        auto oldSize = size;
        auto oldArray = array;

        size = size * 2;
        array = new LinkedList<valueT>* [size];
        initializeArray(array, size);


        for (int i = 0; i < oldSize; i++) {
            HashtableNode<std::string>* popped;
            while ((popped = oldArray[i]->pop())!= nullptr) {
                set(popped->key, popped->value);
            }
        }
        // TODO: delete all elements first
        delete [] oldArray;
    }

public:
    HashTable(unsigned long initialSize=512) {
        size = initialSize;
        array = new LinkedList<valueT>* [size];
        initializeArray(array, size);
    }

    // inserts the node if key doesn't exist
    // updates the node's value if key exist
    void set(std::string key, valueT value) {
        unsigned long index = this->getIndex(key);
        auto* node = new HashtableNode<valueT>(key, value);

        if (array[index]->find(node->key) != nullptr) {
            array[index]->update(node);
        } else {
            array[index]->push(node);
        }
        bucketsUsed++;

        if ((float)bucketsUsed / size > 0.75) {
            std::cout << "resizing" << std::endl;
            std::cout << "buckets used - " << bucketsUsed << std::endl;
            std::cout << "current size - " << size;

            resize();
            std::cout << ", new size - " << size << std::endl;
        }
    }

    // returns node's value if the node with key exists
    // returns nullptr if node with key does not exist
    valueT get(std::string key, valueT defaultValue) {
        unsigned long index = this->getIndex(key);
        auto foundNode = array[index]->find(key);
        if (foundNode != nullptr) {
            return foundNode->value;
        }
        return defaultValue;
    }

};
#endif