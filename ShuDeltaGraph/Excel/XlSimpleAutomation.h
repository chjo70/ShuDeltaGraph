#pragma once

/**
 * [�ĺ��� : CLS-GMU-GR-U-XSA]
 *
 * [������ : SRS-G-SOFR-001]
 *
 * @class    CXlSimpleAutomation
 * @brief  �ڵ�ȭ Ŭ����
 *
 * (1) Ŭ���� ����
 * -    �ڵ�ȭ Ŭ����
 *
 * (2) �����������
 * - �ش���׾���
 *
 * (3) ���� �� ����ó��
 * - �ش���׾���
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
	* @brief	������
	* @param	i_lpszFileName	LPCTSTR ���� ������
	* @param	i_bVisible	BOOL ���� ������
	* @return	
	*/
	CXlSimpleAutomation(LPCTSTR i_lpszFileName, BOOL i_bVisible = FALSE);
	/**
	* @brief	�Ҹ���
	* @param		void ���� ������
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
	* @brief	���Ϸ� ����
	* @param	i_lpszFileName	LPCTSTR ���� ������
	* @param	i_format	XlFormat ���� ������
	* @return	BOOL
	*/
	BOOL SaveAsFile(LPCTSTR i_lpszFileName, XlFormat i_format = XLSX);
	/**
	* @brief	���Ϸ� ����
	* @param	i_lpszFileName	LPCTSTR ���� ������
	* @param	i_strFileExt	CString ���� ������
	* @return	BOOL
	*/
	BOOL SaveAsFile( LPCTSTR i_lpszFileName, CString i_strFileExt );
#ifdef SSDLLMFC_H
	/**
	* @brief	��Ʈ �� ����
	* @param	i_pFpSpread	TSpread * ���� ������
	* @param	i_nBeginRow	int ���� ������
	* @param	i_nBeginCol	int ���� ������
	* @return	BOOL
	*/
	BOOL SetSheetValues(TSpread* i_pFpSpread, int i_nBeginRow = 1,int i_nBeginCol =0);
	/**
	* @brief	��Ʈ�� ��ȯ
	* @param	i_pFpSpread	TSpread * ���� ������
	* @param	i_nBeginRow	int ���� ������
	* @return	BOOL
	*/
	BOOL GetSheetValues(TSpread* i_pFpSpread, int i_nBeginRow = 1);
	/**
	* @brief	���� �迭 ������ ��ȯ
	* @param	i_plistMissionListData	list<SGRMissionInfo> * ���� ������
	* @param	i_nBeginRow	int ���� ������
	* @return	BOOL
	*/
	BOOL GetSafeArrayData(list<SGRMissionInfo>* i_plistMissionListData, int i_nBeginRow = 1 );		// ü���뿡�������
	/**
	* @brief	�ð� ������ ����
	* @param	i_pTimeStruct	SGRTimeInfo * ���� ������
	* @param	i_strTimeInfo	CString ���� ������
	* @return	BOOL
	*/
	BOOL SetTimeData(SGRTimeInfo* i_pTimeStruct, CString i_strTimeInfo);										// ü���뿡�������
#endif
	//BOOL SetBitStreamValues(CBitStreamWnd* i_pwndBitStream);
	//BOOL GetBitStreamValues(CBitStreamWnd* i_pwndBitStream);

	/**
	* @brief	�� �� ����
	* @param	i_unColumn	UINT ���� ������
	* @param	i_unRow	UINT ���� ������
	* @param	i_strValue	CString & ���� ������
	* @return	BOOL
	*/
	BOOL SetCellValue(UINT i_unColumn, UINT i_unRow, CString& i_strValue);
	/**
	* @brief	�� �� ��ȯ
	* @param	i_unColumn	UINT ���� ������
	* @param	i_unRow	UINT ���� ������
	* @param	i_strValue	CString & ���� ������
	* @return	BOOL
	*/
	BOOL GetCellValue(UINT i_unColumn, UINT i_unRow, CString& i_strValue);

	/**
	* @brief	�� �� ��ȯ
	* @return	long
	*/
	long GetRowNum();
	/**
	* @brief	�� �� ��ȯ
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

