
// ShuDeltaGraphView.h : CShuDeltaGraphView 클래스의 인터페이스
//

#pragma once

#include "resource.h"

#include "DlgFilterSetup.h"

typedef enum {
	enUnselectedGraphView = -1,

	enGraphPulseInfo = 0,
	enGRAPH_PIE,
	enGRAPH_POLAR,
	enGRAPH_2D,
	enGRAPH_MULTI,
	enGRAPH_3D,

	enGRAPH_Seperated,

	enFilter_Set,
	
	enGRAPH_BubbleChart,

} ENUM_GRAPH_VIEW ;

// typedef enum {
// 	enUnselectedSubGraph = -1,
// 
// 	enSubMenu_1 = 1,
// 	enSubMenu_2,
// 	enSubMenu_3,
// 	enSubMenu_4,
// 	enSubMenu_5,
// 	enSubMenu_6,
// 
// 
// } ENUM_SUB_GRAPH ;


class CShuDeltaGraphView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CShuDeltaGraphView();
	DECLARE_DYNCREATE(CShuDeltaGraphView)

public:
	enum{ IDD = IDD_SHUDELTAGRAPH_FORM };

// 특성입니다.
public:
	CShuDeltaGraphDoc* GetDocument() const;

// 작업입니다.
public:
	static CDlgFilterSetup *m_pDlgFilterSetup;

	ENUM_GRAPH_VIEW m_enGraphView;

private:
	HWND m_hPE;			// the proessentials control object
	CShuDeltaGraphDoc *m_pDoc;
	
	ENUM_SUB_GRAPH m_enSubGraph;

	CString m_strPathName;

	CListCtrl *m_pListCtrl;

	double m_dToaMinX;
	double m_dToaMaxX;
	double m_dAoaMin;
	double m_dAoaMax;
	double m_dFrqMin;
	double m_dFrqMax;
	double m_dPaMin;
	double m_dPaMax;
	
private:
	void ShowPolarGraph(ENUM_SUB_GRAPH enSubGraph);
	void ShowPieGraph(ENUM_SUB_GRAPH enSubGraph);
	void Show2DGraph(ENUM_SUB_GRAPH enSubGraph );
	void ShowMultiGraph( ENUM_SUB_GRAPH enSubGraph );
	void Show3DGraph(ENUM_SUB_GRAPH enSubGraphid);
	void ShowPulseInfo(ENUM_SUB_GRAPH enSubGraph);
	void ShowAnnotation();

	void SetupCommonGraph(ENUM_DataType enDataType=en_PDW_DATA);
	void SetupGrid(void);
	void SetupPopupMenu(ENUM_DataType enDataType=en_PDW_DATA);
	void SetupFont(void);
	void SetupPlotting(void);
	void SetupGraph(void);

	void SetFreqRange(void);
	void ShowFilterSetup( ENUM_SUB_GRAPH enSubGraph );

public:
	void ShowGraph( ENUM_GRAPH_VIEW enGraphView, ENUM_SUB_GRAPH enSubGraph );

	inline void SetWindowTitle( CString strWindowTitle ) { m_strPathName=strWindowTitle; }

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CShuDeltaGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	LRESULT	OnPEKeyDwon(WPARAM wp, LPARAM lp);

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPdwPulseInfo();
	afx_msg void OnIqPulseInfo();
	afx_msg void OnPdwPiePulse();
	afx_msg void OnPdwPolarAoaPa();
	afx_msg void OnPdwPieDirection();
	afx_msg void OnPdwPolarAoaFreq();
	afx_msg void OnPdw2DToaAoa();
	afx_msg void OnPdw2dToaFreq();
	afx_msg void OnPdw2DToaPa();
	afx_msg void OnPdw2DToaPw();
	afx_msg void OnPdwMultiAoaFreqPaPw();
	afx_msg void OnPdwMultiAoa_FreqPaPw();
	afx_msg void OnPdwMultiAoa_FreqPa();
	afx_msg void OnPdw3DAoaFreq();
	afx_msg void OnPdw3DAoaFreqPw();
	afx_msg void OnIq2DIndexIQ();
	afx_msg void OnIq2DIndexPA();
	afx_msg void OnIq2DIndexPhase();
	afx_msg void OnIq2DIndexFFT();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnUpdateDlgCollist(CCmdUI *pCmdUI);
	afx_msg void OnDlgCollist();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

#ifndef _DEBUG  // ShuDeltaGraphView.cpp의 디버그 버전
inline CShuDeltaGraphDoc* CShuDeltaGraphView::GetDocument() const
   { return reinterpret_cast<CShuDeltaGraphDoc*>(m_pDocument); }
#endif

