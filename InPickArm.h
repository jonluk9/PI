/////////////////////////////////////////////////////////////////
//	INPICKARM.cpp : interface of the CInPickArm class
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


#define TRANSFERARM_EXTERN
#include "TransferArm.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef INPICKARM_EXTERN
extern BOOL					g_bStopCycle;
	
extern BOOL					g_bFlushMode;

extern STATUS_TYPE			INPICKARM_Status;
extern STATUS_TYPE			IN_CONVEYOR_Status;
extern STATUS_TYPE			INSPWH_Status[MAX_NUM_OF_WH];
extern STATUS_TYPE			INSPWH_GLASS_Status[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
extern STATUS_TYPE			INPICKARM_GLASS_Status[MAX_NUM_OF_GLASS];
extern STATUS_TYPE			BUFFERWH_Status;
extern STATUS_TYPE			TA_MANAGER_Status;

extern GLASS_INFO			g_stGlass;
extern LONG					g_lGlassInputMode;

extern BOOL					GlassOnInBelt;
extern LONG					g_lGlassInputMode; //20120412

extern BOOL					g_bIsPerformCleanEdge;
extern LONG					g_lGatewayINPICKARMStatus; //20130919

extern PAPosnType			INPICKARM_ARM_Posn;

#endif

typedef enum
{
	INPICKARM_X_CTRL_NOTHING = 0, 
	INPICKARM_X_CTRL_GO_HOME, 
	INPICKARM_X_CTRL_GO_STANDBY_POSN, 
	INPICKARM_X_CTRL_GO_PICK_GLASS_POSN,
	INPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET,
	INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_POSN,
	INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_POSN, 
	INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_OFFSET, 
	INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_OFFSET, 
} INPICKARM_X_CTRL_SIGNAL;

typedef enum
{
	INPICKARM_Z_CTRL_NOTHING = 0, 
	INPICKARM_Z_CTRL_GO_HOME, 
	INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, 
	INPICKARM_Z_CTRL_GO_PICK_GLASS_LEVEL,
	INPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET,
	INPICKARM_Z_CTRL_GO_WH1_PLACE_GLASS_LEVEL,
	INPICKARM_Z_CTRL_GO_WH2_PLACE_GLASS_LEVEL, 
	INPICKARM_Z_CTRL_GO_WH1_PLACE_GLASS_OFFSET, 
	INPICKARM_Z_CTRL_GO_WH2_PLACE_GLASS_OFFSET, 
	INPICKARM_Z_CTRL_GO_PR_LEVEL, 
	INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL, 
	INPICKARM_Z_CTRL_GO_DL1_REF_LEVEL, 
	INPICKARM_Z_CTRL_GO_DL2_REF_LEVEL, 
} INPICKARM_Z_CTRL_SIGNAL;


// States
//
typedef enum
{
	INPICKARM_IDLE_Q = 0, 

	INPICKARM_WAIT_WH_UNLOAD_Q,
	INPICKARM_GO_TO_PICK_LEVEL_Q,
	INPICKARM_WAIT_WH_VAC_OFF_OK_Q,
	INPICKARM_CHECK_INPA_PANEL_EXIST_Q,
	INPICKARM_IS_OUTPA_PICKING_Q,		//5

	INPICKARM_WAIT_TA_MGR_REQ_Q,
	INPICKARM_PLACE_GLASS_Q,
	INPICKARM_CHECK_PLACE_GLASS_Q,
	INPICKARM_CHECK_G2_EXIST_N_READY_Q,
	INPICKARM_GRAB_N_SEARCH_G2_PR2_Q,		//10

	INPICKARM_GRAB_N_SEARCH_G1_PR2_Q,
	INPICKARM_GRAB_N_SEARCH_G2_PR1_Q,
	INPICKARM_GRAB_N_SEARCH_G1_PR1_Q,
	INPICKARM_PLACEMENT_PR_DONE_Q,
	INPICKARM_CHECK_VAC_ERROR_GLASS_ON_WH_Q,	//15

	INPICKARM_STOP_Q = 99,

} INPICKARM_STATE;

class CInPickArm : public CTransferArm, public CPRTaskStn
{
	DECLARE_DYNCREATE(CInPickArm)
	friend class CPI9000App;
	friend class CSettingFile;
	friend class CWinEagle;
	friend class CInspWH;
	friend class CWH1;
	friend class CWH2;
	friend class COutPickArm;
	friend class CInspOpt;
	friend class CHouseKeeping;

	//friend class CPreBondWH;
	//friend class CMainBondWH;
	//friend class CInspection;
	//friend class CInSHWH;
	//friend class CShBufferWH;
	//friend class CRejectArm;

protected:
	

protected:
	VOID UpdateOutput();

	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();
	virtual VOID CycleOperation();

	virtual BOOL GetGlass1Status() 
	{
		return m_bGlass1Exist;
	}


private:
	virtual CString		GetStation() 
	{
		return "InPickArm_";
	}

public:
	CInPickArm();
	virtual	~CInPickArm();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------

	CSiInfo		m_stXLmtSnr;

	CSiInfo		m_stUpStreamInLineSnr;
	CSiInfo		m_stUpStreamReadySnr;
	CSiInfo		m_stUpStreamStageMotionSnr;
	CSiInfo		m_stUpStreamStageInPosnSnr;
	CSiInfo		m_stUpStreamStageVacSnr;
	CSiInfo		m_stUpStreamReceivingSnr;
	CSiInfo		m_stUpStreamPanelTransferLSnr;
	CSiInfo		m_stUpStreamPanelTransferRSnr;
	CSiInfo		m_stUpStreamCancelSnr;
	CSiInfo		m_stUpStreamInterLockBackSnr;
	CSiInfo		m_stUpStreamEMOSnr;

	CSoInfo		m_stDownStreamInLineSol;
	CSoInfo		m_stDownStreamReadySol;
	CSoInfo		m_stDownStreamSendingSol;
	CSoInfo		m_stDownStreamTAMotionSol;
	CSoInfo		m_stDownStreamTAInPosnSol;
	CSoInfo		m_stDownStreamTAVacOnOkSol;
	CSoInfo		m_stDownStreamStageLVacOffSol;
	CSoInfo		m_stDownStreamStageRVacOffSol;
	CSoInfo		m_stDownStreamArmLVacErrorSol;
	CSoInfo		m_stDownStreamArmRVacErrorSol;
	CSoInfo		m_stDownStreamCancelSol;
	CSoInfo		m_stDownStreamInterLockFrontSol;

	// 
	// ------------------------------------------------------------------------
	// End of Motor SI/SO
	// ------------------------------------------------------------------------
	
	// Statistic
	LONG		m_lDLPR1_G1_ErrorCnt;
	LONG		m_lDLPR1_G2_ErrorCnt;
	LONG		m_lDLPR2_G1_ErrorCnt;
	LONG		m_lDLPR2_G2_ErrorCnt;
	
	// cycle status
	BOOL		m_bIsDefaultIndex;	// 1: Place Glass2 first 0: Place Glass1 first
	LONG		m_lCurrAssignPlace; // Try to pretend place to which Glass1 or Glass2
	LONG		m_lCurInspWH;
	//LONG		m_lCurGlass;
	
	//Posn
	LONG		m_lPickGlassOffset;
	LONG		m_lPlaceGlassPosn[MAX_NUM_OF_WH];
	LONG		m_lPlaceGlassOffset[MAX_NUM_OF_WH]; 

	//Level
	LONG		m_lPlaceLevel[MAX_NUM_OF_WH];
	LONG		m_lPlaceLevelOffset[MAX_NUM_OF_WH];
	LONG		m_lPRLevel[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
	LONG		m_lPRCalibLevel[MAX_NUM_OF_WH];
	LONG		m_lDL1RefLevel[MAX_NUM_OF_WH];
	LONG		m_lDL2RefLevel[MAX_NUM_OF_WH];

	BOOL		m_bLPanelTransfer;
	BOOL		m_bRPanelTransfer;

	BOOL		HMI_bStandAlone;
	BOOL		m_bInlineInBack;
	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	// Virtual Transfer Function delcaration
	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	// END Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	CString GetSequenceString();
	VOID OperationSequence();

	VOID UpdatePlaceToWHGlassStatus();
	BOOL CheckPlaceStatus();	
	BOOL CheckReplaceStatus();	
	BOOL ResetPlaceStatus();	
	BOOL SetPlaceStatus();

//	GlassNum AssignPickFromWhere();
//	GlassNum AssignPickFromWhere_ShBuf();
//	BOOL IsFlushStop();
//	GlassNum AssignPlaceToWhere();
//	GlassNum AssignPlaceToWhere_edge_cleaner();
//
//	INT PickGlassFromBuffer(GlassNum lGlassNum);
//	INT PickGlassFromInspWH(GlassNum lGlassNum);
//	INT PlaceGlassToBuffer(GlassNum lGlassNum);
//	INT PlaceGlassOnOutBelt();
//	BOOL IsPickGlassFromBuffer_OK(GlassNum lGlassNum);
//	BOOL IsPickGlassFromInspWH_OK(GlassNum lGlassNum);
//	BOOL IsPlaceGlassToBuffer_OK();
//	BOOL IsUnloadToFlipperWH();
//	BOOL CheckPlaceStatus();
//
//	///////////////////////////////////////
//	// HandShaking Signals
//	///////////////////////////////////////
//	VOID HS_INPICKARMSelectPanelEnable();
//	VOID HS_INPICKARMSendPanelReady();
//	BOOL HS_IsFlipperSendPanelReadyACK();	
//	BOOL HS_IsFlipperRequestArmVacOff();	
//	VOID HS_SetPanelDisabled();
//	BOOL HS_IsFlipperReceivedPanel();	
//	VOID HS_INPICKARMSendPanelNotReady();
//	BOOL HS_IsHandshakeCanel();
//	VOID HS_ClearAllHandShakeSignal();
//
//	INT SetHandShakeBit(CSoInfo *stTempSo, BOOL bMode);
//	BOOL IsHandShakeBitOn(CSiInfo stTempSi);
//
//	////////////////////////////////////////
//
//	INT PickGlass_DOWN();
//	INT PickGlass_UP();
	INT PlaceGlass(GlassNum lGlassNum, LONG lCurWH);
	INT PickGlass(GlassNum lGlassNum);
//	
	BOOL PickGlassOperation();
//	BOOL PickGlassFromInspWHOperation(GlassNum lGlassNum);
//	BOOL PickGlassFromShbufOperation(GlassNum lGlassNum);
	BOOL PlaceGlassOperation();
	BOOL PlaceGlassToInspWH(GlassNum lGlassNum, LONG lCurWH);
//	BOOL PlaceGlassToShbufOperation(GlassNum lGlassNum);
//	BOOL PlaceGlassToOutBeltOperation(GlassNum lGlassNum);
//
//	//Motor Related

	INT SetupGoStandbyPosn();
	INT SetupGoPickGlassPosn();
	INT SetupGoPickGlassOffset();
	INT SetupGoPlaceWH1GlassPosn();
	INT SetupGoPlaceWH2GlassPosn();
	INT SetupGoPlaceWH1GlassOffset(); //20121011
	INT SetupGoPlaceWH2GlassOffset(); //20121011

	INT SetupGoStandbyLevel();
	INT SetupGoPickLevel();
	INT SetupGoPickLevelOffset();
	INT SetupGoPlaceWH1Level();
	INT SetupGoPlaceWH1LevelOffset();
	INT SetupGoPlaceWH2Level();
	INT SetupGoPlaceWH2LevelOffset();

	INT MoveX(INPICKARM_X_CTRL_SIGNAL ctrl, BOOL bWait);
	INT MoveXToPlacePosn(LONG lCurWH, BOOL bWait);
	INT MoveZ(INPICKARM_Z_CTRL_SIGNAL ctrl, BOOL bWait);

	INT MoveZToStandbyLevel(BOOL bWait);
	INT MoveZToPRLevel(BOOL bWait);
	INT MoveXYToCurPR1Posn(LONG lCurWH, GlassNum CurGlass, BOOL bWait);
	INT MoveXYToCurPR2Posn(LONG lCurWH, GlassNum CurGlass, BOOL bWait);

//	// Module Setup Related
	VOID ModuleSetupPreTask();
	VOID ModuleSetupPostTask();
	
	BOOL ModuleSetupSetPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupGoPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupSetLevel(LONG lSetupTitle = 0);
	BOOL ModuleSetupGoLevel(LONG lSetupTitle = 0);
	BOOL ModuleSetupSetPara(PARA stPara);

	VOID UpdateModuleSetupPosn();
	VOID UpdateModuleSetupLevel();
	VOID UpdateModuleSetupPara();
//
//	INT	SetSendUpStreamInLine(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSendReady(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSendStageMotion(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSendStageInPosition(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSendStageVacOn(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSending(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSendG1Exist(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSendG2Exist(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetSendCancel(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetInterLockBack(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//	INT	SetPAEMO(BOOL bMode, BOOL bWait = GMP_NOWAIT);
//
//
//	INT MoveRelatedModuleToStandby();	// 20150327
//
	INT KeepQuiet();

	BOOL CycleTest();
//
//	virtual VOID PrintMachineSetup(FILE *fp);
//	virtual VOID PrintDeviceSetup(FILE *fp);
//#ifdef EXCEL_MACHINE_DEVICE_INFO
//	virtual BOOL printMachineDeviceInfo();
//#endif
//
//
//protected:
//	// ------------------------------------------------------------------------
//	// HMI Variable delcaration
//	// ------------------------------------------------------------------------
//
//	// ------------------------------------------------------------------------
//	// HMI Command delcaration
//	// ------------------------------------------------------------------------
//
	LONG HMI_PickGlassOperation(IPC_CServiceMessage &svMsg);
	LONG HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg);
	LONG HMI_SetStandAlone(IPC_CServiceMessage &svMsg);

//	
//	LONG HMI_SetPlaceGlassPrority(IPC_CServiceMessage &svMsg);
//	LONG HMI_SetStandAlone(IPC_CServiceMessage &svMsg);

	INT SetPlaceToWH1VacSol(LONG lGlassNum, BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetPlaceToWH2VacSol(LONG lGlassNum, BOOL bMode, BOOL bWait = GMP_NOWAIT);

	//UpStream SI
	BOOL IsUpStreamInLineSnrOn();
	BOOL IsUpStreamReadySnrOn();
	BOOL IsUpStreamStageMotionSnrOn();
	BOOL IsUpStreamStageInPosnSnrOn();
	BOOL IsUpStreamStageVACSnrOn();
	BOOL IsUpStreamReceivingSnrOn();
	BOOL IsUpStreamPanelTransferLeftSnrOn();
	BOOL IsUpStreamPanelTransferRightSnrOn();
	BOOL IsUpStreamCancelSnrOn();
	BOOL IsUpStreamInterlockBackSnrOn();
	BOOL IsUpStreamEMOSnrOn();

	//UpStream SO
	INT SetDownStreamInLineSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamReadySol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamSendingSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamTAMotionSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamTAInPosnSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamTAVacOnOkSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamStageLVacOffSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamStageRVacOffSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamArmLVacErrorSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamArmRVacErrorSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamCancelSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetDownStreamInterLockFrontSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	LONG HMI_ToggleDownStreamInLineSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamReadySol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamSendingSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamTAMotionSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamTAInPosnSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamTAVacOnOkSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamStageLVacOffSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamStageRVacOffSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamArmLVacErrorSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamArmRVacErrorSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamCancelSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleDownStreamInterLockFrontSol(IPC_CServiceMessage &svMsg);

	STATUS_TYPE IsUpStreamUnloadReady();
	INT GetPickAction();
	GlassNum GetUpStreamGlassStatus();

	BOOL HS_IsWHSendReady();
	VOID HS_InPickArmSendReadyACK();
	BOOL HS_IsHandshakeCancel();
	VOID HS_ClearAllHandShakeSignal();
	VOID HS_RequestWHVacOFF();
	BOOL HS_IsWHVacOff();
	VOID HS_SendInPickArmReceivedPanel();
	VOID UpdateInPickArmStatus();
	BOOL IsInPickArmGlassExist();
	VOID InPickArmVacOn();
	VOID InPickArmVacOff();
	BOOL IsTA5PlaceMBUnitReq();

	INT SetHandShakeBit(CSoInfo *stTempSo, BOOL bMode);
	BOOL IsHandShakeBitOn(CSiInfo stTempSi);

//	LONG HMI_ToggleInlineInBack(IPC_CServiceMessage &svMsg);

//	// Saving / loading parameters
	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();


};
