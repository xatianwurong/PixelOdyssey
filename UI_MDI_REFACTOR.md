# UI 架构改造：从单文档到多文档 (MDI) 界面

## 概述

本次改造将原有的单文档界面 (SDI) 重构为多文档界面 (MDI)，支持同时打开和编辑多个绘图场景。

## 架构变更

### 原架构 (SDI - 单文档界面)
```
CWinApp (CGLDrawApp)
    └── CFrameWnd (CAcadMainWindow)
        ├── OpenGL 渲染区域
        ├── 工具面板 (左侧)
        ├── 属性面板 (右侧)
        └── 命令行 (底部)
```

### 新架构 (MDI - 多文档界面)
```
CWinAppEx (CGLDrawApp)
    ├── CMultiDocTemplate (文档模板)
    │   ├── CPixelDocument (文档类)
    │   ├── CPixelChildFrame (子框架)
    │   └── CPixelView (视图类)
    └── CPixelMainFrame (MDI 主框架)
        ├── 菜单栏
        ├── 工具栏
        └── 状态栏
```

## 新增文件

### 1. 文档层 (`src/ui/document/`)
- **PixelDocument.h/cpp** - 文档类
  - 管理场景数据 (`Scene*`)
  - 处理文档的创建、保存、加载
  - 维护文档标题和文件路径

### 2. 视图层 (`src/ui/view/`)
- **PixelView.h/cpp** - OpenGL 渲染视图
  - 继承自 `CView`
  - 每个视图包含独立的 OpenGL 上下文
  - 渲染关联文档的场景数据
  - 支持相机控制和实时渲染

### 3. 子框架 (`src/ui/childframe/`)
- **PixelChildFrame.h/cpp** - MDI 子窗口
  - 继承自 `CMDIChildWnd`
  - 管理单个文档的 UI 组件
  - 包含工具面板和属性面板
  - 响应窗口大小变化

### 4. 主框架 (`src/ui/mainframe/`)
- **PixelMainFrame.h/cpp** - MDI 主窗口
  - 继承自 `CMDIFrameWnd`
  - 管理全局菜单栏、工具栏、状态栏
  - 提供窗口管理功能（平铺、层叠等）

### 5. 应用入口修改
- **GLDrawApp.h/cpp** - 应用程序类升级
  - 从 `CWinApp` 升级到 `CWinAppEx`
  - 集成 MDI 文档模板系统
  - 支持标准文件操作命令

## 核心特性

### 1. 多文档支持
- 可同时打开多个绘图文件
- 每个文档有独立的场景数据
- 支持文档间切换

### 2. 灵活的视图管理
- 每个文档可创建多个视图
- 支持平铺、层叠等窗口布局
- 独立的 OpenGL 渲染上下文

### 3. 标准化的文件操作
- 新建文档 (`Ctrl+N`)
- 打开文档 (`Ctrl+O`)
- 保存文档 (`Ctrl+S`)
- 另存为 (`F12`)
- 关闭文档 (`Ctrl+W`)

### 4. 窗口管理
- 新建窗口
- 窗口层叠
- 窗口平铺（横向/纵向）
- 排列图标

## 资源 ID 定义

在 `Resource.h` 中新增了以下资源 ID：

```cpp
// MDI 框架资源
#define IDR_MAINFRAME                     200
#define IDR_PIXELTYPE                     201

// 文件操作
#define ID_FILE_NEW                       32771
#define ID_FILE_OPEN                      32772
#define ID_FILE_SAVE                      32773
#define ID_FILE_SAVE_AS                   32774
#define ID_FILE_CLOSE                     32775
#define ID_FILE_EXIT                      32776

// 编辑操作
#define ID_EDIT_UNDO                      32777
#define ID_EDIT_REDO                      32778
#define ID_EDIT_CUT                       32779
#define ID_EDIT_COPY                      32780
#define ID_EDIT_PASTE                     32781

// 视图管理
#define ID_VIEW_TOOLBAR                   32782
#define ID_VIEW_STATUSBAR                 32783
#define ID_VIEW_WINDOWS                   32784

// 窗口布局
#define ID_WINDOW_NEW                     32785
#define ID_WINDOW_CASCADE                 32786
#define ID_WINDOW_TILE_HORZ               32787
#define ID_WINDOW_TILE_VERT               32788
#define ID_WINDOW_ARRANGE                 32789

// 帮助
#define ID_HELP_ABOUT                     32790

// 状态栏指示器
#define ID_INDICATOR_CAPS                 0xE700
#define ID_INDICATOR_NUM                  0xE701
#define ID_INDICATOR_SCRL                 0xE702
```

## 使用方式

### 启动应用
应用启动时会自动创建第一个文档窗口，显示默认测试场景。

### 新建文档
- 菜单：`文件` -> `新建`
- 快捷键：`Ctrl+N`
- 每次新建会创建一个独立的场景

### 打开多个文档
可以通过以下方式打开多个文档：
1. 多次执行"新建"命令
2. 使用"打开"命令加载已有文件
3. 每个文档在单独的子窗口中显示

### 窗口布局
- **层叠窗口**: `窗口` -> `层叠`
- **横向平铺**: `窗口` -> `横向平铺`
- **纵向平铺**: `窗口` -> `纵向平铺`

## 技术细节

### OpenGL 上下文管理
每个 `CPixelView` 实例都有独立的：
- OpenGL 渲染上下文 (`HGLRC`)
- 相机实例 (`Camera`)
- 定时器用于动画更新

### 文档 - 视图同步
- 文档数据修改后，所有关联视图自动更新
- 通过 `UpdateAllViews()` 机制实现
- 视图只负责渲染，不存储场景数据

### 内存管理
- 文档销毁时自动清理场景数据
- 视图销毁时释放 OpenGL 资源
- 使用智能指针和 RAII 原则

## 待实现功能

1. **文件序列化**
   - 实现场景数据的保存/加载
   - 支持自定义文件格式

2. **撤销/重做**
   - 集成命令栈系统
   - 支持多文档独立撤销历史

3. **剪贴板操作**
   - 对象复制/粘贴
   - 跨文档对象转移

4. **打印支持**
   - 打印预览
   - 导出为图像/PDF

## 兼容性说明

- 保留原有的 `AcadMainWindow` 代码，可作为参考
- UI 布局常量 (`UILayout.h`) 保持不变
- 工具面板、属性面板等组件可复用

## 编译要求

- Visual Studio 2019 或更高版本
- MFC 库支持
- Windows SDK 10.0 或更高版本
- GLEW 和 OpenGL 库

## 下一步计划

1. 完善文件序列化功能
2. 添加文档模板向导
3. 实现最近使用的文件列表 (MRU)
4. 增强窗口管理功能
5. 添加文档比较功能
