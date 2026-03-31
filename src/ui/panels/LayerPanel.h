#pragma once
#include "PanelBase.h"
#include <afxcmn.h>
#include "../../core/ecs/Entity.h"
#include <vector>

// 控件 ID 定义
#define IDC_LIST_LAYERS        3001
#define IDC_BTN_ADD            3002
#define IDC_BTN_DELETE         3003
#define IDC_BTN_VISIBLE        3004
#define IDC_BTN_LOCK           3005
#define IDC_BTN_RENAME         3006

/**
 * @brief 图层项
 */
struct LayerItem {
    Entity entity;
    CString name;
    bool isVisible;
    bool isLocked;
    bool isSelected;
};

/**
 * @brief 图层面板
 * @details 管理场景中所有对象的可见性和选择
 */
class CLayerPanel : public CPanelBase {
    DECLARE_DYNAMIC(CLayerPanel)

public:
    CLayerPanel();
    virtual ~CLayerPanel();

    /**
     * @brief 刷新图层列表
     */
    void RefreshLayers();

    /**
     * @brief 更新图层状态
     */
    virtual void Update() override;

    /**
     * @brief 获取选中的图层
     */
    Entity GetSelectedEntity() const;

    /**
     * @brief 设置选中实体
     */
    void SetSelectedEntity(Entity entity);

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLvnKeyDownList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedBtnVisible();
    afx_msg void OnBnClickedBtnLock();
    afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedBtnAdd();

    virtual void OnDraw(CDC* pDC) override;
    virtual void OnPanelSize(int cx, int cy) override;
    virtual void CreateControls() override;

private:
    /**
     * @brief 更新按钮状态
     */
    void UpdateButtonStates();

    CListCtrl m_layerList;
    CButton m_btnVisible;
    CButton m_btnLock;
    CButton m_btnDelete;
    CButton m_btnAdd;
    
    std::vector<LayerItem> m_layers;
};
