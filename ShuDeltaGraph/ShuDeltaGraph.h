
// ShuDeltaGraph.h : ShuDeltaGraph ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include <map>

#include "./PDW/DataFile.h"

#include "DlgColList.h"
#include "Dlg2DHisto.h"
#include "DlgMulti.h"
#include "Dlg3DBar.h"

// CShuDeltaGraphApp:
// �� Ŭ������ ������ ���ؼ��� ShuDeltaGraph.cpp�� �����Ͻʽÿ�.
//

class CShuDeltaGraphApp : public CWinAppEx
{
private:
	CString m_strArgument;
	CString m_strIniFile;

	CMapData m_theMapData;

public:
	CDlgColList *m_pDlgColList;
	CDlg2DHisto *m_pDlg2DHisto;
	CDlgMulti *m_pDlgMulti;
	CDlg3DBar *m_pDlg3DBar;

public:
	CShuDeltaGraphApp();
	bool IsExistFile( CString &strPathname );

	void FilteredOpenFile( CString strPathname );

	void SaveProfile( STR_FILTER_SETUP *pstFilterSetup );
	void LoadProfile( STR_FILTER_SETUP *pstFilterSetup );
	void LoadProfile( STR_COL_ITEM *pstColList );
	void SaveProfile( STR_COL_ITEM *pstColList );

	bool OpenFile( CString &strPathname, TCHAR *pTitle, ENUM_OPENTYPE enOpenType=enOpenPDW );

	void RawDataOpen( CString *pStrPathname );
	void ActivateGraph( BOOL bEnable );

	inline void CloseMapData( CString *pStrPathname ) { m_theMapData.CloseMapData( pStrPathname ); }
	inline CData *FindMapData( CString *pStrPathname ) { return m_theMapData.FindMapData( pStrPathname ); }
	inline void AddMapData( CString *pStrPathname, CData *pData ) { m_theMapData.AddMapData( pStrPathname, pData ); }
	inline void IncWindowNumber( CData *pData ) { return m_theMapData.IncWindowNumber( pData ); }

private:
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
	afx_msg void OnDlgCollist();
	afx_msg void OnAppExit();
	afx_msg void OnGraphCollist();
};

extern CShuDeltaGraphApp theApp;
