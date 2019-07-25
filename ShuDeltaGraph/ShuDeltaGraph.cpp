
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

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow( SW_SHOWMAXIMIZED /* m_nCmdShow */ );
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CShuDeltaGraphApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	CShuDeltaGraphDoc::CloseMapData();

	AfxOleTerm(FALSE);

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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_REV, m_CStaticRev);
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

	strRev.Format( _T("SVN Rev : %d") , GetRevision() );
	m_CStaticRev.SetWindowText( strRev );

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
	bool bRet=false;
	CString strPathName;
	CDocTemplate *pDocTemplate=nullptr;	
	TCHAR warningMessage[100];

	int i;
	ENUM_GRAPH_VIEW viewPDWGraph[PDW_MULTI_WINDOWS] = { enGraphPulseInfo, enGRAPH_PIE, enGRAPH_POLAR, enGRAPH_2D, enGRAPH_MULTI, enGRAPH_3D } ;

	ENUM_GRAPH_VIEW viewIQGraph[IQ_MULTI_WINDOWS] = { enGraphPulseInfo, enGRAPH_2D, enGRAPH_2D, enGRAPH_2D, enGRAPH_2D } ;
	ENUM_SUB_GRAPH viewIQSubGraph[IQ_MULTI_WINDOWS] = { enSubMenu_1, enSubMenu_2, enSubMenu_3, enSubMenu_4, enSubMenu_1 } ;

	if( m_strArgument.IsEmpty() == true ) {
		if( false == OpenFile( strPathName ) ) {
			bRet = true;
		}
	}
	else {
		strPathName = m_strArgument;
		m_strArgument.Empty();
	}

	if( bRet == false ) {
		POSITION pos;
		CMainFrame *pMainFrame=( CMainFrame * ) AfxGetMainWnd();
		CShuDeltaGraphDoc *pDoc;
		CShuDeltaGraphView *pView;
		CChildFrame *pChild;

		if( GetDataType(strPathName) == en_PDW_DATA ) {
			if( true == IsExistFile( strPathName ) ) {
				for( i=0 ; i < PDW_MULTI_WINDOWS ; ++i ) {
					pos = GetFirstDocTemplatePosition();

					pDocTemplate = GetNextDocTemplate( pos );

					pDoc= ( CShuDeltaGraphDoc *) pDocTemplate->OpenDocumentFile(NULL);

					pChild = ( CChildFrame * ) pMainFrame->GetActiveFrame();
					pView = (CShuDeltaGraphView *) pChild->GetActiveView();

					if( true == pDoc->OpenFile( strPathName ) ) {
						if( pView != NULL && pDoc->GetDataItems() != 0 ) {
							pView->ShowGraph( viewPDWGraph[i], enSubMenu_1 );
						}
						else {
							wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , strPathName );
							AfxMessageBox( warningMessage );
							break;
						}
					}
				}
			}
			else {
				wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , strPathName );
				AfxMessageBox( warningMessage );
			}
		}
		else {

			if( true == IsExistFile( strPathName ) ) {
				for( i=0 ; i < IQ_MULTI_WINDOWS ; ++i ) {
					pos = GetFirstDocTemplatePosition();

					pDocTemplate = GetNextDocTemplate( pos );

					pDoc = ( CShuDeltaGraphDoc *) pDocTemplate->OpenDocumentFile(NULL);

					pChild = ( CChildFrame * ) pMainFrame->GetActiveFrame();
					pView = (CShuDeltaGraphView *) pChild->GetActiveView();

					if( true == pDoc->OpenFile( strPathName ) ) {
						if( pView != NULL && pDoc->GetDataItems() != 0 ) {
							pView->ShowGraph( viewIQGraph[i], viewIQSubGraph[i] );
						}
						else {
							wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , strPathName );
							AfxMessageBox( warningMessage );
							break;
						}
					}
				}
			}
			else {
				wsprintf( warningMessage, _T("파일명[%s]을 잘못 입력했습니다.") , strPathName );
				AfxMessageBox( warningMessage );
			}
		}

		::PostMessage( m_pMainWnd->m_hWnd, WM_COMMAND, ID_WINDOW_TILE_HORZ, NULL );
	}

}

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

bool CShuDeltaGraphApp::OpenFile( CString &strPathname )
{
	bool bRet = true;
	CFileDialog *pWndFile;

	// 로그 파일을 오픈할 FILE Dialog창을 생성한다.
	pWndFile = new CFileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("PDW/IQ 파일들 (*.spdw,*.pdw;*.npw;*.epdw;*.iq;*.siq)|*.spdw;*.pdw;*.npw;*.epdw;*.iq;*.siq|PDW 파일들 (*.pdw;*.npw;*.spdw;*.epdw)|*.pdw;*.npw;*.spdw;*.epdw|IQ 파일들 (*.iq;*.siq;*.eiq)|*.iq;*.siq;*.eiq|All Files (*.*)|*.*||") );

	// Initializes m_ofn structure
	pWndFile->m_ofn.lpstrTitle = _T("PDW 및 IQ 파일 읽어오기...");

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
	if( NULL != wcsstr( strPathName.GetBuffer(), L"pdw" ) || NULL != wcsstr( strPathName.GetBuffer(), L"npw" ) ) {
		enDataType = en_PDW_DATA;
	}

	if( NULL != wcsstr( strPathName.GetBuffer(), L"iq" ) ) {
		enDataType = en_IQ_DATA;
	}

	return enDataType;

}