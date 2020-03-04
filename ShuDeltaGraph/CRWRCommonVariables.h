/**
* @file CRWRCommonVariables.h
* RWR Operation CSC���� ����ϴ� ���뺯�� �� �Լ� ��� ����
* RWR Operation CSC�� ���뺯�� �� �Լ��� �����Ѵ�.
*/
#ifndef RWRCOMMONVARIABLES_H_
#define RWRCOMMONVARIABLES_H_

//#include <Data/CPDWContainer.h>
//#include <SignalProcessing/CSignalProcessingVariables.h>
//#include <Util/Time/CStopWatch.h>
// #include <ns_api.h>
// #include <algorithm>
// #include <stdio.h>
// #include <vector>
// #include <list>
// #include <map>

/**
* @brief �������¸޽��� 
* COLLECT_STOP: ��������, ��밪: 1
* MDF_MODE_CHANGE_REQ: MDF��庯��, ��밪: 2
* THREAT_INIT: �����ʱ�ȭ, ��밪: 3
* SEARCH_BAND_SEND_END: Ž���뿪�����Ϸ�, ��밪: 4
* UNKNOWN_MSG: ��ȿ �޽���, ��밪: 5
* SIZE_OF_NEXT_ACTION: ������
*/
typedef enum _NextActionMsg
{
	COLLECT_STOP 							= 1,
	MDF_MODE_CHANGE_REQ						= 2,
	THREAT_INIT								= 3,
	SEARCH_BAND_SEND_END					= 4, 
	UNKNOWN_MSG								= 5,
	SIZE_OF_NEXT_ACTION
}NextActionMsg;

/**
* @brief ����Ž���뿪 ���� 
* UNKNOWN_NO: �̽ĺ�, ��밪: 0
* JLT_NO: Jamming LT, ��밪: 1
* ILT_NO: Interference LT, ��밪: 2
* XCC_NO: X�뿪 ����Ž���뿪 ��û, ��밪: 3
* SCAN_NO: ��ĵ�м� ����Ž���뿪 ����, ��밪: 4
*/
typedef enum _DynamicBandType
{
	UNKNOWN_NO								= 0,
	JLT_NO 									= 1,
	ILT_NO	 								= 2,
	XCC_NO									= 3,
	SCAN_NO									= 4
}DynamicBandType;

/**
* @brief PDW�����̳ʻ���
* INIT: �ʱ����, ��밪: 0x0000
* COLLET_START: ��������, ��밪: 0x1000
* COLLECT_END: �����Ϸ���� ���ſϷ�, ��밪: 0x0001
* PDW_STORE_REQ: PDW ���� ��û, ��밪: 0x0010
* FULL: PDW ���� �Ϸ�, ��밪: 0x0100
* PDW_STORE_END: �����Ϸ� ���� ���� �� PDW ���� �Ϸ�, ��밪: 0x0101
* PDW_STORING: �����Ϸ� ���� ���� �� PDW ���� ��û, ��밪: 0x0011
* SIGPROC_DOING: ��ȣó����, ��밪: 0x0111
* UNKNOWN: ������, ��밪: 0xFFFF
*/
typedef enum _EContainerState
{
	INIT							= 0x0000,		///< �ʱ����	
	COLLET_START					= 0x1000,		///< ��������
	COLLECT_END						= 0x0001,		///< �����Ϸ� ���� ���� �Ϸ�
	PDW_STORE_REQ					= 0x0010,		///< PDW ���� ��û
	FULL							= 0x0100,		///< PDW ���� �Ϸ�
	PDW_STORE_END					= 0x0101,		///< �����Ϸ� ���� ���� �� PDW ���� �Ϸ�
	PDW_STORING						= 0x0011, 		///< �����Ϸ� ���� ���� �� PDW ���� ��û
	SIGPROC_DOING					= 0x0111,		///< ��ȣó����
	UNKNOWN							= 0xFFFF,
}EContainerState;

/**
 * @brief �������ɻ���
 * CAN_COLLECT: ��������, ��밪: 0x0
 * CAN_NOT_COLLECT: �����Ұ���, ��밪: 0x1
 */
typedef enum _ECollectFlag
{
	CAN_COLLECT						= 0x0,		///< �ʱ����	
	CAN_NOT_COLLECT					= 0x1		///< �����Ϸ� ���� ���� �Ϸ�
}ECollectFlag;

/**
 * @brief �����뿪��� ���� ������
 * DRC_DES: DRC, ��밪: 0x0
 * JMMC_DES: JMMC, ��밪: 0x1
 */
typedef enum _DesType
{
	DRC_DES = 0,
	JMMC_DES = 1
}DesType;

/**
 * @brief ��ȣó�� ����ð�
 * PDW_STORE_ELAPSED_TIME: PDW���� ����ð�, ��밪: 0
 * SIG_ANALYSIS_TIME: ��ȣ�м� ����ð�, ��밪: 1
 * EMITTER_IDENTIFY_TIME: �����ͽĺ� ����ð�, ��밪: 2
 * INSERT_PRE_AET_TIME: PreAET ���� �ð�, ��밪: 3
 * MANAGE_THREAT_TIME: �������� �ð�, ��밪: 4
 * ARRANGE_THREAT_TIME: �������� �ð�, ��밪: 5
 * POST_CLEANING_TIME: �����ʱ�ȭ �ð�, ��밪: 6
 */
typedef enum _ElapsedTimeType
{
	PDW_STORE_ELAPSED_TIME 	= 0,
	SIG_ANALYSIS_TIME		= 1,
	EMITTER_IDENTIFY_TIME	= 2,
	INSERT_PRE_AET_TIME		= 3,
	MANAGE_THREAT_TIME		= 4,
	ARRANGE_THREAT_TIME		= 5, 
	POST_CLEANING_TIME		= 6,
}ElapsedTimeType;

/**
* @struct SAESAOPerationFreqData
* - ����ü ����
* AESA�����
* - �����������
* ����
*/
struct SAESAOPerationFreqData
{
	SAESAOPerationFreqData() : m_operationMode(0), m_freqMin(0), m_freqMax(0) {}
	
	unsigned short m_operationMode;
	unsigned int m_freqMin;
	unsigned int m_freqMax;
};


/**
* @struct SXCCDynamicBand
* - ����ü ����
* X�뿪 ����Ž���뿪��û ����
* - �����������
* ����
*/
struct SXCCDynamicBand
{
	SXCCDynamicBand() : m_beamID(0), m_freqMin(0), m_freqMax(0) {}
	
	unsigned int m_beamID;
	unsigned int m_freqMin;
	unsigned int m_freqMax;
};

/**
* @struct SInterferenceInfo
* - ����ü ����
* �����뿪��û����
* - �����������
* ����
*/
struct SInterferenceInfo
{
	SInterferenceInfo() : m_jammingAllocNo(0), m_AETNumber(0), m_freqMin(0), m_freqMax(0) {}
	
	unsigned char m_jammingAllocNo;
	unsigned int m_AETNumber;
	unsigned int m_freqMin;
	unsigned int m_freqMax;
};


/**
* @struct SJammingFreq
* - ����ü ����
* �����뿪���ļ�����
* - �����������
* ����
*/
struct SJammingFreq
{
	SJammingFreq() : m_freqMin(0.0), m_freqMax(0.0){}
	
	float m_freqMin;
	float m_freqMax;
};


/**
* @struct SDRCCollectEndState
* - ����ü ����
* ������������ ����ü
* - �����������
* ����
*/
struct SDRCCollectEndState
{
	SDRCCollectEndState() : m_searchBandNumber(0), m_isOVP(0), m_endState(0), m_PDWCount(0){}
				
	unsigned int m_searchBandNumber;	
	unsigned int m_isOVP;		
	unsigned int m_endState;				
	unsigned int m_PDWCount;					
};


/**
* @struct SCollectEndState
* - ����ü ����
* ��ȣ�����Ϸ� ����ü
* - �����������
* ����
*/
// struct SCollectEndState
// {
// 	SCollectEndState() : m_PDWCount(0), m_searchBandNumber(0), m_containerIndex(0), m_fullCheck(CAN_COLLECT), m_endState(0), m_collectFail(0), m_bandGroup(INIT_BAND){}
// 	
// 	void Init()
// 	{
// 		m_PDWCount = 0;
// 		m_searchBandNumber = 0;
// 		m_containerIndex = 0;
// 		m_fullCheck = CAN_COLLECT;
// 		m_endState = 0;
// 		m_collectFail = 0;
// 		m_bandGroup = INIT_BAND;
// 	}
// 	
// 	unsigned int m_PDWCount;					///< ��������
// 	unsigned int m_searchBandNumber;			///< Ž���뿪��ȣ
// 	int m_containerIndex;						///< PDW�����̳� �ε���
// 	ECollectFlag m_fullCheck;					///< Container Full ����
// 	unsigned int m_endState;					///< �Ϸ���� ����
// 	unsigned char m_collectFail;				///< �������л���
// 	EBandGroup m_bandGroup;						///< ���׷�
// };


/**
* @struct SInformationOfPDWs
* - ����ü ����
* ��ȣ�����Ϸ� ����ü
* - �����������
* ����
*/
struct SInformationOfPDWs
{
	SInformationOfPDWs() : m_searchBandNumber(0), m_eraseState(0), m_PDWCount(0){}
	
	unsigned int m_searchBandNumber;			///< �����Ϸ��� Ž���뿪��ȣ
	unsigned int m_eraseState;					///< �Ϸ���� ����
	unsigned int m_PDWCount;					///< PDW ����
};


/**
* @struct SReqPDW
* - ����ü ����
* PDW���ۿ�û ������ ����ü
* - �����������
* ����
*/
struct SReqPDW
{	
	SReqPDW() : m_bandNumber(0), m_PDWCount(0){}
	
	unsigned int m_bandNumber;					///< ��������
	unsigned int m_PDWCount;					///< Ž���뿪��ȣ
};

/**
* @struct SJLTData 
* - ����ü ����
* Jamming LT ������ ����ü
* - �����������
* ����
*/
struct SJLTData
{
	SJLTData() : m_AETNo(0), m_freqMin(0), m_freqMax(0){}
		
	void Init()
	{
		m_AETNo = 0;
		m_freqMin = 0;
		m_freqMax = 0;
	}
	
	unsigned int m_AETNo;						///< AET��ȣ
	unsigned int m_freqMin;						///< �ּ� ���ļ�
	unsigned int m_freqMax;						///< �ִ� ���ļ�
};

/**
* @struct SBandForLT 
* - ����ü ����
* LT�� ���� Ž���뿪
* - �����������
* ����
*/
struct SBandForLT
{	
	SBandForLT() : m_searchBandNumber(0), m_JLTAETNo(0), m_freqMin(0.0), m_freqMax(0.0){}
	
	int m_searchBandNumber;
	unsigned int m_JLTAETNo;
	float m_freqMin;
	float m_freqMax;
};
 
/**
* @struct SNewCollectData 
* - ����ü ����
* ������������
* - �����������
* ����
*/
struct SNewCollectData
{
	SNewCollectData() : m_bandGroup(0), m_currentNumber(0), m_nextNumber(0) {}
	
	unsigned int m_bandGroup;
	unsigned int m_currentNumber;
	unsigned int m_nextNumber;
};


/**
* @struct SScanCollectInfo
* - ����ü ����
* ��ĵ�󼼺м��� ���� �⺻����
* - �����������
* ����
*/
struct SScanCollectInfo
{	
	SScanCollectInfo() : m_searchNo(0), m_AETNo(0),  m_collectTime(0), m_collectCount(0){}
	
	int m_searchNo;
	int m_AETNo;
	float m_collectTime;
	unsigned int m_collectCount;
};

/**
* @struct SElapsedTimeData
* - ����ü ����
* ����ð� ���� ����ü
* - �����������
* ����
*/
// struct SElapsedTimeData
// {
// 	SElapsedTimeData() : pWatchTimer(NULL), m_PDWStoreTime(0.0), m_analysisTime(0.0), m_identifyTime(0.0), 
// 			m_preAETMakeTime(0.0), m_manageThreatTime(0.0), m_arrangeThreatTime(0.0), m_postCleaningTime(0.0), m_totalTime(0.0)
// 	{
// 		pWatchTimer = new CStopWatch(); 
// 	}
// 	
// 	~SElapsedTimeData()
// 	{
// 		if(pWatchTimer != NULL)
// 		{
// 			delete pWatchTimer;
// 		}
// 	}
// 	
// 	void Init()
// 	{
// 		m_PDWStoreTime = 0.0;
// 		m_analysisTime = 0.0;
// 		m_identifyTime = 0.0;
// 		m_preAETMakeTime = 0.0;
// 		m_manageThreatTime = 0.0;
// 		m_arrangeThreatTime = 0.0;
// 		m_postCleaningTime = 0.0;
// 		m_totalTime = 0.0;
// 	}
// 	
// //	CStopWatch* pWatchTimer;
// 	double m_PDWStoreTime;
// 	double m_analysisTime;
// 	double m_identifyTime;
// 	double m_preAETMakeTime;
// 	double m_manageThreatTime;
// 	double m_arrangeThreatTime;
// 	double m_postCleaningTime;
// 	
// 	double m_totalTime;
// };

/**
* union UDRCPDW
* - union ����
* PDW
* - �����������
* ����
*/
union UDRCPDW
{
	char chData[32];
	
#ifndef LITTLE_ENDIAN_MODE
	struct
	{
		//1
		unsigned int m_LSBTOA :			32;
		
		//2
		unsigned int m_MSBTOA :			10;
		unsigned int m_Ovp :			1;
		unsigned int m_PA :				9; 
		unsigned int m_Rsvd :			3;
		unsigned int m_DOA :			9;
		
		//3
		unsigned int m_ChNo :			4;
		unsigned int m_sigType :		1; // 0:CW, 1:Pulse
		unsigned int m_BLK :			1;
		unsigned int m_FMOP :			1;
		unsigned int m_PMOP :			1;
		unsigned int m_ChChg :			2;
		unsigned int m_DI :				1;
		unsigned int m_freq :			21; 
		
		//4
		unsigned char m_ft1 : 			1;
		unsigned char m_ft2 : 			1;
		unsigned char m_ft3 : 			1;
		unsigned char m_ft4 : 			1;
		unsigned char m_ft5 : 			1;
		unsigned char m_ft6 : 			1;
		unsigned char m_ft7 : 			1;
		unsigned char m_ft8 : 			1;
		unsigned char PWI : 			1;
		unsigned char TOAI:				1;
		unsigned int m_PW :				22;

		//5
		unsigned int m_PDWNum :			32;
		
		//6
		unsigned int m_Rsvd2 :			3;
		unsigned int m_FMOPMinFreq :	13;
		unsigned int m_Rsvd3 :			3;
		unsigned int m_FMOPMaxFreq :	13;
		
		//7
		unsigned int m_PMOPRsvdTime	:	20;
		unsigned int m_PMOPCount :		8;
		unsigned int m_Rsvd4 :			2;
		unsigned int m_PMOPMode :		2;
		
		//8
		unsigned int m_Rsvd5 :			16;
		unsigned int m_AntPAGap : 		8;
		unsigned char m_Rsvd6 : 		2;
		unsigned char m_secondAnt :		2;
		unsigned char m_Rsvd7 : 		2;
		unsigned char m_firstAnt : 		2;
		
	} sPDWFormat;

#else
	struct
	{
		//1
		unsigned int m_LSBTOA :			32;
		
		//2
		unsigned int m_DOA :			9;
		unsigned int m_Rsvd :			3;
		unsigned int m_PA :				9;
		unsigned int m_Ovp :			1;
		unsigned int m_MSBTOA :			10;
		
		//3
		unsigned int m_freq :			21;
		unsigned int m_DI :				1;
		unsigned int m_ChChg :			2;
		unsigned int m_PMOP :			1;
		unsigned int m_FMOP :			1;
		unsigned int m_BLK :			1;
		unsigned int m_sigType :		1;
		unsigned int m_ChNo :			4;
		
		//4
		unsigned int m_PW :				22;
		unsigned char TOAI:				1;
		unsigned char PWI : 			1;
		unsigned int m_ft8 : 			1;
		unsigned int m_ft7 : 			1;
		unsigned int m_ft6 : 			1;
		unsigned int m_ft5 : 			1;
		unsigned int m_ft4 : 			1;
		unsigned int m_ft3 : 			1;
		unsigned int m_ft2 : 			1;
		unsigned int m_ft1 : 			1;

		//5
		unsigned int m_PDWNum :			32;
		
		//6
		unsigned int m_FMOPMaxFreq :	13;
		unsigned int m_Rsvd3 :			3;
		unsigned int m_FMOPMinFreq :	13;
		unsigned int m_Rsvd2 :			3;
		
		//7
		unsigned int m_PMOPMode :		2;
		unsigned int m_Rsvd4 :			2;
		unsigned int m_PMOPCount :		8;
		unsigned int m_PMOPRsvdTime :	20;
		
		//8
		unsigned char m_firstAnt : 		2;
		unsigned char m_Rsvd7 : 		2;
		unsigned char m_secondAnt :		2;
		unsigned char m_Rsvd6 : 		2;
		unsigned int m_AntPAGap : 		8;
		unsigned int m_Rsvd5 :			16;

	}sPDWFormat;
#endif
};

#if CPU == _VX_PPCE6500
	#define SEND_MASKING(Value)		(ERWRTaskMsg)(((Value & 0xFFFF) << 8) | 0x11120000)
	#define RECV_MASKING(Value)		(ERWRTaskMsg)((Value & 0x0000FF00) >> 8)
#else
	#define SEND_MASKING(Value)		(ERWRTaskMsg)(((Value & 0xFFFF) << 8) | 0x11120000)
	#define RECV_MASKING(Value)		(ERWRTaskMsg)((Value & 0x0000FF00) >> 8)
#endif


extern bool sigProcLog;
extern bool sigProcTimeFlag;
extern bool totalTimeFlag;

void SigProcLogOff();
void SigProcLogOn();

void SigProcTimeOff();
void SigProcTimeOn();

void SigProcLogOff();
void SigProcLogOn();

/**
* @class CRWRCommonVariables 
* @brief
* RWR Operation manager CSC�� ���뺯�� �� �Լ��� �����Ѵ�
* - �����������
* ����
*/
// class CRWRCommonVariables
// {
// 	private : 
// 		/**
// 		* @brief �ִ� �½�ũ ����
// 		*/
// 		int m_taskCount;
// 		
// 		/**
// 		* @brief Ž���뿪��ȣ�� Ÿ�Ӿƿ� ī��Ʈ �� 
// 		*/
// 		std::map<int, bool> m_pTimeOutPDWContainersMap;
// 		
// 		/**
// 		* @brief ��ȣ�м� PA �ػ�
// 		*/
// 		double m_SigProcPAResolution;
// 		
// 		/**
// 		* @brief ��ȣ�м� AOA �ػ�
// 		*/
// 		double m_SigProcAOAResolution;
// 		
// 		/**
// 		* @brief �����Ϸ� Timeout ���� ����ó�� Ȯ�ο� PDW��Ű�� �ε���
// 		*/
// 		int m_PDWPackageIndex;
// 		
// 		/**
// 		* @brief �����Ϸ� Ÿ�̸� Ȱ��ȭ ���� �Ǵ� ����
// 		*/
// 		std::vector<bool> m_collectEndTimeoutList;
// 		
// 		/**
// 		* @brief ��ȣó�� ����ð� ���� ������ ��
// 		*/
// 		std::map<int, SElapsedTimeData*> m_SigProcElapsedTimerList;
// 		
// 		/**
// 		* @brief ��ȣó�� ����ð� ���� ������ �� type definition
// 		*/
// 		typedef std::map<int, SElapsedTimeData*> TimerMap;
// 		
// 	public:
// 		
// 		/**
// 		* @brief ��Ȯ�� ��ȣó���½�ũ ID
// 		*/
// 		const static int m_unknownSigProcID;
// 		
// 		/**
// 		* @brief ��ȿ�������� Ž���뿪��ȣ
// 		*/
// 		const static int m_notValidBandNo;
// 		
// 		/**
// 		* @brief ��ȿ��������  AET��ȣ
// 		*/
// 		const static int m_notValidAETNo;
// 		
// 		/**
// 		* @brief �½�Ʈ�� Ž���뿪��ȣ �ִ� ����
// 		*/
// 		const static int m_searchBandMaxCount;
// 		
// 		/**
// 		* @brief PDW �ִ� ����
// 		*/
// 		const static unsigned int m_MaxPDWCount;
// 
// 		/**
// 		* @brief PDW���� ��õ� Ƚ��
// 		*/
// 		const static int m_PDWStoreTryCount;
// 		
// 		/**
// 		* @brief ������û ��õ� Ƚ��
// 		*/
// 		const static int m_collectTryCount;
// 		
// 		/**
// 		* @brief ��������Ϸ�
// 		*/
// 		const static int m_normalCollectEnd;
// 		
// 		/**
// 		* @brief PDW �����γ� Full�Ϸ�
// 		*/
// 		const static int m_PDWContainerFullEnd;
// 		
// 		/**
// 		* @brief Timeout �Ϸ�
// 		*/
// 		const static int m_timeoutCollectEnd;
// 
// 		/**
// 		* @brief ��������
// 		*/
// 		const static int m_collectFail;
// 		
// 		/**
// 		* @brief �ð� �ػ�
// 		*/
// 		const static double	m_timeRes;
// 		
// 		/**
// 		* @brief PA �ػ�
// 		*/
// 		const static double m_PARes;
// 		
// 		/**
// 		* @brief PA ������
// 		*/
// 		const static double m_PAOffset;
// 		
// 		/**
// 		* @brief ���� �ػ�
// 		*/
// 		const static double	m_DOARes;
// 		
// 		/**
// 		* @brief ���ļ� �ػ�
// 		*/
// 		const static double m_freqRes;
// 		
// 		/**
// 		* @brief �޽��� �ػ�
// 		*/
// 		const static double m_PWRes;
// 		
// 		/**
// 		* @brief PMOP �ּҰ��� �ػ�
// 		*/
// 		const static double m_PMOPRes;
// 		
// 		/**
// 		* @brief PMOP �ּҰ��� �ػ�
// 		*/
// 		const static int m_FMOPOffSet;
// 		
// 		/**
// 		* @brief ���������Ǵ� �ð� ����
// 		*/
// 		const static int m_collectWaitMarginTime;
// 
// 		/**
// 		* @brief PDW���� ��� �ð�
// 		*/
// 		const static int m_storeWaitTime;
// 		
// 		/**
// 		* @brief ����Ž���뿪��ȣ ���� �Ӱ�ġ
// 		*/
// 		const static int m_dynamicBandNoCountThreshold;
// 		
// 		/**
// 		* @brief ILT����Ž���뿪���������� �ִ밪
// 		*/
// 		const static int m_ILTRefDataMax;
// 		
// 		/**
// 		* @brief ��ȿ RLog ������ ��ȣ
// 		*/
// 		const static int m_notAvailableSequence;
// 		
// 		CRWRCommonVariables(int PDWContainerCount);
// 		
// 		~CRWRCommonVariables();
// 		
// 		void SetResolutions(CPDWContainer* pPDWContainer);
// 		
// 		double GetSigProcPAResolution();
// 		
// 		double GetSigProcAOAResolution();
// 		
// 		bool CheckTimeOutPDWContainer(int PDWContainerIndex);
// 		
// 		void InsertTimeOutFlag(int PDWContainerIndex, bool flag);
// 		
// 		void SetTimeOutCollectEndFlag(int containerIndex, bool flag);
// 		
// 		bool GetTimeOutCollectEndFlag(int containerIndex2);
// 		
// 		CStopWatch* GetSigProcElapsedTimer(int sigProcIndex);
// 		
// 		void SaveCurrentElapsedTime(ElapsedTimeType timeType, int currSigProcIndex, double toSaveTime);
// 		
// 		SElapsedTimeData* GetCurrentElapsedTimeData(int currSigProcIndex);
// 		
// 		const char* GetConvertedSearchBandGroupString(EBandGroup searchBandGroup) const;
// 		
// 		double GetTotalSigProcElapsedTime(int completeSigProcIndex);		
// 		
// };

#endif
