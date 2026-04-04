#include "AcadCommandLine.h"
#include "../core/ColorScheme.h"
#include "winUser.h"


IMPLEMENT_DYNAMIC(CAcadCommandLine, CWnd)

CAcadCommandLine::CAcadCommandLine()
  : m_nScrollPos(0)
  , m_bHasFocus(false)
  , m_nLineHeight(20)
  , m_nMaxHistoryLines(100)
{}

CAcadCommandLine::~CAcadCommandLine()
{}

BEGIN_MESSAGE_MAP(CAcadCommandLine, CAcadUIElement)
  ON_WM_SIZE()
  ON_WM_LBUTTONDOWN()
  ON_WM_CHAR()
  ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CAcadCommandLine::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
  if (!CAcadUIElement::Create(pParent, nID, rect)) {
    return FALSE;
  }

  // 创建命令输入框（隐藏，自定义绘制）
  DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
  m_editCommand.Create(dwStyle, CRect(0, 0, 100, 30), this, nID + 1);
  m_editCommand.SetFont(&m_fontNormal);

  // 初始隐藏，使用自绘制
  m_editCommand.ShowWindow(SW_HIDE);

  AppendOutput(_T("PixelOdyssey Command Line v1.0"), true);
  AppendOutput(_T("Type commands and press Enter to execute"));

  return TRUE;
}

void CAcadCommandLine::SetCommandText(const CString& text)
{
  m_strCommand = text;
  m_editCommand.SetWindowText(text);
  Invalidate();
}

CString CAcadCommandLine::GetCommandText() const
{
  return m_strCommand;
}

void CAcadCommandLine::AppendOutput(const CString& text, bool isImportant)
{
  HistoryLine line;
  line.text = text;
  line.isImportant = isImportant;
  m_history.push_back(line);

  // 限制历史记录数量
  if ((int)m_history.size() > m_nMaxHistoryLines) {
    m_history.erase(m_history.begin());
  }

  ScrollToBottom();
  Invalidate();
}

void CAcadCommandLine::ClearHistory()
{
  m_history.clear();
  m_nScrollPos = 0;
  Invalidate();
}

void CAcadCommandLine::ExecuteCommand()
{
  if (m_strCommand.IsEmpty())
    return;

  // 添加到历史
  AppendOutput(_T("Command: ") + m_strCommand, true);

  // 解析命令（简单示例）
  if (m_strCommand == _T("help")) {
    AppendOutput(_T("Available commands:"));
    AppendOutput(_T("  help     - Show this help"));
    AppendOutput(_T("  clear    - Clear command history"));
    AppendOutput(_T("  grid     - Toggle grid"));
    AppendOutput(_T("  snap     - Toggle snap"));
  }
  else if (m_strCommand == _T("clear")) {
    ClearHistory();
    AppendOutput(_T("Command history cleared"));
  }
  else if (m_strCommand == _T("grid")) {
    AppendOutput(_T("Toggling grid..."));
  }
  else if (m_strCommand == _T("snap")) {
    AppendOutput(_T("Toggling snap..."));
  }
  else {
    AppendOutput(_T("Unknown command: ") + m_strCommand);
  }

  // 清空输入
  SetCommandText(_T(""));
}

void CAcadCommandLine::OnDraw(CDC* pDC)
{
  CRect rect;
  GetClientRect(&rect);

  int inputHeight = 30;
  m_rectInput = CRect(rect.left + 5, rect.bottom - inputHeight - 5,
    rect.right - 5, rect.bottom - 5);
  m_rectHistory = CRect(rect.left + 5, rect.top + 5,
    rect.right - 5, rect.bottom - inputHeight - 10);

  // 绘制历史
  DrawHistory(pDC, m_rectHistory);

  // 绘制输入框
  DrawInputBox(pDC, m_rectInput);
}

void CAcadCommandLine::OnSize(UINT nType, int cx, int cy)
{
  CAcadUIElement::OnSize(nType, cx, cy);

  if (m_editCommand.GetSafeHwnd()) {
    CRect rect;
    GetClientRect(&rect);
    m_editCommand.MoveWindow(rect.left + 40, rect.bottom - 25, rect.Width() - 45, 25);
  }
}

void CAcadCommandLine::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (m_rectInput.PtInRect(point)) {
    m_editCommand.SetFocus();
    m_editCommand.ShowWindow(SW_SHOW);
    m_editCommand.SetWindowText(m_strCommand);
    m_editCommand.SetSel(-1);
  }
}

void CAcadCommandLine::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar == VK_RETURN) {
    UpdateData(TRUE);
    ExecuteCommand();
  }
}

void CAcadCommandLine::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar == VK_UP) {
    // 显示上一条历史
    if (!m_history.empty()) {
      SetCommandText(m_history.back().text);
    }
  }
  else if (nChar == VK_DOWN) {
    // 清空
    SetCommandText(_T(""));
  }
}

void CAcadCommandLine::OnSetFocus()
{
  m_bHasFocus = true;
  m_editCommand.SetFocus();
  m_editCommand.ShowWindow(SW_SHOW);
  Invalidate();
}

void CAcadCommandLine::OnKillFocus()
{
  m_bHasFocus = false;
  m_editCommand.ShowWindow(SW_HIDE);
  Invalidate();
}

void CAcadCommandLine::DrawInputBox(CDC* pDC, const CRect& rect)
{
  auto& colors = ColorScheme::Instance();
  
  // 背景
  COLORREF bgColor = m_bHasFocus ? colors.GetColor(ColorScheme::ColorRole::Primary) : colors.GetColor(ColorScheme::ColorRole::Surface);
  CBrush* brush = new CBrush(bgColor);
  pDC->FillRect(&rect, brush); delete brush;

  // 边框
  CPen* pen = new CPen(PS_SOLID, m_bHasFocus ? 2 : 1, m_bHasFocus ? colors.GetColor(ColorScheme::ColorRole::Primary) : colors.GetColor(ColorScheme::ColorRole::Border));
  CPen* pOldPen = pDC->SelectObject(pen);
  pDC->SelectStockObject(NULL_BRUSH);
  pDC->Rectangle(&rect);
  pDC->SelectObject(pOldPen); delete pen;

  // 提示符
  pDC->SetBkMode(TRANSPARENT);
  pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::Primary));
  pDC->SelectObject(&m_fontNormal);
  pDC->DrawText(_T("Command:"), CRect(rect.left + 5, rect.top, rect.left + 80, rect.bottom),
    DT_LEFT | DT_VCENTER | DT_SINGLELINE);

  // 输入文本
  if (!m_bHasFocus) {
    pDC->SetTextColor(colors.GetColor(ColorScheme::ColorRole::TextPrimary));
    pDC->DrawText(m_strCommand, CRect(rect.left + 85, rect.top, rect.right - 5, rect.bottom),
      DT_LEFT | DT_VCENTER | DT_SINGLELINE);
  }
}

void CAcadCommandLine::DrawHistory(CDC* pDC, const CRect& rect)
{
  auto& colors = ColorScheme::Instance();
  
  // 背景
  CBrush* brush = new CBrush(colors.GetColor(ColorScheme::ColorRole::Background));
  pDC->FillRect(&rect, brush); delete brush;

  pDC->SetBkMode(TRANSPARENT);
  pDC->SelectObject(&m_fontSmall);

  int startY = rect.top;
  int lineHeight = m_nLineHeight;
  int maxLines = (rect.Height() - 10) / lineHeight;

  int startIdx = max(0, (int)m_history.size() - maxLines - m_nScrollPos);
  int endIdx = min((int)m_history.size(), startIdx + maxLines);

  for (int i = startIdx; i < endIdx; i++) {
    const HistoryLine& line = m_history[i];

    COLORREF textColor = line.isImportant ? colors.GetColor(ColorScheme::ColorRole::Highlight) : colors.GetColor(ColorScheme::ColorRole::TextPrimary);

    pDC->SetTextColor(textColor);

    CRect lineRect(rect.left, startY + (i - startIdx) * lineHeight,
      rect.right, startY + (i - startIdx + 1) * lineHeight);
    pDC->DrawText(line.text, &lineRect, DT_LEFT | DT_TOP | DT_SINGLELINE);
  }
}

void CAcadCommandLine::ScrollToBottom()
{
  m_nScrollPos = 0;
  Invalidate();
}

