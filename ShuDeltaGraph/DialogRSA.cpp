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
	DDX_Control(pDX, IDC_SPIN_PW_LOW, m_CSpinPWLow);
	DDX_Control(pDX, IDC_SPIN_PW_HGH, m_CSpinPWHgh);
	DDX_Control(pDX, IDC_SPIN_COL_TIME, m_CSpinColTime);
	DDX_Control(pDX, IDC_SPIN_COL_NUM, m_CSpinColNum);
	DDX_Control(pDX, IDC_SPIN_NUM, m_CSpinNum);
	DDX_Control(pDX, IDC_SPIN_MAG, m_CSpinMagThreshold);
	DDX_Control(pDX, IDC_SPIN_COR, m_CSpinCorThreshold);
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
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogRSA::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LIIST, &CDialogRSA::OnBnClickedButtonRemoveLiist)
	ON_BN_CLICKED(IDC_BUTTON_RXTHRESHOLD, &CDialogRSA::OnBnClickedButtonRxthreshold)
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

		m_pParentDlg->ClearRawDataList();

		m_pParentDlg->m_pConnected[enRSA]->InitVar();
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
			m_theThread.Attach( FuncColListRSA );
			m_theThread.Start( this );

			m_CButtonColStart.SetWindowText( _T("대기 취소") );
		}
		else {
			AfxMessageBox( _T("선택한 수집 과제가 없습니다 !") );
		}
	}
	else {
		//m_pParentDlg->m_pConnected[enRSA]->InitVar();
		InitListCtrl( false );

		MakeStopMessage( 0 );
		m_pParentDlg->Send( enRSA, m_ptxData );

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
DWORD WINAPI FuncColListRSA( LPVOID lpData )
{
	CDialogRSA *pDlg;

	STR_QUEUE_MSG stQueueMsg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDialogRSA * ) pParent->GetParam();

	pDlg->SetIBkColorOfColList( 0, 1 );

	pDlg->ReadyColStart( 0 );
	pDlg->MakeSetModeMessage( 0 );
	pDlg->m_pParentDlg->Send( enRSA, pDlg->m_ptxData );

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
				pDlg->m_pParentDlg->m_pConnected[enRSA]->LanMsg( false, & stQueueMsg );

				if( stQueueMsg.stMsg.uiOpcode != 0 ) {
					//TRACE( "\n 메시지 OpCode : %d", stQueueMsg.stMsg.uiOpcode );
					pDlg->ProcessColList( & stQueueMsg );
				}
				else {
					break;
				}
			} while( true );

		}
	}

	return 0;
}

/**
 * @brief		ReadyColStart
 * @param		UINT uiIndex
 * @return		void
 * @author		조철희 (churlhee.jo@lignex1.com)
 * @version		0.0.1
 * @date		2020/12/29 10:47:30
 * @warning		
 */
void CDialogRSA::ReadyColStart( UINT uiIndex )
{
	m_pRawData->uiItem = 0;

	memset( & m_stResCol, 0, sizeof(m_stResCol) );

	memcpy( & m_stColList, m_pColList+uiIndex, sizeof(STR_COL_LIST) );

}

void CDialogRSA::SetIBkColorOfColList( UINT uiIndex, int nStep )
{
	STR_COL_LIST *pColList;

	pColList = m_pColList + uiIndex;

	//TRACE( "\n 번호 : %d, 단계 : %d", uiIndex, nStep );

	if( nStep == -1 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(255, 255, 255) );
	else if( nStep <= 2 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 100, 0) );
	else if( nStep == 3 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 150, 0) );
	else if( nStep == 4 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 250, 0) );
	else 
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 0, 10) );

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
void CDialogRSA::ProcessColList( STR_QUEUE_MSG *pQueueMsg )
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

		//TRACE( "\n PDW 자동 과제[%d] 입니다.", m_uiColList );
		switch( pQueueMsg->stMsg.uiOpcode ) {
		case RES_SET_CONFIG :
			//TRACE( "\n RES_SET_CONFIG 처리 입니다." );
			SetIBkColorOfColList( m_uiColList, 2 );
			MakeColStartMessage();
			m_pParentDlg->Send( enRSA, m_ptxData );
			break;

		case RES_COL_START :
			//TRACE( "\n RES_COL_START 처리 입니다." );
			if( pQueueMsg->stData.stColStart.uiCoPulseNum != 0 ) {
				SetIBkColorOfColList( m_uiColList, 3 );

				MakeReqRawDataMessage();
				m_pParentDlg->Send( enRSA, m_ptxData );

				memcpy( & m_stResCol, & pQueueMsg->stData.stColStart, sizeof(STR_RES_COL_START) );
				m_pRawData->uiItem = 0;
			}
			else {
				SetIBkColorOfColList( m_uiColList, -1 );

				UpdateColList();

				ReadyColStart( m_uiColList );
				MakeSetModeMessage( m_uiColList );
				m_pParentDlg->Send( enRSA, m_ptxData );

				SetIBkColorOfColList( m_uiColList, 1 );
			}
			break;

		case RES_RAWDATA_PDW :
			m_pParentDlg->SetUnitID( enRSA );
			m_pParentDlg->InitUnitRes( enRSA );

			//TRACE( "\n RES_RAWDATA_PDW 처리 입니다." );
			memcpy( & m_pRawData->unRawData.stRSAPDWData[m_pRawData->uiItem], & pQueueMsg->stData.stRSAPDWData[0], sizeof(STR_RES_PDW_DATA_RSA)*PDW_BLOCK );
			m_pRawData->uiItem = ( m_pRawData->uiItem+PDW_BLOCK > m_stResCol.uiCoPulseNum ? m_stResCol.uiCoPulseNum : m_pRawData->uiItem+PDW_BLOCK );

			if( m_pRawData->uiItem >= m_stResCol.uiCoPulseNum ) {
				// PDW 수신판에서 강제 첫번재 PDW 제거함. PDW수신판의 첫번째 PDW TOA 오류로 제거함.
				-- m_pRawData->uiItem;
				memcpy( & m_pRawData->unRawData.stRSAPDWData[0], & m_pRawData->unRawData.stRSAPDWData[1], sizeof(STR_RES_PDW_DATA_RSA)*m_pRawData->uiItem );

				SetIBkColorOfColList( m_uiColList, 4 );

				m_pParentDlg->InsertPDWRawDataItem( & pQueueMsg->stData, m_pRawData->uiItem, m_uiColList, & m_stColList, m_pRawData, enRSA, & m_stResCol );
				m_pParentDlg->ViewGraph( pQueueMsg->stMsg.uiOpcode );

				SetIBkColorOfColList( m_uiColList, -1 );

				UpdateColList();

				ReadyColStart( m_uiColList );
				MakeSetModeMessage( m_uiColList );
				m_pParentDlg->Send( enRSA, m_ptxData );

				SetIBkColorOfColList( m_uiColList, 1 );

				TRACE( "\n 과제 번호 : %d", m_uiColList );
			}
			break;

		case RES_RAWDATA_ZPDW :
			m_pParentDlg->SetUnitID( enZPocketSonata );
			m_pParentDlg->SetBoardID( m_stResCol.uiBoardID );
			m_pParentDlg->InitUnitRes( enZPocketSonata );

			//TRACE( "\n RES_RAWDATA_PDW 처리 입니다." );
			memcpy( & m_pRawData->unRawData.stRSA2PDWData[m_pRawData->uiItem], & pQueueMsg->stData.stRSAPDWData[0], sizeof(UDRCPDW)*PDW_BLOCK );
			m_pRawData->uiItem = ( m_pRawData->uiItem+PDW_BLOCK > m_stResCol.uiCoPulseNum ? m_stResCol.uiCoPulseNum : m_pRawData->uiItem+PDW_BLOCK );

			if( m_pRawData->uiItem >= m_stResCol.uiCoPulseNum ) {
				// PDW 수신판에서 강제 첫번재 PDW 제거함. PDW수신판의 첫번째 PDW TOA 오류로 제거함.
				//-- m_RawData.uiItem;
				//memcpy( & m_RawData.unRawData.stRSA2PDWData[0], & m_RawData.unRawData.stRSA2PDWData[1], sizeof(UDRCPDW)*m_RawData.uiItem );

				SetIBkColorOfColList( m_uiColList, 4 );

				m_pParentDlg->InsertPDWRawDataItem( & pQueueMsg->stData, m_pRawData->uiItem, m_uiColList, & m_stColList, m_pRawData, enZPocketSonata, & m_stResCol );
				m_pParentDlg->ViewGraph( pQueueMsg->stMsg.uiOpcode );

				SetIBkColorOfColList( m_uiColList, -1 );

				UpdateColList();

				ReadyColStart( m_uiColList );
				MakeSetModeMessage( m_uiColList );
				m_pParentDlg->Send( enRSA, m_ptxData );

				SetIBkColorOfColList( m_uiColList, 1 );

				TRACE( "\n 과제 번호 : %d", m_uiColList );
			}
			break;

		default :
			TRACE( "\n 에러 처리 입니다." );
			break;
		}

	}
	else {

	}

}

void CDialogRSA::MakeReqRawDataMessage()
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
void CDialogRSA::MakeColStartMessage()
{
	STR_MESSAGE *pTxMessage;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_COL_START;
	pTxMessage->uiDataLength = 0;

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
void CDialogRSA::InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3 )
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
void CDialogRSA::MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
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
	case RES_RAWDATA_ZPDW:
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
void CDialogRSA::MakeStopMessage( UINT uiIndex )
{
	STR_MESSAGE *pTxMessage;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_STOP;
	pTxMessage->uiDataLength = 0;

}

void CDialogRSA::MakeSetModeMessage( UINT uiIndex )
{
	STR_COL_LIST *pColList;

	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_SET_CONFIG;
	pTxMessage->uiDataLength = sizeof(STR_REQ_SETMODE_RSA);

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );
	pColList = m_pColList + uiIndex;

	pTxData->stSetModeRSA.fAOALow = pColList->stColItem.fAOALow;
	pTxData->stSetModeRSA.fAOAHgh = pColList->stColItem.fAOAHgh;
	pTxData->stSetModeRSA.fFreqLow = pColList->stColItem.fFreqLow;
	pTxData->stSetModeRSA.fFreqHgh = pColList->stColItem.fFreqHgh;
	pTxData->stSetModeRSA.fPALow = pColList->stColItem.fPALow;
	pTxData->stSetModeRSA.fPAHgh = pColList->stColItem.fPAHgh;
	pTxData->stSetModeRSA.fPWLow = pColList->stColItem.fPWLow;
	pTxData->stSetModeRSA.fPWHgh = pColList->stColItem.fPWHgh;
	pTxData->stSetModeRSA.coPulseNum = pColList->stColItem.uiColNumber;
	pTxData->stSetModeRSA.fColTime = pColList->stColItem.fColTime;

}

void CDialogRSA::MakeRxThresholdMessage()
{
	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_RX_Threshold;
	pTxMessage->uiDataLength = sizeof(STR_RX_THRESHOLD);

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );

	pTxData->stRxThreshold.iBand = 0xFF;
	m_pParentDlg->m_stRxThreshold.uiMagThreshold = pTxData->stRxThreshold.uiMagThreshold = (unsigned int) m_CSpinMagThreshold.GetPos();	
	m_pParentDlg->m_stRxThreshold.uiCorThreshold = pTxData->stRxThreshold.uiCorThreshold = (unsigned int) m_CSpinCorThreshold.GetPos();

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

	pColList->stColItem.enMode = enModeUnknown;

	strTemp = m_ColList.GetItemText( iRow, 2 );
	swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & pColList->stColItem.fFreqLow, & pColList->stColItem.fFreqHgh );

	strTemp = m_ColList.GetItemText( iRow, 3 );
	swscanf_s( strTemp.GetBuffer(), _T("%d/%f"), & pColList->stColItem.uiColNumber, & pColList->stColItem.fColTime );

	strTemp = m_ColList.GetItemText( iRow, 4 );
	swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & pColList->stColItem.fPALow, & pColList->stColItem.fPAHgh );

	strTemp = m_ColList.GetItemText( iRow, 5 );
	swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & pColList->stColItem.fPWLow, & pColList->stColItem.fPWHgh );

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
	GetDlgItem(IDC_BUTTON_RXTHRESHOLD)->EnableWindow( bEnable );

}

void CDialogRSA::OnBnClickedButtonOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strPathName;

	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( FALSE );

	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

#ifdef EXAUTOMATION
	if( true == pApp->OpenFile( strPathName, _T("수집 목록 읽어오기..."), enOpenXLS ) ) {
		m_ColList.DeleteAllItems();
		OpenXLSViewList( strPathName );
	}
#elif defined(_EXCELLIB_)
#else
	if( true == pApp->OpenFile( strPathName, _T("수집 목록 읽어오기..."), enOpenCSV ) ) {
		m_ColList.DeleteAllItems();
		OpenXLSViewList( strPathName );
	}
#endif

	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( TRUE );
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
	m_CSpinAOALow.SetRangeAndDelta( 0, 359, 10.0 );
	m_CSpinAOALow.SetPos( (double) m_pParentDlg->m_stColItem.fAOALow );

	m_CSpinAOAHgh.SetDecimalPlaces(1);
	m_CSpinAOAHgh.SetTrimTrailingZeros(FALSE);
	m_CSpinAOALow.SetRangeAndDelta( 0, 359, 10.0 );
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
	m_CSpinPALow.SetRangeAndDelta( -70, 10, 1.0 );
	m_CSpinPALow.SetPos( (double) m_pParentDlg->m_stColItem.fPALow );

	m_CSpinPAHgh.SetDecimalPlaces(1);
	m_CSpinPAHgh.SetTrimTrailingZeros(FALSE);
	m_CSpinPAHgh.SetRangeAndDelta( -70, 10, 1.0 );
	m_CSpinPAHgh.SetPos( (double) m_pParentDlg->m_stColItem.fPAHgh );

	m_CSpinPWLow.SetDecimalPlaces(1);
	m_CSpinPWLow.SetTrimTrailingZeros(FALSE);
	m_CSpinPWLow.SetRangeAndDelta( 0, 1000000, 1.0 );
	m_CSpinPWLow.SetPos( (double) m_pParentDlg->m_stColItem.fPWLow );

	m_CSpinPWHgh.SetDecimalPlaces(1);
	m_CSpinPWHgh.SetTrimTrailingZeros(FALSE);
	m_CSpinPWHgh.SetRangeAndDelta( 0, 1000000, 1.0 );
	m_CSpinPWHgh.SetPos( (double) m_pParentDlg->m_stColItem.fPWHgh );

	m_CSpinColTime.SetDecimalPlaces(1);
	m_CSpinColTime.SetTrimTrailingZeros(FALSE);
	m_CSpinColTime.SetRangeAndDelta( 1000, 90000, 1000.0 );
	m_CSpinColTime.SetPos( (double) m_pParentDlg->m_stColItem.fColTime );

	m_CSpinMagThreshold.SetDecimalPlaces(0);
	m_CSpinMagThreshold.SetTrimTrailingZeros(FALSE);
	m_CSpinMagThreshold.SetRangeAndDelta( 0, 65536, 1.0 );
	m_CSpinMagThreshold.SetPos( (double) m_pParentDlg->m_stRxThreshold.uiMagThreshold );

	m_CSpinCorThreshold.SetDecimalPlaces(0);
	m_CSpinCorThreshold.SetTrimTrailingZeros(FALSE);
	m_CSpinCorThreshold.SetRangeAndDelta( 0, 65536, 1.0 );
	m_CSpinCorThreshold.SetPos( (double) m_pParentDlg->m_stRxThreshold.uiCorThreshold );

	CString strPathname = GetFilePath();
#ifdef EXAUTOMATION
	strPathname += _T("//수집 목록(레이더).xlsx");
#elif defined(_EXCELLIB_)
	strPathname += _T("//수집 목록(레이더).xlsx");
#else
	strPathname += _T("//수집 목록(레이더).csv");
#endif
	
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
		m_ColList.InsertColumn(i++, _T("세기 범위[dBm]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.15 ), -1);
		m_ColList.InsertColumn(i++, _T("펄스폭[ns]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.15 ), -1);
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
	
	m_ColList.DeleteAllItems();

#ifdef EXAUTOMATION
	long l, lMaxRow;
	float fValue1, fValue2;
	CString strNumber, strDOARange, strFreqRange, strColTime, strPARange, strPWRange;

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
		strPWRange = XL.GetCellValue( 6, l );

		strNumber.Replace( _T(".0"), _T("") );
		nIndex = m_ColList.InsertItem( INT_MAX, strNumber, NULL );

		//

		swscanf_s( strDOARange.GetBuffer(), _T("%f/%f"), & fValue1, &fValue2 );
		strDOARange.Format( _T("%.2f/%.2f"), fValue1, fValue2 );

		swscanf_s( strFreqRange.GetBuffer(), _T("%f/%f"), & fValue1, &fValue2 );
		strFreqRange.Format( _T("%.2f/%.2f"), fValue1, fValue2 );

		swscanf_s( strPARange.GetBuffer(), _T("%f/%f"), & fValue1, &fValue2 );
		strPARange.Format( _T("%.1f/%.1f"), fValue1, fValue2 );

		swscanf_s( strPWRange.GetBuffer(), _T("%f/%f"), & fValue1, &fValue2 );
		strPWRange.Format( _T("%.1f/%.1f"), fValue1, fValue2 );

		m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strDOARange, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strFreqRange, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 4, LVIF_TEXT, strPARange, NULL, NULL, NULL, NULL);
		m_ColList.SetItem( nIndex, 5, LVIF_TEXT, strPWRange, NULL, NULL, NULL, NULL);
	}

	XL.ReleaseExcel();
#elif defined(_EXCELLIB_)
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
#else
	FILE *pCSVFile;

	TCHAR szCheck[10], szNumber[100], szDOARange[100], szFreqRange[100], szColTime[100], szPARange[100], szPWRange[100];

	setlocale( LC_ALL, ".OCP" );
	if( 0 == _wfopen_s( & pCSVFile, T2W(strPathname.GetBuffer()), _T("rt") ) ) {
		TCHAR buffer[100];
		int iIndex, nIndex=0;

		fgetws( buffer, 100, pCSVFile );
		while( ! feof(pCSVFile) ) {
			fgetws( buffer, 100, pCSVFile );

			iIndex = 0;
			iIndex += GetStringInComma( szCheck, & buffer[iIndex] );
			//m_ColList.SetItemStates( nIndex, RC_ITEM_SELECTED );

			iIndex += GetStringInComma( szNumber, & buffer[iIndex] );
			nIndex = m_ColList.InsertItem( INT_MAX, szNumber, NULL );

			if( szCheck[0] == 'O' )
				m_ColList.SetItemStates( nIndex, RC_ITEM_CHECKED );
			else
				m_ColList.SetItemStates( nIndex, RC_ITEM_UNCHECKED );

			iIndex += GetStringInComma( szDOARange, & buffer[iIndex] );
			m_ColList.SetItem( nIndex, 1, LVIF_TEXT, szDOARange, NULL, NULL, NULL, NULL);

			iIndex += GetStringInComma( szFreqRange, & buffer[iIndex] );
			m_ColList.SetItem( nIndex, 2, LVIF_TEXT, szFreqRange, NULL, NULL, NULL, NULL);

			iIndex += GetStringInComma( szColTime, & buffer[iIndex] );
			m_ColList.SetItem( nIndex, 3, LVIF_TEXT, szColTime, NULL, NULL, NULL, NULL);

			iIndex += GetStringInComma( szPARange, & buffer[iIndex] );
			m_ColList.SetItem( nIndex, 4, LVIF_TEXT, szPARange, NULL, NULL, NULL, NULL);

			iIndex += GetStringInComma( szPWRange, & buffer[iIndex] );
			m_ColList.SetItem( nIndex, 5, LVIF_TEXT, szPWRange, NULL, NULL, NULL, NULL);
		}

		fclose( pCSVFile );
	}
	

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

		m_CSpinPWLow.SetPos( (double) stColList.stColItem.fPWLow );
		m_CSpinPWHgh.SetPos( (double) stColList.stColItem.fPWHgh );

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

	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetColList( & stColList );

	m_uiCoColList = m_ColList.GetItemCount();
	//
	//pColList = m_pColList + m_uiCoColList;
	stColList.stColItem.uiNo = GetNextNo();
	//memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	//
	strTemp.Format(_T("%d"), stColList.stColItem.uiNo );
	nIndex = m_ColList.InsertItem( INT_MAX, strTemp, NULL );

	//strTemp.Format(_T("%s"), g_stColListMode[pColList->stColItem.enMode] );
	strTemp.Format(_T("%.1f/%.1f"), stColList.stColItem.fAOALow, stColList.stColItem.fAOAHgh );
	m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.2f/%.2f"), stColList.stColItem.fFreqLow, stColList.stColItem.fFreqHgh );
	m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%d/%.1f"), stColList.stColItem.uiColNumber, stColList.stColItem.fColTime );
	m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f/%.1f"), stColList.stColItem.fPALow, stColList.stColItem.fPAHgh );
	m_ColList.SetItem( nIndex, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f/%.1f"), stColList.stColItem.fPWLow, stColList.stColItem.fPWHgh );
	m_ColList.SetItem( nIndex, 5, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

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
	pstColItem->fPWLow = (float) m_CSpinPWLow.GetPos();
	pstColItem->fPWHgh = (float) m_CSpinPWHgh.GetPos();
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
	CString strNumber, strMode, strCenterFreq, strColTime, strPA, strPW;

	STR_COL_LIST stColList;
	STR_COL_LIST *pColList;

	GetDlgItem(IDC_EDIT_NUM)->EnableWindow( FALSE );
	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetColList( & stColList );

	// 수집 목록 버퍼에 복사
	pColList = m_pColList + ( stColList.stColItem.uiNo - 1 );
	memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	// 목록창에 전시
	MakeColListString( & strNumber, & strMode, & strCenterFreq, & strColTime, & strPA, & strPW, & stColList );

	m_ColList.SetItem( m_iSelItem, 0, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 1, LVIF_TEXT, strMode, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 2, LVIF_TEXT, strCenterFreq, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 4, LVIF_TEXT, strPA, NULL, NULL, NULL, NULL);
	m_ColList.SetItem( m_iSelItem, 5, LVIF_TEXT, strPW, NULL, NULL, NULL, NULL);
}


void CDialogRSA::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strPathName;

	CString strNumber, strAOARange, strFreqRange, strColTime, strPARange, strPWRange;

	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_SAVE )->EnableWindow( FALSE );

	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

#ifdef EXAUTOMATION
	if( true == pApp->OpenFile( strPathName, _T("수집 목록 저장하기..."), enSaveXLS ) ) {
		int i;	

		STR_COL_LIST stColList;

		// 엑셀 수집 파일 로딩하기...
		CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

		XL.SetCellValue( 1, 1, _T("과제 번호") );
		XL.SetCellValue( 2, 1, _T("방위 범위[도]") ); 
		XL.SetCellValue( 3, 1, _T("주파수 범위[MHz]") ); 
		XL.SetCellValue( 4, 1, _T("수집 개수/시간[ms]") ); 
		XL.SetCellValue( 5, 1, _T("세기 범위[dBm]") ); 
		XL.SetCellValue( 6, 1, _T("기타") ); 

		for ( i = 0; i < m_ColList.GetItemCount(); i++) {
			if( TRUE == m_ColList.GetCheck(i) || true ) {
				GetColListFromList( i, & stColList );

				MakeColListString( & strNumber, & strAOARange, & strFreqRange, & strColTime, & strPARange, & stColList );

				XL.SetCellValue( 1, i+2, strNumber ); 
				XL.SetCellValue( 2, i+2, strAOARange ); 
				XL.SetCellValue( 3, i+2, strFreqRange ); 
				XL.SetCellValue( 4, i+2, strColTime ); 
				XL.SetCellValue( 5, i+2, strPARange ); 

			}
		}

		XL.SaveFileAs(strPathName);

		XL.ReleaseExcel();
#elif defined(_EXCELLIB_)

#else
		int i;

		STR_COL_LIST stColList;

		if( true == pApp->OpenFile( strPathName, _T("수집 목록 저장하기..."), enSaveCSV ) ) {
			FILE *pCSVFile;

			setlocale( LC_ALL, ".OCP" );
			if( 0 == _wfopen_s( &pCSVFile, T2W(strPathName.GetBuffer()), _T("wt") ) ) {
				//swprintf( buffer, sizeof(buffer),  );
				fwprintf_s( pCSVFile, _T("과제 번호,방위 범위[도],주파수 범위[MHz],수집 개수/시간[ms],세기 범위[dBm],기타" ) );

				for ( i = 0; i < m_ColList.GetItemCount(); i++) {
					GetColListFromList( i, & stColList );

					MakeColListString( & strNumber, & strAOARange, & strFreqRange, & strColTime, & strPARange, & strPWRange, & stColList );

					if( TRUE == m_ColList.GetCheck(i) ) {
						fwprintf_s( pCSVFile, _T("\nO,%s,%s,%s,%s,%s,%s," ), strNumber, strAOARange, strFreqRange, strColTime, strPARange, strPWRange );

					}
					else {
						fwprintf_s( pCSVFile, _T("\nX,%s,%s,%s,%s,%s,%s," ), strNumber, strAOARange, strFreqRange, strColTime, strPARange, strPWRange );
					}
				}

				fclose( pCSVFile );
			}

#endif

	}

	GetDlgItem( IDC_BUTTON_SAVE )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_OPEN )->EnableWindow( TRUE );
}

void CDialogRSA::MakeColListString( CString *pstrNum, CString *pstrAOARange, CString *pstrFreqRange, CString *pstrColTime, CString *pstrPARange, CString *pstrPWRange, STR_COL_LIST *pstColList )
{
	pstrNum->Format(_T("%d"), pstColList->stColItem.uiNo );

	pstrAOARange->Format(_T("%.1f/%1.f"), pstColList->stColItem.fAOALow, pstColList->stColItem.fAOAHgh );

	pstrFreqRange->Format(_T("%.2f/%.2f"), pstColList->stColItem.fFreqLow, pstColList->stColItem.fFreqHgh );

	pstrColTime->Format(_T("%d/%.1f"), pstColList->stColItem.uiColNumber, pstColList->stColItem.fColTime );

	pstrPARange->Format(_T("%.1f/%.1f"), pstColList->stColItem.fPALow, pstColList->stColItem.fPAHgh );

	pstrPWRange->Format(_T("%.1f/%.1f"), pstColList->stColItem.fPWLow, pstColList->stColItem.fPWHgh );


}

void CDialogRSA::OnBnClickedButtonRemoveLiist()
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

int CDialogRSA::GetNextNo()
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

void CDialogRSA::LogTxMessage( void *pData, CString *pStrEtc )
{
	CString strTemp1, strTemp2;

	MakeLogReqMessage( & strTemp1, & strTemp2, pData );

	//m_pTabThreatDialog->MakeLogTxMessage( & strTemp1, & strTemp2, pData, true );

	InsertItem( & strTemp1, & strTemp2, pStrEtc );

}

void CDialogRSA::MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
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
		pstrTemp2->Format( _T("%.1f-%.1f[도], %.1f-%.1f[MHz], %.1f-%.1f[dBm], %.1f-%.1f[ns], %d[개수], %.1f[ms]"), pstData->stSetModeRSA.fAOALow, pstData->stSetModeRSA.fAOAHgh, pstData->stSetModeRSA.fFreqLow, pstData->stSetModeRSA.fFreqHgh,
			pstData->stSetModeRSA.fPALow, pstData->stSetModeRSA.fPAHgh,
			pstData->stSetModeRSA.fPWLow, pstData->stSetModeRSA.fPWHgh,
			pstData->stSetModeRSA.coPulseNum , pstData->stSetModeRSA.fColTime );
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



BOOL CDialogRSA::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		return TRUE;
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogRSA::OnBnClickedButtonRxthreshold()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	MakeRxThresholdMessage();
	m_pParentDlg->Send( enRSA, m_ptxData );
}
