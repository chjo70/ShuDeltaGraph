// Dlg2DHisto.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "Dlg2DHisto.h"
#include "afxdialogex.h"


// CDlg2DHisto 대화 상자입니다.

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


// CDlg2DHisto 메시지 처리기입니다.


BOOL CDlg2DHisto::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlg2DHisto::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LPMINMAXINFO lpminmax = ( LPMINMAXINFO ) lpMMI;

	// 창 최소크기 지정

	lpminmax->ptMinTrackSize.x = m_rectCurDlg.right - m_rectCurDlg.left;
	lpminmax->ptMinTrackSize.y = m_rectCurDlg.bottom - m_rectCurDlg.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
