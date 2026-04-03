#pragma once
#include "AcadUIElement.h"
#include <vector>
#include <string>

/**
 * @brief AutoCAD 风格命令行
 * @details 类似 AutoCAD 的命令行界面，支持命令输入和历史记录
 */
class CAcadCommandLine : public CAcadUIElement 
{
  DECLARE_DYNAMIC(CAcadCommandLine)

public:
  CAcadCommandLine();
  virtual ~CAcadCommandLine();

  /**
   * @brief 创建命令行
   */
  BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

  /**
   * @brief 设置命令文本
   */
  void SetCommandText(const CString& text);

  /**
   * @brief 获取命令文本
   */
  CString GetCommandText() const;

  /**
   * @brief 添加输出信息
   */
  void AppendOutput(const CString& text, bool isImportant = false);

  /**
   * @brief 清空历史
   */
  void ClearHistory();

  /**
   * @brief 执行命令
   */
  void ExecuteCommand();

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg void OnDraw(CDC* pDC) override;
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnSetFocus();
  afx_msg void OnKillFocus();

  /**
   * @brief 绘制输入框
   */
  void DrawInputBox(CDC* pDC, const CRect& rect);

  /**
   * @brief 绘制历史
   */
  void DrawHistory(CDC* pDC, const CRect& rect);

  /**
   * @brief 滚动到底部
   */
  void ScrollToBottom();

private:
  struct HistoryLine 
  {
    CString text;
    bool isImportant;
  };

  CString m_strCommand;
  std::vector<HistoryLine> m_history;
  int m_nScrollPos;
  bool m_bHasFocus;
  CRect m_rectInput;
  CRect m_rectHistory;
  CEdit m_editCommand;
  int m_nLineHeight;
  int m_nMaxHistoryLines;
};
