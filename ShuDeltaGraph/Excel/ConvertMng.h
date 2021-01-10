#pragma once

class /* AFX_EXT_CLASS */ CConvertMng
{
public:
	CConvertMng(void);
	~CConvertMng(void);

	static UINT ConvertDoubleToUINT(double Value);
	static WORD ConvertUINTToWORD(UINT Value);
	static WORD ConvertIntToWORD(int Value);
	static UINT Convert__int64ToUINT(__int64 Value);
	static int Convert__int64ToInt(__int64 Value);
	static size_t Convert__int64ToSize_t(__int64 Value);
	static double Convert__int64ToDouble(__int64 Value);
	static int ConvertUINT64ToInt(unsigned __int64 Value);
	static __int64 ConvertDoubleTo__int64(double Value);
	static int ConvertDoubleToInt(double Value);
	static float ConvertDoubleToFloat(double Value);
	static UCHAR ConvertDoubleToUCHAR(double Value);
	static USHORT ConvertDoubleToUSHORT(double Value);
	static short ConvertDoubleToShort(double Value);
	static UCHAR ConvertUINTToUCHAR(UINT Value);
	static char ConvertUINTToChar(UINT Value);
	static USHORT ConvertUINTToUSHORT(UINT Value);
	static UINT Convert__time64_tToUINT(__time64_t Value);
	static DWORD ConvertDoubleToDWORD(double Value);
	static int ConvertDoubleToint32_T(double Value);
	static float ConvertDoubleToReal32_T(double Value);
	static char ConvertFloatToChar(float Value);
	static UCHAR ConvertFloatToUCHAR(float Value);
	static int ConvertFloatToInt(float Value);
	static UINT ConvertFloatToUINT(float Value);
	static short ConvertFloatToShort(float Value);
	static USHORT ConvertFloatToUSHORT(float Value);
	static char ConvertIntToChar(int Value);
	static float ConvertIntToFloat(int Value);
	static float ConvertUINTToFloat(UINT Value);
	static short ConvertIntToShort(int Value);
	static short ConvertUINTToShort(UINT Value);
	static USHORT ConvertIntToUSHORT(int Value);
	static UCHAR ConvertIntToUCHAR(int Value);	
	static USHORT ConvertLongToUSHORT(long Value);
	static int ConvertLONG64Toint(LONG64 Value);
	static UINT ConvertLONG64ToUINT(LONG64 Value);
	static UINT ConvertLONGLONGToUINT(long long Value);
	static int ConvertLONGLONGToint(long long Value);
	static UINT ConvertLONGToUINT(LONG Value);
	static UINT ConvertReal_TToUINT(double Value);
	static int ConvertReal_TToInt(double Value);
	static char ConvertShortToChar(short Value);
	static UCHAR ConvertShortToUCHAR(short Value);
	static UCHAR ConvertUSHORTToUCHAR(USHORT Value);
	static UINT ConvertTime_tToUINT(time_t Value);
	static byte ConvertUINTToBYTE(UINT Value);
	static byte ConvertIntToBYTE(int Value);
	static byte ConvertBOOLToBYTE(BOOL Value);
	static int ConvertUnsigned__int64ToInt(unsigned __int64 Value);
	static UINT ConvertUnsigned__int64ToUINT(unsigned __int64 Value);
	static INT_PTR ConvertUINT64ToINT_PTR(UINT64 Value);

	//추가됨.
	static size_t ConvertLONG64ToSize_t(LONG64 Value);
	static int ConvertReal_tToInt(double Value);
	static UINT ConvertUINT64ToUINT(UINT64 Value);
	static size_t ConvertLONGLONGToSize_t(LONGLONG Value);
	static double ConvertUINT64ToDouble(UINT64 Value);
	static double ConvertUSHORTToDouble(USHORT Value);

	//sign / UnSign 간 대입 연살할때
	static int ConvertUINTToInt(UINT Value);
	static UINT ConvertIntToUINT(int Value);
	static long ConvertULONGTolong(ULONG Value);
	static ULONG ConvertlongToULONG(long Value);
	static short ConvertUSHORTToShort(USHORT Value);
	static USHORT ConvertshortToUSHORT(short Value);
	static char ConvertUCHARTToChar(UCHAR Value);
	static UCHAR ConvertsCharToUCHAR(char Value);
	static short ConvertWORDToShort(WORD Value);
	static int ConvertWORDToInt(WORD Value);
	static WORD ConvertShortToWORD(short Value);	
	static long ConvertDWORDToLong(DWORD Value);
	static int ConvertDWORDToInt(DWORD Value);
	static short ConvertDWORDToShort(DWORD Value);
	static char ConvertDWORDToChar(DWORD Value);
	static DWORD ConvertLongToDWORD(long Value);
	static DWORD ConvertIntToDWORD(int Value);
	static DWORD ConvertShortToDWORD(short Value);
	static DWORD ConvertcharToDWORD(char Value);

	static UCHAR ConvertSigned(char Value);
	static char ConvertSigned(UCHAR Value);
	static USHORT ConvertSigned(short Value);
	static short ConvertSigned(USHORT Value);
	static ULONG ConvertSigned(long Value);
	static long ConvertSigned(ULONG Value);
	static UINT ConvertSigned(int Value);
	static int ConvertSigned(UINT Value);
	static ULONGLONG ConvertSigned(LONGLONG Value);
	static LONGLONG ConvertSigned(ULONGLONG Value);

	static int GetCalcValue(int nValue1, int nValue2, CString sCalcType);

	//실수형 비교
	static bool	CheckFloatingValue(double dLeftVal, double dRightVal, CString strComparison);
	static bool	CheckFloatingValue(float fLeftVal, float fRightVal, CString strComparison);

	static UINT32 		GetCalcBitWise(UINT32 uiVal, INT32 iVal, CString sBitWiseType);	
	static INT32 		GetCalcBitWise(INT32 uiVal, INT32 iVal, CString sBitWiseType);	
	static UINT64 		GetCalcBitWise(UINT64 uiVal, INT64 iVal, CString sBitWiseType);
};

