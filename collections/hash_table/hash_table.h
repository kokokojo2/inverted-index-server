#include <string>
#include <iostream>

#ifndef INVERTED_INDEX_HASH_TABLE_H
#define INVERTED_INDEX_HASH_TABLE_H
template<class valueT> class HashtableNode {
public:
    std::string key;
    valueT value;
    HashtableNode(std::string k, valueT val) : key(std::move(k)), value(val) {};
    void display() {
        std::cout << "<HashtableNode key='" << key << "' val='" << value << "'>";
    }

    bool operator ==(const HashtableNode &hashtableNode) {
        return this->key == hashtableNode.key;
    }
};
#endif