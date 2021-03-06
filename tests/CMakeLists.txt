enable_testing()

# for net.proto
set(Protobuf_IMPORT_DIRS ${Protobuf_IMPORT_DIRS} ${PROJECT_SOURCE_DIR}/src/2lgc/poco)

add_custom_target(git_pre_commit_tests_proto
  COMMAND ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}/pattern/abstract_factory -I ${PROJECT_SOURCE_DIR}/src/2lgc/poco ${CMAKE_CURRENT_SOURCE_DIR}/pattern/abstract_factory/abstract_factory.proto
  COMMAND ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher -I ${PROJECT_SOURCE_DIR}/src/2lgc/poco ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/direct.proto
  COMMAND ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher -I ${PROJECT_SOURCE_DIR}/src/2lgc/poco ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/tcp.proto
  COMMAND ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher -I ${PROJECT_SOURCE_DIR}/src/2lgc/poco ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/rpc.proto
  COMMAND ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}/utils/undomanager -I ${PROJECT_SOURCE_DIR}/src/2lgc/poco ${CMAKE_CURRENT_SOURCE_DIR}/utils/undomanager/undo_command1.proto
  DEPENDS git_pre_commit_lib_proto)

# add_custom_command need a local TARGET
add_custom_target(check_all_tests)

add_executable(files1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/files1.cc)
add_executable(compare_decimal1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/math/compare_decimal1.cc)
add_executable(hash1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/math/hash1.cc)
add_executable(visitor1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/visitor/visitor1.cc)

protobuf_generate_cpp(ABSTRACT_FACTORY_PROTO_SRC ABSTRACT_FACTORY_PROTO_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/abstract_factory/abstract_factory.proto)
set_source_files_properties(${ABSTRACT_FACTORY_PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/abstract_factory/abstract_factory1.cc
  PROPERTIES COMPILE_FLAGS "${PROTOBUF_CXX_FLAGS}")
add_executable(abstract_factory1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/abstract_factory/abstract_factory1.cc
  ${ABSTRACT_FACTORY_PROTO_SRC})

protobuf_generate_cpp(PUBLISHER_DIRECT_PROTO_SRC PUBLISHER_PROTO_ACTIONS_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/direct.proto)
set_source_files_properties(${PUBLISHER_DIRECT_PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_direct.cc
  PROPERTIES COMPILE_FLAGS "${PROTOBUF_CXX_FLAGS}")
add_executable(publisher_direct EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_direct.cc
  ${PUBLISHER_DIRECT_PROTO_SRC})

protobuf_generate_cpp(PUBLISHER_TCP_PROTO_SRC PUBLISHER_TCP_PROTO_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/tcp.proto)
set_source_files_properties(${PUBLISHER_TCP_PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_tcp_ipv4.cc
  PROPERTIES COMPILE_FLAGS "${PROTOBUF_CXX_FLAGS}")
add_executable(publisher_tcp_ipv4 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_tcp_ipv4.cc
  ${PUBLISHER_TCP_PROTO_SRC})
set_source_files_properties(${PUBLISHER_TCP_PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_tcp_ipv6.cc
  PROPERTIES COMPILE_FLAGS "${PROTOBUF_CXX_FLAGS}")
add_executable(publisher_tcp_ipv6 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_tcp_ipv6.cc
  ${PUBLISHER_TCP_PROTO_SRC})
set_source_files_properties(${PUBLISHER_TCP_PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_tcp_openssl_ipv4.cc
  PROPERTIES COMPILE_FLAGS "${PROTOBUF_CXX_FLAGS}")
add_executable(publisher_tcp_openssl_ipv4 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_tcp_openssl_ipv4.cc
  ${PUBLISHER_TCP_PROTO_SRC})

if (GRPC_LIBRARY)
  protobuf_generate_cpp(PUBLISHER_RPC_PROTO_SRC PUBLISHER_RPC_PROTO_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/rpc.proto)
  list(APPEND _protobuf_include_path -I ${PROJECT_SOURCE_DIR}/src/2lgc/poco)
  grpc_generate_cpp(PUBLISHER_RPC_GRPC_SRC PUBLISHER_RPC_GRPC_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/rpc.proto)
  set_source_files_properties(${PUBLISHER_RPC_PROTO_SRC} ${PUBLISHER_RPC_GRPC_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_grpc.cc
    PROPERTIES COMPILE_FLAGS "${PROTOBUF_CXX_FLAGS}")
  add_executable(publisher_grpc EXCLUDE_FROM_ALL
    ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher_grpc.cc
    ${PUBLISHER_RPC_PROTO_SRC} ${PUBLISHER_RPC_GRPC_SRC})
endif()

add_executable(strategy1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/strategy/strategy1.cc)

add_executable(poco_number EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/poco/poco_number.cc)
add_executable(gdb_backtrace_factory EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb_backtrace_factory.cc)
add_executable(gdb_set_stack_add EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb_set_stack_add.cc)
add_executable(gdb_set_stack_add_multiple EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb_set_stack_add_multiple.cc)
add_executable(deadbeef EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/deadbeef.c)
add_executable(gdb_run_gdb EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb_run_gdb.cc)
add_dependencies(gdb_run_gdb deadbeef)
add_executable(tree1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/tree/tree1.cc)

protobuf_generate_cpp(UNDO_COMMAND_ADD_PROTO_SRC UNDO_COMMAND_ADD_PROTO_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/undomanager/undo_command1.proto)
set_source_files_properties(${UNDO_COMMAND_ADD_PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/undomanager/undomanager_undo_interface_undo_do_redo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/undomanager/undomanager_undomanager_undo_do_redo.cc
  PROPERTIES COMPILE_FLAGS "${PROTOBUF_CXX_FLAGS}")
add_executable(undomanager_undo_interface_undo_do_redo EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/undomanager/undomanager_undo_interface_undo_do_redo.cc
  ${UNDO_COMMAND_ADD_PROTO_SRC})
add_executable(undomanager_undomanager_undo_do_redo EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/undomanager/undomanager_undomanager_undo_do_redo.cc
  ${UNDO_COMMAND_ADD_PROTO_SRC})

set(ALL_LIBS ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY} 2lgc)

target_link_libraries(files1 ${ALL_LIBS})
target_link_libraries(compare_decimal1 ${ALL_LIBS})
target_link_libraries(hash1 ${ALL_LIBS})
target_link_libraries(visitor1 ${ALL_LIBS})
target_include_directories(abstract_factory1 PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(abstract_factory1 ${ALL_LIBS})
target_include_directories(publisher_direct PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(publisher_direct ${ALL_LIBS})
target_include_directories(publisher_tcp_ipv4 PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../src/2lgc/poco)
target_link_libraries(publisher_tcp_ipv4 ${ALL_LIBS})
target_include_directories(publisher_tcp_openssl_ipv4 PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../src/2lgc/poco)
target_link_libraries(publisher_tcp_openssl_ipv4 ${ALL_LIBS})
target_include_directories(publisher_tcp_ipv6 PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../src/2lgc/poco)
target_link_libraries(publisher_tcp_ipv6 ${ALL_LIBS})

if (GRPC_LIBRARY)
  target_include_directories(publisher_grpc PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../src/2lgc/poco)
  target_link_libraries(publisher_grpc ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} ${GRPC_GRPC++_LIBRARY} 2lgc)
endif()

target_link_libraries(strategy1 ${ALL_LIBS})

target_link_libraries(poco_number ${ALL_LIBS})
target_link_libraries(gdb_backtrace_factory ${ALL_LIBS})
target_link_libraries(gdb_set_stack_add ${ALL_LIBS})
target_link_libraries(gdb_set_stack_add_multiple ${ALL_LIBS})
target_link_libraries(gdb_run_gdb ${ALL_LIBS})
target_link_libraries(tree1 ${ALL_LIBS})
target_include_directories(undomanager_undo_interface_undo_do_redo PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils/undomanager)
target_link_libraries(undomanager_undo_interface_undo_do_redo ${ALL_LIBS})
target_include_directories(undomanager_undomanager_undo_do_redo PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils/undomanager)
target_link_libraries(undomanager_undomanager_undo_do_redo ${ALL_LIBS})

add_test(files1 files1)
add_test(compare_decimal1 compare_decimal1)
add_test(hash1 hash1)
add_test(visitor1 visitor1)
add_test(abstract_factory1 abstract_factory1)
add_test(publisher_direct publisher_direct)
add_test(publisher_tcp_ipv4 publisher_tcp_ipv4)
add_test(publisher_tcp_openssl_ipv4 publisher_tcp_openssl_ipv4 "${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/cert.pem" "${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/key.pem")
add_test(publisher_tcp_ipv6 publisher_tcp_ipv6)
if (GRPC_LIBRARY)
  add_test(publisher_grpc publisher_grpc)
endif()
add_test(strategy1 strategy1)
add_test(poco_number poco_number)
add_test(gdb_backtrace_factory gdb_backtrace_factory)
add_test(gdb_set_stack_add gdb_set_stack_add "${CMAKE_CURRENT_SOURCE_DIR}/software/gdb")
add_custom_command(TARGET gdb_set_stack_add_multiple PRE_BUILD
  COMMAND echo "${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/btfull10.success" > "${CMAKE_CURRENT_BINARY_DIR}/list.bt")
add_test(gdb_set_stack_add_multiple gdb_set_stack_add_multiple "${CMAKE_CURRENT_SOURCE_DIR}/software/gdb" "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(TARGET gdb_run_gdb PRE_BUILD
  COMMAND echo "${CMAKE_CURRENT_BINARY_DIR}/deadbeef" > "${CMAKE_CURRENT_BINARY_DIR}/deadbeef.list")
add_test(gdb_run_gdb gdb_run_gdb "${CMAKE_CURRENT_BINARY_DIR}")
add_test(tree1 tree1)
add_test(undomanager_undo_interface_undo_do_redo undomanager_undo_interface_undo_do_redo)
add_test(undomanager_undomanager_undo_do_redo undomanager_undomanager_undo_do_redo)

if (IWYU)
  set_target_properties(
    files1
    compare_decimal1
    hash1
    visitor1
    abstract_factory1
    publisher_direct
    publisher_tcp_ipv4
    publisher_tcp_openssl_ipv4
    publisher_tcp_ipv6
    strategy1
    poco_number
    gdb_backtrace_factory
    gdb_set_stack_add
    gdb_set_stack_add_multiple
    deadbeef
    gdb_run_gdb
    tree1
    undomanager_undo_interface_undo_do_redo
    undomanager_undomanager_undo_do_redo
    PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  if (GRPC_LIBRARY)
    set_target_properties(publisher_grpc PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  endif()
endif()  # IWYU

#add_custom_command(TARGET check_all_tests POST_BUILD
#  COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
add_dependencies(check_all_tests
  files1
  compare_decimal1
  hash1
  visitor1
  abstract_factory1
  publisher_direct
  publisher_tcp_ipv4
  publisher_tcp_openssl_ipv4
  publisher_tcp_ipv6
  poco_number
  strategy1
  gdb_backtrace_factory
  gdb_set_stack_add
  gdb_set_stack_add_multiple
  gdb_run_gdb
  tree1
  undomanager_undomanager_undo_do_redo
  undomanager_undo_interface_undo_do_redo)
if (GRPC_LIBRARY)
  add_dependencies(check_all_tests publisher_grpc)
endif()

add_dependencies(check check_all_tests)
