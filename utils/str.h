#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>


#ifndef INVERTED_INDEX_STR_H
#define INVERTED_INDEX_STR_H
std::vector<std::string> splitString(const std::string& string, char delimiter);
std::string vectorToString(const std::vector<std::string>& vector, bool inLine);
std::string joinStrings(const std::vector<std::string>& vector, char delimiter);
bool checkStringAlNum(const std::string& string);
std::string cleanString(std::string str);
bool isNumber(const std::string& s);
bool isNotAlNum(char c);
#endif