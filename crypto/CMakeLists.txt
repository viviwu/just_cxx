cmake_minimum_required(VERSION 3.5)

project(crypto LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(crypto main.cpp
  crypto.h crypto.cpp)

include(GNUInstallDirs)
install(TARGETS crypto
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
