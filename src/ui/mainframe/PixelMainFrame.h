#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>

/**
 * @brief MDI 主框架窗口类
 * @details 继承自 CMDIFrameWnd，管理整个应用程序的 MDI 界面
 *          包含菜单栏、工具栏和状态栏
 */
class CPixelMainFrame : public CMDIFrameWnd
{
  DECLARE_DYNAMIC(CPixelMainFrame)

public:
  CPixelMainFrame();
  virtual ~CPixelMainFrame();

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
  virtual void AssertValid() const;
#ifndef _WIN32_WCE
  virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnClose();

private:
  // UI 组件
  CToolBar m_wndToolBar;      ///< 工具栏
  CStatusBar m_wndStatusBar;  ///< 状态栏

  /**
   * @brief 创建工具栏和状态栏
   */
  BOOL CreateBars();
};
