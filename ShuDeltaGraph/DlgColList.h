#pragma once

#include "./Thread/Thread.h"
#include "./EchoSocket/MyEchoSocket.h"
#include "./ListCtrl/ReportCtrl.h"
#include "./SpinCtrl/NumSpinCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgColList 대화 상자입니다.

typedef struct {
	float fFrqLow;
	float fFrqHigh;

	float fColTime;
	UINT uiColNumber;

} STR_COL_LIST;

class CDlgColList : public CDialogEx
{
private:
	CStatusBarCtrl m_StatusBar;
	bool m_bConnected;
	EN_CONNECT_MODE m_enConnectMode;

	char *m_ptxData;
	char *m_prxData;

	STR_COL_LIST m_stColList;

public:
	CThread m_theThread;

	MyEchoSocket *m_pListener;
	MyEchoSocket *m_pConnected;

private:
	void InitStatusBar();
	void InitBuffer();
	void InitThread();
	void FreeBuffer();
	void InitListCtrl();

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
	CNumSpinCtrl m_CEditFrqLow;
	CNumSpinCtrl m_CEditFrqHigh;
};
