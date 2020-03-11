
// DeltaGraph.h : DeltaGraph ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "../ShuDeltaGraph/PDW/DataFile.h"

//
//////////////////////////////////////////////////////////////////////////

typedef enum {
	enOpenPDW = 0,
	enOpenXLS,
	enSavePDW,
	enSaveXLS

} ENUM_OPENTYPE;

//
//////////////////////////////////////////////////////////////////////////


// CDeltaGraphApp:
// �� Ŭ������ ������ ���ؼ��� DeltaGraph.cpp�� �����Ͻʽÿ�.
//

class CDeltaGraphApp : public CWinAppEx
{
private:
	UINT m_uiWindowNumber;
	CString m_strArgument;

private:
	bool OpenFile( CString &strPathname, TCHAR *pTitle, ENUM_OPENTYPE enOpenType=enOpenPDW );
	void RawDataOpen( CString *pStrPathname );
	ENUM_DataType GetDataType(CString &strPathName);
	bool IsExistFile( CString &strPathname );

public:
	CDeltaGraphApp();

	


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnFileOpen();
};

extern CDeltaGraphApp theApp;
