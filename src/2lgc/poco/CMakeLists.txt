# Use this CMakeLists.txt so *.pb.h will be in 2lgc/poco
file(GLOB PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)

protobuf_generate_cpp(PROTO_SRC PROTO_HEADERS ${PROTO_FILES})

set_source_files_properties(${PROTO_SRC} PROPERTIES COMPILE_FLAGS
  "${PROTOBUF_CXX_FLAGS}")

# The only way I found to force a target depends on a function.
add_library(2lgcpoco STATIC ${PROTO_SRC})
# To make -Wl,--whole-archive works.
set_property(TARGET 2lgcpoco PROPERTY POSITION_INDEPENDENT_CODE ON)
