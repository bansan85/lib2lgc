cmake_minimum_required (VERSION 2.8.11)

include(GNUInstallDirs)

add_library(2lgcerror INTERFACE)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/inc/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc
  FILES_MATCHING PATTERN "*.h*")

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)
add_custom_target(check
  COMMAND clang-format -style=Google -i ${ALL_SOURCE_FILES}
  COMMAND cpplint.py --root=libheader/inc --filter=-runtime/printf ${ALL_SOURCE_FILES})
