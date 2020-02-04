#if !defined(AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_)
#define AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEchoSocket.h : header file
//

#define MAX_LAN_BUFFER			(sizeof(int)*150)			//(sizeof(int)*2000)

#include "ShuICD.h"

enum EN_CONNECT_MODE {
	IDLE = 0,
	LISTEN,
	CONNECTED,
	CLOSE

};


/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket command target

class MyEchoSocket : public CAsyncSocket
{
// Attributes
public:
	bool m_bConnected;
	bool m_bBigEndian;

private:
	char *m_pData;
	UINT m_uiErrorCode;

// Operations
public:
	MyEchoSocket( bool bBigEndian );
	virtual ~MyEchoSocket();

	bool Send( void *pData, int iDataLength );
	int Receive( void *lpBuf, int nBufLen );
	void AllSwapData32( void *pData, int iLength );

	int GetLastError();

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
