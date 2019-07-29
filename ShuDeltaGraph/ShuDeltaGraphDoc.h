
// ShuDeltaGraphDoc.h : CShuDeltaGraphDoc Ŭ������ �������̽�
//


#pragma once

#include <map>

#include "./PDW/DataFile.h"

using namespace std;

class CShuDeltaGraphDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CShuDeltaGraphDoc();
	DECLARE_DYNCREATE(CShuDeltaGraphDoc)

// Ư���Դϴ�.
public:
private:
	CMainFrame *m_pFrame;
	CString m_strPathname;

	CDataFile m_theDataFile;

	static map<CString, CData *> m_gMapData;
	
// �۾��Դϴ�.
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

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CShuDeltaGraphDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
