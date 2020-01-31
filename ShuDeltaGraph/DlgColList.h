#pragma once

#include "./EchoSocket/MyEchoSocket.h"

// CDlgColList 대화 상자입니다.

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
	CDlgColList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgColList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_COL_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
