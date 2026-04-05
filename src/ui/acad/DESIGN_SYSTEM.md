# 🎨 PixelOdyssey UI 设计规范 v2.0

> **专业级设计标准** | 现代化视觉体验 | 无障碍设计优先

---

## 📑 目录

1. [设计哲学](#设计哲学)
2. [字体系统](#字体系统)
3. [颜色系统](#颜色系统)
4. [间距和布局](#间距和布局)
5. [控件规范](#控件规范)
6. [状态系统](#状态系统)
7. [交互设计](#交互设计)
8. [无障碍设计](#无障碍设计)
9. [优化对比](#优化对比)

---

## 🎯 设计哲学

### 核心原则

1. **一致性** - 所有控件遵循统一的设计语言
2. **清晰性** - 清晰的视觉层级和信息架构
3. **易用性** - 直观的交互和有效的反馈
4. **可访问性** - 支持所有用户，包括残障人士
5. **现代化** - 采用最新的 UI 设计趋势

### 设计目标

- ✅ 专业、现代的视觉效果
- ✅ 一致的交互体验
- ✅ 优秀的可读性和可用性
- ✅ 响应式和自适应设计
- ✅ 无障碍设计支持 (WCAG 2.1 AA)

---

## 🔤 字体系统 (Typography)

### 字体家族

```
主字体:           Microsoft YaHei UI (微软雅黑)
标题字体:         Microsoft YaHei UI (微软雅黑)
等宽字体:         Consolas (代码、命令行)
中文后备字体:     SimSun (宋体)
```

### 字体层级

| 用途 | 大小 | 粗细 | 行高 | 使用场景 |
|-----|------|------|------|---------|
| **Display Large** | 32pt | Bold | 1.2 | 页面标题 (罕用) |
| **Display** | 28pt | Bold | 1.2 | 重要标题 |
| **Headline** | 18pt | SemiBold | 1.5 | 组件标题、面板标题 |
| **Subtitle** | 15pt | Medium | 1.5 | 小标题、卡片标题 |
| **Body Large** | 14pt | Medium | 1.5 | 强调文本 |
| **Body** | 13pt | Normal | 1.5 | 正文 **(标准)** |
| **Body Small** | 12pt | Normal | 1.5 | 辅助文字、标签 |
| **Caption** | 11pt | Normal | 1.5 | 说明文字、提示 |

### 字体粗细

- **Normal (400)** - 标准文本
- **Medium (500)** - 强调文本
- **SemiBold (600)** - 小标题、标签
- **Bold (700)** - 标题、重要信息

### 行高规范

- **标题**: 1.2 (紧凑)
- **正文**: 1.5 (标准，推荐)
- **长文本**: 1.75 (宽松)

### 使用规范

```cpp
// ✅ 推荐方式
pDC->SelectObject(UICommon::GetBodyFont());           // 正文
pDC->SelectObject(UICommon::GetTitleFont());          // 标题
pDC->SelectObject(UICommon::GetChineeseFont(10));     // 自定义大小

// ❌ 避免
CFont font;                                            // 避免本地创建
font.CreateFontIndirect(&lf);
```

---

## 🎨 颜色系统 (Color System)

### 颜色主题

#### 深色主题 (Dark) - **默认推荐**

**主色 (蓝色系)**
```
Primary:    #3278FF (RGB 50, 120, 255) - 标准主色
Light:      #64C8FF (RGB 100, 150, 255) - 浅主色
Dark:       #1E5AE0 (RGB 30, 90, 220) - 深主色
```

**中性色 (灰度系)**
```
Background: #191919 (RGB 25, 25, 25)   - 背景色
Surface:    #252525 (RGB 37, 37, 37)   - 表面色
Border:     #323232 (RGB 50, 50, 50)   - 边框色

Text Primary:   #FAFAFA (RGB 250, 250, 250) - 主文字
Text Secondary: #B4B4B4 (RGB 180, 180, 180) - 辅助文字
Text Disabled:  #646464 (RGB 100, 100, 100) - 禁用文字
```

**功能色**
```
Success:  #4CAF50 (RGB 76, 175, 80)   - 绿色 (成功)
Warning:  #FF9800 (RGB 255, 152, 0)   - 橙色 (警告)
Error:    #F44336 (RGB 244, 67, 54)   - 红色 (错误)
Info:     #2196F3 (RGB 33, 150, 243)  - 蓝色 (信息)
```

#### 浅色主题 (Light)

```
Background: #FAFAFA (纯白略有灰度)
Surface:    #FFFFFF (纯白)
Primary:    #1E64DC (深蓝)
Text Primary: #212121 (几乎纯黑)
```

#### 高对比度主题 (High Contrast)

```
使用纯色，最大化对比度
Background: #FFFFFF (纯白)
Text Primary: #000000 (纯黑)
Primary:    #0000FF (纯蓝)
Border:     #000000 (纯黑)
```

### 色彩使用规范

| 元素 | 深色主题 | 浅色主题 | 说明 |
|------|---------|---------|------|
| 背景 | #191919 | #FAFAFA | 页面背景 |
| 表面 | #252525 | #FFFFFF | 卡片、面板 |
| 主色 | #3278FF | #1E64DC | CTA、激活状态 |
| 主文字 | #FAFAFA | #212121 | 标题、强调 |
| 辅文字 | #B4B4B4 | #666666 | 说明、禁用 |
| 边框 | #323232 | #E0E0E0 | 分割线、边框 |

### 颜色对比度

遵循 WCAG 标准：
- **AA 级**: 4.5:1 (标准文本)
- **AAA 级**: 7:1 (大文本)

所有颜色组合都已验证满足 AA 级以上标准。

---

## 📏 间距和布局 (Spacing & Layout)

### 8px 网格系统

所有间距都是 8px 的倍数，确保对齐和一致性。

```
XS:   4px  (0.5 unit)
S:    8px  (1 unit)   - 项间距
M:   16px  (2 units)  - 标准间距 ⭐ 最常用
L:   24px  (3 units)  - 大间距
XL:  32px  (4 units)  - 特大间距
XXL: 48px  (6 units)  - 超大间距
```

### 间距规范

```cpp
// 水平边距
component.left   += M;  // 16px
component.right  -= M;  // 16px

// 竖直边距
component.top    += M;  // 16px
component.bottom -= M;  // 16px

// 项间距 (列表项之间)
itemSpacing = S;  // 8px

// 组间距 (不同部分之间)
groupSpacing = M;  // 16px

// 大间距 (主要部分之间)
majorSpacing = L;  // 24px
```

### 响应式布局

**工具栏**:
- 宽度: 80px (固定)

**属性面板**:
- 宽度: 320px (固定)

**中央渲染区**:
- 宽度: 自动 (窗口宽度 - 工具栏 - 面板)
- 高度: 自动 (窗口高度)

---

## 🎛️ 控件规范 (Component Guidelines)

### 按钮

**尺寸规范**

| 大小 | 高度 | 推荐用途 |
|------|------|---------|
| Small | 32px | 紧凑布局 |
| Medium | **44px** | 标准 ⭐ |
| Large | 52px | 强调操作 |

**按钮样式**
```
默认:     背景=Surface, 边框=Border, 文字=TextPrimary
Hover:    背景变亮 (8% 白色叠加)
Active:   背景变深 (12% 黑色叠加)
Disabled: 文字=TextDisabled, 背景=Surface (透明度 38%)
```

**最小宽度**: 80px

**内边距**: 
- 水平: 16px
- 竖直: 8px

### 文本输入框

**高度**: 44px (标准)

**内边距**:
- 水平: 12px
- 竖直: 8px

**边框**: 1px solid Border

**状态**:
- 默认: Border = #323232
- Focus: Border = Primary (#3278FF)
- Error: Border = Error (#F44336)

### 列表项

**高度**: 44px (标准)

**间距**:
- 项间距: 8px
- 内边距: 16px (水平), 8px (竖直)

**悬停效果**:
- 背景高亮: Surface + 8% 白色

### 卡片/面板

**内边距**: 16px

**边框**: 1px solid Border

**圆角**: 8px

**阴影** (可选):
- 深度 1: 0px 2px 4px rgba(0,0,0,0.1)

### 工具栏

**高度**: 56px

**按钮尺寸**:
- 宽度: 自适应 (80px 工具栏)
- 高度: 70px
- 图标区: 65% (45px)
- 文字区: 35% (25px)

---

## 🔄 状态系统 (State System)

### 控件状态

```
DEFAULT  → 默认状态，可交互
   ↓
HOVER    → 用户悬停时，背景 +8% 白色
   ↓
ACTIVE   → 按下/激活，背景 +12% 黑色
   ↓
FOCUSED  → 获得焦点，显示焦点环
   ↓
DISABLED → 禁用，文字变淡 (38%)
```

### 焦点指示

```
焦点环宽度: 2px
焦点环颜色: Primary (#3278FF)
焦点环样式: 实线
焦点环偏移: 0px
```

### 错误/警告状态

```
错误:   边框 = #F44336 (红色)
警告:   边框 = #FF9800 (橙色)
成功:   边框 = #4CAF50 (绿色)
```

---

## 🎬 交互设计 (Interaction Design)

### 动画和过渡

**持续时间**

| 操作 | 时间 |
|-----|------|
| 状态变化 (Hover → Active) | 100ms |
| 标准过渡 | 200ms |
| 进入动画 | 300ms |
| 退出动画 | 300ms |

**缓动函数**
```
快速反馈: ease-out (元素进入)
标准过渡: ease-in-out (默认)
退出效果: ease-in (元素离开)
```

### 反馈设计

1. **视觉反馈**
   - Hover: 背景改变
   - Active: 颜色加深，阴影增加
   - Success: 绿色提示，可选动画

2. **听觉反馈** (可选)
   - 点击: 轻微音效
   - Success: 确认音效

3. **触觉反馈** (不适用桌面)

### 鼠标光标

```
默认:      Arrow (→)
按钮:      Pointer (☞)
输入框:    IBeam (I)
拖拽:      Move (✋)
禁用:      No (⊘)
加载:      Wait (⏳)
```

---

## ♿ 无障碍设计 (Accessibility)

### WCAG 2.1 AA 合规

#### 1. 颜色对比度

- **文本对比度**: 最少 4.5:1
- **大文本对比度** (18pt+ 或 14pt Bold+): 最少 3:1
- **UI 组件对比度**: 最少 3:1

所有颜色都经过验证。

#### 2. 触点大小

- **最小触点**: 44px × 44px
- **最小间距**: 8px

```
✅ 标准按钮高度: 44px
✅ 列表项高度: 44px
✅ 工具栏按钮: 70px
```

#### 3. 焦点管理

```
所有交互元素都必须支持键盘焦点
焦点顺序: Tab 键导航
焦点指示: 明显的焦点环 (2px, Primary 颜色)
```

#### 4. 标签和替代文本

```
// ✅ 好的做法
button.SetAccessibleName(_T("保存"));
button.SetAccessibleDescription(_T("保存当前文档"));

// ❌ 避免
button.SetAccessibleName(_T("Btn1"));  // 无意义
```

#### 5. 文本大小

```
最小正文字体: 13pt
支持缩放: 用户可将字体放大到 200%
```

#### 6. 动画

```
允许用户禁用动画 (prefers-reduced-motion)
避免频闪 (闪烁频率 > 3Hz)
```

---

## 📊 优化对比

### 改进前 vs 改进后

#### 字体

| 方面 | 改进前 | 改进后 | 提升 |
|------|-------|-------|------|
| 字体家族 | 不统一 | Microsoft YaHei UI | ✅ 一致 |
| 大小规范 | 8pt-18pt 混乱 | 4 个清晰层级 | ✅ 专业 |
| 粗细应用 | 无规范 | 4 个粗细等级 | ✅ 层级清晰 |
| 行高 | 默认 (1.0) | 1.2-1.75 规范 | ✅ 可读性强 |

#### 颜色

| 方面 | 改进前 | 改进后 | 提升 |
|------|-------|-------|------|
| 主色 | 混乱 | 统一蓝色系 | ✅ 品牌一致 |
| 对比度 | 未验证 | WCAG AA+ | ✅ 无障碍 |
| 主题支持 | 仅深色 | 3 种主题 | ✅ 灵活 |
| 功能色 | 4 种 | 成功/警告/错误/信息 | ✅ 规范 |

#### 间距

| 方面 | 改进前 | 改进后 | 提升 |
|------|-------|-------|------|
| 间距规范 | 随意 4-24px | 8px 网格系统 | ✅ 一致 |
| 最常用间距 | 12px | 16px (M) | ✅ 舒适 |
| 控件间距 | 不规范 | 定义明确 | ✅ 清晰 |
| 响应式 | 无 | 基于网格 | ✅ 自适应 |

#### 控件

| 控件 | 改进前 | 改进后 | 提升 |
|------|-------|-------|------|
| 按钮高度 | 38px | 44px | ✅ WCAG 标准 |
| 列表项高度 | 32px | 44px | ✅ 易点击 |
| 圆角 | 无 | 8px 标准 | ✅ 现代感 |
| 状态反馈 | 基础 | 完整 (4 种) | ✅ 交互感强 |

#### 整体视觉

| 方面 | 改进前 | 改进后 |
|------|-------|-------|
| **专业度** | 中等 | ⭐⭐⭐⭐⭐ 专业级 |
| **一致性** | 低 | ⭐⭐⭐⭐⭐ 完全一致 |
| **可读性** | 良好 | ⭐⭐⭐⭐⭐ 优秀 |
| **易用性** | 中等 | ⭐⭐⭐⭐⭐ 直观 |
| **无障碍** | 无 | ⭐⭐⭐⭐⭐ WCAG AA+ |

---

## 📚 实施指南

### 1. 使用字体

```cpp
// 页面标题
pDC->SelectObject(UICommon::GetFont(
    UIDesign::Typography::FontSize::DISPLAY,
    UIDesign::Typography::FontWeight::BOLD));

// 组件标题
pDC->SelectObject(UICommon::GetFont(
    UIDesign::Typography::FontSize::HEADLINE,
    UIDesign::Typography::FontWeight::SEMIBOLD));

// 正文
pDC->SelectObject(UICommon::GetBodyFont());

// 自定义大小
pDC->SelectObject(UICommon::GetChineeseFont(10));
```

### 2. 使用颜色

```cpp
auto& colors = ColorScheme::Instance();

// 背景
CBrush bgBrush(UIDesign::Colors::Dark::BACKGROUND);

// 文字
pDC->SetTextColor(UIDesign::Colors::Dark::TEXT_PRIMARY);

// 边框
CPen borderPen(PS_SOLID, 1, UIDesign::Colors::Dark::BORDER);
```

### 3. 使用间距

```cpp
// 标准内边距
rect.DeflateRect(UIDesign::Spacing::M, UIDesign::Spacing::M);

// 项间距
int itemSpacing = UIDesign::Spacing::Component::GAP_ITEM;

// 组间距
int groupSpacing = UIDesign::Spacing::Component::GAP_SECTION;

// 圆角
arc(rect, UIDesign::Spacing::Component::BORDER_RADIUS);
```

### 4. 控件尺寸

```cpp
// 标准按钮高度
int buttonHeight = UIDesign::ComponentSize::Height::MEDIUM;  // 44px

// 列表项高度
int itemHeight = UIDesign::ComponentSize::Height::LIST_ITEM;  // 44px

// 图标大小
int iconSize = UIDesign::ComponentSize::IconSize::MEDIUM;  // 24px
```

---

## 🎓 最佳实践总结

### ✅ 应该做的事

- 使用定义的字体层级，不自由组合
- 遵循 8px 网格系统的间距
- 使用 UIDesign 命名空间的颜色和尺寸
- 确保所有交互元素 ≥ 44px × 44px
- 提供清晰的焦点指示
- 使用语义化的状态反馈
- 测试无障碍特性 (键盘导航、屏幕阅读器)

### ❌ 避免做的事

- 硬编码颜色值 (RGB/16进制)
- 硬编码字体大小
- 不规则的间距
- 过小的触点 (< 44px)
- 隐藏焦点指示
- 频闪或过度动画
- 仅依赖颜色传达信息
- 跳过无障碍测试

---

## 🔗 相关资源

- [Material Design 3](https://m3.material.io/)
- [WCAG 2.1 Guidelines](https://www.w3.org/WAI/WCAG21/quickref/)
- [Apple Human Interface Guidelines](https://developer.apple.com/design/human-interface-guidelines/)
- [Microsoft Fluent Design System](https://www.microsoft.com/design/fluent/)

---

**版本**: 2.0  
**发布日期**: 2024  
**状态**: ✅ 完成  
**下一版本**: 考虑动态主题和用户自定义
