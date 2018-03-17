add_subdirectory(2lgc/poco)
set_source_files_properties(${PROTO_SRC} PROPERTIES COMPILE_FLAGS "-Wno-suggest-override -Wno-zero-as-null-pointer-constant -Wno-unused-parameter -Wno-switch-default -Wno-effc++ -Wno-float-equal -Wno-extended-offsetof")

file(GLOB HEADERS
  ${PROTO_HEADER}
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/pattern/visitor/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/error/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/compatibility/*.h)

add_library(2lgc SHARED ${PROTO_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_unit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/2lgc/poco/number_visitor_value.cc
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
