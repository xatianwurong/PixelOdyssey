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

  //LOG(_T("[App] InitInstance called with AutoCAD Style UI"));
  
  // 创建 AutoCAD 风格的主窗口
  CAcadMainWindow* pFrame = new CAcadMainWindow();
  if (!pFrame)
  {
    //LOG(_T("[App] ERROR: Failed to create AutoCAD style main window"));
    return FALSE;
  }
  
  //LOG(_T("[App] AutoCAD style main window created successfully"));

  m_pMainWnd = pFrame;
  //LOG(_T("[App] Creating AutoCAD style main window frame"));
  if (!pFrame->Create(NULL,
    _T("PixelOdyssey - AutoCAD Style UI"),
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CRect(100, 100, 1500, 1000),
    NULL, NULL))
  {
    //LOG(_T("[App] ERROR: Failed to create AutoCAD style main window frame"));
    delete pFrame;
    return FALSE;
  }
  //LOG(_T("[App] AutoCAD style main window frame created successfully"));

  pFrame->ShowWindow(SW_SHOW);
  pFrame->UpdateWindow();

  //LOG(_T("[App] InitInstance completed successfully - AutoCAD Style UI Ready"));

  return TRUE;
}
