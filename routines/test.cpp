#include "test.h"

void batchSetConcurrentHashTable(ConcurrentHashTable<std::string>* concurrentHashTable, int size, int start) {
    for (int i = start; i < start + size; i++) {
        concurrentHashTable->set(std::to_string(i), "Text for node with key = " + std::to_string(i));
    }
}

void testHashTable () {
    auto *concurrentHashTable = new ConcurrentHashTable<std::string>(1);
    int testSize = 1000;

    std::thread t1(batchSetConcurrentHashTable, concurrentHashTable, testSize, 0);
    std::thread t2(batchSetConcurrentHashTable, concurrentHashTable, testSize, 900);
    std::thread t3(batchSetConcurrentHashTable, concurrentHashTable, testSize, 2000);

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


void batchInsertInvertedIndex(ConcurrentInvertedIndex* index, int size, int start, int procId) {
    for (int i = start; i < start + size; i++) {
        safe_print("inserting word key = " + std::to_string(i));
        index->addToIndex(std::to_string(i), "some sample doc id" + std::to_string(i) + " ins by " + std::to_string(procId));
    }
}

void testInvertedIndex (int testSize) {
    auto *concurrentInvertedIndex = new ConcurrentInvertedIndex(100000);

    std::thread t1(batchInsertInvertedIndex, concurrentInvertedIndex, testSize, 0, 1);
    std::thread t2(batchInsertInvertedIndex, concurrentInvertedIndex, testSize, 0, 2);
    std::thread t3(batchInsertInvertedIndex, concurrentInvertedIndex, testSize, 0, 3);

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

void testBuildIndex() {
    auto *concurrentInvertedIndex = new ConcurrentInvertedIndex(100000);
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    buildIndex(concurrentInvertedIndex, 1);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

}


void testSocketServer() {
    auto socket = Socket(SERVER_PORT);
    socket.bindAndListen();
    auto connection = socket.waitForConnection();
    std::cout << connection->read() << std::endl;
    connection->write("Dummy response.");
}

void testSocketClient() {
    auto socket = Socket(2022);
    auto connection = socket.connect(SERVER_ADDRESS, SERVER_PORT);
    connection->write("Dummy request.");
    std::cout << connection->read() << std::endl;
}

void testProtocolStructures() {
    std::string rawRequest = "1:dummy_word";
    auto request = new Request(rawRequest);
    std::cout << "request from text" << std::endl;
    request->print();
    std::cout << "request to text" << std::endl;
    std::cout << request->toText() << std::endl;

    std::vector<std::string> docIds = {"doc1", "doc2", "doc5"};
    auto response = new Response(1, docIds);
    std::cout << "response to text" << std::endl;
    std::cout << response->toText() << std::endl;
    std::cout << "response from text" << std::endl;
    auto res = Response(response->toText());
    response->print();
}
