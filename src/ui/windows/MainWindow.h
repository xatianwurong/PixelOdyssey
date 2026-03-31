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
 * @class CAcadStyleBar
 * @brief AutoCAD风格工具栏
 */
class CAcadStyleBar : public CWnd
{
    DECLARE_DYNAMIC(CAcadStyleBar)

public:
    CAcadStyleBar();
    virtual ~CAcadStyleBar();

    BOOL Create(CWnd* pParentWnd, UINT nID);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
    CFont m_font;
    CArray<CRect, CRect&> m_btnRects;
    int m_nHoverBtn;
    int m_nSelectedTool;

    void DrawButton(CDC* pDC, int index, const CString& text, bool bHover, bool bSelected);
};

/**
 * @class CCommandBar
 * @brief AutoCAD风格命令行
 */
class CCommandBar : public CWnd
{
    DECLARE_DYNAMIC(CCommandBar)

public:
    CCommandBar();
    virtual ~CCommandBar();

    BOOL Create(CWnd* pParentWnd, UINT nID);
    void SetCommandText(const CString& text);
    void AppendOutput(const CString& text);
    CString GetCommandText() const;

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
    CEdit m_editCommand;
    CEdit m_editOutput;
    CStatic m_staticPrompt;
    CFont m_font;
    CBrush m_brushBg;
};

/**
 * @class CPropertiesPanel
 * @brief AutoCAD风格属性面板
 */
class CPropertiesPanel : public CWnd
{
    DECLARE_DYNAMIC(CPropertiesPanel)

public:
    CPropertiesPanel();
    virtual ~CPropertiesPanel();

    BOOL Create(CWnd* pParentWnd, UINT nID);
    void UpdateProperties(const CString& name, const CString& type, 
                          float x, float y, float z, float scale);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnEditChange();

private:
    CStatic m_staticTitle;
    CStatic m_staticName;
    CEdit m_editName;
    CStatic m_staticType;
    CEdit m_editType;
    CStatic m_staticPos;
    CEdit m_editX;
    CEdit m_editY;
    CEdit m_editZ;
    CStatic m_staticScale;
    CEdit m_editScale;
    CFont m_font;
    CFont m_fontBold;
    CBrush m_brushBg;

    void LayoutControls();
};

/**
 * @class CMainWindow
 * @brief AutoCAD风格主窗口
 * @details 类似AutoCAD布局：顶部工具栏、中央渲染区、底部命令行、右侧属性面板
 */
class CMainWindow : public CFrameWnd
{
public:
    CMainWindow();
    virtual ~CMainWindow();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnToolSelect();
    afx_msg void OnToolPan();
    afx_msg void OnToolZoom();
    afx_msg void OnToolDrawLine();
    afx_msg void OnToolDrawRect();
    afx_msg void OnToolDrawCircle();
    afx_msg void OnViewWireframe();
    afx_msg void OnViewRendered();
    afx_msg void OnHelpAbout();
    afx_msg void OnCameraXChange();
    afx_msg void OnCameraYChange();
    afx_msg void OnCameraZChange();
    afx_msg void OnCheckWireframe();
    afx_msg void OnCheckSmooth();
    afx_msg void OnCheckDepth();
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    afx_msg LRESULT OnNcHitTest(CPoint point);

private:
    // OpenGL
    HGLRC m_hGLRC;
    HWND m_hWndOpenGL;
    CStatic m_renderView;
    CStatic m_staticOpenGL;
    
    // 菜单栏
    CMenu m_menu;
    
    // 状态
    SceneManager* m_pSceneManager;
    float m_fDeltaTime;
    ULONGLONG m_dwLastTime;
    bool m_bIsRunning;
    bool m_bWireframe;
    int m_nCurrentTool;
    
    // 字体
    CFont m_fontModern;
    CFont m_fontBold;

    // 状态
    bool m_bSmoothShading;
    bool m_bDepthTest;
    int m_nClientWidth;
    int m_nClientHeight;
    int m_nRenderAreaX;
    int m_nRenderAreaY;
    int m_nRenderAreaWidth;
    int m_nRenderAreaHeight;

    // 缺失的UI组件
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

    bool InitOpenGL();
    void RenderOpenGL();
    void CleanupOpenGL();
    void CreateTestScene();
    void UpdateLayout();
    void UpdateUILayout();
    void CreateControls();
    void SetStatusText(const CString& text);
    void UpdateStatus(const CString& strStatus);
    void ApplyCameraSettings();
};
