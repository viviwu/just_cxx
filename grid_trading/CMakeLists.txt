cmake_minimum_required(VERSION 3.5)

project(grid_trading LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
        message("Tell the compiler(MSVC) that cpp source code is encoded in utf-8!")
        add_compile_options("/source-charset:utf-8")
        #    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
endif()

add_executable(grid_trading main.cpp
  grid_strategy.h grid_strategy.cpp
  market_simulator.h market_simulator.cpp
  account.h account.cpp)

include(GNUInstallDirs)
install(TARGETS grid_trading
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
