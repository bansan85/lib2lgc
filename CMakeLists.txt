cmake_minimum_required (VERSION 3.8.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/util/cmake;${CMAKE_MODULE_PATH}")

include(2lgc)
include(GNUInstallDirs)
include(FindOpenSSL)
include(FindSWIG)

find_package(Protobuf REQUIRED)
find_package(Doxygen)
find_package(GRPC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(WALL)
set(CLANGFORMAT)
set(IWYU)
set(COVERAGE)
set(CPP_LINT)
set(CLANGTIDY)
set(PMD)

set(DISABLE_VISITABLE_CACHE)
set(PROTOBUF_CXX_FLAGS "-Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal -Wno-global-constructors -Wno-missing-variable-declarations -Wno-c++98-compat-pedantic -Wno-reserved-id-macro -Wno-sign-conversion -Wno-suggest-override")

llgc_init_cflags()
llgc_iwyu()

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/src)

add_custom_target(check)

if (SWIG_FOUND)
  if (SWIG_VERSION VERSION_LESS "4.0.0")
    message("Swig minimum version required 4.0.0. Found: ${SWIG_VERSION}. Skipped.")
  else()
    add_custom_command(TARGET check
      COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/src/swig && ${SWIG_EXECUTABLE} -c++ -java -I${SWIG_DIR} -I${SWIG_DIR}/java -outdir ${CMAKE_CURRENT_BINARY_DIR}/src/swig lib2lgc.i)
  endif()
endif()

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_subdirectory(src)
add_subdirectory(tests)

set(REMOVE_COVERAGE '${CMAKE_CURRENT_SOURCE_DIR}/tests/*' '${CMAKE_CURRENT_BINARY_DIR}/*' '/usr/include/*' '*/include/g++*')
llgc_check_all("${ALL_SOURCE_FILES}" FALSE "${REMOVE_COVERAGE}")
