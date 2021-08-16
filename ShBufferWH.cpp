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

#include "stdafx.h"
#include "MarkConstant.h"
#include "PI9000.h"
#include "PI9000_Constant.h"
#include "Cal_Util.h"
#define SHBUFFER_WH_EXTERN
#include "ShBufferWH.h"
#include "PickArm.h"
#include "RejectBelt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CShBufferWH, CWorkHolder)

CShBufferWH::CShBufferWH()
{

	m_lStnErrorCode		= IDS_SHBUFFER_WH_NOT_SELECTED_ERR;
	// Si Related
	m_stYInPosnSnr		= CSiInfo("ShBufferWHYInPosnSnr",		"IOMODULE_IN_PORT_6", BIT_0, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stYInPosnSnr);

	m_stYOutPosnSnr		= CSiInfo("ShBufferWHYOutPosnSnr",		"IOMODULE_IN_PORT_6", BIT_1, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stYOutPosnSnr);

	m_stGlass1VacSnr		= CSiInfo("ShBufferWH1VacSnr",		"IOMODULE_IN_PORT_6", BIT_2, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass1VacSnr); 

	m_stGlass2VacSnr		= CSiInfo("ShBufferWH2VacSnr",		"IOMODULE_IN_PORT_6", BIT_3, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass2VacSnr);

	m_stSliderDetectSnr		= CSiInfo("ShBufferWHSliderDetectSnr",		"IOMODULE_IN_PORT_6", BIT_4, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stSliderDetectSnr);


	// So Related
	m_stGlass1VacSol		= CSoInfo("ShBufferWH1VacSol",		"IOMODULE_OUT_PORT_5", BIT_0, SHBUFFER_WH_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass1VacSol);	

	m_stGlass2VacSol		= CSoInfo("ShBufferWH2VacSol",		"IOMODULE_OUT_PORT_5", BIT_1, SHBUFFER_WH_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass2VacSol);


	m_stWeakBlowSol		= CSoInfo("ShBufferWH1WBSol",		"IOMODULE_OUT_PORT_5", BIT_2, SHBUFFER_WH_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stWeakBlowSol);	

	//m_stWeakBlow2Sol	= CSoInfo("ShBufferWH2WBSol",		"IOMODULE_OUT_PORT_5", BIT_3, SHBUFFER_WH_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stWeakBlow2Sol);	

	m_stYLoadSol		= CSoInfo("ShBufferWHYLoadSol",		"IOMODULE_OUT_PORT_5", BIT_3, SHBUFFER_WH_SOL, ACTIVE_LOW, TRUE);
	m_mySoList.Add((CSoInfo*)&m_stYLoadSol);


}

CShBufferWH::~CShBufferWH()
{
}

BOOL CShBufferWH::InitInstance()
{
	CWorkHolder::InitInstance();

	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CGmpSystem *pGmpSys = pAppMod->m_pCGmpSystem;
	CString		szMsg	= _T("");

	INT i = 0;

	try
	{
		// Acutators
		//if (pAppMod->IsInitNuDrive())
		//{
		for (i = 0; i < m_myMotorList.GetSize(); i++)
		{
				// Initi Motor Configuration
			GetmyMotorList().GetAt(i)->SetAppStation(pAppMod);
			GetmyMotorList().GetAt(i)->SetStnName(GetStnName());
			if (pAppMod->IsInitNuDrive())
			{
				GetmyMotorList().GetAt(i)->SetGmpChannel(pGmpSys->AddChannel(m_myMotorList.GetAt(i)->stAttrib.szAxis, m_myMotorList.GetAt(i)->stAttrib.dDistPerCount));
				GetmyMotorList().GetAt(i)->InitMtrAxis();
			}
		}
		//}

		// IOs
		if (pAppMod->IsInitNuDrive())
		{
			// SI
			for (i = 0; i < m_mySiList.GetSize(); i++)
			{
				GetmySiList().GetAt(i)->SetGmpPort(
					pGmpSys->AddSiPort(m_mySiList.GetAt(i)->GetPortName(), 
									   m_mySiList.GetAt(i)->GetMask(), 
									   m_mySiList.GetAt(i)->GetActiveState()));
			}

			// SO
			for (i = 0; i < m_mySoList.GetSize(); i++)
			{
				GetmySoList().GetAt(i)->SetGmpPort(
					pGmpSys->AddSoPort(m_mySoList.GetAt(i)->GetPortName(), 
									   m_mySoList.GetAt(i)->GetMask(), 
									   m_mySoList.GetAt(i)->GetActiveState()));

				GetmySoList().GetAt(i)->bHMI_Status = GetmySoList().GetAt(i)->GetGmpPort().IsOn();

			}
		}
	}
	catch (CAsmException e)
	{
		pAppMod->m_bHWInitError = TRUE;

		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	while (pAppMod->m_bHWInitCheckingPressure) //20130711
	{
		Sleep(100);
	}

	return TRUE;
}

INT CShBufferWH::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWorkHolder::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CShBufferWH::Operation()
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

	if (m_bUpdateProfile)
	{
		UpdateProfile();
	}

	if (m_bUpdatePosition)
	{
		UpdatePosition();
	}

	Sleep(10);
}


/////////////////////////////////////////////////////////////////
//Update Functions
/////////////////////////////////////////////////////////////////

VOID CShBufferWH::UpdateOutput()
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
		if (m_fHardware && m_bCheckMotor)
		{
			for (i = 0; i < m_myMotorList.GetSize(); i++)
			{
				GetmyMotorList().GetAt(i)->bHMI_On			= GetmyMotorList().GetAt(i)->IsPowerOn();
				GetmyMotorList().GetAt(i)->lHMI_CurrPosn	= GetmyMotorList().GetAt(i)->GetEncPosn();
			}

			if (m_bModSelected)
			{
				CheckModSelected(TRUE);
			}

		}

		// I/Os
		if (pAppMod->IsInitNuDrive() && m_bCheckIO)
		{
			for (i = 0; i < m_mySiList.GetSize(); i++)
			{
				GetmySiList().GetAt(i)->bHMI_Status			= GetmySiList().GetAt(i)->GetGmpPort().IsOn();
			}
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	Sleep(UPDATEOUTPUT_SLEEP);

}

VOID CShBufferWH::UpdateProfile()
{
	//Add Your Code Here
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CSingleLock slLock(&pAppMod->m_csMachine);

	m_bUpdateProfile = FALSE;

	try
	{
	}
	catch (CAsmException e)
	{
		DisplayException(e);
	}

	slLock.Unlock();
}

VOID CShBufferWH::UpdatePosition()
{
	//Add Your Code Here
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CSingleLock slLock(&pAppMod->m_csDevice);
	
	m_bUpdatePosition = FALSE;

	try
	{
	}
	catch (CAsmException e)
	{
		DisplayException(e);
	}

	slLock.Unlock();
}


/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CShBufferWH::PreStartOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	m_bModError = FALSE;
	m_bGlass1Exist = FALSE;
	m_bGlass2Exist = FALSE;

	if (!m_bModSelected)
	{	
		BUFFERWH_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = BUFFERWH_STOP_Q;
		m_bStopCycle = FALSE;
	}
	else
	{

		if (
			//((m_nLastError = MoveZ(Z_CTRL_UNLOAD_LEVEL, GMP_WAIT)) != GMP_SUCCESS) || 
			((m_nLastError = MoveY(Y_CTRL_OUT_POSN, GMP_WAIT)) != GMP_SUCCESS)
			)
		{
			Command(glSTOP_COMMAND, TRUE);
			BUFFERWH_Status = ST_STOP;
			m_qSubOperation = BUFFERWH_STOP_Q;
			m_bStopCycle = TRUE;
		}
		else
		{
			LightOFF_InspReqButton();
			LightOFF_EmptyWHReqButton();
			ClearInspPanelReq();
			ClearEmptyWHReq();
			LockDoor();

			BUFFERWH_Status = ST_IDLE;
			m_qSubOperation = BUFFERWH_IDLE_Q;
			m_bStopCycle = FALSE;
		}
	}

	Motion(FALSE);
	State(IDLE_Q);

	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), BUFFERWH_Status, m_bGlass1Exist, m_bGlass2Exist);

	CPI9000Stn::PreStartOperation();
}

VOID CShBufferWH::AutoOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bStopCycle = TRUE;
	}

	OperationSequence();

	// cycle completed or motor error or abort requested: end operation immediately
	if (
		(m_bModSelected && m_qSubOperation == BUFFERWH_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			LightOFF_InspReqButton();
			LightOFF_EmptyWHReqButton();
			ClearInspPanelReq();
			ClearEmptyWHReq();
			//MoveZ(Z_CTRL_UNLOAD_LEVEL, GMP_WAIT);
			if (BUFFERWH_Status != ST_VAC_ERROR)
			{
				MoveY(Y_CTRL_OUT_POSN, GMP_WAIT);
			}
		}
	
		if (GMP_SUCCESS != m_nLastError)
		{
			m_bModError = TRUE;
		}

		
		BUFFERWH_Status = ST_STOP;

		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

		CPI9000Stn::StopOperation();
	}
}

/////////////////////////////////////////////////////////////////
//Operational Sequences
/////////////////////////////////////////////////////////////////
CString CShBufferWH::GetSequenceString()
{

	switch (m_qSubOperation)
	{
	case BUFFERWH_IDLE_Q:
		return "BUFFERWH_IDLE_Q";
		
	case BUFFERWH_WAIT_DOOR_CLOSE_Q:
		return "BUFFERWH_WAIT_DOOR_CLOSE_Q";

	case BUFFERWH_CHECK_BUFFER_GLASS_EXIST_Q:
		return "BUFFERWH_CHECK_BUFFER_GLASS_EXIST_Q";

	case BUFFERWH_EMPTY_Q:
		return "BUFFERWH_EMPTY_Q";

	case BUFFERWH_WAIT_PICKARM_LOAD_ACK_Q:
		return "BUFFERWH_WAIT_PICKARM_LOAD_ACK_Q";

	case BUFFERWH_WAIT_PA_PLACE_GLASS_COMPLETE_Q:
		return "BUFFERWH_WAIT_PA_PLACE_GLASS_COMPLETE_Q";

	case BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q:
		return "BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q";

	case BUFFERWH_WAIT_DOOR_OPEN_Q:
		return "BUFFERWH_WAIT_DOOR_OPEN_Q";

	case BUFFERWH_WAIT_DOOR_CLOSE_2_Q:
		return "BUFFERWH_WAIT_DOOR_CLOSE_2_Q";

	case BUFFERWH_GLASS_EXIST_Q:
		return "BUFFERWH_GLASS_EXIST_Q";

	case BUFFERWH_WAIT_PICKARM_UNLOAD_ACK_Q:
		return "BUFFERWH_WAIT_PICKARM_UNLOAD_ACK_Q";

	case BUFFERWH_WAIT_PA_PICK_GLASS_COMPLETE_Q:
		return "BUFFERWH_WAIT_PA_PICK_GLASS_COMPLETE_Q";

	case BUFFERWH_STOP_Q:
		return "BUFFERWH_STOP_Q";

		
	}

	return "ASSASSINATE JASON";
}

VOID CShBufferWH::OperationSequence()
{
	static clock_t clkAlartStart;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	static BOOL bSetAlert = TRUE;

	m_nLastError = GMP_SUCCESS;
	PRINT_DATA	stPrintData;


	CString szMsg = "";


	switch (m_qSubOperation)
	{
		case BUFFERWH_IDLE_Q:	// 0

			if (m_bStopCycle)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (IsDoorOpen())
			{
				UnLockDoor();
				bSetAlert = TRUE;
				LightFlash_EmptyWHReqButton();
				m_qSubOperation = BUFFERWH_WAIT_DOOR_CLOSE_Q;  // go to 1
			}
			else
			{
				BUFFERWH_Status = ST_IDLE;

				if (
					((m_nLastError = SetVacSol(MAX_NUM_OF_GLASS, ON, GMP_WAIT)) == GMP_SUCCESS) &&
					//((m_nLastError = MoveZ(Z_CTRL_UNLOAD_LEVEL, GMP_WAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveY(Y_CTRL_OUT_POSN, GMP_WAIT)) == GMP_SUCCESS)
				)
				{
					m_qSubOperation = BUFFERWH_CHECK_BUFFER_GLASS_EXIST_Q;  // go to 2
				}
				else
				{
					m_qSubOperation = BUFFERWH_STOP_Q;  // go to 99
				}
			}

		break;

		case BUFFERWH_WAIT_DOOR_CLOSE_Q:	// 1
			
			if (m_bStopCycle)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (IsDoorClose() && m_stSliderDetectSnr.GetGmpPort().IsOn())
			{
				Sleep(500);
				LockDoor();

				CloseAlarm();
				pCHouseKeeping->m_bAlertExist = FALSE;
				LightOFF_InspReqButton();
				LightOFF_EmptyWHReqButton();
				ClearInspPanelReq();
				ClearEmptyWHReq();
				
				m_qSubOperation = BUFFERWH_IDLE_Q;  // go to 0

			}
			else
			{
				if (bSetAlert)
				{
					clkAlartStart = clock();
					bSetAlert = FALSE;
					SetAlert(IDS_HK_SHBUF_DOOR_NOT_DETECTED_ERR);
				}
				if (pCHouseKeeping->m_bBeepSound == TRUE && ((DOUBLE)(clock() - clkAlartStart) / CLOCKS_PER_SEC) > 5.0)
				{
					pCHouseKeeping->m_bBeepSound = FALSE; //turn off sound
				}
			}

		break;

		case BUFFERWH_CHECK_BUFFER_GLASS_EXIST_Q:	// 2
			
			if (IsVacSensorOn(GLASS1) || IsVacSensorOn(GLASS2)) // some glass exist
			{
				if (!IsVacSensorOn(GLASS1))
				{
					m_bGlass1Exist = FALSE;
					SetVacSol(GLASS1, OFF, GMP_NOWAIT);
				}
				else
				{
					m_bGlass1Exist = TRUE;
				}

				if (!IsVacSensorOn(GLASS2))
				{
					m_bGlass2Exist = FALSE;
					SetVacSol(GLASS2, OFF, GMP_NOWAIT);
				}
				else
				{
					m_bGlass2Exist = TRUE;
				}
				if (
					//((m_nLastError = MoveZ(Z_CTRL_UNLOAD_LEVEL, GMP_WAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveY(Y_CTRL_IN_POSN, GMP_WAIT)) == GMP_SUCCESS)		//??? Posn
				)
				{
					BUFFERWH_Status = ST_BUSY;
					m_qSubOperation = BUFFERWH_GLASS_EXIST_Q;  // go to 9
				}
				else
				{
					m_qSubOperation = BUFFERWH_STOP_Q;  // go to 99
				}
			}
			else	// no Glass
			{

				if (((m_nLastError = SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_NOWAIT)) == GMP_SUCCESS) && getKlocworkTrue())
				{
					m_bGlass1Exist = FALSE;
					m_bGlass2Exist = FALSE;

					BUFFERWH_Status = ST_EMPTY;
					m_qSubOperation = BUFFERWH_EMPTY_Q;  // go to 3
				}
				else
				{
					m_qSubOperation = BUFFERWH_STOP_Q;  // go to 99
				}
			}

		break;

		case BUFFERWH_EMPTY_Q:	// 3

			if (m_bStopCycle)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (IsInspPanelReq())	// INSP Panel Request Button Pressed
			{
				if (
					//((m_nLastError = MoveZ(Z_CTRL_UNLOAD_LEVEL, GMP_WAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveY(Y_CTRL_IN_POSN, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					BUFFERWH_Status = ST_LOAD_REQ;
					m_qSubOperation = BUFFERWH_WAIT_PICKARM_LOAD_ACK_Q;	// go to 4
				}
				else
				{
					DisplayMessage("*********************************************");
					szMsg.Format("BUFFERWH_EMPTY_Q: ShBuffer WH Y go Load Posn Error");
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
					m_bModError = TRUE;
					m_qSubOperation = BUFFERWH_STOP_Q;  // go to 99
				}
			}
			else if (IsEmptyWHReq())	// Empty WH Request Button Pressed
			{
				if (
					//((m_nLastError = MoveZ(Z_CTRL_UNLOAD_LEVEL, GMP_WAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveY(Y_CTRL_OUT_POSN, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					UnLockDoor();
					//pCHouseKeeping->SetEmptyOutLamp(LIGHT_ON);
					m_qSubOperation = BUFFERWH_WAIT_DOOR_OPEN_Q;  // go to 7
				}
				else
				{
					DisplayMessage("*********************************************");
					szMsg.Format("BUFFERWH_EMPTY_Q: ShBuffer WH Y go Unload Posn Error");
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
					m_bModError = TRUE;
					m_qSubOperation = BUFFERWH_STOP_Q;  // go to 99
				}
			}
			else if (IsRejBeltReqDoorOpen())
			{
				UnLockDoor();
				m_qSubOperation = BUFFERWH_WAIT_DOOR_OPEN_Q;    // go to 1
			}

		break;

		case BUFFERWH_WAIT_PICKARM_LOAD_ACK_Q:	// 4
			
			if (OUTPICKARM_Status == ST_STOP)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (OUTPICKARM_Status == ST_BUF_LOAD_ACK)
			{
				m_qSubOperation = BUFFERWH_WAIT_PA_PLACE_GLASS_COMPLETE_Q;	// go to 5
			}

		break;

		case BUFFERWH_WAIT_PA_PLACE_GLASS_COMPLETE_Q:	// 5
			
			if(OUTPICKARM_Status == ST_STOP)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (OUTPICKARM_Status != ST_BUF_LOAD_ACK && OUTPICKARM_Status != ST_ERROR && OUTPICKARM_Status != ST_STOP)
			{
				m_qSubOperation = BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q;	// go to 6
			}
			else if (OUTPICKARM_Status == ST_ERROR || OUTPICKARM_Status == ST_VAC_ERROR)
			{		
				SetVacSol(MAX_NUM_OF_GLASS, ON, GMP_NOWAIT);
				BUFFERWH_Status = ST_VAC_ERROR;
				if (!IsGLASS1_VacSensorOn())
				{
					m_bModError = TRUE;
					DisplayMessage("*********************************************");
					SetError(IDS_SHBUFFER_WH_GLASS1_VAC_ERR);
					szMsg.Format("BUFFERWH_WAIT_PA_PLACE_GLASS_COMPLETE_Q: GLASS1 VAC ERROR");
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
				if (!IsGLASS2_VacSensorOn())
				{
					m_bModError = TRUE;
					DisplayMessage("*********************************************");
					SetError(IDS_SHBUFFER_WH_GLASS2_VAC_ERR);
					szMsg.Format("BUFFERWH_WAIT_PA_PLACE_GLASS_COMPLETE_Q: GLASS2 VAC ERROR");
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}


		break;

		case BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q:	// 6
			
			if (IsVacSensorOn(GLASS1) && IsVacSensorOn(GLASS2)) //both Exist
			{
				m_bGlass1Exist = TRUE;
				m_bGlass2Exist = TRUE;

				BUFFERWH_Status = ST_BUSY;	// Prevent Pick Arm Drop glass
				if (
					//((m_nLastError = MoveZ(Z_CTRL_UNLOAD_LEVEL, GMP_WAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveY(Y_CTRL_OUT_POSN, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					//SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_WAIT);	//both Vac OFF
					UnLockDoor();
					SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_WAIT);	//both Vac OFF
					//pCHouseKeeping->SetPanelOutLamp(LIGHT_ON);
					m_qSubOperation = BUFFERWH_WAIT_DOOR_OPEN_Q;	// go to 7
				}
				else
				{
					DisplayMessage("*********************************************");
					szMsg.Format("BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q: ShBuffer WH Y go Unload Posn Error");
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
					m_bModError = TRUE;
					m_qSubOperation = BUFFERWH_STOP_Q;  // go to 99
				}
				
			}
			else
			{
				BUFFERWH_Status = ST_VAC_ERROR;
				SetVacSol(MAX_NUM_OF_GLASS, ON, GMP_NOWAIT);
				if (!IsGLASS1_VacSensorOn())
				{
					m_bModError = TRUE;
					DisplayMessage("*********************************************");
					SetError(IDS_SHBUFFER_WH_GLASS1_VAC_ERR);
					szMsg.Format("BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q: GLASS1 VAC ERROR");
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
				if (!IsGLASS2_VacSensorOn())
				{
					m_bModError = TRUE;
					DisplayMessage("*********************************************");
					SetError(IDS_SHBUFFER_WH_GLASS2_VAC_ERR);
					szMsg.Format("BUFFERWH_CHECK_BUFFER_GLASS_EXIST_2_Q: GLASS2 VAC ERROR");
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}

		break;

		case BUFFERWH_WAIT_DOOR_OPEN_Q:	// 7

			if (m_bStopCycle)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (IsDoorOpen())
			{
				//LightOFF_InspReqButton();
				//LightOFF_EmptyWHReqButton();
				//ClearInspPanelReq();
				//ClearEmptyWHReq();
				bSetAlert = TRUE;
				m_qSubOperation = BUFFERWH_WAIT_DOOR_CLOSE_2_Q;	// go to 8
			}

		break;

		case BUFFERWH_WAIT_DOOR_CLOSE_2_Q:	// 8

			if (m_bStopCycle)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (IsDoorClose() && m_stSliderDetectSnr.GetGmpPort().IsOn())
			{
				LightOFF_InspReqButton();
				LightOFF_EmptyWHReqButton();
				ClearInspPanelReq();
				ClearEmptyWHReq();
				Sleep(500);
				LockDoor();
				CloseAlarm();
				pCHouseKeeping->m_bAlertExist = FALSE;
				BUFFERWH_Status = ST_IDLE;
				m_qSubOperation = BUFFERWH_IDLE_Q;	// go to 0
			}
			else
			{
				if (bSetAlert)
				{
					clkAlartStart = clock();
					bSetAlert = FALSE;
					SetAlert(IDS_HK_SHBUF_DOOR_NOT_DETECTED_ERR);
				}
				if (pCHouseKeeping->m_bBeepSound == TRUE && ((DOUBLE)(clock() - clkAlartStart) / CLOCKS_PER_SEC) > 5.0)
				{
					pCHouseKeeping->m_bBeepSound = FALSE; //turn off sound
				}
			}

		break;

		case BUFFERWH_GLASS_EXIST_Q:	// 9

			BUFFERWH_Status = ST_UNLOAD_REQ;
			m_qSubOperation = BUFFERWH_WAIT_PICKARM_UNLOAD_ACK_Q;	// go to 10

		break;

		case BUFFERWH_WAIT_PICKARM_UNLOAD_ACK_Q:	// 10
			
			if (m_bStopCycle)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (OUTPICKARM_Status == ST_BUF_UNLOAD_ACK)
			{
				m_qSubOperation = BUFFERWH_WAIT_PA_PICK_GLASS_COMPLETE_Q;	// go to 11
			}

		break;

		case BUFFERWH_WAIT_PA_PICK_GLASS_COMPLETE_Q:	// 11

			if(OUTPICKARM_Status == ST_STOP)
			{
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}
			else if (OUTPICKARM_Status != ST_BUF_UNLOAD_ACK && OUTPICKARM_Status != ST_ERROR && OUTPICKARM_Status != ST_STOP)
			{
				m_bGlass1Exist = FALSE;
				m_bGlass2Exist = FALSE;

				BUFFERWH_Status = ST_EMPTY;
				m_qSubOperation = BUFFERWH_CHECK_BUFFER_GLASS_EXIST_Q;	// go to 2
			}
			else if (OUTPICKARM_Status == ST_ERROR || OUTPICKARM_Status == ST_STOP)
			{		
				SetVacSol(MAX_NUM_OF_GLASS, ON, GMP_NOWAIT);
				BUFFERWH_Status = ST_VAC_ERROR;
				m_qSubOperation = BUFFERWH_STOP_Q;	// go to 99
			}

		break;

		case BUFFERWH_STOP_Q:	// 99
		break;



	}


	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			if (BUFFERWH_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("PICKARM Go State: %ld", m_qSubOperation);
			}
			else
			{
				szMsg.Format("PICKARM Go Stop");
			}
			//DisplayMessage(szMsg);
		}
		UpdateDebugSeq(GetSequenceString(), BUFFERWH_Status, m_bGlass1Exist, m_bGlass2Exist);
		m_qPreviousSubOperation = m_qSubOperation;

		if (HMI_bDebugSeq)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				stPrintData.szTitle[i]	= "";
			}

			stPrintData.szTitle[0].Format("%ld", GetTickCount());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", BUFFERWH_Status);

			LogFile->log(stPrintData.szTitle);
		}

	}


}

/////////////////////////////////////////////////////////////////
//Motor Related
/////////////////////////////////////////////////////////////////
INT CShBufferWH::MoveY(Y_CTRL_SIGNAL ctrl, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	LONG lMtrCnt = 1000;

	CString szMsg = _T("");

	if (!m_bModSelected)
	{
		return nResult;
	}

	try
	{

		switch (ctrl)
		{

			case Y_CTRL_IN_POSN:

				nResult = SetYInSol(ON, bWait);	
			break;

			case Y_CTRL_OUT_POSN:

				nResult = SetYInSol(OFF, bWait);
			break;
		}
	}
	catch (CAsmException e)
	{
		m_bModSelected = FALSE;

		if (nResult == GMP_SUCCESS)
		{
			nResult = GMP_FAIL;
		}
	}

	return nResult;
}


/////////////////////////////
// button lamp Operation
////////////////////////////
VOID CShBufferWH::LightOFF_InspReqButton()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->SetPanelOutLamp(LIGHT_OFF);
}

VOID CShBufferWH::LightOFF_EmptyWHReqButton()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->SetEmptyOutLamp(LIGHT_OFF);
}

VOID CShBufferWH::LightFlash_InspReqButton()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->SetPanelOutLamp(LIGHT_FLASH);
}

VOID CShBufferWH::LightFlash_EmptyWHReqButton()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->SetEmptyOutLamp(LIGHT_FLASH);
}


BOOL CShBufferWH::IsInspPanelReq()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	BOOL bPanelOut = pCHouseKeeping->m_bPanelOutReq;
	return bPanelOut;
}

VOID CShBufferWH::ClearInspPanelReq()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->m_bPanelOutReq = FALSE;
}

BOOL CShBufferWH::IsEmptyWHReq()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	BOOL bEmpty = pCHouseKeeping->m_bEmptyOutReq;
	return bEmpty;
}

BOOL CShBufferWH::IsRejBeltReqDoorOpen()
{
	CRejectBelt *pCRejectBelt = (CRejectBelt*)m_pModule->GetStation("RejectBelt");
	BOOL bOpenDoor = pCRejectBelt->m_bRequestDoorOpen;
	return bOpenDoor;
}
VOID CShBufferWH::ClearEmptyWHReq()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->m_bEmptyOutReq = FALSE;
}

VOID CShBufferWH::LockDoor()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->SetShBufDoorLockSol(ON, GMP_WAIT);
	//SetSol
}

VOID CShBufferWH::UnLockDoor()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	pCHouseKeeping->SetShBufDoorLockSol(OFF, GMP_WAIT);
	//SetSol(OFF, GMP_WAIT);
}


BOOL CShBufferWH::IsDoorOpen()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	BOOL bDoorOpen = !(pCHouseKeeping->m_bShBufDoorClose);
	return bDoorOpen;
}


BOOL CShBufferWH::IsDoorClose()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	BOOL bDoorClose = pCHouseKeeping->m_bShBufDoorClose;
	return bDoorClose;
}

VOID CShBufferWH::RegisterVariables()
{
	try
	{
		RegVariable(GetStation() + _T("bCheckMotor"), &m_bCheckMotor);
		RegVariable(GetStation() + _T("bCheckIO"), &m_bCheckIO);

		RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		RegVariable(GetStation() + _T("bShowModSelected"), &m_bShowModSelected);
		RegVariable(GetStation() + _T("bModError"), &m_bModError);

		RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		RegVariable(GetStation() + _T("bDebugCal"), &HMI_bDebugCal);
		RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeAll"), HMI_HomeAll);
		//Register Sensors here 
		//Sensor
		RegVariable(_T("HMI_bShBufferWHYLoadPosnSnr"), &m_stYInPosnSnr.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHYUnLoadPosnSnr"), &m_stYOutPosnSnr.bHMI_Status);
		RegVariable(_T("HMI_bShBufferSliderDetectSnr"), &m_stSliderDetectSnr.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHG1VacSnr"), &m_stGlass1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHG2VacSnr"), &m_stGlass2VacSnr.bHMI_Status);
		//RegVariable(_T("HMI_bShBufferWHG1ExistSnr"), &m_stGlass1ExistSnr.bHMI_Status);
		//RegVariable(_T("HMI_bShBufferWHG2ExistSnr"), &m_stGlass2ExistSnr.bHMI_Status);
		//RegVariable(_T("HMI_bShBufferWHZLoadPosnSnr"), &m_stZLoadPosnSnr.bHMI_Status);
		//RegVariable(_T("HMI_bShBufferWHZUnLoadPosnSnr"), &m_stZUnLoadPosnSnr.bHMI_Status);

		//Sol
		RegVariable(_T("HMI_bShBufferWHG1VacSol"), &m_stGlass1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHG2VacSol"), &m_stGlass2VacSol.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHG1WBSol"), &m_stWeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHG2WBSol"), &m_stWeakBlow2Sol.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHYLoadSol"), &m_stYLoadSol.bHMI_Status);
		//RegVariable(_T("HMI_bShBufferWHZLoadSol"), &m_stZLoadSol.bHMI_Status);
		

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlowSol"), HMI_ToggleWeakBlowSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlow2Sol"), HMI_ToggleWeakBlow2Sol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG1VacSol"), HMI_ToggleG1VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG2VacSol"), HMI_ToggleG2VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleYLoadSol"), HMI_ToggleYLoadSol);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleZLoadSol"), HMI_ToggleZLoadSol);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T(""), );
	}
	catch (CAsmException e)
	{
		DisplayException(e);
	}
}

INT CShBufferWH::SetYInSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stYLoadSol;

#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());

			if (!g_bDiagMode)
			{
				stTempSo->GetGmpPort().On();
				stTempSo->bHMI_Status = TRUE;
			}
			else
			{
				stTempSo->GetGmpPort().Off();
				stTempSo->bHMI_Status = FALSE;
			}

			if (bWait)
			{
				//Sleep(stTempSo->GetOnDelay());
				if (!g_bDiagMode)
				{
					LONG lDelay = stTempSo->GetOnDelay();
					LONG lTime = 0;
					while (m_stYOutPosnSnr.GetGmpPort().IsOn() || !m_stYInPosnSnr.GetGmpPort().IsOn())
					{
						if (lTime >= lDelay)
						{
							nResult = GMP_FAIL;
							DisplayMessage("ShBuffer WH Y Load Sol ON Timeout error");
							break;
						}
						Sleep(10);
						lTime += 10;
					}
				}
				else
				{
					Sleep(stTempSo->GetOffDelay());
				}
			}

			TC_DelJob(stTempSo->GetTCObjectID());
		}
		else
		{
			TC_AddJob(stTempSo->GetTCObjectID());

			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;

			if (bWait)
			{
				//Sleep(stTempSo->GetOffDelay());
				LONG lDelay = stTempSo->GetOffDelay();
				LONG lTime = 0;
				while (m_stYInPosnSnr.GetGmpPort().IsOn() || !m_stYOutPosnSnr.GetGmpPort().IsOn())
				{
					if (lTime >= lDelay)
					{
						nResult = GMP_FAIL;
						DisplayMessage("ShBuffer WH Y Load Sol OFF Timeout error");
						break;
					}
					Sleep(10);
					lTime += 10;
				}
			}

			TC_DelJob(stTempSo->GetTCObjectID());
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	if (nResult != GMP_SUCCESS)
	{
		if ((m_qState == AUTO_Q) || (m_qState == DEMO_Q))
		{

			SetError(bMode ? IDS_SHBUFFER_WH_Y_SOL_ON_ERR : IDS_SHBUFFER_WH_Y_SOL_OFF_ERR);
		}
		else
		{
			CString szMessage;
			szMessage.Format("%s Y Load %s Error.", GetStnName(), (bMode ? "ON" : "OFF"));
			HMIMessageBox(MSG_OK, "SET SHBUFFER WH SOL", szMessage);
		}
	}
	
	return nResult;
}

LONG CShBufferWH::HMI_ToggleYLoadSol(IPC_CServiceMessage &svMsg)
{
	if (m_stYLoadSol.bHMI_Status)
	{
		SetYInSol(OFF);
	}
	else
	{
		SetYInSol(ON);
	}

	return 0;
}

VOID CShBufferWH::ModuleSetupPreTask()
{
	m_bCheckMotor		= TRUE;
	m_bCheckIO			= TRUE;
	
	
	UpdateModuleSetupPara();
}

VOID CShBufferWH::ModuleSetupPostTask()
{
	m_bCheckMotor		= FALSE;
	m_bCheckIO			= FALSE;
}

VOID CShBufferWH::UpdateModuleSetupPara()
{
	INT i = 0;
	INT j = 0;

	// Para Tab
	// Delay
	i = 0;		
	g_stSetupPara[i].szPara.Format("Y Sol On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= MACHINE_PARA;
	g_stSetupPara[i].lPara			= m_stYLoadSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 1
	g_stSetupPara[i].szPara.Format("Y Sol Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= MACHINE_PARA;
	g_stSetupPara[i].lPara			= m_stYLoadSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 2
	g_stSetupPara[i].szPara.Format("Vac On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stGlass1VacSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 3
	g_stSetupPara[i].szPara.Format("Vac Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stGlass1VacSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;
	
	i++;	// 4
	g_stSetupPara[i].szPara.Format("WeakBlow On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stWeakBlowSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 5
	g_stSetupPara[i].szPara.Format("WeakBlow Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stWeakBlowSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;


	// Others
	for (j = i + 1; j < NUM_OF_PARA; j++)
	{
		g_stSetupPara[j].szPara.Format("Reserved");
		g_stSetupPara[j].bEnable		= FALSE;
		g_stSetupPara[j].bVisible		= FALSE;
		g_stSetupPara[j].lType			= MACHINE_PARA;
		g_stSetupPara[j].lPara			= 0;
		g_stSetupPara[j].dPara			= 0.0;
	}
}



BOOL CShBufferWH::ModuleSetupSetPara(PARA stPara)
{

	if (	g_ulUserGroup < SERVICE &&
		g_stSetupPara[stPara.lSetupTitle].lType == MACHINE_PARA)
	{
		HMIMessageBox(MSG_OK, "WARNING", "You are not allowed to change Machine Parameters");
		return FALSE;
	}

	switch (stPara.lSetupTitle)
	{
	case 0:
		m_stYLoadSol.SetOnDelay(stPara.lPara);
		break;

	case 1:
		m_stYLoadSol.SetOffDelay(stPara.lPara);
		break;

	case 2:
		m_stGlass1VacSol.SetOnDelay(stPara.lPara);
		m_stGlass2VacSol.SetOnDelay(stPara.lPara);
		break;

	case 3:
		m_stGlass1VacSol.SetOffDelay(stPara.lPara);
		m_stGlass2VacSol.SetOffDelay(stPara.lPara);
		break;

	case 4:
		m_stWeakBlowSol.SetOnDelay(stPara.lPara);
		m_stWeakBlow2Sol.SetOnDelay(stPara.lPara);
		break;

	case 5:
		m_stWeakBlowSol.SetOffDelay(stPara.lPara);
		m_stWeakBlow2Sol.SetOffDelay(stPara.lPara);
		break;	
	}


	return CPI9000Stn::ModuleSetupSetPara(stPara);
}

LONG CShBufferWH::IPC_SaveMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	//Posn
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"]		= m_mtrStandbyPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.y"]		= m_mtrLoadGlassPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.y"]	= m_mtrUnloadGlassPosn.y;

	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lStandbyLevel"]			= m_lStandbyLevel[i];
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lCalibLoadLevel"]		= m_lCalibLoadLevel[i]; 
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lCalibUnloadLevel"]		= m_lCalibUnloadLevel[i];
	}
	return CWorkHolder::IPC_SaveMachineParam();
}

LONG CShBufferWH::IPC_LoadMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	//Posn
	m_mtrStandbyPosn.y		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"];
	m_mtrLoadGlassPosn.y	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.y"];
	m_mtrUnloadGlassPosn.y	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.y"];

	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		m_lStandbyLevel[i]		= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lStandbyLevel"];
		m_lCalibLoadLevel[i]	= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lCalibLoadLevel"]; 
		m_lCalibUnloadLevel[i]	= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lCalibUnloadLevel"];
	}

	return CWorkHolder::IPC_LoadMachineParam();
}

LONG CShBufferWH::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;
	//Posn

	//Level	
	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lLoadLevelOffset"]		= m_lLoadLevelOffset[i]; 
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadLevelOffset"]	= m_lUnloadLevelOffset[i];
	}
	
	//Param

	return CWorkHolder::IPC_SaveDeviceParam();
}

LONG CShBufferWH::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str;

	//Level	
	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		m_lLoadLevelOffset[i]	= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lLoadLevelOffset"]; 
		m_lUnloadLevelOffset[i]	= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadLevelOffset"];
	}
	
	//Param

	return CWorkHolder::IPC_LoadDeviceParam();
}