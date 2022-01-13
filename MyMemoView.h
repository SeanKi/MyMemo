// MyMemoView.h : interface of the CMyMemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST3VIEW_H__7995FD38_A3A9_4D3C_B7DE_AB167DE74E72__INCLUDED_)
#define AFX_TEST3VIEW_H__7995FD38_A3A9_4D3C_B7DE_AB167DE74E72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyMemoView : public CEditView//CListView
{
protected: // create from serialization only
	CMyMemoView();
	DECLARE_DYNCREATE(CMyMemoView)
	LOGFONT NEAR m_lfDefFont;
	LOGFONT NEAR m_lfDefFontOld;
	CFont m_font;

	LOGFONT NEAR m_lfDefPrintFont;
	LOGFONT NEAR m_lfDefPrintFontOld;
	CFont m_fontPrint;
	BOOL m_bDefWordWrap;
	BOOL m_bTabShapeProtect; // 블록상태에서 탭을 줬을 때 shift탭에서 모양 보호
	int m_iAutoSave;
	HWND		m_hwndEdit;
// Attributes
public:
	CMyMemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetNewFont(LOGFONT lf);
	BOOL SetWordWrap(BOOL bWordWrap);
	BOOL SaveViewFile();
	BOOL OpenViewFile(int viewType, CString sPath, CString *sVPath, BOOL bAddRML, VirtualFolderList * pVFL);
	virtual ~CMyMemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyMemoView)
	afx_msg void OnChoosefont();
	afx_msg void OnViewWordwrap();
	afx_msg void OnUpdateViewWordwrap(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnFilePrint();
	afx_msg void OnViewReadonly();
	afx_msg void OnUpdateViewReadonly(CCmdUI* pCmdUI);
	afx_msg void OnAddtoday();
	afx_msg void OnSet2RDonly();
	afx_msg void OnViewTapstops1();
	afx_msg void OnViewTapstops2();
	afx_msg void OnUpdateViewTapstops1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTapstops2(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
public:
	// 에디트 상자에 대해 일반 탭 및 블록상태에서의 탭 처리
	int AddOrRemoveTabs(int bAdd);
};

#ifndef _DEBUG  // debug version in test3View.cpp
inline CMyMemoDoc* CMyMemoView::GetDocument()
   { return (CMyMemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST3VIEW_H__7995FD38_A3A9_4D3C_B7DE_AB167DE74E72__INCLUDED_)
