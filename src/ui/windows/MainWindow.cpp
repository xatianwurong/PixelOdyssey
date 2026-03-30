#include "MainWindow.h"
#include <GL/glew.h>
#include <afxdialogex.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include "OpenGLLogger.h"
#include "../../core/entity/Triangle.h"
#include "../../core/entity/Square.h"
#include "../../core/entity/Circle.h"
#include "../../core/entity/Rectangle.h"
#include "../../core/entity/Ellipse.h"
#include "../../core/entity/Arrow.h"
class Rectangle;
class Ellipse;
class Arrow;

#include "../../core/entity/Cube.h"
#include "../../core/entity/Circle.h"
#include "../../core/entity/Square.h"
#include "../../core/entity/Triangle.h"
#include "../../core/entity/Rectangle.h"
#include "../../core/entity/Ellipse.h"
#include "../../core/entity/Arrow.h"
#include "../../core/entity/Star.h"
#include "../../core/entity/Heart.h"
#include "../../core/entity/Pentagram.h"
#include "../../core/entity/Cup.h"
#include "../../core/entity/Book.h"
#include "../../core/entity/Pencil.h"
#include "../../core/entity/House.h"
#include "../../core/entity/Car.h"
#include "../../core/entity/Cat.h"
#include "../../core/entity/Dog.h"
#include "../../core/entity/Fish.h"
#include "../../core/entity/Bird.h"
#include "../../core/entity/StandingPerson.h"
#include "../../core/entity/SittingPerson.h"
#include "../../core/entity/RunningPerson.h"
#include "../utils/DataStorage.h"

BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_TIMER()
  ON_WM_ERASEBKGND()
  ON_WM_WINDOWPOSCHANGING()
  ON_WM_DESTROY()
  ON_WM_NCHITTEST()
  ON_EN_CHANGE(1003, OnCameraXChange)
  ON_EN_CHANGE(1004, OnCameraYChange)
  ON_EN_CHANGE(1005, OnCameraZChange)
  ON_BN_CLICKED(1006, OnCheckWireframe)
  ON_BN_CLICKED(1007, OnCheckSmooth)
  ON_BN_CLICKED(1008, OnCheckDepth)
END_MESSAGE_MAP()

CMainWindow::CMainWindow()
  : m_hGLRC(nullptr),
  m_hWndOpenGL(nullptr),
  m_staticOpenGL(),
  m_editStatus(),
  m_editCameraX(),
  m_editCameraY(),
  m_editCameraZ(),
  m_checkWireframe(),
  m_checkSmooth(),
  m_checkDepth(),
  m_staticStatus(),
  m_staticCamera(),
  m_staticOptions(),
  m_pSceneManager(nullptr),
  m_fDeltaTime(0.0f),
  m_dwLastTime(0),
  m_bIsRunning(false),
  m_bWireframe(false),
  m_bSmoothShading(false),
  m_bDepthTest(true),
  m_nClientWidth(0),
  m_nClientHeight(0),
  m_nRenderAreaX(0),
  m_nRenderAreaY(0),
  m_nRenderAreaWidth(0),
  m_nRenderAreaHeight(0)
{
  LOG(_T("[MainWindow] Constructor called"));
}

CMainWindow::~CMainWindow()
{
  LOG(_T("[MainWindow] Destructor called"));
  CleanupOpenGL();
}

BOOL CMainWindow::PreCreateWindow(CREATESTRUCT& cs)
{
  CString strMessage;
  strMessage.Format(_T("[MainWindow] PreCreateWindow called, style=0x%08X"), cs.style);
  LOG(strMessage);
  cs.style |= WS_CLIPCHILDREN;
  cs.cx = 1200;
  cs.cy = 800;
  strMessage.Format(_T("[MainWindow] PreCreateWindow after, style=0x%08X"), cs.style);
  LOG(strMessage);
  return CFrameWnd::PreCreateWindow(cs);
}

BOOL CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  LOG(_T("[MainWindow] OnCreate called"));

  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
  {
    LOG(_T("[MainWindow] ERROR: CFrameWnd::OnCreate failed"));
    return -1;
  }

  CreateControls();

  if (!InitOpenGL())
  {
    LOG(_T("[MainWindow] ERROR: InitOpenGL failed"));
    return -1;
  }

  LOG(_T("[MainWindow] OnCreate completed successfully"));

  InvalidateRect(NULL, FALSE);

  return 0;
}

void CMainWindow::OnPaint()
{
  LOG(_T("[MainWindow] OnPaint called"));

  if (!m_hGLRC)
  {
    LOG(_T("[MainWindow] OpenGL not initialized yet, skipping paint"));
    return;
  }

  CPaintDC dc(this);

  RenderOpenGL();
}

BOOL CMainWindow::OnEraseBkgnd(CDC* pDC)
{
  LOG(_T("[MainWindow] OnEraseBkgnd called"));

  CRect rect;
  GetClientRect(rect);

  CBrush brush(RGB(240, 240, 240));
  pDC->FillRect(rect, &brush);

  return TRUE;
}

void CMainWindow::OnSize(UINT nType, int cx, int cy)
{
  LOG(_T("[MainWindow] OnSize called"));

  CFrameWnd::OnSize(nType, cx, cy);

  m_nClientWidth = cx;
  m_nClientHeight = cy;

  UpdateUILayout();

  if (m_hWndOpenGL && IsWindow(m_hWndOpenGL))
  {
    CRect rect;
    m_staticOpenGL.GetWindowRect(rect);
    ScreenToClient(rect);

    glViewport(0, 0, rect.Width(), rect.Height());

    if (rect.Width() > 0 && rect.Height() > 0)
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      GLfloat aspectRatio = (GLfloat)rect.Width() / (GLfloat)rect.Height();
      if (aspectRatio > 1.0f)
      {
        glFrustum(-aspectRatio, aspectRatio, -1.0, 1.0, 1.0, 10.0);
      }
      else
      {
        glFrustum(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, 1.0, 10.0);
      }

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
  }
}

void CMainWindow::OnTimer(UINT_PTR nIDEvent)
{
  LOG(_T("[MainWindow] OnTimer called"));

  CFrameWnd::OnTimer(nIDEvent);

  if (m_bIsRunning)
  {
    ULONGLONG dwCurrentTime = GetTickCount64();
    m_fDeltaTime = (float)(dwCurrentTime - m_dwLastTime) / 1000.0f;
    m_dwLastTime = dwCurrentTime;

    RenderOpenGL();
  }
}

void CMainWindow::OnDestroy()
{
  LOG(_T("[MainWindow] OnDestroy called"));

  CleanupOpenGL();

  CFrameWnd::OnDestroy();
}

void CMainWindow::OnWindowPosChanging(WINDOWPOS* lpWindowPos)
{
  LOG(_T("[MainWindow] OnWindowPosChanging called"));
  CFrameWnd::OnWindowPosChanging(lpWindowPos);
}

LRESULT CMainWindow::OnNcHitTest(CPoint point)
{
  LOG(_T("[MainWindow] OnNcHitTest called"));
  return CFrameWnd::OnNcHitTest(point);
}

void CMainWindow::CreateControls()
{
  LOG(_T("[MainWindow] CreateControls called"));

  CRect rect;
  GetClientRect(rect);

  int nClientWidth = rect.Width();
  int nClientHeight = rect.Height();

  m_nRenderAreaX = 0;
  m_nRenderAreaY = 0;
  m_nRenderAreaWidth = (int)(nClientWidth * 0.7f);
  m_nRenderAreaHeight = nClientHeight;

  DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY;

  if (!m_staticOpenGL.Create(_T(""), dwStyle, CRect(m_nRenderAreaX, m_nRenderAreaY, m_nRenderAreaX + m_nRenderAreaWidth, m_nRenderAreaY + m_nRenderAreaHeight), this, 1001))
  {
    LOG(_T("[MainWindow] ERROR: Failed to create OpenGL render area"));
    return;
  }

  m_hWndOpenGL = m_staticOpenGL.m_hWnd;

  LOG(_T("[MainWindow] OpenGL render area created successfully"));

  int nControlStartX = m_nRenderAreaX + m_nRenderAreaWidth + 20;
  int nControlWidth = nClientWidth - nControlStartX - 20;
  int nControlHeight = 30;
  int nControlSpacing = 45;
  int nY = 30;

  LOGFONT lf;
  memset(&lf, 0, sizeof(LOGFONT));
  lf.lfHeight = -14;
  lf.lfWeight = FW_NORMAL;
  lf.lfQuality = CLEARTYPE_QUALITY;
  _tcscpy_s(lf.lfFaceName, _T("Segoe UI"));
  m_fontModern.CreateFontIndirect(&lf);

  LOGFONT lfBold;
  memset(&lfBold, 0, sizeof(LOGFONT));
  lfBold.lfHeight = -16;
  lfBold.lfWeight = FW_BOLD;
  lfBold.lfQuality = CLEARTYPE_QUALITY;
  _tcscpy_s(lfBold.lfFaceName, _T("Segoe UI"));
  m_fontBold.CreateFontIndirect(&lfBold);

  m_staticStatus.Create(_T("Status"), WS_CHILD | WS_VISIBLE,
    CRect(nControlStartX, nY, nControlStartX + nControlWidth, nY + 20),
    this, 2001);
  m_staticStatus.SetFont(&m_fontBold);

  nY += 25;

  m_editStatus.Create(ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER,
    CRect(nControlStartX, nY, nControlStartX + nControlWidth, nY + nControlHeight),
    this, 1002);
  m_editStatus.SetFont(&m_fontModern);
  m_editStatus.SetWindowText(_T("Ready"));

  nY += nControlSpacing + 10;

  m_staticCamera.Create(_T("Camera Position"), WS_CHILD | WS_VISIBLE,
    CRect(nControlStartX, nY, nControlStartX + nControlWidth, nY + 20),
    this, 2002);
  m_staticCamera.SetFont(&m_fontBold);

  nY += 25;

  m_editCameraX.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
    CRect(nControlStartX, nY, nControlStartX + nControlWidth, nY + nControlHeight),
    this, 1003);
  m_editCameraX.SetFont(&m_fontModern);
  m_editCameraX.SetWindowText(_T("0.0"));

  m_editCameraY.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
    CRect(nControlStartX, nY + nControlSpacing, nControlStartX + nControlWidth, nY + nControlSpacing + nControlHeight),
    this, 1004);
  m_editCameraY.SetFont(&m_fontModern);
  m_editCameraY.SetWindowText(_T("0.0"));

  m_editCameraZ.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
    CRect(nControlStartX, nY + nControlSpacing * 2, nControlStartX + nControlWidth, nY + nControlSpacing * 2 + nControlHeight),
    this, 1005);
  m_editCameraZ.SetFont(&m_fontModern);
  m_editCameraZ.SetWindowText(_T("5.0"));

  nY += nControlSpacing * 3 + 10;

  m_staticOptions.Create(_T("Rendering Options"), WS_CHILD | WS_VISIBLE,
    CRect(nControlStartX, nY, nControlStartX + nControlWidth, nY + 20),
    this, 2003);
  m_staticOptions.SetFont(&m_fontBold);

  nY += 25;

  m_checkWireframe.Create(_T("Wireframe Mode"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
    CRect(nControlStartX, nY, nControlStartX + nControlWidth, nY + nControlHeight),
    this, 1006);
  m_checkWireframe.SetFont(&m_fontModern);
  m_checkWireframe.SetCheck(0);

  m_checkSmooth.Create(_T("Smooth Shading"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
    CRect(nControlStartX, nY + nControlSpacing, nControlStartX + nControlWidth, nY + nControlSpacing + nControlHeight),
    this, 1007);
  m_checkSmooth.SetFont(&m_fontModern);
  m_checkSmooth.SetCheck(1);

  m_checkDepth.Create(_T("Depth Test"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
    CRect(nControlStartX, nY + nControlSpacing * 2, nControlStartX + nControlWidth, nY + nControlSpacing * 2 + nControlHeight),
    this, 1008);
  m_checkDepth.SetFont(&m_fontModern);
  m_checkDepth.SetCheck(1);

  LOG(_T("[MainWindow] All controls created successfully"));
}

bool CMainWindow::InitOpenGL()
{
  LOG(_T("[OpenGL] Starting initialization..."));

  if (!m_hWndOpenGL)
  {
    LOG(_T("[OpenGL] ERROR: m_hWndOpenGL is NULL!"));
    AfxMessageBox(_T("OpenGL render area not found!"));
    return false;
  }
  LOG(_T("[OpenGL] m_hWndOpenGL is valid"));

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
  if (!pDC)
  {
    LOG(_T("[OpenGL] ERROR: Failed to get DC!"));
    return false;
  }
  LOG(_T("[OpenGL] Got device context"));

  int nPixelFormat = ::ChoosePixelFormat(pDC->m_hDC, &pfd);
  if (nPixelFormat == 0)
  {
    LOG(_T("[OpenGL] ERROR: ChoosePixelFormat failed"));
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }
  LOG(_T("[OpenGL] Pixel format selected"));

  if (!::SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd))
  {
    LOG(_T("[OpenGL] ERROR: SetPixelFormat failed"));
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }
  LOG(_T("[OpenGL] Pixel format set successfully"));

  m_hGLRC = wglCreateContext(pDC->m_hDC);
  if (!m_hGLRC)
  {
    LOG(_T("[OpenGL] ERROR: wglCreateContext failed"));
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }
  LOG(_T("[OpenGL] OpenGL context created"));

  if (!wglMakeCurrent(pDC->m_hDC, m_hGLRC))
  {
    LOG(_T("[OpenGL] ERROR: wglMakeCurrent failed"));
    wglDeleteContext(m_hGLRC);
    m_hGLRC = NULL;
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }
  LOG(_T("[OpenGL] OpenGL context made current"));

  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    LOG(_T("[OpenGL] ERROR: glewInit failed"));
    wglDeleteContext(m_hGLRC);
    m_hGLRC = NULL;
    ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
    return false;
  }
  LOG(_T("[OpenGL] GLEW initialized successfully"));

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);

  glShadeModel(GL_SMOOTH);

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  LOG(_T("[OpenGL] OpenGL initialization completed successfully"));

  m_pSceneManager = &SceneManager::Instance();
  if (m_pSceneManager)
  {
    LOG(_T("[OpenGL] SceneManager retrieved successfully"));

    Scene* testScene = new Scene("TestScene");
    m_pSceneManager->AddScene(testScene);
    m_pSceneManager->SetActiveScene("TestScene");
    LOG(_T("[OpenGL] Test scene created and set as active"));

    struct EntityInfo {
      std::string type;
      glm::vec4 color;
    };

    std::vector<EntityInfo> entityInfos = {
        {"Triangle", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {"Square", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {"Circle", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {"Arrow", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
        {"Star", glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)},
        {"Heart", glm::vec4(0.5f, 0.0f, 1.0f, 1.0f)},
        {"Pentagram", glm::vec4(0.5f, 1.0f, 0.0f, 1.0f)},
        {"Cup", glm::vec4(1.0f, 0.0f, 0.5f, 1.0f)},
        {"Book", glm::vec4(0.0f, 0.5f, 1.0f, 1.0f)},
        {"Pencil", glm::vec4(1.0f, 1.0f, 0.5f, 1.0f)},
        {"House", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)},
        {"Car", glm::vec4(1.0f, 0.5f, 0.5f, 1.0f)},
        {"Cat", glm::vec4(0.5f, 0.5f, 1.0f, 1.0f)},
        {"Dog", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {"Bird", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)},
        {"Fish", glm::vec4(0.7f, 0.3f, 0.0f, 1.0f)},
        {"StandingPerson", glm::vec4(0.3f, 0.7f, 0.0f, 1.0f)},
        {"SittingPerson", glm::vec4(0.3f, 0.0f, 0.7f, 1.0f)},
        {"RunningPerson", glm::vec4(0.7f, 0.7f, 0.0f, 1.0f)}
    };

    float scale = 0.2f; // 进一步减小实体大小，确保所有实体大小一致
    int entityCount = entityInfos.size();

    int gridCols = 5; // 固定5列布局，确保均匀分布
    int gridRows = (entityCount + gridCols - 1) / gridCols;

    float spacingX = 2.6f / (gridCols + 1); // 大幅增加间距以避免拥挤
    float spacingY = 2.6f / (gridRows + 1);

    for (int i = 0; i < entityCount; i++) {
      int row = i / gridCols;
      int col = i % gridCols;

      float x = -1.3f + (col + 1) * spacingX; // 进一步扩展布局范围
      float y = 1.3f - (row + 1) * spacingY;

      glm::vec2 position(x, y);
      glm::vec4 color = entityInfos[i].color;

      if (entityInfos[i].type == "Triangle") {
        Triangle* triangle = new Triangle(position, color, scale);
        testScene->AddObject(triangle);
        LOG(_T("[OpenGL] Triangle entity added to scene"));
      }
      else if (entityInfos[i].type == "Square") {
        Square* square = new Square(position, color, scale);
        testScene->AddObject(square);
        LOG(_T("[OpenGL] Square entity added to scene"));
      }
      else if (entityInfos[i].type == "Circle") {
        Circle* circle = new Circle(position, color, scale);
        testScene->AddObject(circle);
        LOG(_T("[OpenGL] Circle entity added to scene"));
      }
      else if (entityInfos[i].type == "Arrow") {
        Arrow* arrow = new Arrow(position, color, scale);
        testScene->AddObject(arrow);
        LOG(_T("[OpenGL] Arrow entity added to scene"));
      }
      else if (entityInfos[i].type == "Star") {
        Star* star = new Star(position, color, scale);
        testScene->AddObject(star);
        LOG(_T("[OpenGL] Star entity added to scene"));
      }
      else if (entityInfos[i].type == "Heart") {
        Heart* heart = new Heart(position, color, scale);
        testScene->AddObject(heart);
        LOG(_T("[OpenGL] Heart entity added to scene"));
      }
      else if (entityInfos[i].type == "Pentagram") {
        Pentagram* pentagram = new Pentagram(position, color, scale);
        testScene->AddObject(pentagram);
        LOG(_T("[OpenGL] Pentagram entity added to scene"));
      }
      else if (entityInfos[i].type == "Cup") {
        Cup* cup = new Cup(position, color, scale);
        testScene->AddObject(cup);
        LOG(_T("[OpenGL] Cup entity added to scene"));
      }
      else if (entityInfos[i].type == "Book") {
        Book* book = new Book(position, color, scale);
        testScene->AddObject(book);
        LOG(_T("[OpenGL] Book entity added to scene"));
      }
      else if (entityInfos[i].type == "Pencil") {
        Pencil* pencil = new Pencil(position, color, scale);
        testScene->AddObject(pencil);
        LOG(_T("[OpenGL] Pencil entity added to scene"));
      }
      else if (entityInfos[i].type == "House") {
        House* house = new House(position, color, scale);
        testScene->AddObject(house);
        LOG(_T("[OpenGL] House entity added to scene"));
      }
      else if (entityInfos[i].type == "Car") {
        Car* car = new Car(position, color, scale);
        testScene->AddObject(car);
        LOG(_T("[OpenGL] Car entity added to scene"));
      }
      else if (entityInfos[i].type == "Cat") {
        Cat* cat = new Cat(position, color, scale);
        testScene->AddObject(cat);
        LOG(_T("[OpenGL] Cat entity added to scene"));
      }
      else if (entityInfos[i].type == "Dog") {
        Dog* dog = new Dog(position, color, scale);
        testScene->AddObject(dog);
        LOG(_T("[OpenGL] Dog entity added to scene"));
      }
      else if (entityInfos[i].type == "Bird") {
        Bird* bird = new Bird(position, color, scale);
        testScene->AddObject(bird);
        LOG(_T("[OpenGL] Bird entity added to scene"));
      }
      else if (entityInfos[i].type == "Fish") {
        Fish* fish = new Fish(position, color, scale);
        testScene->AddObject(fish);
        LOG(_T("[OpenGL] Fish entity added to scene"));
      }
      else if (entityInfos[i].type == "StandingPerson") {
        StandingPerson* standingPerson = new StandingPerson(position, color, scale);
        testScene->AddObject(standingPerson);
        LOG(_T("[OpenGL] StandingPerson entity added to scene"));
      }
      else if (entityInfos[i].type == "SittingPerson") {
        SittingPerson* sittingPerson = new SittingPerson(position, color, scale);
        testScene->AddObject(sittingPerson);
        LOG(_T("[OpenGL] SittingPerson entity added to scene"));
      }
      else if (entityInfos[i].type == "RunningPerson") {
        RunningPerson* runningPerson = new RunningPerson(position, color, scale);
        testScene->AddObject(runningPerson);
        LOG(_T("[OpenGL] RunningPerson entity added to scene"));
      }
    }

    //Cube* cube = new Cube(glm::vec2(positions[21][0], positions[21][1]), colors[21], scale);
    //testScene->AddObject(cube);
    //LOG(_T("[OpenGL] Cube entity added to scene"));

    LOG(_T("[OpenGL] Test scene created with multiple entities"));
  }
  else
  {
    LOG(_T("[OpenGL] ERROR: Failed to get SceneManager!"));
  }

  m_bIsRunning = true;
  m_dwLastTime = GetTickCount64();

  SetTimer(1, 16, NULL);

  LOG(_T("[OpenGL] Timer started"));

  return true;
}

void CMainWindow::RenderOpenGL()
{
  LOG(_T("[OpenGL] RenderOpenGL called"));

  if (!m_hGLRC)
  {
    LOG(_T("[OpenGL] ERROR: m_hGLRC is NULL!"));
    return;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  LOG(_T("[OpenGL] Framebuffer cleared"));

  if (m_bDepthTest)
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }

  // 始终使用传统OpenGL渲染默认对象，无论SceneManager是否存在
  LOG(_T("[OpenGL] Rendering default objects"));

  // 使用传统OpenGL渲染默认对象
  glLoadIdentity();

  GLfloat fCameraX = 0.0f, fCameraY = 0.0f, fCameraZ = 5.0f;

  CString strCameraX, strCameraY, strCameraZ;
  m_editCameraX.GetWindowText(strCameraX);
  m_editCameraY.GetWindowText(strCameraY);
  m_editCameraZ.GetWindowText(strCameraZ);

  _stscanf_s(strCameraX, _T("%f"), &fCameraX);
  _stscanf_s(strCameraY, _T("%f"), &fCameraY);
  _stscanf_s(strCameraZ, _T("%f"), &fCameraZ);

  gluLookAt(fCameraX, fCameraY, fCameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  if (m_bWireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (m_bSmoothShading)
  {
    glShadeModel(GL_SMOOTH);
  }
  else
  {
    glShadeModel(GL_FLAT);
  }

  //glPopMatrix();

  // 如果SceneManager存在，也调用其Draw方法
  if (m_pSceneManager)
  {
    LOG(_T("[OpenGL] Rendering scene"));
    m_pSceneManager->Draw();
  }

  CDC* pDC = CDC::FromHandle(::GetDC(m_hWndOpenGL));
  SwapBuffers(pDC->m_hDC);
  ::ReleaseDC(m_hWndOpenGL, pDC->m_hDC);
  LOG(_T("[OpenGL] Buffers swapped"));
}

void CMainWindow::CleanupOpenGL()
{
  LOG(_T("[OpenGL] CleanupOpenGL called"));

  if (m_hGLRC)
  {
    if (wglGetCurrentContext() == m_hGLRC)
    {
      wglMakeCurrent(NULL, NULL);
    }
    wglDeleteContext(m_hGLRC);
    m_hGLRC = NULL;
    LOG(_T("[OpenGL] OpenGL context deleted"));
  }

  if (m_pSceneManager)
  {
    m_pSceneManager = NULL;
    LOG(_T("[OpenGL] SceneManager released"));
  }

  KillTimer(1);
  LOG(_T("[OpenGL] Timer killed"));
}

void CMainWindow::UpdateStatus(const CString& strStatus)
{
  LOG(_T("[MainWindow] UpdateStatus"));

  if (m_editStatus.m_hWnd)
  {
    m_editStatus.SetWindowText(strStatus);
  }
}

void CMainWindow::OnCameraXChange()
{
  LOG(_T("[MainWindow] Camera X changed"));
  UpdateStatus(_T("Camera X position updated"));
}

void CMainWindow::OnCameraYChange()
{
  LOG(_T("[MainWindow] Camera Y changed"));
  UpdateStatus(_T("Camera Y position updated"));
}

void CMainWindow::OnCameraZChange()
{
  LOG(_T("[MainWindow] Camera Z changed"));
  UpdateStatus(_T("Camera Z position updated"));
}

void CMainWindow::OnCheckWireframe()
{
  LOG(_T("[MainWindow] Wireframe checkbox changed"));
  m_bWireframe = (m_checkWireframe.GetCheck() == 1);
  UpdateStatus(m_bWireframe ? _T("Wireframe mode enabled") : _T("Wireframe mode disabled"));
}

void CMainWindow::OnCheckSmooth()
{
  LOG(_T("[MainWindow] Smooth shading checkbox changed"));
  m_bSmoothShading = (m_checkSmooth.GetCheck() == 1);
  UpdateStatus(m_bSmoothShading ? _T("Smooth shading enabled") : _T("Smooth shading disabled"));
}

void CMainWindow::OnCheckDepth()
{
  LOG(_T("[MainWindow] Depth test checkbox changed"));
  m_bDepthTest = (m_checkDepth.GetCheck() == 1);
  UpdateStatus(m_bDepthTest ? _T("Depth test enabled") : _T("Depth test disabled"));
}

void CMainWindow::ApplyCameraSettings()
{
  LOG(_T("[MainWindow] ApplyCameraSettings called"));

  if (m_hGLRC)
  {
    CDC* pDC = CDC::FromHandle(::GetDC(m_hWndOpenGL));
    wglMakeCurrent(pDC->m_hDC, m_hGLRC);
    RenderOpenGL();
    wglMakeCurrent(NULL, NULL);
  }
}

void CMainWindow::UpdateUILayout()
{
  LOG(_T("[MainWindow] UpdateUILayout called"));

  if (!IsWindow(m_hWnd))
    return;

  CRect rect;
  GetClientRect(rect);

  int nClientWidth = rect.Width();
  int nClientHeight = rect.Height();

  if (nClientWidth <= 0 || nClientHeight <= 0)
    return;

  m_nRenderAreaX = 0;
  m_nRenderAreaY = 0;
  m_nRenderAreaWidth = (int)(nClientWidth * 0.7f);
  m_nRenderAreaHeight = nClientHeight;

  if (IsWindow(m_staticOpenGL.m_hWnd))
  {
    m_staticOpenGL.MoveWindow(m_nRenderAreaX, m_nRenderAreaY, m_nRenderAreaWidth, m_nRenderAreaHeight, TRUE);
  }

  int nControlStartX = m_nRenderAreaX + m_nRenderAreaWidth + 20;
  int nControlWidth = std::max(150, nClientWidth - nControlStartX - 20);
  int nControlHeight = 30;
  int nControlSpacing = 45;
  int nY = 30;

  if (IsWindow(m_staticStatus.m_hWnd))
  {
    m_staticStatus.MoveWindow(nControlStartX, nY, nControlWidth, 20, TRUE);
  }

  nY += 25;

  if (IsWindow(m_editStatus.m_hWnd))
  {
    m_editStatus.MoveWindow(nControlStartX, nY, nControlWidth, nControlHeight, TRUE);
  }

  nY += nControlSpacing + 10;

  if (IsWindow(m_staticCamera.m_hWnd))
  {
    m_staticCamera.MoveWindow(nControlStartX, nY, nControlWidth, 20, TRUE);
  }

  nY += 25;

  if (IsWindow(m_editCameraX.m_hWnd))
  {
    m_editCameraX.MoveWindow(nControlStartX, nY, nControlWidth, nControlHeight, TRUE);
  }

  if (IsWindow(m_editCameraY.m_hWnd))
  {
    m_editCameraY.MoveWindow(nControlStartX, nY + nControlSpacing, nControlWidth, nControlHeight, TRUE);
  }

  if (IsWindow(m_editCameraZ.m_hWnd))
  {
    m_editCameraZ.MoveWindow(nControlStartX, nY + nControlSpacing * 2, nControlWidth, nControlHeight, TRUE);
  }

  nY += nControlSpacing * 3 + 10;

  if (IsWindow(m_staticOptions.m_hWnd))
  {
    m_staticOptions.MoveWindow(nControlStartX, nY, nControlWidth, 20, TRUE);
  }

  nY += 25;

  if (IsWindow(m_checkWireframe.m_hWnd))
  {
    m_checkWireframe.MoveWindow(nControlStartX, nY, nControlWidth, nControlHeight, TRUE);
  }

  if (IsWindow(m_checkSmooth.m_hWnd))
  {
    m_checkSmooth.MoveWindow(nControlStartX, nY + nControlSpacing, nControlWidth, nControlHeight, TRUE);
  }

  if (IsWindow(m_checkDepth.m_hWnd))
  {
    m_checkDepth.MoveWindow(nControlStartX, nY + nControlSpacing * 2, nControlWidth, nControlHeight, TRUE);
  }

  InvalidateRect(NULL, FALSE);
}
