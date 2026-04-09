# PixelOdyssey MDI 多文档界面架构

## 📁 文件结构

```
src/ui/
├── GLDrawApp.h/cpp                    # 应用程序入口 (升级为 CWinAppEx)
├── document/                          # 文档层
│   ├── PixelDocument.h               # 文档类头文件
│   └── PixelDocument.cpp             # 文档类实现
├── view/                             # 视图层
│   ├── PixelView.h                   # OpenGL 视图头文件
│   └── PixelView.cpp                 # OpenGL 视图实现
├── childframe/                       # 子框架层
│   ├── PixelChildFrame.h             # MDI 子窗口头文件
│   └── PixelChildFrame.cpp           # MDI 子窗口实现
├── mainframe/                        # 主框架层
│   ├── PixelMainFrame.h              # MDI 主窗口头文件
│   └── PixelMainFrame.cpp            # MDI 主窗口实现
├── acad/                             # UI 组件 (复用)
│   ├── AcadUIManager.h/cpp           # UI 管理器
│   ├── AcadToolPalette.h/cpp         # 工具面板
│   ├── AcadPropertyPanel.h/cpp       # 属性面板
│   ├── AcadCommandLine.h/cpp         # 命令行
│   ├── UILayout.h                    # 布局常量
│   └── ...                           # 其他 UI 组件
├── core/                             # UI 核心
│   └── ColorScheme.h/cpp             # 配色方案
├── resources/                        # 资源文件
│   ├── Resource.h                    # 资源 ID 定义
│   └── GLDrawApp.rc                  # 资源脚本
└── utils/                            # 工具类
    └── OpenGLLogger.h                # OpenGL 日志
```

## 🏗️ 类层次结构

### 应用程序层
```
CWinAppEx (MFC)
    └── CGLDrawApp
        ├── m_pDocTemplate (CMultiDocTemplate*)
        └── m_pMainFrame (CPixelMainFrame*)
```

### 文档层
```
CDocument (MFC)
    └── CPixelDocument
        ├── m_pScene (Scene*)          # 场景数据
        ├── m_strTitle (CString)       # 文档标题
        └── m_strFilePath (CString)    # 文件路径
```

### 视图层
```
CView (MFC)
    └── CPixelView
        ├── m_hGLRC (HGLRC)            # OpenGL 上下文
        ├── m_camera (Camera)          # 相机
        └── m_renderView (CStatic)     # 渲染区域
```

### 框架层
```
CMDIFrameWnd (MFC)                     CMDIChildWnd (MFC)
    └── CPixelMainFrame                    └── CPixelChildFrame
        ├── m_wndToolBar (CToolBar)            ├── m_wndToolPalette (CWnd)
        └── m_wndStatusBar (CStatusBar)        ├── m_wndPropertyPanel (CWnd)
                                               └── m_wndCommandLine (CWnd)
```

## 🔄 数据流

```
用户操作
    │
    ▼
CPixelMainFrame (菜单栏/工具栏)
    │
    ▼
命令路由 → CPixelChildFrame (子窗口)
    │
    ▼
CPixelDocument (文档数据)
    │
    ├───────→ UpdateAllViews()
    │              │
    │              ▼
    │         CPixelView* (视图 1)
    │              │
    │              ▼
    │         RenderOpenGL() → Scene::Draw()
    │
    ├───────→ UpdateAllViews()
    │              │
    │              ▼
    │         CPixelView* (视图 2)
    │              │
    │              ▼
    │         RenderOpenGL() → Scene::Draw()
    │
    └───────→ ... (更多视图)
```

## 🎯 核心功能

### 1. 文档管理
| 功能 | 快捷键 | 说明 |
|------|--------|------|
| 新建文档 | Ctrl+N | 创建新的绘图场景 |
| 打开文档 | Ctrl+O | 加载已有文件 |
| 保存文档 | Ctrl+S | 保存当前场景 |
| 另存为 | F12 | 保存为新文件 |
| 关闭文档 | Ctrl+W | 关闭当前窗口 |

### 2. 窗口管理
| 功能 | 说明 |
|------|------|
| 层叠窗口 | 所有子窗口层叠排列 |
| 横向平铺 | 所有子窗口横向平铺 |
| 纵向平铺 | 所有子窗口纵向平铺 |
| 排列图标 | 整理最小化的窗口图标 |

### 3. OpenGL 渲染
- 每个视图独立的 OpenGL 上下文
- 独立的相机控制
- 实时动画更新 (60FPS)
- 支持多个场景同时渲染

## 📊 性能特性

### 内存使用
- 每个文档：约 1-5MB (取决于场景复杂度)
- 每个视图：约 2-10MB (OpenGL 资源)
- 共享资源：UI 组件、着色器、纹理

### 渲染性能
- 单视图：60 FPS (垂直同步)
- 多视图：根据 GPU 性能动态调整
- 后台渲染：非活动窗口降低刷新率

## 🔧 扩展点

### 添加新文档类型
1. 创建新的 `CDocument` 子类
2. 创建对应的 `CView` 子类
3. 注册新的 `CMultiDocTemplate`

### 添加新视图类型
1. 继承 `CView` 或 `CPixelView`
2. 实现特定的渲染逻辑
3. 在文档模板中注册

### 自定义 UI 组件
1. 在 `acad/` 目录下创建新组件
2. 在 `CPixelChildFrame` 中集成
3. 使用 `UILayout` 进行布局

## 📝 使用示例

### 创建新文档
```cpp
// 通过菜单或快捷键
ID_FILE_NEW → CWinAppEx::OnFileNew()
    → CMultiDocTemplate::CreateNewFrame()
    → CPixelChildFrame::OnCreate()
    → CPixelView::OnCreate()
    → CreateTestSceneIfEmpty()
```

### 文档保存
```cpp
CPixelDocument::SaveDocument(LPCTSTR lpszPathName)
    → 序列化场景数据
    → 更新文档标题
    → 通知所有视图更新
```

### 视图渲染
```cpp
CPixelView::RenderOpenGL()
    → glClear()
    → 设置相机矩阵
    → GetScene()->Draw()
    → SwapBuffers()
```

## 🚀 编译配置

### 必需的头文件
```cpp
#include <afxwin.h>      // MFC 核心
#include <afxmdi.h>      // MDI 支持
#include <afxdoc.h>      // 文档/视图框架
#include <GL/glew.h>     // OpenGL
```

### 链接库
```
mfc140.lib           // MFC
opengl32.lib         // OpenGL
glu32.lib            // GLU
glew32.lib           // GLEW
```

## ⚠️ 注意事项

1. **OpenGL 上下文**: 每个视图有独立的上下文，不能跨视图共享资源
2. **线程安全**: 文档修改需要在主线程中进行
3. **内存管理**: 文档销毁时会自动清理关联的场景和视图
4. **资源 ID**: 确保资源 ID 不冲突，使用连续的 ID 范围

## 📖 相关文档

- [UI_MDI_REFACTOR.md](./UI_MDI_REFACTOR.md) - 详细改造说明
- [README.md](./README.md) - 项目总体说明
