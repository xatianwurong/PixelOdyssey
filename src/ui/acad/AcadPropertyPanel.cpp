#include "AcadPropertyPanel.h"
#include "UICommon.h"
#include "UILayout.h"
#include "../core/ColorScheme.h"

IMPLEMENT_DYNAMIC(CAcadPropertyPanel, CUnifiedPanel)

CAcadPropertyPanel::CAcadPropertyPanel()
    : m_title(_T("Properties"))
{
}

CAcadPropertyPanel::~CAcadPropertyPanel()
{
}

BEGIN_MESSAGE_MAP(CAcadPropertyPanel, CUnifiedPanel)
END_MESSAGE_MAP()

BOOL CAcadPropertyPanel::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
    if (!CUnifiedPanel::Create(NULL, _T("Properties"),
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        rect, pParent, nID))
    {
        return FALSE;
    }

    // 初始化默认属性
    ClearItems();
    AddItem(std::make_unique<CUILabel>(_T("No selection")));

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

    if (!m_selectedEntity.IsValid())
    {
        AddItem(std::make_unique<CUILabel>(_T("No object selected")));
        return;
    }

    // 对象名称
    CString name = m_selectedEntity.GetName().c_str();
    AddItem(std::make_unique<CUIPropertyItem>(_T("Name:"), name));

    // 变换组件
    if (m_selectedEntity.HasComponent<TransformComponent>())
    {
        auto& transform = m_selectedEntity.GetComponent<TransformComponent>();

        CString str;
        str.Format(_T("(%.2f, %.2f, %.2f)"),
            transform.position.x, transform.position.y, transform.position.z);
        AddItem(std::make_unique<CUIPropertyItem>(_T("Position:"), str));

        str.Format(_T("(%.2f, %.2f, %.2f)"),
            transform.rotation.x, transform.rotation.y, transform.rotation.z);
        AddItem(std::make_unique<CUIPropertyItem>(_T("Rotation:"), str));

        str.Format(_T("(%.2f, %.2f, %.2f)"),
            transform.scale.x, transform.scale.y, transform.scale.z);
        AddItem(std::make_unique<CUIPropertyItem>(_T("Scale:"), str));
    }

    // 渲染组件
    if (m_selectedEntity.HasComponent<RenderComponent>())
    {
        auto& render = m_selectedEntity.GetComponent<RenderComponent>();
        AddItem(std::make_unique<CUIPropertyItem>(_T("Visible:"),
            render.isVisible ? _T("Yes") : _T("No")));
    }
}

void CAcadPropertyPanel::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    auto& colors = ColorScheme::Instance();

    // 绘制背景
    CBrush bgBrush(colors.GetColor(ColorScheme::ColorRole::Background));
    pDC->FillRect(&rect, &bgBrush);

    // 绘制标题栏 - 上部 48px
    CRect titleRect(0, 0, rect.Width(), 48);
    CBrush titleBrush(colors.GetColor(ColorScheme::ColorRole::Surface));
    pDC->FillRect(&titleRect, &titleBrush);

    // 标题文本 - 使用更大的标题字体
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
    pDC->SelectObject(UICommon::GetTitleFont());

    CRect titleTextRect = titleRect;
    titleTextRect.DeflateRect(16, 0, 16, 0);
    pDC->DrawText(m_title, reinterpret_cast<LPRECT>(&titleTextRect), DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // 绘制分隔线
    CPen pen(PS_SOLID, 1, colors.GetColor(ColorScheme::ColorRole::Border));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(0, 48);
    pDC->LineTo(rect.Width(), 48);
    pDC->SelectObject(pOldPen);

    // 绘制内容区域的项目，使用改进的间距
    CRect contentRect = rect;
    contentRect.top = 48;

    int y = contentRect.top + paddingTop - scrollOffset;

    for (size_t i = 0; i < items.size(); i++) {
        int itemHeight = items[i]->GetHeight();

        // 检查是否在可见范围内
        if (y + itemHeight > contentRect.top && y < contentRect.bottom - paddingBottom) {
            CRect itemRect(contentRect.left + paddingLeft, y,
                          contentRect.right - paddingRight, y + itemHeight);

            bool bHover = (i == (size_t)hoverIndex);
            items[i]->Draw(pDC, itemRect, bHover);
        }

        y += itemHeight + itemSpacing;
    }
}
