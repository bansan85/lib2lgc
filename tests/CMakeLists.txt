enable_testing()

# add_custom_command need a local TARGET
add_custom_target(check_all_tests)

add_executable(visitor1 EXCLUDE_FROM_ALL pattern/visitor/visitor1.cc)
add_executable(gdb1 EXCLUDE_FROM_ALL software/gdb/gdb1.cc)
target_link_libraries(visitor1 ${Protobuf_LIBRARIES} 2lgc)
target_link_libraries(gdb1 ${Protobuf_LIBRARIES} 2lgc)
add_test(visitor1 visitor1)
add_test(gdb1 gdb1)

if (CHECK_CODE)
  set_target_properties(visitor1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  set_target_properties(gdb1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_custom_command(TARGET check_all_tests POST_BUILD
  COMMAND ${CMAKE_CTEST_COMMAND})
add_dependencies(check_all_tests visitor1 gdb1)

add_dependencies(check check_all_tests)
