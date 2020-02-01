#pragma once

#include "./Thread/Thread.h"
#include "./EchoSocket/MyEchoSocket.h"
#include "./ListCtrl/ReportCtrl.h"
#include "./SpinCtrl/NumSpinCtrl.h"
#include "./XColorStatic/XColorStatic.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgColList 대화 상자입니다.

#define MAX_COL_ITEMS	(1000)

typedef struct {
	float fFrqLow;
	float fFrqHigh;

	float fColTime;
	UINT uiColNumber;

} STR_COL_ITEM;

typedef struct {
	UINT uiNo;
	STR_COL_ITEM stColItem;;

} STR_COL_LIST;

class CDlgColList : public CDialogEx
{
private:
	CStatusBarCtrl m_StatusBar;
	bool m_bConnected;
	EN_CONNECT_MODE m_enConnectMode;

	char *m_ptxData;
	char *m_prxData;

	STR_COL_ITEM m_stColList;

	UINT m_uiCoColList;
	STR_COL_LIST *m_pColList;

	UINT m_iSelItem;

public:
	CThread m_theThread;

	MyEchoSocket *m_pListener;
	MyEchoSocket *m_pConnected;

private:
	void InitStatusBar();
	void InitBuffer();
	void InitThread();
	void FreeBuffer();
	void InitButton();
	void InitListCtrl();
	void InitStatic();

	void SetTotalColList();

	void LoadColList();
	void GetColList( STR_COL_LIST *pstColList );
	void PutColList( STR_COL_LIST *pstColList );
	static BOOL CALLBACK ItemdataProc(DWORD dwData, LPARAM lParam);

public:
	void InitSocketSetting();
	void CloseSocketSetting();
	void Connect();

	void OnAccept();
	void OnConnect( int nErrorCode );
	void OnClose();
	void OnReceive();

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
	CNumSpinCtrl m_CSpinFreqLow;
	CNumSpinCtrl m_CSpinFreqHigh;
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
};
