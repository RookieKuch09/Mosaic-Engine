# Set CMake minimum version
cmake_minimum_required(VERSION 3.20)

# Create project
project(MosaicEngine LANGUAGES CXX)

# Configure C++ compiler
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Locate all sources
file(GLOB_RECURSE SOURCES "source/**/*.cpp")

# Create the library
add_library(Mosaic ${SOURCES})

# Handle Apple stuff
if(APPLE)
    find_library(MVK_LIB MoltenVK)
    target_link_libraries(Mosaic PRIVATE ${MVK_LIB})
else()
    find_package(Vulkan REQUIRED)
    target_link_libraries(Mosaic PRIVATE Vulkan::Vulkan)
endif()

# Find all platform-independent required packages
find_package(glfw3 REQUIRED)

# Link library against required packages
target_link_libraries(Mosaic PRIVATE glfw)

# Clear install directory to avoid stale files
install(CODE "
    message(STATUS \"Clearing old Mosaic install directory...\")
    file(REMOVE_RECURSE \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/Mosaic\")
")

# Inclure required modules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Set the public include directory
target_include_directories(Mosaic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/include>
)

# Install the library
install(TARGETS Mosaic
    EXPORT MosaicTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install all header files
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/include)
install(DIRECTORY interface/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic)

# Export the CMake config for consumers
install(EXPORT MosaicTargets
    FILE MosaicTargets.cmake
    NAMESPACE Mosaic::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mosaic
)

# Write package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    VERSION 0.0.1
    COMPATIBILITY AnyNewerVersion
)

# Configure package version file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MosaicConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mosaic
)

# Install config files for package
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mosaic
)

# Select Debug or Release compilation
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
else()
    add_compile_definitions(RELEASE)
endif()

# Select platform
if(LINUX)
    add_compile_definitions(LINUX)
elseif(WIN32)
    add_compile_definitions(WINDOWS)
elseif(APPLE)
    add_compile_definitions(MACOS)
endif()