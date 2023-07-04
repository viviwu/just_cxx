cmake_minimum_required(VERSION 3.1)

project(just_cxx)

# 添加C++11支持
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(./)

# 添加可执行文件
if(false)
add_executable(std_thread multi_threads/std_thread.cpp)
add_executable(mutex_lock multi_threads/thread_lock.cpp)
add_executable(recursive_mutex multi_threads/recursive_mutex.cpp)
add_executable(condition_variable multi_threads/condition_variable.cpp)
add_executable(read_write_lock multi_threads/read_write_lock.cpp)
add_executable(atomic_var multi_threads/atomic_var.cpp)
# 查找线程库
find_package(Threads REQUIRED)
# 链接线程库
target_link_libraries(std_thread PRIVATE Threads::Threads)


add_executable(template_func template/template_func.cpp)
add_executable(template_class template/template_class.cpp)
add_executable(template_pecialization template/template_pecialization.cpp)

add_executable(smart_ptr smart_ptr/smart_ptr.cpp)

add_executable(catch_throw exception/catch_throw.cpp)
add_executable(file_exception exception/file_exception.cpp)
add_executable(assert exception/assert.cpp)

add_executable(regex_pattern regex_pattern/regex_pattern.cpp)
endif()

# Define a variable for system name
set(SYSTEM_NAME ${CMAKE_SYSTEM_NAME})

# Check the system name and define a macro accordingly
if(SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(WINDOWS)
    add_executable(machine_info platform_utils/machine_info.cpp)
    add_executable(hardware_info platform_utils/WMI/hardware_info.cpp platform_utils/WMI/hdev_info_query.h platform_utils/WMI/hdev_info_query.cpp)
    add_executable(wdm_mac platform_utils/WDK/wdm_mac.cpp)

elseif(SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(LINUX)
    add_executable(machine_info platform_utils/WDK/machine_info.cpp)
elseif(SYSTEM_NAME STREQUAL "Darwin")
    add_compile_definitions(MACOS)
    add_executable(machine_info platform_utils/WDK/machine_info.cpp)
endif()



