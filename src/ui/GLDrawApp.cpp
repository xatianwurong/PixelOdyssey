#include "GLDrawApp.h"
#include "acad/AcadMainWindow.h"

// The one and only CGLDrawApp object
CGLDrawApp theApp;

BEGIN_MESSAGE_MAP(CGLDrawApp, CWinApp)
END_MESSAGE_MAP()

CGLDrawApp::CGLDrawApp()
{}

BOOL CGLDrawApp::InitInstance()
{
  CWinApp::InitInstance();

  CAcadMainWindow* pFrame = new CAcadMainWindow();
  if (!pFrame)
  {
    return FALSE;
  }

  m_pMainWnd = pFrame;
  if (!pFrame->Create(NULL,
    _T("PixelOdyssey"),
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CRect(100, 100, 1500, 1000),
    NULL, NULL))
  {
    delete pFrame;
    return FALSE;
  }

  pFrame->ShowWindow(SW_SHOW);
  pFrame->UpdateWindow();

  return TRUE;
}
