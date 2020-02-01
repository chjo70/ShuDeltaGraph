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
	REQ_INIT=0x00,
	REQ_INIT_RES=0x02,
	REQ_SET_CONFIG,
	REQ_PBIT=0x05,
	REQ_IBIT=0x10,
	REQ_CBIT=0x11,
	REQ_STAT=0x14,

	REQ_HPACONFIG=0x80,
	REQ_HPASTART=0x81,
	REQ_HPACHANGECFG=0x82,
	REQ_HPASTOP=0x83,
	REQ_HPASTAT=0x84,
	REQ_HPAREADY=0x85,
	REQ_SETIPADDRESS=0x86,
	REQ_IPADDRESS=0x87,
	REQ_HPAMGCSETUP=0x23,

	REQ_HPAFAULTRESET=0x21,

	REQ_HPASSPASETUP=0x20,
	REQ_HPARFSETUP=0x22,
	REQ_HPAPOWERSETUP=0x26,
	REQ_HPAHVSETUP=0x27,

	REQ_SRUSTAT=0x29,
	REQ_HPAALCSETUP=0x2A,

};

enum SHUPERHAT_RES_COMMANDCODE {
	RES_INIT = 0x00,
	RES_INIT_RES = 0x02,
	RES_SET_CONFIG,
	RES_PBIT = 0x05,
	RES_IBIT = 0x10,
	RES_CBIT = 0x11,

	RES_STAT = 0x14,

	RES_HPACONFIG = 0x80,
	RES_HPASTART = 0x81,
	RES_HPACHANGECFG = 0x82,
	RES_HPASTOP = 0x83,
	RES_HPASTAT = 0x84,
	RES_HPAREADY = 0x85,
	RES_SETIPADDRESS = 0x86,
	RES_IPADDRESS = 0x87,
	RES_HPAMGCSETUP = 0x23,

	RES_HPAFAULTRESET=0x21,

	RES_HPASSPASETUP = 0x20,
	RES_HPARFSETUP = 0x22,
	RES_HPAPOWERSETUP = 0x26,
	RES_HPAHVSETUP=0x27,
	RES_SRUSTAT = 0x29,
	RES_HPAALCSETUP = 0x2A,

};

// 헤더 데이터 구조체
typedef struct {
	unsigned short   usDevice;			// 0x55AA
	unsigned char    ucReserve;         // unused
	unsigned char    usCommandCode;
	unsigned short   uiDataLength;
	unsigned short   usOpID;             // unused

} STR_MESSAGE;

typedef union {
	unsigned char buffer[10000];

// 	ENUM_NormalOrNot enNormalorNot;
// 	ENUM_NAKCode enNAKCode;
// 
// 	ENUM_FreqBand enFreqBand;
// 
// 	// 요구 데이터 구조체 정의
// 	STR_REQ_INIT stReqInit;
// 	STR_REQ_SETCONFIG stReqSetConfig;
// 	STR_REQ_HPACONFIG stReqHPAConfig;
// 	STR_REQ_HPAMGC stReqHPAMGC;
// 	STR_REQ_HPASSPA stReqHPASSPA;
// 	STR_REQ_HPARF stReqHPARF;
// 	STR_REQ_HPAPOWER stReqHPAPower;
// 	STR_REQ_HPAHV stReqHPAHV;
// 	STR_REQ_HPAALC stReqHPAALC;
// 	STR_REQ_IP stReqIPAddress;
// 
// 	// 결과 데이터 구조체 정의
// 	STR_RES_INIT stResInit;
// 	STR_RES_PBIT stResPBIT;
// 	STR_RES_IBIT stResIBIT;
// 	STR_RES_IBIT stResCBIT;
// 	STR_RES_UNITSTAT stResUnitStat;
// 	STR_RES_HPASTAT stResHPAStat;
// 	STR_RES_HPAREADY stResHPAReady;
// 	STR_RES_SRUSTAT stResSRUStat;
// 	STR_RES_IP stResIPAddress;

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
