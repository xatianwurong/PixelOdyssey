#pragma once

#include <afxwin.h>

/**
 * @brief MFC 应用程序类
 * @details 应用程序的入口点，负责初始化应用程序和创建主窗口
 * @details 继承自 CWinApp，重写 InitInstance 方法实现应用程序初始化
 * @功能描述 管理应用程序的生命周期，初始化应用程序环境，创建主窗口
 * @设计目的 作为 MFC 应用程序的核心类，提供应用程序级别的功能
 * @使用场景 应用程序启动时自动创建，是整个应用的入口点
 * @关键实现 使用 MFC 框架，通过 InitInstance 实现初始化
 */
class CGLDrawApp : public CWinApp
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
   * @details 重写自 CWinApp，负责创建主窗口和初始化应用程序
   */
  virtual BOOL InitInstance();

  DECLARE_MESSAGE_MAP()
};

extern CGLDrawApp theApp;  ///< 全局应用程序实例