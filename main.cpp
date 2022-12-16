#include <string>
#include <thread>

#include "collections/linked_list.h"
#include "collections/hash_table.h"
#include "collections/set.h"
#include "collections/inverted_index.h"

std::mutex safePrintMtx;

void safe_print(const std::string& message, const std::string& key) {
    safePrintMtx.lock();
    std::cout << "Thread with key=" << key << ": " << message << std::endl;
    safePrintMtx.unlock();
}

auto *concurrentHashTable = new ConcurrentHashTable<std::string>(1);
void batchSetConcurrentHashTable(int size, int start) {
    for (int i = start; i < start + size; i++) {
        //concurrentHashTable->set(std::to_string(i), "Text for node with key = " + std::to_string(i));
    }
}

void testHashTable () {
    int testSize = 1000;

    std::thread t1(batchSetConcurrentHashTable, testSize, 0);
    std::thread t2(batchSetConcurrentHashTable, testSize, 900);
    std::thread t3(batchSetConcurrentHashTable, testSize, 2000);

    t1.join();
    t2.join();
    t3.join();

    int i = 0;
    while(i < 3000) {
        std::string val = concurrentHashTable->get(std::to_string(i), "None");
        std::cout << "key = " << std::to_string(i) << " value = " << val << std::endl;
        i++;

        // if (val == "None") break;
    }

}

auto *concurrentInvertedIndex = new ConcurrentInvertedIndex();

void batchInsertInvertedIndex(int size, int start, int procId) {
    for (int i = start; i < start + size; i++) {
        safe_print("inserting word key = " + std::to_string(i), std::to_string(procId));
        concurrentInvertedIndex->addToIndex(std::to_string(i), "some sample doc id" + std::to_string(i) + " ins by " + std::to_string(procId), procId);
    }
}

void testInvertedIndex (int testSize) {
    std::thread t1(batchInsertInvertedIndex, testSize, 0, 1);
    std::thread t2(batchInsertInvertedIndex, testSize, 0, 2);
    std::thread t3(batchInsertInvertedIndex, testSize, 0, 3);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "finished inserting" << std::endl;
    int i = 0;
    while(i < testSize) {
        auto val = concurrentInvertedIndex->getDocIds(std::to_string(i));
        auto idsVec = val->getItemsVector();
        std::cout << "key = " << std::to_string(i);
        std::cout << ", docIds = { ";
        for(auto & j : idsVec)
            std::cout << j << " ";
        std::cout << "}" << std::endl;

        i++;

        // if (val == "None") break;
    }

}



int main() {
    testInvertedIndex(60000);
    return 0;
}
