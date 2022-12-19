#include "str.h"


std::vector<std::string> splitString(const std::string& string, char delimiter) {
    std::vector<std::string> strings;
    std::istringstream f(string);
    std::string s;
    while (getline(f, s, delimiter)) {
        strings.push_back(s);
    }
    return strings;
}

bool isNotAlNum(char c) {
    return isalnum(c) == 0 && c != ' ';
}

std::string cleanString(std::string str) {
    str.erase(remove_if(str.begin(), str.end(), isNotAlNum), str.end());
    return str;
}
