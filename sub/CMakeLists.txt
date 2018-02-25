cmake_minimum_required(VERSION 3.8.0)

project(gdb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include(2lgc)

add_subdirectory(src)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
        COMMAND clang-format -style=Google -i ${ALL_SOURCE_FILES})

enable_testing()
add_subdirectory(test)
