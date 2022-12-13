#include <string>
#include <thread>

#include "collections/linked_list.h"
#include "collections/hash_table.h"

std::mutex safePrintMtx;

void safe_print(const std::string& message, const std::string& key) {
    safePrintMtx.lock();
    std::cout << "Thread with key=" << key << ": " << message << std::endl;
    safePrintMtx.unlock();
}

auto *hashTable = new ConcurrentHashTable<std::string>(1);

void testHashTable(int size, int start) {
    for (int i = start; i < start + size; i++) {
        hashTable->set(std::to_string(i), "Text for node with key = " + std::to_string(i));
    }
}

int main() {
    int testSize = 1000;

    std::thread t1(testHashTable, testSize, 0);
    std::thread t2(testHashTable, testSize, 900);
    std::thread t3(testHashTable, testSize, 2000);

    t1.join();
    t2.join();
    t3.join();

    int i = 0;
    while(i < 3000) {
        std::string val = hashTable->get(std::to_string(i), "None");
        std::cout << "key = " << std::to_string(i) << " value = " << val << std::endl;
        i++;

        // if (val == "None") break;
    }



    return 0;
}
