cmake_minimum_required(VERSION 2.8.11)

project(publisher)

set(LIB2LGC)
set(CHECK_CODE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
if (LIB2LGC)
  set(CMAKE_MODULE_PATH "${LIB2LGC}/utils/cmake;${CMAKE_MODULE_PATH}")
endif()

include(2lgc)

if (CHECK_CODE)
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  if (LIB2LGC)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path} -Xiwyu --mapping_file=${LIB2LGC}/utils/iwyu/mapping.imp)
  endif()
endif()

file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cc test/*.cc inc/*.h)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
if (CHECK_CODE)
  if (LIB2LGC)
    set(INCLUDE_CPPCHECK "-I${LIB2LGC}/libheader/inc;-I${CMAKE_CURRENT_BINARY_DIR}/test;-I${CMAKE_CURRENT_SOURCE_DIR}/inc")
  else()
    set(INCLUDE_CPPCHECK "")
  endif()
  add_custom_command(TARGET check
    COMMAND clang-format -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES}
    COMMAND cpplint.py --root=../inc --filter=-whitespace/braces,-whitespace/newline,-build/include ${ALL_SOURCE_FILES}
    COMMAND cppcheck --check-config --language=c++ --std=c++14 --inconclusive --enable=all ${INCLUDE_CPPCHECK} ${ALL_SOURCE_FILES})
endif()

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
