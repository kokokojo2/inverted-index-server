# Concurrent Inverted Index
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