cmake_minimum_required(VERSION 3.20)

project(MosaicEngine VERSION 25.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build Mosaic as a shared library" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE MOSAIC_SOURCES "source/*.cpp")
file(GLOB_RECURSE MOSAIC_HEADERS "include/Mosaic/*.hpp" "include/Mosaic/*.inl")

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_library(Mosaic ${MOSAIC_SOURCES} ${MOSAIC_HEADERS})

target_link_libraries(Mosaic PUBLIC glfw GLEW::GLEW OpenGL::GL)

target_compile_options(Mosaic PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_include_directories(Mosaic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(CODE "
    file(REMOVE_RECURSE \"\${CMAKE_INSTALL_PREFIX}/include/Mosaic\")
")

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
    elseif(APPLE)
        target_compile_definitions(Mosaic PUBLIC MOSAIC_PLATFORM_MACOS)
    else()
        message(FATAL_ERROR "Unsupported Unix-like platform for Mosaic")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform for Mosaic")
endif()
