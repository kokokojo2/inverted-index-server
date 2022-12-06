#include "collections/linked_list.h"
#include "collections/hash_table.h"
#include <string>


int main() {
    LinkedList<std::string> linkedList = LinkedList<std::string>();

    for (int i = 0; i < 5; i++) {
        std::string randText = "Rand text";
        auto *node = new HashtableNode<std::string>(std::to_string(i), randText);
        linkedList.push(node);
    }
    linkedList.print();
    auto *node = new HashtableNode<std::string>("1", "Rand text");
    std::cout << 1 << " exists: " << linkedList.find("1") << std::endl;
    node->key = "12";
    std::cout << 12 << " exists: " << linkedList.find("12") << std::endl;

    auto *node3 = new HashtableNode<std::string>("2", "Unreal text");
    linkedList.update(node3);

    linkedList.print();

    HashtableNode<std::string>* popped;
    while ((popped = linkedList.pop())!= nullptr) {
        std::cout << "Popped value with key = " << popped->key << std::endl;
    }
    std::cout << "all values has been popped" << std::endl;

    auto *hashTable = new HashTable<std::string>(1);
    for (int i = 0; i < 100; i++) {
        hashTable->set(std::to_string(i), "Some value for weird node.");

    }

    std::cout << "getting value for key 23 - " << hashTable->get("23", "None") << std::endl;
    std::cout << "getting value for key weird node -  " << hashTable->get("weird node", "None") << std::endl;

    return 0;
}
