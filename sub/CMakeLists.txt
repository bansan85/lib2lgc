cmake_minimum_required(VERSION 2.8.11)

project(visitor)

set(INCLUDE_POCO)
set(LIBRARY_POCO)
set(INCLUDE_POCO_PROTOBUF)
set(INCLUDE_HEADER)
set(INCLUDE_MATH)
set(LIBRARY_MATH)

add_subdirectory(inc)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
enable_testing()
add_subdirectory(test)

