#include "../collections/inverted_index.h"
#include "../collections/hash_table.h"
#include "../routines/index.h"
#include "../network/socket.h"
#include "../network/constant.h"

#ifndef INVERTED_INDEX_TEST_H
#define INVERTED_INDEX_TEST_H
void testHashTable ();
void testInvertedIndex (int testSize);
void testBuildIndex();
void testSocketServer();
void testSocketClient();
#endif