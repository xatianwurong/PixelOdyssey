#pragma once

#include <afxwin.h>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include "../core/ColorScheme.h"
#include "UILayout.h"

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
        pDC->SelectObject(GetNormalFont());
        pDC->DrawText(text, const_cast<LPRECT>(&rect), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override { return 28; }

    void SetText(const CString& newText) { text = newText; }
    const CString& GetText() const { return text; }

private:
    CString text;

    static CFont* GetNormalFont()
    {
        static CFont font;
        static bool initialized = false;
        if (!initialized)
        {
            LOGFONT lf = {};
            lf.lfHeight = -UILayout::UIFonts::BODY_SIZE;
            lf.lfWeight = UILayout::UIFonts::BODY_WEIGHT;
            lf.lfQuality = CLEARTYPE_QUALITY;
            _tcscpy_s(lf.lfFaceName, UILayout::UIFonts::FONT_FAMILY);
            font.CreateFontIndirect(&lf);
            initialized = true;
        }
        return &font;
    }
};

/**
 * @brief 属性项 - 名称 + 值对
 */
class CUIPropertyItem : public CUIPanelItem
{
public:
    CUIPropertyItem(const CString& propName, const CString& propValue)
        : name(propName), value(propValue)
    {
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
        pDC->SelectObject(GetNormalFont());

        // 属性名（次要文字）
        pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextSecondary));
        CRect labelRect(rect.left + 8, rect.top, rect.left + rect.Width() / 2 - 4, rect.bottom);
        pDC->DrawText(name, const_cast<LPRECT>(&labelRect), DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // 属性值（主要文字）- 右对齐显示
        pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
        CRect valueRect(rect.left + rect.Width() / 2 + 4, rect.top, rect.right - 8, rect.bottom);
        pDC->DrawText(value, const_cast<LPRECT>(&valueRect), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override { return 32; }

    void SetValue(const CString& newValue) { value = newValue; }
    const CString& GetValue() const { return value; }

private:
    CString value;

    static CFont* GetNormalFont()
    {
        static CFont font;
        static bool initialized = false;
        if (!initialized)
        {
            LOGFONT lf = {};
            lf.lfHeight = -UILayout::UIFonts::BODY_SIZE;
            lf.lfWeight = UILayout::UIFonts::BODY_WEIGHT;
            lf.lfQuality = CLEARTYPE_QUALITY;
            _tcscpy_s(lf.lfFaceName, UILayout::UIFonts::FONT_FAMILY);
            font.CreateFontIndirect(&lf);
            initialized = true;
        }
        return &font;
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

    int GetHeight() const override { return 16; }
};

/**
 * @brief 按钮项 - 可点击的按钮
 */
class CUIButton : public CUIPanelItem
{
public:
    using ClickCallback = std::function<void()>;

    CUIButton(const CString& btnName, const CString& btnLabel)
        : name(btnName), label(btnLabel), isPressed(false)
    {
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
        btnRect.DeflateRect(4, 2);
        pDC->RoundRect(&btnRect, CPoint(6, 6));
        pDC->SelectObject(pOldPen);

        // 文本
        pDC->SetBkMode(TRANSPARENT);
        COLORREF textColor = !enabled         ? colors.GetColor(ColorScheme::ColorRole::TextDisabled)
                             : isPressed || bHover ? RGB(255, 255, 255)
                                                   : colors.GetColor(ColorScheme::ColorRole::TextPrimary);
        pDC->SetTextColor(textColor);
        pDC->SelectObject(GetNormalFont());
        pDC->DrawText(label, const_cast<LPRECT>(&rect), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override { return 38; }

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
        static CFont font;
        static bool initialized = false;
        if (!initialized)
        {
            LOGFONT lf = {};
            lf.lfHeight = -UILayout::UIFonts::BODY_SIZE;
            lf.lfWeight = UILayout::UIFonts::BODY_WEIGHT;
            lf.lfQuality = CLEARTYPE_QUALITY;
            _tcscpy_s(lf.lfFaceName, UILayout::UIFonts::FONT_FAMILY);
            font.CreateFontIndirect(&lf);
            initialized = true;
        }
        return &font;
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
    int itemSpacing = 4;
    int paddingLeft = 12;
    int paddingRight = 12;
    int paddingTop = 48;
    int paddingBottom = 12;
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

protected:
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
