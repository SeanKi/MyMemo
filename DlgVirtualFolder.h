#if !defined(AFX_DLGVIRTUALFOLDER_H__3E4AC9C4_AFA2_4CEE_80FC_B3266FFF385F__INCLUDED_)
#define AFX_DLGVIRTUALFOLDER_H__3E4AC9C4_AFA2_4CEE_80FC_B3266FFF385F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVirtualFolder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVirtualFolder dialog

class CDlgVirtualFolder : public CDialog
{
// Construction
public:
	CDlgVirtualFolder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVirtualFolder)
	enum { IDD = IDD_VIRTUALFOLDER };
	CString	m_sAlias;
	CString	m_sPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVirtualFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVirtualFolder)
	afx_msg void OnBrowserFolder();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIRTUALFOLDER_H__3E4AC9C4_AFA2_4CEE_80FC_B3266FFF385F__INCLUDED_)
