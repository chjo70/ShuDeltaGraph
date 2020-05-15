// DialogSHU.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DialogSHU.h"
#include "afxdialogex.h"

TCHAR g_stColListMode[6][20] = { _T("IF2(광대역)"), _T("IF1 Course"), _T("IF1 Fine"), _T("광대역IQ"), _T("협대역IQ"), _T("") } ;


// CDialogSHU 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSHU, CDialogEx)

CDialogSHU::CDialogSHU(CWnd* pParent /*=NULL*/, void *pParentDlg )
	: CDialogEx(CDialogSHU::IDD, pParent)
{
	m_pParentDlg = ( CDlgColList * ) pParentDlg;
}

CDialogSHU::~CDialogSHU()
{
	FreeBuffer();
}

void CDialogSHU::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_INIT, m_CButtonInit);
	DDX_Control(pDX, IDC_BUTTON_SETMODE, m_CButtonSetMode);
	DDX_Control(pDX, IDC_STATIC_TOTAL_COL_LIST, m_CStaticTotalColList);
	DDX_Control(pDX, IDC_LIST_COL_LIST, m_ColList);
	DDX_Control(pDX, IDC_BUTTON_COLSTART, m_CButtonColStart);
	DDX_Control(pDX, IDC_SPIN_FRQ_LOW, m_CSpinCenterFreq);
	DDX_Control(pDX, IDC_SPIN_FRQ_HIGH, m_CSpinThreshold);
	DDX_Control(pDX, IDC_SPIN_COL_TIME, m_CSpinColTime);
	DDX_Control(pDX, IDC_SPIN_COL_NUM, m_CSpinColNum);
	DDX_Control(pDX, IDC_SPIN_NUM, m_CSpinNum);
	DDX_Control(pDX, IDC_COMBO_MODE, m_CComboMode);
}


BEGIN_MESSAGE_MAP(CDialogSHU, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COLSTART, &CDialogSHU::OnBnClickedButtonColstart)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDialogSHU::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_CHECKBOX, &CDialogSHU::OnBnClickedButtonAllselCheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_UNCHECKBOX, &CDialogSHU::OnBnClickedButtonAllselUncheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_INVCHECKBOX, &CDialogSHU::OnBnClickedButtonAllselInvcheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSELECT, &CDialogSHU::OnBnClickedButtonAllselect)
	ON_BN_CLICKED(IDC_BUTTON_SEL_DELETE, &CDialogSHU::OnBnClickedButtonSelDelete)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CDialogSHU::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogSHU::OnBnClickedButtonSave)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDialogSHU::OnHdnItemclickListColList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COL_LIST, &CDialogSHU::OnLvnItemchangedListColList)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDialogSHU::OnHdnItemdblclickListColList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COL_LIST, &CDialogSHU::OnDblclkListColList)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LIST, &CDialogSHU::OnBnClickedButtonAddList)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_LIST, &CDialogSHU::OnBnClickedButtonModifyList)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LIIST, &CDialogSHU::OnBnClickedButtonRemoveLiist)
END_MESSAGE_MAP()


// CDialogSHU 메시지 처리기입니다.


BOOL CDialogSHU::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	Log( enNormal, _T("CDialogSHU+++++++++++++++++++++++++++++++++++++++++++") );

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

	m_CSpinCenterFreq.SetDecimalPlaces(1);
	m_CSpinCenterFreq.SetTrimTrailingZeros(FALSE);
	m_CSpinCenterFreq.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinCenterFreq.SetPos( (double) m_pParentDlg->m_stColItem.fCenterFreq );

	m_CSpinThreshold.SetDecimalPlaces(1);
	m_CSpinThreshold.SetTrimTrailingZeros(FALSE);
	m_CSpinThreshold.SetRangeAndDelta( -60, 0, 1.0 );
	m_CSpinThreshold.SetPos( (double) m_pParentDlg->m_stColItem.fThreshold );

	m_CSpinColTime.SetDecimalPlaces(1);
	m_CSpinColTime.SetTrimTrailingZeros(FALSE);
	m_CSpinColTime.SetRangeAndDelta( 100, 100000, 1.0 );
	m_CSpinColTime.SetPos( (double) m_pParentDlg->m_stColItem.fColTime );

	m_CSpinColNum.SetDecimalPlaces(0);
	m_CSpinColNum.SetTrimTrailingZeros(FALSE);
	m_CSpinColNum.SetRangeAndDelta( 10, 1000, 1.0 );
	m_CSpinColNum.SetPos( (double) m_pParentDlg->m_stColItem.uiColNumber );

	m_CComboMode.AddString( _T("IF2(광대역)" ) );
	m_CComboMode.AddString( _T("IF1(Course)" ) );
	m_CComboMode.AddString( _T("IF1(FINE)" ) );
	m_CComboMode.AddString( _T("광대역(IQ)" ) );
	m_CComboMode.AddString( _T("협대역(IQ)" ) );
	m_CComboMode.SetCurSel( m_pParentDlg->m_stColItem.enMode );

	CString strPathname = GetFilePath();
	strPathname += _T("//수집 목록(수퍼헷).xlsx");
	OpenXLSViewList( strPathname );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:01:32
 * @warning   
 */
void CDialogSHU::InitToolTip()
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
 * @date      2020/05/10 21:01:30
 * @warning   
 */
void CDialogSHU::InitVar()
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
void CDialogSHU::InitButton()
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
 * @date      2020/05/10 21:03:16
 * @warning   
 */
void CDialogSHU::InitStatic()
{
	m_CStaticTotalColList.SetTextColor(RGB(0,0,255), FALSE);
	m_CStaticTotalColList.SetBackgroundColor( ::GetSysColor(COLOR_WINDOW), FALSE);
	m_CStaticTotalColList.SetBold(TRUE, FALSE);
	//m_CStaticTotalColList.SetFont(_T("Comic Sans MS"), 20, FALSE);

}

/**
 * @brief     
 * @param     bool bInit
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 22:13:16
 * @warning   
 */
void CDialogSHU::InitListCtrl( bool bInit )
{
	int i;
	CRect rt;

	if( bInit == true ) {
		m_ColList.GetWindowRect(&rt);
		m_ColList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

		i = 0;
		m_ColList.InsertColumn(i++, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.12), -1 );
		m_ColList.InsertColumn(i++, _T("모드"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2) , -1);
		m_ColList.InsertColumn(i++, _T("중심 주파수[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2) , -1);
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
 * @param     CButtonST * pCButtonRouteSetup
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:00:20
 * @warning   
 */
void CDialogSHU::InitButtonST( CButtonST *pCButtonRouteSetup )
{
	short	shBtnColor = 30;

	pCButtonRouteSetup->SetIcon( IDI_ICON_QUESTION, 24, 24, IDI_ICON_QUESTION, 24, 24);
	pCButtonRouteSetup->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:37:59
 * @warning   
 */
void CDialogSHU::InitBuffer()
{

	m_iSelItem = -1;

 	m_ptxData = (char *) malloc(sizeof(char) * 100000);
 	m_pColList = ( STR_COL_LIST *) malloc( sizeof(STR_COL_ITEM) * MAX_COL_ITEMS );
 	m_pRawData = ( STR_RAW_DATA * ) malloc( sizeof(STR_RAW_DATA) );
 
 	m_hReceveLAN = CreateEvent( NULL, TRUE, FALSE, NULL );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:37:56
 * @warning   
 */
void CDialogSHU::FreeBuffer()
{

	free( m_ptxData );
	free( m_pColList );
	free( m_pRawData );
	//free( m_pSonataData );

	CloseHandle( m_hReceveLAN );

	m_pTip->DestroyWindow();
	delete m_pTip;
}

/**
 * @brief     
 * @param     bool bEnable
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:00:23
 * @warning   
 */
void CDialogSHU::SetControl( bool bEnable )
{
	GetDlgItem(IDC_BUTTON_INIT)->EnableWindow( bEnable );
	GetDlgItem(IDC_BUTTON_SETMODE)->EnableWindow( bEnable );

	//GetDlgItem(IDC_BUTTON_ADD_LIST)->EnableWindow( !bEnable );

	GetDlgItem(IDC_BUTTON_COLSTART)->EnableWindow( bEnable );

}



/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:17:20
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonColstart()
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

		m_pParentDlg->ClearRawDataList();

		m_pParentDlg->m_pConnected[enSHU]->InitVar();
		pApp->m_pDlgMulti->InitVar();
		ResetEvent( m_hReceveLAN );

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
		m_pParentDlg->m_pConnected[enSHU]->InitVar();
		InitListCtrl( false );

		Log( enNormal, _T("수집 시작을 취소했습니다." ) );
		m_theThread.Stop( true );

		ActivateGraph( FALSE );


		m_CButtonColStart.SetWindowText( _T("수집 시작") );
	}
}

/**
 * @brief     
 * @param     LPVOID lpData
 * @return    DWORD WINAPI
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:46
 * @warning   
 */
DWORD WINAPI FuncColList( LPVOID lpData )
{
	CDialogSHU *pDlg;

	STR_QUEUE_MSG stQueueMsg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDialogSHU * ) pParent->GetParam();

	//pQueueMsg = pDlg->GetQueueMessage();

	pDlg->SetIBkColorOfColList( 0, 1 );

	pDlg->ReadyColStart( 0 );
	pDlg->MakeSetModeMessage( 0 );
	pDlg->m_pParentDlg->Send( enSHU, pDlg->m_ptxData );

	while( TRUE ) {
		DWORD dRet;

		dRet = WaitForSingleObject( pDlg->m_hReceveLAN, INFINITE);
		
		if( dRet == WAIT_FAILED ) {
			break;
		}
		else if( dRet == WAIT_ABANDONED ) {
			ResetEvent( pDlg->m_hReceveLAN );
			continue;
		}
		else if( dRet == WAIT_TIMEOUT )
			continue;
		else {
			ResetEvent( pDlg->m_hReceveLAN );
			
			//정상 처리를 수행한다.
			do {
				pDlg->m_pParentDlg->m_pConnected[enSHU]->LanMsg( false, & stQueueMsg );

				if( stQueueMsg.stMsg.uiOpcode != 0 ) {
					pDlg->ProcessColList( & stQueueMsg );
				}
				else {
					break;
				}
			} while ( true );

		}
	}

	return 0;
}

/**
 * @brief     
 * @param     STR_QUEUE_MSG * pQueueMsg
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:38:51
 * @warning   
 */
void CDialogSHU::ProcessColList( STR_QUEUE_MSG *pQueueMsg )
{
	STR_COL_LIST *pColList;
	CString strTemp1, strTemp2;

	// 수신한 메시지 출력
	MakeLogResMessage( & strTemp1, & strTemp2, pQueueMsg );
	InsertItem( & strTemp1, & strTemp2 );

	//Sleep( 100 );

	//
	if( m_enMode == enColList_MODE ) {
		pColList = m_pColList + m_uiColList;
		if( pColList->stColItem.enMode >= enIQ_WIDE ) {
			TRACE( "\n IQ 자동 과제 입니다.");
			switch( pQueueMsg->stMsg.uiOpcode ) {
			case RES_SET_CONFIG :
				SetIBkColorOfColList( m_uiColList, 2 );
				MakeIQMessage( m_uiColList );
				m_pParentDlg->Send( enSHU, m_ptxData );
				break;

			case RES_COL_START :
				SetIBkColorOfColList( m_uiColList, 4 );

				if( pQueueMsg->stData.stColStart.uiCoPulseNum != 0 ) {
				}
				else {
					ReadyColStart( m_uiColList );
					MakeSetModeMessage( m_uiColList );
					m_pParentDlg->Send( enSHU, m_ptxData );
				}
				break;

			case RES_RAWDATA_IQ :
				SetIBkColorOfColList( m_uiColList, 4 );

				//
				memcpy( & m_pRawData->stColList, pColList, sizeof(STR_COL_LIST) );

				memcpy( & m_pRawData->unRawData.stIQData[0], & pQueueMsg->stData.stIQData[0], sizeof(STR_RES_IQ_DATA)*MAX_COL_IQ_DATA );
				m_pRawData->uiItem = MAX_COL_IQ_DATA;

				m_pParentDlg->InsertIQRawDataItem( & pQueueMsg->stData, m_pRawData->uiItem, & m_stColList, m_pRawData );
				m_pParentDlg->ViewGraph( pQueueMsg->stMsg.uiOpcode );

				SetIBkColorOfColList( m_uiColList, -1 );

				UpdateColList();

				ReadyColStart( m_uiColList );
				MakeSetModeMessage( m_uiColList );
				m_pParentDlg->Send( enSHU, m_ptxData );

				TRACE( "\n 과제 번호 : %d", m_uiColList );

				break;

			default :
				break;
			}
		}
		else {
			//TRACE( "\n PDW 자동 과제[%d] 입니다.", m_uiColList );
			switch( pQueueMsg->stMsg.uiOpcode ) {
			case RES_SET_CONFIG :
				//TRACE( "\n RES_SET_CONFIG 처리 입니다." );
				SetIBkColorOfColList( m_uiColList, 2 );
				MakeColStartMessage();
				m_pParentDlg->Send( enSHU, m_ptxData );
				break;

			case RES_COL_START :
				//TRACE( "\n RES_COL_START 처리 입니다." );
				if( pQueueMsg->stData.stColStart.uiCoPulseNum != 0 ) {
					SetIBkColorOfColList( m_uiColList, 4 );

					MakeReqRawDataMessage();
					m_pParentDlg->Send( enSHU, m_ptxData );

					memcpy( & m_stResCol, & pQueueMsg->stData.stColStart, sizeof(STR_RES_COL_START) );
					m_pRawData->uiItem = 0;
				}
				else {
					SetIBkColorOfColList( m_uiColList, -1 );

					UpdateColList();

					ReadyColStart( m_uiColList );
					MakeSetModeMessage( m_uiColList );
					m_pParentDlg->Send( enSHU, m_ptxData );

					SetIBkColorOfColList( m_uiColList, 1 );
				}
				break;

			case RES_RAWDATA_PDW :
				m_pParentDlg->InitUnitRes( enRSA );
				//TRACE( "\n RES_RAWDATA_PDW 처리 입니다." );
				//SetIBkColorOfColList( m_uiColList, 4 );

				memcpy( & m_pRawData->unRawData.stPDWData[m_pRawData->uiItem], & pQueueMsg->stData.stPDWData[0], sizeof(STR_RES_PDW_DATA)*30 );
				m_pRawData->uiItem = ( m_pRawData->uiItem+30 > m_stResCol.uiCoPulseNum ? m_stResCol.uiCoPulseNum : m_pRawData->uiItem+30 );

				if( m_pRawData->uiItem >= m_stResCol.uiCoPulseNum ) {
					SetIBkColorOfColList( m_uiColList, 4 );

					m_pParentDlg->InsertPDWRawDataItem( & pQueueMsg->stData, m_pRawData->uiItem, m_uiColList, & m_stColList, m_pRawData, enSHU );
					m_pParentDlg->ViewGraph( pQueueMsg->stMsg.uiOpcode );

					SetIBkColorOfColList( m_uiColList, -1 );

					UpdateColList();

					ReadyColStart( m_uiColList );
					MakeSetModeMessage( m_uiColList );
					m_pParentDlg->Send( enSHU, m_ptxData );

					TRACE( "\n 과제 번호 : %d", m_uiColList );
				}
				break;

			default :
				TRACE( "\n 에러 처리 입니다." );
				break;
			}

		}


	}
	else {

	}


}

void CDialogSHU::SetIBkColorOfColList( UINT uiIndex, int nStep )
{
	STR_COL_LIST *pColList;

	pColList = m_pColList + uiIndex;

	TRACE( "\n 과제 번호: %d, 스텝: %d", uiIndex, nStep );

	if( nStep == -1 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(255, 255, 255) );
	else if( nStep <= 2 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 100, 0) );
	else if( nStep == 3 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 150, 0) );
	else if( nStep == 4 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 220, 0) );
	else if( nStep == 5 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 250, 0) );
	else 
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 0, 10) );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:55:35
 * @warning   
 */
void CDialogSHU::MakeReqRawDataMessage()
{
	STR_MESSAGE *pTxMessage;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_RAWDATA;
	pTxMessage->uiDataLength = 0;

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:51:42
 * @warning   
 */
void CDialogSHU::MakeColStartMessage()
{
	STR_MESSAGE *pTxMessage;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_COL_START;
	pTxMessage->uiDataLength = 0;

}

/**
 * @brief     
 * @param     UINT uiIndex
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:34:34
 * @warning   
 */
void CDialogSHU::MakeIQMessage( UINT uiIndex )
{
	STR_COL_LIST *pColList;

	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_SET_IQCONFIG;
	pTxMessage->uiDataLength = sizeof(int);

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );
	pColList = m_pColList + uiIndex;
	pTxData->uiMode = pColList->stColItem.enMode - 3;

}

/**
 * @brief     
 * @param     CString * pStrTemp1
 * @param     CString * pStrTemp2
 * @param     CString * pStrTemp3
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:41:48
 * @warning   
 */
void CDialogSHU::InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3 )
{
	CString strNum;

	strNum.Format(_T("%d"), m_uiLog );

	// 	int nIndex = m_CListLog.InsertItem( INT_MAX, strNum, NULL );
	// 	m_CListLog.SetItem(nIndex, 1, LVIF_TEXT, *pStrTemp1, NULL, NULL, NULL, NULL);
	// 	m_CListLog.SetItem(nIndex, 2, LVIF_TEXT, *pStrTemp2, NULL, NULL, NULL, NULL);
	// 
	// 	if( pStrTemp3 != NULL ) {
	// 		m_CListLog.SetItem(nIndex, 3, LVIF_TEXT, *pStrTemp3, NULL, NULL, NULL, NULL);
	// 	}

	Log( enNormal, _T("%d\t%s\t%s") , m_uiLog, (char*)(LPCTSTR)*pStrTemp1, (char*)(LPCTSTR)*pStrTemp2 );

	//m_CListCtrlLOG.SetItemBkColor(num, -1, ::GetSysColor(COLOR_INFOBK));
	//m_CListCtrlLOG.SetItemBkColor(num, -1, ::GetSysColor(COLOR_3DLIGHT));
	//m_CListCtrlLOG.SetItemBkColor(num, -1, ::GetSysColor(COLOR_MENUHILIGHT));
	//m_CListCtrlLOG.SetItemBkColor(num, -1, RGB(160, 255, 192));

	++ m_uiLog;

	//m_CListCtrlLOG.SetItemState( -1, 0, LVIS_SELECTED|LVIS_FOCUSED );
	//m_CListCtrlLOG.SetItemState( num, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	//m_CListCtrlLOG.EnsureVisible( num, FALSE); 
}

/**
 * @brief     
 * @param     CString * pstrTemp1
 * @param     CString * pstrTemp2
 * @param     void * pData
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:22
 * @warning   
 */
void CDialogSHU::MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
{
	STR_MESSAGE *pstMessage;
	STR_DATA_CONTENTS *pstData;

	pstMessage = (STR_MESSAGE *) pData;
	pstData = (STR_DATA_CONTENTS * ) ( ( char *) pData + sizeof(STR_MESSAGE) );

	switch (pstMessage->uiOpcode) {
	case RES_INIT:
		*pstrTemp1 = _T(">>초기화요구 응답");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
		break;

	case RES_SET_CONFIG:
		*pstrTemp1 = _T(">>수집 파라메타 설정 결과 응답");
		pstrTemp2->Format( _T("[%d]"), pstData->uiResult );
		break;

	case RES_COL_START :
		*pstrTemp1 = _T(">>수집시작 응답");
		pstrTemp2->Format( _T("ST[%d],Co[%d],Phase[%d]"), pstData->stColStart.uiStatus, pstData->stColStart.uiCoPulseNum, pstData->stColStart.uiPhase3Num );
		break;

	case RES_RAWDATA_PDW:
		*pstrTemp1 = _T(">>PDW 데이터");
		*pstrTemp2 = _T(" ");
		break;

	case RES_RAWDATA_INTRA:
		*pstrTemp1 = _T(">>FMOP 데이터");
		*pstrTemp2 = _T(" ");
		break;

	case RES_RAWDATA_IQ :
		pstrTemp1->Format( _T(">>IQ 데이터[%d]"), pstMessage->uiDataLength );
		*pstrTemp2 = _T(" ");
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("잘못된 명령[0x%x]입니다."), pstMessage->uiOpcode);
		break;
	}
}

/**
 * @brief     
 * @param     UINT uiIndex
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:38:27
 * @warning   
 */
void CDialogSHU::MakeSetModeMessage( UINT uiIndex )
{
	STR_COL_LIST *pColList;

	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_SET_CONFIG;
	pTxMessage->uiDataLength = sizeof(STR_REQ_SETMODE);

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );
	pColList = m_pColList + uiIndex;

	if( pColList->stColItem.enMode >= enIQ_WIDE )
		pTxData->stSetMode.uiMode = pColList->stColItem.enMode - 3;
	else {
		pTxData->stSetMode.uiMode = pColList->stColItem.enMode + 1;
	}

	pTxData->stSetMode.fTuneFreq = pColList->stColItem.fCenterFreq;
	pTxData->stSetMode.coPulseNum = pColList->stColItem.uiColNumber;
	pTxData->stSetMode.fColTime = pColList->stColItem.fColTime;
	pTxData->stSetMode.fThreshold = pColList->stColItem.fThreshold;
	pTxData->stSetMode.uiPAStatus = 0;

}

/**
 * @brief     
 * @param     UINT uiIndex
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:35:48
 * @warning   
 */
void CDialogSHU::ReadyColStart( UINT uiIndex )
{
	m_pRawData->uiItem = 0;

	memset( & m_stResCol, 0, sizeof(m_stResCol) );

	memcpy( & m_stColList, m_pColList+uiIndex, sizeof(STR_COL_LIST) );

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
void CDialogSHU::ActivateGraph( BOOL bEnable )
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	pApp->ActivateGraph( bEnable );
}

/**
 * @brief     
 * @param     int iRow
 * @param     STR_COL_LIST * pColList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:26:45
 * @warning   
 */
void CDialogSHU::GetColListFromList( int iRow, STR_COL_LIST *pColList )
{
	int i;
	CString strTemp;

	pColList->iRowOfList = iRow;
	strTemp = m_ColList.GetItemText( iRow, 0 );
	swscanf_s( strTemp.GetBuffer(), _T("%d"), & pColList->stColItem.uiNo );

	strTemp = m_ColList.GetItemText( iRow, 1 );

	pColList->stColItem.enMode = enIF2_WIDE;
	for( i=0 ; i <= enIQ_NARROW ; ++i ) {
		if( strTemp.Compare( g_stColListMode[i] ) == 0 ) {
			pColList->stColItem.enMode = ( ENUM_COL_MODE ) i;
		}
	}

	strTemp = m_ColList.GetItemText( iRow, 2 );
	swscanf_s( strTemp.GetBuffer(), _T("%f"), & pColList->stColItem.fCenterFreq );

	strTemp = m_ColList.GetItemText( iRow, 3 );
	swscanf_s( strTemp.GetBuffer(), _T("%d/%f"), & pColList->stColItem.uiColNumber, & pColList->stColItem.fColTime );

	strTemp = m_ColList.GetItemText( iRow, 4 );
	swscanf_s( strTemp.GetBuffer(), _T("%f"), & pColList->stColItem.fThreshold );

}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:54:46
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strPathName;

	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( FALSE );

	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	if( true == pApp->OpenFile( strPathName, _T("수집 목록 읽어오기..."), enOpenXLS ) ) {
		m_ColList.DeleteAllItems();
		OpenXLSViewList( strPathName );
	}

	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( TRUE );
}

/**
 * @brief     
 * @param     CString strPathname
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 1:18:46
 * @warning   
 */
void CDialogSHU::OpenXLSViewList( CString strPathname )
{
	long l, lMaxRow;
	float fValue;

	m_ColList.DeleteAllItems();

#ifdef EXAUTOMATION
	CString strNumber, strMode, strCenterFreq, strColTime, strThreshold;

	// 엑셀 수집 파일 로딩하기...
	CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

	XL.OpenExcelFile( strPathname );
	lMaxRow = XL.GetRowNum();
	for( l=2 ; l <= lMaxRow; ++l ) {
		int nIndex;

		strNumber = XL.GetCellValue( 1, l );
		strNumber.Replace( _T("00000"), _T("") );
		strMode = XL.GetCellValue( 2, l );
		strCenterFreq = XL.GetCellValue( 3, l );
		strColTime = XL.GetCellValue( 4, l );
		strThreshold = XL.GetCellValue( 5, l );

		strNumber.Replace( _T(".0"), _T("") );
		nIndex = m_ColList.InsertItem( INT_MAX, strNumber, NULL );

		//
		swscanf_s( strCenterFreq.GetBuffer(), _T("%f"), & fValue );
		strCenterFreq.Format( _T("%.2f"), fValue );

		swscanf_s( strThreshold.GetBuffer(), _T("%f"), & fValue );
		strThreshold.Format( _T("%.1f"), fValue );

		m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strMode, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strCenterFreq, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 4, LVIF_TEXT, strThreshold, NULL, NULL, NULL, NULL);
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
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:57:29
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonAllselCheckbox()
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
 * @date      2020/05/10 21:59:55
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonAllselUncheckbox()
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
 * @date      2020/05/10 21:59:52
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonAllselInvcheckbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ColList.InvertItems(RC_INVERT_CHECKMARK);
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:59:50
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonAllselect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ColList.SetAllItemStates( RC_ITEM_ALL, RC_ITEM_SELECTED );
	m_ColList.SetFocus();
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:59:47
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonSelDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	//m_ColList.DeleteAllSelectedItems(ItemdataProc, (LPARAM)this);

	SetTotalColList();
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/12 1:11:54
 * @warning   
 */
void CDialogSHU::SetTotalColList()
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
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:59:45
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_pParentDlg->m_pListener[enSHU] != NULL && m_pParentDlg->m_pListener[enSHU]->m_bConnected == true ) {
		STR_MESSAGE *pTxMessage;
		STR_DATA_CONTENTS *pTxData;

		pTxMessage = (STR_MESSAGE * ) m_ptxData;
		pTxMessage->uiOpcode = REQ_INIT;
		pTxMessage->uiDataLength = sizeof(STR_RES_INIT);

		pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );

		pTxData->stResInit.uiReqCode = 0;
		pTxData->stResInit.uiErrorCode = 0;

		m_pParentDlg->Send( enSHU, m_ptxData );
	}
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 1:06:11
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strPathName;

	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_SAVE )->EnableWindow( FALSE );

	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	if( true == pApp->OpenFile( strPathName, _T("수집 목록 저장하기..."), enSaveXLS ) ) {
#ifdef EXAUTOMATION
		int i;

		CString strNumber, strMode, strCenterFreq, strColTime, strThreshold;

		STR_COL_LIST stColList;

		// 엑셀 수집 파일 로딩하기...
		CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

		XL.SetCellValue( 1, 1, _T("과제 번호") );
		XL.SetCellValue( 2, 1, _T("모드") ); 
		XL.SetCellValue( 3, 1, _T("중심 주파수[MHz]") ); 
		XL.SetCellValue( 4, 1, _T("수집 개수/시간[ms]") ); 
		XL.SetCellValue( 5, 1, _T("임계값[dBm]") ); 
		XL.SetCellValue( 6, 1, _T("기타") ); 

		for ( i = 0; i < m_ColList.GetItemCount(); i++) {
			if( TRUE == m_ColList.GetCheck(i) || true ) {
				GetColListFromList( i, & stColList );

				MakeColListString( & strNumber, & strMode, & strCenterFreq, & strColTime, & strThreshold, & stColList );

				//SetCellValue( & XL, i+1, & strNumber, & strMode, & strCenterFreq, & strColTime, & strThreshold );
				XL.SetCellValue( 1, i+2, strNumber ); 
				XL.SetCellValue( 2, i+2, strMode ); 
				XL.SetCellValue( 3, i+2, strCenterFreq ); 
				XL.SetCellValue( 4, i+2, strColTime ); 
				XL.SetCellValue( 5, i+2, strThreshold ); 

			}
		}

		XL.SaveFileAs(strPathName);

		XL.ReleaseExcel();
#else


#endif

	}

	GetDlgItem( IDC_BUTTON_SAVE )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( TRUE );

}

void CDialogSHU::MakeColListString( CString *pstrNum, CString *pstrMode, CString *pstrCenterFreq, CString *pstrColTime, CString *pstrThreshold, STR_COL_LIST *pstColList )
{
	pstrNum->Format(_T("%d"), pstColList->stColItem.uiNo );

	pstrMode->Format(_T("%s"), g_stColListMode[pstColList->stColItem.enMode] );

	pstrCenterFreq->Format(_T("%.2f"), pstColList->stColItem.fCenterFreq );

	pstrColTime->Format(_T("%d/%.1f"), pstColList->stColItem.uiColNumber, pstColList->stColItem.fColTime );

	pstrThreshold->Format(_T("%.1f"), pstColList->stColItem.fThreshold );


}


void CDialogSHU::OnHdnItemclickListColList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 1:36:31
 * @warning   
 */
void CDialogSHU::OnLvnItemchangedListColList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 1:36:34
 * @warning   
 */
void CDialogSHU::OnHdnItemdblclickListColList(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_CComboMode.SetCurSel( stColList.stColItem.enMode );
		m_CSpinCenterFreq.SetPos( (double) stColList.stColItem.fCenterFreq );

		m_CSpinColTime.SetPos( (double) stColList.stColItem.fColTime );
		m_CSpinColNum.SetPos( (double) stColList.stColItem.uiColNumber );

		m_CSpinThreshold.SetPos( (double) stColList.stColItem.fThreshold );
	}
}

/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/11 23:56:51
 * @warning   
 */
void CDialogSHU::OnDblclkListColList(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_CComboMode.SetCurSel( stColList.stColItem.enMode );
		m_CSpinCenterFreq.SetPos( (double) stColList.stColItem.fCenterFreq );

		m_CSpinColTime.SetPos( (double) stColList.stColItem.fColTime );
		m_CSpinColNum.SetPos( (double) stColList.stColItem.uiColNumber );

		m_CSpinThreshold.SetPos( (double) stColList.stColItem.fThreshold );
	}

}

/**
 * @brief     
 * @param     STR_COL_ITEM * pstColItem
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:02:44
 * @warning   
 */
void CDialogSHU::GetColItem( STR_COL_ITEM *pstColItem )
{
	pstColItem->enMode = ( ENUM_COL_MODE ) m_CComboMode.GetCurSel();

	pstColItem->fCenterFreq = (float) m_CSpinCenterFreq.GetPos();
	pstColItem->fThreshold = (float) m_CSpinThreshold.GetPos();
	pstColItem->fColTime = (float) m_CSpinColTime.GetPos();
	pstColItem->uiColNumber = (UINT) m_CSpinColNum.GetPos();
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
void CDialogSHU::GetColList( STR_COL_LIST *pstColList )
{
	pstColList->stColItem.uiNo = (UINT) m_CSpinNum.GetPos();

	GetColItem( & pstColList->stColItem );
}

void CDialogSHU::OnBnClickedButtonAddList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex;
	CString strTemp;

	STR_COL_LIST stColList;
	//STR_COL_LIST *pColList;

	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetColList( & stColList );

	//
	//pColList = m_pColList + m_uiCoColList;
	stColList.stColItem.uiNo = GetNextNo();
	//memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	//
	strTemp.Format(_T("%d"), stColList.stColItem.uiNo );
 	nIndex = m_ColList.InsertItem( INT_MAX, strTemp, NULL );

	strTemp.Format(_T("%s"), g_stColListMode[stColList.stColItem.enMode] );
	m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), stColList.stColItem.fCenterFreq );
 	m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%d/%.1f"), stColList.stColItem.uiColNumber, stColList.stColItem.fColTime );
	m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f"), stColList.stColItem.fThreshold );
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
 * @date      2020/05/11 1:56:22
 * @warning   
 */
void CDialogSHU::OnBnClickedButtonModifyList()
{
	//TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strNumber, strMode, strCenterFreq, strColTime, strThreshold;

	STR_COL_LIST stColList;
	STR_COL_LIST *pColList;

	GetDlgItem(IDC_EDIT_NUM)->EnableWindow( FALSE );
	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetColList( & stColList );

	// 수집 목록 버퍼에 복사
	pColList = m_pColList + ( stColList.stColItem.uiNo - 1 );
	memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	// 목록창에 전시
	MakeColListString( & strNumber, & strMode, & strCenterFreq, & strColTime, & strThreshold, & stColList );
	
	m_ColList.SetItem( m_iSelItem, 0, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 1, LVIF_TEXT, strMode, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 2, LVIF_TEXT, strCenterFreq, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 4, LVIF_TEXT, strThreshold, NULL, NULL, NULL, NULL);
}

void CDialogSHU::OnBnClickedButtonRemoveLiist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iNo, iValue;
	CString strTemp;

	iNo = (int) m_CSpinNum.GetPos();
	for (int i = 0; i < m_ColList.GetItemCount(); i++) {
		strTemp = m_ColList.GetItemText( i, 0 );
		swscanf_s( strTemp.GetBuffer(), _T("%d"), & iValue );
		if( iNo == iValue ) {
			m_ColList.DeleteItem( i );
			break;
 		}
	}

}

int CDialogSHU::GetNextNo()
{
	int iMaxNo=0, iValue;
	CString strTemp;

	for (int i = 0; i < m_ColList.GetItemCount(); i++) {
		strTemp = m_ColList.GetItemText( i, 0 );
		swscanf_s( strTemp.GetBuffer(), _T("%d"), & iValue );

		iMaxNo = max( iValue, iMaxNo );

	}

	return iMaxNo + 1;
}

void CDialogSHU::LogTxMessage( void *pData, CString *pStrEtc )
{
	CString strTemp1, strTemp2;

	MakeLogReqMessage( & strTemp1, & strTemp2, pData );

	//m_pTabThreatDialog->MakeLogTxMessage( & strTemp1, & strTemp2, pData, true );

	InsertItem( & strTemp1, & strTemp2, pStrEtc );

}

void CDialogSHU::MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
{
	STR_MESSAGE *pstMessage;
	STR_DATA_CONTENTS *pstData;

	pstMessage = (STR_MESSAGE *) pData;
	pstData = (STR_DATA_CONTENTS * ) ( ( char *) pData + sizeof(STR_MESSAGE) );

	switch (pstMessage->uiOpcode) {
	case REQ_INIT:
		*pstrTemp1 = _T("<<초기화 요구");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
		break;

	case REQ_SETMODE:
		*pstrTemp1 = _T("<<시스템모드 전환 통보");
		*pstrTemp2 = _T("");
		break;

	case REQ_SET_CONFIG:
		*pstrTemp1 = _T("<<수집 파라메터 설정");
		pstrTemp2->Format( _T("M%d, %.1f[MHz], %d[개수], %.1f[ms], %.1f[dBm]"), pstData->stSetMode.uiMode, pstData->stSetMode.fTuneFreq, pstData->stSetMode.coPulseNum, pstData->stSetMode.fColTime, pstData->stSetMode.fThreshold );
		break;

	case REQ_COL_START :
		*pstrTemp1 = _T("<<신호 수집 시작 요구");
		break;

	case REQ_RAWDATA :
		*pstrTemp1 = _T("<<수집 데이터 요구");
		break;

	case REQ_SET_IQCONFIG :
		*pstrTemp1 = _T("<<IQ 수집 시작 요청");
		pstrTemp2->Format( _T("모드%d"), pstData->uiMode );
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("잘못된 명령[0x%x]입니다."), pstMessage->uiOpcode);
		break;
	}
}