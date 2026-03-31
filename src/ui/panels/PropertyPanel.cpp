#include "PropertyPanel.h"
#include "../../core/ecs/World.h"
#include "../../core/command/CommandStack.h"
#include "../../core/command/CreateEntityCommand.h"

IMPLEMENT_DYNAMIC(CPropertyPanel, CPanelBase)

CPropertyPanel::CPropertyPanel()
    : m_bUpdating(false)
{
    m_nPanelID = 1001;
    m_strTitle = _T("Properties");
}

CPropertyPanel::~CPropertyPanel()
{
}

BEGIN_MESSAGE_MAP(CPropertyPanel, CPanelBase)
    ON_EN_CHANGE(IDC_EDIT_POS_X, &CPropertyPanel::OnEnChangeEditPosX)
    ON_EN_CHANGE(IDC_EDIT_POS_Y, &CPropertyPanel::OnEnChangeEditPosY)
    ON_EN_CHANGE(IDC_EDIT_POS_Z, &CPropertyPanel::OnEnChangeEditPosZ)
    ON_EN_CHANGE(IDC_EDIT_ROT_X, &CPropertyPanel::OnEnChangeEditRotX)
    ON_EN_CHANGE(IDC_EDIT_ROT_Y, &CPropertyPanel::OnEnChangeEditRotY)
    ON_EN_CHANGE(IDC_EDIT_ROT_Z, &CPropertyPanel::OnEnChangeEditRotZ)
    ON_EN_CHANGE(IDC_EDIT_SCALE_X, &CPropertyPanel::OnEnChangeEditScaleX)
    ON_EN_CHANGE(IDC_EDIT_SCALE_Y, &CPropertyPanel::OnEnChangeEditScaleY)
    ON_EN_CHANGE(IDC_EDIT_SCALE_Z, &CPropertyPanel::OnEnChangeEditScaleZ)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CPropertyPanel::OnCbnSelchangeComboType)
END_MESSAGE_MAP()

void CPropertyPanel::CreateControls()
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE;
    int y = 30;
    int labelWidth = 60;
    int editWidth = 70;
    int editHeight = 22;
    int gap = 5;
    
    // 名称
    m_staticName.Create(_T("No Selection"), WS_CHILD | WS_VISIBLE, 
                        CRect(10, y, 200, y + 20), this, IDC_STATIC);
    m_staticName.SetFont(&m_fontTitle);
    y += 25;
    
    // 类型
    m_staticType.Create(_T("Type:"), WS_CHILD | WS_VISIBLE,
                        CRect(10, y, 10 + labelWidth, y + editHeight), this, IDC_STATIC);
    m_comboType.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                       CRect(10 + labelWidth + gap, y, 10 + labelWidth + gap + editWidth * 2, y + 150),
                       this, IDC_COMBO_TYPE);
    m_comboType.AddString(_T("Rectangle"));
    m_comboType.AddString(_T("Circle"));
    m_comboType.AddString(_T("Triangle"));
    m_comboType.AddString(_T("Cube"));
    y += 30;
    
    // 位置
    m_staticPosition.Create(_T("Position:"), WS_CHILD | WS_VISIBLE,
                            CRect(10, y, 10 + labelWidth, y + editHeight), this, IDC_STATIC);
    y += 22;
    
    m_editPosX.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                      CRect(10, y, 10 + editWidth, y + editHeight), this, IDC_EDIT_POS_X);
    m_editPosY.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                      CRect(10 + editWidth + gap, y, 10 + editWidth * 2 + gap, y + editHeight), this, IDC_EDIT_POS_Y);
    m_editPosZ.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                      CRect(10 + editWidth * 2 + gap * 2, y, 10 + editWidth * 3 + gap * 2, y + editHeight), this, IDC_EDIT_POS_Z);
    y += 30;
    
    // 旋转
    m_staticRotation.Create(_T("Rotation:"), WS_CHILD | WS_VISIBLE,
                            CRect(10, y, 10 + labelWidth, y + editHeight), this, IDC_STATIC);
    y += 22;
    
    m_editRotX.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                      CRect(10, y, 10 + editWidth, y + editHeight), this, IDC_EDIT_ROT_X);
    m_editRotY.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                      CRect(10 + editWidth + gap, y, 10 + editWidth * 2 + gap, y + editHeight), this, IDC_EDIT_ROT_Y);
    m_editRotZ.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                      CRect(10 + editWidth * 2 + gap * 2, y, 10 + editWidth * 3 + gap * 2, y + editHeight), this, IDC_EDIT_ROT_Z);
    y += 30;
    
    // 缩放
    m_staticScale.Create(_T("Scale:"), WS_CHILD | WS_VISIBLE,
                         CRect(10, y, 10 + labelWidth, y + editHeight), this, IDC_STATIC);
    y += 22;
    
    m_editScaleX.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                        CRect(10, y, 10 + editWidth, y + editHeight), this, IDC_EDIT_SCALE_X);
    m_editScaleY.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                        CRect(10 + editWidth + gap, y, 10 + editWidth * 2 + gap, y + editHeight), this, IDC_EDIT_SCALE_Y);
    m_editScaleZ.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                        CRect(10 + editWidth * 2 + gap * 2, y, 10 + editWidth * 3 + gap * 2, y + editHeight), this, IDC_EDIT_SCALE_Z);
    y += 35;
    
    // 可见性
    m_checkVisible.Create(_T("Visible"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                          CRect(10, y, 100, y + 20), this, IDC_CHECK_VISIBLE);
    y += 30;
    
    // 按钮
    m_btnReset.Create(_T("Reset Transform"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                      CRect(10, y, 120, y + 25), this, IDC_BTN_RESET);
    m_btnDelete.Create(_T("Delete"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                       CRect(130, y, 240, y + 25), this, IDC_BTN_DELETE);
}

void CPropertyPanel::SetSelectedEntity(Entity entity)
{
    m_selectedEntity = entity;
    Update();
}

void CPropertyPanel::Update()
{
    if (m_bUpdating) return;
    
    UpdateControls();
}

void CPropertyPanel::Refresh()
{
    Update();
}

void CPropertyPanel::UpdateControls()
{
    m_bUpdating = true;
    
    if (m_selectedEntity.IsValid()) {
        // 名称
        m_staticName.SetWindowText(CString(m_selectedEntity.GetName().c_str()));
        
        // 获取变换组件
        if (m_selectedEntity.HasComponent<TransformComponent>()) {
            auto& transform = m_selectedEntity.GetComponent<TransformComponent>();
            
            // 位置
            m_editPosX.SetWindowText(CString(std::to_string((int)(transform.position.x * 100) / 100.0).c_str()));
            m_editPosY.SetWindowText(CString(std::to_string((int)(transform.position.y * 100) / 100.0).c_str()));
            m_editPosZ.SetWindowText(CString(std::to_string((int)(transform.position.z * 100) / 100.0).c_str()));
            
            // 旋转
            m_editRotX.SetWindowText(CString(std::to_string((int)transform.rotation.x).c_str()));
            m_editRotY.SetWindowText(CString(std::to_string((int)transform.rotation.y).c_str()));
            m_editRotZ.SetWindowText(CString(std::to_string((int)transform.rotation.z).c_str()));
            
            // 缩放
            m_editScaleX.SetWindowText(CString(std::to_string((int)(transform.scale.x * 100) / 100.0).c_str()));
            m_editScaleY.SetWindowText(CString(std::to_string((int)(transform.scale.y * 100) / 100.0).c_str()));
            m_editScaleZ.SetWindowText(CString(std::to_string((int)(transform.scale.z * 100) / 100.0).c_str()));
        }
        
        // 可见性
        bool isVisible = true;
        if (m_selectedEntity.HasComponent<RenderComponent>()) {
            isVisible = m_selectedEntity.GetComponent<RenderComponent>().isVisible;
        }
        m_checkVisible.SetCheck(isVisible ? BST_CHECKED : BST_UNCHECKED);
    } else {
        m_staticName.SetWindowText(_T("No Selection"));
        m_editPosX.SetWindowText(_T(""));
        m_editPosY.SetWindowText(_T(""));
        m_editPosZ.SetWindowText(_T(""));
        m_editRotX.SetWindowText(_T(""));
        m_editRotY.SetWindowText(_T(""));
        m_editRotZ.SetWindowText(_T(""));
        m_editScaleX.SetWindowText(_T(""));
        m_editScaleY.SetWindowText(_T(""));
        m_editScaleZ.SetWindowText(_T(""));
        m_checkVisible.SetCheck(BST_UNCHECKED);
    }
    
    m_bUpdating = false;
}

void CPropertyPanel::ApplyTransformChange()
{
    if (!m_selectedEntity.IsValid() || m_bUpdating) return;
    
    if (m_selectedEntity.HasComponent<TransformComponent>()) {
        // 读取控件值
        CString str;
        glm::vec3 newPos, newRot, newScale;
        
        m_editPosX.GetWindowText(str);
        newPos.x = str.IsEmpty() ? 0 : _tstof(str);
        m_editPosY.GetWindowText(str);
        newPos.y = str.IsEmpty() ? 0 : _tstof(str);
        m_editPosZ.GetWindowText(str);
        newPos.z = str.IsEmpty() ? 0 : _tstof(str);
        
        m_editRotX.GetWindowText(str);
        newRot.x = str.IsEmpty() ? 0 : _tstof(str);
        m_editRotY.GetWindowText(str);
        newRot.y = str.IsEmpty() ? 0 : _tstof(str);
        m_editRotZ.GetWindowText(str);
        newRot.z = str.IsEmpty() ? 0 : _tstof(str);
        
        m_editScaleX.GetWindowText(str);
        newScale.x = str.IsEmpty() ? 1 : _tstof(str);
        m_editScaleY.GetWindowText(str);
        newScale.y = str.IsEmpty() ? 1 : _tstof(str);
        m_editScaleZ.GetWindowText(str);
        newScale.z = str.IsEmpty() ? 1 : _tstof(str);
        
        // 创建命令
        auto& transform = m_selectedEntity.GetComponent<TransformComponent>();
        
        if (newPos != transform.position) {
            auto cmd = std::make_unique<MoveEntityCommand>(m_selectedEntity.GetId(), newPos);
            CommandStack::Instance().Execute(std::move(cmd));
        }
        
        // TODO: 创建旋转和缩放命令
    }
}

void CPropertyPanel::OnEnChangeEditPosX() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditPosY() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditPosZ() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditRotX() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditRotY() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditRotZ() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditScaleX() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditScaleY() { if (!m_bUpdating) ApplyTransformChange(); }
void CPropertyPanel::OnEnChangeEditScaleZ() { if (!m_bUpdating) ApplyTransformChange(); }

void CPropertyPanel::OnCbnSelchangeComboType()
{
    // TODO: 更改对象类型
}

void CPropertyPanel::OnDraw(CDC* pDC)
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
    rect.left += 10;
}

void CPropertyPanel::OnPanelSize(int cx, int cy)
{
    // 调整控件大小
    // TODO: 实现响应式布局
}
