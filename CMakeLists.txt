cmake_minimum_required(VERSION 3.20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(MosaicEngine VERSION 25.0 LANGUAGES CXX)

file(GLOB_RECURSE MOSAIC_SOURCES "source/Mosaic/*.cpp")
file(GLOB_RECURSE MOSAIC_HEADERS "include/Mosaic/*.hpp" "include/Mosaic/Inline/*.inl")

add_library(Mosaic SHARED
    ${MOSAIC_SOURCES} 
    ${MOSAIC_HEADERS}
)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/vcpkg_installed/x64-linux/share")

target_compile_options(Mosaic PRIVATE
    -Wall 
)

target_include_directories(Mosaic PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(DIRECTORY include/Mosaic DESTINATION include)

install(TARGETS Mosaic 
    EXPORT MosaicTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(EXPORT MosaicTargets
    FILE MosaicTargets.cmake
    NAMESPACE Mosaic::
    DESTINATION lib/cmake/Mosaic
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MosaicConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Mosaic
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    DESTINATION lib/cmake/Mosaic
)

target_compile_definitions(Mosaic PRIVATE MOSAIC_BUILD_ENVIRONMENT)

if(UNIX)
    target_compile_definitions(Mosaic PUBLIC MOSAIC_PLATFORM_UNIX)
    if(LINUX)
        target_compile_definitions(Mosaic PUBLIC MOSAIC_PLATFORM_LINUX)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        target_compile_definitions(Mosaic PUBLIC MOSAIC_PLATFORM_MACOS)
    else()
        target_compile_definitions(Mosaic PUBLIC MOSAIC_PLATFORM_UNSUPPORTED)
        message(FATAL_ERROR "Unsupported platform for Mosaic: ${CMAKE_SYSTEM_NAME}")
    endif()
elseif(WIN32)
    target_compile_definitions(Mosaic PUBLIC MOSAIC_PLATFORM_WINDOWS)
else()
    target_compile_definitions(Mosaic PUBLIC MOSAIC_PLATFORM_UNSUPPORTED)
    message(FATAL_ERROR "Unsupported platform for Mosaic: ${CMAKE_SYSTEM_NAME}")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
    target_compile_definitions(Mosaic PUBLIC MOSAIC_ARCH_X86_64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    target_compile_definitions(Mosaic PUBLIC MOSAIC_ARCH_ARM64)
else()
    message(WARNING "Unsupported system architecture for Mosaic: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Mosaic PUBLIC MOSAIC_BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(Mosaic PUBLIC MOSAIC_BUILD_RELEASE)
else()
    target_compile_definitions(Mosaic PUBLIC MOSAIC_BUILD_UNSUPPORTED)
    message(WARNING "Unrecognised build type for Mosaic: ${CMAKE_BUILD_TYPE}")
endif()