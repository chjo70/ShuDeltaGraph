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
	FreeBuffer();
}

void CDialogRSA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_INIT, m_CButtonInit);
	DDX_Control(pDX, IDC_BUTTON_SETMODE, m_CButtonSetMode);
	DDX_Control(pDX, IDC_STATIC_TOTAL_COL_LIST, m_CStaticTotalColList);
	DDX_Control(pDX, IDC_LIST_COL_LIST, m_ColList);
	DDX_Control(pDX, IDC_BUTTON_COLSTART, m_CButtonColStart);
	DDX_Control(pDX, IDC_SPIN_FRQ_LOW, m_CSpinFreqLow);
	DDX_Control(pDX, IDC_SPIN_FRQ_HGH, m_CSpinFreqHgh);
	DDX_Control(pDX, IDC_SPIN_AOA_LOW, m_CSpinAOALow);
	DDX_Control(pDX, IDC_SPIN_AOA_HGH, m_CSpinAOAHgh);
	DDX_Control(pDX, IDC_SPIN_PA_LOW, m_CSpinPALow);
	DDX_Control(pDX, IDC_SPIN_PA_HGH, m_CSpinPAHgh);
	DDX_Control(pDX, IDC_SPIN_COL_TIME, m_CSpinColTime);
	DDX_Control(pDX, IDC_SPIN_COL_NUM, m_CSpinColNum);
	DDX_Control(pDX, IDC_SPIN_NUM, m_CSpinNum);
}


BEGIN_MESSAGE_MAP(CDialogRSA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COLSTART, &CDialogRSA::OnBnClickedButtonColstart)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDialogRSA::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CDialogRSA::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_CHECKBOX, &CDialogRSA::OnBnClickedButtonAllselCheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_UNCHECKBOX, &CDialogRSA::OnBnClickedButtonAllselUncheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_INVCHECKBOX, &CDialogRSA::OnBnClickedButtonAllselInvcheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSELECT, &CDialogRSA::OnBnClickedButtonAllselect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COL_LIST, &CDialogRSA::OnDblclkListColList)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LIST, &CDialogRSA::OnBnClickedButtonAddList)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_LIST, &CDialogRSA::OnBnClickedButtonModifyList)
END_MESSAGE_MAP()


// CDialogRSA 메시지 처리기입니다.


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/12 0:15:37
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonColstart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTitle;
	STR_COL_LIST *pColList;
	STR_COL_LIST stColList;

	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	m_enMode = enColList_MODE;

	m_CButtonColStart.GetWindowText( strTitle );

	if( strTitle.Compare( _T("수집 시작") ) == 0 ) {
		Log( enNormal, _T("수집 시작을 했습니다." ) );

		m_pParentDlg->m_pConnected[enRSA]->InitVar();
		pApp->m_pDlgMulti->InitVar();

		m_uiColList = 0;

		m_uiCoColList = 0;
		pColList = m_pColList;
		for (int i = 0; i < m_ColList.GetItemCount(); i++) {
			if( TRUE == m_ColList.GetCheck(i) ) {
				GetColListFromList( i, & stColList );

				memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );
				++ pColList;
				++ m_uiCoColList;
			}
		}

		if( m_uiCoColList != 0 ) {
			ActivateGraph( TRUE );

			//
			m_theThread.Attach( FuncColList );
			m_theThread.Start( this );

			m_CButtonColStart.SetWindowText( _T("대기 취소") );
		}
		else {
			AfxMessageBox( _T("선택한 수집 과제가 없습니다 !") );
		}
	}
	else {
		m_pParentDlg->m_pConnected[enRSA]->InitVar();
		InitListCtrl( false );

		Log( enNormal, _T("수집 시작을 취소했습니다." ) );
		m_theThread.Stop( true );

		ActivateGraph( FALSE );

		m_CButtonColStart.SetWindowText( _T("수집 시작") );
	}
}

/**
 * @brief     
 * @param     int iRow
 * @param     STR_COL_LIST * pColList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 23:04:45
 * @warning   
 */
void CDialogRSA::GetColListFromList( int iRow, STR_COL_LIST *pColList )
{
	CString strTemp;

	pColList->iRowOfList = iRow;
	strTemp = m_ColList.GetItemText( iRow, 0 );
	swscanf_s( strTemp.GetBuffer(), _T("%d"), & pColList->stColItem.uiNo );

	strTemp = m_ColList.GetItemText( iRow, 1 );
	swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & pColList->stColItem.fAOALow, & pColList->stColItem.fAOAHgh );

	//pColList->stColItem.enMode = enIF2_WIDE;

	strTemp = m_ColList.GetItemText( iRow, 2 );
	swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & pColList->stColItem.fFreqLow, & pColList->stColItem.fFreqHgh );

	strTemp = m_ColList.GetItemText( iRow, 3 );
	swscanf_s( strTemp.GetBuffer(), _T("%d/%f"), & pColList->stColItem.uiColNumber, & pColList->stColItem.fColTime );

	strTemp = m_ColList.GetItemText( iRow, 4 );
	swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & pColList->stColItem.fPALow, & pColList->stColItem.fPAHgh );

}

/**
 * @brief     
 * @param     BOOL bEnable
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:26:48
 * @warning   
 */
void CDialogRSA::ActivateGraph( BOOL bEnable )
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	pApp->ActivateGraph( bEnable );
}


/**
 * @brief     
 * @param     bool bEnable
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 23:04:40
 * @warning   
 */
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


/**
 * @brief     
 * @return    BOOL
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 21:55:12
 * @warning   
 */
BOOL CDialogRSA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	Log( enNormal, _T("CDialogRSA+++++++++++++++++++++++++++++++++++++++++++") );

	InitVar();
	InitButton();
	InitBuffer();
	InitListCtrl();
	InitStatic();
	InitToolTip();

	// 스핀 카트롤 세팅
	m_CSpinNum.SetDecimalPlaces(0);
	m_CSpinNum.SetTrimTrailingZeros(FALSE);
	m_CSpinNum.SetRangeAndDelta( 1, 1000, 1.0 );
	m_CSpinNum.SetPos( (double) 1 );

	m_CSpinAOALow.SetDecimalPlaces(1);
	m_CSpinAOALow.SetTrimTrailingZeros(FALSE);
	m_CSpinAOALow.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinAOALow.SetPos( (double) m_pParentDlg->m_stColItem.fAOALow );

	m_CSpinAOAHgh.SetDecimalPlaces(1);
	m_CSpinAOAHgh.SetTrimTrailingZeros(FALSE);
	m_CSpinAOAHgh.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinAOAHgh.SetPos( (double) m_pParentDlg->m_stColItem.fAOAHgh );

	m_CSpinFreqLow.SetDecimalPlaces(1);
	m_CSpinFreqLow.SetTrimTrailingZeros(FALSE);
	m_CSpinFreqLow.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinFreqLow.SetPos( (double) m_pParentDlg->m_stColItem.fFreqLow );

	m_CSpinFreqHgh.SetDecimalPlaces(1);
	m_CSpinFreqHgh.SetTrimTrailingZeros(FALSE);
	m_CSpinFreqHgh.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinFreqHgh.SetPos( (double) m_pParentDlg->m_stColItem.fFreqHgh );

	m_CSpinPALow.SetDecimalPlaces(1);
	m_CSpinPALow.SetTrimTrailingZeros(FALSE);
	m_CSpinPALow.SetRangeAndDelta( -60, 0, 1.0 );
	m_CSpinPALow.SetPos( (double) m_pParentDlg->m_stColItem.fPALow );

	m_CSpinPAHgh.SetDecimalPlaces(1);
	m_CSpinPAHgh.SetTrimTrailingZeros(FALSE);
	m_CSpinPAHgh.SetRangeAndDelta( -60, 0, 1.0 );
	m_CSpinPAHgh.SetPos( (double) m_pParentDlg->m_stColItem.fPAHgh );

	m_CSpinColTime.SetDecimalPlaces(1);
	m_CSpinColTime.SetTrimTrailingZeros(FALSE);
	m_CSpinColTime.SetRangeAndDelta( 100, 100000, 1.0 );
	m_CSpinColTime.SetPos( (double) m_pParentDlg->m_stColItem.fColTime );

	m_CSpinColNum.SetDecimalPlaces(0);
	m_CSpinColNum.SetTrimTrailingZeros(FALSE);
	m_CSpinColNum.SetRangeAndDelta( 10, 1000, 1.0 );
	m_CSpinColNum.SetPos( (double) m_pParentDlg->m_stColItem.uiColNumber );

	CString strPathname = GetFilePath();
	strPathname += _T("//수집 목록(레이더).xlsx");
	OpenXLSViewList( strPathname );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 21:55:31
 * @warning   
 */
void CDialogRSA::InitVar()
{

	m_uiCoColList = 0;

	m_uiLog = 1;	

	m_uiCoRawData = 1;

	m_enConnectMode = IDLE;

	m_bClickedOfColList = false;
	m_bClickedOfRawDataList = false;

	m_strFilename = _T("");

	m_enMode = enNormal_Mode;

	m_iClickedItem = -1;
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:00:17
 * @warning   
 */
void CDialogRSA::InitButton()
{
	m_pParentDlg->InitButtonST( & m_CButtonInit, IDI_ICON_QUESTION );
	m_pParentDlg->InitButtonST( & m_CButtonSetMode, IDI_ICON_QUESTION );
	m_pParentDlg->InitButtonST( & m_CButtonColStart, IDI_ICON_CONNECTION );

	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetDlgItem(IDC_EDIT_NUM)->EnableWindow( FALSE );

	SetControl( false );
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:12:35
 * @warning   
 */
void CDialogRSA::FreeBuffer()
{

	free( m_ptxData );
	free( m_pColList );
	free( m_pRawData );

	CloseHandle( m_hReceveLAN );

	m_pTip->DestroyWindow();
	delete m_pTip;
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:03:16
 * @warning   
 */
void CDialogRSA::InitStatic()
{
	m_CStaticTotalColList.SetTextColor(RGB(0,0,255), FALSE);
	m_CStaticTotalColList.SetBackgroundColor( ::GetSysColor(COLOR_WINDOW), FALSE);
	m_CStaticTotalColList.SetBold(TRUE, FALSE);
	//m_CStaticTotalColList.SetFont(_T("Comic Sans MS"), 20, FALSE);

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:12:34
 * @warning   
 */
void CDialogRSA::InitToolTip()
{
	m_pTip = new CXInfoTip;
	m_pTip->Create(this);
	m_pTip->SetIcon( m_hToolTipIcon );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:37:59
 * @warning   
 */
void CDialogRSA::InitBuffer()
{

	m_iSelItem = -1;

 	m_ptxData = (char *) malloc(sizeof(char) * 100000);
 	m_pColList = ( STR_COL_LIST *) malloc( sizeof(STR_COL_ITEM) * MAX_COL_ITEMS );
 	m_pRawData = ( STR_RAW_DATA * ) malloc( sizeof(STR_RAW_DATA) );
 
 	m_hReceveLAN = CreateEvent( NULL, TRUE, FALSE, NULL );

}

/**
 * @brief     
 * @param     bool bInit
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:13:26
 * @warning   
 */
void CDialogRSA::InitListCtrl( bool bInit )
{
	int i;
	CRect rt;

	if( bInit == true ) {
		m_ColList.GetWindowRect(&rt);
		m_ColList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

		i = 0;
		m_ColList.InsertColumn(i++, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.12), -1 );
		m_ColList.InsertColumn(i++, _T("방위 범위[도]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2) , -1);
		m_ColList.InsertColumn(i++, _T("주파수 범위[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2) , -1);
		m_ColList.InsertColumn(i++, _T("수집 개수/시간[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.25), -1);
		m_ColList.InsertColumn(i++, _T("임계값[dBm]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.15 ), -1);
		m_ColList.InsertColumn(i++, _T("기타"), LVCFMT_LEFT, (int) (rt.Width() * 0.07), -1);

		m_ColList.SetGridLines(TRUE);
		m_ColList.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
		//m_ColList.SetCheckboxes(TRUE);

	}
	else {
		int i;

		for( i=0 ; i < m_ColList.GetItemCount() ; ++i ) {
			m_ColList.SetItemBkColor( i, -1, RGB(255, 255, 255) );
		}
	}

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:22:42
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:22:39
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonAllselCheckbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ColList.SetAllItemStates( RC_ITEM_ALL, RC_ITEM_CHECKED );
	m_ColList.SetFocus();
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:22:38
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonAllselUncheckbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ColList.SetAllItemStates( RC_ITEM_ALL, RC_ITEM_UNCHECKED );
	m_ColList.SetFocus();
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:22:34
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonAllselInvcheckbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ColList.InvertItems(RC_INVERT_CHECKMARK);
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:22:36
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonAllselect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ColList.SetAllItemStates( RC_ITEM_ALL, RC_ITEM_SELECTED );
	m_ColList.SetFocus();
}

/**
 * @brief     
 * @param     CString strPathname
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 23:15:09
 * @warning   
 */
void CDialogRSA::OpenXLSViewList( CString strPathname )
{
	long l, lMaxRow;
	float fValue1, fValue2;

	//m_ColList.DeleteAllItems();

#ifdef EXAUTOMATION
	CString strNumber, strDOARange, strFreqRange, strColTime, strPARange;

	// 엑셀 수집 파일 로딩하기...
	CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

	XL.OpenExcelFile( strPathname );
	lMaxRow = XL.GetRowNum();
	for( l=2 ; l <= lMaxRow; ++l ) {
		int nIndex;

		strNumber = XL.GetCellValue( 1, l );
		strNumber.Replace( _T("00000"), _T("") );
		strDOARange = XL.GetCellValue( 2, l );
		strFreqRange = XL.GetCellValue( 3, l );
		strColTime = XL.GetCellValue( 4, l );
		strPARange = XL.GetCellValue( 5, l );

		strNumber.Replace( _T(".0"), _T("") );
		nIndex = m_ColList.InsertItem( INT_MAX, strNumber, NULL );

		//

		swscanf_s( strDOARange.GetBuffer(), _T("%f/%f"), & fValue1, &fValue2 );
		strDOARange.Format( _T("%.2f/%.2f"), fValue1, fValue2 );

		swscanf_s( strFreqRange.GetBuffer(), _T("%f/%f"), & fValue1, &fValue2 );
		strFreqRange.Format( _T("%.2f/%.2f"), fValue1, fValue2 );

		swscanf_s( strPARange.GetBuffer(), _T("%f/%f"), & fValue1, &fValue2 );
		strPARange.Format( _T("%.1f/%.1f"), fValue1, fValue2 );

		m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strDOARange, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strFreqRange, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 4, LVIF_TEXT, strPARange, NULL, NULL, NULL, NULL);
	}

	XL.ReleaseExcel();
#else
	CExcelLib theExcel;

	WCHAR wsResult[MAX_STRING_BUFFER_SIZE];
	DWORD dwBX, dwBY, dwLX, dwLY;
	BOOL bResult;

	// text.xls 파일을 오픈한다.
	theExcel.OpenExcelFile( (TCHAR*)(LPCTSTR) strPathname );

	// 첫번째 Sheet를 선택한다.
	theExcel.SetActiveSheet(1);

	// Active Range, 즉 현재 활성화 되어 있는 범위를 얻어온다.
	theExcel.GetActiveRange(dwBX, dwBY, dwLX, dwLY);

	for( l=2 ; l <= dwLY ; ++l ) {
		int nIndex;
		TCHAR szTemp[100];

		theExcel.GetData( 1, l, szTemp, 100 );
		nIndex = m_ColList.InsertItem( INT_MAX, szTemp, NULL );

		theExcel.GetData( 2, l, szTemp, 100 );
		m_ColList.SetItem( nIndex, 1, LVIF_TEXT, szTemp, NULL, NULL, NULL, NULL);

		theExcel.GetData( 3, l, szTemp, 100 );
		m_ColList.SetItem( nIndex, 2, LVIF_TEXT, szTemp, NULL, NULL, NULL, NULL);

		theExcel.GetData( 4, l, szTemp, 100 );
		m_ColList.SetItem( nIndex, 3, LVIF_TEXT, szTemp, NULL, NULL, NULL, NULL);

		theExcel.GetData( 5, l, szTemp, 100 );
		m_ColList.SetItem( nIndex, 4, LVIF_TEXT, szTemp, NULL, NULL, NULL, NULL);


		//m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strCenterFreq, NULL, NULL, NULL, NULL);
		//m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
		//m_ColList.SetItem( nIndex, 4, LVIF_TEXT, strThreshold, NULL, NULL, NULL, NULL);
	}

	// 엑셀 파일을 닫는다.
	theExcel.CloseExcelFile();


#endif

}

/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 23:57:06
 * @warning   
 */
void CDialogRSA::OnDblclkListColList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	STR_COL_LIST stColList;

	m_iSelItem = pNMItemActivate->iItem;
	if( m_iSelItem >= 0 ) {
		GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( TRUE );

		GetColListFromList( m_iSelItem, & stColList );

		m_CSpinNum.SetPos( (double) stColList.stColItem.uiNo );
		
		m_CSpinAOALow.SetPos( (double) stColList.stColItem.fAOALow );
		m_CSpinAOAHgh.SetPos( (double) stColList.stColItem.fAOAHgh );

		m_CSpinFreqLow.SetPos( (double) stColList.stColItem.fFreqLow );
		m_CSpinFreqHgh.SetPos( (double) stColList.stColItem.fFreqHgh );

		m_CSpinPALow.SetPos( (double) stColList.stColItem.fPALow );
		m_CSpinPAHgh.SetPos( (double) stColList.stColItem.fPAHgh );

		m_CSpinColTime.SetPos( (double) stColList.stColItem.fColTime );
		m_CSpinColNum.SetPos( (double) stColList.stColItem.uiColNumber );

		//m_CSpinThreshold.SetPos( (double) stColList.stColItem.fThreshold );
	}

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/12 1:07:01
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonAddList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex;
	CString strTemp;

	STR_COL_LIST stColList;
	STR_COL_LIST *pColList;

	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetColList( & stColList );

	m_uiCoColList = m_ColList.GetItemCount();
	//
	pColList = m_pColList + m_uiCoColList;
	stColList.stColItem.uiNo = m_uiCoColList + 1;
	memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	//
	strTemp.Format(_T("%d"), pColList->stColItem.uiNo );
	nIndex = m_ColList.InsertItem( INT_MAX, strTemp, NULL );

	//strTemp.Format(_T("%s"), g_stColListMode[pColList->stColItem.enMode] );
	strTemp.Format(_T("%.1f/%.1f"), pColList->stColItem.fAOALow, pColList->stColItem.fAOAHgh );
	m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.2f/%.2f"), pColList->stColItem.fFreqLow, pColList->stColItem.fFreqHgh );
	m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%d/%.1f"), pColList->stColItem.uiColNumber, pColList->stColItem.fColTime );
	m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f/%.1f"), pColList->stColItem.fPALow, pColList->stColItem.fPAHgh );
	m_ColList.SetItem( nIndex, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	//m_ColList.SetItemState( -1, 0, LVIS_SELECTED|LVIS_FOCUSED );
	m_ColList.SetItemState( nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	//m_ColList.EnsureVisible( m_uiCoColList, FALSE); 

	SetTotalColList();

	++ m_uiCoColList;
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/12 1:12:22
 * @warning   
 */
void CDialogRSA::SetTotalColList()
{
	CString strTemp;

	strTemp.Format( _T("%d"), m_ColList.GetItemCount() );
	m_CStaticTotalColList.SetWindowText( strTemp );

	//Invalidate();

	//m_CStaticTotalColList.SetTextColor(RGB(0,0,255), FALSE);
	//m_CStaticTotalColList.SetBackgroundColor(RGB(255,255,255), FALSE);
	//m_CStaticTotalColList.SetBold(TRUE, FALSE);

}

/**
 * @brief     
 * @param     STR_COL_LIST * pstColList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 18:23:00
 * @warning   
 */
void CDialogRSA::GetColList( STR_COL_LIST *pstColList )
{
	pstColList->stColItem.uiNo = (UINT) m_CSpinNum.GetPos();

	GetColItem( & pstColList->stColItem );
}

/**
 * @brief     
 * @param     STR_COL_ITEM * pstColItem
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/12 1:13:01
 * @warning   
 */
void CDialogRSA::GetColItem( STR_COL_ITEM *pstColItem )
{
	//pstColItem->enMode = ( ENUM_COL_MODE ) m_CComboMode.GetCurSel();

	pstColItem->fAOALow = (float) m_CSpinAOALow.GetPos();
	pstColItem->fAOAHgh = (float) m_CSpinAOAHgh.GetPos();
	pstColItem->fFreqLow = (float) m_CSpinFreqLow.GetPos();
	pstColItem->fFreqHgh = (float) m_CSpinFreqHgh.GetPos();
	pstColItem->fPALow = (float) m_CSpinPALow.GetPos();
	pstColItem->fPAHgh = (float) m_CSpinPAHgh.GetPos();
	pstColItem->fColTime = (float) m_CSpinColTime.GetPos();
	pstColItem->uiColNumber = (UINT) m_CSpinColNum.GetPos();
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/12 1:07:25
 * @warning   
 */
void CDialogRSA::OnBnClickedButtonModifyList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
