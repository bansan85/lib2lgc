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

add_dependencies(check visitor1)
add_dependencies(check gdb1)
