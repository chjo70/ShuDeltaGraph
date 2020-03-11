#pragma once
#include "afxwin.h"

#include "./EasySize/EasySize.h"


// CDlg2DHisto 대화 상자입니다.

#define CLK_TIMER		(1000)

typedef enum {
	en_ReceiveData= 0,
	en_Timer

} ENUM_HandleType;

class CDlg2DHisto : public CDialogEx
{
	DECLARE_EASYSIZE;

public:

private:
	CRect m_rectCurDlg;

	HWND m_hPE;			// the proessentials control object
	CThread m_theThread;

public:
	HANDLE m_hHisto[en_Timer+1];

	STR_SONATA_DATA *m_pSonataData;
	UINT m_nFreqX[FREQ_MAX+1];

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
	void UpdateHisto( BOOL bData );

	inline UINT *GetFreqX() { return m_nFreqX; }

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
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CStatic m_CGraph;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
