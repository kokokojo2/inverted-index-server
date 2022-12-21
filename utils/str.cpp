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


std::string vectorToString(const std::vector<std::string>& vector, bool inLine) {
    std::string result = "[";
    if (!inLine) result += "\n";
    for (auto & i : vector)
        result += i + (inLine ? "," : ",\n");
    result +=  "]";
    return result;
}

std::string joinStrings(const std::vector<std::string>& vector, char delimiter) {
    std::string result;
    for (auto & i : vector)
        result += i + delimiter;
    return result;
}

