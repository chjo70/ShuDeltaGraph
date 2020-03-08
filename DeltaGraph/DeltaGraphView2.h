
// DeltaGraphView.h : CDeltaGraphView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"


class CDeltaGraphView2 : public CFormView
{
private:
	CString m_strPathName;

public:

public:
	inline void SetWindowTitle( CString strWindowTitle ) { m_strPathName=strWindowTitle; }

protected: // serialization������ ��������ϴ�.
	CDeltaGraphView2();
	DECLARE_DYNCREATE(CDeltaGraphView2)

public:
	enum{ IDD = IDD_DELTAGRAPH_FORM2 };

// Ư���Դϴ�.
public:
	CDeltaGraphDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CDeltaGraphView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DeltaGraphView.cpp�� ����� ����
inline CDeltaGraphDoc* CDeltaGraphView2::GetDocument() const
   { return reinterpret_cast<CDeltaGraphDoc*>(m_pDocument); }
#endif

