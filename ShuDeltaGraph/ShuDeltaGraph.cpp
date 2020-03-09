
// ShuDeltaGraph.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ShuDeltaGraph.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ShuDeltaGraphDoc.h"
#include "ShuDeltaGraphView.h"
#include "revision.h"
#include "afxwin.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CShuDeltaGraphApp

BEGIN_MESSAGE_MAP(CShuDeltaGraphApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CShuDeltaGraphApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CShuDeltaGraphApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CShuDeltaGraphApp::OnFileOpen)
	ON_COMMAND(ID_Menu_CloseALL, &CShuDeltaGraphApp::OnMenuCloseAll)
	ON_COMMAND(ID_DLG_COLLIST, &CShuDeltaGraphApp::OnDlgCollist)
	ON_COMMAND(ID_APP_EXIT, &CShuDeltaGraphApp::OnAppExit)
	ON_COMMAND(ID_GRAPH_COLLIST, &CShuDeltaGraphApp::OnGraphCollist)
END_MESSAGE_MAP()


// CShuDeltaGraphApp 생성

CShuDeltaGraphApp::CShuDeltaGraphApp()
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ShuDeltaGraph.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CShuDeltaGraphApp 개체입니다.

CShuDeltaGraphApp theApp;


// CShuDeltaGraphApp 초기화

BOOL CShuDeltaGraphApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	afxAmbientActCtx = FALSE;


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ShuDeltaGraphTYPE,
		RUNTIME_CLASS(CShuDeltaGraphDoc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CShuDeltaGraphView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.

	m_strIniFile = AfxGetApp()->m_pszHelpFilePath;
	m_strIniFile.Replace( _T(".HLP"), _T(".ini") );

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if( true == cmdInfo.m_strFileName.IsEmpty() ) {
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}
	else {
		m_strArgument = cmdInfo.m_strFileName;

		OnFileOpen();
		//cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
	}


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
 	//if (!ProcessShellCommand(cmdInfo))
 	//	return FALSE;
	//AfxSetAllocStop( 8253 );

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow( SW_SHOWMAXIMIZED /* m_nCmdShow */ );
	pMainFrame->UpdateWindow();

	m_pDlgColList = new CDlgColList();
	m_pDlgColList->Create( IDD_DIALOG_COL_LIST );

	m_pDlg2DHisto = new CDlg2DHisto();
	m_pDlg2DHisto->Create( IDD_DIALOG_2DHISTO );

	m_pDlgMulti = new CDlgMulti();
	m_pDlgMulti->Create( IDD_DIALOG_MULTI );

	m_pDlg3DBar = new CDlg3DBar();
	m_pDlg3DBar->Create( IDD_DIALOG_3DBAR );

	return TRUE;
}

int CShuDeltaGraphApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	TRACE( "\n ExitInstance..호출" );
	
	CShuDeltaGraphDoc::CloseMapData();

	AfxOleTerm(FALSE);

	delete m_pDlgColList;
	delete m_pDlg2DHisto;
	delete m_pDlgMulti;
	delete m_pDlg3DBar;

	return CWinAppEx::ExitInstance();
}

// CShuDeltaGraphApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	CStatic m_CStaticRev;
	virtual BOOL OnInitDialog();
	CStatic m_CStaticRevDate;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_REV, m_CStaticRev);
	DDX_Control(pDX, IDC_STATIC_REV_DATE, m_CStaticRevDate);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CShuDeltaGraphApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString strRev;

	strRev.Format( _T("SVN Date %s") , GetBuildDate() );
	m_CStaticRev.SetWindowText( strRev );

	strRev.Format( _T("SVN Rev %s") , GetRevision() );
	m_CStaticRevDate.SetWindowText( strRev );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// CShuDeltaGraphApp 사용자 지정 로드/저장 메서드

void CShuDeltaGraphApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CShuDeltaGraphApp::LoadCustomState()
{
}

void CShuDeltaGraphApp::SaveCustomState()
{
}

// CShuDeltaGraphApp 메시지 처리기





void CShuDeltaGraphApp::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDocTemplate *pDocTemplate=nullptr;

	POSITION pos=GetFirstDocTemplatePosition();

	//for( int i=0; i < m_poscntD)
	pDocTemplate = GetNextDocTemplate( pos );

	pDocTemplate->OpenDocumentFile(NULL);
}

#define PDW_MULTI_WINDOWS			(6)
#define IQ_MULTI_WINDOWS			(5)
void CShuDeltaGraphApp::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString strPathName;

	if( m_strArgument.IsEmpty() == true ) {
		if( true == OpenFile( strPathName, _T("PDW 및 IQ 파일 읽어오기...") ) ) {
			RawDataOpen( & strPathName );
		}
	}
	else {
		strPathName = m_strArgument;
		RawDataOpen( & strPathName );
		m_strArgument.Empty();
	}

}

/**
 * @brief     
 * @param     CString * pStrPathname
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/08 21:42:48
 * @warning   
 */
void CShuDeltaGraphApp::RawDataOpen( CString *pStrPathname )
{
	int i;

	CDocTemplate *pDocTemplate=nullptr;	
	TCHAR warningMessage[100];

	POSITION pos;
	CMainFrame *pMainFrame=( CMainFrame * ) AfxGetMainWnd();
	CShuDeltaGraphDoc *pDoc;
	CShuDeltaGraphView *pView;
	CChildFrame *pChild;

	ENUM_GRAPH_VIEW viewPDWGraph[PDW_MULTI_WINDOWS] = { enGraphPulseInfo, enGRAPH_PIE, enGRAPH_POLAR, enGRAPH_2D, enGRAPH_MULTI, enGRAPH_3D } ;
	ENUM_SUB_GRAPH viewPDWSubGraph[PDW_MULTI_WINDOWS] = { enSubMenu_1, enSubMenu_1, enSubMenu_1, enSubMenu_3, enSubMenu_1, enSubMenu_1 } ;

	ENUM_GRAPH_VIEW viewIQGraph[IQ_MULTI_WINDOWS] = { enGraphPulseInfo, enGRAPH_2D, enGRAPH_2D, enGRAPH_2D, enGRAPH_2D } ;
	ENUM_SUB_GRAPH viewIQSubGraph[IQ_MULTI_WINDOWS] = { enSubMenu_1, enSubMenu_2, enSubMenu_3, enSubMenu_4, enSubMenu_1 } ;


	OnMenuCloseAll();

	if( GetDataType(*pStrPathname) == en_PDW_DATA ) {
		if( true == IsExistFile( *pStrPathname ) ) {

			for( i=0 ; i < PDW_MULTI_WINDOWS ; ++i ) {
				pos = GetFirstDocTemplatePosition();

				pDocTemplate = GetNextDocTemplate( pos );

				pDoc = ( CShuDeltaGraphDoc *) pDocTemplate->OpenDocumentFile(NULL);

				pChild = ( CChildFrame * ) pMainFrame->GetActiveFrame();
				pView = (CShuDeltaGraphView *) pChild->GetActiveView();

				if( true == pDoc->OpenFile( *pStrPathname ) ) {
					if( pView != NULL && pDoc->GetDataItems() != 0 ) {
						pView->ShowGraph( viewPDWGraph[i], viewPDWSubGraph[i] );
					}
					else {
						wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , *pStrPathname );
						AfxMessageBox( warningMessage );
						break;
					}
				}
			}
		}
		else {
			wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , *pStrPathname );
			AfxMessageBox( warningMessage );
		}
	}
	else {

		if( true == IsExistFile( *pStrPathname ) ) {
			for( i=0 ; i < IQ_MULTI_WINDOWS ; ++i ) {
				pos = GetFirstDocTemplatePosition();

				pDocTemplate = GetNextDocTemplate( pos );

				pDoc = ( CShuDeltaGraphDoc *) pDocTemplate->OpenDocumentFile(NULL);

				pChild = ( CChildFrame * ) pMainFrame->GetActiveFrame();
				pView = (CShuDeltaGraphView *) pChild->GetActiveView();

				if( true == pDoc->OpenFile( *pStrPathname ) ) {
					if( pView != NULL && pDoc->GetDataItems() != 0 ) {
						pView->ShowGraph( viewIQGraph[i], viewIQSubGraph[i] );
					}
					else {
						wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , *pStrPathname );
						AfxMessageBox( warningMessage );
						break;
					}
				}
			}
		}
		else {
			wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , *pStrPathname );
			AfxMessageBox( warningMessage );
		}
	}

	::PostMessage( m_pMainWnd->m_hWnd, WM_COMMAND, ID_WINDOW_TILE_HORZ, NULL );

}

/**
 * @brief     
 * @param     CString & strPathname
 * @return    bool
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 16:31:42
 * @warning   
 */
bool CShuDeltaGraphApp::IsExistFile( CString &strPathname )
{
	bool bRet;
	CFileStatus fs;

	if( CFile::GetStatus( strPathname, fs ) == TRUE ) {
		bRet = true;
	}
	else {
		bRet = false;
	}
	return bRet;
}

/**
 * @brief     
 * @param     CString & strPathname
 * @return    bool
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 16:31:37
 * @warning   
 */
bool CShuDeltaGraphApp::OpenFile( CString &strPathname, TCHAR *pTitle, ENUM_OPENTYPE enOpenType )
{
	bool bRet = true;
	CFileDialog *pWndFile;
	TCHAR szinitDir[MAX_PATH];

	CString strFilepath;

	strFilepath = GetFilePath();

	// 로그 파일을 오픈할 FILE Dialog창을 생성한다.
	switch( enOpenType ) {
		case enOpenPDW :
			pWndFile = new CFileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("PDW/IQ 파일들 (*.spdw,*.pdw;*.npw;*.epdw;*.kpdw;*.iq;*.siq;*.dat)|*.spdw;*.pdw;*.npw;*.epdw;*.kpdw;*.iq;*.siq;*.dat|PDW 파일들 (*.pdw;*.npw;*.spdw;*.epdw;*.kpdw;*.dat)|*.pdw;*.npw;*.spdw;*.epdw;*.kpdw;*.dat|IQ 파일들 (*.iq;*.siq;*.eiq)|*.iq;*.siq;*.eiq|All Files (*.*)|*.*||") );
			szinitDir[0] = NULL;
			break;

		case enOpenXLS :
			pWndFile = new CFileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("수집 목록 파일들 (*.xls)|*.xls*|All Files (*.*)|*.*||") );
			
			_tcscpy_s( szinitDir, MAX_PATH, strFilepath.GetBuffer(0) );
			strFilepath.ReleaseBuffer();
			break;

		case enSavePDW :
			pWndFile = new CFileDialog(FALSE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("PDW/IQ 파일들 (*.spdw,*.pdw;*.npw;*.epdw;*.iq;*.siq)|*.spdw;*.pdw;*.npw;*.epdw;*.iq;*.siq|PDW 파일들 (*.pdw;*.npw;*.spdw;*.epdw;*.dat)|*.pdw;*.npw;*.spdw;*.epdw;*.dat|IQ 파일들 (*.iq;*.siq;*.eiq)|*.iq;*.siq;*.eiq|All Files (*.*)|*.*||") );
			szinitDir[0] = NULL;
			break;

		case enSaveXLS :
			pWndFile = new CFileDialog(FALSE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("수집 목록 파일들 (*.xls)|*.xls*|All Files (*.*)|*.*||") );

			_tcscpy_s( szinitDir, MAX_PATH, strFilepath.GetBuffer(0) );
			strFilepath.ReleaseBuffer();
			break;

		default :
			break;
	}


	// Initializes m_ofn structure
	pWndFile->m_ofn.lpstrTitle = pTitle;			// 타이틀명
	pWndFile->m_ofn.lpstrInitialDir = szinitDir;			// 타이틀명

	// Call DoModal
	if (pWndFile->DoModal() == IDOK) {
		//m_strWindowtitle = pWndFile->GetFileTitle() + '.' + pWndFile->GetFileExt();
		strPathname = pWndFile->GetPathName();

	}
	else {
		bRet = false;
	}

	delete pWndFile;

	return bRet;

}

/**
  * @brief		차일드 창을 모두 닫는다.
  * @return 	void
  * @return		성공시 true, 실패시 false
  * @date       2019/06/10 10:06
*/
void CShuDeltaGraphApp::OnWindowCloseAll()
{
	POSITION pos;
	CDocTemplate* pTemplate;

	pos = GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		pTemplate = GetNextDocTemplate(pos);
		pTemplate->CloseAllDocuments(FALSE);
	}
}

ENUM_DataType CShuDeltaGraphApp::GetDataType(CString &strPathName)
{
	ENUM_DataType enDataType=en_UnknownData;

	strPathName.MakeLower();
	if( NULL != wcsstr( strPathName.GetBuffer(), L"pdw" ) || NULL != wcsstr( strPathName.GetBuffer(), L"npw" ) || NULL != wcsstr( strPathName.GetBuffer(), L"dat" ) ) {
		enDataType = en_PDW_DATA;
	}

	if( NULL != wcsstr( strPathName.GetBuffer(), L"iq" ) ) {
		enDataType = en_IQ_DATA;
	}

	return enDataType;

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:19
 * @warning   
 */
void CShuDeltaGraphApp::OnMenuCloseAll()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnWindowCloseAll();

}

/**
 * @brief     
 * @param     CString strPathname
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 16:31:49
 * @warning   
 */
void CShuDeltaGraphApp::FilteredOpenFile( CString strPathname )
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i=0;
	POSITION posTemplate, posDoc, posView;
	CDocTemplate* pTemplate;

	CData *pData;
	STR_FILTER_SETUP stFilterSetup;
	
	CShuDeltaGraphDoc *pDoc = (CShuDeltaGraphDoc *) (((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveDocument();

	ENUM_GRAPH_VIEW viewPDWGraph[PDW_MULTI_WINDOWS] = { enGraphPulseInfo, enGRAPH_PIE, enGRAPH_POLAR, enGRAPH_2D, enGRAPH_MULTI, enGRAPH_3D } ;

	pData = pDoc->FindMapData( & strPathname );
	memcpy( & stFilterSetup, & pData->m_stFilterSetup, sizeof(STR_FILTER_SETUP) );
	pDoc->CloseMapData( & strPathname );

	posTemplate = GetFirstDocTemplatePosition();

	while (posTemplate != NULL)
	{
		pTemplate = GetNextDocTemplate(posTemplate);
		posDoc = pTemplate->GetFirstDocPosition();
		while( posDoc ) {
			CShuDeltaGraphDoc *pDoc=( CShuDeltaGraphDoc *) pTemplate->GetNextDoc( posDoc );
			
			pDoc->OpenFile( strPathname, & stFilterSetup );
			posView = pDoc->GetFirstViewPosition();
			
			while( posView ) {
				CShuDeltaGraphView *pView;
				pView = ( CShuDeltaGraphView * ) pDoc->GetNextView(posView);
				pView->ShowGraph( pView->m_enGraphView, enSubMenu_1 );
			}
		}
	}

}

/**
 * @brief     
 * @param     STR_FILTER_SETUP * pstFilterSetup
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/01/31 17:59:42
 * @warning   
 */
void CShuDeltaGraphApp::SaveProfile( STR_FILTER_SETUP *pstFilterSetup )
{
	TCHAR szBuffer[100];

	swprintf( szBuffer, sizeof(szBuffer), L"%f", pstFilterSetup->dAoaMin );
	::WritePrivateProfileString( L"FilterSetup", L"AOA_MIN", szBuffer, m_strIniFile );

	swprintf( szBuffer, sizeof(szBuffer), L"%f", pstFilterSetup->dAoaMax );
	::WritePrivateProfileString( L"FilterSetup", L"AOA_MAX", szBuffer, m_strIniFile );

	swprintf( szBuffer, sizeof(szBuffer), L"%f", pstFilterSetup->dFrqMin );
	::WritePrivateProfileString( L"FilterSetup", L"FRQ_MIN", szBuffer, m_strIniFile );

	swprintf( szBuffer, sizeof(szBuffer), L"%f", pstFilterSetup->dFrqMax );
	::WritePrivateProfileString( L"FilterSetup", L"FRQ_MAX", szBuffer, m_strIniFile );
}

/**
 * @brief     
 * @param     STR_COL_ITEM * pstColList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:14
 * @warning   
 */
void CShuDeltaGraphApp::SaveProfile( STR_COL_ITEM *pstColList )
{
	TCHAR szBuffer[100];

	swprintf( szBuffer, sizeof(szBuffer), L"%d", pstColList->enMode );
	::WritePrivateProfileString( L"COL_LIST", L"MODE", szBuffer, m_strIniFile );

	swprintf( szBuffer, sizeof(szBuffer), L"%f", pstColList->fCenterFreq );
	::WritePrivateProfileString( L"COL_LIST", L"CENTER_FREQ", szBuffer, m_strIniFile );

	swprintf( szBuffer, sizeof(szBuffer), L"%f", pstColList->fThreshold );
	::WritePrivateProfileString( L"COL_LIST", L"THRESHOLD", szBuffer, m_strIniFile );

	swprintf( szBuffer, sizeof(szBuffer), L"%f", pstColList->fColTime );
	::WritePrivateProfileString( L"COL_LIST", L"COL_TIME", szBuffer, m_strIniFile );

	swprintf( szBuffer, sizeof(szBuffer), L"%d", pstColList->uiColNumber );
	::WritePrivateProfileString( L"COL_LIST", L"COL_NUM", szBuffer, m_strIniFile );

}

/**
 * @brief     
 * @param     STR_FILTER_SETUP * pstFilterSetup
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/01/31 17:59:39
 * @warning   
 */
void CShuDeltaGraphApp::LoadProfile( STR_FILTER_SETUP *pstFilterSetup )
{
	TCHAR szBuffer[100];
	
	::GetPrivateProfileString( L"FilterSetup", L"AOA_MIN", L"0.0", szBuffer, 100, m_strIniFile );
	pstFilterSetup->dAoaMin = _wtof(szBuffer);

	::GetPrivateProfileString( L"FilterSetup", L"AOA_MAX", L"0.0", szBuffer, 100, m_strIniFile );
	pstFilterSetup->dAoaMax = _wtof(szBuffer);

	::GetPrivateProfileString( L"FilterSetup", L"FRQ_MIN", L"0.0", szBuffer, 100, m_strIniFile );
	pstFilterSetup->dFrqMin = _wtof(szBuffer);

	::GetPrivateProfileString( L"FilterSetup", L"FRQ_MAX", L"0.0", szBuffer, 100, m_strIniFile );
	pstFilterSetup->dFrqMax = _wtof(szBuffer);

}

/**
 * @brief     
 * @param     STR_COL_ITEM * pstColList
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:09
 * @warning   
 */
void CShuDeltaGraphApp::LoadProfile( STR_COL_ITEM *pstColList )
{
	TCHAR szBuffer[100];

	::GetPrivateProfileString( L"COL_LIST", L"MODE", L"0", szBuffer, 100, m_strIniFile );
	pstColList->enMode = (ENUM_COL_MODE) _wtoi(szBuffer);

	::GetPrivateProfileString( L"COL_LIST", L"CENTER_FREQ", L"8000.0", szBuffer, 100, m_strIniFile );
	pstColList->fCenterFreq = (float) _wtof(szBuffer);

	::GetPrivateProfileString( L"COL_LIST", L"THRESHOLD", L"-30.0", szBuffer, 100, m_strIniFile );
	pstColList->fThreshold = (float) _wtof(szBuffer);

	::GetPrivateProfileString( L"COL_LIST", L"COL_TIME", L"1000.0", szBuffer, 100, m_strIniFile );
	pstColList->fColTime = (float) _wtof(szBuffer);

	::GetPrivateProfileString( L"COL_LIST", L"COL_NUM", L"100.0", szBuffer, 100, m_strIniFile );
	pstColList->uiColNumber = (int) _wtof(szBuffer);

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 16:28:39
 * @warning   
 */
void CShuDeltaGraphApp::OnDlgCollist()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BOOL chk = m_pDlgColList->IsWindowVisible();

	if(chk)
	{
		m_pDlgColList->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pDlgColList->ShowWindow(SW_SHOW);
	}	

	//m_pDlgColList->SetParent();

}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:04
 * @warning   
 */
void CShuDeltaGraphApp::OnAppExit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:05
 * @warning   
 */
void CShuDeltaGraphApp::OnGraphCollist()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BOOL chk;

	chk = m_pDlg2DHisto->IsWindowVisible();
	if(chk & false )
	{
		//m_pDlg2DHisto->ShowWindow(SW_RESTORE);
		m_pDlg2DHisto->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pDlg2DHisto->ShowWindow(SW_RESTORE);
		m_pDlg2DHisto->ShowWindow(SW_SHOW);
	}

	chk = m_pDlgMulti->IsWindowVisible();
	if(chk & false )
	{
		m_pDlgMulti->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pDlgMulti->ShowWindow(SW_RESTORE);
		m_pDlgMulti->ShowWindow(SW_SHOW);
	}

	chk = m_pDlg3DBar->IsWindowVisible();
	if(chk & false )
	{
		m_pDlg3DBar->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pDlg3DBar->ShowWindow(SW_RESTORE);
		m_pDlg3DBar->ShowWindow(SW_SHOW);
	}
}


/**
 * @brief     
 * @param     BOOL bEnable
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:00
 * @warning   
 */
void CShuDeltaGraphApp::ActivateGraph( BOOL bEnable )
{
	if( bEnable == TRUE ) {
		m_pDlg2DHisto->InitVar();
		m_pDlg2DHisto->ViewGraph();

		m_pDlg2DHisto->ShowWindow(SW_RESTORE);
		m_pDlg2DHisto->ShowWindow(SW_SHOW);
		m_pDlg2DHisto->SetTimer( CLK_TIMER, 1500, 0 );

		m_pDlgMulti->ShowWindow(SW_RESTORE);
		m_pDlgMulti->ShowWindow(SW_SHOW);

		m_pDlg3DBar->InitVar();
		m_pDlg3DBar->ShowWindow(SW_RESTORE);
		m_pDlg3DBar->ShowWindow(SW_SHOW);
		m_pDlg3DBar->SetTimer( CLK_TIMER, 1000, 0 );
		
	}
	else {
		m_pDlg3DBar->KillTimer( CLK_TIMER );

		m_pDlg2DHisto->KillTimer( CLK_TIMER );
	}

}

