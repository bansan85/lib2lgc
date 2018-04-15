cmake_minimum_required (VERSION 3.8.0)

include(GNUInstallDirs)
include(FindProtobuf)
find_package(Protobuf REQUIRED)
find_package(Doxygen)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CHECK_CODE)
if (CHECK_CODE)
  # Someone in CHECK_CODE need compile_commands.json.
  # I forget which one.
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
set(DISABLE_VISITABLE_CACHE)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/util/cmake;${CMAKE_MODULE_PATH}")

include(2lgc)

llgc_init_cflags()
llgc_iwyu()

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/src)

add_custom_target(check)

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_subdirectory(src)
add_subdirectory(tests)

set(REMOVE_COVERAGE '${CMAKE_CURRENT_SOURCE_DIR}/tests/*' '${CMAKE_CURRENT_BINARY_DIR}/*' '/usr/include/*' '*/include/g++*')
llgc_check_all("${ALL_SOURCE_FILES}" "${REMOVE_COVERAGE}")
