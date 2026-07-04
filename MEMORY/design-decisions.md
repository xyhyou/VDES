# Key Design Decisions
> Last updated: 2026-06-29

## 1. 文件的 BOM 标志与 CRLF 换行符约束 (Crucial for MSVC)
* **背景**: MSVC 编译器在解析包含非 ASCII 字符（如中文注释）且没有 UTF-8 BOM 标志的文件时，易报中文编译解析错（如 `C2001: 常量中有换行符`）。
* **决策**:
  - 头文件与源文件在保存/修改后，必须保存为 **带 UTF-8 BOM 标志且换行符为 CRLF** 的格式。
  - 项目提供了一个 `fix_bom.py` 的脚本在 `artifacts/` / `scratch/` 中维护。在修改了代码文件后，必须运行此脚本对文件格式进行自动规范化，避免编译爆红。

## 2. 空间虚表与 R-Tree 的 BBox 点选判定
* **背景**: 离线地图系统点选障碍物或军事活动区时，系统需要能够根据点坐标及选择距离，智能提取周边多形态几何物体。
* **决策**:
  - 圆形与扇形：以中心点为圆心，向四周包围半径扩展构建 BBox 写入 SQLite R-Tree 虚表。
  - 多边形与折线：取所有顶点的最大/最小经纬度包围范围构建 BBox 写入虚表。
  - 点选过滤算法：回读比对时，对于圆形/扇形，使用 `GeoCalculator::Distance` 计算到圆心的绝对距离；对于多边形，使用 `GeoCalculator::GetDistanceToPolygon`；对于折线/多点，使用 `GeoCalculator::GetDistanceToLineSegment` 计算至各线段的最小包围距。

## 3. Pimpl 指针与 `unique_ptr` 拷贝语义限制
* **背景**: 工程中部分工具类（如 `BoundingBox`）使用了 Pimpl (Private Implementation) 模式，并把其实现类的指针存放在 `std::unique_ptr<Impl>` 智能指针成员中。
* **决策**:
  - 这导致此类默认隐式生成的 **拷贝赋值运算符 (operator=)** 会被删除。
  - 严禁对含有 deleted `operator=` 成员的类实例使用直接拷贝赋值表达式（如 `bbox = Build(...)`），应当通过指针实例化 `std::unique_ptr<BoundingBox>`、利用拷贝构造函数重新构造（如 `BoundingBox bbox(circleBBox);`）、或执行相应的成员数据更新（如 `bbox.Clear(); bbox.Update(...)`）。
