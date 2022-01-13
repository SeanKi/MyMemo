// MyMemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyMemo.h"

#include "LeftView.h"
#include "MainFrm.h"
#include "MyMemoDoc.h"
#include "Label.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*///////////////////////////////////////////////////////////////////////////
// CMyMemoApp
// Programmed by Ki Seong Hwoe 2001 - 2005
1.3.2.31 - 2005.11.29
 - ��� �� ��� �߰�
1.3.2.30 - 2005.8.22
 - ã��/�ٲٱ� �޴� �߰�
1.3.1.29 - 2005.7.11
 - �ؽ�Ʈ ���ڿ� �¿� ������ 8pixel(���� 1��) 
 - �ǰ����� 16pixel�� 32pixel�� ���� �����ϰ�
 - ���α׷� ���۽� ȭ�鿡 ��Ÿ���� ���� �� �ִ� ������ ������ ��������(�Ƹ��� ������ ����� ���Դϴ� ^^)
1.3.0.28 - 2005.6.27
 - Ȩ���ּ� koolwind.net -> ykish.wo.to
 - �ý��� ����� destroy
1.3.0.24
  -2003.1.9
AddDir���� Connection���������� reconnect
�������� _FolderComment.txt�� ���� �������� �ν��ϴ��� �ذ� 
-> �������� ���� ������ �Ⱥ��̴� ��찡 �־ ������ �����µ� ��������
-> FileExists ��Ʈ��ũ ����
1.3.0.23
-2003.1.5
������丮 ���� �ȵǴ� ���� ����, FTP�� �н����� �Է»��ڸ� ��ǥ(*)���·� �Ⱥ��̰� �ٲ�
1.3.0.22
Remote Folder(FTP)�� �������丮 ���� ��� ����
// 1.3.0.21
// 2002.12.18
FTp��� �߰�, �����ִ� ���丮���� ���� �ذ�
// 2002.12.6
FTP����� ������ ������ �޽��� �߻� -> �������� �����ؼ� �����ִ� ��� ����ؾߵ�. Reconnect�޴��� �ʿ�
StrKi2.cpp�� ��� ���� - STD_GetNextToken ��� PARSE_GetNextToken�� ����ϰ�, �� �� �߻��� ������ �߰� �ȵǴ� ������ ó��
// 2002.12.5
Ctrl+shift+ins -> Ctrl+ins
Ctrl+del -> del
ftp���ٽ� wait cursorǥ��
  // 2002.12.1
���� ������ ������ �ȵǴ� ���� �ذ� - VirtualFolder���� ��� ITEM_ROOT�� �����Ǿ� �վ� ���乮��
FTP���� ���� �����ֱ� ������ ��� ���� �޸� �������� �۾�
// 1.2.5.20 - 2002.7.21
���� ������ �޸� ���� ��� ���� �߰� �� �� ���α׷��� �״� ���� ����
// 1.2.5.19 
- 2002.7.18
���� ���������� ___��������.txt -> _FolderComment.txt�� �⺻���� �ٲ�. ������ ___��������.txt�� ������ �״�� �ν��ϰ�,  Readme.txt������ ������ ��� readme.txt������ ���� ���� ���Ϸ� ������. �̱���� ������� ����ϸ��� �����Ǿ� �߰��Ͽ����ϴ�. 
���� �������������� �������� ���� ��� �켱������ _FolderComment.txt>___��������.txt>ReadMe.txt ���̸�, �������������� ���� ���� _FolderComment.txt�� �����˴ϴ�. 

- 2002.6.27
// ������ ������ �ִ� ���¿��� New Folder�� New Memo�� ������ ���� ���� ������ �� �� �۾�
// 1.2.5.18 - 2002.5.31
// Ʈ���信�� ������ ���� ���� �����ϸ� ���α׷��� �׾������ ������ ���� �ذ�.
// ����������� ���ö� NULL�� �Ѿ�ͼ� ���� ����
// 1.2.5.17 - 2002.5.27
// �ʱ� ����� �߰� ��ġ�� �߰� �ϴ� �Ϳ��� - ȭ�� ũ�⸦ 1024*768�� 
//     �����ߴ� ���� �ذ�(���� ȭ��ũ�� ����)
// ��ġ�� -1���ϸ� �߰����� ��ġ���״� ���� �ذ�(��Ƽ����Ϳ��� ����)
//     , �ϴ� ȯ�����Ͽ� x, y���� ���ų� -4000000�̸� ���� ������ �����Ͽ� �߾����� ��ġ��Ŵ
// 1.2.5.16
// ���� ������ ����� ���� ���� �ʰų� ��Ÿ ������ ����Ű�� ���� ���� - m_mapVFL�� ���� ����
// ���� ���� �߰��� ��� �����ϸ� �������� ���� ���������� �ڵ� �Է�(�̹� �ԷµǾ� ������ �������) 
// 1.2.5.15
// 2002.5.20
// ���� ��� �ٲٸ鼭 �� ���� ���� �߰��� ���Ľ� ���� ����, ��ҹ��� �����Ͽ� ���ĵǴ� �� ����
// 2002.5.?
//*�޸� Ʈ������ �����۵� ���� ���� ���� -> �޸� ���丮(��������), ����, �޸� �� ���� �����Ͽ�, ���Ⱑ ������ ��
//.�޸� ��Ʈ ���� ��ħ �ȵǾ��� ���� �ذ�
// 1.2.1.14
// 2002.5.1
// ToolBar, StatusBar Show/Hide ���� ����
// Always On Top �޴� �߰�, ���� ����
// 2002.4.30
// Root Item�� ���ΰ�ħ�� ���� �ʴ� ���� �ذ� 
// CLeftView::OnUpdateProperty���� ClsItem::nKind�� Enableüũ�ϵ��� ����
// Sort���� - �ѱ�, ���ĺ� ������ 
// Sort����� ���� - �޸� ������ ����
// 1, 2, 1, 13
// m_memoList���� VirtualFolderList�� CMap�� �̿��� �� ������ ����
// Ver1.2.1.12 
// 2002.4.4
//.ó�� ����� '�⺻' ��������(�޸���丮)�� ����� ��� ����(��Ʈ������ �⺻���� ����� �ϹǷ�)
// 2002.3.29
// 
.�ڽľ����۾��� �����ۿ��� �޸� ���� �߰��� ���� �ذ�
.�ڵ�����(�⺻ - ���� 10����)
.������ �� �� ����ϰ� ����
*�޸��Ʈ������ ���� �� �� �ְ� �� �Ʒ� �޸���丮�� ���� ������ �޸� �� �� �ֵ��� ��
.�޸� ���� �巡�� �� ���� �Ʒ��� ���� �����ϸ� ��ũ���� �����Ҷ� ���� �ڵ� ��ũ�� �ȴ�.
.������ �޸� ������ ����� ������ �ڵ����� �� ������ ���������� ��.
.����� ���� �� �ٸ� ������ �������� ���õǴ� ���� ��ħ.
.Tree�����۵��� �̵��� �ణ�� �ð������� �� �� �޸� ǥ���ϵ��� �Ͽ���. Tree�̵��� ��ġ�ʴ� �޸�� ���� �ʵ��� �ϰ� �װ��� �ֱٸ޸��Ͽ� ��ϵǴ� �� ���� ���´�.
.�� �޸� ������ ���� �� ������ �θ� �޸��� ������ ���̴� ������ �ذ�
// 2002. 2.21 - Resource ID - IDC_HAND -> IDC_HAND1���� ����, �⺻ ID�� �ߺ� ������
*/

BEGIN_MESSAGE_MAP(CMyMemoApp, CWinApp)
	//{{AFX_MSG_MAP(CMyMemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMemoApp construction

CMyMemoApp::CMyMemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyMemoApp object

CMyMemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyMemoApp initialization
static HANDLE hImHere;

BOOL CMyMemoApp::InitInstance()
{
	// ���α׷��� �ߺ� ������ ����
	hImHere=::CreateMutex(NULL, TRUE, _T("MyMemoMutex"));
	if(ERROR_ALREADY_EXISTS==::GetLastError())
	{
		HWND hWnd=::FindWindow(AfxGetAppName(), NULL/*m_pszAppName*/);
		::SetForegroundWindow(hWnd);
		::CloseHandle(hImHere);
		::MessageBox(hWnd,MyLoadString(IDS_PROGRAM_ALREADY_STARTED), "Warning",MB_OK);
		return FALSE;
	}
	// ���α׷��� �ߺ� ������ ���� - ��
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

//	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	m_Profile.SetFilename("MyMemo.ini");

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	ClsItemListCount = sizeof m_ClsItemList / sizeof m_ClsItemList[0];
	m_ClsItemList[0].SetItem(CTL_TEXT, ID_ROOT, ID_ROOT, KIND_ROOT|KIND_VIRTUAL, "");
	m_ClsItemList[1].SetItem(CTL_TEXT, ID_VIRTUAL_L, ID_VIRTUAL_L, KIND_VIRTUAL|KIND_FOLDER, "");
	m_ClsItemList[2].SetItem(CTL_TEXT, ID_VIRTUAL_R, ID_VIRTUAL_R, KIND_VIRTUAL|KIND_REMOTE|KIND_FOLDER, ""); //<-network�� ftp�� ���̴� 
	m_ClsItemList[3].SetItem(CTL_TEXT, ID_CLOSEDFOLDER, ID_OPENFOLDER, KIND_FOLDER, ""); //<-�̰��� Ȯ���� ��ſ� ��ǥ�� ������ ���� ������ �����ִ� �� ���.
	m_ClsItemList[4].SetItem(CTL_TEXT, ID_TEXT_FILE, ID_TEXT_FILE, KIND_FILE, "txt");
	m_mapClsItems[ITEM_ROOT] = m_ClsItemList[0]; // ITEM_ROOT - nItem
	m_mapClsItems[ITEM_VIRTUAL_LOCAL] = m_ClsItemList[1];
	m_mapClsItems[ITEM_VIRTUAL_FTP] = m_ClsItemList[2];
	m_mapClsItems[ITEM_FOLDER] = m_ClsItemList[3];
	m_mapClsItems[ITEM_FILE_TEXT] = m_ClsItemList[4];
	// m_mapClsItem[icon image number]
/*	m_mapClsItem[ID_ROOT] = &m_ClsItemList[0];
	m_mapClsItem[ID_VIRTUAL_L] = &m_ClsItemList[1];
	m_mapClsItem[ID_VIRTUAL_R] = &m_ClsItemList[2];
	m_mapClsItem[ID_CLOSEDFOLDER] = &m_ClsItemList[3];
	m_mapClsItem[ID_TEXT_FILE] = &m_ClsItemList[4];
*/
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyMemoDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

#include "FileVersion.cpp"
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CLabel	m_myHomeLink;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_MYHOME, m_myHomeLink);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMyMemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMyMemoApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char szBuf[MAX_PATH]={0,};
	string str, fileVer, fileVer2;
	fileVer = theApp.GetVersion();
	int pos = 0;
	strcpy(szBuf, "");
	PARSE_GetNextToken(szBuf, (char *)fileVer.c_str(), ", ", &pos);
	fileVer2 = szBuf;
	fileVer2 += ".";
	strcpy(szBuf, "");
	PARSE_GetNextToken(szBuf, (char *)fileVer.c_str(), ", ", &pos);
	fileVer2 += szBuf;
	strcpy(szBuf, "");
	PARSE_GetNextToken(szBuf, (char *)fileVer.c_str(), ", ", &pos);
	fileVer2 += szBuf;
	fileVer2 += "Rel.";
	PARSE_GetNextToken(szBuf, (char *)fileVer.c_str(), ", ", &pos);
	fileVer2 += szBuf;

	str += "MyMemo" ;// program name;
	str += " Version ";
	str += fileVer2; //Version Number - MyMemo.cpp�� ��ܿ� ���ǵǾ� ���� 
	SetDlgItemText(IDC_STATIC1, str.c_str());
	m_myHomeLink.SetWindowText("http://ykish.wo.to");
	m_myHomeLink.SetLink(TRUE)
		.SetTextColor(RGB(0,0,255))
		.SetFontUnderline(TRUE)
		.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND1));
	SetDlgItemText(IDC_EDIT1, "������ �ؽ�Ʈ ����� �޸� ���α׷��Դϴ�. \r\n������ ������� �ϼŵ� �˴ϴ�. \r\n��, ��Ȩ�������� ���� ã�ƿ� �ֽð�, �Խ��ǿ� ���븦 ���� �ֽø� ���ڽ��ϴ�.\r\n�Ʒ� Ȩ������ ��ũ�� �� Ŭ�����ּ���! ^^");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

ClsItem * CMyMemoApp::GetClsItem(int iImage)
{
	for(int i=0; i<ClsItemListCount;i++) {
		if (m_ClsItemList[i].iImage  == iImage)
			return &m_ClsItemList[i];
	}
	return NULL;
}

CString CMyMemoApp::GetVersion()
{
	CFileVersion cfv;
	char szBuf[MAX_PATH]={0,};
	string str, fileVer, fileVer2;
	::GetModuleFileName(AfxGetInstanceHandle( ), szBuf, MAX_PATH-1);
	cfv.Open(szBuf);
	fileVer = cfv.GetFileVersion();
	cfv.Close();
	return fileVer.c_str();

}
