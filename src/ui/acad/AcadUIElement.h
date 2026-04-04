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
