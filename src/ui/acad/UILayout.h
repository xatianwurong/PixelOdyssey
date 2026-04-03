#pragma once

#include <afxwin.h>

/**
 * @brief UI 布局常量定义
 * @details 统一管理所有 UI 组件的尺寸和间距
 */
namespace UILayout {
  // 主要尺寸常量
  constexpr int TOOL_WIDTH = 60;           // 左侧工具栏宽度
  constexpr int PANEL_WIDTH = 250;         // 右侧面板宽度
  constexpr int COMMAND_HEIGHT = 150;      // 底部命令行高度

  // 间距常量
  constexpr int MARGIN_LEFT = 0;           // 左边距
  constexpr int MARGIN_TOP = 0;            // 上边距
  constexpr int MARGIN_RIGHT = 0;          // 右边距
  constexpr int MARGIN_BOTTOM = 0;         // 下边距

  // OpenGL 渲染区域自动计算
  inline int GetOpenGLX() { return TOOL_WIDTH; }
  inline int GetOpenGLY() { return MARGIN_TOP; }
  inline int GetOpenGLWidth(int clientWidth) { return clientWidth - TOOL_WIDTH - PANEL_WIDTH; }
  inline int GetOpenGLHeight(int clientHeight) { return clientHeight - COMMAND_HEIGHT; }

  // 工具栏区域
  inline CRect GetToolRect(int clientWidth, int clientHeight) {
    return CRect(0, 0, TOOL_WIDTH, clientHeight);
  }

  // 属性面板区域
  inline CRect GetPropertyPanelRect(int clientWidth, int clientHeight) {
    return CRect(clientWidth - PANEL_WIDTH, 0,
      clientWidth, clientHeight - COMMAND_HEIGHT);
  }

  // 命令行区域
  inline CRect GetCommandLineRect(int clientWidth, int clientHeight) {
    return CRect(TOOL_WIDTH, clientHeight - COMMAND_HEIGHT,
      clientWidth - PANEL_WIDTH, clientHeight);
  }

  // OpenGL 渲染区域
  inline CRect GetOpenGLRect(int clientWidth, int clientHeight) {
    return CRect(TOOL_WIDTH, 0,
      clientWidth - PANEL_WIDTH,
      clientHeight - COMMAND_HEIGHT);
  }
}
