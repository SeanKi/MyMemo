// ExFile.cpp: implementation of the CExFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mymemo.h"
#include "ExFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static HINTERNET g_session=NULL;
static int g_sessionCount=0;
CExFile::CExFile()
{
	if (g_session == NULL) {
		g_session = ::InternetOpen("Assa_Kish",
		INTERNET_OPEN_TYPE_PRECONFIG , NULL, NULL, 0);
	}
	g_sessionCount++;
}

CExFile::~CExFile()
{
	g_sessionCount--;
	if (g_sessionCount == 0) {
		if (g_session) {
			InternetCloseHandle(g_session);
		}
	}
}

BOOL CExFile::Init(LPCSTR url, LPCSTR ID, LPCSTR PW)
{
	// str = url
	HINTERNET hInternet;
	HINTERNET hRemoteFile;
	HANDLE hRFile;
	m_FilePos = 0;
	m_ID = ID;
	m_PW = PW;
	int m_nRemoteType;
	m_nRemoteType = -1; // local
	if (strnicmp(url, "http:", 5) == 0) {
		m_nRemoteType = REMOTE_HTTP; // http:
	} else if (strnicmp(url, "ftp:", 4) == 0) {
		m_nRemoteType = REMOTE_FTP;
	}

	if (m_nRemoteType == -1) {
		return TRUE;
	} 

	if (m_nRemoteType == REMOTE_HTTP){
	} else if (m_nRemoteType == REMOTE_FTP){
		// Open web file for check
		BOOL bRet;
		URL_COMPONENTS urlComponents;
		char szServer[MAX_PATH] = {0,};
		char szURL[MAX_PATH] = {0,};
		char strVerb[10]={0,};
		DWORD dwOutSize;
		DWORD dwResult;
		string strHeaders;
		memset(&urlComponents, 0, sizeof(URL_COMPONENTS));
		urlComponents.dwStructSize = sizeof(URL_COMPONENTS);
		urlComponents.lpszHostName = szServer;
		urlComponents.dwHostNameLength = MAX_PATH;
		urlComponents.lpszUrlPath = szURL;
		urlComponents.dwUrlPathLength = MAX_PATH;

		bRet = InternetCrackUrl(url, strlen(url), ICU_DECODE, &urlComponents);
		m_connect = ::InternetConnect(hInternet,
											urlComponents.lpszHostName,
											urlComponents.nPort,
											ID,
											PW,
											INTERNET_SERVICE_FTP,
											0,
											0) ;

	}
	return TRUE;
	lvErr:
	if(hInternet){
		InternetCloseHandle(hInternet);
	} 
	return FALSE;

}

BOOL CExFile::Deinit()
{
	return TRUE;
}

BOOL CExFile::Open(LPCSTR url, LPCSTR option)
{
	if (m_nRemoteType == REMOTE_FTP) {
	}
	return TRUE;
}

BOOL CExFile::Close()
{
	return TRUE;
}
