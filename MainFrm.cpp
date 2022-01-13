// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MyMemo.h"

#include "LeftView.h"
#include "MainFrm.h"
#include "MyMemoDoc.h"
#include "MyMemoView.h"
#include <imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMyMemoApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_VIEW_TEST, OnViewTest)
	ON_COMMAND(ID_VIEW_ONTOP, OnViewOntop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ONTOP, OnUpdateViewOntop)
	ON_WM_ENDSESSION()
	ON_WM_QUERYENDSESSION()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_HANMODE, OnUpdateHan)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_HANMODE,
};

LPCSTR RegisterSimilarClass(LPCSTR lpszNewClassName, LPCSTR lpszOldClassName, UINT nIDResource)
{
	// Get class info for old class.       //
	HINSTANCE hInst = AfxGetInstanceHandle();       
	WNDCLASS wc;
	if (!::GetClassInfo(hInst, lpszOldClassName, &wc)) {
		TRACE("Can't find window class %s\n", lpszOldClassName);
		return NULL;
	}
	// Register new class with same info, but different name and icon.
	wc.lpszClassName = lpszNewClassName;
	wc.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nIDResource));

	if (!RegisterClass(&wc)) 
	{
		TRACE("Unable to register window class%s\n", lpszNewClassName);
		return NULL;
	}
	return lpszNewClassName;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
/*	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}*/
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) /*||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndStatusBar.ShowWindow(m_ShowStatusBar?SW_SHOW:SW_HIDE);
//	m_wndStatusBar.GetDockingFrame()->RecalcLayout();
	m_wndToolBar.ShowWindow(m_ShowToolBar?SW_SHOW:SW_HIDE); 
	// 현재 쓰지 않는 UI를 잠시 가린다. 2002-5-11
	m_wndToolBar.GetToolBarCtrl().HideButton( ID_VIEW_READONLY, TRUE);
	CMenu * pMenu = GetMenu();
//	pMenu->DeleteMenu(ID_VIEW_READONLY, MF_BYCOMMAND);
	pMenu->DeleteMenu(ID_VIEW_TEST, MF_BYCOMMAND);
	//
	//	m_wndToolBar.GetDockingFrame()->RecalcLayout(); //화면 표시 전이라 재배치 명령 필요없음
	SetWindowPos(m_AlwaysOnTop?&wndTopMost:&wndNoTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	CProfilePrivate & m_profile = theApp.m_Profile;
	m_profile.SetSection("WINDOW");
	int tx = m_profile.GetInt("treeW");
	if (tx < 0) tx= 100;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(tx, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyMemoView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	m_AlwaysOnTop = m_profile.GetInt("AlwaysOnTop", 0);
	m_ShowToolBar = m_profile.GetInt("ShowToolBar", 1);
	m_ShowStatusBar = m_profile.GetInt("ShowStatusBar", 1);
	int val;
	m_profile.SetSection("IMM");
	CString sVal;
	m_profile.GetString("mode", sVal, "HAN");
	HIMC himc;
	DWORD dwConversion, dwSentence;
	himc = ImmGetContext(GetSafeHwnd());
	ImmGetConversionStatus(himc, &dwConversion, &dwSentence);
	if (sVal == "HAN") {
		if (!(dwConversion & IME_CMODE_HANGUL))
		ImmSetConversionStatus(himc, dwConversion | 
		IME_CMODE_HANGUL, dwSentence);
	} else {
		ImmSetConversionStatus(himc, IME_CMODE_ALPHANUMERIC,
		dwSentence);
	}


	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	static LPCSTR s_winClassName = NULL;
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	CString sClass(cs.lpszClass);
	if (sClass.Left(4) == "Afx:") {
		if (s_winClassName==NULL) {
			// One-time initialization: register the class
			//
			s_winClassName = RegisterSimilarClass(AfxGetAppName(),
				cs.lpszClass, IDR_MAINFRAME);
			if (!s_winClassName)
				return FALSE;
		}
		cs.lpszClass = s_winClassName;
	}
	CProfilePrivate & m_profile = theApp.m_Profile;
	m_profile.SetSection("WINDOW");
	const int INITIALX = -32000;
	const int INITIALY = -32000;
	int x = m_profile.GetInt("x", INITIALX);
	int y = m_profile.GetInt("y", INITIALY);
	int cx =  m_profile.GetInt("cx", -1);
	int cy =  m_profile.GetInt("cy", -1);
	const int MINW = 420;
	const int MINH = 330;
	int ScreenCX =GetSystemMetrics(SM_CXSCREEN);
	int ScreenCY =GetSystemMetrics(SM_CYSCREEN);
	//조정
	CRect rcclient(x, y, x + cx, y + cy);
	CRect rcscreen(0, 0, ScreenCX, ScreenCY);
	CRect rcinter;
	if (rcinter.IntersectRect(&rcclient, &rcscreen) == FALSE) // 화면에 보이지 않으면 초기화
	{
		x = -32000;
		y = -32000;
		cx = -1;
		cy = -1;
	}
	if (cx < MINW) cx = MINW;
	if (cy < MINH) cy = MINH;
	if (x == INITIALX) x= (ScreenCX - cx) / 2; //초기값이 없으면 가운데로
	if (y == INITIALY) y= (ScreenCY - cy) / 2; //초기값이 없으면 가운데로
	cs.x = x;
	cs.y = y;
	cs.cx = cx;
	cs.cy = cy;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CMyMemoView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CMyMemoView* pView = DYNAMIC_DOWNCAST(CMyMemoView, pWnd);
	return pView;
}

CLeftView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CMyMemoView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CMyMemoView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CMyMemoView,
	// process the menu commands...
	if (pView != NULL)
	{
/*		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
//				CListCtrl& refListCtrl = pView->GetListCtrl();
		//		refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
			*/
	}
}

void CMainFrame::OnClose() 
{
	CRect rc;
	GetWindowRect(&rc);
	CProfilePrivate & m_profile = theApp.m_Profile;
	m_profile.SetSection("WINDOW");
	CMenu * pMenu = GetMenu();
	m_ShowToolBar = 	m_wndToolBar.IsWindowVisible(); //(pMenu->GetMenuState(ID_VIEW_TOOLBAR, MF_BYCOMMAND)==MF_CHECKED);
	m_ShowStatusBar = m_wndStatusBar.IsWindowVisible(); //(pMenu->GetMenuState(ID_VIEW_STATUS_BAR, MF_BYCOMMAND)==MF_CHECKED);
	
	char buff[255];
	int cur, min;
	m_wndSplitter.GetColumnInfo(0, cur, min);
	m_profile.WriteString("x", itoa(rc.left, buff, 10));
	m_profile.WriteString("y", itoa(rc.top, buff, 10));
	m_profile.WriteString("cx", itoa(rc.Width(), buff, 10));
	m_profile.WriteString("cy", itoa(rc.Height(), buff, 10));
	m_profile.WriteString("treeW", itoa(cur, buff, 10));
	m_profile.WriteString("ShowToolBar", itoa(m_ShowToolBar, buff, 10));
	m_profile.WriteString("ShowStatusBar", itoa(m_ShowStatusBar, buff, 10));
	m_profile.WriteString("AlwaysOnTop", itoa(m_AlwaysOnTop, buff, 10));
	m_profile.SetSection("IMM");
	CString sVal;
	m_wndStatusBar.GetPaneText(3, sVal);
	m_profile.WriteString("mode", sVal);

	((CMyMemoView *)((CMyMemoDoc*)GetActiveDocument())->m_viewMain)->SaveViewFile();		
	CFrameWnd::OnClose();
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if (nIndex == 0) //File menu이면
		((CMyMemoDoc *)GetActiveDocument())->fUpdateMRUMenuItem();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	LRESULT r = CFrameWnd::WindowProc(message, wParam, lParam);
	return r;
}

void CMainFrame::OnUpdateHan(CCmdUI *pCmdUI) 
{
    pCmdUI->Enable(); 
	HIMC himc;
	DWORD dwConversion, dwSentence;
	himc = ImmGetContext(GetSafeHwnd());
	ImmGetConversionStatus(himc, &dwConversion, &dwSentence);
	CString strTmp;
	if (!(dwConversion & IME_CMODE_HANGUL))    
		strTmp= "ENG"; 
	else
		strTmp= "HAN";
    pCmdUI->SetText(strTmp); 
}

void CMainFrame::OnViewTest() 
{
	CMenu * pMenu = GetMenu();
	m_ShowToolBar = (pMenu->GetMenuState(ID_VIEW_TOOLBAR, MF_BYCOMMAND)==MF_CHECKED);
	m_ShowStatusBar = (pMenu->GetMenuState(ID_VIEW_STATUS_BAR, MF_BYCOMMAND)==MF_CHECKED);

//	CColorDialog dlg;
//	dlg.DoModal();
//	COLORREF ref = dlg.GetColor();
	
}

void CMainFrame::OnViewOntop() 
{
	m_AlwaysOnTop = 1 - m_AlwaysOnTop; //Toggle value
	SetWindowPos(m_AlwaysOnTop?&wndTopMost:&wndNoTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

void CMainFrame::OnUpdateViewOntop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_AlwaysOnTop);
}

//DEL void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
//DEL {
//DEL 	// TODO: Add your command update UI handler code here
//DEL 	CMenu * pMenu = GetMenu();
//DEL 	m_ShowToolBar = (pMenu->GetMenuState(ID_VIEW_TOOLBAR, MF_BYCOMMAND)==MF_CHECKED);
//DEL 	pCmdUI->SetCheck(m_ShowToolBar);	
//DEL }

void CMainFrame::OnEndSession(BOOL bEnding) 
{
	CFrameWnd::OnEndSession(bEnding);
	OnClose();
	// TODO: Add your message handler code here
	
}

BOOL CMainFrame::OnQueryEndSession() 
{
	((CMyMemoView *)((CMyMemoDoc*)GetActiveDocument())->m_viewMain)->SaveViewFile();		

	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;

	
	// TODO: Add your specialized query end session code here
	
	return TRUE;
}
