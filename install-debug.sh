#!/bin/bash

cmake -B build -S . -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ && cmake --build build && sudo cmake --install build