// DlgFilterSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgFilterSetup.h"
#include "afxdialogex.h"


// CDlgFilterSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgFilterSetup, CDialogEx)

CDlgFilterSetup::CDlgFilterSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFilterSetup::IDD, pParent)
{

}

CDlgFilterSetup::~CDlgFilterSetup()
{
}

void CDlgFilterSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_AOA_MAX, m_CSpinAoaMax);
	DDX_Control(pDX, IDC_SPIN_AOA_MIN, m_CSpinAoaMin);
	DDX_Control(pDX, IDC_SPIN_FRQ_MAX, m_CSpinFrqMax);
	DDX_Control(pDX, IDC_SPIN_FRQ_MIN, m_CSpinFrqMin);
	DDX_Control(pDX, IDC_SPIN_TOA_MIN, m_CSpinToaMin);
	DDX_Control(pDX, IDC_SPIN_TOA_MAX, m_CSpinToaMax);
}


BEGIN_MESSAGE_MAP(CDlgFilterSetup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgFilterSetup::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgFilterSetup 메시지 처리기입니다.


void CDlgFilterSetup::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strFilterSetup.dAoaMin = m_CSpinAoaMin.GetPos();
	m_strFilterSetup.dAoaMax = m_CSpinAoaMax.GetPos();
	m_strFilterSetup.dFrqMin = m_CSpinFrqMin.GetPos();
	m_strFilterSetup.dFrqMax = m_CSpinFrqMax.GetPos();

	//CDialogEx::OnOK();
}


BOOL CDlgFilterSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// TOA
	m_CSpinToaMin.SetTrimTrailingZeros (FALSE);
	m_CSpinToaMin.SetRangeAndDelta ( 0.0, 999999.0, 1.00);
	m_CSpinToaMin.SetPos ( m_strFilterSetup.dToaMin );
	m_CSpinToaMin.SetDecimalPlaces(1);
	m_CSpinToaMin.SetFormatString (NULL);

	m_CSpinToaMax.SetTrimTrailingZeros (FALSE);
	m_CSpinToaMax.SetRangeAndDelta ( 0.0, 999999.0, 1.00);
	m_CSpinToaMax.SetPos ( m_strFilterSetup.dToaMax );
	m_CSpinToaMax.SetDecimalPlaces(1);
	m_CSpinToaMax.SetFormatString (NULL);

	// 방위
	m_CSpinAoaMin.SetTrimTrailingZeros (FALSE);
	m_CSpinAoaMin.SetRangeAndDelta (-360.0, 360.0, 1.00);
	m_CSpinAoaMin.SetPos ( m_strFilterSetup.dAoaMin );
	m_CSpinAoaMin.SetDecimalPlaces(1);
	m_CSpinAoaMin.SetFormatString (NULL);

	m_CSpinAoaMax.SetTrimTrailingZeros (FALSE);
	m_CSpinAoaMax.SetRangeAndDelta (-360.0, 360.0, 1.00);
	m_CSpinAoaMax.SetPos ( m_strFilterSetup.dAoaMax );
	m_CSpinAoaMax.SetDecimalPlaces(1);
	m_CSpinAoaMax.SetFormatString (NULL);

	// 주파수
	m_CSpinFrqMin.SetTrimTrailingZeros (FALSE);
	m_CSpinFrqMin.SetRangeAndDelta (500.0, 8000.0, 10.00);
	m_CSpinFrqMin.SetPos (m_strFilterSetup.dFrqMin);
	m_CSpinFrqMin.SetDecimalPlaces(1);
	m_CSpinFrqMin.SetFormatString (NULL);

	m_CSpinFrqMax.SetTrimTrailingZeros (FALSE);
	m_CSpinFrqMax.SetRangeAndDelta (500.0, 8000.0, 10.00);
	m_CSpinFrqMax.SetPos (m_strFilterSetup.dFrqMax);
	m_CSpinFrqMax.SetDecimalPlaces(1);
	m_CSpinFrqMax.SetFormatString (NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgFilterSetup::SetValue( STR_FILTER_SETUP *pstFilterSetup )
{
	memcpy( & m_strFilterSetup, pstFilterSetup, sizeof(STR_FILTER_SETUP) );

}

