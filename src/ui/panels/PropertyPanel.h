#pragma once
#include "PanelBase.h"
#include <afxcmn.h>
#include "../../core/ecs/Entity.h"
#include "../../core/ecs/Component.h"

// 控件 ID 定义
#define IDC_STATIC_NAME         2001
#define IDC_COMBO_TYPE          2002
#define IDC_EDIT_POS_X          2003
#define IDC_EDIT_POS_Y          2004
#define IDC_EDIT_POS_Z          2005
#define IDC_EDIT_ROT_X          2006
#define IDC_EDIT_ROT_Y          2007
#define IDC_EDIT_ROT_Z          2008
#define IDC_EDIT_SCALE_X        2009
#define IDC_EDIT_SCALE_Y        2010
#define IDC_EDIT_SCALE_Z        2011
#define IDC_CHECK_VISIBLE       2012
#define IDC_BTN_RESET           2013
#define IDC_BTN_DELETE          2014

/**
 * @brief 属性面板
 * @details 显示和编辑选中实体的属性
 */
class CPropertyPanel : public CPanelBase {
    DECLARE_DYNAMIC(CPropertyPanel)

public:
    CPropertyPanel();
    virtual ~CPropertyPanel();

    /**
     * @brief 设置当前选中的实体
     */
    void SetSelectedEntity(Entity entity);

    /**
     * @brief 获取当前选中的实体
     */
    Entity GetSelectedEntity() const { return m_selectedEntity; }

    /**
     * @brief 更新属性显示
     */
    virtual void Update() override;

    /**
     * @brief 刷新面板
     */
    virtual void Refresh() override;

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnEnChangeEditPosX();
    afx_msg void OnEnChangeEditPosY();
    afx_msg void OnEnChangeEditPosZ();
    afx_msg void OnEnChangeEditRotX();
    afx_msg void OnEnChangeEditRotY();
    afx_msg void OnEnChangeEditRotZ();
    afx_msg void OnEnChangeEditScaleX();
    afx_msg void OnEnChangeEditScaleY();
    afx_msg void OnEnChangeEditScaleZ();
    afx_msg void OnCbnSelchangeComboType();

    virtual void OnDraw(CDC* pDC) override;
    virtual void OnPanelSize(int cx, int cy) override;
    virtual void CreateControls() override;

private:
    /**
     * @brief 更新控件值
     */
    void UpdateControls();

    /**
     * @brief 应用变换修改
     */
    void ApplyTransformChange();

    Entity m_selectedEntity;
    
    // 控件
    CStatic m_staticName;
    CStatic m_staticType;
    CComboBox m_comboType;
    
    // 位置
    CStatic m_staticPosition;
    CEdit m_editPosX;
    CEdit m_editPosY;
    CEdit m_editPosZ;
    
    // 旋转
    CStatic m_staticRotation;
    CEdit m_editRotX;
    CEdit m_editRotY;
    CEdit m_editRotZ;
    
    // 缩放
    CStatic m_staticScale;
    CEdit m_editScaleX;
    CEdit m_editScaleY;
    CEdit m_editScaleZ;
    
    // 可见性
    CButton m_checkVisible;
    
    // 按钮
    CButton m_btnReset;
    CButton m_btnDelete;
    
    bool m_bUpdating;  // 防止递归更新
};
