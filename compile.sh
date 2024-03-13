#!/bin/bash

./compile-grammar.sh

mkdir -p build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
cmake --build .
