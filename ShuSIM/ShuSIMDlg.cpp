
// ShuSIMDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ShuSIM.h"
#include "ShuSIMDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CShuSIMDlg 대화 상자




CShuSIMDlg::CShuSIMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShuSIMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShuSIMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_CListLog);
}

BEGIN_MESSAGE_MAP(CShuSIMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CShuSIMDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_REQ_INIT, &CShuSIMDlg::OnBnClickedButtonReqInit)
END_MESSAGE_MAP()


// CShuSIMDlg 메시지 처리기

BOOL CShuSIMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	InitBuffer();
	InitStatusBar();
	InitLOGListCtrl();

	InitVar();

	InitSocketSetting();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CShuSIMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CShuSIMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CShuSIMDlg::OnAccept()
{
	CString strIP, strMsg;
	UINT port;

	if (m_pListener->Accept( *m_pConnected )) {
		m_pConnected->GetSockName(strIP, port);

		strMsg.Format( _T("연결:%s/%d"), (char*)(LPCTSTR) strIP, port);
		m_StatusBar.SetText( strMsg, 0, 0);
		m_StatusBar.SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );

		m_enConnectMode = CONNECTED;
		UpdateData(FALSE);
	}
	else
	{
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(TRUE);
		AfxMessageBox( _T("Cannoot Accept Connection") );
	}

}

void CShuSIMDlg::OnClose()
{

	//TRACE( "%s(%d번) 소켓을 닫습니다." ,  g_szUnit[enUnit], enUnit );
	//Log( enError, "%s(%d번) 소켓을 닫습니다." ,  g_szUnit[enUnit], enUnit );

	m_pListener->Close();
	m_pListener->ShutDown();

	m_pConnected->Close();

	m_StatusBar.SetText(_T("대기 상태"), 0, 0);
	m_StatusBar.SetBkColor( GetSysColor(CTLCOLOR_DLG) );

	//AfxMessageBox(IDS_STRING_LANCLOSE, MB_OK );
	m_pConnected->m_bConnected = false;

	//OnBnClickedButtonServerrun();

	InitSocketSetting();

	//CALL_DIALOG( enUnit, OnClose() );

}

void CShuSIMDlg::OnReceive( char *pData )
{
	CString strData;

	CString strTemp1, strTemp2;

	MakeLogReqMessage( & strTemp1, & strTemp2, (void *) pData );

	ParseData( (void *) pData );
	//(( CTabTranceiverDialog * ) ( m_pDialog[enUnit]))->UpdateData( m_prxData );

	InsertItem( & strTemp1, & strTemp2 );

}

void CShuSIMDlg::ParseData( void *pData )
{
	STR_MESSAGE *pstMessage;
	STR_DATA_CONTENTS *pstData;

	pstMessage = (STR_MESSAGE *) pData;
	pstData = (STR_DATA_CONTENTS * ) ( ( char *) pData + sizeof(STR_MESSAGE) );

	switch (pstMessage->uiOpcode) {
	case REQ_INIT:
		break;

	case REQ_SETMODE:
		OnClose();
		break;

	case REQ_SET_CONFIG:
		MakeResultOfSetConfigMessage();
		Send();
		break;

	case REQ_COL_START:
		MakeResultOfColStartMessage();
		Send();
		break;

	case REQ_RAWDATA :
		MakeResultOfPDWMessage();
		Send();
		
		//MakeResultOfIQMessage();
		//Send();
		break;

	default:
		break;
	}

}

void CShuSIMDlg::OnConnect(int nErrorCode )
{

	if (nErrorCode != 0) {
		//AfxMessageBox(_T("서버와 연결할 수 없습니다 !!!"));
		m_StatusBar.SetText(_T("대기 상태"), 0, 0);
	}
	else {
		UINT uiPortNum;
		CString strIPAddress, strMsg;

		m_pConnected->GetPeerName( strIPAddress, uiPortNum );

		//Log( enNormal, "%s/%d 에 연결되었습니다.", strIPAddress, uiPortNum );

		strMsg.Format( _T("연결:%s/%d"), strIPAddress, uiPortNum );
		//wprintf_s( _T("연결[%s/%d]"), strIPAddress, uiPortNum );
		m_StatusBar.SetText(strMsg, 0, 0);

		//GetDlgItem(IDC_BUTTON_CLIENT)->SetWindowText(_T("서버 연결끊기"));
		//m_pConnected->m_bConnected = true;

		//CALL_DIALOG( enUnit, OnConnect() );

	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CShuSIMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
  * @brief		
  * @return 	void
  * @date			2020/02/03 11:20:24
*/
void CShuSIMDlg::InitSocketSetting()
{
	bool bRet;
	UINT uiPortNum;
	CString strPortNum;

	CString strIPAddress=GetIpAddress();

	uiPortNum = SHU_PORT_NUM;

	Log( enNormal, _T("[%s/%d]는 서버로 실행합니다."), strIPAddress, uiPortNum );
	m_pConnected->Create();

	Connect();

	m_StatusBar.SetText(_T("대기 상태"), (int) 0, 0);

}

void CShuSIMDlg::Connect()
{
	UINT uiPortNum;
	CString strPortNum;

	enPosition enPos;

	BOOL bRet;

	//m_pCEditPortNum[id]->GetWindowText(strPortNum);
	uiPortNum = SHU_PORT_NUM;

	enPos = GetPosition();

	Log( enNormal, _T("소켓에 [127.0.0.1/%d]로 연결합니다."), uiPortNum );
	if( enOffice == enPos || enMyHome == enPos || enNoNetwork == enPos ) {
		Log( enNormal, _T("소켓에 [127.0.0.1/%d]로 연결합니다."), uiPortNum );
		bRet = m_pConnected->Connect(_T("127.0.0.1"), uiPortNum);
	}
	else {
		Log( enNormal, _T("소켓에 [192.168.0.120/%d]로 연결합니다."), uiPortNum );		// 5010
		bRet = m_pConnected->Connect(_T("192.168.0.120"), uiPortNum);
	}

}

void CShuSIMDlg::Send()
{
	STR_MESSAGE *pTxMessage;

	m_pConnected->Send( m_ptxData, sizeof(STR_MESSAGE) );

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	if( pTxMessage->uiDataLength != 0 ) {
		m_pConnected->Send( & m_ptxData[sizeof(STR_MESSAGE)], pTxMessage->uiDataLength );
	}

	LogTxMessage( m_ptxData );

}


void CShuSIMDlg::InitVar()
{
	m_uiLog = 1;
	

}

void CShuSIMDlg::InitBuffer()
{
	m_pListener = new MyEchoSocket( false );
	m_pConnected = new MyEchoSocket( false );

	m_pListener->SetParentDlg(this);
	m_pConnected->SetParentDlg(this);

	m_ptxData = (char *) malloc(sizeof(char) * 100000);
	m_prxData = (char *) malloc(sizeof(char) * 100000);
}

void CShuSIMDlg::FreeBuffer()
{
	delete m_pListener;
	delete m_pConnected;

	free( m_ptxData );
	free( m_prxData );
}

void CShuSIMDlg::InitLOGListCtrl()
{
	CRect rt;

	m_CListLog.GetWindowRect(&rt);
	m_CListLog.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	//m_CListLog.SetSortable( false );

	// m_CListCtrlLOG.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	m_CListLog.InsertColumn(0, _T("순번"), LVCFMT_LEFT, (int) ( rt.Width()*0.05), -1 );
	m_CListLog.InsertColumn(1, _T("메시지명"), LVCFMT_LEFT, (int) ( rt.Width() * 0.25) , -1);
	m_CListLog.InsertColumn(2, _T("세부 내용"), LVCFMT_LEFT, (int) ( rt.Width() * 0.6 ), -1);
	m_CListLog.InsertColumn(3, _T("에러 상테"), LVCFMT_LEFT, (int) (rt.Width() * 0.09), -1);

}

#define NUM_OF_STATUSBAR			(3)
void CShuSIMDlg::InitStatusBar()
{
	int i;
	int strPartDim[NUM_OF_STATUSBAR];

	CRect rectDlg;

	this->GetWindowRect( rectDlg );

	m_StatusBar.Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0, 0, 0, 0), this, 0 );

	int iWidth = rectDlg.Width() / NUM_OF_STATUSBAR;
	for( i=0 ; i < NUM_OF_STATUSBAR ; ++i ) {
		strPartDim[i] = iWidth * (i+1);
	}

	m_StatusBar.SetParts( NUM_OF_STATUSBAR, strPartDim );

}

void CShuSIMDlg::OnBnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Connect();

}


void CShuSIMDlg::OnBnClickedButtonReqInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//InitButtonST( & m_CButtonEWMode );

	if( m_pConnected != NULL && m_pConnected->m_bConnected == true ) {
		STR_MESSAGE *pTxMessage;
		STR_DATA_CONTENTS *pTxData;

		pTxMessage = (STR_MESSAGE * ) m_ptxData;
		pTxMessage->uiOpcode = RES_INIT;
		pTxMessage->uiDataLength = sizeof(STR_RES_INIT);

		pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );

		pTxData->stResInit.uiReqCode = 0;
		pTxData->stResInit.uiErrorCode = 0;

		Send();

		LogTxMessage( m_ptxData );
	}
}

void CShuSIMDlg::LogTxMessage( void *pData, CString *pStrEtc )
{
	CString strTemp1, strTemp2;

	MakeLogResMessage( & strTemp1, & strTemp2, pData );
	
	//m_pTabThreatDialog->MakeLogTxMessage( & strTemp1, & strTemp2, pData, true );

	InsertItem( & strTemp1, & strTemp2, pStrEtc );

}

void CShuSIMDlg::MakeLogReqMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
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

	case REQ_SETMODE:
		*pstrTemp1 = _T(">>시스템모드 전환 통보");
		*pstrTemp2 = _T("");
		break;

	case REQ_SET_CONFIG:
		*pstrTemp1 = _T(">>수집 파라메터 설정");
		pstrTemp2->Format( _T("M%d, %.1f[MHz], %d[개수], %.1f[ms], %.1f[dBm]"), pstData->stSetMode.uiMode, pstData->stSetMode.fTuneFreq, pstData->stSetMode.coPulseNum, pstData->stSetMode.fColTime, pstData->stSetMode.fThreshold );
		break;

	case REQ_COL_START :
		*pstrTemp1 = _T(">>신호 수집 시작 요구");
		break;

	case REQ_RAWDATA :
		*pstrTemp1 = _T(">>수집 데이터");
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("잘못된 명령[0x%x]입니다."), pstMessage->uiOpcode);
		break;
	}
}

void CShuSIMDlg::MakeLogResMessage( CString *pstrTemp1, CString *pstrTemp2, void *pData )
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

	case RES_SET_CONFIG:
		*pstrTemp1 = _T("<<수집 파라메타 설정 결과 응답");
		pstrTemp2->Format( _T("[%d]"), pstData->uiResult );
		break;

	case RES_COL_START :
		*pstrTemp1 = _T("<<수집시작 응답");
		pstrTemp2->Format( _T("ST[%d],Co[%d],Phase[%d]"), pstData->stColStart.uiStatus, pstData->stColStart.uiCoPulseNum, pstData->stColStart.uiPhase3Num );
		break;

	case RES_RAWDATA_PDW:
		*pstrTemp1 = _T("<<PDW 데이터");
		break;

	case RES_RAWDATA_INTRA:
		*pstrTemp1 = _T("<<INTRA 데이터");
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("잘못된 명령[0x%x]입니다."), pstMessage->uiOpcode);
		break;
	}
}

void CShuSIMDlg::InsertItem( CString *pStrTemp1, CString *pStrTemp2, CString *pStrTemp3 )
{
	CString strNum;

	strNum.Format(_T("%d"), m_uiLog );

	int nIndex = m_CListLog.InsertItem( INT_MAX, strNum, NULL );
	m_CListLog.SetItem(nIndex, 1, LVIF_TEXT, *pStrTemp1, NULL, NULL, NULL, NULL);
	m_CListLog.SetItem(nIndex, 2, LVIF_TEXT, *pStrTemp2, NULL, NULL, NULL, NULL);

	if( pStrTemp3 != NULL ) {
		m_CListLog.SetItem(nIndex, 3, LVIF_TEXT, *pStrTemp3, NULL, NULL, NULL, NULL);
	}

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

void CShuSIMDlg::MakeResultOfSetConfigMessage()
{
	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = RES_SET_CONFIG;
	pTxMessage->uiDataLength = sizeof(int);

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );
	pTxData->uiResult = 0;

}

#define CO_PDW_DATA	(100)
#define CO_INTRA_DATA	(100)

void CShuSIMDlg::MakeResultOfColStartMessage()
{
	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = RES_COL_START;
	pTxMessage->uiDataLength = sizeof(STR_REQ_COL_START);

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );
	pTxData->stColStart.uiStatus = 0;
	pTxData->stColStart.uiCoPulseNum = CO_PDW_DATA;
	pTxData->stColStart.uiPhase3Num = CO_INTRA_DATA;

}

void CShuSIMDlg::MakeResultOfPDWMessage()
{
	int i;

	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = RES_RAWDATA_PDW;
	pTxMessage->uiDataLength = sizeof(STR_RES_PDW_DATA) * CO_PDW_DATA;

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );

	for( i=0 ; i < CO_PDW_DATA ; ++i ) {
		pTxData->stPDWData[i].fFreq = 10000.0;
		pTxData->stPDWData[i].fPA = -50.0;
		pTxData->stPDWData[i].fPW = 1000.0;
		pTxData->stPDWData[i].uiTOA = ( i * 500 );

		pTxData->stPDWData[i].uiIndex = (i+1);
	}


}

void CShuSIMDlg::MakeResultOfIntraMessage()
{
	int i;

	STR_MESSAGE *pTxMessage;
	STR_DATA_CONTENTS *pTxData;

	pTxMessage = (STR_MESSAGE * ) m_ptxData;
	pTxMessage->uiOpcode = RES_RAWDATA_INTRA;
	pTxMessage->uiDataLength = sizeof(STR_RES_INTRA_DATA) * CO_INTRA_DATA;

	pTxData = ( STR_DATA_CONTENTS * ) ( ( char *) m_ptxData + sizeof(STR_MESSAGE) );

	for( i=0 ; i < CO_INTRA_DATA ; ++i ) {
		pTxData->stIntraData[i].fIntraFreq = 1000.0;
	}
}