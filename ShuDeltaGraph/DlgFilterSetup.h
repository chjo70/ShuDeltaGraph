#pragma once

#include "NumSpinCtrl.h"
#include "afxcmn.h"

// CDlgFilterSetup 대화 상자입니다.

class CDlgFilterSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilterSetup)

public:
	CDlgFilterSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgFilterSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FILTER_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
