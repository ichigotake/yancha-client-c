# yancha-client-core

\[WIP\]: [yancha](https://github.com/uzulla/yancha) client common resources writen by C.

## Build and run unit test

``` sh
# depends Jansson
$ git clone https://github.com/akheron/jansson && cd jansson
$ mkdir build && cd build && cmake .. && make && make install

# run unit test
$ mkdir build && cd build
$ cmake .. && make && ./yancha_client_c
# or on the CLion, run to "core"
```

## Develop Environment

- CLion - CL-140.1740.3
- cmake - 3.0.0
- clang - Apple LLVM version 6.0
