#pragma once


// CDlgColList ��ȭ �����Դϴ�.

class CDlgColList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgColList)

public:
	CDlgColList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgColList();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_COL_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
