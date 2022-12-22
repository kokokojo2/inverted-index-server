#include <vector>
#include <string>
#include <iostream>

#include "../str.h"
#ifndef INVERTED_INDEX_INPUT_H
#define INVERTED_INDEX_INPUT_H
int inputOption(const std::vector<int>& options, bool loop);
std::string inputWord(bool alphanum, bool loop);
int inputInt(bool loop);
#endif
