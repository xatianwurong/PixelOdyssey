#pragma once
#include "UIComponents.h"
#include "../../core/ecs/Entity.h"

/**
 * @brief 现代化属性面板 - 使用统一 UI 组件
 * @details 支持自动滚动、整洁布局的属性编辑器
 */
class CAcadPropertyPanel : public CUnifiedPanel {

  DECLARE_DYNAMIC(CAcadPropertyPanel)

public:
  CAcadPropertyPanel();
  virtual ~CAcadPropertyPanel();

  /**
   * @brief 创建属性面板
   */
  BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

  /**
   * @brief 设置选中的实体
   */
  void SetSelectedEntity(Entity entity);

  /**
   * @brief 更新属性显示
   */
  void UpdateProperties();

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg void OnDraw(CDC* pDC);

private:
  Entity m_selectedEntity;
  CString m_title;

  void RebuildProperties();
};

