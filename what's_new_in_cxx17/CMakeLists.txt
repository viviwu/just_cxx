cmake_minimum_required(VERSION 3.17)
project(cxx17news VERSION "1.2.3" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")


add_executable(${PROJECT_NAME}
        main.cpp
        cxx17_new_feature.cpp
        cxx14_new_feature.cpp
        some_cxx11_feature.cpp
        tree_node.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

message("C compiler: " ${CMAKE_C_COMPILER})
message("C compiler name: " ${CMAKE_C_COMPILER_ID})

message("C++ compiler: " ${CMAKE_CXX_COMPILER})
message("C++ compiler name: " ${CMAKE_CXX_COMPILER_ID})
