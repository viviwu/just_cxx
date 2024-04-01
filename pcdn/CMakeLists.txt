cmake_minimum_required(VERSION 3.13)
project(pcdn)

#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

 #-DCMAKE_EXPORT_COMPILE_COMMANDS=on
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(tracker_server
         tracker.cpp
)

add_executable(terminal_app
         terminal.cpp
)
