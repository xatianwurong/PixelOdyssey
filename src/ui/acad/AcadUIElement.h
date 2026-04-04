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

  // 颜色定义 - 优化的 AutoCAD 风格深色主题
  static COLORREF ms_colorBackground;      // 背景色 #1E1E1E
  static COLORREF ms_colorPanelBg;         // 面板背景 #252525
  static COLORREF ms_colorAccent;          // 强调色 #0078D4 (Visual Studio 蓝)
  static COLORREF ms_colorAccentHover;     // 强调色悬停 #1E90FF
  static COLORREF ms_colorText;            // 文字颜色 #F0F0F0
  static COLORREF ms_colorTextMuted;       // 淡文字 #808080
  static COLORREF ms_colorBorder;          // 边框色 #3E3E42
  static COLORREF ms_colorHighlight;       // 高亮色 #FFA500 (橙色)
  static COLORREF ms_colorSuccess;         // 成功色 #00C853
  static COLORREF ms_colorWarning;         // 警告色 #FFB300
  static COLORREF ms_colorError;           // 错误色 #F44336

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
