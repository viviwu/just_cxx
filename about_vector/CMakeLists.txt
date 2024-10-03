cmake_minimum_required(VERSION 3.5)

project(about_vector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(about_vector
  main.cpp
)

include(GNUInstallDirs)
install(TARGETS about_vector
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
