#pragma once

//
//#define EXAUTOMATION


#include "./Thread/Thread.h"
#include "./EchoSocket/MyEchoSocket.h"
#include "./ListCtrl/ReportCtrl.h"
#include "./SpinCtrl/NumSpinCtrl.h"
#include "./XColorStatic/XColorStatic.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "./ButtonST/BtnST.h"
#include "./ToolTip/XInfoTip.h"

#ifdef EXAUTOMATION
#include "./Excel/XLEzAutomation.h"
#elif defined(_EXCELLIB_)
#include "./Excel/ExcelLib.h"
#else
#endif

typedef enum {
	enColList_MODE=0,
	enNormal_Mode,

} ENUM_MODE;


typedef enum {
	enIF2_WIDE=0,
	enIF2_COURSE,
	enIF1_FINE,

	enIQ_WIDE,
	enIQ_NARROW,

	enModeUnknown
} ENUM_COL_MODE;

typedef struct {
	BOOL bCkecked;

	UINT uiNo;

	// 수퍼헷 수신 정보
	ENUM_COL_MODE enMode;
	float fCenterFreq;
	float fThreshold;

	// 레이더 수신 정보
	float fAOALow, fAOAHgh;
	float fFreqLow, fFreqHgh;
	float fPALow, fPAHgh;
	float fPWLow, fPWHgh;

	// 공통 정보
	float fColTime;
	UINT uiColNumber;

} STR_COL_ITEM;

typedef struct {
	int iRowOfList;
	STR_COL_ITEM stColItem;;

} STR_COL_LIST;


#define MAX_RAW_DATA	(65536)

typedef union {
	STR_RES_PDW_DATA stPDWData[MAX_RAW_DATA];
	STR_RES_PDW_DATA_RSA stRSAPDWData[MAX_RAW_DATA];
	STR_RES_IQ_DATA stIQData[MAX_COL_IQ_DATA];
	STR_RES_INTRA_DATA stIntraData[MAX_RAW_DATA];

} UNI_RAW_DATA;

typedef struct {
	UINT uiItem;
	STR_COL_LIST stColList;

	UNI_RAW_DATA unRawData;

} STR_RAW_DATA;

#include "DialogSHU.h"
#include "DialogRSA.h"


// CDlgColList 대화 상자입니다.

static int stPortNum[enRSA+1] = { 13060, 13030 } ;

#define RAWDATA_DIRECTORY		_T("C://RAWDATA")

#define MAX_COL_ITEMS				(1000)

typedef enum {
	enOpenPDW = 0,
	enOpenXLS,
	enOpenCSV,
	enSavePDW,
	enSaveXLS,
	enSaveCSV

} ENUM_OPENTYPE;





typedef enum {
	enPDW_DataType=0,
	enIQ_DataType,

} ENUM_RAWDATA_TYPE;


typedef struct {
	UINT uiNo;
	UINT uiColTaskNum;
	
	ENUM_RAWDATA_TYPE enRawDataType;
	ENUM_COL_MODE enMode;

	float fCenterFreq;
	float fColTime;
	UINT uiColNumber;
	float fThreshold;

	TCHAR szFilename[MAX_PATH];

} STR_RAW_ITEM;

typedef struct {
	int iRowOfList;

	STR_RAW_ITEM stRawItem;
} STR_RAW_LIST;
	
typedef struct {
	float fFreqMean;
	float fFreqMin;
	float fFreqMax;

	float fDtoaMean;
	float fDtoaMin;
	float fDtoaMax;
} STR_STAT_PDW;





class CDlgColList : public CDialogEx
{
private:
	UINT m_uiColList;
	ENUM_MODE m_enMode;

	HICON m_hToolTipIcon;
	CStatusBarCtrl m_StatusBar;
	EN_CONNECT_MODE m_enConnectMode;

	CDataFile m_theDataFile;

	//////////////////////////////////////////////////////////////////////////
	// 수집 목록 제어 관련 정보
	STR_COL_LIST m_stColList;
	STR_RES_COL_START m_stResCol;

	STR_SONATA_DATA *m_pSonataData;

	UINT m_iSelItem;

	UINT m_uiLog;

	CXInfoTip *m_pTip;

	UINT m_uiCoRawData;

	CString m_strFilename;

	bool m_bClickedOfColList;
	bool m_bClickedOfRawDataList;

	int m_iClickedItem;

	STR_STAT_PDW m_stStatPDW;

public:
	CDialogRSA *m_pDlgRSA;
	CDialogSHU *m_pDlgSHU;

	HANDLE m_hReceveLAN;
	bool m_bCompleteOnReceive;

	CThread m_theThread;

	MyEchoSocket *m_pListener[enRSA+1];
	MyEchoSocket *m_pConnected[enRSA+1];

	STR_COL_ITEM m_stColItem;

private:
	void InitStatusBar();
	void InitBuffer();
	void InitVar();
	void InitTab();
	void InitThread();
	void FreeBuffer();
	void InitListCtrl( bool bInit=true );
	void InitStatic();
	void InitToolTip();

	void SetTotalRawList();

	void PutColList( STR_COL_LIST *pstColList );
	void GetRawListFromList( int iRow, STR_RAW_LIST *pRawList );
	static BOOL CALLBACK ItemdataProc(DWORD dwData, LPARAM lParam);

	void UpdateResultData( char *pData );
	void UpdateToolTip( TCHAR *pszBuffer, CWnd *pDlgItem  );
	
	void LogTxMessage( void *pData, CString *pStrEtc=NULL );
	void MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );
	void InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3=NULL );

	void SetControl( bool bEnable );
	void MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );

	void ConvertRAWData( int iItem, ENUM_DataType enDataType, int uiColList, STR_RAW_DATA *pRawData, enUnitID enID );
	void MakeIQFile( int iItem, STR_RAW_DATA *pRawData );
	void MakePDWFile( int iItem, int uiColList, STR_RAW_DATA *pRawData, enUnitID enID );

	void InsertIntraRawDataItem( STR_DATA_CONTENTS *pstData, int iItem );
	
	// sonaat 체계 변환 함수
	UINT GetFreqBand( UINT uiFreqMHz );
	UINT ConvertFreq( float fFreq, int iBc );
	UINT ConvertPA( float fPA );
	UINT ConvertPW( float fPW );
	UINT ConvertAOA( float fAOA );

	void ActivateGraph( BOOL bEanble );

public:
	void ProcessColList( STR_QUEUE_MSG *pQueueMsg );

	void InitSocketSetting( enUnitID id );
	void CloseSocketSetting();
	void Connect();

	void OnAccept( enUnitID id );
	void OnConnect( int nErrorCode, enUnitID id );
	void OnSocketClose( enUnitID id );
	void OnReceive( char *pData );

	void Send( enUnitID id, char *ptxData );

	inline STR_SONATA_DATA *GetSONATAData() { return m_pSonataData; }

	void InsertIQRawDataItem( STR_DATA_CONTENTS *pstData, int iItem, STR_COL_LIST *pColList, STR_RAW_DATA *pRawData );
	void InsertPDWRawDataItem( STR_DATA_CONTENTS *pstData, int iItem, int uiColList, STR_COL_LIST *pColList, STR_RAW_DATA *pRawData, enUnitID enID );

	void ViewGraph( UINT uiOpCode );

	void InitButtonST( CButtonST *pCButtonRouteSetup, int iIcon );

	void InitUnitRes( enUnitID enID );
	void ClearRawDataList();

	DECLARE_DYNAMIC(CDlgColList)

public:
	CDlgColList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgColList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_COL_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CReportCtrl m_RawList;
	afx_msg void OnHdnItemdblclickListColList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLvnItemActivateListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
	CXColorStatic m_CStaticTotalRawList;
	afx_msg void OnHdnItemclickListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndScrollListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};
