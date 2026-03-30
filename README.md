# PixelOdyssey

## 项目简介
PixelOdyssey 是一个基于 C++、MFC 和 OpenGL 的计算机图形学学习应用，旨在提供一个直观的图形绘制和渲染平台，帮助开发者学习和理解计算机图形学的基本概念和实现方法。

## 功能特性

### 核心功能
- **多种图形实体**：支持绘制基本几何形状（矩形、圆形、三角形等）和复杂实体（汽车、猫、狗、鸟等）
- **现代渲染系统**：基于 OpenGL 的现代渲染管线，支持高效的图形绘制
- **场景管理**：提供场景管理功能，支持多场景切换和管理
- **资源管理**：统一的资源管理系统，处理着色器、纹理等资源
- **用户界面**：基于 MFC 的友好用户界面，支持交互操作
- **性能监控**：内置性能监控工具，帮助优化渲染性能

### 支持的图形实体
- **基本几何形状**：矩形、正方形、三角形、圆形、椭圆、五角星、心形等
- **物体**：汽车、杯子、铅笔、书、房子等
- **动物**：猫、狗、鸟、鱼等
- **人物**：站立人物、坐着人物、跑步人物等

## 技术栈

- **编程语言**：C++
- **图形 API**：OpenGL
- **窗口系统**：MFC (Microsoft Foundation Classes)
- **依赖库**：
  - GLEW (OpenGL Extension Wrangler Library)
  - GLFW (Graphics Library Framework)
  - GLM (OpenGL Mathematics)
  - STB (Single-File Public Domain Libraries)

## 项目结构

```
PixelOdyssey/
├── src/
│   ├── core/
│   │   ├── entity/          # 图形实体类
│   │   ├── renderer/        # 渲染系统
│   │   ├── resources/       # 资源管理
│   │   ├── scene/           # 场景管理
│   │   └── utils/           # 工具类
│   └── ui/
│       ├── dialogs/         # 对话框
│       ├── resources/       # UI 资源
│       ├── utils/           # UI 工具类
│       ├── windows/         # 窗口类
│       ├── GLDrawApp.cpp    # 应用程序入口
│       └── GLDrawApp.h      # 应用程序头文件
├── Dependencies/            # 依赖库
│   ├── GLEW/                # OpenGL 扩展库
│   ├── GLFW/                # 窗口管理库
│   ├── glm-0.9.7.1/         # 数学库
│   └── stb-master/          # 单文件工具库
├── PixelOdyssey.sln         # Visual Studio 解决方案
└── README.md                # 项目文档
```

## 安装和使用

### 系统要求
- Windows 10 或更高版本
- Visual Studio 2019 或更高版本
- OpenGL 3.3 或更高版本

### 安装步骤
1. 克隆项目代码到本地
2. 打开 `PixelOdyssey.sln` 解决方案文件
3. 编译项目（Debug 或 Release 模式）
4. 运行生成的可执行文件

### 使用说明
1. 启动应用程序后，将看到一个 OpenGL 渲染窗口
2. 应用程序会自动渲染预定义的图形实体
3. 可以通过代码修改或扩展图形实体的创建和渲染逻辑

## 核心功能模块

### 渲染管理系统
- **RenderManager**：管理所有图形实体的生命周期和渲染过程
- **Camera**：处理相机视角和投影变换
- **VertexBuffer/IndexBuffer**：管理顶点和索引数据

### 图形实体系统
- **Object**：所有可渲染实体的基类
- **Shape**：几何形状的基类
- **各种具体实体**：如 Car、Cat、Dog、Bird 等

### 资源管理系统
- **ResourceManager**：统一管理所有资源
- **Shader**：处理着色器程序
- **Texture**：处理纹理资源

### 场景管理系统
- **Scene**：场景类，包含场景中的实体
- **SceneManager**：管理多个场景的切换

### 用户界面系统
- **MainWindow**：主窗口类
- **MainDialog**：主对话框类
- **DataStorage**：数据存储工具
- **OpenGLLogger**：OpenGL 日志工具

## API 文档

### 核心类

#### RenderManager
```cpp
class RenderManager {
public:
    static RenderManager& Instance();  // 获取单例实例
    void AddObject(Object* object);    // 添加对象
    void RemoveObject(Object* object); // 移除对象
    void Draw();                       // 绘制所有对象
};
```

#### Object
```cpp
class Object {
public:
    virtual void Update(float deltaTime); // 更新方法
    virtual void Draw() = 0;              // 绘制方法（纯虚函数）
    std::string GetId() const;            // 获取对象ID
};
```

#### Shape
```cpp
class Shape : public Object {
public:
    Shape(const glm::vec2& position, const glm::vec4& color, float scale);
    void SetPosition(const glm::vec2& position); // 设置位置
    void SetColor(const glm::vec4& color);       // 设置颜色
    void SetScale(float scale);                  // 设置缩放
    virtual void Draw() override = 0;            // 绘制方法
};
```

## 配置说明

### 项目配置
- **Visual Studio**：使用 Visual Studio 2019 或更高版本
- **平台工具集**：使用最新的平台工具集
- **字符集**：使用 Unicode 字符集
- **OpenGL 版本**：支持 OpenGL 3.3 或更高版本

### 依赖库配置
- **GLEW**：包含头文件和链接库
- **GLFW**：包含头文件和链接库
- **GLM**：包含头文件
- **STB**：包含头文件

## 注意事项

1. **OpenGL 上下文**：确保在使用 OpenGL 函数前正确创建和初始化 OpenGL 上下文
2. **资源管理**：确保正确释放所有 OpenGL 资源，避免内存泄漏
3. **错误处理**：添加适当的错误处理和异常机制
4. **性能优化**：对于大量图形实体，考虑使用批处理和实例化渲染
5. **跨平台兼容性**：当前项目仅支持 Windows 平台，如需跨平台支持需要修改窗口系统

## 潜在问题和优化点

### 潜在问题
1. **注释风格不一致**：部分代码注释使用中文，部分使用英文
2. **缺少错误处理**：部分代码缺少错误处理和异常机制
3. **资源管理**：可能存在资源泄漏风险
4. **性能优化**：渲染大量实体时可能性能下降
5. **缺少单元测试**：项目缺少单元测试

### 优化建议
1. **统一注释风格**：使用英文注释，保持代码风格一致
2. **添加错误处理**：增加错误检查和异常处理
3. **优化资源管理**：使用智能指针和 RAII 模式管理资源
4. **性能优化**：实现批处理渲染和实例化渲染
5. **添加测试**：编写单元测试和集成测试
6. **代码重构**：优化代码结构，减少代码重复

## 许可证

本项目采用 MIT 许可证，详见 LICENSE 文件。

## 贡献

欢迎提交 issue 和 pull request 来改进项目。

## 联系方式

如有问题或建议，请联系项目维护者。

---

© 2026 PixelOdyssey Project
