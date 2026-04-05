# 🚀 PixelOdyssey UI 模块使用指南

## 📖 快速开始

### 1. 理解核心概念（5 分钟）

PixelOdyssey UI 系统由四个核心模块组成：

#### 🔤 字体管理 (UICommon.h)
```cpp
#include "UICommon.h"

pDC->SelectObject(UICommon::GetBodyFont());       // 13px 正文
pDC->SelectObject(UICommon::GetTitleFont());      // 18px 标题
pDC->SelectObject(UICommon::GetMonospaceFont());  // 12px 等宽
```

#### 🎨 颜色系统 (ColorScheme.h)
```cpp
auto& colors = ColorScheme::Instance();
COLORREF textColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
COLORREF bgColor = colors.GetColor(ColorScheme::ColorRole::Surface);
```

#### 📏 间距常量 (UILayout.h)
```cpp
int padding = UISpacing::M;      // 16px 中等间距
int gap = UISpacing::S;          // 8px 小间距
rect.DeflateRect(padding, gap);  // 使用常量
```

#### 🛠️ 工具栏 (UIToolbar.h)
```cpp
UIToolbar::DrawButtonWithLabel(pDC, rect, state, iconType, label, bgColor, borderColor);
UIToolbar::DrawTooltip(pDC, tooltipRect, text, bgColor, textColor);
```

---

### 2. 创建基本控件（15 分钟）

```cpp
#include "UIComponents.h"
#include "UICommon.h"

class MyPanel : public CUnifiedPanel
{
public:
    void InitializeUI()
    {
        // 添加标签
        AddItem(std::make_unique<CUILabel>(_T("用户信息")));
        
        // 添加属性项
        AddItem(std::make_unique<CUIPropertyItem>(_T("姓名:"), _T("张三")));
        AddItem(std::make_unique<CUIPropertyItem>(_T("年龄:"), _T("25")));
        
        // 添加分隔线
        AddItem(std::make_unique<CUISeparator>());
        
        // 添加按钮
        auto btn = std::make_unique<CUIButton>(_T("btnSave"), _T("保存"));
        btn->SetOnClick([]() { 
            MessageBox(NULL, _T("已保存!"), _T("提示"), MB_OK); 
        });
        AddItem(std::move(btn));
    }
};
```

---

### 3. 自定义控件（30 分钟）

```cpp
#include "UIComponents.h"
#include "UICommon.h"
#include "../core/ColorScheme.h"

/**
 * @brief 自定义的滑块控件
 */
class CUISlider : public CUIPanelItem
{
public:
    CUISlider(const CString& label, int minVal, int maxVal, int value)
        : label(label), minVal(minVal), maxVal(maxVal), currentVal(value)
    {
        name = label;
    }

    // 必须实现：绘制
    void Draw(CDC* pDC, const CRect& rect, bool bHover) override
    {
        auto& colors = ColorScheme::Instance();
        
        // 绘制标签
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
        pDC->SelectObject(UICommon::GetBodyFont());
        
        CRect labelRect(rect.left, rect.top, rect.right / 2, rect.bottom);
        pDC->DrawText(label, reinterpret_cast<LPRECT>(&labelRect), 
                      DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        
        // 绘制滑块轨道
        CRect trackRect(rect.right / 2 + 8, rect.top + 8,
                       rect.right - 8, rect.top + 16);
        
        CBrush trackBrush(colors.GetColor(ColorScheme::ColorRole::Border));
        pDC->FillRect(&trackRect, &trackBrush);
        
        // 绘制滑块
        int sliderX = trackRect.left + (trackRect.Width() * (currentVal - minVal)) / (maxVal - minVal);
        CRect sliderRect(sliderX - 5, rect.top + 4, sliderX + 5, rect.top + 20);
        
        COLORREF sliderColor = bHover 
            ? colors.GetColor(ColorScheme::ColorRole::Primary)
            : colors.GetColor(ColorScheme::ColorRole::Surface);
        
        CBrush sliderBrush(sliderColor);
        pDC->FillRect(&sliderRect, &sliderBrush);
        
        // 绘制值
        CString valueStr;
        valueStr.Format(_T("%d"), currentVal);
        pDC->DrawText(valueStr, &sliderRect, DT_CENTER | DT_VCENTER);
    }

    // 必须实现：高度
    int GetHeight() const override { return 32; }

    // 可选：点击处理
    bool OnClick(const CPoint& point) override
    {
        // 检查点击是否在滑块上
        // ...
        return false;
    }

    int GetValue() const { return currentVal; }
    void SetValue(int val) { 
        currentVal = std::max(minVal, std::min(val, maxVal)); 
    }

private:
    CString label;
    int minVal, maxVal, currentVal;
};
```

使用自定义控件：
```cpp
auto slider = std::make_unique<CUISlider>(_T("音量:"), 0, 100, 50);
panel.AddItem(std::move(slider));
```

---

### 4. 主题切换（10 分钟）

```cpp
// 切换到浅色主题
ColorScheme::Instance().SetTheme(ColorScheme::Theme::Light);
panel.Invalidate();  // 重绘面板

// 切换到高对比度主题
ColorScheme::Instance().SetTheme(ColorScheme::Theme::HighContrast);
panel.Invalidate();

// 查询当前主题
if (ColorScheme::Instance().GetTheme() == ColorScheme::Theme::Dark) {
    // 深色主题特定的逻辑
}
```

---

## 📚 文档导航

### 对于不同角色

#### 👤 UI 使用者
- 阅读：`QUICK_REFERENCE.md`（5 分钟）
- 查看：代码示例
- 遇问题：参考 `UIBestPractices.md`

#### 👨‍💻 前端开发者
- 阅读：`UIBestPractices.md`（1 小时）
- 学习：现有控件实现
- 实践：修改控件样式或创建新控件

#### 🏗️ 架构师
- 阅读：`UIRefactor.md` + `COMPLETE_REFACTOR_SUMMARY.md`（2 小时）
- 理解：系统设计和扩展机制
- 规划：长期演进方向

#### 🐛 维护者
- 阅读：所有文档（全面理解）
- 掌握：字体、颜色、间距等全套系统
- 管理：新功能整合和问题修复

---

## 🎯 常见任务

### 任务 1：改变所有按钮的颜色

修改 `ColorScheme.cpp` 中的主题设置：
```cpp
void ColorScheme::SetupDarkTheme() {
    // ...
    m_colors[ColorRole::Primary] = RGB(50, 150, 255);  // 新的蓝色
    // ...
}
```

### 任务 2：增加工具栏按钮

修改 `AcadToolPalette::Create()`:
```cpp
// 添加新工具
toolbar.AddToolButton(ToolType::YourNewTool, _T("新工具 (X)"), 5);
```

并在 `UIToolbar::DrawSimpleIcon()` 中添加第 6 种图标：
```cpp
case 5: // 你的新工具图标
{
    // 使用 LineTo, Ellipse 等绘制图形
}
break;
```

### 任务 3：添加新的属性显示

在 `AcadPropertyPanel::RebuildProperties()` 中添加：
```cpp
AddItem(std::make_unique<CUIPropertyItem>(_T("你的属性:"), value));
```

### 任务 4：自定义面板背景色

创建面板子类并重写 `OnDrawPanel()`:
```cpp
class MyCustomPanel : public CUnifiedPanel
{
    void OnDrawPanel(CDC* pDC, const CRect& rect) override
    {
        CBrush brush(RGB(50, 50, 50));  // 自定义背景
        pDC->FillRect(&rect, &brush);
    }
};
```

---

## 🔍 故障排除

### 问题 1：工具栏看不清楚

**可能原因**: 字体或颜色配置问题

**解决方案**:
```cpp
// 检查字体
auto font = UICommon::GetChineeseFont(10);

// 检查颜色
auto& colors = ColorScheme::Instance();
COLORREF testColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);

// 验证主题
if (colors.GetTheme() == ColorScheme::Theme::Dark) {
    // 深色模式下检查颜色对比度
}
```

### 问题 2：中文显示乱码

**可能原因**: 字体不支持中文

**解决方案**:
```cpp
// 确保使用中文字体
pDC->SelectObject(UICommon::GetChineeseFont());

// 确保代码文件是 UTF-8 编码
// 确保使用 _T("中文") 宏
```

### 问题 3：点击按钮无反应

**可能原因**: 消息映射或事件处理不正确

**解决方案**:
1. 检查 `BEGIN_MESSAGE_MAP` 和 `END_MESSAGE_MAP`
2. 检查 `SetOnClick()` 是否被调用
3. 检查回调函数是否为 nullptr

---

## 💡 最佳实践总结

### ✅ 应该做的事

```cpp
// 1. 使用 UICommon 的字体
pDC->SelectObject(UICommon::GetBodyFont());

// 2. 使用 ColorScheme 的颜色
COLORREF color = colors.GetColor(ColorScheme::ColorRole::Primary);

// 3. 使用 UISpacing 的间距
rect.DeflateRect(UISpacing::M, UISpacing::S);

// 4. 在 Draw() 方法中设置 TRANSPARENT 背景模式
pDC->SetBkMode(TRANSPARENT);

// 5. 使用智能指针管理对象
auto item = std::make_unique<CUIButton>(...);
```

### ❌ 应该避免的事

```cpp
// 1. 不要重复创建字体
CFont font;  // ❌ 不好
font.CreateFontIndirect(&lf);

// 2. 不要硬编码颜色
pDC->SetTextColor(RGB(240, 240, 240));  // ❌ 不好

// 3. 不要硬编码间距
rect.DeflateRect(16, 8);  // ❌ 不好，应该用 UISpacing::M/S

// 4. 不要忘记设置背景模式
// pDC->SetBkMode(TRANSPARENT);  // ❌ 忘了

// 5. 不要使用裸指针
auto item = new CUIButton(...);  // ❌ 不好，应该用 unique_ptr
```

---

## 📞 获取帮助

### 快速查询
- **字体**: QUICK_REFERENCE.md → "字体使用"
- **颜色**: QUICK_REFERENCE.md → "颜色使用"
- **间距**: QUICK_REFERENCE.md → "间距使用"
- **工具栏**: QUICK_REFERENCE.md → "工具栏"

### 详细指南
- **系统设计**: UIRefactor.md
- **最佳实践**: UIBestPractices.md
- **代码示例**: QUICK_REFERENCE.md
- **常见问题**: UIBestPractices.md → FAQ

### 源代码参考
- 基础控件：`UIComponents.h/cpp`
- 工具栏：`UIToolbar.h/cpp`, `AcadToolPalette.cpp`
- 属性面板：`AcadPropertyPanel.cpp`

---

## 🎓 进阶话题

### 性能优化
```cpp
// 在循环外获取颜色
auto& colors = ColorScheme::Instance();
COLORREF textColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);

for (const auto& item : items) {
    pDC->SetTextColor(textColor);  // 重用颜色
    // ...
}
```

### 动画效果
```cpp
// 未来计划支持的动画效果
// 目前使用简单的 Invalidate() 重绘
panel.Invalidate();

// 后续可添加计时器进行平滑过渡
SetTimer(IDT_ANIMATION_TIMER, 16, nullptr);  // 60 FPS
```

### 自定义主题
```cpp
// 创建自定义主题
ColorScheme::Instance().SetTheme(ColorScheme::Theme::Custom);
ColorScheme::Instance().SetColor(ColorScheme::ColorRole::Primary, RGB(255, 100, 100));
```

---

## 📝 总结

PixelOdyssey UI 系统提供了：
- ✅ 统一的字体管理（UICommon）
- ✅ 统一的颜色管理（ColorScheme）
- ✅ 统一的间距管理（UISpacing）
- ✅ 统一的工具栏绘制（UIToolbar）
- ✅ 可扩展的控件系统（CUIPanelItem）
- ✅ 完整的文档和示例

现在你可以：
- 🎯 快速创建一致的 UI
- 🎨 轻松定制样式和主题
- 🔧 便捷添加新的控件
- 📚 参考丰富的示例代码

**祝你使用愉快！**

---

**上次更新**: 2024  
**适用版本**: v2.0+  
**相关文件**: UIComponents.h, UICommon.h, UIToolbar.h, ColorScheme.h, UILayout.h
