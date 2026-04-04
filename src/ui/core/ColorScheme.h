#pragma once

#include <afxwin.h>
#include <unordered_map>

/**
 * @brief 现代化 UI 颜色方案
 * @details 提供多套主题支持，统一的色彩管理系统
 */
class ColorScheme {
public:
  // 预定义主题
  enum class Theme {
    Dark,           // 深色主题 (默认)
    Light,          // 浅色主题
    HighContrast,   // 高对比度主题
    Custom          // 自定义主题
  };

  // 语义化颜色角色
  enum class ColorRole {
    Background,         // 主背景
    Surface,            // 面板/卡片背景
    SurfaceHover,       // 表面悬停
    Primary,            // 主色调
    PrimaryHover,       // 主色调悬停
    Secondary,          // 次色调
    TextPrimary,        // 主要文字
    TextSecondary,      // 次要文字
    TextDisabled,       // 禁用文字
    Border,             // 边框
    BorderFocus,        // 焦点边框
    Success,            // 成功状态
    Warning,            // 警告状态
    Error,              // 错误状态
    Selection,          // 选中背景
    Highlight           // 高亮
  };

  /**
   * @brief 获取单例实例
   */
  static ColorScheme& Instance() {
    static ColorScheme instance;
    return instance;
  }

  /**
   * @brief 设置当前主题
   */
  void SetTheme(Theme theme) {
    m_currentTheme = theme;
    ApplyTheme(theme);
  }

  /**
   * @brief 获取当前主题
   */
  Theme GetTheme() const { return m_currentTheme; }

  /**
   * @brief 获取颜色值
   * @param role 颜色角色
   * @return COLORREF 颜色值
   */
  COLORREF GetColor(ColorRole role) const {
    auto it = m_colors.find(role);
    return (it != m_colors.end()) ? it->second : RGB(0, 0, 0);
  }

  /**
   * @brief 设置自定义颜色
   */
  void SetColor(ColorRole role, COLORREF color) {
    m_colors[role] = color;
  }

  /**
   * @brief 应用预设主题
   */
  void ApplyTheme(Theme theme);

  /**
   * @brief 创建画刷（通过指针避免拷贝问题）
   * @return CBrush* 新创建的画刷指针，调用者负责删除
   */
  CBrush* CreateBrush(ColorRole role) const {
    return new CBrush(GetColor(role));
  }

  /**
   * @brief 创建画笔（通过指针避免拷贝问题）
   * @return CPen* 新创建的画笔指针，调用者负责删除
   */
  CPen* CreatePen(ColorRole role, int width = 1) const {
    return new CPen(PS_SOLID, width, GetColor(role));
  }

private:
  ColorScheme() {
    ApplyTheme(Theme::Dark);
  }

  ~ColorScheme() = default;

  ColorScheme(const ColorScheme&) = delete;
  ColorScheme& operator=(const ColorScheme&) = delete;

  Theme m_currentTheme;
  std::unordered_map<ColorRole, COLORREF> m_colors;

  void SetupDarkTheme();
  void SetupLightTheme();
  void SetupHighContrastTheme();
};

/**
 * @brief UI 间距常量
 * @details 统一的间距系统，基于 4px 基准
 */
namespace UISpacing {
  constexpr int XS = 4;    // 超小间距
  constexpr int S = 8;     // 小间距
  constexpr int M = 16;    // 中间距
  constexpr int L = 24;    // 大间距
  constexpr int XL = 32;   // 超大间距
  constexpr int XXL = 48;  // 特大间距

  // 组件特定间距
  constexpr int BUTTON_PADDING_X = 16;
  constexpr int BUTTON_PADDING_Y = 10;
  constexpr int PANEL_PADDING = 16;
  constexpr int ITEM_SPACING = 8;
  constexpr int SECTION_SPACING = 24;
}

/**
 * @brief UI 字体常量
 */
namespace UIFonts {
  constexpr int TITLE_SIZE = 18;
  constexpr int HEADING_SIZE = 16;
  constexpr int BODY_SIZE = 14;
  constexpr int CAPTION_SIZE = 12;
  constexpr int SMALL_SIZE = 11;

  constexpr int TITLE_WEIGHT = FW_BOLD;
  constexpr int HEADING_WEIGHT = FW_SEMIBOLD;
  constexpr int BODY_WEIGHT = FW_NORMAL;

  const TCHAR* FONT_FAMILY = _T("Microsoft YaHei UI");
  const TCHAR* MONOSPACE_FONT = _T("Consolas");
}

/**
 * @brief UI 尺寸常量
 */
namespace UISizes {
  // 工具栏
  constexpr int TOOLBAR_WIDTH = 56;
  constexpr int TOOL_BUTTON_SIZE = 44;
  constexpr int TOOL_BUTTON_SPACING = 6;

  // 面板
  constexpr int PROPERTY_PANEL_WIDTH = 300;
  constexpr int COMMAND_HEIGHT = 160;
  constexpr int HEADER_HEIGHT = 48;
  constexpr int STATUS_BAR_HEIGHT = 28;

  // 最小窗口尺寸
  constexpr int MIN_WINDOW_WIDTH = 1024;
  constexpr int MIN_WINDOW_HEIGHT = 768;

  // 圆角半径
  constexpr int CORNER_RADIUS_SMALL = 4;
  constexpr int CORNER_RADIUS_MEDIUM = 8;
  constexpr int CORNER_RADIUS_LARGE = 12;
}