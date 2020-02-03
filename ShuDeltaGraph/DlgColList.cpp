// DlgColList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgColList.h"
#include "afxdialogex.h"

DWORD WINAPI FuncReceiveLAN( LPVOID lpData );


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

	pApp->SaveProfile( & m_stColList );
}

void CDlgColList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COL_LIST, m_ColList);
	DDX_Control(pDX, IDC_LIST2_RAWDATA, m_CListRawData);
	DDX_Control(pDX, IDC_SPIN_FRQ_LOW, m_CSpinFreqLow);
	DDX_Control(pDX, IDC_SPIN_FRQ_HIGH, m_CSpinFreqHigh);
	DDX_Control(pDX, IDC_SPIN_COL_TIME, m_CSpinColTime);
	DDX_Control(pDX, IDC_SPIN_COL_NUM, m_CSpinColNum);
	DDX_Control(pDX, IDC_SPIN_NUM, m_CSpinNum);
	DDX_Control(pDX, IDC_STATIC_TOTAL_COL_LIST, m_CStaticTotalColList);
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
	int i;

	delete m_pListener;
	delete m_pConnected;

	free( m_ptxData );
	free( m_prxData );
	free( m_pColList );
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
END_MESSAGE_MAP()


// CDlgColList 메시지 처리기입니다.

void CDlgColList::InitButton()
{
	GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( FALSE );

	GetDlgItem(IDC_EDIT_NUM)->EnableWindow( FALSE );
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

	LoadColList();

	pApp->LoadProfile( & m_stColList );

	// 스핀 카트롤 세팅
	m_CSpinNum.SetDecimalPlaces(0);
	m_CSpinNum.SetTrimTrailingZeros(FALSE);
	m_CSpinNum.SetRangeAndDelta( 1, 1000, 1.0 );
	m_CSpinNum.SetPos( (double) 1 );

	m_CSpinFreqLow.SetDecimalPlaces(1);
	m_CSpinFreqLow.SetTrimTrailingZeros(FALSE);
	m_CSpinFreqLow.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinFreqLow.SetPos( (double) m_stColList.fFrqLow );

	m_CSpinFreqHigh.SetDecimalPlaces(1);
	m_CSpinFreqHigh.SetTrimTrailingZeros(FALSE);
	m_CSpinFreqHigh.SetRangeAndDelta( 500, 180000, 10.0 );
	m_CSpinFreqHigh.SetPos( (double) m_stColList.fFrqHigh );

	m_CSpinColTime.SetDecimalPlaces(1);
	m_CSpinColTime.SetTrimTrailingZeros(FALSE);
	m_CSpinColTime.SetRangeAndDelta( 100, 100000, 1.0 );
	m_CSpinColTime.SetPos( (double) m_stColList.fColTime );

	m_CSpinColNum.SetDecimalPlaces(0);
	m_CSpinColNum.SetTrimTrailingZeros(FALSE);
	m_CSpinColNum.SetRangeAndDelta( 10, 1000, 1.0 );
	m_CSpinColNum.SetPos( (double) m_stColList.uiColNumber );

	InitThread();

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
		m_bConnected = true;
		UpdateData(FALSE);

		//CALL_DIALOG( enUnit, OnConnect() );
	}
	else
	{
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(TRUE);
		AfxMessageBox( _T("Cannoot Accept Connection") );
	}

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
	m_bConnected = false;

	//OnBnClickedButtonServerrun();

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
	bool bRet;
	UINT uiPortNum;
	CString strPortNum;

	CString strIPAddress=GetIpAddress();

	uiPortNum = 1000;

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
	m_theThread.Attach( FuncReceiveLAN );
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
		m_bConnected = true;

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
		//m_pParser->MakeLogResMessage( & strTemp1, & strTemp2, (void *) m_prxData, true );

		//(( CTabTranceiverDialog * ) ( m_pDialog[enUnit]))->UpdateData( m_prxData );

		//InsertItem( enUnit, & strTemp1, & strTemp2 );

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
DWORD WINAPI FuncReceiveLAN( LPVOID lpData )
{
	CDlgColList *pDlg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDlgColList * ) pParent->GetParam();

	while( TRUE ) {
		Sleep( 500 );

	}

	return 0;
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

	m_ColList.InsertColumn(0, _T("과제 번호"), LVCFMT_LEFT, (int) ( rt.Width()*0.10), -1 );
	m_ColList.InsertColumn(1, _T("수집 주파수[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.5) , -1);
	m_ColList.InsertColumn(2, _T("수집 개수/시간[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.3 ), -1);
	m_ColList.InsertColumn(3, _T("기타"), LVCFMT_LEFT, (int) (rt.Width() * 0.09), -1);

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
	pstColList->stColItem.fFrqLow = (float) m_CSpinFreqLow.GetPos();
	pstColList->stColItem.fFrqHigh = (float) m_CSpinFreqHigh.GetPos();
	pstColList->stColItem.fColTime = (float) m_CSpinColTime.GetPos();
	pstColList->stColItem.uiColNumber = (UINT) m_CSpinColNum.GetPos();
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
	pstColList->stColItem.fFrqLow = (float) m_CSpinFreqLow.GetPos();
	pstColList->stColItem.fFrqHigh = (float) m_CSpinFreqHigh.GetPos();
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

	strTemp.Format(_T("%.1f/%.1f"), pColList->stColItem.fFrqLow, pColList->stColItem.fFrqHigh );
 	m_ColList.SetItem( nIndex, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f/%d"), pColList->stColItem.fColTime, pColList->stColItem.uiColNumber );
	m_ColList.SetItem( nIndex, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	
 	//m_ColList.SetItemState( -1, 0, LVIS_SELECTED|LVIS_FOCUSED );
 	m_ColList.SetItemState( nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
 	//m_ColList.EnsureVisible( m_uiCoColList, FALSE); 

	SetTotalColList();

	++ m_uiCoColList;

}

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

	CString strTemp;
	STR_COL_LIST stColList;

	m_iSelItem = pNMItemActivate->iItem;
	if( m_iSelItem >= 0 ) {
		GetDlgItem(IDC_BUTTON_MODIFY_LIST)->EnableWindow( TRUE );

		strTemp = m_ColList.GetItemText( m_iSelItem, 0 );
		swscanf_s( strTemp.GetBuffer(), _T("%d"), & stColList.uiNo );
		m_CSpinNum.SetPos( (double) stColList.uiNo );

		strTemp = m_ColList.GetItemText( m_iSelItem, 1 );
		swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & stColList.stColItem.fFrqLow, & stColList.stColItem.fFrqHigh );
		m_CSpinFreqLow.SetPos( (double) stColList.stColItem.fFrqLow );
		m_CSpinFreqHigh.SetPos( (double) stColList.stColItem.fFrqHigh );

		strTemp = m_ColList.GetItemText( m_iSelItem, 2 );
		swscanf_s( strTemp.GetBuffer(), _T("%f/%d"), & stColList.stColItem.fColTime, & stColList.stColItem.uiColNumber );
		m_CSpinColTime.SetPos( (double) stColList.stColItem.fColTime );
		m_CSpinColNum.SetPos( (double) stColList.stColItem.uiColNumber );
	}


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
	strTemp.Format(_T("%d"), pColList->uiNo );
	m_ColList.SetItem( m_iSelItem, 0, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%.1f/%.1f"), pColList->stColItem.fFrqLow, pColList->stColItem.fFrqHigh );
	m_ColList.SetItem( m_iSelItem, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	strTemp.Format(_T("%.1f/%d"), pColList->stColItem.fColTime, pColList->stColItem.uiColNumber );
	m_ColList.SetItem( m_iSelItem, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

}


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
	for (int i = 0; i < m_ColList.GetItemCount(); i++) {
		if( TRUE == m_ColList.GetCheck(i) ) {
			strTemp = m_ColList.GetItemText( m_iSelItem, 0 );
			swscanf_s( strTemp.GetBuffer(), _T("%d"), & stColList.uiNo );
			m_CSpinNum.SetPos( (double) stColList.uiNo );

			strTemp = m_ColList.GetItemText( m_iSelItem, 1 );
			swscanf_s( strTemp.GetBuffer(), _T("%f/%f"), & stColList.stColItem.fFrqLow, & stColList.stColItem.fFrqHigh );
			m_CSpinFreqLow.SetPos( (double) stColList.stColItem.fFrqLow );
			m_CSpinFreqHigh.SetPos( (double) stColList.stColItem.fFrqHigh );

			strTemp = m_ColList.GetItemText( m_iSelItem, 2 );
			swscanf_s( strTemp.GetBuffer(), _T("%f/%d"), & stColList.stColItem.fColTime, & stColList.stColItem.uiColNumber );
			m_CSpinColTime.SetPos( (double) stColList.stColItem.fColTime );
			m_CSpinColNum.SetPos( (double) stColList.stColItem.uiColNumber );
		}
	}

}
