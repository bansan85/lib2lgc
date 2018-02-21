include(GNUInstallDirs)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

add_library(2lgcgdb SHARED
        bt.cc)
set_target_properties(2lgcgdb
        PROPERTIES FRAMEWORK TRUE
        PUBLIC_HEADER "${HEADERS}")

install(TARGETS 2lgcgdb
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc)
