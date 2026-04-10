#include "PixelMainFrame.h"
#include "../resources/Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPixelMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CPixelMainFrame, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_CLOSE()
END_MESSAGE_MAP()

CPixelMainFrame::CPixelMainFrame()
{
}

CPixelMainFrame::~CPixelMainFrame()
{
}

BOOL CPixelMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CFrameWnd::PreCreateWindow(cs))
    return FALSE;

  // 设置窗口样式和位置
  cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  cs.style &= ~FWS_PREFIXTITLE;
  cs.style |= FWS_ADDTOTITLE;

  // 设置初始窗口大小
  cs.cx = 1400;
  cs.cy = 900;

  // 允许子窗口重叠
  cs.style |= WS_CLIPCHILDREN;

  return TRUE;
}

#ifdef _DEBUG
void CPixelMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

#ifndef _WIN32_WCE
void CPixelMainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}
#endif
#endif

int CPixelMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  return 0;
}

void CPixelMainFrame::OnClose()
{
  CFrameWnd::OnClose();
}
