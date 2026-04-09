#pragma once
#include "AcadToolPalette.h"
#include "AcadPropertyPanel.h"
#include "AcadCommandLine.h"
#include "../../tools/Tool.h"

/**
 * @brief AutoCAD 风格 UI 管理器类
 * @brief 统一管理所有 UI 组件的布局和交互
 * @details 单例模式，管理工具面板、属性面板和命令行
 * @details 提供统一的布局管理和更新机制
 * @功能描述 管理 AutoCAD 风格的 UI 界面
 * @设计目的 实现 UI 组件的集中管理和协调
 * @使用场景 AutoCAD 风格编辑器界面
 * @关键实现 单例模式，组合多个 UI 组件
 */
class CAcadUIManager
{
public:
  /**
   * @brief 获取单例实例
   * @return CAcadUIManager 实例引用
   */
  static CAcadUIManager& Instance()
  {
    static CAcadUIManager instance;
    return instance;
  }

  /**
   * @brief 初始化 UI
   * @param pParent 父窗口指针
   * @return BOOL 是否成功
   * @details 创建并初始化所有 UI 组件
   */
  BOOL Initialize(CWnd* pParent);

  /**
   * @brief 调整布局
   * @param width 窗口宽度
   * @param height 窗口高度
   * @details 根据窗口尺寸重新布局所有 UI 组件
   */
  void Layout(int width, int height);

  /**
   * @brief 更新 UI
   * @details 更新所有 UI 组件的状态
   */
  void Update();

  /**
   * @brief 获取工具面板
   * @return CAcadToolPalette 引用
   */
  CAcadToolPalette& GetToolPalette()
  {
    return m_toolPalette;
  }

  /**
   * @brief 获取属性面板
   * @return CAcadPropertyPanel 引用
   */
  CAcadPropertyPanel& GetPropertyPanel()
  {
    return m_propertyPanel;
  }

  /**
   * @brief 获取命令行
   * @return CAcadCommandLine 引用
   */
  CAcadCommandLine& GetCommandLine()
  {
    return m_commandLine;
  }

  /**
   * @brief 设置当前工具
   * @param type 工具类型
   * @details 激活指定的工具
   */
  void SetActiveTool(ToolType type);

  /**
   * @brief 获取当前工具
   * @return 当前工具类型
   */
  ToolType GetActiveTool() const
  {
    return m_toolPalette.GetActiveTool();
  }

  /**
   * @brief 获取工具栏宽度
   * @return 工具栏宽度（像素）
   */
  int GetToolWidth() const
  {
    return m_toolWidth;
  }

  /**
   * @brief 获取面板宽度
   * @return 面板宽度（像素）
   */
  int GetPanelWidth() const
  {
    return m_panelWidth;
  }

  /**
   * @brief 获取命令行高度
   * @return 命令行高度（像素）
   */
  int GetCommandHeight() const
  {
    return m_commandHeight;
  }

private:
  CAcadUIManager() = default;  ///< 私有构造函数（单例模式）
  ~CAcadUIManager() = default; ///< 析构函数

  CAcadUIManager(const CAcadUIManager&) = delete;  ///< 禁用拷贝构造
  CAcadUIManager& operator=(const CAcadUIManager&) = delete;  ///< 禁用赋值操作

  CWnd* m_pParent;  ///< 父窗口指针

  // UI 组件 - 精简版（移除图层面板）
  CAcadToolPalette m_toolPalette;     ///< 工具面板
  CAcadPropertyPanel m_propertyPanel; ///< 属性面板
  CAcadCommandLine m_commandLine;     ///< 命令行

  // 布局尺寸（从 UILayout.h 获取）
  int m_toolWidth;      ///< 工具栏宽度
  int m_panelWidth;     ///< 面板宽度
  int m_commandHeight;  ///< 命令行高度
};
