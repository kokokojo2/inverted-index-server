#include <vector>
#include "../collections/inverted_index.h"

#ifndef INVERTED_INDEX_INDEX_H
#define INVERTED_INDEX_INDEX_H
const std::vector<std::string> foldersToIndex = {
    "../files_to_index/test/neg",
    "../files_to_index/test/pos",
    "../files_to_index/train/neg",
    "../files_to_index/train/pos",
    "../files_to_index/train/unsup",
};
void indexFileChunk(ConcurrentInvertedIndex* index, const std::vector<std::string>& filesVec, int start, int end);
void buildIndex(ConcurrentInvertedIndex* index, int threadsNum);
std::vector<std::string> getFiles();
std::vector<std::string> getWords(const std::string& filePath);
std::vector<std::string> splitString(const std::string& string, char delimiter);
#endif