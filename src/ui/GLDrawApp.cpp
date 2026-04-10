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
  {
    TRACE(_T("ERROR: Failed to create document template\n"));
    return FALSE;
  }

  AddDocTemplate(m_pDocTemplate);

  // 创建 MDI 主框架窗口
  m_pMainFrame = new CPixelMainFrame();
  if (!m_pMainFrame)
  {
    TRACE(_T("ERROR: Failed to allocate CPixelMainFrame\n"));
    return FALSE;
  }

  if (!m_pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    TRACE(_T("ERROR: Failed to load frame resources (IDR_MAINFRAME=%d)\n"), IDR_MAINFRAME);
    delete m_pMainFrame;
    m_pMainFrame = nullptr;
    return FALSE;
  }

  m_pMainWnd = m_pMainFrame;

  // 解析命令行参数
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // 处理命令行
  if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew ||
      cmdInfo.m_nShellCommand == CCommandLineInfo::FileNothing)
  {
    // 没有指定文件时创建新文档
    OnFileNew();
  }
  else
  {
    // 尝试打开指定的文件
    if (!ProcessShellCommand(cmdInfo))
    {
      TRACE(_T("WARNING: ProcessShellCommand failed\n"));
      // 不返回FALSE，继续用空文档运行
      OnFileNew();
    }
  }

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
