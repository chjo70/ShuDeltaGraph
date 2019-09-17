#pragma once

#include "NumSpinCtrl.h"
#include "afxcmn.h"

// CDlgFilterSetup ��ȭ �����Դϴ�.

class CDlgFilterSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilterSetup)

public:
	CDlgFilterSetup(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgFilterSetup();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_FILTER_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	STR_FILTER_SETUP m_stFilterSetup;

private:
	CNumSpinCtrl m_CSpinAoaMax;
	CNumSpinCtrl m_CSpinAoaMin;
	CNumSpinCtrl m_CSpinFrqMax;
	CNumSpinCtrl m_CSpinFrqMin;
	CNumSpinCtrl m_CSpinToaMin;
	CNumSpinCtrl m_CSpinToaMax;

public:
	void SetValue( STR_FILTER_SETUP *pstFilterSetup );

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

};
