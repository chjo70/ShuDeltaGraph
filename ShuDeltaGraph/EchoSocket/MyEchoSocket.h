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

#define MAX_LAN_BUFFER			(sizeof(int)*150)			//(sizeof(int)*2000)

#include "ShuICD.h"

enum EN_CONNECT_MODE {
	IDLE = 0,
	LISTEN,
	CONNECTED,
	CLOSE

};

typedef enum {
	enUnknown = -1,

	enSHU = 0,
	enRSA,

} enUnitID;



typedef struct {
	STR_MESSAGE stMsg;

	STR_DATA_CONTENTS stData;

} STR_QUEUE_MSG;


/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket command target

class MyEchoSocket : public CAsyncSocket
{
// Attributes
public:
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

	enUnitID m_id;

// Operations
public:
	MyEchoSocket( enUnitID id, bool bBigEndian );
	virtual ~MyEchoSocket();

	bool Send( void *pData, int iDataLength );
	int Receive( void *lpBuf, int nBufLen, bool bBigEndian=true );
	void AllSwapData32( void *pData, int iLength );

	int GetLastError();

	queue <STR_QUEUE_MSG> *GetQueueMessage();
// 	STR_MESSAGE *GetRxMessage();
// 	STR_DATA_CONTENTS *GetRxData();

	void InitVar();

// Overrides
public:
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
