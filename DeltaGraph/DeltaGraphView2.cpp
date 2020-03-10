
// DeltaGraphView.cpp : CDeltaGraphView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DeltaGraph.h"
#endif

#include "MainFrm.h"

#include "DeltaGraphDoc.h"
#include "DeltaGraphView2.h"

#include "Pegrpapi.h"

#include "../ShuDeltaGraph/Log/LogDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static TCHAR strMainTitleLabel[2][5][30] = { { _T("방위"), _T("주파수"), _T("DTOA"), _T("신호세기"), _T("펄스폭") },
											 { _T("I/Q 데이터"), _T("순시진폭"), _T("위상차"), _T("FFT") } };

static TCHAR strYAxisLabel[6][30] = { _T(""), _T("방위[도]"), _T("주파수[MHz]"), _T("DTOA[us]"), _T("신호세기[dBm]"), _T("펄스폭[ns]") };


// CDeltaGraphView

IMPLEMENT_DYNCREATE(CDeltaGraphView2, CFormView)

BEGIN_MESSAGE_MAP(CDeltaGraphView2, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CDeltaGraphView2)
	EASYSIZE(IDC_STATIC_GRAPH,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
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
}

BOOL CDeltaGraphView2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CDeltaGraphView2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	InitGraph();

	m_pDoc = ( CDeltaGraphDoc * ) GetDocument();

	INIT_EASYSIZE;

}

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
 * @date      2020/03/09 14:06:43
 * @warning   
 */
void CDeltaGraphView2::InitGraph()
{
	RECT rect;
	TCHAR  szBuffer[200]; 

	GetClientRect( &rect );
	m_hPE = PEcreate(PECONTROL_GRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

	if( m_hPE )
	{
		PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  // v9

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

		// Pass Data //
// 		PEnset(m_hPE, PEP_nSUBSETS, 4);
// 		PEnset(m_hPE, PEP_nPOINTS, 12);
// 
// 		float fY;
// 		for (int s=0; s<=3; s++)
// 		{
// 			for (int p=0; p<=11; p++)
// 			{
// 				fY = 40;
// 				PEvsetcellEx(m_hPE, PEP_faYDATA, s, p, &fY);
// 			}
// 		}

		PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);
		PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_AREA);
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
		int nLineTypes[] = { PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
			PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
			PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID};
		PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 8);

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
void CDeltaGraphView2::ShowGraph( ENUM_SUB_GRAPH enSubGraph )
{
	TCHAR szBuffer[100];

	ENUM_DataType enDataType;

	char *pcDV;
	float *pfX, *pfY, *pfY2;
	UINT uiDataItems;

	DWORD dwColor[2];

	double dMin, dMax;

	void *pData;
	STR_PDW_DATA *pPDWData=NULL;
	STR_IQ_DATA *pIQData=NULL;

	uiDataItems = m_pDoc->GetDataItems();
	enDataType = m_pDoc->GetDataType();
	pData = m_pDoc->GetData();

	if (enDataType == en_PDW_DATA) {
		wsprintf(szBuffer, _T("시간대 %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiDataItems);

		pPDWData = (STR_PDW_DATA *)pData;
	}
	else {
		wsprintf(szBuffer, _T("인덱스대 %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiDataItems);

		pIQData = (STR_IQ_DATA *)pData;
	}

	PEszset(m_hPE, PEP_szMAINTITLE, szBuffer);
	PEszset(m_hPE, PEP_szSUBTITLE, _T("") ); // no subtitle

	if( uiDataItems > 0 ) {
		if (enDataType == en_PDW_DATA && enSubGraph == enSubMenu_3 ) {
			PEnset(m_hPE, PEP_nSUBSETS, 2 );
		}
		else if (enDataType == en_PDW_DATA || ( enDataType == en_IQ_DATA && enSubGraph == enSubMenu_1 ) ) {
			PEnset(m_hPE, PEP_nSUBSETS, 2 );
		}
		else {
			PEnset(m_hPE, PEP_nSUBSETS, 1 );
		}
		PEnset(m_hPE, PEP_nPOINTS, uiDataItems);

		// This allows plotting of zero values //
		double dNill = -9999.0F;
		PEvset(m_hPE, PEP_fNULLDATAVALUE, &dNill, 1);
		PEvset(m_hPE, PEP_fNULLDATAVALUEX, &dNill, 1);

		switch( enSubGraph ) {
			case enSubMenu_1 :
				if (enDataType == en_PDW_DATA) {
					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfAOA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enSubGraph] );

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					dMin = pPDWData->pfTOA[0];
					PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					dMax = pPDWData->pfTOA[uiDataItems - 1];
					PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					dMin = 0.0;
					PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					dMax = 360.0;
					PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
					//PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

					// subset labels //
					PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("DV"));
					PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("DV" ));
				}
				else {
					pfY = pIQData->pfI;
					pfY2 = pIQData->pfQ;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("값"));

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					dMin = 1.0;
					PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					dMax = (double) uiDataItems;
					PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					//PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					//dMin = 0;
					//PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					//dMax = 360.0;
					//PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}

				// subset colors
				dwColor[0] = RGB(0, 198, 0 );
				dwColor[1] = RGB(198, 0, 0 );
				PEvset( m_hPE, PEP_dwaSUBSETCOLORS, dwColor, 2 );
				break;
			default:
				break;
		}

		if (enDataType == en_PDW_DATA) {
			int i;
			float f1 = -9999.0F;

			for (i = 0; i < uiDataItems; ++i) {
				PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, pfX);
				PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, pfY);

				PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i, pfX);
				PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, pfY);

				++pfX;
				++pfY;
			}

			// 첫번재 시간 마크 없애기
			if( enSubGraph == enSubMenu_3 ) {
				PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, & f1);
				PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 0, & f1);
			}

			for (i = 0; i < uiDataItems; ++i) {
				if (*pcDV == PDW_DV) {
					//PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & f1);

				}
				else {
					//PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, & f1);

				}

				++pcDV;
			}

		}
	}

	//PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  // v9

	//////////////////////////////////////////////////////////////////////////
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
	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORIZONTAL);
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


	::InvalidateRect(m_hPE, NULL, FALSE);
	::UpdateWindow(m_hPE);

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
	if( m_hPE ) {
		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
	}

	UPDATE_EASYSIZE;
}
