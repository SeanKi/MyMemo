// MyMemoDoc.h : interface of the CMyMemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST3DOC_H__9F310818_F9B3_4FA7_BA93_F79D5A92394C__INCLUDED_)
#define AFX_TEST3DOC_H__9F310818_F9B3_4FA7_BA93_F79D5A92394C__INCLUDED_

#pragma warning (disable:4786)

//#include <boost/regex.hpp>
//#include <stack>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyMemoDoc : public CDocument
{
protected: // create from serialization only
	CMyMemoDoc();
	DECLARE_DYNCREATE(CMyMemoDoc)

// Attributes
public:
	VOID * m_viewMain;
	CString m_docPath;
	CStringArray m_RecentMemoList;
	VirtualFolderList * m_pVFL;
	int m_MRUIndex;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void fUpdateMRUMenuItem();
	void AddToRecentMemoList(CString sPath);
	virtual ~CMyMemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyMemoDoc)
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
//	afx_msg void OnUpdateFileMruMemo(CCmdUI* pCmdUI, int nID);
	afx_msg void OnFileMruMemo(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST3DOC_H__9F310818_F9B3_4FA7_BA93_F79D5A92394C__INCLUDED_)
