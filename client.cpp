#include "interface/client.h"

void runClient() {
    std::cout << "Hello there!" << std::endl;
    std::cout << "Input port number: ";
    int port = inputInt(true);
    auto client = new IndexClient(SERVER_ADDRESS, SERVER_PORT, port);
    client->run();
}

int main() {
    runClient();
    return 0;
}
