#include <iostream>
#include <fstream>
#include <dirent.h>

#include "index.h"
#include "../utils/str.h"

void buildIndex(ConcurrentInvertedIndex* index, int threadsNum) {
    auto files = getFiles();
    if (threadsNum > files.size()) std::cout << "Thread number is too big for given set of files." << std::endl;
    unsigned long end, start = 0, chunkSize = files.size() / threadsNum;

    // TODO: use ThreadPool (needs to be implemented)
    std::thread workers[threadsNum];
    for (int i = 0; i < threadsNum; i++) {
        end = i != threadsNum - 1 ? start + chunkSize : files.size();
        workers[i] = std::thread(indexFileChunk, index, files, start, end);
        start += chunkSize;
    }

    for (int i = 0; i < threadsNum; i++) {
        workers[i].join();
    }

}

std::vector<std::string> getFiles(){
    std::vector<std::string> files;
    for (const auto & folderPath : foldersToIndex) {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(folderPath.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                if (ent->d_type == DT_REG) {
                    files.push_back(folderPath + "/" + std::string(ent->d_name));
                }
            }
        }
    }

    return files;
}

void indexFileChunk(ConcurrentInvertedIndex* index, const std::vector<std::string>& filesVec, int start, int end) {
    for (int i = start; i < end; i++) {
        auto words = getWords(filesVec[i]);
        for (auto & word : words) {
            index->addToIndex(word, filesVec[i]);
        }
    }
}

std::vector<std::string> getWords (const std::string& filePath) {
    std::string line, cleanedLine;
    std::vector<std::string> words;

    std::ifstream file(filePath, std::ios_base::in);
    while (file) {
        std::getline(file, line);
        cleanedLine = cleanString(line);
        auto tempWords = splitString(cleanedLine, ' ');
        for (auto & tempWord : tempWords) {
            words.push_back(tempWord);
        }
    }
    file.close();

    return words;
}

