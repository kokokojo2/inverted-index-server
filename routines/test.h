#include "../collections/inverted_index.h"
#include "../collections/hash_table.h"
#include "../routines/index.h"
#include "../network/socket.h"
#include "../network/constant.h"
#include "../network/protocol.h"

#ifndef INVERTED_INDEX_TEST_H
#define INVERTED_INDEX_TEST_H
void testHashTable ();
void testInvertedIndex (int testSize);
void testBuildIndex(int maxThreads);
void testSocketServer();
void testSocketClient();
void testProtocolStructures();
#endif