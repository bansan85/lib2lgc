include(FindProtobuf)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
  ${CMAKE_CURRENT_SOURCE_DIR}/../inc)
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

if (LIB2LGC)
  include_directories(${LIB2LGC}/libheader/inc)
endif()

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER actions.proto)

set_source_files_properties(${PROTO_SRC} PROPERTIES COMPILE_FLAGS "-Wno-suggest-override -Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal -Wno-extended-offsetof")

add_executable(publisher1 EXCLUDE_FROM_ALL ${PROTO_HEADER} ${PROTO_SRC} publisher1.cc)
target_link_libraries(publisher1 ${Protobuf_LIBRARIES} 2lgcpublisher)
add_test(publisher1 publisher1)
if (CHECK_CODE)
  set_target_properties(publisher1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_dependencies(check publisher1)
