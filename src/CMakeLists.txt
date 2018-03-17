add_custom_target(check)

# Install all .h to /usr/include/2lgc/...
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/2lgc
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h")

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)

if (CHECK_CODE)
  add_custom_command(TARGET check
    COMMAND clang-format -style='{BasedOnStyle: google, BreakBeforeBraces: Custom, BraceWrapping: { AfterClass: true, AfterControlStatement: true, AfterEnum : true, AfterFunction : true, AfterNamespace : true, AfterObjCDeclaration: true, AfterStruct : true, AfterUnion : true, BeforeCatch : true, BeforeElse : true, IndentBraces : false } }' -i ${ALL_SOURCE_FILES}
#-readability/braces: disabled false positive because the brace of the macro in the line after the if.
    COMMAND cpplint.py --root=src/2lgc --filter=-runtime/printf,-readability/braces,-whitespace/braces ${ALL_SOURCE_FILES}
    COMMAND cppcheck --check-config -I${CMAKE_CURRENT_SOURCE_DIR} --language=c++ --std=c++14 --inconclusive --enable=all ${ALL_SOURCE_FILES})
endif()
