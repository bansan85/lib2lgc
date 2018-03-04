include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

if (INCLUDE_HEADER)
  include_directories(${INCLUDE_HEADER})
endif()

add_executable(gdb1 EXCLUDE_FROM_ALL gdb1.cc)
target_link_libraries(gdb1 ${Protobuf_LIBRARIES} 2lgcgdb)
add_test(gdb1 gdb1)
if (CHECK_CODE)
  set_target_properties(gdb1 PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_dependencies(check gdb1)
