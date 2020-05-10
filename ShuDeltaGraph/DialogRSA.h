#pragma once
#include "afxwin.h"

#include "./ButtonST/BtnST.h"
#include "./ToolTip/XInfoTip.h"


// CDialogRSA ��ȭ �����Դϴ�.

class CDialogRSA : public CDialogEx
{
private:
	CDlgColList *m_pParentDlg;

public:
	CThread m_theThread;

	HANDLE m_hReceveLAN;

public:
	void SetControl( bool bEnable );

	DECLARE_DYNAMIC(CDialogRSA)

public:
	CDialogRSA(CWnd* pParent = NULL, void *pParentDlg=NULL );   // ǥ�� �������Դϴ�.
	virtual ~CDialogRSA();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SHU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CButtonST m_CButtonInit;
	afx_msg void OnBnClickedButtonColstart();
	CStatic m_CStaticTotalColList;
	afx_msg void OnBnClickedButtonOpen();
	CButton m_CButtonColStart;
	CEdit m_CSpinColTime;
};
