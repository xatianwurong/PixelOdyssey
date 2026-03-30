#include "GLDrawApp.h"
#include "windows/MainWindow.h"
#include "utils/OpenGLLogger.h"

// The one and only CGLDrawApp object
CGLDrawApp theApp;

BEGIN_MESSAGE_MAP(CGLDrawApp, CWinApp)
END_MESSAGE_MAP()

CGLDrawApp::CGLDrawApp()
{}

BOOL CGLDrawApp::InitInstance()
{
  CWinApp::InitInstance();

  LOG(_T("[App] InitInstance called"));
  
  CMainWindow* pFrame = new CMainWindow();
  if (!pFrame)
  {
    LOG(_T("[App] ERROR: Failed to create main window"));
    return FALSE;
  }
  
  LOG(_T("[App] Main window created successfully"));

  m_pMainWnd = pFrame;
  LOG(_T("[App] Creating main window frame"));
  if (!pFrame->Create(NULL,
    _T("OpenGL MFC Application - Modern Renderer"),
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CRect(0, 0, 1200, 750),
    NULL, NULL))
  {
    LOG(_T("[App] ERROR: Failed to create main window frame"));
    delete pFrame;
    return FALSE;
  }
  LOG(_T("[App] Main window frame created successfully"));

  pFrame->ShowWindow(SW_SHOW);
  pFrame->UpdateWindow();

  LOG(_T("[App] InitInstance completed successfully"));

  return TRUE;
}
