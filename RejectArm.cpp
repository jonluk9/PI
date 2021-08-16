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

#include "stdafx.h"
#include "MarkConstant.h"
#include "PI9000.h"
#include "PI9000_Constant.h"
#include "HouseKeeping.h"

#define REJARM_EXTERN
#include "RejectArm.h"
#include "RejectBelt.h"
#include "InspWH.h"
#include "InspOpt.h"
#include "PickArm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CRejectArm, CTransferArm)

CRejectArm::CRejectArm()
{
	m_lStnErrorCode		= IDS_REJARM_NOT_SELECTED;
	//m_stMotorY.InitMotorInfo("REJARM_Y", &m_bModSelected);	//done
	//m_stMotorZ.InitMotorInfo("REJARM_Z", &m_bModSelected);

	//m_myMotorList.Add((CMotorInfo*)&m_stMotorZ);		// For Home Z firstly
	//m_myMotorList.Add((CMotorInfo*)&m_stMotorY);

	////Sol
	//m_stGlass1VacSol			= CSoInfo("RejArmVacSol",			"IOMODULE_OUT_PORT_1", BIT_8, PICKARM_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stGlass1VacSol);
	//m_stCOF1VacSol				= CSoInfo("RejArmCOFVacSol",		"IOMODULE_OUT_PORT_1", BIT_9, PICKARM_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stCOF1VacSol);
	//m_stWeakBlowSol				= CSoInfo("RejArmWBSol",			"IOMODULE_OUT_PORT_1", BIT_10, PICKARM_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stWeakBlowSol);


	////Snr	
	//m_stRejArmYFrontLmtSnr		= CSiInfo("RejArmYFrontLmtSnr",		"IOMODULE_IN_PORT_2", BIT_8, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRejArmYFrontLmtSnr);

	//m_stRejArmYBackLmtSnr		= CSiInfo("RejArmYBackLmtSnr",		"IOMODULE_IN_PORT_2", BIT_9, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRejArmYBackLmtSnr);

	//m_stRejArmZHomeSnr			= CSiInfo("RejArmZHomeSnr",			"IOMODULE_IN_PORT_2", BIT_10, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRejArmZHomeSnr);

	//m_stRejArmExtSnr			= CSiInfo("RejArmExtSnr",			"IOMODULE_IN_PORT_2", BIT_11, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stRejArmExtSnr);

	//m_stGlass1VacSnr			= CSiInfo("RejArmVacSnr",			"IOMODULE_IN_PORT_2", BIT_12, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stGlass1VacSnr);

	//m_stCOF1VacSnr			= CSiInfo("RejArmCOFVacSnr",		"IOMODULE_IN_PORT_2", BIT_13, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stCOF1VacSnr);

	m_lStandbyPosn				= 0;
	m_lPickGlassPosn			= 0;
	m_lPlaceGlassPosn			= 0;
	m_lPickGlassOffset			= 0;
	m_lPlaceGlassOffset			= 0;
	m_lPrePlaceGlassPosn		= 0;

	m_lStandbyLevel				= 0;
	m_lPlaceLevel				= 0;
	m_lPlaceLevelOffset			= 0;

	m_bGlassExist				= FALSE;

}

CRejectArm::~CRejectArm()
{
}

BOOL CRejectArm::InitInstance()
{
	CTransferArm::InitInstance();
	return TRUE;
}

INT CRejectArm::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CRejectArm::Operation()
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

VOID CRejectArm::UpdateOutput()
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

	CTransferArm::UpdateOutput();

	Sleep(UPDATEOUTPUT_SLEEP);
}

VOID CRejectArm::UpdateProfile()
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

VOID CRejectArm::UpdatePosition()
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

VOID CRejectArm::RegisterVariables()
{
	try
	{
		RegVariable(GetStation() + _T("bCheckMotor"), &m_bCheckMotor); //REJARM_
		RegVariable(GetStation() + _T("bCheckIO"), &m_bCheckIO);

		RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		RegVariable(GetStation() + _T("bShowModSelected"), &m_bShowModSelected);
		RegVariable(GetStation() + _T("bModError"), &m_bModError);

		RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		RegVariable(GetStation() + _T("bDebugCal"), &HMI_bDebugCal);
		RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);
		//Motor
		RegVariable(_T("HMI_bRejArmYOn"), &m_stMotorY.bHMI_On);
		RegVariable(_T("HMI_bRejArmZOn"), &m_stMotorZ.bHMI_On);

		RegVariable(_T("HMI_lRejArmYEncoder"), &m_stMotorY.lHMI_CurrPosn);
		RegVariable(_T("HMI_lRejArmZEncoder"), &m_stMotorZ.lHMI_CurrPosn);

		RegVariable(GetStation() + _T("dYDistPerCount"), &m_stMotorY.stAttrib.dDistPerCount);
		RegVariable(GetStation() + _T("dZDistPerCount"), &m_stMotorZ.stAttrib.dDistPerCount);

		RegVariable(GetStation() + _T("lPickPlaceRetryLimit"), &m_lPickPlaceRetryLimit);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeAll"), HMI_HomeAll);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnY"), HMI_PowerOnY);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnZ"), HMI_PowerOnZ);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommY"), HMI_CommY);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeY"), HMI_HomeY);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeZ"), HMI_HomeZ);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexYPos"), HMI_IndexYPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexYNeg"), HMI_IndexYNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZPos"), HMI_IndexZPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZNeg"), HMI_IndexZNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPickPlaceRetryLimit"), HMI_SetPickPlaceRetryLimit);

		//Register Sensors here 
					

		//Si
		RegVariable(_T("HMI_bRejArmYFrontLmtSnr"), &m_stRejArmYFrontLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bRejArmYBackLmtSnr"), &m_stRejArmYBackLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bRejArmZHomeSnr"), &m_stRejArmZHomeSnr.bHMI_Status);		
		RegVariable(_T("HMI_bRejArmExtSnr"), &m_stRejArmExtSnr.bHMI_Status);
		RegVariable(_T("HMI_bRejArmVacSnr"), &m_stGlass1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bRejArmCOFVacSnr"), &m_stCOF1VacSnr.bHMI_Status);
		//So
		RegVariable(_T("HMI_bRejArmVacSol"), &m_stGlass1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bRejArmWBSol"), &m_stWeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bRejArmCOFVacSol"), &m_stCOF1VacSol.bHMI_Status);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleRejArmVacSol"), HMI_ToggleRejArmVacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleRejArmWBSol"), HMI_ToggleRejArmWBSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleRejArmCOFVacSol"), HMI_ToggleRejArmCOFVacSol);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PickGlassOperation"), HMI_PickGlassOperation);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PlaceGlassOperation"), HMI_PlaceGlassOperation);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PickAndPlaceGlassOperation"),HMI_PickAndPlaceGlassOperation);

		RegVariable(GetStation() + _T("bUseCOFVacuum"), &m_bUseCOFVacuum);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleUseCOFVacuum"), HMI_ToggleUseCOFVacuum);

		
	}
	catch (CAsmException e)
	{
		DisplayException(e);
	}
}

// ------------------------------------------------------------------------
// HMI Command delcaration
// ------------------------------------------------------------------------
LONG CRejectArm::HMI_PowerOnY(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorY.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CRejectArm::HMI_PowerOnZ(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorZ.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

// Commutate
LONG CRejectArm::HMI_CommY(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.Commutate();

	return 0;
}

LONG CRejectArm::HMI_CommZ(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ.Commutate();

	return 0;
}

// Home
LONG CRejectArm::HMI_HomeY(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.Home(GMP_WAIT);

	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CRejectArm::HMI_HomeZ(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ.Home(GMP_WAIT);

	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}



LONG CRejectArm::HMI_ToggleRejArmVacSol(IPC_CServiceMessage &svMsg)
{
	if (m_stGlass1VacSol.bHMI_Status)
	{
		SetGLASS1_VacSol(OFF);
	}
	else
	{
		SetGLASS1_VacSol(ON);
	}

	return 0;
}

LONG CRejectArm::HMI_ToggleRejArmWBSol(IPC_CServiceMessage &svMsg)
{
	if (m_stWeakBlowSol.bHMI_Status)
	{
		SetWeakBlowSol(OFF);
	}
	else
	{
		SetWeakBlowSol(ON);
	}

	return 0;
}

LONG CRejectArm::HMI_ToggleRejArmCOFVacSol(IPC_CServiceMessage &svMsg)
{
	if (m_stCOF1VacSol.bHMI_Status)
	{
		SetCOF1_VacSol(OFF);
	}
	else
	{
		SetCOF1_VacSol(ON);
	}

	return 0;
}

LONG CRejectArm::HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg)
{
	ULONG ulSteps;
	svMsg.GetMsg(sizeof(ULONG), &ulSteps);

	g_lDiagnSteps = ulSteps;
	SetDiagnSteps(ulSteps);
	return 0;
}

LONG CRejectArm::HMI_IndexYPos(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CRejectArm::HMI_IndexYNeg(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CRejectArm::HMI_IndexZPos(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CRejectArm::HMI_IndexZNeg(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CRejectArm::HMI_PickGlassOperation(IPC_CServiceMessage &svMsg)
{
	BOOL bOpDone						= TRUE;
	LONG lResponse						= 0;
	BOOL bResult						= FALSE;

	bResult = PickGlassOperation();
	
	if (bResult) 
	{
		MoveY(REJARM_Y_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CRejectArm::HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg)
{
	BOOL bOpDone						= TRUE;
	LONG lResponse						= 0;
	BOOL bResult						= FALSE;

	bResult = PlaceGlassOperation();
	
	if (bResult) 
	{
		MoveY(REJARM_Y_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}
LONG CRejectArm::HMI_PickAndPlaceGlassOperation(IPC_CServiceMessage &svMsg)
{
	BOOL bOpDone						= TRUE;
	LONG lResponse						= 0;
	BOOL bResult						= FALSE;

	bResult = PickAndPlaceGlassOperation();
	
	if (bResult) 
	{
		MoveY(REJARM_Y_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}
VOID CRejectArm::SetDiagnSteps(ULONG ulSteps)
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
		m_lDiagnSteps = 10000;
		break;
	case 5:
		m_lDiagnSteps = 50000;
		break;
	default:
		m_lDiagnSteps = 10000;
	}
}

VOID CRejectArm::ModuleSetupPreTask()
{
	m_bCheckMotor		= TRUE;
	m_bCheckIO			= TRUE;
	
	UpdateModuleSetupPara();
	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
}

VOID CRejectArm::ModuleSetupPostTask()
{
	m_bCheckMotor		= FALSE;
	m_bCheckIO			= FALSE;
}

VOID CRejectArm::UpdateModuleSetupPosn()
{
	
	INT i = 0;
	INT j = 0;

	// Assign Module Setup Posn/Level/Para
	// Position Tab
	i = 0;	// 
	g_stSetupPosn[i].szPosn.Format("Standby Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_lStandbyPosn;

	i++; // 1
	g_stSetupPosn[i].szPosn.Format("Pick Glass Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_lPickGlassPosn;

	i++; // 2
	g_stSetupPosn[i].szPosn.Format("Pick Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_lPickGlassOffset;

	i++; // 3
	g_stSetupPosn[i].szPosn.Format("Place Glass Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_lPlaceGlassPosn;

	i++; // 4
	g_stSetupPosn[i].szPosn.Format("Place Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_lPlaceGlassOffset;

	i++; // 5
	g_stSetupPosn[i].szPosn.Format("Pre Place Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_lPrePlaceGlassPosn;

	for (j = i + 1; j < (NUM_OF_POSN_XY); j++)
	{
		g_stSetupPosn[j].szPosn.Format("Reserved");
		g_stSetupPosn[j].bEnable		= FALSE;
		g_stSetupPosn[j].bVisible		= FALSE;
		g_stSetupPosn[j].lType			= MACHINE_PARA;
		g_stSetupPosn[j].lPosnX			= 0;
		g_stSetupPosn[j].lPosnY			= 0;
	}

}

VOID CRejectArm::UpdateModuleSetupLevel()
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
	g_stSetupLevel[i].szLevel.Format("Place Glass Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevel;

	i++;	// 2
	g_stSetupLevel[i].szLevel.Format("Place Glass Level Offset");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= DEVICE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevelOffset;
	

	
	for (j = i + 1; j < NUM_OF_LEVEL_Z; j++)
	{
		g_stSetupLevel[j].szLevel.Format("Reserved");
		g_stSetupLevel[j].bEnable		= FALSE;
		g_stSetupLevel[j].bVisible		= FALSE;
		g_stSetupLevel[j].lType			= MACHINE_PARA;
		g_stSetupLevel[j].lLevelZ		= 0;
	}
}

VOID CRejectArm::UpdateModuleSetupPara()
{
	INT i = 0;
	INT j = 0;

	// Para Tab
	// Delay
	i = 0;
	g_stSetupPara[i].szPara.Format("Vac Sol On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stGlass1VacSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 1
	g_stSetupPara[i].szPara.Format("Vac Sol Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stGlass1VacSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;
	
	i++;	//2
	g_stSetupPara[i].szPara.Format("COF Vac Sol On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stCOF1VacSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 3
	g_stSetupPara[i].szPara.Format("COF Vac Sol Off Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stCOF1VacSol.GetOffDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 4
	g_stSetupPara[i].szPara.Format("WB Sol On Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_stWeakBlowSol.GetOnDelay();
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 5
	g_stSetupPara[i].szPara.Format("WB Sol Off Delay (ms)");
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

BOOL CRejectArm::ModuleSetupSetPosn(LONG lSetupTitle)
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
	TempPosn.y = m_stMotorY.GetEncPosn();

	switch (lSetupTitle)
	{	
	case 0:
		mtrOld.y			= m_lStandbyPosn;
		m_lStandbyPosn		= TempPosn.y;
		break;

	case 1:
		mtrOld.y			= m_lPickGlassPosn;
		m_lPickGlassPosn	= TempPosn.y;
		break;

	case 2:
		mtrOld.y			= m_lPickGlassOffset;
		m_lPickGlassOffset	= TempPosn.y;
		break;

	case 3:
		mtrOld.y			= m_lPlaceGlassPosn;
		m_lPlaceGlassPosn	= TempPosn.y;
		break;

	case 4:
		mtrOld.y			= m_lPlaceGlassOffset;
		m_lPlaceGlassOffset	= TempPosn.y;
		break;
	
	case 5:
		mtrOld.y			= m_lPrePlaceGlassPosn;
		m_lPrePlaceGlassPosn	= TempPosn.y;
		break;
			
	}

	CPI9000Stn::ModuleSetupSetPosn(lSetupTitle, "Y", mtrOld.y, TempPosn.y);

	return TRUE;
}

BOOL CRejectArm::ModuleSetupSetLevel(LONG lSetupTitle)
{
	LONG lAnswer = rMSG_TIMEOUT;
	LONG lOldLevel		= 0;
	LONG lPrePostLevel	= 0;
	LONG lTempLevel = 0;
	BOOL bResult = TRUE;

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

	switch (lSetupTitle)
	{
	case 0:
		lOldLevel					= m_lStandbyLevel;

		m_lStandbyLevel				= lTempLevel;
		break;

	case 1:
		lOldLevel					= m_lPlaceLevel;

		m_lPlaceLevel				= lTempLevel;
		break;

	case 2:
		lOldLevel					= m_lPlaceLevelOffset;

		m_lPlaceLevelOffset			= lTempLevel;
		break;
	}

	CPI9000Stn::ModuleSetupSetLevel(lSetupTitle, lOldLevel, lTempLevel);

	return bResult;
}


BOOL CRejectArm::ModuleSetupSetPara(PARA stPara)
{
	LogAction(__FUNCTION__);

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
		break;

	case 1:
		m_stGlass1VacSol.SetOffDelay(stPara.lPara);
		break;

	case 2:
		m_stCOF1VacSol.SetOnDelay(stPara.lPara);
		break;

	case 3:
		m_stCOF1VacSol.SetOffDelay(stPara.lPara);
		break;

	case 4:
		m_stWeakBlowSol.SetOnDelay(stPara.lPara);
		break;

	case 5:
		m_stWeakBlowSol.SetOffDelay(stPara.lPara);
		break;

	}

	return CPI9000Stn::ModuleSetupSetPara(stPara);
}

BOOL CRejectArm::ModuleSetupGoPosn(LONG lSetupTitle)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	INT nResult = GMP_SUCCESS;

	CString szTitle = "";
	CString szMsg = "";
	szTitle.Format("GO POSN OPERATION");
	szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());

	if (!m_bModSelected)
	{
		HMIMessageBox(MSG_OK, szTitle, szMsg);
		return FALSE;
	}
	szMsg.Format("%s Module Not Selected. Operation Abort!", pCInspWH->GetStnName());
	if (!pCInspWH->m_bModSelected)
	{
		HMIMessageBox(MSG_OK, szTitle, szMsg);
		return FALSE;
	}
	LONG lAnswer = HMIMessageBox(MSG_OK_CANCEL, "MOVE REJARM Y", "Move InspWH Z to Standby Level");
	//if (lAnswer == MSG_OK)
	//{
	//	if (
	//		//pCInspWH->MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
	//		//pCInspWH->MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS || 
	//		//pCInspWH->m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS
	//		)
	//	{
	//		return FALSE;
	//	}
	//}
	//else
	//{
	//	return FALSE;
	//}

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
		nResult = SetupGoPlaceGlassOffset();
		break;

	case 5:
		nResult = SetupGoPrePlacePosn();
		break;
	}

	if (nResult != GMP_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CRejectArm::ModuleSetupGoLevel(LONG lSetupTitle)
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
		nResult = SetupGoPlaceLevel();
		break;

	case 2:
		nResult = SetupGoPlaceLevelOffset();	
		break;
	}

	if (nResult != GMP_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}

INT CRejectArm::SetupGoStandbyPosn()
{
	INT nResult = GMP_SUCCESS;
	if (MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		return GMP_FAIL;
	}
	 nResult = MoveY(REJARM_Y_CTRL_GO_STANDBY_POSN, GMP_WAIT);	
	 return nResult;
}

INT CRejectArm::SetupGoPickGlassPosn()
{
	INT nResult = GMP_SUCCESS;
	if (MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		return GMP_FAIL;
	}
	nResult = MoveY(REJARM_Y_CTRL_GO_PICK_POSN, GMP_WAIT);	
	return nResult;
}

INT CRejectArm::SetupGoPickGlassOffset()
{
	INT nResult = GMP_SUCCESS;
	if (MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		return GMP_FAIL;
	}
	nResult = MoveY(REJARM_Y_CTRL_GO_PICK_OFFSET, GMP_WAIT);
	return nResult;
}

INT CRejectArm::SetupGoPlaceGlassPosn()
{
	INT nResult = GMP_SUCCESS;
	if (MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		return GMP_FAIL;
	}
	nResult = MoveY(REJARM_Y_CTRL_GO_PLACE_POSN, GMP_WAIT);
	return nResult;
}

INT CRejectArm::SetupGoPlaceGlassOffset()
{
	INT nResult = GMP_SUCCESS;
	if (MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		return GMP_FAIL;
	}
	nResult = MoveY(REJARM_Y_CTRL_GO_PLACE_OFFSET, GMP_WAIT);
	return nResult;
}

INT CRejectArm::SetupGoPrePlacePosn()
{
	INT nResult = GMP_SUCCESS;
	if (MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		return GMP_FAIL;
	}
	nResult = MoveY(REJARM_Y_CTRL_GO_PREPLACE_POSN, GMP_WAIT);
	return nResult;
}


INT CRejectArm::SetupGoStandbyLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);	

	return nResult;
}

INT CRejectArm::SetupGoPlaceLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(REJARM_Z_CTRL_GO_PLACE_LEVEL, GMP_WAIT);	

	return nResult;
}

INT CRejectArm::SetupGoPlaceLevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(REJARM_Z_CTRL_GO_PLACE_LEVEL_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CRejectArm::MoveY(REJARM_Y_CTRL_SIGNAL ctrl, BOOL bWait)
{
	INT		nResult		= GMP_SUCCESS;

	LONG lTempPosn = 0;

	CString szDebug = _T("");
	switch (ctrl)
	{
	case REJARM_Y_CTRL_GO_STANDBY_POSN:
		lTempPosn = m_lStandbyPosn;
		break;

	case REJARM_Y_CTRL_GO_PICK_POSN:
		lTempPosn = m_lPickGlassPosn;
		break;

	case REJARM_Y_CTRL_GO_PICK_OFFSET:

		lTempPosn = m_lPickGlassPosn + m_lPickGlassOffset; 
		
		break;

	case REJARM_Y_CTRL_GO_PLACE_POSN:
		lTempPosn = m_lPlaceGlassPosn;
		break;

	case REJARM_Y_CTRL_GO_PLACE_OFFSET:
		lTempPosn = m_lPlaceGlassPosn + m_lPlaceGlassOffset;
		break;

	case REJARM_Y_CTRL_GO_PREPLACE_POSN:
		lTempPosn = m_lPrePlaceGlassPosn;
		break;

	}

	return m_stMotorY.MoveAbsolute(lTempPosn, GMP_WAIT);
}

INT CRejectArm::MoveZ(REJARM_Z_CTRL_SIGNAL ctrl, BOOL bWait)
{
	INT		nResult		= GMP_SUCCESS;


	LONG lTempPosn = 0;

	switch (ctrl)
	{
	case REJARM_Z_CTRL_GO_STANDBY_LEVEL:
		lTempPosn = m_lStandbyLevel;
		break;

	case REJARM_Z_CTRL_GO_PLACE_LEVEL:
		lTempPosn = m_lPlaceLevel;
		break;

	case REJARM_Z_CTRL_GO_PLACE_LEVEL_OFFSET:
		lTempPosn = m_lPlaceLevel + m_lPlaceLevelOffset;
		break;

	}

	return m_stMotorZ.MoveAbsolute(lTempPosn, GMP_WAIT);
}

LONG CRejectArm::IPC_SaveMachineParam()
{
	return CTransferArm::IPC_SaveMachineParam();
}

LONG CRejectArm::IPC_LoadMachineParam()
{
	return CTransferArm::IPC_LoadMachineParam();

}

LONG CRejectArm::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szTmp;
	szTmp = GetStnName();
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"]		= m_lPickGlassOffset;
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]		= m_lPlaceGlassOffset; 
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPrePlaceGlassPosn"]		= m_lPrePlaceGlassPosn; 
	return CTransferArm::IPC_SaveDeviceParam();
}

LONG CRejectArm::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szTmp;
	szTmp = GetStnName();

	m_lPickGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"];
	m_lPlaceGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]; 
	m_lPrePlaceGlassPosn		= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPrePlaceGlassPosn"]; 

	return CTransferArm::IPC_LoadDeviceParam();
}

CString CRejectArm::GetSequenceString()
{

	switch (m_qSubOperation)
	{
	case REJARM_IDLE_Q:
		return "REJARM_IDLE_Q";
		
	case REJARM_WAIT_INSPWH_GLASS_REJECT_REQ_Q:
		return "REJARM_WAIT_INSPWH_GLASS_REJECT_REQ_Q";

	case REJARM_PICK_GLASS_ACTION_Q:
		return "REJARM_PICK_GLASS_ACTION_Q";

	case REJARM_CHECK_GLASS_EXIST_AFTER_PICK_Q:
		return "REJARM_CHECK_GLASS_EXIST_AFTER_PICK_Q";

	case REJARM_WAIT_REJBELT_READY_TO_PLACE_Q:
		return "REJARM_WAIT_REJBELT_READY_TO_PLACE_Q";

	case REJARM_PICK_PLACE_GLASS_ACTION_Q:
		return "REJARM_PICK_PLACE_GLASS_ACTION_Q";

	case REJARM_CHECK_GLASS_EXIST_AFTER_PLACE_Q:
		return "REJARM_CHECK_GLASS_EXIST_AFTER_PLACE_Q";

	case REJARM_CHECK_EXIST_ON_BELT_SNR_Q:
		return "REJARM_CHECK_EXIST_ON_BELT_SNR_Q";

	case REJARM_STOP_Q:
		return "REJARM_STOP_Q";
		
	}

	return "Hello World!!";
}

//VOID CRejectArm::OperationSequence()
//{
//	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
//	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	CString szMsg;
//	PRINT_DATA	stPrintData;
//
//	m_nLastError = GMP_SUCCESS;
//
//	switch (m_qSubOperation)
//	{
//
//		case REJARM_IDLE_Q:	// 0
//
//			if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
//			{
//				m_bGlassExist = FALSE;
//				REJECTARM_Status = ST_EMPTY;
//
//				if ((m_nLastError = MoveY(REJARM_Y_CTRL_GO_PICK_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
//				{
//					m_qSubOperation = REJARM_WAIT_INSPWH_GLASS_REJECT_REQ_Q;		// go to 1
//				}
//				else
//				{
//					m_qSubOperation = REJARM_STOP_Q;		// go to 99
//				}
//			}
//			else
//			{
//				m_bGlassExist = TRUE;
//				REJECTARM_Status = ST_BUSY;
//
//				if ((m_nLastError = MoveY(REJARM_Y_CTRL_GO_PREPLACE_POSN, GMP_WAIT)) == GMP_SUCCESS)
//				{
//					m_qSubOperation = REJARM_WAIT_REJBELT_READY_TO_PLACE_Q;		// go to 4
//				}
//				else
//				{
//					m_qSubOperation = REJARM_STOP_Q;		// go to 99
//				}		
//			}
//
//			break;
//
//		case REJARM_WAIT_INSPWH_GLASS_REJECT_REQ_Q:	// 1
//			
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = REJARM_STOP_Q;	// go to 99
//			}
//			else if (INSPWH_Status == ST_REJECT_WH1_REQ)
//			{
//				m_nTARGET_WH = GLASS1;
//				REJECTARM_Status = ST_REJECT_ACK;
//				m_qSubOperation = REJARM_PICK_GLASS_ACTION_Q;	// go to 2
//			}
//			else if (INSPWH_Status == ST_REJECT_WH2_REQ)
//			{
//				m_nTARGET_WH = GLASS2;
//				REJECTARM_Status = ST_REJECT_ACK;
//				m_qSubOperation = REJARM_PICK_GLASS_ACTION_Q;	// go to 2
//			}
//
//			break;
//
//		case REJARM_PICK_GLASS_ACTION_Q:	// 2
//
//			if ((m_nLastError = PickGlass(m_nTARGET_WH)) == GMP_SUCCESS)	//PickGlassFromWh()) == GMP_SUCCESS)
//			{
//				m_qSubOperation = REJARM_CHECK_GLASS_EXIST_AFTER_PICK_Q;		// go to 3
//			}
//			else
//			{
//				m_qSubOperation = REJARM_STOP_Q;		// go to 99
//			}
//
//			break;
//
//		case REJARM_CHECK_GLASS_EXIST_AFTER_PICK_Q:	// 3
//			
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = REJARM_STOP_Q;	// go to 99
//			}
//			else if (IsVacStateOn(GetGlassCOF1_VacSensorOnState()) || AutoMode == BURN_IN)	// Glass Exist
//			{
//				m_bGlassExist = TRUE;
//				REJECTARM_Status = ST_BUSY;
//				m_lPickPlaceRetryCounter = 0;
//
//				if ((m_nLastError = MoveY(REJARM_Y_CTRL_GO_PREPLACE_POSN, GMP_WAIT)) == GMP_SUCCESS)
//				{
//					m_qSubOperation = REJARM_WAIT_REJBELT_READY_TO_PLACE_Q;		// go to 4
//				}
//				else
//				{
//					m_qSubOperation = REJARM_STOP_Q;		// go to 99
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
//						REJECTARM_Status = ST_ERROR;
//						m_qSubOperation = REJARM_STOP_Q;	// go to 99
//						DisplayMessage("REJARM ERROR: Pick Glass Retry Limit Hit");
//						m_bModError = TRUE;
//						SetError(IDS_REJARM_PICK_ERR);
//						if (!IsGLASS1_VacSensorOn())
//						{
//							DisplayMessage("*********************************************");
//							SetError(IDS_REJARM_GLASS_VAC_ERR);
//							szMsg.Format("REJARM_CHECK_GLASS_EXIST_AFTER_PICK_Q: GLASS VAC ERROR");
//							DisplayMessage(szMsg);
//							DisplayMessage("*********************************************");
//						}
//						if (m_bUseCOFVacuum && !IsCOF1_VacSensorOn())
//						{
//							DisplayMessage("*********************************************");
//							SetError(IDS_REJARM_COF_VAC_ERR);
//							szMsg.Format("REJARM_CHECK_GLASS_EXIST_AFTER_PICK_Q: COF VAC ERROR");
//							DisplayMessage(szMsg);
//							DisplayMessage("*********************************************");
//						}
//					}
//					else 
//					{
//						m_qSubOperation = REJARM_PICK_GLASS_ACTION_Q;		// go to 2
//					}
//				}
//				else 
//				{
//					m_qSubOperation = REJARM_PICK_GLASS_ACTION_Q;		// go to 2
//				}
//			}
//
//			break;
//
//		case REJARM_WAIT_REJBELT_READY_TO_PLACE_Q:	// 4
//
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = REJARM_STOP_Q;	// go to 99
//			}			
//			else if (!g_bGlassOnRejBelt || REJECTBELT_Status == ST_MOD_NOT_SELECTED) 
//			{
//				m_qSubOperation = REJARM_CHECK_EXIST_ON_BELT_SNR_Q;		// go to 7
//			}
//			break;
//
//		case REJARM_CHECK_EXIST_ON_BELT_SNR_Q:	// 7
//			
//			if (m_bStopCycle)
//			{
//				m_qSubOperation = REJARM_STOP_Q;	// go to 99
//			}	
//			else if (!IsExistSensorOn())
//			{
//				if ((m_nLastError = MoveY(REJARM_Y_CTRL_GO_PLACE_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
//				{
//					m_qSubOperation = REJARM_PICK_PLACE_GLASS_ACTION_Q;		// go to 5
//				}
//			}
//			else
//			{
//				g_bGlassOnRejBelt = TRUE;
//			}
//			break;
//
//		case REJARM_PICK_PLACE_GLASS_ACTION_Q:	// 5
//
//			if ((m_nLastError = PlaceGlass()) == GMP_SUCCESS)
//			{
//				SetGLASS1_VacSol(ON, GMP_WAIT);
//				m_qSubOperation = REJARM_CHECK_GLASS_EXIST_AFTER_PLACE_Q;		// go to 6
//			}
//			else
//			{
//				m_qSubOperation = REJARM_STOP_Q;		// go to 99
//			}
//
//			break;
//
//		case REJARM_CHECK_GLASS_EXIST_AFTER_PLACE_Q:	// 6
//			
//			if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))	// Glass NOT Exist
//			{
//				m_bGlassExist = FALSE;
//				g_bGlassOnRejBelt = TRUE;
//				SetGLASS1_VacSol(OFF, GMP_NOWAIT);
//				REJECTARM_Status = ST_EMPTY;
//				m_lPickPlaceRetryCounter = 0;
//
//				if ((m_nLastError = MoveY(REJARM_Y_CTRL_GO_PICK_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
//				{
//					m_qSubOperation = REJARM_WAIT_INSPWH_GLASS_REJECT_REQ_Q;		// go to 1
//				}
//				else
//				{
//					m_qSubOperation = REJARM_STOP_Q;		// go to 99
//				}
//			}
//			else	//Place Error
//			{
//				//retry sequence
//				if (m_lPickPlaceRetryLimit != 0)
//				{
//					m_lPickPlaceRetryCounter++;
//
//					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
//					{
//						// ERROR!
//						REJECTARM_Status = ST_ERROR;
//						m_qSubOperation = REJARM_STOP_Q;	// go to 99
//						DisplayMessage("REJARM ERROR: Place Glass Retry Limit Hit");
//						m_bModError = TRUE;
//						DisplayMessage("*********************************************");
//						SetError(IDS_REJARM_PLACE_ERR);
//						szMsg.Format("REJARM_CHECK_GLASS_EXIST_AFTER_PLACE_Q: Reject Arm Place ERROR");
//						DisplayMessage(szMsg);
//						DisplayMessage("*********************************************");
//					}
//					else 
//					{
//						m_qSubOperation = REJARM_PICK_PLACE_GLASS_ACTION_Q;		// go to 5
//					}
//				}
//				else 
//				{
//					m_qSubOperation = REJARM_PICK_PLACE_GLASS_ACTION_Q;		// go to 5
//				}
//			}
//			break;
//
//		case REJARM_STOP_Q:	// 99
//			break;
//
//
//	}
//
//
//	if (m_qPreviousSubOperation != m_qSubOperation)
//	{
//		if (HMI_bDebugSeq)
//		{
//			CString szMsg;
//
//			if (REJARM_STOP_Q != m_qSubOperation)
//			{
//				szMsg.Format("REJARM Go State: %ld", m_qSubOperation);
//			}
//			else
//			{
//				szMsg.Format("RejectArm Go Stop");
//			}
//			DisplayMessage(szMsg);
//
//		}
//
//		UpdateDebugSeq(GetSequenceString(), REJECTARM_Status,  m_bGlass1Exist, m_bGlass2Exist);
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
//			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", REJECTARM_Status);
//
//			LogFile->log(stPrintData.szTitle);
//
//		}
//	}
//
//}

VOID CRejectArm::PreStartOperation()
{
	m_bModError = FALSE;
	if (!m_bModSelected)
	{	
		REJECTARM_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = REJARM_STOP_Q;
		m_bStopCycle = FALSE;
		if (m_fHardware)
		{
			SetError(IDS_REJARM_NOT_SELECTED);
		}
	}
	else
	{
		//UpdateInSHWHStatus();
		MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT);
		Sleep(300);
		MoveY(REJARM_Y_CTRL_GO_STANDBY_POSN, GMP_NOWAIT);

		REJECTARM_Status					= ST_IDLE;
		m_qSubOperation						= REJARM_IDLE_Q;
		m_bStopCycle						= FALSE;
		m_lPickPlaceRetryCounter			= 0;
		//m_lPRRetryCounter	= 0;
		//m_lSkipPRCounter	= 0;
		//m_lSkipTrayCounter	= 0;
	}

	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), REJECTARM_Status);

	Motion(FALSE);
	State(IDLE_Q);

	CPI9000Stn::PreStartOperation();
}

VOID CRejectArm::AutoOperation()
{
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bStopCycle = TRUE;
	}

	//OperationSequence();

	// cycle completed or motor error or abort requested: end operation immediately
	if (
		(m_bModSelected && m_qSubOperation == REJARM_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			// Stop Operation Motion
		}
		
		if (GMP_SUCCESS != m_nLastError)
		{
			m_bModError = TRUE;
		}
		//g_bFPCType1Ready = FALSE; //20130916
		REJECTARM_Status = ST_STOP;
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

	}
}

INT CRejectArm::PlaceGlassOnRejBelt()
{
	return GMP_SUCCESS;
}

INT CRejectArm::PlaceGlass()
{
	LONG lDelay;

	if ((m_nLastError = MoveZ(REJARM_Z_CTRL_GO_PLACE_LEVEL_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = max(m_stGlass1VacSol.GetOffDelay(), m_stCOF1VacSol.GetOffDelay());
		SetVacSol(GLASS1, OFF);
		Sleep (lDelay);

		if (m_stWeakBlowSol.GetOnDelay() > 0)
		{
			SetWeakBlowSol(ON, GMP_WAIT);
			SetWeakBlowSol(OFF, GMP_WAIT);
		}
		return m_nLastError = MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT);
	}

	return GMP_FAIL;
}

BOOL CRejectArm::PlaceGlassOperation()
{
	CRejectBelt *pCRejectBelt = (CRejectBelt*)m_pModule->GetStation("RejectBelt");
	BOOL bResult = TRUE;
	CString szMsg;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", szMsg);
		return FALSE;
	}
	if (pCRejectBelt->m_fHardware) 
	{
		pCRejectBelt->CheckModSelected(TRUE);
	}
	if (pCRejectBelt->m_fHardware && !pCRejectBelt->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCRejectBelt->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", szMsg);
		return FALSE;
	}
	SetVacSol(GLASS1, ON);

	if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
	{
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "No Glass Exist On Reject Arm. Operation Abort!");
		return FALSE;
	}

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
	else if (MoveY(REJARM_Y_CTRL_GO_PLACE_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Reject Arm Move to Place Posn Error. Operation Abort!");
		return FALSE;
	}
	else if (PlaceGlass() != GMP_SUCCESS)
	{
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Reject Arm Place Glass Error. Operation Abort!");
		return FALSE;
	}
	
	SetVacSol(GLASS1, ON);

	if (IsVacStateOn(GetGlassCOF1_VacSensorOnState()))
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Glass Exist On Reject Arm After Place!");
		return FALSE;
	}
	else
	{
		SetVacSol(GLASS1, OFF);
	}

	if (MoveZ(REJARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Reject Arm Move to Standby Level Error. Operation Abort!");
		return FALSE;
	}

	return bResult;
}

INT CRejectArm::PickGlass(GlassNum lGlassNum)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	LONG lDelay;

	SetVacSol(GLASS1, ON);

	//if ((m_nLastError = pCInspWH->MoveZToRejUnloadLevel(lGlassNum, GMP_WAIT)) == GMP_SUCCESS)
	{
		lDelay = max(max(pCInspWH->m_stGlass1VacSol.GetOffDelay(), m_stGlass1VacSol.GetOnDelay()), m_stCOF1VacSol.GetOnDelay());
		pCInspWH->SetVacSol(lGlassNum, OFF);
		Sleep (lDelay);

		if (pCInspWH->m_stWeakBlowSol.GetOnDelay() > 0)
		{
			if (lGlassNum == GLASS1)
			{
				pCInspWH->SetWeakBlowSol(ON, GMP_WAIT);
				pCInspWH->SetWeakBlowSol(OFF, GMP_WAIT);
			}
			if (lGlassNum == GLASS2)
			{
				pCInspWH->SetWeakBlow2Sol(ON, GMP_WAIT);
				pCInspWH->SetWeakBlow2Sol(OFF, GMP_WAIT);
			}
		}
		//m_nLastError = pCInspWH->MoveZToStandbyLevel(lGlassNum, GMP_WAIT);
	}
	return m_nLastError;
}

BOOL CRejectArm::PickGlassOperation()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	BOOL bResult = TRUE;
	CString szMsg;
	LONG lDelay;
	GlassNum lGlassNum = GLASS1;
	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", szMsg);
		return FALSE;
	}

	if (MoveRelatedModuleToStandby() != GMP_SUCCESS)
	{
		return FALSE;
	}
	
	LONG lResponse = HMISelectionBox("PICK GLASS OPERATION", "Please select Glass to pick:", "GLASS1", "GLASS2");	

	if (lResponse == -1 || lResponse == 11)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
		return FALSE;
	}

	switch (lResponse)
	{
	case 0:
		lGlassNum = GLASS1;
		break;

	case 1:
		lGlassNum = GLASS2;
		break;
	}
	
	SetVacSol(GLASS1, ON);
	pCInspWH->SetVacSol(lGlassNum, ON);
	lDelay = max(max(pCInspWH->m_stGlass1VacSol.GetOffDelay(), m_stGlass1VacSol.GetOnDelay()), m_stCOF1VacSol.GetOnDelay());
	Sleep (lDelay);
	
	if (!pCInspWH->IsVacSensorOn(lGlassNum))
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "No Glass Exist On InspWH. Operation Abort!");
		pCInspWH->SetVacSol(lGlassNum, OFF);
		return FALSE;
	}

	if (IsVacStateOn(GetGlassCOF1_VacSensorOnState()))
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Glass Exist On Reject Arm. Operation Abort!");
		return FALSE;
	}
	else
	{
		SetVacSol(GLASS1, OFF);
	}

	if (MoveY(REJARM_Y_CTRL_GO_PICK_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Reject Arm Go To Pick Glass Posn Error. Operation Abort!");
		return FALSE;
	}
	else if (pCInspWH->MoveXYToUnloadRejPosn(lGlassNum, GMP_WAIT) != GMP_SUCCESS)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "InspWH Go To Unload Glass Posn Error. Operation Abort!");
		return FALSE;
	}
	else if (PickGlass(lGlassNum) != GMP_SUCCESS)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Reject Arm Pick Glass Error. Operation Abort!");
		return FALSE;
	}
	//else if (pCInspWH->MoveZToStandbyLevel(lGlassNum, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "InspWH Go To Standby Level Error. Operation Abort!");
	//	return FALSE;
	//}
	
	if (IsVacStateOff(GetGlassCOF1_VacSensorOnState()))
	{
		pCInspWH->SetVacSol(lGlassNum, ON, GMP_WAIT);
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Reject Arm Vac Snr Not On!");
		return FALSE;
	}
	else
	{
		if (MoveY(REJARM_Y_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{	
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Reject Arm Go To Standby Posn Error!");
			return FALSE;
		}
	}
	return bResult;
}

BOOL CRejectArm::PickAndPlaceGlassOperation()
{
	if (!PickGlassOperation())
	{
		HMIMessageBox(MSG_OK, "PICK AND PLACE GLASS OPERATION", "Pick Glass Fail!");
		return FALSE;
	}
	else if (!PlaceGlassOperation())
	{
		HMIMessageBox(MSG_OK, "PICK AND PLACE GLASS OPERATION", "Place Glass Fail!");
		return FALSE;
	}
	return TRUE;
}

INT CRejectArm::MoveRelatedModuleToStandby()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CPickArm *pCPickArm = (CPickArm*)m_pModule->GetStation("PickArm");

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
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "InspWH Module Not Selected. Operation Abort!");
		}
		else
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "InspWH Module Not Selected. Continue Operation?");
		}

		if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT || g_ulUserGroup <= SERVICE)
		{
			return GMP_FAIL;
		}
	}

	if (pCInspOpt->m_fHardware) //20140603
	{
		pCInspOpt->CheckModSelected(TRUE);
	}
	if (pCInspOpt->m_fHardware && !pCInspOpt->m_bModSelected)
	{
		if (g_ulUserGroup <= SERVICE)
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "InspOpt Module Not Selected. Operation Abort!");
		}
		else
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "InspOpt Module Not Selected. Continue Operation?");
		}

		if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT || g_ulUserGroup <= SERVICE)
		{
			return GMP_FAIL;
		}
	}

	if (pCPickArm->m_fHardware) 
	{
		pCPickArm->CheckModSelected(TRUE);
	}
	if (pCPickArm->m_fHardware && !pCPickArm->m_bModSelected)
	{
		if (g_ulUserGroup <= SERVICE)
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "PickArm Module Not Selected. Operation Abort!");
		}
		else
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "PickArm Module Not Selected. Continue Operation?");
		}

		if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT || g_ulUserGroup <= SERVICE)
		{
			return GMP_FAIL;
		}
	}

	if (		
		//pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
		pCInspWH->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS ||
		pCInspOpt->MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS ||
		pCPickArm->MoveX(PICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS
	   )
	{
		return GMP_FAIL;
	}
	return nResult;
}

VOID CRejectArm::PrintMachineSetup(FILE *fp)
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

		fprintf(fp, "<<Level>>\n");
		fprintf(fp, "%30s\t : %ld\n", "Standby Level",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lStandbyLevel"]);
		fprintf(fp, "%30s\t : %ld\n", "Place Level",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lPlaceLevel"]);
	}

	CPI9000Stn::PrintMachineSetup(fp);
}

VOID CRejectArm::PrintDeviceSetup(FILE *fp)
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

		fprintf(fp, "<<Level>>\n");
		fprintf(fp, "%30s\t : %ld\n", "Place Level Offset",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Level"]["m_lPlaceLevelOffset"]); //20130315

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

BOOL CRejectArm::IsExistSensorOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return GetGlass1Status();
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return GetGlass1Status();
	}
#endif
	try
	{
		bResult = m_stRejArmExtSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}