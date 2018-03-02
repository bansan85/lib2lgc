include(GNUInstallDirs)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../inc/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

find_package(Threads)

if (INCLUDE_HEADER)
        include_directories(${INCLUDE_HEADER})
endif()

add_library(2lgcgdb SHARED
        backtrace.cc stack.cc set_stack.cc)
target_link_libraries(2lgcgdb stdc++fs ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(2lgcgdb
        PROPERTIES FRAMEWORK TRUE
        PUBLIC_HEADER "${HEADERS}")

install(TARGETS 2lgcgdb
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/2lgc)
