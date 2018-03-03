cmake_minimum_required(VERSION 2.8.11)

project(publisher)

set(INCLUDE_HEADER)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include(2lgc)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)
add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND}
  COMMAND clang-format -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES})

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
