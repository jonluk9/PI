/////////////////////////////////////////////////////////////////
//	RejectArm.cpp : interface of the CRejectArm class
//
//	Description:
//		Motion Application
//
//	Date:		Tuesday, 8 October, 2013
//	Revision:	1.00
//
//	By:			Author Unknown
//				Department
//
//	Copyright @ ASM Technology Singapore Pte. Ltd., 2013.
//	ALL rights reserved.
//
/////////////////////////////////////////////////////////////////

#pragma once

#define TRANSFERARM_EXTERN
#include "TransferArm.h"

#include "PI9000Stn.h"
#include "Cal_Util.h"

#ifdef REJARM_EXTERN
extern BOOL					g_bStopCycle;

extern AUTOMODE				AutoMode;
extern BOOL					g_bDiagMode;
extern LONG					g_lDiagnSteps;
extern BOOL					g_bSelectedReworkMode;

	
extern STATUS_TYPE			REJECTARM_Status;
extern STATUS_TYPE			PB_GLASS_Status[MAX_NUM_OF_GLASS];
extern STATUS_TYPE			INSPWH_Status[MAX_NUM_OF_WH];
extern STATUS_TYPE			REJECTBELT_Status;
extern BOOL					g_bGlassOnRejBelt;

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




#endif


typedef enum
{
	REJARM_IDLE_Q = 0, 

	REJARM_WAIT_INSPWH_GLASS_REJECT_REQ_Q,
	REJARM_PICK_GLASS_ACTION_Q,
	REJARM_CHECK_GLASS_EXIST_AFTER_PICK_Q,
	REJARM_WAIT_REJBELT_READY_TO_PLACE_Q,
	REJARM_PICK_PLACE_GLASS_ACTION_Q,				//5

	REJARM_CHECK_GLASS_EXIST_AFTER_PLACE_Q,
	REJARM_CHECK_EXIST_ON_BELT_SNR_Q,

	REJARM_STOP_Q = 99,

} REJARM_STATE;

typedef enum
{
	REJARM_Y_CTRL_NOTHING = 0, 
	REJARM_Y_CTRL_GO_HOME, 
	REJARM_Y_CTRL_GO_STANDBY_POSN, 
	REJARM_Y_CTRL_GO_PICK_POSN,
	REJARM_Y_CTRL_GO_PICK_OFFSET,
	REJARM_Y_CTRL_GO_PLACE_POSN,
	REJARM_Y_CTRL_GO_PLACE_OFFSET,
	REJARM_Y_CTRL_GO_PREPLACE_POSN,

} REJARM_Y_CTRL_SIGNAL;

typedef enum
{
	REJARM_Z_CTRL_NOTHING = 0,
	REJARM_Z_CTRL_GO_HOME,
	REJARM_Z_CTRL_GO_STANDBY_LEVEL,
	REJARM_Z_CTRL_GO_PLACE_LEVEL,
	REJARM_Z_CTRL_GO_PLACE_LEVEL_OFFSET,

} REJARM_Z_CTRL_SIGNAL;




class CRejectArm : public CTransferArm//public CPI9000Stn
{

	DECLARE_DYNCREATE(CRejectArm)
	friend class CPI9000App;
	friend class CSettingFile;
	friend class CHouseKeeping;
	friend class CInSHWH;

protected:
	CMotorInfo		m_stMotorY;
	CMotorInfo		m_stMotorZ;
	
								
	//Snr
	CSiInfo		m_stRejArmYFrontLmtSnr;
	CSiInfo		m_stRejArmYBackLmtSnr;
	CSiInfo		m_stRejArmZHomeSnr;
	CSiInfo		m_stRejArmExtSnr;


	//Posn

	LONG			m_lPickGlassOffset;
	LONG			m_lPlaceGlassOffset;
	LONG			m_lPrePlaceGlassPosn;

	//Level
	LONG			m_lStandbyLevel;
	LONG			m_lPlaceLevel;
	LONG			m_lPlaceLevelOffset;

	BOOL			m_bGlassExist;
	GlassNum		m_nTARGET_WH;

private:
	virtual CString GetStation() 
	{
		return "RejArm_";
	}

protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	INT PlaceGlassOnRejBelt();

	// Add me for setup wizard later
	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------
	// Values are for HMI display only! Users should not use these in coding.

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------
	// Motor
	LONG HMI_PowerOnY(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnZ(IPC_CServiceMessage &svMsg);
	LONG HMI_CommY(IPC_CServiceMessage &svMsg);	
	LONG HMI_CommZ(IPC_CServiceMessage &svMsg);	
	LONG HMI_HomeY(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeZ(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleRejArmVacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleRejArmWBSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleRejArmCOFVacSol(IPC_CServiceMessage &svMsg);

	LONG HMI_RemoveGlassOperation(IPC_CServiceMessage &svMsg);

	LONG HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexYPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexYNeg(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZNeg(IPC_CServiceMessage &svMsg);

	LONG HMI_PickGlassOperation(IPC_CServiceMessage &svMsg);
	LONG HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg);
	LONG HMI_PickAndPlaceGlassOperation(IPC_CServiceMessage &svMsg);

	BOOL PickGlassOperation();
	BOOL PlaceGlassOperation();
	BOOL PickAndPlaceGlassOperation();

	INT PickGlass(GlassNum lGlassNum);
	INT PlaceGlass();

	INT MoveRelatedModuleToStandby();
	virtual VOID RegisterVariables();

	//Param Setup Related
	VOID SetDiagnSteps(ULONG ulSteps);
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

	BOOL IsExistSensorOn();

	INT MoveY(REJARM_Y_CTRL_SIGNAL ctrl, BOOL bWait);
	INT MoveZ(REJARM_Z_CTRL_SIGNAL ctrl, BOOL bWait);

	INT SetupGoStandbyPosn();
	INT SetupGoPickGlassPosn();
	INT SetupGoPickGlassOffset();
	INT SetupGoPlaceGlassPosn();
	INT SetupGoPlaceGlassOffset();
	INT SetupGoPrePlacePosn();

	INT SetupGoStandbyLevel();
	INT SetupGoPlaceLevel();
	INT SetupGoPlaceLevelOffset();

	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();


	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();
	CString GetSequenceString();
	//VOID	OperationSequence();

	virtual VOID PrintMachineSetup(FILE *fp);
	virtual VOID PrintDeviceSetup(FILE *fp);


public:
	CRejectArm();
	virtual	~CRejectArm();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();
};
