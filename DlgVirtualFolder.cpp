// DlgVirtualFolder.cpp : implementation file
//

#include "stdafx.h"
#include "MyMemo.h"
#include "DlgVirtualFolder.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVirtualFolder dialog


CDlgVirtualFolder::CDlgVirtualFolder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVirtualFolder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVirtualFolder)
	m_sAlias = _T("");
	m_sPath = _T("");
	//}}AFX_DATA_INIT
}


void CDlgVirtualFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVirtualFolder)
	DDX_Text(pDX, IDC_EDIT1, m_sAlias);
	DDX_Text(pDX, IDC_EDIT2, m_sPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVirtualFolder, CDialog)
	//{{AFX_MSG_MAP(CDlgVirtualFolder)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowserFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVirtualFolder message handlers

void CDlgVirtualFolder::OnBrowserFolder() 
{
	UpdateData(TRUE);
	CString strFolderPath = m_sPath;
	if (GetFolder(&strFolderPath, "Select Save Folder.", this->m_hWnd, NULL, strFolderPath)){
		if (!strFolderPath.IsEmpty()){
			m_sPath = strFolderPath;
			if (m_sAlias == "") {
				char Buff[MAX_PATH]="";
				m_sAlias = PARSE_GetFileTitle(Buff, m_sPath);
			}
			UpdateData(FALSE);
		}
	}	
}

void CDlgVirtualFolder::OnOK() 
{
	UpdateData(TRUE);
	static char tmppath[_MAX_PATH];
	BOOL bFind = FALSE;
	GetCurrentDirectory(sizeof tmppath, tmppath);
	if (SetCurrentDirectory(m_sPath)) {
		bFind = TRUE;
		SetCurrentDirectory(tmppath);
	}
/*	CFileFind find;
	bFind= find.FindFile(m_sPath);
	find.Close();*/
	if (bFind) {
		CDialog::OnOK();
	}
	else
		AfxMessageBox(MyLoadString(IDS_CANNOTFOUNDPATH));
}
