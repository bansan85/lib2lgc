cmake_minimum_required (VERSION 2.8.11)

include(GNUInstallDirs)

add_library(2lgcerror INTERFACE)
INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/inc/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc
    FILES_MATCHING PATTERN "*.h*")

