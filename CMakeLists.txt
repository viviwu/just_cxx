cmake_minimum_required(VERSION 3.17)

project(just_cxx)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#add_subdirectory(base_std_features)

#add_subdirectory(protobuf_demo)

#add_subdirectory(platform_app)

add_subdirectory(data_store)