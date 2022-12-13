#include <iostream>
#include <string>
#include <cmath>

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "node.h"
#include "linked_list.h"

#ifndef INVERTED_INDEX_HASH_TABLE_H
#define INVERTED_INDEX_HASH_TABLE_H

std::mutex safePrintMtx;

void safe_print(const std::string& message, const std::string& key) {
    safePrintMtx.lock();
    std::cout << "Thread with key=" << key << ": " << message << std::endl;
    safePrintMtx.unlock();
}

template <class valueT> class ConcurrentHashTable {
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

    void initializeChunkMutexesArray(int newSize) {
        for (int i = 0; i < newSize; i++) {
            chunkMutexesArray[i] = new std::mutex;
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

        chunkMutexesArraySize = getChunkMtxArraySize();
        chunkMutexesArray = new std::mutex* [chunkMutexesArraySize];
        initializeChunkMutexesArray(chunkMutexesArraySize);

        for (int i = 0; i < oldSize; i++) {
            HashtableNode<std::string>* popped;
            while ((popped = oldArray[i]->pop())!= nullptr) {
                unsafeSet(popped->key, popped->value);
            }
        }
        // TODO: delete all elements first
        delete [] oldArray;
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

        chunkMutexesArraySize = getChunkMtxArraySize();
        chunkMutexesArray = new std::mutex* [chunkMutexesArraySize];
        initializeChunkMutexesArray(chunkMutexesArraySize);
    }

    // inserts the node key if key doesn't exist
    // updates the node's value if key exist
    void set(std::string key, valueT value) {
        safe_print("adding node: "+ key + ":" + value, key);

        if (ongoingResize) {
            safe_print("waiting for ongoingResize to finish", key);
            safe_print(" concurrentWriters=" + std::to_string(concurrentWriters), key);
            waitResizing();
        }
        concurrentWriters++;
        safe_print("Entering critical section and adding node", key);
        safe_print(" concurrentWriters=" + std::to_string(concurrentWriters), key);


        unsigned long index = this->getIndex(key);
        auto* node = new HashtableNode<valueT>(key, value);
        safe_print("here1", key);

        // locking the chunk
        unsigned long chunkIndex = getChunkMtxIndex(index);
        safe_print("index = " + std::to_string(index) + ", chunkIndex = " + std::to_string(chunkIndex) + ", mtxArrSize = " + std::to_string(chunkMutexesArraySize), key);
        chunkMutexesArray[chunkIndex]->lock();


        if (array[index]->find(node->key) != nullptr) {
            array[index]->update(node);
        } else {
            array[index]->push(node);
            bucketsUsed++;
        }

        chunkMutexesArray[chunkIndex]->unlock();

        safe_print("checking resize", key);
        // other thread operating on a table
        // is going to make a resize
        checkResizeMtx.lock();
        if (ongoingResize) {
            safe_print("abort checking resize", key);
            checkResizeMtx.unlock();
            concurrentWriters--;
            return;
        }

        // check if resize is needed
        if ((float)bucketsUsed / size > LOAD_FACTOR) {
            safe_print("ongoing resize is needed, setting ongoingResize = true", key);
            ongoingResize = true;
            checkResizeMtx.unlock();
            // wait until all other writers will finish their work
            safe_print("waiting concurrent writers to leave", key);
            waitConcurrentWriters(1);

            safe_print("starting resize", key);
            resize();
            safe_print("finishing resize", key);

            ongoingResize = false;
            safe_print("notifying cv", key);
            resizingCv.notify_all();
        } else {
            // resize is not needed
            safe_print("resize not needed exiting", key);
            checkResizeMtx.unlock();
        }

        concurrentWriters--;
    }

    // returns node's value if the node with key exists
    // returns nullptr if node with key does not exist
    valueT get(std::string key, valueT defaultValue) {
        if (ongoingResize) {
            waitResizing();
        }

        unsigned long index = this->getIndex(key);

        // locking the chunk
        unsigned long chunkIndex = getChunkMtxIndex(index);
        std::lock_guard<std::mutex> lck(*chunkMutexesArray[chunkIndex]);

        auto foundNode = array[index]->find(key);
        if (foundNode != nullptr) {
            return foundNode->value;
        }
        return defaultValue;
    }

};
#endif