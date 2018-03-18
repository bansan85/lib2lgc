add_executable(visitor1 EXCLUDE_FROM_ALL pattern/visitor/visitor1.cc)
target_link_libraries(visitor1 ${Protobuf_LIBRARIES} 2lgc)
add_test(visitor1 visitor1)

if (CHECK_CODE)
  set_target_properties(visitor1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_dependencies(check visitor1)
