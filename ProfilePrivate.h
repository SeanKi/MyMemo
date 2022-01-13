// ProfilePrivate.h: interface for the CProfilePrivate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILEPRIVATE_H__2854E6C1_5A60_11D4_A3C7_004F4E05B3F2__INCLUDED_)
#define AFX_PROFILEPRIVATE_H__2854E6C1_5A60_11D4_A3C7_004F4E05B3F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProfilePrivate  
{
public:
	static int AryFromComboBox(CStringArray &ary, CComboBox &cbo);
	static int AryFromListBox(CStringArray &ary, CListBox &lb);
	static int AryToListBox(CStringArray &ary, CListBox &lb);
	static int AryToComboBox(CStringArray &ary, CComboBox &cbo);
	BOOL WriteStringListFromListBox(LPCTSTR szSection, CListBox &lb);
	BOOL GetStringListToListBox(LPCTSTR szSection, CListBox &lb);
	BOOL GetStringListToComboBox(CComboBox &cbo);
	BOOL WriteStringListFromComboBox(CComboBox &cbo);
	BOOL GetStringListToComboBox(LPCTSTR szSection, CComboBox &cbo);
	BOOL WriteStringListFromComboBox(LPCTSTR szSection, CComboBox &cbo);
	BOOL GetStringList(LPCTSTR szSection, CStringArray &ary);
	BOOL WriteStringList(LPCTSTR szSection, CStringArray &ary);
	UINT GetInt(LPCTSTR szEntry);
	UINT GetInt(LPCTSTR szEntry, int Default);
	void SetFilename(LPCTSTR sz);
	void SetSection(LPCTSTR sz);
	BOOL WriteString(LPCTSTR szEntry, CString sVal);
	CString m_sSection;
	CString m_sFilename;
	BOOL GetString(LPCTSTR szEntry, CString &sVal, LPCTSTR szDefault);
	CProfilePrivate();
	virtual ~CProfilePrivate();

};

#endif // !defined(AFX_PROFILEPRIVATE_H__2854E6C1_5A60_11D4_A3C7_004F4E05B3F2__INCLUDED_)
