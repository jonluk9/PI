/////////////////////////////////////////////////////////////////
//	ShBufferWH.cpp : interface of the CShBufferWH class
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

#define WORKHOLDER_EXTERN
#include "WorkHolder.h"
#include "PRTaskStn.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef SHBUFFER_WH_EXTERN
extern BOOL					g_bStopCycle;

extern AUTOMODE				AutoMode;
extern BOOL					g_bDiagMode;
extern LONG					g_lDiagnSteps;
extern BOOL					g_bSelectedReworkMode;

extern STATUS_TYPE			INSPOPT_Status;

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
extern STATUS_TYPE			BUFFERWH_Status;
extern STATUS_TYPE			OUTPICKARM_Status;

#endif

typedef enum
{
	BUFFERWH_IDLE_Q = 0, 

	BUFFERWH_WAIT_DOOR_CLOSE_Q,
	BUFFERWH_CHECK_BUFFER_GLASS_EXIST_Q,
	BUFFERWH_EMPTY_Q,
	BUFFERWH_WAIT_PICKARM_LOAD_ACK_Q,				
	BUFFERWH_WAIT_PA_PLACE_GLASS_COMPLETE_Q,			//5

	BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q,
	BUFFERWH_WAIT_DOOR_OPEN_Q,
	BUFFERWH_WAIT_DOOR_CLOSE_2_Q,
	BUFFERWH_GLASS_EXIST_Q,
	BUFFERWH_WAIT_PICKARM_UNLOAD_ACK_Q,					//10

	BUFFERWH_WAIT_PA_PICK_GLASS_COMPLETE_Q,			

	BUFFERWH_STOP_Q = 99,

} BUFFERWH_STATE;


typedef enum
{
	Y_CTRL_NOTHING = 0, 
	Y_CTRL_IN_POSN, 
	Y_CTRL_OUT_POSN,

} Y_CTRL_SIGNAL;


//typedef enum
//{
//	Z_CTRL_NOTHING = 0, 
//	Z_CTRL_LOAD_LEVEL, 
//	Z_CTRL_UNLOAD_LEVEL,
//
//} Z_CTRL_SIGNAL;

class CShBufferWH : public CWorkHolder
{
	DECLARE_DYNCREATE(CShBufferWH)
	friend class CPI9000App;
	friend class CSettingFile;
	friend class CHouseKeeping;
	friend class CPickArm;
	friend class CInPickArm;
	friend class COutPickArm;
	friend class CRejectBelt;

private:
	virtual CString GetStation() 
	{
		return "ShBufferWH_";
	}

protected:
	CSiInfo		m_stYInPosnSnr;
	CSiInfo		m_stYOutPosnSnr;
	CSiInfo		m_stSliderDetectSnr;

				
	CSoInfo		m_stYLoadSol;


	INT SetYInSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	LONG HMI_ToggleYLoadSol(IPC_CServiceMessage &svMsg);
	INT MoveY(Y_CTRL_SIGNAL ctrl, BOOL bWait);
	VOID LightOFF_InspReqButton();
	VOID LightOFF_EmptyWHReqButton();
	VOID LightFlash_InspReqButton();
	VOID LightFlash_EmptyWHReqButton();

	BOOL IsInspPanelReq();
	VOID ClearInspPanelReq();
	BOOL IsEmptyWHReq();
	BOOL IsRejBeltReqDoorOpen();
	VOID ClearEmptyWHReq();
	VOID LockDoor();
	VOID UnLockDoor();
	BOOL IsDoorOpen();
	BOOL IsDoorClose();


protected:
	VOID ModuleSetupPreTask();
	VOID ModuleSetupPostTask();

	BOOL ModuleSetupSetPara(PARA stPara);

	VOID UpdateModuleSetupPara();

	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();


	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();

	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();
	CString GetSequenceString();
	VOID	OperationSequence();


public:
	CShBufferWH();
	virtual	~CShBufferWH();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();




	VOID Operation();
};
