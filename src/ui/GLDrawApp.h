#pragma once

#include <afxwin.h>

/**
 * @class CGLDrawApp
 * @brief MFC应用程序类
 * @details 应用程序的入口点，负责初始化应用程序和创建主窗口
 * @功能描述 管理应用程序的生命周期，初始化应用程序环境，创建主窗口
 * @设计目的 作为MFC应用程序的核心类，提供应用程序级别的功能
 * @使用场景 应用程序启动时自动创建，是整个应用的入口点
 * @关键实现 继承自CWinApp，重写InitInstance方法实现应用程序初始化
 */
class CGLDrawApp : public CWinApp
{
public:
    CGLDrawApp();

public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

extern CGLDrawApp theApp;