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
//ID_REFRESH메뉴 아이템을 잠시 쓰지 않으므로 아래는 일시적으로 정의
//#define ID_REFRESH WM_USER + 200

//1.0.0.3
// OnMemoNewMemo와 OnMemoNewVirtual에서 아이템을 추가한 후 펼치기 시도하는 것으로 변경
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
	if (!b) return 1; //첫번째 아이템을 뒤로
	b = (ptc->GetItemImage((HTREEITEM)lParam2, nImage2, nSelImg2));
	if (!b) return -1; //두번째 아이템을 뒤로
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
		if (!(nImage >= ID_VIRTUAL_L && nImage <= ID_VIRTUAL_R)) //가상 폴더가 아닌 경우만 삭제
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
			continue; // Item이 Root인 것은 별도로 저장되므로 제외
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
	//마지막 값은 ""로 채워줘야 함
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
	m_iAutoSave = m_profile.GetInt("AutoSave", 10); //10초 간격 자동 저장 기본
	m_iTrashMode = m_profile.GetInt("TrashMode", 0); //쓰레기통 모드 사용여부 
	m_profile.SetSection("ROOT");
	m_profile.GetString("name", stmp, "메모");
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
			b = pVFL->pInetf->Connect(url.c_str(), pVFL->userid.c_str(), pVFL->password.c_str(), TRUE); //직접 연결하지 않고 정보만 초기화 한다.
		}
		m_vVFL.push_back(VFL);
		i++;
	}

	CString str;
	m_memoTree.Init(&GetTreeCtrl());

	HTREEITEM hItem;
	hItem = GetTreeCtrl().InsertItem(m_RootName, ID_ROOT, ID_ROOT);
	GetTreeCtrl().SetItemData(hItem, (DWORD)hItem);
	// GetPathFromNode등에서 VirtualFolderList를 통해 Root정보를 얻어오게 하기위해 임시로 생성
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
	GetTreeCtrl().Expand(hItem, TVE_EXPAND);//최초 루트를 펼쳐 Virtual Folder를 기본적으로 보여준다.
	m_profile.SetSection("Settings");
	m_profile.GetString("Ver", str, ""); //10초 간격 자동 저장 기본
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

// 최종 수정일 : 2002.5.31
// Update :
// 새로 변경된 Item.hItem이 Null이면 return하여 죽는 걸 방지(아이템 아닌곳 클릭시)
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
	if (iImage >= ID_ROOT/*ID_VIRTUAL_L*/ && iImage <= ID_OPENFOLDER) { //폴더이면 하위에 폴더설명 파일이 있으면 보여준다.
		int pos=0;
		string stdstrTemp;
		while(1) {
			char buffer[255];
			if (!PARSE_GetNextToken(buffer, DirRemarkFILEList, "|", &pos)) {
				 //remark파일 중 해당하는 파일이 하나도 폴더에 존재하지 않은 경우
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
	SetTimer(0, 200, NULL); //0번 이벤트 - OpenMemo
}


int CLeftView::Itemexpand(HTREEITEM hItem, int iImage, UINT action)
{
	LRESULT Result = FALSE;
	// 확장되거나 축소된 노드가 나타내는 디렉토리명을 얻음
	CString str;
	VirtualFolderList *pVFL;
	str = m_memoTree.GetPathFromNode(hItem, this, NULL, NULL, &pVFL);
	TRACE0(str+"\n");
	if (GetTreeCtrl().GetRootItem() == hItem)
		return Result;
	// 노드가 확장되는 경우
	if(action == TVE_EXPAND)
	{
		// 앞서 추가했던 더미 노드 제거
		m_memoTree.DeleteAllChildren(hItem); //modified from DeleteFirstChild, 남아있는 잔재들이 있을 수 있으므로 모두 지우는 것으로 변경 '2002.12.28
		// 진짜 디렉토리 구조를 읽어 표시
		BeginWaitCursor();
		if(m_memoTree.AddDir(hItem, str, m_iSortMode, pVFL) == 0)
			Result = TRUE;
		EndWaitCursor();
	}
	else	// 노드가 축소되는 경우
	{
		// 모든 하위 노드를 제거
		m_memoTree.DeleteAllChildren(hItem);
		// 십자가 모양의 버튼 표시를 위해 더미 노드 추가
		if(GetTreeCtrl().GetParentItem(hItem) == NULL)
			GetTreeCtrl().InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
		else	// 서브 디렉토리 존재 유무 확인
			m_memoTree.SetButtonState(hItem, str, pVFL);
	}
	return Result;
}

void CLeftView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// 확장되거나 축소된 노드를 얻음
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	int iImage = pNMTreeView->itemNew.iImage;
	// 확장되거나 축소된 노드가 나타내는 디렉토리명을 얻음
	*pResult = Itemexpand(hItem, iImage, pNMTreeView->action);

}

LRESULT CLeftView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	/* MoveFile 함수 사용시 주의사항
	MoveFile 함수는 Rename기능까지 포함하기 때문에 이동될 폴더는 물론 파일 이름까지 똑같이 써주어야 한다.
	만약 이름을 바꿀려면 이동될 곳의 패스에서 파일이름만 바꾼다.
	폴더를 이동할 경우 이동할 엄마 폴더 + 폴더이름 ,이동될 폴더 + 폴더이름 으로 써주어야 한다.
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
	if (message == UM_FORCESAVE) { //폴더 이름을 바꿀 때 등 저장이 필요할 때
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
		if (strSrc != strTgtfullpath) //원본과 목적지가 다를 경우에만 이동
			if (pSrcVFL->nRemoteType == REMOTE_FTP) {
				bret = pSrcVFL->pInetf->RenameFile(strSrc, strTgtfullpath);
			} else if(pSrcVFL->nRemoteType == REMOTE_LOCAL) {
				bret = MoveFile(strSrc,strTgtfullpath);
			}
			if (!bret)
				AfxMessageBox("Error Occured in File Moving");
			else { //성공시
				item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE|TVIF_STATE ;
				item.hItem = hTargetItem;
				GetTreeCtrl().GetItem(&item);
				GetTreeCtrl().Select(hTargetItem, TVGN_CARET);
				if (item.state & TVIS_EXPANDED )
					CopyItemProperties(m_hDraggedItem, hTargetItem);
				else 
					//복사할 아이템에 자식 아이템이 없을 경우만 SetButtonState를 수행
					if (!GetTreeCtrl().ItemHasChildren(hTargetItem)) 
						m_memoTree.SetButtonState(hTargetItem, strTgt, pTgtVFL); //strTgt는 Drop한 곳의 path, strTgtfullpath는 그 아래 복사할 파일 path로 구분

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
	return ""; //맞는게 없으면 ""
}

int CLeftView::GetViewfileImgNum(LPCTSTR szExt)
{
	for (int i=0;i<ViewFileListCount;i++) {
		if (stricmp(viewfilelist[i].szExt, szExt) == 0) {
			return viewfilelist[i].imgNum;
		}
	}
	return -1; // 맞는게 없으면 -1
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
//	pCmdUI->Enable((iImage >= ID_VIRTUAL_L) && (iImage <= ID_OPENFOLDER)); //폴더일 경우에만 enable
	pCmdUI->Enable(iImage >= ID_ROOT); //폴더일 경우나 메모인 경우
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
// MyExpand(hItem, TVE_EXPAND); 를 앞으로 옮겨 폴더 가 닫혀있는 상태에서 새 메모 생성시 문제를 해결
void CLeftView::OnMemoNewMemo() 
{
	static const char NEWMEMONAME []= "New Memo ";
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	// 현재 아이템이 메모이면 부모 폴더 아래 아이템을 생성
	// 현재 아이템이 폴더이면 현재 아이템 아래 아이템 생성
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
	BOOL b = MyExpand(hItem, TVE_EXPAND); //먼저 Item을 추가한 후 Item을 확장해야 하므로 위치를 이전으로 부터 여기로 옮김
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
	CString strSrc = m_memoTree.GetPathFromNode((HTREEITEM)hchildItem, this, &ext, NULL, &pVFL); //CString 매개변수를 추가로 주어야 메모일 경우 확장자를 포함하여 얻어온다.
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
		MyExpand(hItem, TVE_EXPAND); //Child가 단지 하나 일때 (앞에서 MyExpand실패했을 때) 2002.7.21
	GetTreeCtrl().Select(hchildItem, TVGN_CARET);
	GetTreeCtrl().EditLabel(hchildItem);	
	//m_NowEditingItem = hchildItem;
	//m_prevEditFoldername = str;
	// 트리를 일단 펼치고
	// 트리모양의 아이템을 추가하고 - 이름을 같은 폴더 안의 "새폴더" + 1,2,3...
	// 아이템명을 바꿀 수 있는 상태로 두고 마침.

	// 아이템 명 바꿀 때 그 함수를 통해 Enter를 치면 폴더가 존재하지 않으면 만듦으로서 이 함수 기능을 완료
	// 아이템 명을 바꿀 때 ESC를 눌렀을 경우는 작업없이 취소 함으로써 새폴더를 생성하지 않고 이 때 실제 폴더가 없으면 아이템을 삭제
}

// Update
// 2002.6.27
// MyExpand(hItem, TVE_EXPAND); 를 앞으로 옮겨 폴더 가 닫혀있는 상태에서 새 폴더 생성시 문제를 해결
void CLeftView::OnMemoNewFolder() 
{
	static const char NEWFOLDERNAME []= "New Folder ";
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	TV_ITEM item;
	item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
	item.hItem = hItem;
	GetTreeCtrl().GetItem(&item);
	// 현재 아이템이 메모이면 부모 폴더 아래 아이템을 생성
	// 현재 아이템이 폴더이면 현재 아이템 아래 아이템 생성
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
		MyExpand(hItem, TVE_EXPAND); //Child가 단지 하나 일때 (앞에서 MyExpand실패했을 때)	
	GetTreeCtrl().Select(hchildItem, TVGN_CARET);
	GetTreeCtrl().EditLabel(hchildItem);	
	//m_NowEditingItem = hchildItem;
	//m_prevEditFoldername = str;
	// 트리를 일단 펼치고
	// 트리모양의 아이템을 추가하고 - 이름을 같은 폴더 안의 "새폴더" + 1,2,3...
	// 아이템명을 바꿀 수 있는 상태로 두고 마침.

	// 아이템 명 바꿀 때 그 함수를 통해 Enter를 치면 폴더가 존재하지 않으면 만듦으로서 이 함수 기능을 완료
	// 아이템 명을 바꿀 때 ESC를 눌렀을 경우는 작업없이 취소 함으로써 새폴더를 생성하지 않고 이 때 실제 폴더가 없으면 아이템을 삭제
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
			//디렉토리 생성
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
				nCount = CDirNFile::DelTreeFiles(strSrc, TRUE, pVFL->pInetf); //디렉토리까지 삭제
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
//DEL 		for (int i=0;i<MemoListCount;i++) { //잠시 배열의 값을 임시로옮겨두었다가..
//DEL 			tmpnames.Add(m_pmemolist[i].name);
//DEL 			tmppaths.Add(m_pmemolist[i].path);
//DEL 			tmphitems.Add((DWORD)m_pmemolist[i].hItem);
//DEL 		}
//DEL 		if (m_pmemolist) {
//DEL 			delete [] m_pmemolist;
//DEL 		}
//DEL 		int addsize = names.GetSize();
//DEL 		m_pmemolist = new MemoList[MemoListCount+addsize];
//DEL 		for (i=0;i<MemoListCount;i++) { // 변수 크기를 늘린 후 다시 원래대로 되돌린다.
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
	HTREEITEM hItem = GetTreeCtrl().GetRootItem(); //루트 Item은 미리 생성되어 있어야 함
	int i;
	for (i=0;i<m_vVFL.size();i++) {
		VirtualFolderList & vfl = m_vVFL[i];
		if(vfl.nItem == ITEM_ROOT) {
			continue; // Item이 Root인 것은 별도로 저장되므로 제외
		}		
		HTREEITEM & l_hItem = vfl.hItem;
		if (l_hItem == 0) {
			int nIcon = (vfl.nRemoteType == 0)?ID_VIRTUAL_L:ID_VIRTUAL_R;
			l_hItem =  GetTreeCtrl().InsertItem(TVIF_IMAGE | TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM , 
				vfl.title.c_str(), nIcon, nIcon, 0, 0, (LPARAM)i, hItem, TVI_LAST);
			// 2002-5-22 kish, m_mapVFL을 사용하지 않음
			vfl.hItem = l_hItem;
			GetTreeCtrl().SetItemData(l_hItem, (DWORD)l_hItem);

			m_memoTree.SetButtonState(vfl.hItem, vfl.path.c_str(), &vfl);
		} else { //이미 존재하는 아이템
			ClsItem * pClsItem = &theApp.m_mapClsItems[vfl.nItem];
			GetTreeCtrl().SetItem(vfl.hItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE|TVIF_TEXT, 
				vfl.title.c_str(), pClsItem->iImage, pClsItem->iSelectedImage, 0, 0, (LPARAM)0); // It's realy fatal bug, don't set to i index into lparam- it caused sort error
			//if ((nUpdateds != NULL) && nUpdateds[i]) { //Updated 된 것만 
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
		// 2002-5-22 kish, m_mapVFL을 쓰지 않음
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
			//이전에는 FTP(또는 inet을 쓰는 다른..., 현재는 로컬 또는 host addr이 바뀌면 
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
				// b = ConnectServer(pVFL); //Onrefresh에서 이 작업을 함
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
		//첫 자식 아이템이 더미폴더이면 - 즉 정상적인 expand 처리가 제대로 되지 않았으면
		if (hCItem && GetTreeCtrl().GetItemText(hCItem)=="") { 
			bOK = TRUE; 
		}
		TRACE0("EXPANDED");
	}
	else {
		//첫 자식 아이템이 일반 아이템이면 - 즉 Collapse 처리가 제대로 되지 않았으면
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
		if (item.iImage <= ID_VIRTUAL_R) { // Virtual folder이거나 루트이면
			pVFL = GetVFL(&m_vVFL, item.hItem);
			break;
		}
		hItem2 = hParent;
	}
	//Refresh할 것이 Folder이면 폴더 내용을 Refresh한다.
	if (nImage >= ID_VIRTUAL_L && nImage <= ID_OPENFOLDER) {
		BOOL b;
		if (pVFL->nRemoteType == REMOTE_LOCAL) {
			CString strSrc = m_memoTree.GetPathFromNode(hItem, this);
			m_memoTree.RefreshFolderItem(hItem, strSrc, pVFL);
		}
		if (pVFL->nRemoteType > REMOTE_LOCAL) {
			b = ConnectServer(pVFL); //Virtual Folder에 선택되어 있으면 무조건 재접속
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
		b = TRUE; // LOCAL일 경우는 TRUE
		if (pVFL->nRemoteType > REMOTE_LOCAL) {
			b = pVFL->pInetf->IsConnect();
			if (!b) { // Connect가 안되어 있다면
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
		Sortcb.lParam = (LPARAM)&GetTreeCtrl(); //오름차순
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
	b = pVFL->pInetf->ReConnect(); //이전에 Connect가 한 번 수행되었어야 함
	if (b) {
		pVFL->pInetf->GetCurrentDirectory(&pVFL->pInetf->m_initDir);
		CString strSrc = m_memoTree.GetPathFromNode(hItem, this);
		CTreeCtrl * ptreeCtrl = &GetTreeCtrl();
		if (hItem != ptreeCtrl->GetRootItem()) // Root일 때는 지우면 안된다. fUpdateFolder에 의해 필요한 부분만 처리해야 된다.
			m_memoTree.RefreshFolderItem(hItem, strSrc, pVFL);
		ptreeCtrl->Select(ptreeCtrl->GetRootItem(), TVGN_CARET); // + 표시가 나타나게 해볼려고
		ptreeCtrl->Select(hItem, TVGN_CARET); //이상 하게 이 노드를 벗어나야 + 가 나온다.
	} else {
		CString tmp;
		tmp.Format("%s\n%s\n%s", pVFL->pInetf->m_URL.c_str(), MyLoadString(IDS_CANNOTCONNECTSERVER), pVFL->pInetf->m_sErrMsg.c_str());
		AfxMessageBox(tmp);
	}
	return b;
}
