add_subdirectory(2lgc/poco)

file(GLOB HEADERS
  ${CMAKE_CURRENT_BINARY_DIR}/2lgc/poco/*.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/compatibility/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/error/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/math/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/visitor/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/*.h
  )

add_library(2lgc SHARED ${PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/math/compare_decimal.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_unit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_value.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/backtrace.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/gdb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/set_stack.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/stack.cc
  )
target_link_libraries(2lgc
  -Wl,--whole-archive 2lgcpoco -Wl,--no-whole-archive
  stdc++fs
  )

set_target_properties(2lgc
  PROPERTIES FRAMEWORK TRUE
  PUBLIC_HEADER "${HEADERS}")

if (CHECK_CODE)
  set_target_properties(2lgc PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

# Install all .h to /usr/include/2lgc/...
install(TARGETS 2lgc
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc)
