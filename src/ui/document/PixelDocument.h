#pragma once

#include <afxwin.h>
#include "../../core/scene/Scene.h"
#include <string>

/**
 * @brief 文档类 - 管理场景数据
 * @details 继承自 CDocument，负责管理场景数据和文档状态
 *          每个文档实例对应一个独立的绘图场景
 */
class CPixelDocument : public CDocument
{
  DECLARE_DYNCREATE(CPixelDocument)

public:
  CPixelDocument();
  virtual ~CPixelDocument();

#ifdef _DEBUG
  virtual void AssertValid() const;
#ifndef _WIN32_WCE
  virtual void Dump(CDumpContext& dc) const;
#endif
#endif

  // 文档操作
  virtual BOOL OnNewDocument();
  virtual void DeleteContents();
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

  // 场景访问
  Scene* GetScene() { return m_pScene; }
  const Scene* GetScene() const { return m_pScene; }

  // 文档信息
  void SetDocumentTitle(const CString& title);
  void SetScene(Scene* pScene);
  const CString& GetDocumentTitle() const { return m_strTitle; }
  const CString& GetFilePath() const { return m_strFilePath; }

  // 修改标记
  void SetModifiedFlag(BOOL bModified = TRUE);

protected:
  Scene* m_pScene;              ///< 当前场景指针
  CString m_strTitle;           ///< 文档标题
  CString m_strFilePath;        ///< 文件路径

  DECLARE_MESSAGE_MAP()
};
