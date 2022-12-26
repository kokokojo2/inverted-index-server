#include "interface/server.h"
#include "utils/io/input.h"

void runServer() {
    std::cout << "Hello there!" << std::endl;
    std::cout << "Input number of threads that will build the index: ";
    int threads = inputInt(true);
    std::cout << "Input path to root folder (where the 'files_to_index' dir is stored): ";
    std::string rootPath = inputWord(false, true);
    auto server = new IndexServer(rootPath, 100000, threads, SERVER_PORT);
    server->run();
}

int main() {
    runServer();
    return 0;
}
