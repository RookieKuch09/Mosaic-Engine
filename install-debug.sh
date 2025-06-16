#!/bin/bash

cmake -B build -S . -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build && sudo cmake --install build