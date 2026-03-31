#include "Toolbar.h"

IMPLEMENT_DYNAMIC(CToolbar, CWnd)

CToolbar::CToolbar()
    : m_activeTool(ToolType::Select)
    , m_hoverButton(-1)
    , m_buttonWidth(32)
    , m_buttonHeight(32)
{
    m_fontTooltip.CreateFont(11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
                             ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Segoe UI"));
}

CToolbar::~CToolbar()
{
    m_fontTooltip.DeleteObject();
}

BEGIN_MESSAGE_MAP(CToolbar, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CToolbar::Create(CWnd* pParent)
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    
    if (!CWnd::Create(NULL, _T("Toolbar"), dwStyle, CRect(0, 0, 400, 40), pParent, 1003, NULL)) {
        return FALSE;
    }
    
    // 添加默认工具按钮
    AddToolButton(ToolType::Select, _T("Select Tool (V)"));
    AddToolButton(ToolType::Move, _T("Move Tool (G)"));
    AddToolButton(ToolType::Rotate, _T("Rotate Tool (R)"));
    AddToolButton(ToolType::Scale, _T("Scale Tool (S)"));
    AddSeparator();
    AddToolButton(ToolType::Pan, _T("Pan Tool"));
    
    return TRUE;
}

void CToolbar::AddToolButton(ToolType type, const CString& tooltip, UINT iconID)
{
    ToolbarButton btn;
    btn.id = 1000 + m_buttons.size();
    btn.tooltip = tooltip;
    btn.toolType = type;
    btn.isToggle = true;
    btn.isActive = (type == m_activeTool);
    
    m_buttons.push_back(btn);
    
    Invalidate();
}

void CToolbar::AddSeparator()
{
    ToolbarButton btn;
    btn.id = 0;
    btn.isToggle = false;
    btn.isActive = false;
    m_buttons.push_back(btn);
}

void CToolbar::SetActiveTool(ToolType type)
{
    m_activeTool = type;
    
    for (auto& btn : m_buttons) {
        btn.isActive = (btn.toolType == type);
    }
    
    Invalidate();
}

void CToolbar::EnableButton(UINT id, bool enabled)
{
    for (auto& btn : m_buttons) {
        if (btn.id == id) {
            btn.isActive = enabled;
            break;
        }
    }
    Invalidate();
}

CRect CToolbar::GetButtonRect(int index) const
{
    CRect rect;
    GetClientRect(&rect);
    
    int x = 5;
    for (int i = 0; i <= index; i++) {
        if (m_buttons[i].id == 0) {
            // 分隔符
            x += 10;
        } else {
            if (i == index) {
                return CRect(x, 4, x + m_buttonWidth, 4 + m_buttonHeight);
            }
            x += m_buttonWidth + 2;
        }
    }
    
    return CRect();
}

int CToolbar::GetButtonAtPoint(CPoint point) const
{
    for (int i = 0; i < (int)m_buttons.size(); i++) {
        CRect rect = GetButtonRect(i);
        if (rect.PtInRect(point)) {
            return i;
        }
    }
    return -1;
}

void CToolbar::DrawButton(CDC* pDC, int index, const CRect& rect)
{
    if (index < 0 || index >= (int)m_buttons.size()) return;
    
    const ToolbarButton& btn = m_buttons[index];
    
    if (btn.id == 0) {
        // 绘制分隔符
        CPen pen(PS_SOLID, 1, RGB(180, 180, 180));
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->MoveTo(rect.left + 3, rect.top);
        pDC->LineTo(rect.left + 3, rect.bottom);
        pDC->SelectObject(pOldPen);
        return;
    }
    
    // 按钮背景
    COLORREF bgColor;
    if (btn.isActive) {
        bgColor = RGB(0, 120, 215);  // 激活蓝色
    } else if (index == m_hoverButton) {
        bgColor = RGB(230, 230, 230);  // 悬停灰色
    } else {
        bgColor = RGB(240, 240, 240);  // 默认
    }
    
    CBrush brush(bgColor);
    pDC->FillRect(&rect, &brush);
    
    // 按钮边框
    CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);
    pDC->SelectObject(pOldPen);
    
    // 绘制图标（简化为文字）
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(btn.isActive ? RGB(255, 255, 255) : RGB(0, 0, 0));
    
    CString iconText;
    switch (btn.toolType) {
        case ToolType::Select: iconText = _T("➤"); break;
        case ToolType::Move: iconText = _T("✥"); break;
        case ToolType::Rotate: iconText = _T("⟳"); break;
        case ToolType::Scale: iconText = _T("⤢"); break;
        case ToolType::Pan: iconText = _T("✋"); break;
        default: iconText = _T("?"); break;
    }
    
    pDC->DrawText(iconText, const_cast<CRect*>(&rect), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

int CToolbar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CToolbar::OnPaint()
{
    CPaintDC dc(this);
    
    CRect clientRect;
    GetClientRect(&clientRect);
    
    // 绘制背景
    CBrush brush(RGB(240, 240, 240));
    dc.FillRect(&clientRect, &brush);
    
    // 绘制所有按钮
    for (int i = 0; i < (int)m_buttons.size(); i++) {
        CRect rect = GetButtonRect(i);
        DrawButton(&dc, i, rect);
    }
    
    // 绘制 Tooltip
    if (m_hoverButton >= 0 && m_hoverButton < (int)m_buttons.size()) {
        const ToolbarButton& btn = m_buttons[m_hoverButton];
        if (!btn.tooltip.IsEmpty()) {
            CRect rect = GetButtonRect(m_hoverButton);
            rect.bottom = rect.top + m_buttonHeight + 25;
            rect.left -= 20;
            rect.right += 20;
            
            dc.SelectObject(&m_fontTooltip);
            dc.SetTextColor(RGB(255, 255, 255));
            dc.SetBkColor(RGB(60, 60, 60));
            dc.DrawText(btn.tooltip, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
}

BOOL CToolbar::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;  // 在 OnPaint 中绘制
}

void CToolbar::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    Invalidate();
}

void CToolbar::OnMouseMove(UINT nFlags, CPoint point)
{
    int newHover = GetButtonAtPoint(point);
    
    if (newHover != m_hoverButton) {
        m_hoverButton = newHover;
        Invalidate();
        
        // 显示 Tooltip
        if (m_hoverButton >= 0 && m_hoverButton < (int)m_buttons.size()) {
            m_currentTooltip = m_buttons[m_hoverButton].tooltip;
        } else {
            m_currentTooltip.Empty();
        }
    }
    
    CWnd::OnMouseMove(nFlags, point);
}

void CToolbar::OnLButtonDown(UINT nFlags, CPoint point)
{
    int index = GetButtonAtPoint(point);
    
    if (index >= 0 && index < (int)m_buttons.size()) {
        ToolbarButton& btn = m_buttons[index];
        
        if (btn.id != 0 && btn.isToggle) {
            SetActiveTool(btn.toolType);
            
            // 通知父窗口工具切换
            GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(btn.id, 0), (LPARAM)m_hWnd);
        }
    }
    
    CWnd::OnLButtonDown(nFlags, point);
}

void CToolbar::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonUp(nFlags, point);
}
