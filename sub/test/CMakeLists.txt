include(FindProtobuf)
find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

if (LIB2LGC)
  include_directories(${LIB2LGC}/libpoco/inc ${LIB2LGC}/libheader/inc ${LIB2LGC}/libmath/inc SYSTEM ${LIB2LGC}/libpoco-build/src)
  link_directories(${LIB2LGC}/libpoco-build/src ${LIB2LGC}/libmath-build/src)
endif()

add_executable(visitor1 EXCLUDE_FROM_ALL ${PROTO_HEADER} ${PROTO_SRC} visitor1.cc)
target_link_libraries(visitor1 ${Protobuf_LIBRARIES} 2lgcpoco 2lgcmath)
add_test(visitor1 visitor1)

if (CHECK_CODE)
  set_target_properties(visitor1 PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_dependencies(check visitor1)

