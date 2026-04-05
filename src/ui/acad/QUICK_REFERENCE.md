# UI 系统快速参考

## 🎨 颜色使用

```cpp
auto& colors = ColorScheme::Instance();

// 主要颜色
RGB_PRIMARY = colors.GetColor(ColorScheme::ColorRole::Primary);
RGB_SURFACE = colors.GetColor(ColorScheme::ColorRole::Surface);
RGB_HOVER = colors.GetColor(ColorScheme::ColorRole::SurfaceHover);

// 文字颜色
RGB_TEXT_PRIMARY = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
RGB_TEXT_SECONDARY = colors.GetColor(ColorScheme::ColorRole::TextSecondary);
RGB_TEXT_DISABLED = colors.GetColor(ColorScheme::ColorRole::TextDisabled);

// 状态颜色
RGB_SUCCESS = colors.GetColor(ColorScheme::ColorRole::Success);
RGB_WARNING = colors.GetColor(ColorScheme::ColorRole::Warning);
RGB_ERROR = colors.GetColor(ColorScheme::ColorRole::Error);
```

## 🔤 字体使用

```cpp
#include "UICommon.h"

pDC->SelectObject(UICommon::GetBodyFont());        // 13px 正文
pDC->SelectObject(UICommon::GetTitleFont());       // 18px 标题
pDC->SelectObject(UICommon::GetMonospaceFont());   // 12px 等宽
pDC->SelectObject(UICommon::GetSymbolFont());      // 符号字体
pDC->SelectObject(UICommon::GetChineeseFont());    // 中文字体
```

## 📏 间距使用

```cpp
#include "UILayout.h"

UISpacing::XS  // 4px  - 超小
UISpacing::S   // 8px  - 小
UISpacing::M   // 16px - 中
UISpacing::L   // 24px - 大
UISpacing::XL  // 32px - 超大

rect.DeflateRect(UISpacing::S, 0);           // 左右各缩小 8px
int gap = UISpacing::M;                      // 16px 间距
```

## 🎛️ 工具栏

```cpp
#include "UIToolbar.h"

// 图标类型 (0-4)
UIToolbar::DrawSimpleIcon(pDC, rect, 0, color);  // 0=选择, 1=移动, etc.

// 按钮状态
UIToolbar::ButtonState::Normal
UIToolbar::ButtonState::Hovered
UIToolbar::ButtonState::Active
UIToolbar::ButtonState::Disabled

// 绘制按钮
UIToolbar::DrawButton(pDC, rect, state, iconType, bgColor, borderColor);

// 绘制提示
UIToolbar::DrawTooltip(pDC, tooltipRect, text, bgColor, textColor);
```

## 🎯 UI 控件

```cpp
#include "UIComponents.h"

// 创建控件
auto label = std::make_unique<CUILabel>(_T("标签"));
auto prop = std::make_unique<CUIPropertyItem>(_T("名称:"), _T("值"));
auto btn = std::make_unique<CUIButton>(_T("id"), _T("按钮"));
auto sep = std::make_unique<CUISeparator>();

// 添加到面板
panel.AddItem(std::move(label));
panel.AddItem(std::move(prop));

// 按钮回调
btn->SetOnClick([]() { /* 处理点击 */ });

// 获取控件
CUIPropertyItem* prop = static_cast<CUIPropertyItem*>(panel.GetItem(0));
prop->SetValue(_T("新值"));
```

## 🌈 主题切换

```cpp
// 深色主题（默认）
ColorScheme::Instance().SetTheme(ColorScheme::Theme::Dark);

// 浅色主题
ColorScheme::Instance().SetTheme(ColorScheme::Theme::Light);

// 高对比度主题
ColorScheme::Instance().SetTheme(ColorScheme::Theme::HighContrast);

// 查询当前主题
if (ColorScheme::Instance().GetTheme() == ColorScheme::Theme::Dark) {
    // 深色主题特定代码
}
```

## 📋 快速清单

添加新 UI 代码时：
- [ ] 使用 `UICommon::Get*Font()` 获取字体
- [ ] 使用 `ColorScheme::GetColor()` 获取颜色
- [ ] 使用 `UISpacing::*` 常量定义间距
- [ ] 使用 `UILayout::*` 常量定义尺寸
- [ ] 中文字符使用 `_T("中文")` 并使用 `GetChineeseFont()`
- [ ] 新控件继承 `CUIPanelItem` 并实现 `Draw()`, `GetHeight()`, `OnClick()`

## 🔗 相关文件

- `UICommon.h` - 字体管理
- `UILayout.h` - 布局常量
- `UIComponents.h` - 基础控件
- `UIToolbar.h` - 工具栏绘制
- `ColorScheme.h` - 颜色和主题
- `UIRefactor.md` - 重构文档
- `UIBestPractices.md` - 最佳实践

## ⚡ 常见用法

### 绘制文本
```cpp
pDC->SetBkMode(TRANSPARENT);
pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
pDC->SelectObject(UICommon::GetBodyFont());
pDC->DrawText(text, reinterpret_cast<LPRECT>(&rect), 
              DT_LEFT | DT_VCENTER | DT_SINGLELINE);
```

### 绘制矩形
```cpp
auto& colors = ColorScheme::Instance();
CBrush brush(colors.GetColor(ColorScheme::ColorRole::Surface));
pDC->FillRect(&rect, &brush);

CPen pen(PS_SOLID, 1, colors.GetColor(ColorScheme::ColorRole::Border));
pDC->SelectObject(&pen);
pDC->SelectStockObject(NULL_BRUSH);
pDC->Rectangle(&rect);
```

### 绘制悬停效果
```cpp
COLORREF bgColor = bHover 
    ? colors.GetColor(ColorScheme::ColorRole::PrimaryHover)
    : colors.GetColor(ColorScheme::ColorRole::Surface);

CBrush brush(bgColor);
pDC->FillRect(&rect, &brush);
```

---

**快速链接**: [完整重构文档](UIRefactor.md) | [最佳实践](UIBestPractices.md) | [重构总结](REFACTOR_SUMMARY.md)
