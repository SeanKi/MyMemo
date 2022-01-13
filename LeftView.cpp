// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "MyMemo.h"

#include "MyMemoDoc.h"
#include "LeftView.h"
#include "parseKi.h"
#include "MyMemoView.h"
#include "DirNFile.h"
#include "DlgVirtualFolder.h"
#include "DlgVirtualFolder2.h"
//#include "StrKi2.h"

#pragma warning(disable:4786)
#include <string>
using namespace std ;
//ID_REFRESH�޴� �������� ��� ���� �����Ƿ� �Ʒ��� �Ͻ������� ����
//#define ID_REFRESH WM_USER + 200

//1.0.0.3
// OnMemoNewMemo�� OnMemoNewVirtual���� �������� �߰��� �� ��ġ�� �õ��ϴ� ������ ����
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMyMemoApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeViewExt)

BEGIN_MESSAGE_MAP(CLeftView, CTreeViewExt)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_UPDATE_COMMAND_UI(ID_MEMO_NEW_VIRTUAL, OnUpdateMemoNewVirtual)
	ON_UPDATE_COMMAND_UI(ID_MEMO_NEW_FOLDER, OnUpdateMemoNewFolder)
	ON_UPDATE_COMMAND_UI(ID_MEMO_NEW_MEMO, OnUpdateMemoNewMemo)
	ON_COMMAND(ID_MEMO_NEW_VIRTUAL, OnMemoNewVirtual)
	ON_COMMAND(ID_MEMO_NEW_MEMO, OnMemoNewMemo)
	ON_COMMAND(ID_MEMO_NEW_FOLDER, OnMemoNewFolder)
	ON_UPDATE_COMMAND_UI(ID_DELETE, OnUpdateDelete)
	ON_UPDATE_COMMAND_UI(ID_RENAME, OnUpdateRename)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_RENAME, OnRename)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY, OnUpdateProperty)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_REFRESH, OnUpdateRefresh)
	//}}AFX_MSG_MAP
//	ON_COMMAND_RANGE(ID_FILE_MRU_MEMO1, ID_FILE_MRU_MEMO12, OnFileMruMemo)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, 
	LPARAM lParamSort)
{
	int nImage1, nSelImg1, nImage2, nSelImg2;
	CTreeCtrl * ptc = (CTreeCtrl*)lParamSort;
	TRACE1("lParam1= %x\n", lParam1);
	BOOL b;
	b = (ptc->GetItemImage((HTREEITEM)lParam1, nImage1, nSelImg1));
	if (!b) return 1; //ù��° �������� �ڷ�
	b = (ptc->GetItemImage((HTREEITEM)lParam2, nImage2, nSelImg2));
	if (!b) return -1; //�ι�° �������� �ڷ�
	if (nImage1 > nImage2) //pC1->nKind > pC1->nKind)
		return 1;
	else if (nImage1 < nImage2) //(pC1->nKind < pC1->nKind)
		return -1;
	ClsItem * pC1 = NULL;
	ClsItem * pC2 = NULL;
		pC1 = theApp.GetClsItem(nImage1);
		pC2 = theApp.GetClsItem(nImage2);//pC2 = theApp.GetClsItem(nImage2);
	CString sParam1, sParam2;
	sParam1 = ptc->GetItemText((HTREEITEM)lParam1);
	sParam2 = ptc->GetItemText((HTREEITEM)lParam2);
	BOOL param1Alpabat = FALSE;
	BOOL param2Alpabat = FALSE;

	if((sParam1[0] >= 'a' && sParam1[0] <= 'z') || (sParam1[0] >= 'A' && sParam1[0] <= 'Z')) {
		param1Alpabat = TRUE;
	}
	if((sParam2[0] >= 'a' && sParam2[0] <= 'z') || (sParam2[0] >= 'A' && sParam2[0] <= 'Z')) {
		param2Alpabat = TRUE;
	}
	if (param1Alpabat && !param2Alpabat)
		return 1;
	if (!param1Alpabat && param2Alpabat)
		return -1;
	if (param1Alpabat && param2Alpabat) {
		int i= stricmp(sParam1, sParam2);
		return i;
	}
	return (sParam1 > sParam2);//lParam1 - lParam2;
}


void DeleteAllChildrenExceptVirtualFolder(CTreeCtrl *ptreeCtrl, HTREEITEM hParent)
{
	HTREEITEM hItem;
	if((hItem = ptreeCtrl->GetChildItem(hParent)) == NULL)
		return;
	do
	{
		HTREEITEM hNextItem = ptreeCtrl->GetNextSiblingItem(hItem);
		int nImage, nSelectedImage;
		ptreeCtrl->GetItemImage(hItem, nImage, nSelectedImage);
		if (!(nImage >= ID_VIRTUAL_L && nImage <= ID_VIRTUAL_R)) //���� ������ �ƴ� ��츸 ����
			ptreeCtrl->DeleteItem(hItem);
		hItem = hNextItem;
	} while (hItem != NULL);
}
/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
//	m_pmemolist = NULL;
//	MemoListCount = 0;
	m_bAddRML = TRUE;
	
	int pos = 0;
	//string stdstrTemp;
	char buffer[255];
	while(1) {
		//stdstrTemp = STR_GetNextToken(DirRemarkFILEList, "|", &pos);
		if (!PARSE_GetNextToken(buffer, DirRemarkFILEList, "|", &pos)) break;
	//	if (pos == -1) break;
		m_vDirRemarkFILEList.push_back(buffer);
	}
	m_iSortMode = 1 ;//no sort - 0, sort - 1(giho,hangul,eng) : 00 -vfolder,folder,memo 10 - ?
	strcpy(viewfilelist[0].szExt, "txt");
	strcpy(viewfilelist[1].szExt, "doc");
	strcpy(viewfilelist[2].szExt, "htm");
	viewfilelist[0].imgNum = ID_TEXT_FILE;
	viewfilelist[1].imgNum = ID_WORD_FILE;
	viewfilelist[2].imgNum = ID_HTML_FILE;
//	ViewFileListCount = 3;
	ViewFileListCount = 1; //Text File Support Only

	//ClsItem::SetItem(int nctrl, int iImage, int iSelectedImage, int nKind, LPCTSTR szExts);
/*	m_ClsItemList[ITEM_FILE_HTML].SetItem(CTL_HTML, ID_ROOT, ID_ROOT, KIND_ROOT, "");
	m_ClsItemList[ITEM_FILE_CSV].SetItem(CTL_CSV, ID_ROOT, ID_ROOT, KIND_ROOT, "");
	m_ClsItemList[ITEM_FILE_DOC].SetItem(-1, ID_ROOT, ID_ROOT, KIND_ROOT, "");
	m_ClsItemList[ITEM_FILE_HWP].SetItem(-1, ID_ROOT, ID_ROOT, KIND_ROOT, "");
	m_ClsItemList[ITEM_FILE_MMO].SetItem(CTL_HTML, ID_ROOT, ID_ROOT, KIND_ROOT, "");*/
}

CLeftView::~CLeftView()
{
	CProfilePrivate & m_profile = theApp.m_Profile;
	char szTmp[32]={0,};
	CString stmp;
	m_profile.SetSection("Settings");
	m_profile.WriteString("AutoSave", itoa(m_iAutoSave, szTmp, 10));
	m_profile.WriteString("TrashMode", itoa(m_iTrashMode, szTmp, 10));
	m_profile.SetSection("ROOT");
	m_profile.WriteString("name", m_RootName);
	m_profile.WriteString("path", m_RootPath);
// 2002-5-22 kish, 
	VirtualFolderList * pVFL;
	int i=0;
	int j=0;
/*	while (pos) {
		m_mapVFL.GetNextAssoc(pos, key, pVFL);*/
	for (i=0; i<m_vVFL.size();i++) {// MemoListCount;i++) {
		pVFL = &m_vVFL[i];
		if(pVFL->nItem == ITEM_ROOT) {
			continue; // Item�� Root�� ���� ������ ����ǹǷ� ����
		}
		stmp.Format("VIRTUAL_F_%d",j);
		m_profile.SetSection(stmp);

		m_profile.WriteString("name", pVFL->title.c_str());
		m_profile.WriteString("path", pVFL->path.c_str());
		m_profile.WriteString("nType", itoa( pVFL->nRemoteType, szTmp, 10)); 
		m_profile.WriteString("hostname", pVFL->hostaddr.c_str());
		m_profile.WriteString("userid", pVFL->userid.c_str());
		m_profile.WriteString("password", pVFL->password.c_str());
		j++;
	}
	//������ ���� ""�� ä����� ��
	stmp.Format("VIRTUAL_F_%d",j); // i -> j , '2003.1.4
	m_profile.SetSection(stmp);
	m_profile.WriteString("name", "");
	m_profile.WriteString("path", ""); 

	for (i=0; i<m_vInetf.size();i++) {
		CInetFile * pInetf = m_vInetf[i];
		pInetf->Disconnect();
		delete pInetf;
	}
//	if (MemoListCount)
//		delete [] m_pmemolist;
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= TVS_HASLINES | /*TVS_LINESATROOT |*/  TVS_EDITLABELS |
		TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CMyMemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
//	SetClassLong(m_hWnd, GCL_STYLE, CS_GLOBALCLASS);

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	InitLeftView();
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CMyMemoDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyMemoDoc)));
	return (CMyMemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_imgIcons.Create(IDB_ICONLIST, 16, 1, RGB(0,128,128));
	GetTreeCtrl().SetImageList(&m_imgIcons, TVSIL_NORMAL);

	return 0;
}

int CLeftView::InitLeftView()
{
	CProfilePrivate & m_profile = theApp.m_Profile;
	m_NowEditingItem = NULL;
	//m_profile.SetFilename("MyMemo.ini");
	CString stmp;
	// m_mapClsItem - for image to item, m_mapClsItem2 - for item no. to item
	/*
	m_mapClsItem2[ITEM_ROOT] = theApp.m_mapClsItem[ID_ROOT];
	m_mapClsItem2[ITEM_VIRTUAL_LOCAL] = theApp.m_mapClsItem[ID_VIRTUAL_L];
	m_mapClsItem2[ITEM_VIRTUAL_FTP] = theApp.m_mapClsItem[ID_VIRTUAL_R];
	m_mapClsItem2[ITEM_FOLDER] = theApp.m_mapClsItem[ID_CLOSEDFOLDER];
	m_mapClsItem2[ITEM_FILE_TEXT] = theApp.m_mapClsItem[ID_TEXT_FILE];
	*/
	m_profile.SetSection("Settings");
	m_iAutoSave = m_profile.GetInt("AutoSave", 10); //10�� ���� �ڵ� ���� �⺻
	m_iTrashMode = m_profile.GetInt("TrashMode", 0); //�������� ��� ��뿩�� 
	m_profile.SetSection("ROOT");
	m_profile.GetString("name", stmp, "�޸�");
	m_RootName = stmp;
	m_profile.GetString("path", stmp, ".");
	m_RootPath = stmp;


	int i=0;
		VirtualFolderList * pVFL;

	i=0;
	BOOL b;
	while(1) {
		stmp.Format("VIRTUAL_F_%d",i);
		m_profile.SetSection(stmp);
		m_profile.GetString("name", stmp, "");
		if (stmp == "")
			break;
		VirtualFolderList VFL;
		pVFL = &VFL; //new VirtualFolderList;
		pVFL->title = stmp;
		m_profile.GetString("path", stmp, "");
		pVFL->path = stmp;
		m_profile.GetString("nType", stmp, "");
		pVFL->nRemoteType = atoi(stmp);
		m_profile.GetString("hostname", stmp, "");
		pVFL->hostaddr = stmp;
		m_profile.GetString("userid", stmp, "");
		pVFL->userid = stmp;
		m_profile.GetString("password", stmp, "");
		pVFL->password = stmp;
		//m_profile.GetString("nItem", stmp, "1");
		//pVFL->nItem = atoi(stmp);
		pVFL->nItem = ITEM_VIRTUAL_LOCAL; //Defaul is local virtual
		if (pVFL->nRemoteType == REMOTE_FTP || pVFL->nRemoteType == REMOTE_HTTP) {
			pVFL->nItem = ITEM_VIRTUAL_FTP;
			if (pVFL->pInetf == NULL) {
				pVFL->pInetf = new CInetFile;
				m_vInetf.push_back(pVFL->pInetf);
			}
			string url;
			if (pVFL->nRemoteType == REMOTE_FTP) {
				url = "ftp://" + pVFL->hostaddr;
			}
			b = pVFL->pInetf->Connect(url.c_str(), pVFL->userid.c_str(), pVFL->password.c_str(), TRUE); //���� �������� �ʰ� ������ �ʱ�ȭ �Ѵ�.
		}
		m_vVFL.push_back(VFL);
		i++;
	}

	CString str;
	m_memoTree.Init(&GetTreeCtrl());

	HTREEITEM hItem;
	hItem = GetTreeCtrl().InsertItem(m_RootName, ID_ROOT, ID_ROOT);
	GetTreeCtrl().SetItemData(hItem, (DWORD)hItem);
	// GetPathFromNode��� VirtualFolderList�� ���� Root������ ������ �ϱ����� �ӽ÷� ����
		VirtualFolderList RootVFL;
		pVFL = &RootVFL; //new VirtualFolderList;
		pVFL->title = m_RootName;
		pVFL->path = m_RootPath;
		pVFL->nRemoteType = 0;
		pVFL->hostaddr = "";
		pVFL->userid = "";
		pVFL->password = "";
		pVFL->hItem = hItem;
		pVFL->nItem = ITEM_ROOT;
		m_vVFL.push_back(RootVFL);

	m_memoTree.AddDir(hItem, m_RootPath, m_iSortMode, &RootVFL);
	fUpdateVirtualFolders();
	GetTreeCtrl().Expand(hItem, TVE_EXPAND);//���� ��Ʈ�� ���� Virtual Folder�� �⺻������ �����ش�.
	m_profile.SetSection("Settings");
	m_profile.GetString("Ver", str, ""); //10�� ���� �ڵ� ���� �⺻
	CString nowVer;
	nowVer = theApp.GetVersion();
	if (nowVer != str) {
		m_profile.WriteString("Ver",nowVer);
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_ABOUT, 0);
	}
	return TRUE;
}

//DEL void CLeftView::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
//DEL {
//DEL }

// ���� ������ : 2002.5.31
// Update :
// ���� ����� Item.hItem�� Null�̸� return�Ͽ� �״� �� ����(������ �ƴѰ� Ŭ����)
void CLeftView::SelChanged(TVITEM ItemOld, TVITEM ItemNew, BOOL bAddRML)
{
	int iImage=ItemNew.iImage;//, iSelImg;
	//GetTreeCtrl().GetItemImage(hItemNew, iImage, iSelImg);
	if (ItemNew.hItem == NULL)
		return;
	LPARAM l = GetTreeCtrl().GetItemData(ItemNew.hItem);
	TRACE1("Selected LParam = %x\n", l);
	BeginWaitCursor();
	((CMyMemoView *)GetDocument()->m_viewMain)->SaveViewFile();
	EndWaitCursor();
	string stdext, stdsVPath, stdstrSrc;
	CString ext, sVPath;
	CString sTemp;
	VirtualFolderList * pVFL;
	CString strSrc = m_memoTree.GetPathFromNode(ItemNew.hItem, this, &ext, &sVPath, &pVFL);
	string SEP;
	SEP = pVFL->GetDirSeparator();//"\\"; 
/*	if (pVFL->nRemoteType == REMOTE_FTP || pVFL->nRemoteType == REMOTE_HTTP) {
		SEP = "/";
	}*/
	stdstrSrc = strSrc;
	int newImage = iImage;
	if (iImage >= ID_ROOT/*ID_VIRTUAL_L*/ && iImage <= ID_OPENFOLDER) { //�����̸� ������ �������� ������ ������ �����ش�.
		int pos=0;
		string stdstrTemp;
		while(1) {
			char buffer[255];
			if (!PARSE_GetNextToken(buffer, DirRemarkFILEList, "|", &pos)) {
				 //remark���� �� �ش��ϴ� ������ �ϳ��� ������ �������� ���� ���
				stdstrTemp = m_vDirRemarkFILEList[0];
				strSrc = (stdstrSrc + SEP + stdstrTemp).c_str();
				break;
			}
			stdstrTemp = buffer; //STR_GetNextToken(DirRemarkFILEList, "|", &pos);
			strSrc = (stdstrSrc + SEP + stdstrTemp).c_str();
			if	(FileExists(strSrc, pVFL->pInetf))
				break;
		}
		//strSrc += DirRemarkFILE;
		newImage = ID_TEXT_FILE;
	} else {
		if (ext != "") {
			strSrc += ("." + ext);//txt"; //AfxMessageBox(strSrc);
		} else {
			;//((CMyMemoView *)GetDocument()->m_viewMain)->GetEditCtrl().SetWindowText("");
		}
	}
	stdstrSrc = strSrc;
	stdsVPath = sVPath;
	stdext = ext;
	m_TIMERDocOpen.newImage = newImage;
	m_TIMERDocOpen.strSrc = strSrc;
	m_TIMERDocOpen.sVPath = sVPath;
	m_TIMERDocOpen.pVFL = pVFL;
	KillTimer(0);
	SetTimer(0, 200, NULL); //0�� �̺�Ʈ - OpenMemo
}


int CLeftView::Itemexpand(HTREEITEM hItem, int iImage, UINT action)
{
	LRESULT Result = FALSE;
	// Ȯ��ǰų� ��ҵ� ��尡 ��Ÿ���� ���丮���� ����
	CString str;
	VirtualFolderList *pVFL;
	str = m_memoTree.GetPathFromNode(hItem, this, NULL, NULL, &pVFL);
	TRACE0(str+"\n");
	if (GetTreeCtrl().GetRootItem() == hItem)
		return Result;
	// ��尡 Ȯ��Ǵ� ���
	if(action == TVE_EXPAND)
	{
		// �ռ� �߰��ߴ� ���� ��� ����
		m_memoTree.DeleteAllChildren(hItem); //modified from DeleteFirstChild, �����ִ� ������� ���� �� �����Ƿ� ��� ����� ������ ���� '2002.12.28
		// ��¥ ���丮 ������ �о� ǥ��
		BeginWaitCursor();
		if(m_memoTree.AddDir(hItem, str, m_iSortMode, pVFL) == 0)
			Result = TRUE;
		EndWaitCursor();
	}
	else	// ��尡 ��ҵǴ� ���
	{
		// ��� ���� ��带 ����
		m_memoTree.DeleteAllChildren(hItem);
		// ���ڰ� ����� ��ư ǥ�ø� ���� ���� ��� �߰�
		if(GetTreeCtrl().GetParentItem(hItem) == NULL)
			GetTreeCtrl().InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
		else	// ���� ���丮 ���� ���� Ȯ��
			m_memoTree.SetButtonState(hItem, str, pVFL);
	}
	return Result;
}

void CLeftView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// Ȯ��ǰų� ��ҵ� ��带 ����
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	int iImage = pNMTreeView->itemNew.iImage;
	// Ȯ��ǰų� ��ҵ� ��尡 ��Ÿ���� ���丮���� ����
	*pResult = Itemexpand(hItem, iImage, pNMTreeView->action);

}

LRESULT CLeftView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	/* MoveFile �Լ� ���� ���ǻ���
	MoveFile �Լ��� Rename��ɱ��� �����ϱ� ������ �̵��� ������ ���� ���� �̸����� �Ȱ��� ���־�� �Ѵ�.
	���� �̸��� �ٲܷ��� �̵��� ���� �н����� �����̸��� �ٲ۴�.
	������ �̵��� ��� �̵��� ���� ���� + �����̸� ,�̵��� ���� + �����̸� ���� ���־�� �Ѵ�.
	*/
	if (message == WM_KEYDOWN) {
		if (VK_DELETE == (int)wParam) {
			PostMessage(WM_COMMAND, ID_DELETE, 0);
		} else
		if (VK_INSERT == (int)wParam) {
			if (GetKeyState(VK_CONTROL)<0) {
				PostMessage(WM_COMMAND, ID_MEMO_NEW_FOLDER, 0);
			} else {
				PostMessage(WM_COMMAND, ID_MEMO_NEW_MEMO, 0);
			}
		} /*else {
		UINT uCode; 
		uCode = MapVirtualKey( wParam, 0 ); 
		if (uCode & VK_SHIFT) {
			short a = VK_SHIFT;}
		if (uCode & VK_INSERT) {
			short a = VK_INSERT;
		}
		short si= GetAsyncKeyState(VK_SHIFT);
		short si3 = GetAsyncKeyState(VK_INSERT);
		if (si3 != 0) {
			short si2 = si;
		}
		}
	*/	
	}
	if (message == UM_NEEDSAVE) {
		KillTimer(1);
		if (m_iAutoSave > 0)
			SetTimer(1, m_iAutoSave * 1000, NULL);
	}
	if (message == UM_FORCESAVE) { //���� �̸��� �ٲ� �� �� ������ �ʿ��� ��
		BeginWaitCursor();
		((CMyMemoView *)GetDocument()->m_viewMain)->SaveViewFile();
		EndWaitCursor();
	}
	if (message == UM_ITEMRESELECT) {
		HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
		HTREEITEM hItemParent = GetTreeCtrl().GetParentItem(hItem);
		GetTreeCtrl().Select(hItemParent, TVGN_CARET);
		GetTreeCtrl().Select(hItem, TVGN_CARET);
	}
	if (message == UM_ITEMDROPED) {
		VirtualFolderList * pSrcVFL;
		VirtualFolderList * pTgtVFL;
		CString strSrc = m_memoTree.GetPathFromNode((HTREEITEM)wParam, this, NULL, NULL, &pSrcVFL);
		CString strTgt = m_memoTree.GetPathFromNode((HTREEITEM)lParam, this, NULL, NULL, &pTgtVFL);
		TV_ITEM item;
		item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE;
		item.hItem = (HTREEITEM)wParam;
		GetTreeCtrl().GetItem(&item);
		CString strfname = CParseKi::GetFileName(strSrc, pSrcVFL->GetDirSeparator());
		CString &ext = GetViewfileExt(item.iImage);
		if (ext != "") {
			strfname += ("." + ext);
			strSrc += ("." + ext);
		}
		HTREEITEM m_hDraggedItem = (HTREEITEM)wParam;
		HTREEITEM hTargetItem = (HTREEITEM)lParam;
		
		CString strTgtfullpath = CParseKi::AddDirSeparator(strTgt, pTgtVFL->GetDirSeparator()).c_str() + strfname; // sPathAddBackSlash(strTgt);
		strTgt.ReleaseBuffer();
			string stringSrc = strSrc;
			string stringTgtfullpath = strTgtfullpath;
			string stringfname = strfname;

		BOOL bret;
		if (strSrc != strTgtfullpath) //������ �������� �ٸ� ��쿡�� �̵�
			if (pSrcVFL->nRemoteType == REMOTE_FTP) {
				bret = pSrcVFL->pInetf->RenameFile(strSrc, strTgtfullpath);
			} else if(pSrcVFL->nRemoteType == REMOTE_LOCAL) {
				bret = MoveFile(strSrc,strTgtfullpath);
			}
			if (!bret)
				AfxMessageBox("Error Occured in File Moving");
			else { //������
				item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE|TVIF_STATE ;
				item.hItem = hTargetItem;
				GetTreeCtrl().GetItem(&item);
				GetTreeCtrl().Select(hTargetItem, TVGN_CARET);
				if (item.state & TVIS_EXPANDED )
					CopyItemProperties(m_hDraggedItem, hTargetItem);
				else 
					//������ �����ۿ� �ڽ� �������� ���� ��츸 SetButtonState�� ����
					if (!GetTreeCtrl().ItemHasChildren(hTargetItem)) 
						m_memoTree.SetButtonState(hTargetItem, strTgt, pTgtVFL); //strTgt�� Drop�� ���� path, strTgtfullpath�� �� �Ʒ� ������ ���� path�� ����

				GetTreeCtrl().DeleteItem(m_hDraggedItem); 
			}
	}
	return CTreeViewExt::WindowProc(message, wParam, lParam);
}

CString CLeftView::GetViewfileExt(int num)
{
	for (int i=0;i<ViewFileListCount;i++) {
		if (viewfilelist[i].imgNum == num) {
			return viewfilelist[i].szExt;
		}
	}
	return ""; //�´°� ������ ""
}

int CLeftView::GetViewfileImgNum(LPCTSTR szExt)
{
	for (int i=0;i<ViewFileListCount;i++) {
		if (stricmp(viewfilelist[i].szExt, szExt) == 0) {
			return viewfilelist[i].imgNum;
		}
	}
	return -1; // �´°� ������ -1
}

#define CHECKITEM_NULL(hItem) if(hItem == NULL) { \
		pCmdUI->Enable(FALSE); \
		return; \
	}
void CLeftView::OnUpdateMemoNewVirtual(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	CHECKITEM_NULL(hItem);
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_IMAGE;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	int iImage = item.iImage;//pNMTreeView->itemNew.iImage;
	pCmdUI->Enable(iImage == ID_ROOT);
}

void CLeftView::OnUpdateMemoNewFolder(CCmdUI* pCmdUI) 
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	CHECKITEM_NULL(hItem);
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_IMAGE;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	int iImage = item.iImage;//pNMTreeView->itemNew.iImage;
//	pCmdUI->Enable((iImage >= ID_VIRTUAL_L) && (iImage <= ID_OPENFOLDER)); //������ ��쿡�� enable
	pCmdUI->Enable(iImage >= ID_ROOT); //������ ��쳪 �޸��� ���
}

void CLeftView::OnUpdateMemoNewMemo(CCmdUI* pCmdUI) 
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	CHECKITEM_NULL(hItem);
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_IMAGE;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	int iImage = item.iImage;//pNMTreeView->itemNew.iImage;
	pCmdUI->Enable(iImage >= ID_ROOT);
}

void CLeftView::OnMemoNewVirtual() 
{
	// TODO: Add your command handler code here
	char szTmp[4096];
	CDlgVirtualFolder2 dlg;
	BOOL b;
	BOOL bFind = FALSE;
	char tmppath[_MAX_PATH];
	if (dlg.DoModal() == IDOK) {
		VirtualFolderList vfl;
		VirtualFolderList * pVFL = &vfl; //new VirtualFolderList;
		pVFL->title = dlg.m_sAlias;
		if (dlg.m_nType == REMOTE_LOCAL) {
			GetCurrentDirectory(sizeof tmppath, tmppath);
			if (SetCurrentDirectory(dlg.m_sPath)) {
				bFind = TRUE;
				SetCurrentDirectory(tmppath);
			}
			pVFL->path = dlg.m_sPath;
			pVFL->nItem = ITEM_VIRTUAL_LOCAL;
		} else 
		if (dlg.m_nType == REMOTE_FTP) {
			if (pVFL->pInetf == NULL) {
				pVFL->pInetf = new CInetFile;
				m_vInetf.push_back(pVFL->pInetf);
			}
			string url;
			url = "ftp://" + dlg.m_sHostname;
			b = pVFL->pInetf->Connect(url.c_str(), dlg.m_sUsername, dlg.m_sPassword, TRUE);
			b = ConnectServer(pVFL);
			if (b) {
				if (pVFL->pInetf->SetCurrentDirectory(dlg.m_sServerdir)) {
					bFind = TRUE;
				}
			}

			pVFL->hostaddr = dlg.m_sHostname;
			pVFL->userid = dlg.m_sUsername;
			pVFL->password = dlg.m_sPassword;
			//wsprintf(szTmp, "ftp://%s%s", dlg.m_sHostname, dlg.m_sServerdir);
			pVFL->path = dlg.m_sServerdir;
			pVFL->nRemoteType = REMOTE_FTP;
			pVFL->nItem = ITEM_VIRTUAL_FTP;
//			pVFL->pInetf = pinetf; //&m_vInetf.at(m_vInetf.size()-1);
		}
		pVFL->NeedUpdatedNow = TRUE;
		if (!bFind)
			AfxMessageBox(MyLoadString(IDS_CANNOTFOUNDPATH));

		/*
		if (strnicmp(pVFL->path.c_str(), "ftp:", 4) == 0) {
			pVFL->nRemoteType = REMOTE_FTP;
		}*/

		int nIcon = (vfl.nRemoteType == 0)?ID_VIRTUAL_L:ID_VIRTUAL_R;
		HTREEITEM l_hItem =  GetTreeCtrl().InsertItem(TVIF_IMAGE | TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM , 
			vfl.title.c_str(), nIcon, nIcon, 0, 0, (LPARAM)0, GetTreeCtrl().GetRootItem(), TVI_LAST);
		pVFL->hItem = l_hItem;
		m_vVFL.push_back(vfl);

		CString str = GetTreeCtrl().GetItemText(l_hItem);
		BOOL b = GetTreeCtrl().SetItemData(l_hItem, (DWORD)l_hItem);


		LPARAM l = GetTreeCtrl().GetItemData(l_hItem);
		//GetTreeCtrl().Select(l_hItem, TVGN_CARET);
		fUpdateVirtualFolders();
	}
}

// Update
// 2002.6.27
// MyExpand(hItem, TVE_EXPAND); �� ������ �Ű� ���� �� �����ִ� ���¿��� �� �޸� ������ ������ �ذ�
void CLeftView::OnMemoNewMemo() 
{
	static const char NEWMEMONAME []= "New Memo ";
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	// ���� �������� �޸��̸� �θ� ���� �Ʒ� �������� ����
	// ���� �������� �����̸� ���� ������ �Ʒ� ������ ����
	int iImage = item.iImage;
	if (iImage >= ID_TEXT_FILE) {
		hItem = GetTreeCtrl().GetParentItem(hItem);
		item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
		item.hItem = hItem;
		GetTreeCtrl().GetItem(&item);
	}

	int nType = 0;
	int max=0;
	BOOL bFind=FALSE;
	BOOL b = MyExpand(hItem, TVE_EXPAND); //���� Item�� �߰��� �� Item�� Ȯ���ؾ� �ϹǷ� ��ġ�� �������� ���� ����� �ű�
	HTREEITEM hchildItem = GetTreeCtrl().GetChildItem(hItem);
	while (hchildItem != NULL) {
		CString &strname = GetTreeCtrl().GetItemText(hchildItem);
		if (strnicmp(strname, NEWMEMONAME, strlen(NEWMEMONAME)) == 0) {
			int n = atoi(strname.Mid(strlen(NEWMEMONAME)));
			if (max < n)
				max = n;
		}
		hchildItem =GetTreeCtrl().GetNextSiblingItem(hchildItem);
	}
	max++;
	CString str,ext;
	str.Format("%s%d",NEWMEMONAME, max) ;
	hchildItem = GetTreeCtrl().InsertItem(str, ID_TEXT_FILE+nType, ID_TEXT_FILE+nType, hItem);
	GetTreeCtrl().SetItemData(hchildItem, (DWORD)hchildItem);
	VirtualFolderList * pVFL;
	CString strSrc = m_memoTree.GetPathFromNode((HTREEITEM)hchildItem, this, &ext, NULL, &pVFL); //CString �Ű������� �߰��� �־�� �޸��� ��� Ȯ���ڸ� �����Ͽ� ���´�.
	if (ext !="") strSrc += "." + ext;
	if (pVFL->nRemoteType > 0) {
		BOOL bOpen = pVFL->pInetf->OpenFile(strSrc, "w");
		if (bOpen) {
			pVFL->pInetf->CloseFile();
		}
	} else {
		CFile cf;
		cf.Open(strSrc, CFile::modeCreate);
		cf.Close();
	}
	if (!b)
		MyExpand(hItem, TVE_EXPAND); //Child�� ���� �ϳ� �϶� (�տ��� MyExpand�������� ��) 2002.7.21
	GetTreeCtrl().Select(hchildItem, TVGN_CARET);
	GetTreeCtrl().EditLabel(hchildItem);	
	//m_NowEditingItem = hchildItem;
	//m_prevEditFoldername = str;
	// Ʈ���� �ϴ� ��ġ��
	// Ʈ������� �������� �߰��ϰ� - �̸��� ���� ���� ���� "������" + 1,2,3...
	// �����۸��� �ٲ� �� �ִ� ���·� �ΰ� ��ħ.

	// ������ �� �ٲ� �� �� �Լ��� ���� Enter�� ġ�� ������ �������� ������ �������μ� �� �Լ� ����� �Ϸ�
	// ������ ���� �ٲ� �� ESC�� ������ ���� �۾����� ��� �����ν� �������� �������� �ʰ� �� �� ���� ������ ������ �������� ����
}

// Update
// 2002.6.27
// MyExpand(hItem, TVE_EXPAND); �� ������ �Ű� ���� �� �����ִ� ���¿��� �� ���� ������ ������ �ذ�
void CLeftView::OnMemoNewFolder() 
{
	static const char NEWFOLDERNAME []= "New Folder ";
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	// ���� �������� �޸��̸� �θ� ���� �Ʒ� �������� ����
	// ���� �������� �����̸� ���� ������ �Ʒ� ������ ����
	int iImage = item.iImage;
	if (iImage >= ID_TEXT_FILE) {
		hItem = GetTreeCtrl().GetParentItem(hItem);
		item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
		item.hItem = hItem;
		GetTreeCtrl().GetItem(&item);
	}
	

	int max=0;
	BOOL bFind=FALSE;
	BOOL b = MyExpand(hItem, TVE_EXPAND);	
	HTREEITEM hchildItem = GetTreeCtrl().GetChildItem(hItem);
	while (hchildItem != NULL) {
		CString &strname = GetTreeCtrl().GetItemText(hchildItem);
		if (strnicmp(strname, NEWFOLDERNAME, strlen(NEWFOLDERNAME)) == 0) {
			int n = atoi(strname.Mid(strlen(NEWFOLDERNAME)));
			if (max < n)
				max = n;
		}
		hchildItem =GetTreeCtrl().GetNextSiblingItem(hchildItem);
	}
	max++;
	CString str;
	str.Format("%s%d",NEWFOLDERNAME, max) ;
	hchildItem = GetTreeCtrl().InsertItem(str, ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
	GetTreeCtrl().SetItemData(hchildItem, (DWORD)hchildItem);
	VirtualFolderList * pVFL;
	CString strSrc = m_memoTree.GetPathFromNode((HTREEITEM)hchildItem, this, NULL, NULL, &pVFL);
	if (pVFL->nRemoteType == REMOTE_LOCAL) {
		CreateDirectory(strSrc, NULL);
	} else
	if (pVFL->nRemoteType == REMOTE_FTP || pVFL->nRemoteType == REMOTE_HTTP) {
		pVFL->pInetf->CreateDirectory(strSrc);
	}
	if (!b)
		MyExpand(hItem, TVE_EXPAND); //Child�� ���� �ϳ� �϶� (�տ��� MyExpand�������� ��)	
	GetTreeCtrl().Select(hchildItem, TVGN_CARET);
	GetTreeCtrl().EditLabel(hchildItem);	
	//m_NowEditingItem = hchildItem;
	//m_prevEditFoldername = str;
	// Ʈ���� �ϴ� ��ġ��
	// Ʈ������� �������� �߰��ϰ� - �̸��� ���� ���� ���� "������" + 1,2,3...
	// �����۸��� �ٲ� �� �ִ� ���·� �ΰ� ��ħ.

	// ������ �� �ٲ� �� �� �Լ��� ���� Enter�� ġ�� ������ �������� ������ �������μ� �� �Լ� ����� �Ϸ�
	// ������ ���� �ٲ� �� ESC�� ������ ���� �۾����� ��� �����ν� �������� �������� �ʰ� �� �� ���� ������ ������ �������� ����
}

BOOL CLeftView::CheckConfirmNewName(CString newItemText)
{
	BOOL bret = FALSE;
//	if (m_NowEditingItem) {
	if (newItemText != "") {
		CString str, ext;
		VirtualFolderList * pVFL;
		CString strSrc = m_memoTree.GetPathFromNode((HTREEITEM)m_NowEditingItem, this, &ext, NULL, &pVFL);
		
		if (ext !="") strSrc += "." + ext;
		CString strTgt = CParseKi::GetFilePath(strSrc, pVFL->GetDirSeparator());
		if (strTgt == "") // path + new file name - only path not blank
			strTgt = newItemText;
		else 
			strTgt += (pVFL->GetDirSeparator() + newItemText);
		if (ext != "")
			strTgt += ("." + ext);
		if (newItemText != m_prevEditFoldername) {
			if(pVFL->nRemoteType == REMOTE_LOCAL) {
				bret = MoveFile(strSrc, strTgt); //Rename
			} else 
			if(pVFL->nRemoteType == REMOTE_FTP) {
				bret = pVFL->pInetf->RenameFile(strSrc, strTgt);
			}
			//���丮 ����
		}
		//m_NowEditingItem = NULL;
	}
	return bret;
}

void CLeftView::OnUpdateDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	CHECKITEM_NULL(hItem);
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_IMAGE;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	int iImage = item.iImage;//pNMTreeView->itemNew.iImage;
	pCmdUI->Enable(iImage > ID_ROOT);	
}

void CLeftView::OnUpdateRename(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	CHECKITEM_NULL(hItem);
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_IMAGE;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	int iImage = item.iImage;//pNMTreeView->itemNew.iImage;
	pCmdUI->Enable(CANEDITITEM(iImage));		
}

void CLeftView::OnDelete() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	int nImage, nSelImg;
	GetTreeCtrl().GetItemImage(hItem, nImage, nSelImg);
	int ret;
	if (nImage >= ID_VIRTUAL_L && nImage <= ID_VIRTUAL_R) {
		ret = AfxMessageBox(MyLoadString(IDS_DELETEVIRTUAL_CONFIRM), MB_YESNO);
		if (ret == IDYES) {
			int a = m_vVFL.size();
			VirtualFolderList * pVFL = GetVFL(&m_vVFL, hItem);
			m_vVFL.erase(pVFL);
			a = m_vVFL.size();
			GetTreeCtrl().DeleteItem(hItem);
		}
	} else {	
		ret = AfxMessageBox(MyLoadString(IDS_DELETE_CONFIRM), MB_YESNO);
		if (ret == IDYES) {
			CString ext;
			VirtualFolderList * pVFL;
			CString strSrc = m_memoTree.GetPathFromNode((HTREEITEM)hItem, this, &ext, NULL, &pVFL);
			if (ext !="") strSrc += "." + ext;
			int nCount = 0;
			if (nImage >= ID_VIRTUAL_L && nImage <= ID_OPENFOLDER)
				nCount = CDirNFile::DelTreeFiles(strSrc, TRUE, pVFL->pInetf); //���丮���� ����
			else if (nImage >= ID_TEXT_FILE)
				if (pVFL->nRemoteType == REMOTE_LOCAL) {
					nCount = DeleteFile(strSrc);
				} else {
					nCount = pVFL->pInetf->DeleteFile(strSrc);
				}
			if (nCount)
				GetTreeCtrl().DeleteItem(hItem);
		}
	}

}

void CLeftView::OnRename() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	GetTreeCtrl().EditLabel(hItem);
}

BOOL CLeftView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTreeViewExt::PreTranslateMessage(pMsg);
}

void CLeftView::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTreeViewExt::PreSubclassWindow();
}

//DEL int CLeftView::_AddArrayVirtualFolder(CStringArray &names, CStringArray &paths)
//DEL {
//DEL 		CStringArray tmpnames, tmppaths;
//DEL 		CDWordArray tmphitems;
//DEL 		for (int i=0;i<MemoListCount;i++) { //��� �迭�� ���� �ӽ÷οŰܵξ��ٰ�..
//DEL 			tmpnames.Add(m_pmemolist[i].name);
//DEL 			tmppaths.Add(m_pmemolist[i].path);
//DEL 			tmphitems.Add((DWORD)m_pmemolist[i].hItem);
//DEL 		}
//DEL 		if (m_pmemolist) {
//DEL 			delete [] m_pmemolist;
//DEL 		}
//DEL 		int addsize = names.GetSize();
//DEL 		m_pmemolist = new MemoList[MemoListCount+addsize];
//DEL 		for (i=0;i<MemoListCount;i++) { // ���� ũ�⸦ �ø� �� �ٽ� ������� �ǵ�����.
//DEL 			m_pmemolist[i].name = tmpnames[i];
//DEL 			m_pmemolist[i].path = tmppaths[i];
//DEL 			m_pmemolist[i].hItem = (HTREEITEM)tmphitems[i];
//DEL 		}
//DEL 		for (i=MemoListCount;i<MemoListCount+addsize;i++) {
//DEL 			m_pmemolist[i].name = names[i-MemoListCount];
//DEL 			m_pmemolist[i].path = paths[i-MemoListCount];
//DEL 			m_pmemolist[i].hItem = 0;
//DEL 		}
//DEL 		MemoListCount += addsize;
//DEL 		return MemoListCount;
//DEL }


void CLeftView::fUpdateVirtualFolders(int * nUpdateds)
{
	HTREEITEM hItem = GetTreeCtrl().GetRootItem(); //��Ʈ Item�� �̸� �����Ǿ� �־�� ��
	int i;
	for (i=0;i<m_vVFL.size();i++) {
		VirtualFolderList & vfl = m_vVFL[i];
		if(vfl.nItem == ITEM_ROOT) {
			continue; // Item�� Root�� ���� ������ ����ǹǷ� ����
		}		
		HTREEITEM & l_hItem = vfl.hItem;
		if (l_hItem == 0) {
			int nIcon = (vfl.nRemoteType == 0)?ID_VIRTUAL_L:ID_VIRTUAL_R;
			l_hItem =  GetTreeCtrl().InsertItem(TVIF_IMAGE | TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM , 
				vfl.title.c_str(), nIcon, nIcon, 0, 0, (LPARAM)i, hItem, TVI_LAST);
			// 2002-5-22 kish, m_mapVFL�� ������� ����
			vfl.hItem = l_hItem;
			GetTreeCtrl().SetItemData(l_hItem, (DWORD)l_hItem);

			m_memoTree.SetButtonState(vfl.hItem, vfl.path.c_str(), &vfl);
		} else { //�̹� �����ϴ� ������
			ClsItem * pClsItem = &theApp.m_mapClsItems[vfl.nItem];
			GetTreeCtrl().SetItem(vfl.hItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE|TVIF_TEXT, 
				vfl.title.c_str(), pClsItem->iImage, pClsItem->iSelectedImage, 0, 0, (LPARAM)0); // It's realy fatal bug, don't set to i index into lparam- it caused sort error
			//if ((nUpdateds != NULL) && nUpdateds[i]) { //Updated �� �͸� 
			if (vfl.NeedUpdatedNow) {
				m_memoTree.RefreshFolderItem(vfl.hItem, vfl.path.c_str(), &vfl);
				vfl.NeedUpdatedNow = FALSE;
			}
		}
	}
	fSortTreeItem(hItem);
}

void CLeftView::OnProperty() 
{
	CDlgVirtualFolder2 dlg;
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	DWORD i = GetTreeCtrl().GetItemData(hItem);
	VirtualFolderList * pVFL;
	int nPrevRemoteType;
	string prevHostaddr;
	pVFL = GetVFL(&m_vVFL, hItem); 
	if (GetTreeCtrl().GetRootItem() == hItem) {
		dlg.m_sAlias = m_RootName;
		dlg.m_sPath = m_RootPath;
		nPrevRemoteType = 0;
	} else {
		// 2002-5-22 kish, m_mapVFL�� ���� ����
		nPrevRemoteType	= pVFL->nRemoteType;
		dlg.m_sAlias = pVFL->title.c_str();// m_pmemolist[i].name;
		dlg.m_nType = pVFL->nRemoteType;
		if (pVFL->nRemoteType == REMOTE_LOCAL) {
			dlg.m_sPath = pVFL->path.c_str(); // m_pmemolist[i].path;
		} else {
			dlg.m_sHostname = pVFL->hostaddr.c_str();
			dlg.m_sUsername = pVFL->userid.c_str();
			dlg.m_sPassword = pVFL->password.c_str();
			dlg.m_sServerdir = pVFL->path.c_str();
			prevHostaddr = pVFL->hostaddr;
		}
	}
	BOOL b;
	if (dlg.DoModal() == IDOK) {
		if ( nPrevRemoteType == REMOTE_FTP && 
			(dlg.m_nType == REMOTE_LOCAL || prevHostaddr.c_str() != dlg.m_sHostname) ) { 
			//�������� FTP(�Ǵ� inet�� ���� �ٸ�..., ����� ���� �Ǵ� host addr�� �ٲ�� 
			pVFL->pInetf->Disconnect();
		}
		if (GetTreeCtrl().GetRootItem() == hItem) {
			m_RootName = dlg.m_sAlias;
			m_RootPath = dlg.m_sPath;
			GetTreeCtrl().SetItemText(hItem, m_RootName);
			pVFL->path = dlg.m_sPath;// m_pmemolist[i].path = dlg.m_sPath;

		} else {
			pVFL->title = dlg.m_sAlias;// m_pmemolist[i].name = dlg.m_sAlias;
			if(dlg.m_nType == REMOTE_LOCAL) {
				pVFL->path = dlg.m_sPath;// m_pmemolist[i].path = dlg.m_sPath;
			} else 
			if(dlg.m_nType == REMOTE_FTP) {
				if (pVFL->pInetf == NULL) {
					pVFL->pInetf = new CInetFile;
					m_vInetf.push_back(pVFL->pInetf);
				}
				GetTreeCtrl().SetItemText(hItem, pVFL->title.c_str());
				string url;
				url = "ftp://" + dlg.m_sHostname;
				b = pVFL->pInetf->Connect(url.c_str(), dlg.m_sUsername, dlg.m_sPassword, TRUE);
				// b = ConnectServer(pVFL); //Onrefresh���� �� �۾��� ��
				/*if (pVFL->pInetf->SetCurrentDirectory(dlg.m_sServerdir)) {
				}*/
				pVFL->hostaddr = dlg.m_sHostname;
				pVFL->userid = dlg.m_sUsername;
				pVFL->password = dlg.m_sPassword;
				//wsprintf(szTmp, "ftp://%s%s", dlg.m_sHostname, dlg.m_sServerdir);
				pVFL->path = dlg.m_sServerdir;
			}
		}
		OnRefresh();
	}
}

void CLeftView::OnUpdateProperty(CCmdUI* pCmdUI) 
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	CHECKITEM_NULL(hItem);
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_IMAGE;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	int iImage = item.iImage;//pNMTreeView->itemNew.iImage;
	ClsItem * pCI = theApp.GetClsItem(iImage);
	
	pCmdUI->Enable(pCI->nKind & KIND_VIRTUAL);//iImage >= ID_ROOT && iImage <= ID_VIRTUAL_R);
}

BOOL CLeftView::MyExpand(HTREEITEM hItem, UINT nCode)
{
	BOOL b = GetTreeCtrl().Expand(hItem, nCode);
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	BOOL bOK= FALSE;
	HTREEITEM hCItem = GetTreeCtrl().GetChildItem(hItem);
	if (item.state & TVIS_EXPANDED) {
		//ù �ڽ� �������� ���������̸� - �� �������� expand ó���� ����� ���� �ʾ�����
		if (hCItem && GetTreeCtrl().GetItemText(hCItem)=="") { 
			bOK = TRUE; 
		}
		TRACE0("EXPANDED");
	}
	else {
		//ù �ڽ� �������� �Ϲ� �������̸� - �� Collapse ó���� ����� ���� �ʾ�����
		if (hCItem && GetTreeCtrl().GetItemText(hCItem)!="") {
			bOK = TRUE;
		}
		TRACE0("COLLAPSED");
	}
	if (bOK) Itemexpand(hItem, 0, nCode);
	return b;

}

// 
void CLeftView::OnRefresh() 
{
	CTreeCtrl * ptreeCtrl = &GetTreeCtrl();
	HTREEITEM hItem = ptreeCtrl->GetSelectedItem();
	HTREEITEM hItem2 = hItem;
	int nImage, nSelImg;
	ptreeCtrl->GetItemImage(hItem, nImage, nSelImg);
	VirtualFolderList *pVFL = GetVFL(&m_vVFL, hItem);	
	HTREEITEM hParent; 
	while(!pVFL && (hParent = ptreeCtrl->GetParentItem(hItem2)) != NULL)
	{
		TV_ITEM item;
		item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE;
		item.hItem = hParent;
		ptreeCtrl->GetItem(&item);
		if (item.iImage <= ID_VIRTUAL_R) { // Virtual folder�̰ų� ��Ʈ�̸�
			pVFL = GetVFL(&m_vVFL, item.hItem);
			break;
		}
		hItem2 = hParent;
	}
	//Refresh�� ���� Folder�̸� ���� ������ Refresh�Ѵ�.
	if (nImage >= ID_VIRTUAL_L && nImage <= ID_OPENFOLDER) {
		BOOL b;
		if (pVFL->nRemoteType == REMOTE_LOCAL) {
			CString strSrc = m_memoTree.GetPathFromNode(hItem, this);
			m_memoTree.RefreshFolderItem(hItem, strSrc, pVFL);
		}
		if (pVFL->nRemoteType > REMOTE_LOCAL) {
			b = ConnectServer(pVFL); //Virtual Folder�� ���õǾ� ������ ������ ������
		}
	}
	if (ptreeCtrl->GetRootItem() == hItem) {
		DeleteAllChildrenExceptVirtualFolder(&GetTreeCtrl(), hItem); 
		m_memoTree.AddDir(hItem, m_RootPath, m_iSortMode, pVFL);
	}
	fSortTreeItem(hItem);

}
//DEL void CLeftView::OnUpdateFileMruMemo(CCmdUI* pCmdUI) 
//DEL {
//DEL 	// TODO: Add your command update UI handler code here
//DEL //	int size = GetDocument()->m_RecentMemoList.GetSize();
//DEL //	pCmdUI->Enable(size > 0);
//DEL }


void CLeftView::doFileMruMemo(int nID) 
{
	CMenu *pMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(0);
	char sz[255];
	MENUITEMINFO menuInfo;
	menuInfo.cbSize = sizeof menuInfo;
	menuInfo.fMask = MIIM_TYPE;
	menuInfo.cch = sizeof sz;
	menuInfo.dwTypeData = sz;
	pMenu->GetMenuItemInfo(nID, &menuInfo);
	HTREEITEM hItem;
	CString strSrc;
	if (m_memoTree.FindItemByVirtualPath(menuInfo.dwTypeData, &hItem))
	{
		strSrc = m_memoTree.GetPathFromNode(hItem, this);
		GetTreeCtrl().Select(hItem, TVGN_CARET);
	}
}


void CLeftView::OnTimer(UINT nIDEvent) 
{
	BOOL b = FALSE;
	VirtualFolderList * pVFL = m_TIMERDocOpen.pVFL;
	if (nIDEvent == 0) { // OpenMemo
		KillTimer(0);
		BeginWaitCursor();
		b = TRUE; // LOCAL�� ���� TRUE
		if (pVFL->nRemoteType > REMOTE_LOCAL) {
			b = pVFL->pInetf->IsConnect();
			if (!b) { // Connect�� �ȵǾ� �ִٸ�
				b = ConnectServer(pVFL);
			}
		}
		if (b) ((CMyMemoView *)GetDocument()->m_viewMain)->OpenViewFile(m_TIMERDocOpen.newImage, m_TIMERDocOpen.strSrc, &m_TIMERDocOpen.sVPath, TRUE, pVFL);
		EndWaitCursor();
	}
	if (nIDEvent == 1) {
		PostMessage(WM_COMMAND, ID_FILE_SAVE, 0);
		KillTimer(1);
	}
	CTreeViewExt::OnTimer(nIDEvent);
}

void CLeftView::OnDestroy() 
{
	CTreeViewExt::OnDestroy();
	
	KillTimer(0);
	KillTimer(1);
}

BOOL CLeftView::fSortTreeItem(HTREEITEM hItem)
{
	if (m_iSortMode == 1) {
		TVSORTCB Sortcb;
		Sortcb.hParent = hItem;
		Sortcb.lParam = (LPARAM)&GetTreeCtrl(); //��������
		Sortcb.lpfnCompare = &CompareFunc;
		//GetTreeCtrl().SortChildren(hItem);
		GetTreeCtrl().SortChildrenCB( &Sortcb );
		return TRUE;
	}
	return FALSE;

}

void CLeftView::OnUpdateRefresh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();//pNMTreeView->itemOld.hItem;
	CHECKITEM_NULL(hItem);
	pCmdUI->Enable(TRUE);	
}

long CLeftView::ConnectServer(VirtualFolderList *pVFL)
{
	BOOL b;
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	b = pVFL->pInetf->ReConnect(); //������ Connect�� �� �� ����Ǿ���� ��
	if (b) {
		pVFL->pInetf->GetCurrentDirectory(&pVFL->pInetf->m_initDir);
		CString strSrc = m_memoTree.GetPathFromNode(hItem, this);
		CTreeCtrl * ptreeCtrl = &GetTreeCtrl();
		if (hItem != ptreeCtrl->GetRootItem()) // Root�� ���� ����� �ȵȴ�. fUpdateFolder�� ���� �ʿ��� �κи� ó���ؾ� �ȴ�.
			m_memoTree.RefreshFolderItem(hItem, strSrc, pVFL);
		ptreeCtrl->Select(ptreeCtrl->GetRootItem(), TVGN_CARET); // + ǥ�ð� ��Ÿ���� �غ�����
		ptreeCtrl->Select(hItem, TVGN_CARET); //�̻� �ϰ� �� ��带 ����� + �� ���´�.
	} else {
		CString tmp;
		tmp.Format("%s\n%s\n%s", pVFL->pInetf->m_URL.c_str(), MyLoadString(IDS_CANNOTCONNECTSERVER), pVFL->pInetf->m_sErrMsg.c_str());
		AfxMessageBox(tmp);
	}
	return b;
}
