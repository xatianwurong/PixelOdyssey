#pragma once

#include <afxwin.h>
#include <memory>
#include <vector>
#include <string>
#include <functional>

/**
 * @brief AutoCAD 风格 UI 基类
 * @details 提供现代化的扁平 UI 设计，类似 AutoCAD 的界面风格
 * 
 * 设计原则:
 * - 统一管理字体和颜色资源
 * - 双缓冲绘制避免闪烁
 * - 支持主题切换
 * - 提供清晰的虚函数接口供子类扩展
 */
class CAcadUIElement : public CWnd
{
    DECLARE_DYNAMIC(CAcadUIElement)

public:
    CAcadUIElement();
    virtual ~CAcadUIElement() override;

    /**
     * @brief 创建 UI 元素
     * @param pParent 父窗口指针
     * @param nID 控件 ID
     * @param rect 初始位置和大小
     * @return 成功返回 TRUE
     */
    virtual BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

    /**
     * @brief 设置可见性
     * @param visible true-显示，false-隐藏
     */
    virtual void SetVisible(bool visible);

    /**
     * @brief 更新 UI (每帧调用)
     * @note 默认空实现，子类可重写
     */
    virtual void Update() {}

    /**
     * @brief 获取客户端区域
     */
    const CRect& GetClientRectRef() const { return m_rectClient; }

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    // 绘制接口 - 子类必须实现
    virtual void OnDraw(CDC* pDC) = 0;

    // 可重写的绘制阶段
    virtual void DrawBackground(CDC* pDC, const CRect& rect);
    virtual void DrawBorder(CDC* pDC, const CRect& rect);

    // 状态标志
    bool m_bVisible;

    // 字体资源 (从 ColorScheme/UILayout 获取配置)
    CFont m_fontTitle;
    CFont m_fontNormal;
    CFont m_fontSmall;

    // 客户端区域缓存
    CRect m_rectClient;

private:
    // 初始化字体
    void InitializeFonts();
};
