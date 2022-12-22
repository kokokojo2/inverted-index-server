#include "server.h"

#include <utility>

IndexServer::IndexServer(std::string rootFolderPath, int indexInitSize, int numWorkers, int port) {
    this->index = new ConcurrentInvertedIndex(indexInitSize);
    this->workers = numWorkers;
    this->port = port;
    this->rootFolderPath = std::move(rootFolderPath);
}

void IndexServer::run() {
    std::cout << "Building index" << std::endl;

    buildIndex(this->rootFolderPath, this->index, this->workers);

    this->socket = new Socket(this->port);
    socket->bindAndListen();
    std::cout << "Server has started." << std::endl;
    while (true) {
        auto connection = socket->waitForConnection();
        std::thread handlerThread(&IndexServer::handleConnection, this, connection);
        handlerThread.detach();
    }
}

void IndexServer::handleConnection(Connection *connection) {
    while(true) {
        auto request = Request::fromConnection(connection);
        safe_print("Serving new request:");
        request->print();

        if (request->operation == OPERATION_EXIT) {
            connection->close();
            break;
        }

        Response* response;
        if (request->operation == OPERATION_GET) {
            auto docIdsSet = this->index->getDocIds(request->word);
            response = new Response(request->operation, docIdsSet->getItemsVector());
        }

        safe_print("Sending response:");
        response->print();
        connection->write(response->toText());
    }
}
