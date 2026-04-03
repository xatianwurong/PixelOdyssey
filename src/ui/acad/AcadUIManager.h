#pragma once
#include "AcadToolPalette.h"
#include "AcadPropertyPanel.h"
#include "AcadCommandLine.h"
#include "../../tools/Tool.h"

/**
 * @brief AutoCAD 风格 UI 管理器
 * @details 统一管理所有 UI 组件的布局和交互
 */
class CAcadUIManager {
public:
  static CAcadUIManager& Instance() {
    static CAcadUIManager instance;
    return instance;
  }

  /**
   * @brief 初始化 UI
   */
  BOOL Initialize(CWnd* pParent);

  /**
   * @brief 调整布局
   */
  void Layout(int width, int height);

  /**
   * @brief 更新 UI
   */
  void Update();

  /**
   * @brief 获取工具面板
   */
  CAcadToolPalette& GetToolPalette() { return m_toolPalette; }

  /**
   * @brief 获取属性面板
   */
  CAcadPropertyPanel& GetPropertyPanel() { return m_propertyPanel; }


  /**
   * @brief 获取命令行
   */
  CAcadCommandLine& GetCommandLine() { return m_commandLine; }

  /**
   * @brief 设置当前工具
   */
  void SetActiveTool(ToolType type);

  /**
   * @brief 获取当前工具
   */
  ToolType GetActiveTool() const { return m_toolPalette.GetActiveTool(); }

  /**
   * @brief 获取工具栏宽度
   */
  int GetToolWidth() const { return m_toolWidth; }

  /**
   * @brief 获取面板宽度
   */
  int GetPanelWidth() const { return m_panelWidth; }

  /**
   * @brief 获取命令行高度
   */
  int GetCommandHeight() const { return m_commandHeight; }

private:
  CAcadUIManager() = default;
  ~CAcadUIManager() = default;

  CAcadUIManager(const CAcadUIManager&) = delete;
  CAcadUIManager& operator=(const CAcadUIManager&) = delete;

  CWnd* m_pParent;

  // UI 组件 - 精简版（移除图层面板）
  CAcadToolPalette m_toolPalette;
  CAcadPropertyPanel m_propertyPanel;
  CAcadCommandLine m_commandLine;

  // 布局尺寸（从 UILayout.h 获取）
  int m_toolWidth;
  int m_panelWidth;
  int m_commandHeight;
};
