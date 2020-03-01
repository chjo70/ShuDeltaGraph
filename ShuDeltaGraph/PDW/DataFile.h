#pragma once

#define _SONATA_

#include "_iq.h"
#include "_pdw.h"
#include "_macro.h"

#include "../FFTW/fftw3.h"

#define			MAX_RAWDATA_SIZE				(4000000)	// 2,432,052

typedef enum {
	en_UnknownData = 0,

	en_PDW_DATA,
	en_IQ_DATA

} ENUM_DataType;

typedef enum {
	en_UnknownUnit = 0,

	en_SONATA,
	en_ELINT,
	en_701

} ENUM_UnitType;

#ifdef _SONATA_
#define MAX_SONATA_DATA		(10000)
typedef union {
	TNEW_PDW stPDWData[MAX_SONATA_DATA];
	TNEW_IQ stIQData[MAX_SONATA_DATA];

} UNI_SONATA_DATA;

typedef struct {
	UINT uiItem;
	ENUM_DataType enDataType;

	UNI_SONATA_DATA unRawData;

} STR_SONATA_DATA;
#else
#endif

typedef struct {
	UINT uiByte;
	void *pDataBuffer;

	UINT uiDataItems;
	ENUM_DataType enDataType;
	ENUM_UnitType enUnitType;
	char szFileName[MAX_PATH];

} STR_RAWDATA ;

typedef long long int _TOA;

// typedef struct {
// 	long long int llTOA;
// 	int iFreq;
// 	int iPulseType;
// 	int iPA;
// 	int iPW;
// 	int iPFTag;
// 	int iAOA;
// 
// } _PDW;

// typedef enum {
// 	en50MHZ_BW=0,
// 	en5MHZ_BW 
// 
// } ENUM_BANDWIDTH ;

// #define LENGTH_OF_TASK_ID			(19+1)		//과제ID 문자열 길이 (TBD)
// #define _MAX_PDW							(4096)
// typedef struct {
// 	unsigned char aucTaskID[LENGTH_OF_TASK_ID];
// 	unsigned int iIsStorePDW;
// 	int iCollectorID;
// 	ENUM_BANDWIDTH enBandWidth;
// 
// 	UINT count;
// 	_PDW stPDW[_MAX_PDW];
// 
// } STR_PDWDATA ;

typedef struct {
	float *pfFreq;			// [KHz]
	float *pfPW;			// [ns]
	float *pfAOA;			// [degree]
	float *pfTOA;
	float *pfPA;
	float *pfDTOA;

	_TOA *pfllTOA;

	char *pcType;			// [신호형태]
	char *pcDV;				// [DV]

	float *pfPh1;
	float *pfPh2;
	float *pfPh3;
	float *pfPh4;

} STR_PDW_DATA ;

typedef struct {
	float *pfI;
	float *pfQ;
	float *pfPA;
	float *pfIP;
	float *pfFFT;

} STR_IQ_DATA ;

typedef struct {
	double dToaMin;
	double dToaMax;
	double dAoaMin;
	double dAoaMax;
	double dFrqMin;
	double dFrqMax;
	double dPaMin;
	double dPaMax;

} STR_FILTER_SETUP;

static char *gstpRawDataBuffer;

class CData
{
public:
	STR_RAWDATA *m_pRawData;

	UINT m_uiWindowNumber;

	bool m_bPhaseData;

	STR_FILTER_SETUP m_stFilterSetup;

public:
	void swapByteOrder(unsigned int& ui);
	void swapByteOrder(unsigned long long& ull);
	void AllSwapData32( void *pData, int iLength );

public:
	CData(STR_RAWDATA *pRawData);
	virtual ~CData();

	virtual void Alloc()=0;
	virtual void Free()=0;
	virtual void ConvertArray() = 0;
	virtual void *GetData() = 0;
};

class CPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	CPDW(STR_RAWDATA *pRawData);
	virtual ~CPDW();

	void Alloc();
	void Free();
	void ConvertArray();
	void *GetData();

};

// 수퍼헷 PDW 데이터
class CSPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	CSPDW(STR_RAWDATA *pRawData);
	virtual ~CSPDW();

	void Alloc();
	void Free();
	void ConvertArray();
	void *GetData();

};

// 인천공항 PDW
class CEPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	CEPDW(STR_RAWDATA *pRawData, STR_FILTER_SETUP *pstFilterSetup );
	virtual ~CEPDW();

	void Alloc();
	void Free();
	void ConvertArray();
	void *GetData();

};

// 701 PDW
class C7PDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	C7PDW(STR_RAWDATA *pRawData, STR_FILTER_SETUP *pstFilterSetup );
	virtual ~C7PDW();

	void Alloc();
	void Free();
	void ConvertArray();
	void *GetData();

};

class CIQ : public CData
{
private:
	STR_IQ_DATA m_IQData;

public:
	CIQ(STR_RAWDATA *pRawData);
	virtual ~CIQ();

	void Alloc();
	void Free();
	void ConvertArray();
	void ConvertArrayForELINT() { }
	void *GetData();

};

class CDataFile
{
private:
	CFile m_RawDataFile;

	STR_RAWDATA m_RawData;
	CData *m_pData;

public:
	CDataFile(void);
	virtual ~CDataFile(void);

	void ReadDataFile( CString & strPathname, STR_FILTER_SETUP *pstFilterSetup=NULL );
	void SaveDataFile( CString & strPathname, void *pData, int iNumData, ENUM_UnitType enUnitType, ENUM_DataType enDataType, void *pDataEtc=NULL, int iSizeOfEtc=0 );
	void Alloc();
	void Free();
	void *GetData();
	void SetData( CData *pData );

	inline UINT GetDataItems() { if( m_pData != NULL ) return m_pData->m_pRawData->uiDataItems; else return 0; }
	inline ENUM_UnitType GetUnitType() { return m_pData->m_pRawData->enUnitType; }
	inline ENUM_DataType GetDataType() { return m_pData->m_pRawData->enDataType; }
	inline UINT GetWindowNumber() { if( m_pData != NULL ) return m_pData->m_uiWindowNumber; else return 0; }
	inline CData *GetRawData() { if( m_pData != NULL ) return m_pData; else return NULL; }
	
	inline bool IsPhaseData() { return m_pData->m_bPhaseData; }
	
private:
	
	
};

