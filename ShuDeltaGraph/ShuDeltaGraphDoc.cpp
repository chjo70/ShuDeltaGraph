
// ShuDeltaGraphDoc.cpp : CShuDeltaGraphDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ShuDeltaGraph.h"
#endif

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


// CShuDeltaGraphDoc ����/�Ҹ�

map<CString, CData *> CShuDeltaGraphDoc::m_gMapData;

CShuDeltaGraphDoc::CShuDeltaGraphDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

	m_pFrame = ( CMainFrame * ) AfxGetMainWnd();

}

CShuDeltaGraphDoc::~CShuDeltaGraphDoc()
{
// 	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
// 	for(pos = pFrm->m_listChild.GetHeadPosition() ; pos != NULL ; )
// 	{
// 		CChildFrame* pChild = (CChildFrame*)pFrm->m_listChild.GetNext(pos);
// 		pChild->GetWindowText( strWindowTitle );
// 		// pChild�� �˰������� ������ Doc�� View�� �����͸� �˾Ƴ��� �ڵ��ϼ���
// 	}

}

void CShuDeltaGraphDoc::CloseMapData( CString *pStrWindowTitle )
{
	auto it=m_gMapData.begin();

	if( pStrWindowTitle == NULL ) {
		while( it != m_gMapData.end() ) {
			it->second->Free();
			delete it->second;

			++ it;
		}
		m_gMapData.clear();
	}
	else {
		while( it != m_gMapData.end() ) {
			if( pStrWindowTitle->Compare( it->first ) == 0 ) {
				it->second->Free();
				delete it->second;

				m_gMapData.erase( it++ );
			}
			else {
				++ it;
			}
		}
	}

}

BOOL CShuDeltaGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CShuDeltaGraphDoc serialization

void CShuDeltaGraphDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		OpenFile( ar.m_strFileName );
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CShuDeltaGraphDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CShuDeltaGraphDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CShuDeltaGraphDoc ����

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


// CShuDeltaGraphDoc ���

/**
  * @brief		������ �����Ѵ�.
  * @param		CString & strPathname
  * @return 	bool
  * @return     ������ true, ���н� false
  * @date		2019/05/31 9:30
*/
bool CShuDeltaGraphDoc::OpenFile( CString &strPathname )
{
	CString strMainTitle;
	map<CString, CData *>::iterator it;
	CChildFrame *pChild;
	//CData *pData;

	pChild = ( CChildFrame * ) m_pFrame->GetActiveFrame();

	m_strPathname = strPathname;

	// ������ �б�
	it = m_gMapData.find( m_strPathname );
	if( it == m_gMapData.end() ) {
		ReadDataFile();

	}
	else {
		m_theDataFile.SetData( it->second );

	}

	// Ÿ��Ʋ �� ����
	strMainTitle.Format( _T("%s:%d") , m_strPathname, m_theDataFile.GetWindowNumber() );
	pChild->SetWindowText( strMainTitle );

	return true;
}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/05 10:23
*/
void CShuDeltaGraphDoc::ReadDataFile()
{
	CData *pData;

	m_theDataFile.ReadDataFile( m_strPathname );

	pData = m_theDataFile.GetRawData();
	if( pData != NULL ) {
		m_gMapData.insert( make_pair( m_strPathname, pData ) );
	}

}

bool CShuDeltaGraphDoc::IsAlreadyOpen()
{
	bool bRet = true;
	map<CString, CData *>::iterator it;

	it = m_gMapData.find( m_strPathname );
	if( it == m_gMapData.end() ) {
		bRet = false;
	}

	return bRet;

}