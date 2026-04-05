#include "UIComponents.h"
#include "../core/ColorScheme.h"

// CUnifiedPanel 消息映射
BEGIN_MESSAGE_MAP(CUnifiedPanel, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

int CUnifiedPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CUnifiedPanel::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    // 绘制背景
    OnDrawPanel(&dc, rect);

    // 绘制项目
    int y = paddingTop - scrollOffset;
    for (size_t i = 0; i < items.size(); i++) {
        int itemHeight = items[i]->GetHeight();

        if (y + itemHeight > 0 && y < rect.Height()) {
            CRect itemRect(paddingLeft, y, rect.right - paddingRight, y + itemHeight);
            bool bHover = (i == (size_t)hoverIndex);
            items[i]->Draw(&dc, itemRect, bHover);
        }

        y += itemHeight + itemSpacing;
    }
}

BOOL CUnifiedPanel::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;  // 在 OnPaint 中统一绘制
}
