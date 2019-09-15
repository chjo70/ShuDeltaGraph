
// ShuDeltaGraph.h : ShuDeltaGraph 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include <map>

#include "./PDW/DataFile.h"

// CShuDeltaGraphApp:
// 이 클래스의 구현에 대해서는 ShuDeltaGraph.cpp을 참조하십시오.
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


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
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
