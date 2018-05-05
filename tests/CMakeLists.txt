enable_testing()

# for net.proto
set(Protobuf_IMPORT_DIRS ${Protobuf_IMPORT_DIRS} ${PROJECT_SOURCE_DIR}/src/2lgc/poco)

# add_custom_command need a local TARGET
add_custom_target(check_all_tests)

add_executable(files1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/files1.cc)
add_executable(compare_decimal1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/math/compare_decimal1.cc)
add_executable(hash1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/math/hash1.cc)
add_executable(printf1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/override/printf1.cc)
add_executable(visitor1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/visitor/visitor1.cc)
protobuf_generate_cpp(PUBLISHER_PROTO_ACTIONS_SRC PUBLISHER_PROTO_ACTIONS_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/actions.proto)
set_source_files_properties(${PUBLISHER_PROTO_ACTIONS_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher1.cc
  PROPERTIES COMPILE_FLAGS "-Wno-suggest-override -Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal -Wno-extended-offsetof -Wno-global-constructors -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-c++98-compat-pedantic -Wno-reserved-id-macro")
add_executable(publisher1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher1.cc
  ${PUBLISHER_PROTO_ACTIONS_SRC})
protobuf_generate_cpp(PUBLISHER_PROTO_ACTIONS_TCP_SRC PUBLISHER_PROTO_ACTIONS_TCP_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/actions_tcp.proto)
set_source_files_properties(${PUBLISHER_PROTO_ACTIONS_TCP_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher2.cc
  PROPERTIES COMPILE_FLAGS "-Wno-suggest-override -Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal -Wno-extended-offsetof -Wno-global-constructors -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-c++98-compat-pedantic -Wno-reserved-id-macro")
add_executable(publisher2 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher2.cc
  ${PUBLISHER_PROTO_ACTIONS_TCP_SRC})
add_executable(poco_number EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/poco/poco_number.cc)
add_executable(gdb1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb1.cc)
add_executable(gdb2 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb2.cc)
add_executable(gdb3 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb3.cc)
add_executable(deadbeef EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/deadbeef.c)
add_executable(gdb4 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb4.cc)
add_dependencies(gdb4 deadbeef)


target_link_libraries(files1 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(compare_decimal1 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(hash1 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(printf1 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(visitor1 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_include_directories(publisher1 PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(publisher1 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_include_directories(publisher2 PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../src/2lgc/poco)
target_link_libraries(publisher2 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(poco_number ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(gdb1 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(gdb2 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(gdb3 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)
target_link_libraries(gdb4 ${Protobuf_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} 2lgc)

add_test(files1 files1)
add_test(compare_decimal1 compare_decimal1)
add_test(hash1 hash1)
add_test(printf1 printf1)
add_test(visitor1 visitor1)
add_test(publisher1 publisher1)
add_test(publisher2 publisher2)
add_test(poco_number poco_number)
add_test(gdb1 gdb1)
add_test(gdb2 gdb2 "${CMAKE_CURRENT_SOURCE_DIR}/software/gdb")
add_custom_command(TARGET gdb3 PRE_BUILD
  COMMAND echo "${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/btfull10.success" > "${CMAKE_CURRENT_BINARY_DIR}/list.bt")
add_test(gdb3 gdb3 "${CMAKE_CURRENT_SOURCE_DIR}/software/gdb" "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(TARGET gdb4 PRE_BUILD
  COMMAND echo "${CMAKE_CURRENT_BINARY_DIR}/deadbeef" > "${CMAKE_CURRENT_BINARY_DIR}/deadbeef.list")
add_test(gdb4 gdb4 "${CMAKE_CURRENT_BINARY_DIR}")

if (CHECK_CODE)
  set_target_properties(files1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(compare_decimal1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(hash1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(printf1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(visitor1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(publisher1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(publisher2 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(poco_number PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(gdb1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(gdb2 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(gdb3 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(deadbeef PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(gdb4 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_custom_command(TARGET check_all_tests POST_BUILD
  COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
add_dependencies(check_all_tests
  files1
  compare_decimal1
  hash1
  printf1
  visitor1
  publisher1
  publisher2
  poco_number
  gdb1
  gdb2
  gdb3
  gdb4)

add_dependencies(check check_all_tests)
