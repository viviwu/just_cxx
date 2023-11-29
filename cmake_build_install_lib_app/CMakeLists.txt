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

set(TARGET_LIB_NAME static_sdk_api)
set(TARGET_DYLIB_NAME dynamic_sdk_api)

configure_file(config/SdkApi.cmake.in config/SdkApi.cmake @ONLY)

include(${CMAKE_CURRENT_BINARY_DIR}/config/SdkApi.cmake)

# 添加生成可执行文件的目标
add_executable(sdk_tester
        src/main.cpp
        )

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

# 链接可执行文件与静态库、动态库
target_link_libraries(sdk_tester ${TARGET_LIB_NAME})
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

# 所有 .h 文件将被复制到 "include" 目录，但 "internal" 目录将被排除
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include
        DESTINATION .
        FILES_MATCHING PATTERN "*.h"    # PATTERN "*.hpp"
        PATTERN "internal" EXCLUDE)     # EXCLUDE PATTERN "internal/*"
# install(DIRECTORY  include DESTINATION .)
# 安装配置文件和README文件
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

#PROGRAMS：安装可执行文件到指定目录。
#install(PROGRAMS /path/to/executable DESTINATION /path/to/destination)

#RUNTIME_DEPENDENCY_SET：将指定目标的运行时依赖项作为依赖项集合进行安装。
#install(RUNTIME_DEPENDENCY_SET sdk_tester DESTINATION ${CMAKE_INSTALL_BINDIR})


install(CODE "message(STATUS 'Installing...')")
# 打印安装路径
message(STATUS "Install path: ${CMAKE_INSTALL_PREFIX}")


#if(${COMPILER_ARCH} STREQUAL "AARCH64")
#    install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/module/
#            DESTINATION ${CMAKE_SOURCE_DIR}/Release/include/aarch64-journey
#            FILES_MATCHING PATTERN "*.h")
#elseif(${COMPILER_ARCH} STREQUAL "X86_64")
#    install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/module/
#            DESTINATION ${CMAKE_SOURCE_DIR}/Release/include/ubuntu
#            FILES_MATCHING PATTERN "*.h")
#endif()

#if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#    # Linux 平台的安装规则 (Installation rules for Linux)
#    install(TARGETS myapp RUNTIME DESTINATION bin/linux)
#    install(FILES config_linux.conf DESTINATION etc)
#elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#    # Windows 平台的安装规则 (Installation rules for Windows)
#    install(TARGETS myapp RUNTIME DESTINATION bin/windows)
#    install(FILES config_windows.conf DESTINATION etc)
#endif()

message(STATUS "CMAKE_INSTALL_PREFIX:${CMAKE_INSTALL_PREFIX}")