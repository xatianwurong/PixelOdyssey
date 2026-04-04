#include "AcadUIElement.h"
#include "../core/ColorScheme.h"

IMPLEMENT_DYNAMIC(CAcadUIElement, CWnd)

CAcadUIElement::CAcadUIElement()
  : m_bVisible(true)
{
  // 创建字体 - 优化中文显示
  LOGFONT lf = { 0 };
  lf.lfHeight = -UIFonts::BODY_SIZE;
  lf.lfWeight = UIFonts::BODY_WEIGHT;
  lf.lfQuality = CLEARTYPE_QUALITY;
  lf.lfCharSet = DEFAULT_CHARSET;
  _tcscpy_s(lf.lfFaceName, UIFonts::FONT_FAMILY);
  m_fontNormal.CreateFontIndirect(&lf);

  lf.lfHeight = -UIFonts::HEADING_SIZE;
  lf.lfWeight = UIFonts::HEADING_WEIGHT;
  m_fontTitle.CreateFontIndirect(&lf);

  lf.lfHeight = -UIFonts::CAPTION_SIZE;
  lf.lfWeight = UIFonts::BODY_WEIGHT;
  m_fontSmall.CreateFontIndirect(&lf);
}

CAcadUIElement::~CAcadUIElement()
{
  m_fontTitle.DeleteObject();
  m_fontNormal.DeleteObject();
  m_fontSmall.DeleteObject();
}

BEGIN_MESSAGE_MAP(CAcadUIElement, CWnd)
  ON_WM_CREATE()
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CAcadUIElement::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
  DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

  if (!CWnd::Create(NULL, _T(""), dwStyle, rect, pParent, nID, NULL)) {
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

  DrawBackground(&dc, m_rectClient);
  OnDraw(&dc);
  DrawBorder(&dc, m_rectClient);
}

BOOL CAcadUIElement::OnEraseBkgnd(CDC* pDC)
{
  return TRUE;  // 在 OnPaint 中绘制背景
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
