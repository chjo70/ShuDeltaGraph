#pragma once

#include <map>

using namespace std;

#include "_iq.h"
#include "_pdw.h"
#include "_macro.h"

#include "../FFTW/fftw3.h"

#define			PDW_ITEMS						(1024*128)			// 9437164
#define			IQ_ITEMS						(1024*128)

#define			MAX_RAWDATA_SIZE				_max( (sizeof(SRxPDWHeader) + sizeof(SRXPDWDataRGroup)*PDW_ITEMS), sizeof(TNEW_IQ)*IQ_ITEMS )	// 2,432,052

#define	RAD2DEG			(180./M_PI)		// 57.295779513082320876798154814114

typedef enum {
	enUnselectedSubGraph = -1,

	enSubMenu_1 = 1,
	enSubMenu_2,
	enSubMenu_3,
	enSubMenu_4,
	enSubMenu_5,
	enSubMenu_6,


} ENUM_SUB_GRAPH ;

typedef enum {
	en_UnknownData = 0,

	en_PDW_DATA,
	en_IQ_DATA,
	en_IF_DATA,

} ENUM_DataType;

typedef enum {
	en_UnknownUnit = 0,

	en_SONATA,
	en_SONATA_SHU,
	en_ELINT,
	en_701,
	en_KFX

} ENUM_UnitType;

#define MAX_SONATA_DATA		(65536)
typedef union {
	TNEW_PDW stPDWData[MAX_SONATA_DATA];
	TNEW_IQ stIQData[MAX_SONATA_DATA];

} UNI_SONATA_DATA;

typedef struct {
	UINT uiItem;
	ENUM_DataType enDataType;

	UNI_SONATA_DATA unRawData;

	UINT uiNo;

} STR_SONATA_DATA;


typedef struct {
	UINT uiByte;
	void *pDataBuffer;

	UINT uiDataItems;
	ENUM_DataType enDataType;
	ENUM_UnitType enUnitType;
	char szFileName[MAX_PATH];

} STR_RAWDATA ;

typedef unsigned long long int _TOA;

typedef struct {
	int iDataItems;

	float *pfFreq;			// [KHz]
	float *pfPW;			// [ns]
	float *pfAOA;			// [degree]
	float *pfTOA;
	float *pfPA;
	float *pfDTOA;

	_TOA *pllTOA;

	char *pcType;			// [신호형태]
	char *pcDV;				// [DV]

	float *pfPh1;
	float *pfPh2;
	float *pfPh3;
	float *pfPh4;

} STR_PDW_DATA ;

typedef struct {
	int iDataItems;

	float *pfI;
	float *pfQ;
	float *pfPA;
	float *pfIP;
	float *pfFFT;

} STR_IQ_DATA ;

typedef struct {
	double dToaMin;
	double dToaMax;
	double dDtoaMin;
	double dDtoaMax;
	double dAoaMin;
	double dAoaMax;
	double dFrqMin;
	double dFrqMax;
	double dPAMin;
	double dPAMax;
	double dPWMin;
	double dPWMax;

	ENUM_SUB_GRAPH enSubGraph;

} STR_FILTER_SETUP;

typedef struct {
	double dZoomMinX;
	double dZoomMaxX;
	double dZoomMinY;
	double dZoomMaxY;

	ENUM_SUB_GRAPH enSubGraph;

} STR_ZOOM_INFO;

static char *gstpRawDataBuffer;

class CData
{
public:
	UINT m_uiWindowNumber;

	STR_RAWDATA m_RawData;

	bool m_bPhaseData;

	_TOA m_ll1stToa;

private:

public:
	void swapByteOrder(unsigned int& ui);
	void swapByteOrder(unsigned long long& ull);
	void AllSwapData32( void *pData, int iLength );
	void AllSwapData64( void *pData, int iLength );
	void ExecuteFFT( int iDataItems, STR_IQ_DATA *pIQData );

public:
	CData(STR_RAWDATA *pRawData);
	virtual ~CData();

	virtual void Alloc( int nItems=0 )=0;
	virtual void Free()=0;
	virtual void ReadDataHeader() = 0;
	virtual void ConvertArray( int iDataItems, int iOffset=0 ) = 0;
	virtual void *GetData() = 0;
	virtual void *GetHeader() = 0;
};

class CPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	CPDW(STR_RAWDATA *pRawData);
	virtual ~CPDW();

	void Alloc( int nItems=0 );
	void Free();
	void ReadDataHeader() {  }
	void ConvertArray( int iDataItems, int iOffset=0 );
	void *GetData();
	void *GetHeader() { return NULL; }

};

// 수퍼헷 PDW 데이터
class CSPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	CSPDW(STR_RAWDATA *pRawData);
	virtual ~CSPDW();

	void Alloc( int iItems=0 );
	void Free();
	void ReadDataHeader() {  }
	void ConvertArray( int iDataItems, int iOffset=0 );
	void *GetData();
	void *GetHeader() { return NULL; }
};

// 인천공항 PDW
class CEPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

	ENUM_BANDWIDTH m_enBandWidth;

public:
	CEPDW(STR_RAWDATA *pRawData );
	virtual ~CEPDW();

	void Alloc( int iItems=0 );
	void Free();
	void ReadDataHeader() {  }
	void ConvertArray( int iDataItems, int iOffset=0 );
	void *GetData();
	void *GetHeader() { return NULL; }

};

// 701 PDW
class C7PDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	C7PDW(STR_RAWDATA *pRawData, STR_FILTER_SETUP *pstFilterSetup );
	virtual ~C7PDW();

	void Alloc( int iItems=0 );
	void Free();
	void ReadDataHeader() {  }
	void ConvertArray( int iDataItems, int iOffset=0 );
	void *GetData();
	void *GetHeader() { return NULL; }
};

// 701 IQ
class C7IQ : public CData
{
private:
	STR_IQ_DATA m_IQData;

public:
	C7IQ(STR_RAWDATA *pRawData );
	virtual ~C7IQ();

	void Alloc( int iItems=0 );
	void Free();
	void ReadDataHeader() {  }
	void ConvertArray( int iDataItems, int iOffset=0 );
	void *GetData();
	void *GetHeader() { return NULL; }
};

// KFX PDW
class CKFXPDW : public CData
{
private:
	STR_PDW_DATA m_PDWData;

public:
	CKFXPDW(STR_RAWDATA *pRawData, STR_FILTER_SETUP *pstFilterSetup );
	virtual ~CKFXPDW();

	void Alloc(int iItems=0);
	void Free();
	void ReadDataHeader() {  }
	void ConvertArray( int iDataItems, int iOffset=0 );
	void *GetData();
	void *GetHeader() { return NULL; }

public:
	float DecodeDOA(int iDOA  ) 
	{
		float fDOA;

		fDOA = (float) ( (float) iDOA * (float) 360. ) / (float) 512;
		return fDOA;	/* [degree] */
	} ;

	float DecodePA(int iPA ) 
	{
		float fPA;

		fPA = (float) ( (float) iPA * (float) 0.25 ) - (float) 110.0;
		return fPA;		/* [dBm] */
	} ;

	float DecodeFREQ( int iFreq ) 
	{
		float fFREQ;

		fFREQ = (float) ( (float) iFreq * (float) 13.1072 ) / (float) 1000.0;
		return fFREQ;	/* [MHz] */
	} ;

	float DecodePW( int iPW ) 
	{
		float fPW;

		fPW = (float) ( (float) iPW * (float) 6.48824007 / (float) 1.0 );
		return fPW;		/* [ns] */
	} ;

	float DecodeTOA( _TOA iTOA  ) 
	{
		float fTOA;

		fTOA = (float) ( (float) iTOA * (float) 6.48824007 / (float) 1000.0 );
		return fTOA;	/* [ns] */
	} ;

};

class CIQ : public CData
{
private:
	STR_IQ_HEADER m_IQHeader;
	STR_IQ_DATA m_IQData;

public:
	CIQ(STR_RAWDATA *pRawData);
	virtual ~CIQ();

	void Alloc( int iItems=0 );
	void Free();
	void ReadDataHeader();
	void ConvertArray( int iDataItems, int iOffset );
	void ConvertArrayForELINT() { }
	void *GetData();
	void *GetHeader();


};

class CEIQ : public CData
{
private:
	STR_IQ_DATA m_IQData;

public:
	CEIQ(STR_RAWDATA *pRawData);
	virtual ~CEIQ();

	void Alloc( int iItems=0 );
	void Free();
	void ReadDataHeader() {  }
	void ConvertArray( int iDataItems, int iOffset );
	void ConvertArrayForELINT() { }
	void *GetData();
	void *GetHeader() { return NULL; }

};

class CMapData {
private:
	bool m_bMapData;

	static map<CString, CData *> m_gMapData;

public:
	CMapData(void)
	{

	}

	virtual ~CMapData(void)
	{

	}

	void IncWindowNumber( CData *pData )
	{
		++ pData->m_uiWindowNumber;
	}

	void AddMapData( CString *pStrPathName, CData *pData )
	{
		Log( enNormal, _T("\n MapData()에 경로명[%s]을 추가했습니다.") , *pStrPathName );
		m_gMapData.insert( make_pair( *pStrPathName, pData ) );

	}

	CData *FindMapData( CString *pStrPathName )
	{
		CData *pData;
		map<CString, CData *>::iterator it;

		it = m_gMapData.find( *pStrPathName );
		if( it == m_gMapData.end() ) {
			pData = NULL;
		}
		else {
			pData = it->second;

			// 데이터 ID 증가
			//++ pData->m_uiLoadFile;
		}

		return pData;
	}

	void CloseMapData( CString *pStrWindowTitle )
	{
		auto it=m_gMapData.begin();

		if( pStrWindowTitle == NULL ) {
			while( it != m_gMapData.end() ) {
				//it->second->Free();
				delete it->second;

				++ it;
			}
			m_gMapData.clear();
		}
		else {
			CData *pData;

			while( it != m_gMapData.end() ) {
				if( pStrWindowTitle->Compare( it->first ) == 0 ) {
					pData = it->second;

					if( pData->m_uiWindowNumber == 0 ) {
						delete it->second;
						m_gMapData.erase( it++ );
					}
					else {
						++ it;

						// 데이터 ID 감소
						-- pData->m_uiWindowNumber;
					}
					
				}
				else {
					++ it;
				}
			}
		}
	}

};

class CDataFile
{
private:
	int m_iFileIndex;
	ULONGLONG m_dwFileEnd;
	CFile m_RawDataFile;

	//STR_RAWDATA m_RawData;
	CData *m_pData;

	CString m_strPathname;

public:
	CDataFile(void);
	virtual ~CDataFile(void);

	UINT LoadRawData( CData *pData, int iFileIndex, UINT uiHeaderLength, UINT uiLengthOf1RawData );

	CData *ReadDataFile( CString & strPathname, int iFileIndex=0, CData *pData=NULL, STR_FILTER_SETUP *pstFilterSetup=NULL );
	void SaveDataFile( CString & strPathname, void *pData, int iNumData, ENUM_UnitType enUnitType, ENUM_DataType enDataType, void *pDataEtc=NULL, int iSizeOfEtc=0 );
	void Alloc();
	void Free();
	void *GetData();
	void *GetHeader();
	void SetData( CData *pData );

	ENUM_DataType WhatDataType( CString *pStrPathname );
	ENUM_UnitType WhatUnitType( CString *pStrPathname );

	inline int GetFileIndex() { return m_iFileIndex; }

	inline UINT GetDataItems() { if( m_pData != NULL ) return m_pData->m_RawData.uiDataItems; else return 0; }
	inline ENUM_UnitType GetUnitType() { return m_pData->m_RawData.enUnitType; }
	inline ENUM_DataType GetDataType() { return m_pData->m_RawData.enDataType; }
	inline UINT GetWindowNumber() { if( m_pData != NULL ) return m_pData->m_uiWindowNumber; else return 0; }
	inline CData *GetRawData() { if( m_pData != NULL ) return m_pData; else return NULL; }
	//inline STR_FILTER_SETUP *GetFilterSetup() { return & m_pData->m_stFilterSetup; }
	//inline void ClearFilterSetup() { m_pData->ClearFilterSetup(); }
	inline UINT GetFilteredDataItems() { STR_PDW_DATA *pPDWData=(STR_PDW_DATA *) m_pData->GetData(); return pPDWData->iDataItems; }

	inline bool IsPhaseData() { return m_pData->m_bPhaseData; }

	//void CloseMapData( CString *pStrPathname );// { return m_theMapData.CloseMapData( pStrPathname ); }
	
private:
	
	
};

