// MemoTreeCtrl.cpp: implementation of the CMemoTreeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMemo.h"
#include "MemoTreeCtrl.h"
#include "Parseki.h"
#include "LeftView.h"
#include "StrKi.h" //STR_GetNextToken

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoTreeCtrl::CMemoTreeCtrl()
{
}

CMemoTreeCtrl::~CMemoTreeCtrl()
{

}

BOOL CMemoTreeCtrl::SetButtonState(HTREEITEM hItem, CString strPath, VirtualFolderList *pVFL)
{
	CString strFindPath = strPath;
	char SEP = '\\';
	if (pVFL) {
		SEP = pVFL->GetDirSeparator();
	}
	if(strFindPath.Right(1) != SEP) strFindPath += SEP;

	strFindPath += "*.*";
	string prevdir;
	CString strDirName, strFileName;
	BOOL bContinue;
	HINTERNET hInternet;
	HANDLE hFindFile;
	WIN32_FIND_DATA finddata;
	if (!pVFL || pVFL->nRemoteType == REMOTE_LOCAL) {
		bContinue = ((hFindFile = FindFirstFile(strFindPath, &finddata)) != INVALID_HANDLE_VALUE); 
	} else if (pVFL->nRemoteType == REMOTE_FTP) {
		while(1) {
			BOOL b = pVFL->pInetf->SetCurrentDirectory(pVFL->pInetf->m_initDir.c_str()); //�߰��� ���� ���丮 ������ �ٸ� ������ �Ǿ� ������ �߻��Ͽ�, �׻� �ʱ� ���丮�� �̵��� �� �۾��ϵ��� ��
			if (!b) {
				if (GetLastError() == ERROR_INTERNET_CONNECTION_ABORTED) {
					if (pVFL->pInetf->ReConnect())
						continue;
				}
			}
			break;
		} 
		pVFL->pInetf->SetCurrentDirectory(strPath);
		strFindPath = "*.*";
		hInternet = FtpFindFirstFile(pVFL->pInetf->GetConnectHandle(), strFindPath, &finddata, 0, 0);
		long l = GetLastError();
		DWORD dwError;
		char buffer[1024];
		DWORD bufsize=1024;
		if (l != 0) {
			InternetGetLastResponseInfo(
				&dwError,
				buffer,
				&bufsize
			);
		}
		bContinue = (hInternet != NULL);
	}
	if(!(bContinue))
		return FALSE;

	vector <WIN32_FIND_DATA> vFinddata;
	while(bContinue) {
		vFinddata.push_back(finddata);
		if (!pVFL || pVFL->nRemoteType == REMOTE_LOCAL) {
			bContinue = FindNextFile(hFindFile, &finddata);
		} else if (pVFL->nRemoteType == REMOTE_FTP) {
			bContinue = InternetFindNextFile(hInternet, &finddata);
		}
	}
	if (!pVFL || pVFL->nRemoteType == REMOTE_LOCAL) {
		FindClose(hFindFile);
	} else if (pVFL->nRemoteType == REMOTE_FTP) {
		InternetCloseHandle(hInternet);
		pVFL->pInetf->SetCurrentDirectory(pVFL->pInetf->m_initDir.c_str());
	}
	// ������ ���丮�� ������ ���ʷ� �о��
	// ���丮 Ž��
	BOOL bDirectory;
	string strFileTitle;
	int nPos;
	for (int i=0; i < vFinddata.size(); i++) {
		finddata = vFinddata[i];
/*	while(bContinue)
	{*/
		bDirectory = finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY; 
		strFileName = finddata.cFileName; 
		// ���丮�̸�...
		if(bDirectory)
		{
			strDirName = strFileName;
			if((strDirName != ".") && (strDirName != ".."))
			{
				// "."�� ".."�� �ƴ� ���丮�̸� ���̳�带 �ϳ� �߰��ϰ� ���� ����
				m_ptreeCtrl->InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
				break;
			}
		} else {
			CString strExt = CParseKi::sRight(strFileName, '.'); //Extention�� ��´�.
			int iImage = ((CLeftView *)m_ptreeCtrl)->GetViewfileImgNum(strExt);
			CString sTmp;
			BOOL b;
			if (iImage != -1) {
				string stdstrTemp;
				int pos = 0;
				while(1) {
					char buffer[255];
					b = PARSE_GetNextToken(buffer, DirRemarkFILEList, "|", &pos);
					if (!b) break;
					stdstrTemp = buffer; //STR_GetNextToken(DirRemarkFILEList, "|", &pos);
					if (lstrcmpi(stdstrTemp.c_str(), strFileName) == 0)
						break;
				}
				if (!b) {//strstr(DirRemarkFILEList, sTmp) == NULL) { //strFileName != DirRemarkFILE) { //TEXT���� �̴��� ���� �����̸� ����
					m_ptreeCtrl->InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
					break;
				}
			}	
		}
	}
	
	return TRUE;
}

string PARSE_ReplaceAll(string sIn, LPCSTR szSub1, LPCSTR szSub2, int * pReplaceCount = NULL);

//�־��� ��尡 ��Ÿ���� ���丮�� ���
CString CMemoTreeCtrl::GetPathFromNode(HTREEITEM hItem, VOID * leftView, CString *ext, CString *sVirtualPath, VirtualFolderList ** ppVFL)
{
	CLeftView * pLeftView = (CLeftView*)leftView;
//	MemoList *memolist = pLeftView->m_pmemolist;
	CString strResult = m_ptreeCtrl->GetItemText(hItem);
		if (sVirtualPath)
			*sVirtualPath = strResult;
		TV_ITEM item;
		item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE;
		item.hItem = hItem;
		m_ptreeCtrl->GetItem(&item);
//	if (item.iImage == 0)
//		return "";
	if (ext)
		*ext = ((CLeftView *)m_ptreeCtrl)->GetViewfileExt(item.iImage);

	VirtualFolderList * pVFL;
	CString sVirtualRoot;
	// ���� Item�� Virtual Folder�̰ų� ��Ʈ�̸� �Ʒ��� loop�� ���� �ʰ� �ٷ� ��θ� �����Ѵ�.
	if (item.iImage <= ID_VIRTUAL_R) {
		pVFL = GetVFL(&pLeftView->m_vVFL, item.hItem);
		if (ppVFL != NULL)
			*ppVFL = pVFL;
		strResult = pVFL->path.c_str(); // memolist[item.lParam].path;
		if (sVirtualPath)
			*sVirtualPath = pVFL->title.c_str(); //memolist[item.lParam].name;
		return strResult;
	}
/*	if (item.iImage == ID_VIRTUAL_L || item.iImage == ID_VIRTUAL_R) {
		pVFL = GetVFL(&pLeftView->m_vVFL, item.hItem);
		if (ppVFL != NULL)
			*ppVFL = pVFL;
		strResult = pVFL->path.c_str(); // memolist[item.lParam].path;
		if (sVirtualPath)
			*sVirtualPath = pVFL->title.c_str(); //memolist[item.lParam].name;
		return strResult;
	} else if (item.iImage == ID_ROOT) {
		pVFL = GetVFL(&pLeftView->m_vVFL, item.hItem);
		if (ppVFL != NULL)
			*ppVFL = pVFL;
		strResult = pLeftView->m_RootPath;
		return strResult;
	}
*/
	HTREEITEM hParent;
	// ��Ʈ ��带 ���� ������ ����...
	while((hParent = m_ptreeCtrl->GetParentItem(hItem)) != NULL)
	{
		CString str;
		CString strVirtual;
		TV_ITEM item;
		item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE;
		item.hItem = hParent;
		m_ptreeCtrl->GetItem(&item);
		CString sVirtualRoot;
		if (item.iImage == ID_VIRTUAL_L || item.iImage == ID_VIRTUAL_R) {
			pVFL = GetVFL(&pLeftView->m_vVFL, item.hItem);
			if (ppVFL != NULL)
				*ppVFL = pVFL;
			str = pVFL->path.c_str();// memolist[item.lParam].path;
			sVirtualRoot = (CString)"�޸�\\" + pVFL->title.c_str();
		} else if (item.iImage == ID_ROOT) {
			pVFL = GetVFL(&pLeftView->m_vVFL, item.hItem);
			if (ppVFL != NULL)
				*ppVFL = pVFL;
			str = pVFL->path.c_str();// memolist[item.lParam].path;
			sVirtualRoot = "�޸�";
		} else {
			str = m_ptreeCtrl->GetItemText(hParent);
		}
		//�ϴ� Directory Separator�� �ϰ�������  \�� �ϰ� �ʿ�� �Ѳ����� ������ ���̴�.
		CString sTmp = CParseKi::AddDirSeparator(str, '\\').c_str() + strResult; // sPathAddBackSlash -> AddDirSeparator 
		strResult = sTmp;
//		str.ReleaseBuffer();
		if (sVirtualPath) {
			if (sVirtualRoot.GetLength() > 0) {
				*sVirtualPath = CParseKi::sPathAddBackSlash(sVirtualRoot) + *sVirtualPath;
				break; 
			} else {
				*sVirtualPath = CParseKi::sPathAddBackSlash(m_ptreeCtrl->GetItemText(hParent)) + *sVirtualPath;
			}
		} else {
			if (sVirtualRoot.GetLength() > 0)
				break;
		}
		hItem = hParent;
	}
//	if (ext != NULL && *ext != "")
//		strResult += ("." + *ext);
	if (pVFL->GetDirSeparator() != '\\')
		return PARSE_ReplaceAll((LPCSTR)strResult, "\\", "/").c_str();
	return strResult;
}

// szSub1�� szSub2�� ���ڿ� ũ�Ⱑ Ʋ����츦 �׽�Ʈ ���� ���ߵ�.
string PARSE_ReplaceAll(string sIn, LPCSTR szSub1, LPCSTR szSub2, int * pReplaceCount)
{
	int n=0;
	while(1) {
		n = sIn.find(szSub1, n);
		if (n > -1) {
			sIn.replace(n, strlen(szSub1), szSub2); //�ι�° Param�� �ٲ� ���ڿ� ũ���ε�, �°� �ϴ� ������ �𸣰���
			if(pReplaceCount)
				(*pReplaceCount)++ ;
		}
		else 
			break;
		n += strlen(szSub1);
	}
	return sIn;
}
// �ϳ��� �ڽ� ��� �����ϱ�
void CMemoTreeCtrl::DeleteFirstChild(HTREEITEM hParent)
{
	HTREEITEM hItem;
	if((hItem = m_ptreeCtrl->GetChildItem(hParent)) != NULL)
		m_ptreeCtrl->DeleteItem(hItem);
}


// ��� �ڽ� ��� �����ϱ�
void CMemoTreeCtrl::DeleteAllChildren(HTREEITEM hParent)
{
	HTREEITEM hItem;
	if((hItem = m_ptreeCtrl->GetChildItem(hParent)) == NULL)
		return;

	do
	{
		HTREEITEM hNextItem = m_ptreeCtrl->GetNextSiblingItem(hItem);
		m_ptreeCtrl->DeleteItem(hItem);
		hItem = hNextItem;
	} while (hItem != NULL);
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, 
	LPARAM lParamSort);

void CMemoTreeCtrl::Init(CTreeCtrl * treeCtrl)
{
	m_ptreeCtrl = treeCtrl;
}

int CMemoTreeCtrl::DelDir(HTREEITEM hItem, CString &strPath)
{
	int nCount=0;
	return nCount;
}


BOOL CMemoTreeCtrl::FindItemByVirtualPath(CString sPath, HTREEITEM * hItem)
{
		((CLeftView *)m_ptreeCtrl)->m_bAddRML = FALSE;
        char seps [] = "\\";
        char * token;
		char * buff = sPath.GetBufferSetLength(sPath.GetLength());
        /* Establish string and get the first token: */
		*hItem = m_ptreeCtrl->GetRootItem();
        token = strtok( buff, seps );
        while( token)
        {
			HTREEITEM hNowItem = FindItem(*hItem, token);
			*hItem = hNowItem; //���� �������� �θ�� �����Ͽ� ���� �������� ã����
			if (hNowItem == NULL) {
				AfxMessageBox("�޸� ���ų� ��ΰ� ����Ǿ����ϴ�.");
				return FALSE;
			} else {
				m_ptreeCtrl->Select(hNowItem, TVGN_CARET);
				TV_ITEM item;
				item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
				item.hItem = hNowItem;
				m_ptreeCtrl->GetItem(&item);
				((CLeftView *)m_ptreeCtrl)->MyExpand(hNowItem, TVE_EXPAND);
				
			}
			/* While there are tokens in "string" */
			CString str(token);
			/* Get next token: */
            token = strtok( NULL, seps );
		}
	((CLeftView *)m_ptreeCtrl)->m_bAddRML = TRUE;
	return TRUE;
}

// 
HTREEITEM CMemoTreeCtrl::FindItem(HTREEITEM hItem, LPCTSTR szBuf)
{
	if (m_ptreeCtrl->GetItemText(hItem) == szBuf) //��Ʈ�̸� 
		return hItem;
	HTREEITEM hItem1 = m_ptreeCtrl->GetChildItem(hItem);
 	while (hItem1) {
		HTREEITEM hNextItem = m_ptreeCtrl->GetNextSiblingItem(hItem1);
		CString stritem = m_ptreeCtrl->GetItemText(hItem1);
		if (stritem == szBuf) {
			return hItem1;
		}
 		hItem1 = hNextItem;
	}
	return NULL;
}

void CMemoTreeCtrl::RefreshFolderItem(HTREEITEM hItem, CString strPath, VirtualFolderList *pVFL)
{
				HTREEITEM &hNowItem = hItem;
				//int iCount = DeleteTreeItems(m_pmemolist[i].hItem);
				TV_ITEM item;
				item.mask = TVIF_HANDLE|TVIF_STATE|TVIF_IMAGE ;
				item.hItem = hNowItem;
				m_ptreeCtrl->GetItem(&item);
					DeleteAllChildren(hNowItem);
					SetButtonState(hNowItem, strPath, pVFL);
				if ((item.state & TVIS_EXPANDED) ) {
					/*GetTreeCtrl().Expand(hNowItem, TVE_COLLAPSE);
					Itemexpand(hNowItem, 0, TVE_EXPAND);*/
					((CLeftView *)m_ptreeCtrl)->MyExpand(hNowItem, TVE_EXPAND);

				} else {
//					m_memoTree.DeleteAllChildren(hNowItem);
				}

}

// strPath -> relative path 
// VFL
int CMemoTreeCtrl::AddDir(HTREEITEM hItem, CString strPath, int iSortMode, VirtualFolderList *pVFL)
{
	CString strFindPath;
	HTREEITEM hNewItem;
	int nCount = 0;

	// ������ ���丮 �ڿ� "\\*.*"�� ������
	// ��: strPath = "C:\\Temp"�̸�, strFindPath = "C:\\Temp\\*.*"�� ��
	strFindPath = strPath;
	char SEP = pVFL->GetDirSeparator();

	if(strFindPath.Right(1) != SEP) strFindPath += SEP;
	strFindPath += "*.*";
	string prevdir;

	CString strFileName, strNewPath; 

	// Ž���� ���丮 ����
	BOOL bContinue;
	HINTERNET hInternet;
	HANDLE hFindFile;
	WIN32_FIND_DATA finddata;
	if (!pVFL || pVFL->nRemoteType == REMOTE_LOCAL) {
		bContinue = ((hFindFile = FindFirstFile(strFindPath, &finddata)) != INVALID_HANDLE_VALUE); 
	} else if (pVFL->nRemoteType == REMOTE_FTP) {
		// '2003.1.9 if disconnected, then reconnect
		while(1) {
			BOOL b = pVFL->pInetf->SetCurrentDirectory(pVFL->pInetf->m_initDir.c_str()); //�߰��� ���� ���丮 ������ �ٸ� ������ �Ǿ� ������ �߻��Ͽ�, �׻� �ʱ� ���丮�� �̵��� �� �۾��ϵ��� ��
			if (!b) {
				if (GetLastError() == ERROR_INTERNET_CONNECTION_ABORTED) {
					if (pVFL->pInetf->ReConnect())
						continue;
				}
			}
			break;
		} 
		pVFL->pInetf->SetCurrentDirectory(strPath);
		strFindPath = "*.*";
		hInternet = FtpFindFirstFile(pVFL->pInetf->GetConnectHandle(), strFindPath, &finddata, 0, 0);
		bContinue = (hInternet != NULL);
		if (!hInternet) {
			nCount = GetLastError();
		DWORD dwError;
		char buffer[1024];
		DWORD bufsize=1024;
			if (nCount != 0) {
				InternetGetLastResponseInfo(
					&dwError,
					buffer,
					&bufsize
				);
			}
			
		}
	}
	if(!(bContinue))
	{
		if(m_ptreeCtrl->GetParentItem(hItem) == NULL)
			m_ptreeCtrl->InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
		return 0;
	}
	vector <WIN32_FIND_DATA> vFinddata;
	while(bContinue) {
		vFinddata.push_back(finddata);
		if (!pVFL || pVFL->nRemoteType == REMOTE_LOCAL) {
			bContinue = FindNextFile(hFindFile, &finddata);
		} else if (pVFL->nRemoteType == REMOTE_FTP) {
			bContinue = InternetFindNextFile(hInternet, &finddata);
		}
	}
	if (!pVFL || pVFL->nRemoteType == REMOTE_LOCAL) {
		FindClose(hFindFile);
	} else if (pVFL->nRemoteType == REMOTE_FTP) {
		InternetCloseHandle(hInternet);
		pVFL->pInetf->SetCurrentDirectory(pVFL->pInetf->m_initDir.c_str());
	}
	// ���丮 Ž��
	BOOL bDirectory;
	string strFileTitle;
	int nPos;
	nCount = 0;
	for (int i=0; i < vFinddata.size(); i++) {
		finddata = vFinddata[i];
/*	while(bContinue)
	{*/
		bDirectory = finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		strFileName = finddata.cFileName;
		nPos = strFileName.ReverseFind('.');
		strFileTitle = "";
		if (nPos > -1)
			strFileTitle = strFileName.Left(nPos);
			
		// ���丮�� ��졦
		if(bDirectory) 
		{
			if(strFileName != "." && strFileName != "..")
			{
				// ��� �߰�
				hNewItem = m_ptreeCtrl->InsertItem((LPCTSTR)strFileName, ID_CLOSEDFOLDER, ID_OPENFOLDER, hItem);
				m_ptreeCtrl->SetItemData(hNewItem, (DWORD)hNewItem);
				
				strNewPath = strPath;
				if(strNewPath.Right(1) != SEP)	strNewPath += SEP;
				
				// �߰��� ��尡 ���� ���丮�� �������� Ȯ��
				strNewPath += strFileName;
				SetButtonState(hNewItem, strNewPath, pVFL);
				nCount++;
			}
		} else {
			CString strExt = CParseKi::sRight(strFileName, '.'); //Extention�� ��´�.
			int iImage = ((CLeftView *)m_ptreeCtrl)->GetViewfileImgNum(strExt);
			CString sTmp;
			if (iImage != -1) {
			//	sTmp = strFileName;
				string stdstrTemp;
				int pos = 0;
				BOOL b;
				while(1) {
					char buffer[255];
					b = PARSE_GetNextToken(buffer, DirRemarkFILEList, "|", &pos);
					if (!b) break;
					stdstrTemp = buffer; //STR_GetNextToken(DirRemarkFILEList, "|", &pos);
					if (lstrcmpi(stdstrTemp.c_str(), strFileName) == 0)
						break;
				}
				if (!b) {//strstr(DirRemarkFILEList, sTmp) == NULL) { //strFileName != DirRemarkFILE) { //TEXT���� �̴��� ���� �����̸� ����
					hNewItem = m_ptreeCtrl->InsertItem((LPCTSTR)strFileTitle.c_str(), iImage, iImage, hItem);
					m_ptreeCtrl->SetItemData(hNewItem, (DWORD)hNewItem);
					nCount++;
				}
			}
		}
	}
	if (iSortMode == 1) {
		TVSORTCB Sortcb;
		Sortcb.hParent = hItem;
		Sortcb.lParam = (LPARAM)m_ptreeCtrl; //��������
		Sortcb.lpfnCompare = &CompareFunc;
		//GetTreeCtrl().SortChildren(hItem);
		m_ptreeCtrl->SortChildrenCB( &Sortcb );
		//m_ptreeCtrl->SortChildren(hItem);// �ڽ� �������� ��Ʈ 2002.2.20
	}

	return nCount;
}
