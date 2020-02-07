#if !defined(AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_)
#define AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEchoSocket.h : header file
//

#include <iostream>
#include <queue>

using namespace std;

#define MAX_LAN_BUFFER			(sizeof(int)*600)			//(sizeof(int)*2000)



#include "../../ShuDeltaGraph/EchoSocket/ShuICD.h"

enum EN_CONNECT_MODE {
	IDLE = 0,
	LISTEN,
	CONNECTED,
	CLOSE

};

typedef struct {
	STR_MESSAGE stMsg;

	STR_DATA_CONTENTS stData;

} STR_QUEUE_MSG;

/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket command target

class MyEchoSocket : public CAsyncSocket
{
public:
// Attributes
	bool m_bConnected;
	bool m_bBigEndian;

	STR_QUEUE_MSG m_stQueueMsg;
	queue <STR_QUEUE_MSG> m_qMsg;

	char *m_prxBuffer;

private:
	int m_uiReceivedData;
	UINT m_uiDataLength;
	bool m_bHeader;
	char *m_pData;

	UINT m_uiErrorCode;

// Operations
public:
	MyEchoSocket( bool bBigEndian );
	virtual ~MyEchoSocket();

	bool Send( void *pData, int iDataLength );
	int Receive( void *lpBuf, int nBufLen, int nFlags=0 );
	void AllSwapData32( void *pData, int iLength );

	int GetLastError();

// Overrides
public:
	void InitVar();
	void SetParentDlg(CDialog *pDlg);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyEchoSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(MyEchoSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
private:
	CDialog * m_pDlg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_)
