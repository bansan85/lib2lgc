cmake_minimum_required(VERSION 3.8.0)

project(gdb)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
enable_testing()
add_subdirectory(test)

