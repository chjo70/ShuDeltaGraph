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

// CDlgColList 대화 상자입니다.

#define SHU_PORT_NUM				(1234)

#define MAX_COL_ITEMS			(1000)

typedef struct {
	int iMode;
	float fCenterFreq;
	float fColTime;
	UINT uiColNumber;
	float fThreshold;

} STR_COL_ITEM;

typedef struct {
	UINT uiNo;
	STR_COL_ITEM stColItem;;

} STR_COL_LIST;

class CDlgColList : public CDialogEx
{
private:
	HICON m_hToolTipIcon;
	CStatusBarCtrl m_StatusBar;
	EN_CONNECT_MODE m_enConnectMode;

	char *m_ptxData;
	char *m_prxData;

	STR_COL_ITEM m_stColItem;

	
	STR_COL_LIST *m_pColList;

	UINT m_iSelItem;

	UINT m_uiLog;

	CXInfoTip *m_pTip;

public:
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
	void InitListCtrl();
	void InitStatic();
	void InitToolTip();

	void SetTotalColList();

	void LoadColList();
	void GetColList( STR_COL_LIST *pstColList );
	void GetColItem( STR_COL_ITEM *pstColItem );
	void PutColList( STR_COL_LIST *pstColList );
	void GetColListFromList( int iRow, STR_COL_LIST *pColList );
	static BOOL CALLBACK ItemdataProc(DWORD dwData, LPARAM lParam);

	void UpdateResultData( char *pData );
	void UpdateToolTip( TCHAR *pszBuffer, CWnd *pDlgItem  );

	
	void LogTxMessage( void *pData, CString *pStrEtc=NULL );
	void MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );
	void InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3=NULL );


	void InitButtonST( CButtonST *pCButtonRouteSetup );

	void SetControl( bool bEnable );
	void MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );

public:
	void InitSocketSetting();
	void CloseSocketSetting();
	void Connect();

	void OnAccept();
	void OnConnect( int nErrorCode );
	void OnClose();
	void OnReceive();

	void Send();

	STR_DATA_CONTENTS *GetRxData();

	// 메시지 만들기
	void MakeSetModeMessage( UINT uiIndex );
	void MakeColStartMessage();
	void MakeReqRawDataMessage();

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
	CReportCtrl m_CListRawData;
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
};
