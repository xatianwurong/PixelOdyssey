#pragma once
#include "panels/PanelBase.h"
#include "panels/PropertyPanel.h"
#include "panels/LayerPanel.h"
#include "panels/Toolbar.h"
#include "panels/MenuBar.h"
#include "../app/Application.h"

/**
 * @brief UI 管理器
 * @details 管理所有 UI 面板和控件
 */
class UIManager {
public:
  static UIManager& Instance() {
    static UIManager instance;
    return instance;
  }

  /**
   * @brief 初始化 UI
   * @param pParent 父窗口
   * @return 是否成功
   */
  bool Initialize(CWnd* pParent) {
    m_pParent = pParent;

    // 创建菜单栏
    m_menuBar.Create(pParent);

    // 创建工具栏
    m_toolbar.Create(pParent);

    // 创建属性面板
    m_propertyPanel.Create(pParent, _T("Properties"), CRect(0, 0, 250, 300));

    // 创建图层面板
    m_layerPanel.Create(pParent, _T("Layers"), CRect(0, 0, 250, 300));

    // 设置菜单回调
    SetupMenuCallbacks();

    return true;
  }

  /**
   * @brief 调整 UI 布局
   * @param cx 宽度
   * @param cy 高度
   */
  void Layout(int cx, int cy) {
    int menuHeight = 25;
    int toolbarHeight = 40;
    int panelWidth = 250;

    // 菜单栏
    if (m_menuBar.GetSafeHwnd()) {
      m_menuBar.MoveWindow(0, 0, cx, menuHeight);
    }

    // 工具栏
    if (m_toolbar.GetSafeHwnd()) {
      m_toolbar.MoveWindow(0, menuHeight, cx, toolbarHeight);
    }

    // 属性面板（右侧）
    if (m_propertyPanel.GetSafeHwnd()) {
      m_propertyPanel.MoveWindow(cx - panelWidth, menuHeight + toolbarHeight,
        panelWidth, (cy - menuHeight - toolbarHeight) / 2);
    }

    // 图层面板（右侧下方）
    if (m_layerPanel.GetSafeHwnd()) {
      m_layerPanel.MoveWindow(cx - panelWidth, menuHeight + toolbarHeight + (cy - menuHeight - toolbarHeight) / 2,
        panelWidth, (cy - menuHeight - toolbarHeight) / 2);
    }
  }

  /**
   * @brief 更新 UI
   */
  void Update() {
    m_propertyPanel.Update();
    m_layerPanel.Update();
  }

  /**
   * @brief 设置选中实体
   */
  void SetSelectedEntity(Entity entity) {
    m_propertyPanel.SetSelectedEntity(entity);
    m_layerPanel.SetSelectedEntity(entity);
  }

  /**
   * @brief 获取菜单栏
   */
  CMenuBar& GetMenuBar() { return m_menuBar; }

  /**
   * @brief 获取工具栏
   */
  CToolbar& GetToolbar() { return m_toolbar; }

  /**
   * @brief 获取属性面板
   */
  CPropertyPanel& GetPropertyPanel() { return m_propertyPanel; }

  /**
   * @brief 获取图层面板
   */
  CLayerPanel& GetLayerPanel() { return m_layerPanel; }

  /**
   * @brief 切换工具
   */
  void SetTool(ToolType type) {
    m_toolbar.SetActiveTool(type);
  }

private:
  UIManager() = default;
  ~UIManager() = default;

  UIManager(const UIManager&) = delete;
  UIManager& operator=(const UIManager&) = delete;

  /**
   * @brief 设置菜单回调
   */
  void SetupMenuCallbacks() {
    // File 菜单
    // Edit 菜单 - 撤销/重做
    // View 菜单 - 网格/吸附
    // Tools 菜单 - 工具切换
  }

  CWnd* m_pParent;
  CMenuBar m_menuBar;
  CToolbar m_toolbar;
  CPropertyPanel m_propertyPanel;
  CLayerPanel m_layerPanel;
};
