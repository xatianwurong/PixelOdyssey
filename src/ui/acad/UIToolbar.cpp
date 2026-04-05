#include "UIToolbar.h"
#include "UICommon.h"
#include "../core/ColorScheme.h"

namespace UIToolbar {

void DrawSimpleIcon(CDC* pDC, const CRect& rect, int iconType, COLORREF color)
{
    // 使用基础几何形状绘制 5 种工具图标
    // 避免使用 Unicode 符号，确保所有字体都支持

    int cx = rect.left + rect.Width() / 2;
    int cy = rect.top + rect.Height() / 2;
    int size = 12;  // 图标基础大小

    CPen pen(PS_SOLID, 2, color);
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush brush(color);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    switch (iconType) {
        case 0: // 选择工具 - 箭头向上
        {
            pDC->MoveTo(cx, cy - size);
            pDC->LineTo(cx - 6, cy);
            pDC->LineTo(cx - 2, cy);
            pDC->LineTo(cx - 2, cy + size);
            pDC->LineTo(cx + 2, cy + size);
            pDC->LineTo(cx + 2, cy);
            pDC->LineTo(cx + 6, cy);
            pDC->LineTo(cx, cy - size);
        }
        break;

        case 1: // 移动工具 - 十字箭头
        {
            pDC->MoveTo(cx - size, cy);
            pDC->LineTo(cx - 4, cy);
            pDC->MoveTo(cx, cy - size);
            pDC->LineTo(cx, cy - 4);
            pDC->MoveTo(cx + size, cy);
            pDC->LineTo(cx + 4, cy);
            pDC->MoveTo(cx, cy + size);
            pDC->LineTo(cx, cy + 4);

            // 绘制四个小三角
            pDC->SelectObject(&brush);
            CPoint pts[] = { CPoint(cx - size, cy), CPoint(cx - size + 3, cy - 3), CPoint(cx - size + 3, cy + 3) };
            pDC->Polygon(pts, 3);
        }
        break;

        case 2: // 旋转工具 - 圆形箭头
        {
            pDC->SelectStockObject(NULL_BRUSH);
            pDC->Ellipse(cx - size, cy - size, cx + size, cy + size);
            // 上方三角表示旋转方向
            CPoint pts[] = { CPoint(cx, cy - size - 3), CPoint(cx - 3, cy - size + 2), CPoint(cx + 3, cy - size + 2) };
            pDC->SelectObject(&brush);
            pDC->Polygon(pts, 3);
        }
        break;

        case 3: // 缩放工具 - 双向箭头
        {
            pDC->MoveTo(cx - size, cy - size);
            pDC->LineTo(cx + size, cy + size);
            pDC->MoveTo(cx + size, cy - size);
            pDC->LineTo(cx - size, cy + size);

            // 左上角箭头
            pDC->MoveTo(cx - size - 2, cy - size - 2);
            pDC->LineTo(cx - size + 4, cy - size - 2);
            pDC->LineTo(cx - size - 2, cy - size + 4);

            // 右下角箭头
            pDC->MoveTo(cx + size + 2, cy + size + 2);
            pDC->LineTo(cx + size - 4, cy + size + 2);
            pDC->LineTo(cx + size + 2, cy + size - 4);
        }
        break;

        case 4: // 平移工具 - 手掌
        {
            pDC->SelectStockObject(NULL_BRUSH);
            // 简单的手掌图形 - 矩形加四个突起
            pDC->Rectangle(cx - 6, cy - 4, cx + 6, cy + 6);
            for (int i = -6; i <= 6; i += 6) {
                pDC->MoveTo(cx + i, cy - 4);
                pDC->LineTo(cx + i, cy - 10);
            }
        }
        break;
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

void DrawButton(CDC* pDC, const CRect& rect, ButtonState state, 
                int iconType, COLORREF bgColor, COLORREF borderColor)
{
    auto& colors = ColorScheme::Instance();

    // 绘制背景
    CBrush brush(bgColor);
    pDC->FillRect(&rect, &brush);

    // 绘制边框 - 更细的线条
    CPen pen(PS_SOLID, 1, borderColor);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);
    pDC->SelectObject(pOldPen);

    // 绘制图标 - 上半部分
    COLORREF iconColor;
    if (state == ButtonState::Disabled) {
        iconColor = colors.GetColor(ColorScheme::ColorRole::TextDisabled);
    } else if (state == ButtonState::Active) {
        iconColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
    } else {
        iconColor = colors.GetColor(ColorScheme::ColorRole::TextSecondary);
    }

    // 图标区域 - 上 60%
    CRect iconRect = rect;
    int splitY = rect.top + rect.Height() * 6 / 10;
    iconRect.bottom = splitY;
    iconRect.DeflateRect(3, 3, 3, 0);
    DrawSimpleIcon(pDC, iconRect, iconType, iconColor);

    // 激活状态边框高亮
    if (state == ButtonState::Active) {
        CPen highlightPen(PS_SOLID, 2, colors.GetColor(ColorScheme::ColorRole::Highlight));
        pDC->SelectObject(&highlightPen);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(&rect);
    }
}

void DrawButtonWithLabel(CDC* pDC, const CRect& rect, ButtonState state, 
                         int iconType, const CString& label,
                         COLORREF bgColor, COLORREF borderColor)
{
    auto& colors = ColorScheme::Instance();

    // 背景
    CBrush brush(bgColor);
    pDC->FillRect(&rect, &brush);

    // 边框
    CPen pen(PS_SOLID, 1, borderColor);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);
    pDC->SelectObject(pOldPen);

    // 图标区域 - 上部 65%
    COLORREF iconColor;
    if (state == ButtonState::Disabled) {
        iconColor = colors.GetColor(ColorScheme::ColorRole::TextDisabled);
    } else if (state == ButtonState::Active) {
        iconColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
    } else {
        iconColor = colors.GetColor(ColorScheme::ColorRole::TextSecondary);
    }

    int splitY = rect.top + rect.Height() * 65 / 100;
    CRect iconRect = rect;
    iconRect.bottom = splitY;
    iconRect.DeflateRect(4, 4, 4, 0);
    DrawSimpleIcon(pDC, iconRect, iconType, iconColor);

    // 文字区域 - 下部 35%
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(state == ButtonState::Active 
        ? colors.GetColor(ColorScheme::ColorRole::TextPrimary)
        : colors.GetColor(ColorScheme::ColorRole::TextSecondary));
    pDC->SelectObject(UICommon::GetChineeseFont(10));  // 增加到 10pt

    CRect labelRect(rect.left + 1, splitY + 2, rect.right - 1, rect.bottom - 2);
    pDC->DrawText(label, labelRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);

    // 激活状态高亮
    if (state == ButtonState::Active) {
        CPen highlightPen(PS_SOLID, 2, colors.GetColor(ColorScheme::ColorRole::Highlight));
        pDC->SelectObject(&highlightPen);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(&rect);
    }
}

void DrawTooltip(CDC* pDC, const CRect& tooltipRect, const CString& text,
                 COLORREF bgColor, COLORREF textColor)
{
    // 背景
    CBrush brush(bgColor);
    pDC->FillRect(&tooltipRect, &brush);
    
    // 边框
    CPen pen(PS_SOLID, 1, textColor);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&tooltipRect);
    pDC->SelectObject(pOldPen);
    
    // 文字
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(textColor);
    pDC->SelectObject(UICommon::GetBodyFont());
    
    CRect textRect = tooltipRect;
    textRect.DeflateRect(6, 3);
    pDC->DrawText(text, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

} // namespace UIToolbar
