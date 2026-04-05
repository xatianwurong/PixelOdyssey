#pragma once

#include <afxwin.h>

/**
 * @file UIDesignSystem.h
 * @brief PixelOdyssey 设计规范系统
 * @details 统一的设计系统，包含字体、颜色、间距、控件规范等
 * 
 * 设计原则：
 * - 8px 网格系统
 * - 4 个字体层级
 * - 3 个控件尺寸（小、中、大）
 * - 响应式和无障碍设计
 */

namespace UIDesign {

// ============================================================================
// 第一部分：字体系统（Typography System）
// ============================================================================

/**
 * @brief 字体层级定义
 * @details 基于设计系统的标准字体规范
 */
namespace Typography {
    
    // 字体家族定义
    namespace FontFamily {
        constexpr const TCHAR* PRIMARY = _T("Microsoft YaHei UI");     // 主字体 (中英文)
        constexpr const TCHAR* HEADING = _T("Microsoft YaHei UI");     // 标题字体
        constexpr const TCHAR* MONOSPACE = _T("Consolas");             // 等宽字体
        constexpr const TCHAR* FALLBACK = _T("SimSun");                // 中文后备
    }

    // 字体尺寸 (pt)
    namespace FontSize {
        // 主文本层级
        constexpr int DISPLAY_LARGE = 32;    // 超大标题 (不常用)
        constexpr int DISPLAY = 28;          // 大标题 (页面标题)
        constexpr int HEADLINE = 18;         // 标题 (组件标题)
        constexpr int SUBTITLE = 15;         // 副标题 (小标题)
        constexpr int BODY_LARGE = 14;       // 大正文 (强调)
        constexpr int BODY = 13;             // 正文 (标准)
        constexpr int BODY_SMALL = 12;       // 小正文 (辅助文字)
        constexpr int CAPTION = 11;          // 说明 (非常小)
        
        // 特殊用途
        constexpr int LABEL = 12;            // 标签
        constexpr int HINT = 11;             // 提示
        constexpr int CODE = 11;             // 代码块
    }

    // 字体粗细 (Weight)
    namespace FontWeight {
        constexpr int THIN = 100;            // 极细 (不推荐)
        constexpr int LIGHT = 300;           // 细体
        constexpr int NORMAL = 400;          // 常规 (默认)
        constexpr int MEDIUM = 500;          // 中粗
        constexpr int SEMIBOLD = 600;        // 半粗
        constexpr int BOLD = 700;            // 粗体
        constexpr int EXTRABOLD = 800;       // 特粗 (不推荐)
    }

    // 行高规范 (相对于字体大小)
    namespace LineHeight {
        constexpr float TIGHT = 1.2f;        // 紧凑 (标题)
        constexpr float NORMAL = 1.5f;       // 标准 (正文推荐)
        constexpr float RELAXED = 1.75f;     // 宽松 (长文本)
        constexpr float SPACED = 2.0f;       // 很宽 (特殊)
    }

    // 字母间距 (Letter Spacing)
    namespace LetterSpacing {
        constexpr float TIGHT = -0.5f;       // 紧凑
        constexpr float NORMAL = 0.0f;       // 标准
        constexpr float WIDE = 0.5f;         // 宽
    }

    // 预定义的文字样式
    struct TextStyle {
        const TCHAR* family;
        int size;
        int weight;
        float lineHeight;
        float letterSpacing;

        TextStyle(const TCHAR* f, int s, int w, float lh = LineHeight::NORMAL, float ls = LetterSpacing::NORMAL)
            : family(f), size(s), weight(w), lineHeight(lh), letterSpacing(ls) {}
    };

    // 快速定义常用文字样式
    namespace Styles {
        static const TextStyle DISPLAY_LARGE_STYLE(
            FontFamily::HEADING, FontSize::DISPLAY_LARGE, FontWeight::BOLD, LineHeight::TIGHT);
        
        static const TextStyle DISPLAY_STYLE(
            FontFamily::HEADING, FontSize::DISPLAY, FontWeight::BOLD, LineHeight::TIGHT);
        
        static const TextStyle HEADLINE_STYLE(
            FontFamily::HEADING, FontSize::HEADLINE, FontWeight::SEMIBOLD, LineHeight::NORMAL);
        
        static const TextStyle SUBTITLE_STYLE(
            FontFamily::HEADING, FontSize::SUBTITLE, FontWeight::MEDIUM, LineHeight::NORMAL);
        
        static const TextStyle BODY_LARGE_STYLE(
            FontFamily::PRIMARY, FontSize::BODY_LARGE, FontWeight::MEDIUM, LineHeight::NORMAL);
        
        static const TextStyle BODY_STYLE(
            FontFamily::PRIMARY, FontSize::BODY, FontWeight::NORMAL, LineHeight::NORMAL);
        
        static const TextStyle BODY_SMALL_STYLE(
            FontFamily::PRIMARY, FontSize::BODY_SMALL, FontWeight::NORMAL, LineHeight::NORMAL);
        
        static const TextStyle CAPTION_STYLE(
            FontFamily::PRIMARY, FontSize::CAPTION, FontWeight::NORMAL, LineHeight::NORMAL);
        
        static const TextStyle LABEL_STYLE(
            FontFamily::PRIMARY, FontSize::LABEL, FontWeight::MEDIUM, LineHeight::NORMAL);
        
        static const TextStyle CODE_STYLE(
            FontFamily::MONOSPACE, FontSize::CODE, FontWeight::NORMAL, LineHeight::NORMAL);
    }
}

// ============================================================================
// 第二部分：间距系统（Spacing System）
// ============================================================================

/**
 * @brief 8px 网格系统间距
 * @details 所有间距都是 8px 的倍数
 */
namespace Spacing {
    // 基础间距单位 (8px)
    constexpr int UNIT = 8;
    
    // 间距规范
    constexpr int XS = 4;        // 0.5 units  - 超小 (不常用)
    constexpr int S = 8;         // 1 unit     - 小
    constexpr int M = 16;        // 2 units    - 中 (最常用)
    constexpr int L = 24;        // 3 units    - 大
    constexpr int XL = 32;       // 4 units    - 超大
    constexpr int XXL = 48;      // 6 units    - 特大
    
    // 特定用途间距
    namespace Component {
        constexpr int PADDING_HORIZONTAL = M;    // 水平内边距
        constexpr int PADDING_VERTICAL = S;      // 竖直内边距
        constexpr int PADDING_LARGE = L;         // 大内边距
        
        constexpr int GAP_ITEM = S;              // 项之间间距 (8px)
        constexpr int GAP_SECTION = M;           // 组之间间距 (16px)
        constexpr int GAP_LARGE = L;             // 大组间距 (24px)
        
        constexpr int BORDER_RADIUS_SMALL = 4;   // 小圆角
        constexpr int BORDER_RADIUS = 8;         // 标准圆角
        constexpr int BORDER_RADIUS_LARGE = 12;  // 大圆角
    }
}

// ============================================================================
// 第三部分：颜色系统（Color System）
// ============================================================================

/**
 * @brief 现代设计颜色系统
 * @details 支持深色、浅色、高对比度三种主题
 */
namespace Colors {
    
    // 色值定义（十六进制）
    namespace Dark {
        // 主色调 (蓝色)
        constexpr COLORREF PRIMARY_50 = RGB(230, 240, 255);   // 极浅
        constexpr COLORREF PRIMARY_100 = RGB(200, 220, 255);
        constexpr COLORREF PRIMARY_200 = RGB(170, 200, 255);
        constexpr COLORREF PRIMARY_300 = RGB(100, 150, 255);
        constexpr COLORREF PRIMARY = RGB(50, 120, 255);        // 标准主色
        constexpr COLORREF PRIMARY_600 = RGB(30, 90, 220);
        constexpr COLORREF PRIMARY_700 = RGB(20, 70, 180);
        
        // 中性色 (灰度)
        constexpr COLORREF NEUTRAL_50 = RGB(250, 250, 250);
        constexpr COLORREF NEUTRAL_100 = RGB(240, 240, 240);
        constexpr COLORREF NEUTRAL_200 = RGB(220, 220, 220);
        constexpr COLORREF NEUTRAL_300 = RGB(180, 180, 180);
        constexpr COLORREF NEUTRAL_400 = RGB(140, 140, 140);
        constexpr COLORREF NEUTRAL_500 = RGB(100, 100, 100);
        constexpr COLORREF NEUTRAL_600 = RGB(70, 70, 70);
        constexpr COLORREF NEUTRAL_700 = RGB(50, 50, 50);
        constexpr COLORREF NEUTRAL_800 = RGB(37, 37, 37);
        constexpr COLORREF NEUTRAL_900 = RGB(25, 25, 25);
        
        // 功能色
        constexpr COLORREF SUCCESS = RGB(76, 175, 80);         // 绿色 - 成功
        constexpr COLORREF WARNING = RGB(255, 152, 0);         // 橙色 - 警告
        constexpr COLORREF ERROR = RGB(244, 67, 54);           // 红色 - 错误
        constexpr COLORREF INFO = RGB(33, 150, 243);           // 蓝色 - 信息
        
        // 语义色
        constexpr COLORREF BACKGROUND = NEUTRAL_900;           // 背景色
        constexpr COLORREF SURFACE = NEUTRAL_800;              // 表面色
        constexpr COLORREF ON_SURFACE = NEUTRAL_50;            // 表面文字色
        constexpr COLORREF TEXT_PRIMARY = NEUTRAL_50;          // 主文字色
        constexpr COLORREF TEXT_SECONDARY = NEUTRAL_300;       // 辅助文字色
        constexpr COLORREF TEXT_DISABLED = NEUTRAL_500;        // 禁用文字色
        constexpr COLORREF DIVIDER = NEUTRAL_600;              // 分割线色
        constexpr COLORREF BORDER = NEUTRAL_700;               // 边框色
    }

    namespace Light {
        // 主色调 (蓝色)
        constexpr COLORREF PRIMARY_50 = RGB(240, 248, 255);
        constexpr COLORREF PRIMARY = RGB(30, 100, 220);
        constexpr COLORREF PRIMARY_600 = RGB(20, 70, 180);
        
        // 中性色
        constexpr COLORREF NEUTRAL_50 = RGB(250, 250, 250);
        constexpr COLORREF NEUTRAL_100 = RGB(245, 245, 245);
        constexpr COLORREF NEUTRAL_300 = RGB(210, 210, 210);
        constexpr COLORREF NEUTRAL_500 = RGB(130, 130, 130);
        constexpr COLORREF NEUTRAL_600 = RGB(90, 90, 90);
        constexpr COLORREF NEUTRAL_700 = RGB(60, 60, 60);
        constexpr COLORREF NEUTRAL_800 = RGB(35, 35, 35);
        
        // 功能色
        constexpr COLORREF SUCCESS = RGB(56, 142, 60);
        constexpr COLORREF WARNING = RGB(245, 127, 23);
        constexpr COLORREF ERROR = RGB(211, 47, 47);
        constexpr COLORREF INFO = RGB(13, 110, 253);
        
        // 语义色
        constexpr COLORREF BACKGROUND = NEUTRAL_50;
        constexpr COLORREF SURFACE = RGB(255, 255, 255);
        constexpr COLORREF ON_SURFACE = NEUTRAL_800;
        constexpr COLORREF TEXT_PRIMARY = NEUTRAL_800;
        constexpr COLORREF TEXT_SECONDARY = NEUTRAL_600;
        constexpr COLORREF TEXT_DISABLED = NEUTRAL_500;
        constexpr COLORREF DIVIDER = NEUTRAL_300;
        constexpr COLORREF BORDER = NEUTRAL_300;
    }

    namespace HighContrast {
        // 高对比度 - 简化颜色、增加对比
        constexpr COLORREF PRIMARY = RGB(0, 0, 255);           // 纯蓝
        constexpr COLORREF SURFACE = RGB(255, 255, 255);       // 纯白
        constexpr COLORREF TEXT_PRIMARY = RGB(0, 0, 0);        // 纯黑
        constexpr COLORREF TEXT_SECONDARY = RGB(80, 80, 80);
        constexpr COLORREF BORDER = RGB(0, 0, 0);              // 纯黑边框
        constexpr COLORREF SUCCESS = RGB(0, 128, 0);           // 深绿
        constexpr COLORREF WARNING = RGB(255, 128, 0);         // 深橙
        constexpr COLORREF ERROR = RGB(255, 0, 0);             // 纯红
    }
}

// ============================================================================
// 第四部分：控件尺寸规范（Component Sizing）
// ============================================================================

namespace ComponentSize {
    
    // 控件高度规范
    namespace Height {
        // 触点尺寸 (最小 44px x 44px 符合 WCAG)
        constexpr int TOUCH_TARGET = 44;
        
        // 按钮和输入框
        constexpr int SMALL = 32;           // 小型控件
        constexpr int MEDIUM = 44;          // 中等控件 (标准)
        constexpr int LARGE = 52;           // 大型控件
        
        // 列表项
        constexpr int LIST_ITEM_COMPACT = 32;
        constexpr int LIST_ITEM = 44;       // 标准列表项
        constexpr int LIST_ITEM_LARGE = 56;
        
        // 标题栏
        constexpr int TOOLBAR = 56;
        constexpr int APPBAR = 64;
        
        // 卡片/面板
        constexpr int CARD_PADDING = Spacing::M;
    }

    namespace Width {
        // 按钮宽度
        constexpr int BUTTON_MIN = 80;      // 最小按钮宽度
        constexpr int BUTTON_ICON = 44;     // 图标按钮 (正方形)
        
        // 输入框宽度
        constexpr int INPUT_SMALL = 120;
        constexpr int INPUT_MEDIUM = 200;
        constexpr int INPUT_LARGE = 300;
    }

    namespace IconSize {
        constexpr int EXTRA_SMALL = 16;
        constexpr int SMALL = 20;           // 小图标
        constexpr int MEDIUM = 24;          // 中等图标 (标准)
        constexpr int LARGE = 32;           // 大图标
        constexpr int EXTRA_LARGE = 48;
    }
}

// ============================================================================
// 第五部分：状态系统（State System）
// ============================================================================

/**
 * @brief 控件状态定义
 */
namespace States {
    enum class ControlState {
        DEFAULT,        // 默认状态
        HOVER,          // 悬停
        ACTIVE,         // 激活/按下
        FOCUSED,        // 获得焦点
        DISABLED,       // 禁用
        ERROR,          // 错误
        SUCCESS,        // 成功
        WARNING         // 警告
    };

    // 状态下的透明度
    namespace Opacity {
        constexpr float FULL = 1.0f;        // 100%
        constexpr float HOVER = 0.92f;      // 92% (轻微高亮)
        constexpr float ACTIVE = 0.85f;     // 85% (明显高亮)
        constexpr float DISABLED = 0.38f;   // 38% (禁用时变淡)
        constexpr float SUBTLE = 0.12f;     // 12% (细微覆盖)
    }

    // 状态下的颜色叠加
    namespace Overlay {
        // 在原色上叠加白色获得 hover 效果
        constexpr float HOVER_OVERLAY = 0.08f;
        // 在原色上叠加黑色获得 active 效果
        constexpr float ACTIVE_OVERLAY = 0.12f;
    }
}

// ============================================================================
// 第六部分：动画系统（Animation System）
// ============================================================================

namespace Animation {
    // 持续时间 (毫秒)
    namespace Duration {
        constexpr int INSTANT = 0;          // 无动画
        constexpr int FAST = 100;           // 快速 (UI 反馈)
        constexpr int STANDARD = 200;       // 标准 (常用)
        constexpr int SLOW = 300;           // 缓慢 (过渡)
        constexpr int SLOWER = 500;         // 更缓慢
    }

    // 缓动函数
    namespace Easing {
        // 线性
        constexpr const char* LINEAR = "linear";
        // 标准缓动 (推荐用于大多数)
        constexpr const char* EASE_IN_OUT = "ease-in-out";
        // 进入 (元素进入屏幕时)
        constexpr const char* EASE_IN = "ease-in";
        // 离开 (元素离开屏幕时)
        constexpr const char* EASE_OUT = "ease-out";
    }
}

// ============================================================================
// 第七部分：阴影系统（Shadow System）
// ============================================================================

namespace Shadows {
    enum class ElevationLevel {
        NONE = 0,       // 无阴影
        LOW = 1,        // 低提升
        MEDIUM = 2,     // 中等提升
        HIGH = 3,       // 高提升
        EXTREME = 4     // 极高提升
    };

    // 阴影参数 (对应 CSS box-shadow)
    // 使用 GDI+ 或手动绘制实现
    struct ShadowStyle {
        int offsetX;
        int offsetY;
        int blurRadius;
        int spreadRadius;
        COLORREF color;
        float opacity;
    };
}

// ============================================================================
// 第八部分：栅栏/网格系统（Grid System）
// ============================================================================

namespace Grid {
    // 基于 8px 的网格
    constexpr int GRID_SIZE = Spacing::UNIT;  // 8px
    
    // 响应式列数
    namespace Columns {
        constexpr int MOBILE = 4;      // 手机 (不适用此应用)
        constexpr int TABLET = 8;      // 平板
        constexpr int DESKTOP = 12;    // 桌面 (推荐)
    }

    // 页边距
    namespace Margin {
        constexpr int MOBILE = Spacing::M;
        constexpr int TABLET = Spacing::L;
        constexpr int DESKTOP = Spacing::L;
    }

    // 间隙
    namespace Gutter {
        constexpr int DEFAULT = Spacing::M;
    }
}

// ============================================================================
// 第九部分：无障碍设计（Accessibility）
// ============================================================================

namespace Accessibility {
    // 最小触点大小 (WCAG 推荐 44x44px)
    constexpr int MIN_TOUCH_TARGET = 44;
    
    // 最小文字对比度
    // 普通文本: 4.5:1 (AA)
    // 大文本: 3:1 (AA)
    // 图形元素: 3:1
    constexpr float MIN_CONTRAST_AA = 4.5f;
    constexpr float MIN_CONTRAST_AAA = 7.0f;
    
    // 焦点指示
    constexpr int FOCUS_RING_WIDTH = 2;
    constexpr COLORREF FOCUS_COLOR = RGB(50, 120, 255);  // 主色
}

// ============================================================================
// 第十部分：快速应用函数
// ============================================================================

/**
 * @brief 快速应用字体样式
 * @param pDC 设备上下文
 * @param style 文字样式
 */
inline void ApplyTextStyle(CDC* pDC, const Typography::TextStyle& style)
{
    LOGFONT lf = {};
    lf.lfHeight = -style.size;
    lf.lfWeight = style.weight;
    lf.lfQuality = CLEARTYPE_QUALITY;
    lf.lfCharSet = DEFAULT_CHARSET;
    _tcscpy_s(lf.lfFaceName, style.family);
    
    static CFont font;
    font.DeleteObject();
    font.CreateFontIndirect(&lf);
    pDC->SelectObject(&font);
}

/**
 * @brief 混合颜色
 * @param color1 第一个颜色
 * @param color2 第二个颜色
 * @param alpha 第二个颜色的透明度 (0-1)
 * @return 混合后的颜色
 */
inline COLORREF BlendColor(COLORREF color1, COLORREF color2, float alpha)
{
    int r1 = GetRValue(color1), g1 = GetGValue(color1), b1 = GetBValue(color1);
    int r2 = GetRValue(color2), g2 = GetGValue(color2), b2 = GetBValue(color2);
    
    int r = static_cast<int>(r1 * (1 - alpha) + r2 * alpha);
    int g = static_cast<int>(g1 * (1 - alpha) + g2 * alpha);
    int b = static_cast<int>(b1 * (1 - alpha) + b2 * alpha);
    
    return RGB(r, g, b);
}

}  // namespace UIDesign

#endif
