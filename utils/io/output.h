#include <mutex>
#include <thread>
#include <iostream>
#include <vector>

#ifndef INVERTED_INDEX_OUTPUT_H
#define INVERTED_INDEX_OUTPUT_H
void safe_print(const std::string& message);
void print_vector(std::vector<std::string> vector);
#endif
