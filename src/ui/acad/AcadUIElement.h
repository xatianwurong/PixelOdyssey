#pragma once
#include <afxwin.h>
#include <vector>
#include <string>
#include <functional>

/**
 * @brief AutoCAD 风格 UI 基类
 * @details 提供现代化的扁平 UI 设计，类似 AutoCAD 的界面风格
 */
class CAcadUIElement : public CWnd {
  DECLARE_DYNAMIC(CAcadUIElement)

public:
  CAcadUIElement();
  virtual ~CAcadUIElement();

  // 颜色定义
  static COLORREF ms_colorBackground;      // 背景色 #2C3E50
  static COLORREF ms_colorPanelBg;         // 面板背景 #34495E
  static COLORREF ms_colorAccent;          // 强调色 #3498DB
  static COLORREF ms_colorAccentHover;     // 强调色悬停 #2980B9
  static COLORREF ms_colorText;            // 文字颜色 #ECF0F1
  static COLORREF ms_colorTextMuted;       // 淡文字 #95A5A6
  static COLORREF ms_colorBorder;          // 边框色 #4A6FA5
  static COLORREF ms_colorHighlight;       // 高亮色 #E74C3C

  /**
   * @brief 创建 UI 元素
   */
  virtual BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

  /**
   * @brief 设置是否可见
   */
  virtual void SetVisible(bool visible);

  /**
   * @brief 更新 UI
   */
  virtual void Update() {}

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnSize(UINT nType, int cx, int cy);

  /**
   * @brief 绘制背景
   */
  virtual void DrawBackground(CDC* pDC, const CRect& rect);

  /**
   * @brief 绘制边框
   */
  virtual void DrawBorder(CDC* pDC, const CRect& rect);

  /**
   * @brief 绘制内容（子类实现）
   */
  virtual void OnDraw(CDC* pDC) = 0;

  bool m_bVisible;
  CFont m_fontTitle;
  CFont m_fontNormal;
  CFont m_fontSmall;
  CRect m_rectClient;
};
