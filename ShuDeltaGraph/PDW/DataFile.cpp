#include "StdAfx.h"

#include <math.h>
#include <string.h>

#define _MAIN_

#include <math.h>
#include <float.h>
#include "DataFile.h"

#include "../Log/LogDebug.h"

#include "../CRWRCommonVariables.h"

static int stDataFile;

char stDV[2] = { ' ', '*' } ;

#define	RAD2DEG			(180./M_PI)		// 57.295779513082320876798154814114

//////////////////////////////////////////////////////////////////////////

CPDW::CPDW(STR_RAWDATA *pRawData) : CData(pRawData )
{
	Alloc( PDW_ITEMS );
}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 15:12:01
 * @warning   
 */
 CPDW::~CPDW(void)
{
	Free();
}

/**
 * @brief     
 * @param     int iItems
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 15:11:58
 * @warning   
 */
 void CPDW::Alloc( int iItems )
{
	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	m_PDWData.pfFreq = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPW = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfAOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfDTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pllTOA = (_TOA *)malloc(sizeof(_TOA) * iItems );

	m_PDWData.pcType = (char *)malloc(sizeof(char) * iItems );
	m_PDWData.pcDV = (char *)malloc(sizeof(char) * iItems );
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/19 15:12:04
 * @warning   
 */
 void CPDW::Free()
{
	free(m_PDWData.pfFreq);
	free(m_PDWData.pfPW);
	free(m_PDWData.pfAOA);
	free(m_PDWData.pfTOA);
	free(m_PDWData.pfDTOA);
	free(m_PDWData.pfPA);

	free( m_PDWData.pllTOA );

	free(m_PDWData.pcType);
	free(m_PDWData.pcDV);

}

void *CPDW::GetData()
{
	return & m_PDWData;
}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/07 10:10
*/
void CPDW::ConvertArray( int iDataItems, int iOffset )
{
	UINT i;

	float *pfFreq = m_PDWData.pfFreq;
	float *pfPW = m_PDWData.pfPW;
	float *pfAOA = m_PDWData.pfAOA;
	float *pfTOA = m_PDWData.pfTOA;
	float *pfDTOA = m_PDWData.pfDTOA;
	float *pfPA = m_PDWData.pfPA;
	_TOA *pfllTOA = m_PDWData.pllTOA;

	char *pcType = m_PDWData.pcType;
	char *pcDV = m_PDWData.pcDV;

	UINT uiToa, preToa, uiDToa;
	UINT uiTemp;

	TNEW_PDW temp;

	TNEW_PDW *pPDW = (TNEW_PDW *)gstpRawDataBuffer;

	m_bPhaseData = false;

	_spOneSec = 20000000.;
	_spOneMilli = FDIV( _spOneSec, 1000. );
	_spOneMicrosec = FDIV( _spOneMilli, 1000. );
	_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

	_spAOAres = (float) ( 0.351562 );
	_spAMPres = (float) (0.351562);
	_spPWres = _spOneMicrosec;

	m_PDWData.iDataItems = 0;

	for (i = 0; i < iDataItems ; ++i) {
		temp.bpdw[0][0] = pPDW->item.toa_1;
		temp.bpdw[0][1] = pPDW->item.toa_2;
		temp.bpdw[0][2] = pPDW->item.toa_3;
		temp.bpdw[0][3] = pPDW->item.toa_4;

		uiToa = temp.wpdw[0];
		*pfTOA = FDIV(uiToa, _spOneMicrosec );

		if (i == 0) {
			*pfDTOA = 0;
			preToa = uiToa;
		}
		else {
			uiDToa = uiToa - preToa;
			*pfDTOA = FDIV( uiDToa, _spOneMicrosec );
			preToa = uiToa;
		}

		*pfllTOA = uiToa;


		uiTemp = BIT_MERGE(pPDW->item.frequency_h, pPDW->item.frequency_l);
		*pfFreq = FFRQCNV(pPDW->item.band + 1, uiTemp);

		uiTemp = BIT_MERGE(pPDW->item.pulse_width_h, pPDW->item.pulse_width_l);
		*pfPW = FPWCNV(uiTemp);

		uiTemp = BIT_MERGE(pPDW->item.direction_h, pPDW->item.direction_l);
		*pfAOA = FAOACNV(uiTemp);

		uiTemp = pPDW->item.amplitude;
		*pfPA = FPACNV(uiTemp);

		*pcType = pPDW->item.stat;
		*pcDV = pPDW->item.dv;

		// printf( "\n [%3d] 0x%02X %5.1f%1c[deg] %8.2f[MHz] %10.3f[us] %8.3f[ns]" , i+1, *pcType, *pfAOA, stDV[*pcDV], *pfFreq, *pfTOA, *pfPW );
		// « ≈Õ∏µ ¡∂∞«
		if( ( m_stFilterSetup.dToaMin <= *pfTOA && m_stFilterSetup.dToaMax >= *pfTOA ) &&
			( m_stFilterSetup.dAoaMin <= *pfAOA && m_stFilterSetup.dAoaMax >= *pfAOA ) &&
			( m_stFilterSetup.dPAMin <= *pfPA && m_stFilterSetup.dPAMax >= *pfPA ) &&
			( m_stFilterSetup.dPWMin <= *pfPW && m_stFilterSetup.dPWMax >= *pfPW ) &&
			( m_stFilterSetup.dFrqMin <= *pfFreq && m_stFilterSetup.dFrqMax >= *pfFreq ) ) {
			++pfFreq;
			++pfAOA;
			++pfPW;
			++pfPA;
			++pfTOA;
			++pfDTOA;
			++pcType;
			++pcDV;

			++ pfllTOA;

			++ m_PDWData.iDataItems;
		}

		++pPDW;
	}
}

//////////////////////////////////////////////////////////////////////////
CEPDW::CEPDW(STR_RAWDATA *pRawData, STR_FILTER_SETUP *pstFilterSetup ) : CData(pRawData )
{
	//STR_PDWDATA *pPDWData;

	Alloc( PDW_ITEMS );
 
	if( pstFilterSetup != NULL ) {
		memcpy( & m_stFilterSetup, pstFilterSetup, sizeof(STR_FILTER_SETUP) );
	}

}

CEPDW::~CEPDW(void)
{
	Free();
}

void CEPDW::Alloc( int iItems )
{

	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	m_PDWData.pfFreq = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPW = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfAOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfDTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pllTOA = (_TOA *)malloc(sizeof(_TOA) * iItems );

	m_PDWData.pcType = (char *)malloc(sizeof(char) * iItems );
	m_PDWData.pcDV = (char *)malloc(sizeof(char) * iItems );

	m_PDWData.pfPh1 = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPh2 = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPh3 = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPh4 = (float *)malloc(sizeof(float) * iItems );

}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/15 18:46:30
 * @warning   
 */
void CEPDW::Free()
{
	free(m_PDWData.pfFreq);
	free(m_PDWData.pfPW);
	free(m_PDWData.pfAOA);
	free(m_PDWData.pfTOA);
	free(m_PDWData.pfDTOA);
	free(m_PDWData.pfPA);

	free( m_PDWData.pllTOA );

	free(m_PDWData.pcType);
	free(m_PDWData.pcDV);

	free( m_PDWData.pfPh1 );
	free( m_PDWData.pfPh2 );
	free( m_PDWData.pfPh3 );
	free( m_PDWData.pfPh4 );

}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/15 18:46:32
 * @warning   
 */
void *CEPDW::GetData()
{
	return & m_PDWData;
}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/07 10:10
*/
void CEPDW::ConvertArray( int iDataItems, int iOffset )
{
	int i;

	float *pfFreq = m_PDWData.pfFreq;
	float *pfPW = m_PDWData.pfPW;
	float *pfAOA = m_PDWData.pfAOA;
	float *pfTOA = m_PDWData.pfTOA;
	float *pfDTOA = m_PDWData.pfDTOA;
	float *pfPA = m_PDWData.pfPA;
	_TOA *pfllTOA = m_PDWData.pllTOA;

	float *pfPh1 = m_PDWData.pfPh1;
	float *pfPh2 = m_PDWData.pfPh2;
	float *pfPh3 = m_PDWData.pfPh3;
	float *pfPh4 = m_PDWData.pfPh4;

	char *pcType = m_PDWData.pcType;
	char *pcDV = m_PDWData.pcDV;

	//_TOA uiToa /*firstToa*/ ;
	float fPreToa;

	STR_PDWDATA *pPDWData = (STR_PDWDATA *) gstpRawDataBuffer;
	_PDW *pPDW = (_PDW *) & gstpRawDataBuffer[iOffset];

	if( iOffset != 0 ) {
		m_enBandWidth = pPDWData->enBandWidth;
	}

	_spOneSec = FDIV( 1000000000, _toaRes[m_enBandWidth] );
	_spOneMilli = FDIV( 1000000, _toaRes[m_enBandWidth] );
	_spOneMicrosec = FDIV( 1000, _toaRes[m_enBandWidth] );
	_spOneNanosec = FDIV( 1, _toaRes[m_enBandWidth] );

	_spAMPres = (float) (0.25);
	_spPWres = _spOneMicrosec;

	m_PDWData.iDataItems = 0;

	for ( i=0; i < iDataItems ; ++i ) {
		if (i == 0) {
			if( pPDWData->iIsStorePDW == 1 && iOffset != 0 ) {
				m_ll1stToa = pPDW->llTOA;
			}
			else {
				// m_ll1stToa = 0;
			}

			*pfDTOA = 0;
			*pfTOA = FDIV(pPDW->llTOA-m_ll1stToa, _spOneMicrosec );
			fPreToa = *pfTOA;
		}
		else {
			//uiToa = pPDW->llTOA - m_ll1stToa;
			*pfTOA = FDIV(pPDW->llTOA-m_ll1stToa, _spOneMicrosec );

			//*pfDTOA = (float) ( uiToa - preToa );
			*pfDTOA = FDIV( *pfTOA-fPreToa, _spOneMicrosec );
			fPreToa = *pfTOA;
		}

		*pfllTOA = pPDW->llTOA;

		*pfFreq = F_FRQMhzCNV( 0, pPDW->iFreq );	//FFRQCNV(pPDW->item.band + 1, uiTemp);

		*pfPW = PWCNV(pPDW->iPW * 1000. );

		if( pPDW->iAOA == 0 ) {
			*pfAOA = (float) 0.00000001;
		}
		else {
			*pfAOA = AOACNV( pPDW->iAOA );
		}

		*pfPA = PACNV(pPDW->iPA);

		if( pPDW->iPulseType == 0 ) {
			*pcType = PDW_CW;
		}
		else {
			*pcType = PDW_NORMAL;
		}

		*pcDV = PDW_DV;

		if( m_bPhaseData == true ) {
			*pfPh1 = pPDW->fPh1;
			*pfPh2 = pPDW->fPh2;
			*pfPh3 = pPDW->fPh3;
			*pfPh4 = pPDW->fPh4;
		}

		printf( "\n [%3d] 0x%02X %5.1f%1c[deg] %8.2f[kHz] %10.3f[us] %8.3f[ns]" , i+1, *pcType, *pfAOA, stDV[*pcDV], *pfFreq, *pfTOA, *pfPW );

		// « ≈Õ∏µ ¡∂∞«
		if( ( m_stFilterSetup.dToaMin <= *pfTOA && m_stFilterSetup.dToaMax >= *pfTOA ) &&
			( m_stFilterSetup.dAoaMin <= *pfAOA && m_stFilterSetup.dAoaMax >= *pfAOA ) &&
			( m_stFilterSetup.dPAMin <= *pfPA && m_stFilterSetup.dPAMax >= *pfPA ) &&
			( m_stFilterSetup.dPWMin <= *pfPW && m_stFilterSetup.dPWMax >= *pfPW ) &&
			( m_stFilterSetup.dFrqMin <= *pfFreq && m_stFilterSetup.dFrqMax >= *pfFreq ) ) {
// 		if( ( (double) *pfAOA >= m_stFilterSetup.dAoaMin && (double) *pfAOA <= m_stFilterSetup.dAoaMax ) &&
// 			( (double) *pfFreq >= m_stFilterSetup.dFrqMin && (double) *pfFreq <= m_stFilterSetup.dFrqMax ) ) {
			++pfFreq;
			++pfAOA;
			++pfPW;
			++pfPA;
			++pfTOA;
			++pfDTOA;
			++pcType;
			++pcDV;

			++ pfllTOA;

			if( m_bPhaseData == true ) {
				++ pfPh1;
				++ pfPh2;
				++ pfPh3;
				++ pfPh4;
			}

			++ m_PDWData.iDataItems;
		}

		if( m_bPhaseData == true ) {
			++ pPDW;
		}
		else {
			char *pByte;

			++ pPDW;
			pByte = (char *) pPDW;
			pByte = pByte - (sizeof(float)*4);
			pPDW = ( _PDW * ) pByte;
		}
	}

	//m_pRawData->uiDataItems = uiDataItems;
	Log( enNormal, _T("« ≈Õ∏µ PDW ∞≥ºˆ¥¬ %d ¿‘¥œ¥Ÿ.") , m_PDWData.iDataItems );
}

//////////////////////////////////////////////////////////////////////////
CSPDW::CSPDW(STR_RAWDATA *pRawData) : CData(pRawData )
{
	Alloc( PDW_ITEMS );
}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:39
 * @warning   
 */
CSPDW::~CSPDW(void)
{
	Free();
}

void CSPDW::Alloc( int iItems )
{
	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	m_PDWData.pfFreq = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPW = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfAOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfDTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPA = (float *)malloc(sizeof(float) * iItems );

	m_PDWData.pcType = (char *)malloc(sizeof(char) * iItems );
	m_PDWData.pcDV = (char *)malloc(sizeof(char) * iItems );
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:58
 * @warning   
 */
void CSPDW::Free()
{
	free(m_PDWData.pfFreq);
	free(m_PDWData.pfPW);
	free(m_PDWData.pfAOA);
	free(m_PDWData.pfTOA);
	free(m_PDWData.pfDTOA);
	free(m_PDWData.pfPA);

	free(m_PDWData.pcType);
	free(m_PDWData.pcDV);

}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:55
 * @warning   
 */
void *CSPDW::GetData()
{
	return & m_PDWData;
}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/07 10:10
*/
void CSPDW::ConvertArray( int iDataItems, int iOffset )
{
	UINT i;

	float *pfFreq = m_PDWData.pfFreq;
	float *pfPW = m_PDWData.pfPW;
	float *pfAOA = m_PDWData.pfAOA;
	float *pfTOA = m_PDWData.pfTOA;
	float *pfDTOA = m_PDWData.pfDTOA;
	float *pfPA = m_PDWData.pfPA;

	char *pcType = m_PDWData.pcType;
	char *pcDV = m_PDWData.pcDV;

	float fToa, /* firstToa, */ preToa;

	TNEW_SPDW *pPDW = (TNEW_SPDW *) gstpRawDataBuffer;

	_spOneSec = 20000000.;
	_spOneMilli = FDIV( _spOneSec, 1000. );
	_spOneMicrosec = FDIV( _spOneMilli, 1000. );
	_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

	_spAOAres = (float) ( 0.351562 );
	_spAMPres = (float) (0.351562);
	_spPWres = _spOneMicrosec;

	for (i = 0; i < iDataItems ; ++i) {

		if (i == 0) {
			//firstToa = pPDW->TOA;
			*pfTOA = FDIV( ( pPDW->TOA * 20 ), 1000. );
			*pfDTOA = 0;
			preToa = FDIV( ( pPDW->TOA * 20 ), 1000. );
		}
		else {
			fToa = (float) pPDW->TOA; // - firstToa;
			//*pfTOA = ( fToa * 20. ) / 1000.;
			//*pfTOA = ( fToa * 4 * 100 );
			*pfTOA = FDIV( ( fToa * 20 ), 1000. );

			*pfDTOA = ( *pfTOA - preToa );

			preToa = *pfTOA;
		}

		*pfFreq = pPDW->Freq;

		*pfPW = FDIV( pPDW->PW, 1000. );

		*pfAOA = 90.0;

		*pfPA = pPDW->PA;

		*pcType = 0;
		*pcDV = 1;

		// printf( "\n [%3d] 0x%02X %5.1f%1c[deg] %8.2f[MHz] %10.3f[us] %8.3f[ns]" , i+1, *pcType, *pfAOA, stDV[*pcDV], *pfFreq, *pfTOA, *pfPW );

		++pfFreq;
		++pfAOA;
		++pfPW;
		++pfPA;
		++pfTOA;
		++pfDTOA;
		++pcType;
		++pcDV;

		++pPDW;
	}
}

//////////////////////////////////////////////////////////////////////////
CKFXPDW::CKFXPDW(STR_RAWDATA *pRawData, STR_FILTER_SETUP *pstFilterSetup ) : CData(pRawData )
{
	m_bPhaseData = false;

	Alloc( PDW_ITEMS );
}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:39
 * @warning   
 */
CKFXPDW::~CKFXPDW(void)
{
	Free();
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:02
 * @warning   
 */
void CKFXPDW::Alloc( int iItems )
{

	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	m_PDWData.pfFreq = (float *)malloc(sizeof(float) * iItems);
	m_PDWData.pfPW = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfAOA = (float *)malloc(sizeof(float) * iItems);
	m_PDWData.pfTOA = (float *)malloc(sizeof(float) * iItems);
	m_PDWData.pfDTOA = (float *)malloc(sizeof(float) * iItems);
	m_PDWData.pfPA = (float *)malloc(sizeof(float) * iItems);
	m_PDWData.pllTOA = (_TOA *)malloc(sizeof(_TOA) * iItems);

	m_PDWData.pcType = (char *)malloc(sizeof(char) * iItems);
	m_PDWData.pcDV = (char *)malloc(sizeof(char) * iItems);
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:58
 * @warning   
 */
void CKFXPDW::Free()
{
	free(m_PDWData.pfFreq);
	free(m_PDWData.pfPW);
	free(m_PDWData.pfAOA);
	free(m_PDWData.pfTOA);
	free(m_PDWData.pfDTOA);
	free(m_PDWData.pfPA);
	free(m_PDWData.pllTOA);

	free(m_PDWData.pcType);
	free(m_PDWData.pcDV);

}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:55
 * @warning   
 */
void *CKFXPDW::GetData()
{
	return & m_PDWData;
}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/07 10:10
*/
void CKFXPDW::ConvertArray( int iDataItems, int iOffset )
{
	int i;

	float *pfFreq = m_PDWData.pfFreq;
	float *pfPW = m_PDWData.pfPW;
	float *pfAOA = m_PDWData.pfAOA;
	float *pfTOA = m_PDWData.pfTOA;
	float *pfDTOA = m_PDWData.pfDTOA;
	float *pfPA = m_PDWData.pfPA;

	char *pcType = m_PDWData.pcType;
	char *pcDV = m_PDWData.pcDV;

	_TOA *pfllTOA = m_PDWData.pllTOA;

	float /*fToa,*/ /* firstToa, */ preToa;

	_TOA llToa;
	UDRCPDW *pPDW = (UDRCPDW *) & gstpRawDataBuffer[iOffset];

	_spOneSec = 20000000.;
	_spOneMilli = FDIV( _spOneSec, 1000. );
	_spOneMicrosec = FDIV( _spOneMilli, 1000. );
	_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

	_spAOAres = (float) ( 0.351562 );
	_spAMPres = (float) (0.351562);
	_spPWres = _spOneMicrosec;

	m_PDWData.iDataItems = 0;

	for (i = 0; i < iDataItems ; ++i) {
		llToa = (_TOA) ( pPDW->sPDWFormat.m_LSBTOA ) | ( (_TOA) pPDW->sPDWFormat.m_MSBTOA << 32 );
		*pfllTOA = llToa;
 		if (i == 0) {
			if( iOffset != 0 ) {
				m_ll1stToa = llToa;
			}
 			*pfTOA = DecodeTOA( llToa-m_ll1stToa );
 			*pfDTOA = 0;
 			preToa = *pfTOA;
 		}
 		else {
			*pfTOA = DecodeTOA( llToa-m_ll1stToa );

 			*pfDTOA = ( *pfTOA - preToa );
 
 			preToa = *pfTOA;
 		}

		*pfFreq = DecodeFREQ( pPDW->sPDWFormat.m_freq );		// MHz
 
 		*pfPW = DecodePW( pPDW->sPDWFormat.m_PW );

 		*pfAOA = DecodeDOA( pPDW->sPDWFormat.m_DOA );

 		*pfPA = DecodePA( pPDW->sPDWFormat.m_PA );
 
 		*pcType = 0; // pPDW->sPDWFormat.m_sigType;

 		*pcDV = pPDW->sPDWFormat.m_DI;

		printf( "\n [%3d] 0x%02X %5.1f%1c[deg] %8.2f[MHz] %10.3f[us] %8.3f[ns]" , i+1, *pcType, *pfAOA, stDV[*pcDV], *pfFreq, *pfTOA, *pfPW );

		// « ≈Õ∏µ ¡∂∞«
		if( ( m_stFilterSetup.dToaMin <= *pfTOA && m_stFilterSetup.dToaMax >= *pfTOA ) &&
			( m_stFilterSetup.dAoaMin <= *pfAOA && m_stFilterSetup.dAoaMax >= *pfAOA ) &&
			( m_stFilterSetup.dPAMin <= *pfPA && m_stFilterSetup.dPAMax >= *pfPA ) &&
			( m_stFilterSetup.dPWMin <= *pfPW && m_stFilterSetup.dPWMax >= *pfPW ) &&
			( m_stFilterSetup.dFrqMin <= *pfFreq && m_stFilterSetup.dFrqMax >= *pfFreq ) ) {
			++pfFreq;
			++pfAOA;
			++pfPW;
			++pfPA;
			++pfTOA;
			++pfDTOA;
			++pcType;
			++pcDV;
			++pfllTOA;

			++ m_PDWData.iDataItems;

			//++ m_stFilterSetup.uiDataItems;

		}

		++pPDW;
	}

	Log( enNormal, _T("« ≈Õ∏µ PDW ∞≥ºˆ¥¬ %d ¿‘¥œ¥Ÿ.") , m_PDWData.iDataItems );
}

//////////////////////////////////////////////////////////////////////////
C7PDW::C7PDW(STR_RAWDATA *pRawData, STR_FILTER_SETUP *pstFilterSetup ) : CData(pRawData )
{
	m_bPhaseData = false;

	Alloc( PDW_ITEMS );
}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:39
 * @warning   
 */
C7PDW::~C7PDW(void)
{
	Free();
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:02
 * @warning   
 */
void C7PDW::Alloc( int iItems )
{

	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	Log( enNormal, _T("Alloc()¿ª [%d]∞≥∏¶ «“¥Á«’¥œ¥Ÿ.") , iItems );

	m_PDWData.pfFreq = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPW = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfAOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfDTOA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pfPA = (float *)malloc(sizeof(float) * iItems );
	m_PDWData.pllTOA = (_TOA *)malloc(sizeof(_TOA) * iItems );

	m_PDWData.pcType = (char *)malloc(sizeof(char) * iItems );
	m_PDWData.pcDV = (char *)malloc(sizeof(char) * iItems );
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:58
 * @warning   
 */
void C7PDW::Free()
{
	Log( enNormal, _T("Free()∏¶ «ÿ¡ˆ«’¥œ¥Ÿ.") );

	free(m_PDWData.pfFreq);
	free(m_PDWData.pfPW);
	free(m_PDWData.pfAOA);
	free(m_PDWData.pfTOA);
	free(m_PDWData.pfDTOA);
	free(m_PDWData.pfPA);
	free(m_PDWData.pllTOA);

	free(m_PDWData.pcType);
	free(m_PDWData.pcDV);

}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:41:55
 * @warning   
 */
void *C7PDW::GetData()
{
	return & m_PDWData;
}

/**
  * @brief		
  * @return 	void
  * @date       2019/06/07 10:10
*/
void C7PDW::ConvertArray( int iDataItems, int iOffset )
{
	int i;

	float *pfFreq = m_PDWData.pfFreq;
	float *pfPW = m_PDWData.pfPW;
	float *pfAOA = m_PDWData.pfAOA;
	float *pfTOA = m_PDWData.pfTOA;
	float *pfDTOA = m_PDWData.pfDTOA;
	float *pfPA = m_PDWData.pfPA;

	_TOA *pllTOA = m_PDWData.pllTOA;

	char *pcType = m_PDWData.pcType;
	char *pcDV = m_PDWData.pcDV;

	_TOA /*fToa*/ /* firstToa, */ preToa;

	Log( enNormal, _T("ConvertArray()∏¶ [%d]∞≥∏¶ ∫Ø»Ø«’¥œ¥Ÿ.") , iDataItems );

	SRXPDWDataRGroup *pPDW = (SRXPDWDataRGroup *) & gstpRawDataBuffer[iOffset];

	_spOneSec = 20000000.;
	_spOneMilli = FDIV( _spOneSec, 1000. );
	_spOneMicrosec = FDIV( _spOneMilli, 1000. );
	_spOneNanosec = FDIV( _spOneMicrosec, 1000. );

	_spAOAres = (float) ( 0.351562 );
	_spAMPres = (float) (0.351562);
	_spPWres = _spOneMicrosec;

	m_PDWData.iDataItems = 0;

	for (i = 0; i < iDataItems ; ++i) {
		swapByteOrder( pPDW->llTOA );
		AllSwapData32( & pPDW->iSignalType, sizeof(SRXPDWDataRGroup)- sizeof(long long int) );

		*pllTOA  = pPDW->llTOA;

		//*pfTOA = FMUL( pPDW->llTOA, 6.48824/1000.0 );
		
 		if (i == 0) {
			if( iOffset != 0 ) {
				m_ll1stToa = pPDW->llTOA;
			}
 			*pfDTOA = 0;
 			preToa = pPDW->llTOA;
 		}
 		else {
 			*pfDTOA = ( pPDW->llTOA - preToa );
 			preToa = pPDW->llTOA;
 		}
		*pfTOA = FDMUL( pPDW->llTOA, 6.48824/1000.0 );
		*pfDTOA = FDMUL( *pfDTOA, 6.48824/1000.0 );

		*pfFreq = FMUL( pPDW->iFreq, 0.010 );		// MHz
 
 		*pfPW = FMUL( pPDW->iPW, 6.48824 );

 		*pfAOA = FMUL( pPDW->iDirection, 0.1 );

 		*pfPA = FMUL( pPDW->iPA, 0.25 ) - (float) 110.;
 
 		*pcType = pPDW->iSignalType;

 		*pcDV = pPDW->iDirectionVaild ^ 1;

		// « ≈Õ∏µ ¡∂∞«
		if( ( m_stFilterSetup.dToaMin <= *pfTOA && m_stFilterSetup.dToaMax >= *pfTOA ) &&
			( m_stFilterSetup.dAoaMin <= *pfAOA && m_stFilterSetup.dAoaMax >= *pfAOA ) &&
			( m_stFilterSetup.dPAMin <= *pfPA && m_stFilterSetup.dPAMax >= *pfPA ) &&
			( m_stFilterSetup.dPWMin <= *pfPW && m_stFilterSetup.dPWMax >= *pfPW ) &&
			( m_stFilterSetup.dFrqMin <= *pfFreq && m_stFilterSetup.dFrqMax >= *pfFreq ) ) {
			++pfFreq;
			++pfAOA;
			++pfPW;
			++pfPA;
			++pfTOA;
			++pfDTOA;
			++pcType;
			++pcDV;
			++pllTOA;

			++ m_PDWData.iDataItems;

			//++ m_stFilterSetup.uiDataItems;
		}

		// printf( "\n [%3d] 0x%02X %5.1f%1c[deg] %8.2f[MHz] %10.3f[us] %8.3f[ns]" , i+1, *pcType, *pfAOA, stDV[*pcDV], *pfFreq, *pfTOA, *pfPW );

		++pPDW;
	}

	Log( enNormal, _T("PDW ∞≥ºˆ¥¬ %d ¿‘¥œ¥Ÿ.") , m_PDWData.iDataItems );

}


//////////////////////////////////////////////////////////////////////////
//

C7IQ::C7IQ(STR_RAWDATA *pRawData) : CData( pRawData )
{
	Alloc( IQ_ITEMS );
}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:06
 * @warning   
 */
C7IQ::~C7IQ(void)
{
	Free();
}


/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:08
 * @warning   
 */
void C7IQ::Alloc( int iItems )
{

	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	m_IQData.pfI = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfQ = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfPA = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfIP = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfFFT = (float *)malloc( sizeof(float) * iItems );
	
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:09
 * @warning   
 */
void C7IQ::Free()
{
	free(m_IQData.pfI);
	free(m_IQData.pfQ);
	free(m_IQData.pfPA);
	free(m_IQData.pfIP);
	free(m_IQData.pfFFT);
}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:12
 * @warning   
 */
void *C7IQ::GetData()
{
	return & m_IQData;
}

/**
  * @brief		IQ µ•¿Ã≈Õ ±∏¡∂ø° ¿˙¿Â«—¥Ÿ.
  * @return 	void
  * @date       2019/06/07 10:11
*/
void C7IQ::ConvertArray( int iDataItems, int iOffset )
{
	int i;

	float *psI = m_IQData.pfI;
	float *psQ = m_IQData.pfQ;
	float *psPA = m_IQData.pfPA;
	float *psIP = m_IQData.pfIP;
	float *psFFT = m_IQData.pfFFT;

	float fVal1, fVal2;

	SRXIQDataRGroup *pIQ;

	if( iOffset <= 0 ) {
		pIQ = (SRXIQDataRGroup *) & gstpRawDataBuffer[0];
	}
	else {
		pIQ = (SRXIQDataRGroup *) & gstpRawDataBuffer[iOffset];
	}

	m_IQData.iDataItems = 0;

	for (i = 0; i < iDataItems ; ++i ) {
		*psI = (float) ( (short) ( pIQ->sI ^ (0x8A5A) ) );
		*psQ = (float) ( (short) ( pIQ->sQ ^ (0x8A5A) ) );

		// º¯Ω√ ¡¯∆¯
		*psPA = sqrt( *psI * *psI + *psQ * *psQ );
		*psPA = (float) (20.*log10( *psPA ) ) - (float) 80.;

		// º¯Ω√ ¿ßªÛ¬˜
		if( i == 0 ) {
			*psIP = 0.0;
			fVal2 = (float) ( atan2( *psQ, *psI ) * RAD2DEG );
		}
		else {
			fVal1 = (float) ( atan2( *psQ, *psI ) * RAD2DEG );
			*psIP = fVal1 - fVal2;
			fVal2 = fVal1;
		}
		if( *psIP > 180. ) {
			*psIP -= ( 2 * 180. );
		}
		else if( *psIP < -180 ) {
			*psIP += ( 2 * 180. );
		}
		else {

		}

		// printf( "\n [%3d] %10d %10d" , i+1, *psI, *psQ );

		++psI;
		++psQ;
		++psPA;
		++psIP;

		++ m_IQData.iDataItems;
		
		++pIQ;
	}

	ExecuteFFT( iDataItems, & m_IQData );

}


//////////////////////////////////////////////////////////////////////////
//

CIQ::CIQ(STR_RAWDATA *pRawData) : CData(pRawData )
{

}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:06
 * @warning   
 */
CIQ::~CIQ(void)
{
	Free();
}


/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:08
 * @warning   
 */
void CIQ::Alloc( int iItems )
{

	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	m_IQData.pfI = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfQ = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfPA = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfIP = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfFFT = (float *)malloc( sizeof(float) * iItems );
	
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:09
 * @warning   
 */
void CIQ::Free()
{
	free(m_IQData.pfI);
	free(m_IQData.pfQ);
	free(m_IQData.pfPA);
	free(m_IQData.pfIP);
	free(m_IQData.pfFFT);
}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:12
 * @warning   
 */
void *CIQ::GetData()
{
	return & m_IQData;
}

/**
  * @brief		IQ µ•¿Ã≈Õ ±∏¡∂ø° ¿˙¿Â«—¥Ÿ.
  * @return 	void
  * @date       2019/06/07 10:11
*/
void CIQ::ConvertArray( int iDataItems, int iOffset )
{
	UINT i;

	float *psI = m_IQData.pfI;
	float *psQ = m_IQData.pfQ;
	float *psPA = m_IQData.pfPA;
	float *psIP = m_IQData.pfIP;
	float *psFFT = m_IQData.pfFFT;

	float fVal1, fVal2;

	TNEW_IQ *pIQ = (TNEW_IQ *) gstpRawDataBuffer;

	for (i = 0; i < m_pRawData->uiDataItems; ++i) {
		*psI = (float) pIQ->sI;
		*psQ = (float) pIQ->sQ;

		// º¯Ω√ ¡¯∆¯
		*psPA = sqrt( *psI * *psI + *psQ * *psQ );
		*psPA = (float) (20.*log10( *psPA ) ) - (float) 80.;

		// º¯Ω√ ¿ßªÛ¬˜
		if( i == 0 ) {
			*psIP = 0.0;
			fVal2 = (float) ( atan2( *psQ, *psI ) * RAD2DEG );
		}
		else {
			fVal1 = (float) ( atan2( *psQ, *psI ) * RAD2DEG );
			*psIP = fVal1 - fVal2;
			fVal2 = fVal1;
		}
		if( *psIP > 180. ) {
			*psIP -= ( 2 * 180. );
		}
		else if( *psIP < -180 ) {
			*psIP += ( 2 * 180. );
		}
		else {

		}

		// printf( "\n [%3d] %10d %10d" , i+1, *psI, *psQ );

		++psI;
		++psQ;
		++psPA;
		++psIP;
		
		++pIQ;
	}

	ExecuteFFT( iDataItems, & m_IQData );

}


//////////////////////////////////////////////////////////////////////////

CEIQ::CEIQ(STR_RAWDATA *pRawData) : CData(pRawData )
{
	Alloc( IQ_ITEMS );
}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:06
 * @warning   
 */
CEIQ::~CEIQ(void)
{
	Free();
}


/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:08
 * @warning   
 */
void CEIQ::Alloc( int iItems )
{

	if( iItems == 0 ) {
		iItems = m_pRawData->uiDataItems;
	}

	m_IQData.pfI = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfQ = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfPA = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfIP = (float *)malloc( sizeof(float) * iItems );
	m_IQData.pfFFT = (float *)malloc( sizeof(float) * iItems );
	
}

/**
 * @brief     
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:09
 * @warning   
 */
void CEIQ::Free()
{
	free(m_IQData.pfI);
	free(m_IQData.pfQ);
	free(m_IQData.pfPA);
	free(m_IQData.pfIP);
	free(m_IQData.pfFFT);
}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:12
 * @warning   
 */
void *CEIQ::GetData()
{
	return & m_IQData;
}

/**
  * @brief		IQ µ•¿Ã≈Õ ±∏¡∂ø° ¿˙¿Â«—¥Ÿ.
  * @return 	void
  * @date       2019/06/07 10:11
*/
void CEIQ::ConvertArray( int iDataItems, int iOffset )
{
	int i;

	float *psI = m_IQData.pfI;
	float *psQ = m_IQData.pfQ;
	float *psPA = m_IQData.pfPA;
	float *psIP = m_IQData.pfIP;
	float *psFFT = m_IQData.pfFFT;

	float fVal1, fVal2;

	TNEW_IQ *pIQ;

	if( iOffset <= 0 ) {
		pIQ = (TNEW_IQ *) & gstpRawDataBuffer[0];
	}	
	else {
		pIQ = (TNEW_IQ *) & gstpRawDataBuffer[iOffset];
	}

	Log( enNormal, _T("ConvertArray()∏¶ [%d]∞≥∏¶ ∫Ø»Ø«’¥œ¥Ÿ.") , iDataItems );

	m_IQData.iDataItems = 0;

	for (i = 0; i < iDataItems ; ++i ) {
		*psI = (float) pIQ->sI;
		*psQ = (float) pIQ->sQ;

		// º¯Ω√ ¡¯∆¯
		*psPA = sqrt( *psI * *psI + *psQ * *psQ );
		*psPA = (float) (20.*log10( *psPA ) ) - (float) 80.;

		// º¯Ω√ ¿ßªÛ¬˜
		if( i == 0 ) {
			*psIP = 0.0;
			fVal2 = (float) ( atan2( *psQ, *psI ) * RAD2DEG );
		}
		else {
			fVal1 = (float) ( atan2( *psQ, *psI ) * RAD2DEG );
			*psIP = fVal1 - fVal2;
			fVal2 = fVal1;
		}
		if( *psIP > 180. ) {
			*psIP -= ( 2 * 180. );
		}
		else if( *psIP < -180 ) {
			*psIP += ( 2 * 180. );
		}
		else {

		}

		// printf( "\n [%3d] %10d %10d" , i+1, *psI, *psQ );

		++psI;
		++psQ;
		++psPA;
		++psIP;

		++ m_IQData.iDataItems;
		
		++pIQ;
	}

	ExecuteFFT( iDataItems, & m_IQData );

}


//////////////////////////////////////////////////////////////////////////
/**
 * @brief     
 * @param     STR_RAWDATA * pRawData
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:19
 * @warning   
 */
CData::CData( STR_RAWDATA *pRawData )
{
	m_pRawData = pRawData;

	m_uiWindowNumber = 1;

	m_bPhaseData = true;

	ClearFilterSetup();

}

void CData::ClearFilterSetup()
{

	m_stFilterSetup.dToaMin = 0.;
	m_stFilterSetup.dToaMax = DBL_MAX;
	m_stFilterSetup.dDtoaMin = 0;
	m_stFilterSetup.dDtoaMax = DBL_MAX;
	m_stFilterSetup.dAoaMin = 0;
	m_stFilterSetup.dAoaMax = DBL_MAX;
	m_stFilterSetup.dFrqMin = 0;
	m_stFilterSetup.dFrqMax = DBL_MAX;
	m_stFilterSetup.dPAMin = -DBL_MAX;
	m_stFilterSetup.dPAMax = DBL_MAX;
	m_stFilterSetup.dPWMin = 0;
	m_stFilterSetup.dPWMax = DBL_MAX;

	//m_stFilterSetup.uiDataItems = 0;

}

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:22
 * @warning   
 */
CData::~CData(void)
{
	
}


void CData::AllSwapData64( void *pData, int iLength )
{
	int i;
	unsigned long long int *pWord;

	pWord = (unsigned long long int *) pData;
	for( i=0 ; i < iLength ; i+=sizeof(int) ) {
		swapByteOrder( *pWord );
		++ pWord;
	}

}

void CData::AllSwapData32( void *pData, int iLength )
{
	int i;
	UINT *pWord;

	pWord = (UINT *) pData;
	for( i=0 ; i < iLength ; i+=sizeof(int) ) {
		swapByteOrder( *pWord );
		++ pWord;
	}

}

void CData::swapByteOrder(unsigned long long& ull)
{
	ull = (ull >> 56) |
		((ull<<40) & 0x00FF000000000000) |
		((ull<<24) & 0x0000FF0000000000) |
		((ull<<8) & 0x000000FF00000000) |
		((ull>>8) & 0x00000000FF000000) |
		((ull>>24) & 0x0000000000FF0000) |
		((ull>>40) & 0x000000000000FF00) |
		(ull << 56);
}

void CData::swapByteOrder(unsigned int& ui) 
{
	ui = (ui >> 24) |
		((ui<<8) & 0x00FF0000) |
		((ui>>8) & 0x0000FF00) |
		(ui << 24);
}

/**
 * @brief     
 * @param     int iDataItems
 * @param     STR_IQ_DATA * pIQData
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/16 22:31:50
 * @warning   
 */
void CData::ExecuteFFT( int iDataItems, STR_IQ_DATA *pIQData )
{
	int i;

	float *psI, *psQ;

	float *psFFT = pIQData->pfFFT;

	fftw_complex *pIn, *pOut;
	fftw_complex *pP;
	fftw_plan plan;

	int N = iDataItems, nShift;

	pIn = ( fftw_complex * ) fftw_malloc( sizeof(fftw_complex) * N );
	pOut = ( fftw_complex * ) fftw_malloc( sizeof(fftw_complex) * N );
	plan = fftw_plan_dft_1d( N, pIn, pOut, FFTW_FORWARD, FFTW_ESTIMATE );

	psI = pIQData->pfI;
	psQ = pIQData->pfQ;

	pP = pIn;
	for( i=0 ; i < N ; ++i ) {
		pP[i][0] = *psI;
		pP[i][1] = *psQ;

		++ psI;
		++ psQ;
	}

	fftw_execute( plan );

	nShift = ( N / 2 );
	fftw_complex swap;
	for( i=0 ; i < (N/2) ; ++i ) {
		swap[0] = pOut[i][0];
		swap[1] = pOut[i][1];

		pOut[i][0] = pOut[nShift][0];
		pOut[i][1] = pOut[nShift][1];

		pOut[nShift][0] = swap[0];
		pOut[nShift][1] = swap[1];

		++ nShift;
	}	

	for( i=0 ; i < N ; ++i ) {
		*psFFT = (float) sqrt( pOut[i][0] * pOut[i][0] + pOut[i][1] * pOut[i][1] );
		++ psFFT;

	}

	fftw_destroy_plan( plan );
	fftw_free( pIn );
	fftw_free( pOut );
}

//////////////////////////////////////////////////////////////////////////

/**
 * @brief     
 * @param     void
 * @return    
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:44:24
 * @warning   
 */
CDataFile::CDataFile(void)
{
	m_iFileIndex = 0;

	m_pData = NULL;

	Alloc();

	++stDataFile;
}


/**
  * @brief		
  * @param		void
  * @return 	
  * @date       2019/05/31 10:43
*/
CDataFile::~CDataFile(void)
{
	--stDataFile;

	Free();

	if( m_RawDataFile.m_hFile != (HANDLE)-1 ) {
		m_RawDataFile.Close();
	}

}

/**
  * @brief		
  * @return 	void
  * @date       2019/05/31 10:43
*/
void CDataFile::Alloc()
{
	if( gstpRawDataBuffer == NULL ) {
		gstpRawDataBuffer = (char *) malloc(sizeof(char)* MAX_RAWDATA_SIZE );
	}
}

void CDataFile::Free()
{

// 	if( gstpRawDataBuffer != NULL ) {
// 		free( gstpRawDataBuffer );
// 		gstpRawDataBuffer = NULL;
// 	}

 	if (stDataFile == 0) {
 		free(gstpRawDataBuffer);
 		gstpRawDataBuffer = NULL;
 	}

	//m_pData->Free();

// 	if( m_pData != NULL ) {
// 		delete m_pData;
// 		m_pData = NULL;
// 	}

}


/**
 * @brief     
 * @param     CString & strPathname
 * @param     void * pData
 * @param     int iNumData
 * @param     ENUM_UnitType enUnitType
 * @param     ENUM_DataType enDataType
 * @param     void * pDataEtc
 * @param     int iSizeOfEtc
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/04 14:09:35
 * @warning   
 */
void CDataFile::SaveDataFile( CString & strPathname, void *pData, int iNumData, ENUM_UnitType enUnitType, ENUM_DataType enDataType, void *pDataEtc, int iSizeOfEtc )
{
	CFileException ex;

	if( enUnitType == en_SONATA || enUnitType == en_KFX || true ) {
		if( enDataType == en_PDW_DATA) {
			
			if( ! m_RawDataFile.Open( strPathname, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex ) ) {
				Log( enError, _T("PDW µ•¿Ã≈Õ ∆ƒ¿œ[%s]¿ª ª˝º∫«œ¡ˆ ∏¯«’¥œ¥Ÿ !") , strPathname );
			}
			else {
				m_RawDataFile.Write( pData, iNumData );
				m_RawDataFile.Close();
			}
		}
		else if( enDataType == en_IQ_DATA ) {
			if( ! m_RawDataFile.Open( strPathname, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex ) ) {
				Log( enError, _T("IQ µ•¿Ã≈Õ ∆ƒ¿œ[%s]¿ª ª˝º∫«œ¡ˆ ∏¯«’¥œ¥Ÿ !") , strPathname );
			}
			else {
				// IQ «Ï¥ı ¡§∫∏ ¿˙¿Â
				if( pDataEtc != NULL ) {
					m_RawDataFile.Write( pDataEtc, iSizeOfEtc );
				}

				m_RawDataFile.Write( pData, iNumData );
				m_RawDataFile.Close();
			}
		}
	}

}

/**
 * @brief     
 * @return    ENUM_DataType
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/12 19:36:17
 * @warning   
 */
ENUM_DataType CDataFile::WhatDataType( CString *pStrPathname )
{
	ENUM_DataType enDataType=en_UnknownData;

#ifdef _MBCS
	if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".pdw") ) || NULL != strcmp( pStrPathname->GetBuffer(), _T(".npw") ) {
		enDataType = en_PDW_DATA;
	}
	else if( NULL != strcmp( pStrPathname->GetBuffer(), _T("e4.dat") ) ) {
		enDataType = en_PDW_DATA;
	}
	else if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".iq") ) || NULL != strcmp( pStrPathname->GetBuffer(), _T(".siq") ||
			 NULL != wcsstr( pStrPathname->GetBuffer(), L"e2.dat" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".eiq" ) ) {
		enDataType = en_IQ_DATA;
	}
	else if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".epdw") ) || NULL != strcmp( pStrPathname->GetBuffer(), _T(".enpw") ) ) {
		enDataType = en_PDW_DATA;
	}
#else
	if( NULL != wcsstr( pStrPathname->GetBuffer(), L".pdw" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".npw" ) ) {
		enDataType = en_PDW_DATA;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L"e4.dat" ) ) {
		enDataType = en_PDW_DATA;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L".kpdw" ) ) {
		enDataType = en_PDW_DATA;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L".iq" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".siq" ) || 
			 NULL != wcsstr( pStrPathname->GetBuffer(), L"e2.dat" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".eiq" ) ) {
		enDataType = en_IQ_DATA;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L".epdw" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".enpw" ) ) {
		enDataType = en_PDW_DATA;
	}

#endif

	return enDataType;

}

/**
 * @brief     
 * @return    ENUM_UnitType
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/12 19:38:24
 * @warning   
 */
 ENUM_UnitType CDataFile::WhatUnitType( CString *pStrPathname )
{
	ENUM_UnitType enUnitType=en_ELINT;

#ifdef _MBCS
	if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".pdw") ) || NULL != strcmp( pStrPathname->GetBuffer(), _T(".npw") ) ||
		NULL != strcmp( pStrPathname->GetBuffer(), _T(".spdw") ) ) {
		enUnitType = en_SONATA;
	}
	else if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".siq") ) || NULL != strcmp( pStrPathname->GetBuffer(), _T(".spdw") ) {
		enUnitType = en_SONATA_SHU;
	}
	else if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".dat") ) ) {
		enUnitType = en_701;
	}
	else if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".iq") ) || NULL != strcmp( pStrPathname->GetBuffer(), _T(".siq") ) ) {
		enUnitType = en_SONATA;
	}
	else if( NULL != strcmp( pStrPathname->GetBuffer(), _T(".epdw") ) || NULL != strcmp( pStrPathname->GetBuffer(), _T(".enpw") ) ) {
		enUnitType = en_ELINT;
	}
#else
	if( NULL != wcsstr( pStrPathname->GetBuffer(), L".pdw" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".npw" ) ) {
		enUnitType = en_SONATA;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L".dat" ) ) {
		enUnitType = en_701;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L".kpdw" ) ) {
		enUnitType = en_KFX;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L".iq" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".siq" ) ) {
		enUnitType = en_SONATA;
	}
	else if( NULL != wcsstr( pStrPathname->GetBuffer(), L".epdw" ) || NULL != wcsstr( pStrPathname->GetBuffer(), L".enpw" ) ) {
		enUnitType = en_ELINT;
	}

#endif

	return enUnitType;

}

/**
  * @brief		RAW µ•¿Ã≈Õ ∆ƒ¿œ ¿–±‚
  * @param		CString & strPathname
  * @return 	void
  * @return		º∫∞¯Ω√ true, Ω«∆–Ω√ false
  * @date       2019/05/31 10:34
*/
bool CDataFile::ReadDataFile( CString & strPathname, int iFileIndex, STR_FILTER_SETUP *pstFilterSetup )
{
	int iDataItems, iFilteredDataItems;
	ULONGLONG dwFileRead;

	if( m_strPathname.IsEmpty() == true ) {
		Log( enNormal, _T("∆ƒ¿œ[%s]¿ª ø¿«¬«’¥œ¥Ÿ."), strPathname.GetBuffer() );

		m_strPathname = strPathname;
		
		m_strPathname.MakeLower();

		m_RawData.enUnitType = WhatUnitType( & m_strPathname );
		m_RawData.enDataType = WhatDataType( & m_strPathname );

	}
	else {
		Log( enNormal, _T("∆ƒ¿œ[%s]¿ª ¿Áø¿«¬«’¥œ¥Ÿ."), m_strPathname.GetBuffer() );
	}

	if( m_RawData.enDataType == en_PDW_DATA && m_RawData.enUnitType == en_SONATA ) {
		if ( m_RawDataFile.m_hFile == (HANDLE) -1 && m_RawDataFile.Open( m_strPathname.GetBuffer(), CFile::shareDenyNone | CFile::typeBinary) == TRUE) {
			if( m_pData == NULL ) {
				m_pData = new CPDW( & m_RawData );

				m_gMapData.insert( make_pair( m_strPathname, m_pData ) );
			}
		}

		if( iFileIndex <= 0 ) {
			m_iFileIndex = 0;

			m_dwFileEnd = m_RawDataFile.SeekToEnd();
			m_RawDataFile.Seek( 0, CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(TNEW_PDW) * PDW_ITEMS );
			m_RawData.uiDataItems = m_RawData.uiByte / sizeof(TNEW_PDW);

			iDataItems = m_RawData.uiByte/ sizeof(TNEW_PDW);

			m_pData->ConvertArray( m_RawData.uiDataItems );

		}
		else {
			m_iFileIndex = iFileIndex;

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(_PDW)*PDW_ITEMS );
			m_RawData.uiDataItems = m_RawData.uiByte / sizeof(TNEW_PDW);

			iDataItems = m_RawData.uiByte/ sizeof(TNEW_PDW);

			m_pData->ConvertArray( m_RawData.uiDataItems );
		}

	}

// 	else if( m_RawData.enDataType == en_PDW_DATA ) {
// 		if ( m_RawDataFile.m_hFile == (HANDLE) -1 && m_RawDataFile.Open( m_strPathname.GetBuffer(), CFile::shareDenyNone | CFile::typeBinary) == TRUE) {
// 			if( m_pData == NULL ) {
// 				m_pData = new CSPDW( & m_RawData );
// 			}
// 
// 		}
// 
// 		if( iFileIndex <= 0 ) {
// 			m_iFileIndex = 0;
// 
// 			m_dwFileEnd = m_RawDataFile.SeekToEnd();
// 			m_RawDataFile.Seek( 0, CFile::begin );
// 
// 			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(TNEW_SPDW) * PDW_ITEMS );
// 			m_RawData.uiDataItems = (UINT) m_dwFileEnd / sizeof(TNEW_SPDW);
// 
// 			m_pData->ConvertArray( 0 );
// 
// 		}
// 		else {
// 			m_iFileIndex = iFileIndex;
// 
// 			//m_RawDataFile.Seek( sizeof(SRxPDWHeader) + sizeof(SRXPDWDataRGroup)*(m_iFileIndex*PDW_ITEMS), CFile::begin );
// 
// 			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(_PDW)*PDW_ITEMS );
// 
// 			iDataItems = m_RawData.uiByte / sizeof(_PDW);
// 
// 			m_pData->ConvertArray( iDataItems, 0 );
// 		}
// 
// 	}

	else if( m_RawData.enDataType == en_PDW_DATA && m_RawData.enUnitType == en_ELINT ) {
		if ( m_RawDataFile.m_hFile == (HANDLE) -1 && m_RawDataFile.Open( m_strPathname.GetBuffer(), CFile::shareDenyNone | CFile::typeBinary) == TRUE) {
			if( m_pData == NULL ) {
				m_pData = new CEPDW( & m_RawData, pstFilterSetup );

				m_gMapData.insert( make_pair( m_strPathname, m_pData ) );
			}
		
		}
		STR_PDWDATA *pPDWData;
		if( iFileIndex <= 0 ) {
			m_iFileIndex = 0;

			m_dwFileEnd = m_RawDataFile.SeekToEnd();
			m_RawDataFile.Seek( 0, CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, (sizeof(STR_PDWDATA)-sizeof(pPDWData->stPDW)) + sizeof(_PDW)*PDW_ITEMS );
			pPDWData = ( STR_PDWDATA * ) gstpRawDataBuffer;
			m_RawData.uiDataItems = pPDWData->count;

			if( m_dwFileEnd == (sizeof(STR_PDWDATA)-sizeof(pPDWData->stPDW) ) + sizeof(_PDW)*m_RawData.uiDataItems ) {
				m_pData->m_bPhaseData = true;
			}
			else {
				m_pData->m_bPhaseData = false;
			}

			iDataItems = ( m_RawData.uiByte - (sizeof(STR_PDWDATA)-sizeof(pPDWData->stPDW)) ) / sizeof(_PDW);
			m_pData->ConvertArray( iDataItems, (sizeof(STR_PDWDATA)-sizeof(pPDWData->stPDW)) );

		}
		else {
			m_iFileIndex = iFileIndex;

			//m_RawDataFile.Seek( sizeof(SRxPDWHeader) + sizeof(SRXPDWDataRGroup)*(m_iFileIndex*PDW_ITEMS), CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(_PDW)*PDW_ITEMS );

			iDataItems = m_RawData.uiByte / sizeof(_PDW);

			m_pData->ConvertArray( iDataItems, 0 );
		}

	}

	else if( m_RawData.enDataType == en_PDW_DATA && m_RawData.enUnitType == en_701 ) {
		if( m_RawDataFile.m_hFile == (HANDLE) -1 && m_RawDataFile.Open( m_strPathname.GetBuffer(), CFile::shareDenyNone | CFile::typeBinary) == TRUE ) {
			if( m_pData == NULL ) {
				m_pData = new C7PDW( & m_RawData, pstFilterSetup );

				m_gMapData.insert( make_pair( m_strPathname, m_pData ) );
			}
		}
		SRxPDWHeader *pPDWHeader;
		if( iFileIndex <= 0 ) {
			m_iFileIndex = 0;

			m_dwFileEnd = m_RawDataFile.SeekToEnd();
			m_RawDataFile.Seek( 0, CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(SRxPDWHeader) + sizeof(SRXPDWDataRGroup)*PDW_ITEMS );

			pPDWHeader = ( SRxPDWHeader * ) gstpRawDataBuffer;

			m_pData->AllSwapData32( & pPDWHeader->uiAcqTime, sizeof(int)*4 );
			m_pData->AllSwapData32( & pPDWHeader->iSearchBandID, sizeof(int)*4 );

			m_RawData.uiDataItems = pPDWHeader->iNumOfPDW;

			iDataItems = ( m_RawData.uiByte - sizeof(SRxPDWHeader) ) / sizeof(SRXPDWDataRGroup);

			m_pData->ConvertArray( iDataItems, sizeof(SRxPDWHeader) );
		}
		else {
			m_iFileIndex = iFileIndex;

			m_RawDataFile.Seek( sizeof(SRxPDWHeader) + sizeof(SRXPDWDataRGroup)*(m_iFileIndex*PDW_ITEMS), CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(SRXPDWDataRGroup)*PDW_ITEMS );

			iDataItems = m_RawData.uiByte / sizeof(SRXPDWDataRGroup);

			m_pData->ConvertArray( iDataItems, 0 );
		}

		// m_RawDataFile.Close();
	}

	else if( m_RawData.enDataType == en_IQ_DATA && m_RawData.enUnitType == en_701 ) {
		if( m_RawDataFile.m_hFile == (HANDLE) -1 && m_RawDataFile.Open( m_strPathname.GetBuffer(), CFile::shareDenyNone | CFile::typeBinary) == TRUE ) {
			if( m_pData == NULL ) {
				m_pData = new C7IQ( & m_RawData );

				m_gMapData.insert( make_pair( m_strPathname, m_pData ) );
			}
		}

		//SRxIQHeader *pIQHeader;
		if( iFileIndex <= 0 ) {
			m_iFileIndex = 0;

			m_dwFileEnd = m_RawDataFile.SeekToEnd();
			m_RawDataFile.Seek( 0, CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(SRXIQDataRGroup)*IQ_ITEMS );

			//pIQHeader = ( SRxIQHeader * ) gstpRawDataBuffer;

			//m_pData->AllSwapData32( & pIQHeader->uiAcqTime, sizeof(int)*6 );
			//m_pData->AllSwapData64( & pIQHeader->llToa, sizeof(int)*6 );
			//m_pData->AllSwapData32( & pIQHeader->iSearchBandID, sizeof(int)*7 );

			m_RawData.uiDataItems = m_RawData.uiByte /*m_dwFileEnd*/ / sizeof(SRXIQDataRGroup);

			iDataItems = m_RawData.uiByte / sizeof(SRXIQDataRGroup);

			m_pData->ConvertArray( iDataItems, 0 );
		}
		else {
			m_iFileIndex = iFileIndex;

			m_RawDataFile.Seek( sizeof(SRXIQDataRGroup)*m_iFileIndex, CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(SRxPDWHeader) + sizeof(SRXPDWDataRGroup)*PDW_ITEMS );

			iDataItems = m_RawData.uiByte / sizeof(SRXPDWDataRGroup);

			m_pData->ConvertArray( iDataItems, 0 );

		}

	}

	else if( m_RawData.enDataType == en_PDW_DATA && m_RawData.enUnitType == en_KFX ) {
		if( m_RawDataFile.m_hFile == (HANDLE) -1 && m_RawDataFile.Open( m_strPathname.GetBuffer(), CFile::shareDenyNone | CFile::typeBinary) == TRUE ) {
			if( m_pData == NULL ) {
				m_pData = new CKFXPDW( & m_RawData, pstFilterSetup );

				m_gMapData.insert( make_pair( m_strPathname, m_pData ) );
			}

		}
		STR_PDWFILE_HEADER *pPDWFile;

		if( iFileIndex <= 0 ) {
			m_iFileIndex = 0;

			m_dwFileEnd = m_RawDataFile.SeekToEnd();
			m_RawDataFile.Seek( 0, CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(STR_PDWFILE_HEADER) + sizeof(UDRCPDW)*PDW_ITEMS );

			pPDWFile = ( STR_PDWFILE_HEADER * ) gstpRawDataBuffer;

			m_RawData.uiDataItems = pPDWFile->uiSignalCount;

			iDataItems = ( m_RawData.uiByte - sizeof(STR_PDWFILE_HEADER) ) / sizeof(UDRCPDW);

			m_pData->ConvertArray( iDataItems, sizeof(STR_PDWFILE_HEADER) );
		}
		else {
			m_iFileIndex = iFileIndex;

			m_RawDataFile.Seek( sizeof(STR_PDWFILE_HEADER) + sizeof(UDRCPDW)*(m_iFileIndex*PDW_ITEMS), CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(UDRCPDW)*PDW_ITEMS );

			iDataItems = m_RawData.uiByte / sizeof(UDRCPDW);

			m_pData->ConvertArray( iDataItems, 0 );

		}

	}

	//////////////////////////////////////////////////////////////////////////
	else if( m_RawData.enDataType == en_IQ_DATA && m_RawData.enUnitType == en_SONATA ) {
		if (m_RawDataFile.Open( strPathname.GetBuffer(), CFile::modeRead | CFile::typeBinary) == TRUE) {
			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, MAX_RAWDATA_SIZE );
			m_RawData.uiDataItems = m_RawData.uiByte / sizeof(TNEW_IQ);

			m_pData = new CIQ( & m_RawData );

			m_pData->Alloc();

			m_pData->ConvertArray( 0 );

			m_RawDataFile.Close();

		}
		else {
			m_RawData.uiByte = -1;
			m_RawData.uiDataItems = -1;
		}
		
	}

	//////////////////////////////////////////////////////////////////////////
	else if( m_RawData.enDataType == en_IQ_DATA && m_RawData.enUnitType == en_ELINT ) {
		if( m_RawDataFile.m_hFile == (HANDLE) -1 && m_RawDataFile.Open( m_strPathname.GetBuffer(), CFile::shareDenyNone | CFile::typeBinary) == TRUE ) {
			if( m_pData == NULL ) {
				m_pData = new CEIQ( & m_RawData );

				m_gMapData.insert( make_pair( m_strPathname, m_pData ) );
			}
		}

		if( iFileIndex <= 0 ) {
			m_iFileIndex = 0;

			m_dwFileEnd = m_RawDataFile.SeekToEnd();
			m_RawDataFile.Seek( 0, CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(TNEW_IQ)*IQ_ITEMS );

			m_RawData.uiDataItems = m_RawData.uiByte /*m_dwFileEnd*/ / sizeof(TNEW_IQ);

			iDataItems = m_RawData.uiByte / sizeof(TNEW_IQ);

			m_pData->ConvertArray( iDataItems, -1 );

		}
		else {
			m_iFileIndex = iFileIndex;

			//m_RawDataFile.Seek( sizeof(STR_PDWFILE_HEADER) + sizeof(UDRCPDW)*(m_iFileIndex*IQ_ITEMS), CFile::begin );

			m_RawData.uiByte = m_RawDataFile.Read( gstpRawDataBuffer, sizeof(TNEW_IQ)*IQ_ITEMS );

			iDataItems = m_RawData.uiByte / sizeof(TNEW_IQ);

			m_pData->ConvertArray( iDataItems, 0 );
		}

	}

	else {
		printf("\n Error !!");

	}

	dwFileRead = m_RawDataFile.Seek( 0, CFile::current );
	if( m_dwFileEnd == dwFileRead ) {
		m_RawDataFile.Close();
		Log( enNormal, _T("∆ƒ¿œ¿ª ¥›Ω¿¥œ¥Ÿ.") );
	}
	else {
		Log( enNormal, _T("∆ƒ¿œ¿ª ¥›¡ˆ æ æ“Ω¿¥œ¥Ÿ !!") );
	}

	if( m_RawData.enDataType == en_PDW_DATA ) {
		STR_PDW_DATA *pData=(STR_PDW_DATA *) m_pData->GetData();
		iFilteredDataItems = pData->iDataItems;
	}
	else {
		STR_IQ_DATA *pData=(STR_IQ_DATA *) m_pData->GetData();
		iFilteredDataItems = pData->iDataItems;
	}

	Log( enNormal, _T("«ˆ¿Á ¿ßƒ°[%d], √— ∞≥ºˆ[%d], « ≈Õ∏µ ∞≥ºˆ[%d]") , m_iFileIndex, iDataItems, iFilteredDataItems );

	return dwFileRead == m_dwFileEnd;

}

/**
 * @brief     
 * @return    void *
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/02/27 21:49:58
 * @warning   
 */
void *CDataFile::GetData() 
{ 
	if( m_pData != NULL ) 
		return m_pData->GetData(); 
	else 
		return NULL; 
}

/**
 * @brief     
 * @param     CData * pData
 * @return    void
 * @author    ¡∂√∂»Ò (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2020/03/12 19:03:55
 * @warning   
 */
 void CDataFile::SetData( CData *pData )
{
	m_pData = pData;

	++ m_pData->m_uiWindowNumber;

}


