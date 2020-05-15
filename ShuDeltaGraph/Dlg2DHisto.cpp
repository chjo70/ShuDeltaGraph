// Dlg2DHisto.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "Dlg2DHisto.h"
#include "afxdialogex.h"

#include "Pegrpapi.h"




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
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
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

	// 초기 설정
	InitVar();
	InitButton();
	InitBuffer();
	InitStatusBar();
	InitListCtrl();
	InitStatic();
	InitToolTip();
	InitThread();

	KillTimer( CLK_TIMER );

	// 그래프 설정
	InitGraph();

	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:46
 * @warning   
 */
void CDlg2DHisto::InitVar()
{
	memset( m_nFreqX, 0, sizeof(m_nFreqX) );
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:44
 * @warning   
 */
void CDlg2DHisto::InitButton()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:43
 * @warning   
 */
void CDlg2DHisto::InitBuffer()
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	m_pSonataData = pApp->m_pDlgColList->GetSONATAData();

	m_hHisto[0] = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hHisto[1] = CreateEvent( NULL, TRUE, FALSE, NULL );
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:40
 * @warning   
 */
void CDlg2DHisto::InitListCtrl()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:50
 * @warning   
 */
void CDlg2DHisto::InitStatusBar()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:52
 * @warning   
 */
void CDlg2DHisto::InitStatic()
{

}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:54
 * @warning   
 */
void CDlg2DHisto::InitGraph()
{
	RECT rect;

 	GetClientRect( &rect );
 
 	m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1002);

	// v9 features new default CursorPromptLocation = ToolTip
	PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  
	PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);

	// Enable Bar Glass Effect //
	//PEnset(m_hPE, PEP_bBARGLASSEFFECT, TRUE);

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

	// Set DataShadows to show shadows
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);

	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	//PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSSPLINE);
	PEnset(m_hPE, PEP_nGRIDLINECONTROL, PEGLC_BOTH);
	PEnset(m_hPE, PEP_nGRIDSTYLE, PEGS_DOT);
	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
	PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_RO2_NOT);

	// Enable middle mouse dragging //
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);

	PEszset(m_hPE, PEP_szMAINTITLE, TEXT("주파수 히스토그램"));
	PEszset(m_hPE, PEP_szSUBTITLE, TEXT("")); // no subtitle
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("빈도율[%]"));
	PEszset(m_hPE, PEP_szXAXISLABEL, TEXT("주파수[GHz]"));

	// subset labels
	//PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("Horsepower"));
	//PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("Torque" ));
	//PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 2, TEXT("Temperature" ));
	//PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 3, TEXT("Pressure" ));

	// subset colors
//  	DWORD dwArray[SUBSETS] = { PERGB(128,198,  0,0), PERGB(128,  0,198, 198 ), PERGB(128, 0,198,0 ), PERGB(0, 128,198,0 ),
// 							   PERGB(  0,198,  0,0), PERGB(  0,  0,198, 198 ), PERGB(128, 198,198,0 ), PERGB(128, 0,198,0 ),
// 							   PERGB(  0,198,  0,0), PERGB(128, 0, 198, 198 ) } ;
//  	PEvsetEx( m_hPE, PEP_dwaSUBSETCOLORS, 0, SUBSETS, dwArray, 0 );

	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE, PEP_bSIMPLEPOINTLEGEND, TRUE);
	PEnset(m_hPE, PEP_bSIMPLELINELEGEND, TRUE);
	PEnset(m_hPE, PEP_nLEGENDSTYLE, PELS_1_LINE);
	//PEnset(m_hPE, PEP_nMULTIAXISSTYLE, PEMAS_SEPARATE_AXES);

	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	//PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_LIGHT_INSET );

	PEnset(m_hPE, PEP_nAUTOMINMAXPADDING, 1);

	PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
	PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
	PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_SMALL);
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

	// Set up cursor //
// 	PEnset(m_hPE, PEP_nCURSORMODE, PECM_DATACROSS);
// 
// 	// Help see data points //
 	PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);
// 
// 	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, FALSE);
// 	//PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);
// 	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_LIGHT_SHADOW );
 	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_BAR );		
// 
// 	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
// 	PEnset(m_hPE, PEP_nMOUSEWHEELFUNCTION, PEMWF_HORZPLUSVERT_ZOOM);
// 	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);  // note that pan gestures require MouseDragging to be enabled 
// 	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);  
// 
// 	// Enable MouseWheel Zoom Smoothness
// 	PEnset(m_hPE, PEP_nMOUSEWHEELZOOMSMOOTHNESS, 5);
// 	PEnset(m_hPE, PEP_nPINCHZOOMSMOOTHNESS, 2);
// 
// 	PEnset(m_hPE, PEP_bZOOMWINDOW, TRUE);
// 
// 	// v9 features 
// 	PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);
// 	PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);
// 	PEnset(m_hPE, PEP_nCURSORPROMPTLOCATION, PECPL_TRACKING_TOOLTIP);
// 
// 	PEnset(m_hPE, PEP_bTRACKINGCUSTOMDATATEXT, TRUE);
// 	PEnset(m_hPE, PEP_bTRACKINGCUSTOMOTHERTEXT, TRUE);
// 
// 	PEnset(m_hPE, PEP_bALLOWTITLEHOTSPOTS, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWSUBSETHOTSPOTS, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWAXISLABELHOTSPOTS, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWGRIDNUMBERHOTSPOTSY, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWGRIDNUMBERHOTSPOTSX, TRUE);
// 
// 	PEnset(m_hPE, PEP_bMOUSECURSORCONTROLCLOSESTPOINT, TRUE);

	//////////////////////////////////////////////////////////////////////////
	double d;
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
	d = 500.0F / 1000.0;
	PEvset(m_hPE, PEP_fMANUALMINX, &d, 1);
	d = 18000.0F / 1000.0;
	PEvset(m_hPE, PEP_fMANUALMAXX, &d, 1);

	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	d = 0;
	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
	d = 100.0F;
	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

	PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,.0|") );

	d = 0.001F;
	PEvset( m_hPE, PEP_fBARWIDTH, &d, 1 );

	// Set number of Subsets and Points //
	PEnset(m_hPE, PEP_nSUBSETS, 1);	
	PEnset(m_hPE, PEP_nPOINTS, FREQ_MAX-FREQ_MIN+1 );

	for( int p=FREQ_MIN; p<=FREQ_MAX; p++ ) {
		float fX, fY;

		fX = (float) p / (float) 1000.;
		PEvsetcellEx (m_hPE, PEP_faXDATA, 0, p, &fX);
		fY = 1;
		PEvsetcellEx (m_hPE, PEP_faYDATA, 0, p, &fY);
	}

	::InvalidateRect(m_hPE, NULL, FALSE);

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:43:01
 * @warning   
 */
void CDlg2DHisto::InitToolTip()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:43:51
 * @warning   
 */
void CDlg2DHisto::InitThread()
{
	m_theThread.Attach( Func2DHisto );
	m_theThread.Start( this );
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:43:03
 * @warning   
 */
void CDlg2DHisto::FreeBuffer()
{
	m_theThread.Stop();
	//KillTimer( CLK_TIMER );

	CloseHandle( m_hHisto[en_ReceiveData] );
	CloseHandle( m_hHisto[en_Timer] );

	if (m_hPE)
	{
		PEdestroy(m_hPE);
		m_hPE = 0; 
	}
	
}

/**
 * @brief     
 * @param     UINT nType
 * @param     int cx
 * @param     int cy
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:43:32
 * @warning   
 */
void CDlg2DHisto::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_hPE ) {
		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
	}

	UPDATE_EASYSIZE;
}

/**
 * @brief     
 * @param     MINMAXINFO * lpMMI
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:43:45
 * @warning   
 */
void CDlg2DHisto::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LPMINMAXINFO lpminmax = ( LPMINMAXINFO ) lpMMI;

	// 창 최소크기 지정

	lpminmax->ptMinTrackSize.x = m_rectCurDlg.right - m_rectCurDlg.left;
	lpminmax->ptMinTrackSize.y = m_rectCurDlg.bottom - m_rectCurDlg.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


/**
 * @brief     
 * @param     UINT fwSide
 * @param     LPRECT pRect
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:43:48
 * @warning   
 */
void CDlg2DHisto::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EASYSIZE_MINSIZE(m_rectCurDlg.right - m_rectCurDlg.left,m_rectCurDlg.bottom - m_rectCurDlg.top,fwSide,pRect);
}


/**
 * @brief     
 * @param     LPVOID lpData
 * @return    DWORD WINAPI
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:43:55
 * @warning   
 */
DWORD WINAPI Func2DHisto( LPVOID lpData )
{
	CDlg2DHisto *pDlg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDlg2DHisto * ) pParent->GetParam();

	pDlg->InitVar();

	while( TRUE ) {
		DWORD dRet;

		dRet = WaitForMultipleObjects( 2, pDlg->m_hHisto, FALSE, INFINITE );

		if( dRet == WAIT_FAILED ) {
			break;
		}
		else if( dRet == WAIT_ABANDONED ) {
			ResetEvent( pDlg->m_hHisto[en_ReceiveData] );
			ResetEvent( pDlg->m_hHisto[en_Timer] );
			continue;
		}
		else if( dRet == WAIT_TIMEOUT ) {

		}
		else if( dRet == WAIT_OBJECT_0 ) {
			ResetEvent( pDlg->m_hHisto[en_ReceiveData] );
			pDlg->UpdateHisto( TRUE );
			pDlg->ViewGraph();

			pDlg->View3DGraph();
		}
		else if( dRet == WAIT_OBJECT_0+1 ) {
			ResetEvent( pDlg->m_hHisto[en_Timer] );
			pDlg->UpdateHisto( FALSE );
			pDlg->ViewGraph();
		}
		else {

		}

	}

	return 0;

}

/**
 * @brief     
 * @param     BOOL bData
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:07
 * @warning   
 */
void CDlg2DHisto::UpdateHisto( BOOL bData )
{
	if( bData == TRUE ) {
		UINT i, uiItem, uiTemp32, iMaxFreq=0;

		float fFreq, fNorm;

		TNEW_PDW *pPDW;

		UINT *pFreqX;

		uiItem = m_pSonataData->uiItem;

		pPDW = & m_pSonataData->unRawData.stPDWData[0];

		InitVar();

		for( i=0 ; i < uiItem ; ++i ) {
			UINT uiIndex;

			uiTemp32 = BIT_MERGE( pPDW->item.frequency_h, pPDW->item.frequency_l);
			fFreq = FFRQCNV( pPDW->item.band + 1, uiTemp32 );

			uiIndex = (UINT) ( fFreq + 0.5 );
			//uiIndex = IMUL( UDIV( uiIndex, 10 ), 10 );
			++ m_nFreqX[uiIndex];

			iMaxFreq = max( m_nFreqX[uiIndex], iMaxFreq );

			++ pPDW;
		}

		fNorm = FDIV( 100, iMaxFreq );
		pFreqX = & m_nFreqX[FREQ_MIN];
		for( i=FREQ_MIN ; i <= FREQ_MAX ; ++i ) {
			*pFreqX = IMUL( *pFreqX, fNorm );
			++ pFreqX;
		}

	}
	else {
		int i;
		UINT *pFreqX;

		pFreqX = & m_nFreqX[FREQ_MIN];
		for( i=FREQ_MIN ; i <= FREQ_MAX ; ++i ) {
			if( *pFreqX != 0 ) {
				*pFreqX -= 10;
			}
			else if( *pFreqX == 0 ) {
			}
			else {

			}

			++ pFreqX;
		}
	}
}

/**
 * @brief     
 * @param     UINT nID
 * @param     LPARAM lParam
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:16
 * @warning   
 */
void CDlg2DHisto::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(nID == SC_CLOSE)	
	{
// 		if (m_hPE)
// 		{
// 			PEdestroy(m_hPE);
// 			m_hPE = 0; 
// 		}
// 		InitGraph();

		//GetColItem( & m_stColItem );
		//종료버튼 눌릴 시
	}
	else if(nID == SC_MAXIMIZE)
	{ 
		//최대화 버튼 눌릴 시
	}
	else if(nID == SC_MINIMIZE)
	{ 
		//최소화 버튼 눌릴 시
		CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();
		pApp->MinimuzeGraph();
	}
	else if(nID == SC_RESTORE) 
	{ 
		//복원 상황에서
	} 

	CDialogEx::OnSysCommand(nID, lParam);
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:22
 * @warning   
 */
void CDlg2DHisto::ViewGraph()
{
	int s=0;
	float fX, fY;
	DWORD dwColor;

	PEnset(m_hPE, PEP_nSUBSETS, 1);	
	PEnset(m_hPE, PEP_nPOINTS, FREQ_MAX-FREQ_MIN+1 );

	int nSFPC[] = {0, 1}; // First and Second subsets use SubsetPointColors feature //
	PEvset(m_hPE, PEP_naSUBSETFORPOINTCOLORS, nSFPC, 2);

	dwColor = PERGB( 255, 100, /* 15 + ((m_pSonataData->uiNo - 1) * 20) */ 255, 0);  
	for( int p=FREQ_MIN; p<=FREQ_MAX; p++ ) {
		if( m_pSonataData->uiNo % 2 ) {
			PEvsetcellEx(m_hPE, PEP_dwaPOINTCOLORS, 0, p, &dwColor);

			fX = (float) p / (float) 1000.;
			PEvsetcellEx (m_hPE, PEP_faXDATA, 0, p, &fX);
			fY = (float) m_nFreqX[p];
			PEvsetcellEx (m_hPE, PEP_faYDATA, 0, p, &fY);
		}
		else {
			PEvsetcellEx(m_hPE, PEP_dwaPOINTCOLORS, 1, p, &dwColor);

			fX = (float) p / (float) 1000.;
			PEvsetcellEx (m_hPE, PEP_faXDATA, 1, p, &fX);
			fY = (float) m_nFreqX[p];
			PEvsetcellEx (m_hPE, PEP_faYDATA, 1, p, &fY);
		}

	}

	::InvalidateRect(m_hPE, NULL, FALSE);

}

void CDlg2DHisto::View3DGraph()
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	pApp->m_pDlg3DBar->ViewGraph();
}

/**
 * @brief     
 * @param     WPARAM wParam
 * @param     LPARAM lParam
 * @return    BOOL
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:31
 * @warning   
 */
BOOL CDlg2DHisto::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CDialogEx::OnCommand(wParam, lParam);

	if( (HIWORD(wParam) == PEWN_CUSTOMTRACKINGDATATEXT )) {
		// v9 features 
		double dX;
		PEvget(m_hPE, PEP_fCURSORVALUEX, &dX);
		TCHAR  szBuffer[200]; 

		double dY;
		PEvget(m_hPE, PEP_fCURSORVALUEY, &dY);

		int iIndex = (int) ( dX * 1000. + 0.5 );
		_stprintf_s(szBuffer, 200, _T("%.1fGHz\n%.1f%%"), dX, m_nFreqX[iIndex]);
		//wsprintf( & szBuffer[0], _T("X Axis: %.3f  \nY Axis: %.3f"), (float) dX, (float)  dY);

		if (dX < 2.F) {
			PEszset(m_hPE, PEP_szTRACKINGTOOLTIPTITLE, TEXT("대역 1"));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(0,0,0,0));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,0,245,0));
		}
		else if( dX < 6.F) {
			PEszset(m_hPE, PEP_szTRACKINGTOOLTIPTITLE, TEXT("대역 2"));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(0,50,50,0));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(245,0,0,0));
		}
		else if( dX < 12.F) {
			PEszset(m_hPE, PEP_szTRACKINGTOOLTIPTITLE, TEXT("대역 3"));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(50,50,0,0));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
		}
		else
		{
			PEszset(m_hPE, PEP_szTRACKINGTOOLTIPTITLE, TEXT("대역 4"));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(0,50,50,50));
			PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,0,225,0));
		}

		PEszset(m_hPE, PEP_szTRACKINGTEXT, szBuffer );
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


/**
 * @brief     
 * @param     UINT_PTR nIDEvent
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 16:49:17
 * @warning   
 */
void CDlg2DHisto::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case CLK_TIMER:
		//SetEvent( m_hHisto[en_Timer] );
		//1초마다 처리할 내용
		break;

	}

	CDialogEx::OnTimer(nIDEvent);
}

