#pragma once

#include "./EchoSocket/MyEchoSocket.h"

// CDlgColList ��ȭ �����Դϴ�.

class CDlgColList : public CDialogEx
{
public:
	void InitSocketSetting( enUnitID enUnit );
	void CloseSocketSetting( enUnitID enUnit );
	void Connect( enUnitID enUnit );

	void OnAccept(enUnitID enUnit );
	void OnConnect(int nErrorCode, enUnitID enUnit );
	void OnClose(enUnitID enUnit );
	void OnReceive(enUnitID enUnit );

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
