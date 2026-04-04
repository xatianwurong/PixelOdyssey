#pragma once

#include <afxwin.h>

/**
 * @brief UI 布局常量定义
 * @details 统一管理所有 UI 组件的尺寸和间距
 */
namespace UILayout {
  // 主要尺寸常量 - 优化空间分配
  constexpr int TOOL_WIDTH = 50;           // 左侧工具栏宽度（减小以节省空间）
  constexpr int PANEL_WIDTH = 280;         // 右侧面板宽度（增加以显示更多信息）
  constexpr int COMMAND_HEIGHT = 180;      // 底部命令行高度（增加以显示更多历史）
  
  // 间距常量
  constexpr int MARGIN_LEFT = 0;
  constexpr int MARGIN_TOP = 0;
  constexpr int MARGIN_RIGHT = 0;
  constexpr int MARGIN_BOTTOM = 0;

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
}
