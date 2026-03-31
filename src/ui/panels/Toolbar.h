#pragma once
#include <afxwin.h>
#include "../../tools/Tool.h"
#include <vector>

/**
 * @brief 工具栏按钮
 */
struct ToolbarButton {
    UINT id;
    CString tooltip;
    ToolType toolType;
    bool isToggle;
    bool isActive;
};

/**
 * @brief 工具栏类
 * @details 提供快速工具切换和常用操作按钮
 */
class CToolbar : public CWnd {
    DECLARE_DYNAMIC(CToolbar)

public:
    CToolbar();
    virtual ~CToolbar();

    /**
     * @brief 创建工具栏
     */
    BOOL Create(CWnd* pParent);

    /**
     * @brief 添加工具按钮
     */
    void AddToolButton(ToolType type, const CString& tooltip, UINT iconID = 0);

    /**
     * @brief 添加分隔符
     */
    void AddSeparator();

    /**
     * @brief 设置当前激活的工具
     */
    void SetActiveTool(ToolType type);

    /**
     * @brief 获取当前激活的工具
     */
    ToolType GetActiveTool() const { return m_activeTool; }

    /**
     * @brief 启用/禁用按钮
     */
    void EnableButton(UINT id, bool enabled);

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
    /**
     * @brief 绘制按钮
     */
    void DrawButton(CDC* pDC, int index, const CRect& rect);

    /**
     * @brief 获取按钮矩形
     */
    CRect GetButtonRect(int index) const;

    /**
     * @brief 获取按钮索引
     */
    int GetButtonAtPoint(CPoint point) const;

    std::vector<ToolbarButton> m_buttons;
    ToolType m_activeTool;
    int m_hoverButton;
    int m_buttonWidth;
    int m_buttonHeight;
    CFont m_fontTooltip;
    CString m_currentTooltip;
};
