#include <string>
#include <utility>
#include <regex>

#include "../utils/str.h"
#include "../network/socket.h"

#ifndef INVERTED_INDEX_PROTOCOL_H
#define INVERTED_INDEX_PROTOCOL_H

#define OPERATION_GET 1
#define OPERATION_EXIT 2

class Request {
    // OPERATION:arg1, arg2, ..., argn  (optional - depends on operation logic)
    std::string rawText;

    void parse();

public:
    std::string errorMessage;
    std::string word;
    int operation;
    bool valid;

    Request(int operation);
    Request(std::string raw);
    static Request* fromConnection(Connection* connection);
    void print() const;
    static std::string getOperationRepr(int o);
    static std::string getStatusRepr(bool s);
    static bool operationValid(int rawOperation);
    std::string toText() const;
};

class Response {
    // OPERATION:arg1, arg2, ... , argn
    std::string rawText;
    void parse();
public:
    std::string errorMessage;
    int requestOperation;
    bool valid;
    std::vector<std::string> docIds;

    Response(int operation, std::vector<std::string> docIds);
    Response(std::string raw);
    static Response* fromConnection(Connection* connection);
    void print() const;
    std::string toText() const;


};


#endif