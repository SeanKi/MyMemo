/**************************************************************************
File name : InetFile.cpp

m_lError - Error Code :
-1 : Internet Session Error
-2 : Local File Open Error
-3 : Internet Open Url Error - Connection Error, SendRequest Error include
-4 : Local File Read/Write Error
-5 : Internet File Read/Write Error
-6 : User abort
-7 : Internet File Set to End.
-8 : Internet Open Url Error - StatusCode not success, suberrorcode return status code
**************************************************************************/

// InetFile.cpp: implementation of the CInetFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "InetTest.h"
#include "InetFile.h"
#include "TimeOutSocket1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static HINTERNET g_session=NULL; //internetsession�� �ѹ��� �����ϵ���
static int g_sessionCount=0;

BOOL Sock_Connect(LPCSTR szIP, int iPort, LONG *pErr = NULL);

CInetFile::CInetFile()
{
	m_dwService = 0xffffffff; // Service type
	ErrCodeInit();
	if (g_session == NULL) {
		g_session = ::InternetOpen("Assa_Kish",
		INTERNET_OPEN_TYPE_PRECONFIG , NULL, NULL, 0);
	}
	InterlockedIncrement((long*)&g_sessionCount);
    if(!g_session){
        m_lError = -1;
		m_lSubError = GetLastError();
        //return FALSE;
    }
	m_hConnect = NULL;
	m_hFile = NULL;
	m_bStop = FALSE;
}

CInetFile::~CInetFile()
{
	InterlockedDecrement((long*)&g_sessionCount);
	if (g_sessionCount == 0) {
		if (g_session) {
			InternetCloseHandle(g_session);
			g_session = NULL;
		}
	}
}


BOOL CInetFile::Connect(LPCSTR url, LPCSTR ID, LPCSTR PW, BOOL bOnlyInit)
{
// url������ http://serverhost:8000/ �� ���� ���� �ȴ�. ��Ʈ��ȣ�� ��������
	// str = url
	ErrCodeInit();
	m_FilePos = 0;
	m_URL = url;
	m_ID = ID;
	m_PW = PW;
	if (strnicmp(url, "http:", 5) == 0) {
		m_dwService = INTERNET_SERVICE_HTTP;
	} else if (strnicmp(url, "ftp:", 4) == 0) {
		m_dwService = INTERNET_SERVICE_FTP;
	} else {
		return FALSE; // ���� �ΰ����� Internet Service�� ����
	}
	if (bOnlyInit)
		return TRUE; //�ʱ�ȭ�� ó���ϰ� �����Ѵ�.
	if (TRUE){ //if������ ó���ص������� �ٸ� ���������� �߰��� �� �����Ƿ� �ϴ� if������ ó��
		// Open web file for check
		BOOL bRet;
		URL_COMPONENTS urlComponents;
		char szServer[MAX_PATH] = {0,};
		char szURL[MAX_PATH] = {0,};
		char strVerb[10]={0,};
		string strHeaders;
		memset(&urlComponents, 0, sizeof(URL_COMPONENTS));
		urlComponents.dwStructSize = sizeof(URL_COMPONENTS);
		urlComponents.lpszHostName = szServer;
		urlComponents.dwHostNameLength = MAX_PATH;
		urlComponents.lpszUrlPath = szURL;
		urlComponents.dwUrlPathLength = MAX_PATH;

		bRet = InternetCrackUrl(url, strlen(url), ICU_DECODE, &urlComponents);
		// Portüũ�ϴ� �Լ��� �߰� 2003.11.2
		bRet = Sock_Connect(urlComponents.lpszHostName, urlComponents.nPort);
		if (bRet) {
			m_hConnect = ::InternetConnect(g_session,
												urlComponents.lpszHostName,
												urlComponents.nPort,
												ID,
												PW,
												m_dwService,
												INTERNET_FLAG_PASSIVE , // 0 -> INTERNET_FLAG_PASSIVE, FtpFindFirstFile ���� ȯ�濡 ���� 500 Illegal PORT command. ���� ������ �����ش�.
												0) ;
		}
		if (m_hConnect == NULL) {
			m_lError = -3;
			m_lSubError = GetLastError();
			char Mes[1024];
			DWORD dwMesLength = 1024;
			DWORD dwError;
			//dwError =  GetLastError(); FTP��� ���н� ������ ���� ERROR_INTERNET_EXTENDED_ERROR �̰� �̶��� ���� InternetGetLastResponseInfo�� �̿��� ������ ����ؾ��Ѵ�.
			InternetGetLastResponseInfo(
				&dwError,
				Mes,
				&dwMesLength
			);
			m_sErrMsg = Mes;
			goto lvErr;
		}

	}
	return TRUE;
	lvErr:
	return FALSE;

}

BOOL CInetFile::Disconnect()
{
	if(m_hConnect){
		InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;

	} 
	return TRUE;
}

BOOL CInetFile::OpenFile(LPCSTR path, LPCSTR option)
{
	//url is relative filepath
	int nOption=GENERIC_READ; // default is READ
	if (strstr(option, "r") != NULL)
		nOption = GENERIC_READ;
	if (strstr(option, "w") != NULL)
		nOption = GENERIC_WRITE;

	if (m_dwService = INTERNET_SERVICE_FTP) {
		while(1) {
			m_hFile = FtpOpenFile(m_hConnect, path, nOption, FTP_TRANSFER_TYPE_BINARY, 0);
			if (m_hFile == NULL) {
				if (GetLastError() == ERROR_INTERNET_CONNECTION_ABORTED) {
					if (ReConnect())
						continue;
				}
				m_lError = -3;
				m_lSubError = GetLastError();
/*				DWORD dwError;
				char buffer[1024];
				DWORD bufsize=1024;
				InternetGetLastResponseInfo(
					&dwError,
					buffer,
					&bufsize
				);
				string dir;
				GetCurrentDirectory(&dir);*/
			}
			break;
		}
		return (m_hFile != NULL);
	}
	return TRUE;
}

BOOL CInetFile::CloseFile()
{
	if(m_hFile){
		InternetCloseHandle(m_hFile);
		m_hFile = NULL;

	} 
	return TRUE;
}

BOOL CInetFile::SaveFile(LPCTSTR sfilename, DWORD &gettingsize, DWORD dwSizeToDL)
{
	 ErrCodeInit();
	 BOOL bRet;
	DWORD nRead, nRead2;
	 HANDLE hRFile;
	hRFile = CreateFile(sfilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hRFile == INVALID_HANDLE_VALUE) {
		m_lError = -2; //���� ������ ���� ����
		m_lSubError = GetLastError();
		return FALSE;
	}
	char szBuff[4*1024]="";
	gettingsize = 0;//initialize
bRet = ::InternetReadFile(m_hFile, (void
	*)szBuff, sizeof szBuff, & nRead);
bRet = TRUE;
   while (nRead > 0)
   {
		gettingsize += nRead;
		if (dwSizeToDL > 0 && gettingsize > dwSizeToDL)
			break;
		
		bRet = ::WriteFile(hRFile, szBuff, nRead, &nRead2, NULL);
		if (!bRet) {
			m_lError = -5; //local file write error
			m_lSubError = GetLastError();
			break;
		}
		bRet = ::InternetReadFile(m_hFile, (void
			*)szBuff, sizeof szBuff, & nRead);
		if (!bRet) {
			m_lError = -5;
			m_lSubError = GetLastError();
			break;
		}
		if (m_bStop) {
			m_lError = -6; //user abort
			m_bStop = FALSE;
		   break;
		}
   };
		
	CloseHandle(hRFile);
	return TRUE;
}

void CInetFile::ErrCodeInit()
{
	 m_lError = 0;
	 m_lSubError = 0;
	 m_sErrMsg = "";
}


HINTERNET CInetFile::GetFileHandle()
{
	return m_hFile;
}

BOOL CInetFile::RenameFile(LPCSTR szExisting, LPCSTR szNew)
{
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b =  FtpRenameFile(m_hConnect, szExisting, szNew);
		if (!b) {
				m_lSubError = GetLastError();
				DWORD dwError;
				char buffer[1024];
				DWORD bufsize=1024;
				InternetGetLastResponseInfo(
					&dwError,
					buffer,
					&bufsize
				);
		}
	}
	return b;

}

BOOL CInetFile::DeleteFile(LPCSTR szFilepath)
{
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b =  FtpDeleteFile(m_hConnect, szFilepath);
	}
	return b;
}

BOOL CInetFile::CreateDirectory(LPCSTR szFilepath)
{
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b =  FtpCreateDirectory(m_hConnect, szFilepath);
	}
	return b;
}

BOOL CInetFile::PutFile(LPCSTR szLocal, LPCSTR szRemote)
{
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b = FtpPutFile(m_hConnect, szLocal, szRemote, INTERNET_FLAG_TRANSFER_BINARY, 0);
	}
	return b;
}

BOOL CInetFile::GetFile(LPCSTR szRemote, LPCSTR szLocal)
{
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b = FtpGetFile(m_hConnect, szRemote, szLocal, FALSE/*������ ����*/, FILE_ATTRIBUTE_ARCHIVE, FTP_TRANSFER_TYPE_BINARY,0);
	}
	return b;

}

BOOL CInetFile::GetCurrentDirectory(string * psDir)
{
	char szTemp[4096];
	DWORD dwsize = sizeof szTemp;
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b = FtpGetCurrentDirectory(m_hConnect, szTemp, &dwsize);
		if (b) {
			*psDir = szTemp;
		}
	}
	return b;
}

BOOL CInetFile::SetCurrentDirectory(LPCSTR szDir)
{
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b = FtpSetCurrentDirectory(m_hConnect, szDir);
	}
	return b;
}

HINTERNET CInetFile::GetConnectHandle()
{
	return m_hConnect;
}

BOOL CInetFile::ReadFile(PBYTE pBuff, DWORD dwToRead, PDWORD pdwRead)
{
	return ::InternetReadFile(m_hFile, (void
		*)pBuff, dwToRead, pdwRead);
}

DWORD CInetFile::GetFileSize(PDWORD dwSizeHigh)
{
	return 0;
}

BOOL CInetFile::IsConnect()
{
	if(m_hConnect && m_dwService == INTERNET_SERVICE_FTP) {
		string sDir;
		return GetCurrentDirectory(&sDir);
	}
	return FALSE;
}

BOOL Sock_Connect(LPCSTR szIP, int iPort, LONG *pErr)
{
	BOOL bConnect= FALSE;
	DWORD Err;
	CTimeOutSocket sock;
	do {
		Err = 0;
		BOOL ret = sock.Connect(szIP, iPort);
		if (!ret) {
			Err = GetLastError();
			if (Err == WSAENOTSOCK) {
				if (sock.Create())
					continue;
			}
		} else 
			bConnect = TRUE;
		break;
	} while (true);
	if (pErr)
		*pErr = Err;
	return bConnect;
}



BOOL CInetFile::ReConnect()
{
	if (m_hConnect) //Connect handle�� �����Ǿ��� ���� ReConnect����
		if (IsConnect()) //����Ǿ� �ִٸ� �ٽ� ������ �ʿ�� ����.
			return TRUE;
	
	return Connect(m_URL.c_str(), m_ID.c_str(), m_PW.c_str());
}

BOOL CInetFile::WriteFile(PBYTE pBuff, DWORD dwToRead, PDWORD pdwRead)
{
	return ::InternetWriteFile(m_hFile, (void
		*)pBuff, dwToRead, pdwRead);
}

BOOL CInetFile::RemoveDirectory(LPCSTR szpath)
{
	BOOL b = FALSE;
	if (m_dwService = INTERNET_SERVICE_FTP) {
		b =  FtpRemoveDirectory(m_hConnect, szpath);
	}
	return b;

}
