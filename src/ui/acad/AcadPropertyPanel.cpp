#include "AcadPropertyPanel.h"

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

  // 绘制标题背景 - 优化视觉效果
  CRect titleRect(0, 0, rect.Width(), 35);
  CBrush titleBrush(RGB(37, 37, 37));  // 与面板背景一致
  pDC->FillRect(&titleRect, &titleBrush);

  // 绘制标题 - 优化字体和颜色
  pDC->SetBkMode(TRANSPARENT);
  pDC->SetTextColor(RGB(240, 240, 240));  // 优化对比度
  CFont fontTitle;
  fontTitle.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Microsoft YaHei UI"));  // 使用微软雅黑
  pDC->SelectObject(&fontTitle);
  pDC->DrawText(m_title, &titleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
  titleRect.left += 10;

  // 分隔线 - 优化颜色
  CPen borderPen(PS_SOLID, 1, RGB(62, 62, 66));  // 与边框颜色一致
  pDC->SelectObject(&borderPen);
  pDC->MoveTo(0, 35);
  pDC->LineTo(rect.Width(), 35);

  // 调用基类绘制内容
  // CUnifiedPanel::OnDraw(pDC);  // 如果需要自定义绘制内容区域
}
