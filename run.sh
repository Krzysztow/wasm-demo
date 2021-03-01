#!/bin/bash 

set -x

emcc -g4 -s "EXPORTED_FUNCTIONS=['_main','_unsafe_memcpy','_add','_malloc']" ./main.cpp -o add.html
