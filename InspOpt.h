/////////////////////////////////////////////////////////////////
//	InspOpt.cpp : interface of the CInspOpt class
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

#include "PI9000Stn.h"
#include "PRTaskStn.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef INSPOPT_EXTERN
extern BOOL					g_bStopCycle;

extern STATUS_TYPE			INSPOPT_Status;
extern STATUS_TYPE			INSPWH_Status[MAX_NUM_OF_WH];
extern STATUS_TYPE			INSPWH_GLASS_Status[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
extern AUTOMODE			AutoMode; //20120808

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

extern LONG					g_lDiagnSteps;
extern ULONG					g_lCalAverageFreq;

#endif

#define PLACEMENT_TOL_X		(4.0)	// um
#define NUM_OF_OFFSET_DISPLAY		(10)
#define NUM_OF_SKIP_GLASS		3			//20161123 skip 3 glass for each MB

typedef enum
{
	INSPOPT_XY_CTRL_GO_STANDBY_POSN = 0,
	//INSPOPT_XY_CTRL_GO_PB1_REF_POSN,
	//INSPOPT_XY_CTRL_GO_PB2_REF_POSN,
	//INSPOPT_XY_CTRL_GO_PB1_CALIB_POSN,
	//INSPOPT_XY_CTRL_GO_PB2_CALIB_POSN,
	//INSPOPT_XY_CTRL_GO_PR1_POSN,
	//INSPOPT_XY_CTRL_GO_PR2_POSN,
	INSPOPT_XY_CTRL_GO_PREPR_POSN,
	//INSPOPT_XY_CTRL_GO_POSTPR1_POSN,
	//INSPOPT_XY_CTRL_GO_POSTPR2_POSN,

} INSPOPT_XY_CTRL_SIGNAL;

typedef enum
{
	INSPOPT_Z_CTRL_NOTHING = 0, 
	INSPOPT_Z_CTRL_GO_HOME, 
	INSPOPT_Z_CTRL_GO_STANDBY_LEVEL,
	INSPOPT_Z_CTRL_GO_CALIB_PR_LEVEL, 
	INSPOPT_Z_CTRL_GO_PR_LEVEL, 
	INSPOPT_Z_CTRL_GO_REF_LEVEL, 
} INSPOPT_Z_CTRL_SIGNAL;

// States
//
typedef enum
{
	INSPOPT_IDLE_Q = 0, 

	INSPOPT_WAIT_INSPWH_REQ_1_Q,
	INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q,
	INSPOPT_DO_GRAB_COF_PR1_INSP_NO_LATCH_Q,
	INSPOPT_DO_SRCH_COF_PR1_INSP_Q,
	INSPOPT_PR1_TO_NEXT,					//5

	INSPOPT_WAIT_INSPWH_REQ_2_Q,
	INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q,
	INSPOPT_DO_GRAB_COF_PR2_INSP_NO_LATCH_Q,
	INSPOPT_DO_SRCH_COF_PR2_INSP_Q,
	INSPOPT_PR2_TO_NEXT,					//10

	INSPOPT_GET_NEXT_GLASS_Q,
	INSPOPT_GO_FIRST_PR1_INSPECTION_Q,
	INSPOPT_GO_FIRST_PR2_INSPECTION_Q,
	INSPOPT_WAIT_INSPWH_ACK_Q,


	INSPOPT_STOP_Q = 99,

} INSPOPT_STATE;

typedef enum
{
	NO_MB_TYPE = -1,
	INSP_MB1 = 0,
	INSP_MB2,
	INSP_MB3,
	INSP_MB4,
	MAX_NUM_OF_MB_TYPE,

}	INSP_MB_TYPE;



class CInspOpt : public CPI9000Stn, public CPRTaskStn
{
	DECLARE_DYNCREATE(CInspOpt)
	friend class CPI9000App;
	friend class CSettingFile;
	friend class CWinEagle;
	friend class CInspWH;
	friend class CInPickArm;
	friend class CRejectArm;
	friend class CHouseKeeping;


protected:
	

protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();
	virtual VOID CycleOperation();

private:
	virtual CString		GetStation() 
	{
		return "InspOpt_";
	}

public:
	CInspOpt();
	virtual	~CInspOpt();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();
#if 1 //20120808 debug only
	BOOL LogXYMotor(CString &szLabel, LONG lWhichPB);
#endif
protected:
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------
	CMotorInfo	m_stMotorX;
	//CMotorInfo	m_stMotorY;
	CMotorInfo	m_stMotorZ;

	CSiInfo		m_stXLmtSnr;
	CSiInfo		m_stZLmtSnr;

	// Add NuMotion 
	// 
	// ------------------------------------------------------------------------
	// End of Motor SI/SO
	// ------------------------------------------------------------------------
	
	// cycle status
	LONG		m_lCurPRDelay;
	LONG		m_lCurInspWH;
	LONG		m_lCurGlass;
	LONG		m_lCurMarkType;	// 0--Glass; 1--COF
	INSPWH_INFO	m_stULGlass[MAX_NUM_OF_GLASS];
	INSPWH_INFO	m_stULCOF[MAX_NUM_OF_GLASS];

	//Posn
	MTR_POSN		m_mtrStandbyPosn;
	MTR_POSN		m_mtrPrePRPosn[MAX_NUM_OF_WH];
	MTR_POSN		m_mtrPRCalibPosn[MAX_NUM_OF_WH];
	MTR_POSN		m_mtrPR1Posn[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	MTR_POSN		m_mtrPR2Posn[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrPR1Offset[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrPR2Offset[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrGlassPR1Posn[MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrGlassPR2Posn[MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrCOFPR1Posn[MAX_NUM_OF_GLASS];
	D_MTR_POSN		m_dmtrCOFPR2Posn[MAX_NUM_OF_GLASS];

	DOUBLE			m_dPlacementOffsetX[MAX_NUM_OF_MAINBOND][MAX_NUM_OF_GLASS];
	DOUBLE			m_dPlacementOffsetY[MAX_NUM_OF_MAINBOND][MAX_NUM_OF_GLASS];
	DOUBLE			m_dPlacementAngle[MAX_NUM_OF_MAINBOND][MAX_NUM_OF_GLASS];

	DOUBLE			m_dPlacementTolX;	// um
	DOUBLE			m_dPlacementTolY;	// um
	DOUBLE			m_dPlacementTolAng;	// deg

	DOUBLE		m_dAlignAngle[MAX_NUM_OF_GLASS];			// store the current Glass Rotate Angle		
	MTR_POSN	m_mtrAlignCentre[MAX_NUM_OF_GLASS];			// store the current Glass Rotate Angle		
	MTR_POSN	m_mtrCurrAlignDelta[MAX_NUM_OF_GLASS];			// store the current Glass Delta x,y
	DOUBLE		m_dGlassRefAng[MAX_NUM_OF_GLASS];
	D_MTR_POSN	m_dmtrAlignRef[MAX_NUM_OF_GLASS];
	DOUBLE		m_dAlignRefAngle[MAX_NUM_OF_GLASS];

	LONG			m_lSkipGlass[MAX_NUM_OF_MAINBOND][MAX_NUM_OF_GLASS]; //20161123 number of glass skip for each MB
	
	//Level
	LONG			m_lULPRLevel[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	LONG			m_lStandbyLevel;
	LONG			m_lPRCalibLevel[MAX_NUM_OF_WH];
	LONG			m_lULRefLevel[MAX_NUM_OF_WH];

	// Measurement Routine
	DOUBLE		m_dMeasuredDistance;
	DOUBLE		m_dMeasuredAngle;

	DOUBLE		HMI_dUplookSpeedFactor; //20150213
	BOOL		m_bEnableMeasure;
	MTR_POSN	m_mtrMeasureSetZeroPt;
	D_MTR_POSN	m_dmtrCalibIncludedAngOffset; //20121024

	BOOL		m_bSearchPlacementCycle;
	BOOL		m_bMoveINPA;
	BOOL		m_bMoveWH;
	// Menu Option
	BOOL		m_bDoPR2First;
	BOOL		m_bGoPrePRPosn;

	INSP_MB_TYPE	m_lCurMBHead;
	BOOL		bPR1_InspDone;
	BOOL		bPR2_InspDone;
	LONG		m_lPR1_1_ErrorCnt;
	LONG		m_lPR1_2_ErrorCnt;
	LONG		m_lPR2_1_ErrorCnt;
	LONG		m_lPR2_2_ErrorCnt;
	LONG		m_lINSP_GLASS_OK_Cnt;

	std::deque <DOUBLE>	m_stdPlacementOffsetX[MAX_NUM_OF_MB_TYPE];
	std::deque <DOUBLE>	m_stdPlacementOffsetY[MAX_NUM_OF_MB_TYPE];
	std::deque <DOUBLE>	m_stdPlacementAngle[MAX_NUM_OF_MB_TYPE];

	DOUBLE		m_dOffsetX[MAX_NUM_OF_MB_TYPE];
	DOUBLE		m_dOffsetY[MAX_NUM_OF_MB_TYPE];
	DOUBLE		m_dOffsetAng[MAX_NUM_OF_MB_TYPE];

	std::deque <DOUBLE>	m_stdDisplayOffsetX[MAX_NUM_OF_MB_TYPE];
	std::deque <DOUBLE>	m_stdDisplayOffsetY[MAX_NUM_OF_MB_TYPE];
	std::deque <DOUBLE>	m_stdDisplayAngle[MAX_NUM_OF_MB_TYPE];
	std::deque <CString>	m_stszDisplayError[MAX_NUM_OF_MB_TYPE];

	DOUBLE		HMI_dOffsetX[MAX_NUM_OF_MB_TYPE][NUM_OF_OFFSET_DISPLAY];
	DOUBLE		HMI_dOffsetY[MAX_NUM_OF_MB_TYPE][NUM_OF_OFFSET_DISPLAY];
	DOUBLE		HMI_dOffsetAng[MAX_NUM_OF_MB_TYPE][NUM_OF_OFFSET_DISPLAY];

	BOOL		m_bFeedBackPlacement;
	LONG		m_lInspCycle;
	//PR
	VOID Cal_MB_Offset(GlassNum lCurGlass, INSP_MB_TYPE lCurMBHead = NO_MB_TYPE);
	BOOL IsPlacementTolError(GlassNum lCurGlass, INSP_MB_TYPE lCurMBHead);
	LONG HMI_ToggleFeedBackPlacement(IPC_CServiceMessage &svMsg);

	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	// Virtual PRTaskStn Function delcaration
	// ------------------------------------------------------------------------
	// All Virtual Function from PRTaskStn.cpp

	virtual CMotorInfo &GetMotorX(); 
	virtual CMotorInfo &GetMotorZ(); 
	virtual CMotorInfo &GetMotorY();
	virtual CMotorInfo &GetMotorT();

	virtual INT MoveXYToCurrPR1Posn(BOOL bWait = GMP_WAIT);
	virtual INT MoveXYToCurrPR2Posn(BOOL bWait = GMP_WAIT);
	virtual INT MoveXYToPRCalibPosn(BOOL bWait = GMP_WAIT);
	virtual INT MoveZToPRLevel(BOOL bWait = GMP_WAIT);
	virtual INT MoveZToPRCalibLevel(BOOL bWait = GMP_WAIT);
	virtual INT MoveTToStandby(BOOL bWait = GMP_WAIT);

	virtual INT SyncX();
	virtual INT SyncY();
	virtual INT SyncT();
	virtual INT SyncZ();
	virtual PRU &SelectCurPRU(ULONG ulPRU);
	//--------------------------------------------------------------
	CString GetSequenceString();
	VOID	OperationSequence();
	
	VOID SetDiagnSteps(ULONG ulSteps);

	INT MoveXYToCurPRPosn(LONG lPRU);

	INT MoveXY(INSPOPT_XY_CTRL_SIGNAL ctrl, BOOL bWait);
	INT MoveZ(INSPOPT_Z_CTRL_SIGNAL ctrl, BOOL bWait);

	INT MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait);
	INT MoveAbsoluteX(LONG lCntX, BOOL bWait);
	INT MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait);

	INT SetupGoStandbyPosn();

	VOID UpdateSetupStatus();
	VOID SelectPRU(ULONG ulPRU, BOOL bIsMove = TRUE); //20121221
	VOID SetupWizard();
	VOID SetupPRPatternPreTask(PRU *pPRU);
	VOID SetupPRPatternPostTask();

	VOID ModuleSetupPreTask();
	VOID ModuleSetupPostTask();
	VOID ModulePRSetupPreTask();
	VOID ModulePRSetupPostTask();

	VOID SetupAlignPt();
	VOID CalcAlignRefPt(LONG lGlass);
	BOOL SetGlassRef(LONG lGlass, BOOL bSetAng);
	VOID CalcAlignPt(GlassNum lGlass, PRMarkType lMarkType);

	BOOL SearchPlacementTest();


	// Module Setup Related
	LONG	HMI_lInspectionPt;

	BOOL	HMI_bXYControl;
	BOOL	HMI_bCursorControl;

	BOOL	HMI_bShowDigitalZoom;

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------
	//PR Setup Page
	LONG HMI_SetPRSelected(IPC_CServiceMessage &svMsg);
	LONG HMI_SelectGlassNum(IPC_CServiceMessage &svMsg);
	LONG HMI_SelectWHNum(IPC_CServiceMessage &svMsg);
	LONG HMI_SelectMarkType(IPC_CServiceMessage &svMsg);
	LONG HMI_SelectPRU(IPC_CServiceMessage &svMsg);
	LONG HMI_UpdateSetupStatus(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRPosn(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRLevel(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchAp1(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchAp2(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchCOFPlacement(IPC_CServiceMessage &svMsg);
	LONG HMI_StartSearchPlacementCycle(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPlacementTolX(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPlacementTolY(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPlacementTolAngle(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPR2PosnAsPR1Posn(IPC_CServiceMessage &svMsg);

//	BOOL SearchAlignCentre(INSPWH_PR_POSN gotoPosn);

	LONG HMI_SearchCentre(IPC_CServiceMessage &svMsg);
	LONG HMI_StartPRCycleTest(IPC_CServiceMessage &svMsg);

	LONG HMI_SearchDLnGoULPR1Posn(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchDLnGoULPR2Posn(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchAp1AfterDL(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchAp2AfterDL(IPC_CServiceMessage &svMsg);
	LONG HMI_SearchCentreAfterDL(IPC_CServiceMessage &svMsg);
	
	// Motor
	LONG HMI_PowerOnX(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnZ(IPC_CServiceMessage &svMsg);

	LONG HMI_CommX(IPC_CServiceMessage &svMsg);
	LONG HMI_CommZ(IPC_CServiceMessage &svMsg);

	LONG HMI_HomeX(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeZ(IPC_CServiceMessage &svMsg);

	// Setup Page Related
	LONG HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexXPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexXNeg(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZNeg(IPC_CServiceMessage &svMsg);

	LONG HMI_SetInspectionPt(IPC_CServiceMessage &svMsg);
	LONG HMI_MeasureSetZero(IPC_CServiceMessage &svMsg);
	LONG HMI_MeasureEnable(IPC_CServiceMessage &svMsg); //20121016
	LONG HMI_SetCalbIncludedAngXOffset(IPC_CServiceMessage &svMsg); //20121024
	LONG HMI_SetCalbIncludedAngYOffset(IPC_CServiceMessage &svMsg);
	LONG HMI_SetCustMoveProf(IPC_CServiceMessage &svMsg); //20121024

	LONG HMI_ToggleDoPR2First(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleGoPrePRPosn(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleMoveWH(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleMoveINPA(IPC_CServiceMessage &svMsg);

	VOID FeedBackOffsetToCOF(INSP_MB_TYPE lCurMBHead);
	VOID FeedBackOutOfTolToCOF(MainbondType lMBModule);

	VOID PrintPlacementDisplay(INSP_MB_TYPE lCurMBHead);

	// Saving / loading parameters
	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();

	virtual LONG IPC_SaveMachineRecord();
	virtual LONG IPC_LoadMachineRecord();
	virtual LONG IPC_SaveDeviceRecord();
	virtual LONG IPC_LoadDeviceRecord();

#ifdef EXCEL_MACHINE_DEVICE_INFO
	virtual BOOL printMachineDeviceInfo();
#endif


};
