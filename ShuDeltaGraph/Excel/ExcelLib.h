#pragma once

#include <io.h>

enum OPEN_EXCEK_FILE_MODE
{
	CREATE_EXCEL_FILE,
	OPEN_EXCEL_FILE
};

enum DIRECTION
{
	UPPER,
	BOTTOM,
	LEFT,
	RIGHT
};

#define POSITION_BUFFER_SIZE	32

#define SEARCH_ALL				0
#define SEARCH_X				1
#define SEARCH_Y				2

#define MAX_STRING_BUFFER_SIZE	1024

class CExcelLib
{
public:
	CExcelLib(void);
	~CExcelLib(void);

	// �ν��Ͻ� �ʱ�ȭ
	BOOL	InitExcelLib(void);
	// �ν��Ͻ��� �ʱ�ȭ�ϰ� ���� ��ü�� �����Ѵ�.
	BOOL	CreateExcel(void);
	// �ν��Ͻ��� �ʱ�ȭ�ϰ� ������ �����Ѵ�.
	BOOL	OpenExcelFile(TCHAR *wsFileName);
	// ������ �ݰ� �ν��Ͻ��� �����Ѵ�.
	BOOL	CloseExcelFile(void);
	// ������ �����Ѵ�.
	BOOL	SaveExcelFile(void);
	BOOL	SaveAs(TCHAR *pwsFileName);

	// Excel â�� ����.
	BOOL	SetVisible(BOOL fOn=TRUE);

	// Sheet �̸��� ���Ѵ�.
	BOOL	GetSheetName(DWORD dwSheetNumber, TCHAR *pwsSheetName);
	// Sheet �̸��� �����Ѵ�.
	BOOL	ChangeSheetName(DWORD dwSheetNumber, TCHAR *pwsSheetName);
	// Sheet ������ ���Ѵ�.
	DWORD	GetSheetCount(void);
	// Sheet�� �߰��Ѵ�.
	BOOL	AddSheet(TCHAR *pwsSheetName);
	// Sheet�� �����Ѵ�.
	BOOL	SetActiveSheet(INT nSheetNum);

	// Data�� �о�´�.
	BOOL	GetData(DWORD dwX, DWORD dwY, TCHAR *pwsOut, DWORD dwBufferLen);
	BOOL	GetData(INT nSheetNum, DWORD dwX, DWORD dwY, TCHAR *pwsOut, DWORD dwBufferLen);
	// ����ĭ�� �����͸� �д´�. �����Ͱ� ����ִٸ� FALSE ����
	// fDirection: UPPER, BOTTOM, LEFT, RIGHT 
	BOOL	GetNextData(INT fDirection, TCHAR *pwsOut, DWORD dwBufferSize);

	// Data�� ����.
	BOOL	SetData(DWORD dwX, DWORD dwY, TCHAR *pwsInput);
	BOOL	SetData(INT nSheetNum, DWORD dwX, DWORD dwY, TCHAR *pwsOut);
	// ����ĭ�� �����͸� ����.
	// fDirection: UPPER, BOTTOM, LEFT, RIGHT
	BOOL	SetNextData(INT fDirection, TCHAR *pwsInput);

	// �˻�	
	// pwsSeed		: �˻��� �ܾ�
	// pwsResult	: �˻��� ����
	// pdwX, pdwY	: �˻��� ��ġ

	// Active page ��ü�� �˻�
	BOOL	SearchAll(TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);
	// ������ dwX�� dwY ��ǥ�� �˻�
	BOOL	SearchX(DWORD dwX, TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);
	BOOL	SearchY(DWORD dwY, TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);
	// ���� ã��
	BOOL	SearchNext(TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);

	// Active range ���ϱ� 
	BOOL	GetActiveRange(DWORD &pdwBX, DWORD &pdwBY, DWORD &pdwLX, DWORD &pdwLY);

public:
	TCHAR	m_wsExcelFileName[255];

private:
	// Automation �ڵ�ȭ
	HRESULT AutoWrap(INT nAutoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, INT cArgs...);
	HRESULT AutoWrap(int nAutoType, VARIANT *pvResult, IDispatch *pDisp, DISPID dwDispID, INT cArgs...);
	// �ʱ�ȭ ���� Ȯ��
	BOOL	CheckInit(void){return m_fInit;};
	// ��ǥ�� Column ���
	BOOL	XYToColumn(DWORD dwX, DWORD dwY, TCHAR *pwsPosition);
	// Range string($A$1:$G$100)�� ���ڷ� ��ȯ�Ѵ�.
	BOOL	ConverRangeString(char *pbVal, DWORD &pdwBX, DWORD &pdwBY, DWORD &pdwLX, DWORD &pdwLY);
	// ��� ���ڷ� ��ȯ�Ѵ�.(A : 1, AA:27)
	BOOL	ConvertAlphabet2Number(char *pbVal, DWORD &pdwNumber);

private:
	typedef struct _ExcelPosition
	{
		DWORD	dwX;
		DWORD	dwY;
	}ExcelPosition;

	struct PROPERTIES
	{
		IDispatch *pXlApp;
		IDispatch *pXlBooks;
		IDispatch *pXlBook;
		IDispatch *pXlSheets;
		IDispatch *pXlSheet;
	}m_stProperties;

	BOOL			m_fCOMInited;			// COM�� �ʱ�ȭ ����
	BOOL			m_fInit;				// �ν��Ͻ��� �ʱ�ȭ ����
	INT				m_nActiveSheet;			// ���� Active�� Sheet ��ȣ	
	ExcelPosition	m_stLastPosition;		// ���������� ����� Excel ��ġ
	ExcelPosition	m_stLastSearch;			// ���������� �˻��� Excel ��ġ
	TCHAR			m_wsSeed[MAX_STRING_BUFFER_SIZE]; // ���������� �˻��� ����� Seed
	INT				m_fSearchMethod;		// ���������� �˻��� ����� ���
};
