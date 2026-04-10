#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>

/**
 * @brief MDI 子框架窗口类
 * @details 继承自 CMDIChildWnd，管理每个文档的视图
 *          每个子窗口包含一个独立的绘图场景视图
 */
class CPixelChildFrame : public CMDIChildWnd
{
  DECLARE_DYNCREATE(CPixelChildFrame)

public:
  CPixelChildFrame();
  virtual ~CPixelChildFrame();

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
  afx_msg void OnDestroy();
};

