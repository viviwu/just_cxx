cmake_minimum_required(VERSION 3.1)

project(protobuf_tester)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(protobuf_path "/opt/homebrew/Cellar/protobuf/25.2")
set(abseil_path "/opt/homebrew/Cellar/abseil/20230802.1")
#list(APPEND CMAKE_PREFIX_PATH ${protobuf_DIR} ${abseil_DIR})
#error "C++ versions less than C++14 are not supported.
find_package(protobuf REQUIRED PATHS ${protobuf_path}/lib/cmake/protobuf)
find_package(absl REQUIRED PATHS ${abseil_path}/lib/cmake/abseil)

add_executable(protobuf_tester
        test_msg.pb.h
        test_msg.pb.cc
        test_protobuf.cpp
        ../protobuf_demo2/tree_node_zip.cpp)

target_link_libraries(protobuf_tester
        protobuf::libprotobuf
        absl::strings
        absl::status
        absl::flat_hash_map
        )

message(STATUS "protobuf_DIR: ${protobuf_DIR}")

#include_directories(
#        ${protobuf_path}/include
#        ${abseil_path}/include
#        )

#target_link_directories(protobuf_tester
#        PUBLIC ${protobuf_path}/lib
#        PUBLIC ${abseil_path}/lib
#        )

#get_target_property(result protobuf_tester LINK_DIRECTORIES)
#message("result: ${result}") # result: /private/dir