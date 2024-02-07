cmake_minimum_required(VERSION 3.1)

#file(GLOB_RECURSE SOURCES "*.cpp")
#message("cpp_files: ${SOURCES}")

file(GLOB cpp_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  "*.cpp")

foreach(cpp_file IN LISTS cpp_files)
    get_filename_component(file_without_extension ${cpp_file} NAME_WE)
    message("File without .cpp extension: ${file_without_extension}")
    add_executable(${file_without_extension} ${cpp_file})
endforeach()