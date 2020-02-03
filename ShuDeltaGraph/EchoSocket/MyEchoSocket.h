#if !defined(AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_)
#define AFX_MYECHOSOCKET_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEchoSocket.h : header file
//

#define MAX_LAN_BUFFER			(sizeof(int)*150)			//(sizeof(int)*2000)

// typedef enum {
// 	enUnknown = -1,
// 
// 	enSIM_EWC = 0,
// 
// 	enMax_Unit_ID,
// 
// 	enLogPrint
// 
// } enUnitID;

// 연동장비 목록
//static char g_szUnit[enMax_Unit_ID][30] = { "EWC", "DRC", "위협신호발생기", "송수신고주파제어장치", "DRC", "위협신호발생기", "송수신고주파제어장치" } ;

enum EN_CONNECT_MODE {
	IDLE = 0,
	LISTEN,
	CONNECTED,
	CLOSE

};

enum SHUPERHAT_REQ_COMMANDCODE {
	REQ_INIT=1001,
	REQ_SETMODE=1101,
	REQ_SET_CONFIG=9301,
	REQ_COL_START=9401,
	REQ_PBIT=0x05,
	REQ_IBIT=0x10,
	REQ_CBIT=0x11,
	REQ_STAT=0x14,

};

enum SHUPERHAT_RES_COMMANDCODE {
	RES_INIT = 1002,
	RES_INIT_RES = 0x02,
	RES_SET_CONFIG,
	RES_PBIT = 0x05,
	RES_IBIT = 0x10,
	RES_CBIT = 0x11,

	RES_STAT = 0x14,

};

// 헤더 데이터 구조체
typedef struct {
	unsigned int uiOpcode;
	unsigned int uiDataLength;

} STR_MESSAGE;

typedef struct {
	UINT uiReqCode;
	UINT uiErrorCode;

} STR_RES_INIT;

typedef struct {
	UINT uiMode;
	float fTuneFreq;
	UINT coPulseNum;
	float fColTime;
	float fThreshold;
	UINT uiPAStatus;

} STR_REQ_SETMODE;

typedef union {
	unsigned char buffer[10000];
	

	// 	ENUM_NormalOrNot enNormalorNot;
	// 	ENUM_NAKCode enNAKCode;
	// 
	// 	ENUM_FreqBand enFreqBand;
	// 
	// 요구 데이터 구조체 정의
	STR_REQ_SETMODE stSetMode;

	// 	STR_REQ_HPARF stReqHPARF;
	// 	STR_REQ_HPAPOWER stReqHPAPower;
	// 	STR_REQ_HPAHV stReqHPAHV;
	// 	STR_REQ_HPAALC stReqHPAALC;
	// 	STR_REQ_IP stReqIPAddress;
	// 
	// 결과 데이터 구조체 정의
	STR_RES_INIT stResInit;

} STR_DATA_CONTENTS;


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
