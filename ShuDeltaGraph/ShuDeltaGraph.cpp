
// ShuDeltaGraph.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CShuDeltaGraphApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CShuDeltaGraphApp::OnFileOpen)
	ON_COMMAND(ID_Menu_CloseALL, &CShuDeltaGraphApp::OnMenuCloseAll)
	ON_COMMAND(ID_DLG_COLLIST, &CShuDeltaGraphApp::OnDlgCollist)
	ON_COMMAND(ID_APP_EXIT, &CShuDeltaGraphApp::OnAppExit)
END_MESSAGE_MAP()


// CShuDeltaGraphApp ����

CShuDeltaGraphApp::CShuDeltaGraphApp()
{
	m_bHiColorIcons = TRUE;

	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ShuDeltaGraph.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CShuDeltaGraphApp ��ü�Դϴ�.

CShuDeltaGraphApp theApp;


// CShuDeltaGraphApp �ʱ�ȭ

BOOL CShuDeltaGraphApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	afxAmbientActCtx = FALSE;


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ShuDeltaGraphTYPE,
		RUNTIME_CLASS(CShuDeltaGraphDoc),
		RUNTIME_CLASS(CChildFrame), // ����� ���� MDI �ڽ� �������Դϴ�.
		RUNTIME_CLASS(CShuDeltaGraphView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �� MDI ������ â�� ����ϴ�.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  MDI ���� ���α׷������� m_pMainWnd�� ������ �� �ٷ� �̷��� ȣ���� �߻��ؾ� �մϴ�.

	m_strIniFile = AfxGetApp()->m_pszHelpFilePath;
	m_strIniFile.Replace( _T(".HLP"), _T(".ini") );

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
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

	


	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
 	//if (!ProcessShellCommand(cmdInfo))
 	//	return FALSE;

	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pMainFrame->ShowWindow( SW_SHOWMAXIMIZED /* m_nCmdShow */ );
	pMainFrame->UpdateWindow();

	m_pDlgColList = new CDlgColList();
	m_pDlgColList->Create( IDD_DIALOG_COL_LIST );

	return TRUE;
}

int CShuDeltaGraphApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	CShuDeltaGraphDoc::CloseMapData();

	AfxOleTerm(FALSE);

	delete m_pDlgColList;

	return CWinAppEx::ExitInstance();
}

// CShuDeltaGraphApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CShuDeltaGraphApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString strRev;

	strRev.Format( _T("SVN Rev : %d") , GetRevision() );
	m_CStaticRev.SetWindowText( strRev );

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


// CShuDeltaGraphApp ����� ���� �ε�/���� �޼���

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

// CShuDeltaGraphApp �޽��� ó����





void CShuDeltaGraphApp::OnFileNew()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bRet=false;
	CString strPathName;
	CDocTemplate *pDocTemplate=nullptr;	
	TCHAR warningMessage[100];

	int i;
	ENUM_GRAPH_VIEW viewPDWGraph[PDW_MULTI_WINDOWS] = { enGraphPulseInfo, enGRAPH_PIE, enGRAPH_POLAR, enGRAPH_2D, enGRAPH_MULTI, enGRAPH_3D } ;
	ENUM_SUB_GRAPH viewPDWSubGraph[PDW_MULTI_WINDOWS] = { enSubMenu_1, enSubMenu_1, enSubMenu_1, enSubMenu_3, enSubMenu_1, enSubMenu_1 } ;

	ENUM_GRAPH_VIEW viewIQGraph[IQ_MULTI_WINDOWS] = { enGraphPulseInfo, enGRAPH_2D, enGRAPH_2D, enGRAPH_2D, enGRAPH_2D } ;
	ENUM_SUB_GRAPH viewIQSubGraph[IQ_MULTI_WINDOWS] = { enSubMenu_1, enSubMenu_2, enSubMenu_3, enSubMenu_4, enSubMenu_1 } ;

	OnMenuCloseAll();

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

					pDoc = ( CShuDeltaGraphDoc *) pDocTemplate->OpenDocumentFile(NULL);

					pChild = ( CChildFrame * ) pMainFrame->GetActiveFrame();
					pView = (CShuDeltaGraphView *) pChild->GetActiveView();

					if( true == pDoc->OpenFile( strPathName ) ) {
						if( pView != NULL && pDoc->GetDataItems() != 0 ) {
							pView->ShowGraph( viewPDWGraph[i], viewPDWSubGraph[i] );
						}
						else {
							wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , strPathName );
							AfxMessageBox( warningMessage );
							break;
						}
					}
				}
			}
			else {
				wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , strPathName );
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
							wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , strPathName );
							AfxMessageBox( warningMessage );
							break;
						}
					}
				}
			}
			else {
				wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , strPathName );
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

	// �α� ������ ������ FILE Dialogâ�� �����Ѵ�.
	pWndFile = new CFileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("PDW/IQ ���ϵ� (*.spdw,*.pdw;*.npw;*.epdw;*.iq;*.siq)|*.spdw;*.pdw;*.npw;*.epdw;*.iq;*.siq|PDW ���ϵ� (*.pdw;*.npw;*.spdw;*.epdw)|*.pdw;*.npw;*.spdw;*.epdw|IQ ���ϵ� (*.iq;*.siq;*.eiq)|*.iq;*.siq;*.eiq|All Files (*.*)|*.*||") );

	// Initializes m_ofn structure
	pWndFile->m_ofn.lpstrTitle = _T("PDW �� IQ ���� �о����...");

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
  * @brief		���ϵ� â�� ��� �ݴ´�.
  * @return 	void
  * @return		������ true, ���н� false
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

void CShuDeltaGraphApp::OnMenuCloseAll()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OnWindowCloseAll();

}

void CShuDeltaGraphApp::FilteredOpenFile( CString strPathname )
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @param     STR_FILTER_SETUP * pstFilterSetup
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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

void CShuDeltaGraphApp::OnDlgCollist()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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


void CShuDeltaGraphApp::OnAppExit()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}
