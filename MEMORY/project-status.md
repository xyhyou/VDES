# Project Status & Database Reference
> Last updated: 2026-06-29

## 数据库表与索引映射
本工程的核心业务使用 SQLite 数据库管理，且集成了 SQLite R-Tree 模块支持地理空间范围 BBox 点选与过滤。各业务升级后的关键数据库设计如下：

* **MSIObstacle / MSIObstacleBBox** (FI=35 碍航物):
  - 存储阻碍航行事件（如沉船、浅滩等）。
  - 支持多态几何（0: 圆形, 1: 扇形, 2: 多边形, 3: 折线）。
* **MSIMaritimeOperation / MSIMaritimeOperationBBox** (FI=36 水上水下活动):
  - 存储水上水下施工、演习、疏浚等活动。
  - 增加了 `GeometryType`、`SectorStartAngle`、`SectorEndAngle`、`Polygon` (Blob)、`TimeType`、`Caution` 等列。
* **MSIMaritimeTowing / MSIMaritimeTowingBBox** (FI=37 海上拖带):
  - 存储大型海上拖带事件。
  - 包含 `MMSI`、起点/终点坐标、拖带船组总长度/总宽度、最大航速等。
* **MSIMilitaryActivity / MSIMilitaryActivityBBox** (FI=38 军事活动):
  - 存储军事演习与活动区域。
  - 支持多态几何与 `TimeType` (连续/间歇) 自适应 BBox，修正了原本遗漏 1-bit 时间类型标志的错误。
* **ChannelBoundary** (FI=43 航道边线):
  - 存储航路/航道的两侧边线路径。
  - 包含 `MRN`、`Fragment`、`EdgeType` (左/右边线)、多点坐标 BLOB。
* **NetSounder** (FI=44 渔网标识/网位仪):
  - 存储渔网位置与网位仪状态.
  - 包含 `MRN`、`Fragment`、`Type` (渔网类型)、`IsContinous` (独立/连续关联标志)、多点坐标 BLOB.
* **FrontendPrompt** (DAC=413, FI=5 前端提示文字):
  - 存储前端海图滚动警告文字，支持生命周期自适应到期过滤与手动删除。
  - 包含 `PromptContent` (提示内容类型)、`DisplayDuration` (显示时长，分钟数)、`Text` (警告文本)、`Timestamp Receive` (接收时间戳)、`Timestamp Expiry` (到期时间戳)。

## 历史解析与已覆盖测试用例
演示程序 `VDESDemo/VDESDemo.cpp` 包含以下成熟测试：
* `GenerateDAC_412_FI_26` / `27` / `28` / `29` / `30` — 气象与环保信息（点、线、多边形）
* `GenerateDAC_412_FI_31` — 气象与环保预警信息 (WarningType 1/2/5/6)
* `GenerateDAC_412_FI_32` — 潮汐预报 (Tide Forecast)
* `GenerateDAC_412_FI_33` / `34` — 实体航标 / AIS航标动态信息
* `GenerateDAC_412_FI_35` — 碍航物 (全几何类型)
* `GenerateDAC_412_FI_36` — 水上水下活动 (全几何类型)
* `GenerateDAC_412_FI_37` — 海上拖带
* `GenerateDAC_412_FI_38` — 军事活动 (全几何类型)
* `GenerateDAC_412_FI_41` — 桥梁桥孔通道信息
* `GenerateDAC_412_FI_42` — 航道中线信息
* `GenerateDAC_412_FI_43` — 航道边线信息
* `GenerateDAC_412_FI_44` — 网位仪信息
