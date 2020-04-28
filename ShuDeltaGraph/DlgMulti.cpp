// DlgMulti.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgMulti.h"
#include "afxdialogex.h"

#include "Pegrpapi.h"

#define GetRandom( min, max ) ((rand() % (int)(((max)+1) - (min))) + (min))


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
	//InitGraph();
	InitGraph2();

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
	int j;
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	m_pSonataData = pApp->m_pDlgColList->GetSONATAData();

	m_hMulti = CreateEvent( NULL, TRUE, FALSE, NULL );

	_spOneSec = 50000000.;
	_spOneMilli = FDIV( _spOneSec, 1000. );
	_spOneMicrosec = FDIV( _spOneMilli, 1000. );
	_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

	m_pfPA = ( float * ) malloc( sizeof(float)*16*1024 );
	m_pfIndex = ( float * ) malloc( sizeof(float)*16*1024 );
	m_pfIP = ( float * ) malloc( sizeof(float)*16*1024 );

	m_pGlobalXData = new float[400000];

	for (j = 0; j <= 99999; j++)
	{
		m_pGlobalXData[j] = (j + 1) * .01F;
		m_pGlobalXData[j + 100000] = (j + 1) * .01F;
		m_pGlobalXData[j + 200000] = (j + 1) * .01F;
		m_pGlobalXData[j + 300000] = (j + 1) * .01F;
	}

	m_pGlobalYData = new float[500000];

	float fShift = (float) GetRandom(1, 90);
	for (j=0; j<=124999; j++)
		m_pGlobalYData[j] = (sin(0.003F * (double) j + fShift) * 5.0F) + 10 + (float) GetRandom(1, 100)/10.0F ;
	fShift = (float) GetRandom(1, 90);
	for (j=0; j<=124999; j++)
		m_pGlobalYData[j+125000] = (sin(0.002F * (double) j + fShift) * 5.0F) + 20 + (float) GetRandom(1, 100)/10.0F ;
	fShift = (float) GetRandom(1, 90);
	for (j=0; j<=124999; j++)
		m_pGlobalYData[j+250000] = (sin(0.003F * (double) j + fShift) * 5.0F) + 30 + (float) GetRandom(1, 100)/10.0F ;
	fShift = (float) GetRandom(1, 90);
	for (j=0; j<=124999; j++)
		m_pGlobalYData[j+375000] = (sin(0.003F * (double) j + fShift) * 5.0F) + 40 + (float) GetRandom(1, 100)/10.0F ;

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
	double d;

	GetClientRect( &rect );

	m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1002);

	// v9 features new default CursorPromptLocation = ToolTip
	//PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  
	//PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);

	// Enable Bar Glass Effect //
	PEnset(m_hPE, PEP_bBARGLASSEFFECT, TRUE);

	// Enable Plotting style gradient and bevel features //
	//PEnset(m_hPE, PEP_nAREAGRADIENTSTYLE,  PEPGS_VERTICAL_ASCENT_INVERSE);
	//PEnset(m_hPE, PEP_nAREABEVELSTYLE, PEBS_MEDIUM_SMOOTH);
	//PEnset(m_hPE, PEP_nSPLINEGRADIENTSTYLE,  PEPGS_VERTICAL_ASCENT_INVERSE);
	//PEnset(m_hPE, PEP_nSPLINEBEVELSTYLE, PESBS_MEDIUM_SMOOTH);

	// v7.2 new features //
	//PEnset(m_hPE, PEP_nPOINTGRADIENTSTYLE,PEPGS_VERTICAL);// PEPGS_VERTICAL_ASCENT_INVERSE);
	//PEnset(m_hPE, PEP_dwPOINTBORDERCOLOR, PERGB(100, 0, 0, 0));
	//PEnset(m_hPE, PEP_nLINESYMBOLTHICKNESS, 3);
	//PEnset(m_hPE, PEP_nAREABORDER, 1);
	//PEnset(m_hPE, PEP_bALLOWSVGEXPORT, 1);

	PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE, PEP_bCACHEBMP, TRUE);

	// Set number of Subsets and Points //
	//PEnset(m_hPE, PEP_nSUBSETS, 3);	
	//PEnset(m_hPE, PEP_nPOINTS, 10 );	

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
	//PEnset(m_hPE, PEP_nMOUSEWHEELZOOMSMOOTHNESS, 5);
	//PEnset(m_hPE, PEP_nPINCHZOOMSMOOTHNESS, 2);

	// Set DataShadows to show shadows
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);

	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSSPLINE);
	PEnset(m_hPE, PEP_nGRIDLINECONTROL, PEGLC_BOTH);
	PEnset(m_hPE, PEP_nGRIDSTYLE, PEGS_DOT);
	//PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORIZONTAL);
	//PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_RO2_NOT);

	// Enable middle mouse dragging //
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);

	PEszset(m_hPE, PEP_szMAINTITLE, TEXT("멀티 그래프"));
	PEszset(m_hPE, PEP_szSUBTITLE, TEXT("")); // no subtitle
	//PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("빈도율[%]"));
//	PEszset(m_hPE, PEP_szXAXISLABEL, TEXT("시간[us]"));

	// subset labels
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("주파수"));
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("신호세기" ));
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 2, TEXT("DTOA" ));

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
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_LIGHT_INSET );

	PEnset(m_hPE, PEP_nAUTOMINMAXPADDING, 1);

	PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
	PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
	PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_SMALL);
	//PEnset(m_hPE, PEP_bSCROLLINGHORZZOOM, TRUE);
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
	//PEnset(m_hPE, PEP_nCURSORMODE, PECM_DATACROSS);

	// Help see data points //
	PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);

	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, FALSE);
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_BAR );		

	//PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
	PEnset(m_hPE, PEP_nMOUSEWHEELFUNCTION, PEMWF_HORZPLUSVERT_ZOOM);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);  // note that pan gestures require MouseDragging to be enabled 
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);  

	// Enable MouseWheel Zoom Smoothness
	//PEnset(m_hPE, PEP_nMOUSEWHEELZOOMSMOOTHNESS, 5);
	//PEnset(m_hPE, PEP_nPINCHZOOMSMOOTHNESS, 2);

	int nArray[3] = {1,1,1};
	PEvset (m_hPE, PEP_naMULTIAXESSUBSETS, nArray, 3);

	// Add Axis Separator //
	PEnset(m_hPE, PEP_nMULTIAXESSEPARATORS, PEMAS_THIN);

	// subset colors
	DWORD dwArray[3] = { PERGB(128,198,0,0), PERGB(128, 0, 198, 198 ), PERGB(128, 198,198,0 ) };
	PEvsetEx( m_hPE, PEP_dwaSUBSETCOLORS, 0, 3, dwArray, 0 );


	PEnset(m_hPE, PEP_nSUBSETS, 3 );
	PEnset(m_hPE, PEP_nPOINTS, 16*1024 );

	// Set XData, it does not change
	PEvsetW(m_hPE, PEP_faXDATA, m_pGlobalXData, 16384*3 );

	// Clear out default y data, start with an empty chart, not really noticable with this fast of real-time //
	float val = 0;
	PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, &val);
	PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 1, &val);
	PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 2, &val);

	// Set first axis parameters //
// 	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
// 	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("주파수"));
// 	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);
// 	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,00| GHz") );
// 	PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[0] );
// 	PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
// 	d = 500;
// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
// 	d = 18000.0;
// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);
// 	PEnset(m_hPE, PEP_nSHOWTICKMARKY, PESTM_TICKS_HIDE); 
// 	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);
// 
// 	// Set second axis parameters //
// 	PEnset(m_hPE, PEP_nWORKINGAXIS, 1);
// 	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("신호세기"));
// 	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);
// 	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,| dBm") );
// 	PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[1] );
// 	PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
// 	d = -60;
// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
// 	d = 0.0F;
// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);
// 
// 	// Set third axis parameters //
// 	PEnset(m_hPE, PEP_nWORKINGAXIS, 2);
// 	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("DTOA"));
// 	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE );
// 	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|.0|s") );
// 	PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[2] );
// 	PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
	// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	// 	d = 0;
	// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
	// 	d = 18000.0F;
	// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

	// Reset WorkingAxis when done //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

	// Axis Sizing //
	PEnset(m_hPE, PEP_nMULTIAXESSIZING, TRUE);

	// Other various properties //
	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	PEnset(m_hPE, PEP_bMARKDATAPOINTS, FALSE);
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);

	// Set some random annotations //       
	//PEnset(m_hPE, PEP_bALLOWANNOTATIONCONTROL, TRUE);

	//PEnset(m_hPE, PEP_bZOOMWINDOW, TRUE);

	// v9 features 
	//PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);
	//PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);
	//PEnset(m_hPE, PEP_nCURSORPROMPTLOCATION, PECPL_TRACKING_TOOLTIP);

	//PEnset(m_hPE, PEP_bTRACKINGCUSTOMDATATEXT, TRUE);
	//PEnset(m_hPE, PEP_bTRACKINGCUSTOMOTHERTEXT, TRUE);

// 	PEnset(m_hPE, PEP_bALLOWTITLEHOTSPOTS, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWSUBSETHOTSPOTS, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWAXISLABELHOTSPOTS, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWGRIDNUMBERHOTSPOTSY, TRUE);
// 	PEnset(m_hPE, PEP_bALLOWGRIDNUMBERHOTSPOTSX, TRUE);
// 
// 	PEnset(m_hPE, PEP_bMOUSECURSORCONTROLCLOSESTPOINT, TRUE);

	//PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,|") );

	// Setting to help with new Direct3D rendering and example expanded to 400K points //
	//PEnset(m_hPE, PEP_bNULLDATAGAPS, FALSE);
	//PEnset(m_hPE, PEP_nHOURGLASSTHRESHOLD, 10000000);
	//PEnset(m_hPE, PEP_nFILTER2D, 1);
	//PEnset(m_hPE, PEP_bAUTOIMAGERESET, FALSE);


}

void CDlgMulti::InitGraph2()
{
	RECT rect;
	GetClientRect( &rect );

	// Construct Object //
	m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

	PEnset(m_hPE, PEP_nSUBSETS, 3 );
	PEnset(m_hPE, PEP_nPOINTS, 100000);

	int nval;
	// Create 4 separate axes and then overlap all //
	nval = 1;
	PEvsetcell(m_hPE, PEP_naMULTIAXESSUBSETS, 0, &nval);
	PEvsetcell(m_hPE, PEP_naMULTIAXESSUBSETS, 1, &nval);
	PEvsetcell(m_hPE, PEP_naMULTIAXESSUBSETS, 2, &nval);
	//nval = 2; PEvsetcell(m_hPE, PEP_naOVERLAPMULTIAXES, 0, &nval);
	//nval = 1; PEvsetcell(m_hPE, PEP_naOVERLAPMULTIAXES, 1, &nval);
	//PEvsetcell(m_hPE, PEP_naOVERLAPMULTIAXES, 2, &nval);

	// Match axis color and label to subset label //
	double d;
	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("Signal 1"));
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	d = 0.0F;  PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
	d = 30.0F; PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

	PEnset(m_hPE, PEP_nWORKINGAXIS, 1);
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("Signal 2"));
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	d = 10.0F;  PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
	d = 35.0F; PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

	PEnset(m_hPE, PEP_nWORKINGAXIS, 2);
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("Signal 3"));
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	d = 30.0F;  PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
	d = 40.0F; PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);  // v9 

// 	PEnset(m_hPE, PEP_nWORKINGAXIS, 3);
// 	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("Signal 4"));
// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
// 	d = 42.0F;  PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
// 	d = 48.0F; PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);
// 	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);  // v9 

	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

	// subset colors //
	DWORD dwArray[4] = { PERGB(255,255,255,255), PERGB(255,0, 255, 255 ), PERGB(255,255,255, 0), PERGB(255,0,255,0 ) };
	PEvsetEx(m_hPE, PEP_dwaSUBSETCOLORS, 0, 4, dwArray, 0);

	// subset line types //
	int nLineTypes[] = { PELT_THINSOLID, PELT_THINSOLID, PELT_THINSOLID, PELT_THINSOLID};
	PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 4);

	// Manually configure x and y axes //
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
	d = 0.0F;    PEvset(m_hPE, PEP_fMANUALMINX, &d, 1);
	d = 100.0F;  PEvset(m_hPE, PEP_fMANUALMAXX, &d, 1);

	// Set XData, it does not change
	PEvsetW(m_hPE, PEP_faXDATA, m_pGlobalXData, 400000);

	// Clear out default y data, start with an empty chart, not really noticable with this fast of real-time //
	float val = 0;
	PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, &val);
	PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 1, &val);
	PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 2, &val);
	PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 3, &val);

	PEszset(m_hPE, PEP_szMAINTITLE, TEXT("Scientific Real-Time"));
	PEszset(m_hPE, PEP_szSUBTITLE, TEXT(""));
	PEnset(m_hPE, PEP_bNORANDOMPOINTSTOEXPORT, TRUE);
	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	PEnset(m_hPE, PEP_bALLOWBAR, FALSE);

	// v9 features, support for Point plotting method
	PEnset(m_hPE, PEP_bALLOWPOPUP, TRUE);
	PEnset(m_hPE, PEP_bZOOMWINDOW, TRUE);
	PEnset(m_hPE, PEP_bSCROLLINGHORZZOOM, TRUE);
	PEnset(m_hPE, PEP_nMOUSEWHEELFUNCTION, PEMWF_HORZ_ZOOM);
	PEnset(m_hPE, PEP_bALLOWPOINT, TRUE);
	PEnset(m_hPE, PEP_bALLOWPLOTCUSTOMIZATION, TRUE);
	PEnset(m_hPE, PEP_bALLOWPOINTSPLUSLINE, FALSE);
	PEnset(m_hPE, PEP_bALLOWPOINTSPLUSSPLINE, FALSE);
	PEnset(m_hPE, PEP_bALLOWBESTFITLINE, FALSE);
	PEnset(m_hPE, PEP_bALLOWBESTFITCURVE, FALSE);
	PEnset(m_hPE, PEP_bALLOWAREA, FALSE);
	PEnset(m_hPE, PEP_bALLOWBAR, FALSE);
	PEnset(m_hPE, PEP_bALLOWSPLINE, FALSE);

	PEnset(m_hPE, PEP_bALLOWPAGE2, TRUE); 
	PEnset(m_hPE, PEP_bALLOWAXISPAGE, FALSE);
	PEnset(m_hPE, PEP_bALLOWSUBSETSPAGE, FALSE);

	PEnset(m_hPE, PEP_bALLOWTEXTEXPORT, FALSE);
	PEnset(m_hPE, PEP_bALLOWEMFEXPORT, FALSE);
	PEnset(m_hPE, PEP_bALLOWWMFEXPORT, FALSE);
	PEnset(m_hPE, PEP_bALLOWSVGEXPORT, FALSE);

	PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
	PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);

	// Set Various Other Properties //

	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);  
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);

	PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
	PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
	PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_bLINESHADOWS, FALSE);
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_NONE);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_MEDIUM);
	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE, PEP_bSHOWLEGEND, FALSE);
	PEnset(m_hPE, PEP_bGRIDINFRONT, TRUE);
	PEnset(m_hPE, PEP_nGRIDLINECONTROL, PEGLC_BOTH);

	PEnset(m_hPE, PEP_nIMAGEADJUSTRIGHT, 100);
	PEnset(m_hPE, PEP_nIMAGEADJUSTLEFT, 20);

	// Set export defaults //
	PEnset(m_hPE, PEP_nDPIX, 600);
	PEnset(m_hPE, PEP_nDPIY, 600);

	// Setting to help with new Direct3D rendering and example expanded to 400K points //
// 	PEnset(m_hPE, PEP_bNULLDATAGAPS, FALSE);
// 	PEnset(m_hPE, PEP_nHOURGLASSTHRESHOLD, 10000000);
// 	PEnset(m_hPE, PEP_nFILTER2D, 1);
// 	PEnset(m_hPE, PEP_bAUTOIMAGERESET, FALSE);

	PEnset(m_hPE, PEP_bSHOWANNOTATIONS, TRUE);
	PEszset(m_hPE, PEP_szRIGHTMARGIN, TEXT("XXXXXXXXXXXX" ));		
	PEnset(m_hPE, PEP_nIMAGEADJUSTLEFT, 100);

	CMDIFrameWnd* pWnd = (CMDIFrameWnd*) AfxGetApp()->GetMainWnd();
	PEnset(m_hPE, PEP_nRENDERENGINE, PERE_DIRECT3D);

	// Composite2D3D // Faster settings are 1-force only one D2D layer in back, 2-force only one D2D layer in front 
	// Default setting 0 creates 2 D2D layers, one in back ground and one in foreground, allowing normal separation 
	// of graphics as properties dictate.
	PEnset(m_hPE, PEP_nCOMPOSITE2D3D, PEC2D_FOREGROUND);

	::InvalidateRect(m_hPE, NULL, FALSE);
	::UpdateWindow(m_hPE);


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

	free( m_pfPA );
	free( m_pfIndex );
	free( m_pfIP );

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
			//pDlg->ViewGraph();
			pDlg->ViewGraph2();

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

	float fFreq, fPA, fTOA, fFirstToa, fDtoa, fPreviousToa, fXToa;

	double d;

	uiItem = m_pSonataData->uiItem;

	if( m_pSonataData->enDataType == en_PDW_DATA ) {
		TNEW_PDW *pPDW;
		TNEW_PDW temp;

		pPDW = & m_pSonataData->unRawData.stPDWData[0];

		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("주파수"));
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("신호세기" ));
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 2, TEXT("DTOA" ));

		PEszset(m_hPE, PEP_szXAXISLABEL, TEXT("시간[us]"));

		PEnset(m_hPE, PEP_nPOINTS, uiItem );

		for( i=0 ; i < uiItem ; ++i ) {
			temp.bpdw[0][0] = pPDW->item.toa_1;
			temp.bpdw[0][1] = pPDW->item.toa_2;
			temp.bpdw[0][2] = pPDW->item.toa_3;
			temp.bpdw[0][3] = pPDW->item.toa_4;

			uiToa = temp.wpdw[0];
			//fTOA = FDIV(uiToa, _spOneSec );
			fXToa = FDIV(uiToa*20., 1000. );
			fTOA = fXToa / 1000000.;

			if( i == 0 ) {
				fFirstToa = fXToa;
				fPreviousToa = fTOA;
				fDtoa = 0;
			}
			else {
				fDtoa = fTOA - fPreviousToa;
				fPreviousToa = fTOA;
			}

			uiTemp32 = BIT_MERGE( pPDW->item.frequency_h, pPDW->item.frequency_l);
			fFreq = FFRQCNV( pPDW->item.band + 1, uiTemp32 );
			fFreq = (float) ( fFreq / 1000. );

			uiTemp32 = pPDW->item.amplitude;
			fPA = FPACNV(uiTemp32);

			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, & fXToa );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, & fFreq );

			PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i, & fXToa );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & fPA );

			PEvsetcellEx(m_hPE, PEP_faXDATA, 2, i, & fXToa );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 2, i, & fDtoa );

			++ pPDW;
		}

		double dMin, dMax;
		PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
		dMin = fFirstToa;
		PEvset(m_hPE, PEP_fMANUALMINX, & dMin, 1);
		dMax = fXToa;
		PEvset(m_hPE, PEP_fMANUALMAXX, & dMax, 1);

		// Reset WorkingAxis when done //
		PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
	}
	else {
		TNEW_IQ *pIQ;

		float fVal1, fVal2;
		float *pfPA, *pfIndex, *pfIP;

		// subset colors
		DWORD dwArray[3] = { PERGB(128,198,0,0), PERGB(128, 0, 198, 198 ), PERGB(128, 198,198,0 ) };
		PEvsetEx( m_hPE, PEP_dwaSUBSETCOLORS, 0, 3, dwArray, 0 );

		PEszset(m_hPE, PEP_szXAXISLABEL, TEXT("인덱스"));

		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("위상차"));
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("순시진폭" ));
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 2, TEXT("FFT" ));

		PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,|"));

		// Set first axis parameters //
		PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
		PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("위상차[도]"));
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);
		PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|도"));
		PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[0] );
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
		// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
		// 	d = 500;
		// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
		// 	d = 18000.0F;
		// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

		// Set second axis parameters //
		PEnset(m_hPE, PEP_nWORKINGAXIS, 1);
		PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("순시진폭[dB]"));
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);
		PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,| dB"));
		PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[1] );
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
		// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
		// 	d = -60;
		// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
		// 	d = 0.0F;
		// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

		// Set third axis parameters //
		PEnset(m_hPE, PEP_nWORKINGAXIS, 2);
		PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("크기"));
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE );
		PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));
		PEnset(m_hPE, PEP_dwYAXISCOLOR, dwArray[2] );
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
		// 	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
		// 	d = 0;
		// 	PEvset(m_hPE, PEP_fMANUALMINY, &d, 1);
		// 	d = 18000.0F;
		// 	PEvset(m_hPE, PEP_fMANUALMAXY, &d, 1);

		// Reset WorkingAxis when done //
		PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

		PEnset(m_hPE, PEP_nPOINTS, uiItem );

		pIQ = & m_pSonataData->unRawData.stIQData[0];

		pfPA = m_pfPA;
		pfIndex = m_pfIndex;
		pfIP = m_pfIP;
		for( i=0 ; i < uiItem ; ++i ) {
			float fI, fQ, fPA, fIndex;

			fI = (float) pIQ->sI;
			fQ = (float) pIQ->sQ;

			// 순시 진폭
			fPA = sqrt( fI * fI + fQ * fQ );
			*pfPA++ = (float) (20.*log10( fPA ) ) - (float) 80.;

			if( i == 0 ) {
				*pfIP = 0.0;
				fVal2 = (float) ( atan2( fQ, fI ) * RAD2DEG );
			}
			else {
				fVal1 = (float) ( atan2( fQ, fI ) * RAD2DEG );
				*pfIP = fVal1 - fVal2;
				fVal2 = fVal1;
			}
			if( *pfIP > 180. ) {
				*pfIP -= ( 2 * 180. );
			}
			else if( *pfIP < -180 ) {
				*pfIP += ( 2 * 180. );
			}
			else {

			}

			*pfIndex = (float) i;
			
			//PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, pfIndex );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, pfIP );

			//PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i, pfIndex );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & fPA );

			//PEvsetcellEx(m_hPE, PEP_faXDATA, 2, i, pfIndex );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 2, i, & fPA );

			pfIndex++;

			++ pfIP;

			++ pIQ;

		}

		// Composite2D3D // Faster settings are 1-force only one D2D layer in back, 2-force only one D2D layer in front 
		// Default setting 0 creates 2 D2D layers, one in back ground and one in foreground, allowing normal separation 
		// of graphics as properties dictate.
		PEnset(m_hPE, PEP_nCOMPOSITE2D3D, PEC2D_FOREGROUND);

		//PEnset(m_hPE, PEP_nSUBSETS, 3 );
		//PEnset(m_hPE, PEP_nPOINTS, uiItem );
		PEnset(m_hPE, PEP_bANTIALIASGRAPHICS, TRUE);

		PEvsetEx(m_hPE, PEP_faXDATA, uiItem*0, 10000, m_pfIndex, 0 );
		//PEvsetEx(m_hPE, PEP_faYDATA, uiItem*0, 10000, m_pfIP, 0);

		PEvsetEx(m_hPE, PEP_faXDATA, uiItem*1, 10000, m_pfIndex, 0 );
		//PEvsetEx(m_hPE, PEP_faYDATA, uiItem*1, 10000, m_pfPA, 0);

		PEvsetEx(m_hPE, PEP_faXDATA, uiItem*2, 10000, m_pfIndex, 0 );
		//PEvsetEx(m_hPE, PEP_faYDATA, uiItem*2, 10000, m_pfPA, 0);

		double dMin, dMax;
		PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
		dMin = 0;
		PEvset(m_hPE, PEP_fMANUALMINX, & dMin, 1);
		dMax = (double) uiItem;
		PEvset(m_hPE, PEP_fMANUALMAXX, & dMax, 1);

		// Reset WorkingAxis when done //
		PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
	}

	::InvalidateRect(m_hPE, NULL, FALSE);

}

void CDlgMulti::ViewGraph2()
{
	int i;
	RECT rec;

	TCHAR szTmp[48];

	int nHTypes;
	float fShift;
	double fYLoc;
	DWORD dwArray;
	TCHAR buffer[40];

	PEnset(m_hPE, PEP_nSUBSETS, 3);
	PEnset(m_hPE, PEP_nPOINTS, 100000);
	PEnset(m_hPE, PEP_bANTIALIASGRAPHICS, TRUE);   // Direct2D is often faster with anti alias for some reason. 

	// Perform the actual transfer of data, all y data is repassed //
	i = GetRandom(1, 4500); // randomize the start of waveform data to produce variation
	PEvsetEx(m_hPE, PEP_faYDATA, 0, 100000, &m_pGlobalYData[i], NULL);
	PEvsetEx(m_hPE, PEP_faYDATA, 100000, 100000, &m_pGlobalYData[i + 125000], NULL);
	PEvsetEx(m_hPE, PEP_faYDATA, 200000, 100000, &m_pGlobalYData[i + 250000], NULL);
	//PEvsetEx(m_hPE, PEP_faYDATA, 300000, 100000, &m_pGlobalYData[i + 375000], NULL);

	//Add a random line annotation //
	fShift = 13.0F + (float) GetRandom(1, 3);
	fYLoc = fShift;  PEvset (m_hPE, PEP_faHORZLINEANNOTATION, &fYLoc, 1);
	nHTypes = PELT_THICKDOT;  PEvset (m_hPE, PEP_naHORZLINEANNOTATIONTYPE, &nHTypes, 1);
	dwArray = PERGB( 255,255,0,0); PEvset( m_hPE, PEP_dwaHORZLINEANNOTATIONCOLOR, &dwArray, 1 );

	_stprintf(buffer, TEXT("|R%.2f"), fShift);
	PEvsetcell(m_hPE, PEP_szaHORZLINEANNOTATIONTEXT, 0, buffer);
	PEnset(m_hPE, PEP_bANNOTATIONSINFRONT, TRUE);
	PEnset(m_hPE, PEP_bSHOWANNOTATIONS, TRUE);
	PEszset(m_hPE, PEP_szRIGHTMARGIN, TEXT("XXXXXXXXXXXX" ));		
	PEnset(m_hPE, PEP_nIMAGEADJUSTLEFT, 100);

	// Construct a table annotation //
	PEnset(m_hPE, PEP_nWORKINGTABLE, 0);
	PEnset(m_hPE, PEP_nTAROWS, 7);
	PEnset(m_hPE, PEP_nTACOLUMNS, 1);

	float fp;
	int nTmp;
	int r, c;
// 	c = 0;
// 	nTmp = PERGB(255, 255, 0, 0);
// 	for (r=0; r<7; r++)
// 	{
// 		fp = (sin(0.3F * (double) c + 1) * 500.0F) + GetRandom(2, 50);
// 		_stprintf(szTmp, TEXT("%.3f"), fp);
// 		PEvsetcellEx(m_hPE, PEP_szaTATEXT, r, 0, szTmp);
// 		PEvsetcellEx(m_hPE, PEP_dwaTACOLOR, r, 0, &nTmp); 
// 	}
// 	PEnset(m_hPE, PEP_bSHOWTABLEANNOTATION, TRUE);
// 	PEnset(m_hPE, PEP_nTALOCATION, PETAL_INSIDE_PIXEL_UNITS);
// 	GetClientRect(&rec);
// 	PEnset(m_hPE, PEP_nTAX, rec.right - 80);
// 	PEnset(m_hPE, PEP_nTAY, 10);
// 	PEnset(m_hPE, PEP_dwTABACKCOLOR, PERGB( 0, 0, 0, 0) );
// 	PEnset(m_hPE, PEP_nTATEXTSIZE, 95);
// 	PEnset(m_hPE, PEP_nTABORDER, PETAB_NO_BORDER);

	// Construct a simple table annotation //
// 	PEnset(m_hPE, PEP_nWORKINGTABLE, 1);  
// 	PEnset(m_hPE, PEP_nTAROWS, 17);
// 	PEnset(m_hPE, PEP_nTACOLUMNS, 1);
// 	for (r=0; r<17; r++)
// 	{
// 		fp = (sin(0.3F * (double) r + 1) * 500.0F) + GetRandom(2, 50);
// 		_stprintf(szTmp, TEXT("%.3f"), fp);
// 		PEvsetcellEx(m_hPE, PEP_szaTATEXT, r, 1, szTmp);
// 		PEvsetcellEx(m_hPE, PEP_dwaTACOLOR, r, 0, &nTmp); 
// 	}
// 	PEnset(m_hPE, PEP_bSHOWTABLEANNOTATION, TRUE);
// 	PEnset(m_hPE, PEP_nTALOCATION, PETAL_INSIDE_PIXEL_UNITS);
// 	PEnset(m_hPE, PEP_nTAX, rec.right - 80);
// 	PEnset(m_hPE, PEP_nTAY, rec.bottom - 265);
// 	PEnset(m_hPE, PEP_dwTABACKCOLOR, PERGB( 0,0,0,0));
// 	PEnset(m_hPE, PEP_nTATEXTSIZE, 95);
// 	PEnset(m_hPE, PEP_nTABORDER, PETAB_NO_BORDER);

	// Construct a simple table annotation //
// 	PEnset(m_hPE, PEP_nWORKINGTABLE, 2);  
// 	PEnset(m_hPE, PEP_nTAROWS, 3);
// 	PEnset(m_hPE, PEP_nTACOLUMNS, 2);
// 	c = 0;
// 	for (r=0; r<3; r++)
// 	{
// 		PEvsetcellEx(m_hPE, PEP_szaTATEXT, r, c, TEXT(""));
// 		nTmp = PEGAT_SQUARESOLID;
// 		PEvsetcellEx(m_hPE, PEP_naTATYPE, r, c, &nTmp);
// 
// 		if (((int) fShift) == 15)
// 		{
// 			int nRnd;
// 			nRnd = GetRandom(1, 3);
// 			if (nRnd < 2)
// 			{
// 				nTmp = PERGB(255, 0, 255, 0);
// 				PEvsetcellEx(m_hPE, PEP_dwaTACOLOR, r, c, &nTmp); 
// 			}
// 			else
// 			{
// 				nTmp = PERGB(255, 255, 255, 0);
// 				PEvsetcellEx(m_hPE, PEP_dwaTACOLOR, r, c, &nTmp); 
// 			}
// 		}
// 		else
// 		{
// 			nTmp = PERGB(255, 255, 255, 255);
// 			PEvsetcellEx(m_hPE, PEP_dwaTACOLOR, r, c, &nTmp); 
// 		}
// 	}

	c = 1;
	for (r=0; r<3; r++)
	{
		PEvsetcellEx(m_hPE, PEP_szaTATEXT, r, c, TEXT(""));
		nTmp = PEGAT_SQUARESOLID;
		PEvsetcellEx(m_hPE, PEP_naTATYPE, r, c, &nTmp);

		if (((int) fShift) == 16)
		{
			nTmp = PERGB(255, 255, 0, 0);
			PEvsetcellEx(m_hPE, PEP_dwaTACOLOR, r, c, &nTmp); 
		}
		else
		{
			nTmp = PERGB(255, 255, 255, 255);
			PEvsetcellEx(m_hPE, PEP_dwaTACOLOR, r, c, &nTmp); 
		}
	}

// 	PEnset(m_hPE, PEP_bSHOWTABLEANNOTATION, TRUE);
// 	PEnset(m_hPE, PEP_nTALOCATION, PETAL_INSIDE_PIXEL_UNITS);
// 	PEnset(m_hPE, PEP_nTAX, 15);
// 	PEnset(m_hPE, PEP_nTAY, rec.bottom - 75);
// 	PEnset(m_hPE, PEP_dwTABACKCOLOR, PERGB( 0,0,0,0));
// 	PEnset(m_hPE, PEP_nTATEXTSIZE, 95);
// 	PEnset(m_hPE, PEP_nTABORDER, PETAB_NO_BORDER);

	if (((int) fShift) == 16)
	{
		nTmp = PERGB(255, 255, 0, 0);
		PEnset(m_hPE, PEP_dwYAXISCOLOR, nTmp); 
	}
	else
	{
		nTmp = PERGB(255, 255, 255, 255);
		PEnset(m_hPE, PEP_dwYAXISCOLOR, nTmp); 
	}

	if (PEnget(m_hPE, PEP_nRENDERENGINE) == PERE_DIRECT3D)
		PEreconstruct3dpolygons(m_hPE);
	else
	{
		PEreinitialize(m_hPE); // Because 115 create sets AutoImageReset = False, we need to call PEreinitialize to re-filter new data
		PEresetimage(m_hPE, 0, 0);
	}
	::InvalidateRect(m_hPE, NULL, FALSE);


}

#define GET_NUMBER_FORMAT_SIZE		(200)

BOOL CDlgMulti::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*
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
	}		*/

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