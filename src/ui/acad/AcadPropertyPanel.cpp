#include "AcadPropertyPanel.h"
#include "../core/ColorScheme.h"

IMPLEMENT_DYNAMIC(CAcadPropertyPanel, CUnifiedPanel)

CAcadPropertyPanel::CAcadPropertyPanel()
  : m_title(_T("Properties"))
{}

CAcadPropertyPanel::~CAcadPropertyPanel()
{}

BEGIN_MESSAGE_MAP(CAcadPropertyPanel, CUnifiedPanel)
END_MESSAGE_MAP()

BOOL CAcadPropertyPanel::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
  if (!CUnifiedPanel::Create(NULL, _T("Properties"),
    WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
    rect, pParent, nID)) {
    return FALSE;
  }

  // 初始化默认属性
  ClearItems();
  AddItem(new CUILabel(_T("No selection")));

  return TRUE;
}

void CAcadPropertyPanel::SetSelectedEntity(Entity entity)
{
  m_selectedEntity = entity;
  RebuildProperties();
}

void CAcadPropertyPanel::UpdateProperties()
{
  RebuildProperties();
}

void CAcadPropertyPanel::RebuildProperties()
{
  ClearItems();

  // 添加标题间隔
  paddingTop = 40;  // 给标题留空间

  if (!m_selectedEntity.IsValid()) {
    AddItem(new CUILabel(_T("No object selected")));
    return;
  }

  // 对象名称
  CString name = m_selectedEntity.GetName().c_str();
  AddItem(new CUIPropertyItem(_T("Name:"), name));

  // 变换组件
  if (m_selectedEntity.HasComponent<TransformComponent>()) {
    auto& transform = m_selectedEntity.GetComponent<TransformComponent>();

    CString str;
    str.Format(_T("(%.2f, %.2f, %.2f)"),
      transform.position.x, transform.position.y, transform.position.z);
    AddItem(new CUIPropertyItem(_T("Position:"), str));

    str.Format(_T("(%.2f, %.2f, %.2f)"),
      transform.rotation.x, transform.rotation.y, transform.rotation.z);
    AddItem(new CUIPropertyItem(_T("Rotation:"), str));

    str.Format(_T("(%.2f, %.2f, %.2f)"),
      transform.scale.x, transform.scale.y, transform.scale.z);
    AddItem(new CUIPropertyItem(_T("Scale:"), str));
  }

  // 渲染组件
  if (m_selectedEntity.HasComponent<RenderComponent>()) {
    auto& render = m_selectedEntity.GetComponent<RenderComponent>();
    AddItem(new CUIPropertyItem(_T("Visible:"),
      render.isVisible ? _T("Yes") : _T("No")));
  }
}

void CAcadPropertyPanel::OnDraw(CDC* pDC)
{
  CRect rect;
  GetClientRect(&rect);
  auto& colors = ColorScheme::Instance();

  // 绘制标题背景
  CRect titleRect(0, 0, rect.Width(), 35);
  CBrush titleBrush(colors.GetColor(ColorScheme::ColorRole::Surface));
  pDC->FillRect(&titleRect, &titleBrush);

  // 绘制标题
  pDC->SetBkMode(TRANSPARENT);
  pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
  CFont fontTitle;
  fontTitle.CreateFont(-UIFonts::HEADING_SIZE, 0, 0, 0, UIFonts::HEADING_WEIGHT, FALSE, FALSE, 0,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, UIFonts::FONT_FAMILY);
  pDC->SelectObject(&fontTitle);
  pDC->DrawText(m_title, &titleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
  titleRect.left += 10;

  // 分隔线
  CPen borderPen(PS_SOLID, 1, colors.GetColor(ColorScheme::ColorRole::Border));
  pDC->SelectObject(&borderPen);
  pDC->MoveTo(0, 35);
  pDC->LineTo(rect.Width(), 35);
}
