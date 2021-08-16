/////////////////////////////////////////////////////////////////
//	PICKARM.cpp : interface of the CPickArm class
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
#include "Cal_Util.h"

#define PICKARM_EXTERN
#include "PICKARM.h"

#include "INSPWH.h"
#include "ShBufferWH.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPickArm, CTransferArm)

CPickArm::CPickArm()
{
	//I/O and Motors are done!!
	// CPI9000Srn
	m_lStnErrorCode		= IDS_PICKARM_NOT_SELECTED_ERR;
	m_lModSelectMode	= GLASS_MOD;	

	//// Motor Related
	//m_stMotorX.InitMotorInfo("PICKARM_X", &m_bModSelected);	//done

	//m_myMotorList.Add((CMotorInfo*)&m_stMotorX);


	//// Si Related
	//m_stXLmtSnr			= CSiInfo("PickArmXLmtSnr",				"IOMODULE_IN_PORT_4", BIT_13, ACTIVE_HIGH);
	//m_mySiList.Add((CSiInfo*)&m_stXLmtSnr);	
	//m_stCOF1VacSnr		= CSiInfo("PickArmCOF1VacSnr",			"IOMODULE_IN_PORT_4", BIT_14, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stCOF1VacSnr);
	//m_stGlass1VacSnr		= CSiInfo("PickArmLVacSnr",			"IOMODULE_IN_PORT_4", BIT_15, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stGlass1VacSnr);
	//m_stCOF2VacSnr		= CSiInfo("PickArmCOF2VacSnr",			"IOMODULE_IN_PORT_4", BIT_16, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stCOF2VacSnr);
	//m_stGlass2VacSnr		= CSiInfo("PickArmRVacSnr",			"IOMODULE_IN_PORT_4", BIT_17, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stGlass2VacSnr);

	//m_stRecieveUpStreamInLine		= CSiInfo("PickArmRecieveUpStreamInLine",		"IOMODULE_IN_PORT_8", BIT_0, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveUpStreamInLine);	
	//m_stRecieveReady				= CSiInfo("PickArmRecieveReady",				"IOMODULE_IN_PORT_8", BIT_1, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveReady);
	//m_stRecieving					= CSiInfo("PickArmRecieving",					"IOMODULE_IN_PORT_8", BIT_2, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieving);
	//m_stRecieveStageMotion			= CSiInfo("PickArmRecieveStageMotion",			"IOMODULE_IN_PORT_8", BIT_3, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveStageMotion);
	//m_stRecieveStageInPosition		= CSiInfo("PickArmRecieveStageInPosition",		"IOMODULE_IN_PORT_8", BIT_4, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveStageInPosition);
	//m_stRecieveStageVacOK			= CSiInfo("PickArmRecieveStageVacOK",			"IOMODULE_IN_PORT_8", BIT_5, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveStageVacOK);
	//m_stRecieveG1VacOff				= CSiInfo("PickArmRecieveG1VacOff",				"IOMODULE_IN_PORT_8", BIT_6, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveG1VacOff);
	//m_stRecieveG2VacOff				= CSiInfo("PickArmRecieveG2VacOff",				"IOMODULE_IN_PORT_8", BIT_7, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveG2VacOff);
	//m_stRecieveG1VacError			= CSiInfo("PickArmRecieveG1VacError",			"IOMODULE_IN_PORT_8", BIT_8, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveG1VacError);
	//m_stRecieveG2VacError			= CSiInfo("PickArmRecieveG2VacError",			"IOMODULE_IN_PORT_8", BIT_9, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveG2VacError);
	//m_stRecieveCancel				= CSiInfo("PickArmRecieveCancel",				"IOMODULE_IN_PORT_8", BIT_10, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRecieveCancel);
	//m_stInterLockFront				= CSiInfo("PickArmInterLockFront",				"IOMODULE_IN_PORT_8", BIT_11, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stInterLockFront);

	//// So Related
	//m_stCOF1VacSol		= CSoInfo("PickArmCOF1VacSol",			"IOMODULE_OUT_PORT_3", BIT_7, PICKARM_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stCOF1VacSol);
	//m_stGlass1VacSol		= CSoInfo("PickArmLVacSol",			"IOMODULE_OUT_PORT_3", BIT_8, PICKARM_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stGlass1VacSol);
	//m_stCOF2VacSol		= CSoInfo("PickArmCOF2VacSol",			"IOMODULE_OUT_PORT_3", BIT_9, PICKARM_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stCOF2VacSol);
	//m_stGlass2VacSol		= CSoInfo("PickArmRVacSol",			"IOMODULE_OUT_PORT_3", BIT_10, PICKARM_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stGlass2VacSol);
	//m_stWeakBlowSol			= CSoInfo("PickArmWeakBlowSol",		"IOMODULE_OUT_PORT_3", BIT_11, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stWeakBlowSol);

	//m_stSendUpStreamInLine		= CSoInfo("PickArmSendUpStreamInLine",		"IOMODULE_OUT_PORT_7", BIT_0, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendUpStreamInLine);
	//m_stSendReady				= CSoInfo("PickArmSendReady",				"IOMODULE_OUT_PORT_7", BIT_1, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendReady);
	//m_stSendStageMotion			= CSoInfo("PickArmSendMotion",				"IOMODULE_OUT_PORT_7", BIT_2, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendStageMotion);
	//m_stSendStageInPosition		= CSoInfo("PickArmSendInPosition",			"IOMODULE_OUT_PORT_7", BIT_3, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendStageInPosition);
	//m_stSendStageVacOn			= CSoInfo("PickArmSendStageVacOn",			"IOMODULE_OUT_PORT_7", BIT_4, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendStageVacOn);
	//m_stSending					= CSoInfo("PickArmSending",					"IOMODULE_OUT_PORT_7", BIT_5, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSending);
	//m_stSendG1Exist				= CSoInfo("PickArmSendG1Exist",				"IOMODULE_OUT_PORT_7", BIT_6, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendG1Exist);
	//m_stSendG2Exist				= CSoInfo("PickArmSendG2Exist",				"IOMODULE_OUT_PORT_7", BIT_7, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendG2Exist);
	//m_stSendCancel				= CSoInfo("PickArmSendCancel",				"IOMODULE_OUT_PORT_7", BIT_8, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stSendCancel);
	//m_stInterLockBack			= CSoInfo("PickArmInterLockBack",			"IOMODULE_OUT_PORT_7", BIT_9, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stInterLockBack);
	//m_stPAEMO					= CSoInfo("PickArmEMO",						"IOMODULE_OUT_PORT_7", BIT_10, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stPAEMO);

	m_bIsDefaultIndex	= FALSE;	// TRUE: Place Glass2 First, then Glass1

	m_lPickGlassOffset	= 0;
	
	m_bIsVacError = FALSE; //20121224 vac error
	HMI_bStandAlone		= FALSE;
	m_bInlineInBack		= FALSE;
}

CPickArm::~CPickArm()
{
}

BOOL CPickArm::InitInstance()
{
	CTransferArm::InitInstance();
	return TRUE;
}

INT CPickArm::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CTransferArm::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CPickArm::Operation()
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
VOID CPickArm::UpdateOutput()
{
	// Keep it for debug breakpoint

	CTransferArm::UpdateOutput();
}

VOID CPickArm::RegisterVariables()
{
	try
	{
		//Register Sensors here 
		// -------------------------------------------------------
		// PI9000Stn Variable and Function
		// -------------------------------------------------------
		RegVariable(GetStation() + _T("bCheckMotor"), &m_bCheckMotor);
		RegVariable(GetStation() + _T("bCheckIO"), &m_bCheckIO);

		RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		RegVariable(GetStation() + _T("bShowModSelected"), &m_bShowModSelected);
		RegVariable(GetStation() + _T("bModError"), &m_bModError);

		RegVariable(GetStation() + _T("bGlass1Exist"), &m_bGlass1Exist);
		RegVariable(GetStation() + _T("bGlass2Exist"), &m_bGlass2Exist); //edge-cleaner
		RegVariable(GetStation() + _T("bUseCOFVacuum"), &m_bUseCOFVacuum);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleUseCOFVacuum"), HMI_ToggleUseCOFVacuum);
		

		RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeAll"), HMI_HomeAll);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogMotion"), HMI_SetLogMotion);

		// -------------------------------------------------------
		// TransferArm Variable and Function
		// -------------------------------------------------------
		RegVariable(_T("HMI_bPickArmXOn"), &m_stMotorX.bHMI_On);
		RegVariable(_T("HMI_lPickArmXEncoder"), &m_stMotorX.lHMI_CurrPosn);
		RegVariable(GetStation() + _T("dXDistPerCount"), &m_stMotorX.stAttrib.dDistPerCount);

		RegVariable(_T("HMI_bPickArmXLmtSnr"), &m_stXLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bPickArmG1VacSnr"), &m_stGlass1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bPickArmG2VacSnr"), &m_stGlass2VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bPickArmCOF1VacSnr"), &m_stCOF1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bPickArmCOF2VacSnr"), &m_stCOF2VacSnr.bHMI_Status);

		RegVariable(_T("HMI_bPickArmRecieveUpStreamInLine"), &m_stRecieveUpStreamInLine.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveReady"), &m_stRecieveReady.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieving"), &m_stRecieving.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveStageMotion"), &m_stRecieveStageMotion.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveStageInPosition"), &m_stRecieveStageInPosition.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveStageVacOK"), &m_stRecieveStageVacOK.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveG1VacOff"), &m_stRecieveG1VacOff.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveG2VacOff"), &m_stRecieveG2VacOff.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveG1VacError"), &m_stRecieveG1VacError.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveG2VacError"), &m_stRecieveG2VacError.bHMI_Status);
		RegVariable(_T("HMI_bPickArmRecieveCancel"), &m_stRecieveCancel.bHMI_Status);
		RegVariable(_T("HMI_bPickArmInterLockFront"), &m_stInterLockFront.bHMI_Status);

		RegVariable(_T("HMI_bPickArmSendUpStreamInLine"), &m_stSendUpStreamInLine.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSendReady"), &m_stSendReady.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSendStageMotion"), &m_stSendStageMotion.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSendStageInPosition"), &m_stSendStageInPosition.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSendStageVacOn"), &m_stSendStageVacOn.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSending"), &m_stSending.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSendG1Exist"), &m_stSendG1Exist.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSendG2Exist"), &m_stSendG2Exist.bHMI_Status);
		RegVariable(_T("HMI_bPickArmSendCancel"), &m_stSendCancel.bHMI_Status);
		RegVariable(_T("HMI_bPickArmInterLockBack"), &m_stInterLockBack.bHMI_Status);
		RegVariable(_T("HMI_bPickArmEMO"), &m_stPAEMO.bHMI_Status);
		
		RegVariable(_T("HMI_bPickArmG1VacSol"), &m_stGlass1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bPickArmWeakBlowSol"), &m_stWeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bPickArmCOF1VacSol"), &m_stCOF1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bPickArmCOF2VacSol"), &m_stCOF2VacSol.bHMI_Status);

		RegVariable(_T("HMI_bPickArmG2VacSol"), &m_stGlass2VacSol.bHMI_Status);

		RegVariable(_T("HMI_bStandAlone"), &HMI_bStandAlone);
		RegVariable(GetStation() + _T("lPickPlaceRetryLimit"), &m_lPickPlaceRetryLimit);
		RegVariable(GetStation() + _T("bInlineInBack"), &m_bInlineInBack);

		RegVariable(GetStation() + _T("dCycleTime"), &m_dCycleTime);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnX"), HMI_PowerOnX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommX"), HMI_CommX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeX"), HMI_HomeX);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleCOF1VacSol"), HMI_ToggleCOF1VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleCOF2VacSol"), HMI_ToggleCOF2VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG1VacSol"), HMI_ToggleG1VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlowSol"), HMI_ToggleWeakBlowSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG2VacSol"), HMI_ToggleG2VacSol); 

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendUpStreamInLine"), HMI_ToggleSendUpStreamInLine);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendReady"), HMI_ToggleSendReady);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendStageMotion"), HMI_ToggleSendStageMotion);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendStageInPosition"), HMI_ToggleSendStageInPosition);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendStageVacOn"), HMI_ToggleSendStageVacOn);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSending"), HMI_ToggleSending);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendG1Exist"), HMI_ToggleSendG1Exist);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendG2Exist"), HMI_ToggleSendG2Exist);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleSendCancel"), HMI_ToggleSendCancel);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleInterLockBack"), HMI_ToggleInterLockBack);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("TogglePAEMO"), HMI_TogglePAEMO);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleInlineInBack"), HMI_ToggleInlineInBack);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetStandAlone"), HMI_SetStandAlone);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXPos"), HMI_IndexXPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXNeg"), HMI_IndexXNeg);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPickPlaceRetryLimit"), HMI_SetPickPlaceRetryLimit);

		// -------------------------------------------------------
		// varibales
		// -------------------------------------------------------
	
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PickGlassOperation"), HMI_PickGlassOperation);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PlaceGlassOperation"), HMI_PlaceGlassOperation);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPlaceGlassPrority"), HMI_SetPlaceGlassPrority);

		//Test

	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

INT	CPickArm::SetSendUpStreamInLine(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendUpStreamInLine;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSendReady(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendReady;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSendStageMotion(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendStageMotion;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSendStageInPosition(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendStageInPosition;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSendStageVacOn(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendStageVacOn;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSending(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSending;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSendG1Exist(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendG1Exist;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSendG2Exist(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendG2Exist;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetSendCancel(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stSendCancel;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetInterLockBack(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stInterLockBack;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

INT	CPickArm::SetPAEMO(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stPAEMO;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return nResult;
}

LONG CPickArm::HMI_ToggleSendUpStreamInLine(IPC_CServiceMessage &svMsg)
{
	if (m_stSendUpStreamInLine.bHMI_Status)
	{
		SetSendUpStreamInLine(OFF);
	}
	else
	{
		SetSendUpStreamInLine(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSendReady(IPC_CServiceMessage &svMsg)
{
	if (m_stSendReady.bHMI_Status)
	{
		SetSendReady(OFF);
	}
	else
	{
		SetSendReady(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSendStageMotion(IPC_CServiceMessage &svMsg)
{
	if (m_stSendStageMotion.bHMI_Status)
	{
		SetSendStageMotion(OFF);
	}
	else
	{
		SetSendStageMotion(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSendStageInPosition(IPC_CServiceMessage &svMsg)
{
	if (m_stSendStageInPosition.bHMI_Status)
	{
		SetSendStageInPosition(OFF);
	}
	else
	{
		SetSendStageInPosition(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSendStageVacOn(IPC_CServiceMessage &svMsg)
{
	if (m_stSendStageVacOn.bHMI_Status)
	{
		SetSendStageVacOn(OFF);
	}
	else
	{
		SetSendStageVacOn(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSending(IPC_CServiceMessage &svMsg)
{
	if (m_stSending.bHMI_Status)
	{
		SetSending(OFF);
	}
	else
	{
		SetSending(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSendG1Exist(IPC_CServiceMessage &svMsg)
{
	if (m_stSendG1Exist.bHMI_Status)
	{
		SetSendG1Exist(OFF);
	}
	else
	{
		SetSendG1Exist(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSendG2Exist(IPC_CServiceMessage &svMsg)
{
	if (m_stSendG2Exist.bHMI_Status)
	{
		SetSendG2Exist(OFF);
	}
	else
	{
		SetSendG2Exist(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleSendCancel(IPC_CServiceMessage &svMsg)
{
	if (m_stSendCancel.bHMI_Status)
	{
		SetSendCancel(OFF);
	}
	else
	{
		SetSendCancel(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleInterLockBack(IPC_CServiceMessage &svMsg)
{
	if (m_stInterLockBack.bHMI_Status)
	{
		SetInterLockBack(OFF);
	}
	else
	{
		SetInterLockBack(ON);
	}

	return 0;
}

LONG CPickArm::HMI_TogglePAEMO(IPC_CServiceMessage &svMsg)
{
	if (m_stPAEMO.bHMI_Status)
	{
		SetPAEMO(OFF);
	}
	else
	{
		SetPAEMO(ON);
	}

	return 0;
}

LONG CPickArm::HMI_ToggleInlineInBack(IPC_CServiceMessage &svMsg)
{
	m_bInlineInBack = !m_bInlineInBack;
	return 0;
}
/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CPickArm::PreStartOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	m_bModError = FALSE;
	m_bGlass1Exist = FALSE;
	m_bGlass2Exist = FALSE;
	m_bIsVacError = FALSE; //20121224 vac error

	if (!m_bModSelected)
	{	
		PICKARM_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = PICKARM_STOP_Q;
		m_bStopCycle = FALSE;
	}
	else
	{
		if (
			((m_nLastError = SetWeakBlowSol(OFF)) != GMP_SUCCESS) || 
			((m_nLastError = MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT)) != GMP_SUCCESS)
			)
		{
			Command(glSTOP_COMMAND, TRUE);
			PICKARM_Status = ST_STOP;
			m_qSubOperation = PICKARM_STOP_Q;
			m_bStopCycle = TRUE;
		}
		else
		{

			if (!IsGLASS1_VacSensorOn())
			{
				SetVacSol(GLASS1, OFF);
				m_bGlass1Exist = FALSE;
			}


			if (!IsGLASS2_VacSensorOn())
			{
				SetVacSol(GLASS2, OFF);
				m_bGlass2Exist = FALSE;
			}
			PICKARM_Status = ST_IDLE;
			HS_ClearAllHandShakeSignal();
			SetHandShakeBit(&m_stSendUpStreamInLine, ON);
			SetHandShakeBit(&m_stSendReady, ON);

			m_qSubOperation = PICKARM_IDLE_Q;
			m_bStopCycle = FALSE;
			m_lPickPlaceRetryCounter = 0;
		}
	}

	// cycle time
	m_dwMachineTime = GetTickCount();
	m_dwStartTime = GetTickCount();
	for (m_nCycleCounter = 0; m_nCycleCounter < 10; m_nCycleCounter++)
	{
		m_dTempCycleTime[m_nCycleCounter] = 0;
	}
	m_nCycleCounter = 0;
	m_dCycleTime = 0.0;

	Motion(FALSE);
	State(IDLE_Q);

	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), PICKARM_Status, m_bGlass1Exist, m_bGlass2Exist);

	CPI9000Stn::PreStartOperation();
}

VOID CPickArm::AutoOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bStopCycle = TRUE;
	}

//	OperationSequence();

	// cycle completed or motor error or abort requested: end operation immediately
	if (
		(m_bModSelected && m_qSubOperation == PICKARM_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			if (PICKARM_Status != ST_VAC_ERROR && (HMI_bStandAlone || IsHandShakeBitOn(m_stInterLockFront)))	// 20160615
			{
				MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
			}

			if (AutoMode == BURN_IN) //20130620
			{
				if (!IsGLASS1_VacSensorOn())
				{
					SetGLASS1_VacSol(OFF);
				}
			}
			if (AutoMode == BURN_IN)
			{
				if (!IsGLASS2_VacSensorOn())
				{
					SetGLASS2_VacSol(OFF);
				}
			}
		}

		if (GMP_SUCCESS != m_nLastError)
		{
			m_bModError = TRUE;
		}

		
		PICKARM_Status = ST_STOP;
		HS_ClearAllHandShakeSignal();
		SetHandShakeBit(&m_stSendReady, OFF);
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

		CPI9000Stn::StopOperation();
	}
}

/////////////////////////////////////////////////////////////////
//Operational Sequences
/////////////////////////////////////////////////////////////////
CString CPickArm::GetSequenceString()
{
	switch (m_qSubOperation)
	{
	case PICKARM_IDLE_Q:
		return "PICKARM_IDLE_Q";
		
	case PICKARM_WAIT_BUFFER_UNLOAD_Q:
		return "PICKARM_WAIT_BUFFER_UNLOAD_Q";

	case PICKARM_PICK_GLASS_FROM_BUFFER_Q:
		return "PICKARM_PICK_GLASS_FROM_BUFFER_Q";

	case PICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q:
		return "PICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q";

	case PICKARM_WAIT_INSPWH_UNLOAD_Q:
		return "PICKARM_WAIT_INSPWH_UNLOAD_Q";

	case PICKARM_PICK_GLASS_FROM_INSPWH_Q:
		return "PICKARM_PICK_GLASS_FROM_INSPWH_Q";

	case PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q:
		return "PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q";

	case PICKARM_UNLOAD_TO_BUFFER_WH_Q:
		return "PICKARM_UNLOAD_TO_BUFFER_WH_Q";

	case PICKARM_PLACE_GLASS_TO_BUFFER_Q:
		return "PICKARM_PLACE_GLASS_TO_BUFFER_Q";
		
	case PICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q:
		return "PICKARM_CHEKC_PLACE_BUFFER_GLASS_OK_Q";
		
	case PICKARM_UNLOAD_TO_FLIPPER_WH_Q:
		return "PICKARM_UNLOAD_TO_FLIPPER_WH_Q";

	case PICKARM_WAIT_FLIPPER_WH_ACK_Q:
		return "PICKARM_WAIT_FLIPPER_WH_ACK_Q";
		
	case PICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q:
		return "PICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q";
		
	case PICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q:
		return "PICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q";

	case PICKARM_CHECK_PLACE_GLASS_Q:
		return "PICKARM_CHECK_PLACE_GLASS_Q";
		
	case PICKARM_STOP_Q:
		return "PICKARM_STOP_Q";
		
	}

	return "Hello World!!";
}

//VOID CPickArm::OperationSequence()
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//
//	m_nLastError = GMP_SUCCESS;
//	PRINT_DATA	stPrintData;
//
//
//	CString szMsg = "";
//
//	switch (m_qSubOperation)
//	{
//		case PICKARM_IDLE_Q:		//0
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if (!IsGLASS1_VacSensorOn() && !IsGLASS2_VacSensorOn())   //no Glass 
//			{
//				m_bGlass1Exist = FALSE;
//				m_bGlass2Exist = FALSE;
//
//				if (		
//					((m_nLastError = MoveX(PICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
//					)
//				{
//					PICKARM_Status = ST_EMPTY;
//					m_qSubOperation = PICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
//				}
//			}
//			else  // Glass Exist
//			{
//				if (IsGLASS1_VacSensorOn())
//				{
//					m_bGlass1Exist = TRUE;
//				}
//
//				if (IsGLASS2_VacSensorOn())
//				{
//					m_bGlass2Exist = TRUE;
//				}
//
//				PICKARM_Status = ST_BUSY;
//				m_qSubOperation = PICKARM_UNLOAD_TO_BUFFER_WH_Q;		// go to 7
//
//			}
//
//			break;
//			
//		case PICKARM_WAIT_BUFFER_UNLOAD_Q:		//1
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if (BUFFERWH_Status == ST_UNLOAD_REQ)  
//			{
//				if ((m_nLastError = MoveX(PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
//				{
//					PICKARM_Status = ST_BUF_UNLOAD_ACK;
//					PPAction = AssignPickFromWhere_ShBuf();
//					m_qSubOperation = PICKARM_PICK_GLASS_FROM_BUFFER_Q;		// go to 2
//				}
//				else
//				{
//					PICKARM_Status = ST_ERROR;
//					m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//					m_bModError = TRUE;
//				}
//			}
//			else
//			{
//				m_qSubOperation = PICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
//			}
//
//			break;
//
//		case PICKARM_PICK_GLASS_FROM_BUFFER_Q:		//2
//
//			if ((m_nLastError = PickGlassFromBuffer(PPAction)) == GMP_SUCCESS)
//			{
//				m_qSubOperation = PICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q;		// go to 3
//			}
//
//			break;
//
//		case PICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q:		//3
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if (IsPickGlassFromBuffer_OK(PPAction))	// Glass Exist
//			{
//				PICKARM_Status = ST_DONE;
//				m_lPickPlaceRetryCounter = 0;
//
//				m_qSubOperation = PICKARM_UNLOAD_TO_BUFFER_WH_Q;		// go to 7
//			}
//			else	//Pick Error
//			{
//				//retry sequence
//				if (m_lPickPlaceRetryLimit != 0)
//				{
//					m_lPickPlaceRetryCounter++;
//
//					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
//					{
//						// ERROR!
//						PICKARM_Status = ST_ERROR;
//						m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//						DisplayMessage("PICK GLASS ERROR: Pick Buffer Glass Retry Limit Hit");
//						m_bModError = TRUE;
//						SetError(IDS_PICKARM_PICK_BUFFER_ERR);
//						if (PPAction != GLASS2)
//						{
//							if (!IsGLASS1_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_GLASS1_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS1 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//							if (m_bUseCOFVacuum && !IsCOF1_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_COF1_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF1 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//						}
//						if (PPAction != GLASS1)
//						{
//							if(!IsGLASS2_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_GLASS2_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS2 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//							if (m_bUseCOFVacuum && !IsCOF2_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_COF2_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF2 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//						}
//					}
//					else 
//					{
//						m_qSubOperation = PICKARM_PICK_GLASS_FROM_BUFFER_Q;		// go to 2
//					}
//				}
//				else 
//				{
//					m_qSubOperation = PICKARM_PICK_GLASS_FROM_BUFFER_Q;		// go to 2
//				}
//			}
//
//			break;
//
//		case PICKARM_WAIT_INSPWH_UNLOAD_Q:		//4
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if (INSPWH_Status == ST_UNLOAD_REQ)  
//			{
//				PICKARM_Status = ST_INSPWH_UNLOAD_ACK;
//				PPAction = AssignPickFromWhere();
//				m_qSubOperation = PICKARM_PICK_GLASS_FROM_INSPWH_Q;		// go to 5
//			}
//			else
//			{
//				m_qSubOperation = PICKARM_WAIT_BUFFER_UNLOAD_Q;		// go to 1
//			}
//
//			break;
//
//		case PICKARM_PICK_GLASS_FROM_INSPWH_Q:		//5
//
//			if (
//				((m_nLastError = PickGlassFromInspWH(PPAction)) == GMP_SUCCESS)
//				)
//			{
//				m_qSubOperation = PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q;		// go to 6
//			}
//
//			break;
//
//		case PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q:		//6
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if (IsPickGlassFromInspWH_OK(PPAction) || AutoMode == BURN_IN )	// Glass Exist
//			{
//				PICKARM_Status = ST_DONE;
//				m_lPickPlaceRetryCounter = 0;
//
//				m_qSubOperation = PICKARM_UNLOAD_TO_BUFFER_WH_Q;		// go to 7
//			}
//			else	//Pick Error
//			{
//				//retry sequence
//				if (m_lPickPlaceRetryLimit != 0)
//				{
//					m_lPickPlaceRetryCounter++;
//
//					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
//					{
//						// ERROR!
//						PICKARM_Status = ST_ERROR;
//						m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//						DisplayMessage("PICK GLASS ERROR: Pick InspWH Glass Retry Limit Hit");
//						m_bModError = TRUE;
//						SetError(IDS_PICKARM_PICK_INSPWH_ERR);
//						if (PPAction != GLASS2)
//						{
//							if (!IsGLASS1_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_GLASS1_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS1 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//							if (m_bUseCOFVacuum && !IsCOF1_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_COF1_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF1 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//						}
//						if (PPAction != GLASS1)
//						{
//							if(!IsGLASS2_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_GLASS2_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS2 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//							if (m_bUseCOFVacuum && !IsCOF2_VacSensorOn())
//							{
//								DisplayMessage("*********************************************");
//								SetError(IDS_PICKARM_COF2_VAC_ERR);
//								szMsg.Format("PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF2 VAC ERROR");
//								DisplayMessage(szMsg);
//								DisplayMessage("*********************************************");
//							}
//						}
//					}
//					else 
//					{
//						m_qSubOperation = PICKARM_PICK_GLASS_FROM_INSPWH_Q;		// go to 5
//					}
//				}
//				else 
//				{
//					m_qSubOperation = PICKARM_PICK_GLASS_FROM_INSPWH_Q;		// go to 5
//				}
//			}
//
//			break;
//
//		case PICKARM_UNLOAD_TO_BUFFER_WH_Q:		//7
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if ((BUFFERWH_Status == ST_LOAD_REQ) && IsVacSensorOn(GLASS1) && IsVacSensorOn(GLASS2))
//			{
//				if (
//					((m_nLastError = MoveX(PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS) //BUFFER WH
//					)
//				{
//					PICKARM_Status = ST_BUF_LOAD_ACK;
//					m_qSubOperation = PICKARM_PLACE_GLASS_TO_BUFFER_Q;		// go to 8
//				}
//			}
//			else
//			{
//				if (
//					((m_nLastError = MoveX(PICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS) //Flipper WH
//					)
//				{
//					PICKARM_Status = ST_BUSY;
//					m_qSubOperation = PICKARM_UNLOAD_TO_FLIPPER_WH_Q;		// go to 10
//				}
//			}
//			
//			break;
//
//		case PICKARM_PLACE_GLASS_TO_BUFFER_Q:		//8
//
//			if (
//				((m_nLastError = PlaceGlassToBuffer(MAX_NUM_OF_GLASS)) == GMP_SUCCESS)  // Both Glasses
//				)
//			{
//				SetVacSol(MAX_NUM_OF_GLASS, ON, GMP_WAIT);	//Vac ON
//				m_qSubOperation = PICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q;		// go to 9
//			}
//
//			break;
//			
//		case PICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q:		//9
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if (IsPlaceGlassToBuffer_OK())	// Arm Empty
//			{
//				PICKARM_Status = ST_EMPTY;
//				m_lPickPlaceRetryCounter = 0;
//				SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_WAIT);	//Vac OFF
//
//				if (
//					((m_nLastError = MoveX(PICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
//					)
//				{
//					m_qSubOperation = PICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
//				}
//			}
//			else	//Pick Error
//			{
//				//retry sequence
//				if (m_lPickPlaceRetryLimit != 0)
//				{
//					m_lPickPlaceRetryCounter++;
//
//					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
//					{
//						// ERROR!
//						PICKARM_Status = ST_ERROR;
//						m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//						DisplayMessage("PLACE GLASS ERROR: Place Buffer Glass Retry Limit Hit");
//						m_bModError = TRUE;
//						//SetError(IDS_PICKARM_PLACE_BUFFER_ERR);	// set error at ShBuffer
//					}
//					else 
//					{
//						m_qSubOperation = PICKARM_PLACE_GLASS_TO_BUFFER_Q;		// go to 8
//					}
//				}
//				else 
//				{
//					m_qSubOperation = PICKARM_PLACE_GLASS_TO_BUFFER_Q;		// go to 8
//				}
//			}
//
//			break;
//			
//		case PICKARM_UNLOAD_TO_FLIPPER_WH_Q:		//10
//			
//			if (IsUnloadToFlipperWH())
//			{
//				HS_PickArmSelectPanelEnable();
//				HS_PickArmSendPanelReady();
//				m_qSubOperation = PICKARM_WAIT_FLIPPER_WH_ACK_Q;		// go to 11
//
//			}
//			else		//Place on OutBelt
//			{
//				if (
//					((m_nLastError = PlaceGlassOnOutBelt()) == GMP_SUCCESS) //&&
//					//((m_nLastError = MoveX(PICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
//					)
//				{
//					m_qSubOperation = PICKARM_CHECK_PLACE_GLASS_Q;	//14
//					//PICKARM_Status = ST_EMPTY;
//					//m_qSubOperation = PICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
//				}
//			}
//
//			break;
//
//		case PICKARM_WAIT_FLIPPER_WH_ACK_Q:		//11
//
//			//if (m_bStopCycle)
//			//{
//			//	m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			//}
//			//else 
//			if (HS_IsHandshakeCanel())
//			{
//				// addcode Cancel error
//				HS_ClearAllHandShakeSignal();
//				m_qSubOperation = PICKARM_IDLE_Q;	// go to 0
//			}
//			else if (HS_IsFlipperSendPanelReadyACK())
//			{
//				m_qSubOperation = PICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q;		// go to 12
//			}
//
//			break;
//			
//		case PICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q:		//12
//
//			//if (m_bStopCycle)
//			//{
//			//	HS_ClearAllHandShakeSignal();
//			//	m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			//}
//			//else 
//			if (HS_IsHandshakeCanel())
//			{
//				HS_ClearAllHandShakeSignal();
//				m_qSubOperation = PICKARM_UNLOAD_TO_FLIPPER_WH_Q;		// go to 10
//			}
//			else if (HS_IsFlipperRequestArmVacOff())
//			{
//				SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_NOWAIT);
//				SetWeakBlowSol(ON, GMP_WAIT);
//				//SetWeakBlowSol(OFF, GMP_NOWAIT);	// Turn off weak blow after WH Z back to standby level.
//				HS_SetPanelDisabled();
//				m_qSubOperation = PICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q;		// go to 13
//			}
//
//			break;
//			
//		case PICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q:		//13
//			//
//			//if (m_bStopCycle)
//			//{
//			//	HS_ClearAllHandShakeSignal();
//			//	m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			//}
//			//else 
//			if (HS_IsHandshakeCanel())
//			{
//				HS_ClearAllHandShakeSignal();
//				m_qSubOperation = PICKARM_IDLE_Q;	// go to 0
//			}
//			else if (HS_IsFlipperReceivedPanel())
//			{
//				SetWeakBlowSol(OFF, GMP_NOWAIT);	// Turn off weak blow after WH Z back to standby level.
//				HS_PickArmSendPanelNotReady();
//				m_qSubOperation = PICKARM_CHECK_PLACE_GLASS_Q;	//14
//			}
//			break;
//			
//		case PICKARM_CHECK_PLACE_GLASS_Q:	//14
//			if (m_bStopCycle)
//			{
//				HS_ClearAllHandShakeSignal();
//				m_qSubOperation = PICKARM_STOP_Q;	// go to 99
//			}
//			else if (CheckPlaceStatus())
//			{
//				TC_ResetTime();
//				m_dTempCycleTime[m_nCycleCounter] = (DOUBLE)(GetTickCount() - m_dwStartTime) / 1000.0;
//
//				if (m_bGlass1Exist && m_bGlass2Exist)
//				{
//					m_dTempCycleTime[m_nCycleCounter] = m_dTempCycleTime[m_nCycleCounter] / 2;
//				}
//
//				if (
//					m_dTempCycleTime[0] == 0.0 || 
//					m_dTempCycleTime[1] == 0.0 || 
//					m_dTempCycleTime[2] == 0.0 || 
//					m_dTempCycleTime[3] == 0.0 || 
//					m_dTempCycleTime[4] == 0.0 || 
//					m_dTempCycleTime[5] == 0.0 || 
//					m_dTempCycleTime[6] == 0.0 || 
//					m_dTempCycleTime[7] == 0.0 || 
//					m_dTempCycleTime[8] == 0.0 || 
//					m_dTempCycleTime[9] == 0.0 
//					)
//				{
//					m_dCycleTime = (m_dTempCycleTime[0] + 
//						m_dTempCycleTime[1] + 
//						m_dTempCycleTime[2] + 
//						m_dTempCycleTime[3] + 
//						m_dTempCycleTime[4] + 
//						m_dTempCycleTime[5] + 
//						m_dTempCycleTime[6] + 
//						m_dTempCycleTime[7] + 
//						m_dTempCycleTime[8] + 
//						m_dTempCycleTime[9]) / (m_nCycleCounter + 1);
//				}
//				else
//				{
//					m_dCycleTime = (m_dTempCycleTime[0] + 
//						m_dTempCycleTime[1] + 
//						m_dTempCycleTime[2] + 
//						m_dTempCycleTime[3] + 
//						m_dTempCycleTime[4] + 
//						m_dTempCycleTime[5] + 
//						m_dTempCycleTime[6] + 
//						m_dTempCycleTime[7] + 
//						m_dTempCycleTime[8] + 
//						m_dTempCycleTime[9]) / 10;
//				}
//				if (m_nCycleCounter < 9)
//				{
//					m_nCycleCounter++;
//				}
//				else
//				{
//					m_nCycleCounter = 0;
//				}
//				m_dwStartTime = GetTickCount();
//				pAppMod->m_dUPH = (DOUBLE)(pAppMod->m_ulUnitsInpected / (DOUBLE)(GetTickCount() - m_dwMachineTime)) * 60.0 * 60.0 * 1000.0;
//
//				m_lPickPlaceRetryCounter = 0;
//
//				m_bGlass1Exist = FALSE;
//				m_bGlass2Exist = FALSE;
//				PICKARM_Status = ST_EMPTY;
//				m_qSubOperation = PICKARM_IDLE_Q;		// go to 0
//
//			}
//			else
//			{
//				PICKARM_Status = ST_VAC_ERROR;
//				m_bModError = TRUE;
//				DisplayMessage("*********************************************");
//				SetError(IDS_PICKARM_PLACE_OUT_ERR);
//				szMsg.Format("PICKARM_CHECK_PLACE_GLASS_Q: PICKARM PLACE OUT ERROR");
//				DisplayMessage(szMsg);
//				DisplayMessage("*********************************************");
//
//				m_qSubOperation = PICKARM_STOP_Q;
//			}
//			break;
//
//		case PICKARM_STOP_Q:		//99
//			break;
//		
//	}
//
//
//	if (m_qPreviousSubOperation != m_qSubOperation)
//	{
//		if (HMI_bDebugSeq)
//		{
//			CString szMsg;
//			if (PICKARM_STOP_Q != m_qSubOperation)
//			{
//				szMsg.Format("PICKARM Go State: %ld", m_qSubOperation);
//			}
//			else
//			{
//				szMsg.Format("PICKARM Go Stop");
//			}
//			//DisplayMessage(szMsg);
//		}
//		UpdateDebugSeq(GetSequenceString(), PICKARM_Status, m_bGlass1Exist, m_bGlass2Exist);
//		m_qPreviousSubOperation = m_qSubOperation;
//
//		if (HMI_bDebugSeq)
//		{
//			for (INT i = 0; i < MAX_PRINT_DATA; i++)
//			{
//				stPrintData.szTitle[i]	= "";
//			}
//
//			stPrintData.szTitle[0].Format("%ld", GetTickCount());
//			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
//			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", PICKARM_Status);
//
//			LogFile->log(stPrintData.szTitle);
//		}
//
//	}
//
//}

INT CPickArm::PickGlassFromBuffer(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	LONG lDelay;
	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{                
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
	SetVacSol(lGlassNum, ON);

	if ((m_nLastError = pCShBufferWH->SetYInSol(ON, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = max(max(pCShBufferWH->m_stGlass1VacSol.GetOffDelay(), m_stGlass1VacSol.GetOnDelay()), m_stCOF1VacSol.GetOnDelay());
		pCShBufferWH->SetVacSol(lGlassNum, OFF);
		Sleep(lDelay);

		if (pCShBufferWH->m_stWeakBlowSol.GetOnDelay() > 0)
		{
				pCShBufferWH->SetWeakBlowSol(ON, GMP_WAIT);
				pCShBufferWH->SetWeakBlowSol(OFF, GMP_WAIT);
		}
		return m_nLastError = pCShBufferWH->SetYInSol(OFF, GMP_WAIT); 

	}

	return GMP_FAIL;
}

INT CPickArm::PickGlassFromInspWH(GlassNum lGlassNum)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	LONG lDelay;

	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{                
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
	SetVacSol(lGlassNum, ON);

	//if ((m_nLastError = pCInspWH->MoveZToUnloadLevel(lGlassNum, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = max(max(pCInspWH->m_stGlass1VacSol.GetOffDelay(), m_stGlass1VacSol.GetOnDelay()), m_stCOF1VacSol.GetOnDelay());
		pCInspWH->SetVacSol(lGlassNum, OFF);
		Sleep(lDelay);
		if (pCInspWH->m_stWeakBlowSol.GetOnDelay() > 0)
		{
			switch (lGlassNum)
			{
			case GLASS1:
				pCInspWH->SetWeakBlowSol(ON, GMP_WAIT);
				pCInspWH->SetWeakBlowSol(OFF, GMP_WAIT);
				break;

			case GLASS2:
				pCInspWH->SetWeakBlow2Sol(ON, GMP_WAIT);
				pCInspWH->SetWeakBlow2Sol(OFF, GMP_WAIT);
				break;

			case MAX_NUM_OF_GLASS:
				pCInspWH->SetWeakBlowSol(ON, GMP_NOWAIT);
				pCInspWH->SetWeakBlow2Sol(ON, GMP_WAIT);
				pCInspWH->SetWeakBlowSol(OFF, GMP_NOWAIT);
				pCInspWH->SetWeakBlow2Sol(OFF, GMP_WAIT);
				break;
			}
		}
		//return m_nLastError = pCInspWH->MoveZToStandbyLevel(lGlassNum, GMP_WAIT); 
	}

	return GMP_FAIL;
}

INT CPickArm::PlaceGlassToBuffer(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	LONG lDelay;
	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{                
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
	pCShBufferWH->SetVacSol(lGlassNum, ON);

	if ((m_nLastError = pCShBufferWH->MoveY(Y_CTRL_IN_POSN, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = max(max(pCShBufferWH->m_stGlass1VacSol.GetOnDelay(), m_stGlass1VacSol.GetOffDelay()), m_stCOF1VacSol.GetOffDelay());
		SetVacSol(lGlassNum, OFF);
		Sleep (lDelay);

		if (m_stWeakBlowSol.GetOnDelay() > 0)
		{
			SetWeakBlowSol(ON, GMP_WAIT);
			SetWeakBlowSol(OFF, GMP_WAIT);
		}
		m_nLastError = pCShBufferWH->MoveY(Y_CTRL_OUT_POSN, GMP_WAIT);
	}


	return m_nLastError;
}

INT CPickArm::PlaceGlassOnOutBelt()
{
	INT nResult = GMP_FAIL;

	if (
		((m_nLastError = SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_WAIT)) == GMP_SUCCESS) &&
		((m_nLastError = SetWeakBlowSol(ON, GMP_WAIT)) == GMP_SUCCESS) &&
		((m_nLastError = SetWeakBlowSol(OFF, GMP_WAIT)) == GMP_SUCCESS)
		)
	{
		nResult = GMP_SUCCESS;
	}

	return nResult;
}

BOOL CPickArm::IsPickGlassFromBuffer_OK(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	BOOL bResult = TRUE;
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCShBufferWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			bResult = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCShBufferWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			bResult = FALSE;
		}
	}
	return bResult;
}

BOOL CPickArm::IsPickGlassFromInspWH_OK(GlassNum lGlassNum)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	BOOL bResult = TRUE;
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCInspWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			bResult = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCInspWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			bResult = FALSE;
		}
	}
	return bResult;
}

BOOL CPickArm::IsPlaceGlassToBuffer_OK()	//both Glass
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	BOOL bResult = FALSE;
	if (pCShBufferWH->IsVacSensorOn(GLASS1) && pCShBufferWH->IsVacSensorOn(GLASS1))
	{
		bResult = TRUE;
	}
	else
	{
		SetVacSol(MAX_NUM_OF_GLASS, ON);
	}
	return bResult;
}

BOOL CPickArm::IsUnloadToFlipperWH()	//use OutBelt, when FlipperWH is not available
{
//	BOOL bResult = TRUE;
	return m_bInlineInBack;
}


VOID CPickArm::HS_PickArmSelectPanelEnable()
{
	if (IsGLASS1_VacSensorOn())
	{
		SetHandShakeBit(&m_stSendG1Exist, ON);
	}
	
	if (IsGLASS2_VacSensorOn())
	{
		SetHandShakeBit(&m_stSendG2Exist, ON);
	}
}
 
VOID CPickArm::HS_PickArmSendPanelReady()
{
	SetHandShakeBit(&m_stSending, ON);
}

BOOL CPickArm::HS_IsFlipperSendPanelReadyACK()	
{
	BOOL bResult = FALSE;
	
		bResult = IsHandShakeBitOn(m_stRecieving);

	return bResult;
}

BOOL CPickArm::HS_IsFlipperRequestArmVacOff()	
{
	BOOL bResult = FALSE;

		bResult = (IsHandShakeBitOn(m_stRecieveG1VacOff) || IsHandShakeBitOn(m_stRecieveG2VacOff));

	return bResult;
}

VOID CPickArm::HS_SetPanelDisabled()
{

	SetHandShakeBit(&m_stSendG1Exist, OFF);
	SetHandShakeBit(&m_stSendG2Exist, OFF);

}

BOOL CPickArm::HS_IsFlipperReceivedPanel()	
{
	BOOL bResult = FALSE;
		
		bResult = !IsHandShakeBitOn(m_stRecieving);
	
	return bResult;
}

VOID CPickArm::HS_PickArmSendPanelNotReady()
{

	SetHandShakeBit(&m_stSending, OFF);
}

BOOL CPickArm::HS_IsHandshakeCanel()	
{
	BOOL bResult = FALSE;

	if (IsHandShakeBitOn(m_stRecieveCancel) || !IsHandShakeBitOn(m_stRecieveReady) || !IsHandShakeBitOn(m_stRecieveUpStreamInLine))
	{
		bResult = TRUE;
	}

	return bResult;
}


VOID CPickArm::HS_ClearAllHandShakeSignal()
{

	SetHandShakeBit(&m_stSending, OFF);
	SetHandShakeBit(&m_stSendG1Exist, OFF);
	SetHandShakeBit(&m_stSendG2Exist, OFF);

}

INT CPickArm::SetHandShakeBit(CSoInfo *stTempSo, BOOL bMode)
{
	INT nResult = GMP_SUCCESS;

//	CSoInfo *stTempSo = &m_stWeakBlowSol;

	if (IsBurnInDiagBond())
	{
		Sleep(100);
		stTempSo->bHMI_Status = bMode;
		return GMP_SUCCESS;
	}

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

				stTempSo->GetGmpPort().On();
				stTempSo->bHMI_Status = TRUE;

			TC_DelJob(stTempSo->GetTCObjectID());
		}
		else
		{
			TC_AddJob(stTempSo->GetTCObjectID());

			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;

			TC_DelJob(stTempSo->GetTCObjectID());
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return nResult;
}	

BOOL CPickArm::IsHandShakeBitOn(CSiInfo stTempSi)
{
	BOOL bResult = FALSE;

	try
	{
		bResult = stTempSi.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPickArm::PickGlassFromInspWHOperation(GlassNum lGlassNum)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult = TRUE;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCInspWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCShBufferWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCShBufferWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (lGlassNum != NO_GLASS)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Contact Software");
		return FALSE;
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		SetVacSol((GlassNum)i, ON);
		pCInspWH->SetVacSol((GlassNum)i, ON);
	}
	Sleep(GetMaxValue(m_stGlass1VacSol.GetOnDelay(), pCInspWH->m_stGlass1VacSol.GetOnDelay()));

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		bWHGlassStatus[i] = pCInspWH->IsVacSensorOn((GlassNum)i);
		bArmGlassStatus[i] = IsVacSensorOn((GlassNum)i);

		if (!pCInspWH->IsVacSensorOn((GlassNum)i))
		{
			pCInspWH->SetVacSol((GlassNum)i, OFF);
		}

		if (!IsVacSensorOn((GlassNum)i))
		{
			SetVacSol((GlassNum)i, OFF);
		}
	}

	//if (pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	strMsg.Format("InspWH Move To Standby Level Error. Operation Abort!");
	//	HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
	//	return FALSE;
	//}
	//if (pCShBufferWH->SetZLoadSol(OFF, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return FALSE;
	//}
	if ((lGlassNum = CalcPickStatus(bWHGlassStatus, bArmGlassStatus)) == NO_GLASS)
	{
		strMsg.Format("Invalid Glass Status at %s and %s", GetStnName(), pCInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCInspWH->MoveXYToUnloadPosn() != GMP_SUCCESS)
	{
		strMsg.Format("InspWH Move To Unload Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (MoveX(PICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Move To Pick Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	if (PickGlassFromInspWH(lGlassNum) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Pick Glass From InspWH Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCInspWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass1 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCInspWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass2 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (bResult)
	{
		if (MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("PickArm Pick Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}
	return bResult;
}

BOOL CPickArm::PickGlassFromShbufOperation(GlassNum lGlassNum)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult = TRUE;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCInspWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCShBufferWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCShBufferWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (lGlassNum != NO_GLASS)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Contact Software");
		return FALSE;
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		SetVacSol((GlassNum)i, ON);
		pCShBufferWH->SetVacSol((GlassNum)i, ON);
	}
	Sleep(GetMaxValue(m_stGlass1VacSol.GetOnDelay(), pCShBufferWH->m_stGlass1VacSol.GetOnDelay()));

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		bWHGlassStatus[i] = pCShBufferWH->IsVacSensorOn((GlassNum)i);
		bArmGlassStatus[i] = IsVacSensorOn((GlassNum)i);

		if (!pCShBufferWH->IsVacSensorOn((GlassNum)i))
		{
			pCShBufferWH->SetVacSol((GlassNum)i, OFF);
		}

		if (!IsVacSensorOn((GlassNum)i))
		{
			SetVacSol((GlassNum)i, OFF);
		}
	}
	//if (pCShBufferWH->SetZLoadSol(OFF, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return FALSE;
	//}
	//if (pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	strMsg.Format("InspWH Move To Standby Level Error. Operation Abort!");
	//	HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
	//	return FALSE;
	//}

	if ((lGlassNum = CalcPickStatus(bWHGlassStatus, bArmGlassStatus)) == NO_GLASS)
	{
		strMsg.Format("Invalid Glass Status at %s and %s", GetStnName(), pCShBufferWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCShBufferWH->SetYInSol(ON, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("ShBufferWH Move To Unload Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (MoveX(PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Move To ShBuf Pick Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	if (PickGlassFromBuffer(lGlassNum) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Pick Glass From ShBufferWH Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCInspWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass1 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCInspWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass2 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (bResult)
	{
		if (MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("PickArm Pick Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}
	return bResult;
}

BOOL CPickArm::PlaceGlassToShbufOperation(GlassNum lGlassNum)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult = TRUE;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCInspWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCShBufferWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCShBufferWH->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (lGlassNum != NO_GLASS)
	{
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Contact Software");
		return FALSE;
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		SetVacSol((GlassNum)i, ON);
		pCShBufferWH->SetVacSol((GlassNum)i, ON);
	}
	Sleep(GetMaxValue(m_stGlass1VacSol.GetOnDelay(), pCShBufferWH->m_stGlass1VacSol.GetOnDelay()));

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		bWHGlassStatus[i] = pCShBufferWH->IsVacSensorOn((GlassNum)i);
		bArmGlassStatus[i] = IsVacSensorOn((GlassNum)i);

		if (!pCShBufferWH->IsVacSensorOn((GlassNum)i))
		{
			pCShBufferWH->SetVacSol((GlassNum)i, OFF);
		}

		if (!IsVacSensorOn((GlassNum)i))
		{
			SetVacSol((GlassNum)i, OFF);
		}
	}
	//if (pCShBufferWH->SetZLoadSol(OFF, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return FALSE;
	//}
	//if (pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	strMsg.Format("ShBuffer WH Move To Standby Level Error. Operation Abort!");
	//	HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
	//	return FALSE;
	//}

	if ((lGlassNum = CalcPlaceStatus(bWHGlassStatus, bArmGlassStatus)) == NO_GLASS)
	{
		strMsg.Format("Invalid Glass Status at %s and %s", GetStnName(), pCShBufferWH->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCShBufferWH->SetYInSol(ON, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("ShBuffer WH Move To Load Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (MoveX(PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Move To ShBuffer WH Place Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	if (PlaceGlassToBuffer(lGlassNum) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Place Glass To ShBuffer WH Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (!pCShBufferWH->IsVacSensorOn(GLASS1))
		{
			SetVacSol(GLASS1, ON, GMP_WAIT);
			strMsg.Format("Place Glass Error: %s Glass1 Vac Sensor Not On.", pCShBufferWH->GetStnName());
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{

		if (!pCShBufferWH->IsVacSensorOn(GLASS2))
		{
			SetVacSol(GLASS1, ON, GMP_WAIT);
			strMsg.Format("Place Glass Error: %s Glass2 Vac Sensor Not On.", pCShBufferWH->GetStnName());
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (bResult)
	{
		if (MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("PickArm Place Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}
	return bResult;
}

BOOL CPickArm::PlaceGlassToOutBeltOperation(GlassNum lGlassNum)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult = TRUE;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCInspWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCShBufferWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCShBufferWH->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (lGlassNum != NO_GLASS)
	{
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Contact Software");
		return FALSE;
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		SetVacSol((GlassNum)i, ON, GMP_WAIT);
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		bArmGlassStatus[i] = IsVacSensorOn((GlassNum)i);

		if (!IsVacSensorOn((GlassNum)i))
		{
			SetVacSol((GlassNum)i, OFF);
		}
	}
	if (bArmGlassStatus[GLASS1] && bArmGlassStatus[GLASS2])
	{
		lGlassNum = MAX_NUM_OF_GLASS;
	}
	else if (bArmGlassStatus[GLASS1])
	{
		lGlassNum = GLASS1;
	}
	else if (bArmGlassStatus[GLASS2])
	{
		lGlassNum = GLASS2;
	}
	else
	{
		strMsg.Format("No Glass Exist on Pick Arm. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;

	}
	//if (pCShBufferWH->SetZLoadSol(OFF, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return FALSE;
	//}
	//if (pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	strMsg.Format("ShBuffer WH Move To Standby Level Error. Operation Abort!");
	//	HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
	//	return FALSE;
	//}


	if (MoveX(PICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Move Place Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	if (PlaceGlassOnOutBelt() != GMP_SUCCESS || getKlocworkFalse())
	{
		strMsg.Format("PickArm Place Glass On OutBelt Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		SetVacSol((GlassNum)i, ON, GMP_WAIT);
	}

	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOn(GetGlassCOF1_VacSensorOnState()))
		{
			strMsg.Format("Place Glass Error: Glass 1 Still Exist On Pick Arm.");
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOn(GetGlassCOF2_VacSensorOnState()))
		{
			strMsg.Format("Place Glass Error: Glass 2 Still Exist On Pick Arm");
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}	

	if (bResult)
	{
		if (MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("PickArm Place Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult = FALSE;
		}
	}
	return bResult;
}

BOOL CPickArm::PickGlassOperation()
{
	BOOL bResult = FALSE;
	LONG lResponse = HMISelectionBox("PICK GLASS OPERATION", "Please select Where to pick:", "Insp WH", "ShBuffer WH");	

		if (lResponse == -1 || lResponse == 11)
		{
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
			return FALSE;
		}

		switch (lResponse)
		{
		case 0:
			bResult = PickGlassFromInspWHOperation(NO_GLASS);
			break;

		case 1:
			bResult = PickGlassFromShbufOperation(NO_GLASS);
			break;
		}
	return bResult;
}

BOOL CPickArm::PlaceGlassOperation()
{
	BOOL bResult = FALSE;
	LONG lResponse = HMISelectionBox("PLACE GLASS OPERATION", "Please select Where to Place:", "Flipper WH / Out Belt", "ShBuffer WH");	

		if (lResponse == -1 || lResponse == 11)
		{
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Operation Abort!");
			return FALSE;
		}

		switch (lResponse)
		{
		case 0:
			if (m_bInlineInBack)
			{
				//Place to Flipper WH
			}
			else
			{
				bResult = PlaceGlassToOutBeltOperation(NO_GLASS);
			}
			break;

		case 1:
			bResult = PlaceGlassToShbufOperation(NO_GLASS);
			break;

		}
	return bResult;
}
BOOL CPickArm::IsFlushStop() //20130408
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	if (g_bFlushMode)
	{
		if (AutoMode == BURN_IN || AutoMode == DIAG_BOND)
		{
			switch (g_lGlassInputMode)
			{
			case GLASS_1_INPUT:
				return TRUE;

			case GLASS_2_INPUT:
				return TRUE;

			default:
				if (m_bIsDefaultIndex) // Place Glass 2 First, then Glass 1
				{
					if (!pCInspWH->m_bGlass2Exist)
					{
						return TRUE;
					}
					else if (!pCInspWH->m_bGlass1Exist)
					{
						return FALSE;
					}
					else
					{
						return TRUE;
					}
				}
				else
				{
					if (!pCInspWH->m_bGlass1Exist)
					{
						return TRUE;
					}
					else if (!pCInspWH->m_bGlass2Exist)
					{
						return FALSE;
					}
					else
					{
						return TRUE;
					}
				}
			}
		}
		else
		{
			switch (g_lGlassInputMode)
			{
			case GLASS_1_INPUT:
				return TRUE;

			case GLASS_2_INPUT:
				return TRUE;

			default:
			
					if (m_bIsDefaultIndex) // Place Glass 2 First, then Glass 1
					{
						if (!pCInspWH->IsGLASS2_VacSensorOn())
						{
							return TRUE;
						}
						else if (!pCInspWH->IsGLASS1_VacSensorOn())
						{
							return FALSE;
						}
						else
						{
							return TRUE;
						}
					}
					else 
					{

						if (!pCInspWH->IsGLASS1_VacSensorOn())
						{
							return TRUE;
						}
						else if (!pCInspWH->IsGLASS2_VacSensorOn())
						{
							return FALSE;
						}
						else
						{
							return TRUE;
						}
					}
				break;

			}
		}
	}
	return FALSE;
}

GlassNum CPickArm::AssignPlaceToWhere()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	if (AutoMode == BURN_IN || AutoMode == DIAG_BOND)
	{
		switch (g_lGlassInputMode)
		{
		case GLASS_1_INPUT:

			if (!pCInspWH->m_bGlass1Exist)
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		case GLASS_2_INPUT:

			if (!pCInspWH->m_bGlass2Exist)
			{
				return GLASS2;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		default:
			if (m_bIsDefaultIndex) // Place Glass 2 First, then Glass 1
			{
				if (!pCInspWH->m_bGlass2Exist)
				{
					return GLASS2;
				}
				else if (!pCInspWH->m_bGlass1Exist)
				{
					return GLASS1;
				}
				else
				{
					return NO_GLASS;
				}
			}
			else
			{
				if (!pCInspWH->m_bGlass1Exist)
				{
					return GLASS1;
				}
				else if (!pCInspWH->m_bGlass2Exist)
				{
					return GLASS2;
				}
				else
				{
					return NO_GLASS;
				}
			}
		}
		
	}
	else
	{
		switch (g_lGlassInputMode)
		{
		case GLASS_1_INPUT:

			if (!pCInspWH->IsGLASS1_VacSensorOn())
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		case GLASS_2_INPUT:

			if (!pCInspWH->IsGLASS2_VacSensorOn())
			{
				return GLASS2;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		default:
			if (m_bIsDefaultIndex) // Place Glass 2 First, then Glass 1
			{
				if (!pCInspWH->IsGLASS2_VacSensorOn())
				{
					return GLASS2;
				}
				else if (!pCInspWH->IsGLASS1_VacSensorOn())
				{
					return GLASS1;
				}
				else
				{
					return NO_GLASS;
				}
			}
			else 
			{

				if (!pCInspWH->IsGLASS1_VacSensorOn())
				{
					return GLASS1;
				}
				else if (!pCInspWH->IsGLASS2_VacSensorOn())
				{
					return GLASS2;
				}
				else
				{
					return NO_GLASS;
				}
			}

			break;

		}
	}

	return NO_GLASS;
}

GlassNum CPickArm::AssignPlaceToWhere_edge_cleaner()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	if (AutoMode == BURN_IN || AutoMode == DIAG_BOND)
	{
#if 1
		if (m_bGlass1Exist && m_bGlass2Exist)
		{ 
		//11
			if (!pCInspWH->m_bGlass1Exist && !pCInspWH->m_bGlass2Exist)
			{
				return MAX_NUM_OF_GLASS;
			}
			else
			{
				return NO_GLASS;
			}
		}
		else if (m_bGlass1Exist && !m_bGlass2Exist)
		{ 
		//10
			if (!pCInspWH->m_bGlass1Exist)
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}
		}
		else if (!m_bGlass1Exist && m_bGlass2Exist)
		{ 
		//01
			if (!pCInspWH->m_bGlass2Exist)
			{
				return GLASS2;
			}
			else
			{
				return NO_GLASS;
			}
		}
		else
		{ 
		//00
			return NO_GLASS;
		}
#else
		switch (g_lGlassInputMode)
		{
		case GLASS_1_INPUT:

			if (!pCInspWH->m_bGlass1Exist)
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		case GLASS_2_INPUT:

			if (!pCInspWH->m_bGlass2Exist)
			{
				return GLASS2;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		default:
			if (!pCInspWH->m_bGlass1Exist && !pCInspWH->m_bGlass2Exist)
			{
				return MAX_NUM_OF_GLASS;
			}
			else
			{
				return NO_GLASS;
			}
			break;
		}
#endif
	}
	else
	{
#if 1
		if (m_bGlass1Exist && m_bGlass2Exist)
		{
			if (!pCInspWH->IsGLASS1_VacSensorOn() && !pCInspWH->IsGLASS2_VacSensorOn())
			{
				return MAX_NUM_OF_GLASS;
			}
			else
			{
				return NO_GLASS;
			}
		}
		else if (m_bGlass1Exist)
		{
			if (!pCInspWH->IsGLASS1_VacSensorOn())
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}
		}
		else if (m_bGlass2Exist)
		{
			if (!pCInspWH->IsGLASS2_VacSensorOn())
			{
				return GLASS2;
			}
			else
			{
				return NO_GLASS;
			}
		}
		else //no glass
		{
			return NO_GLASS;
		}
#else
		switch (g_lGlassInputMode)
		{
		case GLASS_1_INPUT:

			if (!pCInspWH->IsGLASS1_VacSensorOn())
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		case GLASS_2_INPUT:

			if (!pCInspWH->IsGLASS2_VacSensorOn())
			{
				return GLASS2;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		default:
			if (!pCInspWH->IsGLASS1_VacSensorOn() && !pCInspWH->IsGLASS2_VacSensorOn())
			{
				return MAX_NUM_OF_GLASS;
			}
			else
			{
				return NO_GLASS;
			}
			break;
		}
#endif
	}

	return NO_GLASS;
}





INT CPickArm::PickGlass_DOWN()
{
#if 1 //20121208
	if (
		(SetGLASS2_VacSol(ON, GMP_WAIT) != GMP_SUCCESS)//PI9000
	   )
	{
		return GMP_FAIL;
	}
#else
	if (
		(SetGLASS2_VacSol(ON, GMP_WAIT) != GMP_SUCCESS) ||
	   )
	{
		return GMP_FAIL;
	}
#endif
	return GMP_SUCCESS;
}

INT CPickArm::PickGlass_UP()
{
	INT nResult = GMP_SUCCESS;
	
	if (m_bIsOfflineMode)
	{
		m_bGlass1Exist = TRUE;
	}

	return GMP_SUCCESS;

}

INT CPickArm::PlaceGlass(GlassNum lGlassNum) // ONLY ONE PICK PLACE EACH TIME
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	LONG lDelay;

	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		DisplayMessage("CPickArm::PlaceGlass : SW Error");
		return GMP_FAIL;
	}

#if 1 //debug only 20121030
	if (HMI_bDebugSeq)	// 20140801 Yip: Only Show Message When Debug Sequence
	{
		if (lGlassNum == GLASS1)
		{
			DisplayMessage("CPickArm::PlaceGlass 1");
		}
		else if (lGlassNum == GLASS2)
		{
			DisplayMessage("CPickArm::PlaceGlass 2");
		}
	}
#endif
	pCInspWH->SetVacSol(lGlassNum, ON);

	//if ((m_nLastError = pCInspWH->MoveZToLoadLevel(lGlassNum, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = pCInspWH->m_stGlass1VacSol.GetOnDelay();
		if (m_stGlass2VacSol.GetOffDelay() > lDelay)
		{
			lDelay = m_stGlass2VacSol.GetOffDelay();
		}
		SetGLASS2_VacSol(OFF, GMP_WAIT);
		Sleep(lDelay);

		if (m_stWeakBlowSol.GetOnDelay() > 0)
		{
			SetWeakBlowSol(ON, GMP_WAIT);
			SetWeakBlowSol(OFF, GMP_WAIT);
		}

	}

	return GMP_FAIL;
}

/////////////////////////////////////////////////////////////////
//Motor Related
/////////////////////////////////////////////////////////////////
INT CPickArm::MoveX(PICKARM_X_CTRL_SIGNAL ctrl, BOOL bWait)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	INT		nResult		= GMP_SUCCESS;

	LONG lTempPosn = 0;

	CString szDebug = _T("");
	switch (ctrl)
	{
	case PICKARM_X_CTRL_GO_STANDBY_POSN:
		lTempPosn = m_lStandbyPosn;
		break;

	case PICKARM_X_CTRL_GO_PICK_GLASS_POSN:
		lTempPosn = m_lPickGlassPosn;
		break;

	case PICKARM_X_CTRL_GO_PICK_GLASS_OFFSET:
		lTempPosn = m_lPickGlassPosn + m_lPickGlassOffset;
		break;

	case PICKARM_X_CTRL_GO_PLACE_GLASS_POSN:
		lTempPosn = m_lPlaceGlassPosn;
		break;

	case PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_POSN:
		lTempPosn = m_lShBufPlaceGlassPosn;
		break;

	case PICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET: 
		lTempPosn = m_lPlaceGlassPosn + m_lPlaceGlassOffset;
		break;

	case PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET: //20121011
		lTempPosn = m_lShBufPlaceGlassPosn + m_lShBufPlaceGlassOffset;
		break;
	}

	if (HMI_bDebugCal)	// 20140801 Yip: Only Show Message When Debug Calculation
	{
		szDebug.Format("MotorX Posn: %d", lTempPosn);
		DisplayMessage(szDebug);
	}
	return m_stMotorX.MoveAbsolute(lTempPosn, GMP_WAIT);
}

/////////////////////////////////////////////////////////////////
//I/O Related
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//SETUP Related
/////////////////////////////////////////////////////////////////
VOID CPickArm::ModuleSetupPreTask()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	pCInspWH->m_bCheckMotor	= TRUE;
	pCInspWH->m_bCheckIO		= TRUE;

	m_bCheckMotor		= TRUE;
	m_bCheckIO			= TRUE;
	
	CheckModSelected(TRUE);
	
	SetDiagnSteps(g_lDiagnSteps);

	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();
}

VOID CPickArm::ModuleSetupPostTask()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	pCInspWH->m_bCheckMotor	= FALSE;
	pCInspWH->m_bCheckIO		= FALSE;

	m_bCheckMotor		= FALSE;
	m_bCheckIO			= FALSE;

}

BOOL CPickArm::ModuleSetupSetPosn(LONG lSetupTitle)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	BOOL		bResult	= TRUE;
	MTR_POSN	mtrOld;
	mtrOld.x	= 0;
	mtrOld.y	= 0;
	MTR_POSN TempPosn;
	TempPosn.x	= 0;
	TempPosn.y	= 0;

	if (	g_ulUserGroup < SERVICE &&
		g_stSetupPosn[lSetupTitle].lType == MACHINE_PARA)
	{
		HMIMessageBox(MSG_OK, "WARNING", "You are not allowed to change Machine Parameters");
		return FALSE;
	}

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Overwrite Position?");
	if (lAnswer == rMSG_TIMEOUT || lAnswer == rMSG_CANCEL)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
		return FALSE;
	}

	TempPosn.x = m_stMotorX.GetEncPosn();

	switch (lSetupTitle)
	{
	case 0:
		mtrOld.x			= m_lStandbyPosn;

		m_lStandbyPosn		= TempPosn.x;
		break;

	case 1:
		mtrOld.x			= m_lPickGlassPosn;

		m_lPickGlassPosn	= TempPosn.x;

		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Pick Glass Offset?");
		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPickGlassOffset = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
		}
		break;

	case 2:
		mtrOld.x			= m_lPickGlassOffset;

		m_lPickGlassOffset	= TempPosn.x - m_lPickGlassPosn; //20121225
		TempPosn.x			= m_lPickGlassOffset;
		break;

	case 3:
		mtrOld.x			= m_lPlaceGlassPosn;

		m_lPlaceGlassPosn	= TempPosn.x;
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Place Glass Offset?");
		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPlaceGlassOffset = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
		}
		break;

	case 4:
		mtrOld.x			= m_lShBufPlaceGlassPosn;

		m_lShBufPlaceGlassPosn	= TempPosn.x;

		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Place Glass Offset(ShBuffer)?");
		if (lAnswer == rMSG_CONTINUE)
		{
			m_lShBufPlaceGlassOffset = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
		}
		break;

	case 5: 
		mtrOld.x			= m_lPlaceGlassOffset;

		m_lPlaceGlassOffset	= TempPosn.x - m_lPlaceGlassPosn;

		TempPosn.x			= m_lPlaceGlassOffset;
		break;

	case 6: 
		mtrOld.x			= m_lShBufPlaceGlassOffset;

		m_lShBufPlaceGlassOffset	= TempPosn.x - m_lShBufPlaceGlassPosn;

		TempPosn.x			= m_lShBufPlaceGlassOffset;
		break;
	}

	CPI9000Stn::ModuleSetupSetPosn(lSetupTitle, "X", mtrOld.x, TempPosn.x);

	return TRUE;
}

BOOL CPickArm::ModuleSetupGoPosn(LONG lSetupTitle)
{
	INT nResult = GMP_SUCCESS;
	CString szMsg;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "GO POSN OPERATION", szMsg);
		return FALSE;
	}

	if (MoveRelatedModuleToStandby() != GMP_SUCCESS)	// 20150327
	{
		return FALSE;
	}

	switch (lSetupTitle)
	{
	case 0:
		nResult = SetupGoStandbyPosn();
		break;
	
	case 1:
		nResult = SetupGoPickGlassPosn();
		break;
		
	case 2:
		nResult = SetupGoPickGlassOffset();
		break;

	case 3:
		nResult = SetupGoPlaceGlassPosn();
		break;

	case 4:
		nResult = SetupGoShBufPlaceGlassPosn();
		break;

	case 5: 
		nResult = SetupGoPlaceGlassOffset();
		break;

	case 6: 
		nResult = SetupGoShBufPlaceGlassOffset();
		break;
	}
	
	return nResult;
}

BOOL CPickArm::ModuleSetupSetPara(PARA stPara)
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
		m_stGlass1VacSol.SetOnDelay(stPara.lPara);
		m_stGlass2VacSol.SetOnDelay(stPara.lPara);
		break;

	case 1:
		m_stGlass1VacSol.SetOffDelay(stPara.lPara);
		m_stGlass2VacSol.SetOffDelay(stPara.lPara);
		break;

	case 2:
		m_stWeakBlowSol.SetOnDelay(stPara.lPara);
		break;

	case 3:
		m_stWeakBlowSol.SetOffDelay(stPara.lPara);
		break;

	case 4:
		m_lPickPlaceRetryLimit = stPara.lPara;
		break;

	case 5:
		m_stCOF1VacSol.SetOnDelay(stPara.lPara);
		m_stCOF2VacSol.SetOnDelay(stPara.lPara);
		break;


	case 6:
		m_stCOF1VacSol.SetOffDelay(stPara.lPara);
		m_stCOF2VacSol.SetOffDelay(stPara.lPara);
		break;
	}

	return CPI9000Stn::ModuleSetupSetPara(stPara);
}
	
VOID CPickArm::UpdateModuleSetupPosn()
{	
	INT i = 0;
	INT j = 0;

	// Assign Module Setup Posn/Level/Para
	// Position Tab
	i = 0;
	g_stSetupPosn[i].szPosn.Format("Standby Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lStandbyPosn;
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 1
	g_stSetupPosn[i].szPosn.Format("Calib Pick Glass Posn");	
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPickGlassPosn;
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 2
	g_stSetupPosn[i].szPosn.Format("Pick Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPickGlassOffset;
	g_stSetupPosn[i].lPosnY			= 0;


	i++;	// 3
	g_stSetupPosn[i].szPosn.Format("Place Glass Posn(Out)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassPosn;
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 4
	g_stSetupPosn[i].szPosn.Format("Pick / Place Glass Posn(ShBuffer)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lShBufPlaceGlassPosn; //20121011
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 5
	g_stSetupPosn[i].szPosn.Format("Place Glass Offset(Out)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassOffset;
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 6 
	g_stSetupPosn[i].szPosn.Format("Pick / Place Glass Offset(ShBuffer)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lShBufPlaceGlassOffset;
	g_stSetupPosn[i].lPosnY			= 0;

	for (j = i + 1; j < NUM_OF_POSN_XY; j++)
	{
		g_stSetupPosn[j].szPosn.Format("Reserved");
		g_stSetupPosn[j].bEnable		= FALSE;
		g_stSetupPosn[j].bVisible		= FALSE;
		g_stSetupPosn[j].lType			= MACHINE_PARA;
		g_stSetupPosn[j].lPosnX			= 0;
		g_stSetupPosn[j].lPosnY			= 0;
	}
}

VOID CPickArm::UpdateModuleSetupPara()
{
	INT i = 0;
	INT j = 0;

	// Para Tab
	// Delay
	i = 0;
	g_stSetupPara[i].szPara.Format("Vac On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stGlass1VacSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 1
	g_stSetupPara[i].szPara.Format("Vac Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stGlass1VacSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 2
	g_stSetupPara[i].szPara.Format("WeakBlow On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stWeakBlowSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 3
	g_stSetupPara[i].szPara.Format("WeakBlow Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stWeakBlowSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 4
	g_stSetupPara[i].szPara.Format("Pick / Place Retry Limit");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_lPickPlaceRetryLimit;
	g_stSetupPara[i].dPara			= 0;

	i++;	// 5
	g_stSetupPara[i].szPara.Format("COF Vac On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stCOF1VacSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 6
	g_stSetupPara[i].szPara.Format("COF Vac Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stCOF1VacSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;


	// Others
	for (i = i + 1; i < NUM_OF_PARA; i++)
	{
		g_stSetupPara[i].szPara.Format("Reserved");
		g_stSetupPara[i].bEnable		= FALSE;
		g_stSetupPara[i].bVisible		= FALSE;
		g_stSetupPara[i].lType			= MACHINE_PARA;
		g_stSetupPara[i].lPara			= 0;
		g_stSetupPara[i].dPara			= 0.0;
	}
}

INT CPickArm::SetupGoStandbyPosn()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);	

	return nResult;
}

INT CPickArm::SetupGoPickGlassPosn()
{
	INT nResult = GMP_SUCCESS;


	nResult = MoveX(PICKARM_X_CTRL_GO_PICK_GLASS_POSN, GMP_WAIT);

	return nResult;
}

INT CPickArm::SetupGoPickGlassOffset()
{
	INT nResult = GMP_SUCCESS;


	nResult = MoveX(PICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT);

	return nResult;
}

INT CPickArm::SetupGoPlaceGlassPosn()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;


	//lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	//if (lAnswer == rMSG_YES)
	//{
	//	if (
	//		pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
	//		pCInspWH->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
	//		)
	//	{
	//		return GMP_FAIL;
	//	}
	//}
	//else if (lAnswer == rMSG_TIMEOUT)
	//{
	//	return GMP_FAIL;
	//}

	nResult = MoveX(PICKARM_X_CTRL_GO_PLACE_GLASS_POSN, GMP_WAIT);	

	return nResult;
}

INT CPickArm::SetupGoShBufPlaceGlassPosn()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;


	//lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	//if (lAnswer == rMSG_YES)
	//{
	//	if (
	//		pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
	//		pCInspWH->MoveXYToUnloadPosn(GMP_WAIT) != GMP_SUCCESS
	//	   )
	//	{
	//		return GMP_FAIL;
	//	}
	//}
	//else if (lAnswer == rMSG_TIMEOUT)
	//{
	//	return GMP_FAIL;
	//}

	nResult = MoveX(PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_POSN, GMP_WAIT);	

	return nResult;
}

INT CPickArm::SetupGoPlaceGlassOffset() //20121011
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;


	lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	if (lAnswer == rMSG_YES)
	{
		if (
			//pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
			pCInspWH->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
		   )
		{
			return GMP_FAIL;
		}
	}
	else if (lAnswer == rMSG_TIMEOUT)
	{
		return GMP_FAIL;
	}

	nResult = MoveX(PICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CPickArm::SetupGoShBufPlaceGlassOffset() //20121011
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;

	lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	if (lAnswer == rMSG_YES)
	{
		if (
			//pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
			pCInspWH->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
		   )
		{
			return GMP_FAIL;
		}
	}
	else if (lAnswer == rMSG_TIMEOUT)
	{
		return GMP_FAIL;
	}

	nResult = MoveX(PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CPickArm::KeepQuiet()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	INT nResult = GMP_SUCCESS;

	if (!m_fHardware)
	{
		return nResult;
	}

	if (!IsGLASS1_VacSensorOn())
	{
		SetGLASS1_VacSol(OFF, GMP_NOWAIT);
	}
	return nResult;
}

VOID CPickArm::PrintMachineSetup(FILE *fp)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str = "";

	if (fp != NULL)
	{
		fprintf(fp, "\n<<<<%s>>>>\n", (const char*)GetStnName());
		fprintf(fp, "======================================================\n");
		fprintf(fp, "<<Posn>>\n");
		fprintf(fp, "%30s\t : %ld\n", "Standby Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lStandbyPosn"]);
		fprintf(fp, "%30s\t : %ld\n", "Pick Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lPickGlassPosn"]);
		fprintf(fp, "%30s\t : %ld\n", "Place Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lPlaceGlassPosn"]);
		fprintf(fp, "%30s\t : %ld\n", "ShBuf Place Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]);

	}

	CPI9000Stn::PrintMachineSetup(fp);
}

VOID CPickArm::PrintDeviceSetup(FILE *fp)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str = "";

	if (fp != NULL)
	{
		fprintf(fp, "\n<<<<%s>>>>\n", (const char*)GetStnName());
		fprintf(fp, "======================================================\n");
		fprintf(fp, "<<Posn>>\n");
		fprintf(fp, "%30s\t : %ld\n", "Pick Glass Offset",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lPickGlassOffset"]);
		fprintf(fp, "<<Posn>>\n");
		fprintf(fp, "%30s\t : %ld\n", "Place Glass Offset",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lPlaceGlassOffset"]);
		fprintf(fp, "<<Posn>>\n");
		fprintf(fp, "%30s\t : %ld\n", "ShBuf Place Glass Offset",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lShBufPlaceGlassOffset"]);

		fprintf(fp, "<<Para>>\n");
		fprintf(fp, "%30s\t : %ld\n", "Vac On Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stGlass1VacSol.GetName()]["lOnDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "Vac Off Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stGlass1VacSol.GetName()]["lOffDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "WeakBlow On Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stWeakBlowSol.GetName()]["lOnDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "WeakBlow Off Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stWeakBlowSol.GetName()]["lOffDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "Pick Place Retry Limit",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Para"]["m_lPickPlaceRetryLimit"]);

	}

	CPI9000Stn::PrintDeviceSetup(fp);
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
BOOL CPickArm::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	//DOUBLE dValue = 0.0;

	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lStandbyLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), CString("StandbyLevel"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lSafetyLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), CString("SafetyLevel"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		bResult = CTransferArm::printMachineDeviceInfo();
	}
	//if (bResult)
	//{
	//	bResult = CPI9000Stn::printMachineDeviceInfo();
	//}
	return bResult;
}
#endif

// ------------------------------------------------------------------------
// HMI Command delcaration
// ------------------------------------------------------------------------


LONG CPickArm::HMI_PickGlassOperation(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bOpDone						= TRUE;
	BOOL bResult						= FALSE;


	bResult = PickGlassOperation();
	if (bResult)
	{
		MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CPickArm::HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bOpDone						= TRUE;
	LONG lResponse						= 0;
	BOOL bResult						= TRUE;

	bResult = PlaceGlassOperation();

	if (bResult)
	{
		MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CPickArm::HMI_SetPlaceGlassPrority(IPC_CServiceMessage &svMsg)
{
	LONG lGlassType = HMISelectionBox("PLACE GLASS PRORITY", "Place at where first", "GLASS1", "GLASS2");
	switch (lGlassType)
	{
	case 0:
		m_bIsDefaultIndex = FALSE;		
		break;
	case 1:
		m_bIsDefaultIndex = TRUE;
		break;
	}

	return 0;
}


LONG CPickArm::HMI_SetStandAlone(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	
	if (bMode)
	{
		HMI_bStandAlone = TRUE;
	}
	else
	{
		HMI_bStandAlone = FALSE;
	}
#if 1
	{
		INT nIndex = 0;
		CString szMsg = _T(" ");
		const char szTxt1[] = _T("GC902_PickGlass1");
		const char szTxt2[] = _T("GC902_PickGlass2");
		const char szTxtBoth[] = _T("GC902_PickGlass1_2_123456789");
		char *pCmd = NULL;

		pCmd = (char*)&szTxtBoth[0];

	//nIndex = sizeof(szTxtBoth);
		nIndex = strlen(pCmd) + 1;
		szMsg.Format("sizeof=%d", nIndex);
		DisplayMessage(szMsg);
	}
#endif
	return 0;
}

LONG CPickArm::IPC_SaveMachineParam()
{	
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]	= m_lShBufPlaceGlassPosn;

	pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bInlineInBack"]		= m_bInlineInBack; 
	return CTransferArm::IPC_SaveMachineParam();
}

LONG CPickArm::IPC_LoadMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	
	m_lShBufPlaceGlassPosn	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]; 

	m_bInlineInBack			= (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bInlineInBack"];

	return CTransferArm::IPC_LoadMachineParam();
}

LONG CPickArm::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szTmp;
	szTmp = GetStnName();
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"]		= m_lPickGlassOffset;
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]		= m_lPlaceGlassOffset; 
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]	= m_lShBufPlaceGlassOffset; 
	return CTransferArm::IPC_SaveDeviceParam();
}

LONG CPickArm::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	
	m_lPickGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"];
	m_lPlaceGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]; 
	m_lShBufPlaceGlassOffset	= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]; 

	return CTransferArm::IPC_LoadDeviceParam();
}


INT CPickArm::MoveRelatedModuleToStandby()	// 20150327
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	
	LONG lAnswer = rMSG_TIMEOUT;
	INT nResult = GMP_SUCCESS;
	CString szMsg = "";

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", szMsg);
		return GMP_FAIL;
	}

	if (pCInspWH->m_fHardware) //20140603
	{
		pCInspWH->CheckModSelected(TRUE);
	}
	if (pCInspWH->m_fHardware && !pCInspWH->m_bModSelected)
	{
		if (g_ulUserGroup <= SERVICE)
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "INSPWH Module Not Selected. Operation Abort!");
		}
		else
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "INSPWH Module Not Selected. Continue Operation?");
		}

		if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT || g_ulUserGroup <= SERVICE)
		{
			return GMP_FAIL;
		}
	}

	if (
		pCInspWH->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS //||
		//pCInspWH->MoveZ(WHZ_1,INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
		//pCInspWH->MoveZ(WHZ_2,INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS
	   )
	{
		return GMP_FAIL;
	}
	return nResult;
}

//GlassNum CPickArm::AssignPickFromWhere()
//{
//	if (
//		INSPWH_GLASS_Status[GLASS1] == ST_INSP_DONE &&
//		INSPWH_GLASS_Status[GLASS2] == ST_EMPTY
//		)
//	{
//		return GLASS1;
//	}
//	else if (
//		INSPWH_GLASS_Status[GLASS2] == ST_INSP_DONE &&
//		INSPWH_GLASS_Status[GLASS1] == ST_EMPTY
//		)
//	{
//		return GLASS2;
//	}
//	else if (
//		(INSPWH_GLASS_Status[GLASS1] == ST_INSP_DONE && INSPWH_GLASS_Status[GLASS2] == ST_INSP_DONE) ||
//		INSPWH_Status == ST_MOD_NOT_SELECTED
//		)
//	{
//		return MAX_NUM_OF_GLASS;
//	}
//	else 
//	{
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		DisplayMessage(__FUNCTION__);
//		return NO_GLASS;
//	}
//}

GlassNum CPickArm::AssignPickFromWhere_ShBuf()
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	if (
		pCShBufferWH->m_bGlass1Exist &&
		!pCShBufferWH->m_bGlass2Exist
		)
	{
		return GLASS1;
	}
	else if (
		pCShBufferWH->m_bGlass2Exist &&
		!pCShBufferWH->m_bGlass1Exist
		)
	{
		return GLASS2;
	}
	else if (
		(pCShBufferWH->m_bGlass1Exist && pCShBufferWH->m_bGlass2Exist) ||
		BUFFERWH_Status == ST_MOD_NOT_SELECTED
		)
	{
		return MAX_NUM_OF_GLASS;
	}
	else 
	{
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		DisplayMessage(__FUNCTION__);
		return NO_GLASS;
	}
}

BOOL CPickArm::CheckPlaceStatus()
{
	if (
		!IsGLASS1_VacSensorOn() && !IsGLASS2_VacSensorOn() && 
		(!m_bUseCOFVacuum || (!IsCOF1_VacSensorOn() && !IsCOF2_VacSensorOn()))
	   )
	{
		return TRUE;
	}

	return FALSE;
}