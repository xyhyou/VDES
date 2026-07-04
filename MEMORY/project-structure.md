# Project Structure & Build Configuration
> Last updated: 2026-06-29

## 1. 工程目录结构
```
e:\VDES\
├── Headers\                  ← 库公共头文件目录
│   ├── ASMDTD.h             ← 核心结构定义（AIS ASM 数据传输对象定义）
│   ├── VDES.h               ← 库主导出入口声明（VDESManager 单例等）
│   ├── VDESDTD.h            ← 业务实体结构定义 (MSI, Distress, AtoN, Towing)
│   ├── BoundingBox.h        ← 地理空间包围盒工具声明（包含 Pimpl 实现）
│   └── GeoCalculator.h      ← 地球球体与平面距离计算辅助工具
├── Sources\                  ← 库实现文件目录
│   ├── ASMManager.cpp       ← ASM 电文解码器逻辑 (ParseASMDAC412FIxx 系列方法)
│   ├── VDES.cpp             ← 主管理器与 SQLite DB 存取及 BBox 计算存储核心
│   └── BitsManager.cpp      ← AIS 报文比特流读写器实现
├── VDESDemo\                 ← 测试演示模块
│   └── VDESDemo.cpp         ← 主演示程序入口，各 FI 电文模拟报文生成与解析流测试
└── CMakeLists.txt            ← 主 CMake 构建描述文件
```

## 2. 构建与运行环境
* **语言标准**: C++14/C++17。
* **编译器**: MSVC (Visual Studio 2022 / Visual Studio Build Tools)。
* **CMake 预设**: 工程根目录下使用 `out/build/win-x86-debug` 目标进行 Debug 调试与测试。
* **日志系统**: 使用 `spdlog`（输出包括 trace/debug/info 的解析调试包）。
* **数据库依赖**: SQLiteCpp、内部轻量封装。运行后会产生 `VDES.db` 进行本地离线持久化。
