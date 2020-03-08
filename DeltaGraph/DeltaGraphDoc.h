
// DeltaGraphDoc.h : CDeltaGraphDoc Ŭ������ �������̽�
//


#pragma once

#include <map>

#include "../ShuDeltaGraph/PDW/DataFile.h"

using namespace std;

class CDeltaGraphDoc : public CDocument
{
private:
	CMainFrame *m_pFrame;
	CString m_strPathname;

	CDataFile m_theDataFile;

	static map<CString, CData *> m_gMapData;

private:
	void ReadDataFile( STR_FILTER_SETUP *pstFilterSetup );

public:
	bool OpenFile( CString &strPathname, STR_FILTER_SETUP *pstFilterSetup=NULL );


protected: // serialization������ ��������ϴ�.
	CDeltaGraphDoc();
	DECLARE_DYNCREATE(CDeltaGraphDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

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
	virtual ~CDeltaGraphDoc();
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
