configure_file(${CMAKE_CURRENT_SOURCE_DIR}/2lgc/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/2lgc/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/swig/lib2lgc.i.in ${CMAKE_CURRENT_BINARY_DIR}/swig/lib2lgc.i)

add_subdirectory(2lgc/poco)

file(GLOB_RECURSE 2LGC_HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h)

file(GLOB_RECURSE 2LGC_SOURCES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.cc)

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
