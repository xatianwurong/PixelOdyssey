#include "LayerPanel.h"
#include "../../core/ecs/World.h"
#include "../../core/command/CommandStack.h"
#include "../../core/command/CreateEntityCommand.h"

IMPLEMENT_DYNAMIC(CLayerPanel, CPanelBase)

CLayerPanel::CLayerPanel()
{
    m_nPanelID = 1002;
    m_strTitle = _T("Layers");
}

CLayerPanel::~CLayerPanel()
{
}

BEGIN_MESSAGE_MAP(CLayerPanel, CPanelBase)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LAYERS, &CLayerPanel::OnLvnItemchangedList)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_LAYERS, &CLayerPanel::OnLvnKeyDownList)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_LAYERS, &CLayerPanel::OnNMDblclkList)
    ON_BN_CLICKED(IDC_BTN_VISIBLE, &CLayerPanel::OnBnClickedBtnVisible)
    ON_BN_CLICKED(IDC_BTN_LOCK, &CLayerPanel::OnBnClickedBtnLock)
    ON_BN_CLICKED(IDC_BTN_DELETE, &CLayerPanel::OnBnClickedBtnDelete)
    ON_BN_CLICKED(IDC_BTN_ADD, &CLayerPanel::OnBnClickedBtnAdd)
END_MESSAGE_MAP()

void CLayerPanel::CreateControls()
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE;
    
    // 图层列表
    m_layerList.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                       CRect(5, 30, 245, 200), this, IDC_LIST_LAYERS);
    
    // 设置列表样式
    m_layerList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
    
    // 添加列
    m_layerList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
    m_layerList.InsertColumn(1, _T("Visible"), LVCFMT_CENTER, 50);
    m_layerList.InsertColumn(2, _T("Lock"), LVCFMT_CENTER, 40);
    
    // 按钮栏
    int btnWidth = 45;
    int btnHeight = 25;
    int gap = 5;
    int y = 205;
    
    m_btnAdd.Create(_T("+"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    CRect(5, y, 5 + btnWidth, y + btnHeight), this, IDC_BTN_ADD);
    m_btnDelete.Create(_T("-"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                       CRect(5 + btnWidth + gap, y, 5 + btnWidth * 2 + gap, y + btnHeight), this, IDC_BTN_DELETE);
    m_btnVisible.Create(_T("👁"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                        CRect(5 + btnWidth * 2 + gap * 2, y, 5 + btnWidth * 3 + gap * 2, y + btnHeight), this, IDC_BTN_VISIBLE);
    m_btnLock.Create(_T("🔒"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     CRect(5 + btnWidth * 3 + gap * 3, y, 5 + btnWidth * 4 + gap * 3, y + btnHeight), this, IDC_BTN_LOCK);
}

void CLayerPanel::RefreshLayers()
{
    m_layerList.DeleteAllItems();
    m_layers.clear();
    
    auto& world = World::Instance();
    auto entities = world.GetEntities();
    
    int index = 0;
    for (auto& entity : entities) {
        LayerItem item;
        item.entity = entity;
        item.name = CString(entity.GetName().c_str());
        item.isVisible = true;
        item.isLocked = false;
        item.isSelected = false;
        
        if (entity.HasComponent<RenderComponent>()) {
            item.isVisible = entity.GetComponent<RenderComponent>().isVisible;
        }
        
        if (entity.HasComponent<SelectionComponent>()) {
            item.isSelected = entity.GetComponent<SelectionComponent>().isSelected;
        }
        
        m_layers.push_back(item);
        
        // 添加到列表
        m_layerList.InsertItem(index, item.name);
        
        // 设置可见性
        m_layerList.SetCheck(index, item.isVisible ? TRUE : FALSE);
        
        // 设置锁定状态（用文本表示）
        m_layerList.SetItemText(index, 1, item.isVisible ? _T("✓") : _T(""));
        m_layerList.SetItemText(index, 2, item.isLocked ? _T("🔒") : _T(""));
        
        // 设置选中状态
        if (item.isSelected) {
            m_layerList.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
        }
        
        index++;
    }
}

void CLayerPanel::Update()
{
    RefreshLayers();
}

Entity CLayerPanel::GetSelectedEntity() const
{
    int sel = m_layerList.GetNextItem(-1, LVNI_SELECTED);
    if (sel >= 0 && sel < (int)m_layers.size()) {
        return m_layers[sel].entity;
    }
    return Entity();
}

void CLayerPanel::SetSelectedEntity(Entity entity)
{
    RefreshLayers();
    
    // 找到并选中对应的项
    for (int i = 0; i < (int)m_layers.size(); i++) {
        if (m_layers[i].entity == entity) {
            m_layerList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
            m_layerList.EnsureVisible(i, FALSE);
            break;
        }
    }
}

void CLayerPanel::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    
    if (pNMLV->uNewState & LVIS_SELECTED) {
        int index = pNMLV->iItem;
        if (index >= 0 && index < (int)m_layers.size()) {
            // 通知属性面板更新
            // TODO: 发送事件或回调
        }
    }
    
    *pResult = 0;
}

void CLayerPanel::OnLvnKeyDownList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    
    if (pLVKeyDow->wVKey == VK_DELETE) {
        OnBnClickedBtnDelete();
    }
    
    *pResult = 0;
}

void CLayerPanel::OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    
    int index = pNMItemActivate->iItem;
    if (index >= 0 && index < (int)m_layers.size()) {
        // 双击重命名
        // TODO: 实现重命名
    }
    
    *pResult = 0;
}

void CLayerPanel::OnBnClickedBtnVisible()
{
    int sel = m_layerList.GetNextItem(-1, LVNI_SELECTED);
    if (sel >= 0 && sel < (int)m_layers.size()) {
        auto& entity = m_layers[sel].entity;
        
        if (entity.HasComponent<RenderComponent>()) {
            auto& render = entity.GetComponent<RenderComponent>();
            render.isVisible = !render.isVisible;
            
            // 更新列表
            m_layers[sel].isVisible = render.isVisible;
            m_layerList.SetItemText(sel, 1, render.isVisible ? _T("✓") : _T(""));
        }
    }
}

void CLayerPanel::OnBnClickedBtnLock()
{
    int sel = m_layerList.GetNextItem(-1, LVNI_SELECTED);
    if (sel >= 0 && sel < (int)m_layers.size()) {
        m_layers[sel].isLocked = !m_layers[sel].isLocked;
        m_layerList.SetItemText(sel, 2, m_layers[sel].isLocked ? _T("🔒") : _T(""));
    }
}

void CLayerPanel::OnBnClickedBtnDelete()
{
    int sel = m_layerList.GetNextItem(-1, LVNI_SELECTED);
    if (sel >= 0 && sel < (int)m_layers.size()) {
        auto& entity = m_layers[sel].entity;
        
        auto cmd = std::make_unique<DeleteEntityCommand>(entity.GetId());
        CommandStack::Instance().Execute(std::move(cmd));
        
        RefreshLayers();
    }
}

void CLayerPanel::OnBnClickedBtnAdd()
{
    // 创建新实体
    auto cmd = std::make_unique<CreateEntityCommand>("New Object");
    CommandStack::Instance().Execute(std::move(cmd));
    
    RefreshLayers();
}

void CLayerPanel::OnDraw(CDC* pDC)
{
    // 绘制标题栏
    CRect rect;
    GetClientRect(&rect);
    rect.bottom = 25;
    
    CBrush brushTitle(RGB(60, 60, 60));
    pDC->FillRect(&rect, &brushTitle);
    
    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SelectObject(&m_fontTitle);
    pDC->DrawText(m_strTitle, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CLayerPanel::OnPanelSize(int cx, int cy)
{
    // 调整列表大小
    if (m_layerList.GetSafeHwnd()) {
        m_layerList.MoveWindow(5, 30, cx - 10, cy - 60);
    }
}
