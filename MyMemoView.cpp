// test3View.cpp : implementation of the CMyMemoView class
//

#include "stdafx.h"
#include "MyMemo.h"

#include "MyMemoDoc.h"
#include "MyMemoView.h"
#include "Global.h"
#include ".\mymemoview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PrintString(char *p, DWORD pSize)
{
    CDC             dc;
    CPrintDialog    printDlg(FALSE);
    CRect           r;
    int             nHeight;
    
    // ask the user to select a printer
    if (printDlg.DoModal() == IDCANCEL)
        return;
    
    // Attach a printer DC
    dc.Attach(printDlg.GetPrinterDC());
    dc.m_bPrinting = TRUE;

    // use Textmappingmode, that's easiest to map the fontsize
    dc.SetMapMode(MM_TEXT);
    
    // setup font specifics
    LOGFONT LogFont;
    
    CFont	aFont, *oldFont;
    
    LogFont.lfHeight = -MulDiv(10, GetDeviceCaps(dc, LOGPIXELSY), 72);
    LogFont.lfWidth = 0;
    LogFont.lfEscapement = 0;
    LogFont.lfOrientation = 0;
    LogFont.lfWeight = 0;
    LogFont.lfItalic = false;
    LogFont.lfUnderline = 0;
    LogFont.lfStrikeOut = 0;
    LogFont.lfCharSet = ANSI_CHARSET;
    LogFont.lfOutPrecision = OUT_TT_PRECIS;
    LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    LogFont.lfQuality = DEFAULT_QUALITY;
    LogFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
    lstrcpy (LogFont.lfFaceName, "MS Sans Serif");
    dc.SetBkMode(OPAQUE);
    aFont.CreateFontIndirect ( &LogFont );
    // ok, we've build the font, now use it
    oldFont = dc.SelectObject( &aFont );        
        
    // Get the application title
    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);
    
    // Initialise print document details
    
    DOCINFO di;
    ::ZeroMemory (&di, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    // application title appears in the spooler view
    di.lpszDocName = strTitle;
    
    // Begin a new print job
    BOOL bPrintingOK = dc.StartDoc( &di );
        
    // Get the printing extents and store in the m_rectDraw field of a 
    // CPrintInfo object
    CPrintInfo Info;
    int w = dc.GetDeviceCaps(HORZRES);
    int h = dc.GetDeviceCaps(VERTRES);
	// by kish
	const int OFFSET_LEFT=64;
	const int OFFSET_TOP=64;
	//	
    Info.m_rectDraw.SetRect(OFFSET_LEFT,OFFSET_TOP, w, h);
    
    char *startAt = p;
    int totalDone = 0;
    int lengthToGo = pSize;
    
   for (UINT page = Info.GetMinPage();
    bPrintingOK && totalDone < lengthToGo; page++)
    {
        // begin new page
        dc.StartPage();
        Info.m_nCurPage = page;
        
		int nLo = 0; 
		int nHi = lengthToGo-totalDone; 
		int nCount = lengthToGo-totalDone; 
		
		r = Info.m_rectDraw;

		// Calculate the amount of data that will fit on this page
		while(nLo<nHi) { 
			r.right = Info.m_rectDraw.right; 
			// Calculate the size of the rect but do not print
			nHeight = dc.DrawText(startAt, nCount, r, DT_CALCRECT|DT_WORDBREAK|DT_NOCLIP|DT_EXPANDTABS); 
			// use binary search to calculate size of this page
			if(nHeight<h) nLo = nCount; 
			if(nHeight>h) nHi = nCount; 
			if(nLo == nHi-1) nLo = nHi; 
			if(nLo<nHi) nCount = nLo+(nHi-nLo)/2;
		}
		//print the page 
		dc.DrawText(startAt, nCount, r, DT_WORDBREAK|DT_NOCLIP|DT_EXPANDTABS); 

        // go to next page
        startAt += nCount;
        totalDone += nCount;  

        // end page
        bPrintingOK = (dc.EndPage() > 0);
    } 


    // end a print job
    if (bPrintingOK)
        dc.EndDoc();
    else
        // abort job.
        dc.AbortDoc();
    
    
    // restore font
    dc.SelectObject(oldFont);
    // free font memory
    aFont.DeleteObject();
    // detach the printer DC
    dc.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// CMyMemoView

IMPLEMENT_DYNCREATE(CMyMemoView, CEditView)

BEGIN_MESSAGE_MAP(CMyMemoView, CEditView)
	//{{AFX_MSG_MAP(CMyMemoView)
	ON_COMMAND(ID_CHOOSEFONT, OnChoosefont)
	ON_COMMAND(ID_VIEW_WORDWRAP, OnViewWordwrap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORDWRAP, OnUpdateViewWordwrap)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_VIEW_READONLY, OnViewReadonly)
	ON_UPDATE_COMMAND_UI(ID_VIEW_READONLY, OnUpdateViewReadonly)
	ON_COMMAND(ID_ADDTODAY, OnAddtoday)
	ON_COMMAND(ID_SETTORDONLY, OnSet2RDonly)
	ON_COMMAND(ID_VIEW_TAPSTOPS_1, OnViewTapstops1)
	ON_COMMAND(ID_VIEW_TAPSTOPS_2, OnViewTapstops2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAPSTOPS_1, OnUpdateViewTapstops1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAPSTOPS_2, OnUpdateViewTapstops2)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

static void GetProfileFont(LPCTSTR szSec, LOGFONT* plf)
{
	CWinApp* pApp = AfxGetApp();
	plf->lfHeight = pApp->GetProfileInt(szSec, szHeight, -15);
	if (1)//plf->lfHeight != 0)
	{
		plf->lfWeight = pApp->GetProfileInt(szSec, szWeight, DefWeight);
		plf->lfWidth = pApp->GetProfileInt(szSec, szWidth, DefWidth);
		plf->lfOutPrecision = pApp->GetProfileInt(szSec, szOutPrecision, DefOutPrecision);
		plf->lfQuality = pApp->GetProfileInt(szSec, szQuality, DefQuality);
		plf->lfItalic = (BYTE)pApp->GetProfileInt(szSec, szItalic, DefItalic);
		plf->lfUnderline = (BYTE)pApp->GetProfileInt(szSec, szUnderline, DefUnderline);
		plf->lfPitchAndFamily = (BYTE)pApp->GetProfileInt(szSec, szPitchAndFamily, DefPitchAndFamily);
		plf->lfCharSet = (BYTE)pApp->GetProfileInt(szSec, szCharSet, DefCharSet);
		CString strFont = pApp->GetProfileString(szSec, szFaceName, DefFaceName);
		lstrcpyn((TCHAR*)plf->lfFaceName, strFont, sizeof plf->lfFaceName);
		plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
	}
}

static void WriteProfileFont(LPCTSTR szSec, const LOGFONT* plf, LOGFONT* plfOld)
{
	CWinApp* pApp = AfxGetApp();

	if (plf->lfHeight != plfOld->lfHeight)
		pApp->WriteProfileInt(szSec, szHeight, plf->lfHeight);
	if (plf->lfHeight != 0)
	{
		if (plf->lfHeight != plfOld->lfHeight)
			pApp->WriteProfileInt(szSec, szHeight, plf->lfHeight);
		if (plf->lfWeight != plfOld->lfWeight)
			pApp->WriteProfileInt(szSec, szWeight, plf->lfWeight);
		if (plf->lfWidth != plfOld->lfWidth)
			pApp->WriteProfileInt(szSec, szWidth, plf->lfWidth);
		if (plf->lfOutPrecision != plfOld->lfOutPrecision)
			pApp->WriteProfileInt(szSec, szOutPrecision, plf->lfOutPrecision);
		if (plf->lfQuality != plfOld->lfQuality)
			pApp->WriteProfileInt(szSec, szQuality, plf->lfQuality);
		if (plf->lfItalic != plfOld->lfItalic)
			pApp->WriteProfileInt(szSec, szItalic, plf->lfItalic);
		if (plf->lfUnderline != plfOld->lfUnderline)
			pApp->WriteProfileInt(szSec, szUnderline, plf->lfUnderline);
		if (plf->lfPitchAndFamily != plfOld->lfPitchAndFamily)
			pApp->WriteProfileInt(szSec, szPitchAndFamily, plf->lfPitchAndFamily);
		if (plf->lfCharSet != plfOld->lfCharSet)
			pApp->WriteProfileInt(szSec, szCharSet, plf->lfCharSet);
		if (_tcscmp(plf->lfFaceName, plfOld->lfFaceName) != 0)
			pApp->WriteProfileString(szSec, szFaceName, (LPCTSTR)plf->lfFaceName);
	}
	*plfOld = *plf;
}
/////////////////////////////////////////////////////////////////////////////
// CMyMemoView construction/destruction

CMyMemoView::CMyMemoView()
{
	// TODO: add construction code here
	m_hwndEdit = NULL;


}

CMyMemoView::~CMyMemoView()
{
}

BOOL CMyMemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);
		cs.dwExStyle |= WS_EX_CLIENTEDGE ;
	return CEditView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyMemoView drawing

void CMyMemoView::OnDraw(CDC* pDC)
{
	pDC->SetBkColor(RGB(255,0,0)); 	
	CMyMemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
//	CListCtrl& refCtrl = GetListCtrl();
//	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
}

extern CMyMemoApp theApp;
void CMyMemoView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();
	CMyMemoDoc* pDoc = GetDocument();
	pDoc->m_viewMain = this;
//생성자에서 이곳으로 옮김 2002.2.20
	CWinApp* pApp = AfxGetApp();
	m_bDefWordWrap = pApp->GetProfileInt(szSettings, szWordWrap, 1); //Wordwrap은 On이 기본
	m_iAutoSave = pApp->GetProfileInt(szSettings, "AutoSave", 10); //Wordwrap은 On이 기본
	m_bTabShapeProtect = pApp->GetProfileInt(szSettings, "TabShapeProtect", 1);
//	m_bDefWordWrapOld = m_bDefWordWrap;
	memset(&m_lfDefFont, 0, sizeof m_lfDefFont);
	GetProfileFont(szFont, &m_lfDefFont);
	m_lfDefFontOld = m_lfDefFont; //종료시 폰트가 바뀌었을 때만 저장하기 위한 환경 변수
	GetProfileFont(szPrintFont, &m_lfDefPrintFont);
	m_lfDefPrintFontOld = m_lfDefPrintFont;

	CProfilePrivate & m_profile = theApp.m_Profile;
	m_profile.SetSection("VIEW");
	m_nTabStops = m_profile.GetInt("TapStops", 32);
	SetWordWrap(m_bDefWordWrap);
	SetNewFont(m_lfDefFont);

}

/////////////////////////////////////////////////////////////////////////////
// CMyMemoView printing

BOOL CMyMemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyMemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyMemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyMemoView diagnostics

#ifdef _DEBUG
void CMyMemoView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMyMemoView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMyMemoDoc* CMyMemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyMemoDoc)));
	return (CMyMemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyMemoView message handlers
void CMyMemoView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

BOOL CMyMemoView::OpenViewFile(int viewType, CString sPath, CString *sVPath, BOOL bAddRML, VirtualFolderList * pVFL)
{
	CFile * pcf = NULL;
	CInetFile * pInetf = NULL;
	BOOL bOpen=FALSE;
	string sBuff;
	char szTmp[2049];
	DWORD dwRead;

	if (pVFL->nRemoteType > 0) {
		pInetf = pVFL->pInetf;
		bOpen = pInetf->OpenFile(sPath, "");
		if (bOpen) {
			while(1) {
				pInetf->ReadFile((PBYTE)szTmp, 2048, &dwRead);
				if (dwRead == 0) break;
				szTmp[dwRead] = '\0';
				sBuff += szTmp;
			};
			pInetf->CloseFile();
		}
	} else {
		pcf = new CFile;
		bOpen = pcf->Open(sPath, CFile::modeRead);
		if (bOpen) {
			while(1) {
				dwRead = pcf->Read((PBYTE)szTmp, 2048);
				if (dwRead == 0) break;
				szTmp[dwRead] = '\0';
				sBuff += szTmp;
			};
			pcf->Close();
		}
		delete pcf;
	}
	GetDocument()->m_docPath = sPath;
	DWORD dw = GetFileAttributes(GetDocument()->m_docPath);
	if (dw != 0xFFFFFFFF) { //파일이 없는 경우는 새로 작성할 수 있도록 ReadOnly를 주면 안된다.
		GetEditCtrl().SetReadOnly(dw & FILE_ATTRIBUTE_READONLY);
	}
	GetDocument()->m_pVFL = pVFL;
	if (sVPath)
		GetDocument()->SetTitle(*sVPath);
	if (bOpen) {
		GetDocument()->AddToRecentMemoList(*sVPath);
		if (viewType >= ID_VIRTUAL_L && viewType <= ID_TEXT_FILE) {
			GetEditCtrl().SetModify(FALSE);
			GetEditCtrl().SetWindowText(sBuff.c_str());
		}
		return TRUE;
	} else { //해당하는 곳에 파일을 못찾으면 
		//GetDocument()->m_docPath = (viewType >= ID_VIRTUAL_L && viewType <= ID_OPENFOLDER)?sPath:""; //폴더이면 
		GetEditCtrl().SetModify(FALSE);
		GetEditCtrl().SetWindowText("");
	}
	return FALSE;
}

void CMyMemoView::OnChoosefont() 
{
   // get current font description
   CFont* pFont = GetFont();
   LOGFONT lf;
   if (pFont != NULL)
	   pFont->GetObject(sizeof(LOGFONT), &lf);
   else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog dlg(&lf, CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	if (dlg.DoModal() == IDOK)
	{
		// switch to new font.
		m_font.DeleteObject();
		if (m_font.CreateFontIndirect(&lf))
		{
			DWORD dwHeight = lf.lfHeight ;
			CWaitCursor wait;
			SetFont(&m_font);
			m_lfDefFont = lf;

			if (GetPrinterFont() == NULL)
			{
				// notify container that content has changed
				//GetDocument()->UpdateAllItems(NULL);
			}
		}
	}
}

BOOL CMyMemoView::SaveViewFile()
{
	if (!GetEditCtrl().GetModify())
		return FALSE;
	CFile * pcf = NULL;
	CInetFile * pInetf = NULL;
	BOOL bOpen=FALSE;
	CString str;
	GetEditCtrl().GetWindowText(str);
	VirtualFolderList *pVFL = GetDocument()->m_pVFL;
	DWORD dwRead;
	if (pVFL->nRemoteType > 0) {
		pInetf = pVFL->pInetf;
		bOpen = pInetf->OpenFile(GetDocument()->m_docPath, "w");
		if (bOpen) {
			pInetf->WriteFile((unsigned char *)str.GetBuffer(str.GetLength()), str.GetLength(), &dwRead);
			pInetf->CloseFile();
			str.ReleaseBuffer();
		}
	} else {
		pcf = new CFile;
		bOpen = pcf->Open(GetDocument()->m_docPath, CFile::modeCreate | CFile::modeWrite);
		if (bOpen) {
			pcf->Write((const void *)str, str.GetLength());
			pcf->Close();
		}
		delete pcf;
	}
//	CFile cf;
//	if (cf.Open(GetDocument()->m_docPath, CFile::modeCreate | CFile::modeWrite)) { //변경 사항이 있으면 무조건 다시 써야 하므로 modeCreate flag필요
	if (bOpen) {
//		cf.WriteHuge((const void *)str, str.GetLength());
//		cf.Close();
		GetEditCtrl().SetModify(FALSE);
		GetDocument()->SetModifiedFlag(FALSE);
		return TRUE;
	}
	return FALSE;
}

void CMyMemoView::OnViewWordwrap() 
{
	// TODO: Add your command handler code here
	SetWordWrap(!((GetStyle() & ES_AUTOHSCROLL) == 0));
	/*
	LONG style = GetWindowLong(GetEditCtrl().m_hWnd, GWL_STYLE);
	//BOOL b = (style & ES_AUTOHSCROLL)?0:ES_AUTOHSCROLL;
	//style ^= ES_AUTOHSCROLL;//|= b;
	style ^= WS_HSCROLL;

	SetWindowLong(GetEditCtrl().m_hWnd, GWL_STYLE, style);
	CRect rect;
	GetClientRect(&rect);
	GetEditCtrl().SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
*/
}

void CMyMemoView::OnUpdateViewWordwrap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	pCmdUI->SetCheck(!(style & WS_HSCROLL));
}

//WordWrap시 ReadOnly status고려 '2003.1.10
BOOL CMyMemoView::SetWordWrap(BOOL bWordWrap)
{
	LONG l = ::GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bRead = (l & ES_READONLY);
	bWordWrap = !!bWordWrap;    // make sure ==TRUE || ==FALSE
	if (((GetStyle() & ES_AUTOHSCROLL) == 0) == bWordWrap)
		return FALSE;

	// preserve original control's state.
	CFont* pFont = GetFont();
	int nLen = GetBufferLength();
	TCHAR* pSaveText = new TCHAR[GetBufferLength()+1];
	GetWindowText(pSaveText, nLen+1);

	// create new edit control with appropriate style and size.
	DWORD dwStyle = dwStyleDefault & ~(ES_AUTOHSCROLL|WS_HSCROLL|WS_VISIBLE);
	if (!bWordWrap)
		dwStyle |= ES_AUTOHSCROLL|WS_HSCROLL;

	CWnd* pParent = GetParent();
	CRect rect;
	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	CWnd* pFocus = GetFocus();

	UINT nID = GetDlgCtrlID();

	HWND hWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE, _T("edit"), NULL, dwStyle,
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
		pParent->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);

	if (hWnd == NULL)
	{
		delete[] pSaveText;
		return FALSE;
	}

	// set the window text to nothing to make sure following set doesn't fail
	SetWindowText(NULL);

	m_hwndEdit = hWnd;
	// restore visual state
	::SetWindowText(hWnd, pSaveText);
	delete[] pSaveText;
	if (pFont != NULL)
	{
		ASSERT(pFont->m_hObject != NULL);
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)pFont->m_hObject, 0);
	}

	// detach old window, attach new
	SetDlgCtrlID(nID+1);
	HWND hWndOld = Detach();
	::SetWindowLong(hWndOld, GWL_WNDPROC, (LONG)*GetSuperWndProcAddr());
	ASSERT(m_hWnd == NULL);
	SubclassWindow(hWnd);
	ASSERT(m_hWnd == hWnd);
//	GetParentFrame()->SendMessage(WM_RECALCPARENT);

	UINT nTabStops = m_nTabStops;
	GetEditCtrl().SetTabStops(nTabStops);
	GetEditCtrl().SetMargins(8, 8);

	GetClientRect(&rect);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_DRAWFRAME);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	UpdateWindow();

	// destroy old
	::SetWindowPos(hWndOld, NULL, 0, 0, 0, 0,
		SWP_HIDEWINDOW|SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|
		SWP_NOZORDER);
	::DestroyWindow(hWndOld);

	// restore rest of state...
	GetEditCtrl().LimitText(nMaxSize);
	if (pFocus == this)
		SetFocus();

	// notify container that doc changed
//	GetDocument()->UpdateAllItems(NULL);
	GetEditCtrl().SetReadOnly(bRead);
	ASSERT_VALID(this);
	return TRUE;
}

int CMyMemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
/*   CFont* pFont = GetFont();
   LOGFONT lf;
   if (pFont != NULL)
	   pFont->GetObject(sizeof(LOGFONT), &lf);
   else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
*/
	GetObject (GetStockObject (OEM_FIXED_FONT), sizeof (LOGFONT),
										  (PSTR) &m_lfDefFont) ;
/*	m_font.CreateFontIndirect(&lf);
	SetFont(&m_font);
	m_font.DeleteObject();
*/		
	if (m_lfDefFont.lfHeight != 0)
	{
		m_font.CreateFontIndirect(&m_lfDefFont);
		SetFont(&m_font);
	}
	if (m_lfDefPrintFont.lfHeight != 0)
	{
		m_fontPrint.CreateFontIndirect(&m_lfDefPrintFont);
		SetPrinterFont(&m_fontPrint);
	}
	
	return 0;
}

void CMyMemoView::SetNewFont(LOGFONT lf)
{
	m_font.DeleteObject();
	if (m_font.CreateFontIndirect(&lf))
	{
		CWaitCursor wait;
		SetFont(&m_font);
		m_lfDefFont = lf;

		if (GetPrinterFont() == NULL)
		{
			// notify container that content has changed
			//GetDocument()->UpdateAllItems(NULL);
		}
	}

}

void CMyMemoView::OnDestroy() 
{
	CEditView::OnDestroy();
	CWinApp* pApp = AfxGetApp();
	
	pApp->WriteProfileInt(szSettings, szWordWrap, (GetStyle() & ES_AUTOHSCROLL)==0);
	WriteProfileFont(szFont, &m_lfDefFont, &m_lfDefFontOld);
	WriteProfileFont(szPrintFont, &m_lfDefPrintFont, &m_lfDefPrintFontOld);
	pApp->WriteProfileInt(szSettings, "TabShapeProtect", m_bTabShapeProtect); // by kish 2005.11.29
	char szTmp[32]={0,};
	CProfilePrivate & m_profile = theApp.m_Profile;
	m_profile.SetSection("VIEW");
	m_profile.WriteString("TapStops", itoa(m_nTabStops, szTmp, 10));

	
}

void CMyMemoView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	int len = GetWindowTextLength();
	char * str = new char[len+1];
	GetEditCtrl().GetWindowText(str, len);
	PrintString(str, len);
}

// ReadOnly기능 추가 '2003.1.10
void CMyMemoView::OnViewReadonly() 
{
	// TODO: Add your command handler code here
	LONG l = ::GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL b = (l & ES_READONLY);
 //	GetEditCtrl().SetReadOnly(!b);
	GetEditCtrl().SendMessage(EM_SETREADONLY, !b, 0);
}

void CMyMemoView::OnUpdateViewReadonly(CCmdUI* pCmdUI) 
{
	LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	pCmdUI->SetCheck((style & ES_READONLY)?1:0);
}

//DEL void CMyMemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	if ( (nChar >= '0' && nChar <= '9') || // VK_0 = '0' VK_9 = '9'
//DEL 		nChar >= 'A' && nChar <= 'Z' || //VK_A = 'A' VK_Z = 'Z'
//DEL 		nChar == VK_DELETE || nChar == VK_BACK)
//DEL 		return;
//DEL 
//DEL 	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
//DEL }

LRESULT CMyMemoView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_CTLCOLOR) {
		OutputDebugString("CTLColor");
	}
	if (message == WM_KEYDOWN) {
		if (GetKeyState(VK_CONTROL)<0) {
			if ('A' == (int)wParam) {
				GetEditCtrl().SetSel(0, -1);
			}
			if ('T' == (int)wParam) {
				OnAddtoday();
			}
		}


	}
	if (message == WM_CHAR)
	{
		if (!(GetEditCtrl().GetStyle() & ES_READONLY) && VK_TAB == (int)wParam) { // Read Only일 때는 X
			BOOL bshift = (GetKeyState(VK_SHIFT)<0);
			if (AddOrRemoveTabs(!bshift))
				return CEditView::WindowProc(message, 0, lParam);//1;
			else
				if (bshift)	wParam = VK_BACK; // shift일 경우는 백스페이스로 변경
			//if (GetKeyState(VK_SHIFT)<0) 
			//{
			//	TRACE("Shift Tab\n");
			//} else
			//{
			//	TRACE("Tab\n");
			//}
		}
	}
/*
	if (message == WM_QUERYENDSESSION) {
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
	}
	if (message == WM_ENDSESSION) {
		OnDestroy();
	}
*/
	if (message == WM_COMMAND)
	{
//		if (LOWORD(wParam) == ID_EDIT_FIND) // 찾기 바꾸기는 코딩할 필요없이 풀다운 메뉴에 추가만 하면 된다.
//		{
//		} else if (LOWORD(wParam) == ID_EDIT_REPEAT)
//		{
//		} else if (LOWORD(wParam) == ID_EDIT_REPLACE)
//		{
//		}
	}
	return CEditView::WindowProc(message, wParam, lParam);
}

void CMyMemoView::OnAddtoday() 
{
	// TODO: Add your command handler code here
	//GetEditCtrl().
	//2003.11.2
	if (GetEditCtrl().GetStyle() & ES_READONLY) return; // Read Only일 때는 X
	CTime ct = CTime::GetCurrentTime();

	CString s = ct.Format( "%Y-%m-%d" );
	GetEditCtrl().ReplaceSel( s );

}

void CMyMemoView::OnSet2RDonly() 
{
	// TODO: Add your command handler code here
	DWORD dw = GetFileAttributes(GetDocument()->m_docPath);
	if (dw == 0xFFFFFFFF) return;
	DWORD newdw;
	newdw = dw ^ FILE_ATTRIBUTE_READONLY;
 	GetEditCtrl().SetReadOnly(newdw & FILE_ATTRIBUTE_READONLY);
	SetFileAttributes(GetDocument()->m_docPath, newdw);
}

void CMyMemoView::OnViewTapstops1() 
{
	// TODO: Add your command handler code here
	m_nTabStops = 16;
	GetEditCtrl().SetTabStops(m_nTabStops);
}

void CMyMemoView::OnViewTapstops2() 
{
	// TODO: Add your command handler code here
	m_nTabStops = 32;
	GetEditCtrl().SetTabStops(m_nTabStops);
}

void CMyMemoView::OnUpdateViewTapstops1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nTabStops == 16); // by kish 20050711, 16과 32는 하드코딩된 샘
}

void CMyMemoView::OnUpdateViewTapstops2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nTabStops == 32); // by kish 20050711, 16과 32는 하드코딩된 샘
}

// 에디트 상자에 대해 일반 탭 및 블록상태에서의 탭 처리
int CMyMemoView::AddOrRemoveTabs(int bAdd)
{
	// 셀랙션 영역을 체크
	int nStartC, nEndC;
	GetEditCtrl().GetSel(nStartC, nEndC);
	//
	if (nStartC == nEndC && bAdd)
			return 0;
	int nStartL = 0, nEndL=0;
	int nStartLfC = 0, nEndLfC=0;
	int nStartCofL = 0, nEndCofL= 0;
	if (nStartC>=0)
	{
		nStartL = GetEditCtrl().SendMessage(EM_LINEFROMCHAR, nStartC, 0);
		nStartLfC =GetEditCtrl().SendMessage(EM_LINEINDEX, nStartL, 0); // 라인 0번째의 전체 character pos
		nStartCofL = nStartC - nStartLfC; //시작줄에서의 블록 시작 위치까지
	}
	if (nEndL >=0)
	{
		nEndL = GetEditCtrl().SendMessage(EM_LINEFROMCHAR, nEndC, 0);
		nEndLfC =GetEditCtrl().SendMessage(EM_LINEINDEX, nEndL, 0); // 라인 0번째의 전체 character pos
		nEndCofL = nEndC - nEndLfC; //끝줄에서의 블록 시작 위치까지
	}
	char buff[8*1024]="";
	CString sLinebuff;
	CString sLinesbuff;
	for (int nCurL = nStartL; nCurL <= nEndL; nCurL++)
	{
		memset(buff, 0, 8*1024);
		GetEditCtrl().GetLine(nCurL, buff, 8*1024);
		sLinebuff = buff;
		if (bAdd)
		{
			sLinebuff = "\t" + sLinebuff;
		} else
		{
			if (nStartC == nEndC)
			{
				int cposofline = nStartC - nStartLfC;
				if (cposofline > 0)
					if (sLinebuff[cposofline-1] == '\t')
						return 0; // 앞에 탭이 있으므로 탭버튼은 다음에서 백스페이스로 변환된다.
				return 1; // 탭 누른 행위가 무시된다.
			}

			if (buff[0] == '\t')
				sLinebuff = buff + 1;
			else
				if (!m_bTabShapeProtect)
					sLinebuff = buff;// 이줄은 땡길게 없으므로 그대로. 2005.11.29//
				else
					return 1; // 기본 탭동작이 안되도록 1을 넘긴다. 0이면 기본 탭동작이다. 
		}
		sLinesbuff += sLinebuff;
		sLinesbuff += "\r\n";
		if (nCurL == nStartL) // 시작 줄이면
		{
			;
		}
		if (nCurL == nEndL) // 끝줄이면
		{
			if (sLinesbuff.Right(2) == "\r\n")
				sLinesbuff = sLinesbuff.Left(sLinesbuff.GetLength() - 2);
		}
	}
	int nEndLfC_Length = GetEditCtrl().LineLength(nEndLfC);
	GetEditCtrl().SetSel(nStartLfC, nEndLfC + nEndLfC_Length);
	GetEditCtrl().ReplaceSel(sLinesbuff, TRUE);
	// 변경후 블록을 다시 씌움
	nStartLfC =GetEditCtrl().SendMessage(EM_LINEINDEX, nStartL, 0); // 라인 0번째의 전체 character pos
	nEndLfC =GetEditCtrl().SendMessage(EM_LINEINDEX, nEndL, 0); // 라인 0번째의 전체 character pos
	nEndLfC_Length = GetEditCtrl().LineLength(nEndLfC);
	GetEditCtrl().SetSel(nStartLfC, nEndLfC + nEndLfC_Length);

	return 1;
}
