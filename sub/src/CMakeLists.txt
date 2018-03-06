include(GNUInstallDirs)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

if (LIB2LGC)
  include_directories(${LIB2LGC}/libheader/inc)
endif()

add_library(2lgcpublisher SHARED
  publisher_base.cc
  subscriber_direct.cc
  connector_direct.cc
  publisher_remote.cc
  subscriber_interface.cc
  connector_interface.cc)
set_target_properties(2lgcpublisher
  PROPERTIES FRAMEWORK TRUE
  PUBLIC_HEADER "${HEADERS}")

if (CHECK_CODE)
  set_target_properties(2lgcpublisher PROPERTIES CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

install(TARGETS 2lgcpublisher
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc)
