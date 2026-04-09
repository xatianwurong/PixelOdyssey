#include "GLDrawApp.h"
#include "mainframe/PixelMainFrame.h"
#include "childframe/PixelChildFrame.h"
#include "document/PixelDocument.h"
#include "view/PixelView.h"
#include "./../ui/resources/Resource.h"

// The one and only CGLDrawApp object
CGLDrawApp theApp;

BEGIN_MESSAGE_MAP(CGLDrawApp, CWinAppEx)
  ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
  ON_COMMAND(ID_APP_EXIT, &CWinAppEx::OnAppExit)
END_MESSAGE_MAP()

CGLDrawApp::CGLDrawApp()
  : m_pDocTemplate(nullptr)
  , m_pMainFrame(nullptr)
{
}

BOOL CGLDrawApp::InitInstance()
{
  // 初始化基类
  CWinAppEx::InitInstance();

  // 启用管理器
  SetRegistryKey(_T("PixelOdyssey"));

  // 创建多文档模板
  m_pDocTemplate = new CMultiDocTemplate(
    IDR_PIXELTYPE,
    RUNTIME_CLASS(CPixelDocument),
    RUNTIME_CLASS(CPixelChildFrame),
    RUNTIME_CLASS(CPixelView));

  if (!m_pDocTemplate)
    return FALSE;

  AddDocTemplate(m_pDocTemplate);

  // 创建 MDI 主框架窗口
  m_pMainFrame = new CPixelMainFrame();
  if (!m_pMainFrame)
    return FALSE;

  if (!m_pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    delete m_pMainFrame;
    return FALSE;
  }

  m_pMainWnd = m_pMainFrame;

  // 解析命令行参数
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // 根据命令行执行操作
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

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
    //RemoveDocTemplate(m_pDocTemplate);
    delete m_pDocTemplate;
    m_pDocTemplate = nullptr;
  }

  return CWinAppEx::ExitInstance();
}
