@echo off

REM 设置项目路径和构建路径
set PROJECT_DIR=%cd%
set BUILD_DIR=%PROJECT_DIR%\build

REM 检查构建目录是否存在，如果存在则清空目录
if exist "%BUILD_DIR%" (
    echo 清空构建目录...
    rd /s /q "%BUILD_DIR%"
)

REM 创建构建目录
mkdir "%BUILD_DIR%"

REM 进入构建目录
pushd "%BUILD_DIR%"

REM 运行 CMake 生成项目文件
echo 生成项目文件...
cmake -DCMAKE_BUILD_TYPE=Release "%PROJECT_DIR%" 

REM 编译项目并将日志保存到文件
echo 编译项目...
cmake --build .  --config Release 

REM 安装项目到指定目录
cmake --install %BUILD_PATH%

REM 退出构建目录
popd

echo 编译完成。

