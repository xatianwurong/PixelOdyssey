#include "AcadMainWindow.h"
#include <GL/glew.h>
#include <afxdialogex.h>
#include <time.h>
#include "../../core/scene/SceneManager.h"
#include "../../core/entity/Circle.h"
#include "../../core/entity/Rectangle.h"
#include "../../core/entity/Square.h"
#include "../../core/entity/Triangle.h"
#include "UILayout.h"  // 统一布局管理器

IMPLEMENT_DYNAMIC(CAcadMainWindow, CFrameWnd)

CAcadMainWindow::CAcadMainWindow()
  : m_hGLRC(nullptr)
  , m_hWndOpenGL(nullptr)
  , m_pUIManager(nullptr)
  , m_pSceneManager(nullptr)
  , m_bIsRunning(false)
  , m_fDeltaTime(0.0f)
  , m_dwLastTime(0)
  , m_bWireframe(false)
  , m_bSmoothShading(true)
  , m_bDepthTest(true)
{
  
}

CAcadMainWindow::~CAcadMainWindow()
{
  
  CleanupOpenGL();
  
}

BEGIN_MESSAGE_MAP(CAcadMainWindow, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_TIMER()
  ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CAcadMainWindow::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style |= WS_CLIPCHILDREN;
  cs.cx = 1400;
  cs.cy = 900;
  return CFrameWnd::PreCreateWindow(cs);
}

int CAcadMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  //LOG(_T("[AcadMainWindow] OnCreate called"));

  // 创建 UI 管理器
  m_pUIManager = &CAcadUIManager::Instance();
  if (!m_pUIManager->Initialize(this)) {
    //LOG(_T("[AcadMainWindow] Failed to initialize UI Manager"));
    return -1;
  }

  // 创建 OpenGL 渲染区域（使用统一布局）
  DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY;
  CRect rect;
  GetClientRect(&rect);

  CRect openGLRect = UILayout::GetOpenGLRect(rect.Width(), rect.Height());
  if (!m_renderView.Create(_T(""), dwStyle,
    openGLRect,
    this, 1001)) {
    //LOG(_T("[AcadMainWindow] Failed to create OpenGL view"));
    return -1;
  }

  m_hWndOpenGL = m_renderView.m_hWnd;
  //LOG(_T("[AcadMainWindow] OpenGL view created at %dx%d"), openGLRect.Width(), openGLRect.Height());

  if (!InitOpenGL()) {
    //LOG(_T("[AcadMainWindow] Failed to initialize OpenGL"));
    return -1;
  }

  CreateTestScene();

  m_bIsRunning = true;
  m_dwLastTime = GetTickCount64();
  SetTimer(1, 16, NULL);

  UpdateStatus(_T("Ready - AutoCAD Style UI"));

  return 0;
}

void CAcadMainWindow::OnSize(UINT nType, int cx, int cy)
{
  CFrameWnd::OnSize(nType, cx, cy);

  // 使用统一布局管理器调整所有 UI 组件
  if (m_pUIManager) {
    m_pUIManager->Layout(cx, cy);
  }

  // 调整 OpenGL 视图（使用统一布局）
  if (m_renderView.GetSafeHwnd()) {
    CRect openGLRect = UILayout::GetOpenGLRect(cx, cy);
    m_renderView.MoveWindow(openGLRect);

    if (m_hGLRC) {
      CDC* pDC = CDC::FromHandle(::GetDC(m_hWndOpenGL));
      wglMakeCurrent(pDC->m_hDC, m_hGLRC);

      // 更新视口
      glViewport(0, 0, openGLRect.Width(), openGLRect.Height());

      // 更新投影矩阵
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      GLfloat aspect = (GLfloat)openGLRect.Width() / (GLfloat)openGLRect.Height();
      gluPerspective(45.0f, aspect, 0.1f, 100.0f);
      glMatrixMode(GL_MODELVIEW);

      wglMakeCurrent(NULL, NULL);
    }
  }
}

void CAcadMainWindow::OnPaint()
{
  CPaintDC dc(this);
  ValidateRect(NULL);
}

BOOL CAcadMainWindow::OnEraseBkgnd(CDC* pDC)
{
  return TRUE;
}

void CAcadMainWindow::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent == 1 && m_bIsRunning) {
    ULONGLONG dwCurrentTime = GetTickCount64();
    m_fDeltaTime = (float)(dwCurrentTime - m_dwLastTime) / 1000.0f;
    m_dwLastTime = dwCurrentTime;

    RenderOpenGL();

    if (m_pUIManager) {
      m_pUIManager->Update();
    }
  }

  CFrameWnd::OnTimer(nIDEvent);
}

void CAcadMainWindow::OnDestroy()
{
  CleanupOpenGL();
  KillTimer(1);
  CFrameWnd::OnDestroy();
}

bool CAcadMainWindow::InitOpenGL()
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
  if (nPixelFormat == 0) {
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }

  ::SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

  m_hGLRC = wglCreateContext(pDC->m_hDC);
  if (!m_hGLRC) {
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }

  wglMakeCurrent(pDC->m_hDC, m_hGLRC);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
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

void CAcadMainWindow::RenderOpenGL()
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
  if (m_pSceneManager) {
    m_pSceneManager->Draw();
  }

  // 绘制网格（如果启用）
  // GridManager::Instance().Render();

  SwapBuffers(pDC->m_hDC);
  wglMakeCurrent(NULL, NULL);
}

void CAcadMainWindow::CleanupOpenGL()
{
  if (m_hGLRC) {
    if (wglGetCurrentContext() == m_hGLRC) {
      wglMakeCurrent(NULL, NULL);
    }
    wglDeleteContext(m_hGLRC);
    m_hGLRC = nullptr;
  }
}

void CAcadMainWindow::CreateTestScene()
{
  m_pSceneManager = &SceneManager::Instance();

  Scene* testScene = new Scene("MainScene");
  m_pSceneManager->AddScene(testScene);
  m_pSceneManager->SetActiveScene("MainScene");

  // 添加测试图形
  float scale = 0.5f;

  //Rectangle* rect1 = new Rectangle(glm::vec2(-2.0f, 0.0f), 
  //                                  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 
  //                                  scale);
  //testScene->AddObject(rect1);

  Circle* circle = new Circle(glm::vec2(0.0f, 0.0f),
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
    scale);
  testScene->AddObject(circle);

  Triangle* triangle = new Triangle(glm::vec2(2.0f, 0.0f),
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
    scale);
  testScene->AddObject(triangle);

  UpdateStatus(_T("Test scene created with 3 objects"));
}

void CAcadMainWindow::UpdateStatus(const CString& text)
{
  if (m_pUIManager) {
    m_pUIManager->GetCommandLine().AppendOutput(text);
  }
}
