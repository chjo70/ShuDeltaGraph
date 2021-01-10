#pragma once

/**
 * [식별자 : CLS-GMU-GR-U-XSA]
 *
 * [추적성 : SRS-G-SOFR-001]
 *
 * @class    CXlSimpleAutomation
 * @brief  자동화 클래스
 *
 * (1) 클래스 설명
 * -    자동화 클래스
 *
 * (2) 설계결정사항
 * - 해당사항없음
 *
 * (3) 제한 및 예외처리
 * - 해당사항없음
 */
class CXlSimpleAutomation
{
#define MAX_DISP_ARGS			10
#define DISPARG_NOFREEVARIANT	0x01
#define DISP_FREEARGS			0x02
#define DISP_NOSHOWEXCEPTIONS	0x04
#define xlWorksheet				-4167

// Constructor
public:
	/**
	* @brief	생성자
	* @param	i_lpszFileName	LPCTSTR 형의 데이터
	* @param	i_bVisible	BOOL 형의 데이터
	* @return	
	*/
	CXlSimpleAutomation(LPCTSTR i_lpszFileName, BOOL i_bVisible = FALSE);
	/**
	* @brief	소멸자
	* @param		void 형의 데이터
	* @return	
	*/
	virtual ~CXlSimpleAutomation(void);

// Operations:
public:
	enum XlFormat 
	{ 
		XLS = 56,	// 97 ~ 2003
		XLSB = 50,	// 
		XLSX = 51,  // open xml
		XLSM = 52
	};

	/**
	* @brief	파일로 저장
	* @param	i_lpszFileName	LPCTSTR 형의 데이터
	* @param	i_format	XlFormat 형의 데이터
	* @return	BOOL
	*/
	BOOL SaveAsFile(LPCTSTR i_lpszFileName, XlFormat i_format = XLSX);
	/**
	* @brief	파일로 저장
	* @param	i_lpszFileName	LPCTSTR 형의 데이터
	* @param	i_strFileExt	CString 형의 데이터
	* @return	BOOL
	*/
	BOOL SaveAsFile( LPCTSTR i_lpszFileName, CString i_strFileExt );
#ifdef SSDLLMFC_H
	/**
	* @brief	시트 값 설정
	* @param	i_pFpSpread	TSpread * 형의 데이터
	* @param	i_nBeginRow	int 형의 데이터
	* @param	i_nBeginCol	int 형의 데이터
	* @return	BOOL
	*/
	BOOL SetSheetValues(TSpread* i_pFpSpread, int i_nBeginRow = 1,int i_nBeginCol =0);
	/**
	* @brief	시트값 반환
	* @param	i_pFpSpread	TSpread * 형의 데이터
	* @param	i_nBeginRow	int 형의 데이터
	* @return	BOOL
	*/
	BOOL GetSheetValues(TSpread* i_pFpSpread, int i_nBeginRow = 1);
	/**
	* @brief	안전 배열 데이터 반환
	* @param	i_plistMissionListData	list<SGRMissionInfo> * 형의 데이터
	* @param	i_nBeginRow	int 형의 데이터
	* @return	BOOL
	*/
	BOOL GetSafeArrayData(list<SGRMissionInfo>* i_plistMissionListData, int i_nBeginRow = 1 );		// 체계운용에서만사용
	/**
	* @brief	시간 데이터 설정
	* @param	i_pTimeStruct	SGRTimeInfo * 형의 데이터
	* @param	i_strTimeInfo	CString 형의 데이터
	* @return	BOOL
	*/
	BOOL SetTimeData(SGRTimeInfo* i_pTimeStruct, CString i_strTimeInfo);										// 체계운용에서만사용
#endif
	//BOOL SetBitStreamValues(CBitStreamWnd* i_pwndBitStream);
	//BOOL GetBitStreamValues(CBitStreamWnd* i_pwndBitStream);

	/**
	* @brief	셀 값 설정
	* @param	i_unColumn	UINT 형의 데이터
	* @param	i_unRow	UINT 형의 데이터
	* @param	i_strValue	CString & 형의 데이터
	* @return	BOOL
	*/
	BOOL SetCellValue(UINT i_unColumn, UINT i_unRow, CString& i_strValue);
	/**
	* @brief	셀 값 반환
	* @param	i_unColumn	UINT 형의 데이터
	* @param	i_unRow	UINT 형의 데이터
	* @param	i_strValue	CString & 형의 데이터
	* @return	BOOL
	*/
	BOOL GetCellValue(UINT i_unColumn, UINT i_unRow, CString& i_strValue);

	/**
	* @brief	열 수 반환
	* @return	long
	*/
	long GetRowNum();
	/**
	* @brief	행 수 반환
	* @return	long
	*/
	long GetColumnNum();

protected:
	BOOL CreateNewWorkbook();
	BOOL OpenWorkbook(LPCTSTR i_lpszFileName);
	BOOL GetActiveSheet();

	BOOL ExlInvoke(IDispatch* i_pdisp, LPOLESTR i_szName, 
		VARIANTARG* i_pvargResult, WORD i_wInvokeAction, WORD i_wFlags);
	void ShowException(LPOLESTR i_szName, HRESULT i_hResult, 
		EXCEPINFO* i_pExcep, unsigned int i_unArgErr);

	BOOL StartExcelInstance(BOOL i_bVisible);
	void ReleaseDispatch();
	BOOL TerminateExcelInstance();
	CString ConvertIndexToName(UINT i_unIndex);

#ifdef SSDLLMFC_H	
	BOOL AddArgumentSheet(LPOLESTR i_lpszArgName, WORD i_wFlags, 
		TSpread *i_pSpread,const long i_lBeginRow, const long i_lColNum, const long i_lRowNum, const long i_lBeginCol =0);
#endif
// 	BOOL AddArgumentBitStream(LPOLESTR i_lpszArgName, WORD i_wFlags, CBitStreamWnd* i_pwndBitStream);
 	BOOL AddArgumentDouble(LPOLESTR i_lpszArgName, WORD i_wFlags, double i_dbValue);
 	BOOL AddArgumentCString(LPOLESTR i_lpszArgName, WORD i_wFlags, CString i_StrValue);
 	BOOL AddArgumentBool(LPOLESTR i_lpszArgName, WORD i_wFlags, BOOL i_bValue);
 	BOOL AddArgumentInt2(LPOLESTR i_lpszArgName, WORD i_wFlags, int i_nValue);
 	void AddArgumentCommon(LPOLESTR i_lpszArgName, WORD i_wFlags, VARTYPE i_vtType);
	void ClearAllArgs();
	void ReleaseVariant(VARIANTARG* i_pvarg);

	BOOL GetValueToCString(CString& i_strValue, VARIANTARG* i_pvarg);
	void ExcelSerialDateToDMY(int i_nSerialDate, int &i_nYear, int &i_nMonth, int &i_nDay);

// Attributes:
private:
	IDispatch*	m_pdispExcelApp;
	IDispatch*	m_pdispWorkbooks;
	IDispatch*	m_pdispWorkbook;
	IDispatch*	m_pdispWorksheet;

	int			m_nArgCount;
	int			m_nNamedArgCount;

	VARIANTARG	m_aVargs[MAX_DISP_ARGS];
	DISPID		m_aDispIds[MAX_DISP_ARGS + 1];		// one extra for the member name
	LPOLESTR	m_alpszArgNames[MAX_DISP_ARGS + 1];	// used to hold the argnames for GetIDs
	WORD		m_awFlags[MAX_DISP_ARGS];
};

