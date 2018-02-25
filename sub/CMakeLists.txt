cmake_minimum_required(VERSION 2.8.11)

project(poco)

set(INCLUDE_VISITOR)
set(INCLUDE_HEADER)
set(INCLUDE_MATH)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include(2lgc)

add_subdirectory(src)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)
add_custom_target(check COMMAND clang-format -style=Google -i ${ALL_SOURCE_FILES})
