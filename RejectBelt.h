/////////////////////////////////////////////////////////////////
//	RejectBelt.cpp : interface of the CRejectBelt class
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

#include "PI9000Stn.h"

#ifdef REJBELT_EXTERN
extern BOOL					g_bStopCycle;
extern LONG					g_lDiagnSteps;
extern STATUS_TYPE			REJECTBELT_Status;
extern BOOL					g_bGlassOnRejBelt;
extern GLASS_INFO			g_stGlass;
#endif
#define REJBELT_DINSTANCE_STEP	(20.15)	// um
typedef enum
{
	REJBELT_IDLE_Q = 0, 

	REJBELT_CHECK_END_SNR_Q,
	REJBELT_DISPLAY_MESSAGE_Q,
	REJBELT_WAIT_END_SNR_BLOCKED_Q,
	REJBELT_WAIT_END_SNR_UNBLOCK_Q,

	REJBELT_STOP_Q = 99,

} REJBELT_STATE;


typedef enum
{
	REJ_BELT_CTRL_NOTHING = 0, 
	REJ_BELT_CTRL_MOVE_FORWARD, 
	REJ_BELT_CTRL_MOVE_HALF_FORWARD,
	REJ_BELT_CTRL_MOVE_NONSTOP,
	REJ_BELT_CTRL_MOVE_STOP,

} REJ_BELT_CTRL_SIGNAL;

typedef enum
{
	REJ_BELT_SRCH_SNR_ON,
	REJ_BELT_SRCH_SNR_OFF,

	// ALERT! must be less than MAX_NUM_OF_SRCH_PROF

} InConveyorSearchProf;

class CRejectBelt : public CPI9000Stn
{
	DECLARE_DYNCREATE(CRejectBelt)
	friend class CSettingFile;
	friend class CHouseKeeping;
	friend class CRejectArm;
	friend class COutPickArm;
	friend class CShBufferWH;

protected:
	CMotorInfo		m_stMotorBelt;

	CSiInfo			m_stRejBeltEndSnr;
	CSiInfo			m_stRejBeltInPosnSnr;
//Motion

	SRCH_PROF_PARA		m_stInConvBeltSrchForwardPara;
	SRCH_PROF_PARA		m_stInConvBeltSrchBackwardPara;


protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	VOID ModuleSetupPreTask();
	VOID ModuleSetupPostTask();

	VOID SetDiagnSteps(ULONG ulSteps);
private:
	virtual CString GetStation() 
	{
		return "RejBelt_";
	}

protected:
	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();
	CString GetSequenceString();
	VOID	OperationSequence();

	BOOL m_bRequestDoorOpen;

	BOOL IsRejBeltEndSnrOn();
	BOOL IsRejBeltAtInPosn();
	INT	MoveBelt(REJ_BELT_CTRL_SIGNAL ctrl, BOOL bWait);

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------
	LONG HMI_PowerOnBelt(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexBeltForward(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexBeltBackward(IPC_CServiceMessage &svMsg);

	LONG HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg);

public:
	CRejectBelt();
	virtual	~CRejectBelt();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();
};
