// MyMemo.h : main header file for the TEST3 application
//

#if !defined(AFX_TEST3_H__50F91850_F906_46D5_8E46_87F9FA673423__INCLUDED_)
#define AFX_TEST3_H__50F91850_F906_46D5_8E46_87F9FA673423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "ProfilePrivate.h"
#include "Global.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CMyMemoApp:
// See MyMemo.cpp for the implementation of this class
//

class CMyMemoApp : public CWinApp
{
public:
	CString GetVersion();
	ClsItem * GetClsItem(int iImage);
	CMyMemoApp();
	CProfilePrivate m_Profile;

	ClsItem m_ClsItemList[6];
	map<int, ClsItem> m_mapClsItems;
	int ClsItemListCount;
//	CMap<int,int,ClsItem*,ClsItem*&> m_mapClsItem; //VirtualFolderList map
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyMemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST3_H__50F91850_F906_46D5_8E46_87F9FA673423__INCLUDED_)
