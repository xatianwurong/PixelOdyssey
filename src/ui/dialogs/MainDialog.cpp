#include "MainDialog.h"
#include <GL/glew.h>
#include <afxdialogex.h>

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

BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_ERASEBKGND()
    ON_WM_WINDOWPOSCHANGING()
    ON_BN_CLICKED(IDC_BUTTON_START, &CMainDialog::OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CMainDialog::OnBnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_RESET, &CMainDialog::OnBnClickedButtonReset)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CMainDialog::OnBnClickedButtonApply)
    ON_BN_CLICKED(IDC_CHECK_WIREFRAME, &CMainDialog::OnBnClickedCheckWireframe)
    ON_BN_CLICKED(IDC_CHECK_SMOOTH, &CMainDialog::OnBnClickedCheckSmooth)
    ON_BN_CLICKED(IDC_CHECK_DEPTH, &CMainDialog::OnBnClickedCheckDepth)
END_MESSAGE_MAP()

CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MAINDIALOG, pParent),
    m_hGLRC(nullptr),
    m_hWndOpenGL(nullptr),
    m_pSceneManager(nullptr),
    m_fDeltaTime(0.0f),
    m_dwLastTime(0),
    m_bIsRunning(false),
    m_bWireframe(false),
    m_bSmoothShading(true),
    m_bDepthTest(true),
    m_nClientWidth(1280),
    m_nClientHeight(800),
    m_nRenderAreaX(400),
    m_nRenderAreaY(20),
    m_nRenderAreaWidth(860),
    m_nRenderAreaHeight(760)
{
}

CMainDialog::~CMainDialog()
{
    CleanupOpenGL();
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
    DDX_Control(pDX, IDC_EDIT_CAMERA_X, m_editCameraX);
    DDX_Control(pDX, IDC_EDIT_CAMERA_Y, m_editCameraY);
    DDX_Control(pDX, IDC_EDIT_CAMERA_Z, m_editCameraZ);
    DDX_Control(pDX, IDC_CHECK_WIREFRAME, m_checkWireframe);
    DDX_Control(pDX, IDC_CHECK_SMOOTH, m_checkSmooth);
    DDX_Control(pDX, IDC_CHECK_DEPTH, m_checkDepth);
    DDX_Control(pDX, IDC_STATIC_OPENGL, m_staticOpenGL);
}

BOOL CMainDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetWindowText(_T("OpenGL MFC Application - Modern Renderer"));

    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, MAX_PATH);
    CString strPath(szPath);
    int nPos = strPath.ReverseFind('\\');
    if (nPos != -1)
    {
        strPath = strPath.Left(nPos);
        nPos = strPath.ReverseFind('\\');
        if (nPos != -1)
        {
            strPath = strPath.Left(nPos);
        }
        SetCurrentDirectory(strPath);
    }

    m_hWndOpenGL = ::GetDlgItem(m_hWnd, IDC_STATIC_OPENGL);

    m_pSceneManager = &SceneManager::Instance();

    if (!InitOpenGL())
    {
        UpdateStatus(_T("OpenGL initialization failed!"));
        return FALSE;
    }

    try
    {
        Scene* pScene = new Scene("MainScene");
        
        Circle* pCircle = new Circle(glm::vec2(-0.5f, 0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.3f);
        Square* pSquare = new Square(glm::vec2(0.0f, 0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.4f);
        Triangle* pTriangle = new Triangle(glm::vec2(0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.4f);
        
        Arrow* pArrow = new Arrow(glm::vec2(0.5f, 0.0f), glm::vec4(0.5f, 0.5f, 1.0f, 1.0f), 0.4f, 0.1f);
        Star* pStar = new Star(glm::vec2(-0.5f, -0.5f), glm::vec4(1.0f, 0.8f, 0.0f, 1.0f), 0.4f);
        Heart* pHeart = new Heart(glm::vec2(0.0f, -0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.4f);
        Pentagram* pPentagram = new Pentagram(glm::vec2(0.5f, -0.5f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.4f);
        
        pScene->AddObject(pCircle);
        pScene->AddObject(pSquare);
        pScene->AddObject(pTriangle);
        pScene->AddObject(pArrow);
        pScene->AddObject(pStar);
        pScene->AddObject(pHeart);
        pScene->AddObject(pPentagram);
        
        Cup* pCup = new Cup(glm::vec2(-1.0f, 0.0f), glm::vec4(0.8f, 0.8f, 1.0f, 1.0f), 0.3f);
        Book* pBook = new Book(glm::vec2(-1.0f, -0.5f), glm::vec4(0.9f, 0.8f, 0.7f, 1.0f), 0.3f);
        Pencil* pPencil = new Pencil(glm::vec2(1.0f, 0.0f), glm::vec4(0.8f, 0.6f, 0.4f, 1.0f), 0.5f);
        House* pHouse = new House(glm::vec2(1.0f, 0.5f), glm::vec4(0.8f, 0.6f, 0.4f, 1.0f), 0.3f);
        Car* pCar = new Car(glm::vec2(1.0f, -0.5f), glm::vec4(0.0f, 0.6f, 0.8f, 1.0f), 0.4f);
        
        Cat* pCat = new Cat(glm::vec2(-1.0f, 0.5f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 0.3f);
        Dog* pDog = new Dog(glm::vec2(-1.0f, -1.0f), glm::vec4(0.8f, 0.6f, 0.4f, 1.0f), 0.3f);
        Fish* pFish = new Fish(glm::vec2(1.0f, -1.0f), glm::vec4(0.0f, 0.6f, 0.8f, 1.0f), 0.4f);
        Bird* pBird = new Bird(glm::vec2(1.0f, 1.0f), glm::vec4(0.8f, 0.4f, 0.8f, 1.0f), 0.3f);
        
        StandingPerson* pStandingPerson = new StandingPerson(glm::vec2(-0.5f, -1.0f), glm::vec4(0.6f, 0.4f, 0.2f, 1.0f), 0.4f);
        SittingPerson* pSittingPerson = new SittingPerson(glm::vec2(0.0f, -1.0f), glm::vec4(0.6f, 0.4f, 0.2f, 1.0f), 0.3f);
        RunningPerson* pRunningPerson = new RunningPerson(glm::vec2(0.5f, -1.0f), glm::vec4(0.6f, 0.4f, 0.2f, 1.0f), 0.4f);
        
        pScene->AddObject(pCup);
        pScene->AddObject(pBook);
        pScene->AddObject(pPencil);
        pScene->AddObject(pHouse);
        pScene->AddObject(pCar);
        pScene->AddObject(pCat);
        pScene->AddObject(pDog);
        pScene->AddObject(pFish);
        pScene->AddObject(pBird);
        pScene->AddObject(pStandingPerson);
        pScene->AddObject(pSittingPerson);
        pScene->AddObject(pRunningPerson);
        
        m_pSceneManager->AddScene(pScene);
        m_pSceneManager->SetActiveScene("MainScene");
    }
    catch (const std::exception& e)
    {
        CString strError;
        strError.Format(_T("Scene initialization failed: %s"), e.what());
        AfxMessageBox(strError);
        UpdateStatus(_T("Scene initialization failed!"));
        return FALSE;
    }

    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetCameraPosition(0.0f, 0.0f, 5.0f);
    dataStorage.SetWireframe(false);
    dataStorage.SetSmoothShading(true);
    dataStorage.SetDepthTest(true);
    dataStorage.SetIsRunning(false);

    m_editCameraX.SetWindowText(_T("0"));
    m_editCameraY.SetWindowText(_T("0"));
    m_editCameraZ.SetWindowText(_T("5"));
    m_checkWireframe.SetCheck(BST_UNCHECKED);
    m_checkSmooth.SetCheck(BST_CHECKED);
    m_checkDepth.SetCheck(BST_CHECKED);

    SetTimer(1, 16, nullptr);
    m_dwLastTime = GetTickCount64();

    UpdateStatus(_T("Ready - OpenGL Renderer initialized"));

    return TRUE;
}

void CMainDialog::OnBnClickedButtonApply()
{
    ApplyCameraSettings();
    UpdateStatus(_T("Camera settings applied successfully"));
}

void CMainDialog::OnBnClickedCheckWireframe()
{
    m_bWireframe = (m_checkWireframe.GetCheck() == BST_CHECKED);
    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetWireframe(m_bWireframe);
    if (m_hGLRC && m_hWndOpenGL)
    {
        HDC hDC = ::GetDC(m_hWndOpenGL);
        wglMakeCurrent(hDC, m_hGLRC);
        if (m_bWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        wglMakeCurrent(nullptr, nullptr);
        ::ReleaseDC(m_hWndOpenGL, hDC);
    }
}

void CMainDialog::OnBnClickedCheckSmooth()
{
    m_bSmoothShading = (m_checkSmooth.GetCheck() == BST_CHECKED);
    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetSmoothShading(m_bSmoothShading);
    if (m_hGLRC && m_hWndOpenGL)
    {
        HDC hDC = ::GetDC(m_hWndOpenGL);
        wglMakeCurrent(hDC, m_hGLRC);
        if (m_bSmoothShading)
        {
            glShadeModel(GL_SMOOTH);
        }
        else
        {
            glShadeModel(GL_FLAT);
        }
        wglMakeCurrent(nullptr, nullptr);
        ::ReleaseDC(m_hWndOpenGL, hDC);
    }
}

void CMainDialog::OnBnClickedCheckDepth()
{
    m_bDepthTest = (m_checkDepth.GetCheck() == BST_CHECKED);
    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetDepthTest(m_bDepthTest);
    if (m_hGLRC && m_hWndOpenGL)
    {
        HDC hDC = ::GetDC(m_hWndOpenGL);
        wglMakeCurrent(hDC, m_hGLRC);
        if (m_bDepthTest)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        wglMakeCurrent(nullptr, nullptr);
        ::ReleaseDC(m_hWndOpenGL, hDC);
    }
}

void CMainDialog::ApplyCameraSettings()
{
    CString strX, strY, strZ;
    m_editCameraX.GetWindowText(strX);
    m_editCameraY.GetWindowText(strY);
    m_editCameraZ.GetWindowText(strZ);

    float x = _tstof(strX);
    float y = _tstof(strY);
    float z = _tstof(strZ);

    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetCameraPosition(x, y, z);
}

void CMainDialog::OnBnClickedButtonStart()
{
    m_bIsRunning = true;
    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetIsRunning(true);
    UpdateStatus(_T("Rendering - Animation playing"));
}

void CMainDialog::OnBnClickedButtonStop()
{
    m_bIsRunning = false;
    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetIsRunning(false);
    UpdateStatus(_T("Paused - Animation stopped"));
}

void CMainDialog::OnBnClickedButtonReset()
{
    m_bIsRunning = false;
    CDataStorage& dataStorage = CDataStorage::Instance();
    dataStorage.SetIsRunning(false);
    Scene* pActiveScene = m_pSceneManager->GetActiveScene();
    if (pActiveScene)
    {
        m_pSceneManager->RemoveScene(pActiveScene->GetName());
    }
    Scene* pScene = new Scene("MainScene");
    
    Circle* pCircle = new Circle(glm::vec2(-0.6f, 0.6f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.3f);
    Square* pSquare = new Square(glm::vec2(0.0f, 0.6f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.6f);
    Triangle* pTriangle = new Triangle(glm::vec2(0.6f, 0.6f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.6f);

    Arrow* pArrow = new Arrow(glm::vec2(0.6f, 0.0f), glm::vec4(0.5f, 0.5f, 1.0f, 1.0f), 0.6f, 0.1f);
    Star* pStar = new Star(glm::vec2(-0.6f, -0.6f), glm::vec4(1.0f, 0.8f, 0.0f, 1.0f), 0.6f);
    Heart* pHeart = new Heart(glm::vec2(0.0f, -0.6f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.6f);
    Pentagram* pPentagram = new Pentagram(glm::vec2(0.6f, -0.6f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.6f);
    
    pScene->AddObject(pCircle);
    pScene->AddObject(pSquare);
    pScene->AddObject(pTriangle);
    pScene->AddObject(pArrow);
    pScene->AddObject(pStar);
    pScene->AddObject(pHeart);
    pScene->AddObject(pPentagram);
    
    Cup* pCup = new Cup(glm::vec2(-1.0f, 0.0f), glm::vec4(0.8f, 0.8f, 1.0f, 1.0f), 0.3f);
    Book* pBook = new Book(glm::vec2(-1.0f, -0.5f), glm::vec4(0.9f, 0.8f, 0.7f, 1.0f), 0.3f);
    Pencil* pPencil = new Pencil(glm::vec2(1.0f, 0.0f), glm::vec4(0.8f, 0.6f, 0.4f, 1.0f), 0.5f);
    House* pHouse = new House(glm::vec2(1.0f, 0.5f), glm::vec4(0.8f, 0.6f, 0.4f, 1.0f), 0.3f);
    Car* pCar = new Car(glm::vec2(1.0f, -0.5f), glm::vec4(0.0f, 0.6f, 0.8f, 1.0f), 0.4f);
    
    Cat* pCat = new Cat(glm::vec2(-1.0f, 0.5f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 0.3f);
    Dog* pDog = new Dog(glm::vec2(-1.0f, -1.0f), glm::vec4(0.8f, 0.6f, 0.4f, 1.0f), 0.3f);
    Fish* pFish = new Fish(glm::vec2(1.0f, -1.0f), glm::vec4(0.0f, 0.6f, 0.8f, 1.0f), 0.4f);
    Bird* pBird = new Bird(glm::vec2(1.0f, 1.0f), glm::vec4(0.8f, 0.4f, 0.8f, 1.0f), 0.3f);
    
    StandingPerson* pStandingPerson = new StandingPerson(glm::vec2(-0.5f, -1.0f), glm::vec4(0.6f, 0.4f, 0.2f, 1.0f), 0.4f);
    SittingPerson* pSittingPerson = new SittingPerson(glm::vec2(0.0f, -1.0f), glm::vec4(0.6f, 0.4f, 0.2f, 1.0f), 0.3f);
    RunningPerson* pRunningPerson = new RunningPerson(glm::vec2(0.5f, -1.0f), glm::vec4(0.6f, 0.4f, 0.2f, 1.0f), 0.4f);
    
    pScene->AddObject(pCup);
    pScene->AddObject(pBook);
    pScene->AddObject(pPencil);
    pScene->AddObject(pHouse);
    pScene->AddObject(pCar);
    pScene->AddObject(pCat);
    pScene->AddObject(pDog);
    pScene->AddObject(pFish);
    pScene->AddObject(pBird);
    pScene->AddObject(pStandingPerson);
    pScene->AddObject(pSittingPerson);
    pScene->AddObject(pRunningPerson);
    
    m_pSceneManager->AddScene(pScene);
    m_pSceneManager->SetActiveScene("MainScene");
    UpdateStatus(_T("Reset - Scene reinitialized"));
}

void CMainDialog::UpdateStatus(const CString& strStatus)
{
    m_editStatus.SetWindowText(strStatus);
}

BOOL CMainDialog::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    
    pDC->FillSolidRect(&rect, RGB(245, 247, 250));
    
    CRect leftPanel(0, 0, 350, rect.Height());
    pDC->FillSolidRect(&leftPanel, RGB(255, 255, 255));
    
    return TRUE;
}

void CMainDialog::OnPaint()
{
    CPaintDC dc(this);

    RenderOpenGL();
}

void CMainDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (m_hGLRC && m_hWndOpenGL && cx > 0 && cy > 0)
    {
        CRect rect;
        ::GetClientRect(m_hWndOpenGL, &rect);
        
        if (rect.Width() > 0 && rect.Height() > 0)
        {
            HDC hDC = ::GetDC(m_hWndOpenGL);
            wglMakeCurrent(hDC, m_hGLRC);
            glViewport(0, 0, rect.Width(), rect.Height());
            wglMakeCurrent(nullptr, nullptr);
            ::ReleaseDC(m_hWndOpenGL, hDC);
        }
    }
}

void CMainDialog::OnTimer(UINT_PTR nIDEvent)
{
    CDialogEx::OnTimer(nIDEvent);

    DWORD dwCurrentTime = GetTickCount64();
    m_fDeltaTime = (dwCurrentTime - m_dwLastTime) / 1000.0f;
    m_dwLastTime = dwCurrentTime;

    if (m_pSceneManager)
    {
        m_pSceneManager->Update(m_fDeltaTime);
    }

    if (m_hWndOpenGL)
    {
        ::InvalidateRect(m_hWndOpenGL, nullptr, FALSE);
    }
}

bool CMainDialog::InitOpenGL()
{
    if (!m_hWndOpenGL)
    {
        AfxMessageBox(_T("OpenGL render area not found!"));
        return false;
    }

    HDC hDC = ::GetDC(m_hWndOpenGL);
    if (!hDC)
    {
        AfxMessageBox(_T("Failed to get DC!"));
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, nPixelFormat, &pfd);

    m_hGLRC = wglCreateContext(hDC);
    if (!m_hGLRC)
    {
        AfxMessageBox(_T("Failed to create OpenGL context!"));
        ::ReleaseDC(m_hWndOpenGL, hDC);
        return false;
    }

    wglMakeCurrent(hDC, m_hGLRC);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        CString strError;
        strError.Format(_T("GLEW initialization failed: %s"), glewGetErrorString(err));
        AfxMessageBox(strError);
        ::ReleaseDC(m_hWndOpenGL, hDC);
        return false;
    }

    glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    CRect rect;
    ::GetClientRect(m_hWndOpenGL, &rect);
    glViewport(0, 0, rect.Width(), rect.Height());

    ::ReleaseDC(m_hWndOpenGL, hDC);

    wglMakeCurrent(nullptr, nullptr);
    return true;
}

void CMainDialog::RenderOpenGL()
{
    if (!m_hGLRC || !m_hWndOpenGL)
        return;

    HDC hDC = ::GetDC(m_hWndOpenGL);
    if (!hDC)
        return;

    wglMakeCurrent(hDC, m_hGLRC);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_pSceneManager)
    {
        m_pSceneManager->Draw();
    }

    SwapBuffers(hDC);

    wglMakeCurrent(nullptr, nullptr);

    ::ReleaseDC(m_hWndOpenGL, hDC);
}

void CMainDialog::CleanupOpenGL()
{
    if (m_hGLRC)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hGLRC);
        m_hGLRC = nullptr;
    }
}

void CMainDialog::OnWindowPosChanging(WINDOWPOS* lpWindowPos)
{
    CDialogEx::OnWindowPosChanging(lpWindowPos);
    
    if (lpWindowPos->cx > 1000 && lpWindowPos->cy > 600)
    {
        m_nClientWidth = lpWindowPos->cx;
        m_nClientHeight = lpWindowPos->cy;
        
        m_nRenderAreaX = 400;
        m_nRenderAreaY = 20;
        m_nRenderAreaWidth = m_nClientWidth - 420;
        m_nRenderAreaHeight = m_nClientHeight - 40;
        
        if (m_nRenderAreaWidth < 600) m_nRenderAreaWidth = 600;
        if (m_nRenderAreaHeight < 500) m_nRenderAreaHeight = 500;
        
        ::SetWindowPos(m_hWndOpenGL, nullptr, m_nRenderAreaX, m_nRenderAreaY, 
                      m_nRenderAreaWidth, m_nRenderAreaHeight, SWP_NOZORDER | SWP_NOACTIVATE);
    }
}
