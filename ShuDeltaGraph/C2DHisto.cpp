// C2DHisto.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShuDeltaGraph.h"
#include "C2DHisto.h"
#include "afxdialogex.h"


// C2DHisto 대화 상자입니다.

IMPLEMENT_DYNAMIC(C2DHisto, CDialogEx)

C2DHisto::C2DHisto(CWnd* pParent /*=NULL*/)
	: CDialogEx(C2DHisto::IDD, pParent)
{

}

C2DHisto::~C2DHisto()
{
}

void C2DHisto::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(C2DHisto, CDialogEx)
END_MESSAGE_MAP()


// C2DHisto 메시지 처리기입니다.
