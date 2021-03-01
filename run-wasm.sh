#!/bin/bash

set -x

#emcmake cmake -DCMAKE_BUILD_TYPE=RELWITHDEBINFO ..
emcmake cmake -DFOR_WASM=On -DCMAKE_CXX_FLAGS_DEBUG="-g4" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j 8

export WASM_BUILD_SRCD=$(pwd)/source/wasm/
pushd ../source/wasm-browser/
npm install --dev
cp $WASM_BUILD_SRCD/wasmDemo.* ./
npm test
