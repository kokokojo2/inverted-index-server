#include <iostream>
#include "node.h"

#ifndef INVERTED_INDEX_LINKED_LIST_H
#define INVERTED_INDEX_LINKED_LIST_H
template <class valueT> class LinkedList {
    LinkedListNode<valueT>* head;

public:
    LinkedList() {
        head = nullptr;
    }

    // adds newNode element to the front
    void push(HashtableNode<valueT>* hashtableNode) {
        auto* newNode = new LinkedListNode<valueT>(hashtableNode);
        if (head == nullptr) {
            head = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
    }

    // updates the node if the key matches
    bool update(HashtableNode<valueT>* newNode) {
        LinkedListNode<valueT>* currentNode = head;
        while (currentNode != nullptr) {
            if (*currentNode->hashtableNode == *newNode) {
                currentNode->hashtableNode = newNode;
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }

    bool find(HashtableNode<valueT>* hashtableNode) {
        LinkedListNode<valueT>* currentNode = head;
        while (currentNode != nullptr) {
            if (*currentNode->hashtableNode == *hashtableNode) {
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }

    void print() {
        LinkedListNode<valueT>* currentNode = head;
        while (currentNode != nullptr) {
            currentNode->display();
            currentNode = currentNode->next;
            std::cout << std::endl;
        }
    }
};

#endif
