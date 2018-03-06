include(FindProtobuf)
include(GNUInstallDirs)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER
  ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/area.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/load.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/number.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/project.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/raw.proto)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

if (LIB2LGC)
  include_directories(${LIB2LGC}/libvisitor/inc ${LIB2LGC}/libheader/inc ${LIB2LGC}/libmath/inc)
endif()

file(GLOB HEADERS ${PROTO_HEADER} ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.h)

set_source_files_properties(${PROTO_SRC} PROPERTIES COMPILE_FLAGS "-Wno-suggest-override -Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal")

add_library(2lgcpoco SHARED ${PROTO_SRC} number.cc number_visitor_unit.cc number_visitor_value.cc)
set_target_properties(2lgcpoco
  PROPERTIES FRAMEWORK TRUE
  PUBLIC_HEADER "${HEADERS}")
if (CHECK_CODE)
  set_target_properties(2lgcpoco PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

install(TARGETS 2lgcpoco
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc)
