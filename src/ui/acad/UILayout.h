#pragma once

#include <afxwin.h>

/**
 * @brief UI 布局常量定义
 * @details 统一管理所有 UI 组件的尺寸和间距，使用语义化命名
 */
namespace UILayout {
  // 主要尺寸常量 - 优化空间分配 (从 UISizes 获取)
  constexpr int TOOL_WIDTH = 56;           // 左侧工具栏宽度
  constexpr int PANEL_WIDTH = 300;         // 右侧面板宽度
  constexpr int COMMAND_HEIGHT = 160;      // 底部命令行高度
  
  // 间距常量 (基于 4px 基准)
  constexpr int MARGIN_LEFT = 0;
  constexpr int MARGIN_TOP = 0;
  constexpr int MARGIN_RIGHT = 0;
  constexpr int MARGIN_BOTTOM = 0;
  constexpr int GAP = 8;                   // 组件间距

  // OpenGL 渲染区域自动计算
  inline int GetOpenGLX() { return TOOL_WIDTH; }
  inline int GetOpenGLY() { return MARGIN_TOP; }
  inline int GetOpenGLWidth(int clientWidth) { return clientWidth - TOOL_WIDTH - PANEL_WIDTH; }
  inline int GetOpenGLHeight(int clientHeight) { return clientHeight - COMMAND_HEIGHT; }

  // 工具栏区域
  inline CRect GetToolRect(int clientWidth, int clientHeight) {
    return CRect(0, 0, TOOL_WIDTH, clientHeight);
  }

  // 属性面板区域 - 占满右侧
  inline CRect GetPropertyPanelRect(int clientWidth, int clientHeight) {
    return CRect(clientWidth - PANEL_WIDTH, 0, 
      clientWidth, clientHeight - COMMAND_HEIGHT);
  }
  
  // 命令行区域 - 占满底部
  inline CRect GetCommandLineRect(int clientWidth, int clientHeight) {
    return CRect(TOOL_WIDTH, clientHeight - COMMAND_HEIGHT,
      clientWidth, clientHeight);
  }
  
  // OpenGL 渲染区域 - 最大化利用
  inline CRect GetOpenGLRect(int clientWidth, int clientHeight) {
    return CRect(TOOL_WIDTH, 0, 
      clientWidth - PANEL_WIDTH, 
      clientHeight - COMMAND_HEIGHT);
  }
  
  // 最小窗口尺寸限制
  constexpr int MIN_WINDOW_WIDTH = 1024;
  constexpr int MIN_WINDOW_HEIGHT = 768;

  /**
   * @brief 计算响应式布局
   * @details 根据窗口大小自动调整布局参数
   */
  struct LayoutMetrics {
    int toolWidth;
    int panelWidth;
    int commandHeight;
    CRect openGLRect;
    CRect toolRect;
    CRect propertyRect;
    CRect commandRect;

    static LayoutMetrics Calculate(int clientWidth, int clientHeight) {
      LayoutMetrics metrics;
      
      // 小屏幕适配
      bool isSmallScreen = clientWidth < 1200 || clientHeight < 800;
      
      if (isSmallScreen) {
        metrics.toolWidth = 48;
        metrics.panelWidth = 240;
        metrics.commandHeight = 120;
      } else {
        metrics.toolWidth = TOOL_WIDTH;
        metrics.panelWidth = PANEL_WIDTH;
        metrics.commandHeight = COMMAND_HEIGHT;
      }

      metrics.toolRect = CRect(0, 0, metrics.toolWidth, clientHeight);
      metrics.propertyRect = CRect(clientWidth - metrics.panelWidth, 0,
        clientWidth, clientHeight - metrics.commandHeight);
      metrics.commandRect = CRect(metrics.toolWidth, clientHeight - metrics.commandHeight,
        clientWidth, clientHeight);
      metrics.openGLRect = CRect(metrics.toolWidth, 0,
        clientWidth - metrics.panelWidth,
        clientHeight - metrics.commandHeight);

      return metrics;
    }
  };
}
