cmake_minimum_required(VERSION 3.1)

project(just_cxx)

# 添加C++11支持
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(std_thread multi_threads/std_thread.cpp)
add_executable(mutex_lock multi_threads/thread_lock.cpp)
add_executable(recursive_mutex multi_threads/recursive_mutex.cpp)
add_executable(condition_variable multi_threads/condition_variable.cpp)
add_executable(read_write_lock multi_threads/read_write_lock.cpp)
add_executable(atomic_var multi_threads/atomic_var.cpp)

add_executable(template_func template/template_func.cpp)
add_executable(template_class template/template_class.cpp)
add_executable(template_pecialization template/template_pecialization.cpp)

add_executable(smart_ptr smart_ptr/smart_ptr.cpp)

add_executable(catch_throw exception/catch_throw.cpp)
add_executable(file_exception exception/file_exception.cpp)
add_executable(assert exception/assert.cpp)

add_executable(regex_pattern regex_pattern/regex_pattern.cpp)

add_executable(win_hardware_info win_api/hardware/hardware_info.cpp)

# 查找线程库
find_package(Threads REQUIRED)
# 链接线程库
target_link_libraries(std_thread PRIVATE Threads::Threads)
