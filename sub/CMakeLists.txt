cmake_minimum_required(VERSION 2.8.11)

project(visitor)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

add_subdirectory(inc)

enable_testing()
add_subdirectory(test)

