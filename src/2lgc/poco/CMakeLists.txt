# Use this CMakeLists.txt so *.pb.h will be in 2lgc/poco
file(GLOB PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)

protobuf_generate_cpp(PROTO_SRC PROTO_HEADERS ${PROTO_FILES})

set_source_files_properties(${PROTO_SRC} PROPERTIES COMPILE_FLAGS
  "-Wno-suggest-override -Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal -Wno-extended-offsetof -Wno-global-constructors -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-c++98-compat-pedantic -Wno-reserved-id-macro -Wno-useless-cast -Wno-suggest-final-methods -Wno-suggest-final-types")

# The only way I found to force a target depends on a function.
add_library(2lgcpoco STATIC ${PROTO_SRC})
# To make -Wl,--whole-archive works.
set_property(TARGET 2lgcpoco PROPERTY POSITION_INDEPENDENT_CODE ON)
