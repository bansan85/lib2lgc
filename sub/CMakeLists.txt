cmake_minimum_required(VERSION 2.8.11)

project(visitor)

set(INCLUDE_POCO)
set(LIBRARY_POCO)
set(INCLUDE_POCO_PROTOBUF)
set(INCLUDE_HEADER)
set(INCLUDE_MATH)
set(LIBRARY_MATH)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include(2lgc)

add_subdirectory(inc)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)
add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND}
        COMMAND clang-format -style=Google -i ${ALL_SOURCE_FILES})
enable_testing()
add_subdirectory(test)

