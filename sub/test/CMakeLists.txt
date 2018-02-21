include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

add_executable(gdb1 EXCLUDE_FROM_ALL gdb1.cc)
target_link_libraries(gdb1 ${Protobuf_LIBRARIES} 2lgcgdb)
add_test(gdb1 gdb1)

add_dependencies(check gdb1)
