// DialogRSA.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DialogRSA.h"
#include "afxdialogex.h"


// CDialogRSA 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRSA, CDialogEx)

CDialogRSA::CDialogRSA(CWnd* pParent /*=NULL*/, void *pParentDlg )
	: CDialogEx(CDialogRSA::IDD, pParent)
{
	m_pParentDlg = ( CDlgColList * ) pParentDlg;
}

CDialogRSA::~CDialogRSA()
{
}

void CDialogRSA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_INIT, m_CButtonInit);
	DDX_Control(pDX, IDC_STATIC_TOTAL_COL_LIST, m_CStaticTotalColList);
	DDX_Control(pDX, IDC_BUTTON_COLSTART, m_CButtonColStart);
	DDX_Control(pDX, IDC_EDIT7, m_CSpinColTime);
}


BEGIN_MESSAGE_MAP(CDialogRSA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COLSTART, &CDialogRSA::OnBnClickedButtonColstart)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDialogRSA::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CDialogRSA 메시지 처리기입니다.


void CDialogRSA::OnBnClickedButtonColstart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDialogRSA::SetControl( bool bEnable )
{
	GetDlgItem(IDC_BUTTON_INIT)->EnableWindow( bEnable );
	GetDlgItem(IDC_BUTTON_SETMODE)->EnableWindow( bEnable );

	//GetDlgItem(IDC_BUTTON_ADD_LIST)->EnableWindow( !bEnable );

	GetDlgItem(IDC_BUTTON_COLSTART)->EnableWindow( bEnable );

}

void CDialogRSA::OnBnClickedButtonOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
