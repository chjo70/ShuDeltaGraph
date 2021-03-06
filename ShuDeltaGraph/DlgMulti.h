#pragma once
#include "afxwin.h"

#include "./EasySize/EasySize.h"

typedef enum {
	enUnknownGraph=-1,

	enPDWGraph=0,
	enIQGraph

} ENUM_Graph;

#define MAX_IQ_DATA				(64*1024)


// CDlgMulti 대화 상자입니다.

class CDlgMulti : public CDialogEx
{
	DECLARE_EASYSIZE;

public:

private:
	CRect m_rectCurDlg;

	HWND m_hPE;			// the proessentials control object

	CThread m_theThread;

	float *m_pfPA;
	float *m_pfIndex;
	float *m_pfIP;
	float *m_pFFT;
	float *m_pfI;
	float *m_pfQ;

	float* m_pGlobalXData;

	ENUM_Graph m_enGraph;

	double m_dIMax, m_dIMin;
	double m_dQMax, m_dQMin;

	UINT m_uiPreItem;

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
	void InitGraph2();

	void GetNumberFormat( TCHAR *pSZ, double dValue );

	void ExecuteFFT( float *pfFFT, UINT UIuiItem, TNEW_IQ *pstIQData );

	DECLARE_DYNAMIC(CDlgMulti)

public:
	CDlgMulti(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMulti();

	void InitVar();
	void ViewGraph();

	void InitGraphSetting( ENUM_Graph enGraph, bool bForced=false );

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
