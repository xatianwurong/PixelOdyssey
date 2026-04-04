#include "AcadToolPalette.h"

IMPLEMENT_DYNAMIC(CAcadToolPalette, CWnd)

CAcadToolPalette::CAcadToolPalette()
  : m_activeTool(ToolType::Select)
  , m_hoverIndex(-1)
  , m_bTrackingMouse(false)
  , m_buttonWidth(40)
  , m_buttonHeight(40)
  , m_buttonSpacing(5)
{}

CAcadToolPalette::~CAcadToolPalette()
{}

BEGIN_MESSAGE_MAP(CAcadToolPalette, CAcadUIElement)
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

BOOL CAcadToolPalette::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
  if (!CAcadUIElement::Create(pParent, nID, rect)) {
    return FALSE;
  }

  // 添加默认工具
  AddToolButton(ToolType::Select, _T("Select Tool (V)"), 0x25B6);  // ▶
  AddToolButton(ToolType::Move, _T("Move Tool (G)"), 0x271A);      // ➤
  AddToolButton(ToolType::Rotate, _T("Rotate Tool (R)"), 0x27F2);  // ⟲
  AddToolButton(ToolType::Scale, _T("Scale Tool (S)"), 0x2194);    // ↔
  AddToolButton(ToolType::Pan, _T("Pan Tool (H)"), 0x270B);        // ✋

  return TRUE;
}

void CAcadToolPalette::AddToolButton(ToolType type, const CString& tooltip, int iconChar)
{
  ToolButton btn;
  btn.type = type;
  btn.tooltip = tooltip;
  btn.iconChar = iconChar;
  btn.isActive = (type == m_activeTool);
  btn.isHover = false;
  m_buttons.push_back(btn);
  Invalidate();
}

void CAcadToolPalette::SetActiveTool(ToolType type)
{
  m_activeTool = type;

  for (auto& btn : m_buttons) {
    btn.isActive = (btn.type == type);
  }

  Invalidate();
}

void CAcadToolPalette::OnDraw(CDC* pDC)
{
  CRect rect;
  GetClientRect(&rect);

  int x = 10;
  int y = 10;

  for (size_t i = 0; i < m_buttons.size(); i++) {
    CRect btnRect(x, y, x + m_buttonWidth, y + m_buttonHeight);
    m_buttons[i].rect = btnRect;

    DrawToolButton(pDC, (int)i, btnRect);

    y += m_buttonHeight + m_buttonSpacing;
  }
}

void CAcadToolPalette::OnMouseMove(UINT nFlags, CPoint point)
{
  int newHover = GetButtonAtPoint(point);

  if (newHover != m_hoverIndex) {
    m_hoverIndex = newHover;

    for (size_t i = 0; i < m_buttons.size(); i++) {
      m_buttons[i].isHover = (i == (size_t)newHover);
    }

    Invalidate();
  }

  if (!m_bTrackingMouse) {
    TRACKMOUSEEVENT tme = { 0 };
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = m_hWnd;
    TrackMouseEvent(&tme);
    m_bTrackingMouse = true;
  }
}

void CAcadToolPalette::OnLButtonDown(UINT nFlags, CPoint point)
{
  int index = GetButtonAtPoint(point);

  if (index >= 0 && index < (int)m_buttons.size()) {
    SetActiveTool(m_buttons[index].type);
  }
}

void CAcadToolPalette::OnLButtonUp(UINT nFlags, CPoint point)
{}

void CAcadToolPalette::OnMouseLeave()
{
  m_bTrackingMouse = false;
  m_hoverIndex = -1;

  for (auto& btn : m_buttons) {
    btn.isHover = false;
  }

  Invalidate();
}

void CAcadToolPalette::DrawToolButton(CDC* pDC, int index, const CRect& rect)
{
  if (index < 0 || index >= (int)m_buttons.size()) return;

  const ToolButton& btn = m_buttons[index];

  // 背景色
  COLORREF bgColor;
  if (btn.isActive) {
    bgColor = ms_colorAccent;  // 激活蓝色
  }
  else if (btn.isHover) {
    bgColor = ms_colorAccentHover;  // 悬停深蓝色
  }
  else {
    bgColor = RGB(50, 50, 50);  // 优化默认深色
  }

  // 圆角矩形背景 - 优化边框
  CBrush brush(bgColor);
  CPen pen(PS_SOLID, 1, btn.isHover ? RGB(240, 240, 240) : ms_colorBorder);  // 优化边框颜色
  CBrush* pOldBrush = pDC->SelectObject(&brush);
  CPen* pOldPen = pDC->SelectObject(&pen);

  // 绘制矩形
  pDC->Rectangle(&rect);

  pDC->SelectObject(pOldBrush);
  pDC->SelectObject(pOldPen);

  // 图标 - 优化字体和颜色
  pDC->SetBkMode(TRANSPARENT);
  pDC->SetTextColor(btn.isActive ? RGB(255, 255, 255) : RGB(240, 240, 240));  // 优化文本颜色
  pDC->SelectObject(&m_fontNormal);

  CString iconText;
  iconText.Format(_T("%c"), btn.iconChar);

  CRect textRect = rect;
  textRect.DeflateRect(0, 5, 0, 0);
  pDC->DrawText(iconText, &textRect, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);

  // 边框高亮
  if (btn.isActive) {
    CPen highlightPen(PS_SOLID, 2, ms_colorHighlight);
    pDC->SelectObject(&highlightPen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);
  }
}

int CAcadToolPalette::GetButtonAtPoint(CPoint point) const
{
  for (size_t i = 0; i < m_buttons.size(); i++) {
    if (m_buttons[i].rect.PtInRect(point)) {
      return (int)i;
    }
  }
  return -1;
}

