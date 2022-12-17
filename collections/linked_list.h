#include <iostream>
#include <vector>

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
            if (currentNode->hashtableNode->key == newNode->key) {
                currentNode->hashtableNode = newNode;
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }

    HashtableNode<valueT>* find(const std::string& key) {
        LinkedListNode<valueT>* currentNode = head;
        while (currentNode != nullptr) {
            if (currentNode->hashtableNode->key == key) {
                return currentNode->hashtableNode;
            }
            currentNode = currentNode->next;
        }
        return nullptr;
    }

    HashtableNode<valueT>* pop() {
        if (head == nullptr) return nullptr;
        LinkedListNode<valueT>* popped = head;
        head = head->next;
        return popped->hashtableNode;
    }

    void print() {
        LinkedListNode<valueT>* currentNode = head;
        while (currentNode != nullptr) {
            currentNode->display();
            currentNode = currentNode->next;
            std::cout << std::endl;
        }
    }

    std::vector<HashtableNode<valueT>*> toVector() {
        std::vector<HashtableNode<valueT>*> vec;
        LinkedListNode<valueT>* currentNode = head;

        while (currentNode != nullptr) {
            vec.push_back(currentNode->hashtableNode);
            currentNode = currentNode->next;
        }

        return vec;
    }
};

#endif
