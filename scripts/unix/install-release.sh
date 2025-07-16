#!/bin/bash

cmake -B build -S . --preset release && cmake --build build && sudo cmake --install build