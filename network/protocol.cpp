#include "protocol.h"

#include <utility>


void Request::parse() {
    std::regex pattern("^([0-9]):(.*)$");
    std::smatch matches;
    if(!std::regex_search(this->rawText, matches, pattern)) {
        this->valid = false;
        this->errorMessage = "Syntax error found in request body.";
    }

    // parsing operation
    int parsedOperation = std::stoi(matches[1].str());
    if(!operationValid(parsedOperation)) {
        this->valid = false;
        this->errorMessage = "Invalid operation found in request body.";
    }
    this->operation = parsedOperation;

    // parsing params for specific operations
    if (this->operation == OPERATION_GET) {
        // get one word from arguments
        this->word = matches[2].str();
    }

    this->valid = true;
}

bool Request::operationValid(int rawOperation) {
    return rawOperation == OPERATION_GET || rawOperation == OPERATION_EXIT;
}

Request::Request(std::string raw)  {
    this->rawText = std::move(raw);
    parse();
}

Request::Request(int operation) {
    this->operation = operation;
    this->valid = true;
};

Request *Request::fromConnection(Connection *connection) {
    auto rawRequest = connection->read();
    return new Request(rawRequest);
}

void Request::print() const  {
    safe_print("<Request status=" + getStatusRepr(valid) + this->errorMessage + " operation=" +
    getOperationRepr(this->operation) + (!this->word.empty() ? " word="+this->word : "") + ">");
}

std::string Request::getOperationRepr(int o) {
    if (o == OPERATION_GET) return "get";
    if (o == OPERATION_EXIT) return "exit";
    return "unknown";
}

std::string Request::getStatusRepr(bool s) {
    if (s) return "valid";
    return "invalid";
}

std::string Request::toText() const {
    return std::to_string(this->operation) + ":" + this->word;
}

void Response::parse() {
    std::regex pattern("^([0-9]):(.*)$");
    std::smatch matches;
    if(!std::regex_search(this->rawText, matches, pattern)) {
        this->valid = false;
        this->errorMessage = "Syntax error found in response body.";
    }

    // parsing operation
    int parsedOperation = std::stoi(matches[1].str());
    if(!Request::operationValid(parsedOperation)) {
        this->valid = false;
        this->errorMessage = "Invalid operation found in response body.";
    }
    this->requestOperation = parsedOperation;

    // parsing response for specific operations
    if (this->requestOperation == OPERATION_GET) {
        // get vector of docIds from arguments
        this->docIds = splitString(matches[2].str(), ',');
    }
    this->valid = true;
}

Response::Response(std::string raw) {
    this->rawText = std::move(raw);
    parse();
}

Response *Response::fromConnection(Connection *connection) {
    auto rawResponse = connection->read();
    return new Response(rawResponse);
}

void Response::print() const {
    safe_print("<Response status=" + Request::getStatusRepr(valid) + this->errorMessage + " operation=" +
        Request::getOperationRepr(this->requestOperation) + " docIds=" + vectorToString(this->docIds, true) + ">");
}

std::string Response::toText() const {
    return std::to_string(this->requestOperation) + ":" + joinStrings(this->docIds, ',');
}

Response::Response(int operation, std::vector<std::string> docIds) {
    this->requestOperation = operation;
    this->docIds = std::move(docIds);
    this->valid = true;
}
