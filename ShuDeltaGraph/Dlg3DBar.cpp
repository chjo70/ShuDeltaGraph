// Dlg3DBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "Dlg3DBar.h"
#include "afxdialogex.h"

#include "Pegrpapi.h"



DWORD WINAPI Func3DBar( LPVOID lpData );

// 3D Bar 그래프를 그리기 위한 범위
#define START_OF_FREQ_MHZ		(FREQ_MIN)
#define END_OF_FREQ_MHZ			(FREQ_MAX)
#define STEP_OF_FREQ_MHZ		(500)
#define THREED_BAR_DEPPTH		(26)


// CDlg3DBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg3DBar, CDialogEx)

CDlg3DBar::CDlg3DBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg3DBar::IDD, pParent)
{

}

/**
 * @brief     
 * @return    
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:28:07
 * @warning   
 */
CDlg3DBar::~CDlg3DBar()
{
	FreeBuffer();
}

void CDlg3DBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg3DBar, CDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZING()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CDlg3DBar)
	EASYSIZE(IDC_STATIC_3DBAR,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP


// CDlg3DBar 메시지 처리기입니다.


BOOL CDlg3DBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	this->GetWindowRect( m_rectCurDlg );

	Log( enNormal, _T("CDlg3DBar +++++++++++++++++++++++++++++++++++++++++++") );

	// 초기 설정
	InitButton();
	InitBuffer();
	InitVar();
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
 * @date      2020/02/20 21:42:46
 * @warning   
 */
void CDlg3DBar::InitVar()
{
	memset( m_pfFreqX, 0, sizeof(float)*m_nSubset );
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:44
 * @warning   
 */
void CDlg3DBar::InitButton()
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
void CDlg3DBar::InitBuffer()
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	m_pFreqXFromHisto = pApp->m_pDlg2DHisto->GetFreqX();

	m_h3DBar = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_nSubset = (END_OF_FREQ_MHZ-START_OF_FREQ_MHZ)/STEP_OF_FREQ_MHZ + 1;

	m_pfFreqX = ( float * ) malloc( sizeof(float ) * m_nSubset );
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:42:40
 * @warning   
 */
void CDlg3DBar::InitListCtrl()
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
void CDlg3DBar::InitStatusBar()
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
void CDlg3DBar::InitStatic()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:39:52
 * @warning   
 */
void CDlg3DBar::InitGraph()
{
	RECT rect;
	TCHAR  szBuffer[200]; 

	GetClientRect( &rect );
	m_hPE = PEcreate(PECONTROL_3D, WS_VISIBLE, &rect, m_hWnd, 1001);

	// Non-Surface chart, set PolyMode //
	PEnset(m_hPE, PEP_nPOLYMODE, PEPM_3DBAR);

	Point3D p3d;
	p3d.X = -8.0F;    p3d.Y = -1.8F;    p3d.Z = 6.3F;
	PEvset(m_hPE, PEP_struct3DXLIGHT0, &p3d, 1);

	// Define quantity of data //
	
	PEnset(m_hPE, PEP_nSUBSETS, m_nSubset);
	PEnset(m_hPE, PEP_nPOINTS, THREED_BAR_DEPPTH );

	// Set all data to start with zero //
	int s;
	int p;
	float fZero = 0.0F, fFreq;
	for( s=0, fFreq=START_OF_FREQ_MHZ ; fFreq <= END_OF_FREQ_MHZ ; fFreq += STEP_OF_FREQ_MHZ ) {
		_stprintf_s(szBuffer, 200, _T("%.1f"), (float) fFreq/1000. );
		PEvsetcell(m_hPE, PEP_szaSUBSETLABELS, s, szBuffer );

		for (p=0; p<THREED_BAR_DEPPTH; p++)
		{
			PEvsetcellEx(m_hPE, PEP_faYDATA, s, p, &fZero);
		}
		++ s;
	}

	// Set initial point labels for x axis //
	TCHAR szEmpty[] = TEXT(" ");
	for (p=0; p < THREED_BAR_DEPPTH ; p++)
		PEvsetcell(m_hPE, PEP_szaPOINTLABELS, p, szEmpty);

	// Manually scale y axis //
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	double dMinY = 0.0F;    PEvset(m_hPE, PEP_fMANUALMINY, &dMinY, 1);
	double dMaxY = 100.0F;  PEvset(m_hPE, PEP_fMANUALMAXY, &dMaxY, 1);

	// Set Titles //
	PEszset(m_hPE, PEP_szMAINTITLE, TEXT(""));
	PEszset(m_hPE, PEP_szSUBTITLE, TEXT("||휠마우스 줌. Mouse Drag rotates. 마우스 드래그+쉬프트시 이미지 이동. "));
	PEvsetcell(m_hPE, PEP_szaMULTISUBTITLES, 0, TEXT("||더블 클릭시 회전 시작/멈춤. 마우스 드래그+중간 rotates light. "));

	PEnset(m_hPE, PEP_nRENDERENGINE, PERE_DIRECT3D); 

	PEszset(m_hPE, PEP_szXAXISLABEL, TEXT("시간[초]"));
	PEszset(m_hPE, PEP_szYAXISLABEL, TEXT("빈도율[%]"));
	PEszset(m_hPE, PEP_szZAXISLABEL, TEXT("주파수[GHz]"));

	float fZ = 2.0F;  PEvset(m_hPE, PEP_fGRIDASPECTZ, &fZ, 1);
	float fX = 5.0F;  PEvset(m_hPE, PEP_fGRIDASPECTX, &fX, 1);

	// Set eye/camera distance, or Zoom amount //
	PEnset(m_hPE, PEP_nVIEWINGHEIGHT, 29);				   
	PEnset(m_hPE, PEP_nDEGREEOFROTATION, 312);

	float fMyZoom = -4.57F; PEvset(m_hPE, PEP_f3DXZOOM, &fMyZoom, 1);

	// Set a chart translation //
	float fTweakX = 1.8F;  PEvset(m_hPE, PEP_f3DXVIEWPORTX, &fTweakX, 1);
	float fTweakY = 1.8F; PEvset(m_hPE, PEP_f3DXVIEWPORTY, &fTweakY, 1);
	PEnset(m_hPE, PEP_bDEGREEPROMPTING, TRUE);

	// Set Plotting methods //
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, 2);   

	// Add padding around chart //
	PEnset(m_hPE, PEP_nIMAGEADJUSTLEFT, 50);
	PEnset(m_hPE, PEP_nIMAGEADJUSTRIGHT, 50);
	PEnset(m_hPE, PEP_nIMAGEADJUSTBOTTOM, 50);

	// Set Various Other Properties //
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_SMALL);
	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);

	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, FALSE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);
	PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	PEnset(m_hPE, PEP_bALLOWSTYLEPAGE, FALSE);
	PEnset(m_hPE, PEP_bALLOWPLOTCUSTOMIZATION, FALSE);
	PEnset(m_hPE, PEP_bALLOWWIREFRAME, FALSE);
	PEnset(m_hPE, PEP_nROTATIONSPEED, 50);
	PEnset(m_hPE, PEP_nROTATIONINCREMENT, PERI_INCBY1);

	// export defaults //
	PEnset(m_hPE, PEP_nDPIX, 600);
	PEnset(m_hPE, PEP_nDPIY, 600);
	PEnset(m_hPE, PEP_nEXPORTSIZEDEF, PEESD_NO_SIZE_OR_PIXEL );
	PEnset(m_hPE, PEP_nEXPORTTYPEDEF, PEETD_PNG );
	PEnset(m_hPE, PEP_nEXPORTDESTDEF, PEEDD_CLIPBOARD );
	PEszset(m_hPE, PEP_szEXPORTUNITXDEF, TEXT("1280"));
	PEszset(m_hPE, PEP_szEXPORTUNITYDEF, TEXT("768"));
	PEnset(m_hPE, PEP_nEXPORTIMAGEDPI, 300 );
	PEnset(m_hPE, PEP_bALLOWTEXTEXPORT, FALSE);
	PEnset(m_hPE, PEP_bALLOWEMFEXPORT, FALSE);
	PEnset(m_hPE, PEP_bALLOWWMFEXPORT, FALSE);
	PEnset(m_hPE, PEP_bALLOWSVGEXPORT, FALSE);

	// Enable mouse dragging //
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);

	PEreinitialize(m_hPE);
	PEresetimage(m_hPE, 0, 0);
	::InvalidateRect(m_hPE, 0, 0);

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:39:51
 * @warning   
 */
void CDlg3DBar::InitToolTip()
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
void CDlg3DBar::InitThread()
{
	m_theThread.Attach( Func3DBar );
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
void CDlg3DBar::FreeBuffer()
{
	m_theThread.Stop();
	//KillTimer( CLK_TIMER );

	CloseHandle( m_h3DBar );

	if (m_hPE)
	{
		PEdestroy(m_hPE);
		m_hPE = 0; 
	}

	free( m_pfFreqX );
	
}

/**
 * @brief     
 * @param     UINT nType
 * @param     int cx
 * @param     int cy
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:39:48
 * @warning   
 */
void CDlg3DBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_hPE ) {
		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
	}

	UPDATE_EASYSIZE;
}


void CDlg3DBar::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LPMINMAXINFO lpminmax = ( LPMINMAXINFO ) lpMMI;

	// 창 최소크기 지정

	lpminmax->ptMinTrackSize.x = m_rectCurDlg.right - m_rectCurDlg.left;
	lpminmax->ptMinTrackSize.y = m_rectCurDlg.bottom - m_rectCurDlg.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

DWORD WINAPI Func3DBar( LPVOID lpData )
{
	CDlg3DBar *pDlg;

	CThread *pParent = reinterpret_cast<CThread*>(lpData);
	pDlg = ( CDlg3DBar * ) pParent->GetParam();

	pDlg->InitVar();

	while( TRUE ) {
		DWORD dRet;

		dRet = WaitForSingleObject( pDlg->m_h3DBar, INFINITE );

		if( dRet == WAIT_FAILED ) {
			break;
		}
		else if( dRet == WAIT_ABANDONED ) {
			ResetEvent( pDlg->m_h3DBar );
			continue;
		}
		else if( dRet == WAIT_TIMEOUT ) {
			continue;
		}
		else {
			ResetEvent( pDlg->m_h3DBar );
			//pDlg->UpdateMulti();
			pDlg->ViewGraph();

		}

	}

	return 0;

}

/**
 * @brief     
 * @param     UINT fwSide
 * @param     LPRECT pRect
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:44:19
 * @warning   
 */
void CDlg3DBar::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EASYSIZE_MINSIZE(m_rectCurDlg.right - m_rectCurDlg.left,m_rectCurDlg.bottom - m_rectCurDlg.top,fwSide,pRect);
}


void CDlg3DBar::OnSysCommand(UINT nID, LPARAM lParam)
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
 * @param     UINT_PTR nIDEvent
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 1:44:08
 * @warning   
 */
void CDlg3DBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case CLK_TIMER:
		//SetEvent( m_h3DBar );
		//1초마다 처리할 내용
		break;

	}

	CDialogEx::OnTimer(nIDEvent);
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/22 17:21:53
 * @warning   
 */
void CDlg3DBar::ViewGraph()
{
	int i, iIndex;

	CTime t;
	CString strTime;

	t = CTime::GetCurrentTime();
	strTime = t.Format("%I:%M:%S");

	// Graph Real Time Feed //
	PEvset(m_hPE, PEP_szaAPPENDPOINTLABELDATA, (void *) (LPCTSTR) strTime, 1);

	InitVar();

	for( i=FREQ_MIN ; i < FREQ_MAX ; i++ )	{
		iIndex = ( i - FREQ_MIN ) / STEP_OF_FREQ_MHZ;
		m_pfFreqX[iIndex] = max( m_pfFreqX[iIndex], m_pFreqXFromHisto[i] );
	}

	// transfer new YData //
	PEvset(m_hPE, PEP_faAPPENDYDATA, m_pfFreqX, 1);

	// causes 3D object to reconstruct polygon data //
	PEreconstruct3dpolygons(m_hPE);
	::InvalidateRect(m_hPE, NULL, FALSE);

}