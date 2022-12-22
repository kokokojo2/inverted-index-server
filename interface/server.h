#include <string>
#include <thread>

#include "../network/socket.h"
#include "../network/protocol.h"
#include "../network/constant.h"
#include "../routines/index.h"

#ifndef INVERTED_INDEX_SERVER_H
#define INVERTED_INDEX_SERVER_H
class IndexServer {
    ConcurrentInvertedIndex *index;
    int workers;
    std::string rootFolderPath;

    std::string host;
    int port;
    Socket* socket;

    void handleConnection(Connection* connection);
public:
    IndexServer(std::string rootFolderPath, int indexInitSize, int numWorkers, int port);
    void run ();
};

#endif