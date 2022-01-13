// InetFile.h: interface for the CInetFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INETFILE_H__E976A4DE_6D28_467C_9BC3_E5DBA51C05EA__INCLUDED_)
#define AFX_INETFILE_H__E976A4DE_6D28_467C_9BC3_E5DBA51C05EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <string>
#include <vector>

//#include "StrKi.h"

using namespace std ;

#include "wininet.h" 
class CInetFile  
{
public:
	BOOL RemoveDirectory(LPCSTR szpath);
	BOOL WriteFile(PBYTE pBuff, DWORD dwToRead, PDWORD pdwRead);
	BOOL ReConnect();
	BOOL IsConnect();
	DWORD GetFileSize(PDWORD dwSizeHigh);
	BOOL ReadFile(PBYTE pBuff, DWORD dwToRead, PDWORD pdwRead);
	HINTERNET GetConnectHandle();
	BOOL SetCurrentDirectory(LPCSTR szDir);
	BOOL GetCurrentDirectory(string * psDir);
	BOOL GetFile(LPCSTR szRemote, LPCSTR szLocal);
	BOOL PutFile(LPCSTR szLocal, LPCSTR szRemote);
	BOOL CreateDirectory(LPCSTR szFilepath);
	BOOL DeleteFile(LPCSTR szFilepath);
	BOOL RenameFile(LPCSTR szExisting, LPCSTR szNew);
	HINTERNET GetFileHandle();
	BOOL SaveFile(LPCTSTR sfilename, DWORD &gettingsize, DWORD dwSizeToDL=0);
	CInetFile();
	virtual ~CInetFile();
	BOOL CloseFile();
	BOOL OpenFile(LPCSTR url, LPCSTR option);
	BOOL Disconnect();
	BOOL Connect(LPCSTR str, LPCSTR ID, LPCSTR PW, BOOL bOnlyInit = FALSE);
	long m_lError;
	long m_lSubError;
	string m_sErrMsg;
	string m_initDir; //√ ±‚ Directory
	DWORD m_dwService;
	string m_URL;
private:
	string m_ID;
	string m_PW;
	void ErrCodeInit();
	BOOL m_bStop;
	HINTERNET m_hConnect;
	HINTERNET m_hFile;
	DWORD m_FilePos;
	DWORD m_FileTotalSize;
};

#endif // !defined(AFX_INETFILE_H__E976A4DE_6D28_467C_9BC3_E5DBA51C05EA__INCLUDED_)
