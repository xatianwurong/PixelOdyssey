#pragma once

#include "UIComponents.h"
#include "../../core/ecs/Entity.h"

/**
 * @brief 现代化属性面板 - 使用统一 UI 组件
 * @details 支持自动滚动、整洁布局的属性编辑器
 * 
 * 功能特性:
 * - 自动显示选中实体的属性
 * - 支持变换组件 (位置/旋转/缩放)
 * - 支持渲染组件 (可见性)
 * - 智能内存管理 (使用智能指针)
 */
class CAcadPropertyPanel : public CUnifiedPanel
{
    DECLARE_DYNAMIC(CAcadPropertyPanel)

public:
    CAcadPropertyPanel();
    virtual ~CAcadPropertyPanel() override;

    /**
     * @brief 创建属性面板
     * @param pParent 父窗口
     * @param nID 控件 ID
     * @param rect 初始位置和大小
     * @return 成功返回 TRUE
     */
    BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

    /**
     * @brief 设置选中的实体
     * @param entity 要显示的实体
     */
    void SetSelectedEntity(Entity entity);

    /**
     * @brief 更新属性显示
     */
    void UpdateProperties();

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnDraw(CDC* pDC) override;

private:
    Entity m_selectedEntity;      ///< 当前选中的实体
    CString m_title;              ///< 面板标题

    /**
     * @brief 重建属性列表
     */
    void RebuildProperties();

    /**
     * @brief 获取标题字体 (静态缓存)
     */
    static CFont* GetTitleFont();
};
