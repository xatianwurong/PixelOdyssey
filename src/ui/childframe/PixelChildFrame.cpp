#include "PixelChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPixelChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CPixelChildFrame, CMDIChildWnd)
  ON_WM_CREATE()
  ON_WM_DESTROY()
END_MESSAGE_MAP()

CPixelChildFrame::CPixelChildFrame()
{
}

CPixelChildFrame::~CPixelChildFrame()
{
}

BOOL CPixelChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CMDIChildWnd::PreCreateWindow(cs))
    return FALSE;

  // 设置子窗口样式
  cs.style |= WS_CLIPCHILDREN;

  return TRUE;
}

#ifdef _DEBUG
void CPixelChildFrame::AssertValid() const
{
  CMDIChildWnd::AssertValid();
}

#ifndef _WIN32_WCE
void CPixelChildFrame::Dump(CDumpContext& dc) const
{
  CMDIChildWnd::Dump(dc);
}
#endif
#endif

int CPixelChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  return 0;
}

void CPixelChildFrame::OnDestroy()
{
  CMDIChildWnd::OnDestroy();
}

