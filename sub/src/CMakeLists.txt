include(GNUInstallDirs)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

if (INCLUDE_HEADER)
  include_directories(${INCLUDE_HEADER})
endif()

add_library(2lgcmath SHARED
  compare_decimal.cc)
set_target_properties(2lgcmath
  PROPERTIES FRAMEWORK TRUE
  PUBLIC_HEADER "${HEADERS}")

if (CHECK_CODE)
  set_target_properties(2lgcmath PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

install(TARGETS 2lgcmath
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc)
