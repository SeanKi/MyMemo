// TimeOutSocket1.cpp : implementation file
//

#include "stdafx.h"
#include "TimeOutSocket1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeOutSocket

CTimeOutSocket::CTimeOutSocket()
{
	m_Kill = TRUE; 
	m_pReceiver = NULL;
}

CTimeOutSocket::~CTimeOutSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CTimeOutSocket, CSocket)
	//{{AFX_MSG_MAP(CTimeOutSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CTimeOutSocket member functions

BOOL CTimeOutSocket::OnMessagePending() 
{
	// TODO: Add your specialized code here and/or call the base class
	
  MSG msg;
  if(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
  {
    if (msg.wParam == (UINT) m_nTimerID)
    {
      // Remove the message and call CancelBlockingCall
      ::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
      CancelBlockingCall();
      return FALSE;  // No need for idle time processing
    };
  };

  return CSocket::OnMessagePending();

}

BOOL CTimeOutSocket::SetTimeOut(UINT uTimeOut) { 

  m_nTimerID = SetTimer(NULL,0,uTimeOut,NULL);
  return m_nTimerID;

} 
BOOL CTimeOutSocket::KillTimeOut() { 

  return KillTimer(NULL,m_nTimerID);

} 


void CTimeOutSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	char buff[1024];
	int nBuff = 1024;
	int nBytes;
	int nTotal=0;
	do {
		SetTimeOut(500);
		nBytes = Receive(buff, nBuff);
		KillTimeOut();
		if ( nBytes == SOCKET_ERROR ) {
			DWORD Err = GetLastError();
			break;
		}
		m_ReceivedData.SetSize(nTotal + nBytes);
		memcpy(m_ReceivedData.GetData()+nTotal, buff, nBytes);
		nTotal += nBytes;
	} while(nBytes > 0);
	int nCount = 0;
/*	if ( nBytes != SOCKET_ERROR )
	{
		m_ReceivedData.SetSize(m_ReceivedData.GetLength() + nBuff);
		memcpy(m_ReceivedData, nCount, nBytes */

	if (m_pReceiver) {
//		((CSendSMSCls*)m_pReceiver)->Sock_OnReceive(&m_ReceivedData, nErrorCode);
	}
	CSocket::OnReceive(nErrorCode);
}

/// CSocket modify - timeout module.
BOOL CTimeOutSocket::ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	if (m_pbBlocking != NULL)
	{
		WSASetLastError(WSAEINPROGRESS);
		return  FALSE;
	}

	m_nConnectError = -1;

	if (!CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen))
	{
		if (GetLastError() == WSAEWOULDBLOCK)
		{
	// Insert....
			CTime		curt, st;
			CTimeSpan	span(0, 0, 0, m_nTimeOut);

			st = CTime().GetCurrentTime();
	//.......
			while (PumpMessages(FD_CONNECT))
			{
				if (m_nConnectError != -1)
				{
					WSASetLastError(m_nConnectError);
					return (m_nConnectError == 0);
				}
	// Insert....
				curt = CTime().GetCurrentTime();
				if(curt > (st+span))
					return FALSE;
	//..............
			}
		}
		return FALSE;
	}
	m_Kill = FALSE;
	return TRUE;
}

void CTimeOutSocket::OnClose(int nErrorCode)
{
	this->Close();
	m_Kill = TRUE;
}

void CTimeOutSocket::Receiver_Init(void *pVoid)
{
	m_pReceiver = pVoid;
}
