cmake_minimum_required(VERSION 3.23)
project(invertedIndex)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        tb_build_index build_index.cpp
        collections/linked_list.h
        collections/node.h
        collections/hash_table.h
        collections/set.h
        collections/inverted_index.h
        utils/io/output.cpp
        utils/io/output.h
        routines/index.h
        routines/index.cpp
        utils/str.cpp
        utils/str.h
        routines/test.cpp
        routines/test.h
        interface/server.cpp
        interface/server.h
        network/socket.h
        network/socket.cpp
        network/constant.h
        network/protocol.cpp
        network/protocol.h
        interface/client.cpp
        interface/client.h
        utils/io/input.cpp
        utils/io/input.h
)

add_executable(
        server server.cpp
        collections/linked_list.h
        collections/node.h
        collections/hash_table.h
        collections/set.h
        collections/inverted_index.h
        utils/io/output.cpp
        utils/io/output.h
        routines/index.h
        routines/index.cpp
        utils/str.cpp
        utils/str.h
        routines/test.cpp
        routines/test.h
        interface/server.cpp
        interface/server.h
        network/socket.h
        network/socket.cpp
        network/constant.h
        network/protocol.cpp
        network/protocol.h
        interface/client.cpp
        interface/client.h
        utils/io/input.cpp
        utils/io/input.h
)

add_executable(
        client client.cpp
        collections/linked_list.h
        collections/node.h
        collections/hash_table.h
        collections/set.h
        collections/inverted_index.h
        utils/io/output.cpp
        utils/io/output.h
        routines/index.h
        routines/index.cpp
        utils/str.cpp
        utils/str.h
        routines/test.cpp
        routines/test.h
        interface/server.cpp
        interface/server.h
        network/socket.h
        network/socket.cpp
        network/constant.h
        network/protocol.cpp
        network/protocol.h
        interface/client.cpp
        interface/client.h
        utils/io/input.cpp
        utils/io/input.h
)