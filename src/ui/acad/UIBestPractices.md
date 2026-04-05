/**
 * @file UIBestPractices.md
 * @brief PixelOdyssey UI 系统最佳实践和代码示例
 */

# UI 系统最佳实践指南

## 1️⃣ 字体使用规范

### ✅ 推荐做法

```cpp
#include "UICommon.h"

void DrawMyCustomPanel(CDC* pDC, const CRect& rect)
{
    // 使用语义化的字体获取函数
    pDC->SelectObject(UICommon::GetBodyFont());      // 正文
    pDC->SetTextColor(RGB(240, 240, 240));
    pDC->DrawText(text, &rect, DT_LEFT | DT_VCENTER);
}
```

### ❌ 避免做法

```cpp
// 不要重复创建字体！
CFont font;
LOGFONT lf = {};
lf.lfHeight = -13;
lf.lfWeight = FW_NORMAL;
_tcscpy_s(lf.lfFaceName, _T("Microsoft YaHei UI"));
font.CreateFontIndirect(&lf);
pDC->SelectObject(&font);  // 每次都创建新对象 ❌
```

---

## 2️⃣ 颜色使用规范

### ✅ 推荐做法

```cpp
#include "../core/ColorScheme.h"

void DrawButton(CDC* pDC, const CRect& rect, bool isActive)
{
    auto& colors = ColorScheme::Instance();
    
    // 使用语义化颜色角色
    COLORREF bgColor = isActive 
        ? colors.GetColor(ColorScheme::ColorRole::Primary)      // 主色
        : colors.GetColor(ColorScheme::ColorRole::Surface);     // 表面色
    
    COLORREF textColor = colors.GetColor(
        ColorScheme::ColorRole::TextPrimary);
    
    // 绘制...
    CBrush brush(bgColor);
    pDC->FillRect(&rect, &brush);
    
    pDC->SetTextColor(textColor);
    pDC->SelectObject(UICommon::GetBodyFont());
    pDC->DrawText(label, &rect, DT_CENTER | DT_VCENTER);
}
```

### ❌ 避免做法

```cpp
// 硬编码颜色值 - 无法随主题变化
pDC->SetTextColor(RGB(240, 240, 240));      // ❌ 硬编码
CBrush brush(RGB(37, 37, 37));              // ❌ 难以维护
```

---

## 3️⃣ 间距和尺寸规范

### ✅ 推荐做法

```cpp
#include "UILayout.h"

void DrawPanel(CDC* pDC, const CRect& rect)
{
    // 使用语义化间距常量
    int padding = UISpacing::M;              // 中等间距 = 16px
    int itemGap = UISpacing::S;              // 小间距 = 8px
    
    CRect contentRect = rect;
    contentRect.DeflateRect(padding, padding);
    
    int y = contentRect.top;
    for (const auto& item : items) {
        int h = item->GetHeight();
        CRect itemRect(contentRect.left, y, contentRect.right, y + h);
        
        item->Draw(pDC, itemRect, false);
        y += h + itemGap;
    }
}
```

### ❌ 避免做法

```cpp
// 魔法数字散布在代码中
contentRect.DeflateRect(16, 16);            // ❌ 为什么是 16?
y += height + 8;                            // ❌ 8 代表什么?
```

---

## 4️⃣ UI 控件创建规范

### 创建新的 UI 控件

继承 `CUIPanelItem` 实现以下三个方法：

```cpp
#include "UIComponents.h"

/**
 * @brief 自定义复选框控件
 */
class CUICheckBox : public CUIPanelItem
{
public:
    CUICheckBox(const CString& label, bool checked = false)
        : label(label), isChecked(checked)
    {
    }

    // 必须实现：绘制
    void Draw(CDC* pDC, const CRect& rect, bool bHover) override
    {
        auto& colors = ColorScheme::Instance();
        
        // 复选框背景
        int boxSize = UILayout::Controls::CHECKBOX_SIZE;
        CRect boxRect(rect.left, rect.top + (rect.Height() - boxSize) / 2,
                      rect.left + boxSize, rect.top + (rect.Height() - boxSize) / 2 + boxSize);
        
        COLORREF boxBg = isChecked 
            ? colors.GetColor(ColorScheme::ColorRole::Primary)
            : colors.GetColor(ColorScheme::ColorRole::Surface);
        
        CBrush brush(boxBg);
        pDC->FillRect(&boxRect, &brush);
        
        // 复选框边框
        CPen pen(PS_SOLID, 1, colors.GetColor(ColorScheme::ColorRole::Border));
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(&boxRect);
        pDC->SelectObject(pOldPen);
        
        // 如果勾选，绘制对勾
        if (isChecked) {
            pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
            pDC->SelectObject(UICommon::GetBodyFont());
            CRect checkRect = boxRect;
            pDC->DrawText(_T("✓"), &checkRect, DT_CENTER | DT_VCENTER);
        }
        
        // 标签文本
        CRect labelRect(boxRect.right + UISpacing::S, rect.top,
                        rect.right, rect.bottom);
        pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
        pDC->SelectObject(UICommon::GetBodyFont());
        pDC->DrawText(label, &labelRect, DT_LEFT | DT_VCENTER);
    }

    // 必须实现：高度
    int GetHeight() const override 
    { 
        return UILayout::Controls::CHECKBOX_SIZE + 4; 
    }

    // 可选：点击处理
    bool OnClick(const CPoint& point) override
    {
        if (enabled) {
            isChecked = !isChecked;
            return true;
        }
        return false;
    }

    bool IsChecked() const { return isChecked; }
    void SetChecked(bool checked) { isChecked = checked; }

private:
    CString label;
    bool isChecked;
};
```

### 在面板中使用

```cpp
void InitializePanel()
{
    panel.ClearItems();
    
    // 添加标签
    panel.AddItem(std::make_unique<CUILabel>(_T("设置")));
    
    // 添加属性项
    panel.AddItem(std::make_unique<CUIPropertyItem>(_T("名称:"), _T("对象1")));
    
    // 添加分隔线
    panel.AddItem(std::make_unique<CUISeparator>());
    
    // 添加自定义复选框
    auto checkbox = std::make_unique<CUICheckBox>(_T("启用阴影"), true);
    panel.AddItem(std::move(checkbox));
    
    // 添加按钮
    auto button = std::make_unique<CUIButton>(_T("btnSave"), _T("保存"));
    button->SetOnClick([]() { MessageBox(NULL, _T("Saved!"), _T("Info"), MB_OK); });
    panel.AddItem(std::move(button));
}
```

---

## 5️⃣ 工具栏按钮规范

### 添加工具栏按钮

```cpp
#include "AcadToolPalette.h"

void InitializeToolbar(CAcadToolPalette& toolbar)
{
    // AddToolButton(type, tooltip, iconType)
    // iconType: 0=选择, 1=移动, 2=旋转, 3=缩放, 4=平移
    
    toolbar.AddToolButton(ToolType::Select, _T("选择 (V)"), 0);
    toolbar.AddToolButton(ToolType::Move, _T("移动 (G)"), 1);
    toolbar.AddToolButton(ToolType::Rotate, _T("旋转 (R)"), 2);
    toolbar.AddToolButton(ToolType::Scale, _T("缩放 (S)"), 3);
    toolbar.AddToolButton(ToolType::Pan, _T("平移 (H)"), 4);
}
```

### 自定义工具栏按钮外观

```cpp
#include "UIToolbar.h"

void MyCustomDraw(CDC* pDC, const CRect& rect, bool isHovered)
{
    auto& colors = ColorScheme::Instance();
    
    // 决定按钮状态
    UIToolbar::ButtonState state = isHovered 
        ? UIToolbar::ButtonState::Hovered
        : UIToolbar::ButtonState::Normal;
    
    // 决定颜色
    COLORREF bgColor = colors.GetColor(
        isHovered ? ColorScheme::ColorRole::PrimaryHover 
                  : ColorScheme::ColorRole::Surface);
    
    COLORREF borderColor = colors.GetColor(
        ColorScheme::ColorRole::Border);
    
    // 使用统一的绘制函数
    UIToolbar::DrawButton(pDC, rect, state, 0, bgColor, borderColor);
}
```

---

## 6️⃣ 主题切换

### 支持的主题

```cpp
enum class Theme {
    Dark,           // 深色主题（默认）- 类似 VS Dark
    Light,          // 浅色主题 - 类似 VS Light
    HighContrast,   // 高对比度 - 提高可访问性
    Custom          // 自定义主题
};
```

### 切换主题

```cpp
#include "../core/ColorScheme.h"

void SwitchTheme(ColorScheme::Theme newTheme)
{
    auto& colors = ColorScheme::Instance();
    colors.SetTheme(newTheme);
    
    // 通知所有 UI 元素重绘
    // 需要调用 Invalidate() 或发送 WM_PAINT 消息
}
```

### 查询当前主题

```cpp
ColorScheme::Theme currentTheme = ColorScheme::Instance().GetTheme();
if (currentTheme == ColorScheme::Theme::Dark) {
    // 深色模式特定的逻辑
}
```

---

## 7️⃣ 常见问题解答 (FAQ)

### Q: 如何改变全局字体大小？
A: 修改 `UILayout.h` 中的字体大小常量：
```cpp
namespace UIFonts {
    constexpr int BODY_SIZE = 14;  // 改为 14 代替 13
}
```

### Q: 如何自定义按钮样式？
A: 在 `CUIButton::Draw()` 中修改颜色逻辑：
```cpp
// 在 CUIButton 中添加自定义颜色成员
COLORREF m_customBgColor = RGB(0, 120, 212);
```

### Q: 工具栏图标可以自定义吗？
A: 可以！修改 `UIToolbar::DrawSimpleIcon()` 添加更多图标类型，或使用 `UIToolbar::DrawButton()` 绘制自定义内容。

### Q: 如何支持高 DPI？
A: 目前字体大小是硬编码的。计划改进：
```cpp
// 未来改进
int ScaledFontSize(int baseSize) {
    return baseSize * GetSystemDPI() / 96;  // 96 DPI = 100%
}
```

---

## 8️⃣ 性能建议

### ✅ 高效做法

1. **缓存字体对象**：`UICommon` 已自动缓存
2. **避免重复颜色计算**：在循环外获取颜色
3. **使用双缓冲绘制**：MFC 的 `OnPaint()` 自动处理

```cpp
void DrawItemsEfficiently(CDC* pDC, const std::vector<Item>& items)
{
    // 在循环外获取颜色 ✅
    auto& colors = ColorScheme::Instance();
    COLORREF textColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
    
    pDC->SelectObject(UICommon::GetBodyFont());
    
    for (const auto& item : items) {
        // 仅设置必要的属性
        pDC->SetTextColor(textColor);
        pDC->DrawText(item.text, &rect, DT_LEFT | DT_VCENTER);
    }
}
```

### ❌ 低效做法

```cpp
for (const auto& item : items) {
    // 每次循环都获取颜色 ❌
    auto& colors = ColorScheme::Instance();
    
    // 每次循环都选择字体 ❌
    pDC->SelectObject(UICommon::GetBodyFont());
    
    pDC->DrawText(item.text, &rect, DT_LEFT | DT_VCENTER);
}
```

---

## 9️⃣ 测试清单

在提交 UI 相关的代码前，检查以下项目：

- [ ] 使用 `UICommon::Get*Font()` 而非硬编码字体
- [ ] 使用 `ColorScheme::GetColor()` 获取颜色
- [ ] 使用 `UISpacing::*` 和 `UILayout::*` 常量
- [ ] 新控件继承 `CUIPanelItem` 并实现三个必需方法
- [ ] 代码在 Dark、Light、HighContrast 三种主题下都清晰可读
- [ ] 中文文本能正确显示（测试工具栏）
- [ ] 无内存泄漏（特别是 CDC 对象和画笔）
- [ ] 代码符合项目的命名和格式规范

---

**最后更新**: 2024
**相关文件**: UIRefactor.md, UILayout.h, UICommon.h, UIComponents.h, UIToolbar.h
