#!/bin/bash

# Shrinkly 构建脚本 (macOS/Linux)

set -e

echo "======================================"
echo "   Shrinkly 构建脚本"
echo "======================================"
echo ""

# 检查是否指定了Qt路径
if [ -z "$1" ]; then
    echo "用法: ./build.sh /path/to/Qt/6.x/gcc_64"
    echo "示例: ./build.sh ~/Qt/6.5.0/macos"
    echo ""
    echo "或者设置环境变量 CMAKE_PREFIX_PATH:"
    echo "export CMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64"
    echo "./build.sh"
    exit 1
fi

QT_PATH="$1"

# 检查Qt路径是否存在
if [ ! -d "$QT_PATH" ]; then
    echo "错误: Qt路径不存在: $QT_PATH"
    exit 1
fi

echo "Qt 路径: $QT_PATH"
echo ""

# 检查pngquant
if [[ "$OSTYPE" == "darwin"* ]]; then
    PNGQUANT_PATH="resources/pngquant/mac/pngquant"
else
    PNGQUANT_PATH="resources/pngquant/linux/pngquant"
fi

if [ ! -f "$PNGQUANT_PATH" ]; then
    echo "警告: 未找到 pngquant 可执行文件: $PNGQUANT_PATH"
    echo "请参考 resources/pngquant/README.md 获取 pngquant"
    echo ""
    read -p "是否继续构建? (y/n) " -n 1 -r
    echo ""
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# 创建并进入构建目录
echo "创建构建目录..."
mkdir -p build
cd build

# 运行CMake配置
echo "运行 CMake 配置..."
cmake .. -DCMAKE_PREFIX_PATH="$QT_PATH" -DCMAKE_BUILD_TYPE=Release

# 编译
echo "开始编译..."
cmake --build . --config Release

echo ""
echo "======================================"
echo "   构建成功!"
echo "======================================"
echo ""

if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "运行应用: ./build/bin/Shrinkly.app/Contents/MacOS/Shrinkly"
else
    echo "运行应用: ./build/bin/Shrinkly"
fi
echo ""
