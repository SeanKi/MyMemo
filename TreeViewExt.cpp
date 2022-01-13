// TreeViewExt.cpp : implementation file
//

#include "stdafx.h"
#include "MyMemo.h"
#include "LeftView.h"
#include "TreeViewExt.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeViewExt

IMPLEMENT_DYNCREATE(CTreeViewExt, CTreeView)
/* 2001.07.25
codeguru.com�� Ran Almog�� �ҽ� CTreeViewExt�� MSDN Samples�� Cmlctrl1�� CMyTreeCtrl�� �ҽ��� �����Ͽ� ������

ItemCanBeDragged�� IsItemCanBeDroppedOn�Լ��� ������ false�� ������ ���� Drag�� drop�� �ȵǾ �̸�  True�� �ٲپ� ����� �ǰ��ϰ�
Drag������ MouseMove�� ���콺�� �÷��� �������� ���õǵ��� �ϴ� ��� �߰�
�������� Drop �� �� ���� �����۵��� �̵���Ű�� �κ��� ���� �־� �߰��ϰ�(CopyItemProperties) UM_ITEMDROPED�޽����� �߻���Ű���� ��
*/
CTreeViewExt::CTreeViewExt()
{
	m_bDraggingNow		= FALSE;
	m_hDraggedItem		= NULL;
	m_pDragImageList	= NULL;
}

CTreeViewExt::~CTreeViewExt()
{
}


BEGIN_MESSAGE_MAP(CTreeViewExt, CTreeView)
	//{{AFX_MSG_MAP(CTreeViewExt)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeViewExt drawing

void CTreeViewExt::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTreeViewExt diagnostics

#ifdef _DEBUG
void CTreeViewExt::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTreeViewExt::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeViewExt message handlers

void CTreeViewExt::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (!m_bDraggingNow)
	{
		if (ItemCanBeDragged(pNMTreeView->itemNew.hItem))
		{
			CTreeCtrl& tree = GetTreeCtrl();
			tree.SetCapture();
			m_bDraggingNow = TRUE;
			m_hDraggedItem = pNMTreeView->itemNew.hItem;
			tree.Select(m_hDraggedItem, TVGN_CARET);
			m_pDragImageList = tree.CreateDragImage(m_hDraggedItem);
			m_pDragImageList->DragEnter(&tree, pNMTreeView->ptDrag);
			m_pDragImageList->BeginDrag(0, CPoint(0, 0));
		}
	}
	
	*pResult = 0;
}

void CTreeViewExt::OnMouseMove(UINT nFlags, CPoint point) 
{
/*	if (m_bDraggingNow)
	{
		CTreeCtrl& tree = GetTreeCtrl();
		m_pDragImageList->DragEnter(&tree, point);
		m_pDragImageList->DragMove(point);
	}*/
	HTREEITEM           hitem;
	UINT                flags;

	if (m_bDraggingNow)
	{
		CTreeCtrl& tree = GetTreeCtrl(); //

		ASSERT(m_pDragImageList != NULL);
		m_pDragImageList->DragMove(point);
		CRect TrRt;
		tree.GetClientRect(&TrRt);
		TRACE2("Hittest y=%d, trrtbottom=%d\n", point.y, TrRt.bottom);

		if ((hitem = tree.HitTest(point, &flags)) != NULL)
		{
			/*
			if( m_htiOldDrop == NULL )
				m_htiOldDrop = theTree.GetDropHilightItem();
			if( m_idTimer && hitem == m_htiOldDrop )
			{
				KillTimer( m_idTimer );
				m_idTimer = 0;
			}
			
			if( !m_idTimer )
				m_idTimer = SetTimer( 1000, 2000, NULL );
			*/
			m_pDragImageList->DragLeave(&tree);
			tree.SelectDropTarget(hitem);
			m_hitemDrop = hitem;
			m_pDragImageList->DragEnter(&tree, point);
			CRect rt;
			tree.GetItemRect(hitem, &rt, TRUE);

			if (tree.GetFirstVisibleItem() == hitem) {
				tree.Invalidate(TRUE);
				tree.PostMessage(WM_VSCROLL, SB_LINEUP, NULL);
			} else if (TrRt.bottom <point.y+ rt.Height()) {
				tree.Invalidate(TRUE);
				tree.PostMessage(WM_VSCROLL, SB_LINEDOWN, NULL);
			
			}
		}
	}
	
	CTreeView::OnMouseMove(nFlags, point);
}

void CTreeViewExt::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDraggingNow)
	{
		ReleaseCapture();

		m_bDraggingNow = FALSE;
		m_pDragImageList->EndDrag();
		delete m_pDragImageList;
		m_pDragImageList = NULL;

		CTreeCtrl& tree = GetTreeCtrl();
		UINT flags;
		HTREEITEM hTargetItem = tree.HitTest(point, &flags);
		if (hTargetItem != NULL && IsItemCanBeDroppedOn(m_hDraggedItem, hTargetItem))
		{
			//HTREEITEM hNewItem = tree.InsertItem("Untitled", hTargetItem);
			//CopyItemProperties(hNewItem, m_hDraggedItem);
			// Drop�� Item�� CLOSEDFOLDER�̸� ������ �ʿ䰡 ����.
			
			/*if (nFlags != MK_CONTROL)
				tree.DeleteItem(m_hDraggedItem);*/
			tree.Expand(hTargetItem, TVE_EXPAND );
			SendMessage(UM_ITEMDROPED, (WPARAM)m_hDraggedItem, (LPARAM)hTargetItem);
			//if ( !(nFlags & MK_CONTROL) ) //���� ����� �ʿ��� ��, �޸� ���α׷������� �ʿ����
			/*if( m_idTimer )
			{
				KillTimer( m_idTimer );
				m_idTimer = 0;
			}*/
		}	

		m_hDraggedItem = NULL;
		tree.SelectDropTarget(NULL);
	}
	
	CTreeView::OnLButtonUp(nFlags, point);
}

BOOL CTreeViewExt::ItemCanBeDragged(HTREEITEM hItem)
{
		TV_ITEM item;
		item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE;
		item.hItem = hItem;
		GetTreeCtrl().GetItem(&item);
	if (item.iImage <= ID_VIRTUAL_R) return FALSE;
	return TRUE;//FALSE;
}

BOOL CTreeViewExt::IsItemCanBeDroppedOn(HTREEITEM hSource, HTREEITEM hTarget)
{ //
		TV_ITEM item;
		item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE;
		item.hItem = hTarget;
		GetTreeCtrl().GetItem(&item);
	if (hSource == hTarget) return FALSE; //���� ���� ������ ���
	if (item.iImage >= ID_ROOT/*ID_VIRTUAL_L*/ && item.iImage <= ID_OPENFOLDER) return TRUE; //ROOT(�޸�)���� �������� ���� �� �ֵ��� �����
	return FALSE;
}

void CTreeViewExt::CopyItemProperties(HTREEITEM hitemDrag, HTREEITEM hitemDrop)
{
	TV_INSERTSTRUCT     tvstruct;
	TCHAR               sztBuffer[50];
	HTREEITEM           hNewItem, hFirstChild;
	CTreeCtrl &tree = GetTreeCtrl();
		// avoid an infinite recursion situation
	tvstruct.item.hItem = hitemDrag;
	tvstruct.item.cchTextMax = 49;
	tvstruct.item.pszText = sztBuffer;
	tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	tree.GetItem(&tvstruct.item);  // get information of the dragged element
	tvstruct.hParent = hitemDrop;
	tvstruct.hInsertAfter = TVI_SORT;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	hNewItem = tree.InsertItem(&tvstruct);
	tree.SetItemData(hNewItem, (DWORD)hNewItem);

	while ((hFirstChild = tree.GetChildItem(hitemDrag)) != NULL)
	{
		CopyItemProperties(hFirstChild, hNewItem);  // recursively transfer all the items
		tree.DeleteItem(hFirstChild);        // delete the first child and all its children
	}
}

void CTreeViewExt::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	//Edit����� ���
	int iImage, iSelimg;
	GetTreeCtrl().GetItemImage(pTVDispInfo->item.hItem, iImage, iSelimg);
	if(!CANEDITITEM(iImage)) 
	{
		*pResult = 1; //Virtual Folder�����̸� Edit�� �� �� ����.
		return;
	}
	// �������̸��� �ٲ�� �� ������ ���� ��
	SendMessage(UM_FORCESAVE);
	// TODO: Add your control notification handler code here
	TRACE0(pTVDispInfo->item.pszText);
	TRACE0("\n");
	((CLeftView *)this)->m_NowEditingItem = pTVDispInfo->item.hItem;
	((CLeftView *)this)->m_prevEditFoldername = pTVDispInfo->item.pszText;
	*pResult = 0;
}

void CTreeViewExt::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	BOOL bret = FALSE;
	if (pTVDispInfo->item.pszText == NULL) //Text�� �Ѿ���� ������ Cancel�̹Ƿ� ���� ó���� ���� �ʴ´�.
		return;
	if  ( ((CLeftView *)this)->CheckConfirmNewName(pTVDispInfo->item.pszText) )//�ٲ���� ���� pTVDispInfo->item.pszText �� ���� �����
	{
		PostMessage(UM_ITEMRESELECT);
		bret = TRUE;
	}
	else
	{
		AfxMessageBox(MyLoadString(IDS_CANNOTRENAME));
	}
//	CString str;
//	if (pEdit) {
//		pEdit->GetWindowText(str);
//	}
//	pEdit = NULL;
	*pResult = bret;//TRUE;
}

void CTreeViewExt::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_RClickLast.Flags = nFlags;
	m_RClickLast.pt = point;
	CTreeView::OnRButtonDown(nFlags, point);
}

void CTreeViewExt::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu	menu,  menu2, * pmenu;
	menu.LoadMenu(IDR_MAINFRAME);
	pmenu = menu.GetSubMenu(1);
	pmenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN, point.x, point.y, /*this*/AfxGetMainWnd());//���� Window��ſ� MainFrame�� CWnd�� �Ѱ��ִ� �޴��� Enale, Disable�� ���������� ǥ�õ�	
}

void CTreeViewExt::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;//GetTreeCtrl().GetSelectedItem();
	if (hItem == NULL)
		return;
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);

	((CLeftView *)this)->SelChanged(pNMTreeView->itemOld, item);

	*pResult = 0;
}

void CTreeViewExt::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM hitem = tree.HitTest(m_RClickLast.pt, &m_RClickLast.Flags);

	tree.SelectItem(hitem); //�̳��� ��� �ұ�
		OnContextMenu(this, GetMessagePos());

	*pResult = 0;
}

//DEL int CTreeViewExt::DeleteTreeItems(HTREEITEM hItem)
//DEL {
//DEL 	int iCount = 0;
//DEL 	CTreeCtrl& tree = GetTreeCtrl();
//DEL 	HTREEITEM hItem1 = tree.GetChildItem(hItem);
//DEL 	while (hItem1) {
//DEL 		HTREEITEM hNextItem = tree.GetNextSiblingItem(hItem1);
//DEL 		HTREEITEM hItem2 = tree.GetChildItem(hItem1);
//DEL 		if (hItem2)
//DEL 			iCount += DeleteTreeItems(hItem2);
//DEL 		else {
//DEL 			tree.DeleteItem(hItem1);
//DEL 			iCount ++;
//DEL 		}
//DEL 		hItem1 = hNextItem;
//DEL 	}
//DEL 	tree.DeleteItem(hItem);
//DEL 	iCount ++;
//DEL 	return iCount;
//DEL }
