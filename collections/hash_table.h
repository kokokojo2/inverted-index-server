#include <iostream>
#include <string>
#include <cmath>

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "node.h"
#include "linked_list.h"
#include "../utils/io/output.h"

#ifndef INVERTED_INDEX_HASH_TABLE_H
#define INVERTED_INDEX_HASH_TABLE_H
template <class valueT> class HashTable {
protected:
    unsigned long size;
    unsigned long bucketsUsed;
    const float LOAD_FACTOR = 0.75;

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

    void unsafeSet(std::string key, valueT value) {
        unsigned long index = this->getIndex(key);
        auto* node = new HashtableNode<valueT>(key, value);

        // determining if the new node will be added
        bool isNewKey = array[index]->find(node->key) == nullptr;
        bool shouldResize = (float)(bucketsUsed + isNewKey) / size > LOAD_FACTOR;

        if (!isNewKey) {
            array[index]->update(node);
        } else {
            array[index]->push(node);
            bucketsUsed++;
        }

    }

    void resize() {
        auto oldSize = size;
        auto oldArray = array;

        size = size * 2;
        array = new LinkedList<valueT>* [size];
        initializeArray(array, size);


        for (int i = 0; i < oldSize; i++) {
            HashtableNode<valueT>* popped;
            while ((popped = oldArray[i]->pop())!= nullptr) {
                unsafeSet(popped->key, popped->value);
            }
        }
        // TODO: delete all elements first
        delete [] oldArray;
    }

public:
    HashTable(unsigned long initialSize=64) {
        size = initialSize;
        array = new LinkedList<valueT>* [size];
        initializeArray(array, size);
    }

    // inserts the node if key doesn't exist
    // updates the node's value if key exist
    void set(std::string key, valueT value) {
        unsigned long index = this->getIndex(key);
        auto* node = new HashtableNode<valueT>(key, value);

        // determining if the new node will be added
        bool isNewKey = array[index]->find(node->key) == nullptr;
        bool shouldResize = (float)(bucketsUsed + isNewKey) / size > LOAD_FACTOR;

        if (!isNewKey) {
            array[index]->update(node);
        } else {
            array[index]->push(node);
            bucketsUsed++;
        }

        if (shouldResize) {
            resize();
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

template <class valueT> class ConcurrentHashTable {
protected:
    unsigned long size;
    const float LOAD_FACTOR = 0.75;
    LinkedList<valueT>** array;

    // thread safe members
    std::atomic<bool> ongoingResize;
    std::atomic<unsigned long> bucketsUsed;
    std::atomic<unsigned int> concurrentWriters;

    // synchronization members
    unsigned long CHUNK_SIZE = 100;
    std::mutex checkResizeMtx, resizingCvMtx;
    std::condition_variable resizingCv;
    std::mutex** chunkMutexesArray;
    unsigned long chunkMutexesArraySize;

    void initializeArray(LinkedList<valueT>** arr, unsigned long arraySize) {
        for (int i = 0; i < arraySize; i++) {
            arr[i] = new LinkedList<valueT>();
        }
    }

    void deallocateArray(LinkedList<valueT>** arr, unsigned long arraySize) {
        for (int i = 0; i < arraySize; i++) {
            delete arr[i];
        }
        delete[] arr;
    }

    void initializeChunkMutexesArray(int newSize) {
        for (int i = 0; i < newSize; i++) {
            chunkMutexesArray[i] = new std::mutex;
        }
    }

    void deallocateChunkMutexesArray() {
        for (int i = 0; i < chunkMutexesArraySize; i++) {
            delete chunkMutexesArray[i];
        }
        delete[] chunkMutexesArray;
    }

    static unsigned long getHash(const std::string& key) {
        std::hash<std::string> hash;
        return hash(key);
    }

    unsigned long getIndex(const std::string& key) const {
        return ConcurrentHashTable::getHash(key) % size;
    }

    void unsafeSet(std::string key, valueT value) {
        unsigned long index = this->getIndex(key);
        auto* node = new HashtableNode<valueT>(key, value);

        if (array[index]->find(node->key) != nullptr) {
            array[index]->update(node);
        } else {
            array[index]->push(node);
            bucketsUsed++;
        }
    }


    void resize() {
        auto oldSize = size;
        auto oldArray = array;

        size = size * 2;
        array = new LinkedList<valueT>* [size];
        initializeArray(array, size);

        deallocateChunkMutexesArray();
        chunkMutexesArraySize = getChunkMtxArraySize();
        chunkMutexesArray = new std::mutex* [chunkMutexesArraySize];
        initializeChunkMutexesArray(chunkMutexesArraySize);
        for (int i = 0; i < oldSize; i++) {
            HashtableNode<valueT>* popped;
            while ((popped = oldArray[i]->pop())!= nullptr) {
                unsafeSet(popped->key, popped->value);
            }
        }
        deallocateArray(oldArray, oldSize);
    }

    void waitConcurrentWriters(unsigned int desiredValue) {
        while(concurrentWriters != desiredValue) {
        }
    }

    void waitResizing() {
        std::unique_lock<std::mutex> lck(resizingCvMtx);
        while (ongoingResize) resizingCv.wait(lck);
    }

    unsigned long getChunkMtxIndex(int arrayIndex) {
        return floor((double)arrayIndex / CHUNK_SIZE);
    }

    unsigned long getChunkMtxArraySize() {
        return ceil((double)size / CHUNK_SIZE);
    }

public:
    ConcurrentHashTable(unsigned long initialSize=512) {
        size = initialSize;
        array = new LinkedList<valueT>* [size];
        initializeArray(array, size);

        ongoingResize = false;
        concurrentWriters = 0;
        bucketsUsed = 0;

        chunkMutexesArraySize = getChunkMtxArraySize();
        chunkMutexesArray = new std::mutex* [chunkMutexesArraySize];
        initializeChunkMutexesArray(chunkMutexesArraySize);
    }
    ~ConcurrentHashTable() {
        deallocateChunkMutexesArray();
        deallocateArray(array, size);
    }

    // inserts the node key if key doesn't exist
    // updates the node's value if key exist
    void set(std::string key, valueT value) {
        if (ongoingResize) {
            waitResizing();
        }
        concurrentWriters++;


        unsigned long index = this->getIndex(key);
        auto* node = new HashtableNode<valueT>(key, value);

        // locking the chunk
        unsigned long chunkIndex = getChunkMtxIndex(index);
        chunkMutexesArray[chunkIndex]->lock();


        if (array[index]->find(node->key) != nullptr) {
            array[index]->update(node);
        } else {
            array[index]->push(node);
            bucketsUsed++;
        }

        chunkMutexesArray[chunkIndex]->unlock();

        // other thread operating on a table
        // is going to make a resize
        checkResizeMtx.lock();
        if (ongoingResize) {
            checkResizeMtx.unlock();
            concurrentWriters--;
            return;
        }

        // check if resize is needed
        if ((float)bucketsUsed / size > LOAD_FACTOR) {
            ongoingResize = true;
            checkResizeMtx.unlock();
            // wait until all other writers will finish their work
            waitConcurrentWriters(1);

            resize();

            ongoingResize = false;
            resizingCv.notify_all();
        } else {
            // resize is not needed
            checkResizeMtx.unlock();
        }

        concurrentWriters--;
    }

    // returns node's value if the node with key exists
    // returns defaultValue if node with key does not exist
    valueT get(std::string key, valueT defaultValue) {
        if (ongoingResize) {
            waitResizing();
        }
        concurrentWriters++;
        unsigned long index = this->getIndex(key);
        // locking the chunk
        unsigned long chunkIndex = getChunkMtxIndex(index);
        std::lock_guard<std::mutex> lck(*chunkMutexesArray[chunkIndex]);

        auto foundNode = array[index]->find(key);
        if (foundNode != nullptr) {
            return foundNode->value;
        }
        concurrentWriters--;
        return defaultValue;
    }

    // returns node's value if the node with key exists
    // set's provided defaultValue if the node with key doesn't exist and returns that value
    valueT getOrSetDefault(std::string key, valueT defaultValue) {
        if (ongoingResize) {
            waitResizing();
        }
        concurrentWriters++;
        unsigned long index = this->getIndex(key);
        // locking the chunk
        unsigned long chunkIndex = getChunkMtxIndex(index);
        std::lock_guard<std::mutex> lck(*chunkMutexesArray[chunkIndex]);

        auto foundNode = array[index]->find(key);
        if (foundNode != nullptr) {
            return foundNode->value;
        }
        unsafeSet(key, defaultValue);

        concurrentWriters--;
        return defaultValue;
    }

};
#endif