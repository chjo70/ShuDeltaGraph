#pragma once

#define _SONATA_

#include "_iq.h"
#include "_pdw.h"
#include "_macro.h"

#include "../FFTW/fftw3.h"

#define			MAX_RAWDATA_SIZE				(1000000)

typedef enum {
	en_UnknownData = 0,

	en_PDW_DATA,
	en_IQ_DATA

} ENUM_DataType;

typedef enum {
	en_UnknownUnit = 0,

	en_SONATA,
	en_ELINT

} ENUM_UnitType;

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

} STR_PDW_DATA ;

typedef struct {
	float *pfI;
	float *pfQ;
	float *pfPA;
	float *pfIP;
	float *pfFFT;

} STR_IQ_DATA ;

static char *gstpRawDataBuffer;

class CData
{
public:
	STR_RAWDATA *m_pRawData;

	ENUM_DataType m_enDataType;
	ENUM_UnitType m_enUnitType;
	UINT m_uiDataItems;

	UINT m_uiWindowNumber;

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

class CEPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	CEPDW(STR_RAWDATA *pRawData);
	virtual ~CEPDW();

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

	void ReadDataFile( CString & strPathname);
	void Alloc();
	void Free();
	void *GetData();
	void SetData( CData *pData );

	inline UINT GetDataItems() { if( m_pData != NULL ) return m_pData->m_uiDataItems; else return 0; }
	inline ENUM_UnitType GetUnitType() { return m_pData->m_enUnitType; }
	inline ENUM_DataType GetDataType() { return m_pData->m_enDataType; }
	inline UINT GetWindowNumber() { if( m_pData != NULL ) return m_pData->m_uiWindowNumber; else return 0; }
	inline CData *GetRawData() { if( m_pData != NULL ) return m_pData; else return NULL; }
	
	
private:
	//CFile m_PDWFile;
	//void ReadDataFile( CString & strPathname);
};

