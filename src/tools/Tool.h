#pragma once
#include "../core/event/Event.h"
#include <memory>
#include <string>

/**
 * @brief 工具类型枚举
 * @details 定义编辑器中所有可用的工具类型
 */
enum class ToolType
{
  None = 0,       ///< 无工具
  Select,         ///< 选择工具
  Move,           ///< 移动工具
  Rotate,         ///< 旋转工具
  Scale,          ///< 缩放工具
  CreateShape,    ///< 创建形状工具
  CreateText,     ///< 创建文本工具
  Pan,            ///< 平移工具
  Orbit           ///< 环绕工具
};

/**
 * @brief 工具基类
 * @details 所有工具必须继承此类
 * @details 提供工具的生命周期管理和事件处理接口
 * @功能描述 定义编辑器的交互式工具接口
 * @设计目的 实现工具模式的可扩展架构
 * @使用场景 选择、移动、旋转、缩放等编辑器操作
 * @关键实现 使用虚函数接口，支持多态和动态切换
 */
class Tool
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~Tool() = default;

  /**
   * @brief 获取工具类型
   * @return 工具类型枚举值
   */
  virtual ToolType GetType() const = 0;

  /**
   * @brief 获取工具名称
   * @return 工具名称字符串
   */
  virtual std::string GetName() const = 0;

  /**
   * @brief 工具激活时调用
   * @details 可重写此方法进行初始化操作
   */
  virtual void OnActivate() {}

  /**
   * @brief 工具停用时调用
   * @details 可重写此方法进行清理操作
   */
  virtual void OnDeactivate() {}

  /**
   * @brief 处理事件
   * @param event 事件对象引用
   * @return 如果事件已处理返回 true，否则返回 false
   */
  virtual bool HandleEvent(Event& event)
  {
    return false;
  }

  /**
   * @brief 每帧更新
   * @param deltaTime 帧间隔时间（秒）
   */
  virtual void Update(float deltaTime) {}

  /**
   * @brief 渲染工具 UI
   * @details 可重写此方法绘制工具相关的 UI 元素
   */
  virtual void Render() {}
};

/**
 * @brief 工具管理器
 * @details 管理所有工具的切换和使用
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @功能描述 管理工具的激活、停用和事件分发
 * @设计目的 提供统一的工具管理平台
 * @使用场景 编辑器工具切换和管理
 * @关键实现 单例模式，使用 unique_ptr 管理工具生命周期
 */
class ToolManager
{
public:
  /**
   * @brief 获取工具管理器单例实例
   * @return 工具管理器实例引用
   */
  static ToolManager& Instance()
  {
    static ToolManager instance;
    return instance;
  }

  /**
   * @brief 设置当前工具
   * @param tool 工具对象的唯一指针
   * @details 自动调用旧工具的 OnDeactivate 和新工具的 OnActivate
   */
  void SetTool(std::unique_ptr<Tool> tool)
  {
    if (m_currentTool)
    {
      m_currentTool->OnDeactivate();
    }

    m_currentTool = std::move(tool);

    if (m_currentTool)
    {
      m_currentTool->OnActivate();
    }
  }

  /**
   * @brief 获取当前工具
   * @return 当前工具指针（可能为 nullptr）
   */
  Tool* GetCurrentTool() const
  {
    return m_currentTool.get();
  }

  /**
   * @brief 处理事件
   * @param event 事件对象引用
   * @return 如果事件已处理返回 true，否则返回 false
   */
  bool HandleEvent(Event& event)
  {
    if (m_currentTool)
    {
      return m_currentTool->HandleEvent(event);
    }
    return false;
  }

  /**
   * @brief 更新工具
   * @param deltaTime 帧间隔时间（秒）
   */
  void Update(float deltaTime)
  {
    if (m_currentTool)
    {
      m_currentTool->Update(deltaTime);
    }
  }

  /**
   * @brief 渲染工具
   * @details 调用当前工具的 Render 方法
   */
  void Render()
  {
    if (m_currentTool)
    {
      m_currentTool->Render();
    }
  }

private:
  /**
   * @brief 私有构造函数
   */
  ToolManager() = default;

  std::unique_ptr<Tool> m_currentTool;  ///< 当前激活的工具
};
