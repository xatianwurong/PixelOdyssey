# 🚀 重构实施指南 - 详细步骤和代码示例

> **目标**: 指导开发者系统化地实施重构  
> **适用于**: 所有开发团队成员  
> **时间投入**: 周期为 8 周  

---

## 第一阶段：基础设施建设（Week 1-2）

### 任务 1.1：建立服务注入容器

#### 目标
创建一个轻量级的依赖注入容器，允许解耦 UI 组件

#### 实施步骤

**Step 1**: 创建新文件 `services/ServiceContainer.h`

```cpp
#pragma once

#include <map>
#include <memory>
#include <functional>
#include <typeinfo>
#include <typeindex>

/**
 * @brief 轻量级服务注入容器
 * @details 支持单例和工厂模式
 */
class ServiceContainer {
private:
    std::map<std::type_index, std::function<std::shared_ptr<void>()>> m_factories;
    std::map<std::type_index, std::shared_ptr<void>> m_singletons;
    static ServiceContainer* m_instance;
    
    ServiceContainer() = default;

public:
    static ServiceContainer& Instance() {
        if (!m_instance) {
            m_instance = new ServiceContainer();
        }
        return *m_instance;
    }

    // 注册单例服务
    template<typename TInterface, typename TImpl>
    void RegisterSingleton() {
        std::type_index key(typeid(TInterface));
        m_factories[key] = [](){ 
            return std::static_pointer_cast<void>(
                std::make_shared<TImpl>());
        };
    }

    // 注册工厂服务
    template<typename TInterface, typename TImpl>
    void RegisterFactory() {
        std::type_index key(typeid(TInterface));
        m_factories[key] = [](){ 
            return std::static_pointer_cast<void>(
                std::make_shared<TImpl>());
        };
    }

    // 获取服务
    template<typename T>
    std::shared_ptr<T> GetService() {
        std::type_index key(typeid(T));
        
        auto it = m_singletons.find(key);
        if (it != m_singletons.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        
        auto factory = m_factories.find(key);
        if (factory != m_factories.end()) {
            auto instance = factory->second();
            m_singletons[key] = instance;
            return std::static_pointer_cast<T>(instance);
        }
        
        return nullptr;
    }
};
```

**Step 2**: 在 `core/ServiceProvider.h` 中定义服务接口

```cpp
#pragma once

// 日志服务接口
class ILogger {
public:
    virtual void LogInfo(const CString& message) = 0;
    virtual void LogWarning(const CString& message) = 0;
    virtual void LogError(const CString& message) = 0;
    virtual ~ILogger() = default;
};

// 颜色主题服务接口
class IColorScheme {
public:
    virtual COLORREF GetColor(int role) = 0;
    virtual void SetTheme(int theme) = 0;
    virtual ~IColorScheme() = default;
};

// 工具服务接口
class IToolService {
public:
    virtual ToolType GetActiveTool() = 0;
    virtual void SetActiveTool(ToolType type) = 0;
    virtual ~IToolService() = default;
};
```

**Step 3**: 在主窗口初始化时注册服务

```cpp
// MainWindow.cpp
void MainWindow::InitializeServices() {
    auto& container = ServiceContainer::Instance();
    
    // 注册服务
    container.RegisterSingleton<ILogger, ConsoleLogger>();
    container.RegisterSingleton<IColorScheme, ColorSchemeImpl>();
    container.RegisterSingleton<IToolService, ToolServiceImpl>();
}
```

#### 验证
```cpp
// 使用示例
auto logger = ServiceContainer::Instance().GetService<ILogger>();
logger->LogInfo(_T("Application started"));
```

---

### 任务 1.2：建立命令系统框架

#### 目标
实现命令模式，支持撤销/重做

#### 实施步骤

**Step 1**: 创建 `commands/ICommand.h`

```cpp
#pragma once

/**
 * @brief 命令接口 - 所有可撤销操作必须实现此接口
 */
class ICommand {
public:
    virtual ~ICommand() = default;
    
    /**
     * @brief 执行命令
     */
    virtual void Execute() = 0;
    
    /**
     * @brief 撤销命令
     */
    virtual void Undo() = 0;
    
    /**
     * @brief 重做命令（可选，默认等同于 Execute）
     */
    virtual void Redo() { Execute(); }
    
    /**
     * @brief 获取命令描述（用于菜单、日志）
     */
    virtual CString GetDescription() const = 0;
};
```

**Step 2**: 创建 `commands/CommandHistory.h`

```cpp
#pragma once

#include <stack>
#include <memory>
#include "ICommand.h"

/**
 * @brief 命令历史管理器
 * @details 维护撤销和重做栈
 */
class CommandHistory {
private:
    std::stack<std::unique_ptr<ICommand>> m_undoStack;
    std::stack<std::unique_ptr<ICommand>> m_redoStack;
    static constexpr int MAX_HISTORY = 50;  // 最多保存 50 个操作
    int m_size = 0;

public:
    /**
     * @brief 执行命令并添加到历史
     */
    void Execute(std::unique_ptr<ICommand> cmd) {
        if (!cmd) return;
        
        cmd->Execute();
        m_undoStack.push(std::move(cmd));
        
        // 清空重做栈
        while (!m_redoStack.empty()) {
            m_redoStack.pop();
        }
        
        // 限制历史大小
        if (m_size >= MAX_HISTORY) {
            std::stack<std::unique_ptr<ICommand>> temp;
            while (!m_undoStack.empty() && temp.size() < MAX_HISTORY - 1) {
                temp.push(std::move(m_undoStack.top()));
                m_undoStack.pop();
            }
            m_undoStack = std::move(temp);
        }
        m_size = (int)m_undoStack.size();
    }

    /**
     * @brief 撤销上一个命令
     */
    void Undo() {
        if (m_undoStack.empty()) return;
        
        auto cmd = std::move(m_undoStack.top());
        m_undoStack.pop();
        
        cmd->Undo();
        m_redoStack.push(std::move(cmd));
        m_size = (int)m_undoStack.size();
    }

    /**
     * @brief 重做上一个被撤销的命令
     */
    void Redo() {
        if (m_redoStack.empty()) return;
        
        auto cmd = std::move(m_redoStack.top());
        m_redoStack.pop();
        
        cmd->Redo();
        m_undoStack.push(std::move(cmd));
        m_size = (int)m_undoStack.size();
    }

    /**
     * @brief 检查是否可撤销
     */
    bool CanUndo() const { return !m_undoStack.empty(); }

    /**
     * @brief 检查是否可重做
     */
    bool CanRedo() const { return !m_redoStack.empty(); }

    /**
     * @brief 获取下一个撤销操作的描述
     */
    CString GetUndoDescription() const {
        if (m_undoStack.empty()) return _T("");
        return m_undoStack.top()->GetDescription();
    }

    /**
     * @brief 获取下一个重做操作的描述
     */
    CString GetRedoDescription() const {
        if (m_redoStack.empty()) return _T("");
        return m_redoStack.top()->GetDescription();
    }

    /**
     * @brief 清空历史
     */
    void Clear() {
        while (!m_undoStack.empty()) m_undoStack.pop();
        while (!m_redoStack.empty()) m_redoStack.pop();
        m_size = 0;
    }
};
```

**Step 3**: 创建具体命令示例 `commands/SetPropertyCommand.h`

```cpp
#pragma once

#include "ICommand.h"
#include "../../core/ecs/Entity.h"

/**
 * @brief 设置实体属性命令
 */
class SetPropertyCommand : public ICommand {
private:
    Entity m_entity;
    CString m_propertyName;
    CString m_oldValue;
    CString m_newValue;

public:
    SetPropertyCommand(Entity entity, const CString& propName,
                      const CString& oldValue, const CString& newValue)
        : m_entity(entity)
        , m_propertyName(propName)
        , m_oldValue(oldValue)
        , m_newValue(newValue)
    {
    }

    void Execute() override {
        // 设置新值
        if (m_propertyName == _T("Position")) {
            // 解析和设置位置
            auto& transform = m_entity.GetComponent<TransformComponent>();
            // 解析 m_newValue 并设置 transform.position
        }
        // 处理其他属性...
    }

    void Undo() override {
        // 恢复旧值
        if (m_propertyName == _T("Position")) {
            auto& transform = m_entity.GetComponent<TransformComponent>();
            // 恢复为 m_oldValue
        }
    }

    CString GetDescription() const override {
        CString desc;
        desc.Format(_T("Set %s to %s"), m_propertyName, m_newValue);
        return desc;
    }
};
```

**Step 4**: 在属性面板中使用命令系统

```cpp
// AcadPropertyPanel.cpp
void CAcadPropertyPanel::OnPropertyEdited(const CString& name,
                                          const CString& oldValue,
                                          const CString& newValue) {
    if (oldValue == newValue) return;  // 无变化
    
    // 创建命令并执行
    auto cmd = std::make_unique<SetPropertyCommand>(
        m_selectedEntity, name, oldValue, newValue);
    
    m_commandHistory->Execute(std::move(cmd));
    
    // 更新 UI
    UpdateProperties();
}
```

#### 验证
```cpp
// 测试撤销/重做
cmd1->Execute();
cmd2->Execute();

history.Undo();   // 撤销 cmd2
history.Undo();   // 撤销 cmd1

history.Redo();   // 重做 cmd1
history.Redo();   // 重做 cmd2
```

---

### 任务 1.3：建立统一日志系统

#### 目标
创建中央化的日志记录机制

#### 实施步骤

**Step 1**: 创建 `logging/Logger.h`

```cpp
#pragma once

#include <fstream>
#include <ctime>

/**
 * @brief 统一的日志系统
 */
class Logger {
public:
    enum class Level {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
        Critical = 4
    };

private:
    static Logger* m_instance;
    std::ofstream m_fileStream;
    Level m_minLevel;
    bool m_consoleOutput;
    
    Logger();

public:
    static Logger& Instance() {
        if (!m_instance) {
            m_instance = new Logger();
        }
        return *m_instance;
    }

    /**
     * @brief 初始化日志系统
     */
    void Initialize(const CString& filename, Level minLevel = Level::Debug,
                   bool consoleOutput = true);

    /**
     * @brief 记录日志
     */
    void Log(Level level, const CString& category, const CString& message);

    // 便捷方法
    void Debug(const CString& category, const CString& message) {
        Log(Level::Debug, category, message);
    }

    void Info(const CString& category, const CString& message) {
        Log(Level::Info, category, message);
    }

    void Warning(const CString& category, const CString& message) {
        Log(Level::Warning, category, message);
    }

    void Error(const CString& category, const CString& message) {
        Log(Level::Error, category, message);
    }

    void Critical(const CString& category, const CString& message) {
        Log(Level::Critical, category, message);
    }

    ~Logger();
};

// 宏定义，方便使用
#define LOG_DEBUG(msg) Logger::Instance().Debug(_T(__FUNCTION__), msg)
#define LOG_INFO(msg) Logger::Instance().Info(_T(__FUNCTION__), msg)
#define LOG_WARNING(msg) Logger::Instance().Warning(_T(__FUNCTION__), msg)
#define LOG_ERROR(msg) Logger::Instance().Error(_T(__FUNCTION__), msg)
```

**Step 2**: 创建 `logging/Logger.cpp`

```cpp
#include "Logger.h"
#include <iomanip>
#include <sstream>

Logger* Logger::m_instance = nullptr;

Logger::Logger() 
    : m_minLevel(Level::Debug), m_consoleOutput(true)
{
}

Logger::~Logger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::Initialize(const CString& filename, Level minLevel, bool consoleOutput) {
    m_minLevel = minLevel;
    m_consoleOutput = consoleOutput;
    
    // 打开日志文件
    const char* pszFilename = CT2A(filename);
    m_fileStream.open(pszFilename, std::ios::app);
}

void Logger::Log(Level level, const CString& category, const CString& message) {
    if (level < m_minLevel) return;
    
    // 获取当前时间
    time_t now = time(nullptr);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    
    // 格式化时间
    char timeStr[32];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
    
    // 转换日志级别为字符串
    const char* levelStr = "";
    switch (level) {
        case Level::Debug: levelStr = "DEBUG"; break;
        case Level::Info: levelStr = "INFO"; break;
        case Level::Warning: levelStr = "WARN"; break;
        case Level::Error: levelStr = "ERROR"; break;
        case Level::Critical: levelStr = "CRITICAL"; break;
    }
    
    // 构造日志消息
    std::stringstream ss;
    ss << "[" << timeStr << "] "
       << "[" << levelStr << "] "
       << "[" << CT2A(category) << "] "
       << CT2A(message);
    
    std::string logMsg = ss.str();
    
    // 输出到文件
    if (m_fileStream.is_open()) {
        m_fileStream << logMsg << std::endl;
        m_fileStream.flush();
    }
    
    // 输出到控制台
    if (m_consoleOutput) {
        OutputDebugStringA(logMsg.c_str());
        OutputDebugStringA("\n");
    }
}
```

**Step 3**: 在主窗口初始化日志

```cpp
// MainWindow.cpp
BOOL CMainWindow::OnInitDialog() {
    // 初始化日志系统
    Logger::Instance().Initialize(_T("pixelodyssey.log"), 
                                   Logger::Level::Debug, true);
    LOG_INFO(_T("Application started"));
    
    // ...其他初始化代码
    return TRUE;
}
```

#### 验证
```cpp
LOG_DEBUG(_T("Debug message"));
LOG_INFO(_T("Tool changed: Select"));
LOG_WARNING(_T("Property value out of range"));
LOG_ERROR(_T("Failed to load entity"));
```

---

## 第二阶段：重构 UI 架构（Week 3-4）

### 任务 2.1：创建 ViewModel 层

#### 目标
分离 UI 逻辑和数据，提高可测试性

#### 实施步骤

**Step 1**: 创建 `viewmodels/PropertyPanelViewModel.h`

```cpp
#pragma once

#include <functional>
#include <vector>
#include "../../core/ecs/Entity.h"

/**
 * @brief 属性值项
 */
struct PropertyValueItem {
    CString name;      // 属性名称
    CString value;     // 属性值
    CString type;      // 属性类型 (float, int, bool, string)
    bool isEditable;   // 是否可编辑
};

/**
 * @brief 属性面板 ViewModel
 * @details 管理属性数据、更新逻辑，不涉及 UI 渲染
 */
class PropertyPanelViewModel {
public:
    // 事件代理
    using PropertyChangedFunc = std::function<void(const CString&)>;
    using SelectionChangedFunc = std::function<void()>;

    PropertyPanelViewModel();
    ~PropertyPanelViewModel();

    // 数据属性
    const CString& GetTitle() const { return m_title; }
    const std::vector<PropertyValueItem>& GetProperties() const {
        return m_properties;
    }
    Entity GetSelectedEntity() const { return m_selectedEntity; }
    bool IsEntitySelected() const { return m_selectedEntity.IsValid(); }

    // 操作
    void SetSelectedEntity(Entity entity);
    void EditProperty(const CString& name, const CString& value);
    void RefreshProperties();
    void ClearSelection();

    // 事件订阅
    void OnPropertyChanged(PropertyChangedFunc callback) {
        m_propertyChangedCallback = callback;
    }

    void OnSelectionChanged(SelectionChangedFunc callback) {
        m_selectionChangedCallback = callback;
    }

private:
    Entity m_selectedEntity;
    CString m_title;
    std::vector<PropertyValueItem> m_properties;
    
    PropertyChangedFunc m_propertyChangedCallback;
    SelectionChangedFunc m_selectionChangedCallback;

    void RebuildPropertiesFromEntity();
    void NotifyPropertyChanged(const CString& propertyName);
    void NotifySelectionChanged();
};
```

**Step 2**: 实现 `viewmodels/PropertyPanelViewModel.cpp`

```cpp
#include "PropertyPanelViewModel.h"
#include "logging/Logger.h"

PropertyPanelViewModel::PropertyPanelViewModel()
    : m_title(_T("Properties"))
{
}

PropertyPanelViewModel::~PropertyPanelViewModel()
{
}

void PropertyPanelViewModel::SetSelectedEntity(Entity entity) {
    if (m_selectedEntity.GetId() == entity.GetId()) {
        return;  // 相同实体，不处理
    }

    m_selectedEntity = entity;
    RebuildPropertiesFromEntity();
    NotifySelectionChanged();

    LOG_INFO(_T("Entity selected: ") + entity.GetName().c_str());
}

void PropertyPanelViewModel::EditProperty(const CString& name, 
                                          const CString& value) {
    if (!m_selectedEntity.IsValid()) {
        return;
    }

    // 查找属性
    auto it = std::find_if(m_properties.begin(), m_properties.end(),
        [&name](const PropertyValueItem& item) {
            return item.name == name;
        });

    if (it == m_properties.end() || !it->isEditable) {
        return;
    }

    // 更新属性值
    CString oldValue = it->value;
    it->value = value;

    // 应用到实体
    if (name == _T("Position")) {
        // 解析并设置位置
        auto& transform = m_selectedEntity.GetComponent<TransformComponent>();
        // 格式: "x,y,z"
        float x, y, z;
        if (_stscanf_s(value, _T("%f,%f,%f"), &x, &y, &z) == 3) {
            transform.position = { x, y, z };
            NotifyPropertyChanged(name);
        }
    } else if (name == _T("Rotation")) {
        auto& transform = m_selectedEntity.GetComponent<TransformComponent>();
        float x, y, z;
        if (_stscanf_s(value, _T("%f,%f,%f"), &x, &y, &z) == 3) {
            transform.rotation = { x, y, z };
            NotifyPropertyChanged(name);
        }
    } else if (name == _T("Scale")) {
        auto& transform = m_selectedEntity.GetComponent<TransformComponent>();
        float x, y, z;
        if (_stscanf_s(value, _T("%f,%f,%f"), &x, &y, &z) == 3) {
            transform.scale = { x, y, z };
            NotifyPropertyChanged(name);
        }
    }
    // ... 处理其他属性

    LOG_INFO(_T("Property ") + name + _T(" changed to ") + value);
}

void PropertyPanelViewModel::RefreshProperties() {
    RebuildPropertiesFromEntity();
    NotifySelectionChanged();
}

void PropertyPanelViewModel::ClearSelection() {
    m_selectedEntity = Entity();
    m_properties.clear();
    NotifySelectionChanged();
}

void PropertyPanelViewModel::RebuildPropertiesFromEntity() {
    m_properties.clear();

    if (!m_selectedEntity.IsValid()) {
        return;
    }

    // 实体名称
    PropertyValueItem nameItem;
    nameItem.name = _T("Name");
    nameItem.value = m_selectedEntity.GetName().c_str();
    nameItem.type = _T("string");
    nameItem.isEditable = true;
    m_properties.push_back(nameItem);

    // Transform 组件
    if (m_selectedEntity.HasComponent<TransformComponent>()) {
        auto& transform = m_selectedEntity.GetComponent<TransformComponent>();

        PropertyValueItem posItem;
        posItem.name = _T("Position");
        posItem.type = _T("Vector3");
        posItem.isEditable = true;
        posItem.value.Format(_T("%.2f,%.2f,%.2f"),
            transform.position.x, transform.position.y, transform.position.z);
        m_properties.push_back(posItem);

        // 类似地处理 Rotation 和 Scale...
    }

    // Render 组件
    if (m_selectedEntity.HasComponent<RenderComponent>()) {
        auto& render = m_selectedEntity.GetComponent<RenderComponent>();
        
        PropertyValueItem visItem;
        visItem.name = _T("Visible");
        visItem.type = _T("bool");
        visItem.isEditable = true;
        visItem.value = render.isVisible ? _T("true") : _T("false");
        m_properties.push_back(visItem);
    }
}

void PropertyPanelViewModel::NotifyPropertyChanged(const CString& propertyName) {
    if (m_propertyChangedCallback) {
        m_propertyChangedCallback(propertyName);
    }
}

void PropertyPanelViewModel::NotifySelectionChanged() {
    if (m_selectionChangedCallback) {
        m_selectionChangedCallback();
    }
}
```

---

### 任务 2.2：重构 AcadPropertyPanel

#### 目标
改用 ViewModel，支持属性编辑

#### 实施步骤

修改 `AcadPropertyPanel.h`:

```cpp
#pragma once

#include "UIComponents.h"
#include "viewmodels/PropertyPanelViewModel.h"

/**
 * @brief 现代化属性面板 - 使用 ViewModel
 */
class CAcadPropertyPanel : public CUnifiedPanel {
    DECLARE_DYNAMIC(CAcadPropertyPanel)

public:
    CAcadPropertyPanel();
    virtual ~CAcadPropertyPanel() override;

    /**
     * @brief 创建属性面板
     */
    BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

    /**
     * @brief 设置选中的实体
     */
    void SetSelectedEntity(Entity entity);

    /**
     * @brief 获取 ViewModel（用于数据绑定）
     */
    PropertyPanelViewModel* GetViewModel() { return m_viewModel.get(); }

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnDraw(CDC* pDC) override;

private:
    std::unique_ptr<PropertyPanelViewModel> m_viewModel;

    /**
     * @brief 绑定 ViewModel 事件
     */
    void BindViewModelEvents();

    /**
     * @brief 更新 UI（从 ViewModel）
     */
    void UpdateUIFromViewModel();
};
```

详见下一部分的完整实现...

---

## 任务检查和验收标准

### 第一阶段验收标准
- [ ] ServiceContainer 可以正常注册和获取服务
- [ ] 命令系统可以执行、撤销、重做
- [ ] 日志系统记录所有 UI 事件
- [ ] 编译无错误和警告
- [ ] 所有新代码都有详细注释

### 第二阶段验收标准  
- [ ] ViewModel 独立于 UI 层
- [ ] 属性可以在面板中编辑
- [ ] 编辑操作支持撤销/重做
- [ ] 属性变化事件正确传播
- [ ] 所有编辑操作都被记录

---

## 下一步行动

1. **立即**：选择一个开发者开始实施 Task 1.1（服务容器）
2. **并行**：另一个开发者开始 Task 1.2（命令系统）
3. **第 2 周**：完成第一阶段所有任务
4. **第 3 周**：开始第二阶段（ViewModel 和 UI 重构）

**预期结果**：打造一个高质量、易维护的 UI 架构
