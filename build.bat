@echo off
REM Shrinkly 构建脚本 (Windows)

echo ======================================
echo    Shrinkly 构建脚本
echo ======================================
echo.

REM 检查是否指定了Qt路径
if "%1"=="" (
    echo 用法: build.bat C:\Qt\6.x\msvc2019_64
    echo 示例: build.bat C:\Qt\6.5.0\msvc2019_64
    echo.
    echo 或者设置环境变量 CMAKE_PREFIX_PATH:
    echo set CMAKE_PREFIX_PATH=C:\Qt\6.x\msvc2019_64
    echo build.bat use-env
    exit /b 1
)

if "%1"=="use-env" (
    if "%CMAKE_PREFIX_PATH%"=="" (
        echo 错误: CMAKE_PREFIX_PATH 环境变量未设置
        exit /b 1
    )
    set QT_PATH=%CMAKE_PREFIX_PATH%
) else (
    set QT_PATH=%1
)

REM 检查Qt路径是否存在
if not exist "%QT_PATH%" (
    echo 错误: Qt路径不存在: %QT_PATH%
    exit /b 1
)

echo Qt 路径: %QT_PATH%
echo.

REM 检查pngquant
if not exist "resources\pngquant\win\pngquant.exe" (
    echo 警告: 未找到 pngquant.exe
    echo 请参考 resources\pngquant\README.md 获取 pngquant
    echo.
    set /p CONTINUE="是否继续构建? (y/n): "
    if /i not "%CONTINUE%"=="y" exit /b 1
)

REM 创建并进入构建目录
echo 创建构建目录...
if not exist build mkdir build
cd build

REM 运行CMake配置
echo 运行 CMake 配置...
cmake .. -DCMAKE_PREFIX_PATH="%QT_PATH%" -DCMAKE_BUILD_TYPE=Release

REM 编译
echo 开始编译...
cmake --build . --config Release

echo.
echo ======================================
echo    构建成功!
echo ======================================
echo.
echo 运行应用: .\build\bin\Release\Shrinkly.exe
echo.

cd ..
