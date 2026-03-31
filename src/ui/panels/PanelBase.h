#pragma once
#include <afxwin.h>
#include <string>
#include <functional>

/**
 * @brief 面板基类
 * @details 所有 UI 面板的基类
 */
class CPanelBase : public CWnd {
    DECLARE_DYNAMIC(CPanelBase)

public:
    CPanelBase();
    virtual ~CPanelBase();

    /**
     * @brief 创建面板
     * @param pParent 父窗口
     * @param title 面板标题
     * @param rect 初始大小
     */
    virtual BOOL Create(CWnd* pParent, const CString& title, const CRect& rect);

    /**
     * @brief 获取面板标题
     */
    CString GetTitle() const { return m_strTitle; }

    /**
     * @brief 设置面板标题
     */
    void SetTitle(const CString& title) { m_strTitle = title; }

    /**
     * @brief 获取面板 ID
     */
    UINT GetPanelID() const { return m_nPanelID; }

    /**
     * @brief 设置面板 ID
     */
    void SetPanelID(UINT id) { m_nPanelID = id; }

    /**
     * @brief 是否可见
     */
    bool IsVisible() const { return m_bVisible; }

    /**
     * @brief 设置可见性
     */
    virtual void SetVisible(bool visible);

    /**
     * @brief 更新面板内容
     */
    virtual void Update() {}

    /**
     * @brief 刷新面板
     */
    virtual void Refresh() {}

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    /**
     * @brief 绘制面板内容（子类重写）
     */
    virtual void OnDraw(CDC* pDC) {}

    /**
     * @brief 面板大小改变时调用
     */
    virtual void OnPanelSize(int cx, int cy) {}

    /**
     * @brief 创建子控件（子类重写）
     */
    virtual void CreateControls() {}

    CString m_strTitle;
    UINT m_nPanelID;
    bool m_bVisible;
    CFont m_fontTitle;
    CFont m_fontNormal;
    COLORREF m_colorBackground;
    COLORREF m_colorBorder;
};
