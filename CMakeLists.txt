cmake_minimum_required(VERSION 3.20)

# Set output directories for the build
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include the CMake module path
include(CMakePackageConfigHelpers)

# Create the project
project(MosaicEngine VERSION 25.0 LANGUAGES CXX)

# Locate all source and header files
file(GLOB_RECURSE MOSAIC_SOURCES "source/Mosaic/*.cpp")
file(GLOB_RECURSE MOSAIC_HEADERS "include/Mosaic/*.hpp" "include/Mosaic/Inline/*.inl")

find_package(magic_enum CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS type_index)

# Create the library target
add_library(Mosaic SHARED
    ${MOSAIC_SOURCES} 
    ${MOSAIC_HEADERS}
)

# Set the target compile options
target_compile_options(Mosaic PRIVATE
    -Wall 
)

# Link the target with the required libraries
target_link_libraries(Mosaic PUBLIC
    Boost::type_index
    magic_enum::magic_enum
)

# Set the target include directories
target_include_directories(Mosaic PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(Mosaic PRIVATE MOSAIC_BUILD_ENVIRONMENT)

# Set the platform specific definitions
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

# Set the architecture specific definitions
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
    target_compile_definitions(Mosaic PUBLIC MOSAIC_ARCH_X86_64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    target_compile_definitions(Mosaic PUBLIC MOSAIC_ARCH_ARM64)
else()
    message(WARNING "Unsupported system architecture for Mosaic: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Set the build type specific definitions
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Mosaic PUBLIC MOSAIC_BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(Mosaic PUBLIC MOSAIC_BUILD_RELEASE)
else()
    target_compile_definitions(Mosaic PUBLIC MOSAIC_BUILD_UNSUPPORTED)
    message(WARNING "Unrecognised build type for Mosaic: ${CMAKE_BUILD_TYPE}")
endif()

# Create the config file for the package
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MosaicConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Mosaic
)

# Generate the version file for the package
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Prepare to clean old include directories
set(MOSAIC_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/Mosaic")

# Clear the old header files to avoid stale code
install(CODE "
    message(STATUS \"Removing old headers from ${MOSAIC_INSTALL_INCLUDE_DIR}\")
    file(REMOVE_RECURSE \"${MOSAIC_INSTALL_INCLUDE_DIR}\")
")

# Install all headers and inline files
install(DIRECTORY include/Mosaic DESTINATION include)

# Install the library and create the export set
install(TARGETS Mosaic 
    EXPORT MosaicTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Export the target for use in other projects
install(EXPORT MosaicTargets
    FILE MosaicTargets.cmake
    NAMESPACE Mosaic::
    DESTINATION lib/cmake/Mosaic
)

# Install the config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    DESTINATION lib/cmake/Mosaic
)