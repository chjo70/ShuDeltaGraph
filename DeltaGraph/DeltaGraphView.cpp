
// DeltaGraphView.cpp : CDeltaGraphView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "DeltaGraph.h"
#endif

#include "DeltaGraphDoc.h"
#include "DeltaGraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeltaGraphView

IMPLEMENT_DYNCREATE(CDeltaGraphView, CFormView)

BEGIN_MESSAGE_MAP(CDeltaGraphView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDeltaGraphView ����/�Ҹ�

CDeltaGraphView::CDeltaGraphView()
	: CFormView(CDeltaGraphView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CDeltaGraphView::~CDeltaGraphView()
{
}

void CDeltaGraphView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CDeltaGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CDeltaGraphView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

}

void CDeltaGraphView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDeltaGraphView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDeltaGraphView ����

#ifdef _DEBUG
void CDeltaGraphView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeltaGraphView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDeltaGraphDoc* CDeltaGraphView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeltaGraphDoc)));
	return (CDeltaGraphDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeltaGraphView �޽��� ó����
