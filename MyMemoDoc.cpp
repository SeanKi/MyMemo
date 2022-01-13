// MyMemoDoc.cpp : implementation of the CMyMemoDoc class
//


#include "stdafx.h"
#pragma warning (disable:4786)

// #include <boost/regex.hpp>
#include "MyMemo.h"

#include "MyMemoDoc.h"
#include "MyMemoView.h"
#include "LeftView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#ifdef _UNICODE
//typedef boost::wregex CRegEx;
//#else
//typedef boost::regex CRegEx;
//#endif
//
//CRegEx m_regex;

static const int MAXMRUMEMO = 8;
extern CMyMemoApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMyMemoDoc

IMPLEMENT_DYNCREATE(CMyMemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyMemoDoc, CDocument)
	//{{AFX_MSG_MAP(CMyMemoDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
//	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_MEMO1, ID_FILE_MRU_MEMO12, OnUpdateFileMruMemo)
	ON_COMMAND_RANGE(ID_FILE_MRU_MEMO1, ID_FILE_MRU_MEMO12, OnFileMruMemo)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyMemoDoc construction/destruction

CMyMemoDoc::CMyMemoDoc()
{
	// TODO: add one-time construction code here
	m_MRUIndex = 0;
	m_viewMain = NULL;
	m_pVFL = NULL;
}

BOOL CMyMemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CStringArray ary;
	theApp.m_Profile.GetStringList("MEMO_MRULIST", ary);
	for (int i=0;i<ary.GetSize();i++)
		AddToRecentMemoList(ary[i]);
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CMyMemoDoc::~CMyMemoDoc()
{
	CStringArray ary;
	int size = m_RecentMemoList.GetSize();
	int viewsize = (size > MAXMRUMEMO)?MAXMRUMEMO:size;
	for (int i=0;i<viewsize;i++) 
		 ary.Add(m_RecentMemoList[(size-viewsize)+i]);
	theApp.m_Profile.WriteStringList("MEMO_MRULIST", ary);
}


/////////////////////////////////////////////////////////////////////////////
// CMyMemoDoc serialization

void CMyMemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyMemoDoc diagnostics

#ifdef _DEBUG
void CMyMemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyMemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyMemoDoc commands

void CMyMemoDoc::OnFileSave() 
{
	((CMyMemoView *)m_viewMain)->SaveViewFile();
}

void CMyMemoDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	static BOOL bPrevNS = FALSE;
	BOOL bNeedSave = ((CMyMemoView *)m_viewMain)->GetEditCtrl().GetModify();
	pCmdUI->Enable(bNeedSave);
	if (bNeedSave != bPrevNS) {
		bPrevNS = bNeedSave; //바뀔 때 딱 한 번만 실행시키기 위해
		if (bNeedSave) {
			((CMainFrame *)AfxGetMainWnd())->GetLeftPane()->PostMessage(UM_NEEDSAVE, 0, 0);
		}
	}
}

BOOL CMyMemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

void CMyMemoDoc::AddToRecentMemoList(CString sPath)
{
	int size = m_RecentMemoList.GetSize();
//	BOOL bFind = FALSE;
	for(int i=(size-1);i>=0;i--) {
		if (sPath == m_RecentMemoList[i]) 
		{//이미 같은 Path가 존재하면 이전 것을 지우고 새로 등록
			m_RecentMemoList.RemoveAt(i);
//			m_RecentMemoList.Add(sPath);
//			bFind = TRUE;
			break;
		}
	}
//	if (!bFind)
		m_RecentMemoList.Add(sPath);
	//fUpdateMRUMenuItem();
}

/*
void CMyMemoDoc::OnUpdateFileMruMemo(CCmdUI* pCmdUI, int nID) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_RecentMemoList.GetSize()>0);
}*/
void CMyMemoDoc::OnFileMruMemo(UINT nID) 
{
	((CMainFrame*)AfxGetMainWnd())->GetLeftPane()->doFileMruMemo(nID);
}

void CMyMemoDoc::fUpdateMRUMenuItem()
{
	int size = m_RecentMemoList.GetSize();
	CMenu *pMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(0);
	int viewsize = (size > MAXMRUMEMO)?MAXMRUMEMO:size;
//	pMenu->InsertMenu(ID_FILE_MRU_MEMO1, MF_BYCOMMAND, ID_FILE_MRU_MEMO1 + 1000, sPath);
	//먼저 갯수만큼 MenuItem를 추가한다.
	for(int i=ID_FILE_MRU_MEMO1; i<ID_FILE_MRU_MEMO1+viewsize;i++) {
		if ( !pMenu->ModifyMenu(i, MF_BYCOMMAND, i, "") ) {
			pMenu->InsertMenu(i-1, MF_BYCOMMAND, i, "");
		}
	}
//	int max = size -1;
	int nID = ID_FILE_MRU_MEMO1;
	for (i=0;i<viewsize;i++) {
	//for(i= max; i>=0;i--) {
		if ( !pMenu->ModifyMenu(nID, MF_BYCOMMAND, nID, m_RecentMemoList[(size-viewsize)+i]) ) {
		}
		nID++;
	}

}
