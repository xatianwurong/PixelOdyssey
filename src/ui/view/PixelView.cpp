#include "PixelView.h"
#include "PixelDocument.h"
#include <GL/glew.h>
#include <afxdialogex.h>
#include <time.h>
#include "../../core/entity/Circle.h"
#include "../../core/entity/Rectangle.h"
#include "../../core/entity/Square.h"
#include "../../core/entity/Triangle.h"
#include "../acad/UILayout.h"

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
  ASSERT_VALID(pDoc);
  return pDoc->GetScene();
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

  // 如果文档没有场景，创建测试场景
  CreateTestSceneIfEmpty();

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
      CDC* pDC = CDC::FromHandle(::GetDC(m_hWndOpenGL));
      wglMakeCurrent(pDC->m_hDC, m_hGLRC);

      // 更新视口
      glViewport(0, 0, cx, cy);

      // 更新投影矩阵
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      GLfloat aspect = (GLfloat)cx / (GLfloat)cy;
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

  CDC* pDC = CDC::FromHandle(::GetDC(m_hWndOpenGL));

  int nPixelFormat = ::ChoosePixelFormat(pDC->m_hDC, &pfd);
  if (nPixelFormat == 0)
  {
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }

  ::SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

  m_hGLRC = wglCreateContext(pDC->m_hDC);
  if (!m_hGLRC)
  {
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }

  wglMakeCurrent(pDC->m_hDC, m_hGLRC);

  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    wglDeleteContext(m_hGLRC);
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }

  // OpenGL 设置
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
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
  if (!m_hGLRC)
    return;

  CDC* pDC = CDC::FromHandle(::GetDC(m_hWndOpenGL));
  wglMakeCurrent(pDC->m_hDC, m_hGLRC);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 设置相机
  glm::mat4 view = m_camera.GetViewMatrix();
  glm::mat4 projection = m_camera.GetProjectionMatrix();

  // 渲染场景
  Scene* pScene = GetScene();
  if (pScene)
  {
    pScene->Draw();
  }

  SwapBuffers(pDC->m_hDC);
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
}

void CPixelView::CreateTestSceneIfEmpty()
{
  CPixelDocument* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  Scene* pScene = pDoc->GetScene();
  if (!pScene || pScene->GetName().empty())
  {
    // 文档没有有效场景，创建测试场景
    if (pScene)
    {
      delete pScene;
    }

    pScene = new Scene("MainScene");
    // 注意：这里需要更新文档中的场景指针
    // 由于文档类设计限制，这里仅作演示

    // 添加测试图形
    float scale = 0.5f;

    Circle* circle = new Circle(glm::vec2(0.0f, 0.0f),
      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
      scale);
    pScene->AddObject(circle);

    Triangle* triangle = new Triangle(glm::vec2(2.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
      scale);
    pScene->AddObject(triangle);
  }
}
