#include "PixelMainFrame.h"
#include "../resources/Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CPixelMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CPixelMainFrame, CMDIFrameWnd)
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
  if (!CMDIFrameWnd::PreCreateWindow(cs))
    return FALSE;

  // 设置窗口样式和位置
  cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
             WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

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
  CMDIFrameWnd::AssertValid();
}

#ifndef _WIN32_WCE
void CPixelMainFrame::Dump(CDumpContext& dc) const
{
  CMDIFrameWnd::Dump(dc);
}
#endif
#endif

int CPixelMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  // 创建工具栏和状态栏
  if (!CreateBars())
  {
    return -1;
  }

  return 0;
}

void CPixelMainFrame::OnClose()
{
  CMDIFrameWnd::OnClose();
}

BOOL CPixelMainFrame::CreateBars()
{
  // 创建工具栏
  if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
  {
    return FALSE;
  }

  // 创建状态栏
  if (!m_wndStatusBar.Create(this))
  {
    return FALSE;
  }

  // 配置状态栏指示器
  static UINT indicators[] =
  {
    ID_SEPARATOR,           // 状态行指示器
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
  };

  if (!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
  {
    return FALSE;
  }

  // 设置面板样式
  m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_STRETCH, 0);
  m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_CAPS, SBPS_NORMAL, 40);
  m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_NUM, SBPS_NORMAL, 40);
  m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_SCRL, SBPS_NORMAL, 40);

  // 停靠设置
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  return TRUE;
}
