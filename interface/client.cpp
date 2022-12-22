#include "client.h"

IndexClient::IndexClient(const std::string& host, int port) {
    this->serverHost = host;
    this->port = port;
}

void IndexClient::run() {
    std::cout << "Hello there!" << std::endl;
    this->socket = new Socket(this->port);
    auto connection = socket->connect(this->serverHost, this->port);

    while (true) {
        int option;
        while (true) {
            std::cout << "Choose one of the following options:" << std::endl;
            std::cout << "[1][Get document ids for a word]" << std::endl;
            std::cout << "[2][Exit]" << std::endl;
            option = inputOption(std::vector<int> {1, 2}, false);
            if (option != -1) break;
        }
        if (option == 2) {
            auto request = new Request(OPERATION_EXIT);
            connection->write(request->toText());
            connection->close();
            return;
        }
        if (option == 1) {
            std::cout << "Input word (no spaces allowed): ";
            std::string word = inputWord(true, true);

            auto request = new Request(OPERATION_GET);
            request->word = word;

            std::cout << "Sending request to a server..." << std::endl;
            connection->write(request->toText());

            std::cout << "Sent. Waiting for response..." << std::endl;
            auto response = Response::fromConnection(connection);
            std::cout << "Response from the server:" << std::endl;
            response->print();
            std::cout << "DocIds:" << std::endl;
            std::cout << vectorToString(response->docIds, false) << std::endl;
        }
    }
}
