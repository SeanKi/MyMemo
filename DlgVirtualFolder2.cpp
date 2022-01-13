// DlgVirtualFolder2.cpp : implementation file
//

#include "stdafx.h"
#include "mymemo.h"
#include "DlgVirtualFolder2.h"
#include "Global.h"
#include "InetFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVirtualFolder2 dialog


CDlgVirtualFolder2::CDlgVirtualFolder2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVirtualFolder2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVirtualFolder2)
	m_sAlias = _T("");
	m_sPath = _T("");
	m_sHostname = _T("");
	m_sUsername = _T("");
	m_sPassword = _T("");
	m_sServerdir = _T("");
	//}}AFX_DATA_INIT
	m_nType = REMOTE_LOCAL;
}


void CDlgVirtualFolder2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVirtualFolder2)
	DDX_Text(pDX, IDC_EDIT1, m_sAlias);
	DDX_Text(pDX, IDC_EDIT2, m_sPath);
	DDX_Text(pDX, IDC_EDIT3, m_sHostname);
	DDX_Text(pDX, IDC_EDIT4, m_sUsername);
	DDX_Text(pDX, IDC_EDIT5, m_sPassword);
	DDX_Text(pDX, IDC_EDIT6, m_sServerdir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVirtualFolder2, CDialog)
	//{{AFX_MSG_MAP(CDlgVirtualFolder2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowserFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVirtualFolder2 message handlers

void CDlgVirtualFolder2::OnBrowserFolder() 
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

void CDlgVirtualFolder2::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	BOOL b;
	BOOL bFind = FALSE;
	if (m_nType == REMOTE_LOCAL) { // if local
	} else if (m_nType == REMOTE_FTP) { // if ftp
		if (m_sServerdir == "") m_sServerdir = ".";
	}
	CDialog::OnOK();
}
const int arynIDType0[] = {IDC_EDIT2, IDC_BUTTON1};
const int arynIDType1[] = {IDC_EDIT3,IDC_EDIT4,IDC_EDIT5,IDC_EDIT6};

void CDlgVirtualFolder2::ChangeType(int nType)
{
	CButton * pwnd1 = (CButton*)GetDlgItem(IDC_RADIO1);
	CButton * pwnd2 = (CButton*)GetDlgItem(IDC_RADIO2);
	int i;
	CWnd *pwnd;
	for (i=0;i<sizeof arynIDType0/sizeof arynIDType0[0];i++) {
		pwnd = GetDlgItem(arynIDType0[i]);
		pwnd->EnableWindow(FALSE);
	}
	for (i=0;i<sizeof arynIDType1/sizeof arynIDType1[0];i++) {
		pwnd = GetDlgItem(arynIDType1[i]);
		pwnd->EnableWindow(FALSE);
	}
	if (nType == REMOTE_LOCAL) {
		pwnd1->SetCheck(1);
		for (i=0;i<sizeof arynIDType0/sizeof arynIDType0[0];i++) {
			pwnd = GetDlgItem(arynIDType0[i]);
			pwnd->EnableWindow(TRUE);
		}
	} else if(nType == REMOTE_FTP){
		pwnd2->SetCheck(1);
		for (i=0;i<sizeof arynIDType1/sizeof arynIDType1[0];i++) {
			pwnd = GetDlgItem(arynIDType1[i]);
			pwnd->EnableWindow(TRUE);
		}
	} else { //no change
		return;
	}
	m_nType = nType;
}
void CDlgVirtualFolder2::GetType()
{
	CButton * pwnd1 = (CButton*)GetDlgItem(IDC_RADIO1);
	CButton * pwnd2 = (CButton*)GetDlgItem(IDC_RADIO2);
	if (pwnd1->GetCheck() == 1) {
		m_nType = REMOTE_LOCAL; // local
	} else 
	if (pwnd2->GetCheck() == 1) {
		m_nType = REMOTE_FTP; // ftp
	} else { // 둘다 체크되어 있지 않으면 local을 기본으로 체크
		m_nType = REMOTE_LOCAL;
		pwnd1->SetCheck(1);
	}
}

BOOL CDlgVirtualFolder2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ChangeType(m_nType); //외부에서 세팅한 m_nType대로 상태를 변경
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDlgVirtualFolder2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_COMMAND) {
		int idButton;
		idButton = (int) LOWORD(wParam);
		if (idButton == IDC_RADIO1) {
			ChangeType(REMOTE_LOCAL);
		} else
		if (idButton == IDC_RADIO2) {
			ChangeType(REMOTE_FTP);
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
