/////////////////////////////////////////////////////////////////
//	INPICKARM.cpp : interface of the CInPickArm class
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

#define INPICKARM_EXTERN
#include "InPickArm.h"
#include "OutPickArm.h"
#include "WinEagle.h"
#include "INSPWH.h"
#include "WH1.h"
#include "WH2.h"
#include "InspOpt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CInPickArm, CTransferArm)

CInPickArm::CInPickArm()
{
	////I/O and Motors are done!!
	//// CPI9000Srn
	m_lStnErrorCode		= IDS_PICKARM_NOT_SELECTED_ERR;
	m_lModSelectMode	= GLASS_MOD;	

	//// Motor Related
	m_stMotorX.InitMotorInfo("INPICKARM_X", &m_bModSelected);	//done
	m_stMotorZ.InitMotorInfo("INPICKARM_Z", &m_bModSelected);	//done

	m_myMotorList.Add((CMotorInfo*)&m_stMotorX);
	m_myMotorList.Add((CMotorInfo*)&m_stMotorZ);


	//// Si Related
	m_stGlass1VacSnr		= CSiInfo("INPALVacSnr",			"IOMODULE_IN_PORT_2", BIT_0, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass1VacSnr);
	m_stCOF1VacSnr		= CSiInfo("INPACOF1VacSnr",			"IOMODULE_IN_PORT_2", BIT_1, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stCOF1VacSnr);
	m_stGlass2VacSnr		= CSiInfo("INPARVacSnr",			"IOMODULE_IN_PORT_2", BIT_2, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass2VacSnr);
	m_stCOF2VacSnr		= CSiInfo("INPACOF2VacSnr",			"IOMODULE_IN_PORT_2", BIT_3, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stCOF2VacSnr);
	m_stXLmtSnr			= CSiInfo("INPAXLmtSnr",				"IOMODULE_IN_PORT_2", BIT_4, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stXLmtSnr);	
	m_stZHomeSnr			= CSiInfo("INPAZHomeSnr",				"IOMODULE_IN_PORT_2", BIT_5, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stZHomeSnr);	
	m_stZLmtSnr			= CSiInfo("INPAZLmtSnr",				"IOMODULE_IN_PORT_2", BIT_6, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stZLmtSnr);	

	m_stUpStreamInLineSnr					= CSiInfo("UpStreamInLineSnr",			"IOMODULE_IN_PORT_9",	BIT_0,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamInLineSnr);
	m_stUpStreamReadySnr					= CSiInfo("UpStreamReadySnr",			"IOMODULE_IN_PORT_9",	BIT_1,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamReadySnr);
	m_stUpStreamStageMotionSnr				= CSiInfo("UpStreamStageMotionSnr",		"IOMODULE_IN_PORT_9",	BIT_2,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamStageMotionSnr);
	m_stUpStreamStageInPosnSnr				= CSiInfo("UpStreamStageInPosnSnr",		"IOMODULE_IN_PORT_9",	BIT_3,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamStageInPosnSnr);
	m_stUpStreamStageVacSnr					= CSiInfo("UpStreamStageVacSnr",		"IOMODULE_IN_PORT_9",	BIT_4,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamStageVacSnr);
	m_stUpStreamReceivingSnr				= CSiInfo("UpStreamReceivingSnr",		"IOMODULE_IN_PORT_9",	BIT_5,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamReceivingSnr);
	m_stUpStreamPanelTransferLSnr			= CSiInfo("UpStreamPanelTransferLSnr",	"IOMODULE_IN_PORT_9",	BIT_6,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamPanelTransferLSnr);
	m_stUpStreamPanelTransferRSnr			= CSiInfo("UpStreamPanelTransferRSnr",	"IOMODULE_IN_PORT_9",	BIT_7,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamPanelTransferRSnr);
	m_stUpStreamCancelSnr					= CSiInfo("UpStreamCancelSnr",			"IOMODULE_IN_PORT_9",	BIT_8,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamCancelSnr);
	m_stUpStreamInterLockBackSnr			= CSiInfo("UpStreamInterLockBackSnr",	"IOMODULE_IN_PORT_9",	BIT_9,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamInterLockBackSnr);
	m_stUpStreamEMOSnr						= CSiInfo("UpStreamEMOSnr",				"IOMODULE_IN_PORT_9",	BIT_10,	ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stUpStreamEMOSnr);
	

	//// So Related
	m_stGlass1VacSol		= CSoInfo("INPALVacSol",			"IOMODULE_OUT_PORT_5", BIT_6, INPICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass1VacSol);
	m_stCOF1VacSol		= CSoInfo("INPACOF1VacSol",			"IOMODULE_OUT_PORT_5", BIT_7, PICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stCOF1VacSol);
	m_stGlass2VacSol		= CSoInfo("INPARVacSol",			"IOMODULE_OUT_PORT_5", BIT_8, INPICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass2VacSol);
	m_stCOF2VacSol		= CSoInfo("INPACOF2VacSol",			"IOMODULE_OUT_PORT_5", BIT_9, INPICKARM_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stCOF2VacSol);
	m_stWeakBlowSol			= CSoInfo("INPAG1WeakBlowSol",		"IOMODULE_OUT_PORT_5", BIT_10, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stWeakBlowSol);
	m_stGlass2WeakBlowSol			= CSoInfo("INPAG2WeakBlowSol",		"IOMODULE_OUT_PORT_5", BIT_4, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stGlass2WeakBlowSol);

	m_stDownStreamInLineSol				= CSoInfo("DownStreamInLineSol",		"IOMODULE_OUT_PORT_9", BIT_12,	INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamInLineSol);
	m_stDownStreamReadySol				= CSoInfo("DownStreamReadySol",			"IOMODULE_OUT_PORT_9", BIT_13,	INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamReadySol);
	m_stDownStreamSendingSol			= CSoInfo("DownStreamSendingSol",		"IOMODULE_OUT_PORT_9", BIT_14,	INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamSendingSol);
	m_stDownStreamTAMotionSol			= CSoInfo("DownStreamTAMotionSol",		"IOMODULE_OUT_PORT_9", BIT_15, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamTAMotionSol);
	m_stDownStreamTAInPosnSol			= CSoInfo("DownStreamTAInPosnSol",		"IOMODULE_OUT_PORT_9", BIT_16, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamTAInPosnSol);
	m_stDownStreamTAVacOnOkSol			= CSoInfo("DownStreamTAVacOnOkSol",		"IOMODULE_OUT_PORT_9", BIT_17, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamTAVacOnOkSol);
	m_stDownStreamStageLVacOffSol		= CSoInfo("DownStreamStageLVacOffSol",	"IOMODULE_OUT_PORT_9", BIT_18, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamStageLVacOffSol);
	m_stDownStreamStageRVacOffSol		= CSoInfo("DownStreamStageRVacOffSol",	"IOMODULE_OUT_PORT_9", BIT_19, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamStageRVacOffSol);
	m_stDownStreamArmLVacErrorSol		= CSoInfo("DownStreamArmLVacErrorSol",	"IOMODULE_OUT_PORT_9", BIT_20, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamArmLVacErrorSol);
	m_stDownStreamArmRVacErrorSol		= CSoInfo("DownStreamArmRVacErrorSol",	"IOMODULE_OUT_PORT_9", BIT_21, INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamArmRVacErrorSol);
	m_stDownStreamCancelSol				= CSoInfo("DownStreamCancelSol",		"IOMODULE_OUT_PORT_9", BIT_22,INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamCancelSol);
	m_stDownStreamInterLockFrontSol		= CSoInfo("DownStreamInterLockFrontSol","IOMODULE_OUT_PORT_9", BIT_23,INPICKARM_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stDownStreamInterLockFrontSol);

	//m_bIsDefaultIndex	= FALSE;	// TRUE: Place Glass2 First, then Glass1

	//m_lPickGlassOffset	= 0;
	//
	//m_bIsVacError = FALSE; //20121224 vac error
	//HMI_bStandAlone		= FALSE;
	//m_bInlineInBack		= FALSE;
	m_lCurInspWH			= WH_1;
	//m_lCurGlass				= GLASS1;
}

CInPickArm::~CInPickArm()
{
}

BOOL CInPickArm::InitInstance()
{
	CTransferArm::InitInstance();
	return TRUE;
}

INT CInPickArm::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CTransferArm::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CInPickArm::Operation()
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
INT CInPickArm::MoveX(INPICKARM_X_CTRL_SIGNAL ctrl, BOOL bWait)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	INT		nResult		= GMP_SUCCESS;

	LONG lTempPosn = 0;

	CString szDebug = _T("");
	switch (ctrl)
	{
	case INPICKARM_X_CTRL_GO_STANDBY_POSN:
		INPICKARM_ARM_Posn = INPAStandby_POSN;
		lTempPosn = m_lStandbyPosn;
		break;

	case INPICKARM_X_CTRL_GO_PICK_GLASS_POSN:
		INPICKARM_ARM_Posn = INPAStandby_POSN;
		lTempPosn = m_lPickGlassPosn;
		break;

	case INPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET:
		INPICKARM_ARM_Posn = INPAStandby_POSN;
		lTempPosn = m_lPickGlassPosn + m_lPickGlassOffset;
		break;

	case INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_POSN:
		INPICKARM_ARM_Posn = WH1_POSN;
		lTempPosn = m_lPlaceGlassPosn[WH_1];
		break;

	case INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_POSN:
		INPICKARM_ARM_Posn = WH2_POSN;
		lTempPosn = m_lPlaceGlassPosn[WH_2];
		break;

	case INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_OFFSET:
		INPICKARM_ARM_Posn = WH1_POSN;
		lTempPosn = m_lPlaceGlassPosn[WH_1] + m_lPlaceGlassOffset[WH_1];
		break;

	case INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_OFFSET: 
		INPICKARM_ARM_Posn = WH2_POSN;
		lTempPosn = m_lPlaceGlassPosn[WH_2] + m_lPlaceGlassOffset[WH_2];
		break;
	}

	if (HMI_bDebugCal)	// 20140801 Yip: Only Show Message When Debug Calculation
	{
		szDebug.Format("MotorX Posn: %d", lTempPosn);
		DisplayMessage(szDebug);
	}
	return m_stMotorX.MoveAbsolute(lTempPosn, bWait);
}

INT CInPickArm::MoveZ(INPICKARM_Z_CTRL_SIGNAL ctrl, BOOL bWait)
{
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurWH;
	
	DOUBLE dDistPerCnt = 0.0;
	DOUBLE dGap = 0.0;
	DOUBLE	dThickness	= g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;

	dDistPerCnt	= m_stMotorZ.stAttrib.dDistPerCount;
	
	if (m_lCurInspWH == WH_1)
	{
		pCurWH = pCWH1;
	}
	else
	{
		pCurWH = pCWH2;
	}
	INT		nResult		= GMP_SUCCESS;

	LONG lTempPosn = 0;

	switch (ctrl)
	{
	case INPICKARM_Z_CTRL_GO_STANDBY_LEVEL:
		lTempPosn = m_lStandbyLevel;
		break;

	case INPICKARM_Z_CTRL_GO_PICK_GLASS_LEVEL:
		lTempPosn = m_lPickLevel;
		break;

	case INPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET:
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

	case INPICKARM_Z_CTRL_GO_WH1_PLACE_GLASS_LEVEL:

		lTempPosn = m_lPlaceLevel[WH_1];
		break;

	case INPICKARM_Z_CTRL_GO_WH2_PLACE_GLASS_LEVEL:

		lTempPosn = m_lPlaceLevel[WH_2];
		break;

	case INPICKARM_Z_CTRL_GO_WH1_PLACE_GLASS_OFFSET:
		//if (IsBurnInDiagBond())
		//{
		//	dGap = DistanceToCount(GAP_THICKNESS * FPC_GAP * 1000.0, dDistPerCnt); 
		//}
		//else
		//{
		//	dGap = DistanceToCount(dThickness * 1000.0, dDistPerCnt); 
		//}
		lTempPosn = m_lPlaceLevel[WH_1] + m_lPlaceLevelOffset[WH_1] + (LONG)dGap;
		break;

	case INPICKARM_Z_CTRL_GO_WH2_PLACE_GLASS_OFFSET:
		//if (IsBurnInDiagBond())
		//{
		//	dGap = DistanceToCount(GAP_THICKNESS * FPC_GAP * 1000.0, dDistPerCnt); 
		//}
		//else
		//{
		//	dGap = DistanceToCount(dThickness * 1000.0, dDistPerCnt); 
		//}
		lTempPosn = m_lPlaceLevel[WH_2] + m_lPlaceLevelOffset[WH_2] + (LONG)dGap;
		break;

	case INPICKARM_Z_CTRL_GO_PR_LEVEL:
		lTempPosn = m_lPRLevel[m_lCurInspWH][pCurWH->m_lCurGlass];
		break;

	case INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL:
		lTempPosn = m_lPRCalibLevel[m_lCurInspWH];
		break;

	case INPICKARM_Z_CTRL_GO_DL1_REF_LEVEL:
		lTempPosn = m_lDL1RefLevel[m_lCurInspWH];
		break;

	case INPICKARM_Z_CTRL_GO_DL2_REF_LEVEL:
		lTempPosn = m_lDL2RefLevel[m_lCurInspWH];
		break;

	}

	return m_stMotorZ.MoveAbsolute(lTempPosn, GMP_WAIT);
}

VOID CInPickArm::UpdateModuleSetupPosn()
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
	g_stSetupPosn[i].szPosn.Format("Place Glass Posn(WH1)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassPosn[WH_1];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 4
	g_stSetupPosn[i].szPosn.Format("Place Glass Offset(WH1)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassOffset[WH_1];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 5
	g_stSetupPosn[i].szPosn.Format("Place Glass Posn(WH2)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassPosn[WH_2];
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 6 
	g_stSetupPosn[i].szPosn.Format("Place Glass Offset(WH2)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lPlaceGlassOffset[WH_2];
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

VOID CInPickArm::UpdateModuleSetupLevel()
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
	g_stSetupLevel[i].szLevel.Format("Place WH1 Glass Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevel[WH_1];

	i++;	// 4
	g_stSetupLevel[i].szLevel.Format("Place WH1 Glass Level Offset");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= DEVICE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevelOffset[WH_1];

	i++;	// 5
	g_stSetupLevel[i].szLevel.Format("Place WH2 Glass Level");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= MACHINE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevel[WH_2];

	i++;	// 6
	g_stSetupLevel[i].szLevel.Format("Place WH2 Glass Level Offset");
	g_stSetupLevel[i].bEnable		= TRUE;
	g_stSetupLevel[i].bVisible		= TRUE;
	g_stSetupLevel[i].lType			= DEVICE_PARA;
	g_stSetupLevel[i].lLevelZ		= m_lPlaceLevelOffset[WH_2];

	

	
	for (j = i + 1; j < NUM_OF_LEVEL_Z; j++)
	{
		g_stSetupLevel[j].szLevel.Format("Reserved");
		g_stSetupLevel[j].bEnable		= FALSE;
		g_stSetupLevel[j].bVisible		= FALSE;
		g_stSetupLevel[j].lType			= MACHINE_PARA;
		g_stSetupLevel[j].lLevelZ		= 0;
	}
}

VOID CInPickArm::UpdateModuleSetupPara()
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

BOOL CInPickArm::ModuleSetupSetPara(PARA stPara)
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


BOOL CInPickArm::ModuleSetupSetLevel(LONG lSetupTitle)
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
		lOldLevel					= m_lPlaceLevel[WH_1];

		m_lPlaceLevel[WH_1]				= lTempLevel;
		
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Place Level Offset?");

		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPlaceLevelOffset[WH_1] = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
			break;
		}
		break;

	case 4:
		lOldLevel			= m_lPlaceLevelOffset[WH_1];

		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
		
		m_lPlaceLevelOffset[WH_1]	= lTempLevel - m_lPlaceLevel[WH_1] - DistanceToCount(1000 * dThickness, dDistPerCnt);
			
		lTempLevel	= m_lPlaceLevelOffset[WH_1];

		//lOldLevel					= m_lPlaceLevelOffset[WH_1];

		//m_lPlaceLevelOffset[WH_1]			= lTempLevel;
		break;

	case 5:
		lOldLevel					= m_lPlaceLevel[WH_2];

		m_lPlaceLevel[WH_2]				= lTempLevel;
		
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Place Level Offset?");

		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPlaceLevelOffset[WH_2] = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
			break;
		}
		break;

	case 6:
		lOldLevel			= m_lPlaceLevelOffset[WH_2];

		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
		
		m_lPlaceLevelOffset[WH_2]	= lTempLevel - m_lPlaceLevel[WH_2] - DistanceToCount(1000 * dThickness, dDistPerCnt);
			
		lTempLevel	= m_lPlaceLevelOffset[WH_2];

		//lOldLevel					= m_lPlaceLevelOffset[WH_1];

		//m_lPlaceLevelOffset[WH_1]			= lTempLevel;
		break;
	}

	CPI9000Stn::ModuleSetupSetLevel(lSetupTitle, lOldLevel, lTempLevel);

	return bResult;
}

BOOL CInPickArm::ModuleSetupSetPosn(LONG lSetupTitle)
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
		mtrOld.x			= m_lPlaceGlassPosn[WH_1];

		m_lPlaceGlassPosn[WH_1]	= TempPosn.x;
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Place Glass Offset?");
		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPlaceGlassOffset[WH_1] = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
		}
		break;

	case 4: 
		mtrOld.x			= m_lPlaceGlassOffset[WH_1];

		m_lPlaceGlassOffset[WH_1]	= TempPosn.x - m_lPlaceGlassPosn[WH_1];

		TempPosn.x			= m_lPlaceGlassOffset[WH_1];
		break;

	case 5:
		mtrOld.x			= m_lPlaceGlassPosn[WH_2];

		m_lPlaceGlassPosn[WH_2]	= TempPosn.x;
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Reset Place Glass Offset?");
		if (lAnswer == rMSG_CONTINUE)
		{
			m_lPlaceGlassOffset[WH_2] = 0;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			bResult = FALSE;
		}
		break;

	case 6: 
		mtrOld.x			= m_lPlaceGlassOffset[WH_2];

		m_lPlaceGlassOffset[WH_2]	= TempPosn.x - m_lPlaceGlassPosn[WH_2];

		TempPosn.x			= m_lPlaceGlassOffset[WH_2];
		break;
	}

	CPI9000Stn::ModuleSetupSetPosn(lSetupTitle, "X", mtrOld.x, TempPosn.x);

	return TRUE;
}

BOOL CInPickArm::ModuleSetupGoPosn(LONG lSetupTitle)
{
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");
	INT nResult = GMP_SUCCESS;
	CString szMsg;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "GO POSN OPERATION", szMsg);
		return FALSE;
	}
	if (!pCOutPickArm->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCOutPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "GO POSN OPERATION", szMsg);
		return FALSE;
	}

	if (
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS
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
		nResult = SetupGoPickGlassPosn();
		break;
		
	case 2:
		nResult = SetupGoPickGlassOffset();
		break;

	case 3:
		nResult = SetupGoPlaceWH1GlassPosn();
		break;

	case 4: 
		nResult = SetupGoPlaceWH1GlassOffset();
		break;

	case 5:
		nResult = SetupGoPlaceWH2GlassPosn();
		break;

	case 6: 
		nResult = SetupGoPlaceWH2GlassOffset();
		break;
	}
	
	return nResult;
}

INT CInPickArm::SetupGoPickGlassPosn()
{
	INT nResult = GMP_SUCCESS;


	nResult = MoveX(INPICKARM_X_CTRL_GO_PICK_GLASS_POSN, GMP_WAIT);

	return nResult;
}

INT CInPickArm::SetupGoStandbyPosn()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPickGlassOffset()
{
	INT nResult = GMP_SUCCESS;


	nResult = MoveX(INPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT);

	return nResult;
}

INT CInPickArm::SetupGoStandbyLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPickLevel()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(INPICKARM_Z_CTRL_GO_PICK_GLASS_LEVEL, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPickLevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(INPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH1Level()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(INPICKARM_Z_CTRL_GO_WH1_PLACE_GLASS_LEVEL, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH1LevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(INPICKARM_Z_CTRL_GO_WH1_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH2Level()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(INPICKARM_Z_CTRL_GO_WH2_PLACE_GLASS_LEVEL, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH2LevelOffset()
{
	INT nResult = GMP_SUCCESS;

	nResult = MoveZ(INPICKARM_Z_CTRL_GO_WH2_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH1GlassPosn()
{
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;


	lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	if (lAnswer == rMSG_YES)
	{
		if (
			MoveZToStandbyLevel(GMP_WAIT) != GMP_SUCCESS ||
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			pCWH1->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
	//		pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
	//		pCInspWH->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
			)
		{
			return GMP_FAIL;
		}
	}
	else if (lAnswer == rMSG_TIMEOUT)
	{
		return GMP_FAIL;
	}

	nResult = MoveX(INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_POSN, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH2GlassPosn()
{
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;


	lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	if (lAnswer == rMSG_YES)
	{
		if (
			MoveZToStandbyLevel(GMP_WAIT) != GMP_SUCCESS ||
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			pCWH2->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
	//		pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
	//		pCInspWH->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
			)
		{
			return GMP_FAIL;
		}
	}
	else if (lAnswer == rMSG_TIMEOUT)
	{
		return GMP_FAIL;
	}

	nResult = MoveX(INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_POSN, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH1GlassOffset() //20121011
{
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;


	lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	if (lAnswer == rMSG_YES)
	{
		if (
			MoveZToStandbyLevel(GMP_WAIT) != GMP_SUCCESS ||
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			pCWH1->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
		   )
		{
			return GMP_FAIL;
		}
	}
	else if (lAnswer == rMSG_TIMEOUT)
	{
		return GMP_FAIL;
	}

	nResult = MoveX(INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::SetupGoPlaceWH2GlassOffset() //20121011
{
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	INT nResult = GMP_SUCCESS;
	LONG lAnswer = rMSG_CANCEL;


	lAnswer = HMIMessageBox(MSG_YES_CANCEL, "OPTION", "Move INSPWH Y to Load Glass Posn?");

	if (lAnswer == rMSG_YES)
	{
		if (
			MoveZToStandbyLevel(GMP_WAIT) != GMP_SUCCESS ||
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS ||
			pCWH2->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS
		   )
		{
			return GMP_FAIL;
		}
	}
	else if (lAnswer == rMSG_TIMEOUT)
	{
		return GMP_FAIL;
	}

	nResult = MoveX(INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_OFFSET, GMP_WAIT);	

	return nResult;
}

INT CInPickArm::MoveZToStandbyLevel(BOOL bWait)
{
		if (
			MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
		)
		{
			return GMP_FAIL;
		}
		if (bWait == GMP_WAIT)
		{
			if (
				m_stMotorZ.Sync() != GMP_SUCCESS
			)
			{
				return GMP_FAIL;
			}
		}
		return GMP_SUCCESS;
}

INT CInPickArm::MoveZToPRLevel(BOOL bWait)
{
		if (
			MoveZ(INPICKARM_Z_CTRL_GO_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
		)
		{
			return GMP_FAIL;
		}
		if (bWait == GMP_WAIT)
		{
			if (
				m_stMotorZ.Sync() != GMP_SUCCESS
			)
			{
				return GMP_FAIL;
			}
		}
		return GMP_SUCCESS;
}

BOOL CInPickArm::ModuleSetupGoLevel(LONG lSetupTitle)
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
		nResult = SetupGoPlaceWH1Level();
		break;

	case 4:
		nResult = SetupGoPlaceWH1LevelOffset();	
		break;

	case 5:
		nResult = SetupGoPlaceWH2Level();
		break;

	case 6:
		nResult = SetupGoPlaceWH2LevelOffset();	
		break;
	}

	if (nResult != GMP_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}

VOID CInPickArm::ModuleSetupPreTask()
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	pCWH1->m_bCheckMotor	= TRUE;
	pCWH1->m_bCheckIO		= TRUE;
	pCWH2->m_bCheckMotor	= TRUE;
	pCWH2->m_bCheckIO		= TRUE;

	m_bCheckMotor		= TRUE;
	m_bCheckIO			= TRUE;
	
	CheckModSelected(TRUE);
	
	SetDiagnSteps(g_lDiagnSteps);

	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();
}

VOID CInPickArm::ModuleSetupPostTask()
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	pCWH1->m_bCheckMotor	= FALSE;
	pCWH1->m_bCheckIO		= FALSE;
	pCWH2->m_bCheckMotor	= FALSE;
	pCWH2->m_bCheckIO		= FALSE;

	m_bCheckMotor		= FALSE;
	m_bCheckIO			= FALSE;

}

VOID CInPickArm::UpdateOutput()
{
	// Keep it for debug breakpoint

	CTransferArm::UpdateOutput();
}

VOID CInPickArm::RegisterVariables()
{
	try
	{
		////Register Sensors here 
		//// -------------------------------------------------------
		//// PI9000Stn Variable and Function
		//// -------------------------------------------------------
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

		//// -------------------------------------------------------
		//// TransferArm Variable and Function
		//// -------------------------------------------------------
		RegVariable(_T("HMI_bInPickArmXOn"), &m_stMotorX.bHMI_On);
		RegVariable(_T("HMI_lInPickArmXEncoder"), &m_stMotorX.lHMI_CurrPosn);
		RegVariable(GetStation() + _T("dXDistPerCount"), &m_stMotorX.stAttrib.dDistPerCount);

		RegVariable(_T("HMI_bInPickArmZOn"), &m_stMotorZ.bHMI_On);
		RegVariable(_T("HMI_lInPickArmZEncoder"), &m_stMotorZ.lHMI_CurrPosn);
		RegVariable(GetStation() + _T("dZDistPerCount"), &m_stMotorZ.stAttrib.dDistPerCount);

		RegVariable(_T("HMI_bInPickArmXLmtSnr"), &m_stXLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmZLmtSnr"), &m_stZLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmZHomeSnr"), &m_stZHomeSnr.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmG1VacSnr"), &m_stGlass1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmG2VacSnr"), &m_stGlass2VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmCOF1VacSnr"), &m_stCOF1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmCOF2VacSnr"), &m_stCOF2VacSnr.bHMI_Status);

		RegVariable(_T("HMI_bUpStreamInLineSnr"), &m_stUpStreamInLineSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamReadySnr"), &m_stUpStreamReadySnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamStageMotionSnr"), &m_stUpStreamStageMotionSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamStageInPosnSnr"), &m_stUpStreamStageInPosnSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamStageVacSnr"), &m_stUpStreamStageVacSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamReceivingSnr"), &m_stUpStreamReceivingSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamPanelTransferLSnr"), &m_stUpStreamPanelTransferLSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamPanelTransferRSnr"), &m_stUpStreamPanelTransferRSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamCancelSnr"), &m_stUpStreamCancelSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamInterLockBackSnr"), &m_stUpStreamInterLockBackSnr.bHMI_Status);
		RegVariable(_T("HMI_bUpStreamEMOSnr"), &m_stUpStreamEMOSnr.bHMI_Status);

		RegVariable(_T("HMI_bDownStreamInLineSol"), &m_stDownStreamInLineSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamReadySol"), &m_stDownStreamReadySol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamSendingSol"), &m_stDownStreamSendingSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamTAMotionSol"), &m_stDownStreamTAMotionSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamTAInPosnSol"), &m_stDownStreamTAInPosnSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamTAVacOnOkSol"), &m_stDownStreamTAVacOnOkSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamStageLVacOffSol"), &m_stDownStreamStageLVacOffSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamStageRVacOffSol"), &m_stDownStreamStageRVacOffSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamArmLVacErrorSol"), &m_stDownStreamArmLVacErrorSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamArmRVacErrorSol"), &m_stDownStreamArmRVacErrorSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamCancelSol"), &m_stDownStreamCancelSol.bHMI_Status);
		RegVariable(_T("HMI_bDownStreamInterLockFrontSol"), &m_stDownStreamInterLockFrontSol.bHMI_Status);
		
		RegVariable(_T("HMI_bInPickArmG1VacSol"), &m_stGlass1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmG2VacSol"), &m_stGlass2VacSol.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmWeakBlowSol"), &m_stWeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmWeakBlow2Sol"), &m_stGlass2WeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmCOF1VacSol"), &m_stCOF1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bInPickArmCOF2VacSol"), &m_stCOF2VacSol.bHMI_Status);


		RegVariable(_T("HMI_bStandAlone"), &HMI_bStandAlone);
		RegVariable(GetStation() + _T("lPickPlaceRetryLimit"), &m_lPickPlaceRetryLimit);
		//RegVariable(GetStation() + _T("bInlineInBack"), &m_bInlineInBack);

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

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamInLineSol"), HMI_ToggleDownStreamInLineSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamReadySol"), HMI_ToggleDownStreamReadySol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamSendingSol"), HMI_ToggleDownStreamSendingSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamTAMotionSol"), HMI_ToggleDownStreamTAMotionSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamTAInPosnSol"), HMI_ToggleDownStreamTAInPosnSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamTAVacOnOkSol"), HMI_ToggleDownStreamTAVacOnOkSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamStageLVacOffSol"), HMI_ToggleDownStreamStageLVacOffSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamStageRVacOffSol"), HMI_ToggleDownStreamStageRVacOffSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamArmLVacErrorSol"), HMI_ToggleDownStreamArmLVacErrorSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamArmRVacErrorSol"), HMI_ToggleDownStreamArmRVacErrorSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamCancelSol"), HMI_ToggleDownStreamCancelSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDownStreamInterLockFrontSol"), HMI_ToggleDownStreamInterLockFrontSol);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleInlineInBack"), HMI_ToggleInlineInBack);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetStandAlone"), HMI_SetStandAlone);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXPos"), HMI_IndexXPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXNeg"), HMI_IndexXNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZPos"), HMI_IndexZPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZNeg"), HMI_IndexZNeg);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPickPlaceRetryLimit"), HMI_SetPickPlaceRetryLimit);

		//// -------------------------------------------------------
		//// varibales
		//// -------------------------------------------------------
	
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PickGlassOperation"), HMI_PickGlassOperation);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PlaceGlassOperation"), HMI_PlaceGlassOperation);
		//
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

BOOL CInPickArm::PickGlassOperation()
{
	BOOL bResult = FALSE;
	//LONG lResponse = HMISelectionBox("PICK GLASS OPERATION", "Please select Where to pick:", "Insp WH", "ShBuffer WH");	

	//	if (lResponse == -1 || lResponse == 11)
	//	{
	//		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
	//		return FALSE;
	//	}

	//	switch (lResponse)
	//	{
	//	case 0:
			bResult = PickGlass(NO_GLASS);
	//		break;

	//	case 1:
	//		bResult = PickGlassFromShbufOperation(NO_GLASS);
	//		break;
	//	}
	return bResult;
}

BOOL CInPickArm::PlaceGlassOperation()
{
	BOOL bResult = FALSE;
	LONG lResponse = HMISelectionBox("PLACE GLASS OPERATION", "Please select Where to Place:", "WH1", "WH2");	

		if (lResponse == -1 || lResponse == 11)
		{
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Operation Abort!");
			return FALSE;
		}

		switch (lResponse)
		{
		case 0:
			bResult = PlaceGlassToInspWH(NO_GLASS, WH_1);
			break;

		case 1:
			bResult = PlaceGlassToInspWH(NO_GLASS, WH_2);
			break;

		}
	return bResult;
}

BOOL CInPickArm::PlaceGlassToInspWH(GlassNum lGlassNum, LONG lCurWH)
{
	//LONG lDelay;
	CString strMsg;

	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");

	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurInspWH;

	INPICKARM_X_CTRL_SIGNAL  PLACE_INSPWH_POSN;

	BOOL bWHGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bArmGlassStatus[MAX_NUM_OF_GLASS] = {FALSE, FALSE};
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	if(lCurWH == WH_1)
	{
		pCurInspWH = pCWH1;
		PLACE_INSPWH_POSN = INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_OFFSET;
	}
	else
	{
		pCurInspWH = pCWH2;
		PLACE_INSPWH_POSN = INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_OFFSET;
	}

	if (lGlassNum != NO_GLASS)
	{
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", "Contact Software");
		return FALSE;
	}

	if (!m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCOutPickArm->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCOutPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (!pCurInspWH->m_bModSelected)
	{
		strMsg.Format("%s Module Not Selected. Operation Abort!", pCurInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("INPA Move To Standby Level Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPA Move To Standby Level Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("OUTPA Move To Standby Position Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
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

	if ((lGlassNum = CalcPlaceStatus(bWHGlassStatus, bArmGlassStatus)) == NO_GLASS)
	{
		strMsg.Format("Invalid Glass Status at %s and %s", GetStnName(), pCurInspWH->GetStnName());
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (MoveX(PLACE_INSPWH_POSN, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("INPA Move To Place Position Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCurInspWH->m_stMotorT.MoveAbsolute(pCurInspWH->m_lStandbyPosnT, GMP_NOWAIT) != GMP_SUCCESS)
	{
		strMsg.Format("InspWH Move To Load Offset Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (pCurInspWH->MoveXY(INSPWH_XY_CTRL_GO_LOAD_OFFSET, GMP_WAIT) != GMP_SUCCESS)
	{
		strMsg.Format("InspWH Move To Load Offset Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}

	if (PlaceGlass(lGlassNum, lCurWH) != GMP_SUCCESS)
	{
		strMsg.Format("INPICKARM Place Glass To InspWH Error. Operation Abort!");
		HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
		return FALSE;
	}
	
	if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
	{
		if (!pCurInspWH->IsVacSensorOn(GLASS1))
		{
			SetVacSol(GLASS1, ON, GMP_WAIT);
			strMsg.Format("Place Glass Error: %s Glass1 Vac Sensor Not On.", pCurInspWH->GetStnName());
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}	

	if (lGlassNum == GLASS2 || lGlassNum == MAX_NUM_OF_GLASS)
	{

		if (!pCurInspWH->IsVacSensorOn(GLASS2))
		{
			SetVacSol(GLASS1, ON, GMP_WAIT);
			strMsg.Format("Place Glass Error: %s Glass2 Vac Sensor Not On.", pCurInspWH->GetStnName());
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult2 = FALSE;
		}
	}	

	if (bResult1 && bResult2)
	{
		if (MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
		{
			strMsg.Format("INPICKARM Place Glass Go Standby Posn Error. Operation Abort!");
			HMIMessageBox(MSG_OK, "PLACE GLASS OPERATION", strMsg);
			bResult1 = FALSE;
		}
	}
	return bResult1 && bResult2;
}

LONG CInPickArm::HMI_PickGlassOperation(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bOpDone						= TRUE;
	BOOL bResult						= FALSE;


	bResult = PickGlassOperation();
	if (bResult)
	{
		MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CInPickArm::HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bOpDone						= TRUE;
	LONG lResponse						= 0;
	BOOL bResult						= TRUE;

	bResult = PlaceGlassOperation();

	if (bResult)
	{
		MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	}
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CInPickArm::IPC_SaveMachineParam()
{	
	CString str;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	//pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]	= m_lShBufPlaceGlassPosn;
	pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bInlineInBack"]		= m_bInlineInBack; 

	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
	{
		str.Format("WH_%ld", j + 1);
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_lPlaceGlassPosn"]				= m_lPlaceGlassPosn[j];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPlaceLevel"]					= m_lPlaceLevel[j];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"]				= m_lPRCalibLevel[j];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lDL1RefLevel"]					= m_lDL1RefLevel[j];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lDL2RefLevel"]					= m_lDL2RefLevel[j];
	}

	return CTransferArm::IPC_SaveMachineParam();		
}

LONG CInPickArm::IPC_LoadMachineParam()
{
	CString str,str1;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	
	//m_lShBufPlaceGlassPosn	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lShBufPlaceGlassPosn"]; 
	m_bInlineInBack			= (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bInlineInBack"];

	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
	{
		str.Format("WH_%ld", j + 1);
		m_lPlaceGlassPosn[j]		= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_lPlaceGlassPosn"];
		m_lPlaceLevel[j]			= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPlaceLevel"];
		m_lPRCalibLevel[j]			= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"];
		m_lDL1RefLevel[j]			= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lDL1RefLevel"];
		m_lDL2RefLevel[j]			= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lDL2RefLevel"];
		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{
			str1.Format("GLASS%ld", i + 1);
			m_lPRLevel[j][i]			= pAppMod->m_smfDevice[GetStnName()]["Level"][str][str1]["m_lPRLevel"];
		}

	}
	return CTransferArm::IPC_LoadMachineParam();
}

LONG CInPickArm::IPC_SaveDeviceParam()
{
	CString str,str1;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szTmp;
	szTmp = GetStnName();
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"]		= m_lPickGlassOffset;
	//pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]		= m_lPlaceGlassOffset; 
	//pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]	= m_lShBufPlaceGlassOffset; 

	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
	{
		str.Format("WH_%ld", j + 1);
		pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_lPlaceGlassOffset"]		= m_lPlaceGlassOffset[j];
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lPlaceLevelOffset"]		= m_lPlaceLevelOffset[j];
		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{
			str1.Format("GLASS%ld", i + 1);
			pAppMod->m_smfDevice[GetStnName()]["Level"][str][str1]["m_lPRLevel"]				= m_lPRLevel[j][i];
		}
	}
	return CTransferArm::IPC_SaveDeviceParam();
}

LONG CInPickArm::IPC_LoadDeviceParam()
{
	CString str;
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	
	m_lPickGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPickGlassOffset"];
	//m_lPlaceGlassOffset			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lPlaceGlassOffset"]; 
	//m_lShBufPlaceGlassOffset	= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_lShBufPlaceGlassOffset"]; 

	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
	{
		str.Format("WH_%ld", j + 1);
		m_lPlaceGlassOffset[j]			= pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_lPlaceGlassOffset"];
		m_lPlaceLevelOffset[j]			= pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_lPlaceLevelOffset"];

	}

	return CTransferArm::IPC_LoadDeviceParam();
}

STATUS_TYPE CInPickArm::IsUpStreamUnloadReady()
{
	if (AutoMode == BURN_IN || AutoMode == DIAG_BOND || HMI_bStandAlone)
	{
		return ST_UNLOAD_READY;
	}
	if (
		IsUpStreamInLineSnrOn() &&
		IsUpStreamReadySnrOn() &&
		IsUpStreamReceivingSnrOn()
		)
	{
		return ST_UNLOAD_READY;
	}
	else if (!IsUpStreamInLineSnrOn() || !IsUpStreamReadySnrOn())
	{
		return ST_STOP;
	}
	else
	{
		return ST_IDLE;
	}
}

INT CInPickArm::GetPickAction()
{
	if (AutoMode == BURN_IN || AutoMode == DIAG_BOND)
	{
		PPAction = MAX_NUM_OF_GLASS;
		return GMP_SUCCESS;
	}

	if (!IsUpStreamInLineSnrOn() || !IsUpStreamReadySnrOn())
	{
		return GMP_FAIL;
	}
	else if (
		IsUpStreamPanelTransferLeftSnrOn() &&
		IsUpStreamPanelTransferRightSnrOn()
		)
	{
		PPAction = MAX_NUM_OF_GLASS;
		return GMP_SUCCESS;
	}
	else if (IsUpStreamPanelTransferLeftSnrOn())
	{
		PPAction = GLASS1;
		return GMP_SUCCESS;
	}
	else if (IsUpStreamPanelTransferRightSnrOn())
	{
		PPAction = GLASS2;
		return GMP_SUCCESS;
	}
	else
	{
		return ST_IDLE;
	}
}

GlassNum CInPickArm::GetUpStreamGlassStatus()
{
	return MAX_NUM_OF_GLASS;
}

BOOL CInPickArm::IsUpStreamInLineSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamInLineSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamReadySnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamInLineSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamStageMotionSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamStageMotionSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamStageInPosnSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamStageInPosnSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamStageVACSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamStageVacSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamReceivingSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamReceivingSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamPanelTransferLeftSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamPanelTransferLSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamPanelTransferRightSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamPanelTransferRSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamCancelSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamCancelSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamInterlockBackSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamInterLockBackSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CInPickArm::IsUpStreamEMOSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stUpStreamEMOSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

//AutoGen
INT CInPickArm::SetDownStreamInLineSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamInLineSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif

	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamReadySol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamReadySol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}


	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamSendingSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamSendingSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}


	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamTAMotionSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamTAMotionSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}


	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamTAInPosnSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamTAInPosnSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamTAVacOnOkSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamTAVacOnOkSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}


	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamStageLVacOffSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamStageLVacOffSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamStageRVacOffSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamStageRVacOffSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamArmLVacErrorSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamArmLVacErrorSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif


	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamArmRVacErrorSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamArmRVacErrorSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif

	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamCancelSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamCancelSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif

	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}
//AutoGen
INT CInPickArm::SetDownStreamInterLockFrontSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stDownStreamInterLockFrontSol;
#ifdef OFFLINE
	stTempSo->bHMI_Status = bMode;
	return nResult;
#endif
#ifdef _DEBUG
	
	if (!theApp.IsInitNuDrive())
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}
#endif

	if ((State() == AUTO_Q || State() == DEMO_Q || State() == PRESTART_Q || State() == STOPPING_Q) && (AutoMode == BURN_IN || AutoMode == DIAG_BOND))//20150630
	{
		stTempSo->bHMI_Status = bMode;
		return nResult;
	}

	try
	{
		if (bMode)
		{
			TC_AddJob(stTempSo->GetTCObjectID());
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
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
				Sleep(stTempSo->GetOffDelay());
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
	return nResult;
}

//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamInLineSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamInLineSol(!m_stDownStreamInLineSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamReadySol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamReadySol(!m_stDownStreamReadySol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamSendingSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamSendingSol(!m_stDownStreamSendingSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamTAMotionSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamTAMotionSol(!m_stDownStreamTAMotionSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamTAInPosnSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamTAInPosnSol(!m_stDownStreamTAInPosnSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamTAVacOnOkSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamTAVacOnOkSol(!m_stDownStreamTAVacOnOkSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamStageLVacOffSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamStageLVacOffSol(!m_stDownStreamStageLVacOffSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamStageRVacOffSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamStageRVacOffSol(!m_stDownStreamStageRVacOffSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamArmLVacErrorSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamArmLVacErrorSol(!m_stDownStreamArmLVacErrorSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamArmRVacErrorSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamArmRVacErrorSol(!m_stDownStreamArmRVacErrorSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamCancelSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamCancelSol(!m_stDownStreamCancelSol.bHMI_Status);
	return 0;
}
//AutoGen
LONG CInPickArm::HMI_ToggleDownStreamInterLockFrontSol(IPC_CServiceMessage &svMsg)
{
	SetDownStreamInterLockFrontSol(!m_stDownStreamInterLockFrontSol.bHMI_Status);
	return 0;
}

BOOL CInPickArm::HS_IsWHSendReady()
{

	BOOL bResult = FALSE;


	if (IsHandShakeBitOn(m_stUpStreamReceivingSnr/*m_stUpStreamReadySnr*/))
	{
		if (IsHandShakeBitOn(m_stUpStreamPanelTransferLSnr))	
		{
			m_bLPanelTransfer = TRUE;
		}
		else
		{
			m_bLPanelTransfer = FALSE;
		}
		
		if (IsHandShakeBitOn(m_stUpStreamPanelTransferRSnr))
		{
			m_bRPanelTransfer = TRUE;
		}
		else
		{
			m_bRPanelTransfer = FALSE;
		}

		bResult = TRUE;
	}

	return bResult;
}

VOID CInPickArm::HS_InPickArmSendReadyACK()
{
	SetHandShakeBit(&m_stDownStreamSendingSol, ON);
}

BOOL CInPickArm::HS_IsHandshakeCancel()
{

	BOOL bResult = FALSE;

	if (IsBurnInDiagBond())
	{
		return FALSE;
	}
	if (IsHandShakeBitOn(m_stUpStreamCancelSnr) || !IsHandShakeBitOn(m_stUpStreamReadySnr) || !IsHandShakeBitOn(m_stUpStreamInLineSnr))
	{
		bResult = TRUE;
	}

	return bResult;
}

VOID CInPickArm::HS_ClearAllHandShakeSignal()
{
	SetHandShakeBit(&m_stDownStreamSendingSol, OFF);
	SetHandShakeBit(&m_stDownStreamStageLVacOffSol, OFF);
	SetHandShakeBit(&m_stDownStreamStageRVacOffSol, OFF);
	m_bLPanelTransfer = m_bRPanelTransfer = FALSE;
}

VOID CInPickArm::HS_RequestWHVacOFF()
{

	if (m_bLPanelTransfer)
	{
		SetHandShakeBit(&m_stDownStreamStageLVacOffSol, ON);
	}

	if (m_bRPanelTransfer)
	{
		SetHandShakeBit(&m_stDownStreamStageRVacOffSol, ON);
	}
}

BOOL CInPickArm::HS_IsWHVacOff()
{
	BOOL bResultL = FALSE;
	BOOL bResultR = FALSE;

	if (m_bLPanelTransfer)
	{
		if (!IsHandShakeBitOn(m_stUpStreamPanelTransferLSnr))	
		{
			bResultL = TRUE;
		}
	}
	else
	{
		bResultL = TRUE;
	}

	if (m_bRPanelTransfer)
	{
		if (!IsHandShakeBitOn(m_stUpStreamPanelTransferRSnr))	
		{
			bResultR = TRUE;
		}
	}
	else
	{
		bResultR = TRUE;
	}

	return (bResultL && bResultR);
}

VOID CInPickArm::HS_SendInPickArmReceivedPanel()
{
	SetHandShakeBit(&m_stDownStreamSendingSol, OFF);
	SetHandShakeBit(&m_stDownStreamStageLVacOffSol, OFF);
	SetHandShakeBit(&m_stDownStreamStageRVacOffSol, OFF);
}

VOID CInPickArm::UpdateInPickArmStatus()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	m_bGlass1Exist = (IsGLASS1_VacSensorOn() || IsBurnInDiagBond());
	m_bGlass2Exist = (IsGLASS2_VacSensorOn() || IsBurnInDiagBond());


	if (m_bGlass1Exist)
	{
		INPICKARM_GLASS_Status[GLASS1] = ST_READY;
		pAppMod->m_ulGlassPicked++;
		pAppMod->m_ulTotalGlassPicked++;
	}
	else
	{
		INPICKARM_GLASS_Status[GLASS1] = ST_EMPTY;
	}


	if (m_bGlass2Exist)
	{
		INPICKARM_GLASS_Status[GLASS2] = ST_READY;
		pAppMod->m_ulGlassPicked++;
		pAppMod->m_ulTotalGlassPicked++;
	}
	else
	{
		INPICKARM_GLASS_Status[GLASS2] = ST_EMPTY;
	}
}

BOOL CInPickArm::IsHandShakeBitOn(CSiInfo stTempSi)
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

INT CInPickArm::SetHandShakeBit(CSoInfo *stTempSo, BOOL bMode)
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

VOID CInPickArm::InPickArmVacOn()
{
	if (m_bLPanelTransfer)
	{
		SetGLASS1_VacSol(ON, GMP_WAIT);
	}

	if (m_bRPanelTransfer)
	{
		SetGLASS2_VacSol(ON, GMP_WAIT);
	}

	if (m_bLPanelTransfer && m_bRPanelTransfer)
	{
		SetGLASS1_VacSol(ON, GMP_NOWAIT);
		SetGLASS2_VacSol(ON, GMP_WAIT);
	}

}

VOID CInPickArm::InPickArmVacOff()
{
	SetGLASS1_VacSol(OFF, GMP_NOWAIT);
	SetGLASS2_VacSol(OFF, GMP_WAIT);
}

BOOL CInPickArm::IsInPickArmGlassExist()
{
	BOOL bResultL = FALSE;
	BOOL bResultR = FALSE;

	if (m_bLPanelTransfer)
	{
		bResultL = IsGLASS1_VacSensorOn();
		m_bGlass1Exist = bResultL;
	}
	else
	{
		bResultL = TRUE;
	}

	if (m_bRPanelTransfer)
	{
		bResultR = IsGLASS2_VacSensorOn();
		m_bGlass2Exist = bResultR;
	}
	else
	{
		bResultR = TRUE;
	}

	return (bResultL && bResultR);
}

INT CInPickArm::MoveXToPlacePosn(LONG lCurWH, BOOL bWait)
{
	INT		nResult		= GMP_SUCCESS;
	
	switch (lCurWH)
	{
	case WH_1:
		return MoveX(INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_OFFSET, bWait);
		break;
	
	case WH_2:
		return MoveX(INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_OFFSET, bWait);
		break;

	default:
		DisplayMessage(__FUNCTION__);
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
}

//////////////////////////////////////////////////////////
//// New functions for PI9000 WH1, WH2
///////////////////////////////////////////////////////////
//// WH1
//BOOL CInPickArm::IsPlaceToWH1VacSensorOn(LONG lGlassNum)
//{
//	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
//
//	if (lGlassNum == GLASS1)
//	{
//		return pCWH1->IsGLASS1_VacSensorOn();
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		return pCWH1->IsGLASS2_VacSensorOn();
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return FALSE;
//	}
//	return FALSE;
//}

INT CInPickArm::SetPlaceToWH1VacSol(LONG lGlassNum, BOOL bMode, BOOL bWait)
{
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");

	LONG lDelay;

	if (lGlassNum == GLASS1)
	{
		return pCWH1->SetVacSol((GlassNum)lGlassNum, bMode, bWait);
	}
	else if (lGlassNum == GLASS2)
	{
		return pCWH1->SetVacSol((GlassNum)lGlassNum, bMode, bWait);
	}
	else
	{
		if (
			pCWH1->SetVacSol((GlassNum)lGlassNum, bMode, GMP_NOWAIT) != GMP_SUCCESS
		   )
		{
			return GMP_FAIL;
		}
		if (bWait == GMP_WAIT)
		{
			if (bMode)
			{
				lDelay = pCWH1->m_stGlass1VacSol.GetOnDelay();
				if (lDelay < pCWH1->m_stGlass2VacSol.GetOnDelay())
				{
					lDelay = pCWH1->m_stGlass2VacSol.GetOnDelay();
				}
			}
			else
			{
				lDelay = pCWH1->m_stGlass1VacSol.GetOffDelay();
				if (lDelay < pCWH1->m_stGlass2VacSol.GetOffDelay())
				{
					lDelay = pCWH1->m_stGlass2VacSol.GetOffDelay();
				}
			}
			
			Sleep(lDelay);

			return GMP_SUCCESS;
		}
	}

	return GMP_SUCCESS;
}

//// WH2
//BOOL CInPickArm::IsPlaceToWH2VacSensorOn(LONG lGlassNum)
//{
//	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
//
//	if (lGlassNum == GLASS1)
//	{
//		return pCWH2->IsGLASS1_VacSensorOn();
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		return pCWH2->IsGLASS2_VacSensorOn();
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return FALSE;
//	}
//	return FALSE;
//}

INT CInPickArm::SetPlaceToWH2VacSol(LONG lGlassNum, BOOL bMode, BOOL bWait)
{
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");

	LONG lDelay;

	if (lGlassNum == GLASS1)
	{
		return pCWH2->SetVacSol((GlassNum)lGlassNum, bMode, bWait);
	}
	else if (lGlassNum == GLASS2)
	{
		return pCWH2->SetVacSol((GlassNum)lGlassNum, bMode, bWait);
	}
	else
	{
		if (
			pCWH2->SetVacSol((GlassNum)lGlassNum, bMode, GMP_NOWAIT) != GMP_SUCCESS
		   )
		{
			return GMP_FAIL;
		}
		if (bWait == GMP_WAIT)
		{
			if (bMode)
			{
				lDelay = pCWH2->m_stGlass1VacSol.GetOnDelay();
				if (lDelay < pCWH2->m_stGlass2VacSol.GetOnDelay())
				{
					lDelay = pCWH2->m_stGlass2VacSol.GetOnDelay();
				}
			}
			else
			{
				lDelay = pCWH2->m_stGlass1VacSol.GetOffDelay();
				if (lDelay < pCWH2->m_stGlass2VacSol.GetOffDelay())
				{
					lDelay = pCWH2->m_stGlass2VacSol.GetOffDelay();
				}
			}
			
			Sleep(lDelay);

			return GMP_SUCCESS;
		}
	}

	return GMP_SUCCESS;
}

INT CInPickArm::PlaceGlass(GlassNum lGlassNum, LONG lCurWH)
{
	LONG lDelay;
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");

	if (lCurWH == WH_1)
	{
		SetPlaceToWH1VacSol(lGlassNum, ON);
		
			if ((m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_WH1_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
			{
				lDelay = max(pCWH1->m_stGlass1VacSol.GetOnDelay(), m_stGlass1VacSol.GetOffDelay());
				if (m_bUseCOFVacuum)
				{
					lDelay = max(lDelay, m_stCOF1VacSol.GetOffDelay());
				}

				SetVacSol(lGlassNum, OFF);
				Sleep(lDelay);

				if (m_stWeakBlowSol.GetOnDelay() > 0)
				{
					SetWeakBlowSol(ON, GMP_WAIT);
					SetG2WeakBlowSol(ON, GMP_WAIT);
					SetWeakBlowSol(OFF, GMP_WAIT);
					SetG2WeakBlowSol(OFF, GMP_WAIT);
				}
				m_lCurInspWH = WH_1;
				return m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_PR_LEVEL, GMP_WAIT);
			}
		
	}
	else if (lCurWH == WH_2)
	{
		SetPlaceToWH2VacSol(lGlassNum, ON);
		
			if ((m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_WH2_PLACE_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
			{
				lDelay = max(pCWH2->m_stGlass1VacSol.GetOnDelay(), m_stGlass1VacSol.GetOffDelay());
				if (m_bUseCOFVacuum)
				{
					lDelay = max(lDelay, m_stCOF1VacSol.GetOffDelay());
				}

				SetVacSol(lGlassNum, OFF);
				Sleep(lDelay);

				if (m_stWeakBlowSol.GetOnDelay() > 0)
				{
					SetWeakBlowSol(ON, GMP_WAIT);
					SetG2WeakBlowSol(ON, GMP_WAIT);
					SetWeakBlowSol(OFF, GMP_WAIT);
					SetG2WeakBlowSol(OFF, GMP_WAIT);
				}

				m_lCurInspWH = WH_2;
				return m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_PR_LEVEL, GMP_WAIT);
			}
		
	}
	else
	{
		DisplayMessage(__FUNCTION__);
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}

	return m_nLastError;
}

BOOL CInPickArm::SetPlaceStatus()
{
	BOOL bStatus = TRUE;

	switch (PPAction)
	{
	case GLASS1:
		if (
			//INPICKARM_GLASS_Status[GLASS1] == ST_READY_1 || 
			INPICKARM_GLASS_Status[GLASS1] == ST_READY
		   )
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_READY1_EMPTY2;
			}
		}
		else
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_ERR1_EMPTY2;
			}
		}

		break;

	case GLASS2:
		if (
			//INPICKARM_GLASS_Status[GLASS2] == ST_READY_1 ||
			INPICKARM_GLASS_Status[GLASS2] == ST_READY	
		   )
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_EMPTY1_READY2;
			}
		}
		else 
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED) 
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_EMPTY1_ERR2;
			}
		}
		break;

	case MAX_NUM_OF_GLASS:
		if (
			(INPICKARM_GLASS_Status[GLASS1] == ST_READY/* || INPICKARM_GLASS_Status[GLASS1] == ST_READY_2*/) && 
			(INPICKARM_GLASS_Status[GLASS2] == ST_READY/* || INPICKARM_GLASS_Status[GLASS2] == ST_READY_2*/)
		   )
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_READY1_READY2;
			}
		}
		else if (
				 (INPICKARM_GLASS_Status[GLASS1] == ST_READY /*|| INPICKARM_GLASS_Status[GLASS1] == ST_READY_2*/) && 
				 (INPICKARM_GLASS_Status[GLASS2] == ST_ERROR)
				)
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_READY1_ERR2;
			}
		}
		else if (
				 (INPICKARM_GLASS_Status[GLASS1] == ST_ERROR) && 
				 (INPICKARM_GLASS_Status[GLASS2] == ST_READY /*|| INPICKARM_GLASS_Status[GLASS2] == ST_READY_2*/)
				)
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_ERR1_READY2;
			}
		}
		else
		{
			if (
				(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
			   )
			{
				INSPWH_Status[m_lCurInspWH] = ST_PLACE_ERR1_ERR2;
			}
		}
		break;

	default:
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		DisplayMessage(__FUNCTION__);
		bStatus = FALSE;
		if (
			(INSPWH_Status[m_lCurInspWH] != ST_MOD_NOT_SELECTED)
		   )
		{
			INSPWH_Status[m_lCurInspWH] = ST_PLACE_ERR1_ERR2;
		}
	}

	return bStatus;
}

VOID CInPickArm::UpdatePlaceToWHGlassStatus()
{
	//ACFTA Glass1 and Glass2's Glass Status transfer to PostACFWH Glass Status
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurWH;

	if (m_lCurInspWH == WH_1)
	{
		pCurWH = pCWH1;
	}
	else
	{
		pCurWH = pCWH2;
	}

	INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = INPICKARM_GLASS_Status[GLASS1];	//transfer
	INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = INPICKARM_GLASS_Status[GLASS2];

	//INPICKARM_GLASS_Status[GLASS1] = ST_EMPTY;							//Reset
	//INPICKARM_GLASS_Status[GLASS2] = ST_EMPTY;

	m_bGlass1Exist = m_bGlass2Exist = FALSE;

	if (INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] == ST_EMPTY)
	{
		pCurWH->m_bGlass1Exist = FALSE;
	}
	else
	{
		pCurWH->m_bGlass1Exist = TRUE;
	}

	if (INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] == ST_EMPTY)
	{
		pCurWH->m_bGlass2Exist = FALSE;
	}
	else
	{
		pCurWH->m_bGlass2Exist = TRUE;
	}
}

BOOL CInPickArm::CheckPlaceStatus()
{

	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurWH;

	if (m_lCurInspWH == WH_1)
	{
		pCurWH = pCWH1;
	}
	else
	{
		pCurWH = pCWH2;
	}

	BOOL bStatus1 = FALSE;
	BOOL bStatus2 = FALSE;

	switch (PPAction)
	{
	case GLASS1:
		if(pCurWH->IsVacSensorOn(GLASS1))
		{
			INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = INPICKARM_GLASS_Status[GLASS1];
			bStatus1 = TRUE;
		}
		else
		{
			INPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = ST_VAC_ERROR;
		}
		bStatus2 = TRUE;
		break;

	case GLASS2:
		if(pCurWH->IsVacSensorOn(GLASS2))
		{
			INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = INPICKARM_GLASS_Status[GLASS2];
			bStatus2 = TRUE;
		}
		else
		{
			INPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = ST_VAC_ERROR;
		}
		bStatus1 = TRUE;
		break;
	
	case MAX_NUM_OF_GLASS:

		if(pCurWH->IsVacSensorOn(GLASS1))
		{
			INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = INPICKARM_GLASS_Status[GLASS1];
			bStatus1 = TRUE;
		}
		else
		{
			INPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = ST_VAC_ERROR;
		}

		if(pCurWH->IsVacSensorOn(GLASS2))
		{
			INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = INPICKARM_GLASS_Status[GLASS2];
			bStatus2 = TRUE;
		}
		else
		{
			INPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = ST_VAC_ERROR;
		}
		break;

	default:
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		DisplayMessage(__FUNCTION__);
	}

	return (bStatus1 && bStatus2);
}

//BOOL CInPickArm::CheckReplaceStatus()
//{
//	if (INPICKARM_GLASS_Status[GLASS1] == ST_READY || INPICKARM_GLASS_Status[GLASS1] == ST_ERROR)
//	{
//		m_bGlass1Exist = TRUE;
//	}
//	else
//	{
//		m_bGlass1Exist = FALSE;
//	}
//
//	if (INPICKARM_GLASS_Status[GLASS2] == ST_READY || INPICKARM_GLASS_Status[GLASS2] == ST_ERROR)
//	{
//		m_bGlass2Exist = TRUE;
//	}
//	else
//	{
//		m_bGlass2Exist = FALSE;
//	}
//	//if (
//	//	INSPWH_Status[m_lCurInspWH] == ST_VAC_ERROR
//	//   )
//	//{
//	//	return TRUE;
//	//}
//
//	return FALSE;
//}

BOOL CInPickArm::ResetPlaceStatus()
{
	switch (PPAction)
	{
	case GLASS1:
		INPICKARM_GLASS_Status[GLASS1] = ST_EMPTY;
		m_bGlass1Exist = FALSE;
		break;

	case GLASS2:
		INPICKARM_GLASS_Status[GLASS2] = ST_EMPTY;
		m_bGlass2Exist = FALSE;
		break;

	case MAX_NUM_OF_GLASS:
		INPICKARM_GLASS_Status[GLASS1] = ST_EMPTY;
		INPICKARM_GLASS_Status[GLASS2] = ST_EMPTY;
		m_bGlass1Exist = FALSE;
		m_bGlass2Exist = FALSE;
		break;

	default:
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		DisplayMessage(__FUNCTION__);
		return FALSE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CInPickArm::PreStartOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	m_bModError = FALSE;
	m_bGlass1Exist = FALSE;
	m_bGlass2Exist = FALSE;
	m_bIsVacError = FALSE; //20121224 vac error

	if (!m_bModSelected)
	{	
		INPICKARM_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = INPICKARM_STOP_Q;
		m_bStopCycle = FALSE;
	}
	else
	{
		if (
			((m_nLastError = SetWeakBlowSol(OFF)) != GMP_SUCCESS) || 
			((m_nLastError = SetG2WeakBlowSol(OFF)) != GMP_SUCCESS) || 
			((m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT)) != GMP_SUCCESS) || 
			((m_nLastError = MoveX(INPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) != GMP_SUCCESS)
			)
		{
			Command(glSTOP_COMMAND, TRUE);
			INPICKARM_Status = ST_STOP;
			m_qSubOperation = INPICKARM_STOP_Q;
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
			INPICKARM_Status = ST_IDLE;
			HS_ClearAllHandShakeSignal();

			SetHandShakeBit(&m_stDownStreamInLineSol, ON);		//Inline Ready
			SetHandShakeBit(&m_stDownStreamReadySol, ON);		// machine Ready
			SetHandShakeBit(&m_stDownStreamInterLockFrontSol, ON);	// 20160615 Indicate no VAC error		
			SetHandShakeBit(&m_stDownStreamArmLVacErrorSol, OFF);		
			SetHandShakeBit(&m_stDownStreamArmRVacErrorSol, OFF);		
			SetHandShakeBit(&m_stDownStreamCancelSol, OFF);	// 20160615 Indicate no VAC error		

			m_qSubOperation = INPICKARM_IDLE_Q;
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
	UpdateDebugSeq(GetSequenceString(), INPICKARM_Status, m_bGlass1Exist, m_bGlass2Exist);

	CPI9000Stn::PreStartOperation();
}

VOID CInPickArm::AutoOperation()
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
		(m_bModSelected && m_qSubOperation == INPICKARM_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			if (INPICKARM_Status != ST_VAC_ERROR /*&& (HMI_bStandAlone || IsHandShakeBitOn(m_stInterLockFront))*/)	// 20160615
			{
				MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
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

		
		INPICKARM_Status = ST_STOP;
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;
		SetHandShakeBit(&m_stDownStreamReadySol, OFF);		// machine NOT Ready

		CPI9000Stn::StopOperation();
	}
}

/////////////////////////////////////////////////////////////////
//Operational Sequences
/////////////////////////////////////////////////////////////////
CString CInPickArm::GetSequenceString()
{
	
	switch (m_qSubOperation)
	{
	case INPICKARM_IDLE_Q:
		return "INPICKARM_IDLE_Q";
		
	case INPICKARM_WAIT_WH_UNLOAD_Q:
		return "INPICKARM_WAIT_WH_UNLOAD_Q";

	case INPICKARM_GO_TO_PICK_LEVEL_Q:
		return "INPICKARM_GO_TO_PICK_LEVEL_Q";

	case INPICKARM_WAIT_WH_VAC_OFF_OK_Q:
		return "INPICKARM_WAIT_WH_VAC_OFF_OK_Q";

	case INPICKARM_CHECK_INPA_PANEL_EXIST_Q:
		return "INPICKARM_CHECK_INPA_PANEL_EXIST_Q";

	case INPICKARM_IS_OUTPA_PICKING_Q:
		return "INPICKARM_IS_OUTPA_PICKING_Q";

	case INPICKARM_WAIT_TA_MGR_REQ_Q:
		return "INPICKARM_WAIT_TA_MGR_REQ_Q";

	case INPICKARM_PLACE_GLASS_Q:
		return "INPICKARM_PLACE_GLASS_Q";

	case INPICKARM_CHECK_PLACE_GLASS_Q:
		return "INPICKARM_CHECK_PLACE_GLASS_Q";

	case INPICKARM_CHECK_G2_EXIST_N_READY_Q:
		return "INPICKARM_CHECK_G2_EXIST_N_READY_Q";
		
	case INPICKARM_GRAB_N_SEARCH_G2_PR2_Q:
		return "INPICKARM_GRAB_N_SEARCH_G2_PR2_Q";
		
	case INPICKARM_GRAB_N_SEARCH_G1_PR2_Q:
		return "INPICKARM_GRAB_N_SEARCH_G1_PR2_Q";

	case INPICKARM_GRAB_N_SEARCH_G2_PR1_Q:
		return "INPICKARM_GRAB_N_SEARCH_G2_PR1_Q";

	case INPICKARM_GRAB_N_SEARCH_G1_PR1_Q:
		return "INPICKARM_GRAB_N_SEARCH_G1_PR1_Q";

	case INPICKARM_PLACEMENT_PR_DONE_Q:
		return "INPICKARM_PLACEMENT_PR_DONE_Q";

	case INPICKARM_CHECK_VAC_ERROR_GLASS_ON_WH_Q:
		return "INPICKARM_CHECK_VAC_ERROR_GLASS_ON_WH_Q";

	case INPICKARM_STOP_Q:
		return "INPICKARM_STOP_Q";
		
	}

	return "Hello World!!";
}

VOID CInPickArm::OperationSequence()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	m_nLastError = GMP_SUCCESS;
	PRINT_DATA	stPrintData;

	CInspOpt *pCInspOpt	= (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurWH;
	if(m_lCurInspWH == WH_1)
	{
		pCurWH = pCWH1;
	}
	else
	{
		pCurWH = pCWH2;
	}

	CString szMsg = "";

	switch (m_qSubOperation)
	{
		case INPICKARM_IDLE_Q:		//0

			if (m_bStopCycle)
			{
				m_qSubOperation = INPICKARM_STOP_Q;	// go to 99
			}
			else if (!IsGLASS1_VacSensorOn() && !IsGLASS2_VacSensorOn())   //no Glass 
			{
				m_bGlass1Exist = FALSE;
				m_bGlass2Exist = FALSE;

				//if (		
				//	((m_nLastError = MoveX(INPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS)
				//	)
				{
					INPICKARM_Status = ST_ACK;
					m_qSubOperation = INPICKARM_WAIT_WH_UNLOAD_Q;		// go to 1
				}
			}
			else  // Glass Exist
			{
				if (IsGLASS1_VacSensorOn())
				{
					INPICKARM_GLASS_Status[GLASS1] = ST_READY;
					m_bGlass1Exist = TRUE;
				}

				if (IsGLASS2_VacSensorOn())
				{
					INPICKARM_GLASS_Status[GLASS2] = ST_READY;
					m_bGlass2Exist = TRUE;
				}

				INPICKARM_Status = ST_BUSY;
				m_qSubOperation = INPICKARM_IS_OUTPA_PICKING_Q;		// go to 5

			}

			break;
			
		case INPICKARM_WAIT_WH_UNLOAD_Q:		//1

			if (m_bStopCycle)
			{
				m_qSubOperation = INPICKARM_STOP_Q;	// go to 99
			}
			else if (HS_IsWHSendReady() || (IsBurnInDiagBond() && HMI_bStandAlone))  
			{
				HS_InPickArmSendReadyACK();
				m_qSubOperation = INPICKARM_GO_TO_PICK_LEVEL_Q;     //2
			}


			break;

		case INPICKARM_GO_TO_PICK_LEVEL_Q:		//2

			if (
				((m_nLastError = m_stMotorX.Sync()) == GMP_SUCCESS) &&
				((m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS) //&&
				//((m_nLastError = m_stMotorZ.Sync()) == GMP_SUCCESS)
				)
			{
				if (!IsBurnInDiagBond())
				{
					InPickArmVacOn();
				}
				SetHandShakeBit(&m_stDownStreamTAVacOnOkSol, ON);
				HS_RequestWHVacOFF();
				m_qSubOperation = INPICKARM_WAIT_WH_VAC_OFF_OK_Q;		// go to 3
			}

			break;

		case INPICKARM_WAIT_WH_VAC_OFF_OK_Q:		//3

			if (HS_IsHandshakeCancel())
			{
				HS_ClearAllHandShakeSignal();
				if (!IsBurnInDiagBond())
				{
					InPickArmVacOff();
				}
				if (
					((m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT)) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = INPICKARM_WAIT_WH_UNLOAD_Q;			//1
				}
			}
			else if (HS_IsWHVacOff() || (IsBurnInDiagBond() && HMI_bStandAlone))
			{
				if ((IsBurnInDiagBond() && HMI_bStandAlone))
				{
					Sleep (1000);	// 20160624 from 200 to 2000
				}
				if (		
					((m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT)) == GMP_SUCCESS) //&&
					//((m_nLastError = m_stMotorZ.Sync()) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = INPICKARM_CHECK_INPA_PANEL_EXIST_Q;		// go to 4
				}
			}
			break;

		case INPICKARM_CHECK_INPA_PANEL_EXIST_Q:		//4

			if (IsInPickArmGlassExist() || (IsBurnInDiagBond() && HMI_bStandAlone))
			{
				//if ((m_nLastError = SetSHFPWHExtendSol(ON, GMP_WAIT))  == GMP_SUCCESS)	// 20160624
				{
					HS_SendInPickArmReceivedPanel();
					UpdateInPickArmStatus();
					m_lPickPlaceRetryCounter = 0;
					INPICKARM_Status = ST_BUSY;
					m_qSubOperation = INPICKARM_IS_OUTPA_PICKING_Q;     //5
				}
			}
			else
			{
				//retry picking sequence
				if (m_lPickPlaceRetryLimit != 0)
				{
					m_lPickPlaceRetryCounter++;

					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
					{
						HS_SendInPickArmReceivedPanel();
						m_lPickPlaceRetryCounter = 0;
						INPICKARM_Status = ST_VAC_ERROR;
						m_qSubOperation = INPICKARM_STOP_Q;	// go to 99
						DisplayMessage("INPICKARM ERROR: Pick Glass Retry Limit Hit");
						m_bModError = TRUE;
						SetError(IDS_INPICKARM_PICK_GLASS1_ERR + PPAction);
						m_bIsVacError = TRUE; //20121224 vac error
					}
					else
					{
						m_qSubOperation = INPICKARM_GO_TO_PICK_LEVEL_Q;	// go to 2
					}
				}
				else
				{
					m_qSubOperation = INPICKARM_GO_TO_PICK_LEVEL_Q;	// go to 2
				}
			}

			break;

		case INPICKARM_IS_OUTPA_PICKING_Q:		//5

			//if (OUTPICKARM_ARM_Posn > MAINBOND_1 && OUTPICKARM_ARM_Posn < MAX_NUM_OF_MAINBOND)
			//{
			//	INPICKARM_ARM_Posn = (MainbondType)(OUTPICKARM_ARM_Posn - 1);

			//	if ((m_nLastError = MoveXToPlacePosn(INPICKARM_ARM_Posn, GMP_WAIT)) == GMP_SUCCESS)
			//	{
			//		m_qSubOperation = INPICKARM_WAIT_TA_MGR_REQ_Q;		// go to 6
			//	}
			//}
			//else
			//{
				m_qSubOperation = INPICKARM_WAIT_TA_MGR_REQ_Q;		// go to 6
			//}

			break;

		case INPICKARM_WAIT_TA_MGR_REQ_Q:		//6

			if (TA_MANAGER_Status == ST_STOP)
			{
				m_qSubOperation = INPICKARM_STOP_Q;		// go to 99
			}
			else if (TA_MANAGER_Status == ST_MB1_REQ_LOAD)
			{
				m_lCurInspWH = WH_1;
				//pCurWH = pCWH1;

				INPICKARM_Status = ST_MB1_ACK_LOAD;

				if ((m_nLastError = MoveXToPlacePosn(m_lCurInspWH, GMP_WAIT)) == GMP_SUCCESS)
				{
					m_qSubOperation = INPICKARM_PLACE_GLASS_Q;		// go to 7
				}
			}
			else if (TA_MANAGER_Status == ST_MB2_REQ_LOAD)
			{
				m_lCurInspWH = WH_2;
			//	pCurWH = pCWH2;

				INPICKARM_Status = ST_MB2_ACK_LOAD;

				if ((m_nLastError = MoveXToPlacePosn(m_lCurInspWH, GMP_WAIT)) == GMP_SUCCESS)
				{
					m_qSubOperation = INPICKARM_PLACE_GLASS_Q;		// go to 7
				}
			}
			else if (TA_MANAGER_Status == ST_MB1_REQ_LOAD)		// Suppose MB1 is waiting for Glass, however, the m_lCurInspWH is MB2. it lets TAManaer check another workholder
			{
				INPICKARM_Status = ST_MB1_ACK_LOAD;
			}
			else if (TA_MANAGER_Status == ST_MB2_REQ_LOAD) 
			{
				INPICKARM_Status = ST_MB2_ACK_LOAD;
			}

			break;

		case INPICKARM_PLACE_GLASS_Q:		//7

			if (m_bGlass1Exist && m_bGlass2Exist)
			{
				PPAction = MAX_NUM_OF_GLASS;
			}
			else if (m_bGlass1Exist && !m_bGlass2Exist)
			{
				PPAction = GLASS1;
			}
			else if (!m_bGlass1Exist && m_bGlass2Exist)
			{
				PPAction = GLASS2;
			}

			if (
				((m_nLastError = pCurWH->m_stMotorT.MoveAbsolute(pCurWH->m_lStandbyPosnT, GMP_NOWAIT)) == GMP_SUCCESS) &&
				((m_nLastError = pCurWH->MoveXY(INSPWH_XY_CTRL_GO_LOAD_OFFSET, GMP_NOWAIT)) == GMP_SUCCESS) &&
				((m_nLastError = MoveXToPlacePosn(m_lCurInspWH, GMP_WAIT)) == GMP_SUCCESS) &&
				((m_nLastError = pCurWH->m_stMotorY.Sync()) == GMP_SUCCESS) &&	
				((m_nLastError = pCurWH->m_stMotorT.Sync()) == GMP_SUCCESS) &&
				((m_nLastError = PlaceGlass(PPAction, m_lCurInspWH)) == GMP_SUCCESS)
				)
			{
				//szMsg.Format("INPICKARM Place Glass WH%ld", m_lCurInspWH);
				//DisplayMessage(szMsg);

				//SetPlaceStatus();  //Ask WH to Check Glass

				m_qSubOperation = INPICKARM_CHECK_PLACE_GLASS_Q;	// go to 8
			}
		
			
			break;

		case INPICKARM_CHECK_PLACE_GLASS_Q:		//8

			if (AutoMode == BURN_IN || AutoMode == DIAG_BOND || CheckPlaceStatus())
			{
				//if ((INSPWH_Status[m_lCurInspWH][GLASS1] == ST_PR_IN_PROCESS) && (INSPWH_Status[m_lCurInspWH][GLASS2] == ST_PR_IN_PROCESS))
				//{
					//pCInspWH->m_lCurMBType = m_lCurInspWH;
				pCurWH->IsTA5PlaceMBUnitReq();
					//if (
					//	INSPWH_GLASS_Status[m_lCurMainBond % MAX_NUM_OF_MAINBOND][GLASS2] != ST_EMPTY &&
					//	INSPWH_GLASS_Status[m_lCurMainBond % MAX_NUM_OF_MAINBOND][GLASS2] != ST_ERROR //20150728
					//	)
					//{
					//	/*g_lCurTA4Glass = */m_lCurGlass = GLASS2;
					//}
					//else if (
					//	INSPWH_GLASS_Status[m_lCurMainBond % MAX_NUM_OF_MAINBOND][GLASS1] != ST_EMPTY &&
					//	INSPWH_GLASS_Status[m_lCurMainBond % MAX_NUM_OF_MAINBOND][GLASS1] != ST_ERROR //20150728
					//	)
					//{
					//	/*g_lCurTA4Glass = */m_lCurGlass = GLASS1;
					//}
					//else
					//{
					//	//both glass empty
					//	DisplayMessage("TA4: no glass to do PR");
					//	/*g_lCurTA4Glass = */m_lCurGlass = GLASS2;
					//}
					//m_qSubOperation = INPICKARM_GRAB_PR2_Q;	// go to 9
				//}
				//else
				{
					m_lPickPlaceRetryCounter = 0;
					UpdatePlaceToWHGlassStatus();
					INPICKARM_Status = ST_BUSY;

					//if (
					//	((m_nLastError = MoveX(TA4_X_CTRL_GO_PICK_GLASS_POSN, GMP_NOWAIT)) == GMP_SUCCESS) &&
					//	((m_nLastError = MoveZ(TA4_X_CTRL_GO_PICK_GLASS_POSN, GMP_NOWAIT)) == GMP_SUCCESS)
					//)
					//{
					//	if (m_bStopCycle)
					//	{
					//		m_qSubOperation = TA4_STOP_Q;
					//	}
					//	else
					//	{
					//		INPICKARM_ARM_Posn = MAINBOND_NONE;
					//		m_qSubOperation = TA4_CHECK_TA_MANGER_AT_IDLE_Q;	// go to 10
					//	}
					//}

					m_qSubOperation = INPICKARM_CHECK_G2_EXIST_N_READY_Q;	// go to 9
				}
			}
			else /*if (CheckReplaceStatus())*/
			{
				//retry placing sequence
				if (m_lPickPlaceRetryLimit != 0)
				{
					if (m_lPickPlaceRetryCounter >= m_lPickPlaceRetryLimit)
					{
						// VAC ERROR Glass Placed!
						INPICKARM_Status = ST_BUSY;
						m_lPickPlaceRetryCounter = 0;
						m_qSubOperation = INPICKARM_CHECK_G2_EXIST_N_READY_Q;	// go to 9
				
						DisplayMessage("INPICKARM ERROR: Place Glass Retry Limit Hit");
						m_bModError = TRUE;
						SetError(IDS_INPICKARM_PLACE_GLASS1_ERR + PPAction);
						m_bIsVacError = TRUE; //20121224 vac error
					}
					else
					{
						//CheckReplaceStatus();
						m_lPickPlaceRetryCounter++;
						m_qSubOperation = INPICKARM_PLACE_GLASS_Q;	// go to 7
					}
				}
				else
				{
	#if 1 //20130117 problem of press "STOP" when a glass is just placed on WH. "START" again will treat the glass as error glass.
					if (m_bStopCycle)
					{
						m_qSubOperation = INPICKARM_STOP_Q; 
					}	
					else
					{
						m_qSubOperation = INPICKARM_PLACE_GLASS_Q;    // go to 7
					}	
	#else
					m_qSubOperation = INPICKARM_PLACE_GLASS_Q;	// go to 7
	#endif
				}
			}
			break;

			
		case INPICKARM_CHECK_G2_EXIST_N_READY_Q:		//9
			
			if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] == ST_READY)
			{
				pCurWH->m_lCurGlass = GLASS2;
				if (
					(m_nLastError = MoveXYToCurPR2Posn(m_lCurInspWH, GLASS2, GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncX()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncY()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncZ()) == GMP_SUCCESS
					)
				{
					m_qSubOperation = INPICKARM_GRAB_N_SEARCH_G2_PR2_Q;		// go to 10
				}
			}
			else if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] == ST_READY)
			{
				pCurWH->m_lCurGlass = GLASS1;
				if (
					(m_nLastError = MoveXYToCurPR2Posn(m_lCurInspWH, GLASS1, GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncX()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncY()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncZ()) == GMP_SUCCESS
					)
				{
					m_qSubOperation = INPICKARM_GRAB_N_SEARCH_G1_PR2_Q;		// go to 11
				}
			}
			else
			{
				{
					m_qSubOperation = INPICKARM_PLACEMENT_PR_DONE_Q;		// go to 11
				}
			}


			break;
			
		case INPICKARM_GRAB_N_SEARCH_G2_PR2_Q:		//10
			
			if(m_lCurInspWH == WH_1)
			{
				pruDLPR1[GLASS1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
				pruDLPR2[GLASS1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
				pruDLPR1[GLASS2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
				pruDLPR2[GLASS2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
			}
			else
			{
				pruDLPR1[GLASS1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
				pruDLPR2[GLASS1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
				pruDLPR1[GLASS2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
				pruDLPR2[GLASS2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
			}
			Sleep(pCurWH->m_lPRDelay);
			if (pCurWH->AutoSearchPR2GrabOnly(&pruDLPR2[GLASS2]))
			{
				if (pCurWH->AutoSearchPR2ProcessOnly(&pruDLPR2[GLASS2]))
				{
					pCurWH->m_dmtrPR2Posn[GLASS2] = pCurWH->m_dmtrCurrPR2Posn;
					//m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[GLASS2].x + m_dmtrPR2Offset[GLASS2].x;	
					//m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[GLASS2].y + m_dmtrPR2Offset[GLASS2].y;
				}
				else
				{					
					INPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = ST_ERROR;
					m_lDLPR2_G2_ErrorCnt++;
					pAppMod->m_ulDLG2PRError++;
					pAppMod->m_ulTotalDLPRError++;

					DisplayMessage("*********************************************");
					{
						szMsg.Format("INPICKARM_GRAB_N_SEARCH_G2_PR2_Q: DL Glass2 PR2 Process Error.");
						pCurWH->m_szRejectReason[GLASS2] = _T("DL Glass2 PR2 Process Error.") ;
					}
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
					INPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = ST_ERROR;
					m_lDLPR2_G2_ErrorCnt++;
					pAppMod->m_ulDLG2PRError++;
					pAppMod->m_ulTotalDLPRError++;

				DisplayMessage("*********************************************");
				{
					szMsg.Format("INPICKARM_GRAB_N_SEARCH_G2_PR2_Q: DL Glass2 PR2 Grab Error.");
					pCurWH->m_szRejectReason[GLASS2] = _T("DL Glass2 PR2 Grab Error.") ;
				}
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
			}

			if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] != ST_READY)
			{
				if ((m_nLastError = MoveXYToCurPR1Posn(m_lCurInspWH, GLASS2, GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncX()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncY()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncZ()) == GMP_SUCCESS)
				{
					m_qSubOperation = INPICKARM_GRAB_N_SEARCH_G2_PR1_Q;		// go to 12
				}
			}
			else
			{
				pCurWH->m_lCurGlass = GLASS1;
				if (
					(m_nLastError = MoveXYToCurPR2Posn(m_lCurInspWH, GLASS1, GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncX()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncY()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncZ()) == GMP_SUCCESS
					)
				{
					m_qSubOperation = INPICKARM_GRAB_N_SEARCH_G1_PR2_Q;		// go to 11
				}
			}
			break;

		case INPICKARM_GRAB_N_SEARCH_G1_PR2_Q:		//11

			Sleep(pCurWH->m_lPRDelay2);
			if (pCurWH->AutoSearchPR2GrabOnly(&pruDLPR2[GLASS1]))
			{
				if (pCurWH->AutoSearchPR2ProcessOnly(&pruDLPR2[GLASS1]))
				{
					pCurWH->m_dmtrPR2Posn[GLASS1] = pCurWH->m_dmtrCurrPR2Posn;
					//m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[GLASS2].x + m_dmtrPR2Offset[GLASS2].x;	
					//m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[GLASS2].y + m_dmtrPR2Offset[GLASS2].y;
				}
				else
				{					
					INPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = ST_ERROR;
					m_lDLPR2_G1_ErrorCnt++;
					pAppMod->m_ulDLG1PRError++;
					pAppMod->m_ulTotalDLPRError++;

					DisplayMessage("*********************************************");
					{
						szMsg.Format("INPICKARM_GRAB_N_SEARCH_G1_PR2_Q: DL Glass1 PR2 Process Error.");
						pCurWH->m_szRejectReason[GLASS1] = _T("DL Glass1 PR2 Process Error.") ;
					}
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
					INPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = ST_ERROR;
					m_lDLPR2_G1_ErrorCnt++;
					pAppMod->m_ulDLG1PRError++;
					pAppMod->m_ulTotalDLPRError++;

				DisplayMessage("*********************************************");
				{
					szMsg.Format("INPICKARM_GRAB_N_SEARCH_G1_PR2_Q: DL Glass1 PR2 Grab Error.");
					pCurWH->m_szRejectReason[GLASS1] = _T("DL Glass1 PR2 Grab Error.") ;
				}
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
			}

			if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] != ST_READY)
			{
				pCurWH->m_lCurGlass = GLASS1;
				if ((m_nLastError = MoveXYToCurPR1Posn(m_lCurInspWH, GLASS1, GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncX()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncY()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncZ()) == GMP_SUCCESS)
				{
					m_qSubOperation = INPICKARM_GRAB_N_SEARCH_G1_PR1_Q;		// go to 13
				}
			}
			else
			{
				pCurWH->m_lCurGlass = GLASS2;
				if ((m_nLastError = MoveXYToCurPR1Posn(m_lCurInspWH, GLASS2, GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncX()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncY()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncZ()) == GMP_SUCCESS)
				{
					m_qSubOperation = INPICKARM_GRAB_N_SEARCH_G2_PR1_Q;		// go to 12
				}
			}
			break;
			
		case INPICKARM_GRAB_N_SEARCH_G2_PR1_Q:		//12

			Sleep(pCurWH->m_lPRDelay);
			if (pCurWH->AutoSearchPR1GrabOnly(&pruDLPR1[GLASS2]))
			{
				if (pCurWH->AutoSearchPR1ProcessOnly(&pruDLPR1[GLASS2]))
				{
					pCurWH->m_dmtrPR1Posn[GLASS2] = pCurWH->m_dmtrCurrPR1Posn;
					//m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[GLASS2].x + m_dmtrPR2Offset[GLASS2].x;	
					//m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[GLASS2].y + m_dmtrPR2Offset[GLASS2].y;
				}
				else
				{					
					INPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = ST_ERROR;
					m_lDLPR1_G2_ErrorCnt++;
					pAppMod->m_ulDLG2PRError++;
					pAppMod->m_ulTotalDLPRError++;

					DisplayMessage("*********************************************");
					{
						szMsg.Format("INPICKARM_GRAB_N_SEARCH_G2_PR1_Q: DL Glass2 PR1 Process Error.");
						pCurWH->m_szRejectReason[GLASS2] = _T("DL Glass2 PR1 Process Error.") ;
					}
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
					INPICKARM_GLASS_Status[GLASS2] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = ST_ERROR;
					m_lDLPR1_G2_ErrorCnt++;
					pAppMod->m_ulDLG2PRError++;
					pAppMod->m_ulTotalDLPRError++;

				DisplayMessage("*********************************************");
				{
					szMsg.Format("INPICKARM_GRAB_N_SEARCH_G2_PR1_Q: DL Glass2 PR1 Grab Error.");
					pCurWH->m_szRejectReason[GLASS2] = _T("DL Glass2 PR1 Grab Error.") ;
				}
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
			}

			if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] == ST_READY)
			{
				pCurWH->m_lCurGlass = GLASS1;
				if ((m_nLastError = MoveXYToCurPR1Posn(m_lCurInspWH, GLASS1, GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncX()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncY()) == GMP_SUCCESS &&
					(m_nLastError = pCurWH->SyncZ()) == GMP_SUCCESS)
				{
					m_qSubOperation = INPICKARM_GRAB_N_SEARCH_G1_PR1_Q;		// go to 13
				}
			}
			else
			{
					m_qSubOperation = INPICKARM_PLACEMENT_PR_DONE_Q;		// go to 14
			}
			break;
			
		case INPICKARM_GRAB_N_SEARCH_G1_PR1_Q:		//13

			Sleep(pCurWH->m_lPRDelay2);
			if (pCurWH->AutoSearchPR1GrabOnly(&pruDLPR1[GLASS1]))
			{
				if (pCurWH->AutoSearchPR1ProcessOnly(&pruDLPR1[GLASS1]))
				{
					pCurWH->m_dmtrPR1Posn[GLASS1] = pCurWH->m_dmtrCurrPR1Posn;
					//m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[GLASS2].x + m_dmtrPR2Offset[GLASS2].x;	
					//m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[GLASS2].y + m_dmtrPR2Offset[GLASS2].y;
				}
				else
				{					
					INPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = ST_ERROR;
					m_lDLPR1_G1_ErrorCnt++;
					pAppMod->m_ulDLG1PRError++;
					pAppMod->m_ulTotalDLPRError++;

					DisplayMessage("*********************************************");
					{
						szMsg.Format("INPICKARM_GRAB_N_SEARCH_G1_PR1_Q: DL Glass1 PR1 Process Error.");
						pCurWH->m_szRejectReason[GLASS1] = _T("DL Glass1 PR1 Process Error.") ;
					}
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
					INPICKARM_GLASS_Status[GLASS1] = INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = ST_ERROR;
					m_lDLPR1_G1_ErrorCnt++;
					pAppMod->m_ulDLG1PRError++;
					pAppMod->m_ulTotalDLPRError++;

				DisplayMessage("*********************************************");
				{
					szMsg.Format("INPICKARM_GRAB_N_SEARCH_G1_PR1_Q: DL Glass1 PR1 Grab Error.");
					pCurWH->m_szRejectReason[GLASS1] = _T("DL Glass1 PR1 Grab Error.") ;
				}
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
			}

			

			m_qSubOperation = INPICKARM_PLACEMENT_PR_DONE_Q;		// go to 14
			break;
			
		case INPICKARM_PLACEMENT_PR_DONE_Q:	//14

			if ((m_nLastError = MoveX(INPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_NOWAIT)) == GMP_SUCCESS)
			{
				if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] == ST_READY)
				{
					//pCurWH->CalcAlignPt(GLASS1);
					pCurWH->UpdateInspPRPosn(GLASS1);
					if(
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].x + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].x >= pCInspOpt->m_stMotorX.stProtect.lPosLmt || 
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].x + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].x <= pCInspOpt->m_stMotorX.stProtect.lNegLmt || 
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].y + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].y >= pCurWH->m_stMotorY.stProtect.lPosLmt || 
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].y + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS1].y <= pCurWH->m_stMotorY.stProtect.lNegLmt || 
						pCurWH->m_lTInspPosn[GLASS1] >= pCurWH->m_stMotorT.stProtect.lPosLmt || 
						pCurWH->m_lTInspPosn[GLASS1] <= pCurWH->m_stMotorT.stProtect.lNegLmt 
						)
					{
						INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] = ST_ERROR;

						DisplayMessage("*********************************************");
						{
							szMsg.Format("INPICKARM_PLACEMENT_PR_DONE_Q: G1 UL PR Posn exceeds SW Limits.");
							pCurWH->m_szRejectReason[GLASS1] = _T("G1 UL PR Posn exceeds SW Limits.") ;
						}
						DisplayMessage(szMsg);
						DisplayMessage("*********************************************");
					}
				}
				if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] == ST_READY)
				{
					pCurWH->m_dmtrCurrPR1Posn = pCurWH->m_dmtrPR1Posn[GLASS2];
					pCurWH->m_dmtrCurrPR2Posn = pCurWH->m_dmtrPR2Posn[GLASS2];
					pCurWH->UpdateInspPRPosn(GLASS2);
					if(
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].x + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].x >= pCInspOpt->m_stMotorX.stProtect.lPosLmt || 
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].x + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].x <= pCInspOpt->m_stMotorX.stProtect.lNegLmt || 
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].y + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].y >= pCurWH->m_stMotorY.stProtect.lPosLmt || 
						pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].y + pCInspOpt->m_dmtrPR1Offset[m_lCurInspWH][GLASS2].y <= pCurWH->m_stMotorY.stProtect.lNegLmt || 
						pCurWH->m_lTInspPosn[GLASS2] >= pCurWH->m_stMotorT.stProtect.lPosLmt || 
						pCurWH->m_lTInspPosn[GLASS2] <= pCurWH->m_stMotorT.stProtect.lNegLmt 
						)
					{
						INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] = ST_ERROR;

						DisplayMessage("*********************************************");
						{
							szMsg.Format("INPICKARM_PLACEMENT_PR_DONE_Q: G2 UL PR Posn exceeds SW Limits.");
							pCurWH->m_szRejectReason[GLASS2] = _T("G2 UL PR Posn exceeds SW Limits.") ;
						}
						DisplayMessage(szMsg);
						DisplayMessage("*********************************************");
					}
				}

				SetPlaceStatus();			//INSPWH Status = ST_PLACE_xxx_xxx
				ResetPlaceStatus();			//INPA Glass Status = ST_EMPTY
				
				m_qSubOperation = INPICKARM_CHECK_VAC_ERROR_GLASS_ON_WH_Q;		// go to 15
			}

	//		if (m_bStopCycle)
	//		{
	//			HS_ClearAllHandShakeSignal();
	//			m_qSubOperation = INPICKARM_STOP_Q;	// go to 99
	//		}
	//		else if (CheckPlaceStatus())
	//		{
	//			TC_ResetTime();
	//			m_dTempCycleTime[m_nCycleCounter] = (DOUBLE)(GetTickCount() - m_dwStartTime) / 1000.0;

	//			if (m_bGlass1Exist && m_bGlass2Exist)
	//			{
	//				m_dTempCycleTime[m_nCycleCounter] = m_dTempCycleTime[m_nCycleCounter] / 2;
	//			}

	//			if (
	//				m_dTempCycleTime[0] == 0.0 || 
	//				m_dTempCycleTime[1] == 0.0 || 
	//				m_dTempCycleTime[2] == 0.0 || 
	//				m_dTempCycleTime[3] == 0.0 || 
	//				m_dTempCycleTime[4] == 0.0 || 
	//				m_dTempCycleTime[5] == 0.0 || 
	//				m_dTempCycleTime[6] == 0.0 || 
	//				m_dTempCycleTime[7] == 0.0 || 
	//				m_dTempCycleTime[8] == 0.0 || 
	//				m_dTempCycleTime[9] == 0.0 
	//				)
	//			{
	//				m_dCycleTime = (m_dTempCycleTime[0] + 
	//					m_dTempCycleTime[1] + 
	//					m_dTempCycleTime[2] + 
	//					m_dTempCycleTime[3] + 
	//					m_dTempCycleTime[4] + 
	//					m_dTempCycleTime[5] + 
	//					m_dTempCycleTime[6] + 
	//					m_dTempCycleTime[7] + 
	//					m_dTempCycleTime[8] + 
	//					m_dTempCycleTime[9]) / (m_nCycleCounter + 1);
	//			}
	//			else
	//			{
	//				m_dCycleTime = (m_dTempCycleTime[0] + 
	//					m_dTempCycleTime[1] + 
	//					m_dTempCycleTime[2] + 
	//					m_dTempCycleTime[3] + 
	//					m_dTempCycleTime[4] + 
	//					m_dTempCycleTime[5] + 
	//					m_dTempCycleTime[6] + 
	//					m_dTempCycleTime[7] + 
	//					m_dTempCycleTime[8] + 
	//					m_dTempCycleTime[9]) / 10;
	//			}
	//			if (m_nCycleCounter < 9)
	//			{
	//				m_nCycleCounter++;
	//			}
	//			else
	//			{
	//				m_nCycleCounter = 0;
	//			}
	//			m_dwStartTime = GetTickCount();
	//			pAppMod->m_dUPH = (DOUBLE)(pAppMod->m_ulUnitsInpected / (DOUBLE)(GetTickCount() - m_dwMachineTime)) * 60.0 * 60.0 * 1000.0;

	//			m_lPickPlaceRetryCounter = 0;

	//			m_bGlass1Exist = FALSE;
	//			m_bGlass2Exist = FALSE;
	//			INPICKARM_Status = ST_EMPTY;
	//			m_qSubOperation = INPICKARM_IDLE_Q;		// go to 0

	//		}
	//		else
	//		{
	//			INPICKARM_Status = ST_VAC_ERROR;
	//			m_bModError = TRUE;
	//			DisplayMessage("*********************************************");
	//			SetError(IDS_INPICKARM_PLACE_OUT_ERR);
	//			szMsg.Format("INPICKARM_CHECK_PLACE_GLASS_Q: INPICKARM PLACE OUT ERROR");
	//			DisplayMessage(szMsg);
	//			DisplayMessage("*********************************************");

	//			m_qSubOperation = INPICKARM_STOP_Q;
	//		}

			break;


		case INPICKARM_CHECK_VAC_ERROR_GLASS_ON_WH_Q:		//15
			
			if(
				INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] == ST_VAC_ERROR ||
				INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] == ST_VAC_ERROR 
				)
			{
				if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] == ST_VAC_ERROR)
				{
					if( m_lCurInspWH == WH_1 )
					{
						SetError(IDS_INSPWH1_GLASS1_VAC_ERR);
					}
					else
					{
						SetError(IDS_INSPWH2_GLASS1_VAC_ERR);
					}
				}

				if(INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] == ST_VAC_ERROR)
				{
					if( m_lCurInspWH == WH_1 )
					{
						SetError(IDS_INSPWH1_GLASS2_VAC_ERR);
					}
					else
					{
						SetError(IDS_INSPWH2_GLASS2_VAC_ERR);
					}
				}

				m_qSubOperation = INPICKARM_STOP_Q;			//99
			}
			else
			{
				m_qSubOperation = INPICKARM_IDLE_Q;			//0
			}

			break;

		case INPICKARM_STOP_Q:		//99
			break;
		
	}


	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			if (INPICKARM_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("INPICKARM Go State: %ld", m_qSubOperation);
			}
			else
			{
				szMsg.Format("INPICKARM Go Stop");
			}
			//DisplayMessage(szMsg);
		}
		UpdateDebugSeq(GetSequenceString(), INPICKARM_Status, m_bGlass1Exist, m_bGlass2Exist);
		m_qPreviousSubOperation = m_qSubOperation;

		if (HMI_bDebugSeq)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				stPrintData.szTitle[i]	= "";
			}

			stPrintData.szTitle[0].Format("%ld", GetTickCount());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", INPICKARM_Status);

			LogFile->log(stPrintData.szTitle);
		}

	}

}

/////////////////////////////////////////////////////////////////////////////////////////

INT CInPickArm::MoveXYToCurPR1Posn(LONG lCurWH, GlassNum CurGlass, BOOL bWait)
{
	INT		nResult		= GMP_SUCCESS;

	//addcode
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurWH;

	if (lCurWH == WH_1)
	{
		pCurWH = pCWH1;
	}
	else
	{
		pCurWH = pCWH2;
	}

	pCurWH->m_lCurGlass = (LONG)CurGlass;

	if(pCurWH->MoveXYToCurrPR1Posn(bWait) != GMP_SUCCESS)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInPickArm::MoveXYToCurPR2Posn(LONG lCurWH, GlassNum CurGlass, BOOL bWait)
{
	INT		nResult		= GMP_SUCCESS;

	//addcode
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurWH;

	if (lCurWH == WH_1)
	{
		pCurWH = pCWH1;
	}
	else
	{
		pCurWH = pCWH2;
	}

	pCurWH->m_lCurGlass = (LONG)CurGlass;

	if(pCurWH->MoveXYToCurrPR2Posn(bWait) != GMP_SUCCESS)
	{
		nResult = GMP_FAIL;
	}
	return nResult;
}

INT CInPickArm::KeepQuiet()
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

VOID CInPickArm::CycleOperation()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	if (!pCInspOpt->m_bSearchPlacementCycle)
	{
		State(STOPPING_Q);

		return;
	}

	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND)
	{
		pCInspOpt->m_bSearchPlacementCycle = FALSE;
	}

	PRINT_DATA	stPrintData;
	PRINT_DATA	szPrintTitle;
	CString		szMsg;

	if (pCInspOpt->m_bSearchPlacementCycle /*&& pCInspOpt->m_bMoveINPA*/)
	{
		pCInspOpt->m_bSearchPlacementCycle = CycleTest();
	}

	if (!pCInspOpt->m_bSearchPlacementCycle)
	{
		State(STOPPING_Q);
	}
}

BOOL CInPickArm::CycleTest()
{
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CString szMsg = "";
	if(pCInspOpt->m_bMoveINPA)
	{
		if (!m_bModSelected)
		{
			szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
			HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
			return FALSE;
		}

		if (!pCOutPickArm->m_bModSelected)
		{
			szMsg.Format("%s Module Not Selected. Operation Abort!", pCOutPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
			return FALSE;
		}

		if (
			(MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN,GMP_NOWAIT)) != GMP_SUCCESS ||
			(pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_POSN,GMP_WAIT)) != GMP_SUCCESS ||
			m_stMotorX.Sync() != GMP_SUCCESS
		)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Pick Posn Failed!");
			return FALSE;
		}

		//if (
		//	(pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_POSN,GMP_WAIT)) != GMP_SUCCESS ||
		//	m_stMotorX.Sync() != GMP_SUCCESS
		//)
		//{
		//	HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Pick Posn Failed!");
		//	return FALSE;
		//}

		Sleep(500);

		if (
			(pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN,GMP_NOWAIT)) != GMP_SUCCESS ||
			(MoveX(INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_POSN,GMP_WAIT)) != GMP_SUCCESS ||
			pCOutPickArm->m_stMotorX.Sync() != GMP_SUCCESS
		)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Pick Posn Failed!");
			return FALSE;
		}

		//if (
		//	(MoveX(INPICKARM_X_CTRL_GO_WH2_PLACE_GLASS_POSN,GMP_WAIT)) != GMP_SUCCESS ||
		//	pCOutPickArm->m_stMotorX.Sync() != GMP_SUCCESS
		//)
		//{
		//	HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Standby Posn Failed!");
		//	return FALSE;
		//}

		Sleep(500);

	}
	else
	{
		if (!pCWH2->m_bModSelected)
		{
			szMsg.Format("%s Module Not Selected. Operation Abort!", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
			return FALSE;
		}

		if (
			(pCWH2->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN ,GMP_WAIT)) != GMP_SUCCESS 
		)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Pick Posn Failed!");
			return FALSE;
		}

		Sleep(500);

		if (
			(pCWH2->MoveXY(INSPWH_XY_CTRL_GO_PI_POSN,GMP_WAIT)) != GMP_SUCCESS 
		)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Pick Posn Failed!");
			return FALSE;
		}

		Sleep(500);
	}

	return TRUE;
}

INT CInPickArm::PickGlass(GlassNum lGlassNum)
{
	LONG lDelay;
	if (lGlassNum < NO_GLASS || lGlassNum > MAX_NUM_OF_GLASS)
	{                
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return GMP_FAIL;
	}
	SetVacSol(lGlassNum, ON);

	if (
		(m_nLastError = MoveX(INPICKARM_X_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS &&
		(m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET, GMP_WAIT)) == GMP_SUCCESS
		)
	{
		lDelay = max(max(m_stGlass1VacSol.GetOffDelay(), m_stGlass1VacSol.GetOnDelay()), m_stCOF1VacSol.GetOnDelay());
		Sleep(lDelay);

		return m_nLastError = MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);//!IsVacSensorOn(lGlassNum); 

	}

	return GMP_FAIL;
}

LONG CInPickArm::HMI_SetStandAlone(IPC_CServiceMessage &svMsg)
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
	//	COF902_OUTPICKARM_Status = ST_GW_TA5_STOP;
	}

	return 1;
}

//BOOL CInPickArm::IsTA5PlaceMBUnitReq()
//{
//	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
//	BOOL bResult = FALSE;
//
//	//addcode, edit the GateWay String below
//	//two string come from TA5: TA5HS_MBUnitString , TA5PlaceActionString
//
//	//if (TA5HS_MBUnitString == GW_MBUnit1_REQ)		//GateWay String
//	//{
//	//	m_lCurMBHead = MAINBOND_1;	//MB12
//	//	bResult = TRUE;
//	//}
//	//else if (TA5HandShakeString == GW_MBUnit2_REQ)
//	//{
//	//	m_lCurMBHead = MAINBOND_2;	//MB34
//	//	bResult = TRUE;
//	//}
//	if (
//		COF902_OUTPICKARM_Status != ST_GW_TA5_STOP &&
//		COF902_OUTPICKARM_Status != ST_GW_TA5_CHECK_GLASS_EXIST_REQ
//		)
//	{
//		CString szMsg("");
//		szMsg.Format("Target Glass Status = %ld", (LONG)COF902_OUTPICKARM_Status);
//		DisplayMessage(szMsg);
//	}
//
//	if (HMI_bStandAlone)
//	{
//		COF902_OUTPICKARM_Status = ST_GW_TA5_PLACE_MB12_G1READY_G2READY_REQ;
//	}
//
//	switch (COF902_OUTPICKARM_Status)	//define Left and/or Right Action
//	{
//		//case ST_PLACE_EMPTY1_EMPTY2:
//		//		TA5PlaceAction = NO_GLASS;
//		//	break;
//
//	case	ST_GW_TA5_PLACE_MB12_G1READY_G2EMPTY_REQ:
//		TA5PlaceActionString = ST_PLACE_READY1_EMPTY2;
//		TA5PlaceAction = GLASS1;	//left only
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB12_G1ERR_G2EMPTY_REQ:
//		TA5PlaceActionString = ST_PLACE_ERR1_EMPTY2;
//		TA5PlaceAction = GLASS1;	//left only
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1READY_G2EMPTY_REQ:
//		TA5PlaceActionString = ST_PLACE_READY1_EMPTY2;
//		TA5PlaceAction = GLASS1;	//left only
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1ERR_G2EMPTY_REQ:
//		TA5PlaceActionString = ST_PLACE_ERR1_EMPTY2;
//		TA5PlaceAction = GLASS1;	//left only
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB12_G1EMPTY_G2READY_REQ:
//		TA5PlaceActionString = ST_PLACE_EMPTY1_READY2;
//		TA5PlaceAction = GLASS2;	//Right Only
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB12_G1EMPTY_G2ERR_REQ:
//		TA5PlaceActionString = ST_PLACE_EMPTY1_ERR2;
//		TA5PlaceAction = GLASS2;	//Right Only
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1EMPTY_G2READY_REQ:
//		TA5PlaceActionString = ST_PLACE_EMPTY1_READY2;
//		TA5PlaceAction = GLASS2;	//Right Only
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1EMPTY_G2ERR_REQ:
//		TA5PlaceActionString = ST_PLACE_EMPTY1_ERR2;
//		TA5PlaceAction = GLASS2;	//Right Only
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
//		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB12_G1READY_G2READY_REQ:
//		TA5PlaceActionString = ST_PLACE_READY1_READY2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;	//both Glass
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB12_G1ERR_G2READY_REQ:
//		TA5PlaceActionString = ST_PLACE_ERR1_READY2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB12_G1READY_G2ERR_REQ:
//		TA5PlaceActionString = ST_PLACE_READY1_ERR2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB12_G1ERR_G2ERR_REQ:
//		TA5PlaceActionString = ST_PLACE_ERR1_ERR2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
//		m_lCurMBHead = MAINBOND_1;	//MB12
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1READY_G2READY_REQ:
//		TA5PlaceActionString = ST_PLACE_READY1_READY2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1ERR_G2READY_REQ:
//		TA5PlaceActionString = ST_PLACE_ERR1_READY2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1READY_G2ERR_REQ:
//		TA5PlaceActionString = ST_PLACE_READY1_ERR2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	case ST_GW_TA5_PLACE_MB34_G1ERR_G2ERR_REQ:
//		TA5PlaceActionString = ST_PLACE_ERR1_ERR2;
//		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
//		m_lCurMBHead = MAINBOND_2;	//MB34
//		bResult = TRUE;
//#if 1 //20170406
//		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
//		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];
//
//		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
//		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
//#endif
//		break;
//
//	default:
//		TA5PlaceAction = NO_GLASS;  
//		bResult = FALSE;
//		break;
//	}
//
//	return bResult;
//}
