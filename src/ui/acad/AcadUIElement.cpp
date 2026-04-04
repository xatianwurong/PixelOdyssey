#include "AcadUIElement.h"
#include "../core/ColorScheme.h"
#include "UILayout.h"

IMPLEMENT_DYNAMIC(CAcadUIElement, CWnd)

CAcadUIElement::CAcadUIElement()
    : m_bVisible(true)
{
    InitializeFonts();
}

CAcadUIElement::~CAcadUIElement()
{
    // 字体资源自动释放 (CFont 析构函数会处理)
}

BEGIN_MESSAGE_MAP(CAcadUIElement, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

void CAcadUIElement::InitializeFonts()
{
    // 使用 UILayout 中定义的统一字体配置
    LOGFONT lf = {};
    
    // 正常字体 - 正文使用
    lf.lfHeight = -UILayout::UIFonts::BODY_SIZE;
    lf.lfWeight = UILayout::UIFonts::BODY_WEIGHT;
    lf.lfQuality = CLEARTYPE_QUALITY;
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    _tcscpy_s(lf.lfFaceName, UILayout::UIFonts::FONT_FAMILY);
    m_fontNormal.CreateFontIndirect(&lf);

    // 标题字体 - 更粗更大
    lf.lfHeight = -UILayout::UIFonts::HEADING_SIZE;
    lf.lfWeight = UILayout::UIFonts::HEADING_WEIGHT;
    m_fontTitle.CreateFontIndirect(&lf);

    // 小字体 - 说明文字
    lf.lfHeight = -UILayout::UIFonts::CAPTION_SIZE;
    lf.lfWeight = UILayout::UIFonts::BODY_WEIGHT;
    m_fontSmall.CreateFontIndirect(&lf);
}

BOOL CAcadUIElement::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    if (!CWnd::Create(NULL, _T(""), dwStyle, rect, pParent, nID, nullptr))
    {
        return FALSE;
    }

    return TRUE;
}

void CAcadUIElement::SetVisible(bool visible)
{
    m_bVisible = visible;
    ShowWindow(visible ? SW_SHOW : SW_HIDE);
}

int CAcadUIElement::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CAcadUIElement::OnPaint()
{
    CPaintDC dc(this);
    GetClientRect(&m_rectClient);

    // 三阶段绘制：背景 -> 内容 -> 边框
    DrawBackground(&dc, m_rectClient);
    OnDraw(&dc);
    DrawBorder(&dc, m_rectClient);
}

BOOL CAcadUIElement::OnEraseBkgnd(CDC* pDC)
{
    // 在 OnPaint 中统一绘制背景，避免闪烁
    return TRUE;
}

void CAcadUIElement::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    Invalidate();
}

void CAcadUIElement::DrawBackground(CDC* pDC, const CRect& rect)
{
    auto& colors = ColorScheme::Instance();
    CBrush brush(colors.GetColor(ColorScheme::ColorRole::Surface));
    pDC->FillRect(&rect, &brush);
}

void CAcadUIElement::DrawBorder(CDC* pDC, const CRect& rect)
{
    auto& colors = ColorScheme::Instance();
    CPen pen(PS_SOLID, 1, colors.GetColor(ColorScheme::ColorRole::Border));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);
    pDC->SelectObject(pOldPen);
}
