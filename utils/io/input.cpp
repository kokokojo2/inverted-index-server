#include "input.h"

int inputOption(const std::vector<int>& options, bool loop) {
    std::string rawInput;
    while (true) {
        std::cin >> rawInput;
        for (auto & option : options)
            if (rawInput == std::to_string(option)) return std::stoi(rawInput);
        std::cout << "Invalid option, try again." << std::endl;
        if (!loop) break;
    }
    return -1;
}

int inputInt(bool loop) {
    std::string rawInput;
    while (true) {
        std::cin >> rawInput;
        if (isNumber(rawInput)) return std::stoi(rawInput);
        std::cout << "The input value should be integer, try again." << std::endl;
        if (!loop) break;
    }
    return -1;
}

std::string inputWord(bool alphanum, bool loop) {
    std::string rawInput;
    while (loop) {
        std::cin >> rawInput;
        if (alphanum && checkStringAlNum(rawInput)) {
            return rawInput;
        }
        if (!alphanum) return rawInput;
        std::cout << "Invalid string entered, try again." << std::endl;
    }
}

