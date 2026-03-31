#pragma once
#include <afxwin.h>
#include <functional>
#include <map>
#include <vector>

/**
 * @brief 菜单项
 */
struct MenuItem {
    UINT id;
    CString text;
    CString shortcut;
    std::function<void()> callback;
    bool isChecked;
    bool isEnabled;
};

/**
 * @brief 菜单
 */
struct Menu {
    CString title;
    std::vector<MenuItem> items;
};

/**
 * @brief 菜单栏类
 * @details 提供完整的菜单系统
 */
class CMenuBar : public CWnd {
    DECLARE_DYNAMIC(CMenuBar)

public:
    CMenuBar();
    virtual ~CMenuBar();

    /**
     * @brief 创建菜单栏
     */
    BOOL Create(CWnd* pParent);

    /**
     * @brief 添加菜单
     */
    void AddMenu(const CString& title);

    /**
     * @brief 添加菜单项
     */
    void AddMenuItem(UINT menuIndex, UINT id, const CString& text, 
                     const CString& shortcut = _T(""), 
                     std::function<void()> callback = nullptr);

    /**
     * @brief 添加分隔符
     */
    void AddSeparator(UINT menuIndex);

    /**
     * @brief 设置菜单项选中状态
     */
    void SetChecked(UINT id, bool checked);

    /**
     * @brief 设置菜单项启用状态
     */
    void SetEnabled(UINT id, bool enabled);

    /**
     * @brief 获取菜单数量
     */
    int GetMenuCount() const { return (int)m_menus.size(); }

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

private:
    /**
     * @brief 绘制菜单
     */
    void DrawMenu(CDC* pDC, int index, const CRect& rect);

    /**
     * @brief 绘制下拉菜单
     */
    void DrawDropdown(CDC* pDC, int menuIndex);

    /**
     * @brief 获取菜单矩形
     */
    CRect GetMenuRect(int index) const;

    /**
     * @brief 获取下拉菜单矩形
     */
    CRect GetDropdownRect(int menuIndex) const;

    /**
     * @brief 获取鼠标下的菜单索引
     */
    int GetMenuAtPoint(CPoint point) const;

    /**
     * @brief 获取鼠标下的菜单项索引
     */
    int GetMenuItemAtPoint(int menuIndex, CPoint point) const;

    /**
     * @brief 显示下拉菜单
     */
    void ShowDropdown(int menuIndex);

    /**
     * @brief 隐藏下拉菜单
     */
    void HideDropdown();

    std::vector<Menu> m_menus;
    int m_hoverMenu;
    int m_activeMenu;
    int m_hoverMenuItem;
    bool m_showDropdown;
    CFont m_fontMenu;
};
