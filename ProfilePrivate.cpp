// ProfilePrivate.cpp: implementation of the CProfilePrivate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProfilePrivate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 2002.3.27 	UINT GetInt(LPCTSTR szEntry, int Default); 추가, Default값을 넣을 수 있음
// 2001.6.28 배열을 읽고 쓰는 다음 함수 추가
/*	static int AryFromComboBox(CStringArray &ary, CComboBox &cbo);
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
*/
// 2000.11.7
// GetInt를 추가

#define ASSERTSTRING(sz) ASSERT(sz != "")
//#define ASSERT1 ASSERTSTRING(m_sSection);ASSERTSTRING(m_sFilename)
#define ASSERT1 ASSERTSTRING(m_sFilename)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfilePrivate::CProfilePrivate()
{

}

CProfilePrivate::~CProfilePrivate()
{

}

//리턴값은 받는 문자 크기
BOOL CProfilePrivate::GetString(LPCTSTR szEntry, CString &sVal, LPCTSTR szDefault)
{
	ASSERT1;
	//sVal = AfxGetApp()->GetProfileString(m_sSection, szEntry, szDefault);
	char szReturnedString[1024];
	DWORD nSize=1024;
	int nRSize =
	GetPrivateProfileString(
		m_sSection,        // points to section name
		szEntry,        // points to key name
		szDefault,        // points to default string
		szReturnedString,  // points to destination buffer
		nSize,              // size of destination buffer
		m_sFilename        // points to initialization filename
	);
	sVal = szReturnedString;
	return nRSize;
}

BOOL CProfilePrivate::WriteString(LPCTSTR szEntry, CString sVal)
{
	ASSERT1;
	return WritePrivateProfileString(
			m_sSection,        // points to section name
			szEntry,        // points to key name
			sVal,
			m_sFilename  // pointer to initialization filename
			);

}

void CProfilePrivate::SetSection(LPCTSTR sz)
{
	m_sSection = sz;
}

void CProfilePrivate::SetFilename(LPCTSTR sz)
{
	m_sFilename = sz;
}

UINT CProfilePrivate::GetInt(LPCTSTR szEntry, int Default)
{
	return
		GetPrivateProfileInt(
			m_sSection,        // points to section name
			szEntry,        // points to key name
			Default,        // default Int(KeyName이 발견되지 않을 때) // 2002.2.13, -1로 변경
			   // size of destination buffer
			m_sFilename        // points to initialization filename
		);
}
UINT CProfilePrivate::GetInt(LPCTSTR szEntry)
{
	return
		GetPrivateProfileInt(
			m_sSection,        // points to section name
			szEntry,        // points to key name
			-1,        // default Int(KeyName이 발견되지 않을 때) // 2002.2.13, -1로 변경
			   // size of destination buffer
			m_sFilename        // points to initialization filename
		);
}
//특정 섹션에 리스트를 담는다. 리스트의 끝은""로 인식
BOOL CProfilePrivate::WriteStringList(LPCTSTR szSection, CStringArray &ary)
{
	ASSERT1;
	CString sEntry;
	for (int i=0; i<ary.GetSize();i++) {
		sEntry.Format("%d", i);
		if (!WritePrivateProfileString(
			szSection,        // points to section name
			sEntry,        // points to key name
			ary[i],
			m_sFilename  // pointer to initialization filename
			))
			return FALSE;
	}
	sEntry.Format("%d", i);
	return WritePrivateProfileString( //마지막 항목에 ""를 추가함으로써 끝을 표시
			szSection,        // points to section name
			sEntry,        // points to key name
			"",
			m_sFilename);
}

BOOL CProfilePrivate::GetStringList(LPCTSTR szSection, CStringArray &ary)
{
	ASSERT1;
	CString sEntry;
	char szReturnedString[1024];
	DWORD nSize=1024;
	
	for(int i=0;TRUE;i++) {
		sEntry.Format("%d", i);
		int nRSize =GetPrivateProfileString(
			szSection,        // points to section name
			sEntry,        // points to key name
			"",        // points to default string
			szReturnedString,  // points to destination buffer
			nSize,              // size of destination buffer
			m_sFilename        // points to initialization filename
		);
		if (nRSize <= 0)
			break;
		ary.Add(szReturnedString);
	}
	return i;
}

int CProfilePrivate::AryToComboBox(CStringArray &ary, CComboBox &cbo)
{
	for(int i=0;i<ary.GetSize();i++) {
		cbo.AddString(ary[i]);
	}
	return ary.GetSize();
}

BOOL CProfilePrivate::WriteStringListFromComboBox(LPCTSTR szSection, CComboBox &cbo)
{
	CStringArray ary;
	CString rStr;
	for(int i=0;i<cbo.GetCount();i++) {
		cbo.GetLBText(i, rStr);
		ary.Add(rStr);
	}
	WriteStringList(szSection, ary);
	return ary.GetSize();
}

BOOL CProfilePrivate::GetStringListToComboBox(LPCTSTR szSection, CComboBox &cbo)
{
	CStringArray ary;
	GetStringList(szSection, ary);
	AryToComboBox(ary, cbo);
	return ary.GetSize();
}

BOOL CProfilePrivate::WriteStringListFromComboBox(CComboBox &cbo)
{
	CStringArray ary;
	CString rStr;
	for(int i=0;i<cbo.GetCount();i++) {
		cbo.GetLBText(i, rStr);
		ary.Add(rStr);
	}
	CString stmp;
	stmp.Format("COMBOBOX_%d",cbo.GetDlgCtrlID());
	WriteStringList(stmp, ary);
	return ary.GetSize();
}

BOOL CProfilePrivate::GetStringListToComboBox(CComboBox &cbo)
{
	CStringArray ary;
	CString stmp;
	stmp.Format("COMBOBOX_%d",cbo.GetDlgCtrlID());
	GetStringList(stmp, ary);
	for(int i=0;i<ary.GetSize();i++) {
		cbo.AddString(ary[i]);
	}
	return ary.GetSize();
}
BOOL CProfilePrivate::WriteStringListFromListBox(LPCTSTR szSection, CListBox &lb)
{
	CStringArray ary;
	CString rStr;
	for(int i=0;i<lb.GetCount();i++) {
		lb.GetText(i, rStr);
		ary.Add(rStr);
	}
	WriteStringList(szSection, ary);
	return ary.GetSize();
}

BOOL CProfilePrivate::GetStringListToListBox(LPCTSTR szSection, CListBox &lb)
{
	CStringArray ary;
	GetStringList(szSection, ary);
	for(int i=0;i<ary.GetSize();i++) {
		lb.AddString(ary[i]);
	}
	return ary.GetSize();
}

int CProfilePrivate::AryToListBox(CStringArray &ary, CListBox &lb)
{
	for(int i=0;i<ary.GetSize();i++) {
		lb.AddString(ary[i]);
	}
	return ary.GetSize();
}

int CProfilePrivate::AryFromListBox(CStringArray &ary, CListBox &lb)
{
	CString rStr;
	for(int i=0;i<lb.GetCount();i++) {
		lb.GetText(i, rStr);
		ary.Add(rStr);
	}
	return ary.GetSize();
}

int CProfilePrivate::AryFromComboBox(CStringArray &ary, CComboBox &cbo)
{
	CString rStr;
	for(int i=0;i<cbo.GetCount();i++) {
		cbo.GetLBText(i, rStr);
		ary.Add(rStr);
	}
	return ary.GetSize();
}
