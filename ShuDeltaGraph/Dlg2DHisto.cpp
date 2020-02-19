// Dlg2DHisto.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "Dlg2DHisto.h"
#include "afxdialogex.h"


// CDlg2DHisto ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlg2DHisto, CDialogEx)

CDlg2DHisto::CDlg2DHisto(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg2DHisto::IDD, pParent)
{

}

CDlg2DHisto::~CDlg2DHisto()
{
}

void CDlg2DHisto::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg2DHisto, CDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CDlg2DHisto �޽��� ó�����Դϴ�.


BOOL CDlg2DHisto::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	this->GetWindowRect( m_rectCurDlg );

	Log( enNormal, _T("CDlg2DHisto +++++++++++++++++++++++++++++++++++++++++++") );

	InitVar();
	InitButton();
	InitBuffer();
	InitStatusBar();
	InitListCtrl();
	InitStatic();
	InitToolTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlg2DHisto::InitVar()
{

}

void CDlg2DHisto::InitButton()
{

}

void CDlg2DHisto::InitBuffer()
{

}

void CDlg2DHisto::InitListCtrl()
{

}

void CDlg2DHisto::InitStatusBar()
{

}

void CDlg2DHisto::InitStatic()
{

}

void CDlg2DHisto::InitToolTip()
{

}

void CDlg2DHisto::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CDlg2DHisto::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	LPMINMAXINFO lpminmax = ( LPMINMAXINFO ) lpMMI;

	// â �ּ�ũ�� ����

	lpminmax->ptMinTrackSize.x = m_rectCurDlg.right - m_rectCurDlg.left;
	lpminmax->ptMinTrackSize.y = m_rectCurDlg.bottom - m_rectCurDlg.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
