
// ShuDeltaGraphView.cpp : CShuDeltaGraphView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ShuDeltaGraph.h"
#endif

#include "MainFrm.h"

#include "ShuDeltaGraphDoc.h"
#include "ShuDeltaGraphView.h"

#include "Pegrpapi.h"

#include "./Log/LogDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDlgFilterSetup *CShuDeltaGraphView::m_pDlgFilterSetup=NULL;


//////////////////////////////////////////////////////////////////////////
// 팝업 사용자 메뉴

// CShuDeltaGraphView

IMPLEMENT_DYNCREATE(CShuDeltaGraphView, CFormView)

BEGIN_MESSAGE_MAP(CShuDeltaGraphView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_HOTKEY()
	ON_COMMAND(ID_PDW_PULSEINFO, &CShuDeltaGraphView::OnPdwPulseInfo)
	ON_COMMAND(ID_IQ_PULSEINFO, &CShuDeltaGraphView::OnIqPulseInfo)
	ON_COMMAND(ID_PDW_PIE_PULSE, &CShuDeltaGraphView::OnPdwPiePulse)
	ON_COMMAND(ID_PDW_POLAR_AOA_PA, &CShuDeltaGraphView::OnPdwPolarAoaPa)
	ON_COMMAND(ID_PDW_PIE_DIRECTION, &CShuDeltaGraphView::OnPdwPieDirection)
	ON_COMMAND(ID_PDW_POLAR_AOA_FREQ, &CShuDeltaGraphView::OnPdwPolarAoaFreq)
	ON_COMMAND(ID_PDW_2D_TOA_AOA, &CShuDeltaGraphView::OnPdw2DToaAoa)
	ON_COMMAND(ID_PDW_2D_TOA_FREQ, &CShuDeltaGraphView::OnPdw2dToaFreq)
	ON_COMMAND(ID_PDW_2D_TOA_PA, &CShuDeltaGraphView::OnPdw2DToaPa)
	ON_COMMAND(ID_PDW_2D_TOA_PW, &CShuDeltaGraphView::OnPdw2DToaPw)
	ON_COMMAND(ID_PDW_MULTI_AOA_FREQ_PA_PW, &CShuDeltaGraphView::OnPdwMultiAoaFreqPaPw)
	ON_COMMAND(ID_PDW_MULTI_AOAFREQ_PA_PW, &CShuDeltaGraphView::OnPdwMultiAoa_FreqPaPw)
	ON_COMMAND(ID_PDW_MULTI_AOAFREQ_PA, &CShuDeltaGraphView::OnPdwMultiAoa_FreqPa)
	ON_COMMAND(ID_PDW_3D_AOAFREQ, &CShuDeltaGraphView::OnPdw3DAoaFreq)
	ON_COMMAND(ID_PDW_3D_AOAFREQPW, &CShuDeltaGraphView::OnPdw3DAoaFreqPw)
	ON_COMMAND(ID_IQ_2D_INDEX_IQ, &CShuDeltaGraphView::OnIq2DIndexIQ)
	ON_COMMAND(ID_IQ_2D_INDEX_PA, &CShuDeltaGraphView::OnIq2DIndexPA)
	ON_COMMAND(ID_IQ_2D_INDEX_PHASE, &CShuDeltaGraphView::OnIq2DIndexPhase)
	ON_COMMAND(ID_IQ_2D_INDEX_FFT, &CShuDeltaGraphView::OnIq2DIndexFFT)
	ON_WM_KEYDOWN()
	ON_MESSAGE( PEWN_KEYDOWN, OnPEKeyDwon )
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_DLG_COLLIST, &CShuDeltaGraphView::OnDlgCollist)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CShuDeltaGraphView 생성/소멸

CShuDeltaGraphView::CShuDeltaGraphView()
	: CFormView(CShuDeltaGraphView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_hPE = 0;

	m_pListCtrl = NULL;
}

CShuDeltaGraphView::~CShuDeltaGraphView()
{
	if( m_hPE != 0 ) {
		PEdestroy(m_hPE);
	}

	if( m_pListCtrl != NULL ) {
		delete m_pListCtrl;
	}

	if( m_pDlgFilterSetup != NULL ) {
		delete m_pDlgFilterSetup;
		m_pDlgFilterSetup = NULL;
	}

}

void CShuDeltaGraphView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CShuDeltaGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CShuDeltaGraphView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	//CSize scrollSize(0,0);
	//SetScrollSizes( MM_TEXT, scrollSize );

	m_pDoc = ( CShuDeltaGraphDoc * ) GetDocument();

	m_enGraphView = enUnselectedGraphView;
	m_enSubGraph = enUnselectedSubGraph;

	if( m_pDlgFilterSetup == NULL )	{
		m_pDlgFilterSetup = new CDlgFilterSetup;
	}

}

void CShuDeltaGraphView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

/**
  * @brief		팝업 메뉴를 처리한다.
  * @param		CWnd *
  * @param		CPoint point
  * @return 	void
  * @date       2019/06/10 9:06
*/
void CShuDeltaGraphView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{

#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	ENUM_DataType enDataType;

	CMenu popup;
	CMenu *pMenu;

	enDataType = m_pDoc->GetDataType();
	if( enDataType == en_PDW_DATA ) {
		popup.LoadMenu( IDR_ShuDeltaGraphTYPE );
		pMenu = popup.GetSubMenu(3);
	}
	else {
		popup.LoadMenu( IDR_ShuDeltaGraphTYPE );
		pMenu = popup.GetSubMenu(4);
	}
	
	pMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );

}


// CShuDeltaGraphView 진단

#ifdef _DEBUG
void CShuDeltaGraphView::AssertValid() const
{
	CFormView::AssertValid();
}

void CShuDeltaGraphView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CShuDeltaGraphDoc* CShuDeltaGraphView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShuDeltaGraphDoc)));
	return (CShuDeltaGraphDoc*)m_pDocument;
}
#endif //_DEBUG


// CShuDeltaGraphView 메시지 처리기


void CShuDeltaGraphView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
  * @brief		Doc()에 데이터 형태에 따라서 그래프를 도시한다.
  * @param		ENUM_GRAPH_VIEW enGraphView
  * @return 	void
  * @date       2019/05/31 16:35
*/
void CShuDeltaGraphView::ShowGraph( ENUM_GRAPH_VIEW enGraphView, ENUM_SUB_GRAPH enSubGraph )
{
	if(  m_pListCtrl != NULL ) {
		delete m_pListCtrl;
		m_pListCtrl = NULL;
		UpdateWindow();
	}

	if( enGraphView == enGraphPulseInfo ) {
		m_enGraphView = enGraphView;
		m_enSubGraph = enSubGraph;

		if( m_hPE ) {
			PEdestroy( m_hPE );
		}

		ShowPulseInfo( enSubGraph );
	}
	else if( enGraphView == enFilter_Set ) {
		ShowFilterSetup( enSubGraph );
	}
	else {
		m_enGraphView = enGraphView;
		m_enSubGraph = enSubGraph;

		if( m_hPE ) {
			PEdestroy( m_hPE );
		}

		CSize scrollSize(0,0);
		SetScrollSizes( MM_TEXT, scrollSize );

		switch( enGraphView ) {
			case enGRAPH_PIE :
				ShowPieGraph( enSubGraph );
				break;

			case enGRAPH_POLAR:
				ShowPolarGraph(enSubGraph);
				break;

			case enGRAPH_2D :
				Show2DGraph(enSubGraph);
				break;

			case enGRAPH_MULTI :
				ShowMultiGraph(enSubGraph);
				break;

			case enGRAPH_3D :
				Show3DGraph(enSubGraph);
				break;

			default :
				break;

		}

		PEreinitialize(m_hPE);
		PEresetimage(m_hPE, 0, 0);
	}
}

void CShuDeltaGraphView::ShowFilterSetup( ENUM_SUB_GRAPH enSubGraph )
{
	CData *pData;
	SGRAPHPROPERTIES stSGRAPHPROPERTIES;

	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp* )AfxGetApp();

	pData = NULL; //m_pDoc->FindMapData( & m_strPathName );

	// TOA
	PEvget(m_hPE, PEP_fZOOMMINX, & pData->m_stFilterSetup.dToaMin ); 
	PEvget(m_hPE, PEP_fZOOMMAXX, & pData->m_stFilterSetup.dToaMax ); 

	PEgetsgraph( m_hPE, & stSGRAPHPROPERTIES );

	// 2차원 그래프에 어떤 종류의 그래프인지를 확인한다.
	switch( m_enSubGraph ) {
		case enSubMenu_1 :
			PEvget(m_hPE, PEP_fZOOMMINY, & pData->m_stFilterSetup.dAoaMin ); 
			PEvget(m_hPE, PEP_fZOOMMAXY, & pData->m_stFilterSetup.dAoaMax ); 
			break;
		case enSubMenu_2 :
			PEvget(m_hPE, PEP_fZOOMMINY, & pData->m_stFilterSetup.dFrqMin ); 
			PEvget(m_hPE, PEP_fZOOMMAXY, & pData->m_stFilterSetup.dFrqMax ); 
			break;
		case enSubMenu_3 :
			//PEvget(m_hPE, PEP_fMANUALMINY, &dMinY ); 
			//PEvget(m_hPE, PEP_fMANUALMAXY, &dMaxY ); 
			break;

		case enSubMenu_4 :
			PEvget(m_hPE, PEP_fZOOMMINY, & pData->m_stFilterSetup.dPAMin ); 
			PEvget(m_hPE, PEP_fZOOMMAXY, & pData->m_stFilterSetup.dPAMax ); 
			break;
	}

	STR_FILTER_SETUP stFilterSetup;
	pApp->LoadProfile( & stFilterSetup );
	m_pDlgFilterSetup->SetValue( & stFilterSetup );
	if( IDOK == m_pDlgFilterSetup->DoModal() ) {

		pApp->SaveProfile( & m_pDlgFilterSetup->m_stFilterSetup );
		// Redraw Graph
		pApp->FilteredOpenFile( m_strPathName );
	}
	else {

	}
	

}

/**
  * @brief		
  * @param		ENUM_SUB_GRAPH enSubGraph
  * @return 	void
  * @return		성공시 true, 실패시 false
  * @date       2019/06/04 16:11
*/
void CShuDeltaGraphView::ShowPulseInfo( ENUM_SUB_GRAPH enSubGraph )
{
	ENUM_DataType enDataType;

	RECT rect;

	int i, j;
	UINT uiDataItems;

	char *pcDV, *pcType;
	float *pfTOA, *pfDTOA;
	float *pfAOA, *pfFreq, *pfPA, *pfPW;
	float *pfPh1, *pfPh2, *pfPh3, *pfPh4;
	float *pfI, *pfQ, *pfIP, *pfFFT;
	_TOA *pllTOA;
	CString strVal;

	bool bPhaseData;

	void *pData;
	STR_PDW_DATA *pPDWData=NULL;
	STR_IQ_DATA *pIQData=NULL;

	GetClientRect( &rect );

	//CSize scrollSize( 500, 500);
	//SetScrollSizes( MM_TEXT, scrollSize );

	uiDataItems = m_pDoc->GetDataItems();

	if( uiDataItems != 0 ) {
		bPhaseData = m_pDoc->IsPhaseData();
		enDataType = m_pDoc->GetDataType();
		pData = m_pDoc->GetData();

		m_pListCtrl = new CListCtrl;

		m_pListCtrl->Create( WS_VISIBLE | LVS_REPORT | WS_BORDER, CRect( 0, 0, rect.right, rect.bottom ), this, 501 );
		m_pListCtrl->SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

		m_pListCtrl->InsertColumn( 0, _T("순서"), LVCFMT_RIGHT, 80, -1 );

		if (enDataType == en_PDW_DATA) {
			j = 1;
			m_pListCtrl->InsertColumn( j++, _T("신호 형태"), LVCFMT_RIGHT, 12*wcslen(_T("형태")), -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("TOA[us]/TOA"), LVCFMT_RIGHT, 12*wcslen(_T("TOA[us]/TOA[us]")), -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("DTOA[us]"), LVCFMT_RIGHT, 12*wcslen(_T("DTOA[us]")), -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("DV"), LVCFMT_CENTER, 12*wcslen(_T("DV")), -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("방위[도]"), LVCFMT_RIGHT, 12*wcslen(_T("방위[도]")), -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("주파수[MHz]"), LVCFMT_RIGHT, 12*wcslen(_T("주파수[MHz]")), -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("신호세기[dBm]"), LVCFMT_RIGHT, 12*wcslen(_T("신호세기[dBm]")), -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("펄스폭[ns]"), LVCFMT_RIGHT, 12*wcslen(_T("펄스폭[ns]")), -1 ); 

			if( bPhaseData == true ) {
				m_pListCtrl->InsertColumn( j++, _T("위상차"), LVCFMT_RIGHT, 12*wcslen(_T("위상차          ")), -1 ); 
			}

			pPDWData = (STR_PDW_DATA *) pData;
			if( pPDWData != NULL ) {

				pfTOA = pPDWData->pfTOA;
				pfDTOA = pPDWData->pfDTOA;
				pfAOA = pPDWData->pfAOA;
				pfFreq = pPDWData->pfFreq;
				pfPA = pPDWData->pfPA;
				pfPW = pPDWData->pfPW;
				pcDV = pPDWData->pcDV;
				pllTOA = pPDWData->pllTOA;
				pcType = pPDWData->pcType;
				pfPh1 = pPDWData->pfPh1;
				pfPh2 = pPDWData->pfPh2;
				pfPh3 = pPDWData->pfPh3;
				pfPh4 = pPDWData->pfPh4;
				for( i=0 ; i < (int) uiDataItems && i < 1000 ; ++i ) {
					j = 1;

					strVal.Format( _T("%7d") , i+1 );
					m_pListCtrl->InsertItem( i, strVal );

					strVal.Format( _T("%d") , *pcType );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%12.3f/%12llu") , *pfTOA*1., *pllTOA );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%12.3f") , *pfDTOA*1. );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					if( *pcDV == PDW_DV ) {
						m_pListCtrl->SetItemText( i, j++, _T("O") ); 
					}
					else {
						m_pListCtrl->SetItemText( i, j++, _T("X") ); 

					}

					strVal.Format( _T("%4.1f") , *pfAOA );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%8.3f") , *pfFreq );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%5.2f") , *pfPA );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%5.1f") , *pfPW );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					if( bPhaseData == true ) {
						strVal.Format( _T("%7.2f/%7.2f/%7.2f/%7.2f") , *pfPh1, *pfPh2, *pfPh3, *pfPh4 );
						m_pListCtrl->SetItemText( i, j++, strVal ); 
					}

					++ pfTOA;
					++ pfDTOA;
					++ pfAOA;
					++ pfFreq;
					++ pfPA;
					++ pfPW;

					++ pllTOA;

					++ pcDV;
					++ pcType;

					++ pfPh1;
					++ pfPh2;
					++ pfPh3;
					++ pfPh4;

				}
			}
		}
		else {
			j = 1;
			m_pListCtrl->InsertColumn( j++, _T("I 데이터"), LVCFMT_RIGHT, 100, -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("Q 데이터"), LVCFMT_RIGHT, 100, -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("순시 진폭"), LVCFMT_RIGHT, 100, -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("순시 위상차[도]"), LVCFMT_RIGHT, 100, -1 ); 
			m_pListCtrl->InsertColumn( j++, _T("FFT"), LVCFMT_RIGHT, 100, -1 ); 

			pIQData = (STR_IQ_DATA *) pData;
			if( pIQData != NULL ) {
				pfI = pIQData->pfI;
				pfQ = pIQData->pfQ;
				pfPA = pIQData->pfPA;
				pfIP = pIQData->pfIP;
				pfFFT = pIQData->pfFFT;

				for( i=0 ; i < (int) uiDataItems && i < 1000 ; ++i ) {
					j = 1;

					strVal.Format( _T("%7d") , i+1 );
					m_pListCtrl->InsertItem( i, strVal );

					strVal.Format( _T("%8.0f") , *pfI );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%8.0f") , *pfQ );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%8.1f") , *pfPA );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%8.1f") , *pfIP );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%10.1f") , *pfFFT );
					m_pListCtrl->SetItemText( i, j++, strVal ); 

					++ pfI;
					++ pfQ;
					++ pfPA;
					++ pfPA;
					++ pfIP;
					++ pfFFT;

				}
			}
		}
	}
}

/**
  * @brief		
  * @param		void
  * @return 	void
  * @date       2019/05/31 16:36
*/
void CShuDeltaGraphView::ShowPieGraph(ENUM_SUB_GRAPH enSubGraph)
{
	UINT i;
	RECT rect;
	TCHAR szBuffer[100];

	char *pcVal;
	UINT uiDataItems;
	STR_PDW_DATA *pPDWData;

	float fVal;
	UINT uiNormal=0, uiCW=0, uiPMOP=0, uiFMOP=0;
	UINT uiDirectionValid=0, uiDirectionInvalid=0;

	GetClientRect( &rect );

	TCHAR strSubsetLabel[2][4][10] = { { _T("Normal"), _T("CW"), _T("PMOP"), _T("FMOP") },
									   { _T("DV"), _T("DI"), _T(""), _T("") } };
	TCHAR strMainTitleLabel[2][10] = { _T("펄스"), _T("방탐") } ;
	UINT nPoints[2] = { 4, 2 } ;

	uiDataItems = m_pDoc->GetDataItems();
	if( uiDataItems != 0 ) {
		pPDWData = (STR_PDW_DATA *) m_pDoc->GetData();

		m_hPE = PEcreate(PECONTROL_PIE, WS_VISIBLE, &rect, m_hWnd, 1001);

		wsprintf( szBuffer, _T("%s 형태 분포[%d]") , strMainTitleLabel[enSubGraph-1], uiDataItems );

		PEszset( m_hPE, PEP_szMAINTITLE, szBuffer );
		PEszset( m_hPE, PEP_szSUBTITLE, _T("") ); // no subtitle

		PEnset( m_hPE, PEP_nSUBSETS, 1 );
		PEnset( m_hPE, PEP_nPOINTS, nPoints[enSubGraph-1]);

		// 그래프 데이터 계산하기
		if( enSubGraph == enSubMenu_1 ) {
			pcVal = pPDWData->pcType;
			for( i=0 ; i < uiDataItems ; ++i ) {
				switch( *pcVal ) {
					case PDW_NORMAL :
						++ uiNormal;
						break;

					case PDW_CW :
						++ uiCW;
						break;

					case PDW_CHIRPUP :
						++ uiFMOP;
						break;

					case PDW_CHIRPDN :
						++ uiFMOP;
						break;

					case PDW_PMOP :
						++ uiPMOP;
						break;
				}

				++ pcVal;
			}


			fVal = (float) FDIV( uiNormal, uiDataItems ) * (float) 100.;
			printf( "\n Normal[%.2f]" , fVal );
			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 0, &fVal);

			fVal = (float) ( FDIV( uiCW, uiDataItems ) * 100. );
			printf( "\n CW[%.2f]" , fVal );
			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 1, &fVal);

			fVal = FDIV( uiPMOP, uiDataItems ) * (float) 100.;
			printf( "\n PMOP[%.2f]" , fVal );
			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 2, &fVal);

			fVal = (float) FDIV( uiFMOP, uiDataItems ) * (float) 100.;
			printf( "\n FMOP[%.2f]" , fVal );
			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 3, &fVal);

			// Set Subset labels //
			for( i=0 ; i < nPoints[enSubGraph-1]; ++i ) {
				PEvsetcell(m_hPE, PEP_szaPOINTLABELS, i, (void *) strSubsetLabel[enSubGraph-1][i] );
			}
		}
		else {
			pcVal = pPDWData->pcDV;
			for( i=0 ; i < uiDataItems ; ++i ) {
				if( *pcVal == PDW_DV ) {
					++ uiDirectionValid;
				}
				else {
					++ uiDirectionInvalid;
				}

				++ pcVal;
			}

			fVal = (float) FDIV( uiDirectionValid, uiDataItems ) * (float) 100.;
			printf( "\n DV[%.2f]" , fVal );
			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 0, &fVal);

			fVal = (float) FDIV( uiDirectionInvalid, uiDataItems ) * (float) 100.;
			printf( "\n DI[%.2f]" , fVal );
			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 1, &fVal);

			// Set Subset labels //
			for( i=0 ; i < nPoints[enSubGraph-1]; ++i ) {
				PEvsetcell(m_hPE, PEP_szaPOINTLABELS, i, (void *) strSubsetLabel[enSubGraph-1][i] );
			}
		}

		//////////////////////////////////////////////////////////////////////////

		// Set Slice Colors //
		DWORD c[12];
		c[0] = RGB(0, 198, 0);
		c[1] = RGB(198, 0, 0);
		c[2] = RGB(198, 198, 0);
		c[3] = RGB(0, 0, 198);
		c[4] = RGB(198, 0, 198);
		c[5] = RGB(0, 198, 198);
		c[6] = RGB(192, 192, 192);
		c[7] = RGB(148, 0, 0);
		c[8] = RGB(0, 148, 0);
		c[9] = RGB(148, 148, 0);
		c[10] = RGB(0, 148, 148);
		c[11] = RGB(122, 122, 122);
		PEvset(m_hPE, PEP_dwaSUBSETCOLORS, c, 12);

		PEnset(m_hPE, PEP_nDATAPRECISION, 1);
		PEnset(m_hPE, PEP_nGROUPINGPERCENT, 4);
		PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_3D);
		PEnset(m_hPE, PEP_bDISABLE3DSHADOW, FALSE);
		PEnset(m_hPE, PEP_nAUTOEXPLODE, PEAE_ALLSUBSETS);
		PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
		PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
		PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
		PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);
		PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);

		PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
		PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_MEDIUM_NO_BORDER);

		PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
		PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
		PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
		PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
		PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);

		PEnset(m_hPE, PEP_bSHOWPIELEGEND, TRUE);
		PEnset(m_hPE, PEP_nSHOWPIELABELS, PESPL_PERCENT);

		PEnset(m_hPE, PEP_nLEGENDLOCATION, PELL_LEFT);
		PEnset(m_hPE, PEP_bDISABLE3DSHADOW, TRUE);

		// Set various other properties //
		PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
		PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_MEDIUM_INSET);

		// Add Some Padding on Right and Bottom //
		PEnset(m_hPE, PEP_nIMAGEADJUSTRIGHT, 400);
		PEnset(m_hPE, PEP_nIMAGEADJUSTBOTTOM, 400);

		// 팝업 메뉴 및 사용자 메뉴 설정
		SetupPopupMenu();	
	}

}

/**
* @brief
* @param	void
* @return 	void
* @date		2019/05/31 16:36
*/
void CShuDeltaGraphView::ShowPolarGraph(ENUM_SUB_GRAPH enSubGraph)
{
	UINT i;
	RECT rect;
	TCHAR szBuffer[100];

	char *pcDV;
	float *pfX, *pfY, f1, f2;
	UINT uiDataItems;
	STR_PDW_DATA *pPDWData;

	TCHAR strMainTitleLabel[2][10] = { _T("신호세기"), _T("주파수") } ;

	GetClientRect(&rect);

	uiDataItems = m_pDoc->GetDataItems();

	if( uiDataItems != 0 ) {
		pPDWData = (STR_PDW_DATA *) m_pDoc->GetData();

		wsprintf(szBuffer, _T("방위대 %s[%d]"), strMainTitleLabel[enSubGraph-1], uiDataItems);

		m_hPE = PEcreate(PECONTROL_PGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

		PEszset(m_hPE, PEP_szMAINTITLE, szBuffer);
		PEszset(m_hPE, PEP_szSUBTITLE, _T("") ); // no subtitle

		PEnset(m_hPE, PEP_nSUBSETS, 2 );
		PEnset(m_hPE, PEP_nPOINTS, uiDataItems);

		double dnull = -9999.0F;
		PEvset(m_hPE, PEP_fNULLDATAVALUEX, &dnull, 1);
		PEvset(m_hPE, PEP_fNULLDATAVALUE, &dnull, 1);

		//PEnset(m_hPE, PEP_bINVERTEDXAXIS, TRUE );

		// 그래프 데이터 계산하기
		if( enSubGraph == enSubMenu_1 ) {
			pfX = pPDWData->pfAOA;
			pfY = pPDWData->pfPA;

			// Manually Configure Radius Axis Range //
			PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
			double dMin = -80.0F;
			PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
			double dMax = 5.0F;
			PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);

			PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[s]"));
			PEszset(m_hPE, PEP_szYAXISLABEL, _T("방위[도]"));

			PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,.000|") );
			PEszset( m_hPE, PEP_szAXISFORMATY, _T("|.0|") );

		}
		else {
			pfX = pPDWData->pfAOA;
			pfY = pPDWData->pfFreq;

			// Manually Configure Radius Axis Range //
			PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
			double dMin = 500.0F;
			PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
			double dMax = 18000.0F;
			PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);

			PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[s]"));
			PEszset(m_hPE, PEP_szYAXISLABEL, _T("방위[도]"));

			PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,.000|") );
			PEszset( m_hPE, PEP_szAXISFORMATY, _T("|.0|") );
		}
		
		for (i = 0; i < uiDataItems; ++i) {
			f1 = (float) *pfX;
			f2 = (float) *pfY;

			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, & f1);
			PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, & f2);

			PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i, & f1);
			PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & f2);

			++ pfX;
			++ pfY;

		}

		pcDV = pPDWData->pcDV;
		for (i = 0; i < uiDataItems; ++i) {
			float f1 = -9999.0F;

			if (*pcDV == PDW_DV) {
				PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & f1);

			}
			else {
				PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, & f1);

			}

			++pcDV;
		}

// 		if( i2 == 0 ) {
// 			PEnset(m_hPE, PEP_nSUBSETS, 1 );
// 
// 			//PEvsetcell(m_hPE, PEP_szaMULTIBOTTOMTITLES, 0, _T("방탐 유효만 존재합니다." ) );
// 		}

		PEnset(m_hPE, PEP_nZERODEGREEOFFSET, 90);

		PEnset(m_hPE, PEP_bMANUALXAXISTICKNLINE, TRUE);

		double dTL = 7.5;
		PEvset(m_hPE, PEP_fMANUALXAXISTICK, &dTL, 1);

		double dAL = 10;
		PEvset(m_hPE, PEP_fMANUALXAXISLINE, &dAL, 1);

		dTL = 7.5;
		PEvset(m_hPE, PEP_fMANUALYAXISTICK, &dTL, 1);

		dAL = 2;
		PEvset(m_hPE, PEP_fMANUALYAXISLINE, &dAL, 1);

		PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);

		// subset labels
		PEvsetcell(m_hPE, PEP_szaSUBSETLABELS, 0, _T("DV") );
		PEvsetcell(m_hPE, PEP_szaSUBSETLABELS, 1, _T("DI") );

		// Manually Configure Degree Axis Density //
		PEnset(m_hPE, PEP_bMANUALXAXISTICKNLINE, TRUE);
		double dML = 10.0F;  // major grid lines every 10 units
		PEvset(m_hPE, PEP_fMANUALXAXISLINE, &dML, 1);
		double dMT = 2.0F;   // minor tick marks every 2 units
		PEvset(m_hPE, PEP_fMANUALXAXISTICK, &dMT, 1);
	

		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);
		PEnset(m_hPE, PEP_nALLOWZOOMING, 1);
		float ftmp = .9F;
		PEvset(m_hPE, PEP_fFONTSIZELEGENDCNTL, &ftmp, 1);
		PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
		PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
		PEnset(m_hPE, PEP_bFOCALRECT, FALSE);

		// Set line types
		int nTypes[] = { PELT_THINSOLID, PELT_THINSOLID };
		PEvset(m_hPE, PEP_naSUBSETLINETYPES, nTypes, 2);

		// Set point types
		int nPTypes[] = { PEPT_DOTSOLID, PEPT_DOT };
		PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPTypes, 2);

		// subset colors
		DWORD dwArray[2] = { RGB(0,198,0), RGB(198, 0, 0) };
		PEvset(m_hPE, PEP_dwaSUBSETCOLORS, dwArray, 2);

		// Set Various other features //
		PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
		PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
		PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_MEDIUM_NO_BORDER);
		PEnset(m_hPE, PEP_bSIMPLEPOINTLEGEND, TRUE);
		PEnset(m_hPE, PEP_bSIMPLELINELEGEND, TRUE);
		PEnset(m_hPE, PEP_nLEGENDSTYLE, PELS_1_LINE);
		PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_FRAMED_RECT);  // PEZS_RO2_NOT);
		PEnset(m_hPE, PEP_nPOINTSIZE, PEPS_SMALL);

		PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
		PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
		PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
		PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
		PEnset(m_hPE, PEP_bLINESHADOWS, FALSE);
		PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);
		PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
		PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_SHADOWS);
		float f = 1.2F;
		PEvset(m_hPE, PEP_fFONTSIZEGNCNTL, &f, 1);

		PEnset(m_hPE, PEP_nSHOWXAXIS, PESA_ALL );

		// 그래프 공통 기능
		SetupCommonGraph();
	}

}

/**
  * @brief		2차원 그래프를 도시한다.
  * @param		ENUM_SUB_GRAPH enSubGraph
  * @return 	void
  * @date       2019/06/07 10:18
*/
void CShuDeltaGraphView::Show2DGraph( ENUM_SUB_GRAPH enSubGraph )
{
	ENUM_DataType enDataType;

	RECT rect;
	TCHAR szBuffer[100];

	UINT i, i1=0, i2=0;
	char *pcDV;
	float *pfX, *pfY, *pfY2;
	UINT uiDataItems;

	void *pData;
	void *pHeader;
	STR_PDW_DATA *pPDWData=NULL;
	STR_IQ_DATA *pIQData=NULL;
	STR_IQ_HEADER *pIQHeader=NULL;

	DWORD dwColor[2];

	double dMin, dMax;

	TCHAR strMainTitleLabel[2][5][30] = { { _T("방위"), _T("주파수"), _T("DTOA"), _T("신호세기"), _T("펄스폭") },
																				{ _T("I/Q 데이터"), _T("순시진폭"), _T("위상차"), _T("FFT") } };

	TCHAR strYAxisLabel[6][30] = { _T(""), _T("방위[도]"), _T("주파수[MHz]"), _T("DTOA[us]"), _T("신호세기[dBm]"), _T("펄스폭[ns]") };

	GetClientRect( &rect );

	m_hPE = PEcreate( PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

	PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  
	PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);


	ShowAnnotation();

	uiDataItems = m_pDoc->GetDataItems();
	enDataType = m_pDoc->GetDataType();
	pData = m_pDoc->GetData();
	pHeader = m_pDoc->GetHeader();

	if (enDataType == en_PDW_DATA) {
		wsprintf(szBuffer, _T("시간대 %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiDataItems);

		pPDWData = (STR_PDW_DATA *)pData;
	}
	else {
		wsprintf(szBuffer, _T("인덱스대 %s[%d]"), strMainTitleLabel[enDataType - 1][enSubGraph - 1], uiDataItems);

		pIQData = (STR_IQ_DATA *)pData;
		pIQHeader = (STR_IQ_HEADER *) pHeader;
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

		// 그래프 데이터 계산하기
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
					dMin = -360.0;
					PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					dMax = 360.0;
					PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
					//PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
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

			case enSubMenu_2 :
				if (enDataType == en_PDW_DATA) {
					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfFreq;

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
				}
				else {
					pfY = pIQData->pfPA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("순시진폭[dB]"));

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					dMin = 1.0;
					PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					dMax = (double)uiDataItems;
					PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					dMin = -100;
					PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					dMax = 10.0;
					PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}

				dwColor[0] = RGB( 0, 198, 198 );
				dwColor[1] = RGB(198, 0, 0 );
				PEvset( m_hPE, PEP_dwaSUBSETCOLORS, dwColor, 2 );
				break;

			case enSubMenu_3 :
				if (enDataType == en_PDW_DATA) {
					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfDTOA;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]") );
					PEszset(m_hPE, PEP_szYAXISLABEL, strYAxisLabel[enSubGraph] );

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.000|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.0|"));

					PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 2, strYAxisLabel[enSubGraph] );

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					dMin = pPDWData->pfTOA[0];
					PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					dMax = pPDWData->pfTOA[uiDataItems - 1];
					PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);
				}
				else {
					pfY = pIQData->pfIP;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("인덱스"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("위상차[도]"));

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
					dMin = 1.0;
					PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
					dMax = (double)uiDataItems;
					PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

					PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
					dMin = -180;
					PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
					dMax = 180.0;
					PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
				}

				dwColor[0] = RGB( 198,98,0 );
				dwColor[1] = RGB(198, 0, 0 );
				PEvset( m_hPE, PEP_dwaSUBSETCOLORS, dwColor, 2 );
				break;

			case enSubMenu_4 :
				if (enDataType == en_PDW_DATA) {
					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfPA;

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
				}
				else {
					pfY = pIQData->pfFFT;

					PEszset(m_hPE, PEP_szXAXISLABEL, _T("순시 주파수[MHz]"));
					PEszset(m_hPE, PEP_szYAXISLABEL, _T("FFT"));

					PEszset(m_hPE, PEP_szAXISFORMATX, _T("|,.0|"));
					PEszset(m_hPE, PEP_szAXISFORMATY, _T("|.,|"));

					if( pIQHeader != NULL ) {
						PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
						dMin = pIQHeader->fCenterFreq - 500.0;
						PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
						dMax = pIQHeader->fCenterFreq + 500.0;
						PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);
					}
				}

				dwColor[1] = RGB( 198,198,0 );
				dwColor[0] = RGB( 198, 0, 0 );
				PEvset( m_hPE, PEP_dwaSUBSETCOLORS, dwColor, 2 );
				break;

			case enSubMenu_5 :
				if (enDataType == en_PDW_DATA) {
					pcDV = pPDWData->pcDV;
					pfX = pPDWData->pfTOA;
					pfY = pPDWData->pfPW;

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
				}
				else {

				}

				dwColor[0] = RGB( 0,198,0 );
				dwColor[1] = RGB(198, 0, 0 );
				PEvset( m_hPE, PEP_dwaSUBSETCOLORS, dwColor, 2 );
				break;
		}

		if (enDataType == en_PDW_DATA) {
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
					PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, & f1);

				}
				else {
					PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, & f1);

				}

				++pcDV;
			}

		}
		else {
			switch( enSubGraph ) {
				case enSubMenu_1 :
					for (i = 0; i < uiDataItems; ++i) {
						float fVal;

						fVal = (float) ( i + 1 );

						PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, & fVal);
						PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, pfY );

						PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i, & fVal);
						PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, pfY2 );

						++pfY;
						++pfY2;
					}
					break;

				case enSubMenu_2 :
				case enSubMenu_3 :
					for (i = 0; i < uiDataItems; ++i) {
						float fVal;

						fVal = (float) ( i + 1 );

						PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, & fVal);
					}
					PEvset(m_hPE, PEP_faYDATA, pfY, uiDataItems );
					break;

				case enSubMenu_4 :
					if( pIQHeader != NULL ) {
						float fVal;

						for (i = 0; i < uiDataItems; ++i) {
							fVal = (float) ( pIQHeader->fCenterFreq-500.0 + ( i*1000.0 / MAX_COL_IQ_DATA ) );

							PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, & fVal);
						}
					}
					else {
						float fVal;

						for (i = 0; i < uiDataItems; ++i) {
							fVal = (float) ( i );

							PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, & fVal);
						}
					}
					PEvset(m_hPE, PEP_faYDATA, pfY, uiDataItems );
					break;

				default :
					break;
			}

		}
	}

	// subset labels
	if (enDataType == en_PDW_DATA) {
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, _T("DV") );
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, _T("DI") );
	}
	else if( enDataType == en_IQ_DATA && enSubGraph == enSubMenu_1 ) {
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 0, _T("I 데이터") );
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, 1, _T("Q 데이터") );
	}

	PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);  // v9

	// Enable Bar Glass Effect //
	PEnset(m_hPE, PEP_bBARGLASSEFFECT, TRUE);

	//int nPointTypes = PEPT_DOTSOLID;
	int nPointTypes[] = { PEPT_DOTSOLID, PEPT_DOT };
	PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPointTypes, 2);
	//PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, & nPointTypes, 1 );

	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_NONE );
	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORZANDVERT);
	PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_RO2_NOT);

	// Version 4.0 Features //
	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE, PEP_bSIMPLEPOINTLEGEND, TRUE);
	PEnset(m_hPE, PEP_bSIMPLELINELEGEND, TRUE);
	PEnset(m_hPE, PEP_nLEGENDSTYLE, PELS_1_LINE);
	PEnset(m_hPE, PEP_nMULTIAXISSTYLE, PEMAS_SEPARATE_AXES);

	// Set Various Other Properties //
	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_MEDIUM_NO_BORDER);

	PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
	PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
	PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);
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

	PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
	PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);
	PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);

	PEnset(m_hPE, PEP_nDATAPRECISION, 1);
	PEnset(m_hPE, PEP_nGRAPHPLUSTABLE, PEGPT_BOTH);
	PEnset(m_hPE, PEP_bMARKDATAPOINTS, FALSE);

	// 그래프 공통 기능
	SetupCommonGraph( enDataType );

}

/**
  * @brief		멀티 그래프를 전시한다.
  * @param		ENUM_SUB_GRAPH enSubGraph
  * @return 	void
  * @date       2019/06/05 10:23
*/
void CShuDeltaGraphView::ShowMultiGraph( ENUM_SUB_GRAPH enSubGraph )
{
	RECT rect;
	TCHAR szBuffer[100];

	UINT i, i1=0, i2=0;
	char *pcDV;
	float *pfX;
	float *pfAOA, *pfFreq, *pfPA, *pfPW;
	UINT uiDataItems;
	STR_PDW_DATA *pPDWData;

	TCHAR strMainTitleLabel[3][30] = { _T("방위/주파수/신호세기/펄스폭"), _T("방위,주파수/신호세기/펄스폭"), _T("신호세기") } ;
	TCHAR strYLabel[4][30] = { _T("방위[도]"), _T("주파수[MHz]"), _T("신호세기[dB]"), _T("펄스폭[ns]") } ;

	GetClientRect( &rect );

	m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

	uiDataItems = m_pDoc->GetDataItems();
	pPDWData = (STR_PDW_DATA *)m_pDoc->GetData();

	wsprintf(szBuffer, _T("시간대 %s[%d]"), strMainTitleLabel[enSubGraph-1], uiDataItems );

	PEszset(m_hPE, PEP_szMAINTITLE, szBuffer);
	PEszset(m_hPE, PEP_szSUBTITLE, _T("") ); // no subtitle

	double dnull = -9999.0F;
	PEvset(m_hPE, PEP_fNULLDATAVALUEX, &dnull, 1);
	PEvset(m_hPE, PEP_fNULLDATAVALUE, &dnull, 1);

	if( uiDataItems > 0 ) {
		if( enSubGraph == enSubMenu_1 || enSubGraph == enSubMenu_2 ) {
			PEnset(m_hPE, PEP_nSUBSETS, 4 );
		}
		else {
			PEnset(m_hPE, PEP_nSUBSETS, 3 );
		}
		PEnset(m_hPE, PEP_nPOINTS, uiDataItems);

		double dMin, dMax;

		// 그래프 데이터 계산하기
		pcDV = pPDWData->pcDV;
		pfX = pPDWData->pfTOA;
		pfAOA = pPDWData->pfAOA;
		pfFreq= pPDWData->pfFreq;
		pfPA= pPDWData->pfPA;
		pfPW = pPDWData->pfPW;

		PEszset(m_hPE, PEP_szXAXISLABEL, _T("시간[us]"));
		PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,.000|") );

 		PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
 		dMin = pPDWData->pfTOA[0];
 		PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
 		dMax = pPDWData->pfTOA[uiDataItems-1];
 		PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);
 
		for (i = 0; i < uiDataItems; ++i) {
			PEvsetcellEx(m_hPE, PEP_faXDATA, 0, i, pfX );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 0, i, pfAOA );

			PEvsetcellEx(m_hPE, PEP_faXDATA, 1, i, pfX );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 1, i, pfFreq );

			PEvsetcellEx(m_hPE, PEP_faXDATA, 2, i, pfX );
			PEvsetcellEx(m_hPE, PEP_faYDATA, 2, i, pfPA );

			if( enSubGraph != enSubMenu_3 ) {
				PEvsetcellEx(m_hPE, PEP_faXDATA, 3, i, pfX );
				PEvsetcellEx(m_hPE, PEP_faYDATA, 3, i, pfPW );
			}

			++pcDV;
			
			++pfX;
			++pfAOA;
			++pfFreq;
			++pfPA;
			++pfPW;
		}
	}

	PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);

	PEnset(m_hPE, PEP_bALLOWRIBBON, TRUE);
	PEnset(m_hPE, PEP_nALLOWZOOMING, PEAZ_HORIZONTAL);

	//
	PEnset(m_hPE, PEP_nMOUSEWHEELFUNCTION, PEMWF_HORZPLUSVERT_ZOOM);
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGX, TRUE);  // note that pan gestures require MouseDragging to be enabled 
	PEnset(m_hPE, PEP_bMOUSEDRAGGINGY, TRUE);  

	// Enable MouseWheel Zoom Smoothness
	PEnset(m_hPE, PEP_nMOUSEWHEELZOOMSMOOTHNESS, 5);
	PEnset(m_hPE, PEP_nPINCHZOOMSMOOTHNESS, 2);


	PEnset(m_hPE, PEP_nZOOMSTYLE, PEZS_FRAMED_RECT );

	// subset colors //
	DWORD dwArray[4] = { RGB(198,0,0), RGB( 0, 198, 198 ), RGB( 198,198,0 ), RGB( 0,198,0 ) };
	PEvsetEx(m_hPE, PEP_dwaSUBSETCOLORS, 0, 4, dwArray, 0);

	// subset line types //
	int nLineTypes[] = { PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID, PELT_MEDIUMSOLID };
	PEvset(m_hPE, PEP_naSUBSETLINETYPES, nLineTypes, 4);

	// subset point types //
	int nPointTypes[] = { PEPT_DOTSOLID, PEPT_UPTRIANGLESOLID, PEPT_SQUARESOLID, PEPT_DOWNTRIANGLESOLID };
	PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPointTypes, 4);

	// Allow stacked type graphs //
	PEnset(m_hPE, PEP_bNOSTACKEDDATA, FALSE);

	if( enSubGraph == enSubMenu_1 ) {
		int nArray[4] = {1,1,1,1};
		PEvset (m_hPE, PEP_naMULTIAXESSUBSETS, nArray, 4);
	}
	else if( enSubGraph == enSubMenu_2 ) {
		int nArray[4] = {1,1,1,1};
		PEvset (m_hPE, PEP_naMULTIAXESSUBSETS, nArray, 4);

		float fMAP[3] = {.60F, .20F, .20F };
		PEvset(m_hPE, PEP_faMULTIAXESPROPORTIONS, fMAP, 3 );

		// Create Overlapped Axes //
		int nOMA[3] = { 2, 1, 1};
		PEvset (m_hPE, PEP_naOVERLAPMULTIAXES, nOMA, 3 );

	}
	else {
		int nArray[3] = {1,1,1};
		PEvset (m_hPE, PEP_naMULTIAXESSUBSETS, nArray, 3);

		float fMAP[2] = {.70F, .30F };
		PEvset(m_hPE, PEP_faMULTIAXESPROPORTIONS, fMAP, 2 );

		// Create Overlapped Axes //
		int nOMA[2] = { 2, 1 };
		PEvset (m_hPE, PEP_naOVERLAPMULTIAXES, nOMA, 2 );
	}

	i = 0;
	// Set first axis parameters //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);
	PEnset(m_hPE, PEP_dwYAXISCOLOR, PERGB( 255,198,0,0));

	PEnset(m_hPE, PEP_nWORKINGAXIS, 1);
	PEnset(m_hPE, PEP_dwYAXISCOLOR, PERGB( 255,0,198,198));

	PEnset(m_hPE, PEP_nWORKINGAXIS, 2);
	PEnset(m_hPE, PEP_dwYAXISCOLOR, PERGB( 255,198,198,0));

	PEnset(m_hPE, PEP_nWORKINGAXIS, 3);
	PEnset(m_hPE, PEP_dwYAXISCOLOR, PERGB( 255,0,198,0));


	if( enSubGraph == enSubMenu_2 ) {
		// PEnset(m_hPE, PEP_nRYAXISCOMPARISONSUBSETS, 1);
	}

	PEnset(m_hPE, PEP_nWORKINGAXIS, i);
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, i, strYLabel[i] );
	PEszset(m_hPE, PEP_szYAXISLABEL, strYLabel[i] );
	PEnset(m_hPE, PEP_nRYAXISCOMPARISONSUBSETS, 0 );
	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|.0|") );
	PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	double dMin = 0;
	PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
	double dMax = 360.0;
	PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
	PEnset(m_hPE, PEP_nSHOWTICKMARKY, PESTM_TICKS_HIDE); 
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);

	// Set second axis parameters //
	++ i;
	PEnset(m_hPE, PEP_nWORKINGAXIS, i);
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, i, strYLabel[i] );
	PEszset(m_hPE, PEP_szYAXISLABEL, strYLabel[i]);
	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,.000|") );
	dMin = 500;
	PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
	dMax = 18000.0;
	PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
	PEnset(m_hPE, PEP_nSHOWTICKMARKY, PESTM_TICKS_HIDE); 
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);

	// Set third axis parameters //
	++ i;
	PEnset(m_hPE, PEP_nWORKINGAXIS, i);
	PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, i, strYLabel[i] );
	PEszset(m_hPE, PEP_szYAXISLABEL, strYLabel[i]);
	PEszset( m_hPE, PEP_szAXISFORMATY, _T("|.0|") );
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);
	dMin = -80;
	PEvset(m_hPE, PEP_fMANUALMINY, &dMin, 1);
	dMax = 10;
	PEvset(m_hPE, PEP_fMANUALMAXY, &dMax, 1);
	PEnset(m_hPE, PEP_nSHOWTICKMARKY, PESTM_TICKS_HIDE); 
	PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);

	// Set fourth axis parameters //
	if( enSubGraph != enSubMenu_3 ) {
		++ i;
		PEnset(m_hPE, PEP_nWORKINGAXIS, i);
		PEvsetcell( m_hPE, PEP_szaSUBSETLABELS, i, strYLabel[i] );
		PEszset(m_hPE, PEP_szYAXISLABEL, strYLabel[i]);
		PEszset( m_hPE, PEP_szAXISFORMATY, _T("|.000|") );
		PEnset(m_hPE, PEP_nSHOWTICKMARKY, PESTM_TICKS_HIDE); 
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);
		PEnset(m_hPE, PEP_bAUTOSCALEDATA, TRUE );
	}

	// Reset WorkingAxis when done //
	PEnset(m_hPE, PEP_nWORKINGAXIS, 0);

	//PEnset(m_hPE, PEP_nMULTIAXISSTYLE, PEMAS_SEPARATE_AXES);
	PEnset(m_hPE, PEP_nMULTIAXISSTYLE, PEMAS_GROUP_ALL_AXES);

	//PEnset(m_hPE, PEP_nFONTSIZE, PEFS_SMALL);
	//PEnset(m_hPE, PEP_bYAXISONRIGHT, TRUE);

	// Add Axis Separator //
	PEnset(m_hPE, PEP_nMULTIAXESSEPARATORS, PEMAS_THICKPLUSTICK);

	// Axis Sizing //
	PEnset(m_hPE, PEP_nMULTIAXESSIZING, TRUE);

	// Get rid of 3d stuff //
	PEnset(m_hPE, PEP_nDATASHADOWS, PEDS_NONE);

	PEnset(m_hPE, PEP_nMULTIAXISSTYLEMENU, PEMC_SHOW);

	float fSize = 1.1F;
	PEvset(m_hPE, PEP_fFONTSIZEMSCNTL, &fSize, 1);
	PEvset(m_hPE, PEP_fFONTSIZEMBCNTL, &fSize, 1);

	// Other various properties //
	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_NO_BORDER);
	PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);
	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	

	SetupCommonGraph();

}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/05 13:19
*/
void CShuDeltaGraphView::SetupCommonGraph( ENUM_DataType enDataType )
{
	// 그래프
	SetupGraph();

	// 도시 형태
	SetupPlotting();

	// 폰트 크기
	SetupFont();

	// 그리드
	SetupGrid();

	// 팝업 메뉴 및 사용자 메뉴 설정
	SetupPopupMenu( enDataType );
}

/**
  * @brief		
  * @return 	void
  * @return		성공시 true, 실패시 false
  * @date       2019/06/05 14:58
*/
void CShuDeltaGraphView::SetupGraph()
{
	PEnset(m_hPE, PEP_nBORDERTYPES, PETAB_THIN_LINE);

	if( m_enGraphView == enGRAPH_2D || m_enGraphView == enGRAPH_MULTI ) {
		// Set up cursor //
		PEnset(m_hPE, PEP_nCURSORMODE, PECM_DATACROSS);

		PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);

		// This will allow you to move cursor by clicking data point //
		PEnset(m_hPE, PEP_bMOUSECURSORCONTROL, TRUE);
		PEnset(m_hPE, PEP_bALLOWDATAHOTSPOTS, TRUE);

		// Cursor prompting in top left corner //
		PEnset(m_hPE, PEP_bCURSORPROMPTTRACKING, TRUE);
		PEnset(m_hPE, PEP_nCURSORPROMPTSTYLE, PECPS_XYVALUES);
		PEnset(m_hPE, PEP_nCURSORPROMPTLOCATION, PECPL_TOP_RIGHT);

		PEnset(m_hPE, PEP_bALLOWAXISPAGE, TRUE );
	}
	else if( m_enGraphView == enGRAPH_POLAR ) {
		PEnset(m_hPE, PEP_bINVERTEDXAXIS, TRUE );
	}

	// Help see data points //
	PEnset(m_hPE, PEP_bMARKDATAPOINTS, TRUE);
}

void CShuDeltaGraphView::SetupPlotting()
{

	if( m_pDoc->GetDataType() == en_IQ_DATA ) {
		if( m_enGraphView == enGRAPH_2D && m_enSubGraph == enSubMenu_4 ) {
			PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);
		}
		else {
			PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);
		}
	}
	else {
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, PEGPM_POINT);
	}
}

/**
  * @brief		폰트 관련 특성을 설정한다.
  * @return 	void
  * @return		성공시 true, 실패시 false
  * @date       2019/06/05 14:58
*/
void CShuDeltaGraphView::SetupFont()
{
	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_SMALL);
}

/**
  * @brief		그리드 관련 특성을 설정한다.
  * @return 	void
  * @date       2019/06/05 13:19
*/
void CShuDeltaGraphView::SetupGrid()
{
	
	PEnset(m_hPE, PEP_nGRIDSTYLE, PEGS_DOT);
	PEnset(m_hPE, PEP_nGRIDLINECONTROL, PEGLC_BOTH);
	PEnset(m_hPE, PEP_bGRIDINFRONT, TRUE);

}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/04 11:00
*/
void CShuDeltaGraphView::SetupPopupMenu( ENUM_DataType enDataType )
{
	switch( m_enGraphView ) {
		case enGRAPH_PIE :
			PEnset(m_hPE, PEP_bALLOWPOPUP, TRUE );
			PEnset(m_hPE, PEP_naCUSTOMMENU, PECM_SHOW );

			PEnset(m_hPE, PEP_bALLOWCUSTOMIZATION, FALSE );

			//PEvset (m_hPE, PEP_szaCUSTOMMENUTEXT , gszCustomMenu, 6 );
			break;

		case enGRAPH_POLAR :
			PEnset(m_hPE, PEP_bALLOWPOPUP, TRUE );
			PEnset(m_hPE, PEP_naCUSTOMMENU, PECM_SHOW );

			PEnset(m_hPE, PEP_bALLOWCUSTOMIZATION, FALSE );

			//PEvset (m_hPE, PEP_szaCUSTOMMENUTEXT , gszCustomMenu, 0 );
			break;

		case enGRAPH_2D :
			PEnset(m_hPE, PEP_bALLOWPOPUP, TRUE );
			PEnset(m_hPE, PEP_naCUSTOMMENU, PECM_SHOW );

			PEnset(m_hPE, PEP_bALLOWCUSTOMIZATION, TRUE );

			PEnset(m_hPE, PEP_nALLOWUSERINTERFACE, PEAUI_ALL );

			// 테이블
			PEnset(m_hPE, PEP_bSHOWTABLEANNOTATION, TRUE);
			PEnset(m_hPE, PEP_nTALOCATION, PETAL_INSIDE_TOP_RIGHT);
			PEnset(m_hPE, PEP_dwTABACKCOLOR, RGB(255,255,255));
			PEnset(m_hPE, PEP_nTAHEADERROWS, 1);
			PEnset(m_hPE, PEP_bTAHEADERCOLUMN, TRUE);
			PEnset(m_hPE, PEP_nTATEXTSIZE, 85);

			//PEvset (m_hPE, PEP_szaCUSTOMMENUTEXT , gszCustomMenu, 6);
			break;
			
	}

	int i=0;
	TCHAR szTmp[128];

	if( enDataType == en_PDW_DATA ) {
		wcscpy_s( szTmp, L"펄스 정보"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
	}
	else {
		wcscpy_s( szTmp, L"펄스내 정보"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
	}

	wcscpy_s( szTmp, L"파이|펄스 신호|방탐 신호"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
	wcscpy_s( szTmp, L"극좌표|방위대 신호세기|방위대 주파수"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);

	if( enDataType == en_PDW_DATA ) {
		wcscpy_s( szTmp, L"2차원 그래프|시간대 방위|시간대 주파수|시단대 DTOA|시간대 신호세기|시간대 펄스폭"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
		wcscpy_s( szTmp, L"멀티 그래프|시간대 방위/주파수/신호세기/펄스폭|시간대 방위,주파수/신호세기/펄스폭|시간대 방위,주파수/신호세기"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
		wcscpy_s( szTmp, L"3차원 그래프|방위,주파수|방위,주파수,펄스폭"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);

		if( m_enGraphView == enGRAPH_2D || m_enGraphView == enGRAPH_MULTI ) {
			wcscpy_s( szTmp, L"|"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
			wcscpy_s( szTmp, L"필터 설정|필터 설정|필터 적용"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
		}
	}
	else {
		wcscpy_s( szTmp, L"2차원 그래프|인덱스대 I/Q데이터|인덱스대 순시진폭|인덱스대 위상차|인덱스대 FFT"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
		wcscpy_s( szTmp, L"멀티 그래프|시간대 방위/주파수/신호세기/펄스폭|시간대 방위,주파수/신호세기/펄스폭|시간대 방위,주파수/신호세기"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
		wcscpy_s( szTmp, L"3차원 그래프|SFFT|***"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);
	}
	
	wcscpy_s( szTmp, L"|"); PEvsetcell(m_hPE, PEP_szaCUSTOMMENUTEXT, i++, szTmp);

	if( enDataType == en_IQ_DATA ) {
		int nCMS = PECM_GRAYED;
		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_PIE, enSubMenu_1, & nCMS);
		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_PIE, enSubMenu_2, & nCMS);

		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_POLAR, enSubMenu_1, &nCMS);
		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_POLAR, enSubMenu_2, &nCMS);

		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_MULTI, enSubMenu_1, &nCMS);
		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_MULTI, enSubMenu_2, &nCMS);
		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_MULTI, enSubMenu_3, &nCMS);

		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_3D, enSubMenu_1, &nCMS);
		PEvsetcellEx( m_hPE, PEP_naCUSTOMMENU, enGRAPH_3D, enSubMenu_2, &nCMS);


	}

}

#define FREQ_TICK				(100)					// [MHz]
#define AOA_TICK				10					// [도]
#define FREQ_RANGE			18000
#define AOA_RANGE				360
/**
  * @brief		3차원 그래프에 PDW 데이터를 전시한다.
  * @param		ENUM_SUB_GRAPH enSubGraphid
  * @return 	void
  * @date       2019/06/05 15:14
*/
void CShuDeltaGraphView::Show3DGraph(ENUM_SUB_GRAPH enSubGraph)
{
	UINT uiDataItems, i, j, o, nQ;

	UINT uiAoa, uiFreq;
	float *pfAOA, *pfFreq, *pfPW;
	float *pMyXData, *pMyYData, *pMyZData;
	STR_PDW_DATA *pPDWData;

	UINT **pXYData;

	UINT nTargetRows, nTargetCols; 

	TCHAR szBuffer[100];
	TCHAR strMainTitleLabel[2][30] = { _T("방위,주파수"), _T("방위,주파수,펄스폭") } ;

	RECT rect;

	ENUM_DataType enDataType;

	uiDataItems = m_pDoc->GetDataItems();
	pPDWData = (STR_PDW_DATA *)m_pDoc->GetData();

	enDataType = m_pDoc->GetDataType();

	GetClientRect( &rect );
	m_hPE = PEcreate(PECONTROL_3D, WS_VISIBLE, &rect, m_hWnd, 1001);

	// Non-Surface chart, set PolyMode //
	if( enSubGraph == enSubMenu_1 ) {
		PEnset(m_hPE, PEP_nPOLYMODE, PEPM_3DBAR);
	}
	else {
		PEnset(m_hPE, PEP_nPOLYMODE, PEPM_SCATTER);
	}

	SetFreqRange();

	wsprintf(szBuffer, _T("%s[%d]"), strMainTitleLabel[enSubGraph-1], uiDataItems );

	PEszset(m_hPE, PEP_szMAINTITLE, szBuffer);
	PEszset(m_hPE, PEP_szSUBTITLE, _T("") ); // no subtitle

	// 그래프 데이터 계산하기
	if( enSubGraph == enSubMenu_1 ) {
		nTargetRows = 360 / AOA_TICK;
		nTargetCols = (int) ( ( _spFreqMax - _spFreqMin ) / FREQ_TICK );
		PEnset(m_hPE, PEP_nSUBSETS, nTargetRows );
		PEnset(m_hPE, PEP_nPOINTS, nTargetCols );

		pXYData = ( UINT **) malloc( sizeof(UINT) * nTargetRows );
		for( i=0 ; i < nTargetRows ; ++i ) {
			pXYData[i] = ( UINT * ) malloc( sizeof(UINT) * nTargetCols );
			memset( pXYData[i], 0, sizeof(UINT)*nTargetCols );
		}
			
		pfAOA = pPDWData->pfAOA;
		pfFreq = pPDWData->pfFreq;
		for (i = 0; i < uiDataItems; ++i) {
			uiAoa = _DIV( *pfAOA, AOA_TICK );
			uiFreq = _DIV( ( *pfFreq - _spFreqMin ), FREQ_TICK );

			if( ( uiAoa >= 0 && uiAoa < nTargetRows ) && ( uiFreq >= 0 && uiFreq < nTargetCols ) ) {
				++ pXYData[uiAoa][uiFreq];
			}
			else {
				++ pXYData[nTargetRows-1][nTargetCols-1];
				printf( "Erorr !!!" );
			}

			++pfAOA;
			++pfFreq;
		}

		nQ = nTargetRows * nTargetCols;
		pMyXData = new float[nQ];
		pMyYData = new float[nQ];
		pMyZData = new float[nQ];

		memset( pMyYData, 0, sizeof(pMyYData) );

		pfAOA = pPDWData->pfAOA;
		pfFreq = pPDWData->pfFreq;
		for (i = o = 0; i < nTargetRows; ++i ) {
			for (j = 0; j < nTargetCols; ++j ) {
				pMyZData[o] = (float) i;
				pMyXData[o] = (float) j;

				pMyYData[o] = (float) pXYData[i][j];

				++ o;
			}
		}

		PEvset(m_hPE, PEP_faXDATA, pMyXData, nQ);
		PEvset(m_hPE, PEP_faYDATA, pMyYData, nQ);
		PEvset(m_hPE, PEP_faZDATA, pMyZData, nQ);

		for( i=0 ; i < nTargetRows ; ++i ) {
			free( pXYData[i] );
		}
		free( pXYData );

		delete pMyXData;
		delete pMyYData;
		delete pMyZData;

		//
		for( i=0 ; i < nTargetRows ; ++i ) {
			if( i % 5 == 0 ) {
				wsprintf( szBuffer, _T("%d") , i*AOA_TICK );
			}
			else {
				szBuffer[0] = NULL;
			}
			PEvsetcell(m_hPE, PEP_szaSUBSETLABELS, i, szBuffer );
		}
		for( i=0 ; i < nTargetCols ; ++i ) {
			float fVal, fVal2;

			fVal = ( i*FREQ_TICK+_spFreqMin ) / (float) 1000.;
			fVal2 = fVal * (float) 100.;
			fVal2 = fmod( fVal2, 50 );

			if( fVal2 == 0.0 || i == 0 ) {
				_stprintf_s( szBuffer, _T("%.1f") , fVal );
			}
			else {
				szBuffer[0] = NULL;
			}
			PEvsetcell(m_hPE, PEP_szaPOINTLABELS, i, szBuffer );
		}

		// 축
		PEszset(m_hPE, PEP_szZAXISLABEL, _T("방위[도]"));
		PEszset( m_hPE, PEP_szAXISFORMATZ, _T("|,.000|") );

		PEszset(m_hPE, PEP_szXAXISLABEL, _T("주파수[GHz]"));
		PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,.0|") );

		PEszset(m_hPE, PEP_szYAXISLABEL, _T("크기"));
		PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,|") );

		// Set Plotting methods //
		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, 2);   // ' Shading
	}
	else {
		PEnset(m_hPE, PEP_nSUBSETS, 1 );
		PEnset(m_hPE, PEP_nPOINTS, uiDataItems );

		pfAOA = pPDWData->pfAOA;
		pfFreq = pPDWData->pfFreq;
		pfPW = pPDWData->pfPW;
		for (i = 0; i < uiDataItems; ++i) {
			float fFreq;

			fFreq = (float) ( *pfFreq / 1000. );
			PEvsetcellEx( m_hPE, PEP_faXDATA, 0, i, pfAOA );
			PEvsetcellEx( m_hPE, PEP_faZDATA, 0, i, & fFreq );
			PEvsetcellEx( m_hPE, PEP_faYDATA, 0, i, pfPW );

			++pfAOA;
			++pfFreq;
			++pfPW;
		}

		// 축
		PEszset(m_hPE, PEP_szXAXISLABEL, _T("방위[도]"));
		PEszset( m_hPE, PEP_szAXISFORMATX, _T("|,.0|") );

		PEszset(m_hPE, PEP_szZAXISLABEL, _T("주파수[GHz]"));
		PEszset( m_hPE, PEP_szAXISFORMATZ, _T("|,.0|") );

		PEszset(m_hPE, PEP_szYAXISLABEL, _T("펄스폭[ns]"));
		PEszset( m_hPE, PEP_szAXISFORMATY, _T("|,|") );

		PEnset(m_hPE, PEP_nPLOTTINGMETHOD, 0);   // ' Points

		PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
		double dMin = 0;
		PEvset(m_hPE, PEP_fMANUALMINX, &dMin, 1);
		double dMax = 360.0;
		PEvset(m_hPE, PEP_fMANUALMAXX, &dMax, 1);

		PEnset(m_hPE, PEP_nMANUALSCALECONTROLZ, PEMSC_MINMAX);
		dMin = 0.5;
		PEvset(m_hPE, PEP_fMANUALMINZ, &dMin, 1);
		dMax = 18.0;
		PEvset(m_hPE, PEP_fMANUALMAXZ, &dMax, 1);

		int nPTypes[] = { PEPT_DOTSOLID, PEPT_PLUS };
		PEvset(m_hPE, PEP_naSUBSETPOINTTYPES, nPTypes, 2);

	}

	// Set Various Other Properties //
	PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
	PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_DARK_SHADOW);

	// Full rotation detail //
	PEnset(m_hPE, PEP_nROTATIONDETAIL, PERD_FULLDETAIL);

	// Set small font size //
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_SMALL);

	// Prepare images in memory //
	PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
	PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
	PEnset(m_hPE, PEP_bALLOWSTYLEPAGE, FALSE);
	PEnset(m_hPE, PEP_nROTATIONINCREMENT, PERI_INCBY2);

	// Add Some Padding on Right and Bottom //
	PEnset(m_hPE, PEP_nIMAGEADJUSTLEFT, 400);
	PEnset(m_hPE, PEP_nIMAGEADJUSTRIGHT, 400);
	PEnset(m_hPE, PEP_nIMAGEADJUSTBOTTOM, 400);

	PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
	PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);
	PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
	PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);
	PEnset(m_hPE, PEP_dwGRAPHFORECOLOR, RGB(192,192,192));

	SetupCommonGraph();

}

void CShuDeltaGraphView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_hPE ) {
		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
	}
}


BOOL CShuDeltaGraphView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CFormView::OnEraseBkgnd(pDC);
	//return FALSE;
}



void CShuDeltaGraphView::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnHotKey(nHotKeyId, nKey1, nKey2);
}


/**
  * @brief		
  * @param		WPARAM wParam
  * @param		LPARAM lParam
  * @return 	BOOL
  * @date       2019/06/04 13:33
*/
BOOL CShuDeltaGraphView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (lParam == (LPARAM) m_hPE ) {
		CFormView::OnCommand(wParam, lParam);

		if( HIWORD(wParam) != PEWN_MOUSEMOVE ) {
			//printf( "\n 값 : %d" , HIWORD(wParam)-WM_USER );
		}

		if( m_enGraphView == enGRAPH_2D || true ) {
			if( HIWORD(wParam) == PEWN_CUSTOMMENU ) {
				ENUM_GRAPH_VIEW enM;
				ENUM_SUB_GRAPH enSM;

				// Custom Menu was clicked //
				enM = ( ENUM_GRAPH_VIEW ) PEnget(m_hPE, PEP_nLASTMENUINDEX );
				enSM = ( ENUM_SUB_GRAPH ) PEnget(m_hPE, PEP_nLASTSUBMENUINDEX );

				if( m_enGraphView != enM || m_enSubGraph != enSM || enM == enFilter_Set ) {
					ShowGraph( enM, enSM );
				}
				else {
					// 새로이 생성 : 운용 개념...
				}

			}

		}
// 		if( m_enGraphView == enCreate2DPoint && HIWORD(wParam) == PEWN_RBUTTONCLK ) {
// 			CMenu popup;
// 			CMenu *pMenu;
// 			CPoint pos;
// 
// 			popup.LoadMenu( IDR_MENU1 );
// 			pMenu = popup.GetSubMenu(0);
// 			GetCursorPos( & pos );
// 			pMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
// 
// 			popup.DestroyMenu();
// 
// 		}
// 		else

	}
	else {
		CFormView::OnCommand(wParam, lParam);
	}
	return TRUE;
}





void CShuDeltaGraphView::OnPdwPulseInfo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGraphPulseInfo, enSubMenu_1 );


}


void CShuDeltaGraphView::OnIqPulseInfo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGraphPulseInfo, enSubMenu_1 );

}


void CShuDeltaGraphView::OnPdwPiePulse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_PIE, enSubMenu_1 );
}

void CShuDeltaGraphView::OnPdwPieDirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_PIE, enSubMenu_2 );
}



void CShuDeltaGraphView::OnPdwPolarAoaPa()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_POLAR, enSubMenu_1 );
}




void CShuDeltaGraphView::OnPdwPolarAoaFreq()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_POLAR, enSubMenu_2 );
}


void CShuDeltaGraphView::OnPdw2DToaAoa()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_1 );
}


void CShuDeltaGraphView::OnPdw2dToaFreq()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_2 );
}


void CShuDeltaGraphView::OnPdw2DToaPa()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_3 );
}


void CShuDeltaGraphView::OnPdw2DToaPw()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_4 );
}


void CShuDeltaGraphView::OnPdwMultiAoaFreqPaPw()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_MULTI, enSubMenu_1 );
}


void CShuDeltaGraphView::OnPdwMultiAoa_FreqPaPw()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_MULTI, enSubMenu_2 );
}


void CShuDeltaGraphView::OnPdwMultiAoa_FreqPa()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_MULTI, enSubMenu_3 );
}


void CShuDeltaGraphView::OnPdw3DAoaFreq()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_3D, enSubMenu_1 );
}


void CShuDeltaGraphView::OnPdw3DAoaFreqPw()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_3D, enSubMenu_2 );
}


void CShuDeltaGraphView::OnIq2DIndexIQ()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_1 );
}


/**
  * @brief		인덱스 대 FFT 순시진폭을 도시한다.
  * @return 	void
  * @date       2019/06/10 10:04
*/
void CShuDeltaGraphView::OnIq2DIndexPA()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_2 );
}


/**
  * @brief		인덱스 대 FFT 위상차를 도시한다.
  * @return 	void
  * @date       2019/06/10 9:08
*/
void CShuDeltaGraphView::OnIq2DIndexPhase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_3 );
}


/**
  * @brief		인덱스 대 FFT 그래프를 도시한다.
  * @return 	void
  * @date       2019/06/10 9:08
*/
void CShuDeltaGraphView::OnIq2DIndexFFT()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowGraph( enGRAPH_2D, enSubMenu_4 );
}


void CShuDeltaGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}


LRESULT	CShuDeltaGraphView::OnPEKeyDwon(WPARAM wp, LPARAM lp)
{	
	//int x;

	//++ x;
	return (LRESULT)0;
}

void CShuDeltaGraphView::ShowAnnotation()
{

	if( m_pDoc->GetDataType() == en_PDW_DATA ) {
	// Construct a simple table annotation //
		PEnset(m_hPE, PEP_nWORKINGTABLE, 0);
		PEnset(m_hPE, PEP_nTAROWS, 5 );
		PEnset(m_hPE, PEP_nTACOLUMNS, 3 );

		// Pass the table text //
		PEvsetcellEx( m_hPE, PEP_szaTATEXT, 1, 0, _T("평균") );
		PEvsetcellEx( m_hPE, PEP_szaTATEXT, 2, 0, _T("최소") );
		PEvsetcellEx( m_hPE, PEP_szaTATEXT, 3, 0, _T("최대") );
		PEvsetcellEx( m_hPE, PEP_szaTATEXT, 4, 0, _T("편차") );

		PEvsetcellEx( m_hPE, PEP_szaTATEXT, 0, 1, _T("시간  ") );
	}
	else {

	}

}

void CShuDeltaGraphView::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnClose();
}


void CShuDeltaGraphView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strWindowTitle, strPathName, strPathName2;
	GetParent()->GetWindowText( strWindowTitle );

	int iIndex=strWindowTitle.ReverseFind( ':' );
	strPathName = strWindowTitle.Left( iIndex );
	
	bool bRet=true;
	//int iCompare;
	//POSITION pos;
	//CShuDeltaGraphDoc *pDoc;
	CShuDeltaGraphView *pView;
	CWinApp *pApp = AfxGetApp();
	POSITION posTemplate = pApp->GetFirstDocTemplatePosition();

	while ( posTemplate ) {
		CDocTemplate* pTemplate = pApp->GetNextDocTemplate(posTemplate);
		POSITION posDoc = pTemplate->GetFirstDocPosition();
		while ( posDoc ) {
			CDocument* pDoc = pTemplate->GetNextDoc(posDoc);
			POSITION posView = pDoc->GetFirstViewPosition();
			while ( posView ) {
				pView = ( CShuDeltaGraphView * ) pDoc->GetNextView(posView);

				if( this != pView ) {
					pView->GetParent()->GetWindowText( strWindowTitle );

					iIndex = strWindowTitle.ReverseFind( ':' );
					strPathName2 = strWindowTitle.Left( iIndex );
					if( strPathName2.Compare(strPathName) == 0 ) {
						bRet = false;
						return;
					}
				}
				//pView->Invalidate();
			}
		}
	}

	if( bRet == true ) {
		//CShuDeltaGraphDoc::CloseMapData( & strPathName );
	}

}

void CShuDeltaGraphView::SetFreqRange()
{
	if( m_pDoc->GetUnitType() == en_SONATA || m_pDoc->GetUnitType() == en_KFX ) {
		_spFreqMin = 500;
		_spFreqMax = 18000;
	}
	else {
		_spFreqMin = 500;
		_spFreqMax = 8000;
	}

}

void CShuDeltaGraphView::OnDlgCollist()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp* )AfxGetApp();
	BOOL chk = pApp->m_pDlgColList->IsWindowVisible();

	if(chk) {
		pApp->m_pDlgColList->ShowWindow(SW_HIDE);
	}
	else {
		pApp->m_pDlgColList->ShowWindow(SW_SHOW);
	}

}


void CShuDeltaGraphView::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnSysCommand(nID, lParam);
}
