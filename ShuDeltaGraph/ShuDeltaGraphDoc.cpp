
// ShuDeltaGraphDoc.cpp : CShuDeltaGraphDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ShuDeltaGraph.h"
#endif

#include <crtdbg.h>

#include "MainFrm.h"
#include "ChildFrm.h"

#include "ShuDeltaGraphDoc.h"
#include "ShuDeltaGraphView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CShuDeltaGraphDoc

IMPLEMENT_DYNCREATE(CShuDeltaGraphDoc, CDocument)

BEGIN_MESSAGE_MAP(CShuDeltaGraphDoc, CDocument)
END_MESSAGE_MAP()


// CShuDeltaGraphDoc 생성/소멸

CShuDeltaGraphDoc::CShuDeltaGraphDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

	m_pFrame = ( CMainFrame * ) AfxGetMainWnd();

	//_CrtDumpMemoryLeaks();

	//AfxSetAllocStop(8936);

}

CShuDeltaGraphDoc::~CShuDeltaGraphDoc()
{
// 	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
// 	for(pos = pFrm->m_listChild.GetHeadPosition() ; pos != NULL ; )
// 	{
// 		CChildFrame* pChild = (CChildFrame*)pFrm->m_listChild.GetNext(pos);
// 		pChild->GetWindowText( strWindowTitle );
// 		// pChild을 알고있으니 나머지 Doc나 View의 포일터를 알아내서 코딩하세요
// 	}

	theApp.CloseMapData( & m_strPathname );

}

// void CShuDeltaGraphDoc::CloseMapData( CString *pStrWindowTitle )
// {
// 	auto it=m_gMapData.begin();
// 
// 	if( pStrWindowTitle == NULL ) {
// 		while( it != m_gMapData.end() ) {
// 			//it->second->Free();
// 			delete it->second;
// 
// 			++ it;
// 		}
// 		m_gMapData.clear();
// 	}
// 	else {
// 		while( it != m_gMapData.end() ) {
// 			if( pStrWindowTitle->Compare( it->first ) == 0 ) {
// 				//it->second->Free();
// 				delete it->second;
// 
// 				m_gMapData.erase( it++ );
// 			}
// 			else {
// 				++ it;
// 			}
// 		}
// 	}
// 
// }

// CData *CShuDeltaGraphDoc::FindMapData( CString *pStrPathName )
// {
// 	CData *pData;
// 	map<CString, CData *>::iterator it;
// 
// 	it = m_gMapData.find( *pStrPathName );
// 	if( it == m_gMapData.end() ) {
// 		pData = NULL;
// 	}
// 	else {
// 		pData = it->second;
// 	}
// 
// 	return pData;
// }


BOOL CShuDeltaGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CShuDeltaGraphDoc serialization

void CShuDeltaGraphDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		OpenFile( ar.m_strFileName );
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CShuDeltaGraphDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CShuDeltaGraphDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CShuDeltaGraphDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CShuDeltaGraphDoc 진단

#ifdef _DEBUG
void CShuDeltaGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShuDeltaGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CShuDeltaGraphDoc 명령

/**
  * @brief		파일을 오픈한다.
  * @param		CString & strPathname
  * @return 	bool
  * @return     성공시 true, 실패시 false
  * @date		2019/05/31 9:30
*/
bool CShuDeltaGraphDoc::OpenFile( CString &strPathname, STR_FILTER_SETUP *pstFilterSetup, bool bCountOfWindow )
{
	CString strMainTitle;
	map<CString, CData *>::iterator it;
	CChildFrame *pChild;
	CShuDeltaGraphView *pView;

	CData *pData, *pFindMapData;

	pChild = ( CChildFrame * ) m_pFrame->GetActiveFrame();

	m_strPathname = strPathname;

	// 데이터 읽기
	pFindMapData = theApp.FindMapData( & m_strPathname );
	pData = m_theDataFile.ReadDataFile( m_strPathname, 0, pFindMapData, pstFilterSetup );
	if( pFindMapData == NULL ) {
		theApp.AddMapData( & m_strPathname, pData );
	}
	else {
		if( bCountOfWindow == true ) {
			theApp.IncWindowNumber( pFindMapData );
		}
	}
	
// 	it = m_gMapData.find( m_strPathname );
// 	if( it == m_gMapData.end() ) {
// 		ReadDataFile( pstFilterSetup );
// 
// 	}
// 	else {
// 		m_theDataFile.SetData( it->second );
// 
// 	}

	// 타이틀 바 변경
	strMainTitle.Format( _T("%s:%d") , m_strPathname, m_theDataFile.GetWindowNumber() );
	pChild->SetWindowText( strMainTitle );

	pView = (CShuDeltaGraphView *) pChild->GetActiveView();
	pView->SetWindowTitle( m_strPathname );

	return true;
}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/05 10:23
*/
void CShuDeltaGraphDoc::ReadDataFile( STR_FILTER_SETUP *pstFilterSetup )
{
	//CData *pData;

// 	m_theDataFile.ReadDataFile( m_strPathname, 0, pstFilterSetup );
// 
// 	pData = m_theDataFile.GetRawData();
// 	if( pData != NULL ) {
// 		m_gMapData.insert( make_pair( m_strPathname, pData ) );
// 	}

}

/**
 * @brief     
 * @return    bool
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/08 22:35:24
 * @warning   
 */
bool CShuDeltaGraphDoc::IsAlreadyOpen()
{
	bool bRet = true;
	map<CString, CData *>::iterator it;

// 	it = m_gMapData.find( m_strPathname );
// 	if( it == m_gMapData.end() ) {
// 		bRet = false;
// 	}

	return bRet;

}