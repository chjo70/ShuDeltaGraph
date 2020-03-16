
// DeltaGraphView.cpp : CDeltaGraphView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DeltaGraph.h"
#endif

#include "MainFrm.h"
#include "ChildFrm.h"


#include "DeltaGraphDoc.h"
#include "DeltaGraphView2.h"


#include "../ShuDeltaGraph/Log/LogDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static TCHAR strLineType[3][30] = { _T("없음"), _T("실선"), _T("점선") };

static TCHAR strPointSize[4][30] = { _T("Small"), _T("Medium"), _T("Large"), _T("Micro") };

static TCHAR strMainTitleLabel[2][5][30] = { { _T("방위"), _T("주파수"), _T("DTOA"), _T("신호세기"), _T("펄스폭") },
											 { _T("I/Q 데이터"), _T("순시진폭"), _T("위상차"), _T("FFT") } };

static TCHAR strYAxisLabel[2][6][30] = { { _T(""), _T("방위[도]"), _T("주파수[MHz]"), _T("DTOA[us]"), _T("신호세기[dBm]"), _T("펄스폭[ns]") } ,
										 { _T(""), _T("값"), _T("순시진폭[dB]"), _T("도"), _T("세기"), _T("") } };


// CDeltaGraphView

IMPLEMENT_DYNCREATE(CDeltaGraphView2, CFormView)

BEGIN_MESSAGE_MAP(CDeltaGraphView2, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_YAXIS, &CDeltaGraphView2::OnCbnSelchangeComboYaxis)
	ON_CBN_SELCHANGE(IDC_COMBO_LINETYPE, &CDeltaGraphView2::OnCbnSelchangeComboLinetype)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_APPLY, &CDeltaGraphView2::OnBnClickedButtonFilterApply)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_DEAPPLY, &CDeltaGraphView2::OnBnClickedButtonFilterDeapply)
	ON_CBN_SELCHANGE(IDC_COMBO_POINTSIZE, &CDeltaGraphView2::OnCbnSelchangeComboPointsize)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CDeltaGraphView2)
	EASYSIZE(IDC_STATIC_GRAPH,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_COMBO1,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_COMBO2,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,0)
END_EASYSIZE_MAP

// CDeltaGraphView 생성/소멸

CDeltaGraphView2::CDeltaGraphView2()
	: CFormView(CDeltaGraphView2::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	CLEAR_EASYSIZE;
}

CDeltaGraphView2::~CDeltaGraphView2()
{
	CloseGraph();
	//FreeBuffer();
}

void CDeltaGraphView2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_CStaticGraph);
	DDX_Control(pDX, IDC_COMBO_YAXIS, m_CComboYAxis);
	DDX_Control(pDX, IDC_COMBO_LINETYPE, m_CComboLineType);
	DDX_Control(pDX, IDC_STATIC_DATAITEMS, m_CStaticDataItems);
	DDX_Control(pDX, IDC_COMBO_POINTSIZE, m_CComboPointSize);
	DDX_Control(pDX, IDC_STATIC_X1, m_CStaticX1);
	DDX_Control(pDX, IDC_STATIC_X2, m_CStaticX2);
	DDX_Control(pDX, IDC_STATIC_DTOA, m_CStaticDTOA);
}

BOOL CDeltaGraphView2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/12 14:04:18
 * @warning   
 */
 void CDeltaGraphView2::OnInitialUpdate()
{
	

	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	InitVar();

	InitStatic();
	InitCombo();
	InitButton();

	InitGraph();

	DrawGraph( enSubMenu_1 );

	INIT_EASYSIZE;

}

 /**
  * @brief     
  * @param     ENUM_SUB_GRAPH enSubGraph
  * @return    void
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/12 22:43:37
  * @warning   
  */
void CDeltaGraphView2::DrawGraph( ENUM_SUB_GRAPH enSubGraph )
{
	bool bRet;
 	int iFileIndex=0, iFilteredDataItems=0;

	TCHAR szDataItems[100];

	if( m_pDoc->GetDataType() == en_PDW_DATA ) {
		do {
			bRet = m_pDoc->ReadDataFile( iFileIndex );
			iFilteredDataItems += m_pDoc->GetFilteredDataItems();

			if( iFileIndex == 0 ) {
				InitGraph( enSubGraph );
			}

			ShowGraph( enSubGraph, iFileIndex );

			++ iFileIndex;
		} while( bRet == false );
	}
	else {
		bRet = m_pDoc->ReadDataFile( iFileIndex );

		iFilteredDataItems += m_pDoc->GetFilteredDataItems();

 		InitGraph( enSubGraph );
 
 		ShowGraph( enSubGraph, iFileIndex );
	}

	swprintf_s( szDataItems, _countof(szDataItems), _T("%d"), iFilteredDataItems );
	m_CStaticDataItems.SetWindowText( szDataItems );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/15 16:52:33
 * @warning   
 */
void CDeltaGraphView2::ClearGraph()
{
	UINT i;
	float f1 = -9999.0F;
	
	UINT uiPDWDataItems = m_pDoc->GetDataItems();

	for ( i = 0; i < uiPDWDataItems; ++i) {
		PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, & f1);
		PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & f1);

	}

}

/**
 * @brief     
 * @param     UINT
 * @param     CPoint point
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/17 0:32:33
 * @warning   
 */
void CDeltaGraphView2::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDeltaGraphView2::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDeltaGraphView 진단

#ifdef _DEBUG
void CDeltaGraphView2::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeltaGraphView2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDeltaGraphDoc* CDeltaGraphView2::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeltaGraphDoc)));
	return (CDeltaGraphDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeltaGraphView 메시지 처리기

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/17 1:04:34
 * @warning   
 */
void CDeltaGraphView2::InitVar()
{
	CMainFrame *pMainFrame = ( CMainFrame * ) AfxGetMainWnd();

	m_bX = true;
	m_fX1 = m_fX2 = 0.0;

	m_pChild = ( CChildFrame * ) pMainFrame->GetActiveFrame();

	m_pDoc = ( CDeltaGraphDoc * ) GetDocument();

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/17 1:04:31
 * @warning   
 */
void CDeltaGraphView2::InitStatic()
{
	GetDlgItem(IDC_STATIC_X1)->SetWindowText( _T("-") );
	GetDlgItem(IDC_STATIC_X2)->SetWindowText( _T("-") );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/17 1:03:15
 * @warning   
 */
void CDeltaGraphView2::InitButton()
{
	GetDlgItem(IDC_BUTTON_FILTER_DEAPPLY)->EnableWindow( FALSE );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/15 17:14:07
 * @warning   
 */
void CDeltaGraphView2::InitCombo()
{
	int i, iDataType;

	iDataType = (int) m_pDoc->GetDataType();

	m_CComboYAxis.ResetContent();
	if( iDataType == (int) en_PDW_DATA ) {
		for( i=0 ; i < enSubMenu_5 ; ++i ) {
			m_CComboYAxis.AddString( strMainTitleLabel[iDataType-1][i] );
		}
	}
	else {
		for( i=0 ; i < enSubMenu_4 ; ++i ) {
			m_CComboYAxis.AddString( strMainTitleLabel[iDataType-1][i] );
		}
	}
	m_CComboYAxis.SetCurSel( 0 );

	m_CComboLineType.ResetContent();
	for( i=0 ; i <= 2 ; ++i ) {
		m_CComboLineType.AddString( strLineType[i] );
	}
	m_CComboLineType.SetCurSel( 0 );

	m_CComboPointSize.ResetContent();
	for( i=0 ; i <= 3 ; ++i ) {
		m_CComboPointSize.AddString( strPointSize[i] );
	}
	m_CComboPointSize.SetCurSel( 3 );

}

void CDeltaGraphView2::SetCombo( ENUM_SUB_GRAPH enSubGraph )
{
	m_CComboYAxis.SetCurSel( enSubGraph-1 );

	m_CComboLineType.SetCurSel( 0 );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 14:06:43
 * @warning   
 */
void CDeltaGraphView2::InitGraph( ENUM_SUB_GRAPH enSubGraph )
{
	RECT rect;

	if( enSubGraph == enUnselectedSubGraph ) {
		GetClientRect( &rect );
		m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

		if( m_hPE )
		{
			// Enable Bar Glass Effect //
			PEnset(m_hPE, PEP_bBARGLASSEFFECT, TRUE);

			// Enable Plotting style gradient and bevel features //
			PEnset(m_hPE, PEP_nAREAGRADIENTSTYLE,  PEPGS_RADIAL_BOTTOM_RIGHT);
			PEnset(m_hPE, PEP_nAREABEVELSTYLE, PEBS_MEDIUM_SMOOTH);
			PEnset(m_hPE, PEP_nSPLINEGRADIENTSTYLE,  PEPGS_RADIAL_BOTTOM_RIGHT);
			PEnset(m_hPE, PEP_nSPLINEBEVELSTYLE, PESBS_MEDIUM_SMOOTH);

			// v7.2 new features //
			PEnset(m_hPE, PEP_nPOINTGRADIENTSTYLE, PEPGS_VERTICAL_ASCENT_INVERSE);
			PEnset(m_hPE, PEP_dwPOINTBORDERCOLOR, PERGB(100, 0, 0, 0));
			PEnset(m_hPE, PEP_nLINESYMBOLTHICKNESS, 3);
			PEnset(m_hPE, PEP_nAREABORDER, 1);
			PEnset(m_hPE, PEP_bALLOWSVGEXPORT, 1);

			// Prepare images in memory //
			PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);

			PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);
			PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
			//PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_AREA);
			PEnset(m_hPE, PEP_nGRIDLINECONTROL, PEGLC_BOTH);
			PEnset(m_hPE, PEP_nGRIDSTYLE, PEGS_DOT);
			PEnset(m_hPE, PEP_bALLOWRIBBON, TRUE);
			PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
			PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_RO2_NOT);

			// Enable middle mouse dragging //
			PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);
			PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);

			// subset colors //
			DWORD dwArray[4] = { PERGB(128,198,0,0), PERGB(128,0, 198, 198 ), PERGB(128,198,198,0 ), PERGB(128,0,198,0 ) };
			PEvsetEx(m_hPE, PEP_dwaSUBSETCOLORS, 0, 4, dwArray, 0);

			// subset line types //
// 			int nLineTypes[] = { PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
// 				PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
// 				PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID};
// 			PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 8);

			// subset point types //
			int nPointTypes[] = { PEPT_DOTSOLID, PEPT_UPTRIANGLESOLID, 
				PEPT_SQUARESOLID, PEPT_DOWNTRIANGLESOLID, PEPT_DOTSOLID, 
				PEPT_SQUARESOLID, PEPT_DIAMONDSOLID, PEPT_UPTRIANGLESOLID };
			PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPointTypes, 8);

			PEnset(m_hPE, PEP_bSIMPLEPOINTLEGEND, TRUE);
			PEnset(m_hPE, PEP_bSIMPLELINELEGEND, TRUE);
			PEnset(m_hPE, PEP_nLEGENDSTYLE, PELS_1_LINE);

			// Allow stacked type graphs //
			PEnset(m_hPE, PEP_bNOSTACKEDDATA, FALSE);

			// Various other features //
			PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
			PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
			PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);

			PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
			PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);
			PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
			PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
			PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
			PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
			PEnset(m_hPE, PEP_nFONTSIZE, PEFS_SMALL);

			PEnset(m_hPE, PEP_nDATAPRECISION, 1);
			PEnset(m_hPE, PEP_nGRAPHPLUSTABLE, /*PEGPT_BOTH*/PEGPT_GRAPH );
			PEnset(m_hPE, PEP_bMARKDATAPOINTS, FALSE);

			PEnset(m_hPE, PEP_nIMAGEADJUSTLEFT, 20);
			PEnset(m_hPE, PEP_nIMAGEADJUSTRIGHT, 20);
			PEnset(m_hPE, PEP_nIMAGEADJUSTTOP, 10);

			// Set export defaults //
			PEnset(m_hPE, PEP_nDPIX, 600);
			PEnset(m_hPE, PEP_nDPIY, 600);

			PEnset(m_hPE, PEP_nEXPORTSIZEDEF, PEESD_NO_SIZE_OR_PIXEL );
			PEnset(m_hPE, PEP_nEXPORTTYPEDEF, PEETD_PNG );
			PEnset(m_hPE, PEP_nEXPORTDESTDEF, PEEDD_CLIPBOARD );
			PEszset(m_hPE, PEP_szEXPORTUNITXDEF, TEXT("1280"));
			PEszset(m_hPE, PEP_szEXPORTUNITYDEF, TEXT("768"));
			PEnset(m_hPE, PEP_nEXPORTIMAGEDPI, 300 );

			// Set cursor related properties 
			// v9 feature
			//PEnset(m_hPE, PEP_nCURSORPROMPTLOCATION, PECPL_TRACKING_TOOLTIP);
			//PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_YVALUE);
			//PEnset(m_hPE, PEP_bTRACKINGCUSTOMDATATEXT, TRUE);

			// Set up cursor //
			PEnset(m_hPE, PEP_nCURSORMODE, PECM_DATACROSS);

			// Help see data points //
			PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);

		}

	}

	// 서브 그래프 형태에 따른 설정
	else {
		TCHAR szBuffer[100];
		UINT uiDataItems;

		ENUM_DataType enDataType;

		//void *pData;
		STR_PDW_DATA *pPDWData=NULL;
		STR_IQ_DATA *pIQData=NULL;

		DWORD dwColor[2];

		uiDataItems = m_pDoc->GetDataItems();
		enDataType = m_pDoc->GetDataType();
		//pData = m_pDoc->GetData();

		if (enDataType == en_PDW_DATA) {
			wsprintf(szBuffer, _T("시간대 %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiDataItems);

			//pPDWData = (STR_PDW_DATA *)pData;
		}
		else {
			wsprintf(szBuffer, _T("인덱스대 %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiDataItems);

			//pIQData = (STR_IQ_DATA *)pData;
		}

		// 그래프 타이틀 표시
		PEszset(m_hPE, PEP_szMAINTITLE, szBuffer);
		PEszset(m_hPE, PEP_szSUBTITLE, _T("") ); // no subtitle

		// 그래프 데이터 그룹 개수 설정
		PEnset(m_hPE, PEP_nSUBSETS, 2 );

		PEnset(m_hPE, PEP_nPOINTS, uiDataItems);

		// This allows plotting of zero values //
		double dNill = -9999.0F;
		PEvset(m_hPE, PEP_fNULLDATAVALUE, &dNill, 1);
		PEvset(m_hPE, PEP_fNULLDATAVALUEX, &dNill, 1);

		// subset colors
		dwColor[0] = RGB(0, 198, 0 );
		dwColor[1] = RGB(198, 0, 0 );
		PEvset( m_hPE, PEP_dwaSUBSETCOLORS, dwColor, 2 );

		//////////////////////////////////////////////////////////////////////////
		PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  // v9
		PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);
		PEnset(m_hPE, PEP_nCURSORPROMPTLOCATION, PECPL_TRACKING_TOOLTIP);
		PEnset(m_hPE, PEP_nTRACKINGTOOLTIPMAXWIDTH, 200);

		PEnset(m_hPE, PEP_bTRACKINGCUSTOMDATATEXT, TRUE);
		PEnset(m_hPE, PEP_bTRACKINGCUSTOMOTHERTEXT, TRUE);

		// No need to actually click data point, click close to the data point //
		// v9 feature
		PEnset(m_hPE, PEP_bMOUSECURSORCONTROLCLOSESTPOINT, TRUE);

		// This will allow you to move cursor by clicking data point //
		PEnset(m_hPE, PEP_bMOUSECURSORCONTROL, TRUE);
		PEnset(m_hPE, PEP_bALLOWDATAHOTSPOTS, TRUE);


		// Enable MouseWheel and Pinch Smoothing //
		PEnset(m_hPE, PEP_nMOUSEWHEELZOOMSMOOTHNESS, 5);
		PEnset(m_hPE, PEP_nPINCHZOOMSMOOTHNESS, 3);
		float fzf = 1.2F; PEvset(m_hPE, PEP_fMOUSEWHEELZOOMFACTOR, &fzf, 1);
		fzf = 1.1F; PEvset(m_hPE, PEP_fPINCHZOOMFACTOR, &fzf, 1);

		// Enable MouseWheel Zooming
		PEnset(m_hPE, PEP_nMOUSEWHEELFUNCTION, PEMWF_HORZ_ZOOM);

		// Disable some features not ideal for larger data sets.
		PEnset(m_hPE, PEP_nPOINTGRADIENTSTYLE, 0);
		PEnset(m_hPE, PEP_dwPOINTBORDERCOLOR, PERGB(0, 0, 0, 0));
		PEnset(m_hPE, PEP_nLINESYMBOLTHICKNESS, 0);

		// Enable ZoomWindow Feature //
		PEnset(m_hPE, PEP_bZOOMWINDOW, TRUE );

		// Set plotting method to line and allow zooming //
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_LINE);
		//PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
		PEnset(m_hPE, PEP_bALLOWPLOTCUSTOMIZATION, FALSE);

		int nLT = PELT_THINSOLID;
		PEvsetcell(m_hPE, PEP_naSUBSETLINETYPES, 0, &nLT);
		PEvsetcell(m_hPE, PEP_naSUBSETLINETYPES, 1, &nLT);

		PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);

		PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, FALSE);
		PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_LIGHT_SHADOW);
		PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_NONE);

		PEnset(m_hPE, PEP_bALLOWEMFEXPORT, FALSE);
		PEnset(m_hPE, PEP_bALLOWWMFEXPORT, FALSE);
		PEnset(m_hPE, PEP_bALLOWSVGEXPORT, FALSE);

		//PEnset(m_hPE, PEP_nRENDERENGINE, PERE_DIRECT3D);
		//PEnset(m_hPE, PEP_bFORCE3DXNEWCOLORS, 1);
		//PEnset(m_hPE, PEP_bFORCE3DXVERTICEREBUILD, 1);

		// subset labels //
		if (enDataType == en_PDW_DATA) {
			PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("DV"));
			PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("DI" ));
		}
		else {
			PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("I Data"));
			PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("Q Data" ));
		}

		OnCbnSelchangeComboLinetype();

		OnCbnSelchangeComboPointsize();

	}

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 14:06:41
 * @warning   
 */
void CDeltaGraphView2::CloseGraph()
{

	PEdestroy( m_hPE );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 14:06:45
 * @warning   
 */
void CDeltaGraphView2::ShowGraph( ENUM_SUB_GRAPH enSubGraph, int iFileIndex )
{
	ENUM_DataType enDataType;

	char *pcDV;
	float *pfX, *pfY, *pfY2;
	UINT uiPDWDataItems;

	double dMin, dMax;

	void *pData;
	STR_PDW_DATA *pPDWData=NULL;
	STR_IQ_DATA *pIQData=NULL;

	uiPDWDataItems = m_pDoc->GetPDWDataItems();
	enDataType = m_pDoc->GetDataType();
	pData = m_pDoc->GetData();

	if( uiPDWDataItems >= 0 ) {
		switch( enSubGraph ) {
			case enSubMenu_1 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfAOA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enDataType-1][enSubGraph] );

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = pPDWData->pfTOA[0];
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = pPDWData->pfTOA[uiDataItems - 1];
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0.0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
					//PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

				}
				else {
					pIQData = (STR_IQ_DATA *)pData;

					pfY = pIQData->pfI;
					pfY2 = pIQData->pfQ;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = 1.0;
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = (double) uiPDWDataItems;
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}
				break;

			case enSubMenu_2 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfFreq;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enDataType-1][enSubGraph] );

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = pPDWData->pfTOA[0];
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = pPDWData->pfTOA[uiDataItems - 1];
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0.0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
					//PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

				}
				else {
					pIQData = (STR_IQ_DATA *)pData;

					pfY = pIQData->pfPA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

// 					PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
// 					dMin = 1.0;
// 					PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
// 					dMax = (double) uiPDWDataItems;
// 					PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}
				break;

			case enSubMenu_3 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfDTOA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enDataType-1][enSubGraph] );

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = pPDWData->pfTOA[0];
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = pPDWData->pfTOA[uiDataItems - 1];
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0.0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
					//PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

				}
				else {
					pIQData = (STR_IQ_DATA *)pData;

					pfY = pIQData->pfIP;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = 1.0;
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = (double) uiPDWDataItems;
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}
				break;

			case enSubMenu_4 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfPA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enDataType-1][enSubGraph] );

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = pPDWData->pfTOA[0];
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = pPDWData->pfTOA[uiDataItems - 1];
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0.0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
					//PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

				}
				else {
					pIQData = (STR_IQ_DATA *)pData;

					pfY = pIQData->pfFFT;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = 1.0;
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = (double) uiPDWDataItems;
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}
				break;

			case enSubMenu_5 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfPW;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enDataType-1][enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enDataType-1][enSubGraph] );

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					//dMin = pPDWData->pfTOA[0];
					//PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					//dMax = pPDWData->pfTOA[uiDataItems - 1];
					//PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0.0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
					//PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

				}
				else {
					pIQData = (STR_IQ_DATA *)pData;

					pfY = pIQData->pfI;
					pfY2 = pIQData->pfQ;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("값"));

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					dMin = 1.0;
					PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					dMax = (double) uiPDWDataItems;
					PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}
				break;

			default:
				break;
		}

		if (enDataType == en_PDW_DATA) {
			UINT i;
			float f1 = -9999.0F;

			if( uiPDWDataItems > 0 ) {
				for (i = 0; i < uiPDWDataItems; ++i) {
					PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i+(iFileIndex*PDW_ITEMS), pfX);
					PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i+(iFileIndex*PDW_ITEMS), pfY);

					PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i+(iFileIndex*PDW_ITEMS), pfX);
					PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i+(iFileIndex*PDW_ITEMS), pfY);

					++pfX;
					++pfY;
				}

				// 첫번재 시간 마크 없애기
				if( enSubGraph == enSubMenu_3 ) {
					PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, & f1);
					PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 0, & f1);
				}

				for (i = 0; i < uiPDWDataItems; ++i) {
					if (*pcDV == PDW_DV) {
						PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i+(iFileIndex*PDW_ITEMS), & f1);

					}
					else {
						PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i+(iFileIndex*PDW_ITEMS), & f1);

					}

					++pcDV;
				}
			}
			else {
				//PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, & f1);
				//PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 0, & f1);
			}

		}
		else {
			UINT i;
			float f1 = -9999.0F;

			if( uiPDWDataItems > 0 ) {
				for (i = 0; i < uiPDWDataItems; ++i) {
					float fVal;

					fVal = (float) ( i + 1 );

					if( _finite( *pfY ) == 0 ) {
						TRACE( "\n %d, %f" , i , *pfY );
						*pfY = -100.0;
					}

					PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i+(iFileIndex*IQ_ITEMS), & fVal );
					PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i+(iFileIndex*IQ_ITEMS), pfY);

					if( enSubGraph == enSubMenu_1 ) {
						PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i+(iFileIndex*IQ_ITEMS), & fVal );
						PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i+(iFileIndex*IQ_ITEMS), pfY2 );

						++pfY2;
					}

					++pfY;
					
				}

				// 첫번재 시간 마크 없애기
				if( enSubGraph == enSubMenu_3 ) {
					PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, & f1);
					PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 0, & f1);
				}

			}
		}
	}

 	PEnset(m_hPE, PEP_bSCROLLINGHORZZOOM, TRUE);

	//
	PEnset(m_hPE, PEP_bSIMPLEPOINTLEGEND, TRUE);
	PEnset(m_hPE, PEP_bSIMPLELINELEGEND, TRUE);
	PEnset(m_hPE, PEP_nLEGENDSTYLE, PELS_1_LINE);

	// Allow stacked type graphs //
	PEnset(m_hPE, PEP_bNOSTACKEDDATA, FALSE);

	// Various other features //
	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);

	PEnset(m_hPE, PEP_nDATAPRECISION, 1);
	PEnset(m_hPE, PEP_nGRAPHPLUSTABLE, PEGPT_BOTH);
	//PEnset(m_hPE, PEP_bMARKDATAPOINTS, FALSE);

	::InvalidateRect(m_hPE, NULL, FALSE);
	::UpdateWindow(m_hPE);

	Log( enNormal, _T("%d 개수를 그립니다."), uiPDWDataItems );

}

/**
 * @brief     
 * @param     UINT nType
 * @param     int cx
 * @param     int cy
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 16:15:53
 * @warning   
 */
 void CDeltaGraphView2::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UPDATE_EASYSIZE;

 	if( m_hPE && m_CStaticGraph ) {
 		CRect rect;

  		m_CStaticGraph.GetWindowRect( & rect );
		ScreenToClient( rect );

		//TRACE( "(%d %d) ~ (%d, %d)" , rect.left, rect.top, rect.right, rect.bottom );

 		::MoveWindow( m_hPE, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, FALSE );
 	}

	
}


 /**
  * @brief     
  * @return    void
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/12 22:41:47
  * @warning   
  */
 void CDeltaGraphView2::OnCbnSelchangeComboYaxis()
 {
	 // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	 int iCombo= m_CComboYAxis.GetCurSel() + 1;

	 ClearGraph();
	 DrawGraph( (ENUM_SUB_GRAPH) iCombo );

 }


 /**
  * @brief     
  * @return    void
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/13 0:32:46
  * @warning   
  */
 void CDeltaGraphView2::OnCbnSelchangeComboLinetype()
 {
	 // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	 int iCombo= m_CComboLineType.GetCurSel();

	 switch( iCombo ) {
		case 0 :
			{
				// subset line types //
				int nLineTypes[] = { PELT_THINSOLID, PELT_THINSOLID, 
					PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
					PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID};
				PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 2);

			   PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);

			   PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
			   PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
			}
			break;

		case 1 :
			{
				PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_LINE);
				// subset line types //
				int nLineTypes[] = { PELT_THINSOLID, PELT_THINSOLID, 
					PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
					PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID};
				PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 2);

				// subset point types //
// 				int nPointTypes[] = { PEPT_DOTSOLID, PEPT_UPTRIANGLESOLID, 
// 					PEPT_SQUARESOLID, PEPT_DOWNTRIANGLESOLID, PEPT_DOTSOLID, 
// 					PEPT_SQUARESOLID, PEPT_DIAMONDSOLID, PEPT_UPTRIANGLESOLID };
// 				PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPointTypes, 8);

				PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
				PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_LARGE);
			}
			break;

		case 2 :
			{
				PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_LINE);
				// subset line types //
				int nLineTypes[] = { PELT_DOT, PELT_DOT, 
					PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
					PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID};
				PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 2);

				// subset point types //
// 				int nPointTypes[] = { PEPT_DOT, PEPT_UPTRIANGLE, 
// 					PEPT_SQUARESOLID, PEPT_DOWNTRIANGLESOLID, PEPT_DOTSOLID, 
// 					PEPT_SQUARESOLID, PEPT_DIAMONDSOLID, PEPT_UPTRIANGLESOLID };
// 				PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPointTypes, 8);

				PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
				PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_LARGE);
			}
			break;

		default :
			break;

	 }

	 //PEnset(m_hPE, PEP_nRENDERENGINE, PERE_DIRECT2D);

	 ::InvalidateRect(m_hPE, NULL, FALSE);
	 ::UpdateWindow(m_hPE);

 }
 
 /**
  * @brief     
  * @return    void
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/14 15:59:22
  * @warning   
  */
 void CDeltaGraphView2::OnBnClickedButtonFilterApply()
 {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ENUM_DataType enDataType;
	ENUM_SUB_GRAPH enSubGraph;

	STR_FILTER_SETUP *pFilterSetup;

	GetDlgItem( IDC_BUTTON_FILTER_DEAPPLY )->EnableWindow( TRUE );

	enDataType = m_pDoc->GetDataType();

	pFilterSetup =m_pDoc->GetFilterSetup();
	enSubGraph = ( ENUM_SUB_GRAPH ) ( m_CComboYAxis.GetCurSel() + 1 );
 
 	switch( enSubGraph ) {
	case enSubMenu_1 :
		if (enDataType == en_PDW_DATA) {
			PEvget(m_hPE, PEP_fZOOMMINX, & pFilterSetup->dToaMin );
			PEvget(m_hPE, PEP_fZOOMMAXX, & pFilterSetup->dToaMax );

			PEvget(m_hPE, PEP_fZOOMMINY, & pFilterSetup->dAoaMin );
			PEvget(m_hPE, PEP_fZOOMMAXY, & pFilterSetup->dAoaMax );
		}
		else {

		}
		break;
	case enSubMenu_2 :
		if (enDataType == en_PDW_DATA) {
			PEvget(m_hPE, PEP_fZOOMMINX, & pFilterSetup->dToaMin );
			PEvget(m_hPE, PEP_fZOOMMAXX, & pFilterSetup->dToaMax );

			PEvget(m_hPE, PEP_fZOOMMINY, & pFilterSetup->dFrqMin );
			PEvget(m_hPE, PEP_fZOOMMAXY, & pFilterSetup->dFrqMax );
		}
		else {

		}
		break;
	case enSubMenu_3 :
		if (enDataType == en_PDW_DATA) {
			PEvget(m_hPE, PEP_fZOOMMINX, & pFilterSetup->dToaMin );
			PEvget(m_hPE, PEP_fZOOMMAXX, & pFilterSetup->dToaMax );

			PEvget(m_hPE, PEP_fZOOMMINY, & pFilterSetup->dDtoaMin );
			PEvget(m_hPE, PEP_fZOOMMAXY, & pFilterSetup->dDtoaMax );
		}
		else {

		}
		break;

	case enSubMenu_4 :
		if (enDataType == en_PDW_DATA) {
			PEvget(m_hPE, PEP_fZOOMMINX, & pFilterSetup->dToaMin );
			PEvget(m_hPE, PEP_fZOOMMAXX, & pFilterSetup->dToaMax );

			PEvget(m_hPE, PEP_fZOOMMINY, & pFilterSetup->dPAMin );
			PEvget(m_hPE, PEP_fZOOMMAXY, & pFilterSetup->dPAMax );
		}
		else {

		}
		break;

	case enSubMenu_5 :
		if (enDataType == en_PDW_DATA) {
			PEvget(m_hPE, PEP_fZOOMMINX, & pFilterSetup->dToaMin );
			PEvget(m_hPE, PEP_fZOOMMAXX, & pFilterSetup->dToaMax );

			PEvget(m_hPE, PEP_fZOOMMINY, & pFilterSetup->dPWMin );
			PEvget(m_hPE, PEP_fZOOMMAXY, & pFilterSetup->dPWMax );
		}
		else {

		}
		break;

	default:
		break;
	}
	
	DrawGraph( enSubGraph );


 }

 /**
  * @brief     
  * @return    void
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/15 16:26:20
  * @warning   
  */
 void CDeltaGraphView2::OnBnClickedButtonFilterDeapply()
 {
	 // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	 GetDlgItem(IDC_BUTTON_FILTER_DEAPPLY)->EnableWindow( FALSE );

	 m_pDoc->ClearFilterSetup();

	 ENUM_SUB_GRAPH enSubGraph= (ENUM_SUB_GRAPH ) ( m_CComboYAxis.GetCurSel() + 1 );
	 DrawGraph( enSubGraph );

 }


 /**
  * @brief     
  * @param     WPARAM wParam
  * @param     LPARAM lParam
  * @return    BOOL
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/15 21:10:27
  * @warning   
  */
BOOL CDeltaGraphView2::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (lParam != (LPARAM) m_hPE)
		return __super::OnCommand(wParam, lParam);
	else {
		if( (HIWORD(wParam) == PEWN_CUSTOMTRACKINGDATATEXT) ) {
			double dX, dY;
			
			ENUM_DataType enDataType;

			TCHAR buffer[200]; 
			TCHAR szBuffer[200]; 

			PEvget(m_hPE, PEP_fCURSORVALUEX, &dX);
			PEvget(m_hPE, PEP_fCURSORVALUEY, &dY);

			int iCombo= m_CComboYAxis.GetCurSel();

			enDataType = m_pDoc->GetDataType();

			wsprintf(szBuffer, _T("%s"), strMainTitleLabel[enDataType - 1][iCombo] );
			PEszset( m_hPE, PEP_szTRACKINGTOOLTIPTITLE, szBuffer );

			switch( iCombo+1 ) {
				case enSubMenu_1 :
					_stprintf_s(buffer, _countof(buffer), TEXT("시간: %.3f[us]\n방위: %.3f[도]"), dX, dY );
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(255,0,0,0));
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
					break;
				case enSubMenu_2 :
					_stprintf_s(buffer, _countof(buffer), _T("시간: %.3f[us]\n주파수: %.3f[MHz]"), dX, dY );
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(255,0,0,0));
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
					break;
				case enSubMenu_3 :
					_stprintf_s(buffer, _countof(buffer), _T("시간: %.3f[us]\nDTOA: %.3f[us]"), dX, dY );
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(255,0,0,0));
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
					break;
				case enSubMenu_4 :
					_stprintf_s(buffer, _countof(buffer), TEXT("시간: %.3f[us]\n세기: %.3f[dBm]"), dX, dY );
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(255,0,0,0));
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
					break;
				case enSubMenu_5 :
					_stprintf_s(buffer, _countof(buffer), _T("시간: %.3f[us]\n펄스폭: %.3f[ns]"), dX, dY );
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPBKCOLOR, PERGB(255,0,0,0));
					PEnset(m_hPE, PEP_dwTRACKINGTOOLTIPTEXTCOLOR, PERGB(0,245,0,0));
					break;
			}

			PEszset(m_hPE, PEP_szTRACKINGTEXT, buffer);
			
		}
		else if( HIWORD(wParam) == PEWN_MOUSEMOVE ) {
			POINT pt;
			HOTSPOTDATA hsd;
			TCHAR buffer[128];
			TCHAR buffer2[128];

			// get last mouse location within control //
			PEvget(m_hPE, PEP_ptLASTMOUSEMOVE, &pt);

			// call PEgethotspot //
			PEgethotspot(m_hPE, pt.x, pt.y);

			// now look at HotSpotData structure //
			PEvget(m_hPE, PEP_structHOTSPOTDATA, &hsd);

			if (hsd.nHotSpotType == PEHS_DATAPOINT) {
				// get ydata value at hot spot //
				float yvalue;
				PEvgetcellEx(m_hPE, PEP_faYDATA, hsd.w1, hsd.w2, &yvalue);
				_stprintf_s(buffer, _countof(buffer), TEXT("DataPoint value %.2f, s=%d,p=%d"), yvalue, hsd.w1, hsd.w2);
			}
			else if (hsd.nHotSpotType == PEHS_SUBSET) {
				PEvgetcell(m_hPE, PEP_szaSUBSETLABELS, hsd.w1, buffer2);
				_stprintf_s(buffer, _countof(buffer), TEXT("Subset Legend is %s"), buffer2);
			}
			else if (hsd.nHotSpotType == PEHS_POINT) {
				PEvgetcell(m_hPE, PEP_szaPOINTLABELS, hsd.w1, buffer2);
				_stprintf_s(buffer, _countof(buffer), TEXT("Point Label is %s"), buffer2);
			}
			else {
				// v9 features 
				int nResult;
				nResult = PEsearchsubsetpointindex(m_hPE, pt.x, pt.y);
				if (nResult)
				{
					int nCS = PEnget(m_hPE, PEP_nCLOSESTSUBSETINDEX);
					int nCP = PEnget(m_hPE, PEP_nCLOSESTPOINTINDEX);
					{
						lstrcpy(buffer, TEXT(" "));
						_stprintf_s(buffer, _countof(buffer), TEXT("가까운 점 s=%d,p=%d"), nCS, nCP);
					}
				}
				else {
					lstrcpy(buffer, TEXT("No hotspot, Outside of charting area"));
				}
			}

 			CMainFrame *pMainFrame = ( CMainFrame * ) AfxGetMainWnd();
 			pMainFrame->SetChildWindowText( buffer );

			//CWnd* pParent = GetParent()->GetParent();
			//if (pParent) {pParent->SetWindowText(buffer);}
		}

		else if( HIWORD(wParam) == PEWN_CLICKED ) {
			HOTSPOTDATA hsd;

			// now look at HotSpotData structure //
			PEvget(m_hPE, PEP_structHOTSPOTDATA, &hsd);

			SetData( & hsd );

		}
	}

	return __super::OnCommand(wParam, lParam);

}

 /**
  * @brief     
  * @param     HOTSPOTDATA * pHSD
  * @return    void
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/17 0:38:15
  * @warning   
  */
void CDeltaGraphView2::SetData( HOTSPOTDATA *pHSD )
{
	ENUM_DataType enDataType;

	TCHAR szBuffer[100];

	float fValue;

	enDataType = m_pDoc->GetDataType();

	if( pHSD->w1 == 0xbaadf00d || pHSD->w2 == 0xbaadf00d )
		return;

	if (enDataType == en_PDW_DATA) {
		STR_PDW_DATA *pPDWData = (STR_PDW_DATA *) m_pDoc->GetData();

		fValue = pPDWData->pfTOA[pHSD->w2];
		swprintf_s( szBuffer, _countof(szBuffer), _T("%.3f"), fValue );

	}
	else {
		STR_IQ_DATA *pIQData = (STR_IQ_DATA *) m_pDoc->GetData();

		fValue = (float) pHSD->w2;
		swprintf_s( szBuffer, _countof(szBuffer), _T("%.3f"), fValue );
	}

	if( m_bX ) {
		m_CStaticX1.SetWindowText( szBuffer );
		m_fX1 = fValue;
		swprintf_s( szBuffer, _countof(szBuffer), _T("%.3f"), m_fX1-m_fX2 );
		
	 }
	 else {
		 m_CStaticX2.SetWindowText( szBuffer );
		 m_fX2 = fValue;
		 swprintf_s( szBuffer, _countof(szBuffer), _T("%.3f"), m_fX2-m_fX1 );
	 }

	 m_CStaticDTOA.SetWindowText( szBuffer );

	 m_bX = ! m_bX;

 }


 /**
  * @brief     
  * @return    void
  * @author    조철희 (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/17 0:32:16
  * @warning   
  */
void CDeltaGraphView2::OnCbnSelchangeComboPointsize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCombo= m_CComboPointSize.GetCurSel();

	switch( iCombo ) {
	case 0 :
		PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_SMALL);
		break;

	case 1 :
		PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MEDIUM);
		break;

	case 2 :
		PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_LARGE);
		break;

	case 3 :
		PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_MICRO);
		break;

	default :
		break;

	 }

	//PEnset(m_hPE, PEP_nRENDERENGINE, PERE_DIRECT2D);

	::InvalidateRect(m_hPE, NULL, FALSE);
	::UpdateWindow(m_hPE);
}
