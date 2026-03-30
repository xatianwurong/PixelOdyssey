#pragma once

#include <afxwin.h>
#include <afxwinappex.h>
#include <afxext.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../resources/Resource.h"

#include "../../core/scene/SceneManager.h"

/**
 * @class CMainWindow
 * @brief 主窗口类
 * @details 应用程序的主窗口，包含OpenGL渲染区域和UI控件
 * @功能描述 管理窗口创建、OpenGL初始化、渲染和用户交互
 * @设计目的 提供应用程序的主界面和用户交互功能
 * @使用场景 作为应用程序的主窗口，处理用户输入和渲染
 * @关键实现 包含OpenGL渲染区域、相机控制、渲染选项等UI控件
 */
class CMainWindow : public CFrameWnd
{
public:
    CMainWindow();
    virtual ~CMainWindow();

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpWindowPos);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    
    afx_msg void OnCameraXChange();
    afx_msg void OnCameraYChange();
    afx_msg void OnCameraZChange();
    afx_msg void OnCheckWireframe();
    afx_msg void OnCheckSmooth();
    afx_msg void OnCheckDepth();
    
    void CreateControls();

    DECLARE_MESSAGE_MAP()

private:
    HGLRC m_hGLRC;
    HWND m_hWndOpenGL;
    CStatic m_staticOpenGL;
    
    CEdit m_editStatus;
    CEdit m_editCameraX;
    CEdit m_editCameraY;
    CEdit m_editCameraZ;
    CButton m_checkWireframe;
    CButton m_checkSmooth;
    CButton m_checkDepth;
    
    CStatic m_staticStatus;
    CStatic m_staticCamera;
    CStatic m_staticOptions;
    
    CFont m_fontModern;
    CFont m_fontBold;
    
    bool InitOpenGL();
    void RenderOpenGL();
    void CleanupOpenGL();
    
    void UpdateStatus(const CString& strStatus);
    void ApplyCameraSettings();
    void UpdateUILayout();
    
    SceneManager* m_pSceneManager;
    float m_fDeltaTime;
    ULONGLONG m_dwLastTime;
    bool m_bIsRunning;
    bool m_bWireframe;
    bool m_bSmoothShading;
    bool m_bDepthTest;
    
    int m_nClientWidth;
    int m_nClientHeight;
    int m_nRenderAreaX;
    int m_nRenderAreaY;
    int m_nRenderAreaWidth;
    int m_nRenderAreaHeight;
};
