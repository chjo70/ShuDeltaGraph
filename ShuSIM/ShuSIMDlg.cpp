
// ShuSIMDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ShuSIM.h"
#include "ShuSIMDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CShuSIMDlg ��ȭ ����




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


// CShuSIMDlg �޽��� ó����

BOOL CShuSIMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitBuffer();
	InitStatusBar();
	InitLOGListCtrl();

	InitVar();

	InitSocketSetting();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CShuSIMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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

		strMsg.Format( _T("����:%s/%d"), (char*)(LPCTSTR) strIP, port);
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

	//TRACE( "%s(%d��) ������ �ݽ��ϴ�." ,  g_szUnit[enUnit], enUnit );
	//Log( enError, "%s(%d��) ������ �ݽ��ϴ�." ,  g_szUnit[enUnit], enUnit );

	m_pListener->Close();
	m_pListener->ShutDown();

	m_pConnected->Close();

	m_StatusBar.SetText(_T("��� ����"), 0, 0);
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
		//AfxMessageBox(_T("������ ������ �� �����ϴ� !!!"));
		m_StatusBar.SetText(_T("��� ����"), 0, 0);
	}
	else {
		UINT uiPortNum;
		CString strIPAddress, strMsg;

		m_pConnected->GetPeerName( strIPAddress, uiPortNum );

		//Log( enNormal, "%s/%d �� ����Ǿ����ϴ�.", strIPAddress, uiPortNum );

		strMsg.Format( _T("����:%s/%d"), strIPAddress, uiPortNum );
		//wprintf_s( _T("����[%s/%d]"), strIPAddress, uiPortNum );
		m_StatusBar.SetText(strMsg, 0, 0);

		//GetDlgItem(IDC_BUTTON_CLIENT)->SetWindowText(_T("���� �������"));
		//m_pConnected->m_bConnected = true;

		//CALL_DIALOG( enUnit, OnConnect() );

	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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

	Log( enNormal, _T("[%s/%d]�� ������ �����մϴ�."), strIPAddress, uiPortNum );
	m_pConnected->Create();

	Connect();

	m_StatusBar.SetText(_T("��� ����"), (int) 0, 0);

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

	Log( enNormal, _T("���Ͽ� [127.0.0.1/%d]�� �����մϴ�."), uiPortNum );
	if( enOffice == enPos || enMyHome == enPos || enNoNetwork == enPos ) {
		Log( enNormal, _T("���Ͽ� [127.0.0.1/%d]�� �����մϴ�."), uiPortNum );
		bRet = m_pConnected->Connect(_T("127.0.0.1"), uiPortNum);
	}
	else {
		Log( enNormal, _T("���Ͽ� [192.168.0.120/%d]�� �����մϴ�."), uiPortNum );		// 5010
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

	m_CListLog.InsertColumn(0, _T("����"), LVCFMT_LEFT, (int) ( rt.Width()*0.05), -1 );
	m_CListLog.InsertColumn(1, _T("�޽�����"), LVCFMT_LEFT, (int) ( rt.Width() * 0.25) , -1);
	m_CListLog.InsertColumn(2, _T("���� ����"), LVCFMT_LEFT, (int) ( rt.Width() * 0.6 ), -1);
	m_CListLog.InsertColumn(3, _T("���� ����"), LVCFMT_LEFT, (int) (rt.Width() * 0.09), -1);

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Connect();

}


void CShuSIMDlg::OnBnClickedButtonReqInit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		*pstrTemp1 = _T(">>�ʱ�ȭ �䱸");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
		break;

	case REQ_SETMODE:
		*pstrTemp1 = _T(">>�ý��۸�� ��ȯ �뺸");
		*pstrTemp2 = _T("");
		break;

	case REQ_SET_CONFIG:
		*pstrTemp1 = _T(">>���� �Ķ���� ����");
		pstrTemp2->Format( _T("M%d, %.1f[MHz], %d[����], %.1f[ms], %.1f[dBm]"), pstData->stSetMode.uiMode, pstData->stSetMode.fTuneFreq, pstData->stSetMode.coPulseNum, pstData->stSetMode.fColTime, pstData->stSetMode.fThreshold );
		break;

	case REQ_COL_START :
		*pstrTemp1 = _T(">>��ȣ ���� ���� �䱸");
		break;

	case REQ_RAWDATA :
		*pstrTemp1 = _T(">>���� ������");
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("�߸��� ���[0x%x]�Դϴ�."), pstMessage->uiOpcode);
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
		*pstrTemp1 = _T("<<�ʱ�ȭ�䱸 ����");
		pstrTemp2->Format( _T("[%d][%d]"), pstData->stResInit.uiReqCode, pstData->stResInit.uiErrorCode );
		break;

	case RES_SET_CONFIG:
		*pstrTemp1 = _T("<<���� �Ķ��Ÿ ���� ��� ����");
		pstrTemp2->Format( _T("[%d]"), pstData->uiResult );
		break;

	case RES_COL_START :
		*pstrTemp1 = _T("<<�������� ����");
		pstrTemp2->Format( _T("ST[%d],Co[%d],Phase[%d]"), pstData->stColStart.uiStatus, pstData->stColStart.uiCoPulseNum, pstData->stColStart.uiPhase3Num );
		break;

	case RES_RAWDATA_PDW:
		*pstrTemp1 = _T("<<PDW ������");
		break;

	case RES_RAWDATA_INTRA:
		*pstrTemp1 = _T("<<INTRA ������");
		break;

	default:
		*pstrTemp1 = _T("<<");
		pstrTemp2->Format( _T("�߸��� ���[0x%x]�Դϴ�."), pstMessage->uiOpcode);
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