#pragma once

#include "./EasySize/EasySize.h"

// CDlg3DBar 대화 상자입니다.

class CDlg3DBar : public CDialogEx
{
	DECLARE_EASYSIZE;

public:

	DECLARE_DYNAMIC(CDlg3DBar)

private:
	CRect m_rectCurDlg;

	HWND m_hPE;			// the proessentials control object
	CThread m_theThread;

	int m_nSubset;

	float *m_pfFreqX;

private:
	void InitButton();
	void InitBuffer();
	void InitStatusBar();
	void InitListCtrl();
	void InitStatic();
	void InitToolTip();
	void InitThread();
	void FreeBuffer();
	void InitGraph();

public:
	void InitVar();
	void ViewGraph();

public:
	HANDLE m_h3DBar;

	UINT *m_pFreqXFromHisto;

public:
	CDlg3DBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg3DBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_3DBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
