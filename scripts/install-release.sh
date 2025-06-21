#!/bin/bash

cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ && cmake --build build && sudo cmake --install build