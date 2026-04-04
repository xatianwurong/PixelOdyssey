#include "AcadUIElement.h"

IMPLEMENT_DYNAMIC(CAcadUIElement, CWnd)

// 静态颜色定义 - 优化的 AutoCAD 风格深色主题
COLORREF CAcadUIElement::ms_colorBackground = RGB(30, 30, 30);       // #1E1E1E
COLORREF CAcadUIElement::ms_colorPanelBg = RGB(37, 37, 37);          // #252525
COLORREF CAcadUIElement::ms_colorAccent = RGB(0, 120, 212);          // #0078D4
COLORREF CAcadUIElement::ms_colorAccentHover = RGB(30, 144, 255);    // #1E90FF
COLORREF CAcadUIElement::ms_colorText = RGB(240, 240, 240);          // #F0F0F0
COLORREF CAcadUIElement::ms_colorTextMuted = RGB(128, 128, 128);     // #808080
COLORREF CAcadUIElement::ms_colorBorder = RGB(62, 62, 66);           // #3E3E42
COLORREF CAcadUIElement::ms_colorHighlight = RGB(255, 165, 0);       // #FFA500
COLORREF CAcadUIElement::ms_colorSuccess = RGB(0, 200, 83);          // #00C853
COLORREF CAcadUIElement::ms_colorWarning = RGB(255, 179, 0);         // #FFB300
COLORREF CAcadUIElement::ms_colorError = RGB(244, 67, 54);           // #F44336

CAcadUIElement::CAcadUIElement()
  : m_bVisible(true)
{
  // 创建字体 - 优化中文显示
  LOGFONT lf = { 0 };
  lf.lfHeight = -14;
  lf.lfWeight = FW_NORMAL;
  lf.lfQuality = CLEARTYPE_QUALITY;
  lf.lfCharSet = DEFAULT_CHARSET;
  _tcscpy_s(lf.lfFaceName, _T("Microsoft YaHei UI"));  // 使用微软雅黑，更好支持中文
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
