#pragma once


// CDlg2DHisto 대화 상자입니다.

class CDlg2DHisto : public CDialogEx
{
private:
	CRect m_rectCurDlg;

public:

private:
	void InitVar();
	void InitButton();
	void InitBuffer();
	void InitStatusBar();
	void InitListCtrl();
	void InitStatic();
	void InitToolTip();

public:

	DECLARE_DYNAMIC(CDlg2DHisto)

public:
	CDlg2DHisto(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg2DHisto();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_2DHISTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
