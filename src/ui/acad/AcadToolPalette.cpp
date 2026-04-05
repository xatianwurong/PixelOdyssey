#include "AcadToolPalette.h"
#include "UICommon.h"
#include "UIToolbar.h"
#include "../core/ColorScheme.h"

IMPLEMENT_DYNAMIC(CAcadToolPalette, CAcadUIElement)

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

  // 添加默认工具 - 改用简单的中文标签
  AddToolButton(ToolType::Select, _T("选择 (V)"), 'S');  // Select
  AddToolButton(ToolType::Move, _T("移动 (G)"), 'M');    // Move
  AddToolButton(ToolType::Rotate, _T("旋转 (R)"), 'R');  // Rotate
  AddToolButton(ToolType::Scale, _T("缩放 (S)"), 'Z');   // Zoom/Scale
  AddToolButton(ToolType::Pan, _T("平移 (H)"), 'P');     // Pan

  return TRUE;
}

void CAcadToolPalette::AddToolButton(ToolType type, const CString& tooltip, int iconType)
{
  ToolButton btn;
  btn.type = type;
  btn.tooltip = tooltip;
  btn.iconType = iconType;
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

  // 优化尺寸：根据工具栏宽度自动调整
  int buttonWidth = rect.Width() - 4;      // 保留 2px 边距
  int buttonHeight = 70;                   // 高度增加，给文字更多空间
  int spacing = 4;                         // 按钮间距

  int x = 2;                               // 左边距
  int y = 6;                               // 顶部边距

  for (size_t i = 0; i < m_buttons.size(); i++) {
    // 计算按钮位置
    CRect btnRect(x, y, x + buttonWidth, y + buttonHeight);
    m_buttons[i].rect = btnRect;

    // 绘制按钮和标签
    auto& colors = ColorScheme::Instance();

    UIToolbar::ButtonState state = UIToolbar::ButtonState::Normal;
    COLORREF bgColor;
    COLORREF borderColor;

    if (m_buttons[i].isActive) {
        state = UIToolbar::ButtonState::Active;
        bgColor = colors.GetColor(ColorScheme::ColorRole::Primary);
        borderColor = colors.GetColor(ColorScheme::ColorRole::Highlight);
    }
    else if (m_buttons[i].isHover) {
        state = UIToolbar::ButtonState::Hovered;
        bgColor = colors.GetColor(ColorScheme::ColorRole::PrimaryHover);
        borderColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
    }
    else {
        bgColor = colors.GetColor(ColorScheme::ColorRole::Surface);
        borderColor = colors.GetColor(ColorScheme::ColorRole::Border);
    }

    // 提取标签（第一个空格前的部分）
    CString label = m_buttons[i].tooltip;
    int spacePos = label.Find(_T(' '));
    if (spacePos > 0) {
        label = label.Left(spacePos);
    }

    // 使用新的带标签按钮绘制
    UIToolbar::DrawButtonWithLabel(pDC, btnRect, state, 
                                   m_buttons[i].iconType, label,
                                   bgColor, borderColor);

    y += buttonHeight + spacing;
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
  auto& colors = ColorScheme::Instance();

  // 确定按钮状态和颜色
  UIToolbar::ButtonState state = UIToolbar::ButtonState::Normal;
  COLORREF bgColor;
  COLORREF borderColor;

  if (btn.isActive) {
    state = UIToolbar::ButtonState::Active;
    bgColor = colors.GetColor(ColorScheme::ColorRole::Primary);
    borderColor = colors.GetColor(ColorScheme::ColorRole::Highlight);
  }
  else if (btn.isHover) {
    state = UIToolbar::ButtonState::Hovered;
    bgColor = colors.GetColor(ColorScheme::ColorRole::PrimaryHover);
    borderColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
  }
  else {
    bgColor = colors.GetColor(ColorScheme::ColorRole::Surface);
    borderColor = colors.GetColor(ColorScheme::ColorRole::Border);
  }

  // 仅用于兼容性，实际在 OnDraw 中使用 DrawButtonWithLabel
  UIToolbar::DrawButton(pDC, rect, state, btn.iconType, bgColor, borderColor);
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

