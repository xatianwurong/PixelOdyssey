#pragma once

#include <afxwin.h>
#include <vector>
#include <string>
#include <algorithm>  // for max/min

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

  CString name;      // 名称
  bool enabled;      // 是否启用
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
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(240, 240, 240));  // 亮色文本 - 优化对比度
    pDC->DrawText(text, (LPRECT)&rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
  }

  int GetHeight() const override { return 25; }

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
    if (bHover && enabled) {
      CBrush brush(RGB(50, 50, 50));  // 优化悬停背景色
      pDC->FillRect(&rect, &brush);
    }

    pDC->SetBkMode(TRANSPARENT);

    // 属性名（灰色）- 优化对比度
    pDC->SetTextColor(RGB(160, 160, 160));
    CRect labelRect(rect.left + 5, rect.top, rect.left + rect.Width() / 2, rect.bottom);
    pDC->DrawText(name, (LPRECT)&labelRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // 属性值（白色）- 优化对比度
    pDC->SetTextColor(RGB(240, 240, 240));
    CRect valueRect(rect.left + rect.Width() / 2, rect.top, rect.right - 5, rect.bottom);
    pDC->DrawText(value, (LPRECT)&valueRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
  }

  int GetHeight() const override { return 28; }

  void SetValue(const CString& newValue) {
    value = newValue;
  }

private:
  CString value;
};

/**
 * @brief 按钮项 - 可点击的按钮
 */
class CUIButton : public CUIPanelItem {
public:
  CUIButton(const CString& btnName, const CString& btnLabel) {
    name = btnName;
    label = btnLabel;
    isPressed = false;
  }

  void Draw(CDC* pDC, const CRect& rect, bool bHover) override {
    COLORREF bgColor = RGB(52, 73, 94);
    if (isPressed) {
      bgColor = RGB(52, 152, 219);  // 蓝色
    }
    else if (bHover && enabled) {
      bgColor = RGB(60, 90, 120);   // 悬停
    }

    // 背景
    CBrush brush(bgColor);
    pDC->FillRect(&rect, &brush);

    // 边框
    CPen pen(PS_SOLID, 1, RGB(74, 111, 165));
    pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);

    // 文本
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(enabled ? RGB(255, 255, 255) : RGB(150, 150, 150));
  }

  int GetHeight() const override { return 35; }

  bool OnClick(const CPoint& point) override {
    if (enabled) {
      isPressed = !isPressed;
      return true;
    }
    return false;
  }

private:
  CString label;
  bool isPressed;
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
    : itemSpacing(5)
    , paddingLeft(10)
    , paddingRight(10)
    , paddingTop(35)  // 标题空间
    , paddingBottom(10)
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

