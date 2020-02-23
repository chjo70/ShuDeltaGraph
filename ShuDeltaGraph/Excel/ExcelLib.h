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

	// 인스턴스 초기화
	BOOL	InitExcelLib(void);
	// 인스턴스를 초기화하고 엑셀 객체를 생성한다.
	BOOL	CreateExcel(void);
	// 인스턴스를 초기화하고 파일을 오픈한다.
	BOOL	OpenExcelFile(TCHAR *wsFileName);
	// 파일을 닫고 인스턴스를 종료한다.
	BOOL	CloseExcelFile(void);
	// 파일을 저장한다.
	BOOL	SaveExcelFile(void);
	BOOL	SaveAs(TCHAR *pwsFileName);

	// Excel 창을 띄운다.
	BOOL	SetVisible(BOOL fOn=TRUE);

	// Sheet 이름을 구한다.
	BOOL	GetSheetName(DWORD dwSheetNumber, TCHAR *pwsSheetName);
	// Sheet 이름을 변경한다.
	BOOL	ChangeSheetName(DWORD dwSheetNumber, TCHAR *pwsSheetName);
	// Sheet 갯수를 구한다.
	DWORD	GetSheetCount(void);
	// Sheet를 추가한다.
	BOOL	AddSheet(TCHAR *pwsSheetName);
	// Sheet을 선택한다.
	BOOL	SetActiveSheet(INT nSheetNum);

	// Data를 읽어온다.
	BOOL	GetData(DWORD dwX, DWORD dwY, TCHAR *pwsOut, DWORD dwBufferLen);
	BOOL	GetData(INT nSheetNum, DWORD dwX, DWORD dwY, TCHAR *pwsOut, DWORD dwBufferLen);
	// 다음칸의 데이터를 읽는다. 데이터가 비어있다면 FALSE 리턴
	// fDirection: UPPER, BOTTOM, LEFT, RIGHT 
	BOOL	GetNextData(INT fDirection, TCHAR *pwsOut, DWORD dwBufferSize);

	// Data를 쓴다.
	BOOL	SetData(DWORD dwX, DWORD dwY, TCHAR *pwsInput);
	BOOL	SetData(INT nSheetNum, DWORD dwX, DWORD dwY, TCHAR *pwsOut);
	// 다음칸에 데이터를 쓴다.
	// fDirection: UPPER, BOTTOM, LEFT, RIGHT
	BOOL	SetNextData(INT fDirection, TCHAR *pwsInput);

	// 검색	
	// pwsSeed		: 검색할 단어
	// pwsResult	: 검색된 문장
	// pdwX, pdwY	: 검색된 위치

	// Active page 전체를 검색
	BOOL	SearchAll(TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);
	// 지정된 dwX나 dwY 좌표를 검색
	BOOL	SearchX(DWORD dwX, TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);
	BOOL	SearchY(DWORD dwY, TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);
	// 다음 찾기
	BOOL	SearchNext(TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY);

	// Active range 구하기 
	BOOL	GetActiveRange(DWORD &pdwBX, DWORD &pdwBY, DWORD &pdwLX, DWORD &pdwLY);

public:
	TCHAR	m_wsExcelFileName[255];

private:
	// Automation 자동화
	HRESULT AutoWrap(INT nAutoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, INT cArgs...);
	HRESULT AutoWrap(int nAutoType, VARIANT *pvResult, IDispatch *pDisp, DISPID dwDispID, INT cArgs...);
	// 초기화 여부 확인
	BOOL	CheckInit(void){return m_fInit;};
	// 좌표로 Column 계산
	BOOL	XYToColumn(DWORD dwX, DWORD dwY, TCHAR *pwsPosition);
	// Range string($A$1:$G$100)을 숫자로 변환한다.
	BOOL	ConverRangeString(char *pbVal, DWORD &pdwBX, DWORD &pdwBY, DWORD &pdwLX, DWORD &pdwLY);
	// 영어를 숫자로 변환한다.(A : 1, AA:27)
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

	BOOL			m_fCOMInited;			// COM의 초기화 여부
	BOOL			m_fInit;				// 인스턴스의 초기화 여부
	INT				m_nActiveSheet;			// 현재 Active된 Sheet 번호	
	ExcelPosition	m_stLastPosition;		// 마지막으로 사용한 Excel 위치
	ExcelPosition	m_stLastSearch;			// 마지막으로 검색한 Excel 위치
	TCHAR			m_wsSeed[MAX_STRING_BUFFER_SIZE]; // 마지막으로 검색에 사용한 Seed
	INT				m_fSearchMethod;		// 마지막으로 검색에 사용한 방식
};
