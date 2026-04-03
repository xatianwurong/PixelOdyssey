#include "AcadUIElement.h"

IMPLEMENT_DYNAMIC(CAcadUIElement, CWnd)

// 静态颜色定义 - AutoCAD 风格深色主题
COLORREF CAcadUIElement::ms_colorBackground = RGB(44, 62, 80);      // #2C3E50
COLORREF CAcadUIElement::ms_colorPanelBg = RGB(52, 73, 94);         // #34495E
COLORREF CAcadUIElement::ms_colorAccent = RGB(52, 152, 219);        // #3498DB
COLORREF CAcadUIElement::ms_colorAccentHover = RGB(41, 128, 185);   // #2980B9
COLORREF CAcadUIElement::ms_colorText = RGB(236, 240, 241);         // #ECF0F1
COLORREF CAcadUIElement::ms_colorTextMuted = RGB(149, 165, 166);    // #95A5A6
COLORREF CAcadUIElement::ms_colorBorder = RGB(74, 111, 165);        // #4A6FA5
COLORREF CAcadUIElement::ms_colorHighlight = RGB(231, 76, 60);      // #E74C3C

CAcadUIElement::CAcadUIElement()
  : m_bVisible(true)
{
  // 创建字体
  LOGFONT lf = { 0 };
  lf.lfHeight = -14;
  lf.lfWeight = FW_NORMAL;
  lf.lfQuality = CLEARTYPE_QUALITY;
  _tcscpy_s(lf.lfFaceName, _T("Segoe UI"));
  m_fontNormal.CreateFontIndirect(&lf);

  lf.lfHeight = -16;
  lf.lfWeight = FW_BOLD;
  m_fontTitle.CreateFontIndirect(&lf);

  lf.lfHeight = -12;
  lf.lfWeight = FW_NORMAL;
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
  CBrush brush(ms_colorPanelBg);
  pDC->FillRect(&rect, &brush);
}

void CAcadUIElement::DrawBorder(CDC* pDC, const CRect& rect)
{
  CPen pen(PS_SOLID, 1, ms_colorBorder);
  CPen* pOldPen = pDC->SelectObject(&pen);
  pDC->SelectStockObject(NULL_BRUSH);
  pDC->Rectangle(&rect);
  pDC->SelectObject(pOldPen);
}
