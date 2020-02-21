#pragma once

#include "./Thread/Thread.h"
#include "./EchoSocket/MyEchoSocket.h"
#include "./ListCtrl/ReportCtrl.h"
#include "./SpinCtrl/NumSpinCtrl.h"
#include "./XColorStatic/XColorStatic.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "./ButtonST/BtnST.h"
#include "./ToolTip/XInfoTip.h"

#include "./Excel/XLEzAutomation.h"


// CDlgColList 대화 상자입니다.

#define SHU_PORT_NUM				(13060)

#define RAWDATA_DIRECTORY		_T("D://RAWDATA")

#define MAX_COL_ITEMS				(1000)
typedef enum {
	enOpenPDW = 0,
	enOpenXLS,
	enSavePDW,
	enSaveXLS

} ENUM_OPENTYPE;

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
} ENUM_COL_MODE;

typedef enum {
	enPDW_DataType=0,
	enIQ_DataType,

} ENUM_RAWDATA_TYPE;

typedef struct {
	BOOL bCkecked;

	UINT uiNo;

	ENUM_COL_MODE enMode;
	float fCenterFreq;
	float fColTime;
	UINT uiColNumber;
	float fThreshold;

} STR_COL_ITEM;

typedef struct {
	int iRowOfList;
	STR_COL_ITEM stColItem;;

} STR_COL_LIST;

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

#define MAX_RAW_DATA	(10000)

typedef union {
	STR_RES_PDW_DATA stPDWData[MAX_RAW_DATA];

	STR_RES_IQ_DATA stIQData[MAX_RAW_DATA];

	STR_RES_INTRA_DATA stIntraData[MAX_RAW_DATA];

} UNI_RAW_DATA;

typedef struct {
	UINT uiItem;
	STR_COL_LIST stColList;

	UNI_RAW_DATA unRawData;

} STR_RAW_DATA;


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

	STR_RAW_DATA *m_pRawData;

	STR_SONATA_DATA *m_pSonataData;

	// 송신 데이터
	char *m_ptxData;

	STR_COL_ITEM m_stColItem;
	
	UINT m_iSelItem;

	UINT m_uiLog;

	CXInfoTip *m_pTip;

	UINT m_uiCoRawData;

	CString m_strFilename;

	bool m_bClickedOfColList;
	bool m_bClickedOfRawDataList;

	int m_iClickedItem;

public:
	HANDLE m_hReceveLAN;
	bool m_bCompleteOnReceive;

	STR_COL_LIST *m_pColList;

	CThread m_theThread;

	MyEchoSocket *m_pListener;
	MyEchoSocket *m_pConnected;

	UINT m_uiCoColList;

private:
	void InitStatusBar();
	void InitBuffer();
	void InitVar();
	void InitThread();
	void FreeBuffer();
	void InitButton();
	void InitListCtrl( bool bInit=true );
	void InitStatic();
	void InitToolTip();

	void SetTotalColList();
	void SetTotalRawList();

	void LoadColList();
	void GetColList( STR_COL_LIST *pstColList );
	void GetColItem( STR_COL_ITEM *pstColItem );
	void PutColList( STR_COL_LIST *pstColList );
	void GetColListFromList( int iRow, STR_COL_LIST *pColList );
	void GetRawListFromList( int iRow, STR_RAW_LIST *pRawList );
	static BOOL CALLBACK ItemdataProc(DWORD dwData, LPARAM lParam);

	void UpdateResultData( char *pData );
	void UpdateToolTip( TCHAR *pszBuffer, CWnd *pDlgItem  );

	
	void LogTxMessage( void *pData, CString *pStrEtc=NULL );
	void MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );
	void InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3=NULL );

	void InitButtonST( CButtonST *pCButtonRouteSetup );

	void SetControl( bool bEnable );
	void MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );

	void ConvertRAWData( int iItem, ENUM_DataType enDataType );
	void MakeIQFile( int iItem );
	void MakePDWFile( int iItem );

	void MakeColListString( CString *pstrNum, CString *pstrMode, CString *pstrCenterFreq, CString *pstrColTime, CString *pstrThreshold, STR_COL_LIST *pstColList );

	void ViewGraph();
	void InsertPDWRawDataItem( STR_DATA_CONTENTS *pstData, int iItem );
	void InsertIntraRawDataItem( STR_DATA_CONTENTS *pstData, int iItem );
	void InsertIQRawDataItem( STR_DATA_CONTENTS *pstData, int iItem );

	// sonaat 체계 변환 함수
	UINT GetFreqBand( UINT uiFreqMHz );
	UINT ConvertFreq( float fFreq, int iBc );
	UINT ConvertPA( float fPA );
	UINT ConvertPW( float fPW );

	//void GetCellValue( CXlSimpleAutomation *pXL, UINT uiCol, long lRow, CString *pStrNum, CString *pStrMode, CString *pStrCenterFreq, CString *pStrColTime, CString *pStrThreshold );
	//void SetCellValue( CXlSimpleAutomation *pXL, long lRow, CString *pStrNum, CString *pStrMode, CString *pStrCenterFreq, CString *pStrColTime, CString *pStrThreshold );

	void ActivateGraph( BOOL bEanble );

public:
	void ProcessColList( STR_QUEUE_MSG *pQueueMsg );

	void InitSocketSetting();
	void CloseSocketSetting();
	void Connect();

	void OnAccept();
	void OnConnect( int nErrorCode );
	void OnClose();
	void OnReceive( char *pData );

	void Send();

	//STR_DATA_CONTENTS *GetRxData();
	//STR_MESSAGE *GetRxMessage();
	queue <STR_QUEUE_MSG> *GetQueueMessage();

	// 메시지 만들기
	void MakeSetModeMessage( UINT uiIndex );
	void MakeIQMessage( UINT uiIndex );
	void MakeColStartMessage();
	void MakeReqRawDataMessage();

	void SetIBkColorOfColList( UINT uiIndex, int nStep );

	void ReadyColStart( UINT uiIndex );

	inline STR_SONATA_DATA *GetSONATAData() { return m_pSonataData; }

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
	CReportCtrl m_ColList;
	CReportCtrl m_RawList;
	CNumSpinCtrl m_CSpinCenterFreq;
	CNumSpinCtrl m_CSpinThreshold;
	CNumSpinCtrl m_CSpinColTime;
	CNumSpinCtrl m_CSpinColNum;
	afx_msg void OnBnClickedButtonAddList();
	afx_msg void OnHdnItemdblclickListColList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListColList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonModifyList();
	CNumSpinCtrl m_CSpinNum;
	afx_msg void OnBnClickedButtonAllselect();
	afx_msg void OnBnClickedButtonSelDelete();
	afx_msg void OnBnClickedButtonAllselCheckbox();
	afx_msg void OnBnClickedButtonAllselUncheckbox();
	afx_msg void OnBnClickedButtonAllselInvcheckbox();
	CXColorStatic m_CStaticTotalColList;
	afx_msg void OnBnClickedButtonColstart();
	afx_msg void OnBnClickedButtonInit();
	CButtonST m_CButtonInit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButtonST m_CButtonSetMode;
	afx_msg void OnBnClickedButtonSetmode();
	CButtonST m_CButtonColStart;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	CComboBox m_CComboMode;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnLvnItemActivateListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
	CXColorStatic m_CStaticTotalRawList;
	afx_msg void OnHdnItemclickListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndScrollListRawdata(NMHDR *pNMHDR, LRESULT *pResult);
};
