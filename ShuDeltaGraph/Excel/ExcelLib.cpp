#include "stdafx.h"
#include "ExcelLib.h"

CExcelLib::CExcelLib(void)
{
	HRESULT	hResult;

	// COM 초기화
	hResult = CoInitialize(NULL);
	if(hResult == S_OK)
	{
		m_fCOMInited = TRUE;
	}
	else
	{
		m_fCOMInited = FALSE;
	}

	m_fInit = FALSE;

	memset(&m_stLastPosition, 0, sizeof(m_stLastPosition));
	memset(m_wsExcelFileName, 0, sizeof(m_wsExcelFileName));
	m_nActiveSheet = 0;
}

CExcelLib::~CExcelLib(void)
{
	// COM 해제
	if(m_fCOMInited == TRUE)
	{
		CoUninitialize();
	}
}

/****************************************************************************************************/
/*										INTERFACE FUNCTIONS											*/
/****************************************************************************************************/
// Excel library를 초기화한다.
BOOL CExcelLib::InitExcelLib(void)
{
	CLSID	stClassID;
	VARIANT stVariantResult;
	HRESULT	hResult;

	hResult = CLSIDFromProgID(L"Excel.Application", &stClassID);
	if(FAILED(hResult)) 
	{
		// CLSIDFromProgID()가 실패하였습니다.
		return FALSE;
	}

	// Application 생성
	hResult = CoCreateInstance(stClassID, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&m_stProperties.pXlApp);
	if(FAILED(hResult)) 
	{
		// 엑셀이 등록되어 있지 않습니다.
		return FALSE;
	}      

	// Workbooks 생성
	VariantInit(&stVariantResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &stVariantResult, m_stProperties.pXlApp, L"Workbooks", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	m_stProperties.pXlBooks = stVariantResult.pdispVal;

	m_fInit = TRUE;

	return TRUE;
}

// 인스턴스를 초기화한다.
BOOL CExcelLib::CreateExcel(void)
{
	VARIANT stVariantResult;
	HRESULT hResult;

	hResult = InitExcelLib();
	if(hResult == FALSE)
	{
		return FALSE;
	}

	// Call Workbooks.Add() to get a new workbook...
	VariantInit(&stVariantResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &stVariantResult, m_stProperties.pXlBooks, L"Add", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	m_stProperties.pXlBook = stVariantResult.pdispVal;

	VariantInit(&stVariantResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &stVariantResult, m_stProperties.pXlApp, L"Sheets", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	m_stProperties.pXlSheets = stVariantResult.pdispVal; 


	VariantInit(&stVariantResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &stVariantResult, m_stProperties.pXlApp, L"ActiveSheet", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	m_stProperties.pXlSheet = stVariantResult.pdispVal; 

	return TRUE;
}

// 인스턴스를 초기화한다.
BOOL CExcelLib::OpenExcelFile(TCHAR *pwsFileName)
{
	VARIANT	stVariantResult;
	VARIANT vaPath;
	HRESULT	hResult;
	BOOL	bResult;

	bResult = InitExcelLib();
	if(bResult == FALSE)
	{
		return FALSE;
	}

	if(pwsFileName == NULL)
	{
		// 입력된 파일 이름이 잘못되었습니다.
		return FALSE;
	}

	_bstr_t	strFile = pwsFileName;
	if(_access((char *)strFile, 0))
	{
		return FALSE;
	}

	VariantInit(&stVariantResult);

	_bstr_t str = pwsFileName;

	vaPath.vt		= VT_BSTR;
	vaPath.bstrVal	= ::SysAllocString(str);
	
	hResult = AutoWrap(DISPATCH_METHOD, &stVariantResult, m_stProperties.pXlBooks, L"Open", 1, vaPath);
	if(hResult == FALSE)
	{
		SysFreeString(vaPath.bstrVal);	
		return FALSE;
	}

	m_stProperties.pXlBook = stVariantResult.pdispVal;
	SysFreeString(vaPath.bstrVal);	

	// Sheets 구하기
	VariantInit(&stVariantResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &stVariantResult, m_stProperties.pXlApp, L"Sheets", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	m_stProperties.pXlSheets = stVariantResult.pdispVal; 

	wcscpy_s(m_wsExcelFileName, pwsFileName);
 
	return TRUE;
}

// 파일을 닫고 인스턴스를 종료한다.
BOOL CExcelLib::CloseExcelFile(void)
{
	if(!CheckInit())
	{
		return FALSE;
	}

	AutoWrap(DISPATCH_METHOD, NULL, m_stProperties.pXlApp, L"Quit", 0);

	if(m_stProperties.pXlSheet != NULL)
	{
		m_stProperties.pXlSheet->Release();
	}
	m_stProperties.pXlSheets->Release();
	m_stProperties.pXlBook->Release();
	m_stProperties.pXlBooks->Release();
	m_stProperties.pXlApp->Release();

	return TRUE;
}

// 파일을 저장한다.
BOOL CExcelLib::SaveExcelFile(void)
{

	HRESULT hResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	hResult = AutoWrap(DISPATCH_METHOD, NULL, m_stProperties.pXlApp, L"Save", 0);	
	if(hResult == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CExcelLib::SaveAs(TCHAR *pwsFileName)
{
	_bstr_t strFileName;
	VARIANT vaParam, vaNop;

	strFileName = pwsFileName;

	vaNop.vt = VT_NULL;

	vaParam.vt		= VT_BSTR;
	vaParam.bstrVal	= ::SysAllocString(strFileName);

	AutoWrap(DISPATCH_METHOD, NULL, m_stProperties.pXlBook, L"SaveAs", 9, vaNop, vaNop, vaNop, vaNop, vaNop, vaNop, vaNop, vaNop, vaParam);

	return TRUE;
} 

// Excel 창을 보이게 하거나, 보이지 않게 한다.
BOOL CExcelLib::SetVisible(BOOL fOn)
{
	VARIANT x;
	HRESULT	hResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	x.vt	= VT_I4;
	x.lVal	= fOn ? 1 : 0;

	hResult = AutoWrap(DISPATCH_PROPERTYPUT, NULL, m_stProperties.pXlApp, L"Visible", 1, x);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

// Sheet 갯수를 구한다.
DWORD CExcelLib::GetSheetCount(void)
{
	VARIANT vaResult;
	HRESULT	hResult;

	if(!CheckInit())
	{
		return FALSE;
	}
	
	VariantInit(&vaResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlSheets, 0x76, 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	return vaResult.lVal;
}

// Sheet의 이름을 가져온다.
BOOL CExcelLib::GetSheetName(DWORD dwSheetNumber, TCHAR *pwsSheetName)
{
	VARIANT vaResult;
	DWORD	dwOldSheet;
	HRESULT	hResult;

	dwOldSheet = m_nActiveSheet;

	SetActiveSheet(dwSheetNumber);

	/*
	CString result;
	invokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
	*/
	VariantInit(&vaResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlSheet, 0x6e, 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	memcpy(pwsSheetName, vaResult.pbstrVal, wcslen((wchar_t *)vaResult.pbstrVal)*2);

	SetActiveSheet(dwOldSheet);

	return TRUE;
}

// Sheet이름을 변경한다.
BOOL CExcelLib::ChangeSheetName(DWORD dwSheetNumber, TCHAR *pwsSheetName)
{
	VARIANT vaResult;
	HRESULT	hResult;
	DWORD	dwOldSheet;

	dwOldSheet = m_nActiveSheet;

	SetActiveSheet(dwSheetNumber);

	//Select the sheet for changing the name
	VariantInit(&vaResult);
	hResult = AutoWrap(DISPATCH_METHOD, &vaResult, m_stProperties.pXlSheet, L"Select", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	//Change the name of sheet
	VariantInit(&vaResult);
	hResult = AutoWrap(DISPATCH_PROPERTYPUT, &vaResult, m_stProperties.pXlSheet, L"name", 1, COleVariant(pwsSheetName));
	if(hResult == FALSE)
	{
		return FALSE;
	}

	SetActiveSheet(dwOldSheet);

	return TRUE;
}

// Sheet를 추가한다.
BOOL CExcelLib::AddSheet(TCHAR *pwsSheetName)
{
	VARIANT vaResult;
	HRESULT	hResult;

	VariantInit(&vaResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlSheets, L"Add", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	// 새로 추가한 Sheet를 현재의 Sheet로 한다.
	m_stProperties.pXlSheet = vaResult.pdispVal;

	//Select the sheet for changing the name
	VariantInit(&vaResult);
	hResult = AutoWrap(DISPATCH_METHOD, &vaResult, m_stProperties.pXlSheet, L"Select", 0);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	//Change the name of sheet  to GEO
	VariantInit(&vaResult);
	hResult = AutoWrap(DISPATCH_PROPERTYPUT, &vaResult, m_stProperties.pXlSheet, L"name", 1, COleVariant(pwsSheetName));
	if(hResult == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

// Sheet을 선택한다.
BOOL CExcelLib::SetActiveSheet(INT nSheetNum)
{
	HRESULT	hResult;
	VARIANT vaResult;
	VARIANT vaSheet;
	
	if(!CheckInit())
	{
		return FALSE;
	}

	VariantInit(&vaResult);

	vaSheet.vt		= VT_I4;
	vaSheet.lVal	= nSheetNum;

	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlApp, L"Worksheets", 1, vaSheet);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	m_stProperties.pXlSheet = vaResult.pdispVal;

	m_nActiveSheet = nSheetNum;

	return TRUE;
}

// 원하는 칼럼에서 데이터를 읽어온다.
BOOL CExcelLib::GetData(DWORD dwX, DWORD dwY, TCHAR *pwsOut, DWORD dwBufferSize)
{
	_bstr_t strPosition;
	_bstr_t	strData;
	TCHAR	wsColumn[36];
	HRESULT	hResult;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	if(pwsOut == NULL)
	{
		return FALSE;
	}

	memset(wsColumn, 0, 36);
	bResult = XYToColumn(dwX, dwY, wsColumn);
	if(bResult == FALSE)
	{
		return FALSE;
	}

	// XN ~ XN
	strPosition	=	wsColumn;
	strPosition	+=	":";
	strPosition	+=	wsColumn;
	
	IDispatch *pXlRange;
	{
		VARIANT vaParam;
		VARIANT vaResult;

		vaParam.vt		= VT_BSTR;
		vaParam.bstrVal	= ::SysAllocString(strPosition);
		
		VariantInit(&vaResult);
		hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlSheet, L"Range", 1, vaParam);
		if(hResult == FALSE)
		{
			return FALSE;
		}
		SysFreeString(vaParam.bstrVal);
		VariantClear(&vaParam);
		pXlRange = vaResult.pdispVal;
	}
	
	VARIANT vaTmp;

	vaTmp.vt=VT_BSTR;
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaTmp, pXlRange, L"Value", 0, 0);
	if(hResult == FALSE)
	{
		pXlRange->Release();
		return FALSE;
	}

	if(vaTmp.vt==VT_EMPTY)
	{
		pwsOut[0]= L'\0';
	}
	else
	{
		VariantChangeType(&vaTmp, &vaTmp, VARIANT_NOUSEROVERRIDE, VT_BSTR);
		strData = vaTmp.bstrVal;
		memset( pwsOut, 0, sizeof(TCHAR) * dwBufferSize );
		wcsncpy(pwsOut, strData, strData.length());
	}

	pXlRange->Release();

	m_stLastPosition.dwX = dwX;
	m_stLastPosition.dwY = dwY;

	return TRUE;
}


// 원하는 Sheet와 칼럼에서 데이터를 읽어온다.
BOOL CExcelLib::GetData(INT nSheetNum, DWORD dwX, DWORD dwY, TCHAR *pwsOut, DWORD dwBufferSize)
{
	VARIANT vaResult;
	VARIANT vaSheet;
	HRESULT	hResult;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	VariantInit(&vaResult);

	vaSheet.vt		= VT_I4;
	vaSheet.lVal	= nSheetNum;

	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlApp, L"Worksheets", 1, vaSheet);
	if(hResult == FALSE)
	{
		return FALSE;
	}

	m_stProperties.pXlSheet = vaResult.pdispVal;

	bResult = GetData(dwX, dwY, pwsOut, dwBufferSize);

	VariantInit(&vaResult);

	vaSheet.vt		= VT_I4;
	vaSheet.lVal	= m_nActiveSheet;

	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlApp, L"Worksheets", 1, vaSheet);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	
	m_stProperties.pXlSheet = vaResult.pdispVal;

	if(bResult == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

// 옆칸의 데이터를 읽는다. 데이터가 비어있다면 FALSE 리턴
BOOL CExcelLib::GetNextData(INT fDirection, TCHAR *pwsOut, DWORD dwBufferSize)
{
	if(!CheckInit())
	{
		return FALSE;
	}

	switch(fDirection)
	{
	case UPPER:
		if(m_stLastPosition.dwY == 0)
		{
			return FALSE;
		}
		m_stLastPosition.dwY --;
		break;

	case BOTTOM:
		m_stLastPosition.dwY ++;
		break;

	case LEFT:
		if(m_stLastPosition.dwX == 0)
		{
			return FALSE;
		}
		m_stLastPosition.dwX --;
		break;

	case RIGHT:
		m_stLastPosition.dwX ++;
		break;

	default:
		return FALSE;
	}

	return GetData(m_stLastPosition.dwX, m_stLastPosition.dwY, pwsOut, dwBufferSize); 
}

// Data를 쓴다.
BOOL CExcelLib::SetData(DWORD dwX, DWORD dwY, TCHAR *pwsInput)
{
	_bstr_t strPosition;
	_bstr_t	strData;
	TCHAR	wsColumn[36];
	HRESULT	hResult;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	if(pwsInput == NULL)
	{
		return FALSE;
	}

	// Range 설정
	memset(wsColumn, 0, sizeof(wsColumn));
	bResult = XYToColumn(dwX, dwY, wsColumn);
	if(bResult == FALSE)
	{
		return FALSE;
	}

	// XN ~ XN
	strPosition	=	wsColumn;
	strPosition	+=	":";
	strPosition	+=	wsColumn;
	
	IDispatch *pXlRange;
	{
		VARIANT vaParam;
		VARIANT vaResult;

		vaParam.vt		= VT_BSTR;
		vaParam.bstrVal	= ::SysAllocString(strPosition);
		
		VariantInit(&vaResult);
		hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlSheet, L"Range", 1, vaParam);
		if(hResult == FALSE)
		{
			return FALSE;
		}
		SysFreeString(vaParam.bstrVal);
		VariantClear(&vaParam);
		pXlRange = vaResult.pdispVal;
	}

	// 값 쓰기
	VARIANT		vaInputData;
	_bstr_t		strInputData;

	strInputData		= pwsInput;
	vaInputData.vt		= VT_BSTR;
	vaInputData.bstrVal	= ::SysAllocString(strInputData);

	hResult = AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlRange, L"Value", 1, vaInputData);
	if(hResult == FALSE)
	{
		pXlRange->Release();	
		return FALSE;
	}
	SysFreeString(vaInputData.bstrVal);

	pXlRange->Release();	

	return TRUE;
}

BOOL CExcelLib::SetData(INT nSheetNum, DWORD dwX, DWORD dwY, TCHAR *pwsOut)
{
	VARIANT vaResult;
	VARIANT vaSheet;
	HRESULT	hResult;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	VariantInit(&vaResult);

	vaSheet.vt		= VT_I4;
	vaSheet.lVal	= nSheetNum;

	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlApp, L"Worksheets", 1, vaSheet);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	
	m_stProperties.pXlSheet = vaResult.pdispVal;

	bResult = SetData(dwX, dwY, pwsOut);

	VariantInit(&vaResult);

	vaSheet.vt		= VT_I4;
	vaSheet.lVal	= m_nActiveSheet;

	hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlApp, L"Worksheets", 1, vaSheet);
	if(hResult == FALSE)
	{
		return FALSE;
	}
	
	m_stProperties.pXlSheet = vaResult.pdispVal;

	if(bResult == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

// 다음칸에 데이터를 쓴다.
BOOL CExcelLib::SetNextData(INT fDirection, TCHAR *pwsInput)
{
	if(!CheckInit())
	{
		return FALSE;
	}

	switch(fDirection)
	{
	case UPPER:
		if(m_stLastPosition.dwY == 0)
		{
			return FALSE;
		}
		m_stLastPosition.dwY --;
		break;

	case BOTTOM:
		m_stLastPosition.dwY ++;
		break;

	case LEFT:
		if(m_stLastPosition.dwX == 0)
		{
			return FALSE;
		}
		m_stLastPosition.dwX --;
		break;

	case RIGHT:
		m_stLastPosition.dwX ++;
		break;

	default:
		return FALSE;
	}

	return SetData(m_stLastPosition.dwX, m_stLastPosition.dwY, pwsInput); 
}

// Active page 전체를 검색
BOOL CExcelLib::SearchAll(TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY)
{
	TCHAR	wsBuffer[MAX_STRING_BUFFER_SIZE];
	DWORD	dwBX, dwBY, dwLX, dwLY;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	// 값 멤버 변수로 복사
	memset(m_wsSeed, 0, sizeof(m_wsSeed));
	wcscpy(m_wsSeed, pwsSeed);
	m_stLastSearch.dwX = 0;
	m_stLastSearch.dwY = 0;

	// Active Range 받기
	bResult = GetActiveRange(dwBX, dwBY, dwLX, dwLY);
	if(bResult == FALSE)
	{
		return FALSE;
	}

	for(DWORD j=dwBY; j<=dwLY; j++)
	{
		for(DWORD i=dwBX; i<=dwLX; i++)
		{
			memset(wsBuffer, 0, sizeof(wsBuffer));
			bResult = GetData(i, j, wsBuffer, MAX_STRING_BUFFER_SIZE);

			if(bResult == FALSE)
			{
				return FALSE;
			}

			if(!lstrcmpi(wsBuffer, m_wsSeed) != NULL)
			{
				pdwX = i;
				pdwY = j;
				if(pwsResult != NULL)
				{
					wcscpy(pwsResult, wsBuffer);	
				}
				m_stLastSearch.dwX = i;
				m_stLastSearch.dwY = j;

				m_fSearchMethod = SEARCH_ALL;

				return TRUE;
			}
		}
	}

	return FALSE;
}


// 지정된 dwX나 dwY 좌표를 검색
BOOL CExcelLib::SearchX(DWORD dwX, TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY)
{
	TCHAR	wsBuffer[MAX_STRING_BUFFER_SIZE];
	DWORD	dwBX, dwBY, dwLX, dwLY;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	// 값 멤버 변수로 복사
	memset(m_wsSeed, 0, sizeof(m_wsSeed));
	wcscpy(m_wsSeed, pwsSeed);
	m_stLastSearch.dwX = 0;
	m_stLastSearch.dwY = 0;

	// Active Range 받기
	bResult = GetActiveRange(dwBX, dwBY, dwLX, dwLY);
	if(bResult == FALSE)
	{
		return FALSE;
	}

	for(DWORD j=dwBY; j<=dwLY; j++)
	{
		memset(wsBuffer, 0, MAX_STRING_BUFFER_SIZE);
		bResult = GetData(dwX, j, wsBuffer, MAX_STRING_BUFFER_SIZE);
		if(bResult == FALSE)
		{
			return FALSE;
		}
		
		if(!lstrcmpi(wsBuffer, m_wsSeed))
		{
			pdwX = dwX;
			pdwY = j;
			wcscpy(pwsResult, wsBuffer);

			m_stLastSearch.dwX = dwX;
			m_stLastSearch.dwY = j;

			m_fSearchMethod = SEARCH_X;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CExcelLib::SearchY(DWORD dwY, TCHAR *pwsSeed, TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY)
{
	TCHAR	wsBuffer[MAX_STRING_BUFFER_SIZE];
	DWORD	dwBX, dwBY, dwLX, dwLY;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	// 값 멤버 변수로 복사
	memset(m_wsSeed, 0, sizeof(m_wsSeed));
	wcscpy(m_wsSeed, pwsSeed);
	m_stLastSearch.dwX = 0;
	m_stLastSearch.dwY = 0;

	// Active Range 받기
	bResult = GetActiveRange(dwBX, dwBY, dwLX, dwLY);
	if(bResult == FALSE)
	{
		return FALSE;
	}

	for(DWORD j=dwBX; j<=dwLX; j++)
	{
		bResult = GetData(j, dwY, wsBuffer, MAX_STRING_BUFFER_SIZE);
		if(bResult == FALSE)
		{
			return FALSE;
		}
		
		if(!lstrcmpi(wsBuffer, m_wsSeed) != NULL)
		{
			pdwX = j;
			pdwY = dwY;
			wcscpy(pwsResult, wsBuffer);

			m_stLastSearch.dwX = j;
			m_stLastSearch.dwY = dwY;

			m_fSearchMethod = SEARCH_Y;

			return TRUE;
		}
	}

	return FALSE;
}

// 다음 찾기
BOOL CExcelLib::SearchNext(TCHAR *pwsResult, DWORD &pdwX, DWORD &pdwY)
{
	TCHAR	wsBuffer[MAX_STRING_BUFFER_SIZE];
	DWORD	dwBX, dwBY, dwLX, dwLY;
	BOOL	bResult;

	if(!CheckInit())
	{
		return FALSE;
	}

	// Active Range 받기
	bResult = GetActiveRange(dwBX, dwBY, dwLX, dwLY);
	if(bResult == FALSE)
	{
		return FALSE;
	}

	if(m_fSearchMethod == SEARCH_X)
	{
		dwLX = m_stLastSearch.dwX;
		m_stLastSearch.dwY++;
	}
	else if(m_fSearchMethod == SEARCH_Y)
	{
		dwLY = m_stLastSearch.dwY;
		m_stLastSearch.dwX++;
	}
	else
	{
		m_stLastSearch.dwY++;
	}

	for(DWORD i=m_stLastSearch.dwX; i<=dwLX; i++)
	{
		for(DWORD j=m_stLastSearch.dwY; j<=dwLY; j++)
		{
			bResult = GetData(i, j, wsBuffer, MAX_STRING_BUFFER_SIZE);
			if(bResult == FALSE)
			{
				return FALSE;
			}
			
			if(wcswcs(wsBuffer, m_wsSeed) != NULL)
			{
				pdwX = i;
				pdwY = j;
				wcscpy(pwsResult, wsBuffer);

				m_stLastSearch.dwX = i;
				m_stLastSearch.dwY = j;
				return TRUE;
			}
		}
		
		if(m_fSearchMethod == SEARCH_Y)
		{
			dwLY = m_stLastSearch.dwY;
		}
		else
		{
			m_stLastSearch.dwY = 1;
		}
	}

	return FALSE;
}


/****************************************************************************************************/
/*										INTERNAL FUNCTIONS											*/
/****************************************************************************************************/
BOOL CExcelLib::XYToColumn(DWORD dwX, DWORD dwY, TCHAR *pwsPosition)
{
	TCHAR	wsPosition[POSITION_BUFFER_SIZE];
	DWORD	dwMod;
	DWORD	dwNumber;
	int		nCurrent;

	if(dwX < 0 || dwY < 0)
	{
		return FALSE;
	}

	memset(wsPosition, 0, sizeof(wsPosition));
	dwNumber = dwX;
	nCurrent = 0;
	
	while(dwNumber > 0)
	{
		dwMod = dwNumber % 26;

		if(dwMod == 0)
		{
			dwMod = 26;
		}

		wsPosition[nCurrent] = (TCHAR)('A' + dwMod - 1);

		dwNumber /= 26;
		nCurrent++;
	}

	_wcsrev(wsPosition);

	swprintf(pwsPosition, L"%s%u", wsPosition, dwY);

	return TRUE;
}


// Active range 구하기 
BOOL CExcelLib::GetActiveRange(DWORD &pdwBX, DWORD &pdwBY, DWORD &pdwLX, DWORD &pdwLY)
{
	VARIANT vaResult;
	HRESULT	hResult;
	BOOL	bResult;

	// Active range 구하기
	IDispatch *pXlRange;
	{
		VariantInit(&vaResult);
		hResult = AutoWrap(DISPATCH_PROPERTYGET, &vaResult, m_stProperties.pXlSheet, 0x19c/*UsedRange*/, 0);
		if(hResult == FALSE)
		{
			return FALSE;
		}
		pXlRange = vaResult.pdispVal;
	}

	// Range address 구하기
	VARIANT stVariantResult;
	VariantInit(&stVariantResult);
	hResult = AutoWrap(DISPATCH_PROPERTYGET, &stVariantResult, pXlRange, 0xec/*GetAddress*/, 0);
	if(hResult == FALSE)
	{
		pXlRange->Release();	
		return FALSE;
	}

	_bstr_t	strData = stVariantResult.bstrVal;

	//Format : $A$1:$G$17
	bResult = ConverRangeString((char *)strData, pdwBX, pdwBY, pdwLX, pdwLY);
	if(bResult == FALSE)
	{	
		pXlRange->Release();	
		return FALSE;
	}

	if(pdwLX > 26)
	{
		pdwLX = 26;
	}
	if(pdwLY > 3000)
	{
		pdwLY = 3000;	
	}

	pXlRange->Release();	

	return TRUE;
}

// Range string($A$1:$G$100)을 숫자로 변환한다.
BOOL CExcelLib::ConverRangeString(char *pbVal, DWORD &pdwBX, DWORD &pdwBY, DWORD &pdwLX, DWORD &pdwLY)
{
	char szBuffer[16];
	char *pbIndex;

	pbIndex = pbVal;

	if(*pbIndex++ != '$')
	{
		return FALSE;
	}

	// 시작 X 좌표
	memset(szBuffer, 0, sizeof(szBuffer));
	for(int i=0; i<sizeof(szBuffer); i++)
	{
		if(*pbIndex == '$')
		{
			pbIndex++;
			break;
		}
		
		szBuffer[i] = *pbIndex;

		pbIndex++;
	}

	ConvertAlphabet2Number(szBuffer, pdwBX);

	// 시작 Y 좌표
	memset(szBuffer, 0, sizeof(szBuffer));
	for(int i=0; i<sizeof(szBuffer); i++)
	{
		if(*pbIndex == ':')
		{
			pbIndex++;
			break;
		}
		
		szBuffer[i] = *pbIndex;

		pbIndex++;
	}

	pdwBY = atoi(szBuffer);

	if(*pbIndex++ != '$')
	{
		return FALSE;
	}

	// 끝 X 좌표
	memset(szBuffer, 0, sizeof(szBuffer));
	for(int i=0; i<sizeof(szBuffer); i++)
	{
		if(*pbIndex == '$')
		{
			pbIndex++;
			break;
		}
		
		szBuffer[i] = *pbIndex;

		pbIndex++;
	}

	ConvertAlphabet2Number(szBuffer, pdwLX);

	// 끝 Y 좌표
	memset(szBuffer, 0, sizeof(szBuffer));
	for(int i=0; i<sizeof(szBuffer); i++)
	{
		if(*pbIndex == 0)
		{
			pbIndex++;
			break;
		}
		
		szBuffer[i] = *pbIndex;

		pbIndex++;
	}

	pdwLY = atoi(szBuffer);


	return TRUE;
}

// 영어를 숫자로 변환한다.(A : 1, AA:27)
BOOL CExcelLib::ConvertAlphabet2Number(char *pbVal, DWORD &pdwNumber)
{
	DWORD	dwIndex;
	DWORD	dwResult;
	DWORD	dwTmp;
	char	*pbIndex;


	for(int i=0; i<16; i++)
	{
		if(pbVal[i] == 0)
		{
			dwIndex = i;
			break;
		}
	}

	dwResult	= 0;
	pbIndex		= pbVal;
	for(int i = dwIndex; i > 0; i--)
	{
		dwTmp = (*pbIndex) - 'A' + 1;
		for(int j=i; j>1; j--)
		{
			dwTmp *= 26;
		}

		dwResult += dwTmp;

		*pbIndex ++;
	}

	pdwNumber = dwResult;

	return TRUE;
}

// Call excel automation
HRESULT CExcelLib::AutoWrap(int nAutoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs...) 
{
    va_list vlMarker;

    va_start(vlMarker, cArgs);
    if(pDisp == NULL) 
	{
      //  PrintOut(L"NULL IDispatch passed to AutoWrap()");
        return FALSE;
    }

    DISPPARAMS	stDISP			= {NULL, NULL, 0, 0};
    DISPID		dwDispName		= DISPID_PROPERTYPUT;
    DISPID		dwDispID;
    HRESULT		hResult;
    char		szName[200];

    WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);
    
    hResult = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dwDispID);
    if(FAILED(hResult)) 
	{
       // PrintOut(L"IDispatch::GetIDsOfNames(\"%s\") 실패 w/err 0x%08lx", szName, hResult);
        return FALSE;
    }

    VARIANT *pArgs = new VARIANT[cArgs+1];
    for(int i=0; i<cArgs; i++) 
	{
        pArgs[i] = va_arg(vlMarker, VARIANT);
    }
    
    stDISP.cArgs	= cArgs;
    stDISP.rgvarg	= pArgs;
    
    if(nAutoType & DISPATCH_PROPERTYPUT)
	{
        stDISP.cNamedArgs			= 1;
        stDISP.rgdispidNamedArgs	= &dwDispName;
    }
    
    hResult = pDisp->Invoke(dwDispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, nAutoType, &stDISP, pvResult, NULL, NULL);
    if(FAILED(hResult))
	{
		//PrintOut(L"IDispatch::Invoke(\"%s\"=%08lx) 실패 w/err 0x%08lx", szName, hResult);
		switch(hResult)
		{
		case DISP_E_BADPARAMCOUNT:
			//PrintOut(L"DISP_E_BADPARAMCOUNT"); 
			break;
		case DISP_E_BADVARTYPE:
			//PrintOut(L"DISP_E_BADVARTYPE"); 
			break;
		case DISP_E_EXCEPTION:
			//PrintOut(L"DISP_E_EXCEPTION"); 
			break;
		case DISP_E_MEMBERNOTFOUND:
			//PrintOut(L"DISP_E_MEMBERNOTFOUND");
			break;
		case DISP_E_NONAMEDARGS:
			//PrintOut(L"DISP_E_NONAMEDARGS"); 
			break;
		case DISP_E_OVERFLOW:
			//PrintOut(L"DISP_E_OVERFLOW"); 
			break;
		case DISP_E_PARAMNOTFOUND:
			//PrintOut(L"DISP_E_PARAMNOTFOUND"); 
			break;
		case DISP_E_TYPEMISMATCH:
			//PrintOut(L"DISP_E_TYPEMISMATCH"); 
			break;
		case DISP_E_UNKNOWNINTERFACE:
			//PrintOut(L"DISP_E_UNKNOWNINTERFACE");
			break;
		case DISP_E_UNKNOWNLCID:
			//PrintOut(L"DISP_E_UNKNOWNLCID"); 
			break;
		case DISP_E_PARAMNOTOPTIONAL:
			//PrintOut(L"DISP_E_PARAMNOTOPTIONAL");
			break;
		}

		return FALSE;
	}

    va_end(vlMarker);
    
    delete [] pArgs;

    return TRUE;
}

// Call excel automation
HRESULT CExcelLib::AutoWrap(int nAutoType, VARIANT *pvResult, IDispatch *pDisp, DISPID dwDispID, int cArgs...) 
{
    va_list vlMarker;

    va_start(vlMarker, cArgs);
    if(pDisp == NULL) 
	{
        // "NULL IDispatch passed to AutoWrap()"
        return FALSE;
    }

    DISPPARAMS	stDISP			= {NULL, NULL, 0, 0};
    DISPID		dwDispName		= DISPID_PROPERTYPUT;
    HRESULT		hResult;

    VARIANT *pArgs = new VARIANT[cArgs+1];
    for(int i=0; i<cArgs; i++) 
	{
        pArgs[i] = va_arg(vlMarker, VARIANT);
    }
    
    stDISP.cArgs	= cArgs;
    stDISP.rgvarg	= pArgs;
    
    if(nAutoType & DISPATCH_PROPERTYPUT)
	{
        stDISP.cNamedArgs			= 1;
        stDISP.rgdispidNamedArgs	= &dwDispName;
    }
    
    hResult = pDisp->Invoke(dwDispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, nAutoType, &stDISP, pvResult, NULL, NULL);
    if(FAILED(hResult))
	{
		// PrintOut(L"IDispatch::Invoke(\"%u\"=%08lx) 실패 w/err 0x%08lx", dwDispID, hResult);
		switch(hResult)
		{
		case DISP_E_BADPARAMCOUNT:
			// PrintOut(L"DISP_E_BADPARAMCOUNT"); 
			break;
		case DISP_E_BADVARTYPE:
			// PrintOut(L"DISP_E_BADVARTYPE"); 
			break;
		case DISP_E_EXCEPTION:
			// PrintOut(L"DISP_E_EXCEPTION"); 
			break;
		case DISP_E_MEMBERNOTFOUND:
			// PrintOut(L"DISP_E_MEMBERNOTFOUND");
			break;
		case DISP_E_NONAMEDARGS:
			// PrintOut(L"DISP_E_NONAMEDARGS"); 
			break;
		case DISP_E_OVERFLOW:
			// PrintOut(L"DISP_E_OVERFLOW"); 
			break;
		case DISP_E_PARAMNOTFOUND:
			// PrintOut(L"DISP_E_PARAMNOTFOUND"); 
			break;
		case DISP_E_TYPEMISMATCH:
			// PrintOut(L"DISP_E_TYPEMISMATCH"); 
			break;
		case DISP_E_UNKNOWNINTERFACE:
			// PrintOut(L"DISP_E_UNKNOWNINTERFACE");
			break;
		case DISP_E_UNKNOWNLCID:
			// PrintOut(L"DISP_E_UNKNOWNLCID"); 
			break;
		case DISP_E_PARAMNOTOPTIONAL:
			// PrintOut(L"DISP_E_PARAMNOTOPTIONAL");
			break;
		}

		return FALSE;
	}

    va_end(vlMarker);
    
    delete [] pArgs;
    
    return TRUE;
}