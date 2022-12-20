#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../utils/io/output.h"
#define MAX_QUEUE_SIZE 25

#ifndef INVERTED_INDEX_SOCKET_H
#define INVERTED_INDEX_SOCKET_H
class Connection {
    int fd;
public:
    explicit Connection(int connectionDesc);
    void write(const std::string& message);
    std::string read();
};

class Socket {
protected:
    int socketDesc;
    int port;
public:
    explicit Socket(int port);
    ~Socket();
    void bindAndListen();
    Connection* connect(const std::string& serverIP, int serverPort);
    Connection* waitForConnection();
};


#endif