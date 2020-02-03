Onin// DlgColList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgColList.h"
#include "afxdialogex.h"

DWORD WINAPI FuncColList( LPVOID lpData );

TCHAR g_stColListMode[3][20] = { _T("IF2(광대역)"), _T("IF1 Course"), _T("IF1 Fine") } ;


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
	DDX_Control(pDX, IDC_LIST2_RAWDATA, m_CListRawData);
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
}

void CDlgColList::InitVar()
{
	m_uiLog = 1;	
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
	m_pConnected = new MyEchoSocket( false );

	m_pListener->SetParentDlg(this);
	m_pConnected->SetParentDlg(this);

	m_ptxData = (char *) malloc(sizeof(char) * 100000);
	m_prxData = (char *) malloc(sizeof(char) * 100000);

	m_pColList = ( STR_COL_LIST *) malloc( sizeof(STR_COL_ITEM) * MAX_COL_ITEMS );

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
	free( m_prxData );
	free( m_pColList );

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
	m_CStaticTotalColList.SetBackgroundColor(RGB(255,255,255), FALSE);
	m_CStaticTotalColList.SetBold(TRUE, FALSE);
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

	InitButton();
	InitBuffer();
	InitStatusBar();
	InitListCtrl();
	InitStatic();
	InitToolTip();

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
	m_CComboMode.SetCurSel( 0 );

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

	if (m_pListener->Accept( *m_pConnected )) {
		CString strMsg;

		m_pConnected->GetSockName(strIP, port);
		//m_status = "Client Connected,IP :" + strIP;
		//m_sConnected.Send("Connected To Server", strlen("Connected To Server"));
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(FALSE);
		strMsg.Format( _T("연결:%s/%d"), (char*)(LPCTSTR) strIP, port);
		m_StatusBar.SetText( strMsg, 0, 0);
		m_StatusBar.SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );
		
		m_enConnectMode = CONNECTED;
		UpdateData(FALSE);

		SetControl( true );

		//CALL_DIALOG( enUnit, OnConnect() );
	}
	else
	{
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(TRUE);
		AfxMessageBox( _T("Cannoot Accept Connection") );
	}

}

void CDlgColList::SetControl( bool bEnable )
{
	GetDlgItem(IDC_BUTTON_INIT)->EnableWindow( bEnable );
	GetDlgItem(IDC_BUTTON_SETMODE)->EnableWindow( bEnable );

	GetDlgItem(IDC_BUTTON_ADD_LIST)->EnableWindow( !bEnable );
	
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

	//TRACE( "%s(%d번) 소켓을 닫습니다." ,  g_szUnit[enUnit], enUnit );
	//Log( enError, "%s(%d번) 소켓을 닫습니다." ,  g_szUnit[enUnit], enUnit );

	m_pListener->Close();
	m_pListener->ShutDown();

	m_pConnected->Close();

	m_StatusBar.SetText(_T("대기 상태"), 0, 0);
	m_StatusBar.SetBkColor( GetSysColor(CTLCOLOR_DLG) );

	//AfxMessageBox(IDS_STRING_LANCLOSE, MB_OK );

	//OnBnClickedButtonServerrun();
	SetControl( false );

	InitSocketSetting();

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
void CDlgColList::OnReceive()
{
	CString strData;
	int iLenOfHeader, iLenOfData;

	int iSxSize;

	CString strTemp1, strTemp2, strNum;

	STR_MESSAGE *pstRxMessage;
	STR_DATA_CONTENTS *pstRxData;

	pstRxMessage = (STR_MESSAGE * ) m_prxData;
	pstRxData = (STR_DATA_CONTENTS * ) & m_prxData[sizeof(STR_MESSAGE)];

	// 1. 헤더 데이터 수신하기
	iLenOfHeader = m_pConnected->Receive((char *) pstRxMessage, sizeof(STR_MESSAGE) );

	// 2. 데이터 수신하기
	if( pstRxMessage->uiDataLength != 0 ) {
		iLenOfData = m_pConnected->Receive( (char *) pstRxData, pstRxMessage->uiDataLength );
	}
	else {
		iLenOfData = 0;
	}

	if (iLenOfHeader == SOCKET_ERROR) {
		AfxMessageBox(_T("Could not Recieve"));
	}
	else {
		MakeLogResMessage( & strTemp1, & strTemp2, (void *) m_prxData );

		UpdateResultData( m_prxData );

		InsertItem( & strTemp1, & strTemp2 );

	}

}

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
	UINT uiIndex=0, uiTry;
	CDlgColList *pDlg;

	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDlgColList * ) pParent->GetParam();

	while( TRUE ) {
		// 1. 수집 페라미터 설정
		pDlg->MakeSetModeMessage( uiIndex );
		pDlg->Send();
		uiTry = 0;
		do {
			++ uiTry;
			Sleep( 100 );

			if( uiTry > MAX_WAIT_RESPONSE ) {
				break;
			}

		} while( true );
		if( uiTry > MAX_WAIT_RESPONSE ) {
			Log( enError, _T("수집 파리미터 설정에서 에러가 발생합니다.") );
			break;
		}

		// 2. 신호수집 시작 요구
		pDlg->MakeColStartMessage();
		pDlg->Send();
		uiTry = 0;
		do {
			++ uiTry;
			Sleep( 100 );

			if( uiTry > MAX_WAIT_RESPONSE ) {
				break;
			}

		} while( true );
		if( uiTry > MAX_WAIT_RESPONSE ) {
			Log( enError, _T("수집 파리미터 설정에서 에러가 발생합니다.") );
			break;
		}

		//

		// 
		uiIndex = pDlg->m_uiCoColList <= uiIndex ? 0 : ++uiIndex;

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
	pTxData->stSetMode.uiMode = pColList->stColItem.iMode;
	pTxData->stSetMode.fTuneFreq = pColList->stColItem.fCenterFreq;
	pTxData->stSetMode.coPulseNum = pColList->stColItem.uiColNumber;
	pTxData->stSetMode.fColTime = pColList->stColItem.fColTime;
	pTxData->stSetMode.fThreshold = pColList->stColItem.fThreshold;
	pTxData->stSetMode.uiPAStatus = 0;

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 15:44:24
 * @warning   
 */
void CDlgColList::InitListCtrl()
{
	CRect rt;

	m_ColList.GetWindowRect(&rt);
	m_ColList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_ColList.InsertColumn(0, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.12), -1 );
	m_ColList.InsertColumn(1, _T("모드"), LVCFMT_LEFT, (int) ( rt.Width() * 0.12) , -1);
	m_ColList.InsertColumn(2, _T("중심 주파수[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.3) , -1);
	m_ColList.InsertColumn(3, _T("수집 개수/시간[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.25), -1);
	m_ColList.InsertColumn(4, _T("임계값[dBm]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.15 ), -1);
	m_ColList.InsertColumn(5, _T("기타"), LVCFMT_LEFT, (int) (rt.Width() * 0.05), -1);

	m_ColList.SetGridLines(TRUE);
	m_ColList.SetCheckboxes(TRUE);

	m_CListRawData.GetWindowRect(&rt);
	m_CListRawData.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_CListRawData.InsertColumn(0, _T("순번"), LVCFMT_LEFT, (int) ( rt.Width()*0.10), -1 );
	m_CListRawData.InsertColumn(1, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.10), -1 );
	m_CListRawData.InsertColumn(2, _T("종류"), LVCFMT_LEFT, (int) ( rt.Width() * 0.10) , -1);
	m_CListRawData.InsertColumn(3, _T("주파수 범위[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2 ), -1);
	m_CListRawData.InsertColumn(4, _T("수집 개수/시간[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.2 ), -1);
	m_CListRawData.InsertColumn(5, _T("저장 위치"), LVCFMT_LEFT, (int) (rt.Width() * 0.29), -1);

	m_CListRawData.SetGridLines(TRUE);
	m_CListRawData.SetCheckboxes(TRUE);

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
	pstColList->uiNo = (UINT) m_CSpinNum.GetPos();

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
	pstColItem->iMode = m_CComboMode.GetCurSel();

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
	pstColList->uiNo = (UINT) m_CSpinNum.GetPos();
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
	stColList.uiNo = m_uiCoColList + 1;
	memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	//
	strTemp.Format(_T("%d"), pColList->uiNo );
 	nIndex = m_ColList.InsertItem( INT_MAX, strTemp, NULL );

	strTemp.Format(_T("%s"), g_stColListMode[pColList->stColItem.iMode] );
	m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), pColList->stColItem.fCenterFreq );
 	m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f/%d"), pColList->stColItem.fColTime, pColList->stColItem.uiColNumber );
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

	Invalidate();

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
		int iMode;

		GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( TRUE );

		GetColListFromList( m_iSelItem, & stColList );

		m_CSpinNum.SetPos( (double) stColList.uiNo );
		m_CComboMode.SetCurSel( stColList.stColItem.iMode );
		m_CSpinCenterFreq.SetPos( (double) stColList.stColItem.fCenterFreq );

		m_CSpinColTime.SetPos( (double) stColList.stColItem.fColTime );
		m_CSpinColNum.SetPos( (double) stColList.stColItem.uiColNumber );

		m_CSpinThreshold.SetPos( (double) stColList.stColItem.fThreshold );
	}

}

void CDlgColList::GetColListFromList( int iRow, STR_COL_LIST *pColList )
{
	CString strTemp;

	strTemp = m_ColList.GetItemText( iRow, 0 );
	swscanf_s( strTemp.GetBuffer(), _T("%d"), & pColList->uiNo );

	strTemp = m_ColList.GetItemText( m_iSelItem, 1 );
	if( strTemp.Compare( g_stColListMode[0] ) == 0 ) {
		pColList->stColItem.iMode = 0;
	}
	else if( strTemp.Compare( g_stColListMode[1] ) == 0 ) {
		pColList->stColItem.iMode = 1;
	}
	else {
		pColList->stColItem.iMode = 2;
	}

	strTemp = m_ColList.GetItemText( m_iSelItem, 2 );
	swscanf_s( strTemp.GetBuffer(), _T("%f"), & pColList->stColItem.fCenterFreq );

	strTemp = m_ColList.GetItemText( m_iSelItem, 3 );
	swscanf_s( strTemp.GetBuffer(), _T("%f/%d"), & pColList->stColItem.fColTime, & pColList->stColItem.uiColNumber );

	strTemp = m_ColList.GetItemText( m_iSelItem, 4 );
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
	CString strTemp;

	STR_COL_LIST stColList;
	STR_COL_LIST *pColList;

	GetDlgItem(IDC_EDIT_NUM)->EnableWindow( FALSE );
	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetColList( & stColList );

	//
	pColList = m_pColList + ( stColList.uiNo - 1 );
	memcpy( pColList, & stColList, sizeof(STR_COL_LIST) );

	//
	strTemp.Format(_T("%d"), stColList.uiNo );
	m_ColList.SetItem( m_iSelItem, 0, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%s"), g_stColListMode[stColList.stColItem.iMode] );
	m_ColList.SetItem( m_iSelItem, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f"), stColList.stColItem.fCenterFreq );
	m_ColList.SetItem( m_iSelItem, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f/%d"), stColList.stColItem.fColTime, stColList.stColItem.uiColNumber );
	m_ColList.SetItem( m_iSelItem, 3, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f"), stColList.stColItem.fThreshold );
	m_ColList.SetItem( m_iSelItem, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

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
	m_ColList.SelectAllItems();
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

	m_ColList.DeleteAllSelectedItems(ItemdataProc, (LPARAM)this);

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
	m_ColList.CheckAllItems();
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
	m_ColList.UnCheckAllItems();
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
	m_ColList.InvertCheck();
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

	m_CButtonColStart.GetWindowText( strTitle );

	if( strTitle.Compare( _T("수집 시작") ) == 0 ) {
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
		m_theThread.Stop( true );
		//Close();

		m_CButtonColStart.SetWindowText( _T("수집 시작") );
	}

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
		*pstrTemp1 = _T(">>초기화 요구");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
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
		*pstrTemp1 = _T("<<초기화요구 응답");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
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
// 
// 	Log( enNormal, _T("%d\t%s\t%s") , m_uiLog, *pStrTemp1, *pStrTemp2 );

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

		LogTxMessage( m_ptxData );
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

		LogTxMessage( m_ptxData );
	}
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

