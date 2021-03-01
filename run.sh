#!/bin/bash

set -x

/emsdk/upstream/bin/clang++ --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o add.wasm main.cpp
