cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("Configuring for Windows")
    # Add Windows-specific configurations
#    add_executable(system_cmd system_cmd.cpp)
#    add_executable(win32_sys_info win32_sys_info.cpp)
#    add_executable(win32_ip_helper win32_ip_helper.cpp)
#    add_subdirectory(WMI)
    add_subdirectory(WDK)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("Configuring for macOS")
    # Add macOS-specific configurations
    add_executable(unix_sys_info unix_sys_info.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Configuring for Linux")
    # Add Linux-specific configurations
elseif(CMAKE_SYSTEM_NAME STREQUAL "UNIX")
    message("Configuring for UNIX")
    # Add generic UNIX configurations (this includes Linux and macOS)
    add_executable(unix_sys_info unix_sys_info.cpp)
else()
    message("Unknown operating system: ${CMAKE_SYSTEM_NAME}")
endif()
