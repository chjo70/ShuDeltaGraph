#include "StdAfx.h"
#include "ConvertMng.h"
#include <limits>
#include <intsafe.h>

#define DF_EPSILON_FLOAT		0.00000001						//Float type accuracy		1.0E-8
#define DF_EPSILON_DOUBLE		0.000000000000000001			//Double type accuracy		1.0E-18

CConvertMng::CConvertMng(void)
{
}


CConvertMng::~CConvertMng(void)
{
}


UINT CConvertMng::ConvertDoubleToUINT(double Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}

	return ReturnValue;
}
WORD CConvertMng::ConvertUINTToWORD(UINT Value)
{
	WORD ReturnValue = 0;
	if(Value <= USHORT_MAX)
	{
		ReturnValue = (WORD)Value;
	}
	return ReturnValue;
}
WORD CConvertMng::ConvertIntToWORD(int Value)
{
	WORD ReturnValue = 0;
	if(Value >= 0 && Value <= USHORT_MAX)
	{
		ReturnValue = (WORD)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::Convert__int64ToUINT(__int64 Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0)
	{
		if(Value <= UINT_MAX)
		{
			ReturnValue = (UINT)Value;
		}
		else
		{
			ReturnValue = UINT_MAX;
		}
	}
	return ReturnValue;
}
int CConvertMng::Convert__int64ToInt(__int64 Value)
{
	int ReturnValue = 0;
	if(Value >= INT_MIN && Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
double CConvertMng::Convert__int64ToDouble(__int64 Value)
{
	double ReturnValue = 0;
	if(Value >= DBL_MIN && Value <= DBL_MAX)
	{
		ReturnValue = (double)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertUINT64ToInt(unsigned __int64 Value)
{
	int ReturnValue = 0;
	if(Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
size_t CConvertMng::Convert__int64ToSize_t(__int64 Value)
{
	size_t ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (size_t)Value;
	}
	return ReturnValue;
}
__int64 CConvertMng::ConvertDoubleTo__int64(double Value)
{
	__int64 ReturnValue = 0;
	if(Value >= INT64_MIN && Value <= INT64_MAX)
	{
		ReturnValue = (__int64)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertDoubleToInt(double Value)
{
	int ReturnValue = 0;
	if(Value >= INT_MIN && Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
float CConvertMng::ConvertDoubleToFloat(double Value)
{
	float ReturnValue = 0.0f;
	if(Value > FLT_MIN && Value < FLT_MAX)
	{
		ReturnValue = (float)Value;
	}
	return ReturnValue;
}
UCHAR CConvertMng::ConvertDoubleToUCHAR(double Value)
{
	UCHAR ReturnValue = 0;
	if(Value >= 0 && Value <= UCHAR_MAX)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}
USHORT CConvertMng::ConvertDoubleToUSHORT(double Value)
{
	USHORT ReturnValue = 0;
	if(Value >= 0 && Value <= USHORT_MAX)
	{
		ReturnValue = (USHORT)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertDoubleToShort(double Value)
{
	short ReturnValue = 0;
	if(Value >= SHORT_MIN && Value <= SHORT_MAX)
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
UCHAR CConvertMng::ConvertUINTToUCHAR(UINT Value)
{
	UCHAR ReturnValue = 0;
	if(Value <= UCHAR_MAX)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}
char CConvertMng::ConvertUINTToChar(UINT Value)
{
	char ReturnValue = 0;
	if(Value <= CHAR_MAX)
	{
		ReturnValue = (char)Value;
	}
	return ReturnValue;
}
USHORT CConvertMng::ConvertUINTToUSHORT(UINT Value)
{
	USHORT ReturnValue = 0;
	if(Value <= USHORT_MAX)
	{
		ReturnValue = (USHORT)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::Convert__time64_tToUINT(__time64_t Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
DWORD CConvertMng::ConvertDoubleToDWORD(double Value)
{
	DWORD ReturnValue = 0;
	if(Value >= 0 && Value <= ULONG_MAX)
	{
		ReturnValue = (DWORD)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertDoubleToint32_T(double Value)
{
	int ReturnValue = 0;
	if(Value >= INT_MIN && Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
float CConvertMng::ConvertDoubleToReal32_T(double Value)
{
	float ReturnValue = 0.0f;
	if(Value >= FLT_MIN && Value <= FLT_MAX)
	{
		ReturnValue = (float)Value;
	}
	return ReturnValue;
}
char CConvertMng::ConvertFloatToChar(float Value)
{
	char ReturnValue = 0;
	if(Value >= CHAR_MIN && Value <= CHAR_MAX)
	{
		ReturnValue = (char)Value;
	}
	return ReturnValue;
}
UCHAR CConvertMng::ConvertFloatToUCHAR(float Value)
{
	UCHAR ReturnValue = 0;
	if(Value >= 0 && Value <= UCHAR_MAX)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertFloatToInt(float Value)		//170901 ±èÁ¾¿í ÁÖ¼®
{
	int ReturnValue = 0;
	//if(Value >= FLT_MIN && Value <= FLT_MAX)
	//{
		ReturnValue = (int)Value;
	//}
	return ReturnValue;
}
UINT CConvertMng::ConvertFloatToUINT(float Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertFloatToShort(float Value)
{
	short ReturnValue = 0;
	if(Value >= SHORT_MIN && Value <= SHORT_MAX)
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
USHORT CConvertMng::ConvertFloatToUSHORT(float Value)
{
	USHORT ReturnValue = 0;
	if(Value >= 0 && Value <= USHORT_MAX)
	{
		ReturnValue = (USHORT)Value;
	}
	return ReturnValue;
}
char CConvertMng::ConvertIntToChar(int Value)
{
	char ReturnValue = 0;
	if(Value >= CHAR_MIN && Value <= CHAR_MAX)
	{
		ReturnValue = (char)Value;
	}
	return ReturnValue;
}
float CConvertMng::ConvertIntToFloat(int Value)	//170901 ±èÁ¾¿í ÁÖ¼®
{
	float ReturnValue = 0;
	//if(Value >= FLT_MIN && Value <= FLT_MAX)
	//{
		ReturnValue = (float)Value;
	//}
	return ReturnValue;
}
float CConvertMng::ConvertUINTToFloat(UINT Value)
{
	float ReturnValue = 0;
	if(Value <= FLT_MAX)
	{
		ReturnValue = (float)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertIntToShort(int Value)
{
	short ReturnValue = 0;
	if(Value >= SHORT_MIN && Value <= SHORT_MAX)
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertUINTToShort(UINT Value)
{
	short ReturnValue = 0;
	if(Value <= SHORT_MAX)
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
USHORT CConvertMng::ConvertIntToUSHORT(int Value)
{
	USHORT ReturnValue = 0;
	if(Value >= 0 && Value <= USHORT_MAX)
	{
		ReturnValue = (USHORT)Value;
	}
	return ReturnValue;
}
/*
TCHAR CConvertMng::ConvertIntToTCHAR(int Value)
{
	TCHAR ReturnValue = 0;
	return ReturnValue;
}
*/
UCHAR CConvertMng::ConvertIntToUCHAR(int Value)
{
	UCHAR ReturnValue = 0;
	if(Value >= 0 && Value <= UCHAR_MAX)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}
USHORT CConvertMng::ConvertLongToUSHORT(long Value)
{
	USHORT ReturnValue = 0;
	if(Value >= 0 && Value <= USHORT_MAX)
	{
		ReturnValue = (USHORT)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertLONG64Toint(LONG64 Value)
{
	int ReturnValue = 0;
	if(Value >= INT_MIN && Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::ConvertLONG64ToUINT(LONG64 Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::ConvertLONGLONGToUINT(long long Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertLONGLONGToint(long long Value)
{
	int ReturnValue = 0;
	if(Value >= INT_MIN && Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::ConvertLONGToUINT(LONG Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::ConvertReal_TToUINT(double Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertReal_TToInt(double Value)
{
	int ReturnValue = 0;
	if(Value >= INT_MIN && Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
char CConvertMng::ConvertShortToChar(short Value)
{
	char ReturnValue = 0;
	if(Value >= CHAR_MIN && Value <= CHAR_MAX)
	{
		ReturnValue = (char)Value;
	}
	return ReturnValue;
}
UCHAR CConvertMng::ConvertUSHORTToUCHAR(USHORT Value)
{
	UCHAR ReturnValue = 0;
	if(Value <= /*UCHAR_MAX*/(USHORT)0xFF)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}
UCHAR CConvertMng::ConvertShortToUCHAR(short Value)
{
	UCHAR ReturnValue = 0;
	if(Value >= 0 && Value <= UCHAR_MAX)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}

UINT CConvertMng::ConvertTime_tToUINT(time_t Value)
{
	UINT ReturnValue = 0;
	if(Value >= 0 && Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
byte CConvertMng::ConvertUINTToBYTE(UINT Value)
{
	byte ReturnValue = 0;
	if(Value <= UCHAR_MAX)
	{
		ReturnValue = (byte)Value;
	}
	return ReturnValue;
}
byte CConvertMng::ConvertBOOLToBYTE(BOOL Value)
{
	byte ReturnValue = 0;
	if(Value <= UCHAR_MAX)
	{
		ReturnValue = (byte)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertUnsigned__int64ToInt(unsigned __int64 Value)
{
	int ReturnValue = 0;
	if(Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::ConvertUnsigned__int64ToUINT(unsigned __int64 Value)
{
	UINT ReturnValue = 0;
	if(Value <= UINT_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
INT_PTR CConvertMng::ConvertUINT64ToINT_PTR(UINT64 Value)
{
	INT_PTR ReturnValue = 0;
	if(Value <= INT64_MAX)
	{
		ReturnValue = (INT_PTR)Value;
	}
	return ReturnValue;
}

BYTE CConvertMng::ConvertIntToBYTE(int Value)
{
	BYTE ReturnValue = 0;
	if(0 <= Value && Value <= UCHAR_MAX)
	{
		ReturnValue = (BYTE)Value;
	}
	return ReturnValue;
}

size_t CConvertMng::ConvertLONG64ToSize_t(LONG64 Value)
{
	size_t ret = 0;
	return ret;
}

int CConvertMng::ConvertReal_tToInt(double Value)
{
	int ReturnValue = 0;
	if(INT_MIN <= Value && Value <= INT_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
double CConvertMng::ConvertUSHORTToDouble(USHORT Value)
{
	double ReturnValue = 0;
	ReturnValue = (double)Value;
	return ReturnValue;
}

size_t CConvertMng::ConvertLONGLONGToSize_t(LONGLONG Value)
{
	size_t ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (size_t)Value;
	}
	return ReturnValue;
}

double CConvertMng::ConvertUINT64ToDouble(UINT64 Value)
{
	double ReturnValue = 0.0;
	if(Value <= DBL_MAX)
	{
		ReturnValue = (double)Value;
	}
	return ReturnValue;
}

UINT CConvertMng::ConvertUINT64ToUINT(UINT64 Value)
{
	UINT ReturnValue = 0;
	if(Value <= UINT32_MAX)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}

int CConvertMng::ConvertUINTToInt(UINT Value)
{
	int ReturnValue = 0;
	if(Value <= INT32_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::ConvertIntToUINT(int Value)
{
	UINT ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
long CConvertMng::ConvertULONGTolong(ULONG Value)
{
	long ReturnValue = 0;
	if(Value <= LONG_MAX)
	{
		ReturnValue = (long)Value;
	}
	return ReturnValue;
}
ULONG CConvertMng::ConvertlongToULONG(long Value)
{
	ULONG ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (ULONG)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertUSHORTToShort(USHORT Value)
{
	short ReturnValue = 0;
	if(Value <= /*SHORT_MAX*/(USHORT)32767)
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
USHORT CConvertMng::ConvertshortToUSHORT(short Value)
{
	USHORT ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (USHORT)Value;
	}
	return ReturnValue;
}
char CConvertMng::ConvertUCHARTToChar(UCHAR Value)
{
	char ReturnValue = 0;
	if(Value <= CHAR_MAX)
	{
		ReturnValue = (char)Value;
	}
	return ReturnValue;
}
UCHAR CConvertMng::ConvertsCharToUCHAR(char Value)
{
	UCHAR ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertWORDToShort(WORD Value)
{
	short ReturnValue = 0;
	if(Value <= /*SHORT_MAX*/(WORD)(32767))
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertWORDToInt(WORD Value)
{
	int ReturnValue = 0;
	ReturnValue = (int)Value;
	return ReturnValue;
}
WORD CConvertMng::ConvertShortToWORD(short Value)
{
	WORD ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (WORD)Value;
	}
	return ReturnValue;
}
long CConvertMng::ConvertDWORDToLong(DWORD Value)
{
	long ReturnValue = 0;
	if(Value <= LONG_MAX)
	{
		ReturnValue = (long)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertDWORDToInt(DWORD Value)
{
	int ReturnValue = 0;
	if(Value <= INT32_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertDWORDToShort(DWORD Value)
{
	short ReturnValue = 0;
	if(Value <= INT16_MAX)
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
char CConvertMng::ConvertDWORDToChar(DWORD Value)
{
	char ReturnValue = 0;
	if(Value <= CHAR_MAX)
	{
		ReturnValue = (char)Value;
	}
	return ReturnValue;
}
DWORD CConvertMng::ConvertLongToDWORD(long Value)
{
	DWORD ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (DWORD)Value;
	}
	return ReturnValue;
}
DWORD CConvertMng::ConvertIntToDWORD(int Value)
{
	DWORD ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (DWORD)Value;
	}
	return ReturnValue;
}
DWORD CConvertMng::ConvertShortToDWORD(short Value)
{
	DWORD ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (DWORD)Value;
	}
	return ReturnValue;
}
DWORD CConvertMng::ConvertcharToDWORD(char Value)
{
	DWORD ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (DWORD)Value;
	}
	return ReturnValue;
}

//
UCHAR CConvertMng::ConvertSigned(char Value)
{
	UCHAR ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (UCHAR)Value;
	}
	return ReturnValue;
}
char CConvertMng::ConvertSigned(UCHAR Value)
{
	char ReturnValue = 0;
	if(Value <= CHAR_MAX)
	{
		ReturnValue = (char)Value;
	}
	return ReturnValue;
}
USHORT CConvertMng::ConvertSigned(short Value)
{
	USHORT ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (USHORT)Value;
	}
	return ReturnValue;
}
short CConvertMng::ConvertSigned(USHORT Value)
{
	short ReturnValue = 0;
	if(Value <= /*SHORT_MAX*/(USHORT)32767)
	{
		ReturnValue = (short)Value;
	}
	return ReturnValue;
}
ULONG CConvertMng::ConvertSigned(long Value)
{
	ULONG ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (ULONG)Value;
	}
	return ReturnValue;
}
long CConvertMng::ConvertSigned(ULONG Value)
{
	long ReturnValue = 0;
	if(Value <= LONG_MAX)
	{
		ReturnValue = (long)Value;
	}
	return ReturnValue;
}
UINT CConvertMng::ConvertSigned(int Value)
{
	UINT ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (UINT)Value;
	}
	return ReturnValue;
}
int CConvertMng::ConvertSigned(UINT Value)
{
	int ReturnValue = 0;
	if(Value <= INT32_MAX)
	{
		ReturnValue = (int)Value;
	}
	return ReturnValue;
}
ULONGLONG CConvertMng::ConvertSigned(LONGLONG Value)
{
	ULONGLONG ReturnValue = 0;
	if(0 <= Value)
	{
		ReturnValue = (ULONGLONG)Value;
	}
	return ReturnValue;
}
LONGLONG CConvertMng::ConvertSigned(ULONGLONG Value)
{
	LONGLONG ReturnValue = 0;
	if(Value <= INT64_MAX)
	{
		ReturnValue = (LONGLONG)Value;
	}
	return ReturnValue;
}

bool CConvertMng::CheckFloatingValue(double dLeftVal, double dRightVal, CString strComparison)
{
	bool bRet = false;
	double dVal = 0.0;	

	if(strComparison.Compare(_T(">="))==0)
	{
		dVal = fabs(dLeftVal - dRightVal);
		if(dVal<DF_EPSILON_DOUBLE || dLeftVal>dRightVal)
		{
			bRet = true;
		}
	}
	else if(strComparison.Compare(_T("<="))==0)
	{
		dVal = fabs(dLeftVal - dRightVal);
		if(dVal<DF_EPSILON_DOUBLE || dLeftVal<dRightVal)
		{
			bRet = true;
		}
	}
	else if(strComparison.Compare(_T("=="))==0)
	{
		dVal = fabs(dLeftVal - dRightVal);
		if(dVal<DF_EPSILON_DOUBLE)
		{
			bRet = true;
		}
	}
	else if(strComparison.Compare(_T("!="))==0)
	{
		dVal = fabs(dLeftVal - dRightVal);
		if(dVal>DF_EPSILON_DOUBLE)
		{
			bRet = true;
		}
	}
	else
	{//DTEC_Else
	}

	return bRet;
}

bool CConvertMng::CheckFloatingValue(float fLeftVal, float fRightVal, CString strComparison)
{
	bool bRet = false;
	float fVal = 0.0f;	

	if(strComparison.Compare(_T(">="))==0)
	{
		fVal = fabs(fLeftVal - fRightVal);
		if(fVal<DF_EPSILON_FLOAT|| fLeftVal>fRightVal)
		{
			bRet = true;
		}
	}
	else if(strComparison.Compare(_T("<="))==0)
	{
		fVal = fabs(fLeftVal - fRightVal);
		if(fVal<DF_EPSILON_FLOAT || fLeftVal<fRightVal)
		{
			bRet = true;
		}
	}
	else if(strComparison.Compare(_T("=="))==0)
	{
		fVal = fabs(fLeftVal - fRightVal);
		if(fVal<DF_EPSILON_FLOAT)
		{
			bRet = true;
		}
	}
	else if(strComparison.Compare(_T("!="))==0)
	{
		fVal = fabs(fLeftVal - fRightVal);
		if(fVal>DF_EPSILON_FLOAT)
		{
			bRet = true;
		}
	}
	else
	{//DTEC_Else
	}

	return bRet;
}

int CConvertMng::GetCalcValue(int nValue1, int nValue2, CString sCalcType)
{
	int nRet = 0;
	if(sCalcType == "+")
	{
		if(INT32_MAX >= (INT64)(nValue1)+(INT64)(nValue2))
		{
			nRet = nValue1+nValue2;
		}
	}
	return nRet;
}


UINT32 	CConvertMng::GetCalcBitWise(UINT32 uiVal, INT32 iVal, CString sBitWiseType)
{
	UINT32 uiRet = 0;
	if(sBitWiseType == "|")
	{
		uiRet = uiVal | iVal;
	}
	else if (sBitWiseType == "&")
	{
 		uiRet = uiVal & iVal;
	}
	else if (sBitWiseType == ">>")
	{
		if(iVal > 0 && iVal < 31)
		{
			uiRet = uiVal >> iVal;
		}
		else
		{
			uiRet = uiVal;
		}
	}
	else if (sBitWiseType == "<<")
	{
		if(iVal > 0 && iVal < 31)
		{
			uiRet = uiVal << iVal;
		}
		else
		{
			uiRet = uiVal;
		}
	}
	else if (sBitWiseType == "^")
	{
		uiRet = uiVal ^ iVal;
	}
	else
	{
	}
	return uiRet;

}

INT32 	CConvertMng::GetCalcBitWise(INT32 uiVal, INT32 iVal, CString sBitWiseType)
{
	INT32 uiRet = 0;
	if(sBitWiseType == "|")
	{
		uiRet = uiVal | iVal;
	}
	else if (sBitWiseType == "&")
	{
		uiRet = uiVal & iVal;
	}
	else if (sBitWiseType == ">>")
	{
		if(iVal > 0 && iVal < 31)
		{
			uiRet = uiVal >> iVal;
		}
		else
		{
			uiRet = uiVal;
		}
	}
	else if (sBitWiseType == "<<")
	{
		if(iVal > 0 && iVal < 31)
		{
			uiRet = uiVal << iVal;
		}
		else
		{
			uiRet = uiVal;
		}
	}
	else if (sBitWiseType == "^")
	{
		uiRet = uiVal ^ iVal;
	}
	else
	{
	}
	return uiRet;

}

UINT64 	CConvertMng::GetCalcBitWise(UINT64 uiVal, INT64 iVal, CString sBitWiseType)
{
	UINT32 uiRet = 0;
	if(sBitWiseType == "|")
	{
		uiRet = uiVal | iVal;
	}
	else if (sBitWiseType == "&")
	{
		uiRet = uiVal & iVal;
	}
	else if (sBitWiseType == ">>")
	{
		if(iVal > 0 && iVal < 63)
		{
			uiRet = uiVal >> iVal;
		}
	}
	else if (sBitWiseType == "<<")
	{
		if(iVal > 0 && iVal < 63)
		{
			uiRet = uiVal << iVal;
		}		
	}
	else if (sBitWiseType == "^")
	{
		uiRet = uiVal ^ iVal;
	}
	else
	{
	}
	return uiRet;

}