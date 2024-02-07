cmake_minimum_required(VERSION 3.1)

file(GLOB cpp_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  "*.cpp")

foreach(cpp_file IN LISTS cpp_files)
    get_filename_component(file_without_extension ${cpp_file} NAME_WE)
    add_executable(${file_without_extension} ${cpp_file})
endforeach()
