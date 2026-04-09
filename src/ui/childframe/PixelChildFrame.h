#pragma once

#include <afxwin.h>
#include "document/PixelDocument.h"
#include "view/PixelView.h"
#include "acad/AcadUIManager.h"

/**
 * @brief MDI 子框架窗口类
 * @details 继承自 CMDIChildWnd，管理每个文档的视图和 UI 组件
 *          每个子窗口包含一个独立的绘图场景和相关的 UI 面板
 */
class CPixelChildFrame : public CMDIChildWnd
{
  DECLARE_DYNCREATE(CPixelChildFrame)

public:
  CPixelChildFrame();
  virtual ~CPixelChildFrame();

#ifdef _DEBUG
  virtual void AssertValid() const;
#ifndef _WIN32_WCE
  virtual void Dump(CDumpContext& dc) const;
#endif
#endif

  // 获取 UI 管理器
  CAcadUIManager* GetUIManager() { return m_pUIManager; }

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnDestroy();

private:
  CAcadUIManager* m_pUIManager;  ///< UI 管理器指针
  
  // UI 组件句柄
  CWnd m_wndToolPalette;      ///< 工具面板
  CWnd m_wndPropertyPanel;    ///< 属性面板
  CWnd m_wndCommandLine;      ///< 命令行

  /**
   * @brief 创建 UI 组件
   */
  BOOL CreateUIComponents();

  /**
   * @brief 调整 UI 布局
   */
  void LayoutUI(int width, int height);
};
