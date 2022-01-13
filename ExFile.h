// ExFile.h: interface for the CExFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXFILE_H__528431D2_CBD5_4EAF_943B_B9010C294C82__INCLUDED_)
#define AFX_EXFILE_H__528431D2_CBD5_4EAF_943B_B9010C294C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"
#include "wininet.h" 

class CExFile  
{
public:
	int m_nRemoteType;
	string m_ID;
	string m_PW;
	BOOL Close();
	BOOL Open(LPCSTR url, LPCSTR option);
	BOOL Deinit();
	BOOL Init(LPCSTR str, LPCSTR ID, LPCSTR PW);
	CExFile();
	virtual ~CExFile();
private:
	HINTERNET m_connect;
	int m_hFile;
	DWORD m_FilePos;
	DWORD m_FileTotalSize;
};

#endif // !defined(AFX_EXFILE_H__528431D2_CBD5_4EAF_943B_B9010C294C82__INCLUDED_)
