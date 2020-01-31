#if !defined(AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_)
#define AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEchoSocket.h : header file
//

#define MAX_LAN_BUFFER			(sizeof(int)*150)			//(sizeof(int)*2000)

typedef enum {
	enUnknown = -1,

	enSIM_EWC = 0,
	enSIM_DRC,
	enSIM_Threat,
	enSIM_Tranceiver,
	enSIM_Max_Unit_ID,

	enDRC=4,
	enThreat,
	enTranceiver,

	//enSG,
	//enOSC,
	//enSA,

	enMax_Unit_ID,

	enLogPrint

} enUnitID;

// 연동장비 목록
static char g_szUnit[enMax_Unit_ID][30] = { "EWC", "DRC", "위협신호발생기", "송수신고주파제어장치", "DRC", "위협신호발생기", "송수신고주파제어장치" } ;


/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket command target

class MyEchoSocket : public CAsyncSocket
{
// Attributes
public:
	enUnitID m_id;
	bool m_bConnected;
	bool m_bBigEndian;

private:
	char *m_pData;
	UINT m_uiErrorCode;

// Operations
public:
	MyEchoSocket(enUnitID id, bool bBigEndian );
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
