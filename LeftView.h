// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__DE063A21_9180_4632_98F7_8E737FE7E468__INCLUDED_)
#define AFX_LEFTVIEW_H__DE063A21_9180_4632_98F7_8E737FE7E468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MemoTreeCtrl.h"
#include "TreeViewExt.h"

class CMyMemoDoc;

class CLeftView : public CTreeViewExt
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)
private:
	long ConnectServer(VirtualFolderList * pVFL);
	// Attributes
	struct TIMER_DOCOPEN {
		int newImage;
		CString strSrc;
		CString sVPath;
		VirtualFolderList * pVFL;
	} m_TIMERDocOpen;
	vector <MemoItem> m_memoItemList; //요놈을 InsertItem할 때 하나씩 추가하고 아이템의 ItemData에 이 Index를 기록 , DeleteItem 할때 지워야 하나 DeleteChildren할때가 좀 문제가 되겠구망
public:
//	CMap<int,int,ClsItem*,ClsItem*&> m_mapClsItem2; //VirtualFolderList map - item number
	vector <CInetFile *> m_vInetf;
	CMemoTreeCtrl m_memoTree;
	CMyMemoDoc* GetDocument();
	CImageList m_imgIcons;
//	MemoList *m_pmemolist, memolist[1]; //VirtualFolderList
//	int MemoListCount;
	CString m_RootPath;
	CString m_RootName;
	int m_iSortMode; // 0 -no sort, 1 - sort 
	int m_iAutoSave;
	int m_iTrashMode;
	ViewFileList viewfilelist[3]; //Txt, Html, csv등 메모파일 종류
	int ViewFileListCount;
	HTREEITEM m_NowEditingItem;
	CEdit * m_pEdit;
	CString m_prevEditFoldername;
//	CMap<HTREEITEM,HTREEITEM,VirtualFolderList*,VirtualFolderList*&> m_mapVFL; //VirtualFolderList map
	vector <VirtualFolderList> m_vVFL;
	vector <string> m_vDirRemarkFILEList;
	
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // called first time after construct
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL fSortTreeItem(HTREEITEM hItem);
	BOOL m_bAddRML;
	void doFileMruMemo(int nID);
	BOOL MyExpand( HTREEITEM hItem, UINT nCode );
	void fUpdateVirtualFolders(int * nUpdateds=NULL);
	void SelChanged(TVITEM ItemOld, TVITEM ItemNew, BOOL bAddRML=TRUE);
	BOOL CheckConfirmNewName(CString newItemText);
	int Itemexpand(HTREEITEM hItem, int iImage, UINT action);
	int GetViewfileImgNum(LPCTSTR szExt);
	CString GetViewfileExt(int num);
	int InitLeftView();
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateMemoNewVirtual(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMemoNewFolder(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMemoNewMemo(CCmdUI* pCmdUI);
	afx_msg void OnMemoNewVirtual();
	afx_msg void OnMemoNewMemo();
	afx_msg void OnMemoNewFolder();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRename(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnRename();
	afx_msg void OnProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	afx_msg void OnRefresh();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	//}}AFX_MSG
//	afx_msg void OnFileMruMemo(int nID);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CMyMemoDoc* CLeftView::GetDocument()
   { return (CMyMemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__DE063A21_9180_4632_98F7_8E737FE7E468__INCLUDED_)
