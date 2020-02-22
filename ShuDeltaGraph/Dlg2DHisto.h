#pragma once
#include "afxwin.h"


// CDlg2DHisto ��ȭ �����Դϴ�.

#define CLK_TIMER		(1000)

typedef enum {
	en_ReceiveData= 0,
	en_Timer

} ENUM_HandleType;

class CDlg2DHisto : public CDialogEx
{
public:
//DECLARE_EASYSIZE;
	void __ES__RepositionControls(BOOL bInit);
	void __ES__CalcBottomRight(CWnd *pThis, BOOL bBottom, int &bottomright, int &topleft, UINT id, UINT br, int es_br, CRect &rect, int clientbottomright);	
	#define INIT_EASYSIZE __ES__RepositionControls(TRUE); __ES__RepositionControls(FALSE)	
	#define UPDATE_EASYSIZE if(GetWindow(GW_CHILD)!=NULL) __ES__RepositionControls(FALSE)	
	#define EASYSIZE_MINSIZE(mx,my,s,r) if(r->right-r->left < mx) { if((s == WMSZ_BOTTOMLEFT)||(s == WMSZ_LEFT)||(s == WMSZ_TOPLEFT)) r->left = r->right-mx; else r->right = r->left+mx; } if(r->bottom-r->top < my) { if((s == WMSZ_TOP)||(s == WMSZ_TOPLEFT)||(s == WMSZ_TOPRIGHT)) r->top = r->bottom-my; else r->bottom = r->top+my; }

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
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CStatic m_CGraph;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
