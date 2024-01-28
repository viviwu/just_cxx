cmake_minimum_required(VERSION 3.1)

project(tree_node_zip)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(protobuf_path "/opt/homebrew/Cellar/protobuf/25.2")
set(abseil_path "/opt/homebrew/Cellar/abseil/20230802.1")
list(APPEND CMAKE_PREFIX_PATH ${protobuf_DIR} ${abseil_DIR})
#error "C++ versions less than C++14 are not supported.
find_package(protobuf REQUIRED PATHS ${protobuf_path}/lib/cmake/protobuf)
find_package(utf8_range REQUIRED PATHS ${protobuf_path}/lib/cmake/utf8_range)
find_package(absl REQUIRED PATHS ${abseil_path}/lib/cmake/absl)

add_executable(tree_node_zip
        tree.pb.h
        tree.pb.cc
        tree_node_zip.cpp
        )

target_link_libraries(tree_node_zip
        protobuf::libprotobuf
        utf8_range::utf8_range
        absl::strings
        absl::status
        absl::flat_hash_map
        )
