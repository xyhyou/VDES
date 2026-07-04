# Project Memory Index
> Last updated: 2026-06-29

## Files
- [project-status.md](./project-status.md) — 运行与数据库状态、已测试的业务流程（FI=31/32/33/34/35/36/37/38/41/43/44）
- [project-structure.md](./project-structure.md) — 工程目录结构、构建配置及发布约定
- [design-decisions.md](./design-decisions.md) — 重要设计决策（如 R-Tree BBox Bounding Box 适配、BOM-UTF8 编译器兼容决策）
- [user-preferences.md](./user-preferences.md) — AI 协作与代码规范约定（如 Pimpl unique_ptr 限制）

## Quick Summary
本工程基于 C++14/C++17 实现了高性能海事 VDES 报文解析与业务持久化层（基于 SQLite + R-Tree 空间包围盒）。
当前正在推进新标业务协议的升级重构（已成功完成 FI=31/32/33/34/35/36/37/38/41/43/44 的全几何区域和属性升级）。
下一步需要开始对 FI=35（碍航物）/ 36（水上水下活动）/ 37（海上拖带）/ 38（军事活动）之外的其他新标报文和业务指令进行适配与对接。
