cmake_minimum_required(VERSION 2.8.11)

project(poco)

set(INCLUDE_VISITOR)
set(INCLUDE_HEADER)
set(INCLUDE_MATH)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include(2lgc)

add_subdirectory(src)

file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cc inc/*.h)
add_custom_target(check
  COMMAND clang-format -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES}
  COMMAND cpplint.py --root=../inc --filter=-whitespace/braces,-build/include ${ALL_SOURCE_FILES})
