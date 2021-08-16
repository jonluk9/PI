/////////////////////////////////////////////////////////////////
//	INSPWH.cpp : interface of the CInspWH class
//
//	Description:
//		PI9000 Application Software
//
//	Date:		Thursday, September 09, 2010
//	Revision:	1.00
//
//	By:			PI9000
//				CSP
//
//	Copyright @ ASM Assembly Automation Ltd., 2010.
//	ALL rights reserved.
//
/////////////////////////////////////////////////////////////////

#pragma once

#include "WorkHolder.h"
#include "PRTaskStn.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef INSPWH_EXTERN
extern BOOL					g_bStopCycle;
extern DOUBLE				g_dGToGOffset;
extern LONG					g_lGlassInputMode;
extern BOOL					g_bFlushMode; //20130408

extern STATUS_TYPE			INSPWH_Status[MAX_NUM_OF_WH];
extern STATUS_TYPE			INPICKARM_Status;
extern STATUS_TYPE			OUTPICKARM_Status;
extern STATUS_TYPE			INSPOPT_Status;
extern STATUS_TYPE			INSPWH_GLASS_Status[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];

extern AUTOMODE				AutoMode;
extern BOOL					g_bDiagMode;
extern LONG					g_lDiagnSteps;
	
extern GLASS_INFO			g_stGlass;

	//wizard
extern BOOL					g_bWizardEnable;
extern CString				g_szWizardTitle;
extern ULONG				g_ulWizardSubTitle;
extern CString				g_szWizardStep;
extern CString				g_szWizardMess;
extern CString				g_szWizardMessPrev;
extern CString				g_szWizardMessNext;
extern CString				g_szWizardBMPath;
extern BOOL					g_bWizardEnableBack;
extern BOOL					g_bWizardEnableNext;
extern BOOL					g_bWizardEnableExit;
extern ULONG				g_ulWizardStep;
extern LONG					g_lWizardInProgress;

extern ULONG				g_ulUserGroup;
extern SETUP_POSN_XY		g_stSetupPosn[NUM_OF_POSN_XY];
extern SETUP_LEVEL_Z		g_stSetupLevel[NUM_OF_LEVEL_Z];
extern SETUP_PARA			g_stSetupPara[NUM_OF_PARA];

#ifdef GATEWAY
extern GATEWAY_STATUS_TYPE PI9000_Machine_Status;
extern GATEWAY_STATUS_TYPE COF902_Machine_Status;
extern GATEWAY_STATUS_TYPE COF902_OUTPICKARM_Status;
extern GATEWAY_STATUS_TYPE PI9000_INSPWH_Status;
#endif
#endif

// Control Signals
//
typedef enum
{
	INSPWH_XY_CTRL_NOTHING = 0, 
	INSPWH_XY_CTRL_GO_HOME, 
	INSPWH_XY_CTRL_GO_STANDBY_POSN, 
	INSPWH_XY_CTRL_GO_LOAD_POSN, 
	INSPWH_XY_CTRL_GO_LOAD_OFFSET, 
	INSPWH_XY_CTRL_GO_LOAD_POSN_OFFSET,
	INSPWH_XY_CTRL_GO_UNLOAD_POSN, 
	INSPWH_XY_CTRL_GO_UNLOAD_OFFSET,
	//INSPWH_XY_CTRL_GO_DL_PR_CALIB_POSN,
	//INSPWH_XY_CTRL_GO_UL_PR_CALIB_POSN,
	//INSPWH_XY_CTRL_GO_UL_G1_INSP_POSN,
	//INSPWH_XY_CTRL_GO_UL_G2_INSP_POSN,
	INSPWH_XY_CTRL_GO_PI_POSN,

} INSPWH_XY_CTRL_SIGNAL;

// Control Signals
//
typedef enum
{
	INSPWH_T_CTRL_NOTHING = 0, 
	INSPWH_T_CTRL_GO_HOME, 
	INSPWH_T_CTRL_GO_STANDBY, 
	INSPWH_T_CTRL_GO_UNLOAD, 

} INSPWH_T_CTRL_SIGNAL;

// Control Signals
//
typedef enum
{
	INSPWH_PR_POSN_NOT_GO = -1,
	INSPWH_PR_POSN_PR1 = 0, 
	INSPWH_PR_POSN_PR2, 
	INSPWH_PR_POSN_ALIGN_CENTRE, 
	INSPWH_PR_POSN_ALIGN_PT1, 
	INSPWH_PR_POSN_ALIGN_PT2, 
	INSPWH_PR_POSN_TOTAL, 

} INSPWH_PR_POSN;

//typedef enum
//{
//	INSPWH_Z_CTRL_NOTHING = 0, 
//	INSPWH_Z_CTRL_GO_HOME, 
//	INSPWH_Z_CTRL_GO_STANDBY_LEVEL, 
//	INSPWH_Z_CTRL_GO_CALIB_LOAD_LEVEL, 
//	INSPWH_Z_CTRL_GO_CALIB_UNLOAD_LEVEL, 
//	INSPWH_Z_CTRL_GO_REJ_CALIB_UNLOAD_LEVEL,
//	INSPWH_Z_CTRL_GO_PR_CALIB_LEVEL,
//	INSPWH_Z_CTRL_GO_PRE_LOAD_LEVEL, 
//	INSPWH_Z_CTRL_GO_LOAD_LEVEL, 
//	INSPWH_Z_CTRL_GO_PRE_UNLOAD_LEVEL, 
//	INSPWH_Z_CTRL_GO_UNLOAD_LEVEL, 
//	INSPWH_Z_CTRL_GO_REJ_UNLOAD_LEVEL, 
//	INSPWH_Z_CTRL_GO_PR_LEVEL,
//	INSPWH_Z_CTRL_GO_UL_PR_LEVEL,
//
//} INSPWH_Z_CTRL_SIGNAL;

// States
//
typedef enum
{
	INSPWH_IDLE_Q = 0, 

	INSPWH_REQUEST_GLASS_Q,
	INSPWH_WAIT_LOAD_GLASS_COMPLETE_Q,
	INSPWH_WAIT_INSPOPTIC_ACK_Q,
	INSPWH_WAIT_PLACEMENT_INSP_COMPLETE_Q,
	INSPWH_GO_UNLOAD_GLASS_Q,						//5

	INSPWH_WAIT_UNLOAD_GLASS_COMPLETE_Q,

	INSPWH_STOP_Q = 99,

} INSPWH_STATE;


class CInspWH : public CWorkHolder, public CPRTaskStn
{
	DECLARE_DYNCREATE(CInspWH)
	friend class CPI9000App;
	friend class CSettingFile;
	friend class CHouseKeeping;
	friend class CWinEagle;
	friend class CPickArm;
	friend class CRejectArm;
	friend class CInPickArm;
	friend class COutPickArm;
	friend class CInspOpt;
	friend class CWH1;
	friend class CWH2;


protected:
	GLASS_COF_ID m_stDeviceID; //20170406
	

protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID CycleOperation();
	virtual VOID AutoOperation();

	virtual BOOL GetGlass1Status() 
	{
		return m_bGlass1Exist;
	}
	virtual BOOL GetGlass2Status() 
	{
		return m_bGlass2Exist;
	}

public:
	CInspWH();
	virtual	~CInspWH();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:
	CInspWH	*m_pCOtherWH;
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------
	CSiInfo		m_stXLmtSmr;
	CSiInfo		m_stYLmtSnr; //20130614
	BOOL		HMI_bStandAlone;
	BOOL		m_bStartSearchDLnULTest;

	ENC_CALIB_DATA	m_stCalibY[PREBOND_T_CALIB_DATA];
	// Add NuMotion 
	// 
	// ------------------------------------------------------------------------
	// End of Motor SI/SO
	// ------------------------------------------------------------------------

	CString		m_szRejectReason[MAX_NUM_OF_GLASS];
	
	// cycle status
	BOOL		m_bGlassWithError[MAX_NUM_OF_GLASS];
	DWORD		m_dwStartTime;
	DOUBLE		m_dProcessTime;
	LONG		m_lCurGlass;
	LONG		m_lCurSlave;
	MainbondType		m_lCurMBType;
	INSPWH_INFO	m_stInspWH[MAX_NUM_OF_GLASS];

	// Align Glass
	DOUBLE		m_dAlignAngle[MAX_NUM_OF_GLASS];			// store the current Glass Rotate Angle		
	MTR_POSN	m_mtrAlignCentre[MAX_NUM_OF_GLASS];			// store the current Glass Rotate Angle		
	MTR_POSN	m_mtrCurrAlignDelta[MAX_NUM_OF_GLASS];			// store the current Glass Delta x,y
	DOUBLE		m_dGlassRefAng[MAX_NUM_OF_GLASS];
	D_MTR_POSN	m_dmtrAlignRef[MAX_NUM_OF_GLASS];
	DOUBLE		m_dAlignRefAngle[MAX_NUM_OF_GLASS];
	
	//Posn
	MTR_POSN		m_mtrPRCalibPosn[MAX_NUM_OF_GLASS];
	MTR_POSN		m_mtrULPRCalibPosn;
	MTR_POSN		m_mtrPR1Posn[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	MTR_POSN		m_mtrPR2Posn[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	MTR_POSN		m_mtrRejectGlassPosn[MAX_NUM_OF_GLASS];
	MTR_POSN		m_mtrRejectGlassOffset[MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrPR1Posn[MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrPR2Posn[MAX_NUM_OF_GLASS];

	MTR_POSN		m_mtrULRefPosn;							// Position for Camera centre aligns with InspWH centre
	MTR_POSN		m_mtrDL1RefPosn;							// Position for Camera centre aligns with InspWH centre
	MTR_POSN		m_mtrDL2RefPosn;							// Position for Camera centre aligns with InspWH centre

	
	MTR_POSN		m_mtrAlignUnloadOffset;

	LONG			m_lTInspPosn[MAX_NUM_OF_GLASS];
	LONG			m_lTOffset;				// Dynamic No need Save
	LONG			m_lPinSpringTestCount; //20121009 pin detection spring test
	LONG			m_lPolarizerThickness[MAX_NUM_OF_GLASS]; //20120927 pin detect polarizer
	BOOL			m_bMeasurePolarizerThickness; //20120927 pin detect polarizer

	LONG			m_lUnloadXOffset[MAX_NUM_OF_GLASS];	
	LONG			m_lUnloadYOffset[MAX_NUM_OF_GLASS];	
	DOUBLE			m_dUnloadTOffset[MAX_NUM_OF_GLASS];	


	//// Level
	//LONG			m_lPRLevel[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	//LONG			m_lULPRLevel[MAX_NUM_OF_GLASS];


	LONG		m_lCycleTestMethod;
	LONG		m_lCycleMoveTDirection;
	DOUBLE		m_dCurCalibAng;
	DOUBLE		m_dCurCalibX;
	DOUBLE		m_dCurCalibY;

	MainbondType	m_lCurMBHead;
	LONG			m_lCurInspWH;

	GlassNum		TA5PlaceAction;
	INT				TA5PlaceActionString;		//TA5 Place Action
	//GlassNum		m_lCurGlass;

	//Counter
	LONG			m_lPR2AlignErrorCnt;
	LONG			m_lGlassAlignPROKCnt;
	LONG			m_lPR1AlignErrorCnt;
	LONG			m_lGlassInspectOKCnt;
	LONG			m_lGlassInspectRejectCnt;

	BOOL			m_bRejectAlways;
	BOOL			m_bMoveT;
	LONG			m_lEncDiff_y;
	LONG			m_lULDL1Diff_x;
	LONG			m_lULDL2Diff_x;

#ifdef MB_FORCE_ADC
	CPortInfo	m_stADCPort; //20151229
#endif
	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------
	
#ifdef MB_FORCE_ADC
public:
	BOOL GetADCValue(LONG &lForce);
	DOUBLE RawADCToForce(LONG lRawData, DOUBLE dCperLbf);
	LONG	m_lForceADCValue;		//raw value from NU-Motion
protected:
#endif
	// ------------------------------------------------------------------------
	// Virtual PRTaskStn Function delcaration
	// ------------------------------------------------------------------------
	// All Virtual Function from PRTaskStn.cpp
	virtual CMotorInfo &GetMotorX(); 
	virtual CMotorInfo &GetMotorY();
//	virtual CMotorInfo &GetMotorZ(WHZNum lWHZ = WHZ_NONE);
	virtual CMotorInfo &GetMotorT();

	virtual INT SyncX();
	virtual INT SyncY();
	virtual INT SyncZ();
	virtual INT SyncT();

//	virtual INT MoveZToPRCalibLevel(BOOL bWait = GMP_WAIT);
	virtual INT MoveXYToPRCalibPosn(BOOL bWait = GMP_WAIT);

//	virtual INT MoveZToPRLevel(BOOL bWait = GMP_WAIT);
//	INT MoveZToPinDetectLevel(BOOL bWait = GMP_WAIT); //20120927 pin detect polarizer
	virtual INT MoveXYToCurrPR1Posn(BOOL bWait = GMP_WAIT);
	virtual INT MoveXYToCurrPR2Posn(BOOL bWait = GMP_WAIT);

	virtual INT MoveAbsoluteY(LONG lCntY, BOOL bWait);
	virtual INT MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait);
	virtual INT MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait);

	virtual INT MoveTToStandby(BOOL bWait = GMP_WAIT);
	virtual INT MoveRelativeT(LONG lMtrCnt, BOOL bWait);
	virtual INT MoveZToPRCalibLevel(BOOL bWait = GMP_WAIT);

	virtual PRU &SelectCurPRU(ULONG ulPRU);

	INT AutoDetectPolarizerThickness();
	INT Calib2ndEncoderOperation();
	LONG HMI_RotaryEncoderCalibration(IPC_CServiceMessage &svMsg);
	LONG HMI_Find1stEnc2ndEncRelation(IPC_CServiceMessage &svMsg);
	LONG HMI_GoCORPosn(IPC_CServiceMessage &svMsg);
	LONG HMI_SetDiagnStepsByMicron(IPC_CServiceMessage &svMsg);
	VOID SetDiagnStepsByMicron(ULONG ulSteps);
	VOID PrintCalibTData();
	VOID Find1stEnc2ndEncRelation();
	LONG FirstEncToSecondEnc(LONG lMotorCount);
	LONG SecondEncToFirstEnc(LONG lMotorCount);
	LONG DL1ToUL(LONG lMotorCount);
	LONG DL2ToUL(LONG lMotorCount);
	
	// ------------------------------------------------------------------------
	// END Virtual PRTaskStn Function delcaration
	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	// Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	// All Virtual Function from PRTaskStn.cpp
	virtual BOOL PickGlassOperation(GlassNum lGlassNum);

	// ------------------------------------------------------------------------
	// END Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	CString GetSequenceString();
	VOID OperationSequence();

	LONG SetGlassStatus(STATUS_TYPE StationStatus, STATUS_TYPE *pGLASS_Status);


	BOOL IsInspWHEmpty();
	VOID InspWHSetVacON();
	BOOL IsTA5PlaceGlassOK();	
	BOOL IsUnloadWHReady();
	BOOL IsLoadGlassComplete();
	VOID SetUnloadGlassStatus();

	// PR
	VOID UpdateInspPRPosn(LONG lGlass);
	BOOL SearchDLnUL();
	BOOL SearchDLnGoULPR1Posn();
	BOOL SearchDLnGoULPR2Posn();
	LONG HMI_SearchDLnUL(IPC_CServiceMessage &svMsg);
	BOOL SearchDLnULTest();
#ifdef GATEWAY
//	VOID UpdateINSPWHStatus();
//	VOID UpdatePI9000MachineStatus();
	LONG HMI_SetStandAlone(IPC_CServiceMessage &svMsg);
#endif

	// HandShaking with COF902 
	BOOL IsTA5CanelReq();
	BOOL IsTA5PlaceMBUnitReq();
//	VOID InspWHGrantPermissionToTA5();
	BOOL IsTA5CheckGlassExistReq();

	VOID InspWHVacOffACK();
	BOOL IsTA5PlaceGlassComplete();
	VOID InspWHLoadGlassDone();
	VOID UpdateInspWHGlassStatus();

	//Motion
	INT MoveXYToCurPRPosn(LONG lPRU);

	//INT MoveZToStandbyLevel(LONG lGlassNum, BOOL bWait = GMP_WAIT);
	//INT MoveZToLoadLevel(LONG lGlassNum, BOOL bWait = GMP_WAIT);
	//INT MoveZToPreLoadLevel(LONG lGlassNum, BOOL bWait = GMP_WAIT);
	//INT MoveZToUnloadLevel(LONG lGlassNum, BOOL bWait = GMP_WAIT);
	//INT MoveZToRejUnloadLevel(LONG lGlassNum, BOOL bWait = GMP_WAIT);
	//INT MoveZToPRLevel(LONG lGlassNum, BOOL bWait = GMP_WAIT);
	//INT MoveZToPreUnloadLevel(LONG lGlassNum, BOOL bWait);


	INT MoveXYToStandbyPosn(BOOL bWait = GMP_WAIT);
	INT MoveXYToLoadPosn(BOOL bWait = GMP_WAIT);
	INT MoveXYToUnloadPosn(BOOL bWait = GMP_WAIT);
	INT MoveXYToUnloadRejPosn(GlassNum lGlassNum, BOOL bWait);

	INT MoveXY(INSPWH_XY_CTRL_SIGNAL ctrl, BOOL bWait);
	INT MoveT(INSPWH_T_CTRL_SIGNAL ctrl, BOOL bWait);
	//INT MoveZ1(INSPWH_Z_CTRL_SIGNAL ctrl, BOOL bWait);
	//INT MoveZ2(INSPWH_Z_CTRL_SIGNAL ctrl, BOOL bWait);
	//INT MoveZ(WHZNum lWHZNum, INSPWH_Z_CTRL_SIGNAL ctrl, BOOL bWait);		
	
	// calc
	BOOL CalcXYToUnloadOffset(GlassNum lGlassNum, MTR_POSN *mtrOffset, LONG *lOffsetT);
	BOOL SetGlassRef(LONG lGlass, BOOL bSetAng);
	VOID CalcAlignRefPt(LONG lGlass);
	VOID CalcAlignPt(GlassNum lGlass); // Add me Later
	VOID PrintRejectGlassInfo(LONG lGlass);

	// Module Setup Related
	VOID UpdateSetupStatus();
	//VOID SelectPRU(ULONG ulPRU);
	VOID SelectPRU(ULONG ulPRU, BOOL bIsMove = TRUE); //20121221
	VOID SetupWizard();
	VOID SetupPRPatternPreTask(PRU *pPRU);
	VOID SetupPRPatternPostTask();

	VOID SetupAlignPt();

	VOID ModuleSetupPreTask();
	VOID ModuleSetupPostTask();
	VOID ModulePRSetupPreTask();
	VOID ModulePRSetupPostTask();

	BOOL ModuleSetupSetPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupGoPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupSetLevel(LONG lSetupTitle = 0);
	BOOL ModuleSetupGoLevel(LONG lSetupTitle = 0);
	BOOL ModuleSetupSetPara(PARA stPara);

	VOID UpdateModuleSetupPosn();
	VOID UpdateModuleSetupLevel();
	VOID UpdateModuleSetupPara();

	INT SetupGoStandbyPosn();
	INT SetupGoLoadGlassPosn();
	INT SetupGoLoadGlassOffset(); //20131111
	INT SetupGoUnloadGlassPosn();
	INT SetupGoUnloadGlassOffset();
	//INT SetupGoRejectG1Posn();
	//INT SetupGoRejectG2Posn();
	//INT SetupGoRejectG1Offset();
	//INT SetupGoRejectG2Offset();
	INT SetupGoInspOptStandbyPosn();
	INT SetupGoInspOptPrePRPosn(WHNum lCurWH);
	INT SetupGoULPRCalibPosn();
	INT SetupGoPRCalibPosn(LONG lCurGlass);
	INT SetupGoULRefPosn();
	INT SetupGoDL1RefPosn();
	INT SetupGoDL2RefPosn();
	INT SetupGoTStandbyPosn();

	INT SetupGoStandbyLevel();
	//INT SetupGoCalibLoadLevel(WHZNum lCurWHZ);
	//INT SetupGoCalibUnloadLevel(WHZNum lCurWHZ);
	INT SetupGoPRCalibLevel();
	INT SetupGoDLPRCalibLevel();
	//INT SetupGoUnloadLevel(WHZNum lCurWHZ);
	INT SetupGoPRLevel(LONG lCurGlass);
	INT SetupGoULPRLevel(LONG lCurGlass);
	INT SetupGoDL1RefLevel();
	INT SetupGoDL2RefLevel();
	INT SetupGoULRefLevel();
	//INT SetupGoRejCalibUnloadLevel(WHZNum lCurWHZ);
	//INT SetupGoRejUnloadLevel(WHZNum lCurWHZ);
	//INT Calib2ndEncoderOperation();
	INT KeepQuiet();

	virtual VOID PrintMachineSetup(FILE *fp);
	virtual VOID PrintDeviceSetup(FILE *fp);
	virtual VOID PrintCalibSetup(FILE *fp);
#ifdef EXCEL_MACHINE_DEVICE_INFO
	virtual BOOL printMachineDeviceInfo();
#endif


	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------
	// Values are for HMI display only! Users should not use these in coding.	
	
	// SetupPage
	BOOL	HMI_bXYControl;
	//BOOL	HMI_bZ1Control;
	//BOOL	HMI_bZ2Control;
	BOOL	HMI_bTControl;
	BOOL	HMI_bCursorControl;

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------
	// Module Related



	//PR Setup Page
	LONG HMI_SetPRSelected(IPC_CServiceMessage &svMsg);
	LONG HMI_SelectGlassNum(IPC_CServiceMessage &svMsg);
	LONG HMI_SelectWHNum(IPC_CServiceMessage &svMsg);
	LONG HMI_SelectPRU(IPC_CServiceMessage &svMsg);
	LONG HMI_UpdateSetupStatus(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRPosn(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRLevel(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchAp1(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchAp2(IPC_CServiceMessage &svMsg);

	BOOL SearchAlignCentre(INSPWH_PR_POSN gotoPosn);

	LONG HMI_SearchCentre(IPC_CServiceMessage &svMsg);
	LONG HMI_StartPRCycleTest(IPC_CServiceMessage &svMsg);
	LONG HMI_StartSearchDLnULCycle(IPC_CServiceMessage &svMsg);

	LONG HMI_SetG1XUnloadOffset(IPC_CServiceMessage &svMsg);
	LONG HMI_SetG1YUnloadOffset(IPC_CServiceMessage &svMsg);
	LONG HMI_SetG1TUnloadOffset(IPC_CServiceMessage &svMsg);
	LONG HMI_SetG2XUnloadOffset(IPC_CServiceMessage &svMsg);
	LONG HMI_SetG2YUnloadOffset(IPC_CServiceMessage &svMsg);
	LONG HMI_SetG2TUnloadOffset(IPC_CServiceMessage &svMsg);
	LONG HMI_MeasureSetZero(IPC_CServiceMessage &svMsg); //20121016
	LONG HMI_MeasureEnable(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleAlwaysReject(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleMoveT(IPC_CServiceMessage &svMsg);
	LONG HMI_MoveTest(IPC_CServiceMessage &svMsg);

	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();

	virtual LONG IPC_SaveMachineRecord();
	virtual LONG IPC_LoadMachineRecord();
	virtual LONG IPC_SaveDeviceRecord();
	virtual LONG IPC_LoadDeviceRecord();

};

