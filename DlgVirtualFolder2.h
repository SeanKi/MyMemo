#if !defined(AFX_DLGVIRTUALFOLDER2_H__99E007ED_4C8C_4C4F_99B9_12860D3BE825__INCLUDED_)
#define AFX_DLGVIRTUALFOLDER2_H__99E007ED_4C8C_4C4F_99B9_12860D3BE825__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVirtualFolder2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVirtualFolder2 dialog

class CDlgVirtualFolder2 : public CDialog
{
// Construction
public:
	void GetType();
	void ChangeType(int nType);
	int m_nType; //local or remote type - REMOTE_~
	CDlgVirtualFolder2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVirtualFolder2)
	enum { IDD = IDD_VIRTUALFOLDER2 };
	CString	m_sAlias;
	CString	m_sPath;
	CString	m_sHostname;
	CString	m_sUsername;
	CString	m_sPassword;
	CString	m_sServerdir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVirtualFolder2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVirtualFolder2)
	afx_msg void OnBrowserFolder();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIRTUALFOLDER2_H__99E007ED_4C8C_4C4F_99B9_12860D3BE825__INCLUDED_)
