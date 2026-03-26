# WebCAD 项目流程图与模块说明

> 本文档描述 WebCAD 项目的整体架构、各模块功能及数据流转流程。
> WebCAD 是一个基于 C++11 编写、通过 Emscripten 编译为 WebAssembly（WASM）的跨平台 CAD（计算机辅助设计）内核。

---

## 目录

1. [项目概述](#1-项目概述)
2. [整体架构图](#2-整体架构图)
3. [模块详解](#3-模块详解)
   - [Kernel（核心）](#31-kernel核心)
   - [Ac（基础工具）](#32-ac基础工具)
   - [Ge（几何引擎）](#33-ge几何引擎)
   - [Rx（运行时/反射系统）](#34-rx运行时反射系统)
   - [Ev（事件系统）](#35-ev事件系统)
   - [Ap（应用/文档管理）](#36-ap应用文档管理)
   - [Db（数据库层）](#37-db数据库层)
   - [Gi（图形渲染）](#38-gi图形渲染)
   - [Gs（图形元素）](#39-gs图形元素)
   - [Re（渲染器管理）](#310-re渲染器管理)
   - [Ed（编辑器/命令）](#311-ed编辑器命令)
   - [Pr（解析器）](#312-pr解析器)
   - [Emscripten（JS绑定层）](#313-emscriptenjs绑定层)
4. [核心数据流程](#4-核心数据流程)
   - [系统初始化流程](#41-系统初始化流程)
   - [DXF文件加载流程](#42-dxf文件加载流程)
   - [渲染流程](#43-渲染流程)
   - [用户输入与命令执行流程](#44-用户输入与命令执行流程)
   - [对象序列化流程](#45-对象序列化流程)
5. [模块依赖关系](#5-模块依赖关系)
6. [技术栈](#6-技术栈)
7. [构建系统](#7-构建系统)
8. [目录结构](#8-目录结构)

---

## 1. 项目概述

WebCAD 是一个功能完整的 CAD 内核，设计目标是在 Web 浏览器中运行。它的整体架构参考了 AutoCAD 的内部模型（ObjectARX），提供了以下核心能力：

| 能力 | 说明 |
|------|------|
| 二维/三维几何运算 | 点、线、弧、NURBS 曲线/曲面的精确计算与变换 |
| 数据库驱动对象模型 | 类似 AutoCAD 的图层、块、线型、样式等符号表管理 |
| DXF 文件解析 | 读取标准 CAD DXF 格式文件 |
| 图形渲染准备 | 几何数据转换为可供 WebGL 渲染的顶点数据 |
| 命令系统 | 可扩展的 CAD 命令注册与执行框架 |
| 用户交互 | 鼠标、触控、键盘事件统一处理 |
| WebAssembly 导出 | 通过 Emscripten Embind 将 200+ 个 C++ 类暴露给 JavaScript |

---

## 2. 整体架构图

```mermaid
graph TB
    subgraph JS["JavaScript / 浏览器层"]
        WebGL["WebGL 渲染器"]
        JSApp["JS 应用逻辑"]
        UserInput["用户输入事件\n(鼠标/触控/键盘)"]
    end

    subgraph WASM["WebAssembly 绑定层（Emscripten）"]
        Bind["Emscripten Embind\n导出 200+ C++ 类"]
    end

    subgraph Kernel["Kernel 内核单例"]
        KernelSingleton["kernel()\n全局内核实例\n持有所有 Manager 指针"]
    end

    subgraph Managers["Manager 子系统"]
        EvMgr["EvEventManager\n事件管理器"]
        RxMgr["RxManagerDictionary\n管理器注册字典"]
        ApMgr["ApDocManager\n文档管理器"]
        DbMgr["DbObjectManager\n数据库对象管理器"]
        GiEntityMgr["GiEntityManager\n图形实体管理器"]
        GiObjectMgr["GiObjectManager\n图形对象管理器"]
        GiMaterialMgr["GiMaterialManager\n材质管理器"]
        ReMgr["ReRendererManager\n渲染器管理器"]
        EdInputMgr["EdUserInputManager\n用户输入管理器"]
        EdCmdStack["EdCommandStack\n命令堆栈"]
        RxThreadMgr["RxWorkerThreadManager\n工作线程管理器"]
    end

    subgraph CoreModules["核心模块库"]
        Ac["Ac 基础工具\nAcString/AcArray/\nAcJsonObject/AcBinary"]
        Ge["Ge 几何引擎\n点/向量/矩阵/曲线/曲面"]
        Db["Db 数据库\n实体/符号表/序列化"]
        Gi["Gi 图形渲染\n实体绘制/材质/包围盒"]
        Ed["Ed 编辑器\n命令/输入/Jig"]
        Pr["Pr 解析器\nDXF 文件解析"]
        Re["Re 渲染器\n摄像机/视口"]
        Rx["Rx 运行时\n管理器/线程/变量"]
        Ev["Ev 事件\n事件管理/反应器"]
        Ap["Ap 应用\n文档生命周期"]
        Gs["Gs 图形元素\n元素/游标/坐标系"]
    end

    UserInput --> JSApp
    JSApp --> Bind
    WebGL --> JSApp
    Bind --> KernelSingleton
    KernelSingleton --> Managers
    Managers --> CoreModules
    CoreModules --> Ge
    Db --> Pr
    Gi --> Re
    Re --> WebGL
```

---

## 3. 模块详解

### 3.1 Kernel（核心）

**路径**: `kernel/src/kernel/kernel.cpp` | `kernel/Include/public/kernel.h`

Kernel 是整个系统的**单例中枢**，在程序启动时由 `acrxManagerDictionary()->instantiate()` 完成初始化。它持有所有子系统 Manager 的指针，是跨模块通信的唯一入口。

```mermaid
graph LR
    K["kernel()\n全局单例"] --> |持有指针| EM["EvEventManager"]
    K --> |持有指针| RM["RxManagerDictionary"]
    K --> |持有指针| AP["ApDocManager"]
    K --> |持有指针| DB["DbObjectManager"]
    K --> |持有指针| GI_E["GiEntityManager"]
    K --> |持有指针| GI_O["GiObjectManager"]
    K --> |持有指针| GI_M["GiMaterialManager"]
    K --> |持有指针| RE["ReRendererManager"]
    K --> |持有指针| ED_I["EdUserInputManager"]
    K --> |持有指针| ED_C["EdCommandStack"]
    K --> |持有指针| RX_T["RxWorkerThreadManager"]
    K --> |持有指针| GS["GsElementManager"]
```

**主要接口**：

| 函数 | 返回类型 | 说明 |
|------|----------|------|
| `kernel()->acevEventManager()` | `EvEventManager*` | 事件管理器 |
| `kernel()->acdbObjectManager()` | `DbObjectManager*` | 数据库对象管理器 |
| `kernel()->acgiEntityManager()` | `GiEntityManager*` | 图形实体管理器 |
| `kernel()->acreRendererManager()` | `ReRendererManager*` | 渲染器管理器 |
| `kernel()->acedUserInputManager()` | `EdUserInputManager*` | 用户输入管理器 |

---

### 3.2 Ac（基础工具）

**路径**: `kernel/src/Ac/` | `kernel/Include/public/Ac/`

提供整个系统所需的**基础数据结构与工具类**，被所有其他模块依赖。

```mermaid
graph TD
    AcString["AcString\nUTF-8 字符串管理\n支持 BOM 检测"]
    AcArray["AcArray<T>\n动态数组容器"]
    AcMap["AcMap<K,V>\n哈希映射"]
    AcJsonObject["AcJsonObject\nRapidJSON 封装\nJSON 解析与生成"]
    AcBinary["AcBinary\n二进制数据容器"]
    AcBinaryFiler["AcBinaryFiler\n文件 I/O 抽象"]
    AcEvent["AcEvent\n通用事件基类"]
    AcDebug["AcDebug\n调试输出工具"]
    AcSystem["AcSystem\n系统级 API\n定时器/文件选择/JSON"]
```

**核心功能**：
- `AcString`: 线程安全的字符串，支持 UTF-8、宽字符转换
- `AcJsonObject`: 对 RapidJSON 的轻量封装，用于配置和序列化
- `AcSystem::setInterval()`: 供 `main.cpp` 调用以驱动定时渲染
- `AcBinaryFiler`: 为 DWG 序列化提供文件读写抽象

---

### 3.3 Ge（几何引擎）

**路径**: `kernel/src/Ge/`（67个文件） | `kernel/Include/public/Ge/`

几何引擎是 CAD 核心中**最大的模块**，提供完整的二维/三维几何计算能力。

```mermaid
graph TB
    subgraph Primitives["基本几何类型"]
        Pt["GePoint2d / GePoint3d\n2D/3D 点"]
        Vec["GeVector2d / GeVector3d\n2D/3D 向量"]
        Mat["GeMatrix2d / GeMatrix3d\n变换矩阵"]
        Ext["GeExtents2d / GeExtents3d\n包围盒"]
        Quat["GeQuaternion\n四元数旋转"]
    end

    subgraph Curves["曲线类型"]
        Line["GeLine2d / GeLine3d\n直线（无限延伸）"]
        Ray["GeRay2d / GeRay3d\n射线"]
        LineSeg["GeLineSeg2d / GeLineSeg3d\n线段"]
        Arc["GeCircArc2d / GeCircArc3d\n圆弧"]
        Ellip["GeEllipArc2d / GeEllipArc3d\n椭圆弧"]
        Nurbs["GeNurbCurve2d / GeNurbCurve3d\nNURBS 曲线"]
        Spline["GeCubicSplineCurve2d/3d\n三次样条"]
        Poly["GePolyline2d / GePolyline3d\n折线"]
    end

    subgraph Surfaces["曲面类型"]
        NurbsSurf["GeNurbSurface\nNURBS 曲面"]
    end

    subgraph Intersections["相交计算"]
        CCI["GeCurveCurveInt2d/3d\n曲线-曲线相交"]
        SSI["GeSurfSurfInt\n曲面-曲面相交"]
    end

    Pt --> Mat
    Vec --> Mat
    Mat --> Curves
    Curves --> Intersections
    Surfaces --> Intersections
```

**模块职责**：
- 所有几何对象提供变换（`transformBy(GeMatrix3d)`）、最近点（`closestPointTo()`）等通用接口
- NURBS 曲线/曲面支持控制点、权重、节点向量的完整定义
- 交集计算返回参数化交点，支持多个相交结果
- 通过 `GeExport.cpp`（1440行）全量暴露给 JavaScript

---

### 3.4 Rx（运行时/反射系统）

**路径**: `kernel/src/Rx/`（18个文件） | `kernel/Include/public/Rx/`

提供系统级的**管理器注册、线程池和变量系统**。

```mermaid
graph TD
    RxMgrDict["RxManagerDictionary\n管理器注册字典\n调用 instantiate() 初始化所有 Manager"]
    RxManager["RxManager\n所有 Manager 的基类\n提供 init()/uninit() 生命周期"]
    RxSysVar["RxSystemVariable\n运行时变量 (如 ORTHOMODE)"]
    RxEnvVar["RxEnvironmentVariable\nOS 环境变量 (如路径)"]
    RxConfigVar["RxConfigurationVariable\n配置项存储"]
    RxSysReg["RxSysRegistry\n全局对象字典"]
    RxWorkerThread["RxWorkerThread\n工作线程封装"]
    RxWorkerThreadMgr["RxWorkerThreadManager\n线程池管理器"]
    RxAsyncMgr["RxAsyncManager\nWASM Asyncify 支持"]

    RxMgrDict --> RxManager
    RxManager --> RxSysVar
    RxManager --> RxEnvVar
    RxManager --> RxConfigVar
    RxWorkerThreadMgr --> RxWorkerThread
    RxAsyncMgr --> RxWorkerThreadMgr
```

---

### 3.5 Ev（事件系统）

**路径**: `kernel/src/Ev/`（4个文件） | `kernel/Include/public/Ev/`

提供系统级的**事件分发机制**，连接定时器、数据库变更与上层应用。

```mermaid
sequenceDiagram
    participant Main as main.cpp
    participant Sys as AcSystem
    participant EvMgr as EvEventManager
    participant DbReactor as DbDatabaseReactor
    participant App as 上层应用

    Main->>Sys: setInterval(onInterval, 10ms)
    loop 每 10ms
        Sys->>EvMgr: _emit(Ev::Interval::kInterval)
        EvMgr->>App: 触发渲染更新
    end

    Note over DbReactor: 注册到 DbDatabase
    App->>DbReactor: addReactor(reactor)
    DbReactor->>App: onObjectModified(obj)
    DbReactor->>App: onObjectErased(id)
```

---

### 3.6 Ap（应用/文档管理）

**路径**: `kernel/src/Ap/`（7个文件） | `kernel/Include/public/Ap/`

管理 CAD **文档的生命周期**，支持多文档并发（MDI）。

```mermaid
graph TD
    ApDocMgr["ApDocManager\n文档管理器\n持有所有打开的文档"]
    ApDocument["ApDocument\n单个文档实例\n- 文档标题\n- 是否已修改\n- 关联数据库"]
    DbDatabase["DbDatabase\n图形数据库\n（见 Db 模块）"]
    ApWorkingDoc["工作文档指针\n当前激活文档"]

    ApDocMgr --> |1..*| ApDocument
    ApDocument --> |1:1| DbDatabase
    ApDocMgr --> ApWorkingDoc
    ApWorkingDoc --> |指向| ApDocument
```

**关键接口**：
- `ApDocManager::newDoc()` — 创建新文档
- `ApDocManager::openDoc(path)` — 打开文件，触发 DXF 解析
- `ApDocManager::workDoc()` — 获取当前活动文档
- `ApDocument::database()` — 获取文档关联的图形数据库

---

### 3.7 Db（数据库层）

**路径**: `kernel/src/Db/`（134个文件） | `kernel/Include/public/Db/`

这是整个项目**最核心的模块**，实现了类似 AutoCAD 的图形数据库，负责所有 CAD 实体的存储、管理和序列化。

```mermaid
graph TB
    DbDatabase["DbDatabase\n图形数据库根对象"] --> |包含| BlockTable["DbBlockTable\n块表"]
    DbDatabase --> |包含| LayerTable["DbLayerTable\n图层表"]
    DbDatabase --> |包含| LinetypeTable["DbLinetypeTable\n线型表"]
    DbDatabase --> |包含| TextStyleTable["DbTextStyleTable\n文字样式表"]
    DbDatabase --> |包含| DimStyleTable["DbDimStyleTable\n标注样式表"]
    DbDatabase --> |包含| ViewportTable["DbViewportTable\n视口表"]
    DbDatabase --> |包含| ViewTable["DbViewTable\n命名视图表"]
    DbDatabase --> |包含| UCSTable["DbUCSTable\n用户坐标系表"]
    DbDatabase --> |包含| RegAppTable["DbRegAppTable\n注册应用表"]
    DbDatabase --> |包含| LayoutMgr["DbLayoutManager\n布局管理器"]

    BlockTable --> |包含| BlockTableRecord["DbBlockTableRecord\n块定义（含实体）"]
    BlockTableRecord --> |包含| Entities["CAD 实体集合"]

    subgraph EntityTypes["CAD 实体类型"]
        Line2["DbLine\n直线"]
        Arc2["DbArc\n圆弧"]
        Circle["DbCircle\n圆"]
        Polyline["DbPolyline\n多段线"]
        Text["DbText / DbMText\n文字"]
        Solid["DbSolid\n填充"]
        BlockRef["DbBlockReference\n块参照"]
        Hatch["DbHatch\n图案填充"]
        Insert["DbInsert\n外部参照"]
    end

    Entities --> EntityTypes

    DbDatabase --> |管理| Filers["序列化器"]
    subgraph Filers
        DxfFiler["DbDxfFiler\nDXF 格式读写"]
        DwgFiler["DbDwgFileFiler\nDWG 格式读写"]
        UndoFiler["DbDwgUndoFiler\n撤销/重做"]
        CopyFiler["DbDwgCopyFiler\n对象克隆"]
    end
```

**对象 ID 系统**：

```mermaid
graph LR
    DbHandle["DbHandle\n内部句柄 (UInt32/64)"] --> |对应| DbObjectId["DbObjectId\n外部对象引用"]
    DbObjectId --> |通过| DbObjectManager["DbObjectManager\n管理器维护映射表"]
    DbObjectManager --> |返回| DbObject*["DbObject*\n实际对象指针"]
    DbObject* --> |修改后触发| DbDatabaseReactor["DbDatabaseReactor\n反应器通知"]
```

---

### 3.8 Gi（图形渲染）

**路径**: `kernel/src/Gi/`（43个文件） | `kernel/Include/public/Gi/`

负责将数据库中的 **CAD 实体转换为可渲染的几何数据**，是 Db 层到 Re 渲染层之间的桥梁。

```mermaid
graph TD
    DbEntity["DbEntity\n数据库实体"] --> |worldDraw()| GiWorldDraw["GiWorldDrawManager\n世界坐标系绘制接口"]
    GiWorldDraw --> |输出几何| GiGeometry["GiGeometry\n几何输出\n- polyline()\n- circle()\n- polygon()\n- mesh()"]
    GiGeometry --> |创建| GiEntity["GiEntity\n图形实体缓存\n(含顶点数据)"]
    GiEntity --> |注册到| GiEntityMgr["GiEntityManager\n图形实体管理器"]

    subgraph Materials["材质系统"]
        GiMaterial["GiMaterial\n材质基类"]
        GiLineMaterial["GiLineMaterial\n实线材质"]
        GiLineDashed["GiLineDashedMaterial\n虚线材质"]
        GiLinePxDashed["GiLinePixelDashedMaterial\n像素虚线"]
        GiPointMaterial["GiPointMaterial\n点材质"]
        GiMeshMaterial["GiMeshMaterial\n网格/填充材质"]
        GiMaterial --> GiLineMaterial
        GiMaterial --> GiLineDashed
        GiMaterial --> GiLinePxDashed
        GiMaterial --> GiPointMaterial
        GiMaterial --> GiMeshMaterial
    end

    GiEntity --> |使用| Materials
    GiEntityMgr --> |包围盒计算| GiExtents["GiExtentsCalculator\n范围计算器"]
```

---

### 3.9 Gs（图形元素）

**路径**: `kernel/src/Gs/` | `kernel/Include/public/Gs/`

管理屏幕上的**图形元素和光标**，处理坐标系显示。

| 类 | 功能 |
|----|------|
| `GsElementManager` | 管理所有可见图形元素 |
| `GsCursorManager` | 控制 CAD 十字光标的显示 |
| `GsCoordSystem` | 坐标系（UCS/WCS）显示管理 |

---

### 3.10 Re（渲染器管理）

**路径**: `kernel/src/Re/` | `kernel/Include/public/Re/`

管理**视口和摄像机**，协调从场景几何到最终输出的渲染流程。

```mermaid
graph LR
    ReMgr["ReRendererManager\n渲染器管理器"] --> |创建/管理| ReRenderer["ReRenderer\n渲染器实例"]
    ReRenderer --> |包含| ReCamera["ReCamera\n摄像机\n- 视图矩阵\n- 投影矩阵\n- 视口尺寸"]
    ReCamera --> |变换| GiEntity["GiEntity 顶点数据"]
    GiEntity --> |WASM 导出| JSWebGL["JavaScript WebGL\n最终渲染"]
    ReRenderer --> |触发| GiEntityMgr["GiEntityManager\n遍历所有实体"]
```

**摄像机操作**：
- 平移（Pan）：修改视图矩阵的位移分量
- 缩放（Zoom）：修改投影矩阵的缩放分量
- 旋转（Orbit）：通过 `GeQuaternion` 更新视图矩阵

---

### 3.11 Ed（编辑器/命令）

**路径**: `kernel/src/Ed/`（16个文件） | `kernel/Include/public/Ed/`

提供完整的 **CAD 命令系统和用户输入处理**，是 JavaScript 与 CAD 内核交互的主要入口。

```mermaid
graph TD
    subgraph Input["用户输入层"]
        MouseEvt["鼠标事件\nEdMouseEvent"]
        TouchEvt["触控事件\nEdTouchEvent"]
        WheelEvt["滚轮事件\nEdWheelEvent"]
        KeyEvt["键盘事件\nEdKeyboardEvent"]
    end

    EdInputMgr["EdUserInputManager\n用户输入管理器\n统一入口"] --> |接收| Input

    EdInputMgr --> |分发到| EdCtrlMgr["EdControllerManager\n控制器管理器"]

    subgraph Controllers["控制器"]
        AcadCtrl["EdAcadController\n传统 CAD 控制器"]
        TouchCtrl["EdTouchController\n触控专用控制器"]
    end

    EdCtrlMgr --> Controllers

    Controllers --> |触发| EdCmdStack["EdCommandStack\n命令堆栈\nexecute(命令名)"]

    EdCmdStack --> |执行| EdCommand["EdCommand\n命令基类\n- globalName() 全局名\n- localName() 本地化名\n- execute() 执行逻辑"]

    subgraph Commands["内置命令示例"]
        LineCmd["LINE 画线命令"]
        CircleCmd["CIRCLE 画圆命令"]
        EraseCmd["ERASE 删除命令"]
        MoveCmd["MOVE 移动命令"]
    end

    EdCommand --> Commands

    Commands --> |使用| EdJig["EdJig\n绘图辅助\n实时预览拖动效果"]
    EdJig --> |回调| EdInputContextReactor["EdInputContextReactor\n输入上下文反应器"]
```

**Jig 机制**（实时预览）：
```mermaid
sequenceDiagram
    participant User as 用户
    participant InputMgr as EdUserInputManager
    participant Jig as EdJig
    participant Db as DbDatabase
    participant Renderer as ReRenderer

    User->>InputMgr: 移动鼠标
    InputMgr->>Jig: sampler() 采样当前坐标
    Jig->>Db: 临时修改实体位置
    Db->>Renderer: 触发重绘
    Renderer->>User: 显示预览效果
    User->>InputMgr: 点击确认
    Jig->>Db: 提交最终位置
```

---

### 3.12 Pr（解析器）

**路径**: `kernel/src/Pr/` | `kernel/Include/public/Pr/`

负责解析 **DXF（Drawing Exchange Format）文件**，将文本数据转换为 DbDatabase 对象树。

```mermaid
flowchart TD
    DXFFile["DXF 文件数据\n（文本格式）"] --> Parser["PrDXFParser\nDXF 解析器"]

    Parser --> |解析 HEADER 段| Header["文件头信息\n坐标系/单位/版本"]
    Parser --> |解析 TABLES 段| Tables["符号表\n图层/线型/样式"]
    Parser --> |解析 BLOCKS 段| Blocks["块定义\n含实体几何"]
    Parser --> |解析 ENTITIES 段| Entities["实体段\n模型空间实体"]

    Header --> DbDatabase["DbDatabase\n图形数据库"]
    Tables --> DbDatabase
    Blocks --> DbDatabase
    Entities --> DbDatabase
```

---

### 3.13 Emscripten（JS绑定层）

**路径**: `kernel/src/Emscripten/`（12个文件）

通过 Emscripten **Embind 机制**将 C++ 类导出为 JavaScript 可调用的接口。

| 绑定文件 | 导出内容 | 代码行数 |
|----------|----------|----------|
| `GeExport.cpp` | 所有几何类（点/向量/矩阵/曲线等） | 1440行 |
| `DbExport.cpp` | 数据库类（Database/Entity/Table等） | 617行 |
| `Acad.cpp` | 错误码、枚举、基础类型 | 572行 |
| `GiExport.cpp` | 图形渲染类（Entity/Material等） | 230行 |
| `EdExport.cpp` | 编辑器/命令/输入类 | ~200行 |
| `ApExport.cpp` | 文档管理类 | ~100行 |
| `ReExport.cpp` | 渲染器/摄像机类 | ~100行 |
| `kernel.cpp` | 全局函数（Manager 访问器） | ~50行 |

**JS 调用示例**：
```javascript
// 访问渲染器管理器
const rendererMgr = Module.acreRendererManager();

// 访问用户输入管理器  
const inputMgr = Module.acedUserInputManager();

// 访问主机应用服务（加载 DXF）
const hostApp = Module.acdbHostApplicationServices();
hostApp.emsdk_load_dxf_data(dxfTextData);
```

---

## 4. 核心数据流程

### 4.1 系统初始化流程

```mermaid
flowchart TD
    Start(["程序启动"]) --> Main["main()"]
    Main --> NewB["new B()\n测试对象（调试用）"]
    Main --> InstMgr["acrxManagerDictionary()->instantiate()\n实例化所有 Manager"]
    InstMgr --> KernelInit["KernelImpl::KernelImpl()\n创建所有 Manager 实例"]
    KernelInit --> |创建| EvMgr["EvEventManager"]
    KernelInit --> |创建| RxThread["RxWorkerThreadManager"]
    KernelInit --> |创建| GiObjMgr["GiObjectManager"]
    KernelInit --> |创建| GiMatMgr["GiMaterialManager"]
    KernelInit --> |创建| GiEntityMgr["GiEntityManager"]
    KernelInit --> |创建| DbObjMgr["DbObjectManager"]
    KernelInit --> |创建| ReMgr["ReRendererManager"]
    KernelInit --> |创建| ApMgr["ApDocManager"]
    KernelInit --> |创建| EdInputMgr["EdUserInputManager"]
    KernelInit --> |创建| EdCmdStack["EdCommandStack"]
    KernelInit --> Done["所有 Manager 初始化完成"]
    Done --> Interval["AcSystem::setInterval(onInterval, 10)\n设置 10ms 渲染定时器"]
    Interval --> Loop(["进入 WASM 主循环\n等待 JS 调用"])
```

---

### 4.2 DXF文件加载流程

```mermaid
flowchart TD
    JS["JavaScript\n读取 DXF 文件为文本"] --> |调用 WASM| HostApp["DbHostApplicationServices\n::emsdk_load_dxf_data(data)"]
    HostApp --> NewDb["new DbDatabase()\n创建新图形数据库"]
    NewDb --> |初始化| SymTables["创建 9 个符号表\nLayer/Linetype/TextStyle/\nDimStyle/Viewport/View/\nUCS/RegApp/Block"]
    NewDb --> ModelSpace["创建 *MODEL_SPACE 块"]
    NewDb --> PaperSpace["创建 *PAPER_SPACE 块"]
    NewDb --> Layout["创建默认布局 Layout"]

    HostApp --> Parser["PrDXFParser::load()\n解析 DXF 数据"]
    Parser --> |HEADER 段| SetVars["设置数据库变量\n(单位/精度/颜色等)"]
    Parser --> |TABLES 段| FillTables["填充符号表\n图层/线型/样式定义"]
    Parser --> |BLOCKS 段| FillBlocks["创建块定义\n添加实体到块"]
    Parser --> |ENTITIES 段| FillEntities["添加实体到模型空间"]

    FillEntities --> |每个实体| DbEntity["DbEntity 对象\n存入 DbObjectManager"]
    DbEntity --> |触发| GiUpdate["GiEntityManager\n更新图形缓存"]
    GiUpdate --> Ready["图形数据就绪\n等待渲染"]
```

---

### 4.3 渲染流程

```mermaid
flowchart TD
    Timer["定时器触发 (每 10ms)\nonInterval()"] --> EmitEvt["EvEventManager\n_emit(Ev::Interval::kInterval)"]
    EmitEvt --> Renderer["ReRendererManager\n::renderer()"]
    Renderer --> Camera["ReCamera\n计算 MVP 矩阵\n(Model-View-Projection)"]
    Camera --> EntityList["GiEntityManager\n遍历所有图形实体"]
    EntityList --> |每个实体| CalcVisibility["视锥体裁剪\n判断是否在视口内"]
    CalcVisibility --> |可见| GetVertices["获取顶点数据\n(线段/网格/点)"]
    GetVertices --> ApplyMaterial["应用材质\nGiMaterial 参数\n(颜色/线宽/线型)"]
    ApplyMaterial --> ExportWASM["导出到 JavaScript\nEmscripten::val 传递顶点数组"]
    ExportWASM --> WebGL["JavaScript WebGL\n绘制最终图形"]
    CalcVisibility --> |不可见| Skip["跳过该实体"]
```

---

### 4.4 用户输入与命令执行流程

```mermaid
flowchart TD
    UserAction["用户操作\n(鼠标/触控/键盘)"] --> JSEvent["JavaScript\n捕获 DOM 事件"]
    JSEvent --> |调用 WASM| InputMgr["EdUserInputManager\n::onMouseEvent() /\n::onTouchEvent() /\n::onKeyboardEvent()"]
    InputMgr --> CtrlMgr["EdControllerManager\n选择合适的控制器"]
    CtrlMgr --> |传统操作| AcadCtrl["EdAcadController\n处理标准 CAD 操作"]
    CtrlMgr --> |触控操作| TouchCtrl["EdTouchController\n处理多点触控手势"]

    AcadCtrl --> |是命令触发| CmdStack["EdCommandStack\n::execute(commandName)"]
    AcadCtrl --> |是视图操作| ViewCtrl["视图控制\nPan / Zoom / Orbit"]
    ViewCtrl --> Camera["更新 ReCamera\n参数"]

    CmdStack --> FindCmd["查找注册的 EdCommand\n匹配全局名或本地化名"]
    FindCmd --> |找到| ExecCmd["EdCommand::execute()\n执行命令逻辑"]
    ExecCmd --> |需要用户输入| Jig["EdJig::acquire\nPoint/String/Angle()\n阻塞等待输入"]
    Jig --> |用户确认| ModifyDb["修改 DbDatabase\n创建/修改/删除实体"]
    ModifyDb --> ReactorNotify["DbDatabaseReactor\n通知所有观察者"]
    ReactorNotify --> GiUpdate["GiEntityManager\n更新图形缓存"]
    GiUpdate --> Redraw["触发重绘"]
```

---

### 4.5 对象序列化流程

```mermaid
flowchart LR
    subgraph Save["保存流程"]
        DbObj2["DbObject\n图形对象"] --> |dwgOut()| DwgFiler["DbDwgFileFiler\nDWG 序列化器"]
        DbObj2 --> |dxfOut()| DxfFiler["DbDxfFiler\nDXF 序列化器"]
        DwgFiler --> DWGFile["output.dwg"]
        DxfFiler --> DXFFile["output.dxf"]
    end

    subgraph Load["加载流程"]
        DXFFile2["input.dxf"] --> PrParser["PrDXFParser\nDXF 解析器"]
        DWGFile2["input.dwg"] --> DwgReader["DbDwgFileFiler\nDWG 读取器"]
        PrParser --> |dwgIn()| DbObj["DbObject\n恢复的图形对象"]
        DwgReader --> |dwgIn()| DbObj
    end

    subgraph Undo["撤销/重做"]
        UndoFiler["DbDwgUndoFiler\n撤销序列化器"] --> |记录操作| UndoStack["撤销堆栈"]
        UndoStack --> |Ctrl+Z| RestoreObj["恢复对象状态"]
    end
```

---

## 5. 模块依赖关系

```mermaid
graph TB
    Ac["Ac\n基础工具"] 
    Ge["Ge\n几何引擎"]
    Rx["Rx\n运行时"]
    Ev["Ev\n事件"]
    Ap["Ap\n文档"]
    Db["Db\n数据库"]
    Gi["Gi\n图形"]
    Gs["Gs\n元素"]
    Re["Re\n渲染"]
    Ed["Ed\n编辑器"]
    Pr["Pr\n解析器"]
    App["App\n应用"]
    Kernel["Kernel\n内核"]

    Ge --> Ac
    Rx --> Ac
    Ev --> Ac
    Ev --> Rx
    Ap --> Ac
    Ap --> Rx
    Ap --> Db
    Db --> Ac
    Db --> Ge
    Db --> Rx
    Gi --> Ac
    Gi --> Ge
    Gi --> Db
    Gi --> Rx
    Gs --> Gi
    Re --> Gi
    Re --> Ge
    Ed --> Ac
    Ed --> Ge
    Ed --> Db
    Ed --> Gi
    Pr --> Db
    Pr --> Ge
    App --> Ap
    App --> Rx
    Kernel --> Ev
    Kernel --> Rx
    Kernel --> Ap
    Kernel --> Db
    Kernel --> Gi
    Kernel --> Gs
    Kernel --> Re
    Kernel --> Ed
    Kernel --> App
```

---

## 6. 技术栈

| 组件 | 技术 | 说明 |
|------|------|------|
| **核心语言** | C++11 | 现代 C++ 标准，支持移动语义、智能指针 |
| **构建系统** | CMake | 跨平台构建，支持 Windows/macOS/Linux/Web |
| **WASM 编译** | Emscripten | 将 C++ 编译为 WebAssembly |
| **JS 绑定** | Emscripten Embind | 导出 C++ 类和函数到 JavaScript |
| **序列化** | RapidJSON | 快速 JSON 解析，用于配置和数据传输 |
| **线程** | POSIX Threads + Emscripten | 支持 Web Worker 线程 |
| **异步** | Emscripten Asyncify | C++ 协程，支持 WASM 中的阻塞式输入 |
| **设计模式** | Pimpl (Pointer to Implementation) | 所有公共类隐藏实现细节，减小头文件依赖 |
| **对象模型** | 类 AutoCAD ObjectARX | Manager + Object ID + Reactor 架构 |

---

## 7. 构建系统

### 编译目标

```mermaid
graph LR
    Source["C++ 源码\n319 个 .cpp 文件"] --> CMake["CMake"]
    CMake --> |EMSCRIPTEN| WASM["webcad.wasm\n+ webcad.js\n(Web 平台)"]
    CMake --> |WIN32| DLL["webcad.dll/.lib\n(Windows)"]
    CMake --> |UNIX| SO["libwebcad.so\n(Linux)"]
    CMake --> |APPLE| Dylib["libwebcad.dylib\n(macOS)"]
```

### 主要 Emscripten 编译选项

| 选项 | 说明 |
|------|------|
| `-s WASM=1` | 输出 WebAssembly 格式 |
| `-s ASYNCIFY` | 启用异步支持（阻塞式 C++ 转协程） |
| `-s PROXY_TO_PTHREAD=1` | 主线程代理到 Worker 线程 |
| `-s ALLOW_MEMORY_GROWTH=1` | 允许内存动态增长 |
| `-s ALLOW_TABLE_GROWTH` | 允许函数表动态增长 |
| `--bind` | 启用 Embind JS 绑定 |

---

## 8. 目录结构

```
webcad/
└── kernel/                          # CAD 内核（全部源码）
    ├── main.cpp / main.h            # 程序入口
    ├── CMakeLists.txt               # CMake 构建配置（629行）
    ├── Include/
    │   ├── public/                  # 公开 API 头文件
    │   │   ├── Ac/                  # 基础工具头文件
    │   │   ├── Ap/                  # 文档管理头文件
    │   │   ├── Db/                  # 数据库头文件
    │   │   ├── Ed/                  # 编辑器头文件
    │   │   ├── Ge/                  # 几何引擎头文件
    │   │   ├── Gi/                  # 图形渲染头文件
    │   │   ├── Pr/                  # 解析器头文件
    │   │   ├── Rx/                  # 运行时头文件
    │   │   └── Emscripten/          # JS 绑定接口
    │   └── private/                 # 内部实现细节头文件（Pimpl）
    ├── src/
    │   ├── Ac/  (6个文件)           # 基础工具实现
    │   ├── Ge/  (67个文件)          # 几何引擎实现（最大）
    │   ├── Ev/  (4个文件)           # 事件系统实现
    │   ├── Rx/  (18个文件)          # 运行时系统实现
    │   ├── Ap/  (7个文件)           # 文档管理实现
    │   ├── Db/  (134个文件)         # 数据库实现（最核心）
    │   ├── Gi/  (43个文件)          # 图形渲染实现
    │   ├── Gs/  (4个文件)           # 图形元素实现
    │   ├── Ed/  (16个文件)          # 编辑器实现
    │   ├── App/ (2个文件)           # 应用管理实现
    │   ├── Re/  (5个文件)           # 渲染器实现
    │   ├── Pr/  (1个文件)           # DXF 解析实现
    │   ├── Ut/  (2个文件)           # 工具函数
    │   ├── Emscripten/ (12个文件)   # JS 绑定层
    │   └── kernel/ (1个文件)        # 内核单例实现
    ├── 3rdparty/
    │   └── rapidjson/               # JSON 解析库
    └── build/                       # 编译输出目录
        ├── wasm/                    # WebAssembly 输出
        └── win/                     # Windows 输出
```

---

*文档自动生成于 WebCAD 项目代码分析，基于 C++11 源码和 CMake 构建配置整理。*
