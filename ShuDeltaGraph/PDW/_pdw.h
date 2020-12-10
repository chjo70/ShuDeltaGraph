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




union UZPOCKETPDW
{
#define uint32_t unsigned int

	struct {
		//
		// ������ ����ü : ����, ��ȣ����
		union {
			uint32_t pdw_status;
			struct {
				uint32_t cw_pulse		: 1; // '0' - pulse, '1' - CW
				uint32_t pmop_flag		: 1; // '0' - No-mop, '1'-mop
				uint32_t fmop_flag		: 1; // '0' - No-mop, '1'-mop
				uint32_t false_pdw		: 1; // '0' - ture, '1'-false
				uint32_t fmop_dir		: 2; // '0' - tri, '1' - up, '2' - down, '3' - unknown
				uint32_t reserved		: 10;

				uint32_t fmop_bw		: 16; // res = 1.953125 MHz
			} stPdw_status;
		} uniPdw_status;

		//
		// ������ ����ü : ����, ��ȣ����
		union {
			uint32_t	pdw_dir_pa;
			struct {
				uint32_t doa				: 12; // res = 0.087890625 deg
				uint32_t di				: 1; // '0' - Valid, '1' - invalid
				uint32_t reserved		: 3;
				uint32_t pa				: 16; // res = 0 ~ 65536 (linear scale)
			} stPdw_dir_pa;
		} uniPdw_dir_pa;

		//
		// ������ ����ü : �޽���, ���ļ�
		union {
			uint32_t pdw_pw_freq;
			struct {
				uint32_t pulse_width	: 24;
				uint32_t frequency_L	: 8;
			} stPdw_pw_freq;
		} uniPdw_pw_freq;

		//
		// ������ ����ü : ���� ���ļ� �� TOA
		union {
			uint32_t pdw_freq_toa;
			struct {
				uint32_t frequency_H	: 8;
				uint32_t pdw_phch		: 8;
				uint32_t toa_L			: 16;
			} stPdw_freq_toa;
		} uniPdw_freq_toa;

		//
		// ������ ����ü : ���� TOA
		union {
			uint32_t pdw_toa_edge;
			struct {
				uint32_t toa_H			: 28;
				uint32_t edge			: 1;
				uint32_t reserved		: 3;
			} stPdw_toa_edge;
		} uniPdw_toa_edge;

		//
		// ������ ����ü : PDw
		union {
			uint32_t pdw_index;
			struct {
				uint32_t index          : 16;
				uint32_t reserved       : 16;
			} stPdw_index;
		} uniPdw_index;

	} ;

	unsigned char chData[24];

} ;

union DMAPDW {
	char chData[32];
	union UZPOCKETPDW uPDW;

};

////////////////////////////////////////////////////////////////////////////////////

//#define _spTimeres              (20000000)					// 1 sec / 50 ns */
//#define _spOneSec               (_spTimeres)				// 1 sec  / 50 ns */
//#define _spOneMilli             (_spOneSec/1000)		// 1 msec / 50 ns */
//#define _spOneMicrosec          (_spOneMilli/1000)

// PDW�� Phase ����

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



//////////////////////////////////////////////////////////////////////////

// KFX ����
#define KPDW_CW             1
#define KPDW_PULSE          0

//////////////////////////////////////////////////////////////////////////

#define PDW_WORD_LEN        4
#define PDW_WORD_CNT        4

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
	long long int ullTOA;

	int iFreq;
	int iPulseType;
	int iPA;
	int iPW;
	int iPFTag;
	int iAOA;

	float fPh1;
	float fPh2;
	float fPh3;
	float fPh4;

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
	unsigned char aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int iIsStorePDW;
	int iCollectorID;
	ENUM_BANDWIDTH enBandWidth;

	UINT count;
	_PDW stPDW[MAX_PDW];

} STR_PDWDATA ;
#endif

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//

#define _701_LENGTH_OF_TASK_ID			(20)		//����ID ���ڿ� ���� (TBD)

#pragma pack(push, 1)

typedef struct {
	unsigned int uiAcqTime;
	unsigned int uiAcqTimeMilSec;
	int iPDWSetID;
	int iSDFID;
	unsigned char aucTaskID[_701_LENGTH_OF_TASK_ID];
	int iSearchBandID;
	int	iAETID;	
	int	iLOBID;
	int	iNumOfPDW;

} SRxPDWHeader;

typedef struct {
	unsigned long long int	ullTOA;
	int	iSignalType;
	int	iPolFlag;
	int	iFMOPFlag;
	int	iPMOPFlag;
	int	iBlankingTag;
	int	iIsChannelChangePOP;
	int	iBLK;

	int	iDirectionVaild;
	int	iAcqDirectionLimit;
	int	iChannelNumber;
	int	iPA;
	int	iFreq;
	int	iDirection;
	int	iPolization;
	int	iPFTag;
	int	iPW;
	UINT uiPDWID;

} SRXPDWDataRGroup;

typedef struct {
	unsigned int uiAcqTime;
	unsigned int uiAcqTimeMilSec;
	int iGain;
	int iTaskType;
	int iSDFID;
	int iPDWSetID;
	unsigned char aucTaskID[_701_LENGTH_OF_TASK_ID];
	long long int llToa;
	int iSearchBandID;
	unsigned int uiRelatedPDWSN;
	int iChannelNumber;
	int	iAETID;	
	int	iLOBID;
	unsigned int uiNumOfSample;
	unsigned int _reserved;

} SRxIQHeader;

typedef struct {
	short sQ;
	short sI;

} SRXIQDataRGroup;

#pragma pack(pop)


typedef struct {
	unsigned int uiSearchBandNo;
	unsigned int uiSignalDeletingStatus;
	unsigned int uiSignalCount;

	// ZPocketSonata ������ ���� ID �߰�
	unsigned int uiBoardID;

} STR_PDWFILE_HEADER;




#endif  // #ifndef _PDW_H_

