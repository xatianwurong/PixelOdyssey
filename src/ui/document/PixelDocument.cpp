#include "PixelDocument.h"

#include "../../core/entity/Circle.h"
#include "../../core/entity/Rectangle.h"
#include "../../core/entity/Square.h"
#include "../../core/entity/Triangle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPixelDocument, CDocument)

BEGIN_MESSAGE_MAP(CPixelDocument, CDocument)
END_MESSAGE_MAP()

CPixelDocument::CPixelDocument()
  : m_pScene(nullptr)
  , m_strTitle(_T("Untitled"))
  , m_strFilePath(_T(""))
{
}

CPixelDocument::~CPixelDocument()
{
  DeleteContents();
}

#ifdef _DEBUG
void CPixelDocument::AssertValid() const
{
  CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CPixelDocument::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif
#endif

BOOL CPixelDocument::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  try
  {
    // 删除旧内容
    DeleteContents();

    // 创建新场景
    m_pScene = new Scene("Untitled");
    m_strTitle = _T("Untitled");
    m_strFilePath = _T("");

    // 默认在视口中放置一组可见实体，确保单文档启动后有内容可画。
    m_pScene->AddObject(new Circle(
      glm::vec2(-0.45f, 0.15f),
      glm::vec4(0.16f, 0.78f, 0.42f, 1.0f),
      0.18f));
    m_pScene->AddObject(new Square(
      glm::vec2(0.00f, 0.02f),
      glm::vec4(0.12f, 0.56f, 0.95f, 1.0f),
      0.28f));
    m_pScene->AddObject(new Triangle(
      glm::vec2(0.42f, -0.10f),
      glm::vec4(0.98f, 0.63f, 0.18f, 1.0f),
      0.32f));

    // 设置未修改标记
    SetModifiedFlag(FALSE);

    return TRUE;
  }
  catch (...)
  {
    DeleteContents();
    return FALSE;
  }
}

void CPixelDocument::DeleteContents()
{
  if (m_pScene)
  {
    delete m_pScene;
    m_pScene = nullptr;
  }

  m_strTitle = _T("Untitled");
  m_strFilePath = _T("");

  CDocument::DeleteContents();
}

BOOL CPixelDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
  // TODO: 实现文档保存逻辑
  // 这里可以序列化场景数据到文件
  
  m_strFilePath = lpszPathName;
  
  // 提取文件名作为标题
  CString strTitle;
  int nIndex = m_strFilePath.ReverseFind('\\');
  if (nIndex != -1)
    strTitle = m_strFilePath.Mid(nIndex + 1);
  else
    strTitle = m_strFilePath;
  
  SetDocumentTitle(strTitle);
  
  return TRUE;
}

BOOL CPixelDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // TODO: 实现文档加载逻辑
  // 这里可以从文件反序列化场景数据
  
  m_strFilePath = lpszPathName;
  
  // 提取文件名作为标题
  CString strTitle;
  int nIndex = m_strFilePath.ReverseFind('\\');
  if (nIndex != -1)
    strTitle = m_strFilePath.Mid(nIndex + 1);
  else
    strTitle = m_strFilePath;
  
  SetDocumentTitle(strTitle);
  
  return TRUE;
}

void CPixelDocument::SetDocumentTitle(const CString& title)
{
  m_strTitle = title;
  
  // 更新所有关联的视图标题
  POSITION pos = GetFirstViewPosition();
  while (pos != NULL)
  {
    CView* pView = GetNextView(pos);
    if (pView != NULL)
    {
      CFrameWnd* pFrame = (CFrameWnd*)pView->GetParentFrame();
      if (pFrame != NULL)
      {
        pFrame->SetTitle(title);
      }
    }
  }
}

void CPixelDocument::SetScene(Scene* pScene)
{
  if (m_pScene == pScene)
  {
    return;
  }

  delete m_pScene;
  m_pScene = pScene;
}

void CPixelDocument::SetModifiedFlag(BOOL bModified)
{
  CDocument::SetModifiedFlag(bModified);
}
