#include "UIBase.h"

IMPLEMENT_DYNAMIC(UIBase, CWnd)

UIBase::UIBase()
    : m_bVisible(true)
    , m_bEnabled(true)
    , m_bHover(false)
    , m_bFocused(false)
{
    CreateFonts();
}

UIBase::~UIBase()
{
    m_fontTitle.DeleteObject();
    m_fontNormal.DeleteObject();
    m_fontSmall.DeleteObject();
}

BEGIN_MESSAGE_MAP(UIBase, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BOOL UIBase::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    if (!CWnd::Create(NULL, _T(""), dwStyle, rect, pParent, nID, NULL)) {
        return FALSE;
    }

    return TRUE;
}

void UIBase::SetVisible(bool visible)
{
    m_bVisible = visible;
    ShowWindow(visible ? SW_SHOW : SW_HIDE);
    Invalidate();
}

void UIBase::SetEnabled(bool enabled)
{
    m_bEnabled = enabled;
    EnableWindow(enabled ? TRUE : FALSE);
    Invalidate();
}

COLORREF UIBase::GetThemeColor(const CString& colorKey)
{
    // 颜色映射表
    if (colorKey == _T("primary")) return ColorScheme::Current::PRIMARY;
    if (colorKey == _T("primary_hover")) return ColorScheme::Current::PRIMARY_HOVER;
    if (colorKey == _T("primary_press")) return ColorScheme::Current::PRIMARY_PRESS;
    if (colorKey == _T("success")) return ColorScheme::Current::SUCCESS;
    if (colorKey == _T("warning")) return ColorScheme::Current::WARNING;
    if (colorKey == _T("error")) return ColorScheme::Current::ERROR;
    if (colorKey == _T("info")) return ColorScheme::Current::INFO;
    if (colorKey == _T("text")) return ColorScheme::Current::TEXT_PRIMARY;
    if (colorKey == _T("text_secondary")) return ColorScheme::Current::TEXT_SECONDARY;
    if (colorKey == _T("text_tertiary")) return ColorScheme::Current::TEXT_TERTIARY;
    if (colorKey == _T("bg_primary")) return ColorScheme::Current::BG_PRIMARY;
    if (colorKey == _T("bg_panel")) return ColorScheme::Current::BG_PANEL;
    if (colorKey == _T("border")) return ColorScheme::Current::BORDER;
    
    return ColorScheme::Current::TEXT_PRIMARY;  // 默认色
}

void UIBase::OnHover(bool bHover)
{
    if (m_bHover != bHover) {
        m_bHover = bHover;
        Invalidate();
    }
}

void UIBase::OnFocus(bool bFocus)
{
    if (m_bFocused != bFocus) {
        m_bFocused = bFocus;
        Invalidate();
    }
}

int UIBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void UIBase::OnPaint()
{
    CPaintDC dc(this);
    GetClientRect(&m_rectClient);

    DrawBackground(&dc, m_rectClient);
    OnDraw(&dc);
    DrawBorder(&dc, m_rectClient);
    
    if (m_bFocused) {
        DrawFocusRect(&dc, m_rectClient);
    }
}

BOOL UIBase::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;  // 在 OnPaint 中绘制背景
}

void UIBase::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    Invalidate();
}

void UIBase::OnMouseMove(UINT nFlags, CPoint point)
{
    OnHover(true);
    CWnd::OnMouseMove(nFlags, point);
}

void UIBase::OnMouseLeave()
{
    OnHover(false);
    CWnd::OnMouseLeave();
}

void UIBase::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);
    OnFocus(true);
}

void UIBase::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
    OnFocus(false);
}

void UIBase::DrawBackground(CDC* pDC, const CRect& rect)
{
    CBrush brush(ColorScheme::Current::BG_PANEL);
    pDC->FillRect(&rect, &brush);
}

void UIBase::DrawBorder(CDC* pDC, const CRect& rect)
{
    COLORREF borderColor = m_bFocused ? 
        ColorScheme::Current::BORDER_FOCUS : 
        ColorScheme::Current::BORDER;
    
    CPen pen(PS_SOLID, 1, borderColor);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);
    pDC->SelectObject(pOldPen);
}

void UIBase::DrawFocusRect(CDC* pDC, const CRect& rect)
{
    CPen pen(PS_SOLID, 2, ColorScheme::Current::BORDER_FOCUS);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    
    CRect focusRect = rect;
    focusRect.InflateRect(-2, -2);
    pDC->Rectangle(&focusRect);
    
    pDC->SelectObject(pOldPen);
}

void UIBase::CreateFonts()
{
    LOGFONT lf = { 0 };
    lf.lfQuality = CLEARTYPE_QUALITY;
    lf.lfCharSet = DEFAULT_CHARSET;
    wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"Microsoft YaHei UI");

    // 标题字体 - 16pt 粗体
    lf.lfHeight = -16;
    lf.lfWeight = FW_BOLD;
    m_fontTitle.CreateFontIndirect(&lf);

    // 正常字体 - 12pt
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    m_fontNormal.CreateFontIndirect(&lf);

    // 小字体 - 10pt
    lf.lfHeight = -10;
    lf.lfWeight = FW_NORMAL;
    m_fontSmall.CreateFontIndirect(&lf);
}