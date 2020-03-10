
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
#include "DeltaGraphView.h"

//#include "../ShuDeltaGraph/Log/LogDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeltaGraphView

IMPLEMENT_DYNCREATE(CDeltaGraphView, CFormView)

BEGIN_MESSAGE_MAP(CDeltaGraphView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()


BEGIN_EASYSIZE_MAP(CDeltaGraphView)
	EASYSIZE(IDC_LIST_PDW,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_PREVIOUS,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BUTTON_NEXT,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,0)
END_EASYSIZE_MAP

// CDeltaGraphView 생성/소멸

CDeltaGraphView::CDeltaGraphView()
	: CFormView(CDeltaGraphView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	CLEAR_EASYSIZE;

}

CDeltaGraphView::~CDeltaGraphView()
{
}

void CDeltaGraphView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PDW, m_CListPDW);
}

BOOL CDeltaGraphView::PreCreateWindow(CREATESTRUCT& cs)
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
 * @date      2020/03/10 10:05:12
 * @warning   
 */
 void CDeltaGraphView::OnInitialUpdate()
{

	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	InitListCtrl();

	m_pDoc = ( CDeltaGraphDoc * ) GetDocument();

	INIT_EASYSIZE;

}

void CDeltaGraphView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDeltaGraphView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDeltaGraphView 진단

#ifdef _DEBUG
void CDeltaGraphView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeltaGraphView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDeltaGraphDoc* CDeltaGraphView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeltaGraphDoc)));
	return (CDeltaGraphDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeltaGraphView 메시지 처리기

#define TEXT_WIDTH			(18)

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/09 9:19:02
 * @warning   
 */
void CDeltaGraphView::InitListCtrl( bool bInit )
{
	int j=0;
	//CRect rt;

	if( bInit == true ) {
		//m_CListPDW.GetWindowRect(&rt);
		m_CListPDW.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

		m_CListPDW.InsertColumn( j++, _T("순 서"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("순 서")), -1 );
		m_CListPDW.InsertColumn( j++, _T("신호 형태"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("신호 형태")), -1 ); 
		m_CListPDW.InsertColumn( j++, _T("TOA[us]/TOA"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("TOA[us]/TOA[us]")), -1 ); 
		m_CListPDW.InsertColumn( j++, _T("DTOA[us]"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("DTOA[us]")), -1 ); 
		m_CListPDW.InsertColumn( j++, _T("DV"), LVCFMT_CENTER, TEXT_WIDTH*wcslen(_T("DV")), -1 ); 
		m_CListPDW.InsertColumn( j++, _T("방위[도]"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("방위[도]")), -1 ); 
		m_CListPDW.InsertColumn( j++, _T("주파수[MHz]"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("주파수[MHz]")), -1 ); 
		m_CListPDW.InsertColumn( j++, _T("신호세기[dBm]"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("신호세기[dBm]")), -1 ); 
		m_CListPDW.InsertColumn( j++, _T("펄스폭[ns]"), LVCFMT_RIGHT, TEXT_WIDTH*wcslen(_T("펄스폭[ns]")), -1 ); 

		m_CListPDW.SetGridLines(TRUE);
		//m_CListPDW.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
		//m_ColList.SetCheckboxes(TRUE);

	}
	else {
		int i;

		for( i=0 ; i < m_CListPDW.GetItemCount() ; ++i ) {
			m_CListPDW.SetItemBkColor( i, -1, RGB(255, 255, 255) );
		}
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
 * @date      2020/03/10 10:04:38
 * @warning   
 */
 void CDeltaGraphView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
// 	if( m_hPE ) {
// 		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
// 	}

	UPDATE_EASYSIZE;
}

/**
 * @brief     
 * @param     ENUM_SUB_GRAPH enSubGraph
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/10 9:39:40
 * @warning   
 */
 void CDeltaGraphView::ShowGraph( ENUM_SUB_GRAPH enSubGraph )
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
	_TOA *pfllTOA;
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

		if (enDataType == en_PDW_DATA) {

			pPDWData = (STR_PDW_DATA *) pData;
			if( pPDWData != NULL ) {
				pfTOA = pPDWData->pfTOA;
				pfDTOA = pPDWData->pfDTOA;
				pfAOA = pPDWData->pfAOA;
				pfFreq = pPDWData->pfFreq;
				pfPA = pPDWData->pfPA;
				pfPW = pPDWData->pfPW;
				pcDV = pPDWData->pcDV;
				pfllTOA = pPDWData->pllTOA;
				pcType = pPDWData->pcType;
				pfPh1 = pPDWData->pfPh1;
				pfPh2 = pPDWData->pfPh2;
				pfPh3 = pPDWData->pfPh3;
				pfPh4 = pPDWData->pfPh4;
				for( i=0 ; i < (int) uiDataItems && i < 1000 ; ++i ) {
					j = 1;

					strVal.Format( _T("%7d") , i+1 );
					m_CListPDW.InsertItem( i, strVal );

					strVal.Format( _T("%d") , *pcType );
					m_CListPDW.SetItemText( i, j++, strVal ); 

					//strVal.Format( _T("%12.3f/%12llu") , *pfTOA*1., *pfllTOA );
					strVal.Format( _T("%12.3f") , *pfTOA*1. );
					m_CListPDW.SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%12.3f") , *pfDTOA*1. );
					m_CListPDW.SetItemText( i, j++, strVal ); 

					if( *pcDV == PDW_DV ) {
						m_CListPDW.SetItemText( i, j++, _T("O") ); 
					}
					else {
						m_CListPDW.SetItemText( i, j++, _T("X") ); 
					}

					strVal.Format( _T("%4.1f") , *pfAOA );
					m_CListPDW.SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%8.3f") , *pfFreq );
					m_CListPDW.SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%5.2f") , *pfPA );
					m_CListPDW.SetItemText( i, j++, strVal ); 

					strVal.Format( _T("%5.1f") , *pfPW );
					m_CListPDW.SetItemText( i, j++, strVal ); 

					if( bPhaseData == true ) {
						//strVal.Format( _T("%7.2f/%7.2f/%7.2f/%7.2f") , *pfPh1, *pfPh2, *pfPh3, *pfPh4 );
						//m_pListCtrl->SetItemText( i, j++, strVal ); 
					}

					++ pfTOA;
					++ pfDTOA;
					++ pfAOA;
					++ pfFreq;
					++ pfPA;
					++ pfPW;

					++ pfllTOA;

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
// 			j = 1;
// 			m_pListCtrl->InsertColumn( j++, _T("I 데이터"), LVCFMT_RIGHT, 100, -1 ); 
// 			m_pListCtrl->InsertColumn( j++, _T("Q 데이터"), LVCFMT_RIGHT, 100, -1 ); 
// 			m_pListCtrl->InsertColumn( j++, _T("순시 진폭"), LVCFMT_RIGHT, 100, -1 ); 
// 			m_pListCtrl->InsertColumn( j++, _T("순시 위상차[도]"), LVCFMT_RIGHT, 100, -1 ); 
// 			m_pListCtrl->InsertColumn( j++, _T("FFT"), LVCFMT_RIGHT, 100, -1 ); 
// 
// 			pIQData = (STR_IQ_DATA *) pData;
// 			if( pIQData != NULL ) {
// 				pfI = pIQData->pfI;
// 				pfQ = pIQData->pfQ;
// 				pfPA = pIQData->pfPA;
// 				pfIP = pIQData->pfIP;
// 				pfFFT = pIQData->pfFFT;
// 
// 				for( i=0 ; i < (int) uiDataItems && i < 1000 ; ++i ) {
// 					j = 1;
// 
// 					strVal.Format( _T("%7d") , i+1 );
// 					m_pListCtrl->InsertItem( i, strVal );
// 
// 					strVal.Format( _T("%8.0f") , *pfI );
// 					m_pListCtrl->SetItemText( i, j++, strVal ); 
// 
// 					strVal.Format( _T("%8.0f") , *pfQ );
// 					m_pListCtrl->SetItemText( i, j++, strVal ); 
// 
// 					strVal.Format( _T("%8.1f") , *pfPA );
// 					m_pListCtrl->SetItemText( i, j++, strVal ); 
// 
// 					strVal.Format( _T("%8.1f") , *pfIP );
// 					m_pListCtrl->SetItemText( i, j++, strVal ); 
// 
// 					strVal.Format( _T("%10.1f") , *pfFFT );
// 					m_pListCtrl->SetItemText( i, j++, strVal ); 
// 
// 					++ pfI;
// 					++ pfQ;
// 					++ pfPA;
// 					++ pfPA;
// 					++ pfIP;
// 					++ pfFFT;
// 
// 				}
// 			}
		}
	}
}

// void CDeltaGraphView::OnSizing(UINT fwSide, LPRECT pRect)
// {
// 	__super::OnSizing(fwSide, pRect);
// 
// 	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
// 	EASYSIZE_MINSIZE(280,250,fwSide,pRect);
// }
