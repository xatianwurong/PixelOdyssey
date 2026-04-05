#pragma once
#include "AcadUIElement.h"
#include "../../tools/Tool.h"
#include <vector>

/**
 * @brief AutoCAD 风格工具面板
 * @details 垂直排列的工具栏，类似 AutoCAD 的左侧工具面板
 */
class CAcadToolPalette : public CAcadUIElement {
  DECLARE_DYNAMIC(CAcadToolPalette)

public:
  CAcadToolPalette();
  virtual ~CAcadToolPalette();

  /**
   * @brief 创建工具面板
   */
  BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

  /**
   * @brief 添加工具按钮
   */
  void AddToolButton(ToolType type, const CString& tooltip, int iconType);

  /**
   * @brief 设置当前工具
   */
  void SetActiveTool(ToolType type);

  /**
   * @brief 获取当前工具
   */
  ToolType GetActiveTool() const { return m_activeTool; }

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg void OnDraw(CDC* pDC) override;
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseLeave();

  /**
   * @brief 绘制工具按钮
   */
  void DrawToolButton(CDC* pDC, int index, const CRect& rect);

  /**
   * @brief 获取按钮位置
   */
  int GetButtonAtPoint(CPoint point) const;

private:
  struct ToolButton {
    ToolType type;
    CString tooltip;
    int iconType;           // 0-4 对应五种图标
    CRect rect;
    bool isActive;
    bool isHover;
  };

  std::vector<ToolButton> m_buttons;
  ToolType m_activeTool;
  int m_hoverIndex;
  bool m_bTrackingMouse;
  int m_buttonWidth;
  int m_buttonHeight;
  int m_buttonSpacing;
};
