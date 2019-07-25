/****************************************************************************************
 �� �� �� : _pdw.h
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ��    �� : PDW ����
 ��    �� : 
 �� �� �� : ��ö��
 �� �� �� : 
 ���泻�� : (����)
 �����Ͻ� : 2007-12-28 10:05:31
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 �Է����� : 
 ������� : 
 ���ø�� : 
 �ڷᱸ�� : (��������)
 ����ó��/���ͼ���:
 ������ �Ͻ� : 
*****************************************************************************************/

#ifndef _PDW_H_
#define _PDW_H_



#ifdef _SONATA_

//#define _spTimeres              (20000000)					// 1 sec / 50 ns */
//#define _spOneSec               (_spTimeres)				// 1 sec  / 50 ns */
//#define _spOneMilli             (_spOneSec/1000)		// 1 msec / 50 ns */
//#define _spOneMicrosec          (_spOneMilli/1000)

// PDW�� Phase ����
#define PDW_WORD_LEN        4
#define PDW_WORD_CNT        4
#define SizeOfPDW           16
#define ByteOfPhase         4
#define SizeOfPSEUDO_PDW    ByteOfPhase

// PDW ���� �� ���� ��
#define PDW_CW              4
#define PDW_CHIRPUP         2
#define PDW_CHIRPDN         3
#define PDW_PMOP            1
#define PDW_NORMAL          0
#define PDW_BIT             15

#define PDW_DV							(1)

// PDW ����, Platform�� � ���̳Ŀ� ���� UNIX �� �����Ͽ� ������ �Ǵ� ���� ��������.
#ifndef _WIN32
//##ModelId=452B0C500266
typedef union
{
	UCHAR bpdw[ PDW_WORD_CNT ][ PDW_WORD_LEN ];
	UINT wpdw[ PDW_WORD_CNT ];
	struct
	{
		UCHAR phase1 : 2; /* Phase 1 */
		UCHAR stat : 4;
		UCHAR direction_h : 2;
		UCHAR direction_l : 8;
		UCHAR dv : 1;
		UCHAR band : 2;
		UCHAR frequency_h : 5;
		UCHAR frequency_l : 8;

		UCHAR phase2 : 2; /* Phase 2 */
		UCHAR fil1 : 2;
		UCHAR pq : 1;
		UCHAR pulse_width_h : 3;
		UCHAR pulse_width_l : 8;
		UCHAR toa_1 : 8;
		UCHAR amplitude : 8;

		UCHAR phase3 : 2; /* Phase 3 */
		UCHAR fil2 : 6;
		UCHAR toa_2 : 8;
		UCHAR toa_3 : 8;
		UCHAR toa_4 : 8;

		UCHAR phase4 : 2; /* Phase 4 */
		UCHAR fil3 : 5;
		UCHAR chlp : 1;
		UCHAR channel : 8;
		UCHAR pmop : 8;
		UCHAR freq_diff : 8;
	} item;
} TNEW_PDW;

#else /* dos */
typedef union
{
	UCHAR bpdw[ PDW_WORD_CNT ][ PDW_WORD_LEN ];
	UINT wpdw[ PDW_WORD_CNT ];
	struct
	{
		UCHAR direction_h : 2;  /* Phase 1 */
		UCHAR stat : 4;
		UCHAR phase1 : 2;
		UCHAR direction_l : 8;
		UCHAR frequency_h : 5;
		UCHAR band : 2;
		UCHAR dv : 1;

		UCHAR frequency_l : 8;  /* Phase 2 */
		UCHAR pulse_width_h : 3;
		UCHAR pq : 1;
		UCHAR fil1 : 2;
		UCHAR phase2 : 2;
		UCHAR pulse_width_l : 8;
		UCHAR toa_4 : 8;

		UCHAR amplitude : 8;  /* Phase 3 */
		UCHAR fil2 : 6;
		UCHAR phase3 : 2;
		UCHAR toa_3 : 8;
		UCHAR toa_2 : 8;
		UCHAR toa_1 : 8;

		UCHAR chlp : 1; /* Phase 4 */
		UCHAR fil3 : 5;
		UCHAR phase4 : 2;          
		UCHAR channel : 8;
		UCHAR pmop : 8;
		UCHAR freq_diff : 8;
	} item ;
} TNEW_PDW;

#endif

#define SAMPLING_HZ_IN_NARROW_BAND		(71.111*1000000.)							// 71.111 MHz
#define SAMPLING_HZ_IN_WIDE_BAND			(154.125*1000000.)						// 154.111 MHz

#define NARROW_BAND_IN_SAMPLINGTIME		( 1. / SAMPLING_HZ_IN_NARROW_BAND )
#define WIDE_BAND_IN_SAMPLINGTIME		( 1. / SAMPLING_HZ_IN_WIDE_BAND )

// ������ ������ġ ������ ���� PDW ����
typedef struct  
{
	float Freq ;
	float PA ;
	float PW ;
	unsigned int TOA ;
	int Ref_Phase3 ;

} TNEW_SPDW;

// ��õ���� PDW
#define LENGTH_OF_TASK_ID			(19+1)		//����ID ���ڿ� ���� (TBD)
#define	MAX_PDW								(4096)

#ifndef _PDW_STRUCT
#define _PDW_STRUCT
typedef struct {
	long long int llTOA;

	int iFreq;
	int iPulseType;
	int iPA;
	int iPW;
	int iPFTag;
	int iAOA;

} _PDW;
#endif

#ifndef ENUM_BANDWIDTH_ENUM
#define ENUM_BANDWIDTH_ENUM
typedef enum {
	en5MHZ_BW=0,
	en50MHZ_BW,

} ENUM_BANDWIDTH ;
#endif

#ifndef _STR_PDWDATA
#define _STR_PDWDATA
typedef struct {
	unsigned char	aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int iIsStorePDW;
	int iCollectorID;
	ENUM_BANDWIDTH enBandWidth;

	UINT count;
	_PDW stPDW[MAX_PDW];

} STR_PDWDATA ;
#endif

#else

#endif



#endif  // #ifndef _PDW_H_
