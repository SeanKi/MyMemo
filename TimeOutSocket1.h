#if !defined(AFX_TIMEOUTSOCKET1_H__77D3221F_6F91_41A7_AF2A_6520A8DFA1FB__INCLUDED_)
#define AFX_TIMEOUTSOCKET1_H__77D3221F_6F91_41A7_AF2A_6520A8DFA1FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeOutSocket1.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CTimeOutSocket command target

class CTimeOutSocket : public CSocket
{
// Attributes
public:
	BOOL SetTimeOut(UINT uTimeOut);
	BOOL KillTimeOut();

// Operations
public:
	CTimeOutSocket();
	virtual ~CTimeOutSocket();

// Overrides
public:
	void Receiver_Init(void * pVoid);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeOutSocket)
	public:
	virtual BOOL OnMessagePending();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);

	// Generated message map functions
	//{{AFX_MSG(CTimeOutSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
private:
	int m_nTimerID;
	CByteArray m_ReceivedData;
	BOOL	m_Kill;
	void * m_pReceiver; //Socket의 OnReceive를 받을 함수

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEOUTSOCKET1_H__77D3221F_6F91_41A7_AF2A_6520A8DFA1FB__INCLUDED_)
