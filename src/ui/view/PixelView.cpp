#include "PixelView.h"
#include "../document/PixelDocument.h"
#include <GL/glew.h>
#include <afxdialogex.h>
#include <time.h>
#include "../../core/renderer/Camera.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPixelView, CView)

BEGIN_MESSAGE_MAP(CPixelView, CView)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_TIMER()
  ON_WM_DESTROY()
END_MESSAGE_MAP()

CPixelView::CPixelView()
  : m_hGLRC(nullptr)
  , m_hOpenGLDC(nullptr)
  , m_hWndOpenGL(nullptr)
  , m_bIsRunning(false)
  , m_fDeltaTime(0.0f)
  , m_dwLastTime(0)
  , m_bWireframe(false)
  , m_bSmoothShading(true)
  , m_bDepthTest(true)
{
}

CPixelView::~CPixelView()
{
  CleanupOpenGL();
}

#ifdef _DEBUG
void CPixelView::AssertValid() const
{
  CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPixelView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif
#endif

Scene* CPixelView::GetScene()
{
  CPixelDocument* pDoc = GetDocument();
  if (pDoc == nullptr)
    return nullptr;
  return pDoc->GetScene();
}

CPixelDocument* CPixelView::GetDocument() const
{
  // 使用safe cast
  CDocument* pDoc = CView::GetDocument();
  return DYNAMIC_DOWNCAST(CPixelDocument, pDoc);
}

void CPixelView::OnDraw(CDC* pDC)
{
  // OpenGL 渲染在 OnTimer 中进行，这里无需做任何操作
  // 仅由 RenderOpenGL 处理
}

int CPixelView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // 创建 OpenGL 渲染区域
  DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY;
  CRect rect;
  GetClientRect(&rect);

  if (!m_renderView.Create(_T(""), dwStyle, rect, this, 1001))
  {
    return -1;
  }

  m_hWndOpenGL = m_renderView.m_hWnd;

  if (!InitOpenGL())
  {
    return -1;
  }

  m_bIsRunning = true;
  m_dwLastTime = GetTickCount64();
  SetTimer(1, 16, NULL);

  return 0;
}

void CPixelView::OnSize(UINT nType, int cx, int cy)
{
  CView::OnSize(nType, cx, cy);

  // 调整 OpenGL 视图
  if (m_renderView.GetSafeHwnd())
  {
    m_renderView.MoveWindow(0, 0, cx, cy);

    if (m_hGLRC)
    {
      wglMakeCurrent(m_hOpenGLDC, m_hGLRC);

      // 更新视口
      glViewport(0, 0, cx, cy);

      // 更新投影矩阵
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      GLfloat aspect = (cy > 0) ? (GLfloat)cx / (GLfloat)cy : 1.0f;
      gluPerspective(45.0f, aspect, 0.1f, 100.0f);
      glMatrixMode(GL_MODELVIEW);

      wglMakeCurrent(NULL, NULL);
    }
  }
}

void CPixelView::OnPaint()
{
  CPaintDC dc(this);
  ValidateRect(NULL);
}

BOOL CPixelView::OnEraseBkgnd(CDC* pDC)
{
  return TRUE;
}

void CPixelView::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent == 1 && m_bIsRunning)
  {
    ULONGLONG dwCurrentTime = GetTickCount64();
    m_fDeltaTime = (float)(dwCurrentTime - m_dwLastTime) / 1000.0f;
    m_dwLastTime = dwCurrentTime;

    RenderOpenGL();
  }

  CView::OnTimer(nIDEvent);
}

void CPixelView::OnDestroy()
{
  CleanupOpenGL();
  KillTimer(1);
  CView::OnDestroy();
}

bool CPixelView::InitOpenGL()
{
  if (!m_hWndOpenGL)
    return false;

  PIXELFORMATDESCRIPTOR pfd = { 0 };
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;
  pfd.iLayerType = PFD_MAIN_PLANE;

  m_hOpenGLDC = ::GetDC(m_hWndOpenGL);
  if (!m_hOpenGLDC)
    return false;

  int nPixelFormat = ::ChoosePixelFormat(m_hOpenGLDC, &pfd);
  if (nPixelFormat == 0)
  {
    ::ReleaseDC(m_hWndOpenGL, m_hOpenGLDC);
    m_hOpenGLDC = nullptr;
    return false;
  }

  if (!::SetPixelFormat(m_hOpenGLDC, nPixelFormat, &pfd))
  {
    ::ReleaseDC(m_hWndOpenGL, m_hOpenGLDC);
    m_hOpenGLDC = nullptr;
    return false;
  }

  m_hGLRC = wglCreateContext(m_hOpenGLDC);
  if (!m_hGLRC)
  {
    ::ReleaseDC(m_hWndOpenGL, m_hOpenGLDC);
    m_hOpenGLDC = nullptr;
    return false;
  }

  wglMakeCurrent(m_hOpenGLDC, m_hGLRC);

  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    wglDeleteContext(m_hGLRC);
    m_hGLRC = nullptr;
    ::ReleaseDC(m_hWndOpenGL, m_hOpenGLDC);
    m_hOpenGLDC = nullptr;
    return false;
  }

  // OpenGL 设置
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glShadeModel(GL_SMOOTH);
  glClearDepth(1.0f);

  // 初始化相机
  m_camera.SetPosition(glm::vec3(0, 0, 5));
  m_camera.SetTarget(glm::vec3(0, 0, 0));

  wglMakeCurrent(NULL, NULL);
  return true;
}

void CPixelView::RenderOpenGL()
{
  if (!m_hGLRC || !m_hOpenGLDC)
    return;

  wglMakeCurrent(m_hOpenGLDC, m_hGLRC);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 渲染场景
  Scene* pScene = GetScene();
  if (pScene)
  {
    pScene->Draw();
  }

  SwapBuffers(m_hOpenGLDC);
  wglMakeCurrent(NULL, NULL);
}

void CPixelView::CleanupOpenGL()
{
  if (m_hGLRC)
  {
    if (wglGetCurrentContext() == m_hGLRC)
    {
      wglMakeCurrent(NULL, NULL);
    }
    wglDeleteContext(m_hGLRC);
    m_hGLRC = nullptr;
  }

  if (m_hOpenGLDC)
  {
    ::ReleaseDC(m_hWndOpenGL, m_hOpenGLDC);
    m_hOpenGLDC = nullptr;
  }
}
