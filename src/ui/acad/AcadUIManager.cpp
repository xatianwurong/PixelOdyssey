#include "AcadUIManager.h"
#include "UILayout.h"

BOOL CAcadUIManager::Initialize(CWnd* pParent)
{
  m_pParent = pParent;

  // 使用统一的布局常量
  m_toolWidth = UILayout::TOOL_WIDTH;
  m_panelWidth = UILayout::PANEL_WIDTH;
  m_commandHeight = UILayout::COMMAND_HEIGHT;

  CRect rectParent;
  pParent->GetClientRect(&rectParent);

  // 创建工具面板（左侧，占满整个高度）
  m_toolPalette.Create(pParent, 2001,
    UILayout::GetToolRect(rectParent.Width(), rectParent.Height()));

  // 创建属性面板（右侧，占满整个右侧区域，减去命令行高度）
  m_propertyPanel.Create(pParent, 2002,
    UILayout::GetPropertyPanelRect(rectParent.Width(), rectParent.Height()));

  // 创建命令行（底部，占满整个底部宽度，减去左右面板）
  m_commandLine.Create(pParent, 2004,
    UILayout::GetCommandLineRect(rectParent.Width(), rectParent.Height()));

  //LOG(_T("[AcadUIManager] UI components created with unified layout"));
  return TRUE;
}

void CAcadUIManager::Layout(int width, int height)
{
  // 使用统一的布局函数调整所有 UI 组件
  if (m_toolPalette.GetSafeHwnd()) {
    m_toolPalette.MoveWindow(UILayout::GetToolRect(width, height));
  }

  if (m_propertyPanel.GetSafeHwnd()) {
    m_propertyPanel.MoveWindow(UILayout::GetPropertyPanelRect(width, height));
  }

  if (m_commandLine.GetSafeHwnd()) {
    m_commandLine.MoveWindow(UILayout::GetCommandLineRect(width, height));
  }
}

void CAcadUIManager::Update()
{
  // 更新属性面板（重绘）
  if (m_propertyPanel.GetSafeHwnd()) {
    m_propertyPanel.Invalidate();
  }
}

void CAcadUIManager::SetActiveTool(ToolType type)
{
  m_toolPalette.SetActiveTool(type);
}
