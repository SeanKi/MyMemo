// MemoTreeCtrl.h: interface for the CMemoTreeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMOTREECTRL_H__8EF9FE7D_1229_4D30_8533_3D1DB0735AC4__INCLUDED_)
#define AFX_MEMOTREECTRL_H__8EF9FE7D_1229_4D30_8533_3D1DB0735AC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"

class CMemoTreeCtrl  
{
public:
	int AddDir(HTREEITEM hItem, CString strPath, int iSortMode, VirtualFolderList *pVFL);
	void RefreshFolderItem(HTREEITEM hItem, CString strPath, VirtualFolderList *pVFL);
	HTREEITEM FindItem(HTREEITEM hItem, LPCTSTR szBuf);
	BOOL FindItemByVirtualPath(CString sPath, HTREEITEM * hItem);
	CString GetPathFromNode(HTREEITEM hItem, VOID * leftView, CString *ext=NULL, CString *sVirtualPath=NULL, VirtualFolderList ** ppVFL=NULL);
	int DelDir(HTREEITEM hItem, CString &strPath);
	void Init(CTreeCtrl * treeCtrl);
	CMemoTreeCtrl();
	virtual ~CMemoTreeCtrl();
	BOOL SetButtonState(HTREEITEM hItem, CString strPath, VirtualFolderList *pVFL = NULL);
	void DeleteFirstChild(HTREEITEM hParent);
	void DeleteAllChildren(HTREEITEM hParent);

private:
	CTreeCtrl * m_ptreeCtrl;
};

#endif // !defined(AFX_MEMOTREECTRL_H__8EF9FE7D_1229_4D30_8533_3D1DB0735AC4__INCLUDED_)
