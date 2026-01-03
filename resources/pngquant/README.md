# pngquant 可执行文件说明

本目录用于存放 pngquant 可执行文件。

## 获取 pngquant

### macOS

**方法1：使用 Homebrew**
```bash
brew install pngquant
cp $(which pngquant) ./mac/pngquant
```

**方法2：手动下载**
从 [pngquant 官网](https://pngquant.org/) 下载 macOS 版本，解压后将 `pngquant` 文件复制到 `./mac/` 目录。

**重要：设置执行权限**
```bash
chmod +x ./mac/pngquant
```

### Windows

**下载地址**
从 [pngquant 官网](https://pngquant.org/) 下载 Windows 版本。

**安装步骤**
1. 下载 `pngquant-windows.zip`
2. 解压文件
3. 将 `pngquant.exe` 复制到 `./win/` 目录

### Linux

**方法1：包管理器**
```bash
# Ubuntu/Debian
sudo apt-get install pngquant
cp $(which pngquant) ./linux/pngquant

# CentOS/RHEL
sudo yum install pngquant
cp $(which pngquant) ./linux/pngquant
```

**方法2：手动编译**
```bash
git clone https://github.com/kornelski/pngquant.git
cd pngquant
make
cp pngquant ../linux/pngquant
```

## 目录结构

```
resources/pngquant/
├── README.md           # 本文件
├── mac/
│   └── pngquant       # macOS 可执行文件
├── win/
│   └── pngquant.exe   # Windows 可执行文件
└── linux/
    └── pngquant       # Linux 可执行文件
```

## 验证安装

确认文件已正确放置：

**macOS**
```bash
ls -la ./mac/pngquant
# 应该显示可执行权限 -rwxr-xr-x
```

**Windows**
```bash
dir .\win\pngquant.exe
```

**Linux**
```bash
ls -la ./linux/pngquant
# 应该显示可执行权限 -rwxr-xr-x
```

## 注意事项

1. **权限问题**（macOS/Linux）：确保 pngquant 文件有执行权限
2. **版本要求**：建议使用 pngquant 2.12.0 或更高版本
3. **Git 跟踪**：默认情况下，pngquant 可执行文件不会被 git 跟踪（在 .gitignore 中）

## 许可证

pngquant 使用 GPL v3 或更高版本许可证。
详见：https://github.com/kornelski/pngquant
