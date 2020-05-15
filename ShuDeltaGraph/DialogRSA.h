#pragma once
#include "afxwin.h"

#include "./ButtonST/BtnST.h"
#include "./ToolTip/XInfoTip.h"


#include "./DlgColList.h"

// CDialogSHU 대화 상자입니다.

DWORD WINAPI FuncColListRSA( LPVOID lpData );

class CDlgColList;

class CDialogRSA : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRSA)

private:
	UINT m_uiLog;
	UINT m_uiCoRawData;
	EN_CONNECT_MODE m_enConnectMode;

	bool m_bClickedOfColList;
	bool m_bClickedOfRawDataList;

	CString m_strFilename;

	ENUM_MODE m_enMode;

	int m_iClickedItem;

	CXInfoTip *m_pTip;
	HICON m_hToolTipIcon;

	UINT m_iSelItem;

	UINT m_uiColList;

	STR_COL_LIST *m_pColList;

	STR_RAW_DATA *m_pRawData;

	STR_RES_COL_START m_stResCol;

	STR_COL_LIST m_stColList;

public:
	char *m_ptxData;

	CThread m_theThread;

	HANDLE m_hReceveLAN;

	CDlgColList *m_pParentDlg;

	UINT m_uiCoColList;

private:
	void InitVar();
	void InitButton();
	void InitBuffer();
	void InitListCtrl( bool bInit=true );
	void InitStatic();
	void InitToolTip();
	void FreeBuffer();

	void OpenXLSViewList( CString strPathname );

	void GetColListFromList( int iRow, STR_COL_LIST *pColList );
	void ActivateGraph( BOOL bEnable );

	void GetColList( STR_COL_LIST *pstColList );
	void SetTotalColList();
	void GetColItem( STR_COL_ITEM *pstColItem );

	void MakeColListString( CString *pstrNum, CString *pstrMode, CString *pstrCenterFreq, CString *pstrColTime, CString *pstrThreshold, STR_COL_LIST *pstColList );
	void MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );

	int GetNextNo();

	void InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3=NULL );

	void MakeColStartMessage();
	void MakeReqRawDataMessage();
	void MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );

	inline void UpdateColList() {
		++ m_uiColList;	\
		if( m_uiColList >= m_uiCoColList )	\
			m_uiColList = 0;	\
	}

public:
	void SetControl( bool bEnable );

	void SetIBkColorOfColList( UINT uiIndex, int nStep );
	void ReadyColStart( UINT uiIndex );
	void MakeStopMessage( UINT uiIndex );
	void MakeSetModeMessage( UINT uiIndex );
	void ProcessColList( STR_QUEUE_MSG *pQueueMsg );
	void LogTxMessage( void *pData, CString *pStrEtc=NULL );
	

public:
	CDialogRSA(CWnd* pParent = NULL, void *pParentDlg=NULL );   // 표준 생성자입니다.
	virtual ~CDialogRSA();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SHU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButtonST m_CButtonInit;
	afx_msg void OnBnClickedButtonColstart();
	CXColorStatic m_CStaticTotalColList;
	CReportCtrl m_ColList;
	afx_msg void OnBnClickedButtonOpen();
	CButtonST m_CButtonColStart;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonInit();
	CButtonST m_CButtonSetMode;
	CNumSpinCtrl m_CSpinFreqLow;
	CNumSpinCtrl m_CSpinFreqHgh;
	CNumSpinCtrl m_CSpinAOALow;
	CNumSpinCtrl m_CSpinAOAHgh;
	CNumSpinCtrl m_CSpinPALow;
	CNumSpinCtrl m_CSpinPAHgh;
	CNumSpinCtrl m_CSpinColTime;
	CNumSpinCtrl m_CSpinColNum;
	CNumSpinCtrl m_CSpinNum;
	afx_msg void OnBnClickedButtonAllselCheckbox();
	afx_msg void OnBnClickedButtonAllselUncheckbox();
	afx_msg void OnBnClickedButtonAllselInvcheckbox();
	afx_msg void OnBnClickedButtonAllselect();
	afx_msg void OnDblclkListColList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddList();
	afx_msg void OnBnClickedButtonModifyList();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonRemoveLiist();
};
