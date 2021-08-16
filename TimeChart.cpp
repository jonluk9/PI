/////////////////////////////////////////////////////////////////
//	TimeChart.cpp : interface of the CTimeChart class
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

#define TIMECHART_EXTERN
#include "TimeChart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const CString gszTCHART_DATA_PATH	= _T("D:\\sw\\PI9000\\DATA\\");

IMPLEMENT_DYNCREATE(CTimeChart, CPI9000Stn)

CTimeChart::CTimeChart()
{
	m_nStateDiff					= 4;
	m_nObjDiff						= 10;
	m_szFileName					= _T("001.tic");

	m_logfp							= NULL;
	m_dwStartTime					= 0;

	// register new time chart objects
	TC_RegisterObj();

	m_bLogging						= FALSE;

	// -------------------------------------------------------
	// HMI varibales
	// -------------------------------------------------------
	HMI_bSolSelected				= TRUE;
}

CTimeChart::~CTimeChart()
{
}

BOOL CTimeChart::InitInstance()
{
	CPI9000Stn::InitInstance();
	
	m_bModSelected = FALSE;

	return TRUE;
}

INT CTimeChart::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CTimeChart::Operation()
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

VOID CTimeChart::UpdateOutput()
{
	if (!m_fHardware)
	{
		return;
	}

	if ((m_qState == UN_INITIALIZE_Q) || (m_qState == DE_INITIAL_Q))
	{
		return;
	}

	try
	{
		//Update sensor here
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

VOID CTimeChart::RegisterVariables()
{
	try
	{
		// -------------------------------------------------------
		// PI9000Stn Variable and Function
		// -------------------------------------------------------
		RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		// -------------------------------------------------------
		// varibales
		// -------------------------------------------------------

		RegVariable(GetStation() + _T("InspWH.bSelected"), &stTimeObj[INSPWH_Y].bSelected);
		RegVariable(GetStation() + _T("InspWH_SOL.bSelected"), &stTimeObj[INSPWH_SOL].bSelected);


		RegVariable(GetStation() + _T("PickArm.bSelected"), &stTimeObj[PICKARM_X].bSelected);
		RegVariable(GetStation() + _T("PickArm_SOL.bSelected"), &stTimeObj[PICKARM_SOL].bSelected);

		RegVariable(GetStation() + _T("InspOpt.bSelected"), &stTimeObj[INSP_OPT_X].bSelected);

		RegVariable(GetStation() + _T("RejectArm.bSelected"), &stTimeObj[REJECTARM_Y].bSelected);
		RegVariable(GetStation() + _T("RejectArm_SOL.bSelected"), &stTimeObj[REJECTARM_SOL].bSelected);

		RegVariable(GetStation() + _T("RejectBelt.bSelected"), &stTimeObj[REJECT_BELT].bSelected);

		RegVariable(GetStation() + _T("ShBufferWH_SOL.bSelected"), &stTimeObj[SHBUFFER_WH_SOL].bSelected);

		RegVariable(GetStation() + _T("SOL.bSelected"), &HMI_bSolSelected);

		// -------------------------------------------------------
		// commands
		// -------------------------------------------------------
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetInspWHSelected"), HMI_SetInspWHSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPickArmSelected"), HMI_SetPickArmSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetInspOptSelected"), HMI_SetInspOptSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetRejectArmSelected"), HMI_SetRejectArmSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetRejectBeltSelected"), HMI_SetRejectBeltSelected);
		

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetSOLSelected"), HMI_SetSOLSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectAll"), HMI_SelectAll);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("AddCmd"), IPC_AddCmd);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("DelCmd"), IPC_DelCmd);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ResetTimer"), IPC_ResetTimer);
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

VOID CTimeChart::PreStartOperation()
{
	INT i = 0;
	INT nObjCounter = 0;

	if (!m_bModSelected)
	{
		m_bLogging = FALSE;
	}
	else
	{
		DisplayMessage("*******************************");
		DisplayMessage("TimeChart Module is enabled."); //20130513
		DisplayMessage("*******************************");
		
		// open file
		m_logfp = fopen(gszTCHART_DATA_PATH + m_szFileName, "a+");

		if (m_logfp != NULL)
		{
			__time64_t ltime;
			CString szTime = "";

			_time64(&ltime);
			szTime = _ctime64(&ltime);
			fprintf(m_logfp, "Start time: " + szTime);
			fprintf(m_logfp, "Time\t");

			// assign weights to objects
			for (i = 0; i < NUM_OF_TC_OBJECT; i++)
			{
				if (stTimeObj[i].bSelected)
				{
					nObjCounter++;
				}
			}

			for (i = 0; i < NUM_OF_TC_OBJECT; i++)
			{
				if (stTimeObj[i].bSelected)
				{
					nObjCounter--;
					stTimeObj[i].bStatus = FALSE;
					stTimeObj[i].nWeight = (nObjCounter) * m_nObjDiff;
					fprintf(m_logfp, "%s\t", (const char*)stTimeObj[i].stObjName);
				}
			}
			fprintf(m_logfp, "\n");

			m_dwStartTime = GetTickCount();
			m_bLogging = TRUE;
		}
		else
		{
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			DisplayMessage("ERROR: Unable to create Time Chart Log File");
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		}
	}

	Motion(FALSE);
	State(IDLE_Q);
}

VOID CTimeChart::AutoOperation()
{
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bLogging = FALSE;
	}

	if (!m_bLogging)
	{
		// close file
		if (m_logfp != NULL)
		{
			fprintf(m_logfp, "\nEnd of Time Chart Log File\n\n");
			fclose(m_logfp);
		}

		m_comClient.PurgeReplyQueue();

		State(STOPPING_Q);
	}
}


/////////////////////////////////////////////////////////////////
//Function delcaration
/////////////////////////////////////////////////////////////////

VOID CTimeChart::TC_RegisterObj()
{
	INT i = 0;

	// 20140523 Yip: TC entry re-order
	stTimeObj[REJECT_BELT].stObjName = "Reject Belt";
	stTimeObj[PICKARM_X].stObjName = "PickArm X";
	stTimeObj[PICKARM_SOL].stObjName = "PickArm Sol";
	stTimeObj[INSPWH_Y].stObjName = "InspWH Y";
	stTimeObj[INSPWH_Z1].stObjName = "InspWH Z1";
	stTimeObj[INSPWH_Z2].stObjName = "InspWH Z2";
	stTimeObj[INSPWH_X].stObjName = "InspWH X";
	stTimeObj[INSPWH_SOL].stObjName = "InspWH Sol";
	stTimeObj[INSP_OPT_X].stObjName = "InspOpt X";
	stTimeObj[INSP_OPT_Y].stObjName = "InspOpt Y";
	stTimeObj[REJECTARM_Y].stObjName = "RejectArm Y";
	stTimeObj[REJECTARM_Z].stObjName = "RejectArm Z";
	stTimeObj[REJECTARM_SOL].stObjName = "RejectArm Sol";
	stTimeObj[SHBUFFER_WH_SOL].stObjName = "ShBufferWH Sol";

	// 20140523 Yip: TC entry re-order
	stTimeObj[IN_WH_PR_GRAB].stObjName = "InspWH Grab";
	stTimeObj[IN_WH_PR_PROCESS].stObjName = "InspWH Process";

	for (i = 0; i < NUM_OF_TC_OBJECT; i++)
	{
		stTimeObj[i].bSelected	= TRUE;
		stTimeObj[i].nObjID		= i;

		stTimeObj[i].nWeight	= 0;
		stTimeObj[i].bStatus	= FALSE;
		stTimeObj[i].nJobCount	= 0;
	}

}

VOID CTimeChart::TC_AddCmd(INT emTCObjectID)
{
	if ((emTCObjectID < 0) || (emTCObjectID >= NUM_OF_TC_OBJECT))	// 20140717 Yip
	{
		CString szMsg;
		szMsg.Format("TimeChart AddCmd with Invalid ID %d", emTCObjectID);
		DisplayMessage(szMsg);
		return;
	}

	if (!m_bModSelected || !m_bLogging || !stTimeObj[emTCObjectID].bSelected)
	{
		return;
	}

	// print to file
	if (m_logfp == NULL)
	{
		return;
	}

	DWORD dwTime = GetTickCount() - m_dwStartTime;

	// log before
	fprintf(m_logfp, "%ld\t", dwTime);

	for (INT i = 0; i < NUM_OF_TC_OBJECT; i++)
	{
		if (stTimeObj[i].bSelected)
		{
			if (stTimeObj[i].bStatus)
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight + m_nStateDiff);
			}
			else
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight);
			}
		}
	}
	fprintf(m_logfp, "\n");

	stTimeObj[emTCObjectID].bStatus = TRUE;

	// log after
	fprintf(m_logfp, "%ld\t", dwTime);

	for (INT i = 0; i < NUM_OF_TC_OBJECT; i++)
	{
		if (stTimeObj[i].bSelected)
		{
			if (stTimeObj[i].bStatus)
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight + m_nStateDiff);
			}
			else
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight);
			}
		}
	}
	fprintf(m_logfp, "\n");
}

VOID CTimeChart::TC_DelCmd(INT emTCObjectID)
{
	if ((emTCObjectID < 0) || (emTCObjectID >= NUM_OF_TC_OBJECT))	// 20140717 Yip
	{
		CString szMsg;
		szMsg.Format("TimeChart DelCmd with Invalid ID %d", emTCObjectID);
		DisplayMessage(szMsg);
		return;
	}

	if (!m_bModSelected || !m_bLogging || !stTimeObj[emTCObjectID].bSelected)
	{
		return;
	}

	// print to file
	if (m_logfp == NULL)
	{
		return;
	}

	DWORD dwTime = GetTickCount() - m_dwStartTime;

	// log before
	fprintf(m_logfp, "%ld\t", dwTime);

	for (INT i = 0; i < NUM_OF_TC_OBJECT; i++)
	{
		if (stTimeObj[i].bSelected)
		{
			if (stTimeObj[i].bStatus)
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight + m_nStateDiff);
			}
			else
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight);
			}
		}
	}
	fprintf(m_logfp, "\n");

	stTimeObj[emTCObjectID].bStatus = FALSE;

	// log after
	fprintf(m_logfp, "%ld\t", dwTime);

	for (INT i = 0; i < NUM_OF_TC_OBJECT; i++)
	{
		if (stTimeObj[i].bSelected)
		{
			if (stTimeObj[i].bStatus)
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight + m_nStateDiff);
			}
			else
			{
				fprintf(m_logfp, "%d\t", stTimeObj[i].nWeight);
			}
		}
	}
	fprintf(m_logfp, "\n");
}

VOID CTimeChart::TC_ResetTimer()
{
	if (!m_bModSelected || !m_bLogging)
	{
		return;
	}

	// print to file
	if (m_logfp == NULL)
	{
		return;
	}

	fprintf(m_logfp, "Unit Bonded. Cycle Time: %ld\n\n", GetTickCount() - m_dwStartTime);

	fprintf(m_logfp, "Time\t");

	for (INT i = 0; i < NUM_OF_TC_OBJECT; i++)
	{
		if (stTimeObj[i].bSelected)
		{
			fprintf(m_logfp, "%s\t", (const char*)stTimeObj[i].stObjName);
		}
	}
	fprintf(m_logfp, "\n");

	m_dwStartTime = GetTickCount();
}


//-----------------------------------------------------------------------------
// HMI command implementation
//-----------------------------------------------------------------------------
//

LONG CTimeChart::HMI_SetModSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		m_bModSelected = TRUE;
	}
	else
	{
		m_bModSelected = FALSE;
	}

	return 0;
}

LONG CTimeChart::HMI_SetInspWHSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		stTimeObj[INSPWH_Y].bSelected = TRUE;
		stTimeObj[INSPWH_Z1].bSelected = TRUE;
		stTimeObj[INSPWH_Z2].bSelected = TRUE;
		stTimeObj[INSPWH_X].bSelected = TRUE;
		if (HMI_bSolSelected)
		{
			stTimeObj[INSPWH_SOL].bSelected = TRUE;
		}
	}
	else
	{
		stTimeObj[INSPWH_Y].bSelected = FALSE;
		stTimeObj[INSPWH_Z1].bSelected = FALSE;
		stTimeObj[INSPWH_Z2].bSelected = FALSE;
		stTimeObj[INSPWH_X].bSelected = FALSE;
		if (HMI_bSolSelected)
		{
			stTimeObj[INSPWH_SOL].bSelected = FALSE;
		}
	}
	return 0;
}

LONG CTimeChart::HMI_SetPickArmSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		stTimeObj[PICKARM_X].bSelected = TRUE;
		if (HMI_bSolSelected)
		{
			stTimeObj[PICKARM_SOL].bSelected = TRUE;
		}
	}
	else
	{
		stTimeObj[PICKARM_X].bSelected = FALSE;
		if (HMI_bSolSelected)
		{
			stTimeObj[PICKARM_SOL].bSelected = FALSE;
		}
	}
	return 0;
}

LONG CTimeChart::HMI_SetInspOptSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		stTimeObj[INSP_OPT_X].bSelected = TRUE;
		stTimeObj[INSP_OPT_Y].bSelected = TRUE;
	}
	else
	{
		stTimeObj[INSP_OPT_X].bSelected = FALSE;
		stTimeObj[INSP_OPT_Y].bSelected = FALSE;
	}
	return 0;
}

LONG CTimeChart::HMI_SetRejectBeltSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		stTimeObj[REJECT_BELT].bSelected = TRUE;
	}
	else
	{
		stTimeObj[REJECT_BELT].bSelected = FALSE;
	}
	return 0;
}

LONG CTimeChart::HMI_SetRejectArmSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		stTimeObj[REJECTARM_Y].bSelected = TRUE;
		stTimeObj[REJECTARM_Z].bSelected = TRUE;
		if (HMI_bSolSelected)
		{
			stTimeObj[REJECTARM_SOL].bSelected = TRUE;
		}
	}
	else
	{
		stTimeObj[REJECTARM_Y].bSelected = FALSE;
		stTimeObj[REJECTARM_Z].bSelected = FALSE;
		if (HMI_bSolSelected)
		{
			stTimeObj[REJECTARM_SOL].bSelected = FALSE;
		}
	}
	return 0;
}

LONG CTimeChart::HMI_SetSOLSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		HMI_bSolSelected = TRUE;

		stTimeObj[SHBUFFER_WH_SOL].bSelected = TRUE;
		// INSPWH
		if (stTimeObj[INSPWH_Y].bSelected)
		{
			stTimeObj[INSPWH_SOL].bSelected = TRUE;
		}
		else
		{
			stTimeObj[INSPWH_SOL].bSelected = FALSE;
		}

		// PICKARM
		if (stTimeObj[PICKARM_X].bSelected)
		{
			stTimeObj[PICKARM_SOL].bSelected = TRUE;
		}
		else
		{
			stTimeObj[PICKARM_SOL].bSelected = FALSE;
		}

		if (stTimeObj[REJECTARM_Y].bSelected)
		{
			stTimeObj[REJECTARM_SOL].bSelected = TRUE;
		}
		else
		{
			stTimeObj[REJECTARM_SOL].bSelected = FALSE;
		}

	}
	else
	{
		HMI_bSolSelected = FALSE;

		stTimeObj[INSPWH_SOL].bSelected = FALSE;
		stTimeObj[PICKARM_SOL].bSelected = FALSE;
		stTimeObj[REJECTARM_SOL].bSelected = FALSE;
		stTimeObj[SHBUFFER_WH_SOL].bSelected = FALSE;
	}

	return 0;
}

LONG CTimeChart::HMI_SelectAll(IPC_CServiceMessage &svMsg)
{
	INT i;
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction("%s: %s", __FUNCTION__, bMode ? "ON" : "OFF"); //20130510

	if (bMode)
	{
		for (i = 0; i < NUM_OF_TC_OBJECT; i++)
		{
			stTimeObj[i].bSelected = TRUE;
		}
	}
	else
	{
		for (i = 0; i < NUM_OF_TC_OBJECT; i++)
		{
			stTimeObj[i].bSelected = FALSE;
		}
	}

	return 0;
}

LONG CTimeChart::IPC_AddCmd(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;

	INT nObjectID;
	svMsg.GetMsg(sizeof(INT), &nObjectID);

	TC_AddCmd(nObjectID);

	return 0;
}

LONG CTimeChart::IPC_DelCmd(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;

	INT nObjectID;
	svMsg.GetMsg(sizeof(INT), &nObjectID);

	TC_DelCmd(nObjectID);

	return 0;
}

LONG CTimeChart::IPC_ResetTimer(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;

	TC_ResetTimer();

	return 0;
}
