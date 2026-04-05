#pragma once

#include <afxwin.h>
#include "UILayout.h"

/**
 * @brief UI 工具栏绘制辅助
 * @details 提供统一的工具栏按钮绘制逻辑，支持 tooltip 和图标渲染
 */
namespace UIToolbar {

/**
 * @brief 工具栏按钮状态
 */
enum class ButtonState {
    Normal,     // 正常状态
    Hovered,    // 悬停状态
    Active,     // 激活/选中状态
    Disabled    // 禁用状态
};

/**
 * @brief 绘制简单图标 - 几何形状组合
 * @details 使用基础图形绘制，避免字体兼容性问题
 * @param pDC 设备上下文
 * @param rect 绘制区域
 * @param iconType 图标类型 (0-4 对应不同图标)
 * @param color 图标颜色
 */
void DrawSimpleIcon(CDC* pDC, const CRect& rect, int iconType, COLORREF color);

/**
 * @brief 绘制工具栏按钮
 * @param pDC 设备上下文
 * @param rect 按钮区域
 * @param state 按钮状态
 * @param iconType 图标类型
 * @param bgColor 背景色
 * @param borderColor 边框色
 */
void DrawButton(CDC* pDC, const CRect& rect, ButtonState state, 
                int iconType, COLORREF bgColor, COLORREF borderColor);

/**
 * @brief 绘制工具栏按钮（带标签）
 * @param pDC 设备上下文
 * @param rect 按钮区域
 * @param state 按钮状态
 * @param iconType 图标类型
 * @param label 按钮标签文字
 * @param bgColor 背景色
 * @param borderColor 边框色
 */
void DrawButtonWithLabel(CDC* pDC, const CRect& rect, ButtonState state,
                         int iconType, const CString& label,
                         COLORREF bgColor, COLORREF borderColor);

/**
 * @brief 绘制工具提示（悬停提示）
 * @param pDC 设备上下文
 * @param tooltipRect 提示框区域
 * @param text 提示文本
 * @param bgColor 背景色
 * @param textColor 文字色
 */
void DrawTooltip(CDC* pDC, const CRect& tooltipRect, const CString& text,
                 COLORREF bgColor, COLORREF textColor);

} // namespace UIToolbar
