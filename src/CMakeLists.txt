configure_file(${CMAKE_CURRENT_SOURCE_DIR}/2lgc/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/2lgc/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/swig/lib2lgc.i.in ${CMAKE_CURRENT_BINARY_DIR}/swig/lib2lgc.i)

add_subdirectory(2lgc/poco)

set(2LGC_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/compat.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/error/show.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/filesystem/files.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/math/compare_decimal.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/math/hash.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/net/linux.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/net/openssl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/net/strategy_listen_tcp_linux.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/abstract_factory.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/command/undo_interface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/iterator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_direct.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_interface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_grpc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_tcp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_tcp_ipv6.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_subscriber_grpc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_subscriber.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_subscriber_tcp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_direct.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_grpc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_interface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_ip.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp_linux.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp_linux_ipv6.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/strategy_publisher_tcp_linux_open_ssl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/subscriber_interface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/subscriber_local.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/subscriber_server_grpc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/subscriber_server_tcp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/singleton.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/strategy.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/visitor/visitable.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/visitor/visitor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_impl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_unit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_value.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/backtrace.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/function.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/gdb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/gdb_server.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/set_stack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/stack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/text/string_ext.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/utils/count_lock.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/utils/tree.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/utils/undomanager/undomanager.h)

set(2LGC_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/filesystem/files.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/math/compare_decimal.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/math/hash.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/net/linux.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/net/openssl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_impl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_unit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_value.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/backtrace.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/function.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/gdb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/gdb_server.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/set_stack.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/software/gdb/stack.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/text/string_ext.cc)

set(2LGC_TEMPLATE_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/abstract_factory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/net/strategy_listen_tcp_linux.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_direct.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_interface.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_grpc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_tcp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_tcp_ipv4.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_publisher_tcp_ipv6.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_subscriber_grpc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_subscriber.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/connector_subscriber_tcp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_grpc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_interface.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_ip.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp_linux.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp_linux_ipv4.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/publisher_tcp_linux_ipv6.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/strategy_publisher_tcp_linux_open_ssl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/subscriber_local.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/subscriber_server_grpc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/publisher/subscriber_server_tcp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/singleton.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/strategy.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/utils/tree.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/utils/undomanager/undomanager.cc
  )

# Run IWYU or WALL in template class.
if (IWYU OR WALL)
  set(2LGC_SOURCES ${2LGC_SOURCES} ${2LGC_TEMPLATE_SOURCE})
endif()

set(2LGC_HEADERS ${2LGC_HEADERS} ${2LGC_TEMPLATE_SOURCE})

add_library(2lgc SHARED ${2LGC_SOURCES})

target_link_libraries(2lgc
  -Wl,--whole-archive 2lgcpoco -Wl,--no-whole-archive
  stdc++fs
  )

set_target_properties(2lgc
  PROPERTIES FRAMEWORK TRUE
  PUBLIC_HEADER "${2LGC_HEADERS} ${PROTO_HEADERS}")

if (IWYU)
  set_target_properties(2lgc PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

# Install all .h to /usr/include/2lgc/...
install(TARGETS 2lgc
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc)

if (DOXYGEN_FOUND)
  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
