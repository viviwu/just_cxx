@echo off

REM 设置项目路径和构建路径
set PROJECT_PATH=C:\path\to\project
set BUILD_PATH=%PROJECT_PATH%\build

REM 设置安装路径
set INSTALL_PATH=%PROJECT_PATH%\install

REM 创建构建目录
mkdir %BUILD_PATH%
cd %BUILD_PATH%

REM 运行CMake配置，指定构建目录和安装路径
cmake -S "%PROJECT_PATH%" -B "%BUILD_PATH%" -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%"

REM 编译项目
cmake --build %BUILD_PATH%

REM 安装项目到指定目录
cmake --install %BUILD_PATH%

REM 清理构建目录
cd %PROJECT_PATH%
rmdir /s /q %BUILD_PATH%
