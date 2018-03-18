cmake_minimum_required (VERSION 3.8.0)

include(GNUInstallDirs)
include(FindProtobuf)
find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CHECK_CODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/util/cmake;${CMAKE_MODULE_PATH}")

include(2lgc)

if (CHECK_CODE)
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path} -Xiwyu --mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/util/iwyu/mapping.imp)
endif()

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/src)

add_custom_target(check)

add_subdirectory(src)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)

if (CHECK_CODE)
  add_custom_command(TARGET check
    COMMAND clang-format -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES}
#-readability/braces: disabled false positive because the brace of the macro in the line after the if.
    COMMAND cpplint.py --root=lib2lgc/src/2lgc --filter=-runtime/printf,-readability/braces,-whitespace/braces ${ALL_SOURCE_FILES}
    COMMAND cppcheck --check-config -I${CMAKE_CURRENT_SOURCE_DIR}/src --language=c++ --std=c++14 --inconclusive --enable=all ${ALL_SOURCE_FILES})
endif()
