#!/bin/zsh

configure() {
    cmake -B build \
          -G Ninja \
          -DCMAKE_CXX_COMPILER=clang++ \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
          -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld \
          -S .
}

build() {
    cmake --build build
}

install() {
    sudo cmake --install build
}

# Determine execution limit based on input
limit="install"  # default: run all steps

if [[ "$1" == "--configure" ]]; then
    limit="configure"
elif [[ "$1" == "--build" ]]; then
    limit="build"
elif [[ "$1" == "--install" ]]; then
    limit="install"
elif [[ -n "$1" ]]; then
    echo "Unknown option: $1"
    echo "Usage: $0 [--configure | --build | --install]"
    exit 1
fi

# Control flow
configure
[[ "$limit" == "configure" ]] && exit 0

build
[[ "$limit" == "build" ]] && exit 0

install
