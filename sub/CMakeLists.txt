cmake_minimum_required(VERSION 2.8.11)

project(math)

set(INCLUDE_HEADER)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

include(2lgc)

add_subdirectory(src)

file(GLOB_RECURSE ALL_SOURCE_FILES *.h *.cc)
add_custom_target(check
  COMMAND clang-format -style=Google -i ${ALL_SOURCE_FILES}
  COMMAND cpplint.py --root=libmath/inc --filter=-build/include ${ALL_SOURCE_FILES})
