#pragma once

#include <afxwin.h>

/**
 * @brief UI 布局常量定义
 * @details 统一管理所有 UI 组件的尺寸和间距，使用语义化命名
 *          基于现代 UI 设计原则：8px 网格系统、响应式布局
 */
namespace UILayout {
  // =========================================================================
  // 主要尺寸常量 - 优化空间分配
  // =========================================================================
  constexpr int TOOL_WIDTH = 80;           // 左侧工具栏宽度
  constexpr int PANEL_WIDTH = 320;         // 右侧面板宽度
  constexpr int COMMAND_HEIGHT = 0;        // 底部命令行高度 (已移除)

  // =========================================================================
  // 间距常量 - 基于 8px 基准网格系统
  // =========================================================================
  constexpr int MARGIN_LEFT = 0;
  constexpr int MARGIN_TOP = 0;
  constexpr int MARGIN_RIGHT = 0;
  constexpr int MARGIN_BOTTOM = 0;
  constexpr int GAP = 8;                   // 组件间距

  // =========================================================================
  // OpenGL 渲染区域自动计算
  // =========================================================================
  inline int GetOpenGLX() { return TOOL_WIDTH; }
  inline int GetOpenGLY() { return MARGIN_TOP; }
  inline int GetOpenGLWidth(int clientWidth) { return clientWidth - TOOL_WIDTH - PANEL_WIDTH; }
  inline int GetOpenGLHeight(int clientHeight) { return clientHeight - COMMAND_HEIGHT; }

  // =========================================================================
  // 工具栏区域
  // =========================================================================
  inline CRect GetToolRect(int clientWidth, int clientHeight) {
    return CRect(0, 0, TOOL_WIDTH, clientHeight);
  }

  // =========================================================================
  // 属性面板区域 - 占满右侧
  // =========================================================================
  inline CRect GetPropertyPanelRect(int clientWidth, int clientHeight) {
    return CRect(clientWidth - PANEL_WIDTH, 0,
      clientWidth, clientHeight - COMMAND_HEIGHT);
  }

  // =========================================================================
  // 命令行区域 - 已移除
  // =========================================================================
  inline CRect GetCommandLineRect(int clientWidth, int clientHeight) {
    return CRect(0, clientHeight, 0, clientHeight);  // 空矩形
  }

  // =========================================================================
  // OpenGL 渲染区域 - 最大化利用
  // =========================================================================
  inline CRect GetOpenGLRect(int clientWidth, int clientHeight) {
    return CRect(TOOL_WIDTH, 0,
      clientWidth - PANEL_WIDTH,
      clientHeight - COMMAND_HEIGHT);
  }

  // =========================================================================
  // 最小窗口尺寸限制
  // =========================================================================
  constexpr int MIN_WINDOW_WIDTH = 1200;   // 提高最小宽度要求
  constexpr int MIN_WINDOW_HEIGHT = 800;   // 提高最小高度要求

  // =========================================================================
  // 响应式布局计算
  // =========================================================================
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

      // 小屏幕适配 (< 1200x800)
      bool isSmallScreen = clientWidth < 1400 || clientHeight < 900;
      // 超小屏幕适配 (< 1000x700)
      bool isExtraSmall = clientWidth < 1200 || clientHeight < 800;

      if (isExtraSmall) {
        metrics.toolWidth = 52;
        metrics.panelWidth = 260;
        metrics.commandHeight = 0;  // 已移除
      }
      else if (isSmallScreen) {
        metrics.toolWidth = 56;
        metrics.panelWidth = 290;
        metrics.commandHeight = 0;  // 已移除
      }
      else {
        metrics.toolWidth = TOOL_WIDTH;
        metrics.panelWidth = PANEL_WIDTH;
        metrics.commandHeight = COMMAND_HEIGHT;  // 0
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

  // =========================================================================
  // 字体大小配置 - 根据 DPI 缩放
  // =========================================================================
  namespace UIFonts {
    constexpr int TITLE_SIZE = 18;         // 标题字体
    constexpr int HEADING_SIZE = 15;       // 副标题
    constexpr int BODY_SIZE = 13;          // 正文字体
    constexpr int CAPTION_SIZE = 12;       // 说明文字
    constexpr int SMALL_SIZE = 11;         // 小字
    constexpr int MONOSPACE_SIZE = 12;     // 等宽字体 (命令行)
    constexpr int TOOL_ICON_SIZE = 16;     // 工具栏图标字体 (Segoe MDL2)

    constexpr int TITLE_WEIGHT = FW_SEMIBOLD;
    constexpr int HEADING_WEIGHT = FW_MEDIUM;
    constexpr int BODY_WEIGHT = FW_NORMAL;

    inline constexpr const TCHAR* FONT_FAMILY = _T("Microsoft YaHei UI");
    inline constexpr const TCHAR* MONOSPACE_FONT = _T("Consolas");
    inline constexpr const TCHAR* SYMBOL_FONT = _T("Segoe MDL2 Assets");     // Windows 内置符号字体
    inline constexpr const TCHAR* FALLBACK_FONT = _T("SimSun");               // 备用中文字体
  }

  // =========================================================================
  // 按钮和控件尺寸
  // =========================================================================
  namespace Controls {
    constexpr int BUTTON_HEIGHT = 36;
    constexpr int BUTTON_MIN_WIDTH = 80;
    constexpr int ICON_SIZE = 24;
    constexpr int CHECKBOX_SIZE = 18;
  }
}
