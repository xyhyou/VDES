# 开发交接文档 (Context Handoff)

> 最后更新：2026-07-18

> [!IMPORTANT]
> ## 🚨 新会话必须在回复用户之前完成以下操作（不得跳过）
>
> 在处理任何用户请求之前，**立即**按顺序执行以下读取操作：
>
> - [ ] **Step A**: 用 `view_file` 读取 `MEMORY/MEMORY.md`（项目 Memory 索引）
> - [ ] **Step B**: 用 `view_file` 读取 `MEMORY/project-status.md`（运维补充信息）
> - [ ] **Step C**: 用 `view_file` 读取 `MEMORY/project-structure.md`（工程目录结构与发布机制）
> - [ ] **Step D**: 用 `view_file` 读取 `MEMORY/design-decisions.md`（关键设计决策）
> - [ ] **Step E**: 用 `view_file` 读取 `MEMORY/user-preferences.md`（用户工作偏好）
>
> 以上文件包含了**工程结构、配置路径、设计约定**等无法从代码直接推导的关键上下文。
> 不读取这些文件将导致你对工程状态一无所知，给出错误建议。

---

## 0. 项目强制规范 (MANDATORY RULES — Read First)

> [!IMPORTANT]
> The following rules MUST be followed unconditionally on every code edit. No exceptions.
- **文件编码规范**：所有修改或新增的源文件和头文件必须为 **UTF-8 with BOM** 格式，否则 MSVC 编译器会报 C4819 编码字符警告。
- **编译参数规范**：CMake 必须使用 `/utf-8` 编译选项，从而强制 MSVC 编译器以 Unicode 解析包含中文原始字面量的源文件。已在顶级 `CMakeLists.txt` 中配置为 `add_compile_options(/utf-8)`。
- **发布规范**：对外接口声明需同步修改 `Headers/VDES.h`、`Headers/VDESDTD.h` 以及发布目录 `Publish/include/VDES.h`、`Publish/include/VDESDTD.h`，确保对外的 SDK 接口一致。
- **构建规范**：必须在 Visual Studio Developer Command Prompt 环境下构建，因为 ASan 动态运行时库依赖 `clang_rt.asan_dynamic-i386.dll`，普通命令行运行程序会因环境缺失默默退出（Exit Code 1）。
- **锁与多线程规范**：访问数据库或共享状态时必须使用对应的 Mutex 锁（如 `m_mutexChannelBoundary`），保持线程安全。

---

## 1. 项目概述
- **技术栈**：C++11，CMake 构建，SQLite3 (SQLiteCpp)，eventpp，spdlog，Visual Studio 2022。
- **目标平台**：Windows (x86-debug / 32-bit)。
- **测试程序**：`out\build\x86-debug\VDESDemo\VDESDemo.exe`。
- **底层数据库**：本地生成在工作区根目录的 `vdes.db`。

---

## 2. 当前开发阶段
已完成以下阶段的所有重构与升级开发：
1. **气象/环境预警信息重构**（Mew Types 1-6 独立建表与空间索引）。
2. **AIS 助航标志动态（FI=34）报文解析对齐**（去除首部 MRN 并缩减偏移量，引入 2-bit 片段描述）。
3. **桥梁通航孔（FI=41）结构重构**（桥梁唯一，通航孔以 BLOB 数组归并存取）。
4. **航道边线（FI=43/44）双边合并与主键归并**（MRN 主键，左右坐标合并 BLOB）。
5. **网音响（FI=45）报文片段描述及 FI=8 补充文本更新**。
6. **航道中线/边线物理删除、补充描述文本字段（FI 8 关联更新）**。
7. **重载决策陷阱消除**（为航道中线、边线、非 AIS / AIS 航标动态等添加独立单条物理删除接口）。
8. **ABB 短信报文解析机制 (FI=3 / FI=4)**：
   - 实现了对 NMEA 多语句 `!AIABB` 报文的接收拼包缓冲、解析及写入收件箱数据库（`MessageInbox`）。
   - 实现了双向的 GBK 与 UTF-8 字符转码。
   - 分离了 `FI=4` 与 `FI=2` 的解码分支，解除了对前者的 952-bit (AIS Message 8) 载荷长度限制。

---

## 3. 已完成工作

### 3.1 航道中线与边线删除接口及重载问题修复
- 实现了单条删除接口 `DeleteChannelCenterline` 和 `DeleteChannelBoundary` 物理删除机制，解决了 C++ 重载决策的匹配 Bug 陷阱。
- **核心涉及文件**：`Headers/VDES.h`, `Publish/include/VDES.h`, `Sources/VDES.cpp`

### 3.2 助航标志动态单条删除接口补全
- 补充了非 AIS 及 AIS 助航标志动态单条删除接口 `DeleteAtoNDynamic` 和 `DeleteAISAtoNDynamic`。
- **核心涉及文件**：`Headers/VDES.h`, `Publish/include/VDES.h`, `Sources/VDES.cpp`

### 3.3 航道中线与边线描述字段及 VDL 更新 (FI 8)
- 在对外结构体中引入 `std::string description;` 字段并维护在数据库中，实现了 `DAC 413, FI 8` 补充文本对于中线和边线描述的异步更新。
- **核心涉及文件**：`Headers/VDESDTD.h`, `Publish/include/VDESDTD.h`, `Sources/VDES.cpp`, `VDESDemo/VDESDemo.cpp`

### 3.4 增加 ABB 中英文短信语句拼包与解析 (FI=3 / FI=4)
- 实现了 `VDESManager::Impl::ParseABB` 用于识别拼装多语句 `!AIABB`；
- 解码后的 GBK 文本经过 `UtilityInterface::GBKToUTF8` 转换成 UTF-8；
- 数据最终持久化至本地 `MessageInbox` 数据库，并带上了发送源 MMSI；
- **核心涉及文件**：`Sources/VDES.cpp`, `Sources/ASMManager.cpp`, `Headers/VDES.h`

### 3.5 13位中文解码分支解耦 (FI=4)
- 为 `FI=4` 创建了单独的 13位解码逻辑分支，完全摆脱了针对 AIS Message 8 设计的 952-bit 缓冲大小映射检查，支持 ASM 格式下的大负载数据传输。
- **核心涉及文件**：`Sources/AISBitsManager.cpp`

---

## 4. 路由与入口注册
- 所有解析入口与事件总线挂载位于 `Sources/VDES.cpp` 内的 `VDESManager::Impl::HandlePayload` 及 `HandleASMMessage`。
- 新增的 `!AIABB` NMEA 语句解析绑定在 `m_parsersMap` 中以 `Impl::ParseABB` 入口分发。
- 解析后的数据存取通过 `VDESManager::Impl::SaveChannelBoundary` 等方法实现，更新后通过 `notifyEvent` 向界面/应用层分发回调。

---

## 5. 已修改/新增的文件清单

### 本阶段修改文件
- `CMakeLists.txt` — 为 MSVC 增加 `/utf-8` 编译参数，解决中文源码常量跨平台对齐故障。
- `Headers/VDES.h` / `Publish/include/VDES.h` — 声明物理删除接口。
- `Headers/VDESDTD.h` / `Publish/include/VDESDTD.h` — 结构体扩展 `description` 描述信息字段。
- `Sources/VDES.cpp` — 实现了对中线和边线描述合并读取、FI 8 更新分发、单条删除逻辑和 `ParseABB` 事件拼包入口。
- `Sources/ASMManager.cpp` — 实现了对 `FI=3` 和 `FI=4` 中英文长短信的解码。
- `Sources/AISBitsManager.cpp` — 实现了 14位/13位 中英文编码器，并对 `FI=4` 的 13位中文解码算法进行了解耦优化（去除 952-bit 校验）。
- `VDESDemo/VDESDemo.cpp` — 补充了测试中英文短信语句收发与入库的测试用例 `TestABBTextMessages()`。

### 本阶段未修改（但相关的文件）
- `chinese_encoding_limitations.md` — 详细阐述了 14位中文编码对于中文全角符号不兼容导致下溢的技术原理。

---

## 6. 关键设计决策与注意事项
- **MRN 主键**：`ChannelBoundary` 和 `AISAtoNDynamics` 等表的物理主键并非自增 `ID`，因此删除时必须精确匹配到 `MRN` 列，不可以套用通用 `ID` 列删除。
- **保留已收到的描述**：由于 VDES 空中链路报文是无序到达的，可能出现补充描述（FI 8）先于主要位置信息到达的情况。我们在解析保存中线/边线时，会先执行 Select 获取原有的 `Description`，防止新到达的位置信息在 overwrite 时抹除掉旧描述。
- **中文标点处理**：因为 14位中文编码在解析前9区（包含冒号、逗号、句号）时在数学公式上会引发**整数下溢（Underflow）**，导致乱码。因此在使用 14位编码 (FI=3) 发送前，应将所有全角标点统一转为半角；而 13位编码 (FI=4) 具备专属映射偏移（$+3700$），可直接支持全角标点传输。

---

## 7. 下一步计划 (Next Steps)
1. **进行业务稳定性与并发压力测试**：在启用 ASan 的环境下运行高频流式报文解析，观察是否存在多线程锁竞争导致数据库锁（Database locked）异常。
2. **交付对外接口集成**：上层系统可安全对接 `DeleteChannelCenterline` / `DeleteChannelBoundary` 等物理删除接口以实现 UI 的精确控制。
