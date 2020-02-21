// DlgMulti.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "DlgMulti.h"
#include "afxdialogex.h"

#include "Pegrpapi.h"

#include "./EasySize/EasySize.h"


// CDlgMulti 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMulti, CDialogEx)

CDlgMulti::CDlgMulti(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMulti::IDD, pParent)
{

}

CDlgMulti::~CDlgMulti()
{
	FreeBuffer();
}

void CDlgMulti::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MULTI, m_CStatic);
}


BEGIN_MESSAGE_MAP(CDlgMulti, CDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZING()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CDlgMulti)
	//EASYSIZE(IDC_STATIC_GRAPH,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP


// CDlgMulti 메시지 처리기입니다.


BOOL CDlgMulti::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	this->GetWindowRect( m_rectCurDlg );

	Log( enNormal, _T("CDlgMulti +++++++++++++++++++++++++++++++++++++++++++") );

	// 초기 설정
	InitVar();
	InitButton();
	InitBuffer();
	InitStatusBar();
	InitListCtrl();
	InitStatic();
	InitToolTip();
	InitThread();

	// 그래프 설정
	InitGraph();

	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:45:00
 * @warning   
 */
void CDlgMulti::InitVar()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:58
 * @warning   
 */
void CDlgMulti::InitButton()
{

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:44:57
 * @warning   
 */
void CDlgMulti::InitBuffer()
{
	CShuDeltaGraphApp *pApp = ( CShuDeltaGraphApp *) AfxGetApp();

	//m_pSonataData = pApp->m_pDlgColList->GetSONATAData();

	m_hMulti = CreateEvent( NULL, FALSE, FALSE, NULL );

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:45:03
 * @warning   
 */
void CDlgMulti::InitListCtrl()
{

}

void CDlgMulti::InitStatusBar()
{

}

void CDlgMulti::InitStatic()
{

}

void CDlgMulti::InitGraph()
{
	RECT rect;

	GetClientRect( &rect );

	m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001);

}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:46:58
 * @warning   
 */
void CDlgMulti::FreeBuffer()
{
	if (m_hPE)
	{
		PEdestroy(m_hPE);
		m_hPE = 0; 
	}
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:46:55
 * @warning   
 */
void CDlgMulti::InitThread()
{

}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:46:53
 * @warning   
 */
void CDlgMulti::InitToolTip()
{

}



/**
 * @brief     
 * @param     UINT nType
 * @param     int cx
 * @param     int cy
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:49:51
 * @warning   
 */
void CDlgMulti::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_hPE ) {
		::MoveWindow( m_hPE, 0, 0, cx, cy, FALSE);
	}

	UPDATE_EASYSIZE;
}


void CDlgMulti::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LPMINMAXINFO lpminmax = ( LPMINMAXINFO ) lpMMI;

	// 창 최소크기 지정
	lpminmax->ptMinTrackSize.x = m_rectCurDlg.right - m_rectCurDlg.left;
	lpminmax->ptMinTrackSize.y = m_rectCurDlg.bottom - m_rectCurDlg.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


/**
 * @brief     
 * @param     UINT fwSide
 * @param     LPRECT pRect
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/20 21:50:59
 * @warning   
 */
void CDlgMulti::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EASYSIZE_MINSIZE(m_rectCurDlg.right - m_rectCurDlg.left,m_rectCurDlg.bottom - m_rectCurDlg.top,fwSide,pRect);
}


void CDlgMulti::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nID == SC_CLOSE)	
	{
		// 		if (m_hPE)
		// 		{
		// 			PEdestroy(m_hPE);
		// 			m_hPE = 0; 
		// 		}
		// 		InitGraph();

		//GetColItem( & m_stColItem );
		//종료버튼 눌릴 시
	}
	else if(nID == SC_MAXIMIZE)
	{ 
		//최대화 버튼 눌릴 시
	}
	else if(nID == SC_MINIMIZE)
	{ 
		//최소화 버튼 눌릴 시
	}
	else if(nID == SC_RESTORE) 
	{ 
		//복원 상황에서
	} 

	CDialogEx::OnSysCommand(nID, lParam);
}
