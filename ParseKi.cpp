// ParseKi.cpp: implementation of the CParseKi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ParseKi.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TAB '\t'
#define CR "\r\n"

// Version
/*
1.0.0.2g 일반 버전
*/
// Release Note
/*
2001.07.25
sPathAddBackSlash함수를 static으로 전환
  GetFileName함수 추가 static, 전체 패스에서 파일이름만 뽑아낸다.
2000.11.8
GetFilePath함수와 sRight함수를 static으로 전환 - 앞으로도 계속 전환해 갈 것임
GetFilePath함수의 로직을 기존 File의 title로 찾는 방식에서 마지막 \를 기준으로 찾도록
변경. (전에는 왜 그렇게 했을까 -__-)

2000.06.21
::GetAryValue의 파라메터중 CArray <CString, CString> &ar은 CStringArray로 변경
2000.04.20
::sPathAddBackSlash(CString sPath) -> 새로 추가, 디렉토리 명 뒤에 "\"이 없으면 추가한다.

2000.03.23
::sRight(CString &s, char c) -> CString s로 수정(s에 대해 변경된 값을 넘길 필요가 없으므로)
2000.03.22
GetFilePath 버그 수정 
- Path 중간에 파일 타이틀과 같은 문자열이 있을 때 문제와 대소문자 구별 못하는 점 해결
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//extern CGet114App theApp;


CParseKi::CParseKi()
{
	m_aryField.SetSize(6);
	m_aryField[0].sCap = "sangho";
	m_aryField[1].sCap = "dept";
	m_aryField[2].sCap = "tel";
	m_aryField[3].sCap = "post";
	m_aryField[4].sCap = "addr";
	m_aryField[5].sCap = "upjong";
	m_aryField[0].iFno = 0;
	m_aryField[1].iFno = 1;
	m_aryField[2].iFno = 2;
	m_aryField[3].iFno = 3;
	m_aryField[4].iFno = 4;
	m_aryField[5].iFno = 5;
	m_aryField[0].iHno = 0;
	m_aryField[1].iHno = 2;
	m_aryField[2].iHno = 3;
	m_aryField[3].iHno = 4;
	m_aryField[4].iHno = 5;
	m_aryField[5].iHno = 1;
}

CParseKi::~CParseKi()
{

}

CString CParseKi::sMid(CString s, LPCTSTR lpszSub1, LPCTSTR lpszSub2)
{
	char * pszc = strstr(s,lpszSub1);
	m_pStrC = pszc; // lpszSub1을 포함한 psz의 위치
	if (pszc == NULL) return "";
	char * pszc2 = strstr(pszc,lpszSub2);
	m_pStrN = pszc2 + strlen(lpszSub2); // lpszSub2다음의 psz 위치
	if (pszc2 == NULL) return "";
	char cTmp = *pszc2;
	*pszc2 = '\0';
	CString sTmp = pszc + strlen(lpszSub1);
	*pszc2 = cTmp;
	return sTmp;
}

char * CParseKi::szFind(CString s, LPCTSTR lpszSub1)
{
	char * sz = strstr(s,lpszSub1);
	if (sz != NULL) {
		sz += strlen(lpszSub1);
	}
	return sz;
}


CString CParseKi::sTagRm(CString& sz)
{
	//char * pR = new char[sz.GetLength() + 1];
	CString sR;
	//CString sT;
	char * pt = new char[sz.GetLength() + 1];
	char *p1, *p2;
	char *psz = sz.GetBuffer(sz.GetLength() + 1) ;
	//*pR  = '\0';
	
	while (TRUE) {
		p1= strstr(psz,">");
		if (p1 != NULL) {
			*pt = '\0';
			*p1 = '\0';
			strcat(pt, psz);
			p2 = strstr(pt, "<");
			if (p2 != NULL)
				*p2 = '\0';
			else
				*pt = '\0';
			sR += pt;
			psz =  p1 + 1;
		}
		else {
			sR += psz;
			break;
		}
	}
	sz.ReleaseBuffer();
	delete [] pt;
	CString s = sR;
	s = sStrRm(s, "&nbsp;");
	s.TrimLeft(); s.TrimRight();
	return s;
}

CString CParseKi::sStrRm(CString& sz, LPCTSTR lpszSub1)
{
	//char * pR = new char[sz.GetLength() + 1];
	CString sR = "";
//	CString sT = "";
	//char * pt = new char[sz.GetLength() + 1];
	char *p1;
	char *psz = sz.GetBuffer(sz.GetLength() + 1) ;
	
	do  {
		p1= strstr(psz, lpszSub1);
		if (p1 != NULL) {
			*p1 = '\0';
			sR += psz;
			psz =  p1 + strlen(lpszSub1);
		}
		else {
			sR += psz;
		}

	} while (p1 != NULL);
	sz.ReleaseBuffer();
	CString s = sR;
	s.TrimLeft(); s.TrimRight();
	return s;
}

/*
CString CParseKi::sRight(CString &s, CString sSub1)
{//만들기 힘들어서 만들다 맘 
	int iLen = sSub1.GetLength();
	char c = sSub1[0];
	int iC = s.ReverseFind(c);
	if (iC < 1)
		return "";
	CString sTmp = s.Mid(iC,iLen);
	if (sTmp == sSub1)
		return s.Mid(iC + 1);
	return s.Mid(iC + 1);
}
*/

CString CParseKi::sPlusCR(LPCTSTR lpsz)
{
	CString s = lpsz;
	char * sz = s.GetBuffer(strlen(lpsz)+1);
	//strcpy(sz, lpsz);
	char * psz = sz;
	char *p;
	CString sTmp = "";
	do {
		p = strchr(sz, '\n');
		if (p == NULL)
			break;
		*p = '\0';
		sTmp += sz;
		sTmp += "\r\n";
		sz = p + 1;
	} while (*sz != '\0');
	s.ReleaseBuffer();
	return sTmp;
}


CString CParseKi::GetNextRow(int& pos)
{
	CString s = "";

	return s;
}


CString CParseKi::sMid(CString &s, LPCTSTR pSub1, LPCTSTR pSub2, int &pos)
{
	int iPos = s.Find(pSub1, pos);
	if (iPos == -1) {
		pos = -1;
		return "";}
	CString sTmp;
	sTmp = s.Mid(iPos + strlen(pSub1));
	int iTmp = sTmp.Find(pSub2);
	if (iTmp == -1) {
		pos = -1;
		return "";}
	pos = iPos + strlen(pSub1) + iTmp + strlen(pSub2);
	return sTmp.Left(iTmp);

}

CString CParseKi::GetTR(CString &s, int &pos)
{
	return sMid(s, "<tr ", "</tr>", pos);
}

CString CParseKi::GetTD(CString &s, int &pos)
{
	return sMid(s, "<td ", "</td>", pos);
}

CString CParseKi::sMidIn(CString &s, LPCTSTR pSub1, LPCTSTR pSub2, LPCTSTR pInStr, int &pos)
{
	int iPos = s.Find(pInStr, pos);
	if (iPos == -1)
		return "";
	CString sTmp;
	sTmp = s.Mid(pos, iPos - pos);
	do {
		iPos = sTmp.ReverseFind(pSub1[0]);
		if (iPos == -1)
			return "";
		int iTmp = sTmp.Find(pSub1,iPos);
		if (iTmp == -1)
			sTmp = sTmp.Left(iPos);
		else 
			break;
	} while(1);

	if (iPos == -1)
		return "";
	int iPos2 = pos + iPos;
	sTmp = sMid(s,pSub1,pSub2,iPos2);
	if (sTmp == "")
		return "";
	pos = iPos2;
	return sTmp;
}

CString CParseKi::sMidInTag(CString &s, LPCTSTR pTagStr, LPCTSTR pInStr, int &pos)
{
	int iPos = s.Find(pInStr, pos);
	if (iPos == -1)
		return "";
	CString sTmp;
	sTmp = s.Mid(pos, iPos - pos);
	CString sSub1, sSub2;
	sSub1.Format("<%s",pTagStr);
	do {
		iPos = sTmp.ReverseFind(sSub1[0]);
		if (iPos == -1)
			return "";
		if (sTmp.Find(sSub1,iPos) == -1)
			sTmp = sTmp.Left(iPos);
		else 
			break;
	} while(1);
	if (iPos == -1)
		return "";
	iPos = sTmp.Find(">",iPos);
	if (iPos == -1)
		return "";
	sSub2.Format("</%s>", pTagStr);
	int iPos2 = s.Find(sSub2, pos + iPos);
	sTmp = s.Mid(pos + iPos + 1,iPos2 - iPos - pos - 1); //1은 ">"의 글자 크기임
	
	pos = iPos2 + sSub2.GetLength();
	return sTmp;
}

CString CParseKi::sRead1Line(CString &s, int &pos, int iTrim)
{
	const char RETURN[] = "\n";
	int iPos = s.Find(RETURN, pos);
	if (iPos == -1) {
		CString sTmp = s.Mid(pos);
		pos = -1;
		return sTmp;
	}
	CString sTmp = s.Mid(pos, iPos - pos);
	pos = iPos + strlen(RETURN);

	// iTrim 이 0이면 Trim안함
	// iTrim 이 1이면 TrimLeft
	// iTrim 이 2이면 TrimRight
	// iTrim 이 3이면 TrimLeft + TrimRight

	switch (iTrim) {
	case 0 :
		break;
	case 1 :
		sTmp.TrimLeft();
		break;
	case 2:
		sTmp.TrimRight();
		break;
	case 3:
		sTmp.TrimLeft();
		sTmp.TrimRight();
	}

	return sTmp;
}

CString CParseKi::sLeft(CString &s, LPCTSTR pSub1, int &pos)
{
	int iPos = s.Find(pSub1, pos);	//pos 부터 문자열의 위치 찾음..
	if (iPos == -1) return "";		//존재하지 않음..
	CString sTmp = s.Mid(pos,iPos - pos);	//pos부터 찾는 문자열까지의 문자열
	pos = iPos + strlen(pSub1);				//pos는 증가됨..
	return sTmp;
}
CString CParseKi::sLeft(CString &s, char c, int &pos)
{
	int iPos = s.Find(c, pos) ;
	if (iPos == -1) return "" ;
	CString sTmp = s.Mid(pos, iPos - pos) ;
	pos = iPos + 1 ;
	return sTmp ;
}
CString CParseKi::sLeft(CString s, LPCTSTR lpszSub1)
{
	int iC = s.Find(lpszSub1);
	if (iC == -1)
		return s;
	return s.Left(iC);
}

//c문자열을 못찾으면 아무것도 리턴하지 않는다.
CString CParseKi::sRight(CString s, char c)
{
	int iC = s.ReverseFind(c);
	if (iC < 1)
		return "";
	return s.Mid(iC + 1);
}

//c문자열을 못찾으면 원문을 그대로 리턴한다.
CString CParseKi::sRightBack(CString s, char c)
{
	int iC = s.ReverseFind(c);
	if (iC < 1) //c문자가 발견되지 않으면 
		return s;
	return s.Left(iC);
}

// s문자열 중 pSub1이 나오는 다음의 위치를 pos로 넘긴다.
// pSub1을 찾지 못하면 -1을 넘긴다.
int CParseKi::SkipStr(CString &s, LPCTSTR pSub1, int &pos)
{
	int iPos = s.Find(pSub1, pos);
	if (iPos == -1)
		return -1;
	pos = iPos + strlen(pSub1);
	return iPos;
}

BOOL CParseKi::WriteKeyString(CStdioFile &f, LPCTSTR pKey, LPCTSTR pValue)
{
	CString s;
	s.Format("#%s = %s\n", pKey, pValue);
	f.WriteString(s);
	return TRUE;
}

CString CParseKi::GetKeyString(CString &sLine, LPCTSTR szKey)
{
	CString sTmp;
	int pos = 0;
	if (sLine.Left(1) != "#") return "";
	sTmp.Format("#%s",szKey);
	if (SkipStr(sLine, sTmp, pos) == -1)
		return "";
	sTmp = sMid(sLine, "=", "#", pos);
	if (sTmp == "") // 뒷문자열인 #가 없거나 기타 이유
	{
		if(SkipStr(sLine, "=", pos) == -1) // 앞문자열인 =도 없으면
			return "";
		sTmp = sLine.Mid(pos); // = 이후 부터 마지막까지 읽어 온다.
	}
	sTmp.TrimLeft();
	sTmp.TrimRight();
	return sTmp;
}

CString CParseKi::GetKeyString(CString &sLine, CString &sKey)
{
	CString sTmp;
	int pos = 0;
	if (sLine.Left(1) != "#") return "";
/*
	sTmp = sMid(sLine,"#", "=", pos);
	sTmp.TrimRight();
	sKey = sTmp;
	sTmp = sLine.Mid(pos); */
	sTmp = "#" + sKey;
	if (SkipStr(sLine, sTmp, pos) == -1)
		return "";
	sTmp = sMid(sLine, "=", "#", pos);
	sTmp.TrimLeft();
	return sTmp;
}

void CParseKi::WriteSection(CStdioFile &f, LPCTSTR pSection)
{
	f.SeekToEnd();
	CString s;
	s.Format("#[%s]\n",pSection);
	f.WriteString(s);
}

CString CParseKi::GetSectionStrings(CStdioFile &f, LPCTSTR pSub1)
{
	CString sLine, s ="";
	while (f.ReadString(sLine))
	{
		if (sLine.Left(2) == "#[")
		{
			if (sLine.Mid(1) == pSub1)
			{
			while (f.ReadString(sLine) && sLine.Left(2) != "#[")
				{
					sLine.TrimLeft();
					if (sLine != "")
						s += (sLine + '\n');
				}
			break;
			}
		}
	}
	return s;
}

BOOL CParseKi::GetSectionStrings(CStdioFile &f, LPCTSTR pSub1, CArray <ST, ST> &ar)
{
	CString sLine, sTmp;
	int i = 0;
	BOOL b1 = FALSE;
	CString sSub1; sSub1.Format("[%s]", pSub1);
	while (f.ReadString(sLine))
	{
		if (sLine.Left(2) == "#[")
		{
			if (sLine.Mid(1) == sSub1)
			{
				while (f.ReadString(sLine) && sLine.Left(2) != "#[")
				{
					sLine.TrimLeft();
					if (sLine.Left(1) == "#")
					{
						if (GetKeyString(sLine, sTmp) == "") break;
						ar.SetSize(i + 1);
						ar[i].sValue = GetKeyString(sLine, ar[i].sKey);
						i++;
						b1 = TRUE; //한개라도 키를 읽어 올경우 TRUE를 리턴하도록
					}
				}
			break;
			}
		}
	}
	return b1;
}
CString CParseKi::sRevRead1Line(LPTSTR pStr, char * p)
{
	const char RETURN = '\n';
	char *pBackup = p;
	char cBackup = *p;
	*p = '\0';
	while (*(p--) != RETURN)
		if (p == pStr) break;
	CString sTmp = p;
	*pBackup = cBackup;
	int i = strlen(pStr);
	return sTmp;

}

int CParseKi::GetAryValue(CString s, CStringArray &ar)
{
	int pos = 0;
	int prevpos = pos;
	int ic = 0;
	CString sTmp, sTmp2;
	const char pSub1[] = ",";
	sTmp = sLeft(s, pSub1, pos);
	//ar.SetSize(1);
	while (prevpos !=pos) {
		ar.Add(sTmp);
		//ar[ic++] = sTmp;
		//ar.SetSize(ic+1);
		prevpos = pos;
		sTmp = sLeft(s, pSub1, pos);
		sTmp.TrimLeft();
		if (prevpos == pos)
		{	sTmp2 = s.Mid(pos); sTmp2.TrimLeft();
			ar[ic] = sTmp2;
		}
	}
	return ar.GetSize();
}

//문자열에서 일정한 문자를 제외한 문자열 배열을 리턴..
//						전체 문자열,	구분할 문자,   값이 들어갈 스트링 배열..
void CParseKi::GetAryValue(CString &s, LPCTSTR pSub1, CStringArray &ar)
{
	int pos = 0;
	int prevpos = pos;
	int ic = 0;
	CString sTmp, sTmp2;
	
	sTmp = sLeft(s, pSub1, pos);	//문자열찾고 pos증가..
	ar.SetSize(1);
	while (prevpos !=pos) {
		ar[ic++] = sTmp;
		ar.SetSize(ic+1);
		prevpos = pos;
		sTmp = sLeft(s, pSub1, pos);
		sTmp.TrimLeft();			//스페이스 탭 뉴라인등 삭제
		if (prevpos == pos)			//마지막이어서 구분문자 못찾는다면..
		{	
			sTmp2 = s.Mid(pos); 
			sTmp2.TrimLeft();
			ar[ic] = sTmp2;
		}
	}
}

void CParseKi::SetFieldfromAry(CArray <CString, CString> &ar)
{
	int iSize = ar.GetSize();
	for (register i = 0; i < iSize; i++)
		m_aryField[i].sValue = ar[i];
}

void CParseKi::SortAryFieldByiHno()
{
	int iSize = m_aryField.GetSize();
	CArray <FIELD2, FIELD2> aryField;
	aryField.SetSize(iSize);
	for (register i = 0; i < iSize; i++) {
		for (register i2 = 0; i2 < iSize; i2++) {
			if (m_aryField[i2].iHno == i)
			{
				aryField[i].iFno = m_aryField[i2].iFno;
				aryField[i].iDno = m_aryField[i2].iDno;
				aryField[i].iHno = m_aryField[i2].iHno;
				aryField[i].sCap = m_aryField[i2].sCap;
				aryField[i].sValue = m_aryField[i2].sValue;
				break;
			}
		}
	}
	CString s;
	for (i = 0; i < iSize; i++) {
		m_aryField[i].iFno =aryField[i].iFno;
		m_aryField[i].iDno = aryField[i].iDno;
		m_aryField[i].iHno = aryField[i].iHno; 
		m_aryField[i].sCap = aryField[i].sCap;
		m_aryField[i].sValue = aryField[i].sValue;
		s.Format("i=%d, cap=%s, F%d, D%d, H%d, val%s\n", i, m_aryField[i].sCap, m_aryField[i].iFno, m_aryField[i].iDno,m_aryField[i].iHno, m_aryField[i].sValue);
		TRACE0(s);
	}
}

void CParseKi::SortAryFieldByiFno()
{
	int iSize = m_aryField.GetSize();
	CArray <FIELD2, FIELD2> aryField;
	aryField.SetSize(iSize);
	for (int i = 0; i < iSize; i++) {
		for (int i2 = 0; i2 < iSize; i2++) {
			if (m_aryField[i2].iFno == i)
			{
				aryField[i].iFno = m_aryField[i2].iFno;
				aryField[i].iDno = m_aryField[i2].iDno;
				aryField[i].iHno = m_aryField[i2].iHno;
				aryField[i].sCap = m_aryField[i2].sCap;
				aryField[i].sValue = m_aryField[i2].sValue;
				break;
			}
		}
	}
	CString s;
	for (i = 0; i < iSize; i++) {
		m_aryField[i].iFno =aryField[i].iFno;
		m_aryField[i].iDno = aryField[i].iDno;
		m_aryField[i].iHno = aryField[i].iHno; 
		m_aryField[i].sCap = aryField[i].sCap;
		m_aryField[i].sValue = aryField[i].sValue;
		s.Format("i=%d, cap=%s, F%d, D%d, H%d, val%s\n", i, m_aryField[i].sCap, m_aryField[i].iFno, m_aryField[i].iDno,m_aryField[i].iHno, m_aryField[i].sValue);
		TRACE0(s);
	}
}

void CParseKi::WriteAryValue(CStdioFile &f, CArray  <CString, CString> &ar)
{
	int iSize = ar.GetSize();
	if (iSize != 6)
	{
		TRACE0("Error in WriteAryValue()");
		return;
	}
	CString s;
	s.Format("%s\t%s\t%s\t%s\t%s\t%s",ar[0],ar[1],ar[2],ar[3],ar[4],ar[5]);
	f.WriteString(s);

}

CString CParseKi::GetFileName(CString spath, char SEP)
{
	CString val = sRight(spath, SEP);
	if (val == "")
		val = spath;
	return val;
}
//전체 경로를 포함한 파일이름 중에서 순수한 경로만을 뽑아내어 준다.
//리턴된 패스의 문자열 끝에 \ 가 포함된다.
CString CParseKi::GetFilePath(CString sPath, char SEP)
{
	CString sTargetPath;
	int pos = sPath.ReverseFind(SEP);
	return sPath.Left(pos); //첨부터 '\'가 발견된 위치 앞까지 자른다.
}

CString CParseKi::GetFileTitle(CString &sPath)
{
	char sz1[256];
	::GetFileTitle(sPath, sz1, 255);
	return sz1;
}

CString CParseKi::sPathAddBackSlash(CString sPath)
{
	if (sPath != "" && sPath.Right(1) != "\\") // Path 뒤에 역슬레시를 붙인다.
		sPath += "\\";
	return sPath;
}


string CParseKi::AddDirSeparator(LPCSTR szPath, char SEP)
{
	string val;
	if (szPath[0] != '\0' && szPath[strlen(szPath)-1] != SEP) // Path 뒤에 역슬레시를 붙인다.
	{
		val = szPath;
		val += SEP;
		return val;
	}
	return szPath;
}

/*
string CParseKi::AddDirSeparator(string * psPath, char SEP)
{
	if (*psPath != "" && psPath->at(psPath->size()-1) != SEP) // Path 뒤에 역슬레시를 붙인다.
	{
		*psPath += SEP;
	}
	return psPath;
}*/