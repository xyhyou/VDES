# 开发交接文档 (Context Handoff)

> 最后更新：2026-06-29

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

> **[!IMPORTANT]**
> The following rules are extracted from workspace configuration and MUST be followed
> unconditionally on every code edit. No exceptions.

* **文件编码约束**: 
  - 所有修改后的 `.h` 和 `.cpp` 文件必须保存为 **带 UTF-8 BOM 且 CRLF 换行符** 的格式。这是为了解决 MSVC 在解析中文字符时的编译问题。
  - 在完成任何文件修改后，请使用 Python 脚本 `C:\Users\Administrator\.gemini\antigravity-ide\brain\02f6e1a0-193c-46a0-8975-54285840e6e2\scratch\fix_bom.py` 对修改的文件进行格式规范化。
* **Pimpl 限制**:
  - 不要对含有 `std::unique_ptr<Impl>` 成员的类（如 `BoundingBox`）做直接拷贝赋值。应通过指针包装或构造手段绕过其已被删除的拷贝赋值运算符。

---

## 1. 项目概述
* **技术栈**: C++14/C++17, CMake 编译, SQLite (带 R-Tree 模块地理信息引擎), spdlog 日志, fmt 格式化工具。
* **目标平台**: Windows 平台，以 MSVC 编译器为主。

## 2. 当前开发阶段
目前项目正在按照新版国标，对各种 VDES 协议（FI = 31 到 44）进行全面的多态几何功能与时间规则升级。当前已经完成了对 **碍航物 (FI=35)**、**水上水下活动 (FI=36)**、**海上拖带 (FI=37)**、**军事活动 (FI=38)** 的升级工作，接下来将要对接其他新协议及用户发起的具体业务。

## 3. 已完成工作
### 3.1 军事活动业务 (FI=38) 升级
- **解码升级**: 修正了解析中遗漏的 1-bit `时间类型`（导致移位）问题，实现了对圆形、扇形、多边形、折线 4 种类型报文的完整解析提取。
- **持久化升级**: 在数据库初始化中自适应升级添加了几何、扇形角度、时间类型列。在保存和提取军事活动时，能够计算并更新其空间虚表索引。
- **空间点选判定**: 更新了在地图上通过点坐标与距离提取周边军事活动物体的算法。
- **核心涉及文件**：
  - [Headers/ASMDTD.h](file:///e:/VDES/Headers/ASMDTD.h) — 升级通用电文结构
  - [Headers/VDESDTD.h](file:///e:/VDES/Headers/VDESDTD.h) — 升级业务层实体结构
  - [Sources/ASMManager.cpp](file:///e:/VDES/Sources/ASMManager.cpp) — 实现 4 种几何流的安全解码
  - [Sources/VDES.cpp](file:///e:/VDES/Sources/VDES.cpp) — 自适应数据库初始化、Save、Load 及全几何点选 Pick 逻辑

---

## 4. 路由与入口注册
- 所有解析入口注册在 [Sources/ASMManager.cpp](file:///e:/VDES/Sources/ASMManager.cpp) 的 `Parse` 方法及内部映射路由中。
- 所有获取与点选对外 API 注册在 [Headers/VDES.h](file:///e:/VDES/Headers/VDES.h) 及 [Sources/VDES.cpp](file:///e:/VDES/Sources/VDES.cpp) 的 `VDESManager` 接口中。

## 5. 已修改/新增的文件清单
- [Headers/ASMDTD.h](file:///e:/VDES/Headers/ASMDTD.h) — 升级 `ASM_DAC_412_FI_38` 实体
- [Headers/VDESDTD.h](file:///e:/VDES/Headers/VDESDTD.h) — 扩展 `MSIMilitaryActivity` 实体
- [Sources/ASMManager.cpp](file:///e:/VDES/Sources/ASMManager.cpp) — 重新实现 `ParseASMDAC412FI38` 解码器
- [Sources/VDES.cpp](file:///e:/VDES/Sources/VDES.cpp) — 更新数据库读写、R-Tree 包围盒索引构建和空间点选算法
- [VDESDemo/VDESDemo.cpp](file:///e:/VDES/VDESDemo/VDESDemo.cpp) — 重写 `GenerateDAC_412_FI_38` 报文模拟，并加入 3 种不同几何的模拟回读全覆盖用例

## 6. 关键设计决策与注意事项
- R-Tree 包围盒中，圆形与扇形以中心为基准向外辐射 range 扩展，多边形取外包络框。
- 对 Pimpl 类在更新赋值时，声明并实例化 `std::unique_ptr<BoundingBox>` 来操作它。

## 7. 下一步计划 (Next Steps)
1. 在新会话启动后，读取所有项目规范与 Memory 文件。
2. 保持与用户的沟通，了解需要对哪些新版业务协议（如其他未完善的 FI=31/32/33/34/41/43/44 业务或特定的交互流程）做进一步重构开发。
