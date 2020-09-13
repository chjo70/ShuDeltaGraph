#pragma once

#include "./ButtonST/BtnST.h"
#include "./ToolTip/XInfoTip.h"

#include "./DlgColList.h"

// CDialogSHU 대화 상자입니다.

class CDlgColList;

DWORD WINAPI FuncColList( LPVOID lpData );

class CDialogSHU : public CDialogEx
{
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

	void SetIBkColorOfColList( UINT uiIndex, int nStep );

	void ReadyColStart( UINT uiIndex );
	void MakeSetModeMessage( UINT uiIndex );

	void ProcessColList( STR_QUEUE_MSG *pQueueMsg );
	void InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3=NULL );
	void MakeIQMessage( UINT uiIndex );

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

	void InitButtonST( CButtonST *pCButtonRouteSetup );

	void OpenXLSViewList( CString strPathname );
	void SetTotalColList();

	void GetColListFromList( int iRow, STR_COL_LIST *pColList );
	void ActivateGraph( BOOL bEnable );

	void MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );

	void MakeColStartMessage();
	void MakeReqRawDataMessage();

	void GetColList( STR_COL_LIST *pstColList );
	void GetColItem( STR_COL_ITEM *pstColItem );

	int GetNextNo();

	void MakeColListString( CString *pstrNum, CString *pstrMode, CString *pstrCenterFreq, CString *pstrColTime, CString *pstrThreshold, STR_COL_LIST *pstColList );
	void MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );

	inline void UpdateColList() {
		++ m_uiColList;	\
		if( m_uiColList >= m_uiCoColList )	\
			m_uiColList = 0;	\
	}

public:
	void SetControl( bool bEnable );
	void LogTxMessage( void *pData, CString *pStrEtc=NULL );

	DECLARE_DYNAMIC(CDialogSHU)

public:
	CDialogSHU(CWnd* pParent = NULL, void *pParentDlg=NULL);   // 표준 생성자입니다.
	virtual ~CDialogSHU();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SHU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonColstart();
	CButtonST m_CButtonInit;
	CButtonST m_CButtonSetMode;
	CXColorStatic m_CStaticTotalColList;
	CReportCtrl m_ColList;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonAllselCheckbox();
	afx_msg void OnBnClickedButtonAllselUncheckbox();
	afx_msg void OnBnClickedButtonAllselInvcheckbox();
	afx_msg void OnBnClickedButtonAllselect();
	afx_msg void OnBnClickedButtonSelDelete();
	afx_msg void OnBnClickedButtonInit();
	CButtonST m_CButtonColStart;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnHdnItemclickListColList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListColList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemdblclickListColList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListColList(NMHDR *pNMHDR, LRESULT *pResult);
	CNumSpinCtrl m_CSpinCenterFreq;
	CNumSpinCtrl m_CSpinThreshold;
	CNumSpinCtrl m_CSpinColTime;
	CNumSpinCtrl m_CSpinColNum;
	CNumSpinCtrl m_CSpinNum;
	CComboBox m_CComboMode;
	afx_msg void OnBnClickedButtonAddList();
	afx_msg void OnBnClickedButtonModifyList();
	afx_msg void OnBnClickedButtonRemoveLiist();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
