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

#include "stdafx.h"
#include "MarkConstant.h"
#include "PI9000.h"
#include "PI9000_Constant.h"
#include "HouseKeeping.h"
#include "ShBufferWH.h"
#include "Cal_Util.h"

#define REJBELT_EXTERN
#include "RejectBelt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CRejectBelt, CPI9000Stn)

CRejectBelt::CRejectBelt()
{
	m_lStnErrorCode		= IDS_REJBELT_NOT_SELECTED_ERR;
	m_stMotorBelt.InitMotorInfo("REJBELT_CHANNEL", &m_bModSelected); //REJECT_BELT
	m_myMotorList.Add((CMotorInfo*)&m_stMotorBelt);

	m_stRejBeltInPosnSnr		= CSiInfo("RejBeltInPosnSnr",		"IOMODULE_IN_PORT_6", BIT_21, ACTIVE_HIGH);
	m_mySiList.Add((CSiInfo*)&m_stRejBeltInPosnSnr);

	m_stRejBeltEndSnr		= CSiInfo("RejBeltExistSnr",		"IOMODULE_IN_PORT_6", BIT_20, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRejBeltEndSnr);

	m_bRequestDoorOpen = FALSE;

	// Srch Prof
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON].bEnable					= GMP_ENABLE;
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON].sSrchProfID				= USR_DEF_SRCH_PROF1;
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON].sSrchDirection				= POSITIVE_DIR;
	strcpy((char*)m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON].cPort, m_stRejBeltEndSnr.GetPortName());
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON].unSensorMask				= m_stRejBeltEndSnr.GetMask();
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON].bAstate					= m_stRejBeltEndSnr.GetActiveState();

	m_stInConvBeltSrchForwardPara.dSrchVel		= 10;
	m_stInConvBeltSrchForwardPara.dSrchLimit	= 5000000;
	m_stInConvBeltSrchForwardPara.dDriveInVel	= 0;
	m_stInConvBeltSrchForwardPara.dDriveIn		= 0;
	m_stInConvBeltSrchForwardPara.sDebounce		= 10;

	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_OFF].bEnable					= GMP_ENABLE;
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_OFF].sSrchProfID				= USR_DEF_SRCH_PROF2;
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_OFF].sSrchDirection			= NEGATIVE_DIR;
	strcpy((char*)m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_OFF].cPort, m_stRejBeltEndSnr.GetPortName());
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_OFF].unSensorMask				= m_stRejBeltEndSnr.GetMask();
	m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_OFF].bAstate					= !m_stRejBeltEndSnr.GetActiveState();

	m_stInConvBeltSrchBackwardPara.dSrchVel		= 10;
	m_stInConvBeltSrchBackwardPara.dSrchLimit	= 1000;
	m_stInConvBeltSrchBackwardPara.dDriveInVel	= 10;
	m_stInConvBeltSrchBackwardPara.dDriveIn		= 50;
	m_stInConvBeltSrchBackwardPara.sDebounce	= 2;

	//m_dConvBeltSpeedFactor = 1.0;
}

CRejectBelt::~CRejectBelt()
{
}

BOOL CRejectBelt::InitInstance()
{
	CPI9000Stn::InitInstance();
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

				GetmySoList().GetAt(i)->GetGmpPort().Off();
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

	if (m_fHardware && !pAppMod->m_bHWInitError && !pAppMod->m_bDiagnMode)
	{
		szMsg = _T("	") + GetName() + " --- commutating servos";
		DisplayMessage(szMsg);

		// Commutate Motor
		for (i = 0; i < m_myMotorList.GetSize(); i++)
		{
			if (GetmyMotorList().GetAt(i)->Commutate() != GMP_SUCCESS)
			{
				m_bInitInstanceError			= TRUE;
				pAppMod->m_bHWInitError			= TRUE;
			}
		}

		do
		{
			Sleep(100);
		} while (!pAppMod->IsAllServoCommutated());

		if (!m_bInitInstanceError)
		{
			szMsg = _T("	") + GetName() + " --- homing motors";
			DisplayMessage(szMsg);

			// Home Motor
			for (i = 0; i < m_myMotorList.GetSize(); i++)
			{
				if (GetmyMotorList().GetAt(i)->Home(GMP_NOWAIT) != GMP_SUCCESS)
				{
					m_bInitInstanceError	= TRUE;
					pAppMod->m_bHWInitError = TRUE;
				}
			}

			// Sync & Enable Protection
			for (i = 0; i < m_myMotorList.GetSize(); i++)
			{
				if (
					GetmyMotorList().GetAt(i)->Sync() != GMP_SUCCESS ||
					GetmyMotorList().GetAt(i)->EnableProtection() != GMP_SUCCESS
				   )
				{
					m_bInitInstanceError	= TRUE;
					pAppMod->m_bHWInitError = TRUE;
				}
				else
				{
					GetmyMotorList().GetAt(i)->bHomeStatus = TRUE;
				}
			}

			if (!pAppMod->m_bHWInitError)
			{
				m_bModSelected			= TRUE;
			}

		}
	}
	else
	{
		m_bModSelected			= FALSE;
	}
	return TRUE;
}

INT CRejectBelt::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CRejectBelt::Operation()
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

VOID CRejectBelt::UpdateOutput()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	INT i = 0;
	if (!m_fHardware)
	{
		return;
	}

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
		DisplayException(e);
	}
}

/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CRejectBelt::PreStartOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	m_bModError = FALSE;
	m_bRequestDoorOpen = FALSE;
	
	if (!m_bModSelected)
	{	
		REJECTBELT_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = REJBELT_STOP_Q;
		m_bStopCycle = FALSE;
	}
	else
	{
		
			if (
				((m_nLastError = MoveBelt(REJ_BELT_CTRL_MOVE_FORWARD, GMP_WAIT)) != GMP_SUCCESS)
			   )
			{
				Command(glSTOP_COMMAND, TRUE);
				REJECTBELT_Status = ST_STOP;
				m_qSubOperation = REJBELT_STOP_Q;
				m_bStopCycle = TRUE;
			}
			else
			{
				REJECTBELT_Status = ST_IDLE;
				m_qSubOperation = REJBELT_IDLE_Q;
				m_bStopCycle = FALSE;
				g_bGlassOnRejBelt = FALSE;
			}
	}


	Motion(FALSE);
	State(IDLE_Q);

	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), REJECTBELT_Status);

	CPI9000Stn::PreStartOperation();
}

VOID CRejectBelt::AutoOperation()
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
		(m_bModSelected && m_qSubOperation == REJBELT_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			
			//if (AutoMode == BURN_IN) //20130620
			//{
			//	if (!IsGLASS1_VacSensorOn())
			//	{
			//		SetGLASS1_VacSol(OFF);
			//	}
			//}
			//if (AutoMode == BURN_IN)
			//{
			//	if (!IsGLASS2_VacSensorOn())
			//	{
			//		SetGLASS2_VacSol(OFF);
			//	}
			//}
		}

		if (GMP_SUCCESS != m_nLastError)
		{
			m_bModError = TRUE;
		}

		
		REJECTBELT_Status = ST_STOP;
		MoveBelt(REJ_BELT_CTRL_MOVE_STOP, GMP_WAIT);

		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

		CPI9000Stn::StopOperation();
	}
}


CString CRejectBelt::GetSequenceString()
{

	switch (m_qSubOperation)
	{
	case REJBELT_IDLE_Q:
		return "REJBELT_IDLE_Q";
		
	case REJBELT_CHECK_END_SNR_Q:
		return "REJBELT_CHECK_END_SNR_Q";

	case REJBELT_DISPLAY_MESSAGE_Q:
		return "REJBELT_DISPLAY_MESSAGE_Q";

	case REJBELT_WAIT_END_SNR_BLOCKED_Q:
		return "REJBELT_WAIT_END_SNR_BLOCKED_Q";

	case REJBELT_WAIT_END_SNR_UNBLOCK_Q:
		return "REJBELT_WAIT_END_SNR_UNBLOCK_Q";

	case REJBELT_STOP_Q:
		return "REJBELT_STOP_Q";
		
	}

	return "Hello World!!";
}


VOID CRejectBelt::OperationSequence()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	PRINT_DATA	stPrintData;

	m_nLastError = GMP_SUCCESS;


	CString szMsg = "";


	switch (m_qSubOperation)
	{

		case REJBELT_IDLE_Q:	// 0
			
			if (m_bStopCycle)
			{
				m_qSubOperation = REJBELT_STOP_Q;	// go to 99
			}
			//else if (IsRejBeltEndSnrOn())
			//{
			//	SetAlert(IDS_REJBELT_FULL_ERR);
			//	m_qSubOperation = REJBELT_DISPLAY_MESSAGE_Q;  // go to 2
			//}
			else if (g_bGlassOnRejBelt && !IsRejBeltEndSnrOn())
			{
				if ((m_nLastError = MoveBelt(REJ_BELT_CTRL_MOVE_FORWARD, GMP_WAIT)) == GMP_SUCCESS)
				{
					g_bGlassOnRejBelt = FALSE;
					m_qSubOperation = REJBELT_CHECK_END_SNR_Q;  // go to 1
				}
				else
				{
					m_qSubOperation = REJBELT_STOP_Q;  // go to 99
				}
			}
			else if (IsRejBeltEndSnrOn())
			{
				SetAlert(IDS_REJBELT_FULL_ERR);
				m_qSubOperation = REJBELT_DISPLAY_MESSAGE_Q;  // go to 2
			}
			else if (!IsRejBeltAtInPosn())
			{
				MoveBelt(REJ_BELT_CTRL_MOVE_NONSTOP, GMP_NOWAIT);
				g_bGlassOnRejBelt = FALSE;
				m_qSubOperation = REJBELT_WAIT_END_SNR_BLOCKED_Q;  // go to 3
			}

			break;

		case REJBELT_CHECK_END_SNR_Q:	// 1
			
			if (IsRejBeltEndSnrOn())
			{
				SetAlert(IDS_REJBELT_FULL_ERR);
				m_qSubOperation = REJBELT_DISPLAY_MESSAGE_Q;  // go to 2
			}
			else
			{
				m_qSubOperation = REJBELT_IDLE_Q;  // go to 0
			}

			break;

		case REJBELT_DISPLAY_MESSAGE_Q:	// 2

			if (m_bStopCycle)
			{
				m_qSubOperation = REJBELT_STOP_Q;	// go to 99
			}
			//else if (IsRejBeltEndSnrOn())
			//{
			//	g_bGlassOnRejBelt = TRUE;
			//}
			else if (!IsRejBeltAtInPosn())
			{
				CloseAlarm();
				pCHouseKeeping->m_bAlertExist = FALSE;
				Sleep(500);
				//MoveBelt(REJ_BELT_CTRL_MOVE_NONSTOP, GMP_NOWAIT);
				g_bGlassOnRejBelt = FALSE;
				m_qSubOperation = REJBELT_WAIT_END_SNR_BLOCKED_Q;  // go to 3
			
			}

			break;

		case REJBELT_WAIT_END_SNR_BLOCKED_Q:	// 3

			if (m_bStopCycle)
			{
				m_qSubOperation = REJBELT_STOP_Q;	// go to 99
			}
			else if (IsRejBeltAtInPosn())
			{
				//CloseAlarm();
				//pCHouseKeeping->m_bAlertExist = FALSE;
				//Sleep(500);
				MoveBelt(REJ_BELT_CTRL_MOVE_STOP, GMP_WAIT);
				g_bGlassOnRejBelt = FALSE;
				m_qSubOperation = REJBELT_IDLE_Q;  // go to 0
			}
			else if (IsRejBeltEndSnrOn())
			{
				//SetAlert(IDS_REJBELT_FULL_ERR);
				//g_bGlassOnRejBelt = TRUE;

				//MoveBelt(REJ_BELT_CTRL_MOVE_STOP, GMP_WAIT);
				m_qSubOperation = REJBELT_WAIT_END_SNR_UNBLOCK_Q;  // go to 4
			}
			else if (!IsRejBeltEndSnrOn())
			{
				g_bGlassOnRejBelt = FALSE;
			}
			break;

		case REJBELT_WAIT_END_SNR_UNBLOCK_Q:	// 4
			if (m_bStopCycle)
			{
				m_qSubOperation = REJBELT_STOP_Q;	// go to 99
			}
			else if (IsRejBeltAtInPosn())
			{
				//CloseAlarm();
				//pCHouseKeeping->m_bAlertExist = FALSE;
				//Sleep(500);
				//MoveBelt(REJ_BELT_CTRL_MOVE_STOP, GMP_WAIT);
				g_bGlassOnRejBelt = FALSE;
				m_qSubOperation = REJBELT_IDLE_Q;  // go to 0
			}
			else if (!IsRejBeltEndSnrOn())
			{
				//CloseAlarm();
				//pCHouseKeeping->m_bAlertExist = FALSE;
				Sleep(500);
				MoveBelt(REJ_BELT_CTRL_MOVE_NONSTOP, GMP_NOWAIT);
			//	g_bGlassOnRejBelt = FALSE;
				m_qSubOperation = REJBELT_WAIT_END_SNR_BLOCKED_Q;  // go to 3
			}
			break;

		case REJBELT_STOP_Q:	// 0
			break;

	}

	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			if (REJBELT_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("REJECTBELT Go State: %ld", m_qSubOperation);
			}
			else
			{
				szMsg.Format("REJECTBELT Go Stop");
			}
			//DisplayMessage(szMsg);
		}
		UpdateDebugSeq(GetSequenceString(), REJECTBELT_Status);
		m_qPreviousSubOperation = m_qSubOperation;

		if (HMI_bDebugSeq)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				stPrintData.szTitle[i]	= "";
			}

			stPrintData.szTitle[0].Format("%ld", GetTickCount());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", REJECTBELT_Status);

			LogFile->log(stPrintData.szTitle);
		}

	}


}


/////////////////////////////////////////////////////////////////
//Motor Related
/////////////////////////////////////////////////////////////////
INT CRejectBelt::MoveBelt(REJ_BELT_CTRL_SIGNAL ctrl, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	LONG lMtrCnt = DoubleToLong(g_stGlass.dLength * 1000 / REJBELT_DINSTANCE_STEP + 500);// 2000;

	SRCH_PROFILE	stSrchProf;

	CString szMsg = _T("");

	if (!m_bModSelected)
	{
		return nResult;
	}

	try
	{

		switch (ctrl)
		{
			case REJ_BELT_CTRL_MOVE_FORWARD:

				if (!IsRejBeltEndSnrOn())
				{
					nResult = m_stMotorBelt.MoveRelative(lMtrCnt, bWait);	
				}
			break;

			case REJ_BELT_CTRL_MOVE_HALF_FORWARD:

				if (!IsRejBeltEndSnrOn())
				{
					nResult = m_stMotorBelt.MoveRelative(lMtrCnt / 2, bWait);	
				}
			break;

			case REJ_BELT_CTRL_MOVE_NONSTOP:

				m_stInConvBeltSrchForwardPara.dSrchVel = 30 /** m_dConvBeltSpeedFactor*/;
				m_stMotorBelt.SetSrchProfSi(m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON].sSrchProfID, m_stInConvBeltSrchForwardPara);
				stSrchProf = m_stMotorBelt.stUsrDefProfile[REJ_BELT_SRCH_SNR_ON];
				nResult = m_stMotorBelt.Search(stSrchProf, NO_PROCESS_LIST, bWait);	//Srch Action
			break;

			case REJ_BELT_CTRL_MOVE_STOP:

				nResult = m_stMotorBelt.MotorStop();
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

		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		m_bModError = TRUE;
		SetError(IDS_REJBELT_MOVE_ERR);
		DisplayException(e);
		szMsg.Format("    AXIS: axRejBelt");
		DisplayMessage(szMsg);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return nResult;
}


BOOL CRejectBelt::IsRejBeltEndSnrOn()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = FALSE;


	if (!pAppMod->IsInitNuDrive())
	{
		return bResult;
	}

	try
	{
		bResult = m_stRejBeltEndSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CRejectBelt::IsRejBeltAtInPosn()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = FALSE;


	if (!pAppMod->IsInitNuDrive())
	{
		return bResult;
	}

	try
	{
		bResult = m_stRejBeltInPosnSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

VOID CRejectBelt::UpdateProfile()
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

VOID CRejectBelt::UpdatePosition()
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

VOID CRejectBelt::RegisterVariables()
{
	try
	{
		RegVariable(GetStation() + _T("bCheckMotor"), &m_bCheckMotor);
		RegVariable(GetStation() + _T("bCheckIO"), &m_bCheckIO);

		RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		RegVariable(GetStation() + _T("bShowModSelected"), &m_bShowModSelected);
		RegVariable(GetStation() + _T("bModError"), &m_bModError);

		RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);

		RegVariable(_T("HMI_bRejBelt_BeltOn"), &m_stMotorBelt.bHMI_On);
		RegVariable(GetStation() + _T("dBeltDistPerCount"), &m_stMotorBelt.stAttrib.dDistPerCount);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeAll"), HMI_HomeAll);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnBelt"), HMI_PowerOnBelt);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexBeltForward"), HMI_IndexBeltForward);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexBeltBackward"), HMI_IndexBeltBackward);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogMotion"), HMI_SetLogMotion);


		
		RegVariable(_T("HMI_bRejBeltEndSnr"), &m_stRejBeltEndSnr.bHMI_Status);
		RegVariable(_T("HMI_bRejBeltInPosnSnr"), &m_stRejBeltInPosnSnr.bHMI_Status);
		


	}
	catch (CAsmException e)
	{
		DisplayException(e);
	}
}

VOID CRejectBelt::ModuleSetupPreTask()
{
	// Check Motor / IO
	m_bCheckIO						= TRUE;
	m_bCheckMotor					= TRUE;

	// CheckModSelected
	CheckModSelected(TRUE);

	SetDiagnSteps(g_lDiagnSteps);

}

VOID CRejectBelt::SetDiagnSteps(ULONG ulSteps)
{
	switch (ulSteps)
	{
	case 0:
		m_lDiagnSteps = 1;
		break;
	case 1:
		m_lDiagnSteps = 10;
		break;
	case 2:
		m_lDiagnSteps = 100;
		break;
	case 3:
		m_lDiagnSteps = 1000;
		break;
	case 4:
		m_lDiagnSteps = 5000;
		break;
	case 5:
		m_lDiagnSteps = 10000;
		break;
	default:

		m_lDiagnSteps = 1000;
	}
}

VOID CRejectBelt::ModuleSetupPostTask()
{
	// Check Motor / IO
	m_bCheckIO							= FALSE;
	m_bCheckMotor						= FALSE;
}

LONG CRejectBelt::HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg)
{
	ULONG ulSteps;
	svMsg.GetMsg(sizeof(ULONG), &ulSteps);

	g_lDiagnSteps = ulSteps;

	SetDiagnSteps(ulSteps);	
	return 0;
}

LONG CRejectBelt::HMI_PowerOnBelt(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	LogAction(__FUNCTION__);

	m_stMotorBelt.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CRejectBelt::HMI_IndexBeltForward(IPC_CServiceMessage &svMsg)
{
	m_stMotorBelt.MoveRelative(m_lDiagnSteps, GMP_WAIT);  
	return 0;
}

LONG CRejectBelt::HMI_IndexBeltBackward(IPC_CServiceMessage &svMsg)
{
	m_stMotorBelt.MoveRelative(-m_lDiagnSteps, GMP_WAIT);  
	return 0;
}

