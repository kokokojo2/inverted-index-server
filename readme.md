# Concurrent Inverted Index
Inverted inxed is a data structure that is widely used for search optimization. This project is just a simple implementation of inverted index written in C++ that was made as a part of coursework of "concurrency & parallelism" university subject.
## Project Overview
This repo contains implementations of concurrent inverted index along with all required data structures and interface for interacting with it build on top of network sockets.
Brief modules overview:
- `collections` - inverted index and related data structures
- `files_to_index` - text files that are used to build the index
- `interface` - client and server implementations
- `network` - network layer code based on sockets, simple text-based communication protocol
- `routines` - multithreaded algorithm of file indexing
- `utils` - utility functions :)
## Try it out
In order to try the software locally you need to have `gcc` and `cmake` packages
installed.

Clone the repo
```
git clone https://github.com/kokokojo2/inverted-index-server
```

Navigate inside the project
```
cd inverted-index-server
```

Create make build files
```
cmake .
```

Build executables
```
make
```

Run an inverted-index server `server` program that builds the index concurrently
and starts a simple network server
```
./server
```


Run a `client`, that can talk with `server` to search words
```
./client
```
