cmake_minimum_required(VERSION 3.17)
project(standard_cxx_features VERSION "1.2.3" LANGUAGES CXX)

# set c++ standard 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")


add_executable(${PROJECT_NAME}
        main.cpp
        geometry.h
        cxx14_new_feature.cpp
        some_cxx11_feature.cpp
        tree_node.cpp
        )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#message("C compiler: " ${CMAKE_C_COMPILER})
#message("C compiler name: " ${CMAKE_C_COMPILER_ID})
message("C++ compiler: " ${CMAKE_CXX_COMPILER})
message("C++ compiler name: " ${CMAKE_CXX_COMPILER_ID})



#add_definitions(-std=c++17)
#message(STATUS "$ENV{CXXFLAGS} ")
#set(CMAKE_CXX_FLAGS "-std=c++17")
#    -rdynamic  #  通知链接器将所有符号添加到动态符号表中
#    -O3        #  -O3是一个优化选项，告诉编译器优化我们的代码
#    -fPIC      #  作用于编译阶段，告诉编译器产生与位置无关代码
#    -ggdb      #  编译器生成gdb专用的更为丰富的调试信息
#    -std=c++17
#    -Wall      # 告诉编译器 编译后显示所有警告
#    -Wno-deprecated     # 不要警告使用已弃用的功能
#    -Werror   # 告诉编译器视所有警告为错误，出现任何警告立即放弃编译
#    -Wno-unused-function   # 关闭 函数被定义了但没有被使用 而产生的警告，即，不使用的函数不警告。
#    -Wno-builtin-macro-redefined  # 如果某些内置宏被重新定义，请不要警告。
#    -Wno-deprecated-declarations  # 关闭使用废弃API的警告