// DlgColList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgColList.h"
#include "afxdialogex.h"

#include "./PDW/DataFile.h"

extern TCHAR g_stColListMode[6][20];

//DWORD WINAPI FuncColList( LPVOID lpData );

//TCHAR g_stColListMode[6][20] = { _T("IF2(광대역)"), _T("IF1 Course"), _T("IF1 Fine"), _T("광대역IQ"), _T("협대역IQ"), _T("널입니다.") } ;

TCHAR g_stRawListType[6][20] = { _T("PDW"), _T("IQ") } ;

#define WAIT_RESPONSE( A, B ) 	uiTry = 0;	\
								do {	\
									++ uiTry;	\
									Sleep( A );	\
									if( uiTry > MAX_WAIT_RESPONSE ) {	\
										break;	\
									}	\
								} while( B );

extern float _spOneSec;
extern float _spOneMilli;
extern float _spOneMicrosec;
extern float _spOneNanosec;

// CDlgColList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgColList, CDialogEx)

CDlgColList::CDlgColList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgColList::IDD, pParent)
{
	//m_pApp = ( CShuDeltaGraphApp *) AfxGetApp();
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
	DDX_Control(pDX, IDC_LIST2_RAWDATA, m_RawList);
	DDX_Control(pDX, IDC_STATIC_TOTAL_RAW_LIST, m_CStaticTotalRawList);
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 10:12:59
 * @warning   
 */
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
	int i;

	m_iSelItem = -1;

	for( i=0 ; i <= enRSA ; ++i ) {
		m_pListener[i] = new MyEchoSocket( (enUnitID) i, false );
		m_pListener[i]->SetParentDlg(this);

		m_pConnected[i] = new MyEchoSocket( (enUnitID) i, false );
		m_pConnected[i]->SetParentDlg(this);
	}

	//m_ptxData = (char *) malloc(sizeof(char) * 100000);
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
	int i;

	m_theThread.Stop();

	for( i=0 ; i <= enRSA ; ++i ) {
		delete m_pListener[i];
		delete m_pConnected[i];
	}

	//free( m_ptxData );
	//free( m_pColList );
	//free( m_pRawData );
	free( m_pSonataData );

	CloseHandle( m_hReceveLAN );

	m_pTip->DestroyWindow();
	delete m_pTip;
}



BEGIN_MESSAGE_MAP(CDlgColList, CDialogEx)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDlgColList::OnHdnItemdblclickListColList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COL_LIST, &CDlgColList::OnDblclkListColList)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_LIST, &CDlgColList::OnBnClickedButtonModifyList)
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_RAWDATA, &CDlgColList::OnLvnItemActivateListRawdata)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDlgColList::OnHdnItemclickListRawdata)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RAWDATA, &CDlgColList::OnNMClickListRawdata)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_RAWDATA, &CDlgColList::OnLvnEndScrollListRawdata)
	ON_WM_VSCROLL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgColList::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CDlgColList 메시지 처리기입니다.

/**
 * @brief     
 * @param     CButtonST * pCButtonRouteSetup
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 10:09:00
 * @warning   
 */
void CDlgColList::InitButtonST( CButtonST *pCButtonRouteSetup )
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
 * @date      2020/03/09 10:08:57
 * @warning   
 */
void CDlgColList::InitToolTip()
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
 * @date      2020/03/09 10:09:06
 * @warning   
 */
void CDlgColList::InitStatic()
{

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

	pApp->LoadProfile( & m_stColItem );

	InitVar();
	InitBuffer();
	InitStatusBar();
	InitTab();
	InitListCtrl();
	InitStatic();
	InitToolTip();

	CreateDir( RAWDATA_DIRECTORY );

	InitSocketSetting( enSHU );
	InitSocketSetting( enRSA );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:55
 * @warning   
 */
void CDlgColList::OnAccept( enUnitID id )
{
	CString strIP;
	UINT port;

	Log( enNormal, _T("클라이언트와 연결됐습니다.") );

	if ( m_pListener[id]->Accept( *m_pConnected[id] )) {
		CString strMsg;

		m_pListener[id]->Close();

		m_pConnected[id]->GetSockName(strIP, port);
		//m_status = "Client Connected,IP :" + strIP;
		//m_sConnected.Send("Connected To Server", strlen("Connected To Server"));
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(FALSE);
		strMsg.Format( _T("연결:%s/%d"), (char*)(LPCTSTR) strIP, port);
		m_StatusBar.SetText( strMsg, 0, 0);
		m_StatusBar.SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );

		Log( enNormal, _T("랜 연결(%s/%d) 되었습니다."), (char*)(LPCTSTR) strIP, port);
		
		m_enConnectMode = CONNECTED;
		UpdateData(FALSE);

		if( id == enSHU ) {
			m_pDlgSHU->SetControl( true );
		}
		else {
			m_pDlgRSA->SetControl( true );
		}

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

/**
 * @brief     
 * @param     bool bEnable
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 21:00:06
 * @warning   
 */
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
void CDlgColList::OnSocketClose( enUnitID id )
{
	//m_theThread.Stop( true );

	Log( enError, _T("소켓을 닫습니다.") );

	InitVar();

	m_pListener[id]->Close();
	m_pListener[id]->ShutDown();

	m_pConnected[id]->InitVar();
	m_pConnected[id]->Close();

	m_StatusBar.SetText(_T("대기 상태"), 0, 0);
	m_StatusBar.SetBkColor( GetSysColor(CTLCOLOR_DLG) );

	//AfxMessageBox(IDS_STRING_LANCLOSE, MB_OK );

	//OnBnClickedButtonServerrun();
	if( id == enSHU ) {
		m_pDlgSHU->m_theThread.Stop( true );
		m_pDlgSHU->SetControl( false );

	}
	else {
		m_pDlgRSA->m_theThread.Stop( true );
		m_pDlgRSA->SetControl( false );
	}

	InitSocketSetting( id );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:10:36
 * @warning   
 */
void CDlgColList::InitSocketSetting( enUnitID id )
{
	UINT uiPortNum;
	CString strPortNum;
													  
	CString strIPAddress=GetIpAddress();

	uiPortNum = stPortNum[id];

	Log( enNormal, _T("[%s/%d]는 서버로 실행합니다."), (char*)(LPCTSTR) strIPAddress, uiPortNum );

	m_pListener[id]->Create( uiPortNum, SOCK_STREAM);
	m_pListener[id]->Listen();

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
	//m_theThread.Attach( FuncColList );
	//m_theThread.Start( this );
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
void CDlgColList::OnConnect(int nErrorCode, enUnitID id )
{
	TCHAR szBuffer[100];

	Log( enNormal, _T("클라리언트와 연결됐습니다.") );

	if (nErrorCode != 0) {
		//AfxMessageBox(_T("서버와 연결할 수 없습니다 !!!"));
		m_StatusBar.SetText(_T("대기 상태"), 0, 0);
	}
	else {
		UINT uiPortNum;
		CString strIPAddress;

		m_pConnected[id]->GetPeerName( strIPAddress, uiPortNum );

		//Log( enNormal, "%s/%d 에 연결되었습니다.", (char*)(LPCTSTR)strIPAddress, uiPortNum );

		wprintf_s( szBuffer, sizeof(szBuffer), _T("연결[%s/%d]"), (char*)(LPCTSTR)strIPAddress, uiPortNum );
		m_StatusBar.SetText(szBuffer, 0, 0);

		//GetDlgItem(IDC_BUTTON_CLIENT)->SetWindowText(_T("서버 연결끊기"));

		//CALL_DIALOG( enUnit, OnConnect() );

	}
}

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

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/09 23:30:59
 * @warning   
 */
void CDlgColList::InitTab()
{
	CRect rect;

	m_Tab.InsertItem( 0, _T("수퍼헷수신장치"));
	m_Tab.InsertItem( 1, _T("레이더징표분석장치"));

	m_Tab.SetCurSel( 0 );

	m_Tab.GetWindowRect( & rect );

	m_pDlgRSA = new CDialogRSA( NULL, this );
	m_pDlgRSA->Create( IDD_DIALOG_RSA, & m_Tab );
	m_pDlgRSA->MoveWindow( 0, 25, rect.Width(), rect.Height() );
	m_pDlgRSA->ShowWindow( SW_HIDE );

	m_pDlgSHU = new CDialogSHU( NULL , this );
	m_pDlgSHU->Create( IDD_DIALOG_SHU, & m_Tab );
	m_pDlgSHU->MoveWindow( 0, 25, rect.Width(), rect.Height() );
	m_pDlgSHU->ShowWindow( SW_SHOW );

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
	int i;
	CRect rt;

	if( bInit == true ) {
		m_RawList.GetWindowRect(&rt);
		m_RawList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

		i = 0;
		m_RawList.InsertColumn(i++, _T("순번"), LVCFMT_LEFT, (int) ( rt.Width()*0.10), -1 );
		m_RawList.InsertColumn(i++, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.04), -1 );
		m_RawList.InsertColumn(i++, _T("종류"), LVCFMT_LEFT, (int) ( rt.Width() * 0.05) , -1);
		m_RawList.InsertColumn(i++, _T("모드"), LVCFMT_LEFT, (int) ( rt.Width() * 0.10) , -1);
		m_RawList.InsertColumn(i++, _T("과제 중심 주파수[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.1 ), -1);
		m_RawList.InsertColumn(i++, _T("과제 수집개수/시간[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.1 ), -1);
		m_RawList.InsertColumn(i++, _T("평균 주파수[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.1 ), -1);
		m_RawList.InsertColumn(i++, _T("평균 DTOA[us]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.1 ), -1);
		m_RawList.InsertColumn(i++, _T("저장 파일명"), LVCFMT_LEFT, (int) (rt.Width() * 0.19), -1);

		m_RawList.SetGridLines(TRUE);
		//m_CListRawData.SetCheckboxes(TRUE);
	}
	else {

	}

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
// 	pstColList->stColItem.uiNo = (UINT) m_CSpinNum.GetPos();
// 	pstColList->stColItem.fCenterFreq = (float) m_CSpinCenterFreq.GetPos();
// 	pstColList->stColItem.fThreshold = (float) m_CSpinThreshold.GetPos();
// 	pstColList->stColItem.fColTime = (float) m_CSpinColTime.GetPos();
// 	pstColList->stColItem.uiColNumber = (UINT) m_CSpinColNum.GetPos();
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
// 	*pResult = 0;
// 
// 	STR_COL_LIST stColList;
// 
// 	m_iSelItem = pNMItemActivate->iItem;
// 	if( m_iSelItem >= 0 ) {
// 		GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( TRUE );
// 
// 		GetColListFromList( m_iSelItem, & stColList );
// 
// 		m_CSpinNum.SetPos( (double) stColList.stColItem.uiNo );
// 		m_CComboMode.SetCurSel( stColList.stColItem.enMode );
// 		m_CSpinCenterFreq.SetPos( (double) stColList.stColItem.fCenterFreq );
// 
// 		m_CSpinColTime.SetPos( (double) stColList.stColItem.fColTime );
// 		m_CSpinColNum.SetPos( (double) stColList.stColItem.uiColNumber );
// 
// 		m_CSpinThreshold.SetPos( (double) stColList.stColItem.fThreshold );
// 	}

}

void CDlgColList::GetColListFromList( int iRow, STR_COL_LIST *pColList )
{
// 	int i;
// 	CString strTemp;
// 
// 	pColList->iRowOfList = iRow;
// 	strTemp = m_ColList.GetItemText( iRow, 0 );
// 	swscanf_s( strTemp.GetBuffer(), _T("%d"), & pColList->stColItem.uiNo );
// 
// 	strTemp = m_ColList.GetItemText( iRow, 1 );
// 
// 	pColList->stColItem.enMode = enIF2_WIDE;
// 	for( i=0 ; i <= enIQ_NARROW ; ++i ) {
// 		if( strTemp.Compare( g_stColListMode[i] ) == 0 ) {
// 			pColList->stColItem.enMode = ( ENUM_COL_MODE ) i;
// 		}
// 	}
// 
// 	strTemp = m_ColList.GetItemText( iRow, 2 );
// 	swscanf_s( strTemp.GetBuffer(), _T("%f"), & pColList->stColItem.fCenterFreq );
// 
// 	strTemp = m_ColList.GetItemText( iRow, 3 );
// 	swscanf_s( strTemp.GetBuffer(), _T("%d/%f"), & pColList->stColItem.uiColNumber, & pColList->stColItem.fColTime );
// 
// 	strTemp = m_ColList.GetItemText( iRow, 4 );
// 	swscanf_s( strTemp.GetBuffer(), _T("%f"), & pColList->stColItem.fThreshold );

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
// 	CString strNumber, strMode, strCenterFreq, strColTime, strThreshold;
// 
// 	STR_COL_LIST stColList;
// 	STR_COL_LIST *pColList;
// 
// 	GetDlgItem(IDC_EDIT_NUM)->EnableWindow( FALSE );
// 	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );
// 
// 	GetColList( & stColList );
// 
// 	// 수집 목록 버퍼에 복사
// 	pColList = m_pColList + ( stColList.stColItem.uiNo - 1 );
// 	memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );
// 
// 	// 목록창에 전시
// 	MakeColListString( & strNumber, & strMode, & strCenterFreq, & strColTime, & strThreshold, & stColList );
// 	
// 	m_ColList.SetItem( m_iSelItem, 0, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);
// 	m_ColList.SetItem( m_iSelItem, 1, LVIF_TEXT, strMode, NULL, NULL, NULL, NULL);
// 	m_ColList.SetItem( m_iSelItem, 2, LVIF_TEXT, strCenterFreq, NULL, NULL, NULL, NULL);
// 	m_ColList.SetItem( m_iSelItem, 3, LVIF_TEXT, strColTime, NULL, NULL, NULL, NULL);
// 	m_ColList.SetItem( m_iSelItem, 4, LVIF_TEXT, strThreshold, NULL, NULL, NULL, NULL);

}

/**
 * @brief     
 * @param     CString * pstrNum
 * @param     CString * pstrMode
 * @param     CString * pstrCenterFreq
 * @param     CString * pstrColTime
 * @param     CString * pstrThreshold
 * @param     STR_COL_LIST * pstColList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 16:58:48
 * @warning   
 */
 void CDlgColList::MakeColListString( CString *pstrNum, CString *pstrMode, CString *pstrCenterFreq, CString *pstrColTime, CString *pstrThreshold, STR_COL_LIST *pstColList )
{
	pstrNum->Format(_T("%d"), pstColList->stColItem.uiNo );

	pstrMode->Format(_T("%s"), g_stColListMode[pstColList->stColItem.enMode] );

	pstrCenterFreq->Format(_T("%.2f"), pstColList->stColItem.fCenterFreq );

	pstrColTime->Format(_T("%d/%.1f"), pstColList->stColItem.uiColNumber, pstColList->stColItem.fColTime );

	pstrThreshold->Format(_T("%.1f"), pstColList->stColItem.fThreshold );


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
void CDlgColList::Send( enUnitID id, char *ptxData )
{
	STR_MESSAGE *pTxMessage;

	m_pConnected[id]->Send( ptxData, sizeof(STR_MESSAGE) );

	pTxMessage = (STR_MESSAGE * ) ptxData;
	if( pTxMessage->uiDataLength != 0 ) {
		m_pConnected[id]->Send( & ptxData[sizeof(STR_MESSAGE)], pTxMessage->uiDataLength );
	}

	LogTxMessage( ptxData );

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
 * @param     CString * pStrTemp1
 * @param     CString * pStrTemp2
 * @param     CString * pStrTemp3
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:40:00
 * @warning   
 */
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
// 		if( pstData->stResInit.uiReqCode == 0 && pstData->stResInit.uiErrorCode == 0 )
// 			m_CButtonInit.SetIcon( IDI_ICON_OK, 24, 24, IDI_ICON_OK, 24, 24);
// 		else
// 			m_CButtonInit.SetIcon( IDI_ICON_ERROR, 24, 24, IDI_ICON_ERROR, 24, 24);
// 
// 		iCnt += wsprintf( & szBuffer[0], _T("Opcode[0x%x], ErrorCode[0x%x]\n") , pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
// 		UpdateToolTip( szBuffer, & m_CButtonInit );
		break;

	case RES_SET_CONFIG :
		break;

	case RES_COL_START :
		memcpy( & m_stResCol, pstData, sizeof(STR_RES_COL_START) );
		break;

	case RES_RAWDATA_PDW :
		InsertPDWRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_PDW_DATA), m_uiColList, & m_stColList, m_pRawData );
		break;

	case RES_RAWDATA_INTRA :
		InsertIntraRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_INTRA_DATA) );
		break;

	case RES_RAWDATA_IQ:
		InsertIQRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_IQ_DATA), & m_stColList, m_pRawData );
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
// 		GetColItem( & m_stColItem );
// 		//종료버튼 눌릴 시
 	}
// 	else if(nID == SC_MAXIMIZE)
// 	{ 
// 		//최대화 버튼 눌릴 시
// 	}
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

// void CDlgColList::SetIBkColorOfColList( UINT uiIndex, int nStep )
// {
// 	STR_COL_LIST *pColList;
// 
// 	pColList = m_pColList + uiIndex;
// 
// 	if( nStep == -1 )
// 		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(255, 255, 255) );
// 	else if( nStep <= 2 )
// 		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 100, 0) );
// 	else if( nStep == 3 )
// 		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 150, 0) );
// 	else if( nStep == 4 )
// 		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 250, 0) );
// 	else 
// 		m_ColList.SetItemBkColor( pColList->iRowOfList, -1, RGB(0, 0, 10) );
// 
// }

/**
 * @brief     
 * @param     UINT uiIndex
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/10 22:35:36
 * @warning   
 */
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

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:09
 * @warning   
 */
void CDlgColList::ViewGraph( UINT uiOpCode )
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	if( uiOpCode == RES_RAWDATA_PDW ) {
		SetEvent( pApp->m_pDlg2DHisto->m_hHisto[en_ReceiveData] );
		//SetEvent( pApp->m_pDlg3DBar->m_h3DBar );

		SetEvent( pApp->m_pDlgMulti->m_hMulti );
	}
	else {
		SetEvent( pApp->m_pDlgMulti->m_hMulti );
	}

}

/**
 * @brief     
 * @param     STR_DATA_CONTENTS * pstData
 * @param     int iItem
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:12
 * @warning   
 */
void CDlgColList::InsertPDWRawDataItem( STR_DATA_CONTENTS *pstData, int iItem, int uiColList, STR_COL_LIST *pColList, STR_RAW_DATA *pRawData )
{
	int i, nIndex;
	CString strTemp;

	Log( enNormal, _T("PDW 데이터를 완료 처리합니다." ) );

	// 데이터를 체계용 PDW 파일로 변환하여 저장합니다.
	MakePDWFile( iItem, uiColList, pRawData );

	// 목록창 추가
 	strTemp.Format(_T("%d"), m_uiCoRawData );
 	nIndex = m_RawList.InsertItem( INT_MAX, strTemp, NULL );

	i = 1;
	strTemp.Format(_T("%d"), pColList->stColItem.uiNo );
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp = g_stRawListType[enPDW_DataType];
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%s"), g_stColListMode[pColList->stColItem.enMode] );
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), pColList->stColItem.fCenterFreq );
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%d/%.1f"), pColList->stColItem.uiColNumber, pColList->stColItem.fColTime ); 
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	// PDW 제원 통계 값 전시
	strTemp.Format(_T("%.1f"), m_stStatPDW.fFreqMean ); 
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), m_stStatPDW.fDtoaMean ); 
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, m_strFilename, NULL, NULL, NULL, NULL);

	if( m_bClickedOfRawDataList == false ) {
		m_RawList.SetItemState( -1, 0, LVIS_SELECTED|LVIS_FOCUSED );
		m_RawList.SetItemState( nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_RawList.EnsureVisible( nIndex, FALSE); 
	}

	SetTotalRawList();

	++ m_uiCoRawData;

}

/**
 * @brief     
 * @param     int iItem
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:17
 * @warning   
 */
void CDlgColList::MakePDWFile( int iItem, int uiColList, STR_RAW_DATA *pRawData )
{
	CString strPathname, strFolderName;
	SYSTEMTIME cur_time;

	GetLocalTime( & cur_time );

	// 폴더 생성
	strFolderName.Format( _T("%s//%04d_%02d_%02d//PDW"), RAWDATA_DIRECTORY, cur_time.wYear, cur_time.wMonth, cur_time.wDay ); 
	CreateDir( (TCHAR*) (LPCTSTR) strFolderName );

	m_strFilename.Format( _T("%04d_%02d_%02d %02d_%02d_%02d_%03ld.spdw" ), cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds );

	strPathname = strFolderName + _T("//") + m_strFilename;

	ConvertRAWData( iItem, en_PDW_DATA, uiColList, pRawData );

	//m_theDataFile.SaveDataFile( strPathname, (void *) m_pSonataData->unRawData.stPDWData, iItem*sizeof(TNEW_PDW), en_SONATA, en_PDW_DATA );

}



/**
 * @brief     
 * @param     int iItem
 * @param     ENUM_DataType enDataType
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:24
 * @warning   
 */
void CDlgColList::ConvertRAWData( int iItem, ENUM_DataType enDataType, int uiColList, STR_RAW_DATA *pRawData )
{
	int i;

	TNEW_PDW *pNEW_PDW;
	STR_RES_PDW_DATA *pPDWData;

	int iBc=0;
	UINT uiTOA, uiFreq, uiPW, uiDTOA, uiPreTOA;

	switch( enDataType ) {
		case en_PDW_DATA :
			// PDW 통계치 초기화
			m_stStatPDW.fFreqMean = 0;
			m_stStatPDW.fFreqMin = FREQ_MAX;
			m_stStatPDW.fFreqMax = FREQ_MIN;

			m_stStatPDW.fDtoaMean = 0;
			m_stStatPDW.fDtoaMin = (float) 99999999.999;
			m_stStatPDW.fDtoaMax = 0;

			pPDWData = & pRawData->unRawData.stPDWData[0];
			pNEW_PDW = & m_pSonataData->unRawData.stPDWData[0];
			memset( pNEW_PDW, 0, sizeof(TNEW_PDW) * iItem );
			for( i=0 ; i < iItem ; ++i ) {
				pNEW_PDW->item.dv = PDW_DV;
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

				//
				//////////////////////////////////////////////////////////////////////////
				m_stStatPDW.fFreqMean += pPDWData->fFreq;

				if( i == 0 ) {
					uiPreTOA = 0;
				}
				else {
					uiDTOA = uiTOA - uiPreTOA;
					m_stStatPDW.fDtoaMean += uiDTOA;
				}
				uiPreTOA = uiTOA;

				++ pNEW_PDW;
				++ pPDWData;
			}
			m_pSonataData->uiItem = iItem;
			m_pSonataData->enDataType = en_PDW_DATA;

			m_pSonataData->uiNo = m_pColList[uiColList].stColItem.uiNo;

			//
			//////////////////////////////////////////////////////////////////////////
			m_stStatPDW.fFreqMean /= iItem;

			m_stStatPDW.fDtoaMean /= ( iItem - 1 );
			//m_stStatPDW.fDtoaMean = FDIV( m_stStatPDW.fDtoaMean, _spOneMicrosec );
			m_stStatPDW.fDtoaMean = FDIV( m_stStatPDW.fDtoaMean * 20., 1000. );

			break;

		case en_IQ_DATA :
			memcpy( m_pSonataData->unRawData.stIQData, & pRawData->unRawData.stIQData, iItem*sizeof(TNEW_IQ) );
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

/**
 * @brief     
 * @param     int iItem
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 22:34:30
 * @warning   
 */
void CDlgColList::MakeIQFile( int iItem, STR_RAW_DATA *pRawData )
{
	CString strPathname, strFolderName;
	SYSTEMTIME cur_time;

	GetLocalTime( & cur_time );

	// 폴더 생성
	strFolderName.Format( _T("%s//%04d_%02d_%02d//IQ"), RAWDATA_DIRECTORY, cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds ); 
	CreateDir( (TCHAR *) (LPCTSTR) strFolderName );

	m_strFilename.Format( _T("%04d_%02d_%02d %02d_%02d_%02d_%03ld.iq" ), cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds );
	strPathname = strFolderName + _T("//") + m_strFilename;

	ConvertRAWData( iItem, en_IQ_DATA, m_uiColList, pRawData );

	// 저장할 때 수집 정보를 포함해서 저장해야 함.
	STR_IQ_HEADER stIQHeader;

	//stIQHeader.enMode = m_pRawData->stColList.stColItem.enMode;
	stIQHeader.fCenterFreq = m_pRawData->stColList.stColItem.fCenterFreq;
	stIQHeader.fColTime = m_pRawData->stColList.stColItem.fColTime;
	stIQHeader.uiColNumber = m_pRawData->stColList.stColItem.uiColNumber;
	stIQHeader.fThreshold = m_pRawData->stColList.stColItem.fThreshold;
	//m_theDataFile.SaveDataFile( strPathname, (void *) m_pRawData->unRawData.stIQData, iItem, en_SONATA, en_IQ_DATA, & stIQHeader, sizeof(STR_IQ_HEADER) );

}

/**
 * @brief     
 * @param     STR_DATA_CONTENTS * pstData
 * @param     int iItem
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 22:34:32
 * @warning   
 */
void CDlgColList::InsertIQRawDataItem( STR_DATA_CONTENTS *pstData, int iItem, STR_COL_LIST *pColList, STR_RAW_DATA *pRawData )
{
	int nIndex;

	CString strTemp;

	Log( enNormal, _T("%d 개의 IQ 데이터를 처리합니다." ), iItem );

	memcpy( & pRawData->unRawData.stIQData, & pstData->stIQData, iItem*sizeof(STR_RES_IQ_DATA) );

	// 데이터를 체계용 PDW 파일로 변환하여 저장합니다.
	MakeIQFile( iItem, pRawData );

	// 목록창 추가
	strTemp.Format(_T("%d"), m_uiCoRawData );
	nIndex = m_RawList.InsertItem( INT_MAX, strTemp, NULL );

	strTemp.Format(_T("%d"), pColList->stColItem.uiNo );
	m_RawList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp = g_stRawListType[enIQ_DataType];
	m_RawList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%s"), g_stColListMode[pColList->stColItem.enMode] );
	m_RawList.SetItem( nIndex, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), pColList->stColItem.fCenterFreq );
	m_RawList.SetItem( nIndex, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%d/%.1f"), pColList->stColItem.uiColNumber, pColList->stColItem.fColTime ); 
	m_RawList.SetItem( nIndex, 5, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	m_RawList.SetItem( nIndex, 8, LVIF_TEXT, m_strFilename, NULL, NULL, NULL, NULL);

	if( m_bClickedOfRawDataList == false ) {
		m_RawList.SetItemState( -1, 0, LVIS_SELECTED|LVIS_FOCUSED );
		m_RawList.SetItemState( nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_RawList.EnsureVisible( nIndex, FALSE); 
	}
	
	++ m_uiCoRawData;
	
}

/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/08 23:33:30
 * @warning   
 */
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

	if( stRawItem.stRawItem.enRawDataType == enPDW_DataType )
		strPathFileName.Format( _T("%s/%04d_%02d_%02d/PDW/%s"), RAWDATA_DIRECTORY, iYear, iMon, iDay, stRawItem.stRawItem.szFilename ); 
	else
		strPathFileName.Format( _T("%s/%04d_%02d_%02d/IQ/%s"), RAWDATA_DIRECTORY, iYear, iMon, iDay, stRawItem.stRawItem.szFilename ); 

	pApp->RawDataOpen( & strPathFileName );

}

/**
 * @brief     
 * @param     int iRow
 * @param     STR_RAW_LIST * pRawList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/08 23:33:27
 * @warning   
 */
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

	strTemp = m_RawList.GetItemText( iRow, 8 );
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




void CDlgColList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/09 23:36:48
 * @warning   
 */
void CDlgColList::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSel = m_Tab.GetCurSel();

	switch( iSel ) {
	case 0 :
		m_pDlgRSA->ShowWindow( SW_HIDE );
		m_pDlgSHU->ShowWindow( SW_SHOW );
		break;

	case 1 :
		m_pDlgRSA->ShowWindow( SW_SHOW );
		m_pDlgSHU->ShowWindow( SW_HIDE );		
		break;
	}

	*pResult = 0;
}

