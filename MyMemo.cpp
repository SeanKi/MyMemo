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
 - 블록 탭 기능 추가
1.3.2.30 - 2005.8.22
 - 찾기/바꾸기 메뉴 추가
1.3.1.29 - 2005.7.11
 - 텍스트 상자에 좌우 마진을 8pixel(영문 1자) 
 - 탭간격을 16pixel과 32pixel중 선택 가능하게
 - 프로그램 시작시 화면에 나타나지 않을 수 있는 문제를 완전히 때려잡음(아마도 완전히 잡았을 것입니다 ^^)
1.3.0.28 - 2005.6.27
 - 홈피주소 koolwind.net -> ykish.wo.to
 - 시스템 종료시 destroy
1.3.0.24
  -2003.1.9
AddDir에서 Connection끊어졌을때 reconnect
서버에서 _FolderComment.txt만 폴더 설명으로 인식하던거 해결 
-> 서버에서 폴더 설명이 안보이는 경우가 있어서 이유를 몰랐는데 오늘잡음
-> FileExists 네트워크 구현
1.3.0.23
-2003.1.5
가상디렉토리 삭제 안되는 오류 수정, FTP의 패스워드 입력상자를 별표(*)형태로 안보이게 바꿈
1.3.0.22
Remote Folder(FTP)의 하위디렉토리 삭제 기능 구현
// 1.3.0.21
// 2002.12.18
FTp기능 추가, 공백있는 디렉토리명에서 문제 해결
// 2002.12.6
FTP연결시 오류가 있으면 메시지 발생 -> 아이콘을 변경해서 보여주는 방법 고려해야됨. Reconnect메뉴도 필요
StrKi2.cpp의 사용 중지 - STD_GetNextToken 대신 PARSE_GetNextToken을 사용하고, 이 때 발생한 파일이 추가 안되던 오류도 처리
// 2002.12.5
Ctrl+shift+ins -> Ctrl+ins
Ctrl+del -> del
ftp접근시 wait cursor표시
  // 2002.12.1
폴더 정보가 저장이 안되던 문제 해결 - VirtualFolder들이 모두 ITEM_ROOT로 지정되어 잇어 생긴문제
FTP에서 폴더 보여주기 오류를 잡고 파일 메모 지우기까지 작업
// 1.2.5.20 - 2002.7.21
하위 폴더나 메모가 없을 경우 새로 추가 할 때 프로그램이 죽는 에러 수정
// 1.2.5.19 
- 2002.7.18
폴더 설명파일을 ___폴더설명.txt -> _FolderComment.txt을 기본으로 바꿈. 기존의 ___폴더설명.txt이 있으면 그대로 인식하고,  Readme.txt파일이 존재할 경우 readme.txt파일을 폴더 설명 파일로 보여줌. 이기능이 나름대로 요긴하리라 생각되어 추가하였습니다. 
만약 폴더설명파일이 여러가지 있을 경우 우선순위는 _FolderComment.txt>___폴더설명.txt>ReadMe.txt 순이며, 폴더설명파일이 없을 경우는 _FolderComment.txt로 생성됩니다. 

- 2002.6.27
// 폴더가 접혀져 있는 상태에서 New Folder와 New Memo시 문제가 생겨 먼저 폴더를 연 후 작업
// 1.2.5.18 - 2002.5.31
// 트리뷰에서 아이템 없는 곳을 선택하면 프로그램이 죽어버리는 무서운 에러 해결.
// 현재아이템을 얻어올때 NULL이 넘어와서 생긴 문제
// 1.2.5.17 - 2002.5.27
// 초기 실행시 중간 위치에 뜨게 하는 것에서 - 화면 크기를 1024*768로 
//     가정했던 오류 해결(실제 화면크기 조정)
// 위치가 -1이하면 중간으로 위치시켰던 오류 해결(멀티모니터에서 문제)
//     , 일단 환경파일에 x, y값이 없거나 -4000000이면 없는 값으로 간주하여 중앙으로 위치시킴
// 1.2.5.16
// 가상 폴더가 제대로 저장 되지 않거나 기타 오류를 일으키는 문제 수정 - m_mapVFL을 쓰지 않음
// 가상 폴더 추가시 경로 선택하면 폴더명이 가상 폴더명으로 자동 입력(이미 입력되어 있으면 적용안함) 
// 1.2.5.15
// 2002.5.20
// 정렬 방식 바꾸면서 새 가상 폴더 추가후 정렬시 에러 수정, 대소문자 구분하여 정렬되던 것 수정
// 2002.5.?
//*메모 트리뷰의 아이템들 정렬 순서 변경 -> 메모 디렉토리(가상폴더), 폴더, 메모 순 으로 정렬하여, 보기가 쉽도록 함
//.메모 루트 새로 고침 안되었던 문제 해결
// 1.2.1.14
// 2002.5.1
// ToolBar, StatusBar Show/Hide 상태 저장
// Always On Top 메뉴 추가, 상태 저장
// 2002.4.30
// Root Item은 새로고침이 되지 않던 문제 해결 
// CLeftView::OnUpdateProperty에서 ClsItem::nKind로 Enable체크하도록 변경
// Sort순서 - 한글, 알파벳 순서로 
// Sort방법을 폴더 - 메모 순으로 적용
// 1, 2, 1, 13
// m_memoList에서 VirtualFolderList와 CMap을 이용한 새 구조로 변경
// Ver1.2.1.12 
// 2002.4.4
//.처음 실행시 '기본' 가상폴더(메모디렉토리)를 만들던 기능 제거(루트폴더가 기본폴더 기능을 하므로)
// 2002.3.29
// 
.자식아이템없는 아이템에서 메모나 폴더 추가시 에러 해결
.자동저장(기본 - 변경 10초후)
.아이콘 좀 더 깔끔하게 변경
*메모루트폴더에 설명 달 수 있고 그 아래 메모디렉토리는 물론 폴더와 메모도 달 수 있도록 함
.메모나 폴더 드래그 시 위나 아래의 끝에 도달하면 스크롤이 가능할때 가지 자동 스크롤 된다.
.폴더에 메모나 폴더를 끌어다 놓으면 자동으로 그 폴더가 펼쳐지도록 함.
.끌어다 놓은 후 다른 엉뚱한 아이템이 선택되는 것을 고침.
.Tree아이템들을 이동시 약간의 시간간격을 둔 후 메모가 표시하도록 하였다. Tree이동시 원치않는 메모는 보지 않도록 하고 그것이 최근메모목록에 등록되는 것 또한 막는다.
.새 메모나 폴더를 만든 후 여전히 부모 메모의 내용이 보이던 문제를 해결
// 2002. 2.21 - Resource ID - IDC_HAND -> IDC_HAND1으로 변경, 기본 ID와 중복 때문에
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
	// 프로그램의 중복 실행을 방지
	hImHere=::CreateMutex(NULL, TRUE, _T("MyMemoMutex"));
	if(ERROR_ALREADY_EXISTS==::GetLastError())
	{
		HWND hWnd=::FindWindow(AfxGetAppName(), NULL/*m_pszAppName*/);
		::SetForegroundWindow(hWnd);
		::CloseHandle(hImHere);
		::MessageBox(hWnd,MyLoadString(IDS_PROGRAM_ALREADY_STARTED), "Warning",MB_OK);
		return FALSE;
	}
	// 프로그램의 중복 실행을 방지 - 끝
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
	m_ClsItemList[2].SetItem(CTL_TEXT, ID_VIRTUAL_R, ID_VIRTUAL_R, KIND_VIRTUAL|KIND_REMOTE|KIND_FOLDER, ""); //<-network와 ftp의 차이는 
	m_ClsItemList[3].SetItem(CTL_TEXT, ID_CLOSEDFOLDER, ID_OPENFOLDER, KIND_FOLDER, ""); //<-이곳에 확장자 대신에 대표로 보여줄 설명 파일을 보여주는 건 어떨까.
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
	str += fileVer2; //Version Number - MyMemo.cpp의 상단에 정의되어 있음 
	SetDlgItemText(IDC_STATIC1, str.c_str());
	m_myHomeLink.SetWindowText("http://ykish.wo.to");
	m_myHomeLink.SetLink(TRUE)
		.SetTextColor(RGB(0,0,255))
		.SetFontUnderline(TRUE)
		.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND1));
	SetDlgItemText(IDC_EDIT1, "간편한 텍스트 기반의 메모 프로그램입니다. \r\n배포는 마음대로 하셔도 됩니다. \r\n단, 제홈페이지에 가끔 찾아와 주시고, 게시판에 자취를 남겨 주시면 고맙겠습니다.\r\n아래 홈페이지 링크를 꼭 클릭해주세요! ^^");
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
