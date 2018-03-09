cmake_minimum_required (VERSION 2.8.11)

include(GNUInstallDirs)

set(CHECK_CODE)

add_library(2lgcerror INTERFACE)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/inc/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc
  FILES_MATCHING PATTERN "*.h*")

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)

add_custom_target(check)

if (CHECK_CODE)
  add_custom_command(TARGET check
    COMMAND clang-format -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES}
#-readability/braces: disabled false positive because the brace of the macro in the line after the if.
    COMMAND cpplint.py --root=${CMAKE_SOURCE_DIR}/inc --filter=-runtime/printf,-readability/braces ${ALL_SOURCE_FILES}
    COMMAND cppcheck --check-config --language=c++ --std=c++14 --inconclusive --enable=all ${ALL_SOURCE_FILES})
endif()
