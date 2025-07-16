#!/bin/bash

cmake -B build -S . --preset debug && cmake --build build && sudo cmake --install build