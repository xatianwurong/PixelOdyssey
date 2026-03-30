#pragma once

#include <afxwin.h>

class CGLDrawApp : public CWinApp
{
public:
    CGLDrawApp();

public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

extern CGLDrawApp theApp;