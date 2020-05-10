// DialogRSA.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DialogRSA.h"
#include "afxdialogex.h"


// CDialogRSA ��ȭ �����Դϴ�.

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


// CDialogRSA �޽��� ó�����Դϴ�.


void CDialogRSA::OnBnClickedButtonColstart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
