
// ShuDeltaGraph.h : ShuDeltaGraph ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include <map>

#include "./PDW/DataFile.h"

// CShuDeltaGraphApp:
// �� Ŭ������ ������ ���ؼ��� ShuDeltaGraph.cpp�� �����Ͻʽÿ�.
//

class CShuDeltaGraphApp : public CWinAppEx
{
private:
	CString m_strArgument;
	CString m_strIniFile;

public:
	CShuDeltaGraphApp();
	bool IsExistFile( CString &strPathname );

	void FilteredOpenFile( CString strPathname );

	void SaveProfile( STR_FILTER_SETUP *pstFilterSetup );
	void LoadProfile( STR_FILTER_SETUP *pstFilterSetup );

private:
	bool OpenFile( CString &strPathname );
	void OnWindowCloseAll();

	ENUM_DataType GetDataType(CString & strPathName);


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
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnMenuCloseAll();
};

extern CShuDeltaGraphApp theApp;
