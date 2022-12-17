#include <iostream>

#ifndef INVERTED_INDEX_NODE_H
#define INVERTED_INDEX_NODE_H
template<class valueT> class HashtableNode {
public:
    std::string key;
    valueT value;
    HashtableNode(std::string k, valueT val) : key(std::move(k)), value(val) {};
    void display() {
        std::cout << "<HashtableNode key='" << key << "' val='" << value << "'>";
    }
};


template <class valueT> class LinkedListNode {
public:
    HashtableNode<valueT>* hashtableNode;
    LinkedListNode *next = nullptr;
    LinkedListNode(HashtableNode<valueT>* newValue) {
        hashtableNode = newValue;
    };
    void display() {
        std::cout << "<LinkedListNode value=";
        hashtableNode->display();
        std::cout << ">";

    }
};
#endif
