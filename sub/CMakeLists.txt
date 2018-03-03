cmake_minimum_required(VERSION 3.8.0)

project(gdb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(INCLUDE_HEADER)

include(2lgc)

add_subdirectory(src)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
  COMMAND cpplint.py --root=../inc --filter=-whitespace/braces,-whitespace/newline,-build/include,-build/c++11 ${ALL_SOURCE_FILES}
  COMMAND clang-format -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES})

enable_testing()
add_subdirectory(test)
