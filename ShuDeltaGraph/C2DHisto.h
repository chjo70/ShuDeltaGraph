#pragma once


// C2DHisto 대화 상자입니다.

class C2DHisto : public CDialogEx
{
	DECLARE_DYNAMIC(C2DHisto)

public:
	C2DHisto(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~C2DHisto();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_2DHISTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
