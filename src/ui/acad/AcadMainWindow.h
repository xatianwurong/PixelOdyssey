#pragma once

#include <afxwin.h>
#include <afxwinappex.h>
#include <GL/glew.h>
#include "AcadUIManager.h"
#include "../../core/scene/SceneManager.h"
#include "../../core/renderer/Camera.h"

/**
 * @brief AutoCAD 风格主窗口
 * @details 现代化的界面布局，类似 AutoCAD 的简洁设计
 */
class CAcadMainWindow : public CFrameWnd
{
  DECLARE_DYNAMIC(CAcadMainWindow)

public:
  CAcadMainWindow();
  virtual ~CAcadMainWindow();

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnDestroy();

private:
  // OpenGL 上下文
  HGLRC m_hGLRC;
  HWND m_hWndOpenGL;
  CStatic m_renderView;

  // UI 管理器
  CAcadUIManager* m_pUIManager;

  // 场景管理
  SceneManager* m_pSceneManager;

  // 状态
  bool m_bIsRunning;
  float m_fDeltaTime;
  ULONGLONG m_dwLastTime;

  // 渲染设置
  bool m_bWireframe;
  bool m_bSmoothShading;
  bool m_bDepthTest;

  // 相机
  Camera m_camera;

  /**
   * @brief 初始化 OpenGL
   */
  bool InitOpenGL();

  /**
   * @brief 渲染 OpenGL 场景
   */
  void RenderOpenGL();

  /**
   * @brief 清理 OpenGL 资源
   */
  void CleanupOpenGL();

  /**
   * @brief 创建测试场景
   */
  void CreateTestScene();

  /**
   * @brief 更新状态栏
   */
  void UpdateStatus(const CString& text);
};
