#include "MenuBar.h"

IMPLEMENT_DYNAMIC(CMenuBar, CWnd)

CMenuBar::CMenuBar()
    : m_hoverMenu(-1)
    , m_activeMenu(-1)
    , m_hoverMenuItem(-1)
    , m_showDropdown(false)
{
    m_fontMenu.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
                          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                          DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Segoe UI"));
}

CMenuBar::~CMenuBar()
{
    m_fontMenu.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMenuBar, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BOOL CMenuBar::Create(CWnd* pParent)
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    
    if (!CWnd::Create(NULL, _T("MenuBar"), dwStyle, CRect(0, 0, 800, 25), pParent, 1004, NULL)) {
        return FALSE;
    }
    
    // 创建默认菜单
    AddMenu(_T("File"));
    AddMenuItem(0, 10001, _T("New"), _T("Ctrl+N"));
    AddMenuItem(0, 10002, _T("Open..."), _T("Ctrl+O"));
    AddSeparator(0);
    AddMenuItem(0, 10003, _T("Save"), _T("Ctrl+S"));
    AddMenuItem(0, 10004, _T("Save As..."), _T("Ctrl+Shift+S"));
    AddSeparator(0);
    AddMenuItem(0, 10005, _T("Export"), _T(""));
    AddSeparator(0);
    AddMenuItem(0, 10006, _T("Exit"), _T("Alt+F4"));
    
    AddMenu(_T("Edit"));
    AddMenuItem(1, 10011, _T("Undo"), _T("Ctrl+Z"));
    AddMenuItem(1, 10012, _T("Redo"), _T("Ctrl+Y"));
    AddSeparator(1);
    AddMenuItem(1, 10013, _T("Cut"), _T("Ctrl+X"));
    AddMenuItem(1, 10014, _T("Copy"), _T("Ctrl+C"));
    AddMenuItem(1, 10015, _T("Paste"), _T("Ctrl+V"));
    AddSeparator(1);
    AddMenuItem(1, 10016, _T("Delete"), _T("Del"));
    AddMenuItem(1, 10017, _T("Duplicate"), _T("Ctrl+D"));
    
    AddMenu(_T("View"));
    AddMenuItem(2, 10021, _T("Zoom In"), _T("Ctrl++"));
    AddMenuItem(2, 10022, _T("Zoom Out"), _T("Ctrl+-"));
    AddMenuItem(2, 10023, _T("Fit to Screen"), _T("Ctrl+0"));
    AddSeparator(2);
    AddMenuItem(2, 10024, _T("Grid"), _T("Ctrl+G"));
    AddMenuItem(2, 10025, _T("Snap to Grid"), _T("Ctrl+Shift+G"));
    AddMenuItem(2, 10026, _T("Rulers"), _T("Ctrl+R"));
    
    AddMenu(_T("Tools"));
    AddMenuItem(3, 10031, _T("Select"), _T("V"));
    AddMenuItem(3, 10032, _T("Move"), _T("G"));
    AddMenuItem(3, 10033, _T("Rotate"), _T("R"));
    AddMenuItem(3, 10034, _T("Scale"), _T("S"));
    
    AddMenu(_T("Help"));
    AddMenuItem(4, 10041, _T("Documentation"), _T("F1"));
    AddMenuItem(4, 10042, _T("About"), _T(""));
    
    return TRUE;
}

void CMenuBar::AddMenu(const CString& title)
{
    Menu menu;
    menu.title = title;
    m_menus.push_back(menu);
    Invalidate();
}

void CMenuBar::AddMenuItem(UINT menuIndex, UINT id, const CString& text, 
                           const CString& shortcut, std::function<void()> callback)
{
    if (menuIndex >= m_menus.size()) return;
    
    MenuItem item;
    item.id = id;
    item.text = text;
    item.shortcut = shortcut;
    item.callback = callback;
    item.isChecked = false;
    item.isEnabled = true;
    
    m_menus[menuIndex].items.push_back(item);
}

void CMenuBar::AddSeparator(UINT menuIndex)
{
    if (menuIndex >= m_menus.size()) return;
    
    MenuItem item;
    item.id = 0;
    item.text = _T("");
    m_menus[menuIndex].items.push_back(item);
}

void CMenuBar::SetChecked(UINT id, bool checked)
{
    for (auto& menu : m_menus) {
        for (auto& item : menu.items) {
            if (item.id == id) {
                item.isChecked = checked;
                break;
            }
        }
    }
    Invalidate();
}

void CMenuBar::SetEnabled(UINT id, bool enabled)
{
    for (auto& menu : m_menus) {
        for (auto& item : menu.items) {
            if (item.id == id) {
                item.isEnabled = enabled;
                break;
            }
        }
    }
    Invalidate();
}

CRect CMenuBar::GetMenuRect(int index) const
{
    CRect rect;
    GetClientRect(&rect);
    
    int x = 10;
    for (int i = 0; i <= index; i++) {
        CRect menuRect = rect;
        menuRect.left = x;
        
        CDC* pDC = const_cast<CMenuBar*>(this)->GetDC();
        CSize size = pDC->GetTextExtent(m_menus[i].title);
        const_cast<CMenuBar*>(this)->ReleaseDC(pDC);
        
        menuRect.right = x + size.cx + 16;
        
        if (i == index) {
            return menuRect;
        }
        
        x = menuRect.right + 5;
    }
    
    return CRect();
}

CRect CMenuBar::GetDropdownRect(int menuIndex) const
{
    if (menuIndex < 0 || menuIndex >= (int)m_menus.size()) {
        return CRect();
    }
    
    CRect menuRect = GetMenuRect(menuIndex);
    
    int maxHeight = 400;
    int itemHeight = 25;
    int itemCount = (int)m_menus[menuIndex].items.size();
    int height = min(itemCount * itemHeight + 4, maxHeight);
    
    return CRect(menuRect.left - 1, menuRect.bottom, 
                 menuRect.right + 100, menuRect.bottom + height);
}

int CMenuBar::GetMenuAtPoint(CPoint point) const
{
    for (int i = 0; i < (int)m_menus.size(); i++) {
        CRect rect = GetMenuRect(i);
        if (rect.PtInRect(point)) {
            return i;
        }
    }
    return -1;
}

int CMenuBar::GetMenuItemAtPoint(int menuIndex, CPoint point) const
{
    if (menuIndex < 0 || menuIndex >= (int)m_menus.size()) {
        return -1;
    }
    
    CRect dropdownRect = GetDropdownRect(menuIndex);
    if (!dropdownRect.PtInRect(point)) {
        return -1;
    }
    
    int itemHeight = 25;
    int y = dropdownRect.top + 2;
    int index = 0;
    
    for (const auto& item : m_menus[menuIndex].items) {
        if (item.id == 0) {
            y += 10;  // 分隔符高度
        } else {
            CRect itemRect(dropdownRect.left + 2, y, 
                          dropdownRect.right - 2, y + itemHeight);
            if (itemRect.PtInRect(point)) {
                return index;
            }
            y += itemHeight;
        }
        index++;
    }
    
    return -1;
}

void CMenuBar::ShowDropdown(int menuIndex)
{
    if (menuIndex < 0 || menuIndex >= (int)m_menus.size()) return;
    
    m_activeMenu = menuIndex;
    m_showDropdown = true;
    m_hoverMenuItem = -1;
    
    Invalidate();
}

void CMenuBar::HideDropdown()
{
    m_activeMenu = -1;
    m_showDropdown = false;
    m_hoverMenuItem = -1;
    
    Invalidate();
}

void CMenuBar::DrawMenu(CDC* pDC, int index, const CRect& rect)
{
    if (index < 0 || index >= (int)m_menus.size()) return;
    
    const Menu& menu = m_menus[index];
    
    // 背景
    COLORREF bgColor;
    if (index == m_activeMenu) {
        bgColor = RGB(0, 120, 215);  // 激活
    } else if (index == m_hoverMenu) {
        bgColor = RGB(230, 230, 230);  // 悬停
    } else {
        bgColor = RGB(240, 240, 240);  // 默认
    }
    
    CBrush brush(bgColor);
    pDC->FillRect(&rect, &brush);
    
    // 文本
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(index == m_activeMenu ? RGB(255, 255, 255) : RGB(0, 0, 0));
    pDC->SelectObject(&m_fontMenu);
    
    CRect textRect = rect;
    pDC->DrawText(menu.title, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CMenuBar::DrawDropdown(CDC* pDC, int menuIndex)
{
    if (menuIndex < 0 || menuIndex >= (int)m_menus.size()) return;
    
    const Menu& menu = m_menus[menuIndex];
    CRect rect = GetDropdownRect(menuIndex);
    
    // 背景
    CBrush brush(RGB(250, 250, 250));
    CPen pen(PS_SOLID, 1, RGB(180, 180, 180));
    pDC->FillRect(&rect, &brush);
    pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(&rect);
    
    // 绘制菜单项
    int itemHeight = 25;
    int y = rect.top + 2;
    int index = 0;
    
    for (const auto& item : menu.items) {
        if (item.id == 0) {
            // 分隔符
            CPen sepPen(PS_SOLID, 1, RGB(200, 200, 200));
            pDC->SelectObject(&sepPen);
            pDC->MoveTo(rect.left + 25, y + 5);
            pDC->LineTo(rect.right - 5, y + 5);
            y += 10;
        } else {
            CRect itemRect(rect.left + 2, y, rect.right - 2, y + itemHeight);
            
            // 悬停背景
            if (index == m_hoverMenuItem) {
                CBrush hoverBrush(RGB(0, 120, 215));
                pDC->FillRect(&itemRect, &hoverBrush);
            }
            
            // 文本
            pDC->SetBkMode(TRANSPARENT);
            pDC->SetTextColor(index == m_hoverMenuItem ? RGB(255, 255, 255) : RGB(0, 0, 0));
            
            CRect textRect = itemRect;
            textRect.left += 30;
            textRect.right -= 60;
            pDC->DrawText(item.text, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            
            // 快捷键
            if (!item.shortcut.IsEmpty()) {
                CRect shortcutRect = itemRect;
                shortcutRect.left = shortcutRect.right - 70;
                pDC->SetTextColor(index == m_hoverMenuItem ? RGB(200, 200, 200) : RGB(100, 100, 100));
                pDC->DrawText(item.shortcut, &shortcutRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
            }
            
            // 选中标记
            if (item.isChecked) {
                pDC->SetTextColor(index == m_hoverMenuItem ? RGB(255, 255, 255) : RGB(0, 0, 0));
                pDC->DrawText(_T("✓"), &itemRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }
            
            y += itemHeight;
        }
        index++;
    }
}

int CMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CMenuBar::OnPaint()
{
    CPaintDC dc(this);
    
    CRect clientRect;
    GetClientRect(&clientRect);
    
    // 绘制背景
    CBrush brush(RGB(240, 240, 240));
    dc.FillRect(&clientRect, &brush);
    
    // 绘制所有菜单
    for (int i = 0; i < (int)m_menus.size(); i++) {
        CRect rect = GetMenuRect(i);
        DrawMenu(&dc, i, rect);
    }
    
    // 绘制下拉菜单
    if (m_showDropdown && m_activeMenu >= 0) {
        DrawDropdown(&dc, m_activeMenu);
    }
}

BOOL CMenuBar::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CMenuBar::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    Invalidate();
}

void CMenuBar::OnMouseMove(UINT nFlags, CPoint point)
{
    int newHover = GetMenuAtPoint(point);
    
    if (m_showDropdown) {
        // 在下拉菜单中移动
        int newItem = GetMenuItemAtPoint(m_activeMenu, point);
        if (newItem != m_hoverMenuItem) {
            m_hoverMenuItem = newItem;
            Invalidate();
        }
    } else {
        // 在菜单栏中移动
        if (newHover != m_hoverMenu) {
            m_hoverMenu = newHover;
            Invalidate();
        }
    }
    
    CWnd::OnMouseMove(nFlags, point);
}

void CMenuBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    int menuIndex = GetMenuAtPoint(point);
    
    if (menuIndex >= 0) {
        if (m_showDropdown && menuIndex == m_activeMenu) {
            HideDropdown();
        } else {
            ShowDropdown(menuIndex);
        }
    } else {
        HideDropdown();
    }
    
    CWnd::OnLButtonDown(nFlags, point);
}

void CMenuBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_showDropdown && m_activeMenu >= 0) {
        int itemIndex = GetMenuItemAtPoint(m_activeMenu, point);
        
        if (itemIndex >= 0 && itemIndex < (int)m_menus[m_activeMenu].items.size()) {
            MenuItem& item = m_menus[m_activeMenu].items[itemIndex];
            
            if (item.id != 0 && item.isEnabled && item.callback) {
                item.callback();
                
                // 发送 WM_COMMAND 消息
                GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(item.id, 0), (LPARAM)m_hWnd);
            }
        }
        
        HideDropdown();
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CMenuBar::OnKillFocus(CWnd* pNewWnd)
{
    HideDropdown();
    CWnd::OnKillFocus(pNewWnd);
}
