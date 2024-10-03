cmake_minimum_required(VERSION 3.5)

project(boost_cpp_demo LANGUAGES CXX)
# project(BoostAsioDemo)

# 寻找 Boost 库
find_package(Boost REQUIRED COMPONENTS system thread)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含 Boost 头文件目录
include_directories(${Boost_INCLUDE_DIRS})

# 创建服务器可执行文件
add_executable(AsioServer server.cpp)
# 链接 Boost 系统库和线程库
target_link_libraries(AsioServer ${Boost_LIBRARIES})

# 创建客户端可执行文件
add_executable(AsioClient client.cpp)
# 链接 Boost 系统库和线程库
target_link_libraries(AsioClient ${Boost_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS AsioServer AsioClient
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
