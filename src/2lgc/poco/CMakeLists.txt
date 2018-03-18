# Use this CMakeLists.txt so *.pb.h will be in 2lgc/poco
file(GLOB PROTO_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/area.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/load.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/number.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/project.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/raw.proto)

protobuf_generate_cpp(PROTO_SRC PROTO_HEADERS ${PROTO_FILES})
# The only way I found to force a target depends on a function.
add_library(2lgcpoco STATIC ${PROTO_SRC})
