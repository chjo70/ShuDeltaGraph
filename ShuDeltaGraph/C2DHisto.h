#pragma once


// C2DHisto ��ȭ �����Դϴ�.

class C2DHisto : public CDialogEx
{
	DECLARE_DYNAMIC(C2DHisto)

public:
	C2DHisto(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~C2DHisto();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_2DHISTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
