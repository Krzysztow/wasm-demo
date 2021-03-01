#!/bin/bash

set -x

#emcmake cmake -DCMAKE_BUILD_TYPE=RELWITHDEBINFO ..
emcmake cmake -DFOR_WASM=On -DCMAKE_CXX_FLAGS_DEBUG="-g4" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j 8
