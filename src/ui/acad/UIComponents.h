#pragma once

#include <afxwin.h>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include "../core/ColorScheme.h"
#include "UILayout.h"
#include "UICommon.h"

/**
 * @brief UI 面板项基类
 * @details 所有 UI 控件都继承自此类，实现统一的布局和绘制
 * 
 * 设计原则:
 * - 使用智能指针管理资源
 * - 支持 hover 状态反馈
 * - 统一的高度计算
 * - 点击事件回调
 */
class CUIPanelItem
{
public:
    virtual ~CUIPanelItem() = default;

    /**
     * @brief 绘制项
     * @param pDC 设备上下文
     * @param rect 绘制区域
     * @param bHover 是否悬停
     */
    virtual void Draw(CDC* pDC, const CRect& rect, bool bHover) = 0;

    /**
     * @brief 获取项的高度
     */
    virtual int GetHeight() const = 0;

    /**
     * @brief 处理点击事件
     * @param point 点击位置
     * @return 是否处理了点击
     */
    virtual bool OnClick(const CPoint& point) { return false; }

    CString name;           ///< 项名称 (用于标识)
    bool enabled = true;    ///< 是否启用
};

/**
 * @brief 标签项 - 显示文本标签
 */
class CUILabel : public CUIPanelItem
{
public:
    explicit CUILabel(const CString& text) : text(text)
    {
        name.Empty();
    }

    void Draw(CDC* pDC, const CRect& rect, bool /*bHover*/) override
    {
        auto& colors = ColorScheme::Instance();
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
        pDC->SelectObject(UICommon::GetTitleFont());  // 使用标题字体使标签更显眼
        CRect trect = rect;
        trect.DeflateRect(UISpacing::M, UISpacing::S);
        pDC->DrawText(text, reinterpret_cast<LPRECT>(&trect), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override { return 36; }  // 增加到 36px

    void SetText(const CString& newText) { text = newText; }
    const CString& GetText() const { return text; }

private:
    CString text;

    // fonts are provided by UICommon
};

/**
 * @brief 属性项 - 名称 + 值对
 */
class CUIPropertyItem : public CUIPanelItem
{
public:
    CUIPropertyItem(const CString& propName, const CString& propValue)
    {
        name = propName;
        value = propValue;
    }

    void Draw(CDC* pDC, const CRect& rect, bool bHover) override
    {
        auto& colors = ColorScheme::Instance();

        // 悬停背景高亮
        if (bHover && enabled)
        {
            CBrush brush(colors.GetColor(ColorScheme::ColorRole::SurfaceHover));
            pDC->FillRect(&rect, &brush);
        }

        pDC->SetBkMode(TRANSPARENT);
        pDC->SelectObject(UICommon::GetBodyFont());

        // 属性名（次要文字） - 左侧 40%
        pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextSecondary));
        CRect labelRect = rect;
        labelRect.DeflateRect(UISpacing::M, UISpacing::S);  // 使用更大的间距
        labelRect.right = rect.left + (rect.Width() * 40) / 100;
        pDC->DrawText(name, reinterpret_cast<LPRECT>(&labelRect), DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // 属性值（主要文字） - 右侧 60% - 右对齐显示
        pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
        CRect valueRect = rect;
        valueRect.DeflateRect(UISpacing::M, UISpacing::S);
        valueRect.left = rect.left + (rect.Width() * 40) / 100;
        pDC->DrawText(value, reinterpret_cast<LPRECT>(&valueRect), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override { return 40; }  // 增加到 40px，给更多空间

    void SetValue(const CString& newValue) { value = newValue; }
    const CString& GetValue() const { return value; }

private:
    CString value;

    static CFont* GetNormalFont()
    {
        return UICommon::GetBodyFont();
    }
};

/**
 * @brief 分隔线项
 */
class CUISeparator : public CUIPanelItem
{
public:
    CUISeparator() { name.Empty(); }

    void Draw(CDC* pDC, const CRect& rect, bool /*bHover*/) override
    {
        auto& colors = ColorScheme::Instance();
        CPen pen(PS_SOLID, 1, colors.GetColor(ColorScheme::ColorRole::Border));
        CPen* pOldPen = pDC->SelectObject(&pen);
        int midY = rect.top + rect.Height() / 2;
        pDC->MoveTo(rect.left + 8, midY);
        pDC->LineTo(rect.right - 8, midY);
        pDC->SelectObject(pOldPen);
    }

    int GetHeight() const override { return 24; }  // 增加到 24px
};

/**
 * @brief 按钮项 - 可点击的按钮
 */
class CUIButton : public CUIPanelItem
{
public:
    using ClickCallback = std::function<void()>;

    CUIButton(const CString& btnName, const CString& btnLabel)
        : label(btnLabel), isPressed(false)
    {
        name = btnName;
        enabled = true;
    }

    void SetOnClick(ClickCallback callback) { onClick = callback; }

    void Draw(CDC* pDC, const CRect& rect, bool bHover) override
    {
        auto& colors = ColorScheme::Instance();

        COLORREF bgColor;
        if (!enabled)
            bgColor = colors.GetColor(ColorScheme::ColorRole::Surface);
        else if (isPressed)
            bgColor = colors.GetColor(ColorScheme::ColorRole::Primary);
        else if (bHover)
            bgColor = colors.GetColor(ColorScheme::ColorRole::PrimaryHover);
        else
            bgColor = colors.GetColor(ColorScheme::ColorRole::Surface);

        // 背景
        CBrush brush(bgColor);
        pDC->FillRect(&rect, &brush);

        // 边框
        COLORREF borderColor = !enabled ? colors.GetColor(ColorScheme::ColorRole::Border)
                               : bHover  ? colors.GetColor(ColorScheme::ColorRole::Primary)
                                         : colors.GetColor(ColorScheme::ColorRole::Border);
        CPen pen(PS_SOLID, 1, borderColor);
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->SelectStockObject(NULL_BRUSH);

        // 绘制圆角矩形
        CRect btnRect = rect;
        btnRect.DeflateRect(UISpacing::S, 2);
        pDC->RoundRect(&btnRect, CPoint(UISizes::CORNER_RADIUS_MEDIUM, UISizes::CORNER_RADIUS_MEDIUM));
        pDC->SelectObject(pOldPen);

        // 文本
        pDC->SetBkMode(TRANSPARENT);
        COLORREF textColor = !enabled         ? colors.GetColor(ColorScheme::ColorRole::TextDisabled)
                             : isPressed || bHover ? RGB(255, 255, 255)
                                                   : colors.GetColor(ColorScheme::ColorRole::TextPrimary);
        pDC->SetTextColor(textColor);
        pDC->SelectObject(UICommon::GetBodyFont());
        CRect textRect = rect;
        textRect.DeflateRect(UISpacing::BUTTON_PADDING_X / 2, 0);
        pDC->DrawText(label, reinterpret_cast<LPRECT>(&textRect), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override { return 44; }  // 增加到 44px，给更多空间

    bool OnClick(const CPoint& /*point*/) override
    {
        if (enabled && onClick)
        {
            isPressed = !isPressed;
            onClick();
            return true;
        }
        return false;
    }

    void SetPressed(bool pressed) { isPressed = pressed; }
    bool IsPressed() const { return isPressed; }

private:
    CString label;
    bool isPressed;
    ClickCallback onClick;

    static CFont* GetNormalFont()
    {
        return UICommon::GetBodyFont();
    }
};

/**
 * @brief 统一的面板容器
 * @details 自动管理所有 UI 项的布局和滚动
 * 
 * 功能特性:
 * - 自动滚动支持
 * - 鼠标滚轮导航
 * - Hover 状态检测
 * - 点击事件分发
 */
class CUnifiedPanel : public CWnd
{
protected:
    std::vector<std::unique_ptr<CUIPanelItem>> items;  ///< 使用智能指针管理
    int itemSpacing = UISpacing::M;           ///< 项间距 - 增加到 16px
    int paddingLeft = UISpacing::M;           ///< 左内边距
    int paddingRight = UISpacing::M;          ///< 右内边距
    int paddingTop = 48 + UISpacing::M;       ///< 顶部内边距（给标题留空）
    int paddingBottom = UISpacing::L;         ///< 底部内边距 - 增加到 24px 防止重叠
    int scrollOffset = 0;
    int totalHeight = 0;
    int hoverIndex = -1;

public:
    CUnifiedPanel() = default;
    ~CUnifiedPanel() override = default;

    /**
     * @brief 添加项 (转移所有权)
     */
    void AddItem(std::unique_ptr<CUIPanelItem> item)
    {
        items.push_back(std::move(item));
        RecalculateLayout();
        Invalidate();
    }

    /**
     * @brief 添加项 (便捷重载)
     */
    void AddItem(CUIPanelItem* item)
    {
        items.emplace_back(item);
        RecalculateLayout();
        Invalidate();
    }

    /**
     * @brief 清空所有项
     */
    void ClearItems()
    {
        items.clear();
        scrollOffset = 0;
        totalHeight = 0;
        hoverIndex = -1;
        Invalidate();
    }

    /**
     * @brief 重新计算布局
     */
    void RecalculateLayout()
    {
        totalHeight = paddingTop + paddingBottom;
        for (const auto& item : items)
        {
            totalHeight += item->GetHeight() + itemSpacing;
        }
    }

    int GetContentHeight() const { return totalHeight; }
    int GetScrollOffset() const { return scrollOffset; }

    /**
     * @brief 设置滚动偏移
     */
    void SetScrollOffset(int offset)
    {
        CRect rect;
        GetClientRect(&rect);
        int maxScroll = std::max<int>(0, totalHeight - rect.Height());
        scrollOffset = std::max<int>(0, std::min<int>(offset, maxScroll));
        Invalidate();
    }

    /**
     * @brief 获取项数量
     */
    size_t GetItemCount() const { return items.size(); }

    /**
     * @brief 获取指定项 (不转移所有权)
     */
    CUIPanelItem* GetItem(size_t index)
    {
        if (index < items.size())
            return items[index].get();
        return nullptr;
    }

    /**
     * @brief 绘制面板（子类可重写以实现自定义背景）
     */
    virtual void OnDrawPanel(CDC* pDC, const CRect& rect)
    {
        auto& colors = ColorScheme::Instance();
        CBrush bgBrush(colors.GetColor(ColorScheme::ColorRole::Background));
        pDC->FillRect(&rect, &bgBrush);
    }

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
    {
        SetScrollOffset(scrollOffset - zDelta);
        return CWnd::OnMouseWheel(nFlags, zDelta, pt);
    }

    afx_msg void OnMouseMove(UINT nFlags, CPoint point)
    {
        CRect rect;
        GetClientRect(&rect);
        int y = paddingTop - scrollOffset;

        int newHover = -1;
        for (size_t i = 0; i < items.size(); i++)
        {
            CRect itemRect(paddingLeft, y, rect.right - paddingRight,
                           y + items[i]->GetHeight());
            if (itemRect.PtInRect(point))
            {
                newHover = static_cast<int>(i);
                break;
            }
            y += items[i]->GetHeight() + itemSpacing;
        }

        if (newHover != hoverIndex)
        {
            hoverIndex = newHover;
            Invalidate();
        }

        CWnd::OnMouseMove(nFlags, point);
    }

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
    {
        CRect rect;
        GetClientRect(&rect);
        int y = paddingTop - scrollOffset;

        for (size_t i = 0; i < items.size(); i++)
        {
            CRect itemRect(paddingLeft, y, rect.right - paddingRight,
                           y + items[i]->GetHeight());
            if (itemRect.PtInRect(point))
            {
                if (items[i]->OnClick(point))
                {
                    Invalidate();
                }
                break;
            }
            y += items[i]->GetHeight() + itemSpacing;
        }

        CWnd::OnLButtonDown(nFlags, point);
    }

    DECLARE_MESSAGE_MAP()
};
