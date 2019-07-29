
// ShuDeltaGraphDoc.h : CShuDeltaGraphDoc 클래스의 인터페이스
//


#pragma once

#include <map>

#include "./PDW/DataFile.h"

using namespace std;

class CShuDeltaGraphDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CShuDeltaGraphDoc();
	DECLARE_DYNCREATE(CShuDeltaGraphDoc)

// 특성입니다.
public:
private:
	CMainFrame *m_pFrame;
	CString m_strPathname;

	CDataFile m_theDataFile;

	static map<CString, CData *> m_gMapData;
	
// 작업입니다.
public:
	bool OpenFile( CString &strPathname );
	void ReadDataFile();

	inline UINT GetDataItems() { return m_theDataFile.GetDataItems();}
	inline ENUM_DataType GetDataType() { return m_theDataFile.GetDataType(); }
	inline ENUM_UnitType GetUnitType() { return m_theDataFile.GetUnitType(); }
	inline void *GetData() { return m_theDataFile.GetData();}

	static void CloseMapData( CString *pStrWindowTitle=NULL );

private:
	bool IsAlreadyOpen();

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CShuDeltaGraphDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
