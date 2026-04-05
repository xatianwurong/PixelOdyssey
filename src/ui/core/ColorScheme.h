#pragma once

#include <afxwin.h>
#include <unordered_map>

/**
 * @brief 现代化 UI 颜色方案类
 * @brief 提供多套主题支持，统一的色彩管理系统
 * @details 基于语义化颜色角色，实现主题切换
 * @details 支持深色、浅色、高对比度等多种主题
 * @功能描述 统一管理 UI 颜色和主题
 * @设计目的 实现 UI 色彩的系统化和可配置化
 * @使用场景 应用程序的 UI 主题管理和颜色配置
 * @关键实现 单例模式，基于枚举的颜色角色映射
 */
class ColorScheme
{
public:
    /**
     * @brief 主题枚举
     * @details 定义支持的主题类型
     */
    enum class Theme
    {
        Dark,           ///< 深色主题（默认）
        Light,          ///< 浅色主题
        HighContrast,   ///< 高对比度主题
        Custom          ///< 自定义主题
    };

    /**
     * @brief 颜色角色枚举
     * @details 定义语义化的颜色用途
     */
    enum class ColorRole
    {
        Background,         ///< 主背景
        Surface,            ///< 面板/卡片背景
        SurfaceHover,       ///< 表面悬停
        Primary,            ///< 主色调
        PrimaryHover,       ///< 主色调悬停
        Secondary,          ///< 次色调
        TextPrimary,        ///< 主要文字
        TextSecondary,      ///< 次要文字
        TextDisabled,       ///< 禁用文字
        Border,             ///< 边框
        BorderFocus,        ///< 焦点边框
        Success,            ///< 成功状态
        Warning,            ///< 警告状态
        Error,              ///< 错误状态
        Selection,          ///< 选中背景
        Highlight           ///< 高亮
    };

    /**
     * @brief 获取单例实例
     * @return ColorScheme 实例引用
     */
    static ColorScheme& Instance()
    {
        static ColorScheme instance;
        return instance;
    }

    /**
     * @brief 设置当前主题
     * @param theme 主题类型
     * @details 切换主题并应用相应的颜色配置
     */
    void SetTheme(Theme theme)
    {
        m_currentTheme = theme;
        ApplyTheme(theme);
    }

    /**
     * @brief 获取当前主题
     * @return 当前主题类型
     */
    Theme GetTheme() const
    {
        return m_currentTheme;
    }

    /**
     * @brief 获取颜色值
     * @param role 颜色角色
     * @return COLORREF 颜色值
     * @details 根据颜色角色获取对应的 RGB 颜色
     */
    COLORREF GetColor(ColorRole role) const
    {
        auto it = m_colors.find(role);
        return (it != m_colors.end()) ? it->second : RGB(0, 0, 0);
    }

    /**
     * @brief 设置自定义颜色
     * @param role 颜色角色
     * @param color COLORREF 颜色值
     * @details 为特定角色设置自定义颜色
     */
    void SetColor(ColorRole role, COLORREF color)
    {
        m_colors[role] = color;
    }

    /**
     * @brief 应用预设主题
     * @param theme 主题类型
     * @details 根据主题类型配置所有颜色
     */
    void ApplyTheme(Theme theme);

    /**
     * @brief 创建画刷
     * @return CBrush* 新创建的画刷指针，调用者负责删除
     * @param role 颜色角色
     * @details 通过指针避免拷贝问题
     */
    CBrush* CreateBrush(ColorRole role) const
    {
        return new CBrush(GetColor(role));
    }

    /**
     * @brief 创建画笔
     * @return CPen* 新创建的画笔指针，调用者负责删除
     * @param role 颜色角色
     * @param width 画笔宽度（默认 1）
     * @details 通过指针避免拷贝问题
     */
    CPen* CreatePen(ColorRole role, int width = 1) const
    {
        return new CPen(PS_SOLID, width, GetColor(role));
    }

private:
    /**
     * @brief 私有构造函数
     * @details 初始化为深色主题
     */
    ColorScheme()
    {
        ApplyTheme(Theme::Dark);
    }

    ~ColorScheme() = default;  ///< 析构函数

    ColorScheme(const ColorScheme&) = delete;  ///< 禁用拷贝构造
    ColorScheme& operator=(const ColorScheme&) = delete;  ///< 禁用赋值操作

    Theme m_currentTheme;  ///< 当前主题
    std::unordered_map<ColorRole, COLORREF> m_colors;  ///< 颜色映射表

    void SetupDarkTheme();        ///< 设置深色主题
    void SetupLightTheme();       ///< 设置浅色主题
    void SetupHighContrastTheme(); ///< 设置高对比度主题
};

/**
 * @brief UI 间距常量命名空间
 * @brief 统一的间距系统，基于 4px 基准
 * @details 提供一致的间距规范，确保 UI 视觉统一
 */
namespace UISpacing
{
    constexpr int XS = 4;    ///< 超小间距
    constexpr int S = 8;     ///< 小间距
    constexpr int M = 16;    ///< 中间距
    constexpr int L = 24;    ///< 大间距
    constexpr int XL = 32;   ///< 超大间距
    constexpr int XXL = 48;  ///< 特大间距

    // 组件特定间距
    constexpr int BUTTON_PADDING_X = 16;  ///< 按钮水平内边距
    constexpr int BUTTON_PADDING_Y = 10;  ///< 按钮垂直内边距
    constexpr int PANEL_PADDING = 16;     ///< 面板内边距
    constexpr int ITEM_SPACING = 8;       ///< 项目间距
    constexpr int SECTION_SPACING = 24;   ///< 区块间距
}

/**
 * @brief UI 尺寸常量命名空间
 * @details 定义 UI 组件的标准尺寸
 */
namespace UISizes
{
    // 工具栏
    constexpr int TOOLBAR_WIDTH = 56;         ///< 工具栏宽度
    constexpr int TOOL_BUTTON_SIZE = 44;      ///< 工具按钮尺寸
    constexpr int TOOL_BUTTON_SPACING = 6;    ///< 工具按钮间距

    // 面板
    constexpr int PROPERTY_PANEL_WIDTH = 300;  ///< 属性面板宽度
    constexpr int COMMAND_HEIGHT = 160;        ///< 命令行高度
    constexpr int HEADER_HEIGHT = 48;          ///< 标题栏高度
    constexpr int STATUS_BAR_HEIGHT = 28;      ///< 状态栏高度

    // 最小窗口尺寸
    constexpr int MIN_WINDOW_WIDTH = 1024;     ///< 最小窗口宽度
    constexpr int MIN_WINDOW_HEIGHT = 768;     ///< 最小窗口高度

    // 圆角半径
    constexpr int CORNER_RADIUS_SMALL = 4;     ///< 小圆角
    constexpr int CORNER_RADIUS_MEDIUM = 8;    ///< 中圆角
    constexpr int CORNER_RADIUS_LARGE = 12;    ///< 大圆角
}