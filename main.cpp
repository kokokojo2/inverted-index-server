#include "collections/linked_list.h"
#include "collections/hash_table/hash_table.h"
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
    auto *node1 = new HashtableNode<std::string>("1", "Rand text");
    auto *node2 = new HashtableNode<std::string>("2", "Randd text");

    bool equals = *node1 == *node2;
    std::cout <<"equals" << equals << std::endl;

    std::cout << 1 << " exists: " << linkedList.find(node) << std::endl;
    node->key = "12";
    std::cout << 12 << " exists: " << linkedList.find(node) << std::endl;

    auto *node3 = new HashtableNode<std::string>("2", "Unreal text");
    linkedList.update(node3);

    linkedList.print();
    return 0;
}
