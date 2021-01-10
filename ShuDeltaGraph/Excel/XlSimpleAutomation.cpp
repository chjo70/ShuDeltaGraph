#include "StdAfx.h"
#include "XlSimpleAutomation.h"
#include <ole2ver.h>

#include "ConvertMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const static int nMaxBufSize = 256;

CXlSimpleAutomation::CXlSimpleAutomation(LPCTSTR i_lpszFileName, BOOL i_bVisible /*= FALSE*/)
{
	m_nArgCount = -1;
	m_nNamedArgCount = -1;
	m_pdispExcelApp = NULL;
	m_pdispWorkbooks = NULL;
	m_pdispWorkbook = NULL;
	m_pdispWorksheet = NULL;

	StartExcelInstance(i_bVisible);

	if (i_lpszFileName !=nullptr)
	{
		OpenWorkbook(i_lpszFileName);
	}
	else
	{
		CreateNewWorkbook();
	}
}

CXlSimpleAutomation::~CXlSimpleAutomation(void)
{
	TerminateExcelInstance();
}

BOOL CXlSimpleAutomation::StartExcelInstance( BOOL i_bVisible )
{
	BOOL bSucceed = FALSE;

	// check the OLE library version
	DWORD dwOleVer = CoBuildVersion();
	if (rmm != CConvertMng::ConvertSigned(HIWORD(dwOleVer))) 
	{
		MessageBox(NULL, _T("Incorrect version of OLE libraries."), _T("Failed"), MB_OK | MB_ICONSTOP);
	}
	else
	{
		// could also check for minor version, but this application is
		// not sensitive to the minor version of OLE

		// initialize OLE, fail application if we can't get OLE to init.
		//if (SUCCEEDED(OleInitialize(NULL))>= 0) 
		//{
			// if Excel is already running, return with current instance
			if (m_pdispExcelApp == NULL)
			{
				/* Obtain the CLSID that identifies EXCEL.APPLICATION
				 * This value is universally unique to Excel versions 5 and up, and
				 * is used by OLE to identify which server to start.  We are obtaining
				 * the CLSID from the ProgID.
				 */
				CLSID clsExcelApp = CLSID();
				if (SUCCEEDED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp)) == TRUE) 
				{
					// start a new copy of Excel, grab the IDispatch interface
	

					if (SUCCEEDED(CoCreateInstance(clsExcelApp, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&m_pdispExcelApp)) == TRUE)
					{
						// display application
						ClearAllArgs();
						AddArgumentBool(NULL, 0, i_bVisible);
						ExlInvoke(m_pdispExcelApp, L"Visible", NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS);
						if (ExlInvoke(m_pdispExcelApp, L"Visible", NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS))
						{
							ClearAllArgs();
							VARIANTARG varg1 = VARIANTARG();
							ExlInvoke(m_pdispExcelApp, L"Workbooks", &varg1, DISPATCH_PROPERTYGET, 0);
							//if (ExlInvoke(m_pdispExcelApp, L"Workbooks", &varg1, DISPATCH_PROPERTYGET, 0))
							//{
								m_pdispWorkbooks = varg1.pdispVal;
								bSucceed = TRUE;
							//}
						}
					}
					else
					{
						MessageBox(NULL, _T("Cannot start an instance of Excel for Automation."), _T("Failed"), MB_OK | MB_ICONSTOP);
					}
				}
				else
				{
					MessageBox(NULL, _T("Cannot obtain CLSID from ProgID"), _T("Failed"), MB_OK | MB_ICONSTOP);
				}
			}
		//}
		//else 
		//	MessageBox(NULL, _T("Cannot initialize OLE."), "Failed", MB_OK | MB_ICONSTOP);
	}

	return bSucceed;
}

BOOL CXlSimpleAutomation::TerminateExcelInstance()
{
	BOOL bSucceed = FALSE;

	if (m_pdispExcelApp !=nullptr)
	{
		// Tell Excel to quit, since for automation simply releasing the IDispatch
		// object isn't enough to get the server to shut down.

		// Note that this code will hang if Excel tries to display any message boxes.
		// This can occur if a document is in need of saving.  The CreateChart() code
		// always clears the dirty bit on the documents it creates, avoiding this problem.

		ClearAllArgs();
		bSucceed = ExlInvoke(m_pdispExcelApp, L"Quit", NULL, DISPATCH_METHOD, 0);

		// Even though Excel has been told to Quit, we still need to release the
		// OLE object to account for all memory.
		ReleaseDispatch();
	}
	OleUninitialize();

	return bSucceed;
}

void CXlSimpleAutomation::ReleaseDispatch()
{
	if(m_pdispWorksheet !=nullptr)
	{
		m_pdispWorksheet->Release();
		m_pdispWorksheet = NULL;
	}

	if(m_pdispWorkbook !=nullptr)
	{
		m_pdispWorkbook->Release();
		m_pdispWorkbook = nullptr;
	}

	if (m_pdispWorkbooks !=nullptr)
	{
		ExlInvoke(m_pdispWorkbook, L"Close", NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS);
		m_pdispWorkbooks->Release();
		m_pdispWorkbooks = NULL;
	}

	if(m_pdispExcelApp !=nullptr)
	{
		m_pdispExcelApp->Release();
		m_pdispExcelApp = NULL;
	}
}

BOOL CXlSimpleAutomation::CreateNewWorkbook()
{
	BOOL bSucceed = FALSE;

	if (m_pdispWorkbooks !=nullptr)
	{
		ClearAllArgs();
		AddArgumentInt2(L"Template", 0, xlWorksheet);

		VARIANTARG vargWorkbook = VARIANTARG();
		ExlInvoke(m_pdispWorkbooks, L"Add", &vargWorkbook, DISPATCH_METHOD, DISP_FREEARGS);
		//if (ExlInvoke(m_pdispWorkbooks, L"Add", &vargWorkbook, DISPATCH_METHOD, DISP_FREEARGS))
		//{
			m_pdispWorkbook = vargWorkbook.pdispVal;
			bSucceed = GetActiveSheet();
		//}
	}

	return bSucceed;
}

BOOL CXlSimpleAutomation::OpenWorkbook( LPCTSTR i_lpszFileName )
{
	BOOL bSucceed = FALSE;

	if (m_pdispWorkbooks !=nullptr && PathFileExists(i_lpszFileName) ==TRUE)
	{
		ClearAllArgs();
		AddArgumentCString(L"Filename", 0, i_lpszFileName);

		VARIANTARG vargWorkbook;
		if (ExlInvoke(m_pdispWorkbooks, L"Open", &vargWorkbook, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		{
			m_pdispWorkbook = vargWorkbook.pdispVal;
			bSucceed = GetActiveSheet();
		}
	}

	return bSucceed;
}

BOOL CXlSimpleAutomation::GetActiveSheet()
{
	BOOL bSucceed = FALSE;

	if (m_pdispExcelApp !=nullptr)
	{
		ClearAllArgs();
		VARIANTARG vargSheet = VARIANTARG();
		ExlInvoke(m_pdispExcelApp, L"ActiveSheet", &vargSheet, DISPATCH_PROPERTYGET, 0);
		//if ( ExlInvoke(m_pdispExcelApp, L"ActiveSheet", &vargSheet, DISPATCH_PROPERTYGET, 0) == TRUE)
		//{
			m_pdispWorksheet = vargSheet.pdispVal;
			bSucceed =TRUE;
		//}
	}

	return bSucceed;
}

/*
 *  INVOKE
 *
 *  Invokes a method or property.  Takes the IDispatch object on which to invoke,
 *  and the name of the method or property as a String.  Arguments, if any,
 *  must have been previously setup using the AddArgumentXxx() functions.
 *
 *  Returns TRUE if the call succeeded.  Returns FALSE if an error occurred.
 *  A messagebox will be displayed explaining the error unless the DISP_NOSHOWEXCEPTIONS
 *  flag is specified.  Errors can be a result of unrecognized method or property
 *  names, bad argument names, invalid types, or runtime-exceptions defined
 *  by the recipient of the Invoke.
 *
 *  The argument list is reset via ClearAllArgs() if the DISP_FREEARGS flag is
 *  specified.  If not specified, it is up to the caller to call ClearAllArgs().
 *
 *  The return value is placed in pvargReturn, which is allocated by the caller.
 *  If no return value is required, pass NULL.  It is up to the caller to free
 *  the return value (ReleaseVariant()).
 *
 *  This function calls IDispatch::GetIDsOfNames for every invoke.  This is not
 *  very efficient if the same method or property is invoked multiple times, since
 *  the DISPIDs for a particular method or property will remain the same during
 *  the lifetime of an IDispatch object.  Modifications could be made to this code
 *  to cache DISPIDs.  If the target application is always the same, a similar
 *  modification is to statically browse and store the DISPIDs at compile-time, since
 *  a given application will return the same DISPIDs in different sessions.
 *  Eliminating the extra cross-process GetIDsOfNames call can result in a
 *  signficant time savings.
 */
BOOL CXlSimpleAutomation::ExlInvoke( IDispatch* i_pdisp, LPOLESTR i_szName, VARIANTARG* i_pvargResult, WORD i_wInvokeAction, WORD i_wFlags )
{
	BOOL bSucceed = FALSE;

	if (i_pdisp !=nullptr)
	{
		// Get the IDs for the member and its arguments.  GetIDsOfNames expects the
		// member name as the first name, followed by argument names (if any).
		m_alpszArgNames[0] = i_szName;
		HRESULT hRetResult = i_pdisp->GetIDsOfNames(IID_NULL, m_alpszArgNames, CConvertMng::ConvertSigned(1 + m_nNamedArgCount), 
			LOCALE_SYSTEM_DEFAULT, m_aDispIds);

		if (i_pvargResult != NULL)
		{
			VariantInit(i_pvargResult);
		}

		// if doing a property put(ref), we need to adjust the first argument to have a
		// named arg of DISPID_PROPERTYPUT.
		int iwInvokeAction = (INT)CConvertMng::ConvertSigned(CConvertMng::GetCalcBitWise((UINT32)i_wInvokeAction, (INT32)(DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF), _T("&") ));

		if (iwInvokeAction !=0) 
		{
			m_nNamedArgCount = 1;
			m_aDispIds[1] = DISPID_PROPERTYPUT;
			i_pvargResult = nullptr;
		}
		int nRetResult1 = SUCCEEDED(hRetResult);
		
		//if (nRetResult1 >=0)
		//{
			DISPPARAMS dispparams;
			dispparams.rgdispidNamedArgs = m_aDispIds + 1;
			dispparams.rgvarg = m_aVargs;
			dispparams.cArgs = CConvertMng::ConvertSigned(m_nArgCount);
			dispparams.cNamedArgs = CConvertMng::ConvertSigned(m_nNamedArgCount);

			EXCEPINFO excep;
			excep.pfnDeferredFillIn = NULL;

			unsigned int unArgErr =0;

			hRetResult = i_pdisp->Invoke(m_aDispIds[0], IID_NULL, LOCALE_SYSTEM_DEFAULT, 
				i_wInvokeAction, &dispparams, i_pvargResult, &excep, &unArgErr);
			int nRetResult = SUCCEEDED(hRetResult);

			//if (nRetResult >=0)
				bSucceed = TRUE;
			/*else
			{
				// display the exception information if appropriate:
				if ((i_wFlags & DISP_NOSHOWEXCEPTIONS) ==false)
				{
					ShowException(i_szName, hRetResult, &excep, unArgErr);
					}

				// free exception structure information
				SysFreeString(excep.bstrSource);
				SysFreeString(excep.bstrDescription);
				SysFreeString(excep.bstrHelpFile);
			}*/
		//}
		//else
		//{
		//	if ((i_wFlags & DISP_NOSHOWEXCEPTIONS) ==false) 
		//		ShowException(i_szName, hRetResult, NULL, 0);
		//}

		int iFlags = (INT)CConvertMng::ConvertSigned(CConvertMng::GetCalcBitWise((UINT32)CConvertMng::ConvertSigned(i_wFlags), (INT32)DISP_FREEARGS, _T("&") ));
		if ( iFlags !=0)
		{
			ClearAllArgs();
		}
	}

	return bSucceed;
}

/*
 *  ClearAllArgs
 *
 *  Clears the existing contents of the arg array in preparation for
 *  a new invocation.  Frees argument memory if so marked.
 */
void CXlSimpleAutomation::ClearAllArgs()
{
	for (int i = 0; i < m_nArgCount; i++) 
	{
		int nFlags = (INT)CConvertMng::ConvertSigned(CConvertMng::GetCalcBitWise((UINT32)CConvertMng::ConvertSigned(m_awFlags[i]), (INT32)DISPARG_NOFREEVARIANT, _T("&") ));

		if (nFlags !=0)
		{
			VariantInit(&m_aVargs[i]);
		}
		else
		{
			ReleaseVariant(&m_aVargs[i]);
		}
	}

	m_nArgCount = 0;
	m_nNamedArgCount = 0;
}

/*
 *  ReleaseVariant
 *
 *  Clears a particular variant structure and releases any external objects
 *  or memory contained in the variant.  Supports the data types listed above.
 */
void CXlSimpleAutomation::ReleaseVariant( VARIANTARG* i_pvarg )
{
	if (i_pvarg !=nullptr)
	{
		VARTYPE vt;

		vt = i_pvarg->vt & CConvertMng::ConvertSigned(0xfff);		// mask off flags

		// check if an array.  If so, free its contents, then the array itself.
		unsigned int npvarg =V_ISARRAY(i_pvarg);//#FA_Q_3000_T7

		if (npvarg !=0) 
		{
			// variant arrays are all this routine currently knows about.  Since a
			// variant can contain anything (even other arrays), call ourselves
			// recursively.
			if (CConvertMng::ConvertSigned(vt) == VT_VARIANT) 
			{
				long lLowerBound[2] ={0};
				long lUpperBound[2] ={0};
				SafeArrayGetLBound(i_pvarg->parray, 1, &lLowerBound[0]);
				SafeArrayGetUBound(i_pvarg->parray, 1, &lUpperBound[0]);
				SafeArrayGetLBound(i_pvarg->parray, 2, &lLowerBound[1]);
				SafeArrayGetUBound(i_pvarg->parray, 2, &lUpperBound[1]);

				long lRowNum = lUpperBound[0] - lLowerBound[0] + 1;
				long lColNum = lUpperBound[1] - lLowerBound[1] + 1;
				const long lSize = lRowNum * lColNum;

				VARIANTARG* pvargArray =nullptr;
				SafeArrayAccessData(i_pvarg->parray, (void**)&pvargArray);

				for (long i = 0; i < lSize; i++)
				{
					VariantClear(pvargArray);
					pvargArray++;
				}

				SafeArrayUnaccessData(i_pvarg->parray);
			}
			else 
			{
				MessageBox(NULL, _T("ReleaseVariant: Array contains non-variant type"), _T("Failed"), MB_OK | MB_ICONSTOP);
			}
		}

		VariantClear(i_pvarg);
	}
}

/*******************************************************************
 *
 *					   ARGUMENT CONSTRUCTOR FUNCTIONS
 *
 *  Each function adds a single argument of a specific type to the list
 *  of arguments for the current invoke.  If appropriate, memory may be
 *  allocated to represent the argument.  This memory will be
 *  automatically freed the next time ClearAllArgs() is called unless
 *  the NOFREEVARIANT flag is specified for a particular argument.  If
 *  NOFREEVARIANT is specified it is the responsibility of the caller
 *  to free the memory allocated for or contained within the argument.
 *
 *  Arguments may be named.  The name string must be a C-style string
 *  and it is owned by the caller.  If dynamically allocated, the caller
 *  must free the name string.
 *
 *******************************************************************/
BOOL CXlSimpleAutomation::AddArgumentCString( LPOLESTR i_lpszArgName, WORD i_wFlags, CString i_strValue )
{
	AddArgumentCommon(i_lpszArgName, i_wFlags, VT_BSTR);
	m_aVargs[m_nArgCount++].bstrVal = i_strValue.AllocSysString();

	return TRUE;
}

BOOL CXlSimpleAutomation::AddArgumentDouble( LPOLESTR i_lpszArgName, WORD i_wFlags, double i_dbValue )
{
	AddArgumentCommon(i_lpszArgName, i_wFlags, VT_R8);
	m_aVargs[m_nArgCount++].dblVal = i_dbValue;
	return TRUE;
}

BOOL CXlSimpleAutomation::AddArgumentInt2( LPOLESTR i_lpszArgName, WORD i_wFlags, int i_nValue )
{
	AddArgumentCommon(i_lpszArgName, i_wFlags, VT_I2);
	m_aVargs[m_nArgCount++].iVal =CConvertMng::ConvertIntToShort( i_nValue
);	
	return TRUE;
}

BOOL CXlSimpleAutomation::AddArgumentBool( LPOLESTR i_lpszArgName, WORD i_wFlags, BOOL i_bValue )
{
	AddArgumentCommon(i_lpszArgName, i_wFlags, VT_BOOL);
	// Note the variant representation of True as -1
	m_aVargs[m_nArgCount++].boolVal = i_bValue ? -1 : 0;
	return TRUE;
}

/*
 *  Common code used by all variant types for setting up an argument.
 */
void CXlSimpleAutomation::AddArgumentCommon( LPOLESTR i_lpszArgName, WORD i_wFlags, VARTYPE i_vtType )
{
	VariantInit(&m_aVargs[m_nArgCount]);

	m_aVargs[m_nArgCount].vt = i_vtType;
	m_awFlags[m_nArgCount] = i_wFlags;

	if (i_lpszArgName != NULL) 
	{
		m_alpszArgNames[m_nNamedArgCount + 1] = i_lpszArgName;
		m_nNamedArgCount++;
	}
}

BOOL CXlSimpleAutomation::SetCellValue( UINT i_unColumn, UINT i_unRow, CString& i_strValue )
{
	BOOL bSucceed = FALSE;

	if (m_pdispWorksheet !=nullptr && i_strValue.IsEmpty() ==false)
	{
		ClearAllArgs();
		AddArgumentDouble(NULL, 0, i_unColumn);
		AddArgumentDouble(NULL, 0, i_unRow);

		VARIANTARG vargRng = VARIANTARG();
		ExlInvoke(m_pdispWorksheet, L"Cells",&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS);
		//if (ExlInvoke(m_pdispWorksheet, L"Cells",&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		//{
			AddArgumentCString(NULL, 0, i_strValue);
			bSucceed = ExlInvoke(vargRng.pdispVal, L"Value", NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS);
		//}
		ReleaseVariant(&vargRng);
	}

	return bSucceed;
}

BOOL CXlSimpleAutomation::GetCellValue( UINT i_unColumn, UINT i_unRow, CString& i_strValue )
{
	if (m_pdispWorkbook !=nullptr)
	{
		ClearAllArgs();
		AddArgumentDouble(NULL, 0, i_unColumn);
		AddArgumentDouble(NULL, 0, i_unRow);

		VARIANTARG vargRng;
		if (ExlInvoke(m_pdispWorksheet, L"Cells", &vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		{
			VARIANTARG vargValue;
			if (ExlInvoke(vargRng.pdispVal, L"Text", &vargValue, DISPATCH_PROPERTYGET, 0))
			{
				GetValueToCString(i_strValue, &vargValue);
			}
		}
	}

	return TRUE;
}
#ifdef SSDLLMFC_H
BOOL CXlSimpleAutomation::AddArgumentSheet( LPOLESTR i_lpszArgName, WORD i_wFlags, 
	TSpread *i_pSpread, const long i_lBeginRow, const long i_lColNum, const long i_lRowNum,const long i_lBeginCol )
{
	BOOL bSucceed = TRUE;

	if (m_pdispWorksheet !=nullptr && i_pSpread !=nullptr)
	{
		SAFEARRAYBOUND sab[2];
		sab[0].lLbound = 0;
		sab[0].cElements = CConvertMng::ConvertSigned(i_lRowNum);
		sab[1].lLbound = 0;
		sab[1].cElements = CConvertMng::ConvertSigned(i_lColNum);

		SAFEARRAY* pSa = SafeArrayCreate(VT_VARIANT, 2, sab);
		if (pSa !=NULL)
		{
			TCHAR CharBuf[nMaxBufSize] ={0};
			WCHAR WCharBuf[nMaxBufSize] ={0};

			for (long i = 0; i < i_lRowNum && bSucceed; i++)
			{
				for (long j = 0; j < i_lColNum; j++)
				{
					i_pSpread->GetValue(j + i_lBeginCol+1, i + i_lBeginRow, CharBuf);
					
					if (lstrlen(CharBuf) > 0)
					{
						MultiByteToWideChar(CP_ACP, 0, CharBuf, sizeof(CharBuf), 
							WCharBuf, sizeof(WCharBuf));

						VARIANT var;
						var.vt = VT_BSTR;
						var.bstrVal = ::SysAllocString(WCharBuf);
						if (var.bstrVal != NULL)
						{
							long pos[2] = {i, j};
							SafeArrayPutElement(pSa, pos, (void*)&var);
							::SysFreeString(var.bstrVal);
						}
						else
						{
							bSucceed = FALSE;
							VARIANT ar;
							ar.parray = pSa;
							ar.vt = VT_ARRAY | VT_VARIANT;
							ReleaseVariant(&ar);
							break;
						}
					}
				}
			}

			if (bSucceed)
			{
				AddArgumentCommon(i_lpszArgName, i_wFlags, VT_VARIANT | VT_ARRAY);
				m_aVargs[m_nArgCount++].parray = pSa;
			}
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL CXlSimpleAutomation::SetSheetValues( TSpread* i_pFpSpread, int i_nBeginRow /*= 1*/,int i_nBeginCol)
{
	BOOL bSucceed = FALSE;

	if (m_pdispWorksheet !=nullptr && i_pFpSpread !=nullptr)
	{
		const long lMaxRow = i_pFpSpread->GetMaxRows();
		const long lMaxCol = i_pFpSpread->GetMaxCols();
		const long lMinRow = max(0, min(i_nBeginRow, lMaxRow));
		const long lMinCol =  max(0, min(i_nBeginCol, lMaxCol));
		long lRowNum = lMaxRow - lMinRow + 1;

		ClearAllArgs();
		CString strRange ="";
		strRange.Format(_T("A1:%s%d"), ConvertIndexToName(CConvertMng::ConvertSigned(lMaxCol - 1)), lRowNum);
		AddArgumentCString(NULL, 0, strRange);

		VARIANTARG vargRng = VARIANTARG();
		if (ExlInvoke(m_pdispWorksheet, L"Range", &vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		{
			ClearAllArgs();
			AddArgumentCString(NULL, 0, _T("@")); // 텍스트로 서식 지정
			if (ExlInvoke(vargRng.pdispVal, L"NumberFormat", NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS))
			{
				if (AddArgumentSheet(NULL, 0, i_pFpSpread, lMinRow, lMaxCol, lRowNum,lMinCol))
				{
					bSucceed = ExlInvoke(vargRng.pdispVal, L"Value", NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS);
				}
			}
		}
		ReleaseVariant(&vargRng);
	}

	return bSucceed;
}

BOOL CXlSimpleAutomation::GetSheetValues( TSpread* i_pFpSpread, int i_nBeginRow /*= 1*/ )
{
	BOOL bSucceed = FALSE;

	if (m_pdispWorksheet !=nullptr && i_pFpSpread !=nullptr)
	{
		const int nMaxRow = (int)GetRowNum();
		const int nMaxCol = (int)GetColumnNum();
		const int nMinRow = max(1, min(i_nBeginRow, nMaxRow));
		const int nRowNum = nMaxRow - nMinRow + 1;

		i_pFpSpread->SetMaxRows(nRowNum);

		ClearAllArgs();
		CString strRange ="";
		strRange.Format(_T("A%d:%s%d"), nMinRow, ConvertIndexToName(CConvertMng::ConvertSigned(nMaxCol - 1)), nMaxRow);
		AddArgumentCString(NULL, 0, strRange);		
		VARIANTARG vargRng =VARIANTARG();
		if (ExlInvoke(m_pdispWorksheet, L"Range", &vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		{
			VARIANTARG vargValues;
			if (ExlInvoke(vargRng.pdispVal, L"Value", &vargValues, DISPATCH_PROPERTYGET, 0))
			{
				SAFEARRAY* pSa = NULL;
				int nVar = (INT)CConvertMng::ConvertSigned(CConvertMng::GetCalcBitWise((UINT32)vargValues.vt, (INT32)VT_ARRAY, "&"));
				if (nVar !=0)
				{
					pSa = vargValues.parray;
				}

				if (pSa !=NULL)
				{
					long lLowerBound[2] ={0};
					long lUpperBound[2] ={0};
					SafeArrayGetLBound(pSa, 1, &lLowerBound[0]);
					SafeArrayGetUBound(pSa, 1, &lUpperBound[0]);
					SafeArrayGetLBound(pSa, 2, &lLowerBound[1]);
					SafeArrayGetUBound(pSa, 2, &lUpperBound[1]);

					long lRowNum = lUpperBound[0] - lLowerBound[0] + 1;
					long lColNum = lUpperBound[1] - lLowerBound[1] + 1;

					CString strValue ="";
					for (long i = 1; i <= lRowNum; i++)
					{
						for (long j = 1; j <= lColNum; j++)
						{
							long pos[2] = {i, j};

							VARIANT var = VARIANT();
							SafeArrayGetElement(pSa, pos, (void*)&var);
							GetValueToCString(strValue, &var);
							i_pFpSpread->SetValue(j, i, strValue);
						}
					}
					bSucceed = TRUE;

					ReleaseVariant(&vargValues);
				}
			}
			ReleaseVariant(&vargRng);
		}
	}

	return bSucceed;
}
// 체계운용 임무목록에서 불러오기 시, 데이터를 구조체에 담아 Sheet에 출력 하지 않고 매니저로 보내기 위해 만든 함수. 임시.
BOOL CXlSimpleAutomation::GetSafeArrayData(list<SGRMissionInfo>* i_plistMissionListData, int i_nBeginRow /*= 1*/ )	
{
	BOOL bSucceed = TRUE;
	if (m_pdispWorksheet !=nullptr && i_plistMissionListData !=nullptr)
	{
		const int nMaxRow = (int)GetRowNum();
		const int nMaxCol = (int)GetColumnNum();
		const int nMinRow = max(1, min(i_nBeginRow, nMaxRow));

		ClearAllArgs();
		CString strRange ="";
		strRange.Format(_T("A%d:%s%d"), nMinRow, ConvertIndexToName(CConvertMng::ConvertSigned(nMaxCol - 1)), nMaxRow);
		AddArgumentCString(NULL, 0, strRange);		
		VARIANTARG vargRng;
		if (ExlInvoke(m_pdispWorksheet, L"Range", &vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		{
			VARIANTARG vargValues;
			if (ExlInvoke(vargRng.pdispVal, L"Value", &vargValues, DISPATCH_PROPERTYGET, 0))
			{
				SAFEARRAY* pSa = NULL;
				int nVar = (INT)CConvertMng::ConvertSigned(CConvertMng::GetCalcBitWise((UINT32)vargValues.vt, (INT32)VT_ARRAY, "&"));
				if (nVar !=0)
				{
					pSa = vargValues.parray;
				}

				if (pSa !=NULL)
				{
					long lLowerBound[2] ={0};
					long lUpperBound[2] ={0};
					SafeArrayGetLBound(pSa, 1, &lLowerBound[0]);
					SafeArrayGetUBound(pSa, 1, &lUpperBound[0]);
					SafeArrayGetLBound(pSa, 2, &lLowerBound[1]);
					SafeArrayGetUBound(pSa, 2, &lUpperBound[1]);

					long lRowNum = lUpperBound[0] - lLowerBound[0] + 1;

					CString strValue = _T("");
					CString strMsg = _T("");
					for (long lRow = 1; lRow <= lRowNum; lRow++)
					{
						SGRMissionInfo stMissInfo;
						VARIANT var =VARIANT();
						long lCol = 1; 
						long pos[2] = {lRow, lCol};

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 임무ID 가져오기
						SafeArrayGetElement(pSa, pos, (void*)&var);
						GetValueToCString(strValue, &var);
						// 임무ID 검사
						if (strValue.GetLength() < DEF_MISSION_ID_LENGTH && strValue.Mid(0, 3) == "MI_")
						{
							CString strTemp = _T("");
							strTemp = strValue.Mid(3, 17);
							if (strTemp.GetLength() == 17)
							{
								for (int nIndex = 0; nIndex < strTemp.GetLength(); nIndex++)
								{
									if (isdigit(CConvertMng::ConvertSigned(strTemp[nIndex])) <= 0)
									{
										bSucceed = FALSE;
										break;
									}
								}
							}
							else
							{
								bSucceed = FALSE;
							}
						}
						else
						{
							bSucceed = FALSE;							
						}
						// 임무ID 검사 결과
						if (bSucceed == FALSE)
						{
							strMsg.Format("임무ID Error[%d, %d], 가져오기를 중단합니다.\n'MI_' 이후 숫자는 17자리입니다.", lCol, lRow+1);
							AfxMessageBox(strMsg);
							break;
						}
						// 임무ID 저장
						lstrcpyn(stMissInfo.stMissionID.acMissionID, strValue, sizeof(stMissInfo.stMissionID.acMissionID));		

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 임무명 가져오기
						lCol = lCol+1;
						pos[1] = lCol;
						SafeArrayGetElement(pSa, pos, (void*)&var);
						GetValueToCString(strValue, &var);
						// 임무명 검사
						if (strValue.GetLength() >= DEF_MISSION_NAME_LENGTH)
						{
							bSucceed = FALSE;
							strMsg.Format("임무명 Error[%d, %d], 가져오기를 중단합니다.\n임무명의 길이는 최대 [%d]Byte 이며, 한글은 2Byte로 계산됩니다.", lCol, lRow+1, DEF_MISSION_NAME_LENGTH-1);
							AfxMessageBox(strMsg);
							break;
						}
						// 임무명 저장
						lstrcpyn(stMissInfo.acMissionName, strValue, sizeof(stMissInfo.acMissionName));					

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 생성자 가져오기
						lCol = lCol+1;
						pos[1] = lCol;
						SafeArrayGetElement(pSa, pos, (void*)&var);
						GetValueToCString(strValue, &var);
						if (strValue.GetLength() >= DEF_OPERATOR_NAME_LENGTH)
						{
							bSucceed = FALSE;
							strMsg.Format("생성자 Error[%d, %d], 가져오기를 중단합니다.\n생성자의 길이는 최대 [%d]Byte 이며, 한글은 2Byte로 계산됩니다.", lCol, lRow+1, DEF_OPERATOR_NAME_LENGTH-1);
							AfxMessageBox(strMsg);
							break;
						}
						// 생성자 저장
						lstrcpyn(stMissInfo.stCreator.acOperatorName, strValue, sizeof(stMissInfo.stCreator.acOperatorName));		

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 시작시간 가져오기
						lCol = lCol+1;
						pos[1] = lCol;
						SafeArrayGetElement(pSa, pos, (void*)&var);
						GetValueToCString(strValue, &var);
						if (SetTimeData(&stMissInfo.stMissionStartTime, strValue) == FALSE)
						{
							bSucceed = FALSE;
							strMsg.Format("시작시간 Error[%d, %d], 가져오기를 중단합니다.\n시간의 형태는 다음과 같아야합니다. 예)16-04-07-16:07:29", lCol, lRow+1);
							AfxMessageBox(strMsg);
							break;
						}
						
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 종료시간 가져오기
						lCol = lCol+1;
						pos[1] = lCol;
						SafeArrayGetElement(pSa, pos, (void*)&var);
						GetValueToCString(strValue, &var);
						if (SetTimeData(&stMissInfo.stMissionEndTime, strValue) == FALSE)
						{
							bSucceed = FALSE;
							strMsg.Format("종료시간 Error[%d, %d], 가져오기를 중단합니다.\n시간의 형태는 다음과 같아야합니다. 예)16-04-07-16:07:29", lCol, lRow+1);
							AfxMessageBox(strMsg);
							break;
						}

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 생성시간 가져오기
						lCol = lCol+1;
						pos[1] = lCol;
						SafeArrayGetElement(pSa, pos, (void*)&var);
						GetValueToCString(strValue, &var);
						if (SetTimeData(&stMissInfo.stMissionCreationTime, strValue) == FALSE)
						{
							bSucceed = FALSE;
							strMsg.Format("생성시간 Error[%d, %d], 가져오기를 중단합니다.\n시간의 형태는 다음과 같아야합니다. 예)16-04-07-16:07:29", lCol, lRow+1);
							AfxMessageBox(strMsg);
							break;
						}

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 메모 가져오기
						lCol = lCol+1;
						pos[1] = lCol;
						SafeArrayGetElement(pSa, pos, (void*)&var);
						GetValueToCString(strValue, &var);
						if (strValue.GetLength() >= DEF_MEMO_LENGTH)
						{
							bSucceed = FALSE;
							strMsg.Format("메모 Error[%d, %d], 가져오기를 중단합니다.\n메모의 길이는 최대 [%d]Byte 이며, 한글은 2Byte로 계산됩니다.", lCol, lRow+1, DEF_MEMO_LENGTH-1);
							AfxMessageBox(strMsg);
							break;
						}
						lstrcpyn(stMissInfo.acMemo, strValue, sizeof(stMissInfo.acMemo));			// 메모

						i_plistMissionListData->push_back(stMissInfo);
					}
					ReleaseVariant(&vargValues);
				}
			}
			ReleaseVariant(&vargRng);
		}
	}

	return bSucceed;
}
// GetSafeArrayData()에서 사용하는 함수. 임시.
BOOL CXlSimpleAutomation::SetTimeData(SGRTimeInfo* i_pTimeStruct, CString i_strTimeInfo)
{
	BOOL bReturn = TRUE;

	if (i_pTimeStruct !=nullptr && i_strTimeInfo.IsEmpty()==false)
	{
		CString strSub = _T("");
		
		if (i_strTimeInfo.Compare("__-__-__-__:__:__") != 0)
		{

			// 1Byte 씩 형식 및 숫자 검사. 예)15-04-07-16:37:29 
			for (int nIndex = 0; nIndex < i_strTimeInfo.GetLength(); nIndex++)//#FA_C_PotentialUnboundedLoop_T2
			{							
				// 년
				if (nIndex == 0 || nIndex == 1)
				{
					if (isdigit(CConvertMng::ConvertSigned(i_strTimeInfo[nIndex])) <= 0)
					{
						bReturn = FALSE;
						break;
					}
				}
				// '-'
				else if (nIndex == 2)
				{
					if (i_strTimeInfo[nIndex] != '-')
					{
						bReturn = FALSE;
						break;
					}
				}
				// 월
				else if (nIndex == 3 || nIndex == 4)
				{
					if (isdigit(i_strTimeInfo[nIndex]) <= 0)
					{
						bReturn = FALSE;
						break;
					}
					// 1 ~ 12
					if (nIndex == 4)
					{
						strSub = i_strTimeInfo.Mid(3,2);
						if (_ttoi(strSub) < 1 || _ttoi(strSub) > 12)
						{
							bReturn = FALSE;
							break;
						}
					}
				}
				// '-'
				else if (nIndex == 5)
				{
					if (i_strTimeInfo[nIndex] != '-')
					{
						bReturn = FALSE;
						break;
					}
				}
				// 일
				else if (nIndex == 6 || nIndex == 7)
				{
					if (isdigit(i_strTimeInfo[nIndex]) <= 0)
					{
						bReturn = FALSE;
						break;
					}
					// 1 ~ 31
					if (nIndex == 7)
					{
						strSub = i_strTimeInfo.Mid(6,2);
						if (_ttoi(strSub) < 1 || _ttoi(strSub) > 31)
						{
							bReturn = FALSE;
							break;
						}
					}
				}
				// '-'
				else if (nIndex == 8)
				{
					if (i_strTimeInfo[nIndex] != '-')
					{
						bReturn = FALSE;
						break;
					}
				}
				// 시
				else if (nIndex == 9 || nIndex == 10)
				{
					if (isdigit(i_strTimeInfo[nIndex]) <= 0)
					{
						bReturn = FALSE;
						break;
					}
					// 0 ~ 23
					if (nIndex == 10)
					{
						strSub = i_strTimeInfo.Mid(9,2);
						if (_ttoi(strSub) < 0 || _ttoi(strSub) > 23)
						{
							bReturn = FALSE;
							break;
						}
					}

				}
				// ':'
				else if (nIndex == 11)
				{
					if (i_strTimeInfo[nIndex] != ':')
					{
						bReturn = FALSE;
						break;
					}
				}
				// 분
				else if (nIndex == 12 || nIndex == 13)
				{
					if (isdigit(i_strTimeInfo[nIndex]) <= 0)
					{
						bReturn = FALSE;
						break;
					}
					// 0 ~ 59
					if (nIndex == 13)
					{
						strSub = i_strTimeInfo.Mid(12,2);
						if (_ttoi(strSub) < 0 || _ttoi(strSub) > 59)
						{
							bReturn = FALSE;
							break;
						}
					}
				}
				// ':'
				else if (nIndex == 14)
				{
					if (i_strTimeInfo[nIndex] != ':')
					{
						bReturn = FALSE;
						break;
					}
				}
				// 초
				else if (nIndex == 15 || nIndex == 16)
				{
					if (isdigit(CConvertMng::ConvertSigned(i_strTimeInfo[nIndex])) <= 0)
					{
						bReturn = FALSE;
						break;
					}
					// 0 ~ 59
					if (nIndex == 16)
					{
						strSub = i_strTimeInfo.Mid(12,2);
						if (_ttoi(strSub) < 0 || _ttoi(strSub) > 59)
						{
							bReturn = FALSE;
							break;
						}
					}
				}
				else
				{//DTEC_Else
				}
			}
		}

		if (bReturn == TRUE)
		{
			// 년
			strSub = i_strTimeInfo.Mid(0,2);
			i_pTimeStruct->uiYear = CConvertMng::ConvertSigned(_ttoi(strSub));
			// 월
			strSub = i_strTimeInfo.Mid(3,2);
			i_pTimeStruct->uiMonth = CConvertMng::ConvertSigned(_ttoi(strSub));			
			// 일
			strSub = i_strTimeInfo.Mid(6,2);
			i_pTimeStruct->uiDayOfMonth = CConvertMng::ConvertSigned(_ttoi(strSub));
			// 시
			strSub = i_strTimeInfo.Mid(9,2);
			i_pTimeStruct->uiHour = CConvertMng::ConvertSigned(_ttoi(strSub));
			// 분
			strSub = i_strTimeInfo.Mid(12,2);
			i_pTimeStruct->uiMinute = CConvertMng::ConvertSigned(_ttoi(strSub));
			// 초
			strSub = i_strTimeInfo.Mid(15,2);
			i_pTimeStruct->uiSecond = CConvertMng::ConvertSigned(_ttoi(strSub));
		}
	}

	return bReturn;
}
#endif

CString CXlSimpleAutomation::ConvertIndexToName( UINT i_unIndex )
{
	int nCycleNum = CConvertMng::ConvertSigned(i_unIndex) / 26;
	int nWithInNum = CConvertMng::ConvertSigned(i_unIndex) - (nCycleNum * 26);

	CString strName =_T("");
	if (nCycleNum > 0)
	{
		strName.AppendChar((TCHAR)((nCycleNum - 1) + _T('A')));
	}
	strName.AppendChar((TCHAR)(nWithInNum + _T('A')));

	return strName;
}

BOOL CXlSimpleAutomation::GetValueToCString( CString& i_strValue, VARIANTARG* i_pvarg )
{
	VARTYPE Type = i_pvarg->vt;
	switch (CConvertMng::ConvertSigned(Type)) 
	{
	case VT_I4:
		{
			long nVal = i_pvarg->lVal;
			i_strValue.Format(_T("%i"), nVal);
		}
		break;
	case VT_R4:
		{
			float fVal = i_pvarg->fltVal;
			i_strValue.Format(_T("%f"), fVal);
		}
		break;
	case VT_R8:
		{
			double dVal = i_pvarg->dblVal;
			i_strValue.Format(_T("%f"), dVal);
		}
		break;
	///////////////////////////////////////////////////////////////////
	// Date Type에 대해 추가
	case VT_DATE:
		{
			DATE date = i_pvarg->date;
			int nYear=0;
			int nMonth=0;
			int nDay=0;
			ExcelSerialDateToDMY((int)date, nYear, nMonth, nDay);
			i_strValue.Format(_T("%04d-%02d-%02d"), nYear, nMonth, nDay);
		}
		break;
	///////////////////////////////////////////////////////////////////
	case VT_BSTR:
		{
			BSTR b = i_pvarg->bstrVal;
			i_strValue = b;
		}
		break;
	case VT_BYREF|VT_BSTR:
		{
			//Not tested
			BSTR* pb = i_pvarg->pbstrVal;
			i_strValue = *pb;
		}
	default:
		{
			//Empty
			i_strValue = _T("");
		}
		break;
	}

	return TRUE;
}

void CXlSimpleAutomation::ExcelSerialDateToDMY(int i_nSerialDate, int &i_nYear, int &i_nMonth, int &i_nDay)//#FA_Q_4020_T1
{
	if (i_nSerialDate == 60)
	{
		i_nDay = 29;
		i_nMonth = 2;
		i_nYear = 1900;
		return;
	}
	else if (i_nSerialDate < 60)
	{
		i_nSerialDate++;
	}
	else
	{//DTEC_Else
	}

	int l = i_nSerialDate + 68569 + 2415019;
	int n = int(( 4 * l ) / 146097 );
		l = l - int((146097 * n + 3) / 4);
	int i = int((4000 * (l + 1)) / 1461001);
		l = l - int((1461 * i) / 4) + 31;
	int j = int((80 * l) / 2447 );
	i_nDay = l - int ((2447 * j) / 80);
		l = int(j / 11);
		i_nMonth = j + 2 - (12 * l);
	i_nYear = 100 * (n - 49) + i + l;
}

long CXlSimpleAutomation::GetRowNum()
{
	long lCount = 0;

	if (m_pdispWorksheet !=nullptr)
	{
		VARIANTARG vargRng = VARIANTARG();
		ExlInvoke(m_pdispWorksheet, L"UsedRange", &vargRng, DISPATCH_PROPERTYGET, 0);
		//if (ExlInvoke(m_pdispWorksheet, L"UsedRange", &vargRng, DISPATCH_PROPERTYGET, 0))
		//{
			VARIANTARG vargRows;
			if (ExlInvoke(vargRng.pdispVal, L"Rows", &vargRows, DISPATCH_PROPERTYGET, 0))
			{
				VARIANTARG vargCount;
				if (ExlInvoke(vargRows.pdispVal, L"Count", &vargCount, DISPATCH_PROPERTYGET, 0))
				{
					lCount = vargCount.intVal;
				}

				ReleaseVariant(&vargRows);
			}

			ReleaseVariant(&vargRng);
		//}
	}

	return lCount;
}

long CXlSimpleAutomation::GetColumnNum()
{
	long lCount = 0;

	if (m_pdispWorksheet !=nullptr)
	{
		VARIANTARG vargRng = VARIANTARG();
		ExlInvoke(m_pdispWorksheet, L"UsedRange", &vargRng, DISPATCH_PROPERTYGET, 0);
		//if (ExlInvoke(m_pdispWorksheet, L"UsedRange", &vargRng, DISPATCH_PROPERTYGET, 0))
		//{
			VARIANTARG vargColumn = VARIANTARG();
			if (ExlInvoke(vargRng.pdispVal, L"Columns", &vargColumn, DISPATCH_PROPERTYGET, 0))
			{
				VARIANTARG vargCount = VARIANTARG();
				if (ExlInvoke(vargColumn.pdispVal, L"Count", &vargCount, DISPATCH_PROPERTYGET, 0))
				{
					lCount = vargCount.intVal;
				}

				ReleaseVariant(&vargColumn);
			}

			ReleaseVariant(&vargRng);
		//}
	}

	return lCount;
}

BOOL CXlSimpleAutomation::SaveAsFile( LPCTSTR i_lpszFileName, XlFormat i_format /*= XLS*/ )
{
	BOOL bSucceed = FALSE;

	if (m_pdispWorksheet !=nullptr && i_lpszFileName !=nullptr  && lstrlen(i_lpszFileName) > 0)
	{
		if (PathFileExists(i_lpszFileName) ==TRUE)		// 파일이 이미 존재한다면 삭제 후 새로 생성.
		{							
			DeleteFile(i_lpszFileName);
		}
		ClearAllArgs();
		AddArgumentCString(L"Filename", 0, i_lpszFileName);
		AddArgumentInt2(L"FileFormat", 0, i_format);
		bSucceed = ExlInvoke(m_pdispWorksheet, L"SaveAs", NULL, DISPATCH_METHOD, DISP_FREEARGS);
	}

	return bSucceed;
}

BOOL CXlSimpleAutomation::SaveAsFile( LPCTSTR i_lpszFileName, CString i_strFileExt )
{
	XlFormat eXlFormat;
	if (i_strFileExt.IsEmpty() ==false)
	{
		if (i_strFileExt.CompareNoCase(_T("xls")) == 0)
		{
			eXlFormat = XLS;
		}
		else if (i_strFileExt.CompareNoCase(_T("xlsx")) == 0)
		{
			eXlFormat = XLSX;
		}
		else if (i_strFileExt.CompareNoCase(_T("xlsm")) == 0)
		{
			eXlFormat = XLSM;
		}
		else if (i_strFileExt.CompareNoCase(_T("xlsb")) == 0)
		{
			eXlFormat = XLSB;
		}
		else
		{
			eXlFormat = XLS;
		}
	}
	else
	{
		eXlFormat = XLS;
	}


	return SaveAsFile(i_lpszFileName, eXlFormat);
}


