
// DeltaGraphView.cpp : CDeltaGraphView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "DeltaGraph.h"
#endif

#include "MainFrm.h"

#include "DeltaGraphDoc.h"
#include "DeltaGraphView2.h"

//#include "../ShuDeltaGraph/Log/LogDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeltaGraphView

IMPLEMENT_DYNCREATE(CDeltaGraphView2, CFormView)

BEGIN_MESSAGE_MAP(CDeltaGraphView2, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDeltaGraphView ����/�Ҹ�

CDeltaGraphView2::CDeltaGraphView2()
	: CFormView(CDeltaGraphView2::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CDeltaGraphView2::~CDeltaGraphView2()
{
}

void CDeltaGraphView2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CDeltaGraphView2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CDeltaGraphView2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

}

void CDeltaGraphView2::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDeltaGraphView2::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDeltaGraphView ����

#ifdef _DEBUG
void CDeltaGraphView2::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeltaGraphView2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDeltaGraphDoc* CDeltaGraphView2::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeltaGraphDoc)));
	return (CDeltaGraphDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeltaGraphView �޽��� ó����
