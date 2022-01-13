// SaveSpace.h: interface for the CDirNFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVESPACE_H__B063D7D1_59AB_11D4_A3C7_004F4E05B3F2__INCLUDED_)
#define AFX_SAVESPACE_H__B063D7D1_59AB_11D4_A3C7_004F4E05B3F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "inetfile.h"
BOOL FileExists(LPCTSTR FilePath, CInetFile * pInetf=NULL);

class CDirNFile  
{
public:
	static int DelTreeFiles(CString spath, BOOL bDirDelToo, CInetFile * pInetf = NULL);
	static int DelTreeFiles(CString spath, CStringArray &sAry);
	BOOL IsInExceptDelList(LPCTSTR str);
	BOOL m_bExceptDel;
	BOOL m_bDirDelToo;
	BOOL SearchDelX(LPCTSTR str);
	BOOL DelX(LPCTSTR str);
	void InitExceptDelList(CStringArray &sAry);
	void InitDelXList(CStringArray &sAry);
	CDirNFile();
	virtual ~CDirNFile();

	CStringArray m_sAryDelX, m_sAryExceptDel;
private:
	int m_RemoteType; //
};

#endif // !defined(AFX_SAVESPACE_H__B063D7D1_59AB_11D4_A3C7_004F4E05B3F2__INCLUDED_)
