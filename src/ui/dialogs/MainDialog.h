#pragma once

#include <afxwin.h>
#include <afxwinappex.h>
#include <afxdialogex.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../resources/Resource.h"

#include "../../core/scene/SceneManager.h"

class CMainDialog : public CDialogEx
{
public:
    CMainDialog(CWnd* pParent = nullptr);
    virtual ~CMainDialog();

    enum { IDD = IDD_MAINDIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnBnClickedButtonStop();
    afx_msg void OnBnClickedButtonReset();
    afx_msg void OnBnClickedButtonApply();
    afx_msg void OnBnClickedCheckWireframe();
    afx_msg void OnBnClickedCheckSmooth();
    afx_msg void OnBnClickedCheckDepth();
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpWindowPos);

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
    
    bool InitOpenGL();
    void RenderOpenGL();
    void CleanupOpenGL();
    
    void UpdateStatus(const CString& strStatus);
    void ApplyCameraSettings();
    void UpdateUILayout();
    
    SceneManager* m_pSceneManager;
    float m_fDeltaTime;
    DWORD m_dwLastTime;
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
