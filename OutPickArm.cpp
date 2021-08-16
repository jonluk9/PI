/////////////////////////////////////////////////////////////////
//	OUTPICKARM.cpp : interface of the COutPickArm class
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

#define OUTPICKARM_EXTERN
#include "OUTPICKARM.h"
#include "INPICKARM.h"

#include "INSPWH.h"
#include "ShBufferWH.h"
#include "RejectBelt.h"

#include "WH1.h"
#include "WH2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COutPickArm, CTransferArm)

COutPickArm::COutPickArm()
{
	////I/O and Motors are done!!
	//// CPI9000Srn
	m_lStnErrorCode		= IDS_PICKARM_NOT_SELECTED_ERR;
	m_lModSelectMode	= GLASS_MOD;	

	//// Motor Related
	m_stMotorX.InitMotorInfo("OUTPICKARM_X", &m_bModSelected);	//done
	m_stMotorZ.InitMotorInfo("OUTPICKARM_Z", &m_bModSelected);	//done

	m_myMotorList.Add((CMotorInfo*)&m_stMotorX);
	m_myMotorList.Add((CMotorInfo*)&m_stMotorZ);


	//// Si Related
	m_stGlass1VacSnr		= CSiInfo("OUTPALVacSnr",			"IOMODULE_IN_PORT_2", BIT_7, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass1VacSnr);
	m_stCOF1VacSnr		= CSiInfo("OUTPACOF1VacSnr",			"IOMODULE_IN_PORT_2", BIT_8, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stCOF1VacSnr);
	m_stGlass2VacSnr		= CSiInfo("OUTPARVacSnr",			"IOMODULE_IN_PORT_2", BIT_9, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass2VacSnr);
	m_stCOF2VacSnr		= CSiInfo("OUTPACOF2VacSnr",			"IOMODULE_IN_PORT_2", BIT_10, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stCOF2VacSnr);
	m_stXLmtSnr			= CSiInfo("OUTPAXLmtSnr",				"IOMODULE_IN_PORT_2", BIT_11, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stXLmtSnr);	
	m_stZHomeSnr			= CSiInfo("OUTPAZHmSnr",				"IOMODULE_IN_PORT_2", BIT_12, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stZHomeSnr);
	m_stZLmtSnr			= CSiInfo("OUTPAZLmtSnr",				"IOMODULE_IN_PORT_2", BIT_13, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stZLmtSnr);		

	m_stRecieveUpStreamInLine		= CSiInfo("OUTPARecieveUpStreamInLine",		"IOMODULE_IN_PORT_8", BIT_0, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveUpStreamInLine);	
	m_stRecieveReady				= CSiInfo("OUTPARecieveReady",				"IOMODULE_IN_PORT_8", BIT_1, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveReady);
	m_stRecieving					= CSiInfo("OUTPARecieving",					"IOMODULE_IN_PORT_8", BIT_2, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieving);
	m_stRecieveStageMotion			= CSiInfo("OUTPARecieveStageMotion",			"IOMODULE_IN_PORT_8", BIT_3, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveStageMotion);
	m_stRecieveStageInPosition		= CSiInfo("OUTPARecieveStageInPosition",		"IOMODULE_IN_PORT_8", BIT_4, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveStageInPosition);
	m_stRecieveStageVacOK			= CSiInfo("OUTPARecieveStageVacOK",			"IOMODULE_IN_PORT_8", BIT_5, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveStageVacOK);
	m_stRecieveG1VacOff				= CSiInfo("OUTPARecieveG1VacOff",				"IOMODULE_IN_PORT_8", BIT_6, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveG1VacOff);
	m_stRecieveG2VacOff				= CSiInfo("OUTPARecieveG2VacOff",				"IOMODULE_IN_PORT_8", BIT_7, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveG2VacOff);
	m_stRecieveG1VacError			= CSiInfo("OUTPARecieveG1VacError",			"IOMODULE_IN_PORT_8", BIT_8, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveG1VacError);
	m_stRecieveG2VacError			= CSiInfo("OUTPARecieveG2VacError",			"IOMODULE_IN_PORT_8", BIT_9, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveG2VacError);
	m_stRecieveCancel				= CSiInfo("OUTPARecieveCancel",				"IOMODULE_IN_PORT_8", BIT_10, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stRecieveCancel);
	m_stInterLockFront				= CSiInfo("OUTPAInterLockFront",				"IOMODULE_IN_PORT_8", BIT_11, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stInterLockFront);

	//// So Related
	m_stGlass1VacSol		= CSoInfo("OUTPALVacSol",			"IOMODULE_OUT_PORT_5", BIT_11, OUTPICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass1VacSol);
	m_stCOF1VacSol		= CSoInfo("OUTPACOF1VacSol",			"IOMODULE_OUT_PORT_5", BIT_12, OUTPICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stCOF1VacSol);
	m_stGlass2VacSol		= CSoInfo("OUTPARVacSol",			"IOMODULE_OUT_PORT_5", BIT_13, OUTPICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass2VacSol);
	m_stCOF2VacSol		= CSoInfo("OUTPACOF2VacSol",			"IOMODULE_OUT_PORT_5", BIT_14, OUTPICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stCOF2VacSol);
	m_stWeakBlowSol			= CSoInfo("OUTPAG1WeakBlowSol",		"IOMODULE_OUT_PORT_5", BIT_15, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stWeakBlowSol);
	m_stGlass2WeakBlowSol			= CSoInfo("OUTPAG2WeakBlowSol",		"IOMODULE_OUT_PORT_5", BIT_5, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stGlass2WeakBlowSol);

	m_stSendUpStreamInLine		= CSoInfo("PickArmSendUpStreamInLine",		"IOMODULE_OUT_PORT_7", BIT_0, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendUpStreamInLine);
	m_stSendReady				= CSoInfo("PickArmSendReady",				"IOMODULE_OUT_PORT_7", BIT_1, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendReady);
	m_stSendStageMotion			= CSoInfo("PickArmSendMotion",				"IOMODULE_OUT_PORT_7", BIT_2, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendStageMotion);
	m_stSendStageInPosition		= CSoInfo("PickArmSendInPosition",			"IOMODULE_OUT_PORT_7", BIT_3, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendStageInPosition);
	m_stSendStageVacOn			= CSoInfo("PickArmSendStageVacOn",			"IOMODULE_OUT_PORT_7", BIT_4, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendStageVacOn);
	m_stSending					= CSoInfo("PickArmSending",					"IOMODULE_OUT_PORT_7", BIT_5, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSending);
	m_stSendG1Exist				= CSoInfo("PickArmSendG1Exist",				"IOMODULE_OUT_PORT_7", BIT_6, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendG1Exist);
	m_stSendG2Exist				= CSoInfo("PickArmSendG2Exist",				"IOMODULE_OUT_PORT_7", BIT_7, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendG2Exist);
	m_stSendCancel				= CSoInfo("PickArmSendCancel",				"IOMODULE_OUT_PORT_7", BIT_8, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stSendCancel);
	m_stInterLockBack			= CSoInfo("PickArmInterLockBack",			"IOMODULE_OUT_PORT_7", BIT_9, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stInterLockBack);
	m_stPAEMO					= CSoInfo("PickArmEMO",						"IOMODULE_OUT_PORT_7", BIT_10, OUTPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stPAEMO);

	//m_bIsDefaultIndex	= FALSE;	// TRUE: Place Glass2 First, then Glass1

	//m_lPickGlassOffset	= 0;
	//
	//m_bIsVacError = FALSE; //20121224 vac error
	//HMI_bStandAlone		= FALSE;
	//m_bInlineInBack		= FALSE;
	m_lCurInspWH			= WH_1;
	//m_lCurGlass				= GLASS1;
}

COutPickArm::~COutPickArm()
{
}

BOOL COutPickArm::InitInstance()
{
	CTransferArm::InitInstance();
	return TRUE;
}

INT COutPickArm::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CTransferArm::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID COutPickArm::Operation()
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
VOID COutPickArm::UpdateOutput()
{
	// Keep it for debug breakpoint

	CTransferArm::UpdateOutput();
}

VOID COutPickArm::RegisterVariables()
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
		RegVariable(_T("HMI_bOutPickArmXOn"), &m_stMotorX.bHMI_On);
		RegVariable(_T("HMI_lOutPickArmXEncoder"), &m_stMotorX.lHMI_CurrPosn);
		RegVariable(GetStation() + _T("dXDistPerCount"), &m_stMotorX.stAttrib.dDistPerCount);

		RegVariable(_T("HMI_bOutPickArmZOn"), &m_stMotorZ.bHMI_On);
		RegVariable(_T("HMI_lOutPickArmZEncoder"), &m_stMotorZ.lHMI_CurrPosn);
		RegVariable(GetStation() + _T("dZDistPerCount"), &m_stMotorZ.stAttrib.dDistPerCount);

		RegVariable(_T("HMI_bOutPickArmXLmtSnr"), &m_stXLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmZLmtSnr"), &m_stZLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmZHomeSnr"), &m_stZHomeSnr.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmG1VacSnr"), &m_stGlass1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmG2VacSnr"), &m_stGlass2VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmCOF1VacSnr"), &m_stCOF1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmCOF2VacSnr"), &m_stCOF2VacSnr.bHMI_Status);

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
		
		RegVariable(_T("HMI_bOutPickArmG1VacSol"), &m_stGlass1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmG2VacSol"), &m_stGlass2VacSol.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmWeakBlowSol"), &m_stWeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmWeakBlow2Sol"), &m_stGlass2WeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmCOF1VacSol"), &m_stCOF1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bOutPickArmCOF2VacSol"), &m_stCOF2VacSol.bHMI_Status);

		RegVariable(_T("HMI_bStandAlone"), &HMI_bStandAlone);
		RegVariable(GetStation() + _T("lPickPlaceRetryLimit"), &m_lPickPlaceRetryLimit);
		RegVariable(GetStation() + _T("bInlineInBack"), &m_bInlineInBack);

		RegVariable(GetStation() + _T("dCycleTime"), &m_dCycleTime);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnX"), HMI_PowerOnX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommX"), HMI_CommX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeX"), HMI_HomeX);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnZ"), HMI_PowerOnZ);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommZ"), HMI_CommZ);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeZ"), HMI_HomeZ);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleCOF1VacSol"), HMI_ToggleCOF1VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleCOF2VacSol"), HMI_ToggleCOF2VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG1VacSol"), HMI_ToggleG1VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlowSol"), HMI_ToggleWeakBlowSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlow2Sol"), HMI_ToggleG2WeakBlowSol);
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

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetStandAlone"), HMI_SetStandAlone);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXPos"), HMI_IndexXPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXNeg"), HMI_IndexXNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZPos"), HMI_IndexZPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZNeg"), HMI_IndexZNeg);
		//
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPickPlaceRetryLimit"), HMI_SetPickPlaceRetryLimit);

		//// -------------------------------------------------------
		//// varibales
		//// -------------------------------------------------------
	
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PickGlassOperation"), HMI_PickGlassOperation);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PlaceGlassOperation"), HMI_PlaceGlassOperation);
		
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPlaceGlassPrority"), HMI_SetPlaceGlassPrority);

		//Test

	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

INT	COutPickArm::SetSendUpStreamInLine(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSendReady(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSendStageMotion(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSendStageInPosition(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSendStageVacOn(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSending(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSendG1Exist(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSendG2Exist(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetSendCancel(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetInterLockBack(BOOL bMode, BOOL bWait)
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

INT	COutPickArm::SetPAEMO(BOOL bMode, BOOL bWait)
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

LONG COutPickArm::HMI_ToggleSendUpStreamInLine(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSendReady(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSendStageMotion(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSendStageInPosition(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSendStageVacOn(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSending(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSendG1Exist(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSendG2Exist(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleSendCancel(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleInterLockBack(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_TogglePAEMO(IPC_CServiceMessage &svMsg)
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

LONG COutPickArm::HMI_ToggleInlineInBack(IPC_CServiceMessage &svMsg)
{
	m_bInlineInBack = !m_bInlineInBack;
	return 0;
}
///////////////////////////////////////////////////////////////////
////Modified Virtual Operational Functions
///////////////////////////////////////////////////////////////////
VOID COutPickArm::PreStartOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	m_bModError = FALSE;
	m_bGlass1Exist = FALSE;
	m_bGlass2Exist = FALSE;
	m_bIsVacError = FALSE; //20121224 vac error

	if (!m_bModSelected)
	{	
		OUTPICKARM_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = OUTPICKARM_STOP_Q;
		m_bStopCycle = FALSE;
	}
	else
	{
		if (
			((m_nLastError = SetWeakBlowSol(OFF)) != GMP_SUCCESS) || 
			((m_nLastError = SetG2WeakBlowSol(OFF)) != GMP_SUCCESS) || 
			((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT)) != GMP_SUCCESS)
			)
		{
			Command(glSTOP_COMMAND, TRUE);
			OUTPICKARM_Status = ST_STOP;
			m_qSubOperation = OUTPICKARM_STOP_Q;
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

			OUTPICKARM_Status = ST_IDLE;
			HS_ClearAllHandShakeSignal();
			SetHandShakeBit(&m_stSendUpStreamInLine, ON);
			SetHandShakeBit(&m_stSendReady, ON);
			SetHandShakeBit(&m_stSendCancel, OFF);

			m_qSubOperation = OUTPICKARM_IDLE_Q;
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
	UpdateDebugSeq(GetSequenceString(), OUTPICKARM_Status, m_bGlass1Exist, m_bGlass2Exist);

	CPI9000Stn::PreStartOperation();
}

VOID COutPickArm::AutoOperation()
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
		(m_bModSelected && m_qSubOperation == OUTPICKARM_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			if (OUTPICKARM_Status != ST_VAC_ERROR && (HMI_bStandAlone || IsHandShakeBitOn(m_stInterLockFront)))	// 20160615
			{
				MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
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

		
		OUTPICKARM_Status = ST_STOP;
		HS_ClearAllHandShakeSignal();
		SetHandShakeBit(&m_stSendReady, OFF);
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

		CPI9000Stn::StopOperation();
	}
}

///////////////////////////////////////////////////////////////////
////Operational Sequences
///////////////////////////////////////////////////////////////////
CString COutPickArm::GetSequenceString()
{
	switch (m_qSubOperation)
	{
	case OUTPICKARM_IDLE_Q:
		return "OUTPICKARM_IDLE_Q";
		
	case OUTPICKARM_WAIT_BUFFER_UNLOAD_Q:
		return "OUTPICKARM_WAIT_BUFFER_UNLOAD_Q";

	case OUTPICKARM_PICK_GLASS_FROM_BUFFER_Q:
		return "OUTPICKARM_PICK_GLASS_FROM_BUFFER_Q";

	case OUTPICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q:
		return "OUTPICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q";

	case OUTPICKARM_WAIT_INSPWH_UNLOAD_Q:
		return "OUTPICKARM_WAIT_INSPWH_UNLOAD_Q";

	case OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q:
		return "OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q";

	case OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q:
		return "OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q";

	case OUTPICKARM_UNLOAD_TO_BUFFER_WH_Q:
		return "OUTPICKARM_UNLOAD_TO_BUFFER_WH_Q";

	case OUTPICKARM_PLACE_GLASS_TO_BUFFER_Q:
		return "OUTPICKARM_PLACE_GLASS_TO_BUFFER_Q";
		
	case OUTPICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q:
		return "OUTPICKARM_CHEKC_PLACE_BUFFER_GLASS_OK_Q";
		
	case OUTPICKARM_UNLOAD_TO_FLIPPER_WH_Q:
		return "OUTPICKARM_UNLOAD_TO_FLIPPER_WH_Q";

	case OUTPICKARM_WAIT_FLIPPER_WH_ACK_Q:
		return "OUTPICKARM_WAIT_FLIPPER_WH_ACK_Q";
		
	case OUTPICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q:
		return "OUTPICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q";
		
	case OUTPICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q:
		return "OUTPICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q";

	case OUTPICKARM_CHECK_PLACE_GLASS_Q:
		return "OUTPICKARM_CHECK_PLACE_GLASS_Q";
		
	case OUTPICKARM_ANY_ERROR_GLASS_PICKED_Q:
		return "OUTPICKARM_ANY_ERROR_GLASS_PICKED_Q";

	case OUTPICKARM_WAIT_REJBELT_READY_Q:
		return "OUTPICKARM_WAIT_REJBELT_READY_Q";

	case OUTPICKARM_STOP_Q:
		return "OUTPICKARM_STOP_Q";
		
	}

	return "Hello World!!";
}

VOID COutPickArm::OperationSequence()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	m_nLastError = GMP_SUCCESS;
	PRINT_DATA	stPrintData;


	CString szMsg = "";

	switch (m_qSubOperation)
	{
		case OUTPICKARM_IDLE_Q:		//0

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (!IsGLASS1_VacSensorOn() && !IsGLASS2_VacSensorOn())   //no Glass 
			{
				m_bGlass1Exist = FALSE;
				m_bGlass2Exist = FALSE;

				if (		
					((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					OUTPICKARM_Status = ST_EMPTY;
					m_qSubOperation = OUTPICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
				}
			}
			else  // Glass Exist
			{
				if (IsGLASS1_VacSensorOn())
				{
					m_bGlass1Exist = TRUE;
				}

				if (IsGLASS2_VacSensorOn())
				{
					m_bGlass2Exist = TRUE;
				}

				OUTPICKARM_Status = ST_BUSY;
				m_qSubOperation = OUTPICKARM_ANY_ERROR_GLASS_PICKED_Q;		// go to 15

			}

			break;

		case OUTPICKARM_ANY_ERROR_GLASS_PICKED_Q:		//15

			if(OUTPICKARM_GLASS_Status[GLASS1] == ST_ERROR || OUTPICKARM_GLASS_Status[GLASS2] == ST_ERROR)
			{
				if(OUTPICKARM_GLASS_Status[GLASS2] == ST_ERROR)
				{
					if ((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
					{
						PPAction = GLASS2;
						m_qSubOperation = OUTPICKARM_WAIT_REJBELT_READY_Q;		// go to 16
					}
				}
				else if(OUTPICKARM_GLASS_Status[GLASS1] == ST_ERROR)
				{
					if ((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
					{
						PPAction = GLASS1;
						m_qSubOperation = OUTPICKARM_WAIT_REJBELT_READY_Q;		// go to 16
					}
				}
			}
			else	//No Error Glass Picked
			{
				m_qSubOperation = OUTPICKARM_UNLOAD_TO_BUFFER_WH_Q;		// go to 7
			}
			break;

		case OUTPICKARM_WAIT_REJBELT_READY_Q:			//16
			
			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}			
			else if(!g_bGlassOnRejBelt)
			{
				if((m_nLastError = PlaceGlassOnRejBelt(PPAction)) == GMP_SUCCESS)
				{
					Sleep(500);
					g_bGlassOnRejBelt = TRUE;
					OUTPICKARM_GLASS_Status[PPAction] = ST_EMPTY;

					m_qSubOperation = OUTPICKARM_IDLE_Q;		// go to 0				
				}
			}

			break;
			
		case OUTPICKARM_WAIT_BUFFER_UNLOAD_Q:		//1

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (BUFFERWH_Status == ST_UNLOAD_REQ)  
			{
				if ((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
				{
					OUTPICKARM_Status = ST_BUF_UNLOAD_ACK;
					PPAction = AssignPickFromWhere_ShBuf();
					m_qSubOperation = OUTPICKARM_PICK_GLASS_FROM_BUFFER_Q;		// go to 2
				}
				else
				{
					OUTPICKARM_Status = ST_ERROR;
					m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
					m_bModError = TRUE;
				}
			}
			else
			{
				m_qSubOperation = OUTPICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
			}

			break;

		case OUTPICKARM_PICK_GLASS_FROM_BUFFER_Q:		//2

			if ((m_nLastError = PickGlassFromBuffer(PPAction)) == GMP_SUCCESS)
			{
				TransferBufferGlassStatusToPA(PPAction);
				m_qSubOperation = OUTPICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q;		// go to 3
			}

			break;

		case OUTPICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q:		//3

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (IsPickGlassFromBuffer_OK(PPAction))	// Glass Exist
			{
				OUTPICKARM_Status = ST_DONE;
				m_lPickPlaceRetryCounter = 0;

				m_qSubOperation = OUTPICKARM_UNLOAD_TO_BUFFER_WH_Q;		// go to 7
			}
			else	//Pick Error
			{
				//retry sequence
				if (m_lPickPlaceRetryLimit != 0)
				{
					m_lPickPlaceRetryCounter++;

					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
					{
						// ERROR!
						OUTPICKARM_Status = ST_ERROR;
						m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
						DisplayMessage("PICK GLASS ERROR: Pick Buffer Glass Retry Limit Hit");
						m_bModError = TRUE;
						SetError(IDS_OUTPICKARM_PICK_BUFFER_ERR);
						if (PPAction != GLASS2)
						{
							if (!IsGLASS1_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_GLASS1_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS1 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
							if (m_bUseCOFVacuum && !IsCOF1_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_COF1_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF1 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
						}
						if (PPAction != GLASS1)
						{
							if(!IsGLASS2_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_GLASS2_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS2 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
							if (m_bUseCOFVacuum && !IsCOF2_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_COF2_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF2 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
						}
					}
					else 
					{
						m_qSubOperation = OUTPICKARM_PICK_GLASS_FROM_BUFFER_Q;		// go to 2
					}
				}
				else 
				{
					m_qSubOperation = OUTPICKARM_PICK_GLASS_FROM_BUFFER_Q;		// go to 2
				}
			}

			break;

		case OUTPICKARM_WAIT_INSPWH_UNLOAD_Q:		//4

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (TA_MANAGER_Status == ST_MB1_REQ_UNLOAD)  //WH1 requests Unload
			{
				m_lCurInspWH = WH_1;
				OUTPICKARM_Status = ST_MB1_ACK_UNLOAD;		//ACK
				PPAction = AssignPickFromWhere(WH_1);
				
				if ((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
				{
					m_qSubOperation = OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q;		// go to 5
				}
			}
			else if (TA_MANAGER_Status == ST_MB2_REQ_UNLOAD)  //WH2 requests Unload
			{
				m_lCurInspWH = WH_2;
				OUTPICKARM_Status = ST_MB2_ACK_UNLOAD;		//ACK
				PPAction = AssignPickFromWhere(WH_2);

				if ((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
				{
					m_qSubOperation = OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q;		// go to 5
				}
			}
			else
			{
				m_qSubOperation = OUTPICKARM_WAIT_BUFFER_UNLOAD_Q;		// go to 1
			}

			break;

		case OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q:		//5

			if (
				((m_nLastError = PickGlassFromInspWH(m_lCurInspWH, PPAction)) == GMP_SUCCESS)
				)
			{
				TransferInspWHGlassStatusToPA(m_lCurInspWH, PPAction);
				m_qSubOperation = OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q;		// go to 6
			}

			break;

		case OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q:		//6

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (IsPickGlassFromInspWH_OK(m_lCurInspWH, PPAction) || AutoMode == BURN_IN )	// Glass Exist
			{
				OUTPICKARM_Status = ST_DONE;
				m_lPickPlaceRetryCounter = 0;
				INSPWH_Status[m_lCurInspWH] = ST_EMPTY;		// ACK inspWH continue
				if(AutoMode != BURN_IN)
				{
					m_qSubOperation = OUTPICKARM_IDLE_Q;		// go to 0
				}
				else
				{
					m_bGlass1Exist = TRUE;
					m_bGlass2Exist = TRUE;
					m_qSubOperation = OUTPICKARM_ANY_ERROR_GLASS_PICKED_Q;		// go to 0
				}
			}
			else	//Pick Error
			{
				//retry sequence
				if (m_lPickPlaceRetryLimit != 0)
				{
					m_lPickPlaceRetryCounter++;

					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
					{
						// ERROR!
						OUTPICKARM_Status = ST_ERROR;
						m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
						DisplayMessage("PICK GLASS ERROR: Pick InspWH Glass Retry Limit Hit");
						m_bModError = TRUE;
						SetError(IDS_OUTPICKARM_PICK_INSPWH_ERR);
						if (PPAction != GLASS2)
						{
							if (!IsGLASS1_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_GLASS1_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS1 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
							if (m_bUseCOFVacuum && !IsCOF1_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_COF1_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF1 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
						}
						if (PPAction != GLASS1)
						{
							if(!IsGLASS2_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_GLASS2_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: GLASS2 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
							if (m_bUseCOFVacuum && !IsCOF2_VacSensorOn())
							{
								DisplayMessage("*********************************************");
								SetError(IDS_OUTPICKARM_COF2_VAC_ERR);
								szMsg.Format("OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q: COF2 VAC ERROR");
								DisplayMessage(szMsg);
								DisplayMessage("*********************************************");
							}
						}
					}
					else 
					{
						m_qSubOperation = OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q;		// go to 5
					}
				}
				else 
				{
					m_qSubOperation = OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q;		// go to 5
				}
			}

			break;

		case OUTPICKARM_UNLOAD_TO_BUFFER_WH_Q:		//7

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if ((BUFFERWH_Status == ST_LOAD_REQ) && IsVacSensorOn(GLASS1) && IsVacSensorOn(GLASS2))
			{
				if (
					((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS) //BUFFER WH
					)
				{
					OUTPICKARM_Status = ST_BUF_LOAD_ACK;
					m_qSubOperation = OUTPICKARM_PLACE_GLASS_TO_BUFFER_Q;		// go to 8
				}
			}
			else
			{
				if (
					((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS) //Flipper WH
					)
				{
					OUTPICKARM_Status = ST_BUSY;
					m_qSubOperation = OUTPICKARM_UNLOAD_TO_FLIPPER_WH_Q;		// go to 10
				}
			}
			
			break;

		case OUTPICKARM_PLACE_GLASS_TO_BUFFER_Q:		//8

			if (
				((m_nLastError = PlaceGlassToBuffer(MAX_NUM_OF_GLASS)) == GMP_SUCCESS)  // Both Glasses
				)
			{
				SetVacSol(MAX_NUM_OF_GLASS, ON, GMP_WAIT);	//Vac ON
				m_qSubOperation = OUTPICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q;		// go to 9
			}

			break;
			
		case OUTPICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q:		//9

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (IsPlaceGlassToBuffer_OK())	// Arm Empty
			{
				OUTPICKARM_GLASS_Status[GLASS1] = ST_EMPTY;
				OUTPICKARM_GLASS_Status[GLASS2] = ST_EMPTY;

				OUTPICKARM_Status = ST_EMPTY;
				m_lPickPlaceRetryCounter = 0;
				SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_WAIT);	//Vac OFF

				m_qSubOperation = OUTPICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
			
			}
			else	//Pick Error
			{
				//retry sequence
				if (m_lPickPlaceRetryLimit != 0)
				{
					m_lPickPlaceRetryCounter++;

					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
					{
						// ERROR!
						OUTPICKARM_Status = ST_ERROR;
						m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
						DisplayMessage("PLACE GLASS ERROR: Place Buffer Glass Retry Limit Hit");
						m_bModError = TRUE;
						SetError(IDS_OUTPICKARM_PLACE_BUFFER_ERR);	// set error at ShBuffer
					}
					else 
					{
						m_qSubOperation = OUTPICKARM_PLACE_GLASS_TO_BUFFER_Q;		// go to 8
					}
				}
				else 
				{
					m_qSubOperation = OUTPICKARM_PLACE_GLASS_TO_BUFFER_Q;		// go to 8
				}
			}

			break;
			
		case OUTPICKARM_UNLOAD_TO_FLIPPER_WH_Q:		//10
			
			if (IsUnloadToFlipperWH())
			{
				HS_OUTPICKARMSelectPanelEnable();
				HS_OUTPICKARMSendPanelReady();
				m_qSubOperation = OUTPICKARM_WAIT_FLIPPER_WH_ACK_Q;		// go to 11

			}
			else		//Place on OutBelt
			{
				if (
					((m_nLastError = PlaceGlassOnOutBelt()) == GMP_SUCCESS) //&&
					//((m_nLastError = MoveX(OUTPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = OUTPICKARM_CHECK_PLACE_GLASS_Q;	//14
					//OUTPICKARM_Status = ST_EMPTY;
					//m_qSubOperation = OUTPICKARM_WAIT_INSPWH_UNLOAD_Q;		// go to 4
				}
			}

			break;

		case OUTPICKARM_WAIT_FLIPPER_WH_ACK_Q:		//11

			if (m_bStopCycle)
			{
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (HS_IsHandshakeCanel())
			{
				// addcode Cancel error
				HS_ClearAllHandShakeSignal();
				m_qSubOperation = OUTPICKARM_IDLE_Q;	// go to 0
			}
			else if (HS_IsFlipperSendPanelReadyACK()/* || AutoMode == BURN_IN*/)
			{
				if (
					((m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = OUTPICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q;		// go to 12
				}
			}

			break;
			
		case OUTPICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q:		//12

			//if (m_bStopCycle)
			//{
			//	HS_ClearAllHandShakeSignal();
			//	m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			//}
			//else 
			if (HS_IsHandshakeCanel())
			{
				HS_ClearAllHandShakeSignal();
				m_qSubOperation = OUTPICKARM_UNLOAD_TO_FLIPPER_WH_Q;		// go to 10
			}
			else if (HS_IsFlipperRequestArmVacOff() /*|| AutoMode == BURN_IN*/)
			{
				SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_NOWAIT);
				SetWeakBlowSol(ON, GMP_WAIT);
				SetG2WeakBlowSol(ON, GMP_WAIT);
				//SetWeakBlowSol(OFF, GMP_NOWAIT);	// Turn off weak blow after WH Z back to standby level.
				HS_SetPanelDisabled();
				m_qSubOperation = OUTPICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q;		// go to 13
			}

			break;
			
		case OUTPICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q:		//13
			//
			//if (m_bStopCycle)
			//{
			//	HS_ClearAllHandShakeSignal();
			//	m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			//}
			//else 
			if (HS_IsHandshakeCanel())
			{
				HS_ClearAllHandShakeSignal();
				m_qSubOperation = OUTPICKARM_IDLE_Q;	// go to 0
			}
			else if (HS_IsFlipperReceivedPanel())
			{
				SetWeakBlowSol(OFF, GMP_NOWAIT);	// Turn off weak blow after WH Z back to standby level.
				SetG2WeakBlowSol(OFF, GMP_NOWAIT);
				HS_OUTPICKARMSendPanelNotReady();

				if (
					((m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = OUTPICKARM_CHECK_PLACE_GLASS_Q;	//14
				}
			}
			break;
			
		case OUTPICKARM_CHECK_PLACE_GLASS_Q:	//14
			if (m_bStopCycle)
			{
				HS_ClearAllHandShakeSignal();
				m_qSubOperation = OUTPICKARM_STOP_Q;	// go to 99
			}
			else if (CheckPlaceStatus() || IsBurnInDiagBond())
			{
				TC_ResetTime();
				m_dTempCycleTime[m_nCycleCounter] = (DOUBLE)(GetTickCount() - m_dwStartTime) / 1000.0;

				if (m_bGlass1Exist && m_bGlass2Exist)
				{
					m_dTempCycleTime[m_nCycleCounter] = m_dTempCycleTime[m_nCycleCounter] / 2;
				}

				if (
					m_dTempCycleTime[0] == 0.0 || 
					m_dTempCycleTime[1] == 0.0 || 
					m_dTempCycleTime[2] == 0.0 || 
					m_dTempCycleTime[3] == 0.0 || 
					m_dTempCycleTime[4] == 0.0 || 
					m_dTempCycleTime[5] == 0.0 || 
					m_dTempCycleTime[6] == 0.0 || 
					m_dTempCycleTime[7] == 0.0 || 
					m_dTempCycleTime[8] == 0.0 || 
					m_dTempCycleTime[9] == 0.0 
					)
				{
					m_dCycleTime = (m_dTempCycleTime[0] + 
						m_dTempCycleTime[1] + 
						m_dTempCycleTime[2] + 
						m_dTempCycleTime[3] + 
						m_dTempCycleTime[4] + 
						m_dTempCycleTime[5] + 
						m_dTempCycleTime[6] + 
						m_dTempCycleTime[7] + 
						m_dTempCycleTime[8] + 
						m_dTempCycleTime[9]) / (m_nCycleCounter + 1);
				}
				else
				{
					m_dCycleTime = (m_dTempCycleTime[0] + 
						m_dTempCycleTime[1] + 
						m_dTempCycleTime[2] + 
						m_dTempCycleTime[3] + 
						m_dTempCycleTime[4] + 
						m_dTempCycleTime[5] + 
						m_dTempCycleTime[6] + 
						m_dTempCycleTime[7] + 
						m_dTempCycleTime[8] + 
						m_dTempCycleTime[9]) / 10;
				}
				if (m_nCycleCounter < 9)
				{
					m_nCycleCounter++;
				}
				else
				{
					m_nCycleCounter = 0;
				}
				m_dwStartTime = GetTickCount();
				pAppMod->m_dUPH = (DOUBLE)(pAppMod->m_ulUnitsInpected / (DOUBLE)(GetTickCount() - m_dwMachineTime)) * 60.0 * 60.0 * 1000.0;

				m_lPickPlaceRetryCounter = 0;

				m_bGlass1Exist = FALSE;
				m_bGlass2Exist = FALSE;
				OUTPICKARM_GLASS_Status[GLASS1] = ST_EMPTY;
				OUTPICKARM_GLASS_Status[GLASS2] = ST_EMPTY;

				OUTPICKARM_Status = ST_EMPTY;
				m_qSubOperation = OUTPICKARM_IDLE_Q;		// go to 0

			}
			else
			{
				OUTPICKARM_Status = ST_VAC_ERROR;
				m_bModError = TRUE;
				DisplayMessage("*********************************************");
				SetError(IDS_OUTPICKARM_PLACE_OUT_ERR);
				szMsg.Format("OUTPICKARM_CHECK_PLACE_GLASS_Q: OUTPICKARM PLACE OUT ERROR");
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");

				m_qSubOperation = OUTPICKARM_STOP_Q;
			}
			break;

		case OUTPICKARM_STOP_Q:		//99
			break;
		
	}


	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			if (OUTPICKARM_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("OUTPICKARM Go State: %ld", m_qSubOperation);
			}
			else
			{
				szMsg.Format("OUTPICKARM Go Stop");
			}
			//DisplayMessage(szMsg);
		}
		UpdateDebugSeq(GetSequenceString(), OUTPICKARM_Status, m_bGlass1Exist, m_bGlass2Exist);
		m_qPreviousSubOperation = m_qSubOperation;

		if (HMI_bDebugSeq)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				stPrintData.szTitle[i]	= "";
			}

			stPrintData.szTitle[0].Format("%ld", GetTickCount());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", OUTPICKARM_Status);

			LogFile->log(stPrintData.szTitle);
		}

	}

}

///////////////////////////////////////////////////////////////////////////////////
VOID COutPickArm::TransferInspWHGlassStatusToPA(LONG lCurWH, GlassNum lGlassNum)
{
	if(lGlassNum == GLASS1)
	{
		OUTPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[lCurWH][GLASS1];
		OUTPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[lCurWH][GLASS2];
	}
	else if(lGlassNum == GLASS2)
	{
		OUTPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[lCurWH][GLASS1];
		OUTPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[lCurWH][GLASS2];
	}
	else if(lGlassNum == MAX_NUM_OF_GLASS)
	{
		OUTPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[lCurWH][GLASS1];
		OUTPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[lCurWH][GLASS2];
	}

	//InspWH resets Glass Status on its Own
}

VOID COutPickArm::TransferBufferGlassStatusToPA(GlassNum lGlassNum)
{
	if(lGlassNum == GLASS1)
	{
		OUTPICKARM_GLASS_Status[GLASS1] = ST_READY;
		OUTPICKARM_GLASS_Status[GLASS2] = ST_EMPTY;
	}
	else if(lGlassNum == GLASS2)
	{
		OUTPICKARM_GLASS_Status[GLASS1] = ST_EMPTY;
		OUTPICKARM_GLASS_Status[GLASS2] = ST_READY;
	}
	else if(lGlassNum == MAX_NUM_OF_GLASS)
	{
		OUTPICKARM_GLASS_Status[GLASS1] = ST_READY;
		OUTPICKARM_GLASS_Status[GLASS2] = ST_READY;
	}

	//Buffer resets GlassExists on its Own
}

INT COutPickArm::PickGlassFromBuffer(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	LONG lDelay;
	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{                
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
	SetVacSol(lGlassNum, ON);

	if ((m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_SHBUF_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
	{

		if (pCShBufferWH->m_stWeakBlowSol.GetOnDelay() > 0)
		{
			pCShBufferWH->SetWeakBlowSol(ON, GMP_WAIT);
			pCShBufferWH->SetWeakBlowSol(OFF, GMP_NOWAIT);
			pCShBufferWH->SetVacSol(lGlassNum, OFF);
		}
		else
		{
			lDelay = max(max(pCShBufferWH->m_stGlass1VacSol.GetOffDelay(), m_stGlass1VacSol.GetOnDelay()), m_stCOF1VacSol.GetOnDelay());
			pCShBufferWH->SetVacSol(lGlassNum, OFF);
			Sleep(lDelay);
		}
		
		return m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT); 

	}

	return GMP_FAIL;
}

INT COutPickArm::PickGlassFromInspWH(LONG lCurWH, GlassNum lGlassNum)
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurInspWH;
	//OUTPICKARM_Z_CTRL_SIGNAL  Z_PLACE_ACTION;

	LONG lDelay;

	if(lCurWH == WH_1)
	{
		pCurInspWH = pCWH1;
	}
	else
	{
		pCurInspWH = pCWH2;
	}

	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{                
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
	SetVacSol(lGlassNum, ON);

	if ((m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = max(max(pCurInspWH->m_stGlass1VacSol.GetOffDelay(), m_stGlass1VacSol.GetOnDelay()), m_stCOF1VacSol.GetOnDelay());
		pCurInspWH->SetVacSol(lGlassNum, OFF);
		Sleep(lDelay);
		if (pCurInspWH->m_stWeakBlowSol.GetOnDelay() > 0)
		{
			switch (lGlassNum)
			{
			case GLASS1:
				pCurInspWH->SetWeakBlowSol(ON, GMP_WAIT);
				pCurInspWH->SetWeakBlowSol(OFF, GMP_WAIT);
				break;

			case GLASS2:
				pCurInspWH->SetWeakBlow2Sol(ON, GMP_WAIT);
				pCurInspWH->SetWeakBlow2Sol(OFF, GMP_WAIT);
				break;

			case MAX_NUM_OF_GLASS:
				pCurInspWH->SetWeakBlowSol(ON, GMP_NOWAIT);
				pCurInspWH->SetWeakBlow2Sol(ON, GMP_WAIT);
				pCurInspWH->SetWeakBlowSol(OFF, GMP_NOWAIT);
				pCurInspWH->SetWeakBlow2Sol(OFF, GMP_WAIT);
				break;
			}
		}
		
		m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT); 

	}

	return m_nLastError;
}

INT COutPickArm::PlaceGlassToBuffer(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	LONG lDelay;
	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{                
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
	pCShBufferWH->SetVacSol(lGlassNum, ON);

	if ((m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = max(max(pCShBufferWH->m_stGlass1VacSol.GetOnDelay(), m_stGlass1VacSol.GetOffDelay()), m_stCOF1VacSol.GetOffDelay());
		SetVacSol(lGlassNum, OFF);
		Sleep (lDelay);

		if (m_stWeakBlowSol.GetOnDelay() > 0)
		{
			SetWeakBlowSol(ON, GMP_WAIT);
			SetG2WeakBlowSol(ON, GMP_WAIT);
			SetWeakBlowSol(OFF, GMP_WAIT);
			SetG2WeakBlowSol(OFF, GMP_WAIT);
		}
		m_nLastError = MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
	}


	return m_nLastError;
}

INT COutPickArm::PlaceGlassOnRejBelt(GlassNum lGlassNum)
{
	INT nResult = GMP_FAIL;

	switch(lGlassNum)
	{
		case GLASS1:
		if (
			((m_nLastError = SetVacSol(lGlassNum, OFF, GMP_WAIT)) == GMP_SUCCESS) &&
			((m_nLastError = SetWeakBlowSol(ON, GMP_WAIT)) == GMP_SUCCESS) &&
			((m_nLastError = SetWeakBlowSol(OFF, GMP_WAIT)) == GMP_SUCCESS)
			)
		{
			m_bGlass1Exist = FALSE;
			nResult = GMP_SUCCESS;
		}
		break;

		case GLASS2:
		if (
			((m_nLastError = SetVacSol(lGlassNum, OFF, GMP_WAIT)) == GMP_SUCCESS) &&
			((m_nLastError = SetG2WeakBlowSol(ON, GMP_WAIT)) == GMP_SUCCESS) &&
			((m_nLastError = SetG2WeakBlowSol(OFF, GMP_WAIT)) == GMP_SUCCESS)
			)
		{
			m_bGlass2Exist = FALSE;
			nResult = GMP_SUCCESS;
		}
		break;
	}
    return nResult;
}

INT COutPickArm::PlaceGlassOnOutBelt()
{
	INT nResult = GMP_FAIL;

	if (
		((m_nLastError = SetVacSol(MAX_NUM_OF_GLASS, OFF, GMP_WAIT)) == GMP_SUCCESS) &&
		((m_nLastError = SetWeakBlowSol(ON, GMP_WAIT)) == GMP_SUCCESS) &&
		((m_nLastError = SetG2WeakBlowSol(ON, GMP_WAIT)) == GMP_SUCCESS) &&
		((m_nLastError = SetWeakBlowSol(OFF, GMP_WAIT)) == GMP_SUCCESS) &&
		((m_nLastError = SetG2WeakBlowSol(OFF, GMP_WAIT)) == GMP_SUCCESS)
		)
	{
		nResult = GMP_SUCCESS;
	}

	return nResult;
}

BOOL COutPickArm::IsPickGlassFromBuffer_OK(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	BOOL bG1Result = TRUE;
	BOOL bG2Result = TRUE;

	if(IsBurnInDiagBond())
	{
		return TRUE;
	}
	
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCShBufferWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			bG1Result = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCShBufferWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			bG2Result = FALSE;
		}
	}
	return bG1Result && bG2Result;
}

BOOL COutPickArm::IsPickGlassFromInspWH_OK(LONG lCurWH, GlassNum lGlassNum)
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurWH;

	BOOL bG1Result = TRUE;
	BOOL bG2Result = TRUE;

	if(lCurWH == WH_1)
	{
		pCurWH = pCWH1;
	}
	else
	{
		pCurWH = pCWH2;
	}

	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCurWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			bG1Result = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCurWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			bG2Result = FALSE;
		}
	}
	return bG1Result && bG2Result;
}

BOOL COutPickArm::IsPlaceGlassToBuffer_OK()	//both Glass
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

BOOL COutPickArm::IsUnloadToFlipperWH()	//use OutBelt, when FlipperWH is not available
{
//	BOOL bResult = TRUE;
	return m_bInlineInBack;
}


VOID COutPickArm::HS_OUTPICKARMSelectPanelEnable()
{
	if (IsGLASS1_VacSensorOn() || (IsBurnInDiagBond() && m_bInlineInBack))
	{
		SetHandShakeBit(&m_stSendG1Exist, ON);
	}
	
	if (IsGLASS2_VacSensorOn() || (IsBurnInDiagBond() && m_bInlineInBack))
	{
		SetHandShakeBit(&m_stSendG2Exist, ON);
	}
}
 
VOID COutPickArm::HS_OUTPICKARMSendPanelReady()
{
	SetHandShakeBit(&m_stSending, ON);
}

BOOL COutPickArm::HS_IsFlipperSendPanelReadyACK()	
{
	BOOL bResult = FALSE;
	
		bResult = IsHandShakeBitOn(m_stRecieving);

	return bResult;
}

BOOL COutPickArm::HS_IsFlipperRequestArmVacOff()	
{
	BOOL bResult = FALSE;

		bResult = (IsHandShakeBitOn(m_stRecieveG1VacOff) || IsHandShakeBitOn(m_stRecieveG2VacOff));

	return bResult;
}

VOID COutPickArm::HS_SetPanelDisabled()
{

	SetHandShakeBit(&m_stSendG1Exist, OFF);
	SetHandShakeBit(&m_stSendG2Exist, OFF);

}

BOOL COutPickArm::HS_IsFlipperReceivedPanel()	
{
	BOOL bResult = FALSE;
		
		bResult = !IsHandShakeBitOn(m_stRecieving);
	
	return bResult;
}

VOID COutPickArm::HS_OUTPICKARMSendPanelNotReady()
{

	SetHandShakeBit(&m_stSending, OFF);
}

BOOL COutPickArm::HS_IsHandshakeCanel()	
{
	BOOL bResult = FALSE;

	if (IsBurnInDiagBond() && !m_bInlineInBack)
	{
		return FALSE;
	}

	if (IsHandShakeBitOn(m_stRecieveCancel) || !IsHandShakeBitOn(m_stRecieveReady) || !IsHandShakeBitOn(m_stRecieveUpStreamInLine))
	{
		bResult = TRUE;
	}

	return bResult;
}


VOID COutPickArm::HS_ClearAllHandShakeSignal()
{

	SetHandShakeBit(&m_stSending, OFF);
	SetHandShakeBit(&m_stSendG1Exist, OFF);
	SetHandShakeBit(&m_stSendG2Exist, OFF);

}

INT COutPickArm::SetHandShakeBit(CSoInfo *stTempSo, BOOL bMode)
{
	INT nResult = GMP_SUCCESS;

//	CSoInfo *stTempSo = &m_stWeakBlowSol;

	if (IsBurnInDiagBond() && !m_bInlineInBack)
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

BOOL COutPickArm::IsHandShakeBitOn(CSiInfo stTempSi)
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
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL COutPickArm::PlaceGlassToOutBeltOperation(GlassNum lGlassNum)
{
	//CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	//if (!pCInspWH->m_bModSelected)
	//{
	//	strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
	//	HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
	//	return FALSE;
	//}

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


	if (MoveX(OUTPICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("PickArm Move Place Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	if (MoveZ(OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
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
			bResult1 = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOn(GetGlassCOF2_VacSensorOnState()))
		{
			strMsg.Format("Place Glass Error: Glass 2 Still Exist On Pick Arm");
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult2 = FALSE;
		}
	}	

	if (bResult1 && bResult2)
	{
		if (MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("PickArm Place Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}
	return bResult1 && bResult2;
}
BOOL COutPickArm::PlaceGlassToRejBeltOperation(GlassNum lGlassNum)
{
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	CRejectBelt *pCRejectBelt = (CRejectBelt*)m_pModule->GetStation("RejectBelt");

	CString strMsg;
	LONG lCurGlass;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	OUTPICKARM_X_CTRL_SIGNAL  PLACE_REJBELT_POSN;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCShBufferWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCShBufferWH->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCWH2->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCWH2->GetStnName());
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
		PLACE_REJBELT_POSN = OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_OFFSET;
	}
	else if (bArmGlassStatus[GLASS1])
	{
		lGlassNum = GLASS1;
		PLACE_REJBELT_POSN = OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_OFFSET;
	}
	else if (bArmGlassStatus[GLASS2])
	{
		lGlassNum = GLASS2;
		PLACE_REJBELT_POSN = OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_OFFSET;
	}
	else
	{
		strMsg.Format("No Glass Exist on Pick Arm. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;

	}

	if (MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPA Move To Standby Level Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCShBufferWH->MoveY(Y_CTRL_OUT_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("ShBufferWH Move To Out Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		lCurGlass = GLASS2;
	}
	else 
	{
		lCurGlass = GLASS1;
	}
	do
	{
		if (pCRejectBelt->IsRejBeltEndSnrOn())
		{
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Reject Belt is Full. Operation Abort!");
			return FALSE;
		}
		else if (pCRejectBelt->MoveBelt(REJ_BELT_CTRL_MOVE_FORWARD, GMP_WAIT) != GMP_SUCCESS)
		{
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Reject Belt Move Forward Error. Operation Abort!");
			return FALSE;
		}
		else if (
			MoveX(PLACE_REJBELT_POSN, GMP_WAIT) != GMP_SUCCESS ||
			m_stMotorX.Sync() != GMP_SUCCESS ||
			MoveZ(OUTPICKARM_Z_CTRL_GO_REJBELT_PLACE_GLASS_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			m_stMotorZ.Sync() != GMP_SUCCESS ||
			PlaceGlassOnRejBelt((GlassNum)lCurGlass) != GMP_SUCCESS ||
			MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS
		   )
		{
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Place Glass Error. Operation Abort!");
			return FALSE;
		}

		lCurGlass--;
		PLACE_REJBELT_POSN = OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_OFFSET;

	} while (lGlassNum == MAX_NUM_OF_GLASS && lCurGlass > NO_GLASS);

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
			bResult1 = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOn(GetGlassCOF2_VacSensorOnState()))
		{
			strMsg.Format("Place Glass Error: Glass 2 Still Exist On Pick Arm");
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult2 = FALSE;
		}
	}	

	if (bResult1 && bResult2)
	{
		if (MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("PickArm Place Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}
	return bResult1 && bResult2;
}

BOOL COutPickArm::PickGlassFromInspWHOperation(LONG lCurWH, GlassNum lGlassNum)
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurInspWH;

	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm");
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	OUTPICKARM_X_CTRL_SIGNAL  PICK_INSPWH_POSN;

	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;


	if(lCurWH == WH_1)
	{
		pCurInspWH = pCWH1;
		PICK_INSPWH_POSN = OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_OFFSET;
	}
	else
	{
		pCurInspWH = pCWH2;
		PICK_INSPWH_POSN = OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_OFFSET;
	}


	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCInPickArm->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCurInspWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCurInspWH->GetStnName());
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
		pCurInspWH->SetVacSol((GlassNum)i, ON);
	}
	Sleep(GetMaxValue(m_stGlass1VacSol.GetOnDelay(), pCurInspWH->m_stGlass1VacSol.GetOnDelay()));

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		bWHGlassStatus[i] = pCurInspWH->IsVacSensorOn((GlassNum)i);
		bArmGlassStatus[i] = IsVacSensorOn((GlassNum)i);

		if (!pCurInspWH->IsVacSensorOn((GlassNum)i))
		{
			pCurInspWH->SetVacSol((GlassNum)i, OFF);
		}

		if (!IsVacSensorOn((GlassNum)i))
		{
			SetVacSol((GlassNum)i, OFF);
		}
	}

	if ((lGlassNum = CalcPickStatus(bWHGlassStatus, bArmGlassStatus)) == NO_GLASS)
	{
		strMsg.Format("Invalid Glass Status at %s and %s", GetStnName(), pCurInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPA Move To Standby Level Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("INPICKARM Move To Standby Level Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("INPICKARM Move To Pick Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if ((lGlassNum = CalcPickStatus(bWHGlassStatus, bArmGlassStatus)) == NO_GLASS)
	{
		strMsg.Format("Invalid Glass Status at %s and %s", GetStnName(), pCurInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCurInspWH->m_stMotorT.MoveAbsolute(pCurInspWH->m_lStandbyPosnT, GMP_NOWAIT) != GMP_SUCCESS)
	{
		strMsg.Format("InspWH Move To Load Offset Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCurInspWH->MoveXYToUnloadPosn() != GMP_SUCCESS)
	{
		strMsg.Format("InspWH Move To Unload Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (MoveX(PICK_INSPWH_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPICKARM Move To Pick Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (PickGlassFromInspWH(lCurWH, lGlassNum) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPICKARM Pick Glass From InspWH Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCurInspWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass1 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCurInspWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass2 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult2 = FALSE;
		}
	}	

	if (bResult1 && bResult2)
	{
		if (MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("OUTPICKARM Pick Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}
	return bResult1 && bResult2;
}

BOOL COutPickArm::PickGlassFromShbufOperation(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	//if (!pCInspWH->m_bModSelected)
	//{
	//	strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
	//	HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
	//	return FALSE;
	//}

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

	if (pCShBufferWH->MoveY(Y_CTRL_IN_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("ShBufferWH Move To In Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (MoveX(OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPICKARM Move To ShBuf Pick Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	if (PickGlassFromBuffer(lGlassNum) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPICKARM Pick Glass From ShBufferWH Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
		{
			pCShBufferWH->SetGLASS1_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass1 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (IsVacStateOff(GetGlassCOF2_VacSensorOnState()))
		{
			pCShBufferWH->SetGLASS2_VacSol(ON, GMP_WAIT);
			strMsg.Format("Pick Glass Error: %s Glass2 Vac Sensor Not On.", GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult2 = FALSE;
		}
	}	

	if (bResult1 && bResult2)
	{
		if (MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("OUTPICKARM Pick Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}

	if (pCShBufferWH->MoveY(Y_CTRL_OUT_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("ShBufferWH Move To Out Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	return bResult1 && bResult2;
}

BOOL COutPickArm::PlaceGlassToShbufOperation(GlassNum lGlassNum)
{
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");

	CString strMsg;
	
	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	//if (!pCInspWH->m_bModSelected)
	//{
	//	strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
	//	HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
	//	return FALSE;
	//}

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

	if (pCShBufferWH->MoveY(Y_CTRL_IN_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("ShBuffer WH Move To In Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (MoveX(OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPICKARM Move To ShBuffer WH Place Posn Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	if (PlaceGlassToBuffer(lGlassNum) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPICKARM Place Glass To ShBuffer WH Error. Operation Abort!");
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
			bResult1 = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{

		if (!pCShBufferWH->IsVacSensorOn(GLASS2))
		{
			SetVacSol(GLASS1, ON, GMP_WAIT);
			strMsg.Format("Place Glass Error: %s Glass2 Vac Sensor Not On.", pCShBufferWH->GetStnName());
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult2 = FALSE;
		}
	}	

	if (bResult1 && bResult2)
	{
		if (MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("OUTPICKARM Place Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}
	return bResult1 && bResult2;
}

//BOOL COutPickArm::PlaceGlassToOutBeltOperation(GlassNum lGlassNum)
//{
//	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
//	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
//
//	CString strMsg;
//	
//	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
//	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
//	BOOL bResult1 = TRUE;
//	BOOL bResult2 = TRUE;
//
//	if (!m_bModSelected)
//	{
//		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//		return FALSE;
//	}
//
//	if (!pCInspWH->m_bModSelected)
//	{
//		strMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//		return FALSE;
//	}
//
//	if (!pCShBufferWH->m_bModSelected)
//	{
//		strMsg.Format("%s Module Not Selected. Operation Abort!", pCShBufferWH->GetStnName());
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//		return FALSE;
//	}
//
//	if (lGlassNum != NO_GLASS)
//	{
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Contact Software");
//		return FALSE;
//	}
//
//	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
//	{
//		SetVacSol((GlassNum)i, ON, GMP_WAIT);
//	}
//
//	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
//	{
//		bArmGlassStatus[i] = IsVacSensorOn((GlassNum)i);
//
//		if (!IsVacSensorOn((GlassNum)i))
//		{
//			SetVacSol((GlassNum)i, OFF);
//		}
//	}
//	if (bArmGlassStatus[GLASS1] && bArmGlassStatus[GLASS2])
//	{
//		lGlassNum = MAX_NUM_OF_GLASS;
//	}
//	else if (bArmGlassStatus[GLASS1])
//	{
//		lGlassNum = GLASS1;
//	}
//	else if (bArmGlassStatus[GLASS2])
//	{
//		lGlassNum = GLASS2;
//	}
//	else
//	{
//		strMsg.Format("No Glass Exist on Pick Arm. Operation Abort!");
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//		return FALSE;
//
//	}
//	if (pCShBufferWH->SetZLoadSol(OFF, GMP_WAIT) != GMP_SUCCESS)
//	{
//		return FALSE;
//	}
//	if (pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS)
//	{
//		strMsg.Format("ShBuffer WH Move To Standby Level Error. Operation Abort!");
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//		return FALSE;
//	}
//
//
//	if (MoveX(OUTPICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT) != GMP_SUCCESS)
//	{
//		strMsg.Format("OUTPICKARM Move Place Posn Error. Operation Abort!");
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//		return FALSE;
//	}
//	if (PlaceGlassOnOutBelt() != GMP_SUCCESS || getKlocworkFalse())
//	{
//		strMsg.Format("OUTPICKARM Place Glass On OutBelt Error. Operation Abort!");
//		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//		return FALSE;
//	}
//
//	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
//	{
//		SetVacSol((GlassNum)i, ON, GMP_WAIT);
//	}
//
//	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (IsVacStateOn(GetGlassCOF1_VacSensorOnState()))
//		{
//			strMsg.Format("Place Glass Error: Glass 1 Still Exist On Pick Arm.");
//			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//			bResult1 = FALSE;
//		}
//	}	
//
//	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (IsVacStateOn(GetGlassCOF2_VacSensorOnState()))
//		{
//			strMsg.Format("Place Glass Error: Glass 2 Still Exist On Pick Arm");
//			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
//			bResult2 = FALSE;
//		}
//	}	
//
//	if (bResult1 && bResult2)
//	{
//		if (MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
//		{
//			strMsg.Format("OUTPICKARM Place Glass Go Standby Posn Error. Operation Abort!");
//			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
//			bResult1 = FALSE;
//		}
//	}
//	return bResult1 && bResult2;
//}
//
//BOOL COutPickArm::PickGlassOperation()
//{
//	BOOL bResult = FALSE;
//	LONG lResponse = HMISelectionBox("PICK GLASS OPERATION", "Please select Where to pick:", "Insp WH", "ShBuffer WH");	
//
//		if (lResponse == -1 || lResponse == 11)
//		{
//			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
//			return FALSE;
//		}
//
//		switch (lResponse)
//		{
//		case 0:
//			bResult = PickGlassFromInspWHOperation(NO_GLASS);
//			break;
//
//		case 1:
//			bResult = PickGlassFromShbufOperation(NO_GLASS);
//			break;
//		}
//	return bResult;
//}
//
//BOOL COutPickArm::PlaceGlassOperation()
//{
//	BOOL bResult = FALSE;
//	LONG lResponse = HMISelectionBox("PLACE GLASS OPERATION", "Please select Where to Place:", "Flipper WH / Out Belt", "ShBuffer WH");	
//
//		if (lResponse == -1 || lResponse == 11)
//		{
//			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Operation Abort!");
//			return FALSE;
//		}
//
//		switch (lResponse)
//		{
//		case 0:
//			if (m_bInlineInBack)
//			{
//				//Place to Flipper WH
//			}
//			else
//			{
//				bResult = PlaceGlassToOutBeltOperation(NO_GLASS);
//			}
//			break;
//
//		case 1:
//			bResult = PlaceGlassToShbufOperation(NO_GLASS);
//			break;
//
//		}
//	return bResult;
//}
//BOOL COutPickArm::IsFlushStop() //20130408
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
//
//	if (g_bFlushMode)
//	{
//		if (AutoMode == BURN_IN || AutoMode == DIAG_BOND)
//		{
//			switch (g_lGlassInputMode)
//			{
//			case GLASS_1_INPUT:
//				return TRUE;
//
//			case GLASS_2_INPUT:
//				return TRUE;
//
//			default:
//				if (m_bIsDefaultIndex) // Place Glass 2 First, then Glass 1
//				{
//					if (!pCInspWH->m_bGlass2Exist)
//					{
//						return TRUE;
//					}
//					else if (!pCInspWH->m_bGlass1Exist)
//					{
//						return FALSE;
//					}
//					else
//					{
//						return TRUE;
//					}
//				}
//				else
//				{
//					if (!pCInspWH->m_bGlass1Exist)
//					{
//						return TRUE;
//					}
//					else if (!pCInspWH->m_bGlass2Exist)
//					{
//						return FALSE;
//					}
//					else
//					{
//						return TRUE;
//					}
//				}
//			}
//		}
//		else
//		{
//			switch (g_lGlassInputMode)
//			{
//			case GLASS_1_INPUT:
//				return TRUE;
//
//			case GLASS_2_INPUT:
//				return TRUE;
//
//			default:
//			
//					if (m_bIsDefaultIndex) // Place Glass 2 First, then Glass 1
//					{
//						if (!pCInspWH->IsGLASS2_VacSensorOn())
//						{
//							return TRUE;
//						}
//						else if (!pCInspWH->IsGLASS1_VacSensorOn())
//						{
//							return FALSE;
//						}
//						else
//						{
//							return TRUE;
//						}
//					}
//					else 
//					{
//
//						if (!pCInspWH->IsGLASS1_VacSensorOn())
//						{
//							return TRUE;
//						}
//						else if (!pCInspWH->IsGLASS2_VacSensorOn())
//						{
//							return FALSE;
//						}
//						else
//						{
//							return TRUE;
//						}
//					}
//				break;
//
//			}
//		}
//	}
//	return FALSE;
//}
//
GlassNum COutPickArm::AssignPlaceToWhere(LONG lCurWH)
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurInspWH;

	if(lCurWH == WH_1)
	{
		pCurInspWH = pCWH1;
	}
	else
	{
		pCurInspWH = pCWH2;
	}


	if (AutoMode == BURN_IN || AutoMode == DIAG_BOND)
	{
		switch (g_lGlassInputMode)
		{
		case GLASS_1_INPUT:

			if (!pCurInspWH->m_bGlass1Exist)
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		case GLASS_2_INPUT:

			if (!pCurInspWH->m_bGlass2Exist)
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
				if (!pCurInspWH->m_bGlass2Exist)
				{
					return GLASS2;
				}
				else if (!pCurInspWH->m_bGlass1Exist)
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
				if (!pCurInspWH->m_bGlass1Exist)
				{
					return GLASS1;
				}
				else if (!pCurInspWH->m_bGlass2Exist)
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

			if (!pCurInspWH->IsGLASS1_VacSensorOn())
			{
				return GLASS1;
			}
			else
			{
				return NO_GLASS;
			}

			break;

		case GLASS_2_INPUT:

			if (!pCurInspWH->IsGLASS2_VacSensorOn())
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
				if (!pCurInspWH->IsGLASS2_VacSensorOn())
				{
					return GLASS2;
				}
				else if (!pCurInspWH->IsGLASS1_VacSensorOn())
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

				if (!pCurInspWH->IsGLASS1_VacSensorOn())
				{
					return GLASS1;
				}
				else if (!pCurInspWH->IsGLASS2_VacSensorOn())
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

//GlassNum COutPickArm::AssignPlaceToWhere_edge_cleaner()
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
//
//	if (AutoMode == BURN_IN || AutoMode == DIAG_BOND)
//	{
//#if 1
//		if (m_bGlass1Exist && m_bGlass2Exist)
//		{ 
//		//11
//			if (!pCInspWH->m_bGlass1Exist && !pCInspWH->m_bGlass2Exist)
//			{
//				return MAX_NUM_OF_GLASS;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//		}
//		else if (m_bGlass1Exist && !m_bGlass2Exist)
//		{ 
//		//10
//			if (!pCInspWH->m_bGlass1Exist)
//			{
//				return GLASS1;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//		}
//		else if (!m_bGlass1Exist && m_bGlass2Exist)
//		{ 
//		//01
//			if (!pCInspWH->m_bGlass2Exist)
//			{
//				return GLASS2;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//		}
//		else
//		{ 
//		//00
//			return NO_GLASS;
//		}
//#else
//		switch (g_lGlassInputMode)
//		{
//		case GLASS_1_INPUT:
//
//			if (!pCInspWH->m_bGlass1Exist)
//			{
//				return GLASS1;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//
//			break;
//
//		case GLASS_2_INPUT:
//
//			if (!pCInspWH->m_bGlass2Exist)
//			{
//				return GLASS2;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//
//			break;
//
//		default:
//			if (!pCInspWH->m_bGlass1Exist && !pCInspWH->m_bGlass2Exist)
//			{
//				return MAX_NUM_OF_GLASS;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//			break;
//		}
//#endif
//	}
//	else
//	{
//#if 1
//		if (m_bGlass1Exist && m_bGlass2Exist)
//		{
//			if (!pCInspWH->IsGLASS1_VacSensorOn() && !pCInspWH->IsGLASS2_VacSensorOn())
//			{
//				return MAX_NUM_OF_GLASS;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//		}
//		else if (m_bGlass1Exist)
//		{
//			if (!pCInspWH->IsGLASS1_VacSensorOn())
//			{
//				return GLASS1;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//		}
//		else if (m_bGlass2Exist)
//		{
//			if (!pCInspWH->IsGLASS2_VacSensorOn())
//			{
//				return GLASS2;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//		}
//		else //no glass
//		{
//			return NO_GLASS;
//		}
//#else
//		switch (g_lGlassInputMode)
//		{
//		case GLASS_1_INPUT:
//
//			if (!pCInspWH->IsGLASS1_VacSensorOn())
//			{
//				return GLASS1;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//
//			break;
//
//		case GLASS_2_INPUT:
//
//			if (!pCInspWH->IsGLASS2_VacSensorOn())
//			{
//				return GLASS2;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//
//			break;
//
//		default:
//			if (!pCInspWH->IsGLASS1_VacSensorOn() && !pCInspWH->IsGLASS2_VacSensorOn())
//			{
//				return MAX_NUM_OF_GLASS;
//			}
//			else
//			{
//				return NO_GLASS;
//			}
//			break;
//		}
//#endif
//	}
//
//	return NO_GLASS;
//}
//
//
//
//
//
//INT COutPickArm::PickGlass_DOWN()
//{
//#if 1 //20121208
//	if (
//		(SetGLASS2_VacSol(ON, GMP_WAIT) != GMP_SUCCESS)//PI9000
//	   )
//	{
//		return GMP_FAIL;
//	}
//#else
//	if (
//		(SetGLASS2_VacSol(ON, GMP_WAIT) != GMP_SUCCESS) ||
//	   )
//	{
//		return GMP_FAIL;
//	}
//#endif
//	return GMP_SUCCESS;
//}
//
//INT COutPickArm::PickGlass_UP()
//{
//	INT nResult = GMP_SUCCESS;
//	
//	if (m_bIsOfflineMode)
//	{
//		m_bGlass1Exist = TRUE;
//	}
//
//	return GMP_SUCCESS;
//
//}
//
//INT COutPickArm::PlaceGlass(GlassNum lGlassNum) // ONLY ONE PICK PLACE EACH TIME
//{
//	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
//
//	LONG lDelay;
//
//	if (lGlassNum <= NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
//	{
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		DisplayMessage("COutPickArm::PlaceGlass : SW Error");
//		return GMP_FAIL;
//	}
//
//#if 1 //debug only 20121030
//	if (HMI_bDebugSeq)	// 20140801 Yip: Only Show Message When Debug Sequence
//	{
//		if (lGlassNum == GLASS1)
//		{
//			DisplayMessage("COutPickArm::PlaceGlass 1");
//		}
//		else if (lGlassNum == GLASS2)
//		{
//			DisplayMessage("COutPickArm::PlaceGlass 2");
//		}
//	}
//#endif
//	pCInspWH->SetVacSol(lGlassNum, ON);
//
//	if ((m_nLastError = pCInspWH->MoveZToLoadLevel(lGlassNum, GMP_WAIT)) == GMP_SUCCESS)
//	{
//		lDelay = pCInspWH->m_stGlass1VacSol.GetOnDelay();
//		if (m_stGlass2VacSol.GetOffDelay() > lDelay)
//		{
//			lDelay = m_stGlass2VacSol.GetOffDelay();
//		}
//		SetGLASS2_VacSol(OFF, GMP_WAIT);
//		Sleep(lDelay);
//
//		if (m_stWeakBlowSol.GetOnDelay() > 0)
//		{
//			SetWeakBlowSol(ON, GMP_WAIT);
//			SetWeakBlowSol(OFF, GMP_WAIT);
//		}
//
//	}
//
//	return GMP_FAIL;
//}
//
///////////////////////////////////////////////////////////////////
////Motor Related
///////////////////////////////////////////////////////////////////
INT COutPickArm::MoveX(OUTPICKARM_X_CTRL_SIGNAL ctrl, BOOL bWait)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	INT		nResult		= GMP_SUCCESS;

	LONG lTempPosn = 0;

	CString szDebug = _T("");
	switch (ctrl)
	{
	case OUTPICKARM_X_CTRL_GO_STANDBY_POSN:
		OUTPICKARM_ARM_Posn = OUTPAStandby_POSN;
		lTempPosn = m_lStandbyPosn;
		break;

	case OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_POSN:
		OUTPICKARM_ARM_Posn = WH1_POSN;
		lTempPosn = m_lPickGlassPosn[WH_1];
		break;

	case OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_POSN:
		OUTPICKARM_ARM_Posn = WH2_POSN;
		lTempPosn = m_lPickGlassPosn[WH_2];
		break;

	case OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_OFFSET:
		OUTPICKARM_ARM_Posn = WH1_POSN;
		lTempPosn = m_lPickGlassPosn[WH_1] + m_lPickGlassOffset[WH_1];
		break;

	case OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_OFFSET:
		OUTPICKARM_ARM_Posn = WH2_POSN;
		lTempPosn = m_lPickGlassPosn[WH_2] + m_lPickGlassOffset[WH_2];
		break;

	case OUTPICKARM_X_CTRL_GO_PLACE_GLASS_POSN:
		OUTPICKARM_ARM_Posn = OUTPAStandby_POSN;
		lTempPosn = m_lPlaceGlassPosn;
		break;

	case OUTPICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET: 
		OUTPICKARM_ARM_Posn = OUTPAStandby_POSN;
		lTempPosn = m_lPlaceGlassPosn + m_lPlaceGlassOffset;
		break;

	case OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_POSN:
		OUTPICKARM_ARM_Posn = BUFWH_POSN;
		lTempPosn = m_lShBufPlaceGlassPosn;
		break;

	case OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET: //20121011
		OUTPICKARM_ARM_Posn = BUFWH_POSN;
		lTempPosn = m_lShBufPlaceGlassPosn + m_lShBufPlaceGlassOffset;
		break;

	case OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_POSN:
		OUTPICKARM_ARM_Posn = BUFWH_POSN;
		lTempPosn = m_lRejBeltPlaceGlassPosn[GLASS1];
		break;

	case OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_POSN:
		OUTPICKARM_ARM_Posn = BUFWH_POSN;
		lTempPosn = m_lRejBeltPlaceGlassPosn[GLASS2];
		break;

	case OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_OFFSET: //20121011
		OUTPICKARM_ARM_Posn = BUFWH_POSN;
		lTempPosn = m_lRejBeltPlaceGlassPosn[GLASS1] + m_lRejBeltPlaceGlassOffset[GLASS1];
		break;

	case OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_OFFSET: //20121011
		OUTPICKARM_ARM_Posn = BUFWH_POSN;
		lTempPosn = m_lRejBeltPlaceGlassPosn[GLASS2] + m_lRejBeltPlaceGlassOffset[GLASS2];
		break;
	}

	if (HMI_bDebugCal)	// 20140801 Yip: Only Show Message When Debug Calculation
	{
		szDebug.Format("MotorX Posn: %d", lTempPosn);
		DisplayMessage(szDebug);
	}
	return m_stMotorX.MoveAbsolute(lTempPosn, bWait);
}

INT COutPickArm::MoveZ(OUTPICKARM_Z_CTRL_SIGNAL ctrl, BOOL bWait)
{
	INT		nResult		= GMP_SUCCESS;


	DOUBLE	dThickness	= g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
	LONG lTempPosn = 0;
	DOUBLE dDistPerCnt = 0.0;
	DOUBLE dGap = 0.0;

	dDistPerCnt	= m_stMotorZ.stAttrib.dDistPerCount;

	switch (ctrl)
	{
	case OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL:
		lTempPosn = m_lStandbyLevel;
		break;

	case OUTPICKARM_Z_CTRL_GO_PICK_GLASS_LEVEL:
		lTempPosn = m_lPickLevel;
		break;

	case OUTPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET:
		//if (IsBurnInDiagBond())
		//{
		//	dGap = DistanceToCount(GAP_THICKNESS * FPC_GAP * 1000.0, dDistPerCnt); 
		//}
		//else
		//{
		//	dGap = DistanceToCount(dThickness * 1000.0, dDistPerCnt); 
		//}
		lTempPosn = m_lPickLevel + m_lPickLevelOffset + (LONG)dGap;
		break;

	case OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_LEVEL:
		lTempPosn = m_lPlaceLevel;
		break;

	case OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_OFFSET:
		//if (IsBurnInDiagBond())
		//{
		//	dGap = DistanceToCount(GAP_THICKNESS * FPC_GAP * 1000.0, dDistPerCnt); 
		//}
		//else
		//{
		//	dGap = DistanceToCount(dThickness * 1000.0, dDistPerCnt); 
		//}
		lTempPosn = m_lPlaceLevel + m_lPlaceLevelOffset + (LONG)dGap;
		break;

	case OUTPICKARM_Z_CTRL_GO_SHBUF_PLACE_GLASS_LEVEL:
		lTempPosn = m_lShBufPlaceLevel;
		break;

	case OUTPICKARM_Z_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET:
		if (IsBurnInDiagBond())
		{
			dGap = DistanceToCount(GAP_THICKNESS * FPC_GAP * 1000.0, dDistPerCnt); 
		}
		else
		{
			dGap = DistanceToCount(dThickness * 1000.0, dDistPerCnt); 
		}
		lTempPosn = m_lShBufPlaceLevel + m_lShBufPlaceLevelOffset + (LONG)dGap;
		break;

	case OUTPICKARM_Z_CTRL_GO_SHBUF_PICK_GLASS_LEVEL:
		lTempPosn = m_lShBufPlaceLevel; 
		break;

	case OUTPICKARM_Z_CTRL_GO_SHBUF_PICK_GLASS_OFFSET:
		if (IsBurnInDiagBond())
		{
			dGap = DistanceToCount(GAP_THICKNESS * FPC_GAP * 1000.0, dDistPerCnt); 
		}
		else
		{
			dGap = DistanceToCount(dThickness * 1000.0, dDistPerCnt); 
		}
		lTempPosn = m_lShBufPlaceLevel + m_lShBufPlaceLevelOffset + (LONG)dGap;
		break;

	case OUTPICKARM_Z_CTRL_GO_REJBELT_PLACE_GLASS_LEVEL:
		lTempPosn = m_lRejBeltPlaceLevel;
		break;

	case OUTPICKARM_Z_CTRL_GO_REJBELT_PLACE_GLASS_OFFSET:
		if (IsBurnInDiagBond())
		{
			dGap = DistanceToCount(GAP_THICKNESS * FPC_GAP * 1000.0, dDistPerCnt); 
		}
		else
		{
			dGap = DistanceToCount(dThickness * 1000.0, dDistPerCnt); 
		}
		lTempPosn = m_lRejBeltPlaceLevel + m_lRejBeltPlaceLevelOffset - (LONG)dGap;
		break;

	}

	return m_stMotorZ.MoveAbsolute(lTempPosn, GMP_WAIT);
}
//
///////////////////////////////////////////////////////////////////
////I/O Related
///////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////
////SETUP Related
///////////////////////////////////////////////////////////////////
VOID COutPickArm::ModuleSetupPreTask()
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurInspWH;

	if(m_lCurInspWH == WH_1)
	{
		pCurInspWH = pCWH1;
	}
	else
	{
		pCurInspWH = pCWH2;
	}

	pCurInspWH->m_bCheckMotor	= TRUE;
	pCurInspWH->m_bCheckIO		= TRUE;

	m_bCheckMotor		= TRUE;
	m_bCheckIO			= TRUE;
	
	CheckModSelected(TRUE);
	
	SetDiagnSteps(g_lDiagnSteps);

	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();
}

VOID COutPickArm::ModuleSetupPostTask()
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurInspWH;

	if(m_lCurInspWH == WH_1)
	{
		pCurInspWH = pCWH1;
	}
	else
	{
		pCurInspWH = pCWH2;
	}
	pCurInspWH->m_bCheckMotor	= FALSE;
	pCurInspWH->m_bCheckIO		= FALSE;

	m_bCheckMotor		= FALSE;
	m_bCheckIO			= FALSE;

}

BOOL COutPickArm::ModuleSetupSetPosn(LONG lSetupTitle)
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
		mtrOld.x			= m_lPickGlassPosn[WH_1];

		m_lPickGlassPosn[WH_1]	= TempPosn.x;

		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Pick Glass Offset?");
		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPickGlassOffset[WH_1] = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
		}
		break;

	case 2:
		mtrOld.x			= m_lPickGlassOffset[WH_1];

		m_lPickGlassOffset[WH_1]	= TempPosn.x - m_lPickGlassPosn[WH_1]; //20121225
		TempPosn.x			= m_lPickGlassOffset[WH_1];
		break;

	case 3:
		mtrOld.x			= m_lPickGlassPosn[WH_2];

		m_lPickGlassPosn[WH_2]	= TempPosn.x;

		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Pick Glass Offset?");
		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPickGlassOffset[WH_2] = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
		}
		break;

	case 4:
		mtrOld.x			= m_lPickGlassOffset[WH_2];

		m_lPickGlassOffset[WH_2]	= TempPosn.x - m_lPickGlassPosn[WH_2]; //20121225
		TempPosn.x			= m_lPickGlassOffset[WH_2];
		break;

	case 5:
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

	case 6:
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

	case 7: 
		mtrOld.x			= m_lPlaceGlassOffset;

		m_lPlaceGlassOffset	= TempPosn.x - m_lPlaceGlassPosn;

		TempPosn.x			= m_lPlaceGlassOffset;
		break;

	case 8: 
		mtrOld.x			= m_lShBufPlaceGlassOffset;

		m_lShBufPlaceGlassOffset	= TempPosn.x - m_lShBufPlaceGlassPosn;

		TempPosn.x			= m_lShBufPlaceGlassOffset;
		break;

	case 9: 
		mtrOld.x			= m_lRejBeltPlaceGlassPosn[GLASS1];

		m_lRejBeltPlaceGlassPosn[GLASS1]			= TempPosn.x;
		break;

	case 10: 
		mtrOld.x			= m_lRejBeltPlaceGlassPosn[GLASS2];

		m_lRejBeltPlaceGlassPosn[GLASS2]			= TempPosn.x;
		break;

	case 11: 
		mtrOld.x			= m_lRejBeltPlaceGlassOffset[GLASS1];

		m_lRejBeltPlaceGlassOffset[GLASS1]	= TempPosn.x - m_lRejBeltPlaceGlassPosn[GLASS1];

		TempPosn.x			= m_lRejBeltPlaceGlassOffset[GLASS1];
		break;

	case 12: 
		mtrOld.x			= m_lRejBeltPlaceGlassOffset[GLASS2];

		m_lRejBeltPlaceGlassOffset[GLASS2]	= TempPosn.x - m_lRejBeltPlaceGlassPosn[GLASS2];

		TempPosn.x			= m_lRejBeltPlaceGlassOffset[GLASS2];
		break;
	
		default:
		break;
	}

	CPI9000Stn::ModuleSetupSetPosn(lSetupTitle, "X", mtrOld.x, TempPosn.x);

	return TRUE;
}

BOOL COutPickArm::ModuleSetupGoPosn(LONG lSetupTitle)
{
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm");
	INT nResult = GMP_SUCCESS;
	CString szMsg;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "GO POSN OPERATION", szMsg);
		return FALSE;
	}
	if (!pCInPickArm->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "GO POSN OPERATION", szMsg);
		return FALSE;
	}

	if (
			pCInPickArm->MoveZToStandbyLevel(GMP_WAIT) != GMP_SUCCESS ||
			MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS
		)	// 20150327
	{
		return FALSE;
	}

	switch (lSetupTitle)
	{
	case 0:
		nResult = SetupGoStandbyPosn();
		break;
	
	case 1:
		nResult = SetupGoPickGlassPosn(WH_1);
		break;
		
	case 2:
		nResult = SetupGoPickGlassOffset(WH_1);
		break;

	case 3:
		nResult = SetupGoPickGlassPosn(WH_2);
		break;
		
	case 4:
		nResult = SetupGoPickGlassOffset(WH_2);
		break;

	case 5:
		nResult = SetupGoPlaceGlassPosn();
		break;

	case 6:
		nResult = SetupGoShBufPlaceGlassPosn();
		break;

	case 7: 
		nResult = SetupGoPlaceGlassOffset();
		break;

	case 8: 
		nResult = SetupGoShBufPlaceGlassOffset();
		break;

	case 9: 
		nResult = SetupGoPlaceRejGlassPosn(GLASS1);
		break;

	case 10: 
		nResult = SetupGoPlaceRejGlassPosn(GLASS2);
		break;

	case 11: 
		nResult = SetupGoPlaceRejGlassOffset(GLASS1);
		break;

	case 12: 
		nResult = SetupGoPlaceRejGlassOffset(GLASS2);
		break;
	}

	return nResult;
}

BOOL COutPickArm::ModuleSetupSetLevel(LONG lSetupTitle)
{
	LONG lAnswer = rMSG_TIMEOUT;
	LONG lOldLevel		= 0;
	LONG lPrePostLevel	= 0;
	LONG lTempLevel = 0;
	BOOL bResult = TRUE;
	DOUBLE	dThickness	= 0.0;
	DOUBLE	dDistPerCnt	= 0.0;

	if (	g_ulUserGroup < SERVICE &&
		g_stSetupLevel[lSetupTitle].lType == MACHINE_PARA)
	{
		HMIMessageBox(MSG_OK, "WARNING", "You are not allowed to change Machine Parameters");
		return FALSE;
	}

	lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Position?");

	if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT)
	{
		return FALSE;
	}

	lTempLevel = m_stMotorZ.GetEncPosn();
	dDistPerCnt	= m_stMotorZ.stAttrib.dDistPerCount;

	switch (lSetupTitle)
	{
	case 0:
		lOldLevel					= m_lStandbyLevel;

		m_lStandbyLevel				= lTempLevel;
		break;

	case 1:
		lOldLevel					= m_lPickLevel;

		m_lPickLevel				= lTempLevel;
		
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Pick Level Offset?");

		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPickLevelOffset = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
			break;
		}
		break;

	case 2:
		lOldLevel			= m_lPickLevelOffset;

		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
		
		m_lPickLevelOffset	= lTempLevel - m_lPickLevel - DistanceToCount(1000 * dThickness, dDistPerCnt);
			
		lTempLevel	= m_lPickLevelOffset;

		//lOldLevel					= m_lPickLevelOffset;

		//m_lPickLevelOffset			= lTempLevel;
		break;

	case 3:
		lOldLevel					= m_lPlaceLevel;

		m_lPlaceLevel				= lTempLevel;
		
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Place Level Offset?");

		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPlaceLevelOffset = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
			break;
		}
		break;

	case 4:
		lOldLevel			= m_lPlaceLevelOffset;

		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
		
		m_lPlaceLevelOffset	= lTempLevel - m_lPlaceLevel - DistanceToCount(1000 * dThickness, dDistPerCnt);
			
		lTempLevel	= m_lPlaceLevelOffset;
			
		//lOldLevel					= m_lPlaceLevelOffset;

		//m_lPlaceLevelOffset			= lTempLevel;
		break;

	case 5:
		lOldLevel					= m_lShBufPlaceLevel;

		m_lShBufPlaceLevel				= lTempLevel;
		break;

	case 6:
		lOldLevel			= m_lShBufPlaceLevelOffset;

		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
		
		m_lShBufPlaceLevelOffset	= lTempLevel - m_lShBufPlaceLevel - DistanceToCount(1000 * dThickness, dDistPerCnt);
			
		lTempLevel	= m_lShBufPlaceLevelOffset;

		//lOldLevel					= m_lShBufPlaceLevelOffset;;

		//m_lShBufPlaceLevelOffset			= lTempLevel;
		break;

	case 7:
		lOldLevel					= m_lRejBeltPlaceLevel;

		m_lRejBeltPlaceLevel				= lTempLevel;
		break;

	case 8:
		lOldLevel			= m_lRejBeltPlaceLevelOffset;

		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
		
		m_lRejBeltPlaceLevelOffset	= lTempLevel - m_lRejBeltPlaceLevel - DistanceToCount(1000 * dThickness, dDistPerCnt);
			
		lTempLevel	= m_lRejBeltPlaceLevelOffset;

		//lOldLevel					= m_lRejBeltPlaceLevelOffset;

		//m_lRejBeltPlaceLevelOffset			= lTempLevel;
		break;
	}

	CPI9000Stn::ModuleSetupSetLevel(lSetupTitle, lOldLevel, lTempLevel);

	return bResult;
}

BOOL COutPickArm::ModuleSetupSetPara(PARA stPara)
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
		m_stGlass2WeakBlowSol.SetOnDelay(stPara.lPara);
		break;

	case 3:
		m_stWeakBlowSol.SetOffDelay(stPara.lPara);
		m_stGlass2WeakBlowSol.SetOffDelay(stPara.lPara);
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
	
VOID COutPickArm::UpdateModuleSetupPosn()
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
	g_stSetupPosn[i].szPosn.Format("Calib Pick WH1 Glass Posn");	
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPickGlassPosn[WH_1];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 2
	g_stSetupPosn[i].szPosn.Format("Pick WH1 Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPickGlassOffset[WH_1];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 3
	g_stSetupPosn[i].szPosn.Format("Calib Pick WH2 Glass Posn");	
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPickGlassPosn[WH_2];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 4
	g_stSetupPosn[i].szPosn.Format("Pick WH2 Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPickGlassOffset[WH_2];
	g_stSetupPosn[i].lPosnY			= 0;


	i++;	// 5
	g_stSetupPosn[i].szPosn.Format("Place Glass Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassPosn;
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 6
	g_stSetupPosn[i].szPosn.Format("Pick / Place Glass Posn(ShBuffer)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lShBufPlaceGlassPosn; //20121011
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 7
	g_stSetupPosn[i].szPosn.Format("Place Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassOffset;
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 8 
	g_stSetupPosn[i].szPosn.Format("Pick / Place Glass Offset(ShBuffer)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lShBufPlaceGlassOffset;
	g_stSetupPosn[i].lPosnY			= 0;


	i++;	// 9
	g_stSetupPosn[i].szPosn.Format("Place Glass1 Posn(Reject Belt)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lRejBeltPlaceGlassPosn[GLASS1];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 10
	g_stSetupPosn[i].szPosn.Format("Place Glass2 Posn(Reject Belt)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lRejBeltPlaceGlassPosn[GLASS2]; //20121011
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 11
	g_stSetupPosn[i].szPosn.Format("Place Glass1 Offset(Reject Belt)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lRejBeltPlaceGlassOffset[GLASS1];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 12 
	g_stSetupPosn[i].szPosn.Format("Place Glass2 Offset(Reject Belt)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lRejBeltPlaceGlassOffset[GLASS2];
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

VOID COutPickArm::UpdateModuleSetupLevel()
{
	INT i = 0;
	INT j = 0;

	// Level Tab
	i = 0;
	g_stSetupLevel[i].szLevel.Format("Standby Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lStandbyLevel;

	i++;	// 1
	g_stSetupLevel[i].szLevel.Format("Pick Glass Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPickLevel;

	i++;	// 2
	g_stSetupLevel[i].szLevel.Format("Pick Glass Level Offset");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= DEVICE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPickLevelOffset;

	i++;	// 3
	g_stSetupLevel[i].szLevel.Format("Place Glass Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevel;

	i++;	// 4
	g_stSetupLevel[i].szLevel.Format("Place Glass Level Offset");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= DEVICE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevelOffset;

	i++;	// 5
	g_stSetupLevel[i].szLevel.Format("Place BufferWH Glass Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lShBufPlaceLevel;

	i++;	// 6
	g_stSetupLevel[i].szLevel.Format("Place BufferWH Glass Level Offset");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= DEVICE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lShBufPlaceLevelOffset;

	i++;	// 7
	g_stSetupLevel[i].szLevel.Format("Place Reject Belt Glass Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lRejBeltPlaceLevel;

	i++;	// 8
	g_stSetupLevel[i].szLevel.Format("Place Reject Belt Glass Level Offset");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= DEVICE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lRejBeltPlaceLevelOffset;
	

	
	for (j = i + 1; j < NUM_OF_LEVEL_Z; j++)
	{
		g_stSetupLevel[j].szLevel.Format("Reserved");
		g_stSetupLevel[j].bEnable		= FALSE;
		g_stSetupLevel[j].bVisible		= FALSE;
		g_stSetupLevel[j].lType			= MACHINE_PARA;
		g_stSetupLevel[j].lLevelZ		= 0;
	}
}

VOID COutPickArm::UpdateModuleSetupPara()
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

INT COutPickArm::SetupGoStandbyPosn()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoPlaceRejGlassPosn(LONG lCurGlass)
{
	INT nResult = GMP_SUCCESS;


	if (lCurGlass == GLASS1)
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_POSN, GMP_WAIT);
	}
	else
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_POSN, GMP_WAIT);
	}

	return nResult;
}

INT COutPickArm::SetupGoPlaceRejGlassOffset(LONG lCurGlass)
{
	INT nResult = GMP_SUCCESS;


	if (lCurGlass == GLASS1)
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_OFFSET, GMP_WAIT);
	}
	else
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_OFFSET, GMP_WAIT);
	}

	return nResult;
}

INT COutPickArm::SetupGoPickGlassPosn(LONG lCurWH)
{
	INT nResult = GMP_SUCCESS;


	if (lCurWH == WH_1)
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_POSN, GMP_WAIT);
	}
	else
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_POSN, GMP_WAIT);
	}

	return nResult;
}

INT COutPickArm::SetupGoPickGlassOffset(LONG lCurWH)
{
	INT nResult = GMP_SUCCESS;

	if (lCurWH == WH_1)
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_OFFSET, GMP_WAIT);
	}
	else
	{
	nResult = MoveX(OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_OFFSET, GMP_WAIT);
	}


	return nResult;
}

INT COutPickArm::SetupGoPlaceGlassPosn()
{
	INT nResult = GMP_SUCCESS;
//	LONG lAnswer = rMSG_CANCEL;

	nResult = MoveX(OUTPICKARM_X_CTRL_GO_PLACE_GLASS_POSN, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoShBufPlaceGlassPosn()
{

	INT nResult = GMP_SUCCESS;
	//LONG lAnswer = rMSG_CANCEL;

	nResult = MoveX(OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_POSN, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoPlaceGlassOffset() 
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveX(OUTPICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoShBufPlaceGlassOffset() //20121011
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveX(OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}



BOOL COutPickArm::ModuleSetupGoLevel(LONG lSetupTitle)
{
	INT nResult = GMP_SUCCESS;
	CString szMsg;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "GO POSN OPERATION", szMsg);
		return FALSE;
	}

	switch (lSetupTitle)
	{
	case 0:
		nResult = SetupGoStandbyLevel();
		break;

	case 1:
		nResult = SetupGoPickLevel();
		break;

	case 2:
		nResult = SetupGoPickLevelOffset();	
		break;

	case 3:
		nResult = SetupGoPlaceLevel();
		break;

	case 4:
		nResult = SetupGoPlaceLevelOffset();	
		break;

	case 5:
		nResult = SetupGoBufferPlaceLevel();
		break;

	case 6:
		nResult = SetupGoBufferPlaceLevelOffset();	
		break;

	case 7:
		nResult = SetupGoRejBeltPlaceLevel();
		break;

	case 8:
		nResult = SetupGoRejBeltPlaceLevelOffset();	
		break;
	}

	if (nResult != GMP_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}

INT COutPickArm::SetupGoStandbyLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoPickLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_PICK_GLASS_LEVEL, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoPickLevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoPlaceLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_LEVEL, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoPlaceLevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoBufferPlaceLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_SHBUF_PLACE_GLASS_LEVEL, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoBufferPlaceLevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoRejBeltPlaceLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_REJBELT_PLACE_GLASS_LEVEL, GMP_WAIT);	

	return nResult;
}

INT COutPickArm::SetupGoRejBeltPlaceLevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(OUTPICKARM_Z_CTRL_GO_REJBELT_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

BOOL COutPickArm::PickGlassOperation()
{
	BOOL bResult = FALSE;
	LONG lResponse = HMISelectionBox("PICK GLASS OPERATION", "Please select Where to pick:", "WH1", "WH2", "ShBuffer WH");	

		if (lResponse == -1 || lResponse == 11)
		{
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
			return FALSE;
		}

		switch (lResponse)
		{
		case 0:
			bResult = PickGlassFromInspWHOperation(WH_1, NO_GLASS);
			break;

		case 1:
			bResult = PickGlassFromInspWHOperation(WH_2, NO_GLASS);
			break;

		case 2:
			bResult = PickGlassFromShbufOperation(NO_GLASS);
			break;
		}
	return bResult;
}

BOOL COutPickArm::PlaceGlassOperation()
{
	BOOL bResult = FALSE;
	LONG lResponse = HMISelectionBox("PLACE GLASS OPERATION", "Please select Where to Place:", "Flipper WH / Out Belt", "ShBuffer WH", "Reject Belt");	

		if (lResponse == -1 || lResponse == 11)
		{
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Operation Abort!");
			return FALSE;
		}

		switch (lResponse)
		{
		case 0:
			//if (m_bInlineInBack)
			//{
			//	//Place to Flipper WH
			//}
			//else
			{
				bResult = PlaceGlassToOutBeltOperation(NO_GLASS);
			}
			break;

		case 1:
			bResult = PlaceGlassToShbufOperation(NO_GLASS);
			break;

		case 2:
			bResult = PlaceGlassToRejBeltOperation(NO_GLASS);
			break;

		}
	return bResult;
}

LONG COutPickArm::HMI_PickGlassOperation(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bOpDone						= TRUE;
	BOOL bResult						= FALSE;


	bResult = PickGlassOperation();
	if (bResult)
	{
		MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG COutPickArm::HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bOpDone						= TRUE;
	LONG lResponse						= 0;
	BOOL bResult						= TRUE;

	bResult = PlaceGlassOperation();

	if (bResult)
	{
		MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

INT COutPickArm::KeepQuiet()
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

VOID COutPickArm::PrintMachineSetup(FILE *fp)
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

VOID COutPickArm::PrintDeviceSetup(FILE *fp)
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
BOOL COutPickArm::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;

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


//LONG COutPickArm::HMI_PickGlassOperation(IPC_CServiceMessage &svMsg)
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	BOOL bOpDone						= TRUE;
//	BOOL bResult						= FALSE;
//
//
//	bResult = PickGlassOperation();
//	if (bResult)
//	{
//		MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
//	}
//
//	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
//	return 1;
//}
//
//LONG COutPickArm::HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg)
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	BOOL bOpDone						= TRUE;
//	LONG lResponse						= 0;
//	BOOL bResult						= TRUE;
//
//	bResult = PlaceGlassOperation();
//
//	if (bResult)
//	{
//		MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
//	}
//	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
//	return 1;
//}

//LONG COutPickArm::HMI_SetPlaceGlassPrority(IPC_CServiceMessage &svMsg)
//{
//	LONG lGlassType = HMISelectionBox("PLACE GLASS PRORITY", "Place at where first", "GLASS1", "GLASS2");
//	switch (lGlassType)
//	{
//	case 0:
//		m_bIsDefaultIndex = FALSE;		
//		break;
//	case 1:
//		m_bIsDefaultIndex = TRUE;
//		break;
//	}
//
//	return 0;
//}


//LONG COutPickArm::HMI_SetStandAlone(IPC_CServiceMessage &svMsg)
//{
//	BOOL bMode;
//	svMsg.GetMsg(sizeof(BOOL), &bMode);
//	
//	if (bMode)
//	{
//		HMI_bStandAlone = TRUE;
//	}
//	else
//	{
//		HMI_bStandAlone = FALSE;
//	}
//#if 1
//	{
//		INT nIndex = 0;
//		CString szMsg = _T(" ");
//		const char szTxt1[] = _T("GC902_PickGlass1");
//		const char szTxt2[] = _T("GC902_PickGlass2");
//		const char szTxtBoth[] = _T("GC902_PickGlass1_2_123456789");
//		char *pCmd = NULL;
//
//		pCmd = (char*)&szTxtBoth[0];
//
//	//nIndex = sizeof(szTxtBoth);
//		nIndex = strlen(pCmd) + 1;
//		szMsg.Format("sizeof=%d", nIndex);
//		DisplayMessage(szMsg);
//	}
//#endif
//	return 0;
//}

//LONG COutPickArm::IPC_SaveMachineParam()
//{	
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]	= m_lShBufPlaceGlassPosn;
//
//	pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bInlineInBack"]		= m_bInlineInBack; 
//	return CTransferArm::IPC_SaveMachineParam();
//}
//
//LONG COutPickArm::IPC_LoadMachineParam()
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	
//	m_lShBufPlaceGlassPosn	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]; 
//
//	m_bInlineInBack			= (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bInlineInBack"];
//
//	return CTransferArm::IPC_LoadMachineParam();
//}
//
//LONG COutPickArm::IPC_SaveDeviceParam()
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	CString szTmp;
//	szTmp = GetStnName();
//	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"]		= m_lPickGlassOffset;
//	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]		= m_lPlaceGlassOffset; 
//	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]	= m_lShBufPlaceGlassOffset; 
//	return CTransferArm::IPC_SaveDeviceParam();
//}
//
//LONG COutPickArm::IPC_LoadDeviceParam()
//{
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	
//	m_lPickGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"];
//	m_lPlaceGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]; 
//	m_lShBufPlaceGlassOffset	= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]; 
//
//	return CTransferArm::IPC_LoadDeviceParam();
//}
//
//
//INT COutPickArm::MoveRelatedModuleToStandby()	// 20150327
//{
//	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
//	
//	LONG lAnswer = rMSG_TIMEOUT;
//	INT nResult = GMP_SUCCESS;
//	CString szMsg = "";
//
//	if (!m_bModSelected)
//	{
//		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
//		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", szMsg);
//		return GMP_FAIL;
//	}
//
//	if (pCInspWH->m_fHardware) //20140603
//	{
//		pCInspWH->CheckModSelected(TRUE);
//	}
//	if (pCInspWH->m_fHardware && !pCInspWH->m_bModSelected)
//	{
//		if (g_ulUserGroup <= SERVICE)
//		{
//			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "INSPWH Module Not Selected. Operation Abort!");
//		}
//		else
//		{
//			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "INSPWH Module Not Selected. Continue Operation?");
//		}
//
//		if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT || g_ulUserGroup <= SERVICE)
//		{
//			return GMP_FAIL;
//		}
//	}
//
//	if (
//		pCInspWH->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS ||
//		pCInspWH->MoveZ(WHZ_1,INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
//		pCInspWH->MoveZ(WHZ_2,INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS
//	   )
//	{
//		return GMP_FAIL;
//	}
//	return nResult;
//}
//
GlassNum COutPickArm::AssignPickFromWhere(LONG lCurWH)
{
	BOOL bG1Exist = TRUE;
	BOOL bG2Exist = TRUE;

	if(INSPWH_GLASS_Status[lCurWH][GLASS1] == ST_EMPTY)
	{
		bG1Exist = FALSE;
	}

	if(INSPWH_GLASS_Status[lCurWH][GLASS2] == ST_EMPTY)
	{
		bG2Exist = FALSE;
	}

	if(INSPWH_Status[lCurWH] == ST_MOD_NOT_SELECTED)
	{
		return NO_GLASS;
	}
	else if(!bG1Exist && !bG2Exist)	//00
	{
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		DisplayMessage(__FUNCTION__);
		return NO_GLASS;
	}
	else if(!bG1Exist && bG2Exist)	//01
	{
		return GLASS2;
	}
	else if(bG1Exist && !bG2Exist)	//10
	{
		return GLASS1;
	}
	else if(bG1Exist && bG2Exist)	//11
	{
		return MAX_NUM_OF_GLASS;
	}

	return NO_GLASS;
}

GlassNum COutPickArm::AssignPickFromWhere_ShBuf()
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

BOOL COutPickArm::CheckPlaceStatus()
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

LONG COutPickArm::IPC_SaveMachineParam()
{	
	CString str;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]	= m_lShBufPlaceGlassPosn;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lPlaceGlassPosn"]		= m_lPlaceGlassPosn;

	pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lRejBeltPlaceLevel"]	= m_lRejBeltPlaceLevel;
	pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lShBufPlaceLevel"]	= m_lShBufPlaceLevel;

	for (INT i = WH_1; i < MAX_NUM_OF_WH; i++)
	{
		str.Format("WH_%ld", i + 1);
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_lPickGlassPosn"]				= m_lPickGlassPosn[i];
	}
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_lRejBeltPlaceGlassPosn"]				= m_lRejBeltPlaceGlassPosn[i];
	}

	return CTransferArm::IPC_SaveMachineParam();		
}

LONG COutPickArm::IPC_LoadMachineParam()
{
	CString str;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	
	m_lShBufPlaceGlassPosn	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]; 
	m_lPlaceGlassPosn		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lPlaceGlassPosn"];
	
	m_lRejBeltPlaceLevel	= pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lRejBeltPlaceLevel"]; 
	m_lShBufPlaceLevel	= pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lShBufPlaceLevel"]; 

	for (INT i = WH_1; i < MAX_NUM_OF_WH; i++)
	{
		str.Format("WH_%ld", i + 1);
		m_lPickGlassPosn[i]			= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_lPickGlassPosn"];
	}
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);
		m_lRejBeltPlaceGlassPosn[i]			= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_lRejBeltPlaceGlassPosn"];
	}
	return CTransferArm::IPC_LoadMachineParam();
}

LONG COutPickArm::IPC_SaveDeviceParam()
{
	CString str;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szTmp;
	szTmp = GetStnName();
	//pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"]		= m_lPickGlassOffset;
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]		= m_lPlaceGlassOffset; 
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]	= m_lShBufPlaceGlassOffset; 
	pAppMod->m_smfDevice[GetStnName()]["Setting"]["m_bInlineInBack"]		= m_bInlineInBack; 

	pAppMod->m_smfDevice[GetStnName()]["Level"]["m_lShBufPlaceLevelOffset"]		= m_lShBufPlaceLevelOffset; 
	pAppMod->m_smfDevice[GetStnName()]["Level"]["m_lRejBeltPlaceLevelOffset"]	= m_lRejBeltPlaceLevelOffset; 

	for (INT i = WH_1; i < MAX_NUM_OF_WH; i++)
	{
		str.Format("WH_%ld", i + 1);
		pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_lPickGlassOffset"]			= m_lPickGlassOffset[i];

	}
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);
		pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_lRejBeltPlaceGlassOffset"]			= m_lRejBeltPlaceGlassOffset[i];
	}
	return CTransferArm::IPC_SaveDeviceParam();
}

LONG COutPickArm::IPC_LoadDeviceParam()
{
	CString str;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	
	//m_lPickGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"];
	m_lPlaceGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]; 
	m_lShBufPlaceGlassOffset	= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]; 
	m_bInlineInBack			= (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Setting"]["m_bInlineInBack"];

	m_lShBufPlaceLevelOffset	= pAppMod->m_smfDevice[GetStnName()]["Level"]["m_lShBufPlaceLevelOffset"]; 
	m_lRejBeltPlaceLevelOffset	= pAppMod->m_smfDevice[GetStnName()]["Level"]["m_lRejBeltPlaceLevelOffset"]; 

	for (INT i = WH_1; i < MAX_NUM_OF_WH; i++)
	{
		str.Format("WH_%ld", i + 1);
		m_lPickGlassOffset[i]			= pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_lPickGlassOffset"];
	}
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);
		m_lRejBeltPlaceGlassOffset[i]			= pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_lRejBeltPlaceGlassOffset"];
	}

	return CTransferArm::IPC_LoadDeviceParam();
}