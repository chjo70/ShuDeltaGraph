// DlgColList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgColList.h"
#include "afxdialogex.h"




DWORD WINAPI FuncColList( LPVOID lpData );

TCHAR g_stColListMode[6][20] = { _T("IF2(광대역)"), _T("IF1 Course"), _T("IF1 Fine"), _T("광대역IQ"), _T("협대역IQ"), _T("널입니다.") } ;

TCHAR g_stRawListType[6][20] = { _T("PDW"), _T("IQ") } ;

#define WAIT_RESPONSE( A, B ) 	uiTry = 0;	\
								do {	\
									++ uiTry;	\
									Sleep( A );	\
									if( uiTry > MAX_WAIT_RESPONSE ) {	\
										break;	\
									}	\
								} while( B );

// CDlgColList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgColList, CDialogEx)

CDlgColList::CDlgColList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgColList::IDD, pParent)
{

}

CDlgColList::~CDlgColList()
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	FreeBuffer();

	pApp->SaveProfile( & m_stColItem );
}

void CDlgColList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COL_LIST, m_ColList);
	DDX_Control(pDX, IDC_LIST2_RAWDATA, m_RawList);
	DDX_Control(pDX, IDC_SPIN_FRQ_LOW, m_CSpinCenterFreq);
	DDX_Control(pDX, IDC_SPIN_FRQ_HIGH, m_CSpinThreshold);
	DDX_Control(pDX, IDC_SPIN_COL_TIME, m_CSpinColTime);
	DDX_Control(pDX, IDC_SPIN_COL_NUM, m_CSpinColNum);
	DDX_Control(pDX, IDC_SPIN_NUM, m_CSpinNum);
	DDX_Control(pDX, IDC_STATIC_TOTAL_COL_LIST, m_CStaticTotalColList);
	DDX_Control(pDX, IDC_BUTTON_INIT, m_CButtonInit);
	DDX_Control(pDX, IDC_BUTTON_SETMODE, m_CButtonSetMode);
	DDX_Control(pDX, IDC_BUTTON_COLSTART, m_CButtonColStart);
	DDX_Control(pDX, IDC_COMBO_MODE, m_CComboMode);
	DDX_Control(pDX, IDC_STATIC_TOTAL_RAW_LIST, m_CStaticTotalRawList);
}

void CDlgColList::InitVar()
{
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
 * @date      2020/02/01 10:07:51
 * @warning   
 */
void CDlgColList::InitBuffer()
{

	m_iSelItem = -1;

	m_pListener = new MyEchoSocket( false );
	m_pListener->SetParentDlg(this);

	m_pConnected = new MyEchoSocket( false );
	m_pConnected->SetParentDlg(this);

	m_ptxData = (char *) malloc(sizeof(char) * 100000);

	m_pColList = ( STR_COL_LIST *) malloc( sizeof(STR_COL_ITEM) * MAX_COL_ITEMS );

	m_pRawData = ( STR_RAW_DATA * ) malloc( sizeof(STR_RAW_DATA) );

	m_pSonataData = ( STR_SONATA_DATA * ) malloc( sizeof(STR_SONATA_DATA) );

	m_hReceveLAN = CreateEvent( NULL, TRUE, FALSE, NULL );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:08:58
 * @warning   
 */
void CDlgColList::FreeBuffer()
{

	delete m_pListener;
	delete m_pConnected;

	free( m_ptxData );
	free( m_pColList );
	free( m_pRawData );
	free( m_pSonataData );

	m_pTip->DestroyWindow();
	delete m_pTip;
}



BEGIN_MESSAGE_MAP(CDlgColList, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LIST, &CDlgColList::OnBnClickedButtonAddList)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDlgColList::OnHdnItemdblclickListColList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COL_LIST, &CDlgColList::OnDblclkListColList)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_LIST, &CDlgColList::OnBnClickedButtonModifyList)
	ON_BN_CLICKED(IDC_BUTTON_ALLSELECT, &CDlgColList::OnBnClickedButtonAllselect)
	ON_BN_CLICKED(IDC_BUTTON_SEL_DELETE, &CDlgColList::OnBnClickedButtonSelDelete)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_CHECKBOX, &CDlgColList::OnBnClickedButtonAllselCheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_UNCHECKBOX, &CDlgColList::OnBnClickedButtonAllselUncheckbox)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL_INVCHECKBOX, &CDlgColList::OnBnClickedButtonAllselInvcheckbox)
	ON_BN_CLICKED(IDC_BUTTON_COLSTART, &CDlgColList::OnBnClickedButtonColstart)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CDlgColList::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_SETMODE, &CDlgColList::OnBnClickedButtonSetmode)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON_OIPEN, &CDlgColList::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgColList::OnBnClickedButtonSave)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_RAWDATA, &CDlgColList::OnLvnItemActivateListRawdata)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDlgColList::OnHdnItemclickListRawdata)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RAWDATA, &CDlgColList::OnNMClickListRawdata)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_RAWDATA, &CDlgColList::OnLvnEndScrollListRawdata)
END_MESSAGE_MAP()


// CDlgColList 메시지 처리기입니다.

void CDlgColList::InitButton()
{

	InitButtonST( & m_CButtonInit );
	InitButtonST( & m_CButtonSetMode );

	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetDlgItem(IDC_EDIT_NUM)->EnableWindow( FALSE );

	SetControl( false );
}

void CDlgColList::InitButtonST( CButtonST *pCButtonRouteSetup )
{
	short	shBtnColor = 30;

	pCButtonRouteSetup->SetIcon( IDI_ICON_QUESTION, 24, 24, IDI_ICON_QUESTION, 24, 24);
	pCButtonRouteSetup->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

}

void CDlgColList::InitToolTip()
{
	m_pTip = new CXInfoTip;
	m_pTip->Create(this);
	m_pTip->SetIcon( m_hToolTipIcon );

}

void CDlgColList::InitStatic()
{
	m_CStaticTotalColList.SetTextColor(RGB(0,0,255), FALSE);
	m_CStaticTotalColList.SetBackgroundColor( ::GetSysColor(COLOR_WINDOW), FALSE);
	m_CStaticTotalColList.SetBold(TRUE, FALSE);
	//m_CStaticTotalColList.SetFont(_T("Comic Sans MS"), 20, FALSE);

	m_CStaticTotalRawList.SetTextColor(RGB(0,255,0), FALSE);
	m_CStaticTotalRawList.SetBackgroundColor(::GetSysColor(COLOR_WINDOW), FALSE);
	m_CStaticTotalRawList.SetBold(TRUE, FALSE);
	//m_CStaticTotalColList.SetFont(_T("Comic Sans MS"), 20, FALSE);
}

#define NUM_OF_STATUSBAR			(6)
/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:38:01
 * @warning   
 */
void CDlgColList::InitStatusBar()
{
	int i;
	int strPartDim[NUM_OF_STATUSBAR];

	CRect rectDlg;

	this->GetWindowRect( rectDlg );

	m_StatusBar.Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0, 0, 0, 0), this, 0 );

	int iWidth = rectDlg.Width() / NUM_OF_STATUSBAR;
	for( i=0 ; i < NUM_OF_STATUSBAR ; ++i ) {
		//strPartDim[i] = { 200, 400, 600, 800, 1000, 1200, 1400 };
		strPartDim[i] = iWidth * (i+1);
	}

	m_StatusBar.SetParts( NUM_OF_STATUSBAR, strPartDim );

}

/**
 * @brief     
 * @return    BOOL
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:58
 * @warning   
 */
BOOL CDlgColList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_COL_LIST));
	this->SetIcon(hIcon, FALSE);

	Log( enNormal, _T("CDlgColList+++++++++++++++++++++++++++++++++++++++++++") );

	InitVar();
	InitButton();
	InitBuffer();
	InitStatusBar();
	InitListCtrl();
	InitStatic();
	InitToolTip();

	CreateDir( RAWDATA_DIRECTORY );

	LoadColList();

	pApp->LoadProfile( & m_stColItem );

	// 스핀 카트롤 세팅
	m_CSpinNum.SetDecimalPlaces(0);
	m_CSpinNum.SetTrimTrailingZeros(FALSE);
	m_CSpinNum.SetRangeAndDelta( 1, 1000, 1.0 );
	m_CSpinNum.SetPos( (double) 1 );

	m_CSpinCenterFreq.SetDecimalPlaces(1);
	m_CSpinCenterFreq.SetTrimTrailingZeros(FALSE);
	m_CSpinCenterFreq.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinCenterFreq.SetPos( (double) m_stColItem.fCenterFreq );

	m_CSpinThreshold.SetDecimalPlaces(1);
	m_CSpinThreshold.SetTrimTrailingZeros(FALSE);
	m_CSpinThreshold.SetRangeAndDelta( -60, 0, 1.0 );
	m_CSpinThreshold.SetPos( (double) m_stColItem.fThreshold );

	m_CSpinColTime.SetDecimalPlaces(1);
	m_CSpinColTime.SetTrimTrailingZeros(FALSE);
	m_CSpinColTime.SetRangeAndDelta( 100, 100000, 1.0 );
	m_CSpinColTime.SetPos( (double) m_stColItem.fColTime );

	m_CSpinColNum.SetDecimalPlaces(0);
	m_CSpinColNum.SetTrimTrailingZeros(FALSE);
	m_CSpinColNum.SetRangeAndDelta( 10, 1000, 1.0 );
	m_CSpinColNum.SetPos( (double) m_stColItem.uiColNumber );

	m_CComboMode.AddString( _T("IF2(광대역)" ) );
	m_CComboMode.AddString( _T("IF1(Course)" ) );
	m_CComboMode.AddString( _T("IF1(FINE)" ) );
	m_CComboMode.AddString( _T("광대역(IQ)" ) );
	m_CComboMode.AddString( _T("협대역(IQ)" ) );
	m_CComboMode.SetCurSel( m_stColItem.enMode );

	InitSocketSetting();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgColList::LoadColList()
{
	m_uiCoColList = 0;
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:55
 * @warning   
 */
void CDlgColList::OnAccept()
{
	CString strIP;
	UINT port;

	if ( m_pListener->Accept( *m_pConnected )) {
		CString strMsg;

		m_pListener->Close();

		m_pConnected->GetSockName(strIP, port);
		//m_status = "Client Connected,IP :" + strIP;
		//m_sConnected.Send("Connected To Server", strlen("Connected To Server"));
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(FALSE);
		strMsg.Format( _T("연결:%s/%d"), (char*)(LPCTSTR) strIP, port);
		m_StatusBar.SetText( strMsg, 0, 0);
		m_StatusBar.SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );

		Log( enNormal, _T("랜 연결(%s/%d) 되었습니다."), (char*)(LPCTSTR) strIP, port);
		
		m_enConnectMode = CONNECTED;
		UpdateData(FALSE);

		SetControl( true );

		//Connect();

		//CALL_DIALOG( enUnit, OnConnect() );
	}
	else
	{
		Log( enError, _T("연결이 된 상태인데 Accept 가 들어옵니다." ) );
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(TRUE);
		//AfxMessageBox( _T("Cannoot Accept Connection") );
	}

}

void CDlgColList::SetControl( bool bEnable )
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
 * @date      2020/02/01 13:37:52
 * @warning   
 */
void CDlgColList::OnClose()
{

	Log( enError, _T("소켓을 닫습니다.") );

	InitVar();

	m_pListener->Close();
	m_pListener->ShutDown();

	m_pConnected->InitVar();
	m_pConnected->Close();

	m_StatusBar.SetText(_T("대기 상태"), 0, 0);
	m_StatusBar.SetBkColor( GetSysColor(CTLCOLOR_DLG) );

	//AfxMessageBox(IDS_STRING_LANCLOSE, MB_OK );

	//OnBnClickedButtonServerrun();
	SetControl( false );

	InitSocketSetting();

	m_theThread.Stop( true );

	m_CButtonColStart.SetWindowText( _T("수집 시작") );

	//CALL_DIALOG( enUnit, OnClose() );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:10:36
 * @warning   
 */
void CDlgColList::InitSocketSetting()
{
	UINT uiPortNum;
	CString strPortNum;
													  
	CString strIPAddress=GetIpAddress();

	uiPortNum = SHU_PORT_NUM;

	Log( enNormal, _T("[%s/%d]는 서버로 실행합니다."), strIPAddress, uiPortNum );

	m_pListener->Create( uiPortNum, SOCK_STREAM);
	m_pListener->Listen();

	m_StatusBar.SetText(_T("대기 상태"), (int) 0, 0);

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:19:22
 * @warning   
 */
void CDlgColList::InitThread()
{
	m_theThread.Attach( FuncColList );
	m_theThread.Start( this );
}

/**
 * @brief     
 * @param     int nErrorCode
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:03:20
 * @warning   
 */
void CDlgColList::OnConnect(int nErrorCode )
{
	TCHAR szBuffer[100];

	if (nErrorCode != 0) {
		//AfxMessageBox(_T("서버와 연결할 수 없습니다 !!!"));
		m_StatusBar.SetText(_T("대기 상태"), 0, 0);
	}
	else {
		UINT uiPortNum;
		CString strIPAddress;

		m_pConnected->GetPeerName( strIPAddress, uiPortNum );

		//Log( enNormal, "%s/%d 에 연결되었습니다.", strIPAddress, uiPortNum );

		wprintf_s( szBuffer, sizeof(szBuffer), _T("연결[%s/%d]"), strIPAddress, uiPortNum );
		m_StatusBar.SetText(szBuffer, 0, 0);

		//GetDlgItem(IDC_BUTTON_CLIENT)->SetWindowText(_T("서버 연결끊기"));

		//CALL_DIALOG( enUnit, OnConnect() );

	}
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:10:30
 * @warning   
 */
void CDlgColList::OnReceive( char *pData )
{
	CString strTemp1, strTemp2;

	MakeLogResMessage( & strTemp1, & strTemp2, (void *) pData );

	UpdateResultData( pData );

	InsertItem( & strTemp1, & strTemp2 );

}

void CDlgColList::ProcessColList( STR_QUEUE_MSG *pQueueMsg )
{
	STR_COL_LIST *pColList;
	CString strTemp1, strTemp2;

	// 수신한 메시지 출력
	MakeLogResMessage( & strTemp1, & strTemp2, pQueueMsg );
	InsertItem( & strTemp1, & strTemp2 );

	Sleep( 100 );

	//
	if( m_enMode == enColList_MODE ) {
		pColList = m_pColList + m_uiColList;
		if( pColList->stColItem.enMode >= enIQ_WIDE ) {
			TRACE( "\n IQ 자동 과제 입니다.");
			switch( pQueueMsg->stMsg.uiOpcode ) {
				case RES_SET_CONFIG :
					SetIBkColorOfColList( m_uiColList, 2 );
					MakeIQMessage( m_uiColList );
					Send();
					break;

				case RES_COL_START :
					SetIBkColorOfColList( m_uiColList, 3 );

					m_uiColList = (m_uiCoColList-1) <= m_uiColList ? 0 : ++m_uiColList;

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
					Send();
					break;

				case RES_COL_START :
					//TRACE( "\n RES_COL_START 처리 입니다." );
					SetIBkColorOfColList( m_uiColList, 3 );
					MakeReqRawDataMessage();
					Send();

					memcpy( & m_stResCol, & pQueueMsg->stData.stColStart, sizeof(STR_RES_COL_START) );
					m_pRawData->uiItem = 0;
					break;

				case RES_RAWDATA_PDW :
					//TRACE( "\n RES_RAWDATA_PDW 처리 입니다." );
					SetIBkColorOfColList( m_uiColList, 4 );

					memcpy( & m_pRawData->unRawData.stPDWData[m_pRawData->uiItem], & pQueueMsg->stData.stPDWData[0], sizeof(STR_RES_PDW_DATA)*30 );
					m_pRawData->uiItem = ( m_pRawData->uiItem+30 > m_stResCol.uiCoPulseNum ? m_stResCol.uiCoPulseNum : m_pRawData->uiItem+30 );

					if( m_pRawData->uiItem >= m_stResCol.uiCoPulseNum ) {
						InsertPDWRawDataItem( & pQueueMsg->stData, m_pRawData->uiItem  );
						ViewGraph();

						ReadyColStart( m_uiColList );
						MakeSetModeMessage( m_uiColList );
						Send();

						SetIBkColorOfColList( m_uiColList, -1 );

						m_uiColList = (m_uiCoColList-1) <= m_uiColList ? 0 : ++m_uiColList;

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

#define INIT_CODE_BYTE				(0xCC)
#define INIT_CODE_WORD				(0xCCCCCCCC)
#define MAX_WAIT_RESPONSE			(1000)
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
	UINT uiIndex=MAX_COL_ITEMS;
	CDlgColList *pDlg;

	STR_QUEUE_MSG stQueueMsg;
	queue<STR_QUEUE_MSG> *pQueueMsg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDlgColList * ) pParent->GetParam();

	pQueueMsg = pDlg->GetQueueMessage();

	pDlg->SetIBkColorOfColList( uiIndex, -1 );
	uiIndex = (pDlg->m_uiCoColList-1) <= uiIndex ? 0 : ++uiIndex;

	pDlg->ReadyColStart( uiIndex );
	pDlg->MakeSetModeMessage( uiIndex );
	pDlg->Send();

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
			while( ! pQueueMsg->empty() ) {
				stQueueMsg = pQueueMsg->front();
				pQueueMsg->pop();

				pDlg->ProcessColList( & stQueueMsg );
			}

		}
	}

	return 0;
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 23:33:28
 * @warning   
 */
void CDlgColList::MakeColStartMessage()
{
	STR_MESSAGE *pTxMessage;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = REQ_COL_START;
	pTxMessage->uiDataLength = 0;

}

void CDlgColList::MakeReqRawDataMessage()
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
 * @date      2020/02/03 23:15:54
 * @warning   
 */
void CDlgColList::MakeSetModeMessage( UINT uiIndex )
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

void CDlgColList::MakeIQMessage( UINT uiIndex )
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
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 15:44:24
 * @warning   
 */
void CDlgColList::InitListCtrl( bool bInit )
{
	CRect rt;

	if( bInit == true ) {
		m_ColList.GetWindowRect(&rt);
		m_ColList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

		m_ColList.InsertColumn(0, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.12), -1 );
		m_ColList.InsertColumn(1, _T("모드"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2) , -1);
		m_ColList.InsertColumn(2, _T("중심 주파수[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2) , -1);
		m_ColList.InsertColumn(3, _T("수집 개수/시간[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.25), -1);
		m_ColList.InsertColumn(4, _T("임계값[dBm]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.15 ), -1);
		m_ColList.InsertColumn(5, _T("기타"), LVCFMT_LEFT, (int) (rt.Width() * 0.07), -1);

		m_ColList.SetGridLines(TRUE);
		m_ColList.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
		//m_ColList.SetCheckboxes(TRUE);

		m_RawList.GetWindowRect(&rt);
		m_RawList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

		m_RawList.InsertColumn(0, _T("순번"), LVCFMT_LEFT, (int) ( rt.Width()*0.10), -1 );
		m_RawList.InsertColumn(1, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.10), -1 );
		m_RawList.InsertColumn(2, _T("종류"), LVCFMT_LEFT, (int) ( rt.Width() * 0.10) , -1);
		m_RawList.InsertColumn(3, _T("모드"), LVCFMT_LEFT, (int) ( rt.Width() * 0.10) , -1);
		m_RawList.InsertColumn(4, _T("주파수 범위[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2 ), -1);
		m_RawList.InsertColumn(5, _T("수집 개수/시간[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2 ), -1);
		m_RawList.InsertColumn(6, _T("파일명"), LVCFMT_LEFT, (int) (rt.Width() * 0.19), -1);

		m_RawList.SetGridLines(TRUE);
		//m_CListRawData.SetCheckboxes(TRUE);
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
 * @param     STR_COL_LIST * pstColList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 18:23:00
 * @warning   
 */
void CDlgColList::GetColList( STR_COL_LIST *pstColList )
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
 * @date      2020/02/03 22:02:44
 * @warning   
 */
void CDlgColList::GetColItem( STR_COL_ITEM *pstColItem )
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
 * @date      2020/02/01 20:55:07
 * @warning   
 */
void CDlgColList::PutColList( STR_COL_LIST *pstColList )
{
	pstColList->stColItem.uiNo = (UINT) m_CSpinNum.GetPos();
	pstColList->stColItem.fCenterFreq = (float) m_CSpinCenterFreq.GetPos();
	pstColList->stColItem.fThreshold = (float) m_CSpinThreshold.GetPos();
	pstColList->stColItem.fColTime = (float) m_CSpinColTime.GetPos();
	pstColList->stColItem.uiColNumber = (UINT) m_CSpinColNum.GetPos();
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 18:22:57
 * @warning   
 */
void CDlgColList::OnBnClickedButtonAddList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex;
	CString strTemp;

	STR_COL_LIST stColList;
	STR_COL_LIST *pColList;

	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetColList( & stColList );

	//
	pColList = m_pColList + m_uiCoColList;
	stColList.stColItem.uiNo = m_uiCoColList + 1;
	memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	//
	strTemp.Format(_T("%d"), pColList->stColItem.uiNo );
 	nIndex = m_ColList.InsertItem( INT_MAX, strTemp, NULL );

	strTemp.Format(_T("%s"), g_stColListMode[pColList->stColItem.enMode] );
	m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), pColList->stColItem.fCenterFreq );
 	m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%d/%.1f"), pColList->stColItem.uiColNumber, pColList->stColItem.fColTime );
	m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f"), pColList->stColItem.fThreshold );
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
 * @date      2020/02/03 22:02:51
 * @warning   
 */
void CDlgColList::SetTotalColList()
{
	CString strTemp;

	strTemp.Format( _T("%d"), m_ColList.GetItemCount() );
	m_CStaticTotalColList.SetWindowText( strTemp );

	//Invalidate();

	//m_CStaticTotalColList.SetTextColor(RGB(0,0,255), FALSE);
	//m_CStaticTotalColList.SetBackgroundColor(RGB(255,255,255), FALSE);
	//m_CStaticTotalColList.SetBold(TRUE, FALSE);

}

void CDlgColList::SetTotalRawList()
{
	CString strTemp;

	strTemp.Format( _T("%d"), m_RawList.GetItemCount() );
	m_CStaticTotalRawList.SetWindowText( strTemp );

	//Invalidate();

	//m_CStaticTotalColList.SetTextColor(RGB(0,0,255), FALSE);
	//m_CStaticTotalColList.SetBackgroundColor(RGB(255,255,255), FALSE);
	//m_CStaticTotalColList.SetBold(TRUE, FALSE);

}


/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:02:54
 * @warning   
 */
void CDlgColList::OnHdnItemdblclickListColList(NMHDR *pNMHDR, LRESULT *pResult)
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
 * @date      2020/02/01 18:22:53
 * @warning   
 */
void CDlgColList::OnDblclkListColList(NMHDR *pNMHDR, LRESULT *pResult)
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

void CDlgColList::GetColListFromList( int iRow, STR_COL_LIST *pColList )
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
 * @date      2020/02/01 18:23:06
 * @warning   
 */
void CDlgColList::OnBnClickedButtonModifyList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

void CDlgColList::MakeColListString( CString *pstrNum, CString *pstrMode, CString *pstrCenterFreq, CString *pstrColTime, CString *pstrThreshold, STR_COL_LIST *pstColList )
{
	pstrNum->Format(_T("%d"), pstColList->stColItem.uiNo );

	pstrMode->Format(_T("%s"), g_stColListMode[pstColList->stColItem.enMode] );

	pstrCenterFreq->Format(_T("%.1f"), pstColList->stColItem.fCenterFreq );

	pstrColTime->Format(_T("%d/%.1f"), pstColList->stColItem.uiColNumber, pstColList->stColItem.fColTime );

	pstrThreshold->Format(_T("%.1f"), pstColList->stColItem.fThreshold );


}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:02
 * @warning   
 */
void CDlgColList::OnBnClickedButtonAllselect()
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
 * @date      2020/02/01 22:15:41
 * @warning   
 */
void CDlgColList::OnBnClickedButtonSelDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	//m_ColList.DeleteAllSelectedItems(ItemdataProc, (LPARAM)this);

	SetTotalColList();

}

/**
 * @brief     
 * @param     DWORD dwData
 * @param     LPARAM lParam
 * @return    BOOL
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/02 11:01:49
 * @warning   
 */
BOOL CDlgColList::ItemdataProc(DWORD dwData, LPARAM lParam)
{
	// TODO: Process your item data here

	// Please return TRUE to proceed the deletion, return FALSE to abort.
	return TRUE;
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/02 11:01:53
 * @warning   
 */
void CDlgColList::OnBnClickedButtonAllselCheckbox()
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
 * @date      2020/02/02 11:01:43
 * @warning   
 */
void CDlgColList::OnBnClickedButtonAllselUncheckbox()
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
 * @date      2020/02/02 11:01:34
 * @warning   
 */
void CDlgColList::OnBnClickedButtonAllselInvcheckbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ColList.InvertItems(RC_INVERT_CHECKMARK);
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/02 11:16:00
 * @warning   
 */
void CDlgColList::OnBnClickedButtonColstart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTitle;
	STR_COL_LIST *pColList;
	STR_COL_LIST stColList;

	m_enMode = enColList_MODE;

	m_CButtonColStart.GetWindowText( strTitle );

	if( strTitle.Compare( _T("수집 시작") ) == 0 ) {
		Log( enNormal, _T("수집 시작을 했습니다." ) );

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
		m_pConnected->InitVar();
		InitListCtrl( false );
		
		Log( enNormal, _T("수집 시작을 취소했습니다." ) );
		m_theThread.Stop( true );

		ActivateGraph( FALSE );


		m_CButtonColStart.SetWindowText( _T("수집 시작") );
	}

}

void CDlgColList::ActivateGraph( BOOL bEnable )
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	pApp->ActivateGraph( bEnable );
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:14
 * @warning   
 */
void CDlgColList::Send()
{
	STR_MESSAGE *pTxMessage;

	m_pConnected->Send( m_ptxData, sizeof(STR_MESSAGE) );

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	if( pTxMessage->uiDataLength != 0 ) {
		m_pConnected->Send( & m_ptxData[sizeof(STR_MESSAGE)], pTxMessage->uiDataLength );
	}

	LogTxMessage( m_ptxData );

	m_bCompleteOnReceive = false;

}

/**
 * @brief     
 * @param     void * pData
 * @param     CString * pStrEtc
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:17
 * @warning   
 */
void CDlgColList::LogTxMessage( void *pData, CString *pStrEtc )
{
	CString strTemp1, strTemp2;

	MakeLogReqMessage( & strTemp1, & strTemp2, pData );

	//m_pTabThreatDialog->MakeLogTxMessage( & strTemp1, & strTemp2, pData, true );

	InsertItem( & strTemp1, & strTemp2, pStrEtc );

}

/**
 * @brief     
 * @param     CString * pstrTemp1
 * @param     CString * pstrTemp2
 * @param     void * pData
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:19
 * @warning   
 */
void CDlgColList::MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
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
void CDlgColList::MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
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
		break;

	case RES_RAWDATA_INTRA:
		*pstrTemp1 = _T(">>FMOP 데이터");
		break;

	case RES_IQ_DATA :
		pstrTemp1->Format( _T(">>IQ 데이터[%d]"), pstMessage->uiDataLength );
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("잘못된 명령[0x%x]입니다."), pstMessage->uiOpcode);
		break;
	}
}

void CDlgColList::InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3 )
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
 
 	Log( enNormal, _T("%d\t%s\t%s") , m_uiLog, *pStrTemp1, *pStrTemp2 );

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
 * @param     char * pData
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:26
 * @warning   
 */
void CDlgColList::UpdateResultData( char *pData )
{
	STR_MESSAGE *pstMessage;
	STR_DATA_CONTENTS *pstData;

	int iCnt=0;
	TCHAR szBuffer[500];

	pstMessage = (STR_MESSAGE *) pData;
	pstData = (STR_DATA_CONTENTS * ) ( ( char *) pData + sizeof(STR_MESSAGE) );

	switch (pstMessage->uiOpcode) {
	case RES_INIT:
		if( pstData->stResInit.uiReqCode == 0 && pstData->stResInit.uiErrorCode == 0 )
			m_CButtonInit.SetIcon( IDI_ICON_OK, 24, 24, IDI_ICON_OK, 24, 24);
		else
			m_CButtonInit.SetIcon( IDI_ICON_ERROR, 24, 24, IDI_ICON_ERROR, 24, 24);

		iCnt += wsprintf( & szBuffer[0], _T("Opcode[0x%x], ErrorCode[0x%x]\n") , pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
		UpdateToolTip( szBuffer, & m_CButtonInit );
		break;

	case RES_SET_CONFIG :
		break;

	case RES_COL_START :
		memcpy( & m_stResCol, pstData, sizeof(STR_RES_COL_START) );
		break;

	case RES_RAWDATA_PDW :
		InsertPDWRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_PDW_DATA) );
		break;

	case RES_RAWDATA_INTRA :
		InsertIntraRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_INTRA_DATA) );
		break;

	case RES_IQ_DATA:
		InsertIQRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_IQ_DATA) );
		break;

	default :
		break;
	}
}

/**
 * @brief     
 * @param     TCHAR * pszBuffer
 * @param     CWnd * pDlgItem
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:34
 * @warning   
 */
void CDlgColList::UpdateToolTip( TCHAR *pszBuffer, CWnd *pDlgItem  )
{
	m_pTip->Show( pszBuffer );
	m_pTip->AddTool( pDlgItem, pszBuffer, m_hToolTipIcon );

}

/**
 * @brief     
 * @param     MSG * pMsg
 * @return    BOOL
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:31
 * @warning   
 */
BOOL CDlgColList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) 
		return TRUE;
	else
		m_pTip->RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}


/**
  * @brief		
  * @return 	void
  * @date			2020/02/03 16:38:54
*/
void CDlgColList::OnBnClickedButtonInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_pListener != NULL && m_pListener->m_bConnected == true ) {
		STR_MESSAGE *pTxMessage;
		STR_DATA_CONTENTS *pTxData;

		pTxMessage = (STR_MESSAGE * ) m_ptxData;
		pTxMessage->uiOpcode = REQ_INIT;
		pTxMessage->uiDataLength = sizeof(STR_RES_INIT);

		pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );

		pTxData->stResInit.uiReqCode = 0;
		pTxData->stResInit.uiErrorCode = 0;

		Send();
	}
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 21:51:45
 * @warning   
 */
void CDlgColList::OnBnClickedButtonSetmode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_pListener != NULL && m_pListener->m_bConnected == true ) {
		STR_MESSAGE *pTxMessage;
		STR_DATA_CONTENTS *pTxData;

		pTxMessage = (STR_MESSAGE * ) m_ptxData;
		pTxMessage->uiOpcode = REQ_SETMODE;
		pTxMessage->uiDataLength = 0;

		pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );

		Send();

	}

	OnClose();

}


/**
 * @brief     
 * @param     UINT nID
 * @param     LPARAM lParam
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 21:51:39
 * @warning   
 */
void CDlgColList::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nID == SC_CLOSE)	
	{
		GetColItem( & m_stColItem );
		//종료버튼 눌릴 시
	}
	else if(nID == SC_MAXIMIZE)
	{ 
		//최대화 버튼 눌릴 시
	}
	else if(nID == SC_MINIMIZE)
	{ 
		//최소화 버튼 눌릴 시
	}
	else if(nID == SC_RESTORE) 
	{ 
		//복원 상황에서
	} 
	CDialogEx::OnSysCommand(nID, lParam);
}

queue <STR_QUEUE_MSG> *CDlgColList::GetQueueMessage()
{
	queue <STR_QUEUE_MSG> *pQueueMsg;

	pQueueMsg = m_pConnected->GetQueueMessage();

	return pQueueMsg;

}

// STR_DATA_CONTENTS *CDlgColList::GetRxData()
// {
// 	STR_DATA_CONTENTS *pRxData;
// 
// 	pRxData = m_pConnected->GetRxData();
// 
// 	return pRxData;
// }
// 
// STR_MESSAGE *CDlgColList::GetRxMessage()
// {
// 	STR_MESSAGE *pRxData;
// 
// 	pRxData = m_pConnected->GetRxMessage();
// 
// 	return pRxData;
// }

void CDlgColList::SetIBkColorOfColList( UINT uiIndex, int nStep )
{
	STR_COL_LIST *pColList;

	pColList = m_pColList + uiIndex;

	if( nStep == -1 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(255, 255, 255) );
	else if( nStep == 0 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 50, 0) );
	else if( nStep == 1 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 100, 0) );
	else if( nStep == 2 )
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 250, 0) );
	else 
		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(250, 0, 0) );

}

void CDlgColList::ReadyColStart( UINT uiIndex )
{
	m_pRawData->uiItem = 0;

	memset( & m_stResCol, 0, sizeof(m_stResCol) );

	memcpy( & m_stColList, m_pColList+uiIndex, sizeof(STR_COL_LIST) );

}

void CDlgColList::InsertIntraRawDataItem( STR_DATA_CONTENTS *pstData, int iItem )
{
	Log( enNormal, _T("\n 인트라 데이터를 처리합니다." ) );

}

void CDlgColList::ViewGraph()
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	SetEvent( pApp->m_pDlg2DHisto->m_hHisto[en_ReceiveData] );
	SetEvent( pApp->m_pDlgMulti->m_hMulti );

}

void CDlgColList::InsertPDWRawDataItem( STR_DATA_CONTENTS *pstData, int iItem )
{
	int nIndex;
	CString strTemp;

// 	if( m_pRawData->uiItem <= m_stResCol.uiCoPulseNum ) {
// 		Log( enNormal, _T("%d 개의 PDW 데이터를 처리합니다.") , iItem );
// 		memcpy( & m_pRawData->unRawData.stPDWData[m_pRawData->uiItem ], & pstData->stPDWData, sizeof(STR_RES_PDW_DATA)*iItem );
// 
// 		m_pRawData->uiItem += iItem;
// 	}
// 
// 	if( m_pRawData->uiItem >= m_stResCol.uiCoPulseNum ) {
// 		// 수퍼헷에서 30개씩 개수로 들어오기 때문에 수신한 개수는 수집 시작 완료 응답의 수집 개수로 강제 설정
// 		m_pRawData->uiItem = m_stResCol.uiCoPulseNum;

	Log( enNormal, _T("PDW 데이터를 완료 처리합니다." ) );

	// 데이터를 체계용 PDW 파일로 변환하여 저장합니다.
	MakePDWFile( m_pRawData->uiItem );

	// 목록창 추가
 	strTemp.Format(_T("%d"), m_uiCoRawData );
 	nIndex = m_RawList.InsertItem( INT_MAX, strTemp, NULL );

	strTemp.Format(_T("%d"), m_stColList.stColItem.uiNo );
	m_RawList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp = g_stRawListType[enPDW_DataType];
	m_RawList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%s"), g_stColListMode[m_stColList.stColItem.enMode] );
	m_RawList.SetItem( nIndex, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), m_stColList.stColItem.fCenterFreq );
	m_RawList.SetItem( nIndex, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%d/%.1f"), m_stColList.stColItem.uiColNumber, m_stColList.stColItem.fColTime ); 
	m_RawList.SetItem( nIndex, 5, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	m_RawList.SetItem( nIndex, 6, LVIF_TEXT, m_strFilename, NULL, NULL, NULL, NULL);

	if( m_bClickedOfRawDataList == false ) {
		m_RawList.SetItemState( -1, 0, LVIS_SELECTED|LVIS_FOCUSED );
		m_RawList.SetItemState( nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_RawList.EnsureVisible( nIndex, FALSE); 
	}

	SetTotalRawList();

	++ m_uiCoRawData;

}

void CDlgColList::MakePDWFile( int iItem )
{
	CString strPathname, strFolderName;
	SYSTEMTIME cur_time;

	GetLocalTime( & cur_time );

	// 폴더 생성
	strFolderName.Format( _T("%s//%04d_%02d_%02d//PDW"), RAWDATA_DIRECTORY, cur_time.wYear, cur_time.wMonth, cur_time.wDay ); 
	CreateDir( (TCHAR*) (LPCTSTR) strFolderName );

	m_strFilename.Format( _T("%04d_%02d_%02d %02d_%02d_%02d_%03ld.pdw" ), cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds );

	strPathname = strFolderName + _T("//") + m_strFilename;

	ConvertRAWData( iItem, en_PDW_DATA );

	//m_theDataFile.SaveDataFile( strPathname, (void *) m_pSonataData->unRawData.stPDWData, iItem*sizeof(TNEW_PDW), en_SONATA, en_PDW_DATA );

}



void CDlgColList::ConvertRAWData( int iItem, ENUM_DataType enDataType )
{
	int i;

	TNEW_PDW *pNEW_PDW;
	STR_RES_PDW_DATA *pPDWData;

	int iBc=0;
	UINT uiTOA, uiFreq, uiPW;

	switch( enDataType ) {
		case en_PDW_DATA :
			pPDWData = & m_pRawData->unRawData.stPDWData[0];
			pNEW_PDW = & m_pSonataData->unRawData.stPDWData[0];
			memset( pNEW_PDW, 0, sizeof(TNEW_PDW) * iItem );
			for( i=0 ; i < iItem ; ++i ) {
				pNEW_PDW->item.stat = PDW_NORMAL;

				pNEW_PDW->item.pmop = 0;
				pNEW_PDW->item.freq_diff = 0;

				pNEW_PDW->item.direction_h = 0;
				pNEW_PDW->item.direction_l = 0;

				// TOA 설정
				uiTOA = pPDWData->uiTOA;			// IDIV( pPDWData->uiTOA * 5, 2 );
				pNEW_PDW->item.toa_4   = ( uiTOA >> 24 ) & 0xFF;	// TOA 값 입력 
				pNEW_PDW->item.toa_3   = ( uiTOA >> 16 ) & 0xFF;
				pNEW_PDW->item.toa_2   = ( uiTOA >> 8  ) & 0xFF;
				pNEW_PDW->item.toa_1   = uiTOA & 0xFF;

				// 주파수
				iBc = GetFreqBand( (UINT) ( pPDWData->fFreq + 0.5 ) );
				uiFreq = ConvertFreq( pPDWData->fFreq, iBc );
				pNEW_PDW->item.band = iBc - 1;
				pNEW_PDW->item.frequency_l  = uiFreq & 0xFF;
				pNEW_PDW->item.frequency_h  = ( uiFreq >> 8 ) & 0x1F;

				// 신호세기
				pNEW_PDW->item.amplitude = ConvertPA( pPDWData->fPA );

				// 신호세기
				uiPW = ConvertPW( pPDWData->fPW );
				pNEW_PDW->item.pulse_width_l	= uiPW & 0xFF ;		// 펄스폭 값 입력 
				pNEW_PDW->item.pulse_width_h	= ( uiPW >> 8 ) & 0x07;

				++ pNEW_PDW;
				++ pPDWData;
			}
			m_pSonataData->uiItem = iItem;
			m_pSonataData->enDataType = en_PDW_DATA;
			break;

		case en_IQ_DATA :
			//memcpy( m_pSonataData->unRawData.stIQData, & pstData->stIQData, iItem*sizeof(TNEW_IQ) );
			m_pSonataData->uiItem = iItem;
			m_pSonataData->enDataType = en_IQ_DATA;
			break;

		default :
			break;
	}
	
}

UINT CDlgColList::ConvertPA( float fPA )
{
	UINT uiPA;

	fPA = ( fPA - gPaRes[0].offset ) / gPaRes[0].res;
	uiPA = UADD( fPA, 0.5 );

	return uiPA;
}

UINT CDlgColList::ConvertPW( float fPW )
{
	UINT uiPW;

	uiPW = (UINT) ( ( fPW * 20. ) + 0.5 );
	return uiPW;
}

UINT CDlgColList::ConvertFreq( float fFreq, int iBc )
{
	float fCFreq;

	fCFreq = fFreq - gFreqRes[iBc].min;
	fCFreq /= gFreqRes[iBc].res;

	return (UINT) ( fCFreq + 0.5 );

}

UINT CDlgColList::GetFreqBand( UINT uiFreqMHz )
{
	UINT  i;

	for( i=1 ; i <= 3 ; ++i ) {
		if( gFreqRes[i].max >= uiFreqMHz )
			break;
	}
	if( i == 4 ) {
		Log( enError, _T("입력 주파수[%d]에 대해서 대역을 잘못 얻었습니다.") , uiFreqMHz );
	}

	return( i );
}




void CDlgColList::MakeIQFile( int iItem )
{
	CString strPathname, strFolderName;
	SYSTEMTIME cur_time;

	GetLocalTime( & cur_time );

	// 폴더 생성
	strFolderName.Format( _T("%s//%04d_%02d_%02d//IQ"), RAWDATA_DIRECTORY, cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds ); 
	CreateDir( (TCHAR *) (LPCTSTR) strFolderName );

	m_strFilename.Format( _T("%04d_%02d_%02d %02d_%02d_%02d_%03ld.iq" ), cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds );
	strPathname = strFolderName + _T("//") + m_strFilename;

	ConvertRAWData( iItem, en_IQ_DATA );

	// 저장할 때 수집 정보를 포함해서 저장해야 함.
	m_theDataFile.SaveDataFile( strPathname, (void *) m_pRawData->unRawData.stIQData, iItem, en_SONATA, en_IQ_DATA, & m_pRawData->stColList, sizeof(STR_COL_LIST) );

}

void CDlgColList::InsertIQRawDataItem( STR_DATA_CONTENTS *pstData, int iItem )
{
	int nIndex;

	CString strTemp;

	Log( enNormal, _T("%d 개의 IQ 데이터를 처리합니다." ), iItem );

	memcpy( & m_pRawData->unRawData.stIQData, & pstData->stIQData, iItem*sizeof(STR_RES_IQ_DATA) );

//	TRACE( "IQ 값: " );
//	for( i=0 ; i < 10 /* m_pRawData->uiItem */ ; ++i ) {
//		TRACE( "\n%d\t%d", (unsigned short) m_pRawData->unRawData.stIQData[i].sI, (unsigned short) m_pRawData->unRawData.stIQData[i].sQ );
//	}

	// 데이터를 체계용 PDW 파일로 변환하여 저장합니다.
	MakeIQFile( iItem );

	// 목록창 추가
	strTemp.Format(_T("%d"), m_uiCoRawData );
	nIndex = m_RawList.InsertItem( INT_MAX, strTemp, NULL );

	strTemp.Format(_T("%d"), m_stColList.stColItem.uiNo );
	m_RawList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp = g_stRawListType[enIQ_DataType];
	m_RawList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%s"), g_stColListMode[m_stColList.stColItem.enMode-1] );
	m_RawList.SetItem( nIndex, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), m_stColList.stColItem.fCenterFreq );
	m_RawList.SetItem( nIndex, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%d/%.1f"), m_stColList.stColItem.uiColNumber, m_stColList.stColItem.fColTime ); 
	m_RawList.SetItem( nIndex, 5, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	m_RawList.SetItem( nIndex, 6, LVIF_TEXT, m_strFilename, NULL, NULL, NULL, NULL);

	if( m_bClickedOfRawDataList == false ) {
		m_RawList.SetItemState( -1, 0, LVIS_SELECTED|LVIS_FOCUSED );
		m_RawList.SetItemState( nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_RawList.EnsureVisible( nIndex, FALSE); 
	}
	
	++ m_uiCoRawData;
	
}

void CDlgColList::OnBnClickedButtonOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strPathName;
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	if( true == pApp->OpenFile( strPathName, _T("수집 목록 읽어오기..."), enOpenXLS ) ) {
		long l, lMaxRow;

		CString strNumber, strMode, strCenterFreq, strColTime, strThreshold;

		m_ColList.DeleteAllItems();

		// 엑셀 수집 파일 로딩하기...
		CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

		XL.OpenExcelFile(strPathName);
		lMaxRow = XL.GetRowNum();
		for( l=2 ; l <= lMaxRow; ++l ) {
			int nIndex;

			strNumber = XL.GetCellValue( 1, l );
			strNumber.Replace( _T("00000"), _T("") );
			strMode = XL.GetCellValue( 2, l );
			strCenterFreq = XL.GetCellValue( 3, l );
			strColTime = XL.GetCellValue( 4, l );
			strThreshold = XL.GetCellValue( 4, l );

			nIndex = m_ColList.InsertItem( INT_MAX, strNumber, NULL );

			m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strMode, NULL, NULL, NULL, NULL);
			m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strCenterFreq, NULL, NULL, NULL, NULL);
			m_ColList.SetItem( nIndex, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
			m_ColList.SetItem( nIndex, 4, LVIF_TEXT, strThreshold, NULL, NULL, NULL, NULL);
		}

		XL.ReleaseExcel();
	}

}

void CDlgColList::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strPathName;
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	if( true == pApp->OpenFile( strPathName, _T("수집 목록 저장하기..."), enSaveXLS ) ) {
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
	
	}

}


void CDlgColList::OnLvnItemActivateListRawdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	m_bClickedOfRawDataList = true;

	STR_RAW_LIST stRawItem;
	CString strPathFileName;
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	GetRawListFromList( pNMIA->iItem, & stRawItem );

	int iYear, iMon, iDay;
	swscanf_s( stRawItem.stRawItem.szFilename, _T("%d_%d_%d"), & iYear, & iMon, & iDay );

	strPathFileName.Format( _T("%s//%04d_%02d_%02d//PDW//%s"), RAWDATA_DIRECTORY, iYear, iMon, iDay, stRawItem.stRawItem.szFilename ); 

	pApp->RawDataOpen( & strPathFileName );

}

void CDlgColList::GetRawListFromList( int iRow, STR_RAW_LIST *pRawList )
{
	int i;
	CString strTemp;

	strTemp = m_RawList.GetItemText( iRow, 0 );
	swscanf_s( strTemp.GetBuffer(), _T("%d"), & pRawList->stRawItem.uiNo );

	strTemp = m_RawList.GetItemText( iRow, 1 );
	swscanf_s( strTemp.GetBuffer(), _T("%d"), & pRawList->stRawItem.uiColTaskNum );

	strTemp = m_RawList.GetItemText( iRow, 2 );
	pRawList->stRawItem.enRawDataType = enPDW_DataType;
	for( i=0 ; i <= enIQ_DataType ; ++i ) {
		if( strTemp.Compare( g_stRawListType[i] ) == 0 ) {
			pRawList->stRawItem.enRawDataType = ( ENUM_RAWDATA_TYPE ) i;
			break;
		}
	}

	strTemp = m_RawList.GetItemText( iRow, 3 );
	pRawList->stRawItem.enMode = enIF2_WIDE;
	for( i=0 ; i <= enIQ_NARROW ; ++i ) {
		if( strTemp.Compare( g_stColListMode[i] ) == 0 ) {
			pRawList->stRawItem.enMode = ( ENUM_COL_MODE ) i;
			break;
		}
	}

	strTemp = m_RawList.GetItemText( iRow, 4 );
	swscanf_s( strTemp.GetBuffer(), _T("%f"), & pRawList->stRawItem.fCenterFreq );

	strTemp = m_RawList.GetItemText( iRow, 5 );
	swscanf_s( strTemp.GetBuffer(), _T("%d/%f"), & pRawList->stRawItem.uiColNumber, & pRawList->stRawItem.fColTime );

	strTemp = m_RawList.GetItemText( iRow, 6 );
	_tcscpy_s( pRawList->stRawItem.szFilename, MAX_PATH, strTemp.GetBuffer(0) );
	strTemp.ReleaseBuffer();
	
}

void CDlgColList::OnHdnItemclickListRawdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	m_bClickedOfRawDataList = true;
}


void CDlgColList::OnNMClickListRawdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	// 자동 스크롤 기능 OFF
	m_bClickedOfRawDataList = true;

	if( m_iClickedItem >= 0 ) {
		m_RawList.SetItemBkColor( m_iClickedItem, -1, ::GetSysColor(COLOR_WINDOW), FALSE );
	}

	m_iClickedItem = pNMItemActivate->iItem;

	m_RawList.SetItemBkColor( pNMItemActivate->iItem, -1, ::GetSysColor(COLOR_WINDOWFRAME) );
}


void CDlgColList::OnLvnEndScrollListRawdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 이 기능을 사용하려면 Internet Explorer 5.5 이상이 필요합니다.
	// _WIN32_IE 기호는 0x0560보다 크거나 같아야 합니다.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
