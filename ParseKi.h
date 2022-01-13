// ParseKi.h: interface for the CParseKi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSEKI_H__F4A1D104_56F8_11D3_B526_0000E8482C6B__INCLUDED_)
#define AFX_PARSEKI_H__F4A1D104_56F8_11D3_B526_0000E8482C6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#pragma warning(disable:4786)
#include <string>
using namespace std ;

class CParseKi  
{
public:
	CString m_NextDPos;
	CParseKi();
	virtual ~CParseKi();
	struct FIELD {
	CString sangho, upjong, dept, 
		tel, post, addr;
	};
	struct FIELD2 {
		CString sCap, sValue;
		int iDno, iFno, iHno;//화면 출력 순서와 파일에 출력 순서, HTML상에서 배치 순서
	};

	CArray <FIELD2, FIELD2> m_aryField;
	struct ST {
		CString sKey, sValue;
	};
public:
//	static string * AddDirSeparator(string * psPath, char SEP='\\');
	static string AddDirSeparator(LPCSTR szPath, char SEP='\\');
//	static LPSTR AddDirSeparator(LPSTR szPath, char SEP='\\');
	CString sRightBack(CString s, char c);
	static CString sPathAddBackSlash(CString sPath);
	CString GetFileTitle(CString &sPath);
	static CString GetFilePath(CString sPath, char SEP='\\');
	static CString GetFileName(CString spath, char SEP='\\');
	void WriteAryValue(CStdioFile &f, CArray  <CString, CString> &ar);
	void SortAryFieldByiHno();
	void SortAryFieldByiFno();
	void SetFieldfromAry(CArray <CString, CString> &ar);
	int  GetAryValue(CString s, CStringArray &ar);
	void GetAryValue(CString &s, LPCTSTR pSub1, CStringArray &ar);
	CString sRevRead1Line(LPTSTR pStr, char * p);
	void WriteSection(CStdioFile &f, LPCTSTR pSection);
	CString GetKeyString(CString &sLine, LPCTSTR szKey);
	CString GetKeyString(CString &sLine, CString &sKey);
	BOOL GetSectionStrings(CStdioFile &f, LPCTSTR pSub1, CArray <ST, ST> &ar);
	CString GetSectionStrings(CStdioFile &f, LPCTSTR pSub1);
	BOOL WriteKeyString(CStdioFile &f, LPCTSTR pKey, LPCTSTR pValue);
	int SkipStr(CString &s, LPCTSTR pSub1, int &pos);
	CString sLeft(CString &s, LPCTSTR pSub1, int &pos);
	CString sRead1Line(CString &s, int &pos, int iTrim);
	CString sMidInTag(CString &s, LPCTSTR pTagStr, LPCTSTR pInStr, int &pos);
	CString sMidIn(CString &s, LPCTSTR pSub1, LPCTSTR pSub2, LPCTSTR pInStr, int &pos);
	CString GetTD(CString &s, int &pos);
	CString GetTR(CString &s, int &pos);
	CString sMid(CString &s, LPCTSTR pSub1, LPCTSTR pSub2,int &pos);
	CString GetNextRow(int& pos);
	CString m_Column[6];
	CString sPlusCR(LPCTSTR lpsz);
	CString sStrRm(CString& sz, LPCTSTR lpsz);
	CString sLeft(CString s, LPCTSTR lpszSub1);
	CString sLeft(CString &s, char c, int &pos);
	static CString sRight(CString s,  char c);
	CString sTagRm(CString &sz);
/*	CString m_juso;
	CString m_post;
	CString m_tel;
	CString m_dept;
	CString m_upjong;
	CString m_sangho; */
	char * szFind(CString s, LPCTSTR lpszSub1);
	CString sMid(CString s, LPCTSTR lpszSub1, LPCTSTR lpszSub2);
	char * m_pStrC;
	char * m_pStrN;

};

#endif // !defined(AFX_PARSEKI_H__F4A1D104_56F8_11D3_B526_0000E8482C6B__INCLUDED_)
