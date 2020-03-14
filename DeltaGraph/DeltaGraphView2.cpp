
// DeltaGraphView.cpp : CDeltaGraphView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

static TCHAR strLineType[3][30] = { _T("����"), _T("�Ǽ�"), _T("����") };

static TCHAR strMainTitleLabel[2][5][30] = { { _T("����"), _T("���ļ�"), _T("DTOA"), _T("��ȣ����"), _T("�޽���") },
											 { _T("I/Q ������"), _T("��������"), _T("������"), _T("FFT") } };

static TCHAR strYAxisLabel[6][30] = { _T(""), _T("����[��]"), _T("���ļ�[MHz]"), _T("DTOA[us]"), _T("��ȣ����[dBm]"), _T("�޽���[ns]") };


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
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CDeltaGraphView2)
	EASYSIZE(IDC_STATIC_GRAPH,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_COMBO1,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_COMBO2,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,0)
END_EASYSIZE_MAP

// CDeltaGraphView ����/�Ҹ�

CDeltaGraphView2::CDeltaGraphView2()
	: CFormView(CDeltaGraphView2::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	DDX_Control(pDX, IDC_COMBO_LINETYPE, m_ComboLineType);
}

BOOL CDeltaGraphView2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/12 14:04:18
 * @warning   
 */
 void CDeltaGraphView2::OnInitialUpdate()
{

	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	m_pDoc = ( CDeltaGraphDoc * ) GetDocument();

	InitGraph();

	DrawGraph( enSubMenu_1 );


	INIT_EASYSIZE;

}

 /**
  * @brief     
  * @param     ENUM_SUB_GRAPH enSubGraph
  * @return    void
  * @author    ��ö�� (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/12 22:43:37
  * @warning   
  */
 void CDeltaGraphView2::DrawGraph( ENUM_SUB_GRAPH enSubGraph )
 {
	bool bRet;
 	int iFileIndex=0;

	InitCombo( enSubGraph );

	do {
		bRet = m_pDoc->ReadDataFile( iFileIndex );

		if( iFileIndex == 0 ) {
			InitGraph( enSubGraph );
		}

		ShowGraph( enSubGraph, iFileIndex );

		++ iFileIndex;
	} while( bRet == false );

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


// CDeltaGraphView ����

#ifdef _DEBUG
void CDeltaGraphView2::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeltaGraphView2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDeltaGraphDoc* CDeltaGraphView2::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeltaGraphDoc)));
	return (CDeltaGraphDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeltaGraphView �޽��� ó����

void CDeltaGraphView2::InitCombo( ENUM_SUB_GRAPH enSubGraph )
{
	int i, iDataType;

	iDataType = (int) m_pDoc->GetDataType();

	m_CComboYAxis.ResetContent();
	for( i=0 ; i < 5 ; ++i ) {
		m_CComboYAxis.AddString( strMainTitleLabel[iDataType-1][i] );
	}
	m_CComboYAxis.SetCurSel( enSubGraph-1 );

	m_ComboLineType.ResetContent();
	for( i=0 ; i <= 2 ; ++i ) {
		m_ComboLineType.AddString( strLineType[i] );
	}
	m_ComboLineType.SetCurSel( 0 );

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 14:06:43
 * @warning   
 */
void CDeltaGraphView2::InitGraph( ENUM_SUB_GRAPH enSubGraph )
{
	RECT rect;
	TCHAR  szBuffer[200]; 

	if( enSubGraph == enUnselectedSubGraph ) {
		GetClientRect( &rect );
		m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

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
		}

	}

	// ���� �׷��� ���¿� ���� ����
	else {
		TCHAR szBuffer[100];
		UINT uiPDWDataItems;

		ENUM_DataType enDataType;

		void *pData;
		STR_PDW_DATA *pPDWData=NULL;
		STR_IQ_DATA *pIQData=NULL;

		DWORD dwColor[2];

		uiPDWDataItems = m_pDoc->GetPDWDataItems();
		enDataType = m_pDoc->GetDataType();
		pData = m_pDoc->GetData();

		if (enDataType == en_PDW_DATA) {
			wsprintf(szBuffer, _T("�ð��� %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiPDWDataItems);

			pPDWData = (STR_PDW_DATA *)pData;
		}
		else {
			wsprintf(szBuffer, _T("�ε����� %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiPDWDataItems);

			pIQData = (STR_IQ_DATA *)pData;
		}

		// �׷��� Ÿ��Ʋ ǥ��
		PEszset(m_hPE, PEP_szMAINTITLE, szBuffer);
		PEszset(m_hPE, PEP_szSUBTITLE, _T("") ); // no subtitle

		// �׷��� ������ �׷� ���� ����
		PEnset(m_hPE, PEP_nSUBSETS, 2 );

		PEnset(m_hPE, PEP_nPOINTS, uiPDWDataItems);

		// This allows plotting of zero values //
		double dNill = -9999.0F;
		PEvset(m_hPE, PEP_fNULLDATAVALUE, &dNill, 1);
		PEvset(m_hPE, PEP_fNULLDATAVALUEX, &dNill, 1);

		// subset colors
		dwColor[0] = RGB(0, 198, 0 );
		dwColor[1] = RGB(198, 0, 0 );
		PEvset( m_hPE, PEP_dwaSUBSETCOLORS, dwColor, 2 );

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
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, TEXT("DV"));
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, TEXT("DI" ));

		OnCbnSelchangeComboLinetype();

	}

}

/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ð�[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enSubGraph] );

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

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ε���"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("��"));

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

			case enSubMenu_2 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfFreq;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ð�[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enSubGraph] );

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

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ε���"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("��"));

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

			case enSubMenu_3 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfDTOA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ð�[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enSubGraph] );

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

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ε���"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("��"));

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

			case enSubMenu_4 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfPA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ð�[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enSubGraph] );

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

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ε���"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("��"));

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

			case enSubMenu_5 :
				if (enDataType == en_PDW_DATA) {
					pPDWData = (STR_PDW_DATA *)pData;

					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfPW;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ð�[us]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enSubGraph] );

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

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("�ε���"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("��"));

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
			int i;
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

				// ù���� �ð� ��ũ ���ֱ�
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
				PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, & f1);
				PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 0, & f1);
			}

		}
	}

	PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  // v9

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

}

/**
 * @brief     
 * @param     UINT nType
 * @param     int cx
 * @param     int cy
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 16:15:53
 * @warning   
 */
 void CDeltaGraphView2::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
  * @author    ��ö�� (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/12 22:41:47
  * @warning   
  */
 void CDeltaGraphView2::OnCbnSelchangeComboYaxis()
 {
	 // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	 int iCombo= m_CComboYAxis.GetCurSel() + 1;

	 DrawGraph( (ENUM_SUB_GRAPH) iCombo );

 }


 /**
  * @brief     
  * @return    void
  * @author    ��ö�� (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/13 0:32:46
  * @warning   
  */
 void CDeltaGraphView2::OnCbnSelchangeComboLinetype()
 {
	 // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	 int iCombo= m_ComboLineType.GetCurSel();

	 

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
				int nLineTypes[] = { PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, 
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
				int nLineTypes[] = { PELT_MEDIUMDASHDOT, PELT_MEDIUMDASHDOT, 
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
  * @author    ��ö�� (churlhee.jo@lignex1.com)
  * @version   0.0.1
  * @date      2020/03/14 15:59:22
  * @warning   
  */
 void CDeltaGraphView2::OnBnClickedButtonFilterApply()
 {
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ENUM_DataType enDataType;
	ENUM_SUB_GRAPH enSubGraph;

	STR_FILTER_SETUP *pFilterSetup;

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
	
	// DrawGraph( m_stFilterSetup.enSubGraph );


 }



 void CDeltaGraphView2::OnBnClickedButtonFilterDeapply()
 {
	 // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	 m_pDoc->ClearFilterSetup();

	 
 }
