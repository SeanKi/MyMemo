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
1.0.0.2g �Ϲ� ����
*/
// Release Note
/*
2001.07.25
sPathAddBackSlash�Լ��� static���� ��ȯ
  GetFileName�Լ� �߰� static, ��ü �н����� �����̸��� �̾Ƴ���.
2000.11.8
GetFilePath�Լ��� sRight�Լ��� static���� ��ȯ - �����ε� ��� ��ȯ�� �� ����
GetFilePath�Լ��� ������ ���� File�� title�� ã�� ��Ŀ��� ������ \�� �������� ã����
����. (������ �� �׷��� ������ -__-)

2000.06.21
::GetAryValue�� �Ķ������ CArray <CString, CString> &ar�� CStringArray�� ����
2000.04.20
::sPathAddBackSlash(CString sPath) -> ���� �߰�, ���丮 �� �ڿ� "\"�� ������ �߰��Ѵ�.

2000.03.23
::sRight(CString &s, char c) -> CString s�� ����(s�� ���� ����� ���� �ѱ� �ʿ䰡 �����Ƿ�)
2000.03.22
GetFilePath ���� ���� 
- Path �߰��� ���� Ÿ��Ʋ�� ���� ���ڿ��� ���� �� ������ ��ҹ��� ���� ���ϴ� �� �ذ�
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
	m_pStrC = pszc; // lpszSub1�� ������ psz�� ��ġ
	if (pszc == NULL) return "";
	char * pszc2 = strstr(pszc,lpszSub2);
	m_pStrN = pszc2 + strlen(lpszSub2); // lpszSub2������ psz ��ġ
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
{//����� ���� ����� �� 
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
	sTmp = s.Mid(pos + iPos + 1,iPos2 - iPos - pos - 1); //1�� ">"�� ���� ũ����
	
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

	// iTrim �� 0�̸� Trim����
	// iTrim �� 1�̸� TrimLeft
	// iTrim �� 2�̸� TrimRight
	// iTrim �� 3�̸� TrimLeft + TrimRight

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
	int iPos = s.Find(pSub1, pos);	//pos ���� ���ڿ��� ��ġ ã��..
	if (iPos == -1) return "";		//�������� ����..
	CString sTmp = s.Mid(pos,iPos - pos);	//pos���� ã�� ���ڿ������� ���ڿ�
	pos = iPos + strlen(pSub1);				//pos�� ������..
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

//c���ڿ��� ��ã���� �ƹ��͵� �������� �ʴ´�.
CString CParseKi::sRight(CString s, char c)
{
	int iC = s.ReverseFind(c);
	if (iC < 1)
		return "";
	return s.Mid(iC + 1);
}

//c���ڿ��� ��ã���� ������ �״�� �����Ѵ�.
CString CParseKi::sRightBack(CString s, char c)
{
	int iC = s.ReverseFind(c);
	if (iC < 1) //c���ڰ� �߰ߵ��� ������ 
		return s;
	return s.Left(iC);
}

// s���ڿ� �� pSub1�� ������ ������ ��ġ�� pos�� �ѱ��.
// pSub1�� ã�� ���ϸ� -1�� �ѱ��.
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
	if (sTmp == "") // �޹��ڿ��� #�� ���ų� ��Ÿ ����
	{
		if(SkipStr(sLine, "=", pos) == -1) // �չ��ڿ��� =�� ������
			return "";
		sTmp = sLine.Mid(pos); // = ���� ���� ���������� �о� �´�.
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
						b1 = TRUE; //�Ѱ��� Ű�� �о� �ð�� TRUE�� �����ϵ���
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

//���ڿ����� ������ ���ڸ� ������ ���ڿ� �迭�� ����..
//						��ü ���ڿ�,	������ ����,   ���� �� ��Ʈ�� �迭..
void CParseKi::GetAryValue(CString &s, LPCTSTR pSub1, CStringArray &ar)
{
	int pos = 0;
	int prevpos = pos;
	int ic = 0;
	CString sTmp, sTmp2;
	
	sTmp = sLeft(s, pSub1, pos);	//���ڿ�ã�� pos����..
	ar.SetSize(1);
	while (prevpos !=pos) {
		ar[ic++] = sTmp;
		ar.SetSize(ic+1);
		prevpos = pos;
		sTmp = sLeft(s, pSub1, pos);
		sTmp.TrimLeft();			//�����̽� �� �����ε� ����
		if (prevpos == pos)			//�������̾ ���й��� ��ã�´ٸ�..
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
//��ü ��θ� ������ �����̸� �߿��� ������ ��θ��� �̾Ƴ��� �ش�.
//���ϵ� �н��� ���ڿ� ���� \ �� ���Եȴ�.
CString CParseKi::GetFilePath(CString sPath, char SEP)
{
	CString sTargetPath;
	int pos = sPath.ReverseFind(SEP);
	return sPath.Left(pos); //÷���� '\'�� �߰ߵ� ��ġ �ձ��� �ڸ���.
}

CString CParseKi::GetFileTitle(CString &sPath)
{
	char sz1[256];
	::GetFileTitle(sPath, sz1, 255);
	return sz1;
}

CString CParseKi::sPathAddBackSlash(CString sPath)
{
	if (sPath != "" && sPath.Right(1) != "\\") // Path �ڿ� �������ø� ���δ�.
		sPath += "\\";
	return sPath;
}


string CParseKi::AddDirSeparator(LPCSTR szPath, char SEP)
{
	string val;
	if (szPath[0] != '\0' && szPath[strlen(szPath)-1] != SEP) // Path �ڿ� �������ø� ���δ�.
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
	if (*psPath != "" && psPath->at(psPath->size()-1) != SEP) // Path �ڿ� �������ø� ���δ�.
	{
		*psPath += SEP;
	}
	return psPath;
}*/