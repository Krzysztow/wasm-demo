#!/bin/bash

set -x

/emsdk/upstream/bin/clang++ --target=wasm32 --no-standard-libraries -Wl,--allow-undefined,--export-all,--no-entry -o add.wasm main.cpp
