#pragma once

#include <afxwin.h>
#include <afxwinappex.h>

// Forward declarations
class CPixelMainFrame;

/**
 * @brief MFC 应用程序类 - 单文档界面
 * @details 应用程序的入口点，负责初始化应用程序和创建 SDI 主窗口
 * @details 继承自 CWinAppEx，提供现代化的单文档界面支持
 */
class CGLDrawApp : public CWinAppEx
{
public:
  /**
   * @brief 构造函数
   * @details MFC 会自动调用此构造函数创建应用程序实例
   */
  CGLDrawApp();

  /**
   * @brief 初始化实例
   * @return 如果初始化成功返回 TRUE，否则返回 FALSE
   * @details 重写自 CWinAppEx，负责创建 MDI 主窗口和初始化应用程序
   */
  virtual BOOL InitInstance();

  /**
   * @brief 应用程序退出时的清理工作
   * @return 始终返回 0
   */
  virtual int ExitInstance();

  DECLARE_MESSAGE_MAP()

private:
  CSingleDocTemplate* m_pDocTemplate;  ///< 单文档模板
  CPixelMainFrame* m_pMainFrame;       ///< 主框架窗口
};

extern CGLDrawApp theApp;  ///< 全局应用程序实例
