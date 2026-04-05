# 🚀 UI 设计系统实施指南

> **从规范到代码** | 逐步实施 | 可持续演进

---

## 📋 快速导航

- [第一阶段：基础设置](#第一阶段基础设置)
- [第二阶段：控件改进](#第二阶段控件改进)
- [第三阶段：主题系统](#第三阶段主题系统)
- [第四阶段：验证与测试](#第四阶段验证与测试)

---

## 第一阶段：基础设置

### 步骤 1.1: 导入设计系统

```cpp
// UIComponents.h
#include "UIDesignSystem.h"

using namespace UIDesign;
```

### 步骤 1.2: 更新字体管理

在 `UICommon.cpp` 中：

```cpp
// 使用 UIDesignSystem 的字体定义
CFont* UICommon::GetFont(int size, int weight, const TCHAR* family)
{
    // 参考设计系统中的定义
    // FontSize::BODY = 13pt
    // FontWeight::NORMAL = 400
    
    using tstring = std::basic_string<TCHAR>;
    std::hash<tstring> hasher;
    tstring faceStr(family);
    unsigned long long faceHash = hasher(faceStr);
    unsigned long long key = ((unsigned long long)size << 48) ^ 
                            ((unsigned long long)weight << 40) ^ 
                            (faceHash & 0xFFFFFFFFFFULL);

    static std::map<unsigned long long, CFont*> s_fonts;
    auto it = s_fonts.find(key);
    if (it != s_fonts.end())
        return it->second;

    LOGFONT lf = {};
    lf.lfHeight = -size;
    lf.lfWeight = weight;
    lf.lfQuality = CLEARTYPE_QUALITY;
    _tcscpy_s(lf.lfFaceName, family);

    CFont* pFont = new CFont();
    pFont->CreateFontIndirect(&lf);
    s_fonts.insert(std::make_pair(key, pFont));
    return pFont;
}

// 快速函数
CFont* UICommon::GetBodyFont()
{
    return GetFont(Typography::FontSize::BODY, 
                   Typography::FontWeight::NORMAL);
}

CFont* UICommon::GetTitleFont()
{
    return GetFont(Typography::FontSize::HEADLINE, 
                   Typography::FontWeight::SEMIBOLD);
}
```

### 步骤 1.3: 更新布局常量

在 `UILayout.h` 中：

```cpp
namespace UILayout {
    // 引用设计系统间距
    constexpr int STANDARD_PADDING = UIDesign::Spacing::M;      // 16px
    constexpr int ITEM_SPACING = UIDesign::Spacing::Component::GAP_ITEM;  // 8px
    constexpr int BORDER_RADIUS = UIDesign::Spacing::Component::BORDER_RADIUS;  // 8px
    
    // 控件高度
    constexpr int BUTTON_HEIGHT = UIDesign::ComponentSize::Height::MEDIUM;  // 44px
    constexpr int LIST_ITEM_HEIGHT = UIDesign::ComponentSize::Height::LIST_ITEM;  // 44px
}
```

---

## 第二阶段：控件改进

### 步骤 2.1: 按钮控件 (CUIButton)

```cpp
class CUIButton : public CUIPanelItem
{
public:
    void Draw(CDC* pDC, const CRect& rect, bool bHover) override
    {
        auto& colors = ColorScheme::Instance();
        
        // 使用设计系统颜色
        COLORREF bgColor = !enabled
            ? UIDesign::Colors::Dark::SURFACE
            : isPressed || bHover ? UIDesign::Colors::Dark::PRIMARY
                                  : UIDesign::Colors::Dark::SURFACE;

        COLORREF borderColor = !enabled 
            ? UIDesign::Colors::Dark::BORDER
            : isPressed || bHover ? UIDesign::Colors::Dark::PRIMARY
                                  : UIDesign::Colors::Dark::BORDER;

        // 绘制背景
        CBrush brush(bgColor);
        pDC->FillRect(&rect, &brush);
        
        // 绘制边框
        CPen pen(PS_SOLID, 1, borderColor);
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->SelectStockObject(NULL_BRUSH);
        
        // 圆角矩形 - 使用设计系统的圆角大小
        CRect btnRect = rect;
        btnRect.DeflateRect(Spacing::S, 2);
        pDC->RoundRect(&btnRect, 
            CPoint(Spacing::Component::BORDER_RADIUS, 
                   Spacing::Component::BORDER_RADIUS));
        pDC->SelectObject(pOldPen);
        
        // 文本
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(enabled ? UIDesign::Colors::Dark::TEXT_PRIMARY
                                  : UIDesign::Colors::Dark::TEXT_DISABLED);
        pDC->SelectObject(UICommon::GetBodyFont());
        
        CRect textRect = rect;
        textRect.DeflateRect(Spacing::Component::PADDING_HORIZONTAL,
                           Spacing::Component::PADDING_VERTICAL);
        pDC->DrawText(label, reinterpret_cast<LPRECT>(&textRect), 
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override 
    { 
        return ComponentSize::Height::MEDIUM;  // 44px
    }

private:
    using Spacing = UIDesign::Spacing;
    using Colors = UIDesign::Colors::Dark;
    using ComponentSize = UIDesign::ComponentSize;
};
```

### 步骤 2.2: 列表项 (CUIPropertyItem)

```cpp
class CUIPropertyItem : public CUIPanelItem
{
public:
    void Draw(CDC* pDC, const CRect& rect, bool bHover) override
    {
        auto& colors = ColorScheme::Instance();
        
        // 悬停背景
        if (bHover && enabled) {
            // 背景 + 8% 白色叠加
            COLORREF hoverColor = UIDesign::BlendColor(
                UIDesign::Colors::Dark::SURFACE,
                RGB(255, 255, 255),
                UIDesign::States::Opacity::HOVER);
            CBrush brush(hoverColor);
            pDC->FillRect(&rect, &brush);
        }
        
        pDC->SetBkMode(TRANSPARENT);
        
        // 属性名 (辅助文字)
        pDC->SetTextColor(UIDesign::Colors::Dark::TEXT_SECONDARY);
        pDC->SelectObject(UICommon::GetFont(
            Typography::FontSize::BODY_SMALL,
            Typography::FontWeight::NORMAL));
        
        CRect labelRect = rect;
        labelRect.DeflateRect(Spacing::M, Spacing::S);
        labelRect.right = rect.left + (rect.Width() * 40) / 100;
        pDC->DrawText(name, reinterpret_cast<LPRECT>(&labelRect),
                     DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        
        // 属性值 (主文字)
        pDC->SetTextColor(UIDesign::Colors::Dark::TEXT_PRIMARY);
        pDC->SelectObject(UICommon::GetBodyFont());
        
        CRect valueRect = rect;
        valueRect.DeflateRect(Spacing::M, Spacing::S);
        valueRect.left = rect.left + (rect.Width() * 40) / 100;
        pDC->DrawText(value, reinterpret_cast<LPRECT>(&valueRect),
                     DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override 
    { 
        return ComponentSize::Height::LIST_ITEM;  // 44px
    }

private:
    using Spacing = UIDesign::Spacing;
    using Colors = UIDesign::Colors::Dark;
    using ComponentSize = UIDesign::ComponentSize;
    using Typography = UIDesign::Typography;
};
```

### 步骤 2.3: 标签 (CUILabel)

```cpp
class CUILabel : public CUIPanelItem
{
public:
    void Draw(CDC* pDC, const CRect& rect, bool /*bHover*/) override
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(UIDesign::Colors::Dark::TEXT_PRIMARY);
        
        // 使用 Headline 样式使标签更显眼
        pDC->SelectObject(UICommon::GetFont(
            Typography::FontSize::HEADLINE,
            Typography::FontWeight::SEMIBOLD));
        
        CRect trect = rect;
        trect.DeflateRect(Spacing::M, Spacing::S);
        pDC->DrawText(text, reinterpret_cast<LPRECT>(&trect),
                     DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    int GetHeight() const override 
    { 
        return 36;  // Headline + padding
    }

private:
    using Spacing = UIDesign::Spacing;
    using Colors = UIDesign::Colors::Dark;
    using Typography = UIDesign::Typography;
};
```

---

## 第三阶段：主题系统

### 步骤 3.1: 创建主题切换

```cpp
// 在 ColorScheme.h 中添加
class ColorScheme {
public:
    enum class Theme {
        Dark,
        Light,
        HighContrast
    };
    
    static ColorScheme& Instance() {
        static ColorScheme instance;
        return instance;
    }
    
    void SetTheme(Theme newTheme) {
        currentTheme = newTheme;
        // 通知所有窗口重绘
        // ...
    }
    
    Theme GetTheme() const {
        return currentTheme;
    }
    
    COLORREF GetColor(ColorRole role) {
        switch (currentTheme) {
            case Theme::Dark:
                return GetDarkColor(role);
            case Theme::Light:
                return GetLightColor(role);
            case Theme::HighContrast:
                return GetHighContrastColor(role);
            default:
                return GetDarkColor(role);
        }
    }

private:
    Theme currentTheme = Theme::Dark;
    
    COLORREF GetDarkColor(ColorRole role) {
        // 使用 UIDesign::Colors::Dark::*
        // ...
    }
    
    COLORREF GetLightColor(ColorRole role) {
        // 使用 UIDesign::Colors::Light::*
        // ...
    }
    
    COLORREF GetHighContrastColor(ColorRole role) {
        // 使用 UIDesign::Colors::HighContrast::*
        // ...
    }
};
```

### 步骤 3.2: 主题切换按钮

```cpp
// 在主窗口菜单中添加
void MainFrame::OnThemeDark() {
    ColorScheme::Instance().SetTheme(ColorScheme::Theme::Dark);
    GetPropertyPanel().Invalidate();
    GetToolbar().Invalidate();
}

void MainFrame::OnThemeLight() {
    ColorScheme::Instance().SetTheme(ColorScheme::Theme::Light);
    GetPropertyPanel().Invalidate();
    GetToolbar().Invalidate();
}

void MainFrame::OnThemeHighContrast() {
    ColorScheme::Instance().SetTheme(ColorScheme::Theme::HighContrast);
    GetPropertyPanel().Invalidate();
    GetToolbar().Invalidate();
}
```

---

## 第四阶段：验证与测试

### 步骤 4.1: 编译验证

```bash
# 编译检查
msbuild PixelOdyssey.sln /p:Configuration=Release

# 检查结果
# ✅ 0 errors
# ✅ 0 warnings
```

### 步骤 4.2: 无障碍测试

```cpp
// 检查清单
[] 所有按钮 ≥ 44px × 44px
[] 所有文字 ≥ 11pt
[] 焦点环清晰可见
[] 颜色对比度 ≥ 4.5:1
[] 支持键盘导航 (Tab, Shift+Tab)
[] 支持 Ctrl+焦点快捷键
[] 屏幕阅读器兼容
[] 高对比度主题可用
```

### 步骤 4.3: 视觉测试

```cpp
// 验证清单
[] 字体层级清晰
[] 间距规范一致
[] 颜色搭配和谐
[] 圆角一致 (8px)
[] 边框一致 (1px)
[] 状态反馈明显
[] 整体风格统一
[] 现代感强
```

### 步骤 4.4: 性能测试

```cpp
// 基准测试
// 编译时间: < 5s (通常 2.5-3s)
// 启动时间: < 1s (无变化)
// 运行内存: < 1MB 增加
// 帧率: 60 FPS (无变化)
```

---

## 📋 迁移清单

### 必须做的事

- [ ] 导入 UIDesignSystem.h
- [ ] 更新 UICommon 函数
- [ ] 更新 UILayout 常量
- [ ] 改进所有 UI 控件
- [ ] 实施主题系统
- [ ] 编译验证
- [ ] 无障碍测试
- [ ] 视觉验证
- [ ] 文档更新

### 可选的事

- [ ] 添加主题切换 UI
- [ ] 用户主题偏好保存
- [ ] 添加自定义主题编辑器
- [ ] 性能优化
- [ ] 动画效果

---

## 🎯 实施策略

### 选项 A: 一次性全量实施 (推荐快速项目)

```
Week 1: 基础设置 + 控件改进
Week 2: 主题系统 + 测试
Week 3: 文档 + 发布
```

优点: 快速看到效果, 一次性完成  
缺点: 风险集中

### 选项 B: 渐进式实施 (推荐大型项目)

```
Phase 1: 基础设置 + 字体系统
Phase 2: 颜色系统 + 间距
Phase 3: 控件改进
Phase 4: 主题系统
Phase 5: 优化 + 文档
```

优点: 风险分散, 可逐步验证  
缺点: 时间较长

---

## 🔍 检查清单

### 代码审查

```cpp
// ✅ 所有颜色来自 UIDesign::Colors
// ❌ RGB(x, y, z) 硬编码

// ✅ 所有字体来自 UICommon::GetFont()
// ❌ 本地 CFont 创建

// ✅ 所有间距来自 UIDesign::Spacing
// ❌ 硬编码的像素值

// ✅ 所有控件高度来自 ComponentSize
// ❌ 任意高度值

// ✅ 所有尺寸都是 8px 的倍数
// ❌ 奇怪的尺寸
```

### 视觉审查

```
[] 字体层级正确
[] 颜色搭配和谐
[] 间距规范一致
[] 对齐完美
[] 圆角统一
[] 边框统一
[] 阴影一致 (如使用)
[] 整体现代化
```

### 无障碍审查

```
[] 颜色对比度 ≥ 4.5:1
[] 所有元素 ≥ 44px
[] 焦点可见且清晰
[] 键盘可完全操作
[] 屏幕阅读器友好
[] 支持文本缩放
[] 不依赖颜色单独传信息
[] 动画可禁用
```

---

## 📞 常见问题

### Q1: 如何快速迁移现有代码?

```cpp
// 批量替换
RGB(50, 120, 255)        → UIDesign::Colors::Dark::PRIMARY
RGB(250, 250, 250)       → UIDesign::Colors::Dark::TEXT_PRIMARY
8, 16, 24 (间距)         → Spacing::S, ::M, ::L
13, 18 (字体)            → FontSize::BODY, ::HEADLINE
```

### Q2: 如何支持用户自定义主题?

```cpp
struct CustomTheme {
    COLORREF primary;
    COLORREF surface;
    COLORREF textPrimary;
    // ... 其他颜色
};

// 保存到配置文件
// 加载时应用自定义颜色
```

### Q3: 如何添加新的控件?

```cpp
class MyCustomControl : public CUIPanelItem {
    void Draw(CDC* pDC, const CRect& rect, bool bHover) override {
        // 使用 UIDesign::* 的所有常量
        pDC->SelectObject(UICommon::GetFont(...));
        pDC->SetTextColor(UIDesign::Colors::Dark::TEXT_PRIMARY);
        rect.DeflateRect(UIDesign::Spacing::M, ...);
    }
    
    int GetHeight() const override {
        return UIDesign::ComponentSize::Height::MEDIUM;
    }
};
```

### Q4: 性能会受影响吗?

```
字体缓存: 已优化, 无额外开销
颜色查询: O(1) 常数时间
间距常量: 编译时内联, 零开销
```

### Q5: 如何测试无障碍?

```bash
# 使用 Windows 内置工具
# 1. Inspect (检查对比度、大小)
# 2. Narrator (屏幕阅读器)
# 3. 键盘测试 (Tab 导航)
# 4. 放大镜 (高对比度)
```

---

## 📚 相关文档

- [DESIGN_SYSTEM.md](DESIGN_SYSTEM.md) - 完整规范
- [BEFORE_AFTER_COMPARISON.md](BEFORE_AFTER_COMPARISON.md) - 对比分析
- [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - 快速参考

---

**版本**: 1.0  
**最后更新**: 2024  
**状态**: ✅ 完成  
**难度**: 中等  
**预计时间**: 2-4 周
