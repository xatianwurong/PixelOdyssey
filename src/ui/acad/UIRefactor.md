/**
 * @file UIRefactor.md
 * @brief PixelOdyssey UI 模块重构指南
 * @details 本文档总结了 UI 模块的重构改动、设计原则和最佳实践
 */

# PixelOdyssey UI 模块重构总结

## 📋 概述

本次重构重点解决以下问题：
- ✅ 字体乱码问题（工具栏 Unicode 符号显示错误）
- ✅ 代码重复（多处字体初始化逻辑）
- ✅ 样式不一致（间距/颜色/字体参数散落各地）
- ✅ 缺乏 Tooltip 支持
- ✅ 图标渲染不稳定

## 🎨 核心设计变更

### 1. **统一的字体管理系统** (`UICommon.h`)

```cpp
UICommon::GetBodyFont()        // 正文字体
UICommon::GetTitleFont()       // 标题字体
UICommon::GetSymbolFont()      // 符号字体（工具栏用）
UICommon::GetMonospaceFont()   // 等宽字体（命令行用）
UICommon::GetChineeseFont()    // 中文后备字体
```

**优势**：
- 集中管理，减少重复代码
- 字体对象自动缓存，避免频繁创建
- 便于全局切换字体配置

### 2. **语义化的布局和间距常量** (`UILayout.h`)

新增符号字体配置：
```cpp
namespace UIFonts {
    constexpr const TCHAR* SYMBOL_FONT = _T("Segoe MDL2 Assets");
    constexpr const TCHAR* FALLBACK_FONT = _T("SimSun");
}
```

### 3. **改进的颜色方案** (`ColorScheme.h`)

已有的 15 种语义化颜色角色保持不变，支持 3 种主题：
- **Dark（默认）**：深色主题，类似 Visual Studio Dark
- **Light**：浅色主题
- **HighContrast**：高对比度，提高可访问性

### 4. **统一的工具栏绘制系统** (`UIToolbar.h/cpp`)

新增两个绘制辅助函数：

#### `DrawSimpleIcon()` - 几何形状绘制
不再使用 Unicode 字符，而是用基础几何形状绘制 5 种工具图标：
- **0**: 选择工具 - 箭头向上
- **1**: 移动工具 - 十字箭头
- **2**: 旋转工具 - 圆形箭头
- **3**: 缩放工具 - 双向箭头
- **4**: 平移工具 - 手掌形

#### `DrawButton()` - 按钮绘制
统一的按钮绘制逻辑，支持 4 种状态：
```cpp
enum ButtonState { Normal, Hovered, Active, Disabled }
```

#### `DrawTooltip()` - 提示框绘制
显示工具的中文标签和快捷键提示

### 5. **改进的 UI 基类** (`AcadUIElement.h/cpp`)

- 统一的字体初始化逻辑
- 三阶段绘制管线：背景 → 内容 → 边框
- 支持主题切换和可见性控制

### 6. **改进的工具栏** (`AcadToolPalette.h/cpp`)

**变更**：
- 用几何形状代替 Unicode 字符
- 新增 Tooltip 支持（悬停时显示工具名称）
- 改用中文标签（"选择", "移动", "旋转", "缩放", "平移"）
- 简化图标类型定义（iconType: int 替代 iconChar: int）

**新按钮布局**：
- 上半部分：大图标（几何形状）
- 下半部分：中文标签（如 "选择"）
- 悬停时：显示完整标签 + 快捷键（如 "选择 (V)"）

## 📁 文件结构

```
src/ui/acad/
├── UIComponents.h      ← UI 基础控件 (CUILabel, CUIButton, CUIPropertyItem)
├── UIComponents.cpp
├── UICommon.h          ← 字体管理系统 [新增]
├── UILayout.h          ← 布局和间距常量 (更新：新增字体配置)
├── UIToolbar.h         ← 工具栏绘制辅助 [新增]
├── UIToolbar.cpp
├── AcadUIElement.h     ← UI 基类
├── AcadUIElement.cpp
├── AcadToolPalette.h   ← 工具面板
├── AcadToolPalette.cpp
└── ... (其他文件)

src/ui/core/
├── ColorScheme.h       ← 颜色方案 (3 种主题 × 15 种角色)
└── ColorScheme.cpp
```

## 🚀 使用示例

### 例 1: 在自定义绘制中使用统一字体

**之前**（重复代码）：
```cpp
CFont font;
LOGFONT lf = {};
lf.lfHeight = -13;
lf.lfWeight = FW_NORMAL;
_tcscpy_s(lf.lfFaceName, _T("Microsoft YaHei UI"));
font.CreateFontIndirect(&lf);
pDC->SelectObject(&font);
pDC->DrawText(text, &rect, DT_LEFT | DT_VCENTER);
```

**之后**（统一管理）：
```cpp
pDC->SelectObject(UICommon::GetBodyFont());
pDC->DrawText(text, &rect, DT_LEFT | DT_VCENTER);
```

### 例 2: 使用新的工具栏绘制

```cpp
// 绘制工具按钮
UIToolbar::DrawButton(pDC, buttonRect, 
                      UIToolbar::ButtonState::Active, 
                      0,  // 选择工具图标
                      bgColor, borderColor);

// 绘制 Tooltip
UIToolbar::DrawTooltip(pDC, tooltipRect, _T("选择 (V)"), bgColor, textColor);
```

### 例 3: 全局切换主题

```cpp
auto& colorScheme = ColorScheme::Instance();
colorScheme.SetTheme(ColorScheme::Theme::Light);  // 切换到浅色
```

## 🎯 最佳实践

### 字体选择
- **文本**: `UICommon::GetBodyFont()` 或 `UICommon::GetChineeseFont()`
- **标题**: `UICommon::GetTitleFont()`
- **命令行**: `UICommon::GetMonospaceFont()`
- **工具栏图标**: 使用 `UIToolbar::DrawSimpleIcon()` 而非字体

### 颜色选择
使用语义化颜色角色而非硬编码 RGB 值：
```cpp
auto& colors = ColorScheme::Instance();
COLORREF textColor = colors.GetColor(ColorScheme::ColorRole::TextPrimary);
```

### 间距和尺寸
使用 `UILayout` 和 `UISpacing` 命名空间中的常量：
```cpp
constexpr int ITEM_SPACING = UISpacing::M;  // 使用命名常量
rect.DeflateRect(UISpacing::S, 0);          // 代替硬编码的 8
```

### 添加新控件
1. 继承 `CUIPanelItem` 实现 `Draw()`, `GetHeight()`, `OnClick()`
2. 在 `Draw()` 中使用 `UICommon::Get*Font()` 获取字体
3. 使用 `ColorScheme::Instance()` 获取颜色
4. 通过 `CUnifiedPanel::AddItem()` 添加到面板

## ✨ 改进亮点

| 改进项 | 前 | 后 |
|-----|----|----|
| 工具栏乱码 | Unicode 符号 | 几何形状 |
| 字体管理 | 多处重复初始化 | 集中在 UICommon |
| Tooltip | 无 | 完整支持 |
| 代码行数 | ~200 行（重复） | ~150 行（统一） |
| 样式一致性 | 中等 | 高（所有参数集中） |

## 🔮 未来规划

1. **动画支持**：为 Hover/Active 状态添加平滑过渡
2. **DPI 缩放**：自动适配高 DPI 屏幕
3. **主题编辑器**：允许用户自定义颜色方案
4. **Keyboard 导航**：支持键盘快捷键操作
5. **暗模式自适应**：根据系统主题自动切换
6. **更多控件**：文本框、下拉框、数值微调器等

## 🐛 已知问题与解决方案

| 问题 | 原因 | 解决方案 |
|------|------|--------|
| 字体缓存内存泄漏 | 使用 `new CFont()` | 可选：在程序退出时显式释放或使用智能指针容器 |
| 超高 DPI 下模糊 | 未缩放字体大小 | 未来支持：根据系统 DPI 调整 `UILayout::UIFonts::*_SIZE` |
| 中文超长标签截断 | 矩形空间有限 | 目前：截断显示；未来：支持多行 Tooltip |

## 📚 参考资源

- **Windows 内置字体**：https://learn.microsoft.com/en-us/typography/fonts/font-list
- **Segoe MDL2 Icons**：https://learn.microsoft.com/en-us/windows/apps/design/style/segoe-ui-symbol-font
- **MFC 绘制指南**：https://learn.microsoft.com/en-us/cpp/mfc/drawing-in-a-view

---

**最后更新**: 2024
**维护者**: GitHub Copilot
**状态**: ✅ 完成初步重构
