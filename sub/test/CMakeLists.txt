include(FindProtobuf)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR}
        # For generated *.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}
        # For *.cc included
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc)

if (INCLUDE_HEADER)
        include_directories(${INCLUDE_HEADER})
endif()

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER actions.proto)

add_executable(publisher1 EXCLUDE_FROM_ALL ${PROTO_HEADER} ${PROTO_SRC} publisher1.cc)
target_link_libraries(publisher1 ${Protobuf_LIBRARIES} 2lgcpublisher)
add_test(publisher1 publisher1)

#add_executable(publisher2 ${PROTO_HEADER} ${PROTO_SRC} publisher2.cc)
#target_link_libraries(publisher2 ${Protobuf_LIBRARIES} 2lgcpublisher)

add_dependencies(check publisher1)
