# Shrinkly

一个简单高效的 PNG 图片压缩桌面工具，支持 macOS 和 Windows。

## 功能特性

- 🎯 **简单易用**：拖拽或选择 PNG 图片即可压缩
- 🚀 **高效压缩**：使用 pngquant 引擎，压缩率高且质量好
- 💻 **跨平台**：支持 macOS 和 Windows
- 🔒 **本地处理**：所有压缩操作在本地完成，保护隐私
- 📦 **批量处理**：支持同时压缩多个文件

## MVP 版本说明

这是 Shrinkly 的 MVP（最小可行产品）版本，专注于核心功能：

### 支持的功能

- PNG 图片压缩（固定参数：quality=65-80）
- 文件拖拽和选择
- 批量处理
- 压缩结果反馈（文件大小对比）

### 暂不支持

- 图片预览
- 压缩参数调节
- 历史记录
- 多格式支持（仅支持 PNG）
- 多引擎选择（仅 pngquant）
- 用户登录/云端功能

## 系统要求

- **操作系统**：macOS 10.13+ 或 Windows 10+
- **Qt 版本**：Qt 5.15+ 或 Qt 6.x
- **CMake**：3.16+
- **编译器**：支持 C++17 的编译器

## 构建说明

### 前置准备

1. 安装 Qt（推荐使用 Qt 6.x）
2. 安装 CMake
3. 下载 pngquant 可执行文件：
   - macOS: 下载 pngquant 并放置到 `resources/pngquant/mac/pngquant`
   - Windows: 下载 pngquant.exe 并放置到 `resources/pngquant/win/pngquant.exe`

### macOS

```bash
# 确保 pngquant 有执行权限
chmod +x resources/pngquant/mac/pngquant

# 创建构建目录
mkdir build && cd build

# 配置（替换为你的 Qt 路径）
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/macos

# 编译
cmake --build .

# 运行
./bin/Shrinkly.app/Contents/MacOS/Shrinkly
```

### Windows

```bash
# 创建构建目录
mkdir build
cd build

# 配置（替换为你的 Qt 路径）
cmake .. -DCMAKE_PREFIX_PATH=C:/Qt/6.x/msvc2019_64

# 编译
cmake --build . --config Release

# 运行
.\bin\Release\Shrinkly.exe
```

### Linux

```bash
# 创建构建目录
mkdir build && cd build

# 配置
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64

# 编译
cmake --build .

# 运行
./bin/Shrinkly
```

## 使用说明

1. **添加文件**：
   - 拖拽 PNG 图片到窗口中央的拖放区域
   - 或点击"选择文件"按钮选择文件

2. **开始压缩**：
   - 点击"开始压缩"按钮
   - 等待压缩完成

3. **查看结果**：
   - 压缩后的文件保存在原目录
   - 文件名格式：`原文件名-min.png`
   - 如果已存在同名文件，会自动覆盖

## 项目结构

```
Shrinkly/
├── CMakeLists.txt          # CMake 配置文件
├── app/                    # 应用层
│   ├── main.cpp           # 程序入口
│   ├── MainWindow.h       # 主窗口头文件
│   └── MainWindow.cpp     # 主窗口实现
├── core/                   # 核心逻辑
│   ├── ImageTask.h        # 任务数据结构
│   ├── PngQuantRunner.h   # pngquant 调用封装
│   └── PngQuantRunner.cpp
├── resources/              # 资源文件
│   └── pngquant/          # pngquant 可执行文件
│       ├── mac/
│       └── win/
└── README.md              # 本文件
```

## 压缩参数说明

当前 MVP 版本使用固定的 pngquant 参数：

- `--quality=65-80`：质量范围 65-80
- `--speed 3`：速度等级 3（平衡速度和质量）
- `--force`：强制覆盖已存在的文件
- `--skip-if-larger`：如果压缩后更大则跳过

## FAQ

**Q: 压缩后的图片质量如何？**
A: 使用 quality=65-80 参数，肉眼几乎看不出差异，但文件大小通常能减少 50-70%。

**Q: 为什么有些图片压缩失败？**
A: 可能的原因：
- 文件损坏或格式不正确
- 压缩后文件更大（会自动跳过）
- 文件权限问题

**Q: 压缩会覆盖原文件吗？**
A: 不会，压缩后的文件会保存为 `原文件名-min.png`，原文件保持不变。

**Q: 支持其他格式吗？**
A: MVP 版本仅支持 PNG 格式，未来版本会考虑支持 JPG、WebP 等格式。

## 获取 pngquant

- macOS: `brew install pngquant`
- Windows: 从 [pngquant官网](https://pngquant.org/) 下载
- Linux: `apt-get install pngquant` 或 `yum install pngquant`

下载后将可执行文件放到对应的 `resources/pngquant/` 目录。

## 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。

## 贡献

欢迎提交 Issue 和 Pull Request！

## 路线图

未来版本计划：

- [ ] 支持更多图片格式（JPG, WebP）
- [ ] 可调节的压缩参数
- [ ] 图片预览功能
- [ ] 压缩历史记录
- [ ] 批量重命名
- [ ] 多语言支持
