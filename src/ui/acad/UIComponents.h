#pragma once

#include <afxwin.h>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "../core/ColorScheme.h"

/**
 * @brief UI 面板项基类
 * @details 所有 UI 控件都继承自此类，实现统一的布局和绘制
 */
class CUIPanelItem {
public:
  virtual ~CUIPanelItem() = default;

  /**
   * @brief 绘制项
   */
  virtual void Draw(CDC* pDC, const CRect& rect, bool bHover) = 0;

  /**
   * @brief 获取项的高度
   */
  virtual int GetHeight() const = 0;

  /**
   * @brief 处理点击
   */
  virtual bool OnClick(const CPoint& point) { return false; }

  CString name;
  bool enabled;
};

/**
 * @brief 标签项 - 显示文本标签
 */
class CUILabel : public CUIPanelItem {
public:
  CUILabel(const CString& text) {
    name = _T("");
    this->text = text;
  }

  void Draw(CDC* pDC, const CRect& rect, bool bHover) override {
    auto& colors = ColorScheme::Instance();
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
    pDC->DrawText(text, (LPRECT)&rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
  }

  int GetHeight() const override { return 28; }

private:
  CString text;
};

/**
 * @brief 属性项 - 名称 + 值对
 */
class CUIPropertyItem : public CUIPanelItem {
public:
  CUIPropertyItem(const CString& propName, const CString& propValue) {
    name = propName;
    value = propValue;
  }

  void Draw(CDC* pDC, const CRect& rect, bool bHover) override {
    auto& colors = ColorScheme::Instance();
    
    if (bHover && enabled) {
      CBrush* brush = colors.CreateBrush(ColorScheme::ColorRole::SurfaceHover);
      pDC->FillRect(&rect, brush);
      delete brush;
    }

    pDC->SetBkMode(TRANSPARENT);

    // 属性名（次要文字）- 使用更清晰的布局
    pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextSecondary));
    CRect labelRect(rect.left + 8, rect.top, rect.left + rect.Width() / 2 - 4, rect.bottom);
    pDC->DrawText(name, (LPRECT)&labelRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // 属性值（主要文字）- 右对齐显示
    pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
    CRect valueRect(rect.left + rect.Width() / 2 + 4, rect.top, rect.right - 8, rect.bottom);
    pDC->DrawText(value, (LPRECT)&valueRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
  }

  int GetHeight() const override { return 32; }

  void SetValue(const CString& newValue) {
    value = newValue;
  }

private:
  CString value;
};

/**
 * @brief 分隔线项
 */
class CUISeparator : public CUIPanelItem {
public:
  CUISeparator() {
    name = _T("");
  }

  void Draw(CDC* pDC, const CRect& rect, bool bHover) override {
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
class CUIButton : public CUIPanelItem {
public:
  using ClickCallback = std::function<void()>;

  CUIButton(const CString& btnName, const CString& btnLabel) {
    name = btnName;
    label = btnLabel;
    isPressed = false;
    enabled = true;
  }

  void SetOnClick(ClickCallback callback) {
    onClick = callback;
  }

  void Draw(CDC* pDC, const CRect& rect, bool bHover) override {
    auto& colors = ColorScheme::Instance();
    
    COLORREF bgColor;
    if (!enabled) {
      bgColor = colors.GetColor(ColorScheme::ColorRole::Surface);
    } else if (isPressed) {
      bgColor = colors.GetColor(ColorScheme::ColorRole::Primary);
    } else if (bHover) {
      bgColor = colors.GetColor(ColorScheme::ColorRole::PrimaryHover);
    } else {
      bgColor = colors.GetColor(ColorScheme::ColorRole::Surface);
    }

    // 圆角矩形背景
    CBrush* brush = new CBrush(bgColor);
    pDC->FillRect(&rect, brush);
    delete brush;

    // 边框
    COLORREF borderColor = !enabled ? colors.GetColor(ColorScheme::ColorRole::Border) :
                           bHover ? colors.GetColor(ColorScheme::ColorRole::Primary) :
                                    colors.GetColor(ColorScheme::ColorRole::Border);
    CPen* pen = new CPen(PS_SOLID, 1, borderColor);
    CPen* pOldPen = pDC->SelectObject(pen);
    pDC->SelectStockObject(NULL_BRUSH);
    
    // 绘制圆角矩形
    CRect btnRect = rect;
    btnRect.DeflateRect(4, 2);
    pDC->RoundRect(&btnRect, CPoint(6, 6));

    pDC->SelectObject(pOldPen);
    delete pen;

    // 文本
    pDC->SetBkMode(TRANSPARENT);
    COLORREF textColor = !enabled ? colors.GetColor(ColorScheme::ColorRole::TextDisabled) :
                         isPressed || bHover ? RGB(255, 255, 255) :
                                               colors.GetColor(ColorScheme::ColorRole::TextPrimary);
    pDC->SetTextColor(textColor);
    pDC->DrawText(label, (LPRECT)&rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  }

  int GetHeight() const override { return 38; }

  bool OnClick(const CPoint& point) override {
    if (enabled && onClick) {
      isPressed = !isPressed;
      onClick();
      return true;
    }
    return false;
  }

private:
  CString label;
  bool isPressed;
  ClickCallback onClick;
};

/**
 * @brief 统一的面板容器
 * @details 自动管理所有 UI 项的布局和滚动
 */
class CUnifiedPanel : public CWnd {
protected:
  std::vector<CUIPanelItem*> items;
  int itemSpacing;
  int paddingLeft;
  int paddingRight;
  int paddingTop;
  int paddingBottom;
  int scrollOffset;
  int totalHeight;
  int hoverIndex;

public:
  CUnifiedPanel()
    : itemSpacing(4)
    , paddingLeft(12)
    , paddingRight(12)
    , paddingTop(48)
    , paddingBottom(12)
    , scrollOffset(0)
    , totalHeight(0)
    , hoverIndex(-1)
  {}

  ~CUnifiedPanel() {
    for (auto item : items) {
      delete item;
    }
  }

  void AddItem(CUIPanelItem* item) {
    items.push_back(item);
    RecalculateLayout();
    Invalidate();
  }

  void ClearItems() {
    for (auto item : items) {
      delete item;
    }
    items.clear();
    scrollOffset = 0;
    totalHeight = 0;
    Invalidate();
  }

  void RecalculateLayout() {
    totalHeight = paddingTop + paddingBottom;
    for (const auto& item : items) {
      totalHeight += item->GetHeight() + itemSpacing;
    }
  }

  int GetContentHeight() const {
    return totalHeight;
  }

  int GetScrollOffset() const {
    return scrollOffset;
  }

  void SetScrollOffset(int offset) {
    CRect rect;
    GetClientRect(&rect);
    int maxScroll = std::max<int>(0, totalHeight - (int)rect.Height());
    scrollOffset = std::max<int>(0, std::min<int>(offset, maxScroll));
    Invalidate();
  }

protected:
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    SetScrollOffset(scrollOffset - zDelta);
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
  }

  afx_msg void OnMouseMove(UINT nFlags, CPoint point) {
    CRect rect;
    GetClientRect(&rect);
    int y = paddingTop - scrollOffset;

    int newHover = -1;
    for (size_t i = 0; i < items.size(); i++) {
      CRect itemRect(paddingLeft, y, rect.right - paddingRight,
        y + items[i]->GetHeight());
      if (itemRect.PtInRect(point)) {
        newHover = (int)i;
        break;
      }
      y += items[i]->GetHeight() + itemSpacing;
    }

    if (newHover != hoverIndex) {
      hoverIndex = newHover;
      Invalidate();
    }

    CWnd::OnMouseMove(nFlags, point);
  }

  afx_msg void OnLButtonDown(UINT nFlags, CPoint point) {
    CRect rect;
    GetClientRect(&rect);
    int y = paddingTop - scrollOffset;

    for (size_t i = 0; i < items.size(); i++) {
      CRect itemRect(paddingLeft, y, rect.right - paddingRight,
        y + items[i]->GetHeight());
      if (itemRect.PtInRect(point)) {
        if (items[i]->OnClick(point)) {
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

