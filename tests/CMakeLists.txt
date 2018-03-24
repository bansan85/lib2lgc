enable_testing()

# add_custom_command need a local TARGET
add_custom_target(check_all_tests)

add_executable(compare_decimal1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/math/compare_decimal1.cc)
add_executable(visitor1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/visitor/visitor1.cc)
protobuf_generate_cpp(PUBLISHER_PROTO_SRC PUBLISHER_PROTO_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/actions.proto)
set_source_files_properties(${PUBLISHER_PROTO_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher1.cc
  PROPERTIES COMPILE_FLAGS "-Wno-suggest-override -Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal -Wno-extended-offsetof -Wno-global-constructors -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-c++98-compat-pedantic -Wno-reserved-id-macro")
add_executable(publisher1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/pattern/publisher/publisher1.cc
  ${PUBLISHER_PROTO_SRC})
add_executable(gdb1 EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/software/gdb/gdb1.cc)


target_link_libraries(compare_decimal1 ${Protobuf_LIBRARIES} 2lgc)
target_link_libraries(visitor1 ${Protobuf_LIBRARIES} 2lgc)
target_include_directories(publisher1
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(publisher1 ${Protobuf_LIBRARIES} 2lgc)
target_link_libraries(gdb1 ${Protobuf_LIBRARIES} 2lgc)

add_test(compare_decimal1 compare_decimal1)
add_test(visitor1 visitor1)
add_test(publisher1 publisher1)
add_test(gdb1 gdb1)

if (CHECK_CODE)
  set_target_properties(compare_decimal1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(visitor1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(publisher1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(gdb1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_custom_command(TARGET check_all_tests POST_BUILD
  COMMAND ${CMAKE_CTEST_COMMAND})
add_dependencies(check_all_tests
  compare_decimal1
  visitor1
  publisher1
  gdb1)

add_dependencies(check check_all_tests)
