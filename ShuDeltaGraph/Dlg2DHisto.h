#pragma once


// CDlg2DHisto ��ȭ �����Դϴ�.

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
	CDlg2DHisto(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlg2DHisto();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_2DHISTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
