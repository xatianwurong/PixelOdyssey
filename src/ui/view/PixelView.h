#pragma once

#include <afxwin.h>
#include <afxext.h>
#include "GL/glew.h"
#include "../../core/renderer/Camera.h"
#include "../../core/scene/Scene.h"

// Forward declaration
class CPixelDocument;

/**
 * @brief OpenGL 渲染视图类
 * @details 继承自 CView，负责在子窗口中渲染 OpenGL 场景
 *          每个文档可以有多个视图，支持多文档界面
 */
class CPixelView : public CView
{
  DECLARE_DYNCREATE(CPixelView)

public:
  CPixelView();
  virtual ~CPixelView();

#ifdef _DEBUG
  virtual void AssertValid() const;
#ifndef _WIN32_WCE
  virtual void Dump(CDumpContext& dc) const;
#endif
#endif

  // 获取关联的场景
  Scene* GetScene();

  // 获取文档（重载）
  CPixelDocument* GetDocument() const;

protected:
  DECLARE_MESSAGE_MAP()

  // 纯虚函数实现 (CView 要求)
  virtual void OnDraw(CDC* pDC);

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

  // 相机
  Camera m_camera;

  // 状态
  bool m_bIsRunning;
  float m_fDeltaTime;
  ULONGLONG m_dwLastTime;

  // 渲染设置
  bool m_bWireframe;
  bool m_bSmoothShading;
  bool m_bDepthTest;

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
   * @brief 创建测试场景（仅当文档没有场景时）
   */
  void CreateTestSceneIfEmpty();
};
