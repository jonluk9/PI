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

#include "stdafx.h"
#include "MarkConstant.h"
#include "PI9000.h"
#include "PI9000_Constant.h"

#include "HouseKeeping.h"

#define TAManager_EXTERN
#include "TAManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CTAManager, CPI9000Stn)

CTAManager::CTAManager()
{
}

CTAManager::~CTAManager()
{
}

BOOL CTAManager::InitInstance()
{
	CPI9000Stn::InitInstance();

	return TRUE;
}

INT CTAManager::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////
VOID CTAManager::Operation()
{
	switch (State())
	{		
	case IDLE_Q:
		IdleOperation();
		break;

	case DIAGNOSTICS_Q:
		DiagOperation();
		break;

	case SYSTEM_INITIAL_Q:
		InitOperation();
		break;

	case PRESTART_Q:
		PreStartOperation();
		break;

	case CYCLE_Q:
		CycleOperation();
		break;

	case AUTO_Q:
		AutoOperation();
		break;

	case DEMO_Q:
		DemoOperation();
		break;

	case MANUAL_Q:
		ManualOperation();
		break;

	case ERROR_Q:
		ErrorOperation();
		break;

	case STOPPING_Q:
		StopOperation();
		break;

	case DE_INITIAL_Q:
		DeInitialOperation();
		break;

	default:
		State(IDLE_Q);
		Motion(FALSE);
		break;
	}

	UpdateOutput();

	Sleep(10);
}


/////////////////////////////////////////////////////////////////
//Update Functions
/////////////////////////////////////////////////////////////////

VOID CTAManager::UpdateOutput()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	INT i = 0;

	if ((m_qState == UN_INITIALIZE_Q) || (m_qState == DE_INITIAL_Q) || (m_qState == AUTO_Q) || (m_qState == DEMO_Q))
	{
		return;
	}

	if (pAppMod->m_bHWInitError)
	{
		return;
	}

	try
	{

	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	Sleep(UPDATEOUTPUT_SLEEP);
}

VOID CTAManager::RegisterVariables()
{
	try
	{
		// -------------------------------------------------------
		// PI9000Stn Variable and Function
		// -------------------------------------------------------
		// -------------------------------------------------------
		RegVariable(GetStation() + _T("bModError"), &m_bModError);

		RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogMotion"), HMI_SetLogMotion);

	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CTAManager::PreStartOperation()
{
	m_bModError = FALSE;
	
	// Delete me later
	m_bModSelected = TRUE;
#if 0 //klocwork fix 20121211
	if (!m_bModSelected)
	{	
		TA_MANAGER_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = TA_MANAGER_STOP_Q;
		m_bStopCycle = FALSE;
	}
	else
#endif
	{	
		m_lCurrCheck = MAINBOND_1;
		TA_MANAGER_Status = ST_IDLE;
		m_qSubOperation = TA_MANAGER_IDLE_Q;
		m_bStopCycle = FALSE;
	}

	Motion(FALSE);
	State(IDLE_Q);
	
	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), TA_MANAGER_Status, "Current Check", m_lCurrCheck);

	CPI9000Stn::PreStartOperation();
}

VOID CTAManager::AutoOperation()
{
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bStopCycle = TRUE;
	}

	OperationSequence();

	// cycle completed or motor error or abort requested: end operation immediately
	if (
		(m_bModSelected && m_qSubOperation == TA_MANAGER_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			// Add me later
		}

		if (GMP_SUCCESS != m_nLastError)
		{
			m_bModError = TRUE;
		}

		TA_MANAGER_Status = ST_STOP;
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

		CPI9000Stn::StopOperation();
	}
}


/////////////////////////////////////////////////////////////////
//Operational Sequences
/////////////////////////////////////////////////////////////////
CString CTAManager::GetSequenceString()
{
	switch (m_qSubOperation)
	{
	case TA_MANAGER_IDLE_Q:
		return "TA_MANAGER_IDLE_Q";

	case TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q:
		return "TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q";

	case TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_Q:
		return "TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_Q";

	case TA_MANAGER_WAIT_INPICKARM_ACK_Q:
		return "TA_MANAGER_WAIT_INPICKARM_ACK_Q";

	case TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q:
		return "TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q";

	case TA_MANAGER_CHECK_OUTPICKARM_SAFETY_TO_MOVE_Q:
		return "TA_MANAGER_CHECK_OUTPICKARM_SAFETY_TO_MOVE_Q";

	case TA_MANAGER_WAIT_OUTPICKARM_ACK_Q:
		return "TA_MANAGER_WAIT_OUTPICKARM_ACK_Q";

//#ifdef MB_DL_UL_PR	// 20140521 Yip
	//case TA_MANAGER_CHECK_INPICKARM_REQUEST_PR_Q:
	//	return "TA_MANAGER_CHECK_INPICKARM_REQUEST_PR_Q";

	//case TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_TO_PR_POSN_Q:
	//	return "TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_TO_PR_POSN_Q";

	//case TA_MANAGER_WAIT_INPICKARM_ACK_PR_Q:
	//	return "TA_MANAGER_WAIT_INPICKARM_ACK_PR_Q";
//#endif

	case TA_MANAGER_STOP_Q:
		return "TA_MANAGER_STOP_Q";
	}

	return "ASSASSINATE JASON";
}

VOID CTAManager::OperationSequence()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	
	PRINT_DATA	stPrintData;

	m_nLastError = GMP_SUCCESS;

	CString szMsg = "";

	switch (m_qSubOperation)
	{
	case TA_MANAGER_IDLE_Q:	// 0
			
		if(INSPWH_Status[m_lCurrCheck] != ST_MOD_NOT_SELECTED)
		{
			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q;		// go to 1
		}
		else
		{
			m_lCurrCheck = (MainbondType)((m_lCurrCheck + 1) % MAX_NUM_OF_MAINBOND);
		}

		break;

	case TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q: // 1
		if (m_bStopCycle)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;	// go to 7
		}
		else if (INSPWH_Status[m_lCurrCheck] == ST_STOP)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;		// go to 7		
		}
		else if (INSPWH_Status[m_lCurrCheck] == ST_READY_LOAD)		//WH Requests Load Glass
		{
			m_qSubOperation = TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_Q;		// go to 2
		}
		else
		{
//#ifdef MB_DL_UL_PR	// 20140521 Yip
//			m_qSubOperation = TA_MANAGER_CHECK_INPICKARM_REQUEST_PR_Q;
//#else
			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q;		// go to 4
//#endif
		}
		break;

	case TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_Q: // 2
		if (m_bStopCycle)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;	// go to 7
		}
		else if (/*m_lCurrCheck < OUTPICKARM_ARM_Posn-1 ||*/ OUTPICKARM_ARM_Posn == OUTPAStandby_POSN) //&& m_lCurrCheck != INPICKARM_ARM_Posn)
		{
			if (m_lCurrCheck == MAINBOND_1)
			{
				TA_MANAGER_Status = ST_MB1_REQ_LOAD;
			}
			else
			{
				TA_MANAGER_Status = ST_MB2_REQ_LOAD;
			}
			m_qSubOperation = TA_MANAGER_WAIT_INPICKARM_ACK_Q;		// go to 3
		}
		else // TA4 can't place glass, wait // go to main loop wait again
		{
			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q;		// go to 4			
		}
		break;

	case TA_MANAGER_WAIT_INPICKARM_ACK_Q: // 3
		if (INPICKARM_Status == ST_STOP || OUTPICKARM_Status == ST_STOP)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;		// go to 7			
		}
		else if (m_lCurrCheck == MAINBOND_1 && (INPICKARM_Status == ST_MB1_ACK_LOAD || INPICKARM_Status == ST_ACK))
		{
			TA_MANAGER_Status = ST_IDLE;
			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q;		// go to 4			
		}
		else if (m_lCurrCheck == MAINBOND_2 && (INPICKARM_Status == ST_MB2_ACK_LOAD || INPICKARM_Status == ST_ACK))
		{
			TA_MANAGER_Status = ST_IDLE;
			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q;		// go to 4			
		}
		break;

	case TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q: // 4
		if (m_bStopCycle)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;	// go to 7
		}
		else if (INSPWH_Status[m_lCurrCheck] == ST_STOP)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;		// go to 7			
		}
		else if (
				 (
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_READY1_EMPTY2 ) ||			//WH Unload Glass
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_READY1_ERR2 ) ||
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_ERR1_READY2 ) ||
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_ERR1_EMPTY2 ) ||
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_ERR1_ERR2 ) ||
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_EMPTY1_READY2 ) ||
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_EMPTY1_ERR2 ) ||
					 (INSPWH_Status[m_lCurrCheck] == ST_PICK_READY1_READY2 )
				 ) && 
				 OUTPICKARM_Status == ST_EMPTY
				)
		{
			m_qSubOperation = TA_MANAGER_CHECK_OUTPICKARM_SAFETY_TO_MOVE_Q;		// go to 5
		}
		else
		{
			m_lCurrCheck = (MainbondType)((m_lCurrCheck + 1) % MAX_NUM_OF_MAINBOND);	//Next Selected WH

			if(INSPWH_Status[m_lCurrCheck] != ST_MOD_NOT_SELECTED)
			{
				m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q;		// go to 1
			}
		}
		break;

	case TA_MANAGER_CHECK_OUTPICKARM_SAFETY_TO_MOVE_Q: // 5
		if (m_bStopCycle)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;	// go to 7
		}
		else if (/*m_lCurrCheck > INPICKARM_ARM_Posn+1 ||*/ INPICKARM_ARM_Posn == INPAStandby_POSN) //&& m_lCurrCheck != OUTPICKARM_ARM_Posn)
		{
			if (m_lCurrCheck == MAINBOND_1)
			{
				TA_MANAGER_Status = ST_MB1_REQ_UNLOAD;
			}
			else
			{
				TA_MANAGER_Status = ST_MB2_REQ_UNLOAD;
			}
			m_qSubOperation = TA_MANAGER_WAIT_OUTPICKARM_ACK_Q;		// go to 6
		}
		else
		{
			m_lCurrCheck = (MainbondType)((m_lCurrCheck + 1) % MAX_NUM_OF_MAINBOND);
			
			if(INSPWH_Status[m_lCurrCheck] != ST_MOD_NOT_SELECTED)
			{
				m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q;		// go to 1
			}
		}
		break;

	case TA_MANAGER_WAIT_OUTPICKARM_ACK_Q: // 6
		if (INPICKARM_Status == ST_STOP || OUTPICKARM_Status == ST_STOP)
		{
			m_qSubOperation = TA_MANAGER_STOP_Q;		// go to 7
		}
		else if (m_lCurrCheck == MAINBOND_1 && (OUTPICKARM_Status == ST_MB1_ACK_UNLOAD || INSPWH_Status[MAINBOND_1] == ST_MOD_NOT_SELECTED))
		{
			TA_MANAGER_Status = ST_IDLE;
			m_lCurrCheck = (MainbondType)((m_lCurrCheck + 1) % MAX_NUM_OF_MAINBOND);	//NExt WH
			if(INSPWH_Status[m_lCurrCheck] != ST_MOD_NOT_SELECTED)
			{
				m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q;		// go to 1			
			}
		}
		else if (m_lCurrCheck == MAINBOND_2 && (OUTPICKARM_Status == ST_MB2_ACK_UNLOAD || INSPWH_Status[MAINBOND_2] == ST_MOD_NOT_SELECTED))
		{
			TA_MANAGER_Status = ST_IDLE;            
			m_lCurrCheck = (MainbondType)((m_lCurrCheck + 1) % MAX_NUM_OF_MAINBOND);	//NExt WH
			if(INSPWH_Status[m_lCurrCheck] != ST_MOD_NOT_SELECTED)
			{
				m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_LOAD_Q;		// go to 1			
			}
		}
		break;

//#ifdef MB_DL_UL_PR	// 20140521 Yip
//	case TA_MANAGER_CHECK_INPICKARM_REQUEST_PR_Q:
//		if (m_bStopCycle)
//		{
//			m_qSubOperation = TA_MANAGER_STOP_Q;
//		}
//		else if (INPICKARM_Status == ST_REQUEST_PR1)
//		{
//			m_qSubOperation = TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_TO_PR_POSN_Q;
//		}
//		else
//		{
//			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q;
//		}
//		break;
//
//	case TA_MANAGER_CHECK_INPICKARM_SAFETY_TO_MOVE_TO_PR_POSN_Q:
//		if (m_bStopCycle)
//		{
//			m_qSubOperation = TA_MANAGER_STOP_Q;
//		}
//		else if (MAINBOND_1 < OUTPICKARM_ARM_Posn)
//		{
//			TA_MANAGER_Status = ST_ACK_TA4_PR;
//			m_qSubOperation = TA_MANAGER_WAIT_INPICKARM_ACK_PR_Q;
//		}
//		else
//		{
//			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q;		
//		}
//		break;
//
//	case TA_MANAGER_WAIT_INPICKARM_ACK_PR_Q:
//		if (INPICKARM_Status == ST_STOP || OUTPICKARM_Status == ST_STOP)
//		{
//			m_qSubOperation = TA_MANAGER_STOP_Q;		
//		}
//		else if (INPICKARM_Status != ST_REQUEST_PR1)
//		{
//			TA_MANAGER_Status = ST_IDLE;
//			m_qSubOperation = TA_MANAGER_CHECK_WH_REQUEST_UNLOAD_Q;		
//		}
//		break;
//#endif

	case TA_MANAGER_STOP_Q: // 7
		break;
	}

	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			if (TA_MANAGER_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("TA Manager Go State: %ld", m_qSubOperation);
			}
			else
			{
				szMsg.Format("TA Manager Go Stop");
			}
			//DisplayMessage(szMsg);
		}
		UpdateDebugSeq(GetSequenceString(), TA_MANAGER_Status, "Current Check", m_lCurrCheck);
		m_qPreviousSubOperation = m_qSubOperation;

		if (HMI_bDebugSeq)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				stPrintData.szTitle[i]	= "";
			}

			stPrintData.szTitle[0].Format("%ld", GetTickCount());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld, %ld, %ld", TA_MANAGER_Status, INPICKARM_ARM_Posn, OUTPICKARM_ARM_Posn);

			LogFile->log(stPrintData.szTitle);
		}
	}
}
