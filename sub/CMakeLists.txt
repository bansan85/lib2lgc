cmake_minimum_required (VERSION 2.8.11)

add_library(2lgcerror INTERFACE)
INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/inc/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/2lgc
    FILES_MATCHING PATTERN "*.h*")

