cmake_minimum_required(VERSION 3.5)

project(xlogger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("/source-charset:utf-8")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(xlogger
  main.cpp
  logger.h
  logger.cpp
)

include(GNUInstallDirs)
install(TARGETS xlogger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
