add_executable(visitor1 EXCLUDE_FROM_ALL ${PROTO_HEADER} ${PROTO_SRC} visitor1.cc)
add_test(visitor1 visitor1)

add_dependencies(check visitor1)

