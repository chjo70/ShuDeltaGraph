#pragma once
#include "afxwin.h"

#include "./EasySize/EasySize.h"


// CDlgMulti 대화 상자입니다.

class CDlgMulti : public CDialogEx
{
	DECLARE_EASYSIZE;

public:

private:
	CRect m_rectCurDlg;

	HWND m_hPE;			// the proessentials control object

	CThread m_theThread;

public:
	HANDLE m_hMulti;

	STR_SONATA_DATA *m_pSonataData;

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

	void GetNumberFormat( TCHAR *pSZ, double dValue );

	DECLARE_DYNAMIC(CDlgMulti)

public:
	CDlgMulti(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMulti();

	void InitVar();
	void ViewGraph();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MULTI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_CStatic;
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
