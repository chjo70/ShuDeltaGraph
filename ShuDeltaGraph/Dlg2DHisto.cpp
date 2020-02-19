// Dlg2DHisto.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "Dlg2DHisto.h"
#include "afxdialogex.h"

#include "Pegrpapi.h"

#include "./EasySize/EasySize.h"

DWORD WINAPI Func2DHisto( LPVOID lpData );


// CDlg2DHisto 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg2DHisto, CDialogEx)

CDlg2DHisto::CDlg2DHisto(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg2DHisto::IDD, pParent)
{

}

CDlg2DHisto::~CDlg2DHisto()
{
	FreeBuffer();
}

void CDlg2DHisto::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_CGraph);
}


BEGIN_MESSAGE_MAP(CDlg2DHisto, CDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZING()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CDlg2DHisto)
	EASYSIZE(IDC_STATIC_GRAPH,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP


// CDlg2DHisto 메시지 처리기입니다.


BOOL CDlg2DHisto::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	this->GetWindowRect( m_rectCurDlg );

	Log( enNormal, _T("CDlg2DHisto +++++++++++++++++++++++++++++++++++++++++++") );

	InitVar();
	InitButton();
	InitBuffer();
	InitStatusBar();
	InitListCtrl();
	InitStatic();
	InitToolTip();
	InitThread();

	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlg2DHisto::InitVar()
{

}

void CDlg2DHisto::InitButton()
{

}

void CDlg2DHisto::InitBuffer()
{
	m_hHisto = CreateEvent( NULL, FALSE, FALSE, NULL );
}

void CDlg2DHisto::InitListCtrl()
{

}

void CDlg2DHisto::InitStatusBar()
{

}

void CDlg2DHisto::InitStatic()
{
	RECT rect;

	GetClientRect( &rect );

	m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

	float fY;
	float fX;

	// v9 features new default CursorPromptLocation = ToolTip
	PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  
	PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);

	// Enable Bar Glass Effect //
	PEnset(m_hPE, PEP_bBARGLASSEFFECT, TRUE);

	// Enable Plotting style gradient and bevel features //
	PEnset(m_hPE, PEP_nAREAGRADIENTSTYLE,  PEPGS_VERTICAL_ASCENT_INVERSE);
	PEnset(m_hPE, PEP_nAREABEVELSTYLE, PEBS_MEDIUM_SMOOTH);
	PEnset(m_hPE, PEP_nSPLINEGRADIENTSTYLE,  PEPGS_VERTICAL_ASCENT_INVERSE);
	PEnset(m_hPE, PEP_nSPLINEBEVELSTYLE, PESBS_MEDIUM_SMOOTH);

	// v7.2 new features //
	PEnset(m_hPE, PEP_nPOINTGRADIENTSTYLE,PEPGS_VERTICAL);// PEPGS_VERTICAL_ASCENT_INVERSE);
	PEnset(m_hPE, PEP_dwPOINTBORDERCOLOR, PERGB(100, 0, 0, 0));
	PEnset(m_hPE, PEP_nLINESYMBOLTHICKNESS, 3);
	PEnset(m_hPE, PEP_nAREABORDER, 1);
	PEnset(m_hPE, PEP_bALLOWSVGEXPORT, 1);

	PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE, PEP_bCACHEBMP, TRUE);

	// Set number of Subsets and Points //
	PEnset(m_hPE, PEP_nSUBSETS, 1 );	
	PEnset(m_hPE, PEP_nPOINTS, 18000 );	

	int s=0;
	{
		int nOffset = 1;
		for( int p=0; p<18000; p++ )
		{										  
			fX = p;
			PEvsetcellEx (m_hPE, PEP_faXDATA, s, p, &fX);
			fY = 199;
			PEvsetcellEx (m_hPE, PEP_faYDATA, s, p, &fY);
		}
	}

	// Set DataShadows to show shadows
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);

	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSSPLINE);
	PEnset(m_hPE, PEP_nGRIDLINECONTROL, PEGLC_BOTH);
	PEnset(m_hPE, PEP_nGRIDSTYLE, PEGS_DOT);
	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
	PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_RO2_NOT);

	// Enable middle mouse dragging //
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);

	PEszset(m_hPE, PEP_szMAINTITLE, TEXT("주파수 히스토그램"));
	PEszset(m_hPE, PEP_szSUBTITLE, TEXT("")); // no subtitle
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("#"));
	PEszset(m_hPE, PEP_szXAXISLABEL, TEXT("주파수[MHz]"));

	// subset labels
	//PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("Horsepower"));

	// subset colors
	DWORD dwArray[4] = { PERGB(128,198,0,0), PERGB(128, 0, 198, 198 ), PERGB(128, 198,198,0 ), PERGB(128, 0,198,0 ) };
	PEvsetEx( m_hPE, PEP_dwaSUBSETCOLORS, 0, 4, dwArray, 0 );

	// subset line types
	int nLineTypes[] = { PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID };
	PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 4);

	// subset point types //
	int nPointTypes[] = { PEPT_DOTSOLID, PEPT_UPTRIANGLESOLID, 
		PEPT_SQUARESOLID, PEPT_DOWNTRIANGLESOLID, PEPT_DOTSOLID, 
		PEPT_SQUARESOLID, PEPT_DIAMONDSOLID, PEPT_UPTRIANGLESOLID };
	PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPointTypes, 8);

	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE, PEP_bSIMPLEPOINTLEGEND, TRUE);
	PEnset(m_hPE, PEP_bSIMPLELINELEGEND, TRUE);
	PEnset(m_hPE, PEP_nLEGENDSTYLE, PELS_1_LINE);
	PEnset(m_hPE, PEP_nMULTIAXISSTYLE, PEMAS_SEPARATE_AXES);

	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);

	PEnset(m_hPE, PEP_nAUTOMINMAXPADDING, 1);

	PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
	PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
	PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);
	PEnset(m_hPE, PEP_bSCROLLINGHORZZOOM, TRUE);
	PEnset(m_hPE, PEP_nDATAPRECISION, 1);

	PEnset(m_hPE, PEP_nIMAGEADJUSTTOP, 10);
	PEnset(m_hPE, PEP_nIMAGEADJUSTLEFT, 20);
	PEnset(m_hPE, PEP_nIMAGEADJUSTRIGHT, 20);

	// Set export defaults //
	PEnset(m_hPE, PEP_nDPIX, 600);
	PEnset(m_hPE, PEP_nDPIY, 600);

	PEnset(m_hPE, PEP_nEXPORTSIZEDEF, PEESD_NO_SIZE_OR_PIXEL );
	PEnset(m_hPE, PEP_nEXPORTTYPEDEF, PEETD_PNG );
	PEnset(m_hPE, PEP_nEXPORTDESTDEF, PEEDD_CLIPBOARD );
	PEszset(m_hPE, PEP_szEXPORTUNITXDEF, TEXT("1280"));
	PEszset(m_hPE, PEP_szEXPORTUNITYDEF, TEXT("768"));
	PEnset(m_hPE, PEP_nEXPORTIMAGEDPI, 300 );

	PEnset(m_hPE, PEP_nRENDERENGINE, PERE_DIRECT2D);

	// Set Demo's RenderEngine to Direct2D // 
	//CMDIFrameWnd* pWnd = (CMDIFrameWnd*) AfxGetApp()->GetMainWnd();
	//pWnd->SendMessage(WM_CHANGE_DEMO_RENDERENGINE, RENDER_2DX );

}

void CDlg2DHisto::InitToolTip()
{

}

void CDlg2DHisto::FreeBuffer()
{
	PEdestroy(m_hPE);
}

void CDlg2DHisto::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_hPE ) {
		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
	}

	UPDATE_EASYSIZE;
}


void CDlg2DHisto::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LPMINMAXINFO lpminmax = ( LPMINMAXINFO ) lpMMI;

	// 창 최소크기 지정

	lpminmax->ptMinTrackSize.x = m_rectCurDlg.right - m_rectCurDlg.left;
	lpminmax->ptMinTrackSize.y = m_rectCurDlg.bottom - m_rectCurDlg.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CDlg2DHisto::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EASYSIZE_MINSIZE(m_rectCurDlg.right - m_rectCurDlg.left,m_rectCurDlg.bottom - m_rectCurDlg.top,fwSide,pRect);
}

void CDlg2DHisto::UpdateData( STR_SONATA_DATA *pSonataData )
{

}

void CDlg2DHisto::InitThread()
{
	m_theThread.Attach( Func2DHisto );
	m_theThread.Start( this );
}

DWORD WINAPI Func2DHisto( LPVOID lpData )
{
	UINT uiIndex=MAX_COL_ITEMS;
	CDlg2DHisto *pDlg;

	STR_QUEUE_MSG stQueueMsg;
	queue<STR_QUEUE_MSG> *pQueueMsg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDlg2DHisto * ) pParent->GetParam();

	while( TRUE ) {
		DWORD dRet;

		dRet = WaitForSingleObject( pDlg->m_hHisto, 1000 );

		if( dRet == WAIT_FAILED ) {
			break;
		}
		else if( dRet == WAIT_ABANDONED ) {
			ResetEvent( pDlg->m_hHisto );
			continue;
		}
		else if( dRet == WAIT_TIMEOUT ) {
			continue;
		}
		else {
			ResetEvent( pDlg->m_hHisto );

		}

	}

	return 0;

}