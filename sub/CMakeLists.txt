cmake_minimum_required(VERSION 2.8.11)

project(publisher)

set(INCLUDE_HEADER)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
