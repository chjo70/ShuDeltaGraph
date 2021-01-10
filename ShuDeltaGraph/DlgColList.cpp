// DlgColList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgColList.h"
#include "afxdialogex.h"

#include "./Anal/Collect/DataFile/DataFile.h"

extern TCHAR g_stColListMode[6][20];

TCHAR g_stRawListType[6][20] = { _T("PDW"), _T("IQ") } ;

extern float _spOneSec;
extern float _spOneMilli;
extern float _spOneMicrosec;
extern float _spOneNanosec;
extern float _spAOAres;
extern float _spPWres;

// CDlgColList ��ȭ �����Դϴ�.

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

	pApp->SaveProfile( & m_stColItem, & m_stRxThreshold );
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:07:51
 * @warning   
 */
void CDlgColList::InitBuffer()
{
	int i;

	m_iSelItem = -1; 

	for( i=0 ; i <= enRSA ; ++i ) {
		m_pListener[i] = new MyEchoSocket( (ENUM_UnitID) i, false );
		m_pListener[i]->SetParentDlg(this);

		m_pConnected[i] = new MyEchoSocket( (ENUM_UnitID) i, false );
		m_pConnected[i]->SetParentDlg(this);
	}

	//m_ptxData = (char *) malloc(sizeof(char) * 100000);
	//m_pColList = ( STR_COL_LIST *) malloc( sizeof(STR_COL_ITEM) * MAX_COL_ITEMS );
	//m_pRawData = ( STR_RAW_DATA * ) malloc( sizeof(STR_RAW_DATA) );
	m_pSonataData = ( STR_SONATA_DATA * ) malloc( sizeof(STR_SONATA_DATA) );

	m_hReceveLAN = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_pDlgRSA = new CDialogRSA( NULL, this );
	m_pDlgSHU = new CDialogSHU( NULL , this );

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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

	m_pDlgRSA->DestroyWindow();
	delete m_pDlgRSA;
	m_pDlgSHU->DestroyWindow();
	delete m_pDlgSHU;

	m_pTip->DestroyWindow();
	delete m_pTip;
}



BEGIN_MESSAGE_MAP(CDlgColList, CDialogEx)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDlgColList::OnHdnItemdblclickListColList)
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_RAWDATA, &CDlgColList::OnLvnItemActivateListRawdata)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDlgColList::OnHdnItemclickListRawdata)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RAWDATA, &CDlgColList::OnNMClickListRawdata)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_RAWDATA, &CDlgColList::OnLvnEndScrollListRawdata)
	ON_WM_VSCROLL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgColList::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CDlgColList �޽��� ó�����Դϴ�.

/**
 * @brief     
 * @param     CButtonST * pCButtonRouteSetup
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 10:09:00
 * @warning   
 */
void CDlgColList::InitButtonST( CButtonST *pCButtonRouteSetup, int iIcon )
{
	short	shBtnColor = 30;

	pCButtonRouteSetup->SetIcon( iIcon, 24, 24, iIcon, 24, 24);
	pCButtonRouteSetup->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:58
 * @warning   
 */
BOOL CDlgColList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_COL_LIST));
	this->SetIcon(hIcon, FALSE);

	Log( enNormal, _T("CDlgColList+++++++++++++++++++++++++++++++++++++++++++") );

	pApp->LoadProfile( & m_stColItem, & m_stRxThreshold );

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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:55
 * @warning   
 */
void CDlgColList::OnAccept( ENUM_UnitID id )
{
	CString strIP;
	UINT port;

	Log( enNormal, _T("Ŭ���̾�Ʈ�� ����ƽ��ϴ�.") );

	if ( m_pListener[id]->Accept( *m_pConnected[id] )) {
		CString strMsg;

		m_pListener[id]->Close();

		m_pConnected[id]->GetSockName(strIP, port);
		//m_status = "Client Connected,IP :" + strIP;
		//m_sConnected.Send("Connected To Server", strlen("Connected To Server"));
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(FALSE);
		strMsg.Format( _T("����:%s/%d"), (char*)(LPCTSTR) strIP, port);
		m_StatusBar.SetText( strMsg, id, 0);
		m_StatusBar.SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );

		Log( enNormal, _T("�� ����(%s/%d) �Ǿ����ϴ�."), (char*)(LPCTSTR) strIP, port);
		
		m_enConnectMode = CONNECTED;
		UpdateData(FALSE);

		InitUnitRes( id );

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
		Log( enError, _T("������ �� �����ε� Accept �� ���ɴϴ�." ) );
		//GetDlgItem(IDC_BUTTON_SERVERRUN)->EnableWindow(TRUE);
		//AfxMessageBox( _T("Cannoot Accept Connection") );
	}

}

/**
 * @brief     
 * @param     bool bEnable
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:52
 * @warning   
 */
void CDlgColList::OnSocketClose( ENUM_UnitID id )
{
	//m_theThread.Stop( true );

	Log( enError, _T("������ �ݽ��ϴ�.") );

	InitVar();

	m_pListener[id]->Close();
	m_pListener[id]->ShutDown();

	m_pConnected[id]->InitVar();
	m_pConnected[id]->Close();

	m_StatusBar.SetText(_T("��� ����"), id, 0);
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:10:36
 * @warning   
 */
void CDlgColList::InitSocketSetting( ENUM_UnitID id )
{
	UINT uiPortNum;
	CString strPortNum;
													  
	CString strIPAddress=GetIpAddress();

	uiPortNum = stPortNum[id];

	Log( enNormal, _T("[%s/%d]�� ������ �����մϴ�."), (char*)(LPCTSTR) strIPAddress, uiPortNum );

	m_pListener[id]->Create( uiPortNum, SOCK_STREAM);
	m_pListener[id]->Listen();

	m_StatusBar.SetText(_T("��� ����"), (int) id, 0);

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:03:20
 * @warning   
 */
void CDlgColList::OnConnect(int nErrorCode, ENUM_UnitID id )
{
	TCHAR szBuffer[100];

	Log( enNormal, _T("Ŭ�󸮾�Ʈ�� ����ƽ��ϴ�.") );

	if (nErrorCode != 0) {
		//AfxMessageBox(_T("������ ������ �� �����ϴ� !!!"));
		m_StatusBar.SetText(_T("��� ����"), 0, 0);
	}
	else {
		UINT uiPortNum;
		CString strIPAddress;

		m_pConnected[id]->GetPeerName( strIPAddress, uiPortNum );

		//Log( enNormal, "%s/%d �� ����Ǿ����ϴ�.", (char*)(LPCTSTR)strIPAddress, uiPortNum );

		wprintf_s( szBuffer, sizeof(szBuffer), _T("����[%s/%d]"), (char*)(LPCTSTR)strIPAddress, uiPortNum );
		m_StatusBar.SetText(szBuffer, 0, 0);

		//GetDlgItem(IDC_BUTTON_CLIENT)->SetWindowText(_T("���� �������"));

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
		*pstrTemp1 = _T(">>�ʱ�ȭ�䱸 ����");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
		break;

	case RES_SET_CONFIG:
		*pstrTemp1 = _T(">>���� �Ķ��Ÿ ���� ��� ����");
		pstrTemp2->Format( _T("[%d]"), pstData->uiResult );
		break;

	case RES_COL_START :
		*pstrTemp1 = _T(">>�������� ����");
		pstrTemp2->Format( _T("ST[%d],Co[%d],Phase[%d]"), pstData->stColStart.uiStatus, pstData->stColStart.uiCoPulseNum, pstData->stColStart.uiPhase3Num );
		break;

	case RES_RAWDATA_PDW:
		*pstrTemp1 = _T(">>PDW ������");
		*pstrTemp2 = _T(" ");
		break;

	case RES_RAWDATA_INTRA:
		*pstrTemp1 = _T(">>FMOP ������");
		*pstrTemp2 = _T(" ");
		break;

	case RES_RAWDATA_IQ :
		pstrTemp1->Format( _T(">>IQ ������[%d]"), pstMessage->uiDataLength );
		*pstrTemp2 = _T(" ");
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("�߸��� ���[0x%x]�Դϴ�."), pstMessage->uiOpcode);
		break;
	}
}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 10:10:30
 * @warning   
 */
void CDlgColList::OnReceive( char *pData )
{
// 	CString strTemp1, strTemp2;
// 
// 	MakeLogResMessage( & strTemp1, & strTemp2, (void *) pData );
// 
// 	UpdateResultData( pData );
// 
// 	InsertItem( & strTemp1, & strTemp2 );

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/09 23:30:59
 * @warning   
 */
void CDlgColList::InitTab()
{
	CRect rect;

	m_Tab.InsertItem( 0, _T("�����������ġ"));
	m_Tab.InsertItem( 1, _T("���̴�¡ǥ�м���ġ"));

	m_Tab.SetCurSel( 0 );

	m_Tab.GetWindowRect( & rect );

	//m_pDlgRSA = new CDialogRSA( NULL, this );
	m_pDlgRSA->Create( IDD_DIALOG_RSA, & m_Tab );
	m_pDlgRSA->MoveWindow( 0, 25, rect.Width(), rect.Height() );
	m_pDlgRSA->ShowWindow( SW_HIDE );

	//m_pDlgSHU = new CDialogSHU( NULL , this );
	m_pDlgSHU->Create( IDD_DIALOG_SHU, & m_Tab );
	m_pDlgSHU->MoveWindow( 0, 25, rect.Width(), rect.Height() );
	m_pDlgSHU->ShowWindow( SW_SHOW );

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
		m_RawList.InsertColumn(i++, _T("����"), LVCFMT_LEFT, (int) ( rt.Width()*0.05), -1 );
		m_RawList.InsertColumn(i++, _T("���� ��ȣ"), LVCFMT_LEFT, (int) ( rt.Width()*0.04), -1 );
		m_RawList.InsertColumn(i++, _T("����"), LVCFMT_LEFT, (int) ( rt.Width() * 0.05) , -1);
		m_RawList.InsertColumn(i++, _T("���"), LVCFMT_LEFT, (int) ( rt.Width() * 0.10) , -1);
		m_RawList.InsertColumn(i++, _T("���� �߽� ���ļ�[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.15 ), -1);
		m_RawList.InsertColumn(i++, _T("���� ��������/�ð�[ms]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.1 ), -1);
		m_RawList.InsertColumn(i++, _T("��� ���ļ�[MHz]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.1 ), -1);
		m_RawList.InsertColumn(i++, _T("��� DTOA[us]"), LVCFMT_LEFT, (int) ( rt.Width() * 0.1 ), -1);
		m_RawList.InsertColumn(i++, _T("���� ���ϸ�"), LVCFMT_LEFT, (int) (rt.Width() * 0.19), -1);

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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:02:54
 * @warning   
 */
void CDlgColList::OnHdnItemdblclickListColList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 16:58:48
 * @warning   
 */


void CDlgColList::ActivateGraph( BOOL bEnable )
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	pApp->ActivateGraph( bEnable );
}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:14
 * @warning   
 */
void CDlgColList::Send( ENUM_UnitID id, char *ptxData )
{
	STR_MESSAGE *pTxMessage;

	m_pConnected[id]->Send( ptxData, sizeof(STR_MESSAGE) );

	pTxMessage = (STR_MESSAGE * ) ptxData;
	if( pTxMessage->uiDataLength != 0 ) {
		m_pConnected[id]->Send( & ptxData[sizeof(STR_MESSAGE)], pTxMessage->uiDataLength );
	}

	if( id == enSHU )
		m_pDlgSHU->LogTxMessage( ptxData );
	else
		m_pDlgRSA->LogTxMessage( ptxData );

	m_bCompleteOnReceive = false;

}

/**
 * @brief     
 * @param     void * pData
 * @param     CString * pStrEtc
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
		*pstrTemp1 = _T("<<�ʱ�ȭ �䱸");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
		break;

	case REQ_SETMODE:
		*pstrTemp1 = _T("<<�ý��۸�� ��ȯ �뺸");
		*pstrTemp2 = _T("");
		break;

	case REQ_SET_CONFIG:
		*pstrTemp1 = _T("<<���� �Ķ���� ����");
		pstrTemp2->Format( _T("M%d, %.1f[MHz], %d[����], %.1f[ms], %.1f[dBm]"), pstData->stSetMode.uiMode, pstData->stSetMode.fTuneFreq, pstData->stSetMode.coPulseNum, pstData->stSetMode.fColTime, pstData->stSetMode.fThreshold );
		break;

	case REQ_COL_START :
		*pstrTemp1 = _T("<<��ȣ ���� ���� �䱸");
		break;

	case REQ_RAWDATA :
		*pstrTemp1 = _T("<<���� ������ �䱸");
		break;

	case REQ_SET_IQCONFIG :
		*pstrTemp1 = _T("<<IQ ���� ���� ��û");
		pstrTemp2->Format( _T("���%d"), pstData->uiMode );
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("�߸��� ���[0x%x]�Դϴ�."), pstMessage->uiOpcode);
		break;
	}
}

/**
 * @brief     
 * @param     CString * pStrTemp1
 * @param     CString * pStrTemp2
 * @param     CString * pStrTemp3
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:26
 * @warning   
 */
void CDlgColList::UpdateResultData( char *pData )
{
// 	STR_MESSAGE *pstMessage;
// 	STR_DATA_CONTENTS *pstData;
// 
// 	int iCnt=0;
// 
// 	pstMessage = (STR_MESSAGE *) pData;
// 	pstData = (STR_DATA_CONTENTS * ) ( ( char *) pData + sizeof(STR_MESSAGE) );
// 
// 	switch (pstMessage->uiOpcode) {
// 	case RES_INIT:
// // 		if( pstData->stResInit.uiReqCode == 0 && pstData->stResInit.uiErrorCode == 0 )
// // 			m_CButtonInit.SetIcon( IDI_ICON_OK, 24, 24, IDI_ICON_OK, 24, 24);
// // 		else
// // 			m_CButtonInit.SetIcon( IDI_ICON_ERROR, 24, 24, IDI_ICON_ERROR, 24, 24);
// // 
// // 		iCnt += wsprintf( & szBuffer[0], _T("Opcode[0x%x], ErrorCode[0x%x]\n") , pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
// // 		UpdateToolTip( szBuffer, & m_CButtonInit );
// 		break;
// 
// 	case RES_SET_CONFIG :
// 		break;
// 
// 	case RES_COL_START :
// 		memcpy( & m_stResCol, pstData, sizeof(STR_RES_COL_START) );
// 		break;
// 
// 	case RES_RAWDATA_PDW :
// 		InsertPDWRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_PDW_DATA), m_uiColList, & m_stColList, m_pRawData );
// 		break;
// 
// 	case RES_RAWDATA_INTRA :
// 		InsertIntraRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_INTRA_DATA) );
// 		break;
// 
// 	case RES_RAWDATA_IQ:
// 		InsertIQRawDataItem( pstData, pstMessage->uiDataLength / sizeof(STR_RES_IQ_DATA), & m_stColList, m_pRawData );
// 		break;
// 
// 	default :
// 		break;
// 	}
}

/**
 * @brief     
 * @param     TCHAR * pszBuffer
 * @param     CWnd * pDlgItem
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 22:03:31
 * @warning   
 */
BOOL CDlgColList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/03 21:51:39
 * @warning   
 */
void CDlgColList::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
 	if(nID == SC_CLOSE)	
 	{
// 		GetColItem( & m_stColItem );
// 		//�����ư ���� ��
 	}
// 	else if(nID == SC_MAXIMIZE)
// 	{ 
// 		//�ִ�ȭ ��ư ���� ��
// 	}
	else if(nID == SC_MINIMIZE)
	{ 
		CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();
		pApp->MinimuzeColList();
		//�ּ�ȭ ��ư ���� ��
	}
 	else if(nID == SC_RESTORE) 
 	{ 
 		//���� ��Ȳ����
 	} 
 	CDialogEx::OnSysCommand(nID, lParam);
}

void CDlgColList::InsertIntraRawDataItem( STR_DATA_CONTENTS *pstData, int iItem )
{
	Log( enNormal, _T("\n ��Ʈ�� �����͸� ó���մϴ�." ) );

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:09
 * @warning   
 */
void CDlgColList::ViewGraph( UINT uiOpCode )
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	if( uiOpCode == RES_RAWDATA_PDW || uiOpCode == RES_RAWDATA_ZPDW ) {
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:12
 * @warning   
 */
void CDlgColList::InsertPDWRawDataItem( STR_DATA_CONTENTS *pstData, int iItem, int uiColList, STR_COL_LIST *pColList, STR_RAW_DATA *pRawData, ENUM_UnitID enID, STR_RES_COL_START *pResColStart )
{
	int i, nIndex;
	CString strTemp;

	Log( enNormal, _T("PDW ������[%d]�� �Ϸ� ó���մϴ�." ), iItem );

	// �����͸� ü��� PDW ���Ϸ� ��ȯ�Ͽ� �����մϴ�.
	MakePDWFile( iItem, uiColList, pRawData, enID, (ENUM_BoardID) pResColStart->uiBoardID );

	// ���â �߰�
 	strTemp.Format(_T("%d"), m_uiCoRawData );
 	nIndex = m_RawList.InsertItem( INT_MAX, strTemp, NULL );

	i = 1;
	strTemp.Format(_T("%d"), pColList->stColItem.uiNo );
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp = g_stRawListType[enPDW_DataType];
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	strTemp.Format(_T("%s"), g_stColListMode[pColList->stColItem.enMode] );
	m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	if( enID == enSHU ) {
		strTemp.Format(_T("%.1f"), pColList->stColItem.fCenterFreq );
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		strTemp.Format(_T("%d/%.1f"), pColList->stColItem.uiColNumber, pColList->stColItem.fColTime ); 
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		// PDW ���� ��� �� ����
		strTemp.Format(_T("%.1f"), m_stStatPDW.fFreqMean ); 
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		strTemp.Format(_T("%.1f"), m_stStatPDW.fDtoaMean ); 
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, m_strFilename, NULL, NULL, NULL, NULL);

	}
	else {
		strTemp.Format(_T("%.2f/%.2f"), pColList->stColItem.fFreqLow, pColList->stColItem.fFreqHgh );
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		strTemp.Format(_T("%d/%.1f"), pColList->stColItem.uiColNumber, pColList->stColItem.fColTime ); 
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		// PDW ���� ��� �� ����
		strTemp.Format(_T("%.3f"), m_stStatPDW.fFreqMean ); 
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		strTemp.Format(_T("%.1f"), m_stStatPDW.fDtoaMean ); 
		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		m_RawList.SetItem( nIndex, i++, LVIF_TEXT, m_strFilename, NULL, NULL, NULL, NULL);
	}

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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:17
 * @warning   
 */
void CDlgColList::MakePDWFile( int iItem, int uiColList, STR_RAW_DATA *pRawData, ENUM_UnitID enID, ENUM_BoardID enSubBoardID )
{
	CString strPathname, strFolderName;
	SYSTEMTIME cur_time;

	GetLocalTime( & cur_time );

	// ���� ����
	strFolderName.Format( _T("%s//%04d_%02d_%02d//PDW"), RAWDATA_DIRECTORY, cur_time.wYear, cur_time.wMonth, cur_time.wDay ); 
	CreateDir( (TCHAR*) (LPCTSTR) strFolderName );

	m_strFilename.Format( _T("%04d_%02d_%02d %02d_%02d_%02d_%03ld." ), cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds );
	if( enID == enSHU )
		m_strFilename += "spdw";
	else if( enID == enRSA )
		m_strFilename += "pdw";
	else if( enID == enZPocketSonata )
		m_strFilename += "zpdw";
	else {

	}

	strPathname = strFolderName + _T("//") + m_strFilename;

	ConvertRAWData( iItem, en_PDW_DATA, uiColList, pRawData, enID, enSubBoardID );

	if( enID == enSHU || enID == enRSA )
		m_theDataFile.SaveDataFile( (char *) (LPCTSTR) strPathname, (void *) m_pSonataData->unRawData.stPDWData, iItem*sizeof(TNEW_PDW), en_SONATA, en_PDW_DATA );
	else {
		STR_PDWFILE_HEADER stPDWFileHeader;

		stPDWFileHeader.uiSearchBandNo = 0;
		stPDWFileHeader.uiSignalDeletingStatus = 0;
		stPDWFileHeader.uiSignalCount = iItem;
		stPDWFileHeader.uiBoardID = enSubBoardID;
		m_theDataFile.SaveDataFile( (char *) (LPCTSTR) strPathname, (void * ) & pRawData->unRawData, iItem*sizeof(DMAPDW), en_ZPOCKETSONATA, en_PDW_DATA, & stPDWFileHeader, sizeof(STR_PDWFILE_HEADER) );
	}

}



/**
 * @brief     
 * @param     int iItem
 * @param     ENUM_DataType enDataType
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/23 11:55:24
 * @warning   
 */
void CDlgColList::ConvertRAWData( int iItem, ENUM_DataType enDataType, int uiColList, STR_RAW_DATA *pRawData, ENUM_UnitID enID, ENUM_BoardID enSUbBoardID )
{
	int i;
	bool bValid;

	TNEW_PDW *pNEW_PDW;

	int iBc=0, iCoPdw=0;
	_TOA ullTOA, ullDTOA, ullPreTOA;
	UINT uiTOA, uiDTOA, uiPreTOA, uiFreq, uiPW, uiAOA;

	if( enID == enSHU ) {
		STR_RES_PDW_DATA *pPDWData;

		switch( enDataType ) {
			case en_PDW_DATA :
				// PDW ���ġ �ʱ�ȭ
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
					uiTOA = pPDWData->uiTOA;			// IDIV( pPDWData->uiTOA * 5, 2 );

					if( i == 0 ) {
						uiPreTOA = 0;
						uiDTOA = 0;
					}
					else {
						uiDTOA = uiTOA - uiPreTOA;
						if( (int) uiDTOA > 0 ) {
							//m_stStatPDW.fDtoaMean += uiDTOA;
						}
					}

					if( (int) uiDTOA < 0 ) {
						bValid = false;
					}
					else {
						ullPreTOA = uiTOA;
						bValid = true;
					}

					if( bValid == true ) {
						m_stStatPDW.fDtoaMean += uiDTOA;

						pNEW_PDW->item.dv = SONATA::uiPDW_DV;
						pNEW_PDW->item.stat = SONATA::uiPDW_NORMAL;

						pNEW_PDW->item.pmop = 0;
						pNEW_PDW->item.freq_diff = 0;

// 						uiAOA = (int) pPDWData->fAOA;		// ConvertAOA( pPDWData->fAOA );
// 						pNEW_PDW->item.direction_h = uiAOA >> 8;
// 						pNEW_PDW->item.direction_l = uiAOA & 0x00FF;

						// TOA ����
						//uiTOA = pPDWData->uiTOA;			// IDIV( pPDWData->uiTOA * 5, 2 );
						pNEW_PDW->item.toa_4   = ( uiTOA >> 24 ) & 0xFF;	// TOA �� �Է� 
						pNEW_PDW->item.toa_3   = ( uiTOA >> 16 ) & 0xFF;
						pNEW_PDW->item.toa_2   = ( uiTOA >> 8  ) & 0xFF;
						pNEW_PDW->item.toa_1   = uiTOA & 0xFF;

						// ���ļ�
						iBc = GetFreqBand( (UINT) ( pPDWData->fFreq + 0.5 ) );
						uiFreq = ConvertFreq( pPDWData->fFreq, iBc );
						pNEW_PDW->item.band = iBc - 1;
						pNEW_PDW->item.frequency_l  = uiFreq & 0xFF;
						pNEW_PDW->item.frequency_h  = ( uiFreq >> 8 ) & 0x1F;

						// ��ȣ����
						pNEW_PDW->item.amplitude = ConvertPA( pPDWData->fPA );

						// �޽���
						uiPW = ConvertPW( pPDWData->fPW*(float) 1000. );
						pNEW_PDW->item.pulse_width_l	= uiPW & 0xFF ;		// �޽��� �� �Է� 
						pNEW_PDW->item.pulse_width_h	= ( uiPW >> 8 ) & 0x07;

						if( iCoPdw < 10 ) {
							if( iCoPdw == 0 ) {
								Log( enNormal, _T("") );
								Log( enNormal, _T("PDW �������Դϴ�.") );
							}
							Log( enNormal, _T("Idx[%d] F[%.1f], PA[%.1f], PW[%.1f]") , iCoPdw, pPDWData->fFreq, pPDWData->fPA, pPDWData->fPW );

						}

						//
						//////////////////////////////////////////////////////////////////////////
						m_stStatPDW.fFreqMean += pPDWData->fFreq;

						++ pNEW_PDW;
						++ iCoPdw;
					
					}

					++ pPDWData;
				}
				m_pSonataData->uiItem = iCoPdw;
				m_pSonataData->enDataType = en_PDW_DATA;

				m_pSonataData->uiNo = 1;	// m_pColList[uiColList].stColItem.uiNo;

				//
				//////////////////////////////////////////////////////////////////////////
				m_stStatPDW.fFreqMean /= iCoPdw;

				m_stStatPDW.fDtoaMean /= ( iCoPdw - 1 );
				m_stStatPDW.fDtoaMean = FDIV( m_stStatPDW.fDtoaMean, _spOneMicrosec );
				//m_stStatPDW.fDtoaMean = FDIV( m_stStatPDW.fDtoaMean * 20., 1000. );

				break;

			case en_IQ_DATA :
				memcpy( m_pSonataData->unRawData.stIQData, & pRawData->unRawData.stIQData, iItem*sizeof(TNEW_IQ) );
				m_pSonataData->uiItem = iItem;
				m_pSonataData->enDataType = en_IQ_DATA;

				//for(int i=0; i<16384; i++)
				//{
				//	if(m_pSonataData->unRawData.stIQData[i].sI > 32768) m_pSonataData->unRawData.stIQData[i].sI -= 65536;
				//	if(m_pSonataData->unRawData.stIQData[i].sQ > 32768) m_pSonataData->unRawData.stIQData[i].sQ -= 65536;

				//	Log( enNormal, _T("%d = [0x%04X]  [0x%04X]\n") , 
				//		i, m_pSonataData->unRawData.stIQData[i].sI, m_pSonataData->unRawData.stIQData[i].sQ); 

				//	
				//	//Log( enNormal, _T("new = I[0x%04X : %d]Q[0x%04X : %d] ") , 
				//	//	m_pSonataData->unRawData.stIQData[i].sI, m_pSonataData->unRawData.stIQData[i].sI,
				//	//	m_pSonataData->unRawData.stIQData[i].sQ, m_pSonataData->unRawData.stIQData[i].sQ); 
				//}
				break;

			default :
				break;
		}
	}
	else if( enID == enRSA ) {
		STR_RES_PDW_DATA_RSA *pPDWData;


		// PDW ���ġ �ʱ�ȭ
		m_stStatPDW.fFreqMean = 0;
		m_stStatPDW.fFreqMin = FREQ_MAX;
		m_stStatPDW.fFreqMax = FREQ_MIN;

		m_stStatPDW.fDtoaMean = 0;
		m_stStatPDW.fDtoaMin = (float) 99999999.999;
		m_stStatPDW.fDtoaMax = 0;

		pPDWData = & pRawData->unRawData.stRSAPDWData[0];
		pNEW_PDW = & m_pSonataData->unRawData.stPDWData[0];
		memset( pNEW_PDW, 0, sizeof(TNEW_PDW) * iItem );
		for( i=0 ; i < iItem ; ++i ) {
			uiTOA = pPDWData->uiTOA;			// IDIV( pPDWData->uiTOA * 5, 2 );

			if( i == 0 ) {
				uiPreTOA = 0;
				uiDTOA = 0;
			}
			else {
				uiDTOA = uiTOA - uiPreTOA;
				if( (int) uiDTOA > 0 ) {
					//m_stStatPDW.fDtoaMean += uiDTOA;
				}
			}

			if( (int) uiDTOA < 0 ) {
				bValid = false;
			}
			else {
				uiPreTOA = uiTOA;
				bValid = true;
			}

			if( bValid == true ) {
				m_stStatPDW.fDtoaMean += uiDTOA;

				pNEW_PDW->item.dv = SONATA::uiPDW_DV;
				pNEW_PDW->item.stat = SONATA::uiPDW_NORMAL;

				pNEW_PDW->item.pmop = 0;
				pNEW_PDW->item.freq_diff = 0;

				uiAOA = (int) pPDWData->fAOA;		// ConvertAOA( pPDWData->fAOA );
				pNEW_PDW->item.direction_h = uiAOA >> 8;
				pNEW_PDW->item.direction_l = uiAOA & 0x00FF;

				// TOA ����
				//uiTOA = pPDWData->uiTOA;			// IDIV( pPDWData->uiTOA * 5, 2 );
				pNEW_PDW->item.toa_4   = ( uiTOA >> 24 ) & 0xFF;	// TOA �� �Է� 
				pNEW_PDW->item.toa_3   = ( uiTOA >> 16 ) & 0xFF;
				pNEW_PDW->item.toa_2   = ( uiTOA >> 8  ) & 0xFF;
				pNEW_PDW->item.toa_1   = uiTOA & 0xFF;

				// ���ļ�
				iBc = GetFreqBand( (UINT) ( pPDWData->fFreq + 0.5 ) );
				uiFreq = ConvertFreq( pPDWData->fFreq, iBc );
				pNEW_PDW->item.band = iBc - 1;
				pNEW_PDW->item.frequency_l  = uiFreq & 0xFF;
				pNEW_PDW->item.frequency_h  = ( uiFreq >> 8 ) & 0x1F;

				// ��ȣ����
				pNEW_PDW->item.amplitude = ConvertPA( pPDWData->fPA );

				// ��ȣ����
				uiPW = ConvertPW( pPDWData->fPW );
				pNEW_PDW->item.pulse_width_l	= uiPW & 0xFF ;		// �޽��� �� �Է� 
				pNEW_PDW->item.pulse_width_h	= ( uiPW >> 8 ) & 0x07;

				//
				//////////////////////////////////////////////////////////////////////////
				m_stStatPDW.fFreqMean += pPDWData->fFreq;

				++ pNEW_PDW;
				++ iCoPdw;

			}

			++ pPDWData;
		}
		m_pSonataData->uiItem = iCoPdw;
		m_pSonataData->enDataType = en_PDW_DATA;

		m_pSonataData->uiNo = 1;	// m_pColList[uiColList].stColItem.uiNo;

		//
		//////////////////////////////////////////////////////////////////////////
		m_stStatPDW.fFreqMean /= iCoPdw;

		m_stStatPDW.fDtoaMean /= ( iCoPdw - 1 );
		m_stStatPDW.fDtoaMean = FDIV( m_stStatPDW.fDtoaMean, _spOneMicrosec );
		//m_stStatPDW.fDtoaMean = FDIV( m_stStatPDW.fDtoaMean * 20., 1000. );
	}

	else if( enID == enZPocketSonata ) {
		DMAPDW *pPDWData;
		DMAPDW *pSonataPDWData;

		UINT uiCh;

		// PDW ���ġ �ʱ�ȭ
		m_stStatPDW.fFreqMean = 0;
		m_stStatPDW.fFreqMin = FREQ_MAX;
		m_stStatPDW.fFreqMax = FREQ_MIN;

		m_stStatPDW.fDtoaMean = 0;
		m_stStatPDW.fDtoaMin = (float) 99999999.999;
		m_stStatPDW.fDtoaMax = 0;

		pPDWData = & pRawData->unRawData.stZPocketPDWData[0];
		pSonataPDWData = & m_pSonataData->unRawData.stZPDWData[0];
		for( i=0 ; i < iItem ; ++i ) {
			ullTOA = ( pPDWData->uPDW.uniPdw_toa_edge.stPdw_toa_edge.toa_H << 16 ) | pPDWData->uPDW.uniPdw_freq_toa.stPdw_freq_toa.toa_L;			// IDIV( pPDWData->uiTOA * 5, 2 );

			if( i == 0 ) {
				ullPreTOA = 0;
				ullDTOA = 0;
			}
			else {
				ullDTOA = ullTOA - ullPreTOA;
				if( (long long int) ullDTOA > 0 ) {
					//m_stStatPDW.fDtoaMean += uiDTOA;
				}
			}

			if( (long long int) ullDTOA < 0 ) {
				bValid = false;
			}
			else {
				ullPreTOA = ullTOA;
				bValid = true;
			}

			if( bValid == true ) {
				m_stStatPDW.fDtoaMean += (float) ullDTOA;

				memcpy( pSonataPDWData, pPDWData, sizeof(DMAPDW) );

				m_stStatPDW.fFreqMean += pPDWData->uPDW.uniPdw_freq_toa.stPdw_freq_toa.frequency_H << 8 | pPDWData->uPDW.uniPdw_pw_freq.stPdw_pw_freq.frequency_L;

				++ iCoPdw;
				++ pSonataPDWData;

			}

			++ pPDWData;
		}
		m_pSonataData->uiItem = iCoPdw;
		m_pSonataData->enDataType = en_PDW_DATA;

		m_pSonataData->uiNo = 1;	// m_pColList[uiColList].stColItem.uiNo;

		//
		//////////////////////////////////////////////////////////////////////////
		m_stStatPDW.fFreqMean /= iCoPdw;
		uiCh = pPDWData->uPDW.uniPdw_freq_toa.stPdw_freq_toa.pdw_phch;
		m_stStatPDW.fFreqMean = CPOCKETSONATAPDW::DecodeFREQ( (int) m_stStatPDW.fFreqMean, uiCh, enSUbBoardID );

		m_stStatPDW.fDtoaMean /= ( iCoPdw - 1 );
		m_stStatPDW.fDtoaMean = CPOCKETSONATAPDW::DecodeTOAus( (_TOA) m_stStatPDW.fDtoaMean );
		//m_stStatPDW.fDtoaMean = FDIV( m_stStatPDW.fDtoaMean * 20., 1000. );
	}
}

UINT CDlgColList::ConvertPA( float fPA )
{
	UINT uiPA;

	fPA = ( fPA - gPaRes[0].offset ) / gPaRes[0].res;
	uiPA = UADD( fPA, 0.5 );

	return uiPA;
}

UINT CDlgColList::ConvertAOA( float fAOA )
{
	UINT uiAOA;

	uiAOA = IMUL( fAOA, _spAOAres );

	return uiAOA;
}

UINT CDlgColList::ConvertPW( float fPW )
{
	UINT uiPW;

	//uiPW = (UINT) ( ( fPW * _spOneMicrosec ) + 0.5 );
	//uiPW = (UINT) ( ( fPW * 20 ) + 0.5 );
	uiPW = UDIV( fPW, _spPWres );
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
		Log( enError, _T("�Է� ���ļ�[%d]�� ���ؼ� �뿪�� �߸� ������ϴ�.") , uiFreqMHz );
	}

	return( i );
}

/**
 * @brief     
 * @param     int iItem
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 22:34:30
 * @warning   
 */
void CDlgColList::MakeIQFile( int iItem, STR_RAW_DATA *pRawData )
{
	CString strPathname, strFolderName;
	SYSTEMTIME cur_time;

	GetLocalTime( & cur_time );

	// ���� ����
	strFolderName.Format( _T("%s//%04d_%02d_%02d//IQ"), RAWDATA_DIRECTORY, cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds ); 
	CreateDir( (TCHAR *) (LPCTSTR) strFolderName );

	m_strFilename.Format( _T("%04d_%02d_%02d %02d_%02d_%02d_%03ld.iq" ), cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds );
	strPathname = strFolderName + _T("//") + m_strFilename;

	ConvertRAWData( iItem, en_IQ_DATA, m_uiColList, pRawData, enSHU );

	// ������ �� ���� ������ �����ؼ� �����ؾ� ��.
	STR_IQ_HEADER stIQHeader;

	//stIQHeader.enMode = pRawData->stColList.stColItem.enMode;
	stIQHeader.fCenterFreq = pRawData->stColList.stColItem.fCenterFreq;
	stIQHeader.fColTime = pRawData->stColList.stColItem.fColTime;
	stIQHeader.uiColNumber = pRawData->stColList.stColItem.uiColNumber;
	stIQHeader.fThreshold = pRawData->stColList.stColItem.fThreshold;
	m_theDataFile.SaveDataFile( (char*)(LPCTSTR) strPathname, (void *) pRawData->unRawData.stIQData, iItem, en_SONATA, en_IQ_DATA, & stIQHeader, sizeof(STR_IQ_HEADER) );

}

/**
 * @brief     
 * @param     STR_DATA_CONTENTS * pstData
 * @param     int iItem
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 22:34:32
 * @warning   
 */
void CDlgColList::InsertIQRawDataItem( STR_DATA_CONTENTS *pstData, int iItem, STR_COL_LIST *pColList, STR_RAW_DATA *pRawData )
{
	int nIndex;

	CString strTemp;

	Log( enNormal, _T("%d ���� IQ �����͸� ó���մϴ�." ), iItem );

	memcpy( & pRawData->unRawData.stIQData, & pstData->stIQData, iItem*sizeof(STR_RES_IQ_DATA) );

	// �����͸� ü��� PDW ���Ϸ� ��ȯ�Ͽ� �����մϴ�.
	MakeIQFile( iItem, pRawData );

	// ���â �߰�
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/08 23:33:30
 * @warning   
 */
void CDlgColList::OnLvnItemActivateListRawdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	m_bClickedOfRawDataList = true;
}


void CDlgColList::OnNMClickListRawdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	// �ڵ� ��ũ�� ��� OFF
	m_bClickedOfRawDataList = true;

	if( m_iClickedItem >= 0 ) {
		m_RawList.SetItemBkColor( m_iClickedItem, -1, ::GetSysColor(COLOR_WINDOW), FALSE );
	}

	m_iClickedItem = pNMItemActivate->iItem;

	m_RawList.SetItemBkColor( pNMItemActivate->iItem, -1, ::GetSysColor(COLOR_WINDOWFRAME) );
}


/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/15 14:24:40
 * @warning   
 */
 void CDlgColList::OnLvnEndScrollListRawdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Internet Explorer 5.5 �̻��� �ʿ��մϴ�.
	// _WIN32_IE ��ȣ�� 0x0560���� ũ�ų� ���ƾ� �մϴ�.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


/**
 * @brief     
 * @param     UINT nSBCode
 * @param     UINT nPos
 * @param     CScrollBar * pScrollBar
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/15 14:24:53
 * @warning   
 */
 void CDlgColList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


/**
 * @brief     
 * @param     NMHDR * pNMHDR
 * @param     LRESULT * pResult
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/09 23:36:48
 * @warning   
 */
void CDlgColList::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

/**
 * @brief     
 * @param     enUnitID enID
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/15 14:24:28
 * @warning   
 */
 void CDlgColList::InitUnitRes( ENUM_UnitID enID )
{
	if( enID == enSHU ) {
		_spOneSec = 50000000.;
		_spOneMilli = FDIV( _spOneSec, 1000. );
		_spOneMicrosec = FDIV( _spOneMilli, 1000. );
		_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

		_spPWres = _spOneMicrosec;
	}
	else if( enID == enZPocketSonata ) {
		_spOneSec = 20000000;
		_spOneMilli = FDIV( _spOneSec, 1000. );
		_spOneMicrosec = FDIV( _spOneMilli, 1000. );
		_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

		_spPWres = _spOneMicrosec;
	}
	else {
		_spOneSec = 20000000.;
		_spOneMilli = FDIV( _spOneSec, 1000. );
		_spOneMicrosec = FDIV( _spOneMilli, 1000. );
		_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

		_spPWres = FDIV( 50, 1000 );		// us ����
	}
}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/05/15 14:24:30
 * @warning   
 */
 void CDlgColList::ClearRawDataList()
{
	m_RawList.DeleteAllItems();
}