cmake_minimum_required(VERSION 3.5)
# 设置项目名称和版本号
project(SdkApi VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/install/test")

include_directories(
        include
)
# 添加生成动态库的目标
add_library(static_sdk_api  STATIC
         src/sdk_api.cpp
        )
#target_include_directories(sdk_api PUBLIC  include)
# 对static_sdk_api的重名为sdk_api
SET_TARGET_PROPERTIES(static_sdk_api PROPERTIES OUTPUT_NAME "sdk_api")
# cmake 在构建一个新的target 时，会尝试清理掉其他使用这个名字的库，如果没有清理还是会只会构建一个动态库，不会构建出静态库
SET_TARGET_PROPERTIES(static_sdk_api PROPERTIES CLEAN_DIRECT_OUTPUT 1)

add_library(dynamic_sdk_api SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk_api.cpp
        )
# 对static_sdk_api的重名为sdk_api
SET_TARGET_PROPERTIES(dynamic_sdk_api PROPERTIES  OUTPUT_NAME "sdk_api")
SET_TARGET_PROPERTIES(dynamic_sdk_api PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(dynamic_sdk_api PROPERTIES VERSION 1.2 SOVERSION 1)

#target_include_directories(sdk_api PUBLIC  include)
#add_library(bar STATIC IMPORTED)
#set_property()、
#target_link_libraries(INTERFACE)、
#target_link_options(INTERFACE)、
#target_include_directories(INTERFACE)、
#target_compile_options(INTERFACE)、
#target_compile_definitions(INTERFACE)
#target_sources(INTERFACE)。

# 添加生成可执行文件的目标
add_executable(sdk_tester
        src/main.cpp
        )

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

# 链接可执行文件与静态库、动态库
target_link_libraries(sdk_tester static_sdk_api )
# INTERFACE_INCLUDE_DIRECTORIES  让它明白当从构建目录内使用和从安装/包中使用时，需要不同的 INTERFACE 位置。
target_include_directories(sdk_tester
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)


# 安装目标文件和相关文件
install(TARGETS static_sdk_api  dynamic_sdk_api sdk_tester
        EXPORT SdkApiTargets
        BUNDLE DESTINATION .
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PRIVATE_HEADER	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

# 安装配置文件和README文件
install(DIRECTORY  include DESTINATION .)
install(DIRECTORY  config DESTINATION share)
install(FILES README.md DESTINATION share)

# 创建并安装导出文件
install(EXPORT SdkApiTargets
        FILE SdkApiTargets.cmake
        NAMESPACE SdkApi::
        DESTINATION lib/cmake/SdkApi
        )

# 创建并安装配置文件
include(CMakePackageConfigHelpers)
configure_package_config_file(
        config/SdkApiConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/SdkApiConfig.cmake
        INSTALL_DESTINATION lib/cmake/SdkApi
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SdkApiConfig.cmake
        DESTINATION lib/cmake/SdkApi
        )

#在安装期间执行指定的脚本。
#install(SCRIPT /path/to/script)

#RUNTIME_DEPENDENCY_SET：将指定目标的运行时依赖项作为依赖项集合进行安装。
#install(RUNTIME_DEPENDENCY_SET my_target_deps DESTINATION /path/to/destination)

install(CODE "message(STATUS 'Installing...')")
# 打印安装路径
message(STATUS "Install path: ${CMAKE_INSTALL_PREFIX}")