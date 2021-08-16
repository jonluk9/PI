/////////////////////////////////////////////////////////////////
//	TAManager.cpp : interface of the CTAManager class
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


#ifdef TAManager_EXTERN
extern BOOL					g_bStopCycle;

extern STATUS_TYPE			TA_MANAGER_Status;
extern STATUS_TYPE			INPICKARM_Status;
extern STATUS_TYPE			OUTPICKARM_Status;
extern STATUS_TYPE			INSPWH_Status[MAX_NUM_OF_WH];

//extern STATUS_TYPE			WH_Status[MAX_NUM_OF_MAINBOND][MAX_NUM_OF_GLASS];

extern PAPosnType			INPICKARM_ARM_Posn;
extern PAPosnType			OUTPICKARM_ARM_Posn;

#endif

// States
//
typedef enum
{
	TA_MANAGER_IDLE_Q = 0, 
	TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q,
	TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_Q,
	TA_MANAGER_WAIT_INPICKARM_ACK_Q,
	TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q,
	TA_MANAGER_CHECK_OUTPICKARM_SAFETY_TO_MOVE_Q,
	TA_MANAGER_WAIT_OUTPICKARM_ACK_Q,

//#ifdef MB_DL_UL_PR	// 20140521 Yip
	//TA_MANAGER_CHECK_INPICKARM_REQUEST_PR_Q,
	//TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_TO_PR_POSN_Q,
	//TA_MANAGER_WAIT_INPICKARM_ACK_PR_Q,
//#endif

	TA_MANAGER_STOP_Q,

} TA_MANAGER_STATE;


class CTAManager : public CPI9000Stn
{
	DECLARE_DYNCREATE(CTAManager)

protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();

private:
	virtual CString		GetStation() 
	{
		return "TAManager_";
	};

public:
	CTAManager();
	virtual	~CTAManager();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------
	MainbondType m_lCurrCheck;
	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------
	CString GetSequenceString();
	VOID OperationSequence();


	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------
	// Values are for HMI display only! Users should not use these in coding.	

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------

};
