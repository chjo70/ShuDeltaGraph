
// ChildFrm.cpp : CChildFrame Ŭ������ ����
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CChildFrame ����/�Ҹ�

CChildFrame::CChildFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= (LONG) ~FWS_ADDTOTITLE;
	//cs.lpszName = Title;

	return TRUE;
}

// CChildFrame ����

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame �޽��� ó����


void CChildFrame::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CMenu popup;
	CMenu *pMenu;

	popup.LoadMenu( IDR_MENU1 );
	pMenu = popup.GetSubMenu(0);
}


void CChildFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CMDIChildWndEx::OnRButtonDown(nFlags, point);
}
