#if !defined(AFX_TREEVIEWEXT_H__99D8F6F8_79F0_11D1_8DC6_0000E8125FE5__INCLUDED_)
#define AFX_TREEVIEWEXT_H__99D8F6F8_79F0_11D1_8DC6_0000E8125FE5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TreeViewExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeViewExt view

class CTreeViewExt : public CTreeView
{
protected:
	CTreeViewExt();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTreeViewExt)
struct tagRClickLast 
{ 
UINT Flags; 
CPoint pt; 
} m_RClickLast; 
// Attributes
public:
UINT            m_idTimer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeViewExt)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	HTREEITEM m_hDraggedItem;
	HTREEITEM   m_hitemDrop;

	BOOL m_bDraggingNow;
	CImageList *m_pDragImageList;
	virtual ~CTreeViewExt();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	virtual void CopyItemProperties(HTREEITEM hitemDrag, HTREEITEM hitemDrop);
	virtual BOOL IsItemCanBeDroppedOn(HTREEITEM hSource, HTREEITEM hTarget);
	virtual BOOL ItemCanBeDragged(HTREEITEM hItem);
	//{{AFX_MSG(CTreeViewExt)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEVIEWEXT_H__99D8F6F8_79F0_11D1_8DC6_0000E8125FE5__INCLUDED_)
