@echo off

cmake -B build -S . --preset release
if %errorlevel% neq 0 (
    echo CMake configure failed.
    exit /b %errorlevel%
)

cmake --build build
if %errorlevel% neq 0 (
    echo CMake build failed.
    exit /b %errorlevel%
)

cmake --install build
if %errorlevel% neq 0 (
    echo CMake install failed.
    exit /b %errorlevel%
)
