#include "interface/server.h"
#include "interface/client.h"
#include "routines/test.h"

void runServer() {
    std::cout << "Hello there!" << std::endl;
    std::cout << "Input number of threads that will build the index: ";
    int threads = inputInt(true);
    std::cout << "Input path to root folder (where the 'files_to_index' dir is stored): ";
    std::string rootPath = inputWord(false, true);
    auto server = new IndexServer(rootPath, 100000, threads, SERVER_PORT);
    server->run();
}

void runClient() {
    std::cout << "Hello there!" << std::endl;
    std::cout << "Input port number: ";
    int port = inputInt(true);
    auto client = new IndexClient(SERVER_ADDRESS, SERVER_PORT, port);
    client->run();
}

int main() {
    testBuildIndex(10);
    return 0;
}
