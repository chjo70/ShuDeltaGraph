
// DeltaGraphView.h : CDeltaGraphView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "afxcmn.h"

#include "../ShuDeltaGraph/ListCtrl/ReportCtrl.h"

#include "../ShuDeltaGraph/EasySize/EasySize.h"

#include "Show.h"

class CDeltaGraphView : public CFormView, public CShow
{
private:
	CString m_strPathName;

	CDeltaGraphDoc *m_pDoc;

public:

private:
	void InitListCtrl( bool bInit=true );

public:
	void ShowGraph( ENUM_SUB_GRAPH enSubGraph );

public:
	//DECLARE_EASYSIZE;
	void __ES__RepositionControls(BOOL bInit);
	void __ES__CalcBottomRight(CWnd *pThis, BOOL bBottom, int &bottomright, int &topleft, UINT id, UINT br, int es_br, CRect &rect, int clientbottomright);	
#define INIT_EASYSIZE __ES__RepositionControls(TRUE); __ES__RepositionControls(FALSE)	
#define UPDATE_EASYSIZE if(GetWindow(GW_CHILD)!=NULL) __ES__RepositionControls(FALSE)	
#define EASYSIZE_MINSIZE(mx,my,s,r) if(r->right-r->left < mx) { if((s == WMSZ_BOTTOMLEFT)||(s == WMSZ_LEFT)||(s == WMSZ_TOPLEFT)) r->left = r->right-mx; else r->right = r->left+mx; } if(r->bottom-r->top < my) { if((s == WMSZ_TOP)||(s == WMSZ_TOPLEFT)||(s == WMSZ_TOPRIGHT)) r->top = r->bottom-my; else r->bottom = r->top+my; }

	inline void SetWindowTitle( CString strWindowTitle ) { m_strPathName=strWindowTitle; }

protected: // serialization������ ��������ϴ�.
	CDeltaGraphView();
	DECLARE_DYNCREATE(CDeltaGraphView)

public:
	enum{ IDD = IDD_DELTAGRAPH_FORM };

// Ư���Դϴ�.
public:
	CDeltaGraphDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CDeltaGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CReportCtrl m_CListPDW;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // DeltaGraphView.cpp�� ����� ����
inline CDeltaGraphDoc* CDeltaGraphView::GetDocument() const
   { return reinterpret_cast<CDeltaGraphDoc*>(m_pDocument); }
#endif

