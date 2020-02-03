
// ShuSIMDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

#include "../ShuDeltaGraph/ListCtrl/ReportCtrl.h"
#include "./EchoSocket/MyEchoSocket.h"

#define SHU_PORT_NUM				(1234)


// CShuSIMDlg ��ȭ ����
class CShuSIMDlg : public CDialogEx
{
private:
	CStatusBarCtrl m_StatusBar;

	MyEchoSocket *m_pListener;
	MyEchoSocket *m_pConnected;

	EN_CONNECT_MODE m_enConnectMode;

	char *m_ptxData;
	char *m_prxData;

	UINT m_uiLog;

private:
	void InitVar();
	void InitBuffer();
	void FreeBuffer();
	void InitLOGListCtrl();
	void InitStatusBar();
	void InitSocketSetting();

public:
	void OnClose();
	void OnAccept();
	void Connect();
	void OnConnect(int nErrorCode );
	void OnReceive();
	void Send();
	void ParseData( void *pData );

	void LogTxMessage( void *pData, CString *pStrEtc=NULL );
	void CShuSIMDlg::MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );
	void MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData );
	void InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3=NULL );

// �����Դϴ�.
public:
	CShuSIMDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SHUSIM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CReportCtrl m_CListLog;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonReqInit();
};