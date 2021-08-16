/////////////////////////////////////////////////////////////////
//	WH2.cpp : interface of the CWH2 class
//
//	Description:
//		PPI9000 Application Software
//
//	Date:		Thursday, September 09, 2010
//	Revision:	1.00
//
//	By:			PPI9000
//				CSP
//
//	Copyright @ ASM Assembly Automation Ltd., 2010.
//	ALL rights reserved.
//
/////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PI9000.h"
#include "PI9000_Constant.h"
#include "WH2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CWH2, CInspWH)

CWH2::CWH2()
{
	// CPI9000Srn
	m_lStnErrorCode		= IDS_INSPWH2_NOT_SELECTED_ERR;
	m_lModSelectMode	= GLASS_MOD;

	// CPRTaskStn
	m_szPRTaskStnName	= GetStation().Trim('_');
	m_lPRStnErrorCode	= IDS_INSPWH2_PR_MOD_NOT_SELECTED;
	m_lPRModSelectMode	= GLASS_MOD;

	m_lCurSlave = WH_2;
	m_lCurInspWH = WH_2;
	//m_lCurGlass = GLASS1;

	// Motor Related
	m_stMotorY.InitMotorInfo("WH2_Y" , &m_bModSelected, "WH2_Y_ENC_PORT_1");
	//m_stMotorX.InitMotorInfo("WH2_X" , &m_bModSelected);
	m_stMotorT.InitMotorInfo("WH2_T", &m_bModSelected);
//#ifdef SRCH_EMBEDDED_SENSOR //20170810
//	m_stMotorT.SetMtrSnrPortName(CString("WH2_T_MTRSNR_PORT"), TRUE, TRUE); //channel has embeded limit snr and indexer inside the encoder plug. An external limit sensor as home sensor
//#endif

	//Measurement Encoder
	m_myMotorList.Add((CMotorInfo*)&m_stMotorT);
	m_myMotorList.Add((CMotorInfo*)&m_stMotorY);
	//m_myMotorList.Add((CMotorInfo*)&m_stMotorX);
	m_stMotorY.stAttrib.bIsAbsoluteEnc	= TRUE;			

	// Si Related
	m_stTLmtSnr		= CSiInfo("INSPWH2TLmtSnr",		"IOMODULE_IN_PORT_3", BIT_2, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stTLmtSnr);
	m_stYLmtSnr			= CSiInfo("INSPWH2YLmtSnr",		"IOMODULE_IN_PORT_3", BIT_3, ACTIVE_HIGH);
	m_mySiList.Add((CSiInfo*)&m_stYLmtSnr);

	m_stGlass1VacSnr		= CSiInfo("INSPWH2G1VacSnr",		"IOMODULE_IN_PORT_3", BIT_9, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass1VacSnr);	
	m_stGlass1AuxVacSnr		= CSiInfo("INSPWH2G1AuxVacSnr",		"IOMODULE_IN_PORT_3", BIT_10, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass1AuxVacSnr);	
	m_stGlass2VacSnr		= CSiInfo("INSPWH2G2VacSnr",		"IOMODULE_IN_PORT_3", BIT_11, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass2VacSnr);
	m_stGlass2AuxVacSnr		= CSiInfo("INSPWH2G2AuxVacSnr",		"IOMODULE_IN_PORT_3", BIT_12, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stGlass2AuxVacSnr);

	// So Related
	m_stGlass1VacSol		= CSoInfo("INSPWH2G1VacSol",		"IOMODULE_OUT_PORT_4", BIT_6, INSPWH_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass1VacSol);
	m_stGlass1AuxVacSol		= CSoInfo("INSPWH2G1AuxVacSol",		"IOMODULE_OUT_PORT_4", BIT_7, INSPWH_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass1AuxVacSol);
	m_stWeakBlowSol			= CSoInfo("INSPWH2G1WeakBlowSol",		"IOMODULE_OUT_PORT_4", BIT_8, INSPWH_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stWeakBlowSol);
	m_stGlass2VacSol		= CSoInfo("INSPWH2G2VacSol",		"IOMODULE_OUT_PORT_4", BIT_9, INSPWH_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass2VacSol);
	m_stGlass2AuxVacSol		= CSoInfo("INSPWH2G2AuxVacSol",		"IOMODULE_OUT_PORT_4", BIT_10, INSPWH_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stGlass2AuxVacSol);
	m_stWeakBlow2Sol		= CSoInfo("INSPWH2G2WeakBlowSol",		"IOMODULE_OUT_PORT_4", BIT_11, INSPWH_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stWeakBlow2Sol);

#ifdef MB_FORCE_ADC
	m_stADCPort					= CPortInfo("WH2ADCPort",			"IOMODULE_ADC_PORT_1", BIT_0);
#endif
}

CWH2::~CWH2()
{
}

BOOL CWH2::InitInstance()
{
	CInspWH::InitInstance();

	return TRUE;
}

INT CWH2::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CInspWH::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CWH2::Operation()
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

VOID CWH2::RegisterVariables()
{
	try
	{
		// -------------------------------------------------------
		// PPI9000Stn Variable and Function
		// -------------------------------------------------------
		RegVariable(GetStation() + _T("bCheckMotor"), &m_bCheckMotor);
		RegVariable(GetStation() + _T("bCheckIO"), &m_bCheckIO);

		RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		RegVariable(GetStation() + _T("bShowModSelected"), &m_bShowModSelected);
		RegVariable(GetStation() + _T("bModError"), &m_bModError);
		RegVariable(GetStation() + _T("bPRError"), &m_bPRError);

		RegVariable(GetStation() + _T("bGlass1Exist"), &m_bGlass1Exist);
		RegVariable(GetStation() + _T("bGlass2Exist"), &m_bGlass2Exist);

		RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		RegVariable(GetStation() + _T("bDebugCal"), &HMI_bDebugCal);
		RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeAll"), HMI_HomeAll);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugCal"), HMI_SetDebugCal);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogMotion"), HMI_SetLogMotion);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("RotaryEncoderCalibration"), HMI_RotaryEncoderCalibration);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("Find1stEnc2ndEncRelation"), HMI_Find1stEnc2ndEncRelation);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("GoCORPosn"), HMI_GoCORPosn);
		

		// -------------------------------------------------------
		// CPRTaskStn Variable and Function
		// -------------------------------------------------------
		RegVariable(GetStation() + _T("bPRSelected"), &m_bPRSelected);	

		RegVariable(GetStation() + _T("lPRDelay"), &m_lPRDelay);
		RegVariable(GetStation() + _T("lPRDelay2"), &m_lPRDelay2);
		RegVariable(GetStation() + _T("lCalibPRDelay"), &m_lCalibPRDelay);
		RegVariable(GetStation() + _T("lPRRetryLimit"), &m_lPRRetryLimit);
		RegVariable(GetStation() + _T("lRejectGlassLimit"), &m_lRejectGlassLimit);

		RegVariable(GetStation() + _T("ulCurPRU"), &HMI_ulCurPRU);
		RegVariable(GetStation() + _T("ulPRID"), &HMI_ulPRID);
		RegVariable(GetStation() + _T("dPRMatchScore"), &HMI_dPRMatchScore);
		RegVariable(GetStation() + _T("dPRAngleRange"), &HMI_dPRAngleRange);
		RegVariable(GetStation() + _T("dPRPercentVar"), &HMI_dPRPercentVar);
		RegVariable(GetStation() + _T("bShowPRSearch"), &HMI_bShowPRSearch);
		RegVariable(GetStation() + _T("bStandAlone"), &HMI_bStandAlone); //gateway
		
		RegVariable(GetStation() + _T("bRejectAlways"), &m_bRejectAlways);
		RegVariable(GetStation() + _T("bMoveT"), &m_bMoveT);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleAlwaysReject"), HMI_ToggleAlwaysReject);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleMoveT"), HMI_ToggleMoveT);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRMatchScore"), HMI_SetPRMatchScore);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRAngRange"), HMI_SetPRAngRange);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRPercentVar"), HMI_SetPRPercentVar);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SrchPR"), HMI_SrchPR);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ModifyPRCriteria"), HMI_ModifyPRCriteria);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRRetryLimit"), HMI_SetPRRetryLimit);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetRejectGlassLimit"), HMI_SetRejectGlassLimit);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRDelay"), HMI_SetPRDelay);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRDelay2"), HMI_SetPRDelay2);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetCalibPRDelay"), HMI_SetCalibPRDelay);


		// -------------------------------------------------------
		// CWorkHolder Variable and Function
		// -------------------------------------------------------
		RegVariable(_T("HMI_bWH2YOn"), &m_stMotorY.bHMI_On);
		//RegVariable(_T("HMI_bInspWHZ1On"), &m_stMotorZ[WHZ_1].bHMI_On);
		//RegVariable(_T("HMI_bInspWHZ2On"), &m_stMotorZ[WHZ_2].bHMI_On);
		RegVariable(_T("HMI_bWH2TOn"), &m_stMotorT.bHMI_On);
		RegVariable(_T("HMI_lWH2YEncoder"), &m_stMotorY.lHMI_CurrPosn);
		RegVariable(_T("HMI_lWH2YEncoder2nd"), &m_stMotorY.lHMI_Curr2ndPosn);
		//RegVariable(_T("HMI_lInspWHZ1Encoder"), &m_stMotorZ[WHZ_1].lHMI_CurrPosn);
		//RegVariable(_T("HMI_lInspWHZ2Encoder"), &m_stMotorZ[WHZ_2].lHMI_CurrPosn);
		RegVariable(_T("HMI_lWH2TEncoder"), &m_stMotorT.lHMI_CurrPosn);

		RegVariable(_T("HMI_bInspWH2G1VacSnr"), &m_stGlass1VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2G2VacSnr"), &m_stGlass2VacSnr.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2G1AuxVacSnr"), &m_stGlass1AuxVacSnr.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2G2AuxVacSnr"), &m_stGlass2AuxVacSnr.bHMI_Status);

		RegVariable(_T("HMI_bInspWH2TLmtSnr"), &m_stTLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2YLmtSnr"), &m_stYLmtSnr.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHZ1LmtSnr"), &m_stZ1LmtSnr.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHZ2LmtSnr"), &m_stZ2LmtSnr.bHMI_Status);


		RegVariable(_T("HMI_bInspWH2G1VacSol"), &m_stGlass1VacSol.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2G2VacSol"), &m_stGlass2VacSol.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2G1AuxVacSol"), &m_stGlass1AuxVacSol.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2G2AuxVacSol"), &m_stGlass2AuxVacSol.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2WeakBlowSol"), &m_stWeakBlowSol.bHMI_Status);
		RegVariable(_T("HMI_bInspWH2WeakBlow2Sol"), &m_stWeakBlow2Sol.bHMI_Status);

		RegVariable(GetStation() + _T("dYDistPerCount"), &m_stMotorY.stAttrib.dDistPerCount);
		//RegVariable(GetStation() + _T("dZ1DistPerCount"), &m_stMotorZ[WHZ_1].stAttrib.dDistPerCount);
		//RegVariable(GetStation() + _T("dZ2DistPerCount"), &m_stMotorZ[WHZ_2].stAttrib.dDistPerCount);
		RegVariable(GetStation() + _T("dTDistPerCount"), &m_stMotorT.stAttrib.dDistPerCount);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnY"), HMI_PowerOnY);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnT"), HMI_PowerOnT);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnZ1"), HMI_PowerOnZ1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnZ2"), HMI_PowerOnZ2);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommY"), HMI_CommY);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommT"), HMI_CommT);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommZ1"), HMI_CommZ1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommZ2"), HMI_CommZ2);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeY"), HMI_HomeY);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeZ1"), HMI_HomeZ1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeZ2"), HMI_HomeZ2);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeT"), HMI_HomeT);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG1VacSol"), HMI_ToggleG1VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG2VacSol"), HMI_ToggleG2VacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG1AuxVacSol"), HMI_ToggleG1AuxVacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG2AuxVacSol"), HMI_ToggleG2AuxVacSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlowSol"), HMI_ToggleWeakBlowSol);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlow2Sol"), HMI_ToggleWeakBlow2Sol);

		RegVariable(GetStation() + _T("bUseAnvilVacuum"), &m_bUseAnvilVacuum);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleNotUseAnvilVacuum"), HMI_ToggleNotUseAnvilVacuum);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnStepsByMicron"), HMI_SetDiagnStepsByMicron);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexYPos"), HMI_IndexYPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexYNeg"), HMI_IndexYNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexTPos"), HMI_IndexTPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexTNeg"), HMI_IndexTNeg);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ1Pos"), HMI_IndexZ1Pos);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ1Neg"), HMI_IndexZ1Neg);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ2Pos"), HMI_IndexZ2Pos);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ2Neg"), HMI_IndexZ2Neg);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXPos"), HMI_IndexX1Pos);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXNeg"), HMI_IndexX1Neg);

		// -------------------------------------------------------
		// varibales
		// -------------------------------------------------------

			
		RegVariable(GetStation() + _T("ulCurSlave"), &m_lCurSlave);
		RegVariable(GetStation() + _T("ulCurGlass"), &m_lCurGlass);
		//RegVariable(GetStation() + _T("bG1PR1Loaded"),	&pruDLPR1[GLASS1].bLoaded);
		//RegVariable(GetStation() + _T("bG1PR2Loaded"),	&pruDLPR2[GLASS1].bLoaded);
		//RegVariable(GetStation() + _T("bG2PR1Loaded"),	&pruDLPR1[GLASS2].bLoaded);
		//RegVariable(GetStation() + _T("bG2PR2Loaded"),	&pruDLPR2[GLASS2].bLoaded);
		//RegVariable(GetStation() + _T("bAlignPtSet"),	&m_stInspWH[GLASS1].bAlignPtSet);
		//RegVariable(GetStation() + _T("bValid"),	&m_stInspWH[GLASS1].bValid);

		RegVariable(GetStation() + _T("lG1XUnloadOffset"),	&m_lUnloadXOffset[GLASS1]);
		RegVariable(GetStation() + _T("lG1YUnloadOffset"),	&m_lUnloadYOffset[GLASS1]);
		RegVariable(GetStation() + _T("dG1TUnloadOffset"),	&m_dUnloadTOffset[GLASS1]);

		RegVariable(GetStation() + _T("lG2XUnloadOffset"),	&m_lUnloadXOffset[GLASS2]);
		RegVariable(GetStation() + _T("lG2YUnloadOffset"),	&m_lUnloadYOffset[GLASS2]);
		RegVariable(GetStation() + _T("dG2TUnloadOffset"),	&m_dUnloadTOffset[GLASS2]);

		RegVariable(GetStation() + _T("bXYControl"), &HMI_bXYControl);
		//RegVariable(GetStation() + _T("bZ1Control"), &HMI_bZ1Control);
		//RegVariable(GetStation() + _T("bZ2Control"), &HMI_bZ2Control);
		RegVariable(GetStation() + _T("bTControl"), &HMI_bTControl);
		RegVariable(GetStation() + _T("bCursorControl"), &HMI_bCursorControl);

		RegVariable(GetStation() + _T("lPinSpringTestCount"), &m_lPinSpringTestCount); //20121009 pin detection spring test
		RegVariable(GetStation() + _T("bMeasurePolarizerThickness"), &m_bMeasurePolarizerThickness); //20120927 pin detect polarizer
		
		//Meausement Routine
		RegVariable(GetStation() + _T("dMeasuredDistance"), &m_dMeasuredDistance);
		RegVariable(GetStation() + _T("dMeasuredAngle"), &m_dMeasuredAngle);
		RegVariable(GetStation() + _T("bEnableMeasure"), &m_bEnableMeasure);

		// -------------------------------------------------------
		// commands
		// -------------------------------------------------------
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRSelected"), HMI_SetPRSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("UpdateSetupStatus"), HMI_UpdateSetupStatus);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectWHNum"), HMI_SelectWHNum);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectGlassNum"), HMI_SelectGlassNum);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectPRU"), HMI_SelectPRU);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRPosn"), HMI_SetPRPosn);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRLevel"), HMI_SetPRLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp1"), HMI_SearchAp1);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp2"), HMI_SearchAp2);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchCentre"), HMI_SearchCentre);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchDLnUL"), HMI_SearchDLnUL);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MoveTest"), HMI_MoveTest);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("StartSearchDLnULCycle"), HMI_StartSearchDLnULCycle);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("StartPRCycleTest"), HMI_StartPRCycleTest);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG1XUnloadOffset"), HMI_SetG1XUnloadOffset);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG1YUnloadOffset"), HMI_SetG1YUnloadOffset);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG2XUnloadOffset"), HMI_SetG2XUnloadOffset);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG2YUnloadOffset"), HMI_SetG2YUnloadOffset);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetStandAlone"), HMI_SetStandAlone); //gateway

		//Measurement routine
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MeasureSetZero"), HMI_MeasureSetZero);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MeasureEnable"), HMI_MeasureEnable);

#ifdef MB_FORCE_ADC
		RegVariable(_T("HMI_lWH2ADCForce"), &m_lForceADCValue);

		//RegVariable(_T("HMI_dMB4ADCForceWeight"), &m_dForceBondWeight);

		//RegVariable(_T("HMI_dMB4ADCForceSnrFactor"), &m_dForceSensorFactor);

		//RegVariable(GetStation() + _T("bLogForceSensor"), &m_bLogForceSensor);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogForceSensor"), HMI_SetLogForceSensor); //20150315

		//RegVariable(GetStation() + _T("bLogForceSensorEndOfBond"), &m_bLogForceSensorEndOfBond); //20161117
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogForceSensorEndOfBond"), HMI_SetLogForceSensorEndOfBond); 

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MBReadADCForce"), HMI_MBReadADCForce);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetForceSensorFactor"), HMI_SetForceSensorFactor);
#endif
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}
