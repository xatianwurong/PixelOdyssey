#include "PixelChildFrame.h"
#include "../acad/UILayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPixelChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CPixelChildFrame, CMDIChildWnd)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_DESTROY()
END_MESSAGE_MAP()

CPixelChildFrame::CPixelChildFrame()
  : m_pUIManager(nullptr)
{
}

CPixelChildFrame::~CPixelChildFrame()
{
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

  // 创建 UI 组件
  if (!CreateUIComponents())
  {
    return -1;
  }

  return 0;
}

void CPixelChildFrame::OnSize(UINT nType, int cx, int cy)
{
  CMDIChildWnd::OnSize(nType, cx, cy);

  // 调整 UI 布局
  LayoutUI(cx, cy);
}

void CPixelChildFrame::OnDestroy()
{
  CMDIChildWnd::OnDestroy();
}

BOOL CPixelChildFrame::CreateUIComponents()
{
  CRect rectClient;
  GetClientRect(&rectClient);

  DWORD dwStyle = WS_CHILD | WS_VISIBLE;

  // 创建工具面板（左侧）
  CRect toolRect = UILayout::GetToolRect(rectClient.Width(), rectClient.Height());
  if (!m_wndToolPalette.Create(_T("Tools"), dwStyle, toolRect, this, 3001))
  {
    return FALSE;
  }

  // 创建属性面板（右侧）
  CRect propRect = UILayout::GetPropertyPanelRect(rectClient.Width(), rectClient.Height());
  if (!m_wndPropertyPanel.Create(_T("Properties"), dwStyle, propRect, this, 3002))
  {
    return FALSE;
  }

  // 命令行已移除，如需使用可取消注释
  // CRect cmdRect = UILayout::GetCommandLineRect(rectClient.Width(), rectClient.Height());
  // if (!m_wndCommandLine.Create(_T("Command"), dwStyle, cmdRect, this, 3004))
  // {
  //   return FALSE;
  // }

  return TRUE;
}

void CPixelChildFrame::LayoutUI(int width, int height)
{
  if (m_wndToolPalette.GetSafeHwnd())
  {
    m_wndToolPalette.MoveWindow(UILayout::GetToolRect(width, height));
  }

  if (m_wndPropertyPanel.GetSafeHwnd())
  {
    m_wndPropertyPanel.MoveWindow(UILayout::GetPropertyPanelRect(width, height));
  }

  // 命令行已移除，如需使用可取消注释
  // if (m_wndCommandLine.GetSafeHwnd())
  // {
  //   m_wndCommandLine.MoveWindow(UILayout::GetCommandLineRect(width, height));
  // }
}
