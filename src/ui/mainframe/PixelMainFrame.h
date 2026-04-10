#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>

/**
 * @brief 主框架窗口类
 * @details 继承自 CFrameWnd，管理整个应用程序的 SDI 界面
 *          包含菜单栏和状态栏
 */
class CPixelMainFrame : public CFrameWnd
{
  DECLARE_DYNCREATE(CPixelMainFrame)

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
};
