cmake_minimum_required(VERSION 3.8)

set(CMAKE_VERBOSE_MAKEFILE ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(COMPILER_OPTIONS_CXX -Wall -Wextra -Wno-switch)

add_subdirectory(source)
add_subdirectory(tests)