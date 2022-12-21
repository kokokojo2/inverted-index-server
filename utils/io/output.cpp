#include "output.h"

std::mutex safePrintMtx;
void safe_print(const std::string& message) {
    safePrintMtx.lock();
    std::cout << "Thread with id=" <<  std::this_thread::get_id() << ": " << message << std::endl;
    safePrintMtx.unlock();
}

void print_vector(std::vector<std::string> vector) {
    std::cout << "[";
    for (auto & i : vector)
        std::cout  << i << ", ";
    std::cout << "]" << std::endl;
}

