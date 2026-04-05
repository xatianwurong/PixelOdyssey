# 🎯 UI 设计系统快速参考卡

> **随身携带的设计规范** | 开发时即时查询

---

## 📐 间距 (Spacing)

```cpp
XS  = 4px
S   = 8px
M   = 16px   ⭐ 最常用
L   = 24px
XL  = 32px
XXL = 48px

// 使用
rect.DeflateRect(M, M);
y += itemHeight + S;
```

---

## 🔤 字体 (Font Sizes)

| 用途 | 大小 | 粗细 | 代码 |
|------|------|------|------|
| Display | 28pt | Bold | `DISPLAY` |
| Headline | 18pt | SemiBold | `HEADLINE` |
| Subtitle | 15pt | Medium | `SUBTITLE` |
| Body (正文) | **13pt** | Normal | `BODY` ⭐ |
| Body Small | 12pt | Normal | `BODY_SMALL` |
| Caption | 11pt | Normal | `CAPTION` |

```cpp
// 使用
pDC->SelectObject(UICommon::GetBodyFont());  // 13pt
pDC->SelectObject(UICommon::GetFont(
    Typography::FontSize::HEADLINE,
    Typography::FontWeight::SEMIBOLD));
```

---

## 🎨 颜色 (Colors)

### 深色主题 (推荐)

| 用途 | 色值 | RGB |
|------|------|-----|
| Primary | `#3278FF` | 50, 120, 255 |
| Background | `#191919` | 25, 25, 25 |
| Surface | `#252525` | 37, 37, 37 |
| Text Primary | `#FAFAFA` | 250, 250, 250 |
| Text Secondary | `#B4B4B4` | 180, 180, 180 |
| Border | `#323232` | 50, 50, 50 |
| Success | `#4CAF50` | 76, 175, 80 |
| Warning | `#FF9800` | 255, 152, 0 |
| Error | `#F44336` | 244, 67, 54 |

```cpp
// 使用
pDC->SetTextColor(Colors::Dark::TEXT_PRIMARY);
CBrush brush(Colors::Dark::PRIMARY);
CPen pen(PS_SOLID, 1, Colors::Dark::BORDER);
```

---

## 🎛️ 控件高度 (Component Heights)

| 控件 | 高度 | 用途 |
|------|------|------|
| Button | **44px** | 标准按钮 ⭐ |
| List Item | **44px** | 列表项 ⭐ |
| Input | 44px | 文本输入框 |
| Compact | 32px | 紧凑布局 |
| Large | 52px | 强调操作 |

```cpp
// 使用
int height = ComponentSize::Height::MEDIUM;  // 44px
```

---

## 📏 内边距 (Padding)

```cpp
// 水平
left   = M;    // 16px
right  = M;    // 16px

// 竖直
top    = S;    // 8px
bottom = S;    // 8px

// 一次性应用
rect.DeflateRect(M, S);  // H=16, V=8
```

---

## 📦 控件规范速查表

### 按钮

```
高度:     44px
最小宽:   80px
内边距:   16px (H) × 8px (V)
圆角:     8px
边框:     1px solid
```

### 列表项

```
高度:     44px
内边距:   16px (H) × 8px (V)
间距:     8px
悬停:     bg + 8% white
```

### 卡片/面板

```
内边距:   16px
边框:     1px solid
圆角:     8px
阴影:     可选
```

---

## 🔄 状态颜色 (State Colors)

```
DEFAULT    → 原色
HOVER      → bg + 8% white
ACTIVE     → bg + 12% black
FOCUSED    → 2px Primary 焦点环
DISABLED   → 文字变淡 (38% 透明度)
ERROR      → #F44336 (红色)
SUCCESS    → #4CAF50 (绿色)
WARNING    → #FF9800 (橙色)
```

---

## 🖼️ 颜色应用快速表

```cpp
// 背景色
CBrush bg(Colors::Dark::BACKGROUND);

// 表面色 (卡片、面板)
CBrush surface(Colors::Dark::SURFACE);

// 主色 (按钮、链接)
CBrush primary(Colors::Dark::PRIMARY);

// 文字
pDC->SetTextColor(Colors::Dark::TEXT_PRIMARY);        // 主
pDC->SetTextColor(Colors::Dark::TEXT_SECONDARY);      // 辅
pDC->SetTextColor(Colors::Dark::TEXT_DISABLED);       // 禁用

// 边框
CPen border(PS_SOLID, 1, Colors::Dark::BORDER);
```

---

## ♿ 无障碍速查 (A11y Checklist)

```
□ 颜色对比度 ≥ 4.5:1
□ 文字大小 ≥ 11pt
□ 控件大小 ≥ 44px × 44px
□ 焦点可见且清晰
□ 键盘可完全操作
□ 支持文本缩放
□ 非色值单独传信息
```

---

## 🚫 常见错误

```cpp
// ❌ 不要硬编码颜色
pDC->SetTextColor(RGB(250, 250, 250));

// ✅ 使用设计系统
pDC->SetTextColor(Colors::Dark::TEXT_PRIMARY);

// ❌ 不要任意字体
CFont font; font.CreateFontIndirect(&lf);

// ✅ 使用 UICommon
pDC->SelectObject(UICommon::GetBodyFont());

// ❌ 不要随意间距
rect.DeflateRect(12, 10);

// ✅ 使用规范间距
rect.DeflateRect(M, S);
```

---

## 📋 控件高度对照

```
改进前              改进后 (WCAG)
Label:      28px   →   36px
Button:     38px   →   44px ⭐
List Item:  32px   →   44px ⭐
Separator:  16px   →   24px
```

---

## 🎨 主题快速切换

```cpp
// 深色
Colors::Dark::*

// 浅色
Colors::Light::*

// 高对比
Colors::HighContrast::*
```

---

## 💾 常用代码片段

### 绘制按钮

```cpp
COLORREF bgColor = isActive ? Colors::Dark::PRIMARY 
                            : Colors::Dark::SURFACE;
CBrush brush(bgColor);
pDC->FillRect(&rect, &brush);

CPen pen(PS_SOLID, 1, Colors::Dark::BORDER);
pDC->SelectObject(&pen);
pDC->RoundRect(&rect, CPoint(BORDER_RADIUS, BORDER_RADIUS));

pDC->SetTextColor(Colors::Dark::TEXT_PRIMARY);
pDC->SelectObject(UICommon::GetBodyFont());
pDC->DrawText(text, &rect, DT_CENTER | DT_VCENTER);
```

### 绘制列表项

```cpp
if (bHover) {
    COLORREF hoverColor = BlendColor(Colors::Dark::SURFACE, 
                                     RGB(255,255,255), 0.08);
    CBrush brush(hoverColor);
    pDC->FillRect(&rect, &brush);
}

pDC->SetTextColor(Colors::Dark::TEXT_PRIMARY);
pDC->SelectObject(UICommon::GetFont(
    FontSize::BODY, FontWeight::NORMAL));
```

### 计算间距

```cpp
CRect contentRect = rect;
contentRect.DeflateRect(M, M);  // 16px all sides

int y = contentRect.top;
for (auto& item : items) {
    int itemHeight = item->GetHeight();
    CRect itemRect(contentRect.left, y, 
                  contentRect.right, y + itemHeight);
    item->Draw(pDC, itemRect, bHover);
    y += itemHeight + S;  // 8px spacing
}
```

---

## 🔗 相关文档

| 文档 | 用途 |
|------|------|
| **DESIGN_SYSTEM.md** | 完整规范 (必读) |
| **IMPLEMENTATION_GUIDE.md** | 实施步骤 |
| **BEFORE_AFTER_COMPARISON.md** | 改进对比 |
| **UIBestPractices.md** | 最佳实践 |

---

## 📞 快速检查清单

使用此清单验证你的 UI 代码:

```
字体:
□ 使用 UICommon::GetFont() 或 UICommon::Get*Font()
□ 所有字体 ≥ 11pt
□ 标题使用相应大小 (18, 28 等)

颜色:
□ 使用 Colors::Dark::* (或其他主题)
□ 没有硬编码的 RGB()
□ 对比度 ≥ 4.5:1

间距:
□ 使用 Spacing::* (S, M, L 等)
□ 间距都是 8px 倍数
□ 内边距统一 (16px 水平, 8px 竖直)

控件:
□ 高度 ≥ 44px
□ 宽度 ≥ 44px (可点击元素)
□ 圆角 = 8px (如需要)
□ 边框 = 1px

状态:
□ 有 hover 效果
□ 有 active 效果
□ 禁用时变淡
□ 焦点环清晰

无障碍:
□ 所有元素可键盘操作
□ 焦点可见
□ 颜色对比度满足
□ 文字可缩放
```

---

## 🎓 学习路径

1. **5分钟**: 阅读本文档
2. **15分钟**: 浏览 DESIGN_SYSTEM.md
3. **30分钟**: 学习 IMPLEMENTATION_GUIDE.md
4. **开发中**: 频繁查看此速查表

---

**版本**: 1.0  
**最后更新**: 2024  
**格式**: 便携式参考卡  
**大小**: 1 页打印版本
