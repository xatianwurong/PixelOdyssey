#include "PanelBase.h"

IMPLEMENT_DYNAMIC(CPanelBase, CWnd)

CPanelBase::CPanelBase()
    : m_strTitle(_T("Panel"))
    , m_nPanelID(0)
    , m_bVisible(true)
    , m_colorBackground(RGB(240, 240, 240))
    , m_colorBorder(RGB(200, 200, 200))
{
    m_fontTitle.CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
                           ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Segoe UI"));
    
    m_fontNormal.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
                            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Segoe UI"));
}

CPanelBase::~CPanelBase()
{
    m_fontTitle.DeleteObject();
    m_fontNormal.DeleteObject();
}

BOOL CPanelBase::Create(CWnd* pParent, const CString& title, const CRect& rect)
{
    m_strTitle = title;
    
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    
    if (!CWnd::Create(NULL, title, dwStyle, rect, pParent, m_nPanelID, NULL)) {
        return FALSE;
    }
    
    CreateControls();
    
    return TRUE;
}

void CPanelBase::SetVisible(bool visible)
{
    m_bVisible = visible;
    ShowWindow(visible ? SW_SHOW : SW_HIDE);
}

BEGIN_MESSAGE_MAP(CPanelBase, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

int CPanelBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CPanelBase::OnPaint()
{
    CPaintDC dc(this);
    OnDraw(&dc);
}

BOOL CPanelBase::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    
    CBrush brush(m_colorBackground);
    pDC->FillRect(&rect, &brush);
    
    // 绘制边框
    CPen pen(PS_SOLID, 1, m_colorBorder);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->Rectangle(&rect);
    pDC->SelectObject(pOldPen);
    
    return TRUE;
}

void CPanelBase::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    OnPanelSize(cx, cy);
}
