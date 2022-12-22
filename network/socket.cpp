#include "socket.h"

Socket::Socket(int port) {
    this->port = port;

    this->socketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketDesc == -1) {
        safe_print("Some error occur while initializing.");
        exit(EXIT_FAILURE);
    }
    safe_print("A TCP socket was created.");
}

Socket::~Socket() {
    if(close(this->socketDesc) == -1) {
        safe_print("Some error occur while closing socket.");
        exit(EXIT_FAILURE);
    }
}

void Socket::bindAndListen() {
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(this->port);

    if (bind(this->socketDesc, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        safe_print("Some error occur while binding socket.");
        exit(EXIT_FAILURE);
    }

    if(listen(this->socketDesc, MAX_QUEUE_SIZE) != 0) {
        safe_print("Failed to listen...");
        exit(EXIT_FAILURE);
    }
    safe_print("The socket was bound and ready for communication.");
}

Connection* Socket::waitForConnection() {
    struct sockaddr_in clientAddr{};
    socklen_t clientAddressLen = sizeof(clientAddr);

    int connectionDesc = accept(
            this->socketDesc,
            (struct sockaddr*)&clientAddr,
            &clientAddressLen
    );
    if(connectionDesc == -1) {
        safe_print("Some error occur while accepting connection.");
        exit(EXIT_FAILURE);
    }
    return new Connection(connectionDesc);
}

Connection* Socket::connect(const std::string& serverIP, int serverPort) {
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    if (inet_aton(serverIP.c_str() , &serverAddress.sin_addr) == 0) {
        exit(EXIT_FAILURE);
    }

    if (::connect(
            this->socketDesc,
            (struct sockaddr *)&serverAddress,
            sizeof(serverAddress)
    ) == -1) {
        safe_print("Some error occur while connecting to the server.");
        exit(EXIT_FAILURE);
    }

    return new Connection(this->socketDesc);
}

Connection::Connection(int connectionDesc) {
    this->fd = connectionDesc;
}

void Connection::write(const std::string &message) {
    if(::write(this->fd, message.c_str(), message.size()) == -1) {
        safe_print("Some error occur while writing the message.");
        exit(EXIT_FAILURE);
    }
    //safe_print("Message was sent.");
}

std::string Connection::read() {
    // fixme: use generic reading algorithm
    int bufferSize = 64000;
    char receivedMessage [bufferSize];

    int messageSize = ::read(
            this->fd,
            receivedMessage,
            bufferSize
    );

    if (messageSize == -1) {
        safe_print("Some error occur while reading the message.");
        exit(EXIT_FAILURE);
    }
    receivedMessage[messageSize] = '\0';

    return std::string(receivedMessage);
}

void Connection::close() {
    ::close(this->fd);
}
