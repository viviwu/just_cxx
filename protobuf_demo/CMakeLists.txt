cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(debug_flag "")
# Check if the build type is Debug
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Building in Debug mode")
    # Add Debug-specific configurations, compiler flags, etc.
    set(debug_flag "d")
else()
    message("build type Release?: ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")

add_executable(protobuf_tester
        test_protobuf.cpp
        test_msg.pb.h
        test_msg.pb.cc
        )

if (WIN32)
    message("build on windows!")
#    add_compile_definitions(-D_SCL_SECURE_NO_WARNINGS)
    set(protobuf_path "D:/Library/libprotobuf")
    include_directories(${protobuf_path}/include)

    target_link_directories(protobuf_tester  PRIVATE
            ${protobuf_path}/lib/${CMAKE_BUILD_TYPE}/shared_runtime
            )
    target_link_libraries(protobuf_tester
            libprotobuf${debug_flag}
            )

else()
    message("build on macos!")
    set(protobuf_path "/opt/homebrew/Cellar/protobuf/25.2")
    set(abseil_path "/opt/homebrew/Cellar/abseil/20230802.1")

    find_package(protobuf REQUIRED PATHS ${protobuf_path}/lib/cmake/protobuf)
    find_package(absl REQUIRED PATHS ${abseil_path}/lib/cmake/abseil)

    target_link_libraries(protobuf_tester
            protobuf::libprotobuf
            absl::strings
            absl::status
            absl::flat_hash_map
            )

    message(STATUS "protobuf_DIR: ${protobuf_DIR}")

endif ()



