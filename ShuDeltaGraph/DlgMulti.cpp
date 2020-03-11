// DlgMulti.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgMulti.h"
#include "afxdialogex.h"

#include "Pegrpapi.h"


DWORD WINAPI FuncMulti( LPVOID lpData );

extern float _spOneSec;
extern float _spOneMilli;
extern float _spOneMicrosec;
extern float _spOneNanosec;


// CDlgMulti 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMulti, CDialogEx)

CDlgMulti::CDlgMulti(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMulti::IDD, pParent)
{

}

CDlgMulti::~CDlgMulti()
{
	FreeBuffer();
}

void CDlgMulti::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MULTI, m_CStatic);
}


BEGIN_MESSAGE_MAP(CDlgMulti, CDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZING()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CDlgMulti)
	//EASYSIZE(IDC_STATIC_GRAPH,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP


// CDlgMulti 메시지 처리기입니다.


BOOL CDlgMulti::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	this->GetWindowRect( m_rectCurDlg );

	Log( enNormal, _T("CDlgMulti +++++++++++++++++++++++++++++++++++++++++++") );

	// 초기 설정
	InitVar();
	InitButton();
	InitBuffer();
	InitStatusBar();
	InitListCtrl();
	InitStatic();
	InitToolTip();
	InitThread();

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
 * @date      2020/02/20 21:45:00
 * @warning   
 */
void CDlgMulti::InitVar()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:58
 * @warning   
 */
void CDlgMulti::InitButton()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:57
 * @warning   
 */
void CDlgMulti::InitBuffer()
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	m_pSonataData = pApp->m_pDlgColList->GetSONATAData();

	m_hMulti = CreateEvent( NULL, TRUE, FALSE, NULL );

	_spOneSec = 20000000.;
	_spOneMilli = FDIV( _spOneSec, 1000. );
	_spOneMicrosec = FDIV( _spOneMilli, 1000. );
	_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:45:03
 * @warning   
 */
void CDlgMulti::InitListCtrl()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:39:37
 * @warning   
 */
void CDlgMulti::InitStatusBar()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:39:35
 * @warning   
 */
void CDlgMulti::InitStatic()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:39:34
 * @warning   
 */
void CDlgMulti::InitGraph()
{
	RECT rect;
	//double d;

	GetClientRect( &rect );

	m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1002);

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
	PEnset(m_hPE, PEP_nSUBSETS, 3);	
	PEnset(m_hPE, PEP_nPOINTS, 10 );	

	double fX, fY;
	for( int s=0; s< 3; s++ )
	{
		//int nOffset = GetRandom(1, 250);
		for( int p=0; p<10 ; p++ )
		{										  
			fX = p;
			PEvsetcellEx (m_hPE, PEP_faXDATA, s, p, &fX);
			fY = 0.0;
			PEvsetcellEx (m_hPE, PEP_faYDATA, s, p, &fY);
		}
	}

	// Enable MouseWheel Zoom Smoothness
	PEnset(m_hPE, PEP_nMOUSEWHEELZOOMSMOOTHNESS, 5);
	PEnset(m_hPE, PEP_nPINCHZOOMSMOOTHNESS, 2);

	// Set DataShadows to show shadows
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);

	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSSPLINE);
	PEnset(m_hPE, PEP_nGRIDLINECONTROL, PEGLC_BOTH);
	PEnset(m_hPE, PEP_nGRIDSTYLE, PEGS_DOT);
	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORIZONTAL);
	PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_RO2_NOT);

	// Enable middle mouse dragging //
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);

	PEszset(m_hPE, PEP_szMAINTITLE, TEXT("멀티 그래프"));
	PEszset(m_hPE, PEP_szSUBTITLE, TEXT("")); // no subtitle
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("빈도율[%]"));
	PEszset(m_hPE, PEP_szXAXISLABEL, TEXT("시간"));

	// subset labels
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("주파수"));
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("신호세기" ));
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 2, TEXT("DTOA" ));
	//PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 3, TEXT("Pressure" ));

	// subset colors
	DWORD dwArray[3] = { PERGB(128,198,0,0), PERGB(128, 0, 198, 198 ), PERGB(128, 198,198,0 ) };
	PEvsetEx( m_hPE, PEP_dwaSUBSETCOLORS, 0, 3, dwArray, 0 );

	// subset line types
	//  	int nLineTypes[] = { PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID };
	//  	PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 4);

	// 	// subset point types //
	// 	int nPointTypes[] = { PEPT_DOTSOLID, PEPT_UPTRIANGLESOLID, 
	// 		PEPT_SQUARESOLID, PEPT_DOWNTRIANGLESOLID, PEPT_DOTSOLID, 
	// 		PEPT_SQUARESOLID, PEPT_DIAMONDSOLID, PEPT_UPTRIANGLESOLID };
	// 	PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPointTypes, 8);

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

	//////////////////////////////////////////////////////////////////////////

	// Set up cursor //
	PEnset(m_hPE, PEP_nCURSORMODE, PECM_DATACROSS);

	// Help see data points //
	PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);

	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, FALSE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_BAR );		

	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
	PEnset(m_hPE, PEP_nMOUSEWHEELFUNCTION, PEMWF_HORZPLUSVERT_ZOOM);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);  // note that pan gestures require MouseDragging to be enabled 
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);  

	// Enable MouseWheel Zoom Smoothness
	PEnset(m_hPE, PEP_nMOUSEWHEELZOOMSMOOTHNESS, 5);
	PEnset(m_hPE, PEP_nPINCHZOOMSMOOTHNESS, 2);

	int nArray[3] = {1,1,1};
	PEvset (m_hPE, PEP_naMULTIAXESSUBSETS, nArray, 3);

	// Set first axis parameters //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("주파수"));
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);
	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,| Hz") );
	PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[0] );
// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
// 	d = 500;
// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
// 	d = 18000.0F;
// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

	// Set second axis parameters //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 1);
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("신호세기"));
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);
	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,| dBm") );
	PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[1] );
// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
// 	d = -60;
// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
// 	d = 0.0F;
// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

	// Set third axis parameters //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 2);
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("DTOA"));
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE );
	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,00| 초") );
	PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[2] );
// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
// 	d = 0;
// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
// 	d = 18000.0F;
// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

	// Set fourth axis parameters //
	//PEnset(m_hPE, PEP_nWORKINGAXIS, 3);
	//PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("Label 4"));
	//PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);

	// Reset WorkingAxis when done //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

	// Add Axis Separator //
	PEnset(m_hPE, PEP_nMULTIAXESSEPARATORS, PEMAS_THIN);

	// Axis Sizing //
	PEnset(m_hPE, PEP_nMULTIAXESSIZING, TRUE);

	// Other various properties //
	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);
	PEnset(m_hPE, PEP_bMARKDATAPOINTS, FALSE);
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);

	// Set some random annotations //       
	PEnset(m_hPE, PEP_bALLOWANNOTATIONCONTROL, TRUE);

// 	double d;  	int t;   float f;
// 	int aCnt = 0;
// 	TCHAR szAnnotText[128];
// 	for (int annot = 0; annot < 5; annot++)
// 	{
// 		int nRndRow = 0;
// 		int nRndCol = (int)(GetRandom(1, PEnget(m_hPE, PEP_nPOINTS)));
// 		d = nRndCol + 1;   PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONX, aCnt, &d);
// 		PEvgetcellEx(m_hPE, PEP_faYDATA, nRndRow, nRndCol, &f);  d = (double)f;  PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONY, aCnt, &d);
// 		t = PEGAT_POINTER; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONTYPE, aCnt, &t);
// 		t = PERGB(255, 255, 255, 255);  PEvsetcell(m_hPE, PEP_dwaGRAPHANNOTATIONCOLOR, aCnt, &t);
// 		_stprintf(szAnnotText, TEXT("Axis0 %d"), annot);
// 		PEvsetcell(m_hPE, PEP_szaGRAPHANNOTATIONTEXT, aCnt, szAnnotText);
// 		t = 0; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONAXIS, aCnt, &t);
// 		aCnt++;
// 	}
// 	for (int annot = 0; annot < 5; annot++)
// 	{
// 		int nRndRow = 1;
// 		int nRndCol = (int)(GetRandom(1, PEnget(m_hPE, PEP_nPOINTS)));
// 		d = nRndCol + 1;   PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONX, aCnt, &d);
// 		PEvgetcellEx(m_hPE, PEP_faYDATA, nRndRow, nRndCol, &f);  d = (double)f;  PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONY, aCnt, &d);
// 		t = PEGAT_POINTER; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONTYPE, aCnt, &t);
// 		t = PERGB(255, 255, 255, 255);  PEvsetcell(m_hPE, PEP_dwaGRAPHANNOTATIONCOLOR, aCnt, &t);
// 		_stprintf(szAnnotText, TEXT("Axis1 %d"), annot);
// 		PEvsetcell(m_hPE, PEP_szaGRAPHANNOTATIONTEXT, aCnt, szAnnotText);
// 		t = 1; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONAXIS, aCnt, &t);
// 		aCnt++;
// 	}
// 	for (int annot = 0; annot < 5; annot++)
// 	{
// 		int nRndRow = 2;
// 		int nRndCol = (int)(GetRandom(1, PEnget(m_hPE, PEP_nPOINTS)));
// 		d = nRndCol + 1;   PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONX, aCnt, &d);
// 		PEvgetcellEx(m_hPE, PEP_faYDATA, nRndRow, nRndCol, &f);  d = (double)f;  PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONY, aCnt, &d);
// 		t = PEGAT_POINTER; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONTYPE, aCnt, &t);
// 		t = PERGB(255, 255, 255, 255);  PEvsetcell(m_hPE, PEP_dwaGRAPHANNOTATIONCOLOR, aCnt, &t);
// 		_stprintf(szAnnotText, TEXT("Axis1 %d"), annot);
// 		PEvsetcell(m_hPE, PEP_szaGRAPHANNOTATIONTEXT, aCnt, szAnnotText);
// 		t = 2; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONAXIS, aCnt, &t);
// 		aCnt++;
// 	}
// 	for (int annot = 0; annot < 5; annot++)
// 	{
// 		int nRndRow = 3;
// 		int nRndCol = (int)(GetRandom(1, PEnget(m_hPE, PEP_nPOINTS)));
// 		d = nRndCol + 1;   PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONX, aCnt, &d);
// 		PEvgetcellEx(m_hPE, PEP_faYDATA, nRndRow, nRndCol, &f);  d = (double)f;  PEvsetcell(m_hPE, PEP_faGRAPHANNOTATIONY, aCnt, &d);
// 		t = PEGAT_POINTER; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONTYPE, aCnt, &t);
// 		t = PERGB(255, 255, 255, 255);  PEvsetcell(m_hPE, PEP_dwaGRAPHANNOTATIONCOLOR, aCnt, &t);
// 		_stprintf(szAnnotText, TEXT("Axis1 %d"), annot);
// 		PEvsetcell(m_hPE, PEP_szaGRAPHANNOTATIONTEXT, aCnt, szAnnotText);
// 		t = 3; PEvsetcell(m_hPE, PEP_naGRAPHANNOTATIONAXIS, aCnt, &t);
// 		aCnt++;
// 	}

	PEnset(m_hPE, PEP_bZOOMWINDOW, TRUE);

	// v9 features 
	PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);
	PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);
	PEnset(m_hPE, PEP_nCURSORPROMPTLOCATION, PECPL_TRACKING_TOOLTIP);

	PEnset(m_hPE, PEP_bTRACKINGCUSTOMDATATEXT, TRUE);
	PEnset(m_hPE, PEP_bTRACKINGCUSTOMOTHERTEXT, TRUE);

	PEnset(m_hPE, PEP_bALLOWTITLEHOTSPOTS, TRUE);
	PEnset(m_hPE, PEP_bALLOWSUBSETHOTSPOTS, TRUE);
	PEnset(m_hPE, PEP_bALLOWAXISLABELHOTSPOTS, TRUE);
	PEnset(m_hPE, PEP_bALLOWGRIDNUMBERHOTSPOTSY, TRUE);
	PEnset(m_hPE, PEP_bALLOWGRIDNUMBERHOTSPOTSX, TRUE);

	PEnset(m_hPE, PEP_bMOUSECURSORCONTROLCLOSESTPOINT, TRUE);

	PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,|초") );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:46:58
 * @warning   
 */
void CDlgMulti::FreeBuffer()
{
	m_theThread.Stop();

	CloseHandle( m_hMulti );

	if (m_hPE)
	{
		PEdestroy(m_hPE);
		m_hPE = 0; 
	}
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:46:55
 * @warning   
 */
void CDlgMulti::InitThread()
{
	m_theThread.Attach( FuncMulti );
	m_theThread.Start( this );
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:46:53
 * @warning   
 */
void CDlgMulti::InitToolTip()
{

}



/**
 * @brief     
 * @param     UINT nType
 * @param     int cx
 * @param     int cy
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:49:51
 * @warning   
 */
void CDlgMulti::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_hPE ) {
		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
	}

	UPDATE_EASYSIZE;
}


void CDlgMulti::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
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
 * @date      2020/02/20 21:50:59
 * @warning   
 */
void CDlgMulti::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EASYSIZE_MINSIZE(m_rectCurDlg.right - m_rectCurDlg.left,m_rectCurDlg.bottom - m_rectCurDlg.top,fwSide,pRect);
}


void CDlgMulti::OnSysCommand(UINT nID, LPARAM lParam)
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
	}
	else if(nID == SC_RESTORE) 
	{ 
		//복원 상황에서
	} 

	CDialogEx::OnSysCommand(nID, lParam);
}


DWORD WINAPI FuncMulti( LPVOID lpData )
{
	CDlgMulti *pDlg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDlgMulti * ) pParent->GetParam();

	pDlg->InitVar();

	while( TRUE ) {
		DWORD dRet;

		dRet = WaitForSingleObject( pDlg->m_hMulti, INFINITE );

		if( dRet == WAIT_FAILED ) {
			break;
		}
		else if( dRet == WAIT_ABANDONED ) {
			ResetEvent( pDlg->m_hMulti );
			continue;
		}
		else if( dRet == WAIT_TIMEOUT ) {
			continue;
		}
		else {
			ResetEvent( pDlg->m_hMulti );
			pDlg->ViewGraph();

		}

	}

	return 0;

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:39:04
 * @warning   
 */
void CDlgMulti::ViewGraph()
{
	UINT i, uiItem, uiTemp32;

	UINT uiToa;

	float fFreq, fPA, fTOA, fFirstToa, fDtoa, fPreviousToa;

	TNEW_PDW *pPDW;
	TNEW_PDW temp;

	uiItem = m_pSonataData->uiItem;

	pPDW = & m_pSonataData->unRawData.stPDWData[0];

	PEnset(m_hPE, PEP_nPOINTS, uiItem );

	for( i=0 ; i < uiItem ; ++i ) {
		//UINT uiIndex;

		temp.bpdw[0][0] = pPDW->item.toa_1;
		temp.bpdw[0][1] = pPDW->item.toa_2;
		temp.bpdw[0][2] = pPDW->item.toa_3;
		temp.bpdw[0][3] = pPDW->item.toa_4;

		uiToa = temp.wpdw[0];
		fTOA = FDIV(uiToa, _spOneSec );

		if( i == 0 ) {
			fFirstToa = fTOA;
			fPreviousToa = fTOA;
			fDtoa = 0;
		}
		else {
			fDtoa = fTOA - fPreviousToa;
			fPreviousToa = fTOA;
		}

		uiTemp32 = BIT_MERGE( pPDW->item.frequency_h, pPDW->item.frequency_l);
		fFreq = FFRQCNV( pPDW->item.band + 1, uiTemp32 );
		fFreq = (float) ( fFreq * 1000.0 );

		uiTemp32 = pPDW->item.amplitude;
		fPA = FPACNV(uiTemp32);

		PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, & fTOA );
		PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, & fFreq );

		PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i, & fTOA );
		PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & fPA );

		PEvsetcellEx(m_hPE, PEP_faXDATA, 2, i, & fTOA );
		PEvsetcellEx(m_hPE, PEP_faYDATA, 2, i, & fDtoa );

		++ pPDW;
	}

	double dMin, dMax;
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
	dMin = fFirstToa;
	PEvset(m_hPE, PEP_fMANUALMINX, & dMin, 1);
	dMax = fTOA;
	PEvset(m_hPE, PEP_fMANUALMAXX, & dMax, 1);

	// Reset WorkingAxis when done //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

	::InvalidateRect(m_hPE, NULL, FALSE);

}

#define GET_NUMBER_FORMAT_SIZE		(200)

BOOL CDlgMulti::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( (HIWORD(wParam) == PEWN_CUSTOMTRACKINGDATATEXT )) {
		TCHAR  szTOA[GET_NUMBER_FORMAT_SIZE], szY[GET_NUMBER_FORMAT_SIZE]; 

		// v9 features 
		TCHAR  szBuffer[200]; 

		int nA;
		int nX;
		int nY;
		double dX;
		double dY;
		POINT pt;

		// get last mouse location within control //
		PEvget(m_hPE, PEP_ptLASTMOUSEMOVE, &pt);

		nA = 0;      // Initialize axis, non-zero only if using MultiAxesSubsets
		nX = pt.x;   // Initialize nX and nY with mouse location
		nY = pt.y;
		PEconvpixeltograph(m_hPE, &nA, &nX, &nY, &dX, &dY, 0, 0, 0);

		if( nA >= 0 ) {

			GetNumberFormat( szTOA, dX );

			if( nA == 0 ) {
				GetNumberFormat( szY, dY );
				_stprintf_s(szBuffer, 200, _T("%ss\n%sHz"), szTOA, szY);
				PEszset(m_hPE, PEP_szTRACKINGTOOLTIPTITLE, TEXT("주  파  수   "));
				PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(0,0,0,0));
				PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
			}
			else if( nA == 1 ) {
				GetNumberFormat( szY, dY );
				_stprintf_s(szBuffer, 200, _T("%ss\n%sdBm"), szTOA, szY);
				PEszset(m_hPE, PEP_szTRACKINGTOOLTIPTITLE, TEXT("신 호 세 기   "));
				PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(0,0,0,0));
				PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));	
			}
			else {	
				GetNumberFormat( szY, dY );
				_stprintf_s(szBuffer, 200, _T("%ss\n%ss"), szTOA, szY );
				PEszset(m_hPE, PEP_szTRACKINGTOOLTIPTITLE, TEXT("D  T  O  A "));
				PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(0,0,0,0));
				PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
			}

			PEszset(m_hPE, PEP_szTRACKINGTEXT, szBuffer );
		}
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

static NUMBERFMT nFmt = { 3, 1, 3, _T("."), _T(","), 1 };
void CDlgMulti::GetNumberFormat( TCHAR *pSZ, double dValue )
{
	int i;
	double dTemp;

	char szUnit[7] = { ' ', 'm', 'u', 'n', 'K', 'M', 'G' } ;

	_stprintf_s( pSZ, GET_NUMBER_FORMAT_SIZE, _T("%.1f%1c"), dValue, szUnit[0] );

	dTemp = dValue;
	for( i=0 ; i <= 3 ; ++i ) {
		if( dTemp >= 1.0 && dTemp < 1000.0 || dTemp > -1000.0 && dTemp <= -1.0 ) {
			_stprintf_s( pSZ, GET_NUMBER_FORMAT_SIZE, _T("%.2f %1c"), dTemp, szUnit[i] );
			break;
		}
		dTemp = ( dTemp * 1000.0 );
	}

	if( i == 4 ) {
		dTemp = dValue;
		for( i=4 ; i <= 7 ; ++i ) {
			dTemp = (float) ( dTemp / 1000.0 );
			if( dTemp >= 1.0 && dTemp < 1000.0 || dTemp > -1000.0 && dTemp <= -1.0 ) {
				_stprintf_s( pSZ, GET_NUMBER_FORMAT_SIZE, _T("%.2f %1c"), dTemp, szUnit[i] );
				break;
			}
		}
	}

	//_stprintf( szVal, _T("%f"), fValue );
	//::GetNumberFormat ( NULL, NULL, szVal, &nFmt, pSZ, GET_NUMBER_FORMAT_SIZE );

	return;
}