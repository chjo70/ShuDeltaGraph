
// DeltaGraphView.h : CDeltaGraphView 클래스의 인터페이스
//

#pragma once

#include "resource.h"

#include "Show.h"
#include "afxwin.h"

#include "../ShuDeltaGraph/EasySize/EasySize.h"




class CDeltaGraphView2 : public CFormView, public CShow
{
private:
	CString m_strPathName;

	HWND m_hPE;			// the proessentials control object

	CDeltaGraphDoc *m_pDoc;

public:

private:
	void InitGraph();
	void CloseGraph();

public:
	//DECLARE_EASYSIZE;
	void __ES__RepositionControls(BOOL bInit);
	void __ES__CalcBottomRight(CWnd *pThis, BOOL bBottom, int &bottomright, int &topleft, UINT id, UINT br, int es_br, CRect &rect, int clientbottomright);	
#define INIT_EASYSIZE __ES__RepositionControls(TRUE); __ES__RepositionControls(FALSE)	
#define UPDATE_EASYSIZE if(GetWindow(GW_CHILD)!=NULL) __ES__RepositionControls(FALSE)	
#define EASYSIZE_MINSIZE(mx,my,s,r) if(r->right-r->left < mx) { if((s == WMSZ_BOTTOMLEFT)||(s == WMSZ_LEFT)||(s == WMSZ_TOPLEFT)) r->left = r->right-mx; else r->right = r->left+mx; } if(r->bottom-r->top < my) { if((s == WMSZ_TOP)||(s == WMSZ_TOPLEFT)||(s == WMSZ_TOPRIGHT)) r->top = r->bottom-my; else r->bottom = r->top+my; }

	void ShowGraph( ENUM_SUB_GRAPH enSubGraph );

	inline void SetWindowTitle( CString strWindowTitle ) { m_strPathName=strWindowTitle; }

protected: // serialization에서만 만들어집니다.
	CDeltaGraphView2();
	DECLARE_DYNCREATE(CDeltaGraphView2)

public:
	enum{ IDD = IDD_DELTAGRAPH_FORM2 };

// 특성입니다.
public:
	CDeltaGraphDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CDeltaGraphView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_CStaticGraph;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // DeltaGraphView.cpp의 디버그 버전
inline CDeltaGraphDoc* CDeltaGraphView2::GetDocument() const
   { return reinterpret_cast<CDeltaGraphDoc*>(m_pDocument); }
#endif

