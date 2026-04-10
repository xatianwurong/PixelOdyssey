#include "GLDrawApp.h"
#include "mainframe/PixelMainFrame.h"
#include "document/PixelDocument.h"
#include "view/PixelView.h"
#include "./../ui/resources/Resource.h"

// The one and only CGLDrawApp object
CGLDrawApp theApp;

BEGIN_MESSAGE_MAP(CGLDrawApp, CWinAppEx)
  ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
  ON_COMMAND(ID_FILE_EXIT, &CWinAppEx::OnAppExit)
END_MESSAGE_MAP()

CGLDrawApp::CGLDrawApp()
  : m_pDocTemplate(nullptr)
  , m_pMainFrame(nullptr)
{}

BOOL CGLDrawApp::InitInstance()
{
  // 初始化基类
  CWinAppEx::InitInstance();

  // 启用管理器
  SetRegistryKey(_T("PixelOdyssey"));

  // 创建单文档模板
  m_pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CPixelDocument),
    RUNTIME_CLASS(CPixelMainFrame),
    RUNTIME_CLASS(CPixelView));

  if (!m_pDocTemplate)
  {
    TRACE(_T("ERROR: Failed to create document template\n"));
    return FALSE;
  }

  AddDocTemplate(m_pDocTemplate);

  // 解析命令行参数
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // 处理命令行
  if (!ProcessShellCommand(cmdInfo))
  {
    TRACE(_T("WARNING: ProcessShellCommand failed\n"));
    return FALSE;
  }

  m_pMainFrame = DYNAMIC_DOWNCAST(CPixelMainFrame, m_pMainWnd);

  // 主窗口已初始化，显示并更新
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();

  return TRUE;
}

int CGLDrawApp::ExitInstance()
{
  // 清理资源
  if (m_pDocTemplate)
  {
    delete m_pDocTemplate;
    m_pDocTemplate = nullptr;
  }

  return CWinAppEx::ExitInstance();
}
