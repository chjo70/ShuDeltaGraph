
// ShuSIM.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CShuSIMApp:
// �� Ŭ������ ������ ���ؼ��� ShuSIM.cpp�� �����Ͻʽÿ�.
//

class CShuSIMApp : public CWinApp
{
public:
	CShuSIMApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CShuSIMApp theApp;