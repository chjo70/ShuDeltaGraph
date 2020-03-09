
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

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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
	ON_COMMAND(ID_GRAPH_COLLIST, &CShuDeltaGraphApp::OnGraphCollist)
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
	//AfxSetAllocStop( 8253 );

	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
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
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	TRACE( "\n ExitInstance..ȣ��" );
	
	CShuDeltaGraphDoc::CloseMapData();

	AfxOleTerm(FALSE);

	delete m_pDlgColList;
	delete m_pDlg2DHisto;
	delete m_pDlgMulti;
	delete m_pDlg3DBar;

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

	strRev.Format( _T("SVN Date %s") , GetBuildDate() );
	m_CStaticRev.SetWindowText( strRev );

	strRev.Format( _T("SVN Rev %s") , GetRevision() );
	m_CStaticRevDate.SetWindowText( strRev );

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
	CString strPathName;

	if( m_strArgument.IsEmpty() == true ) {
		if( true == OpenFile( strPathName, _T("PDW �� IQ ���� �о����...") ) ) {
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
						wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , *pStrPathname );
						AfxMessageBox( warningMessage );
						break;
					}
				}
			}
		}
		else {
			wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , *pStrPathname );
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
						wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , *pStrPathname );
						AfxMessageBox( warningMessage );
						break;
					}
				}
			}
		}
		else {
			wsprintf( warningMessage, _T("���ϸ�[%s]�� �߸� �Է��߽��ϴ�.") , *pStrPathname );
			AfxMessageBox( warningMessage );
		}
	}

	::PostMessage( m_pMainWnd->m_hWnd, WM_COMMAND, ID_WINDOW_TILE_HORZ, NULL );

}

/**
 * @brief     
 * @param     CString & strPathname
 * @return    bool
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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

	// �α� ������ ������ FILE Dialogâ�� �����Ѵ�.
	switch( enOpenType ) {
		case enOpenPDW :
			pWndFile = new CFileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("PDW/IQ ���ϵ� (*.spdw,*.pdw;*.npw;*.epdw;*.kpdw;*.iq;*.siq;*.dat)|*.spdw;*.pdw;*.npw;*.epdw;*.kpdw;*.iq;*.siq;*.dat|PDW ���ϵ� (*.pdw;*.npw;*.spdw;*.epdw;*.kpdw;*.dat)|*.pdw;*.npw;*.spdw;*.epdw;*.kpdw;*.dat|IQ ���ϵ� (*.iq;*.siq;*.eiq)|*.iq;*.siq;*.eiq|All Files (*.*)|*.*||") );
			szinitDir[0] = NULL;
			break;

		case enOpenXLS :
			pWndFile = new CFileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("���� ��� ���ϵ� (*.xls)|*.xls*|All Files (*.*)|*.*||") );
			
			_tcscpy_s( szinitDir, MAX_PATH, strFilepath.GetBuffer(0) );
			strFilepath.ReleaseBuffer();
			break;

		case enSavePDW :
			pWndFile = new CFileDialog(FALSE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("PDW/IQ ���ϵ� (*.spdw,*.pdw;*.npw;*.epdw;*.iq;*.siq)|*.spdw;*.pdw;*.npw;*.epdw;*.iq;*.siq|PDW ���ϵ� (*.pdw;*.npw;*.spdw;*.epdw;*.dat)|*.pdw;*.npw;*.spdw;*.epdw;*.dat|IQ ���ϵ� (*.iq;*.siq;*.eiq)|*.iq;*.siq;*.eiq|All Files (*.*)|*.*||") );
			szinitDir[0] = NULL;
			break;

		case enSaveXLS :
			pWndFile = new CFileDialog(FALSE, NULL, NULL, OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_SHOWHELP | OFN_HIDEREADONLY, _T("���� ��� ���ϵ� (*.xls)|*.xls*|All Files (*.*)|*.*||") );

			_tcscpy_s( szinitDir, MAX_PATH, strFilepath.GetBuffer(0) );
			strFilepath.ReleaseBuffer();
			break;

		default :
			break;
	}


	// Initializes m_ofn structure
	pWndFile->m_ofn.lpstrTitle = pTitle;			// Ÿ��Ʋ��
	pWndFile->m_ofn.lpstrInitialDir = szinitDir;			// Ÿ��Ʋ��

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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:19
 * @warning   
 */
void CShuDeltaGraphApp::OnMenuCloseAll()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OnWindowCloseAll();

}

/**
 * @brief     
 * @param     CString strPathname
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 16:31:49
 * @warning   
 */
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
 * @param     STR_COL_ITEM * pstColList
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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

/**
 * @brief     
 * @param     STR_COL_ITEM * pstColList
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/01 16:28:39
 * @warning   
 */
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


/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:04
 * @warning   
 */
void CShuDeltaGraphApp::OnAppExit()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


/**
 * @brief     
 * @return    void
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/21 9:21:05
 * @warning   
 */
void CShuDeltaGraphApp::OnGraphCollist()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
 * @author    ��ö�� (churlhee.jo@lignex1.com)
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

