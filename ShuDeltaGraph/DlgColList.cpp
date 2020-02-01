// DlgColList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgColList.h"
#include "afxdialogex.h"

DWORD WINAPI FuncReceiveLAN( LPVOID lpData );


// CDlgColList ��ȭ �����Դϴ�.

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
	DDX_Control(pDX, IDC_EDIT_FREQ_LOW, m_CEditFrqLow);
	DDX_Control(pDX, IDC_EDIT_FREQ_HIGH, m_CEditFrqHigh);
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

	m_pListener = new MyEchoSocket( false );
	m_pConnected = new MyEchoSocket( false );

	m_pListener->SetParentDlg(this);
	m_pConnected->SetParentDlg(this);

	m_ptxData = (char *) malloc(sizeof(char) * 100000);

	m_prxData = (char *)malloc(sizeof(char) * 100000);

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

	delete m_pListener;
	delete m_pConnected;

	free( m_ptxData );
	free(m_prxData);
}



BEGIN_MESSAGE_MAP(CDlgColList, CDialogEx)
END_MESSAGE_MAP()


// CDlgColList �޽��� ó�����Դϴ�.

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

	InitBuffer();
	InitStatusBar();
	InitListCtrl();

	pApp->LoadProfile( & m_stColList );

	m_CEditFrqLow.SetDecimalPlaces(0);
	m_CEditFrqLow.SetTrimTrailingZeros(FALSE);
	m_CEditFrqLow.SetRangeAndDelta( 500, 180000, 1.0 );
	m_CEditFrqLow.SetPos( m_stColList.fFrqLow );

	m_CEditFrqHigh.SetDecimalPlaces(0);
	m_CEditFrqHigh.SetTrimTrailingZeros(FALSE);
	m_CEditFrqHigh.SetRangeAndDelta( 500, 180000, 1.0 );
	m_CEditFrqHigh.SetPos( m_stColList.fFrqHigh );

	InitThread();

	InitSocketSetting();

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
		strMsg.Format( _T("����:%s/%d"), (char*)(LPCTSTR) strIP, port);
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 13:37:52
 * @warning   
 */
void CDlgColList::OnClose()
{

	//TRACE( "%s(%d��) ������ �ݽ��ϴ�." ,  g_szUnit[enUnit], enUnit );
	//Log( enError, "%s(%d��) ������ �ݽ��ϴ�." ,  g_szUnit[enUnit], enUnit );

	m_pListener->Close();
	m_pListener->ShutDown();

	m_pConnected->Close();

	m_StatusBar.SetText(_T("��� ����"), 0, 0);
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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

	Log( enNormal, _T("[%s/%d]�� ������ �����մϴ�."), strIPAddress, uiPortNum );

	m_pListener->Create( uiPortNum, SOCK_STREAM);
	m_pListener->Listen();

	m_StatusBar.SetText(_T("��� ����"), (int) 0, 0);

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
	m_theThread.Attach( FuncReceiveLAN );
	m_theThread.Start( this );
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
void CDlgColList::OnConnect(int nErrorCode )
{
	TCHAR szBuffer[100];

	if (nErrorCode != 0) {
		//AfxMessageBox(_T("������ ������ �� �����ϴ� !!!"));
		m_StatusBar.SetText(_T("��� ����"), 0, 0);
	}
	else {
		UINT uiPortNum;
		CString strIPAddress;

		m_pConnected->GetPeerName( strIPAddress, uiPortNum );

		//Log( enNormal, "%s/%d �� ����Ǿ����ϴ�.", strIPAddress, uiPortNum );

		wprintf_s( szBuffer, sizeof(szBuffer), _T("����[%s/%d]"), strIPAddress, uiPortNum );
		m_StatusBar.SetText(szBuffer, 0, 0);

		//GetDlgItem(IDC_BUTTON_CLIENT)->SetWindowText(_T("���� �������"));
		m_bConnected = true;

		//CALL_DIALOG( enUnit, OnConnect() );

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

	// 1. ��� ������ �����ϱ�
	iLenOfHeader = m_pConnected->Receive((char *) pstRxMessage, sizeof(STR_MESSAGE) );

	// 2. ������ �����ϱ�
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 15:44:24
 * @warning   
 */
void CDlgColList::InitListCtrl()
{
	CRect rt;

	m_ColList.GetWindowRect(&rt);
	m_ColList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_ColList.SetSortable( false );

	// m_CListCtrlLOG.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	m_ColList.InsertColumn(0, _T("���� ��ȣ"), LVCFMT_LEFT, (int) ( rt.Width()*0.10), -1 );
	m_ColList.InsertColumn(1, _T("���� ���ļ�"), LVCFMT_LEFT, (int) ( rt.Width() * 0.20) , -1);
	m_ColList.InsertColumn(2, _T("���� ����/�ð�"), LVCFMT_LEFT, (int) ( rt.Width() * 0.6 ), -1);
	m_ColList.InsertColumn(3, _T("��Ÿ"), LVCFMT_LEFT, (int) (rt.Width() * 0.09), -1);

	m_ColList.SetGridLines(TRUE);
	m_ColList.SetCheckboxes(TRUE);

}