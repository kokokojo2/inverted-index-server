#include <string>

#include "../network/protocol.h"
#include "../network/socket.h"
#include "../network/constant.h"
#include "../utils/io/input.h"

#ifndef INVERTED_INDEX_CLIENT_H
#define INVERTED_INDEX_CLIENT_H
class IndexClient {
    std::string serverHost;
    int port;
    int clientPort;
    Socket* socket;
public:
    IndexClient(const std::string& host, int port, int clientPort);
    void run ();
};

#endif