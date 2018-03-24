cmake_minimum_required (VERSION 3.8.0)

include(GNUInstallDirs)
include(FindProtobuf)
find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CHECK_CODE)
if (CHECK_CODE)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# analyzer-constraints=z3 is totally instlable.
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#  if (CHECK_CODE)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --analyze -Xanalyzer -analyzer-constraints=z3")
#  endif()
#endif()
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

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_subdirectory(src)
add_subdirectory(tests)

if (CHECK_CODE)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(GCOV_BIN ${CMAKE_CURRENT_SOURCE_DIR}/util/llvm/llvm-gcov)
  else()
    set(GCOV_BIN gcov)
  endif()
  find_program(CLANG_FORMAT clang-format)
  if (CLANG_FORMAT)
    add_custom_command(TARGET check
      COMMAND ${CLANG_FORMAT} -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES})
  endif()
  #-readability/braces: disabled false positive because the brace of the macro in the line after the if.
  #-build/include_what_you_use: cpplint.py and iwyu have sometimes differents issues.
  #-build/c++11: is for chrome project.
  #-build/include_order: cpplint.py and iwyu have sometimes differents rules.
  #-readability/nolint: clang -Weverything has more nolint that cpplint.
  #-whitespace/line_length: clang-format do what he can.
  #-build/include: implementation of template with template class in source instead of header needs include .cc.
  find_program(CPPLINT cpplint.py)
  if (CPPLINT)
    add_custom_command(TARGET check
      COMMAND ${CPPLINT} --root=src/2lgc --filter=-runtime/printf,-readability/braces,-whitespace/braces,-build/include_what_you_use,-whitespace/newline,-build/c++11,-build/include_order,-readability/nolint,-whitespace/line_length,-build/include ${ALL_SOURCE_FILES})
  endif()
  find_program(CPPCHECK cppcheck)
  if (CPPCHECK)
    add_custom_command(TARGET check
      COMMAND ${CPPCHECK} -I${CMAKE_CURRENT_SOURCE_DIR}/src -I${CMAKE_CURRENT_BINARY_DIR}/src --language=c++ --std=c++14 --inline-suppr --inconclusive --enable=all ${ALL_SOURCE_FILES})
  endif()
  add_custom_command(TARGET check
    COMMAND lcov --directory ${CMAKE_CURRENT_BINARY_DIR} --gcov-tool ${GCOV_BIN} --capture --rc lcov_branch_coverage=1 --output-file ${CMAKE_CURRENT_BINARY_DIR}/lib2lgc-coverage.info
    COMMAND genhtml --output-directory ${CMAKE_CURRENT_BINARY_DIR}/coverage --demangle-cpp --num-spaces 2 --sort --function-coverage --branch-coverage --legend ${CMAKE_CURRENT_BINARY_DIR}/lib2lgc-coverage.info)
  #-llvm-include-order: Google and LLVM have differents rules.
  #-llvm-header-guard: cpplint do the work well.
  #-fuchsia-default-arguments: don't always write default boring arguments.
  #-clang-diagnostic-covered-switch-default: gcc wants a default case.
  #-cppcoreguidelines-pro-bounds-array-to-pointer-decay: warn all assert.
  #-hicpp-no-array-decay: warn all assert.
  find_program(CLANG_TIDY clang-tidy)
  if (CLANG_TIDY)
    string(REPLACE "bin" "share/clang" RUN_CLANG_TIDY_ ${CLANG_TIDY})
    string(REPLACE "clang-tidy" "run-clang-tidy.py" RUN_CLANG_TIDY ${RUN_CLANG_TIDY_})
    add_custom_command(TARGET check
      COMMAND ${RUN_CLANG_TIDY} ${ALL_SOURCE_FILES} -checks='*,-llvm-include-order,-llvm-header-guard,-fuchsia-default-arguments,-clang-diagnostic-covered-switch-default,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-hicpp-no-array-decay')
  endif()
  #find_program(COVERITY_BUILD cov-build PATHS ~/info/programmation/cov-analysis-linux64-2017.07/bin)
  #if (COVERITY_BUILD)
  #  add_custom_command(TARGET check
  #    COMMAND make clean
  #    COMMAND ${COVERITY_BUILD} --dir cov-int make -j9 check
  #    COMMAND tar caf lib2lgc.lzma cov-int)
  #endif()
endif()
