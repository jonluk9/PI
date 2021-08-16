/////////////////////////////////////////////////////////////////
//	INSPWH.cpp : interface of the CInspWH class
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

#define INSPWH_EXTERN
#include "InspWH.h"
#include "WH1.h"
#include "WH2.h"
#include "InspOpt.h"
#include "InPickArm.h"
#include "OutPickArm.h"
#include "PickArm.h"

#include "WinEagle.h"
#include "HouseKeeping.h"
#include "SettingFile.h"
#include "TimeChart.h" //20120831


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CInspWH, CWorkHolder)

#ifdef MB_FORCE_ADC
//BOOL	CInspWH::m_bLogForceSensor = FALSE;
//BOOL	CInspWH::m_bLogForceSensorEndOfBond = FALSE;
//LONG	CInspWH::m_lForceDifferenceTol = 30; //30%
#endif

CInspWH::CInspWH()
{
	//// CPI9000Srn
	//m_lStnErrorCode		= IDS_INSPWH_NOT_SELECTED_ERR;
	//m_lModSelectMode	= GLASS_MOD;

	//// CPRTaskStn
	//m_szPRTaskStnName	= GetStation().Trim('_');
	//m_lPRStnErrorCode	= IDS_INSPWH_PR_MOD_NOT_SELECTED;
	//m_lPRModSelectMode	= GLASS_MOD;

	// Motor Related
	//m_stMotorY.InitMotorInfo("INSPWH_Y" , &m_bModSelected);
	//m_stMotorX.InitMotorInfo("INSPWH_X" , &m_bModSelected);
	//m_stMotorT.InitMotorInfo("INSPWH_T" , &m_bModSelected);
	//m_stMotorZ[WHZ_1].InitMotorInfo("INSPWH_Z1", &m_bModSelected);
	//m_stMotorZ[WHZ_2].InitMotorInfo("INSPWH_Z2", &m_bModSelected);

	//Measurement Encoder

	//m_myMotorList.Add((CMotorInfo*)&m_stMotorZ[WHZ_1]); //20150130 home Z first
	//m_myMotorList.Add((CMotorInfo*)&m_stMotorZ[WHZ_2]);
	//m_myMotorList.Add((CMotorInfo*)&m_stMotorY);
	//m_myMotorList.Add((CMotorInfo*)&m_stMotorX);
	//m_myMotorList.Add((CMotorInfo*)&m_stMotorT);

	// Si Related
	//m_stGlass1VacSnr		= CSiInfo("INSPWH1VacSnr",		"IOMODULE_IN_PORT_4", BIT_4, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stGlass1VacSnr);	
	//m_stGlass2VacSnr		= CSiInfo("INSPWH2VacSnr",		"IOMODULE_IN_PORT_4", BIT_5, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stGlass2VacSnr);

	//m_stXLmtSmr			= CSiInfo("INSPWHXLmtSnr",		"IOMODULE_IN_PORT_4", BIT_0, ACTIVE_HIGH);
	//m_mySiList.Add((CSiInfo*)&m_stXLmtSmr);	
	//m_stYLmtSnr			= CSiInfo("INSPWHYLmtSnr",		"IOMODULE_IN_PORT_4", BIT_1, ACTIVE_HIGH);
	//m_mySiList.Add((CSiInfo*)&m_stYLmtSnr);
	//m_stZ1LmtSnr		= CSiInfo("INSPWHZ1LmtSnr",		"IOMODULE_IN_PORT_4", BIT_2, ACTIVE_HIGH);
	//m_mySiList.Add((CSiInfo*)&m_stZ1LmtSnr);
	//m_stZ2LmtSnr		= CSiInfo("INSPWHZ2LmtSnr",		"IOMODULE_IN_PORT_4", BIT_3, ACTIVE_HIGH);
	//m_mySiList.Add((CSiInfo*)&m_stZ2LmtSnr);

	//// So Related
	//m_stGlass1VacSol		= CSoInfo("INSPWH1VacSol",		"IOMODULE_OUT_PORT_3", BIT_0, INSPWH_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stGlass1VacSol);
	//m_stGlass2VacSol		= CSoInfo("INSPWH2VacSol",		"IOMODULE_OUT_PORT_3", BIT_2, INSPWH_SOL, ACTIVE_LOW);
	//m_mySoList.Add((CSoInfo*)&m_stGlass2VacSol);
	//m_stWeakBlowSol			= CSoInfo("INSPWH1WeakBlowSol",		"IOMODULE_OUT_PORT_3", BIT_1, INSPWH_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stWeakBlowSol);
	//m_stWeakBlow2Sol		= CSoInfo("INSPWH2WeakBlowSol",		"IOMODULE_OUT_PORT_3", BIT_3, INSPWH_SOL, ACTIVE_HIGH);
	//m_mySoList.Add((CSoInfo*)&m_stWeakBlow2Sol);

	m_szRejectReason[GLASS1]		= "";
	m_szRejectReason[GLASS2]		= "";
	m_dwStartTime			= 0;
	m_dProcessTime			= 0;
	m_lCurGlass				= GLASS1;
	m_lPinSpringTestCount	= 0; //20121009 pin detection spring test
	m_bStartSearchDLnULTest = FALSE;

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		m_bGlassWithError[i]		= FALSE;

		m_stInspWH[i].bAlignPtSet		= FALSE;
		m_stInspWH[i].bBonded			= FALSE;
		m_stInspWH[i].bEnable			= TRUE;
		m_stInspWH[i].bValid			= FALSE;
		m_stInspWH[i].dmtrAlignPt1.x	= 0.0;
		m_stInspWH[i].dmtrAlignPt1.y	= 0.0;
		m_stInspWH[i].dmtrAlignPt2.x	= 0.0;
		m_stInspWH[i].dmtrAlignPt2.y	= 0.0;

		m_dAlignAngle[i]			= 0.0;
		m_mtrAlignCentre[i].x		= 0;
		m_mtrAlignCentre[i].y		= 0;
		m_mtrCurrAlignDelta[i].x	= 0;
		m_mtrCurrAlignDelta[i].y	= 0;
		m_dGlassRefAng[i]			= 0.0;
		m_dmtrAlignRef[i].x			= 0.0;
		m_dmtrAlignRef[i].y			= 0.0;
		m_dAlignRefAngle[i]			= 0.0;
		m_mtrPRCalibPosn[i].x		= 0;
		m_mtrPRCalibPosn[i].y		= 0;

	}

	m_mtrULPRCalibPosn.x		= 0;
	m_mtrULPRCalibPosn.y		= 0;
	
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
		{
			m_mtrPR1Posn[i][j].x		= 0;
			m_mtrPR1Posn[i][j].y		= 0;
			m_mtrPR2Posn[i][j].x		= 0;
			m_mtrPR2Posn[i][j].y		= 0;
		}
			m_mtrRejectGlassPosn[i].x		= 0;
			m_mtrRejectGlassPosn[i].y		= 0;
			m_mtrRejectGlassOffset[i].x		= 0;
			m_mtrRejectGlassOffset[i].y		= 0;

		m_lTInspPosn[i] = 0;

	}

	m_mtrAlignUnloadOffset.x	= 0;
	m_mtrAlignUnloadOffset.y	= 0;

	m_mtrULRefPosn.x	= 0;
	m_mtrULRefPosn.y	= 0;
	m_mtrDL1RefPosn.x	= 0;
	m_mtrDL1RefPosn.y	= 0;
	m_mtrDL2RefPosn.x	= 0;
	m_mtrDL2RefPosn.y	= 0;


	//for (INT i = WH_1; i < MAX_NUM_OF_WH; i++)
	//{
	//	m_lPRLevel[WH_1][i]			= 0;
	//	m_lPRLevel[WH_2][i]			= 0;
	//	m_lULPRLevel[i]			= 0;
	//}

	m_lCurGlass		= GLASS1;
	HMI_ulCurPRU	= 0;
		
	m_lAddJobPR_GRAB = IN_WH_PR_GRAB; //20120831
	m_lAddJobPR_PROCESS = IN_WH_PR_PROCESS;
	m_bMeasurePolarizerThickness = FALSE;

	m_mtrMeasureSetZeroPt.x = 0;
	m_mtrMeasureSetZeroPt.y = 0;

	m_lCurMBHead = MAINBOND_1;
	TA5PlaceAction = NO_GLASS;   // init No Glass
	TA5PlaceActionString = ST_PLACE_EMPTY1_EMPTY2;	//init

	m_lCurGlass = GLASS2;

	m_lPR2AlignErrorCnt = 0;
	m_lGlassAlignPROKCnt = 0;
	m_lPR1AlignErrorCnt = 0;
	m_lGlassInspectOKCnt = 0;
	m_lGlassInspectRejectCnt = 0;
	HMI_bStandAlone = FALSE;
	m_bRejectAlways = FALSE;
	m_bMoveT = FALSE;

	m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
	m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
	m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
	m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
	
#ifdef MB_FORCE_ADC 
	m_lForceADCValue = 0;
	
	//m_dForceBondWeight = 0.0;

	//m_dForceSensorFactor = 50.0;	 //MB1 54.08pC/lbf, MB3 54.47pC/lbf 

	//m_bMBForceLogRun[MAINBOND_1] = FALSE;
	//m_bMBForceLogRun[MAINBOND_2] = FALSE;
	//m_bMBForceLogRun[MAINBOND_3] = FALSE;
	//m_bMBForceLogRun[MAINBOND_4] = FALSE;

	//m_szLogLabel.Format("--");
	//
	////m_bForceTolerenceError = FALSE;
	////m_bForceContactError = FALSE;
	//
	//m_dForceKgEndOfBonding = 0.0; //20161117
	//m_lForceADCEndOfBonding = 0;
	//m_hThreadForceADC = NULL;
#endif
}

CInspWH::~CInspWH()
{
}

BOOL CInspWH::InitInstance()
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
#ifdef MB_FORCE_ADC
			m_stADCPort.SetGmpPort(pGmpSys->AddDACPort(m_stADCPort.GetPortName())); //use dac read function for ADC port
#endif
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

		if (GetStnName() == "WH1")
		{	
			pAppMod->m_bInspWH1Comm = TRUE;
		}
		else if (GetStnName() == "WH2")
		{	
			pAppMod->m_bInspWH2Comm = TRUE;
		}

		// wait for all servo commutated (even with error)
		do
		{
			Sleep(100);
		} while (!pAppMod->IsAllServoCommutated());

		if (!m_bInitInstanceError)
		{
			CString szErrMsg("No_Error");
			szMsg = _T("	") + GetName() + " --- homing motors";
			DisplayMessage(szMsg);
			for (i = 0; i < m_myMotorList.GetSize(); i++)
			{
				szErrMsg = _T("No_Error");
				if (GetmyMotorList().GetAt(i)->Home(GMP_NOWAIT, szErrMsg) != GMP_SUCCESS)
				{
					m_bInitInstanceError	= TRUE;
					pAppMod->m_bHWInitError = TRUE;
				}
				if (szErrMsg != "No_Error")
				{
					HMIMessageBox(MSG_OK, "Home Error", szErrMsg);
					DisplayMessage(szErrMsg);
					//SetError(IDS_MAINBOND_MTR_HOME_ERR + (m_lCurSlave * CONVERT_TO_MAPPING_MB), szErrMsg);
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
				// Everythings OK
				m_bModSelected			= TRUE;
			}

			if (GetStnName() == "WH1")
			{	
				pAppMod->m_bInspWH1Home	= TRUE;
			}
			else if (GetStnName() == "WH2")
			{	
				pAppMod->m_bInspWH2Home	= TRUE;
			}
		}
	}
	else
	{
		if (GetStnName() == "WH1")
		{	
			pAppMod->m_bInspWH1Home	= TRUE;
			pAppMod->m_bInspWH1Comm	= TRUE;
		}
		else if (GetStnName() == "WH2")
		{	
			pAppMod->m_bInspWH2Home	= TRUE;
			pAppMod->m_bInspWH2Comm	= TRUE;
		}
	}

	return TRUE;
}

INT CInspWH::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWorkHolder::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CInspWH::Operation()
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
VOID CInspWH::UpdateOutput()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	if (m_fHardware && m_bCheckMotor) //20121016
	{
		if (m_bEnableMeasure)
		{
			DOUBLE dx, dy;
			dx = CountToDistance(pCInPickArm->m_stMotorX.GetEncPosn() - m_mtrMeasureSetZeroPt.x, pCInPickArm->m_stMotorX.stAttrib.dDistPerCount);
			dy = CountToDistance(m_stMotorY.GetEncPosn() - m_mtrMeasureSetZeroPt.y, m_stMotorY.stAttrib.dDistPerCount);
			m_dMeasuredDistance = sqrt(pow(dy, 2.0) + pow(dx, 2.0));
			m_dMeasuredAngle = calcDisplayAngleDegree(dy, dx);
		}
	}
#ifdef MB_FORCE_ADC
			LONG lForce = 0;
			if (GetADCValue(lForce) == GMP_SUCCESS)
			{
				m_lForceADCValue = lForce;
				//m_dForceBondWeight = RawADCToForce(m_lForceADCValue, m_dForceSensorFactor) / 2.2;
			}
#endif

	// Keep it for debug breakpoint
	CWorkHolder::UpdateOutput();
}

VOID CInspWH::RegisterVariables()
{
	try
	{
		// -------------------------------------------------------
		// PI9000Stn Variable and Function
		// -------------------------------------------------------
		//RegVariable(GetStation() + _T("bCheckMotor"), &m_bCheckMotor);
		//RegVariable(GetStation() + _T("bCheckIO"), &m_bCheckIO);

		//RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		//RegVariable(GetStation() + _T("bShowModSelected"), &m_bShowModSelected);
		//RegVariable(GetStation() + _T("bModError"), &m_bModError);
		//RegVariable(GetStation() + _T("bPRError"), &m_bPRError);

		//RegVariable(GetStation() + _T("bGlass1Exist"), &m_bGlass1Exist);
		//RegVariable(GetStation() + _T("bGlass2Exist"), &m_bGlass2Exist);

		//RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		//RegVariable(GetStation() + _T("bDebugCal"), &HMI_bDebugCal);
		//RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeAll"), HMI_HomeAll);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugCal"), HMI_SetDebugCal);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogMotion"), HMI_SetLogMotion);
		//

		// -------------------------------------------------------
		// CPRTaskStn Variable and Function
		// -------------------------------------------------------
		//RegVariable(GetStation() + _T("bPRSelected"), &m_bPRSelected);	

		//RegVariable(GetStation() + _T("lPRDelay"), &m_lPRDelay);
		//RegVariable(GetStation() + _T("lCalibPRDelay"), &m_lCalibPRDelay);
		//RegVariable(GetStation() + _T("lPRRetryLimit"), &m_lPRRetryLimit);
		//RegVariable(GetStation() + _T("lRejectGlassLimit"), &m_lRejectGlassLimit);

		//RegVariable(GetStation() + _T("ulCurPRU"), &HMI_ulCurPRU);
		//RegVariable(GetStation() + _T("ulPRID"), &HMI_ulPRID);
		//RegVariable(GetStation() + _T("dPRMatchScore"), &HMI_dPRMatchScore);
		//RegVariable(GetStation() + _T("dPRAngleRange"), &HMI_dPRAngleRange);
		//RegVariable(GetStation() + _T("dPRPercentVar"), &HMI_dPRPercentVar);
		//RegVariable(GetStation() + _T("bShowPRSearch"), &HMI_bShowPRSearch);
		//RegVariable(GetStation() + _T("bStandAlone"), &HMI_bStandAlone); //gateway
		//
		//RegVariable(GetStation() + _T("bRejectAlways"), &m_bRejectAlways);
		//RegVariable(GetStation() + _T("bMoveT"), &m_bMoveT);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleAlwaysReject"), HMI_ToggleAlwaysReject);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleMoveT"), HMI_ToggleMoveT);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MoveTest"), HMI_MoveTest);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRMatchScore"), HMI_SetPRMatchScore);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRAngRange"), HMI_SetPRAngRange);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRPercentVar"), HMI_SetPRPercentVar);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SrchPR"), HMI_SrchPR);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ModifyPRCriteria"), HMI_ModifyPRCriteria);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRRetryLimit"), HMI_SetPRRetryLimit);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetRejectGlassLimit"), HMI_SetRejectGlassLimit);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRDelay"), HMI_SetPRDelay);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetCalibPRDelay"), HMI_SetCalibPRDelay);


		//// -------------------------------------------------------
		//// CWorkHolder Variable and Function
		//// -------------------------------------------------------
		//RegVariable(_T("HMI_bInspWHYOn"), &m_stMotorY.bHMI_On);
		////RegVariable(_T("HMI_bInspWHZ1On"), &m_stMotorZ[WHZ_1].bHMI_On);
		////RegVariable(_T("HMI_bInspWHZ2On"), &m_stMotorZ[WHZ_2].bHMI_On);
		////RegVariable(_T("HMI_bInspWHXOn"), &m_stMotorX.bHMI_On);
		//RegVariable(_T("HMI_lInspWHYEncoder"), &m_stMotorY.lHMI_CurrPosn);
		////RegVariable(_T("HMI_lInspWHZ1Encoder"), &m_stMotorZ[WHZ_1].lHMI_CurrPosn);
		////RegVariable(_T("HMI_lInspWHZ2Encoder"), &m_stMotorZ[WHZ_2].lHMI_CurrPosn);
		////RegVariable(_T("HMI_lInspWHXEncoder"), &m_stMotorX.lHMI_CurrPosn);

		//RegVariable(_T("HMI_bInspWHG1VacSnr"), &m_stGlass1VacSnr.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHG2VacSnr"), &m_stGlass2VacSnr.bHMI_Status);

		//RegVariable(_T("HMI_bInspWHXLmtSmr"), &m_stXLmtSmr.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHYLmtSnr"), &m_stYLmtSnr.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHZ1LmtSnr"), &m_stZ1LmtSnr.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHZ2LmtSnr"), &m_stZ2LmtSnr.bHMI_Status);


		//RegVariable(_T("HMI_bInspWHG1VacSol"), &m_stGlass1VacSol.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHG2VacSol"), &m_stGlass2VacSol.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHWeakBlowSol"), &m_stWeakBlowSol.bHMI_Status);
		//RegVariable(_T("HMI_bInspWHWeakBlow2Sol"), &m_stWeakBlow2Sol.bHMI_Status);

		//RegVariable(GetStation() + _T("dYDistPerCount"), &m_stMotorY.stAttrib.dDistPerCount);
		////RegVariable(GetStation() + _T("dZ1DistPerCount"), &m_stMotorZ[WHZ_1].stAttrib.dDistPerCount);
		////RegVariable(GetStation() + _T("dZ2DistPerCount"), &m_stMotorZ[WHZ_2].stAttrib.dDistPerCount);
		////RegVariable(GetStation() + _T("dXDistPerCount"), &m_stMotorX.stAttrib.dDistPerCount);
		//
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnY"), HMI_PowerOnY);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnX"), HMI_PowerOnX1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnZ1"), HMI_PowerOnZ1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnZ2"), HMI_PowerOnZ2);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommY"), HMI_CommY);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommX"), HMI_CommX1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommZ1"), HMI_CommZ1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommZ2"), HMI_CommZ2);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeY"), HMI_HomeY);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeZ1"), HMI_HomeZ1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeZ2"), HMI_HomeZ2);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeX"), HMI_HomeX1);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG1VacSol"), HMI_ToggleG1VacSol);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleG2VacSol"), HMI_ToggleG2VacSol);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlowSol"), HMI_ToggleWeakBlowSol);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleWeakBlow2Sol"), HMI_ToggleWeakBlow2Sol);
		//
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexYPos"), HMI_IndexYPos);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexYNeg"), HMI_IndexYNeg);
		////m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ1Pos"), HMI_IndexZ1Pos);
		////m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ1Neg"), HMI_IndexZ1Neg);
		////m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ2Pos"), HMI_IndexZ2Pos);
		////m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZ2Neg"), HMI_IndexZ2Neg);
		////m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXPos"), HMI_IndexX1Pos);
		////m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXNeg"), HMI_IndexX1Neg);

		//// -------------------------------------------------------
		//// varibales
		//// -------------------------------------------------------

		//	
		//RegVariable(GetStation() + _T("ulCurSlave"), &m_lCurSlave);
		//RegVariable(GetStation() + _T("ulCurGlass"), &m_lCurGlass);
		//RegVariable(GetStation() + _T("ulCurWH"), &m_lCurInspWH);
		//RegVariable(GetStation() + _T("bG1PR1Loaded"),	&pruDLPR1[GLASS1].bLoaded);
		//RegVariable(GetStation() + _T("bG1PR2Loaded"),	&pruDLPR2[GLASS1].bLoaded);
		//RegVariable(GetStation() + _T("bG2PR1Loaded"),	&pruDLPR1[GLASS2].bLoaded);
		//RegVariable(GetStation() + _T("bG2PR2Loaded"),	&pruDLPR2[GLASS2].bLoaded);
		//RegVariable(GetStation() + _T("bAlignPtSet"),	&m_stInspWH[GLASS1].bAlignPtSet);
		//RegVariable(GetStation() + _T("bValid"),	&m_stInspWH[GLASS1].bValid);

		//RegVariable(GetStation() + _T("lG1XUnloadOffset"),	&m_lUnloadXOffset[GLASS1]);
		//RegVariable(GetStation() + _T("lG1YUnloadOffset"),	&m_lUnloadYOffset[GLASS1]);
		//RegVariable(GetStation() + _T("dG1TUnloadOffset"),	&m_dUnloadTOffset[GLASS1]);

		//RegVariable(GetStation() + _T("lG2XUnloadOffset"),	&m_lUnloadXOffset[GLASS2]);
		//RegVariable(GetStation() + _T("lG2YUnloadOffset"),	&m_lUnloadYOffset[GLASS2]);
		//RegVariable(GetStation() + _T("dG2TUnloadOffset"),	&m_dUnloadTOffset[GLASS2]);

		//RegVariable(GetStation() + _T("bXYControl"), &HMI_bXYControl);
		//RegVariable(GetStation() + _T("bZ1Control"), &HMI_bZ1Control);
		//RegVariable(GetStation() + _T("bZ2Control"), &HMI_bZ2Control);
		//RegVariable(GetStation() + _T("bTControl"), &HMI_bTControl);
		//RegVariable(GetStation() + _T("bCursorControl"), &HMI_bCursorControl);

		//RegVariable(GetStation() + _T("lPinSpringTestCount"), &m_lPinSpringTestCount); //20121009 pin detection spring test
		//RegVariable(GetStation() + _T("bMeasurePolarizerThickness"), &m_bMeasurePolarizerThickness); //20120927 pin detect polarizer
		//
		////Meausement Routine
		//RegVariable(GetStation() + _T("dMeasuredDistance"), &m_dMeasuredDistance);
		//RegVariable(GetStation() + _T("dMeasuredAngle"), &m_dMeasuredAngle);
		//RegVariable(GetStation() + _T("bEnableMeasure"), &m_bEnableMeasure);

		//// -------------------------------------------------------
		//// commands
		//// -------------------------------------------------------
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRSelected"), HMI_SetPRSelected);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("UpdateSetupStatus"), HMI_UpdateSetupStatus);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectWHNum"), HMI_SelectWHNum);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectGlassNum"), HMI_SelectGlassNum);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectPRU"), HMI_SelectPRU);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRPosn"), HMI_SetPRPosn);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRLevel"), HMI_SetPRLevel);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp1"), HMI_SearchAp1);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp2"), HMI_SearchAp2);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchCentre"), HMI_SearchCentre);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchDLnUL"), HMI_SearchDLnUL);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("StartPRCycleTest"), HMI_StartPRCycleTest);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG1XUnloadOffset"), HMI_SetG1XUnloadOffset);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG1YUnloadOffset"), HMI_SetG1YUnloadOffset);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG2XUnloadOffset"), HMI_SetG2XUnloadOffset);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetG2YUnloadOffset"), HMI_SetG2YUnloadOffset);

		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetStandAlone"), HMI_SetStandAlone); //gateway

		////Measurement routine
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MeasureSetZero"), HMI_MeasureSetZero);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MeasureEnable"), HMI_MeasureEnable);
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}
// ------------------------------------------------------------------------
// Virtual CPRTaskStn Function delcaration
// -----------------------------------------------------------------------
CMotorInfo &CInspWH::GetMotorX() 
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	return pCInPickArm->m_stMotorX;
}

CMotorInfo &CInspWH::GetMotorY()
{
	return m_stMotorY;
}

CMotorInfo &CInspWH::GetMotorT()
{
	return m_stMotorT;
}

//CMotorInfo &CInspWH::GetMotorZ(WHZNum lWHZ)
//{
//	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
//
//	return pCInspOpt->m_stMotorZ;
//	if (lWHZ == WHZ_1)
//	{
//		return m_stMotorZ[WHZ_1];
//	}
//	else if (lWHZ == WHZ_2)
//	{
//		return m_stMotorZ[WHZ_2];
//	}
//	else
//	{
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return m_stMotorZ[WHZ_1];
//	}
//}

INT CInspWH::SyncZ()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	return pCInPickArm->m_stMotorZ.Sync();
}

INT CInspWH::SyncX()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	return pCInPickArm->m_stMotorX.Sync();
}

INT CInspWH::SyncY()
{
	return m_stMotorY.Sync();
}

INT CInspWH::SyncT()
{
	return m_stMotorT.Sync();
}

//INT CInspWH::SyncZ()
//{
//	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
//
//	return pCInspOpt->m_stMotorZ.Sync();
//	if (m_lCurGlass == GLASS1)
//	{
//		return m_stMotorZ[WHZ_1].Sync();
//	}
//	else if (m_lCurGlass == GLASS2)
//	{
//		return m_stMotorZ[WHZ_2].Sync();
//	}
//	else 
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;
//	}
//}

INT CInspWH::MoveZToPRCalibLevel(BOOL bWait)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	if (
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
	)
	{
		return GMP_FAIL;
	}
	if (bWait == GMP_WAIT)
	{
		if (
			pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS
		)
		{
			return GMP_FAIL;
		}
	}
	return GMP_SUCCESS;
}

INT CInspWH::MoveXYToPRCalibPosn(BOOL bWait)
{
	return MoveAbsoluteXY(m_mtrPRCalibPosn[m_lCurGlass].x, m_mtrPRCalibPosn[m_lCurGlass].y, bWait);
}


//INT CInspWH::MoveZToPRLevel(BOOL bWait)
//{
//	if (m_lCurGlass == GLASS1)
//	{
//		return MoveZ1(INSPWH_Z_CTRL_GO_PR_LEVEL, bWait);
//	}
//	else if (m_lCurGlass == GLASS2)
//	{
//		return MoveZ2(INSPWH_Z_CTRL_GO_PR_LEVEL, bWait);
//	}
//	else 
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;
//	}
//}

INT CInspWH::MoveXYToCurrPR1Posn(BOOL bWait)
{
	CString szMsg = "";
	//MTR_POSN mtrTemp;

	szMsg.Format("MoveXYToCurrPR1Posn : INSPWH Glass = %ld, m_dCurrAlignAngle = %.3f", m_lCurGlass + 1, m_dCurrAlignAngle);
	DisplayMessage(szMsg);

	//CalPosnAfterRotate(m_mtrPR1Posn[m_lCurGlass], m_dCurrAlignAngle, &mtrTemp);

	m_dmtrCurrPR1Posn.x = m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x;	//mtrTemp.x;
	m_dmtrCurrPR1Posn.y = m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y;	//mtrTemp.y;

	return MoveAbsoluteXY((LONG)m_dmtrCurrPR1Posn.x, (LONG)m_dmtrCurrPR1Posn.y, bWait);
}

INT CInspWH::MoveXYToCurrPR2Posn(BOOL bWait)
{
	CString szMsg = "";
	//MTR_POSN mtrTemp;

	szMsg.Format("MoveXYToCurrPR2Posn : INSPWH Glass = %ld, m_dCurrAlignAngle = %.3f", m_lCurGlass + 1, m_dCurrAlignAngle);
	DisplayMessage(szMsg);

	//CalPosnAfterRotate(m_mtrPR2Posn[m_lCurGlass], m_dCurrAlignAngle, &mtrTemp);

	m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x;	//mtrTemp.x;
	m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y;	//mtrTemp.y;

	return MoveAbsoluteXY((LONG)m_dmtrCurrPR2Posn.x, (LONG)m_dmtrCurrPR2Posn.y, bWait);
}

INT CInspWH::MoveAbsoluteY(LONG lCntY, BOOL bWait)
{
	if (
		(m_stMotorY.MoveAbsolute(lCntY, GMP_NOWAIT) != GMP_SUCCESS)
	)
	{
		return GMP_FAIL;
	}
	
	if (GMP_WAIT == bWait)
	{
		if (
			(m_stMotorY.Sync() != GMP_SUCCESS)
		)
		{
			return GMP_FAIL;
		}
	}
	return GMP_SUCCESS;
}

INT CInspWH::MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	if (
		(pCInPickArm->m_stMotorX.MoveAbsolute(lCntX, GMP_NOWAIT) != GMP_SUCCESS) ||
		(m_stMotorY.MoveAbsolute(lCntY, GMP_NOWAIT) != GMP_SUCCESS)
	)
	{
		return GMP_FAIL;
	}
	
	if (GMP_WAIT == bWait)
	{
		if (
			(pCInPickArm->m_stMotorX.Sync() != GMP_SUCCESS) ||
			(m_stMotorY.Sync() != GMP_SUCCESS)
		)
		{
			return GMP_FAIL;
		}
	}
	return GMP_SUCCESS;
}

INT CInspWH::MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	if (
		(pCInPickArm->m_stMotorX.MoveRelative(lCntX, GMP_NOWAIT) != GMP_SUCCESS) ||
		(m_stMotorY.MoveRelative(lCntY, GMP_NOWAIT) != GMP_SUCCESS)
	)
	{
		return GMP_FAIL;
	}
	
	if (GMP_WAIT == bWait)
	{
		if (
			(pCInPickArm->m_stMotorX.Sync() != GMP_SUCCESS) ||
			(m_stMotorY.Sync() != GMP_SUCCESS)
		)
		{
			return GMP_FAIL;
		}
	}
	return GMP_SUCCESS;
}

PRU &CInspWH::SelectCurPRU(ULONG ulPRU)
{
	switch (ulPRU)
	{
	case 0: // PR1
		return pruDLPR1[m_lCurGlass];
		
	case 1:	// PR2
		return pruDLPR2[m_lCurGlass];

	default:
		DisplayMessage("INSPWH SelectCurPRU Error");
		return pruDLPR1[m_lCurGlass];
	}
}

// ------------------------------------------------------------------------
// END Virtual PRTaskStn Function delcaration
// ------------------------------------------------------------------------

VOID CInspWH::SetupAlignPt()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	BOOL		bResult 	= TRUE;
	CString		szAxis 		= "";
	LONG		lAnswer		= rMSG_TIMEOUT;
	static BOOL	bMovePrevPosn = FALSE;
	MTR_POSN	mtrTemp;

	CString		szMsg;
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruDLPR1[m_lCurGlass];
	pPR2PRU = &pruDLPR2[m_lCurGlass];
	pPR_INFO[GLASS1] = &m_stInspWH[GLASS1];
	pPR_INFO[GLASS2] = &m_stInspWH[GLASS2];

	if (g_ulWizardStep > 3 && g_ulWizardStep != 99)
	{
		g_ulWizardStep = 3;
	}

	g_bWizardEnableBack = FALSE;
	g_bWizardEnableNext = FALSE;
	g_bWizardEnableExit = TRUE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");


	// Perform Operations
	switch (g_ulWizardStep)
	{
	case 99:
		g_szWizardStep = "";
		g_szWizardMess = "";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = FALSE;
		g_bWizardEnableExit = TRUE;

		LogAction(__FUNCTION__);
		LogAction("AlignPt1.x = %.5f\t AlignPt1.y = %.5f\t AlignPt2.x = %.5f\t AlignPt2.y = %.5f\t", 
				  pPR_INFO[GLASS1]->dmtrAlignPt1.x, pPR_INFO[GLASS1]->dmtrAlignPt1.y,
				  pPR_INFO[GLASS1]->dmtrAlignPt2.x, pPR_INFO[GLASS1]->dmtrAlignPt2.y);
		LogAction("AlignRef.x = %.5f\t AlignRef.y = %.5f\t AlignAng = %.5f", 
				  m_dmtrAlignRef[GLASS1].x, m_dmtrAlignRef[GLASS1].y,
				  m_dAlignRefAngle[GLASS1]);

		break;

	case 0:

	// 2. Search PR
		LogAction(__FUNCTION__);
		LogAction("AlignPt1.x = %.5f\t AlignPt1.y = %.5f\t AlignPt2.x = %.5f\t AlignPt2.y = %.5f\t", 
				  pPR_INFO[GLASS1]->dmtrAlignPt1.x, pPR_INFO[GLASS1]->dmtrAlignPt1.y,
				  pPR_INFO[GLASS1]->dmtrAlignPt2.x, pPR_INFO[GLASS1]->dmtrAlignPt2.y);
		LogAction("AlignRef.x = %.5f\t AlignRef.y = %.5f\t AlignAng = %.5f", 
				  m_dmtrAlignRef[GLASS1].x, m_dmtrAlignRef[GLASS1].y,
				  m_dAlignRefAngle[GLASS1]);
		
		if (!pPR1PRU->bLoaded || !pPR2PRU->bLoaded)
		{
			szMsg.Format("%s PR Pattern Not Loaded. Operation Abort!", GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", szMsg);
			bResult = FALSE;
		}
		else
		{
			//m_lCurGlass = GLASS1;

			if (
				IndexGlassToTable((GlassNum)m_lCurGlass) != GMP_SUCCESS ||
				!SetGlassRef(m_lCurGlass, TRUE) || 
				MoveAbsoluteXY(DoubleToLong(m_dmtrCurrPR1Posn.x), DoubleToLong(m_dmtrCurrPR1Posn.y), GMP_WAIT) != GMP_SUCCESS ||
				pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_PR_LEVEL, GMP_WAIT) != GMP_SUCCESS 
			   )
			{
				bResult = FALSE;
			}
			else
			{
				//if (rMSG_YES == HMIMessageBox(MSG_YES_NO, "SETUP ALIGN PT", "Auto Set Glass1 PR Posn?"))
				{
					m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR1Posn.x);
					m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR1Posn.y);
					m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR2Posn.x);
					m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR2Posn.y);
				}
				//m_lCurGlass = GLASS2;
				//SetVacSol((GlassNum)m_lCurGlass, ON, GMP_WAIT);
				//if (IsVacSensorOn((GlassNum)m_lCurGlass))
				//{
				//	if (rMSG_YES ==  HMIMessageBox(MSG_YES_NO, "SETUP ALIGN PT", "Auto Set Glass2 PR Posn?"))
				//	{
				//		if (!SetGlassRef(m_lCurGlass, TRUE))
				//		{
				//			bResult = FALSE;
				//		}
				//		else
				//		{
				//			m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR1Posn.x);
				//			m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR1Posn.y);
				//			m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR2Posn.x);
				//			m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR2Posn.y);
				//		}
				//	}
				//}
				lAnswer = HMIMessageBox(MSG_YES_NO, "SET ALIGN POINT", "Move to previously set position?");
				if (lAnswer == rMSG_YES)
				{
					bMovePrevPosn = TRUE;
				}
				else
				{
					bMovePrevPosn = FALSE;
				}
				if (bMovePrevPosn)
				{
					mtrTemp.x = (LONG)(m_dmtrCurrPR1Posn.x + pPR_INFO[m_lCurGlass]->dmtrAlignPt1.x);
					mtrTemp.y = (LONG)(m_dmtrCurrPR1Posn.y + pPR_INFO[m_lCurGlass]->dmtrAlignPt1.y * 100);
				
					if (
						MoveAbsoluteXY(mtrTemp.x, mtrTemp.y,  GMP_WAIT) != GMP_SUCCESS
						)
					{
						bResult = FALSE;
					}
				}
				else
				{
					mtrTemp.x = (LONG)m_dmtrCurrPR1Posn.x;
					mtrTemp.y = (LONG)m_dmtrCurrPR1Posn.y;
				}

				PRS_DisplayVideo(pPR1PRU);
				PRS_DrawHomeCursor(INSPDL_CAM + m_lCurGlass, FALSE);

				g_szWizardStep = "Step 1/2";
				g_szWizardMessPrev = "";
				g_szWizardMess = "1. Set Align Pt 1 Position";	
				g_szWizardMessNext = "2. Set Align Pt 2 Position";
				g_szWizardBMPath = "";
			}
			
			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
		}
		break;

	case 1:
	// 1. Set Align Pick Pt 1 Offset
	// 2. Ready for setting Align Pick Pt 2 Offset
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Operation is irreversible. Continue?");
		if (lAnswer == rMSG_TIMEOUT || lAnswer == rMSG_CANCEL)
		{
			bResult = FALSE;
		}
		else
		{
			pPR_INFO[m_lCurGlass]->bAlignPtSet = FALSE;

			pPR_INFO[m_lCurGlass]->dmtrAlignPt1.x = ((DOUBLE)pCInPickArm->m_stMotorX.GetEncPosn() - m_dmtrCurrPR1Posn.x);
			pPR_INFO[m_lCurGlass]->dmtrAlignPt1.y = ((DOUBLE)m_stMotorY.GetEncPosn() - m_dmtrCurrPR1Posn.y)/100;
			m_pCOtherWH->m_stInspWH[m_lCurGlass].dmtrAlignPt1.x = pPR_INFO[m_lCurGlass]->dmtrAlignPt1.x;
			m_pCOtherWH->m_stInspWH[m_lCurGlass].dmtrAlignPt1.y = pPR_INFO[m_lCurGlass]->dmtrAlignPt1.y;

			if (bMovePrevPosn)
			{
				mtrTemp.x = (LONG)(m_dmtrCurrPR2Posn.x + pPR_INFO[m_lCurGlass]->dmtrAlignPt2.x);
				mtrTemp.y = (LONG)(m_dmtrCurrPR2Posn.y + pPR_INFO[m_lCurGlass]->dmtrAlignPt2.y * 100);
			}
			else
			{
				mtrTemp.x = (LONG)m_dmtrCurrPR2Posn.x;
				mtrTemp.y = (LONG)m_dmtrCurrPR2Posn.y;
			}
			if ( MoveAbsoluteXY(mtrTemp.x, mtrTemp.y,  GMP_WAIT) != GMP_SUCCESS )
			{
				bResult = FALSE;
			}
			else
			{
				g_szWizardStep = "Step 2/2";
				g_szWizardMessPrev = "1. Set Align Pt 1 Position";
				g_szWizardMess = "2. Set Align Pt 2 Position";
				g_szWizardMessNext = "DONE";
				g_szWizardBMPath = "";
			}
		}

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;
		break;

	case 2:
	// 2. Set Align Pick Pt 2 position
		pPR_INFO[m_lCurGlass]->dmtrAlignPt2.x = ((DOUBLE)pCInPickArm->m_stMotorX.GetEncPosn() - m_dmtrCurrPR2Posn.x);
		pPR_INFO[m_lCurGlass]->dmtrAlignPt2.y = ((DOUBLE)m_stMotorY.GetEncPosn() - m_dmtrCurrPR2Posn.y)/100;
		m_pCOtherWH->m_stInspWH[m_lCurGlass].dmtrAlignPt2.x = pPR_INFO[m_lCurGlass]->dmtrAlignPt2.x;
		m_pCOtherWH->m_stInspWH[m_lCurGlass].dmtrAlignPt2.y = pPR_INFO[m_lCurGlass]->dmtrAlignPt2.y;

		CalcAlignRefPt(m_lCurGlass);
		m_pCOtherWH->CalcAlignRefPt(m_lCurGlass);

		pPR_INFO[m_lCurGlass]->bAlignPtSet = TRUE;
		m_pCOtherWH->m_stInspWH[m_lCurGlass].bAlignPtSet = TRUE;

		if(m_lCurGlass == GLASS1)
		{
			pCInspOpt->m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x = DL1ToUL(m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.x);
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x = DL1ToUL(m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.x);
			pCInspOpt->m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y = m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y + (m_mtrULRefPosn.y - m_mtrDL1RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.y * 100;
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y = m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y + (m_mtrULRefPosn.y - m_mtrDL1RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.y * 100;

			pCInspOpt->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].x = m_pCOtherWH->DL1ToUL(m_pCOtherWH->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.x);
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].x = m_pCOtherWH->DL1ToUL(m_pCOtherWH->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.x);
			pCInspOpt->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].y = m_pCOtherWH->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].y + (m_pCOtherWH->m_mtrULRefPosn.y - m_pCOtherWH->m_mtrDL1RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.y * 100;
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].y = m_pCOtherWH->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].y + (m_pCOtherWH->m_mtrULRefPosn.y - m_pCOtherWH->m_mtrDL1RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.y * 100;
		}
		else
		{
			pCInspOpt->m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x = DL2ToUL(m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.x);
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x = DL2ToUL(m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.x);
			pCInspOpt->m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y = m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y + (m_mtrULRefPosn.y - m_mtrDL2RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.y * 100;
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y = m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y + (m_mtrULRefPosn.y - m_mtrDL2RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.y * 100;

			pCInspOpt->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].x = m_pCOtherWH->DL2ToUL(m_pCOtherWH->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.x);
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].x = m_pCOtherWH->DL2ToUL(m_pCOtherWH->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].x + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.x);
			pCInspOpt->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].y = m_pCOtherWH->m_mtrPR1Posn[m_lCurSlave+1][m_lCurGlass].y + (m_pCOtherWH->m_mtrULRefPosn.y - m_pCOtherWH->m_mtrDL2RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt1.y * 100;
			pCInspOpt->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].y = m_pCOtherWH->m_mtrPR2Posn[m_lCurSlave+1][m_lCurGlass].y + (m_pCOtherWH->m_mtrULRefPosn.y - m_pCOtherWH->m_mtrDL2RefPosn.y) + (LONG)pPR_INFO[m_lCurGlass]->dmtrAlignPt2.y * 100;
		}

		//pPR_INFO[GLASS2]->bAlignPtSet = pPR_INFO[GLASS1]->bAlignPtSet;
		//pPR_INFO[GLASS2]->dmtrAlignPt1.x = pPR_INFO[GLASS1]->dmtrAlignPt1.x;
		//pPR_INFO[GLASS2]->dmtrAlignPt1.y = pPR_INFO[GLASS1]->dmtrAlignPt1.y;
		//pPR_INFO[GLASS2]->dmtrAlignPt2.x = pPR_INFO[GLASS1]->dmtrAlignPt2.x;
		//pPR_INFO[GLASS2]->dmtrAlignPt2.y = pPR_INFO[GLASS1]->dmtrAlignPt2.y;

		g_szWizardStep = "DONE";
		g_szWizardMess = "DONE";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = FALSE;
		g_bWizardEnableExit = TRUE;
		break;
	}

	// Failed?
	if (!bResult)
	{
		g_szWizardStep = "FAIL";
		g_szWizardMess = "OPERATION FAIL!";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = FALSE;
		g_bWizardEnableExit = TRUE;
	}

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");
}

// ------------------------------------------------------------------------
// Virtual WorkHolder Function delcaration
// ------------------------------------------------------------------------
BOOL CInspWH::PickGlassOperation(GlassNum lGlassNum)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");


		LONG lCurGlass;
		if (lGlassNum == GLASS1 || lGlassNum == MAX_NUM_OF_GLASS)
		{
			lCurGlass = GLASS1;
		}
		else
		{
			lCurGlass = GLASS2;
		}
		do
		{
			if (
				!pCInPickArm->PickGlassOperation() ||
				!pCInPickArm->PlaceGlassOperation()
			)
			{
				break;
			}

			lCurGlass++;

		} while (lGlassNum == MAX_NUM_OF_GLASS && lCurGlass < MAX_NUM_OF_GLASS);

	return TRUE;
}

// ------------------------------------------------------------------------
// END Virtual WorkHolder Function delcaration
// ------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CInspWH::PreStartOperation()
{
	m_bModError = FALSE;
	//m_bGlass1Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS1] instead
	//m_bGlass2Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS2] instead
	m_lRejectGlassCounter = 0;
	//m_szRejectReason[m_lCurGlass]	= "";
	
	// Reset all PR Data
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		m_stInspWH[i].bBonded = FALSE;
		if(AutoMode == BURN_IN)
		{
			m_lTInspPosn[i] = m_lStandbyPosnT;
		}
	}


	if (!m_bModSelected)
	{	
		INSPWH_Status[m_lCurSlave] = ST_MOD_NOT_SELECTED;
		m_qSubOperation = INSPWH_STOP_Q;
		m_bStopCycle = FALSE;
	}
	else
	{
		if (
			((m_nLastError = SetWeakBlowSol(OFF, GMP_WAIT)) != GMP_SUCCESS) ||
			// 20140616 Yip: Not Vacuum On During Pre Start
//			(AutoMode != BURN_IN && ((m_nLastError = SetGLASS1_VacSol(ON)) != GMP_SUCCESS)) || //20120309 problem 5
//			(AutoMode != BURN_IN && ((m_nLastError = SetGLASS2_VacSol(ON)) != GMP_SUCCESS)) ||
			((m_nLastError = MoveTToStandby(GMP_WAIT)) != GMP_SUCCESS) ||
			((m_nLastError = MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT)) != GMP_SUCCESS)
			)
		{
			Command(glSTOP_COMMAND, TRUE);
			INSPWH_Status[m_lCurSlave] = ST_STOP;
			m_qSubOperation = INSPWH_STOP_Q;
			m_bStopCycle = TRUE;
		}
		else
		{
			if (!IsGLASS1_VacSensorOn() || AutoMode == BURN_IN)
			{
				m_bGlass1Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS1] instead
				if(AutoMode == BURN_IN)
				{
					SetVacSol(GLASS1, OFF, GMP_NOWAIT);
				}
				INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_EMPTY;
				m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
				m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
			}
			else		//Glass1 Exist on WH
			{
				m_bGlass1Exist = TRUE;

				if (INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_ERROR)
				{
					if (rMSG_YES != HMIMessageBox(MSG_YES_NO, "InspWH", "Glass1 is Error Glass. Reject?"))
					{
						INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
					}
				}

				if (INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_EMPTY || INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_IDLE)
				{
					INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;		//Reset to ST_READY if the Glass_Status[] NOT matched, inspection AGAIN
					
					if(m_lCurSlave == WH_1)
					{
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						DisplayMessage("	INSPWH PreStart: INSPWH_GLASS_Status[WH1][GLASS1] NOT matched");
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					}
					else
					{
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						DisplayMessage("	INSPWH PreStart: INSPWH_GLASS_Status[WH2][GLASS1] NOT matched");
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					}
				}
			}

			if (!IsGLASS2_VacSensorOn() || AutoMode == BURN_IN)
			{
				m_bGlass2Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS2] instead
				if(AutoMode == BURN_IN)
				{
					SetVacSol(GLASS2, OFF, GMP_NOWAIT);
				}
				INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_EMPTY;
				m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
				m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
			}
			else		//Glass2 Exist on WH
			{
				m_bGlass2Exist = TRUE;
				if (INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_ERROR)
				{
					if (rMSG_YES != HMIMessageBox(MSG_YES_NO, "InspWH", "Glass2 is Error Glass. Reject?"))
					{
						INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
					}
				}
				if (INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_EMPTY || INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_IDLE)
				{
					INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_READY;		//Reset to ST_Error if the Glass_Status[] NOT matched, inspection AGAIN
					
					if(m_lCurSlave == WH_1)
					{
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						DisplayMessage("	INSPWH PreStart: INSPWH_GLASS_Status[WH1][GLASS2] NOT matched");
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					}
					else
					{
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						DisplayMessage("	INSPWH PreStart: INSPWH_GLASS_Status[WH2][GLASS2] NOT matched");
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					}

				}
			}

			//if (!HMI_bStandAlone)
			//{
			//	PI9000_Machine_Status = ST_GW_MACHINE_READY;
			//	UpdatePI9000MachineStatus();
			//	PI9000_INSPWH_Status = ST_GW_INSPWH_IDLE;
			//	UpdateINSPWHStatus();
			//}
			m_lRejectGlassCounter				= 0;

			INSPWH_Status[m_lCurSlave] = ST_IDLE;
			m_qSubOperation = INSPWH_IDLE_Q;
			m_bStopCycle = FALSE;
		}
	}

	Motion(FALSE);
	State(IDLE_Q);

	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), INSPWH_Status[m_lCurSlave], INSPWH_GLASS_Status[m_lCurSlave]);
	
	CPI9000Stn::PreStartOperation();
}

VOID CInspWH::CycleOperation()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");

	if (!m_bPRCycle && !m_bStartSearchDLnULTest)
	{
		State(STOPPING_Q);

		return;
	}

	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND)
	{
		if(m_bPRCycle)
		{
			m_bPRCycle = !m_bPRCycle;
		}
		if(m_bStartSearchDLnULTest)
		{
			m_bStartSearchDLnULTest = !m_bStartSearchDLnULTest;
		}
	}

	if (m_bPRCycle)
	{

		PRINT_DATA	stPrintData;
		PRINT_DATA	szPrintTitle;
		CString		szMsg;

		for (INT i = 0; i < MAX_PRINT_DATA; i++)
		{
			stPrintData.szTitle[i]	= "";
		}

		stPrintData.szTitle[0]	= "Time";
		stPrintData.szTitle[1]	= "PR1 X";
		stPrintData.szTitle[2]	= "PR1 Y";
		stPrintData.szTitle[3]	= "PR1 T";
		stPrintData.szTitle[4]	= "After Search PR1 X";
		stPrintData.szTitle[5]	= "After Search PR1 Y";
		stPrintData.szTitle[6]	= "After Search PR1 T";
		stPrintData.szTitle[7]	= "Pixel X";
		stPrintData.szTitle[8]	= "Pixel Y";
		stPrintData.szTitle[9]	= "Forward Rotate X";
		stPrintData.szTitle[10]	= "Forward Rotate Y";
		stPrintData.szTitle[11]	= "Forward Rotate T";
		stPrintData.szTitle[12]	= "Backward Rotate X";
		stPrintData.szTitle[13]	= "Backward Rotate Y";
		stPrintData.szTitle[14]	= "Backward Rotate T";
		stPrintData.szTitle[15]	= "Pixel X";
		stPrintData.szTitle[16]	= "Pixel Y"; 

		switch (m_lCycleTestMethod)
		{
		case 0:
			SinglePRCycleOperation(&pruINSP_DL_Calib[m_lCurGlass][m_lCurSlave], m_dCurCalibAng, m_dCurCalibX, m_dCurCalibY, &stPrintData);
			break;

		case 1:
			if (m_lCycleMoveTDirection == 0)
			{
				if (!CheckCORCentre(&pruINSP_DL_Calib[m_lCurGlass][m_lCurSlave], m_dCurCalibAng))
				{
					m_bPRCycle = FALSE;
				}
			}
			else
			{
				if (!CheckCORCentre(&pruINSP_DL_Calib[m_lCurGlass][m_lCurSlave], m_lCycleMoveTDirection * m_dCurCalibAng))
				{
					m_bPRCycle = FALSE;
				}
				// Reserve dir
				m_lCycleMoveTDirection = -1 * m_lCycleMoveTDirection;
			}

			break;
		}

		szMsg.Format("Calculation Log\\%s_PRCycleLog", GetStnName());
		pCHouseKeeping->PrintData(szMsg, stPrintData);
	}

	if (m_bStartSearchDLnULTest)
	{
		m_bStartSearchDLnULTest = SearchDLnULTest();
	}

	if (!m_bPRCycle && !m_bStartSearchDLnULTest)
	{
		State(STOPPING_Q);
	}
}

VOID CInspWH::AutoOperation()
{
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bStopCycle = TRUE;
	}

	OperationSequence();

	// cycle completed or motor error or abort requested: end operation immediately
	if (
		(m_bModSelected && m_qSubOperation == INSPWH_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	)
	{
		if (m_fHardware && m_bModSelected)
		{
			//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			if (INSPWH_Status[m_lCurSlave] != ST_VAC_ERROR)
			{
				MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
			}

			if (AutoMode == BURN_IN) //20130620
			{
				if (!IsGLASS1_VacSensorOn())
				{
					m_bGlass1Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS1] instead
					SetGLASS1_VacSol(OFF);

				}
				if (!IsGLASS2_VacSensorOn())
				{
					m_bGlass2Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS2] instead
					SetGLASS2_VacSol(OFF);
				}
			}
		}

		if (GMP_SUCCESS != m_nLastError)
		{
			m_bModError = TRUE;
		}
		INSPWH_Status[m_lCurSlave] = ST_STOP;
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

//		if (!HMI_bStandAlone)
//		{
////#ifdef GATEWAY
//			PI9000_INSPWH_Status = ST_GW_INSPWH_STOP;
//			UpdateINSPWHStatus();
//			PI9000_Machine_Status = ST_GW_MACHINE_STOP;
//			UpdatePI9000MachineStatus();
////#endif
//		}

		CPI9000Stn::StopOperation();
	}
}

/////////////////////////////////////////////////////////////////
//Operational Sequences
/////////////////////////////////////////////////////////////////
CString CInspWH::GetSequenceString()
{

	switch (m_qSubOperation)
	{
		case INSPWH_IDLE_Q:			//0
			return "INSPWH_IDLE_Q";

		case INSPWH_REQUEST_GLASS_Q:			//1
			return "INSPWH_REQUEST_GLASS_Q";

		case INSPWH_WAIT_LOAD_GLASS_COMPLETE_Q:			//2
			return "INSPWH_WAIT_LOAD_GLASS_COMPLETE_Q";

		case INSPWH_WAIT_INSPOPTIC_ACK_Q:			//3
			return "INSPWH_WAIT_INSPOPTIC_ACK_Q";

		case INSPWH_WAIT_PLACEMENT_INSP_COMPLETE_Q:			//4
			return "INSPWH_WAIT_PLACEMENT_INSP_COMPLETE_Q";

		case INSPWH_GO_UNLOAD_GLASS_Q:			//5
			return "INSPWH_GO_UNLOAD_GLASS_Q";

		case INSPWH_WAIT_UNLOAD_GLASS_COMPLETE_Q:			//6
			return "INSPWH_WAIT_UNLOAD_GLASS_COMPLETE_Q";

		case INSPWH_STOP_Q:			//99
			return "INSPWH_STOP_Q";

	}


	return "ASSASSINATE JASON";
}

VOID CInspWH::OperationSequence()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	m_nLastError = GMP_SUCCESS;
	PRINT_DATA	stPrintData;

	CString szMsg = "";

	//static clock_t clkNow = 0, clkStart = 0;
	//{
	////20120831 for updating time chart no_wait and then no sync movement
	//	CTimeChart *pCTimeChart = (CTimeChart*)m_pModule->GetStation("TimeChart");

	//	if (pCTimeChart->m_bModSelected)
	//	{
	//		m_stMotorZ[WHZ_1].IsMotionCompleted();
	//		m_stMotorZ[WHZ_2].IsMotionCompleted();
	//	}
	//}

	switch (m_qSubOperation)
	{
		case INSPWH_IDLE_Q:			//0
			if (m_bStopCycle)
			{
				m_qSubOperation = INSPWH_STOP_Q;	// go to 99
			}
			else if(!IsInspWHEmpty())
			{
				if(IsUnloadWHReady())
				{	//Ready for Unload
					m_qSubOperation = INSPWH_GO_UNLOAD_GLASS_Q;		// go to 5
				}
				else //Go for Placement Inspection
				{
					if(m_lCurMBType == MAINBOND_1)
					{
						INSPWH_Status[m_lCurSlave] = ST_INSP_MB12_REQ;
					}
					else	//MAINBOND_2
					{
						INSPWH_Status[m_lCurSlave] = ST_INSP_MB34_REQ;
					}

					//if (		
					//	((m_nLastError = MoveXY(INSPWH_XY_CTRL_GO_PI_POSN, GMP_WAIT)) == GMP_SUCCESS)
					//	)
					{
						m_qSubOperation = INSPWH_WAIT_INSPOPTIC_ACK_Q;		// go to 3
					}
				}
			}
			else	// Empty WH
			{
				if (		
					((m_nLastError = MoveTToStandby(GMP_NOWAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveXYToLoadPosn(GMP_NOWAIT)) == GMP_SUCCESS)
					)
				{
					Sleep(100);	//no wait motion, faster request
					INSPWH_Status[m_lCurSlave] = ST_READY_LOAD;
					m_qSubOperation = INSPWH_REQUEST_GLASS_Q;		// go to 1
				}
			}
			break;
	
		case INSPWH_REQUEST_GLASS_Q:			//1
			
			if (INPICKARM_Status == ST_STOP)
			{
				m_qSubOperation = INSPWH_STOP_Q;	// go to 99
			}
			else
			{
				switch(m_lCurSlave)
				{
					case WH_1:
						if(INPICKARM_Status == ST_MB1_ACK_LOAD)
						{
							INSPWH_Status[m_lCurSlave] = ST_WH1_ACK_LOAD_GLASS;
							m_qSubOperation = INSPWH_WAIT_LOAD_GLASS_COMPLETE_Q;	// go to 2
						}
					break;

					case WH_2:
						if(INPICKARM_Status == ST_MB2_ACK_LOAD)
						{
							INSPWH_Status[m_lCurSlave] = ST_WH2_ACK_LOAD_GLASS;
							m_qSubOperation = INSPWH_WAIT_LOAD_GLASS_COMPLETE_Q;	// go to 2
						}
					break;
				}
			}
			

			break;

		case INSPWH_WAIT_LOAD_GLASS_COMPLETE_Q:			//2
			
			if (INPICKARM_Status == ST_STOP || INPICKARM_Status == ST_VAC_ERROR || INPICKARM_Status == ST_ERROR)
			{
				m_qSubOperation = INSPWH_STOP_Q;	// go to 99
			}
			else if(IsLoadGlassComplete()) // Updated Glass Status inside, Updated MBType by InPickArm
			{
				INSPWH_Status[m_lCurSlave] = ST_COARSE_PR_DONE;
				m_qSubOperation = INSPWH_IDLE_Q;		// go to 0			
			}

			break;

		case INSPWH_WAIT_INSPOPTIC_ACK_Q:				//3
			
			if (m_bStopCycle)
			{
				m_qSubOperation = INSPWH_STOP_Q;	// go to 99
			}
			else 
			{
				switch(m_lCurSlave)
				{
					case WH_1:
						if(INSPOPT_Status == ST_INSP_WH1_ACK)
						{
							INSPWH_Status[m_lCurSlave] = ST_BUSY;
							m_qSubOperation = INSPWH_WAIT_PLACEMENT_INSP_COMPLETE_Q;		// go to 4
						}
					break;

					case WH_2:
						if(INSPOPT_Status == ST_INSP_WH2_ACK)
						{
							INSPWH_Status[m_lCurSlave] = ST_BUSY;
							m_qSubOperation = INSPWH_WAIT_PLACEMENT_INSP_COMPLETE_Q;		// go to 4
						}
					break;
				}
			}
			
			break;

		case INSPWH_WAIT_PLACEMENT_INSP_COMPLETE_Q:			//4

			if(INSPOPT_Status == ST_STOP)
			{
				m_qSubOperation = INSPWH_STOP_Q;		// go to 99
			}
			else if(INSPOPT_Status == ST_INSP_DONE || INSPOPT_Status == ST_ERROR)
			{
				for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
				{
					if(INSPWH_GLASS_Status[m_lCurSlave][i] == ST_ERROR)
					{
						pAppMod->m_ulGlassRejected++;
						pAppMod->m_ulTotalGlassRejected++;
						m_lRejectGlassCounter++;
						PrintRejectGlassInfo(i);
					}
				}
				INSPWH_Status[m_lCurSlave] = ST_INSP_DONE;
				m_qSubOperation = INSPWH_GO_UNLOAD_GLASS_Q;		// go to 5
			}

			break;

		case INSPWH_GO_UNLOAD_GLASS_Q:						//5

			if (		
				((m_nLastError = MoveTToStandby(GMP_WAIT)) == GMP_SUCCESS) &&
				((m_nLastError = MoveXYToUnloadPosn(GMP_WAIT)) == GMP_SUCCESS)
				)
			{
				//Sleep(100);	//no wait motion, faster request
				SetUnloadGlassStatus();
				m_qSubOperation = INSPWH_WAIT_UNLOAD_GLASS_COMPLETE_Q;		// go to 6
			}
			break;

		case INSPWH_WAIT_UNLOAD_GLASS_COMPLETE_Q:			//6
			
			if(OUTPICKARM_Status == ST_STOP || OUTPICKARM_Status == ST_ERROR || OUTPICKARM_Status == ST_VAC_ERROR )
			{
				m_qSubOperation = INSPWH_STOP_Q;		// go to 99
			}
			else if(INSPWH_Status[m_lCurSlave] == ST_EMPTY)
			{
				INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_EMPTY;
				INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_EMPTY;
				
				m_bGlass1Exist = FALSE;		
				m_bGlass2Exist = FALSE;	

				if (		
					((m_nLastError = MoveXYToLoadPosn(GMP_NOWAIT)) == GMP_SUCCESS)
					)
				{
					Sleep(100);		//no wait motion, faster request
					INSPWH_Status[m_lCurSlave] = ST_READY_LOAD;
					m_qSubOperation = INSPWH_REQUEST_GLASS_Q;		// go to 1
				}
			}

			break;

		case INSPWH_STOP_Q:			//99

			break;

	}

	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			if (INSPWH_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("INSPWH_%ld Go State: %ld", m_lCurSlave+1, m_qSubOperation);
			}
			else
			{
				szMsg.Format("INSPWH_%ld Go Stop",m_lCurSlave+1);
			}
			DisplayMessage(szMsg);
		}
		UpdateDebugSeq(GetSequenceString(), INSPWH_Status[m_lCurSlave], INSPWH_GLASS_Status[m_lCurSlave]);
		m_qPreviousSubOperation = m_qSubOperation;

		if (HMI_bDebugSeq)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				stPrintData.szTitle[i]	= "";
			}

			stPrintData.szTitle[0].Format("%ld", GetTickCount());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", INSPWH_Status[m_lCurSlave]);
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 3].Format("%ld, %ld", INSPWH_GLASS_Status[GLASS1], INSPWH_GLASS_Status[GLASS2]);	

			LogFile->log(stPrintData.szTitle);

		}
	}

}

VOID CInspWH::SetUnloadGlassStatus()
{
	if(AutoMode == INDEXING && m_bRejectAlways)
	{
		if(INSPWH_GLASS_Status[m_lCurSlave][GLASS1] != ST_EMPTY)
		{
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_ERROR;
		}
		if(INSPWH_GLASS_Status[m_lCurSlave][GLASS2] != ST_EMPTY)
		{
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_ERROR;
		}
	}
	if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_INSP_DONE &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_EMPTY
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_READY1_EMPTY2;
	}
	else if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_INSP_DONE &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_ERROR
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_READY1_ERR2;
	}
	else if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_ERROR &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_INSP_DONE
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_ERR1_READY2;
	}
	else if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_ERROR &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_EMPTY
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_ERR1_EMPTY2;
	}
	else if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_ERROR &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_ERROR
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_ERR1_ERR2;
	}
	else if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_EMPTY &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_ERROR
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_EMPTY1_ERR2;
	}
	else if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_EMPTY &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_INSP_DONE
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_EMPTY1_READY2;
	}
	else if(
		INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_INSP_DONE &&
		INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_INSP_DONE
		)
	{
		INSPWH_Status[m_lCurSlave] = ST_PICK_READY1_READY2;
	}

}

BOOL CInspWH::IsLoadGlassComplete()
{
	BOOL bResult = FALSE;

	switch(INSPWH_Status[m_lCurSlave])
	{

		case ST_PLACE_READY1_EMPTY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_EMPTY;
			bResult = TRUE;
		break;
			
		case ST_PLACE_READY1_ERR2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_ERROR;
			bResult = TRUE;
			break;
			
		case ST_PLACE_ERR1_READY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_ERROR;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_READY;
			bResult = TRUE;
			break;
			
		case ST_PLACE_ERR1_EMPTY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_ERROR;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_EMPTY;
			bResult = TRUE;
			break;
			
		case ST_PLACE_ERR1_ERR2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_ERROR;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_ERROR;
			bResult = TRUE;
			break;

		case ST_PLACE_EMPTY1_ERR2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_EMPTY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_ERROR;
			bResult = TRUE;
			break;

		case ST_PLACE_EMPTY1_READY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_EMPTY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_READY;
			bResult = TRUE;
			break;

		case ST_PLACE_READY1_READY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_READY;
			bResult = TRUE;
			break;
	
	}


	return bResult;
}


BOOL CInspWH::IsInspWHEmpty()
{
	BOOL bResult = FALSE;

	if ((INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_EMPTY) && (INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_EMPTY))		//00
	{
		m_bGlass1Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS1] instead
		m_bGlass2Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS2] instead
		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
		bResult = TRUE;
	}
		return bResult;
}

BOOL CInspWH::IsUnloadWHReady()
{
	BOOL bResult = FALSE;

	if ((INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_EMPTY) && (INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_INSP_DONE || INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_ERROR))		//01
	{
		m_bGlass1Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS1] instead
		m_bGlass2Exist = TRUE;	//using INSPWH_GLASS_Status[GLASS2] instead
		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
		bResult = TRUE;
	}
	else if ((INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_INSP_DONE || INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_ERROR) && (INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_EMPTY))		//10
	{
		m_bGlass1Exist = TRUE;	//using INSPWH_GLASS_Status[GLASS1] instead
		m_bGlass2Exist = FALSE;	//using INSPWH_GLASS_Status[GLASS2] instead
		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
		bResult = TRUE;
	}
	else if ((INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_INSP_DONE || INSPWH_GLASS_Status[m_lCurSlave][GLASS1] == ST_ERROR ) && 
		(INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_INSP_DONE || INSPWH_GLASS_Status[m_lCurSlave][GLASS2] == ST_ERROR ))		//11
	{
		m_bGlass1Exist = TRUE;	//using INSPWH_GLASS_Status[GLASS1] instead
		m_bGlass2Exist = TRUE;	//using INSPWH_GLASS_Status[GLASS2] instead
		bResult = TRUE;
	}
		return bResult;
}

VOID CInspWH::InspWHSetVacON()
{
	//addcode
	SetVacSol(TA5PlaceAction, ON);
}

BOOL CInspWH::IsTA5PlaceGlassOK()
{
	BOOL bResult = FALSE;

	if (TA5PlaceAction == GLASS1)
	{
		if (IsGLASS1_VacSensorOn())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if (TA5PlaceAction == GLASS2)
	{
		if (IsGLASS2_VacSensorOn())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if (TA5PlaceAction == MAX_NUM_OF_GLASS)
	{
		if (IsGLASS1_VacSensorOn() && IsGLASS2_VacSensorOn())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	//addcode
		return bResult;
}

VOID CInspWH::UpdateInspPRPosn(LONG lGlass)
{
	//PR_RCOORD  rcoPRPosn;
	D_MTR_POSN dmtrPosn1, dmtrPosn2;
//#ifdef ENCODER_BASED
//	D_MTR_POSN dmtrPosn1Before, dmtrPosn2Before;
//	D_MTR_POSN dmtrPosn1After, dmtrPosn2After;
//#endif
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	if (m_bPRSelected)
	{
		if (!pruDLPR1[lGlass].bLoaded || !pruDLPR2[lGlass].bLoaded)
		{
			HMIMessageBox(MSG_OK, "WARNING", "PR Pattern Not Loaded. Operation Abort!");

			for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
			{
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].x = 0.0;
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].y = 0.0;
			}

		}
		else if (!m_stInspWH[lGlass].bAlignPtSet)
		{
			HMIMessageBox(MSG_OK, "WARNING", "Align Pts Not Learned. Operation Abort!");

			for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
			{
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].x = 0.0;
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].y = 0.0;
			}

		}
		else
		{

			CalcAlignPt((GlassNum)lGlass);
//#ifdef ENCODER_BASED
//			D_MTR_POSN dmtrPR1OffsetBefore = {0.0, 0.0}, dmtrPR2OffsetBefore = {0.0, 0.0};
//			D_MTR_POSN dmtrPR1OffsetAfter = {0.0, 0.0}, dmtrPR2OffsetAfter = {0.0, 0.0};
//#endif
			if(m_bMoveT)
			{
				MTR_POSN mtrTemp1, mtrTemp;
				if( lGlass == GLASS1 )
				{
					m_lCORCamNum = INSPDL1_COR;
				}
				else
				{
					m_lCORCamNum = INSPDL2_COR;
				}

				LONG m_lTOffset = DistanceToCount(m_dCurrAlignAngle, m_stMotorT.stAttrib.dDistPerCount);
				m_stMotorT.MoveRelative(m_lTOffset, GMP_NOWAIT);
				m_lTInspPosn[lGlass] = m_lStandbyPosnT + m_lTOffset;

				mtrTemp1.x = DoubleToLong(m_dmtrCurrPR1Posn.x + m_dmtrCurrAlign1Offset.x);
				mtrTemp1.y = DoubleToLong(m_dmtrCurrPR1Posn.y + m_dmtrCurrAlign1Offset.y);
				mtrTemp1.y = FirstEncToSecondEnc(mtrTemp1.y);

				CalPosnAfterRotate(mtrTemp1, -m_dCurrAlignAngle, &mtrTemp);
				mtrTemp.y = SecondEncToFirstEnc(mtrTemp.y);
				dmtrPosn1.x = (mtrTemp.x - m_stInspWH[lGlass].dmtrAlignPt1.x - m_mtrPR1Posn[m_lCurSlave][lGlass].x) * 100;
				dmtrPosn1.y = mtrTemp.y - m_stInspWH[lGlass].dmtrAlignPt1.y * 100 - m_mtrPR1Posn[m_lCurSlave][lGlass].y;
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][lGlass]= dmtrPosn1;
				//PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn1, &rcoPRPosn);
				//PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][lGlass]);

//#ifdef ENCODER_BASED
//	//1Before
//				dmtrPosn1Before.x = m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt1.x + m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR1Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn1Before.y = m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt1.y + m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR1Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn1Before, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR1OffsetBefore); //dmtrPR1OffsetBefore
//	//1After
//				dmtrPosn1After.x = m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt1.x + m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR1Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn1After.y = m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt1.y + m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR1Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn1After, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR1OffsetAfter);
//#endif

				mtrTemp1.x = DoubleToLong(m_dmtrCurrPR2Posn.x + m_dmtrCurrAlign2Offset.x);
				mtrTemp1.y = DoubleToLong(m_dmtrCurrPR2Posn.y + m_dmtrCurrAlign2Offset.y);
				mtrTemp1.y = FirstEncToSecondEnc(mtrTemp1.y);
				CalPosnAfterRotate(mtrTemp1, -m_dCurrAlignAngle, &mtrTemp);
				mtrTemp.y = SecondEncToFirstEnc(mtrTemp.y);

				dmtrPosn2.x = (mtrTemp.x - m_stInspWH[lGlass].dmtrAlignPt2.x - m_mtrPR2Posn[m_lCurSlave][lGlass].x) * 100;
				dmtrPosn2.y = mtrTemp.y - m_stInspWH[lGlass].dmtrAlignPt2.y * 100 - m_mtrPR2Posn[m_lCurSlave][lGlass].y;	
				pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass]= dmtrPosn2;		
				//PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn2, &rcoPRPosn);
				//PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass]);

//#ifdef ENCODER_BASED
//	//2Before
//				dmtrPosn2Before.x = m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt2.x + m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR2Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn2Before.y = m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt2.y + m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR2Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn2Before, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR2OffsetBefore);
//	//2After
//				dmtrPosn2After.x = m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt2.x + m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR2Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn2After.y = m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt2.y + m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR2Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn2After, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR2OffsetAfter);
//#endif
			}
			else
			{
//#ifdef ENCODER_BASED
//	//1Before
//				dmtrPosn1Before.x = m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt1.x + m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR1Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn1Before.y = m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt1.y + m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR1Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn1Before, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR1OffsetBefore); //dmtrPR1OffsetBefore
//	//1After
//				dmtrPosn1After.x = m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt1.x + m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR1Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn1After.y = m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt1.y + m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR1Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn1After, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR1OffsetAfter);
//	//2Before
//				dmtrPosn2Before.x = m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt2.x + m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR2Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn2Before.y = m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt2.y + m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR2Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn2Before, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR2OffsetBefore);
//	//2After
//				dmtrPosn2After.x = m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].x - m_stInspWH[lGlass].dmtrAlignPt2.x + m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x - m_mtrPR2Posn[m_lCurSlave][lGlass].x;
//				dmtrPosn2After.y = m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].y - m_stInspWH[lGlass].dmtrAlignPt2.y + m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y - m_mtrPR2Posn[m_lCurSlave][lGlass].y;
//				PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn2After, &rcoPRPosn);
//				PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &dmtrPR2OffsetAfter);
//#endif
				dmtrPosn1.x = (m_dmtrCurrAlign1Offset.x - m_stInspWH[lGlass].dmtrAlignPt1.x + m_dmtrCurrPR1Posn.x - m_mtrPR1Posn[m_lCurSlave][lGlass].x) * 100;
				dmtrPosn1.y = m_dmtrCurrAlign1Offset.y - m_stInspWH[lGlass].dmtrAlignPt1.y * 100 + m_dmtrCurrPR1Posn.y - m_mtrPR1Posn[m_lCurSlave][lGlass].y;
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][lGlass]= dmtrPosn1;
				//PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn1, &rcoPRPosn);
				//PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][lGlass]);
				//pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][m_lCurGlass].x = -pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][m_lCurGlass].x;
				//pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][m_lCurGlass].y = -pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][m_lCurGlass].y;

				dmtrPosn2.x = (m_dmtrCurrAlign2Offset.x - m_stInspWH[lGlass].dmtrAlignPt2.x + m_dmtrCurrPR2Posn.x - m_mtrPR2Posn[m_lCurSlave][lGlass].x) * 100;
				dmtrPosn2.y = m_dmtrCurrAlign2Offset.y - m_stInspWH[lGlass].dmtrAlignPt2.y * 100 + m_dmtrCurrPR2Posn.y - m_mtrPR2Posn[m_lCurSlave][lGlass].y;
				pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass]= dmtrPosn2;		
				//PRS_MtrOffsetToPRPos(&pruINSPWH_UL_Calib[m_lCurSlave], dmtrPosn2, &rcoPRPosn);
				//PRS_PRPosToDMtrOffset(&pruINSPWH_UL_Calib[m_lCurSlave], rcoPRPosn, &pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass]);
				//pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass].x = -pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass].x;
				//pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass].y = -pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass].y;
			}

			if (HMI_bDebugCal)
			{
				CString str;
				
				str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPWH%ld_Glass%ld_Search_DL_and_UL.log", m_lCurSlave + 1, lGlass + 1);
				
				FILE *fp = fopen(str, "a+");

				if (fp != NULL)
				{
					CString szTime = _T(""), szFileName = _T("");
					clock_t clkNow;

					clkNow = clock();
					SYSTEMTIME ltime;		// New Command

					// Get time as 64-bit integer.
					GetLocalTime(&ltime);
					szTime.Format("%u-%u-%u %u:%u:%u", 
						ltime.wYear, ltime.wMonth, ltime.wDay, 
						ltime.wHour, ltime.wMinute, ltime.wSecond);
					fseek(fp, 0, SEEK_END);
					if (ftell(fp) < 10)
					{
//#ifdef ENCODER_BASED
//						fprintf(fp, "Time\tDLGlass_PR1x(Pix)\tDLGlass_PR1y(Pix)\tDLGlass_PR2x(Pix)\tDLGlass_PR2y(Pix)\tDL_AlignPt1_Offset_x(CmdMtr)\tDL_AlignPt1_Offset_y(CmdMtr)\tDL_AlignPt2_Offset_x(CmdMtr)\tDL_AlignPt2_Offset_y(CmdMtr)\tUL_PR1_POSN_OFFSET_x(CmdMtr)\tUL_PR1_POSN_OFFSET_y(CmdMtr)\tUL_PR2_POSN_OFFSET_x(CmdMtr)\tUL_PR2_POSN_OFFSET_y(CmdMtr)\tDL_AlignPt1_Offset_x(BeforeEncMtr)\tDL_AlignPt1_Offset_y(BeforeEncMtr)\tDL_AlignPt2_Offset_x(BeforeEncMtr)\tDL_AlignPt2_Offset_y(BeforeEncMtr)\tUL_PR1_POSN_OFFSET_x(BeforeEncMtr)\tUL_PR1_POSN_OFFSET_y(BeforeEncMtr)\tUL_PR2_POSN_OFFSET_x(BeforeEncMtr)\tUL_PR2_POSN_OFFSET_y(BeforeEncMtr)\tDL_AlignPt1_Offset_x(AfterEncMtr)\tDL_AlignPt1_Offset_y(AfterEncMtr)\tDL_AlignPt2_Offset_x(AfterEncMtr)\tDL_AlignPt2_Offset_y(AfterEncMtr)\tUL_PR1_POSN_OFFSET_x(AfterEncMtr)\tUL_PR1_POSN_OFFSET_y(AfterEncMtr)\tUL_PR2_POSN_OFFSET_x(AfterEncMtr)\tUL_PR2_POSN_OFFSET_y(AfterEncMtr)\tGlassID\tCOF_ID\n");
//#else
						fprintf(fp, "Time\tDLGlass_PR1x(Pix)\tDLGlass_PR1y(Pix)\tDLGlass_PR2x(Pix)\tDLGlass_PR2y(Pix)\tDL_AlignPt1_Offset_x(Motor)\tDL_AlignPt1_Offset_y(Motor)\tDL_AlignPt2_Offset_x(Motor)\tDL_AlignPt2_Offset_y(Motor)\tUL_PR1_POSN_OFFSET_x(Motor)\tUL_PR1_POSN_OFFSET_y(Motor)\tUL_PR2_POSN_OFFSET_x(Motor)\tUL_PR2_POSN_OFFSET_y(Motor)\tGlassID\tCOF_ID\tT offset\n");
//#endif
					}

					fprintf(fp, "%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%s\t%s\t%ld\n",
						(const char*)szTime,
						pruDLPR1[lGlass].rcoDieCentre.x, pruDLPR1[lGlass].rcoDieCentre.y, 
						pruDLPR2[lGlass].rcoDieCentre.x, pruDLPR2[lGlass].rcoDieCentre.y, 
						//dmtrPosn1.x, dmtrPosn1.y,
						//dmtrPosn2.x, dmtrPosn2.y,
						pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][lGlass].x, pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][lGlass].y, 
						pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass].x, pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][lGlass].y,
//#ifdef ENCODER_BASED
//						dmtrPosn1Before.x, dmtrPosn1Before.y,
//						dmtrPosn2Before.x, dmtrPosn2Before.y,
//						dmtrPR1OffsetBefore.x, dmtrPR1OffsetBefore.y, 
//						dmtrPR2OffsetBefore.x, dmtrPR2OffsetBefore.y,
//
//						dmtrPosn1After.x, dmtrPosn1After.y,
//						dmtrPosn2After.x, dmtrPosn2After.y,
//						dmtrPR1OffsetAfter.x, dmtrPR1OffsetAfter.y, 
//						dmtrPR2OffsetAfter.x, dmtrPR2OffsetAfter.y,
//#endif
						m_stDeviceID.szGlass_ID[lGlass].GetString(), m_stDeviceID.szCOF_ID[lGlass].GetString(),
						m_lTOffset
						);

						fclose(fp);
				}

			}
		}
	}
	else
	{
		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
			{
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].x = 0;
				pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].y = 0;
				pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][i].x = 0;
				pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][i].y = 0;
			}
	}
	
}

#ifdef GATEWAY
//VOID CInspWH::UpdateINSPWHStatus()
//{
//
//	IPC_CServiceMessage svMsg;
//	INT nConvID = 0;
//	LONG lStatus = 0;
//
//	lStatus = (LONG) PI9000_INSPWH_Status;
//	svMsg.InitMessage(sizeof(LONG), &lStatus);
//
//	try
//	{
//		nConvID = m_comClient.SendRequest("HK_COF902GW", "HK_UpdateINSPWHStatus", svMsg, 0);
//	}
//	catch (CAsmException e)
//	{
//		CString szMsg;
//		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
//		szMsg.Format("Func:%s", __FUNCTION__);
//		DisplayMessage(szMsg);
//		DisplayException(e);
//		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
//
//		szMsg.Format("TA5 Gateway Send Data Fail. Pls check gateway connection Setting.");
//		DisplayMessage(szMsg);
//		HMIMessageBox(MSG_OK, "Network Update INSPWH", szMsg);
//
//		return;
//	}
//
//	return;
//}

//VOID CInspWH::UpdatePI9000MachineStatus()
//{
//
//	IPC_CServiceMessage svMsg;
//	INT nConvID = 0;
//	LONG lStatus = 0;
//
//	lStatus = (LONG) PI9000_Machine_Status;
//	svMsg.InitMessage(sizeof(LONG), &lStatus);
//
//	try
//	{
//		nConvID = m_comClient.SendRequest("HK_COF902GW", "HK_UpdateMachineStatus", svMsg, 0);
//	}
//	catch (CAsmException e) //20140609
//	{
//		COF902_Machine_Status = ST_GW_MACHINE_STOP;
//		CString szMsg;
//		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
//		szMsg.Format("Func:%s", __FUNCTION__);
//		DisplayMessage(szMsg);
//		DisplayException(e);
//		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
//
//		szMsg.Format("Gateway Send Data Fail. Pls check gateway connection Setting.");
//		DisplayMessage(szMsg);
//		HMIMessageBox(MSG_OK, "Network Machine Status", szMsg);
//
//		return;
//	}
//
//	DisplayMessage("Machine Ready");
//
//	return;
//}

LONG CInspWH::HMI_SetStandAlone(IPC_CServiceMessage &svMsg)
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
#endif

//////////////////////////////////////
// HandShaking with COF902 
//////////////////////////////////////
BOOL CInspWH::IsTA5CanelReq()
{
	BOOL bResult = FALSE;


	//addcode
		return bResult;
}

BOOL CInspWH::IsTA5PlaceMBUnitReq()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	BOOL bResult = FALSE;

	//addcode, edit the GateWay String below
	//two string come from TA5: TA5HS_MBUnitString , TA5PlaceActionString

	//if (TA5HS_MBUnitString == GW_MBUnit1_REQ)		//GateWay String
	//{
	//	m_lCurMBHead = MAINBOND_1;	//MB12
	//	bResult = TRUE;
	//}
	//else if (TA5HandShakeString == GW_MBUnit2_REQ)
	//{
	//	m_lCurMBHead = MAINBOND_2;	//MB34
	//	bResult = TRUE;
	//}
	if (
		COF902_OUTPICKARM_Status != ST_GW_TA5_STOP &&
		COF902_OUTPICKARM_Status != ST_GW_TA5_CHECK_GLASS_EXIST_REQ
		)
	{
		CString szMsg("");
		szMsg.Format("Target Glass Status = %ld", (LONG)COF902_OUTPICKARM_Status);
		DisplayMessage(szMsg);
	}

	if (HMI_bStandAlone)
	{
		COF902_OUTPICKARM_Status = ST_GW_TA5_PLACE_MB12_G1READY_G2READY_REQ;
	}

	switch (COF902_OUTPICKARM_Status)	//define Left and/or Right Action
	{
		//case ST_PLACE_EMPTY1_EMPTY2:
		//		TA5PlaceAction = NO_GLASS;
		//	break;

	case	ST_GW_TA5_PLACE_MB12_G1READY_G2EMPTY_REQ:
		TA5PlaceActionString = ST_PLACE_READY1_EMPTY2;
		TA5PlaceAction = GLASS1;	//left only
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
#endif
		break;

	case ST_GW_TA5_PLACE_MB12_G1ERR_G2EMPTY_REQ:
		TA5PlaceActionString = ST_PLACE_ERR1_EMPTY2;
		TA5PlaceAction = GLASS1;	//left only
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1READY_G2EMPTY_REQ:
		TA5PlaceActionString = ST_PLACE_READY1_EMPTY2;
		TA5PlaceAction = GLASS1;	//left only
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1ERR_G2EMPTY_REQ:
		TA5PlaceActionString = ST_PLACE_ERR1_EMPTY2;
		TA5PlaceAction = GLASS1;	//left only
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
#endif
		break;

	case ST_GW_TA5_PLACE_MB12_G1EMPTY_G2READY_REQ:
		TA5PlaceActionString = ST_PLACE_EMPTY1_READY2;
		TA5PlaceAction = GLASS2;	//Right Only
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB12_G1EMPTY_G2ERR_REQ:
		TA5PlaceActionString = ST_PLACE_EMPTY1_ERR2;
		TA5PlaceAction = GLASS2;	//Right Only
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1EMPTY_G2READY_REQ:
		TA5PlaceActionString = ST_PLACE_EMPTY1_READY2;
		TA5PlaceAction = GLASS2;	//Right Only
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1EMPTY_G2ERR_REQ:
		TA5PlaceActionString = ST_PLACE_EMPTY1_ERR2;
		TA5PlaceAction = GLASS2;	//Right Only
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
		m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB12_G1READY_G2READY_REQ:
		TA5PlaceActionString = ST_PLACE_READY1_READY2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;	//both Glass
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB12_G1ERR_G2READY_REQ:
		TA5PlaceActionString = ST_PLACE_ERR1_READY2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB12_G1READY_G2ERR_REQ:
		TA5PlaceActionString = ST_PLACE_READY1_ERR2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB12_G1ERR_G2ERR_REQ:
		TA5PlaceActionString = ST_PLACE_ERR1_ERR2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
		m_lCurMBHead = MAINBOND_1;	//MB12
		m_lCurMBType = m_lCurMBHead;	//MB12
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1READY_G2READY_REQ:
		TA5PlaceActionString = ST_PLACE_READY1_READY2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1ERR_G2READY_REQ:
		TA5PlaceActionString = ST_PLACE_ERR1_READY2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1READY_G2ERR_REQ:
		TA5PlaceActionString = ST_PLACE_READY1_ERR2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	case ST_GW_TA5_PLACE_MB34_G1ERR_G2ERR_REQ:
		TA5PlaceActionString = ST_PLACE_ERR1_ERR2;
		TA5PlaceAction = MAX_NUM_OF_GLASS;  //both Glass
		m_lCurMBHead = MAINBOND_2;	//MB34
		m_lCurMBType = m_lCurMBHead;	//MB34
		bResult = TRUE;
#if 1 //20170406
		m_stDeviceID.szCOF_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS1];
		m_stDeviceID.szGlass_ID[GLASS1] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS1];

		m_stDeviceID.szCOF_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szCOF_ID[GLASS2];
		m_stDeviceID.szGlass_ID[GLASS2] = pCHouseKeeping->m_stDeviceID.szGlass_ID[GLASS2];
#endif
		break;

	default:
		TA5PlaceAction = NO_GLASS;  
		bResult = FALSE;
		break;
	}

	return bResult;
}


//VOID CInspWH::InspWHGrantPermissionToTA5()
//{
//	//addcode
//	PI9000_INSPWH_Status = ST_GW_INSPWH_VAC_OFF_REQ; //ask TA5 vac off
//	UpdateINSPWHStatus();
//}

BOOL CInspWH::IsTA5CheckGlassExistReq()
{
	if (COF902_OUTPICKARM_Status == ST_GW_TA5_CHECK_GLASS_EXIST_REQ)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID CInspWH::InspWHVacOffACK()
{
	//addcode
}

BOOL CInspWH::IsTA5PlaceGlassComplete()
{
	BOOL bResult = FALSE;


	//addcode
		return bResult;
}

VOID CInspWH::InspWHLoadGlassDone()
{
	//addcode
}

VOID CInspWH::UpdateInspWHGlassStatus()
{
	//Dont care, Glass1 PlaceError || Glass2 Place Error
	//update the INSPWH_GLASS_Status[], if Error goto STOP,
	//Operator removed ERROR Glass, re-init INSPWH_GLASS_Status[] in IDLE state.

	switch (TA5PlaceActionString)
	{
		case ST_PLACE_READY1_READY2:			
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_READY;
			break;

		case ST_PLACE_READY1_ERR2:	
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_ERROR;	
			m_szRejectReason[m_lCurGlass] = "COF Reject";
			break;

		case ST_PLACE_READY1_EMPTY2:	
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_READY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_EMPTY;			
			m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
			m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
			break;

		case ST_PLACE_EMPTY1_READY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_EMPTY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_READY;			
			m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
			m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
			break;

		case ST_PLACE_EMPTY1_ERR2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_EMPTY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_ERROR;	
			m_szRejectReason[m_lCurGlass] = "COF Reject";
			m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
			m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
			break;

		case ST_PLACE_EMPTY1_EMPTY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_EMPTY;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_EMPTY;			
			m_stDeviceID.szGlass_ID[GLASS1] = EMPTY_GLASS_STRING;
			m_stDeviceID.szCOF_ID[GLASS1] = EMPTY_GLASS_STRING;
			m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
			m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
			break;

		case ST_PLACE_ERR1_READY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_ERROR;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_READY;	
			m_szRejectReason[m_lCurGlass] = "COF Reject";
			break;

		case ST_PLACE_ERR1_ERR2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_ERROR;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_ERROR;
			m_szRejectReason[m_lCurGlass] = "COF Reject";
			break;

		case ST_PLACE_ERR1_EMPTY2:
			INSPWH_GLASS_Status[m_lCurSlave][GLASS1] = ST_ERROR;
			INSPWH_GLASS_Status[m_lCurSlave][GLASS2] = ST_EMPTY;
			m_szRejectReason[m_lCurGlass] = "COF Reject";
			m_stDeviceID.szGlass_ID[GLASS2] = EMPTY_GLASS_STRING;
			m_stDeviceID.szCOF_ID[GLASS2] = EMPTY_GLASS_STRING;
			break;
	}
}

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


LONG CInspWH::SetGlassStatus(STATUS_TYPE StationStatus, STATUS_TYPE *pGLASS_Status)
{
	switch (StationStatus)
	{
	case ST_PLACE_READY1_EMPTY2:
		if (pGLASS_Status[GLASS1] == ST_EMPTY)
		{
			pGLASS_Status[GLASS1] = ST_READY;
		}
		pGLASS_Status[GLASS2] = ST_EMPTY;
		break;

	case ST_PLACE_EMPTY1_READY2:
		pGLASS_Status[GLASS1] = ST_EMPTY;
		if (pGLASS_Status[GLASS2] == ST_EMPTY)
		{
			pGLASS_Status[GLASS2] = ST_READY;
		}
		break;

	case ST_PLACE_READY1_READY2:
		if (pGLASS_Status[GLASS1] == ST_EMPTY)
		{
			pGLASS_Status[GLASS1] = ST_READY;
		}
		if (pGLASS_Status[GLASS2] == ST_EMPTY)
		{
			pGLASS_Status[GLASS2] = ST_READY;
		}
		break;

	default:
		return GMP_FAIL;
	}

	return GMP_SUCCESS;
}


/////////////////////////////////////////////////////////////////
//Motor Related
/////////////////////////////////////////////////////////////////
INT CInspWH::MoveXYToCurPRPosn(LONG lPRU)
{
	INT nResult = GMP_FAIL;
	switch (lPRU)
	{
	// Type1 Left Pattern
	case 0:
		nResult = MoveXYToCurrPR1Posn(GMP_WAIT);
		break;

	case 1:
		nResult = MoveXYToCurrPR2Posn(GMP_WAIT);
		break;
	}

	if (nResult != GMP_SUCCESS)
	{
		return GMP_FAIL;
	}
	else 
	{
		return MoveZToPRLevel();
	}

	return GMP_FAIL;
}

//INT CInspWH::MoveZToLoadLevel(LONG lGlassNum, BOOL bWait) // ONLY ONE PICK PLACE EACH TIME
//{
//	INT nResult; 
//	if (lGlassNum == GLASS1)
//	{
//		nResult = MoveZ1(INSPWH_Z_CTRL_GO_LOAD_LEVEL, bWait);
//		return nResult;
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		nResult = MoveZ2(INSPWH_Z_CTRL_GO_LOAD_LEVEL, bWait);
//		return nResult;
//	}
//	else if (lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (
//			MoveZ1(INSPWH_Z_CTRL_GO_LOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
//			MoveZ2(INSPWH_Z_CTRL_GO_LOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
//		)
//		{
//			return GMP_FAIL;
//		}
//		if (bWait == GMP_WAIT)
//		{
//			if (
//				m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//				m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//			)
//			{
//				return GMP_FAIL;
//			}
//		}
//		return GMP_SUCCESS;
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;		
//	}
//}
//
//INT CInspWH::MoveZToPreLoadLevel(LONG lGlassNum, BOOL bWait) 
//{
//	if (lGlassNum == GLASS1)
//	{
//		return MoveZ1(INSPWH_Z_CTRL_GO_PRE_LOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		return MoveZ2(INSPWH_Z_CTRL_GO_PRE_LOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (
//			MoveZ1(INSPWH_Z_CTRL_GO_PRE_LOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
//			MoveZ2(INSPWH_Z_CTRL_GO_PRE_LOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
//		)
//		{
//			return GMP_FAIL;
//		}
//		if (bWait == GMP_WAIT)
//		{
//			if (
//				m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//				m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//			)
//			{
//				return GMP_FAIL;
//			}
//		}
//		return GMP_SUCCESS;
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;		
//	}
//}
//
//INT CInspWH::MoveZToUnloadLevel(LONG lGlassNum, BOOL bWait) // ONLY ONE PICK PLACE EACH TIME
//{
//	if (lGlassNum == GLASS1)
//	{
//		return MoveZ1(INSPWH_Z_CTRL_GO_UNLOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		return MoveZ2(INSPWH_Z_CTRL_GO_UNLOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (
//			MoveZ1(INSPWH_Z_CTRL_GO_UNLOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
//			MoveZ2(INSPWH_Z_CTRL_GO_UNLOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
//		)
//		{
//			return GMP_FAIL;
//		}
//		if (bWait == GMP_WAIT)
//		{
//			if (
//				m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//				m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//			)
//			{
//				return GMP_FAIL;
//			}
//		}
//		return GMP_SUCCESS;
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;		
//	}
//}
//
//INT CInspWH::MoveZToRejUnloadLevel(LONG lGlassNum, BOOL bWait) // ONLY ONE PICK PLACE EACH TIME
//{
//	if (lGlassNum == GLASS1)
//	{
//		return MoveZ1(INSPWH_Z_CTRL_GO_REJ_UNLOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		return MoveZ2(INSPWH_Z_CTRL_GO_REJ_UNLOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (
//			MoveZ1(INSPWH_Z_CTRL_GO_REJ_UNLOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
//			MoveZ2(INSPWH_Z_CTRL_GO_REJ_UNLOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
//		)
//		{
//			return GMP_FAIL;
//		}
//		if (bWait == GMP_WAIT)
//		{
//			if (
//				m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//				m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//			)
//			{
//				return GMP_FAIL;
//			}
//		}
//		return GMP_SUCCESS;
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;		
//	}
//}
//
//INT CInspWH::MoveZToPreUnloadLevel(LONG lGlassNum, BOOL bWait) // ONLY ONE PICK PLACE EACH TIME
//{
//	if (lGlassNum == GLASS1)
//	{
//		return MoveZ1(INSPWH_Z_CTRL_GO_PRE_UNLOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		return MoveZ2(INSPWH_Z_CTRL_GO_PRE_UNLOAD_LEVEL, bWait);
//	}
//	else if (lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (
//			MoveZ1(INSPWH_Z_CTRL_GO_PRE_UNLOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
//			MoveZ2(INSPWH_Z_CTRL_GO_PRE_UNLOAD_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
//		)
//		{
//			return GMP_FAIL;
//		}
//		if (bWait == GMP_WAIT)
//		{
//			if (
//				m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//				m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//			)
//			{
//				return GMP_FAIL;
//			}
//		}
//		return GMP_SUCCESS;
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;		
//	}
//}
//
//INT CInspWH::MoveZToPRLevel(LONG lGlassNum, BOOL bWait) 
//{
//	if (lGlassNum == GLASS1)
//	{
//		return MoveZ1(INSPWH_Z_CTRL_GO_PR_LEVEL, bWait);
//	}
//	else if (lGlassNum == GLASS2)
//	{
//		return MoveZ2(INSPWH_Z_CTRL_GO_PR_LEVEL, bWait);
//	}
//	else if (lGlassNum == MAX_NUM_OF_GLASS)
//	{
//		if (
//			MoveZ1(INSPWH_Z_CTRL_GO_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
//			MoveZ2(INSPWH_Z_CTRL_GO_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS
//		)
//		{
//			return GMP_FAIL;
//		}
//		if (bWait == GMP_WAIT)
//		{
//			if (
//				m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//				m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//			)
//			{
//				return GMP_FAIL;
//			}
//		}
//		return GMP_SUCCESS;
//	}
//	else
//	{
//		DisplayMessage(__FUNCTION__);
//		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
//		return GMP_FAIL;		
//	}
//}




INT CInspWH::MoveXYToStandbyPosn(BOOL bWait)
{
	return MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, bWait);
}

INT CInspWH::MoveXYToLoadPosn(BOOL bWait)
{
	//return MoveXY(INSPWH_XY_CTRL_GO_LOAD_POSN, bWait);
	return MoveXY(INSPWH_XY_CTRL_GO_LOAD_OFFSET, bWait);
}

INT CInspWH::MoveXYToUnloadPosn(BOOL bWait)
{
	//return MoveXY(INSPWH_XY_CTRL_GO_UNLOAD_POSN, bWait);
	return MoveXY(INSPWH_XY_CTRL_GO_UNLOAD_OFFSET, bWait);
}

INT CInspWH::MoveXYToUnloadRejPosn(GlassNum lGlassNum, BOOL bWait)
{
	INT nResult = GMP_FAIL;
	//if (lGlassNum == GLASS1)
	//{
	//	nResult = MoveXY(INSPWH_XY_CTRL_GO_REJECT_G1_OFFSET, bWait);
	//}
	//else if (lGlassNum == GLASS2)
	//{
	//	nResult = MoveXY(INSPWH_XY_CTRL_GO_REJECT_G2_OFFSET, bWait);
	//}
	//else
	//{
	//	DisplayMessage(__FUNCTION__);
	//	SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
	//	return GMP_FAIL;
	//}
	return nResult;
}

INT CInspWH::MoveT(INSPWH_T_CTRL_SIGNAL ctrl, BOOL bWait)
{
	LONG	lMtrCnt	= 0;
	switch (ctrl)
	{
	case INSPWH_T_CTRL_GO_STANDBY:
		
		m_stMotorT.stMotionProfile.sMotionProfID = SLOW_MOT_PROF;
		lMtrCnt = m_lStandbyPosnT;
		break;
	}

	INT nResult = m_stMotorT.MoveAbsolute(lMtrCnt, bWait);
	m_stMotorT.stMotionProfile.sMotionProfID = NORMAL_MOT_PROF;

	return nResult;
}

INT CInspWH::MoveRelativeT(LONG lMtrCnt, BOOL bWait)
{
	LONG lMotorEncCnt = 0;

	LONG lRotaryEncCnt = 0;


	return m_stMotorT.MoveRelative(lMtrCnt, bWait);
}

INT CInspWH::MoveTToStandby(BOOL bWait)
{
	return MoveT(INSPWH_T_CTRL_GO_STANDBY, bWait);
}

INT CInspWH::MoveXY(INSPWH_XY_CTRL_SIGNAL ctrl, BOOL bWait)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	MTR_POSN	stMove;

	stMove.x	= 0;
	stMove.y	= 0;

	if (pAppMod->State() == IDLE_Q)
	{
		if (!m_bModSelected)
		{
			HMIMessageBox(MSG_OK, "WARNING", "InspWH Module Not Selected. Operation Abort");
			return GMP_FAIL;
		}
	}
	switch (ctrl)
	{
	case INSPWH_XY_CTRL_GO_STANDBY_POSN:
		//stMove.x = m_mtrStandbyPosn.x;
		stMove.y = m_mtrStandbyPosn.y;
		break;

	case INSPWH_XY_CTRL_GO_LOAD_POSN:
		//stMove.x = m_mtrLoadGlassPosn.x;
		stMove.y = m_mtrLoadGlassPosn.y;
		break;

	case INSPWH_XY_CTRL_GO_LOAD_OFFSET: 
		//stMove.x = m_mtrLoadGlassPosn.x + m_mtrLoadGlassOffset.x; 
		stMove.y = m_mtrLoadGlassPosn.y + m_mtrLoadGlassOffset.y;
		break;

	case INSPWH_XY_CTRL_GO_UNLOAD_POSN:
		//stMove.x = m_mtrUnloadGlassPosn.x;
		stMove.y = m_mtrUnloadGlassPosn.y;		
		break;

	case INSPWH_XY_CTRL_GO_UNLOAD_OFFSET:
		//stMove.x = m_mtrUnloadGlassPosn.x + m_mtrUnloadGlassOffset.x;
		stMove.y = m_mtrUnloadGlassPosn.y + m_mtrUnloadGlassOffset.y;		
		break;

	case INSPWH_XY_CTRL_GO_PI_POSN:
		//stMove.x = m_mtrUnloadGlassPosn.x + m_mtrUnloadGlassOffset.x;
		stMove.y = pCInspOpt->m_mtrPR1Posn[m_lCurSlave][GLASS1].y;		
		break;

	}

	return MoveAbsoluteY(stMove.y, bWait);
}

//INT CInspWH::MoveZ1(INSPWH_Z_CTRL_SIGNAL ctrl, BOOL bWait)
//{
//	return MoveZ(WHZ_1, ctrl, bWait);
//}
//
//INT CInspWH::MoveZ2(INSPWH_Z_CTRL_SIGNAL ctrl, BOOL bWait)
//{
//	return MoveZ(WHZ_2, ctrl, bWait);
//}

//INT CInspWH::MoveZ(WHZNum lWHZNum, INSPWH_Z_CTRL_SIGNAL ctrl, BOOL bWait)
//{
//
//	DOUBLE	dThickness	= 0.0;
//	LONG	lOffset		= 0;
//	LONG	lMtrCnt		= 0;
//	BOOL	bResult		= TRUE;
//	DOUBLE	dDistPerCnt = 0.0;
//
//
//	if (lWHZNum <= WHZ_NONE || lWHZNum >= MAX_NUM_OF_WHZ)
//	{
//		return GMP_FAIL;
//	}
//
//	//dDistPerCnt	= m_stMotorZ[lWHZNum].stAttrib.dDistPerCount;
//
//	switch (ctrl)
//	{
//	case INSPWH_Z_CTRL_GO_STANDBY_LEVEL:
//		lMtrCnt = m_lStandbyLevel[lWHZNum];
//		break;
//
//	case INSPWH_Z_CTRL_GO_CALIB_LOAD_LEVEL:
//		lMtrCnt = m_lCalibLoadLevel[lWHZNum];
//		break;
//
//	case INSPWH_Z_CTRL_GO_CALIB_UNLOAD_LEVEL:
//		lMtrCnt = m_lCalibUnloadLevel[lWHZNum];
//		break;
//
//	case INSPWH_Z_CTRL_GO_REJ_CALIB_UNLOAD_LEVEL:
//		//lMtrCnt = m_lUnloadRejGlassLevel[lWHZNum];
//		break;
//
//	case INSPWH_Z_CTRL_GO_PR_CALIB_LEVEL:
//		lMtrCnt = m_lPRCalibLevel[lWHZNum];
//		break;
//
//	case INSPWH_Z_CTRL_GO_PRE_LOAD_LEVEL:
//		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
//		if (IsBurnInDiagBond())
//		{
//			lOffset		= DistanceToCount(FPC_GAP * GAP_THICKNESS * 1000.0, dDistPerCnt) + m_lLoadLevelOffset[lWHZNum] + 500; //+1000
//			lMtrCnt		= m_lCalibLoadLevel[lWHZNum] + lOffset;
//		}
//		else
//		{
//			lOffset		= DistanceToCount(2.0 * dThickness * 1000.0, dDistPerCnt) + m_lLoadLevelOffset[lWHZNum] + 500; //+1000
//			lMtrCnt		= m_lCalibLoadLevel[lWHZNum] + lOffset;
//
//		}
//		break;
//
//	case INSPWH_Z_CTRL_GO_LOAD_LEVEL:
//		dThickness = g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
//		if (IsBurnInDiagBond())
//		{
//			lOffset		= DistanceToCount(FPC_GAP * GAP_THICKNESS * 1000.0, dDistPerCnt) + m_lLoadLevelOffset[lWHZNum];
//			lMtrCnt		= m_lCalibLoadLevel[lWHZNum] + lOffset;
//
//		}
//		else
//		{
//			lOffset		= DistanceToCount(dThickness * 1000.0, dDistPerCnt) + m_lLoadLevelOffset[lWHZNum];
//			lMtrCnt		= m_lCalibLoadLevel[lWHZNum] + lOffset;
//
//		}
//		break;
//
//	case INSPWH_Z_CTRL_GO_PRE_UNLOAD_LEVEL:
//		dThickness	= g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
//		if (IsBurnInDiagBond())
//		{
//			lOffset		= DistanceToCount(FPC_GAP * GAP_THICKNESS * 1000.0, dDistPerCnt) + m_lUnloadLevelOffset[lWHZNum] + 15000; //20130122
//			lMtrCnt		= m_lCalibUnloadLevel[lWHZNum] + lOffset;
//
//		}
//		else
//		{
//			lOffset		= DistanceToCount(2.0 * dThickness * 1000.0, dDistPerCnt) + m_lUnloadLevelOffset[lWHZNum] + 15000; //20130122
//			lMtrCnt		= m_lCalibUnloadLevel[lWHZNum] + lOffset;
//		}
//
//		break;
//
//	case INSPWH_Z_CTRL_GO_UNLOAD_LEVEL:
//		dThickness	= g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
//		if (IsBurnInDiagBond())
//		{
//			lOffset		= DistanceToCount(FPC_GAP * GAP_THICKNESS * 1000.0, dDistPerCnt) + m_lUnloadLevelOffset[lWHZNum];
//			lMtrCnt		= m_lCalibUnloadLevel[lWHZNum] + lOffset;
//
//		}
//		else
//		{
//			lOffset		= DistanceToCount(dThickness * 1000.0, dDistPerCnt) + m_lUnloadLevelOffset[lWHZNum];
//			lMtrCnt		= m_lCalibUnloadLevel[lWHZNum] + lOffset;
//		}
//
//		break;
//
//	case INSPWH_Z_CTRL_GO_REJ_UNLOAD_LEVEL:
//		//dThickness	= g_stGlass.dThickness1 + g_stGlass.dThickness2 + g_stGlass.dThickness3 + g_stGlass.dThickness4;
//		//if (IsBurnInDiagBond())
//		//{
//		//	//lOffset		= DistanceToCount(FPC_GAP * GAP_THICKNESS * 1000.0, dDistPerCnt) + m_lUnloadRejGlassOffset[lWHZNum];
//		//	lMtrCnt		= m_lUnloadRejGlassLevel[lWHZNum] + lOffset;
//
//		//}
//		//else
//		//{
//		//	lOffset		= DistanceToCount(dThickness * 1000.0, dDistPerCnt) + m_lUnloadRejGlassOffset[lWHZNum];
//		//	lMtrCnt		= m_lUnloadRejGlassLevel[lWHZNum] + lOffset;
//		//}
//
//		break;
//
//	case INSPWH_Z_CTRL_GO_PR_LEVEL:
//		lMtrCnt		= m_lPRLevel[m_lCurSlave][m_lCurGlass];
//		break;
//	
//	case INSPWH_Z_CTRL_GO_UL_PR_LEVEL:
//		lMtrCnt		= m_lULPRLevel[m_lCurSlave];
//		break;
//
//	}
//
//	return m_stMotorZ[lWHZNum].MoveAbsolute(lMtrCnt, bWait);
//}


/////////////////////////////////////////////////////////////////
//Calc Related
/////////////////////////////////////////////////////////////////
BOOL CInspWH::CalcXYToUnloadOffset(GlassNum lGlassNum, MTR_POSN *mtrOffset, LONG *lOffsetT)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	MTR_POSN mtrTemp0;
	MTR_POSN mtrTemp1;

	CString szMsg = "";

	//m_dCurrAlignAngle = 0.0;

	if (m_bPRSelected)
	{
		if (!pruDLPR1[GLASS1].bLoaded || !pruDLPR2[GLASS1].bLoaded || !pruDLPR1[GLASS2].bLoaded || !pruDLPR2[GLASS2].bLoaded)
		{
			HMIMessageBox(MSG_OK, "WARNING", "PR Pattern Not Loaded. Operation Abort!");
			
			for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
			{
				lOffsetT[i] = 0;
				mtrOffset[i].x = 0;
				mtrOffset[i].y = 0;
			}

			return FALSE;
		}

		if (!m_stInspWH[GLASS1].bAlignPtSet || !m_stInspWH[GLASS2].bAlignPtSet)
		{
			HMIMessageBox(MSG_OK, "WARNING", "Align Pts Not Learned. Operation Abort!");
			
			for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
			{
				lOffsetT[i] = 0;
				mtrOffset[i].x = 0;
				mtrOffset[i].y = 0;
			}

			return FALSE;
		}
	}

	// must use m_lCurGlass HERE
	if (lGlassNum == GLASS1 || (lGlassNum == MAX_NUM_OF_GLASS))//!pAppMod->IsSelectEdgeCleaner())) //20130819
	{
		m_lCurGlass = GLASS1;
	}
	else if (lGlassNum == GLASS2 || (lGlassNum == MAX_NUM_OF_GLASS))//pAppMod->IsSelectEdgeCleaner()))
	{
		m_lCurGlass = GLASS2;
	}

	do
	{
		m_dCurrAlignAngle = 0.0; // moved from this function start, because m_dCurrAlignAngle = 0.0 needed before each glass start searching PR.


		if (
			(AutoSearchPR1(&pruDLPR1[m_lCurGlass]) && AutoSearchPR2(&pruDLPR2[m_lCurGlass])) ||
			(AutoSearchPR2(&pruDLPR2[m_lCurGlass]) && AutoSearchPR1(&pruDLPR1[m_lCurGlass]))  //20130819 do pr2 first for Edge cleaner
		)
		{
			if (m_bPRSelected)
			{
				CalcAlignPt((GlassNum)m_lCurGlass);

				mtrTemp0.x = DoubleToLong(m_dmtrCurrAlignCentre.x);
				mtrTemp0.y = DoubleToLong(m_dmtrCurrAlignCentre.y);

				CalPosnAfterRotate(mtrTemp0, -1 * m_dCurrAlignAngle, &mtrTemp1);
				
				mtrTemp1.x -= DoubleToLong(m_dmtrAlignRef[m_lCurGlass].x);
				mtrTemp1.y -= DoubleToLong(m_dmtrAlignRef[m_lCurGlass].y);

				mtrTemp1.x += m_mtrAlignUnloadOffset.x;
				mtrTemp1.y += m_mtrAlignUnloadOffset.y;
				
				// Any Addition Anlge to add here
				
				// Final Result as here
				lOffsetT[m_lCurGlass] = AngleToCount(-1 * m_dCurrAlignAngle, GetMotorT().stAttrib.dDistPerCount);
				mtrOffset[m_lCurGlass].x = mtrTemp1.x;
				mtrOffset[m_lCurGlass].y = mtrTemp1.y;

				//szMsg.Format("OK m_dmtrAlignRef[%ld]= -[%ld,%ld]",m_lCurGlass ,DoubleToLong(m_dmtrAlignRef[m_lCurGlass].x), DoubleToLong(m_dmtrAlignRef[m_lCurGlass].y));
				//DisplayMessage(szMsg);
				//szMsg.Format("OK m_mtrAlignUnloadOffset = +[%ld,%ld]",m_mtrAlignUnloadOffset.x, m_mtrAlignUnloadOffset.y);
				//DisplayMessage(szMsg);
				//szMsg.Format("OK mtrOffset[%ld] = [%ld,%ld]",m_lCurGlass ,mtrOffset[m_lCurGlass].x, mtrOffset[m_lCurGlass].y);
				//DisplayMessage(szMsg);
			}
			else
			{
				lOffsetT[m_lCurGlass] = 0;
				mtrOffset[m_lCurGlass].x = 0;
				mtrOffset[m_lCurGlass].y = 0;

			}
		}
		else
		{
			// continue?
			if (rMSG_CONTINUE != HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "PR Failed. Continue?"))
			{
				return FALSE;
			}

			lOffsetT[m_lCurGlass] = 0;
			mtrOffset[m_lCurGlass].x = 0;
			mtrOffset[m_lCurGlass].y = 0;

		}

			m_lCurGlass++;


	} while (lGlassNum == MAX_NUM_OF_GLASS && 
			 ((m_lCurGlass < MAX_NUM_OF_GLASS) || (m_lCurGlass >= GLASS1))///*IsSelectEdgeCleaner()*/))
			); //20130819
	
	// Reset
	m_lCurGlass = GLASS1;


	return TRUE;
}

INT CInspWH::SetGlassRef(LONG lGlass, BOOL bSetAng)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	DOUBLE dx,dy;

	m_dmtrCurrPR1Posn.x = (DOUBLE)m_mtrPR1Posn[m_lCurSlave][lGlass].x;
	m_dmtrCurrPR1Posn.y = (DOUBLE)m_mtrPR1Posn[m_lCurSlave][lGlass].y;

	if (
		(MoveAbsoluteXY((LONG)m_dmtrCurrPR1Posn.x, (LONG)m_dmtrCurrPR1Posn.y, GMP_WAIT) != GMP_SUCCESS) ||
		(MoveTToStandby(GMP_WAIT) != GMP_SUCCESS)
	)
	{
		return FALSE;
	}

	if (!SearchPRCentre(&pruDLPR1[lGlass]))
	{
		PRS_DisplayText(pruDLPR1[lGlass].emCameraNo, 1, PRS_MSG_ROW1, "PR Failed!");
		return FALSE;
	}

	PRS_DrawCross(pruDLPR1[lGlass].emCameraNo, pruDLPR1[lGlass].rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	m_dmtrCurrPR1Posn.x = (DOUBLE)pCInPickArm->m_stMotorX.GetEncPosn();
	m_dmtrCurrPR1Posn.y = (DOUBLE)m_stMotorY.GetEncPosn();

	m_dmtrCurrPR2Posn.x = (DOUBLE)m_mtrPR2Posn[m_lCurSlave][lGlass].x;
	m_dmtrCurrPR2Posn.y = (DOUBLE)m_mtrPR2Posn[m_lCurSlave][lGlass].y;

	if (MoveAbsoluteXY((LONG)m_dmtrCurrPR2Posn.x, (LONG)m_dmtrCurrPR2Posn.y, GMP_WAIT) != GMP_SUCCESS)
	{
		return FALSE;
	}

	if (!SearchPRCentre(&pruDLPR2[lGlass]))
	{
		PRS_DisplayText(pruDLPR2[lGlass].emCameraNo, 1, PRS_MSG_ROW1, "PR Failed!");
		return FALSE;
	}
	
	PRS_DrawCross(pruDLPR2[lGlass].emCameraNo, pruDLPR2[lGlass].rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
	
	m_dmtrCurrPR2Posn.x = (DOUBLE)pCInPickArm->m_stMotorX.GetEncPosn();
	m_dmtrCurrPR2Posn.y = (DOUBLE)m_stMotorY.GetEncPosn();

	if (bSetAng)
	{
		dx = 1.0 * (m_dmtrCurrPR2Posn.x - m_dmtrCurrPR1Posn.x);
		dy = 1.0 * (m_dmtrCurrPR2Posn.y - m_dmtrCurrPR1Posn.y)/100;

		if (dx != 0)
		{
			m_dGlassRefAng[lGlass] = (180.0 / PI) * atan((dy / dx));
			m_pCOtherWH->m_dGlassRefAng[lGlass] = (180.0 / PI) * atan((dy / dx));
		}
		else
		{
			m_dGlassRefAng[lGlass] = 0.0;
		}

		CString szMsg = _T("");
		szMsg.Format("Glass Reference Angle Found: %.3f", m_dGlassRefAng[lGlass]);
		WriteHMIMess(szMsg);
		// Debug
		szMsg.Format("PR1 Posn: (%.2f, %.2f), PR2 Posn: (%.2f, %.2f)", m_mtrPR1Posn[m_lCurSlave][lGlass].x, m_mtrPR1Posn[m_lCurSlave][lGlass].y, m_mtrPR2Posn[m_lCurSlave][lGlass].x, m_mtrPR2Posn[m_lCurSlave][lGlass].y);
		DisplayMessage(szMsg);
	}

	return TRUE;
}

VOID CInspWH::CalcAlignRefPt(LONG lGlass)
{
	D_MTR_POSN dmtrCurrAlign1Posn;
	D_MTR_POSN dmtrCurrAlign2Posn;
	DOUBLE dx, dy;

	// current align posn
	dmtrCurrAlign1Posn.x = m_dmtrCurrPR1Posn.x + m_stInspWH[lGlass].dmtrAlignPt1.x;
	dmtrCurrAlign1Posn.y = m_dmtrCurrPR1Posn.y + m_stInspWH[lGlass].dmtrAlignPt1.y* 100;

	dmtrCurrAlign2Posn.x = m_dmtrCurrPR2Posn.x + m_stInspWH[lGlass].dmtrAlignPt2.x;
	dmtrCurrAlign2Posn.y = m_dmtrCurrPR2Posn.y + m_stInspWH[lGlass].dmtrAlignPt2.y* 100;

	// return align centre
	m_dmtrAlignRef[lGlass].x = (dmtrCurrAlign1Posn.x + dmtrCurrAlign2Posn.x) / 2.0;
	m_dmtrAlignRef[lGlass].y = (dmtrCurrAlign1Posn.y + dmtrCurrAlign2Posn.y) / 2.0 /100;

	// return align angle
	dx = 1.0 * (dmtrCurrAlign2Posn.x - dmtrCurrAlign1Posn.x);
	dy = 1.0 * (dmtrCurrAlign2Posn.y - dmtrCurrAlign1Posn.y)/100;
	if (dx != 0)
	{
		m_dAlignRefAngle[lGlass] = (180.0 / PI) * atan((dy / dx));
	}
	else
	{
		m_dAlignRefAngle[lGlass] = 0.0;
	}

	// Auto Set Glass 2 Data
	m_dAlignRefAngle[(lGlass + 1) % MAX_NUM_OF_GLASS] = m_dAlignRefAngle[lGlass];
	m_dmtrAlignRef[(lGlass + 1) % MAX_NUM_OF_GLASS].x = m_dmtrAlignRef[lGlass].x - DistanceToCount(g_dGToGOffset * 1000, GetMotorX().stAttrib.dDistPerCount);
	m_dmtrAlignRef[(lGlass + 1) % MAX_NUM_OF_GLASS].y = m_dmtrAlignRef[lGlass].y;
}

VOID CInspWH::CalcAlignPt(GlassNum lGlass)
{
	D_MTR_POSN dmtrCurrAlign1Posn;
	D_MTR_POSN dmtrCurrAlign2Posn;
	
	DOUBLE dx, dy;
	DOUBLE dAngle, dAngleRad, dRefAngleRad;
//#ifdef ENCODER_BASED
//	DOUBLE dx_Before = 0.0, dy_Before = 0.0;
//	DOUBLE dAngle_Before = 0.0, dAngleRad_Before = 0.0;
//
//	DOUBLE dx_After = 0.0, dy_After = 0.0;
//	DOUBLE dAngle_After = 0.0, dAngleRad_After = 0.0;
//#endif	
	
	CString str;

#if 1 //klocwork fix 20121211
	if (lGlass == MAX_NUM_OF_GLASS || lGlass < GLASS1)
#else
	if (lGlass == MAX_NUM_OF_GLASS)
#endif
	{
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return;
	}

	// if PR not selected, return ref posn
	if (!m_bPRSelected || !m_stInspWH[lGlass].bAlignPtSet)
	{
		m_dCurrAlignAngle = 0.0;
		m_dCurrAlignDist = 0.0; //20160809
		m_dmtrCurrPR1Posn.x = (DOUBLE)m_mtrPR1Posn[m_lCurSlave][lGlass].x;
		m_dmtrCurrPR1Posn.y = (DOUBLE)m_mtrPR1Posn[m_lCurSlave][lGlass].y;
		m_dmtrCurrPR2Posn.x = (DOUBLE)m_mtrPR2Posn[m_lCurSlave][lGlass].x;
		m_dmtrCurrPR2Posn.y = (DOUBLE)m_mtrPR2Posn[m_lCurSlave][lGlass].y;
	}

	dx = 1.0 * (m_dmtrCurrPR2Posn.x - m_dmtrCurrPR1Posn.x);
	dy = 1.0 * (m_dmtrCurrPR2Posn.y - m_dmtrCurrPR1Posn.y)/100;
//#ifdef ENCODER_BASED
//	dRefAngleRad = (PI / 180.0) * m_dGlassRefAng[lGlass];
///////// Before
//	dx_Before = 1.0 * (m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x - m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x);
//	dy_Before = 1.0 * (m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y - m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y)/100;
//
//	if (dx_Before != 0)
//	{
//		dAngle_Before = (180.0 / PI) * atan((dy_Before / dx_Before));
//	}
//	else
//	{
//		dAngle_Before = 0;
//	}
//	dAngleRad_Before = (PI / 180.0) * dAngle_Before;
//
//	m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].x = (+m_stInspWH[lGlass].dmtrAlignPt1.x * cos(dRefAngleRad - dAngleRad_Before) 
//								+ m_stInspWH[lGlass].dmtrAlignPt1.y * sin(dRefAngleRad - dAngleRad_Before) 
//							   );
//
//	m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].y = (-m_stInspWH[lGlass].dmtrAlignPt1.x * sin(dRefAngleRad - dAngleRad_Before) 
//								+ m_stInspWH[lGlass].dmtrAlignPt1.y * cos(dRefAngleRad - dAngleRad_Before) 
//							   )* 100;
//
//	m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].x = (+m_stInspWH[lGlass].dmtrAlignPt2.x * cos(dRefAngleRad - dAngleRad_Before) 
//								+ m_stInspWH[lGlass].dmtrAlignPt2.y * sin(dRefAngleRad - dAngleRad_Before) 
//							   );
//
//	m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].y = (-m_stInspWH[lGlass].dmtrAlignPt2.x * sin(dRefAngleRad - dAngleRad_Before) 
//								+ m_stInspWH[lGlass].dmtrAlignPt2.y * cos(dRefAngleRad - dAngleRad_Before) 
//							   )* 100;
//
//	// current align posn
//	dmtrCurrAlign1Posn.x = m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x + m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].x;
//	dmtrCurrAlign1Posn.y = m_dmtrCurrPR1BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y + m_dmtrCurrAlign1OffsetBefore[lGlass][LOOK_GLASS_TYPE].y;
//	dmtrCurrAlign2Posn.x = m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].x + m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].x;
//	dmtrCurrAlign2Posn.y = m_dmtrCurrPR2BeforePRPosn[lGlass][LOOK_GLASS_TYPE].y + m_dmtrCurrAlign2OffsetBefore[lGlass][LOOK_GLASS_TYPE].y;
//
//	//// return align angle
//	//m_dCurrAlignAngle = dAngle - m_dGlassRefAng[lGlass];
//
//	//// return align distance
//	//m_dCurrAlignDist = sqrt(pow((dmtrCurrAlign2Posn.x - dmtrCurrAlign1Posn.x), 2.0) + pow((dmtrCurrAlign2Posn.y - dmtrCurrAlign1Posn.y), 2.0)); //20160809
//
//	// return align centre
//	m_dmtrCurrAlignCentreBefore[lGlass][LOOK_GLASS_TYPE].x = (dmtrCurrAlign1Posn.x + dmtrCurrAlign2Posn.x) / 2.0;
//	m_dmtrCurrAlignCentreBefore[lGlass][LOOK_GLASS_TYPE].y = (dmtrCurrAlign1Posn.y + dmtrCurrAlign2Posn.y) / 2.0;
//
///////////// After
//	dx_After = 1.0 * (m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x - m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x);
//	dy_After = 1.0 * (m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y - m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y)/100;
//
//	if (dx_After != 0)
//	{
//		dAngle_After = (180.0 / PI) * atan((dy_After / dx_After));
//	}
//	else
//	{
//		dAngle_After = 0;
//	}
//	dAngleRad_After = (PI / 180.0) * dAngle_After;
//
//	m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].x = (+m_stInspWH[lGlass].dmtrAlignPt1.x * cos(dRefAngleRad - dAngleRad_After) 
//								+ m_stInspWH[lGlass].dmtrAlignPt1.y * sin(dRefAngleRad - dAngleRad_After) 
//							   );
//
//	m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].y = (-m_stInspWH[lGlass].dmtrAlignPt1.x * sin(dRefAngleRad - dAngleRad_After) 
//								+ m_stInspWH[lGlass].dmtrAlignPt1.y * cos(dRefAngleRad - dAngleRad_After) 
//							   )* 100;
//
//	m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].x = (+m_stInspWH[lGlass].dmtrAlignPt2.x * cos(dRefAngleRad - dAngleRad_After) 
//								+ m_stInspWH[lGlass].dmtrAlignPt2.y * sin(dRefAngleRad - dAngleRad_After) 
//							   );
//
//	m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].y = (-m_stInspWH[lGlass].dmtrAlignPt2.x * sin(dRefAngleRad - dAngleRad_After) 
//								+ m_stInspWH[lGlass].dmtrAlignPt2.y * cos(dRefAngleRad - dAngleRad_After) 
//							   )* 100;
//
//	// current align posn
//	dmtrCurrAlign1Posn.x = m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x + m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].x;
//	dmtrCurrAlign1Posn.y = m_dmtrCurrPR1AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y + m_dmtrCurrAlign1OffsetAfter[lGlass][LOOK_GLASS_TYPE].y;
//	dmtrCurrAlign2Posn.x = m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].x + m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].x;
//	dmtrCurrAlign2Posn.y = m_dmtrCurrPR2AfterPRPosn[lGlass][LOOK_GLASS_TYPE].y + m_dmtrCurrAlign2OffsetAfter[lGlass][LOOK_GLASS_TYPE].y;
//
//	//// return align angle
//	//m_dCurrAlignAngle = dAngle - m_dGlassRefAng[lGlass];
//
//	//// return align distance
//	//m_dCurrAlignDist = sqrt(pow((dmtrCurrAlign2Posn.x - dmtrCurrAlign1Posn.x), 2.0) + pow((dmtrCurrAlign2Posn.y - dmtrCurrAlign1Posn.y), 2.0)); //20160809
//
//	// return align centre
//	m_dmtrCurrAlignCentreAfter[lGlass][LOOK_GLASS_TYPE].x = (dmtrCurrAlign1Posn.x + dmtrCurrAlign2Posn.x) / 2.0;
//	m_dmtrCurrAlignCentreAfter[lGlass][LOOK_GLASS_TYPE].y = (dmtrCurrAlign1Posn.y + dmtrCurrAlign2Posn.y) / 2.0;
//#endif

	if (dx != 0)
	{
		dAngle = (180.0 / PI) * atan((dy / dx));
	}
	else
	{
		dAngle = 0;
	}

	dAngleRad = (PI / 180.0) * dAngle;
	dRefAngleRad = (PI / 180.0) * m_dGlassRefAng[lGlass];

	// current align offset
	m_dmtrCurrAlign1Offset.x = (+m_stInspWH[lGlass].dmtrAlignPt1.x * cos(dRefAngleRad - dAngleRad) 
								+ m_stInspWH[lGlass].dmtrAlignPt1.y * sin(dRefAngleRad - dAngleRad) 
							   );

	m_dmtrCurrAlign1Offset.y = (-m_stInspWH[lGlass].dmtrAlignPt1.x * sin(dRefAngleRad - dAngleRad) 
								+ m_stInspWH[lGlass].dmtrAlignPt1.y * cos(dRefAngleRad - dAngleRad) 
							   )* 100;

	m_dmtrCurrAlign2Offset.x = (+m_stInspWH[lGlass].dmtrAlignPt2.x * cos(dRefAngleRad - dAngleRad) 
								+ m_stInspWH[lGlass].dmtrAlignPt2.y * sin(dRefAngleRad - dAngleRad) 
							   );

	m_dmtrCurrAlign2Offset.y = (-m_stInspWH[lGlass].dmtrAlignPt2.x * sin(dRefAngleRad - dAngleRad) 
								+ m_stInspWH[lGlass].dmtrAlignPt2.y * cos(dRefAngleRad - dAngleRad) 
							   )* 100;

	// current align posn
	dmtrCurrAlign1Posn.x = m_dmtrCurrPR1Posn.x + m_dmtrCurrAlign1Offset.x;
	dmtrCurrAlign1Posn.y = m_dmtrCurrPR1Posn.y + m_dmtrCurrAlign1Offset.y;
	dmtrCurrAlign2Posn.x = m_dmtrCurrPR2Posn.x + m_dmtrCurrAlign2Offset.x;
	dmtrCurrAlign2Posn.y = m_dmtrCurrPR2Posn.y + m_dmtrCurrAlign2Offset.y;

	// return align angle
#if 1 //20160826
	dx = 1.0 * (dmtrCurrAlign2Posn.x - dmtrCurrAlign1Posn.x);
	dy = 1.0 * (dmtrCurrAlign2Posn.y - dmtrCurrAlign1Posn.y)/100;
	if (dx != 0)
	{
		m_dCurrAlignAngle = (180.0 / PI) * atan((dy / dx));
	}
	else
	{
		m_dCurrAlignAngle = 0.0;
	}
#else
	m_dCurrAlignAngle = dAngle;// - m_dGlassRefAng;
#endif

	// return align distance
	m_dCurrAlignDist = sqrt(pow((dmtrCurrAlign2Posn.x - dmtrCurrAlign1Posn.x), 2.0) + pow((dmtrCurrAlign2Posn.y - dmtrCurrAlign1Posn.y), 2.0)); //20160809

	// return align centre
	m_dmtrCurrAlignCentre.x = (dmtrCurrAlign1Posn.x + dmtrCurrAlign2Posn.x) / 2.0;
	m_dmtrCurrAlignCentre.y = (dmtrCurrAlign1Posn.y + dmtrCurrAlign2Posn.y) / 2.0;

	if (HMI_bDebugCal)
	{
		// open debugging file
		CString str;
		MTR_POSN	mtrTemp;
		MTR_POSN	mtrAlignDelta;

		mtrTemp.x = m_mtrAlignCentre[lGlass].x;
		mtrTemp.y = m_mtrAlignCentre[lGlass].y;

		mtrTemp.x -= DoubleToLong(m_dmtrAlignRef[lGlass].x);
		mtrTemp.y -= DoubleToLong(m_dmtrAlignRef[lGlass].y);

		mtrAlignDelta.x = mtrTemp.x;
		mtrAlignDelta.y = mtrTemp.y;

		CString szTime = _T(""), szFileName = _T("");
		clock_t clkNow;

		clkNow = clock();
		SYSTEMTIME ltime;		// New Command

		// Get time as 64-bit integer.
		GetLocalTime(&ltime);
		szTime.Format("%u-%u-%u %u:%u:%u", 
			ltime.wYear, ltime.wMonth, ltime.wDay, 
			ltime.wHour, ltime.wMinute, ltime.wSecond);
		str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPWH%ld_PRPt_Glass%ld_%04u%02u%02u.log", m_lCurSlave + 1, lGlass + 1, ltime.wYear, ltime.wMonth, ltime.wDay);
		FILE *fp = fopen(str, "a+");


		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			if (ftell(fp) < 10)
			{
				fprintf(fp, "Time\tPR1x(Pix)\tPR1y(Pix)\tPR2x(Pix)\tPR2y(Pix)\tPR1x(Motor)\tPR1y(Motor)\tPR2x(Motor)\tPR2y(Motor)\tAlign1x\tAlign1y\tAlign2x\tAlign2y\tAlignAngle\tAlignDistance(Motor)\tAlignDelta_x(Motor)\tAlignDelta_y(Motor)\n");
			}

			// PR1 PR2 Enc1 Enc2 APt1 APt2 Dist Ang
			fprintf(fp, "%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.5f\t%.5f\t%.5f\t%.5f\t%.5f\t%.5f\t%.5f\t%.5f\t%.5f\t%.5f\t%ld\t%ld\n", 
					(const char*)szTime,
					pruDLPR1[lGlass].rcoDieCentre.x, pruDLPR1[lGlass].rcoDieCentre.y, 
					pruDLPR2[lGlass].rcoDieCentre.x, pruDLPR2[lGlass].rcoDieCentre.y, 
					m_dmtrCurrPR1Posn.x, m_dmtrCurrPR1Posn.y, 
					m_dmtrCurrPR2Posn.x, m_dmtrCurrPR2Posn.y, 
					dmtrCurrAlign1Posn.x, dmtrCurrAlign1Posn.y, 
					dmtrCurrAlign2Posn.x, dmtrCurrAlign2Posn.y,
					m_dCurrAlignAngle,
					m_dCurrAlignDist, //20160809
					mtrAlignDelta.x, mtrAlignDelta.y);

			fclose(fp);
		}
	}
}

/////////////////////////////////////////////////////////////////
//SETUP Related
/////////////////////////////////////////////////////////////////
VOID CInspWH::UpdateSetupStatus()
{
	INT nCurIndex = 0;

	m_stInspWH[GLASS1].bValid = TRUE;
	m_stInspWH[GLASS2].bValid = TRUE;

	if (!pruDLPR1[GLASS1].bLoaded || !pruDLPR2[GLASS1].bLoaded || !pruDLPR1[GLASS2].bLoaded || !pruDLPR2[GLASS2].bLoaded || !m_stInspWH[GLASS1].bAlignPtSet)
	{
		m_stInspWH[GLASS1].bValid = FALSE;
		m_stInspWH[GLASS2].bValid = FALSE;
	}
}

//VOID CInspWH::SelectPRU(ULONG ulPRU)
VOID CInspWH::SelectPRU(ULONG ulPRU, BOOL bIsMove) //20121221
{
	CString strMsg;
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	COutPickArm *pCOutPickArm = (COutPickArm*)m_pModule->GetStation("OutPickArm");

	if (HMI_ulCurPRU > 2)
	{
		HMI_ulCurPRU = 0;
	}
	
	m_dCurrAlignAngle = 0.0;

	if (bIsMove) //20121221
	{
		if (!m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			return;
		}
		if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			return;
		}
		pCInPickArm->MoveZToStandbyLevel(GMP_WAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		MoveTToStandby(GMP_WAIT);
		MoveXYToCurPRPosn(ulPRU);
		pCInPickArm->m_lCurInspWH = m_lCurSlave;
		pCInPickArm->MoveZToPRLevel(GMP_WAIT);
	}
	UpdateHMIPRButton();
}

VOID CInspWH::SetupWizard()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	CWinEagle *pCWinEagle = (CWinEagle*)m_pModule->GetStation("WinEagle");
	CSettingFile *pCSettingFile = (CSettingFile*)m_pModule->GetStation("SettingFile");

	PRU *pPRU;

	switch (g_ulWizardSubTitle)
	{
	case 0:
		g_szWizardTitle.Format("DL Glass Align Pt Setup Procedure");
		SetHmiVariable("SF_szWizardTitle");
		LogAction("InspWH: " + g_szWizardTitle);
		pCSettingFile->HMI_bShowIgnoreWindowBtn = FALSE; 
		SetHmiVariable("SF_bShowIgnoreWindowBtn");
		SetupAlignPt();
		break;

	case 1:
		g_szWizardTitle.Format("PR%ld Pattern Setup Procedure", HMI_ulCurPRU + 1);
		LogAction("InspWH: " + g_szWizardTitle);
		SetHmiVariable("SF_szWizardTitle");
		pPRU = &SelectCurPRU(HMI_ulCurPRU);
		if (g_ulWizardStep == 0 && 
			(pCWinEagle->HMI_ulPRAlg == PATTERN_MATCHING ||
			 pCWinEagle->HMI_ulPRAlg == EDGE_MATCHING ||
			 pCWinEagle->HMI_ulPRAlg == FIDUCIAL_FITTING ||
			 pCWinEagle->HMI_ulPRAlg == RECTANGLE_MATCHING)
		   ) //20130319 PR ignore windows
		{
			pCSettingFile->HMI_bShowIgnoreWindowBtn = TRUE;
			SetHmiVariable("SF_bShowIgnoreWindowBtn");
		}
		else //20150717
		{
			pCSettingFile->HMI_bShowIgnoreWindowBtn = FALSE;
			SetHmiVariable("SF_bShowIgnoreWindowBtn");
		}
		//SetupPRPatternPreTask(pPRU);
		pCWinEagle->SetupPRPattern(pPRU);
		SetupPRPatternPostTask();
		break;

	case 2:
		g_szWizardTitle.Format("PR%ldSrch Region Setup Procedure", HMI_ulCurPRU + 1);
		LogAction("InspWH: " + g_szWizardTitle);
		SetHmiVariable("SF_szWizardTitle");
		pPRU = &SelectCurPRU(HMI_ulCurPRU);
		pCWinEagle->SetupPRSrchArea(pPRU);
		break;

	}
}

VOID CInspWH::SetupPRPatternPreTask(PRU *pPRU)
{
	LONG lAnswer = rMSG_TIMEOUT;
	BOOL bResult = TRUE;
	
	m_dCurrAlignAngle = 0.0;

	switch (g_ulWizardStep)
	{
	case 0:
		if (m_lCurInspWH != WH_1)
		{
			HMIMessageBox(MSG_OK, "WARNING", "Please use WH1 for the setup!");
			bResult = FALSE;
		}
		else 
		{
			if (
				IndexGlassToTable((GlassNum)m_lCurGlass) != GMP_SUCCESS ||
				MoveXYToCurPRPosn(HMI_ulCurPRU) != GMP_SUCCESS ||
				MoveZToPRLevel != GMP_SUCCESS
			)
			{
				bResult = FALSE;
			}
		}
		break;

	case 1:
		if (pPRU->bLoaded)
		{
			lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Operation is irreversible. Continue?");
			
			if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT)
			{
				bResult = FALSE;
			}
		}

		//All belongs to this Edge ACF Unit's data will be reset
		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{
			m_stInspWH[i].bAlignPtSet = FALSE;
			m_pCOtherWH->m_stInspWH[i].bAlignPtSet = FALSE;
		}

		break;
	}
	if (!bResult)
	{
		g_ulWizardStep = 98; 
		g_szWizardStep = "FAIL";
		g_szWizardMess = "OPERATION FAIL!";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = FALSE;
		g_bWizardEnableExit = TRUE;

	}
}


VOID CInspWH::SetupPRPatternPostTask()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	switch (g_ulWizardStep)
	{
	case 99:
		//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;
	}
}

VOID CInspWH::ModuleSetupPreTask()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	pCInPickArm->m_bCheckMotor	= TRUE;
	pCInPickArm->m_bCheckIO		= TRUE;

	m_bCheckMotor		= TRUE;
	m_bCheckIO			= TRUE;
	
	CheckModSelected(TRUE);
	
	SetDiagnSteps(g_lDiagnSteps);

	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();
}

VOID CInspWH::ModuleSetupPostTask()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	pCInPickArm->m_bCheckMotor	= FALSE;
	pCInPickArm->m_bCheckIO		= FALSE;

	m_bCheckMotor		= FALSE;
	m_bCheckIO			= FALSE;
}

VOID CInspWH::ModulePRSetupPreTask()
{

	m_bCheckIO = TRUE;
	m_bCheckMotor = TRUE;

	HMI_bXYControl	= TRUE; //20121221
//	HMI_bZ1Control	= TRUE;
//	HMI_bZ2Control	= TRUE;

	CheckModSelected(TRUE);
		
	//SelectPRU(HMI_ulCurPRU);
	SelectPRU(HMI_ulCurPRU, FALSE); //20121221

	UpdateSetupStatus();

	SetDiagnSteps(g_lDiagnSteps);
}

VOID CInspWH::ModulePRSetupPostTask()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	m_bCheckIO			= FALSE;
	m_bCheckMotor		= FALSE;

	pCInPickArm->m_bCheckMotor	= FALSE;
	pCInPickArm->m_bCheckIO		= FALSE;

	//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
	//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
	MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);

}



BOOL CInspWH::ModuleSetupSetPosn(LONG lSetupTitle)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	LONG		lAnswer = rMSG_TIMEOUT;
	LONG		lResponse = 0;
	BOOL		bResult = TRUE;
	MTR_POSN	mtrOld;
	mtrOld.x	= 0;
	mtrOld.y	= 0;
	BOOL		bIsXY = TRUE;


	lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Position?");

	if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT)
	{
		return FALSE;
	}

	if (	g_ulUserGroup < SERVICE &&
		g_stSetupPosn[lSetupTitle].lType == MACHINE_PARA)
	{
		HMIMessageBox(MSG_OK, "WARNING", "You are not allowed to change Machine Parameters");
		return FALSE;
	}

	MTR_POSN TempPosn;
	LONG	lTempTPosn;

	lTempTPosn = m_stMotorT.GetEncPosn();
	TempPosn.x = pCInPickArm->m_stMotorX.GetEncPosn();
	TempPosn.y = m_stMotorY.GetEncPosn();

	switch (lSetupTitle)
	{
	case 0:
		mtrOld.x				= m_mtrStandbyPosn.x;
		mtrOld.y				= m_mtrStandbyPosn.y;

		m_mtrStandbyPosn.x = TempPosn.x;
		m_mtrStandbyPosn.y = TempPosn.y;
		break;

	case 1:
		mtrOld.x				= m_mtrPRCalibPosn[GLASS1].x;
		mtrOld.y				= m_mtrPRCalibPosn[GLASS1].y;

		m_mtrPRCalibPosn[GLASS1].x = TempPosn.x;
		m_mtrPRCalibPosn[GLASS1].y = TempPosn.y;
		break;

	case 2: //20131111
		mtrOld.x				= m_mtrLoadGlassPosn.x;
		mtrOld.y				= m_mtrLoadGlassPosn.y;

		m_mtrLoadGlassPosn.x = TempPosn.x;
		m_mtrLoadGlassPosn.y = TempPosn.y;

	case 3:
		mtrOld.x				= m_mtrLoadGlassOffset.x;
		mtrOld.y				= m_mtrLoadGlassOffset.y;

		m_mtrLoadGlassOffset.x = TempPosn.x - m_mtrLoadGlassPosn.x;
		m_mtrLoadGlassOffset.y = TempPosn.y - m_mtrLoadGlassPosn.y;
		break;

	case 4:
		mtrOld.x				= m_mtrUnloadGlassPosn.x;
		mtrOld.y				= m_mtrUnloadGlassPosn.y;

		m_mtrUnloadGlassPosn.x = TempPosn.x;
		m_mtrUnloadGlassPosn.y = TempPosn.y;
		break;

	case 5:
		mtrOld.x				= m_mtrUnloadGlassOffset.x;
		mtrOld.y				= m_mtrUnloadGlassOffset.y;

		m_mtrUnloadGlassOffset.x 		= TempPosn.x - m_mtrUnloadGlassPosn.x;
		m_mtrUnloadGlassOffset.y 		= TempPosn.y - m_mtrUnloadGlassPosn.y;
		break;

	case 6:
		mtrOld.x				= pCInspOpt->m_mtrStandbyPosn.x;
		mtrOld.y				= pCInspOpt->m_mtrStandbyPosn.y;

		pCInspOpt->m_mtrStandbyPosn.x		= pCInspOpt->m_stMotorX.GetEncPosn();
		pCInspOpt->m_mtrStandbyPosn.y		= m_stMotorY.GetEncPosn();
		break;

	case 7:
		mtrOld.x				= pCInspOpt->m_mtrPrePRPosn[WH_1].x;
		mtrOld.y				= pCInspOpt->m_mtrPrePRPosn[WH_1].y;

		pCInspOpt->m_mtrPrePRPosn[WH_1].x		= pCInspOpt->m_stMotorX.GetEncPosn();
		pCInspOpt->m_mtrPrePRPosn[WH_1].y		= m_stMotorY.GetEncPosn();
		break;

	case 8:
		mtrOld.x				= pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].x;
		mtrOld.y				= pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].y;

		pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].x		= pCInspOpt->m_stMotorX.GetEncPosn();
		pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].y		= TempPosn.y;
		break;

	case 9:
		mtrOld.x				= m_lStandbyPosnT;
		m_lStandbyPosnT			= lTempTPosn;
		break;

	case 10:
		mtrOld.x				= m_mtrPRCalibPosn[GLASS2].x;
		mtrOld.y				= m_mtrPRCalibPosn[GLASS2].y;

		m_mtrPRCalibPosn[GLASS2].x = TempPosn.x;
		m_mtrPRCalibPosn[GLASS2].y = TempPosn.y;
		break;

	case 11:
		mtrOld.x				= m_mtrDL1RefPosn.x;
		mtrOld.y				= m_mtrDL1RefPosn.y;

		m_mtrDL1RefPosn.x		= TempPosn.x;
		m_mtrDL1RefPosn.y		= TempPosn.y;
		break;

	case 12:
		mtrOld.x				= m_mtrDL2RefPosn.x;
		mtrOld.y				= m_mtrDL2RefPosn.y;

		m_mtrDL2RefPosn.x		= TempPosn.x;
		m_mtrDL2RefPosn.y		= TempPosn.y;
		break;

	case 13:
		mtrOld.x				= m_mtrULRefPosn.x;
		mtrOld.y				= m_mtrULRefPosn.y;

		m_mtrULRefPosn.x		= pCInspOpt->m_stMotorX.GetEncPosn();
		m_mtrULRefPosn.y		= TempPosn.y;
		break;

	case 14:
		mtrOld.x				= pCInspOpt->m_mtrPrePRPosn[WH_2].x;
		mtrOld.y				= pCInspOpt->m_mtrPrePRPosn[WH_2].y;

		pCInspOpt->m_mtrPrePRPosn[WH_2].x		= pCInspOpt->m_stMotorX.GetEncPosn();
		pCInspOpt->m_mtrPrePRPosn[WH_2].y		= m_stMotorY.GetEncPosn();
		break;

	}

	if (bIsXY)
	{
		CPI9000Stn::ModuleSetupSetPosn(lSetupTitle, mtrOld, TempPosn);
	}

	return bResult;
}

BOOL CInspWH::ModuleSetupGoPosn(LONG lSetupTitle)
{
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm");
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm");
	INT nResult = GMP_SUCCESS;

	CString szTitle = "";
	CString szMsg = "";
	szTitle.Format("GO POSN OPERATION");
	szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());

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
	if (!pCOutPickArm->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCOutPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "GO POSN OPERATION", szMsg);
		return FALSE;
	}

	if (
			pCInPickArm->MoveZToStandbyLevel(GMP_WAIT) != GMP_SUCCESS ||
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS
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
		nResult = SetupGoPRCalibPosn(GLASS1);
		break;

	case 2: 
		nResult = SetupGoLoadGlassPosn();
		break;

	case 3:
		nResult = SetupGoLoadGlassOffset();
		break;

	case 4:
		nResult = SetupGoUnloadGlassPosn();
		break;

	case 5:
		nResult = SetupGoUnloadGlassOffset();
		break;

	case 6:
		nResult = SetupGoInspOptStandbyPosn();
		break;

	case 7:
		nResult = SetupGoInspOptPrePRPosn(WH_1);
		break;

	case 8:
		nResult = SetupGoULPRCalibPosn();
		break;

	case 9:
		nResult = SetupGoTStandbyPosn();
		break;

	case 10:
		nResult = SetupGoPRCalibPosn(GLASS2);
		break;

	case 11:
		nResult = SetupGoDL1RefPosn();
		break;

	case 12:
		nResult = SetupGoDL2RefPosn();
		break;

	case 13:
		nResult = SetupGoULRefPosn();
		break;

	case 14:
		nResult = SetupGoInspOptPrePRPosn(WH_2);
		break;
	}

	if (nResult != GMP_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CInspWH::ModuleSetupSetLevel(LONG lSetupTitle)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	LONG	lAnswer							= rMSG_TIMEOUT;
	LONG	lResponse						= 0;
	BOOL	bResult							= TRUE;
	LONG	lCurWHZ							= WHZ_1;
	LONG	lOldLevel						= 0;
	LONG	lTempLevel						= 0;
	LONG	lLevel = 0;
	DOUBLE	dDistPerCnt						= 0.0;
	DOUBLE	dThickness						= 0.0;

	lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Position?");

	if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT)
	{
		return FALSE;
	}

	if (	g_ulUserGroup < SERVICE &&
		g_stSetupLevel[lSetupTitle].lType == MACHINE_PARA)
	{
		HMIMessageBox(MSG_OK, "WARNING", "You are not allowed to change Machine Parameters");
		return FALSE;
	}

		lTempLevel	= pCInspOpt->m_stMotorZ.GetEncPosn();
		dDistPerCnt	= pCInspOpt->m_stMotorZ.stAttrib.dDistPerCount;

	switch (lSetupTitle)
	{
	case 0:
		lOldLevel			= pCInspOpt->m_lStandbyLevel;
		
		pCInspOpt->m_lStandbyLevel = lTempLevel;		
		break;

	case 1:
		lOldLevel			= pCInspOpt->m_lPRCalibLevel[m_lCurSlave];
		
		pCInspOpt->m_lPRCalibLevel[m_lCurSlave] = lTempLevel;

		break;

	case 2:
		lOldLevel			= pCInspOpt->m_lULPRLevel[lCurWHZ][GLASS1];

		pCInspOpt->m_lULPRLevel[lCurWHZ][GLASS1] = lTempLevel;

		break;

	case 3:
		lOldLevel			= pCInspOpt->m_lULPRLevel[m_lCurSlave][GLASS2];

		pCInspOpt->m_lULPRLevel[m_lCurSlave][GLASS2] = lTempLevel;

		break;

	case 4:
		lOldLevel			= pCInPickArm->m_lPRCalibLevel[m_lCurSlave];

		pCInPickArm->m_lPRCalibLevel[m_lCurSlave] = pCInPickArm->m_stMotorZ.GetEncPosn();

		break;

	case 5:
		lOldLevel			= pCInPickArm->m_lPRLevel[m_lCurSlave][GLASS1];

		pCInPickArm->m_lPRLevel[m_lCurSlave][GLASS1] = pCInPickArm->m_stMotorZ.GetEncPosn();

		break;

	case 6:
		lOldLevel			= pCInPickArm->m_lPRLevel[m_lCurSlave][GLASS2];

		pCInPickArm->m_lPRLevel[m_lCurSlave][GLASS2] = pCInPickArm->m_stMotorZ.GetEncPosn();

		break;

	case 7:
		lOldLevel			= pCInPickArm->m_lDL1RefLevel[m_lCurSlave];

		pCInPickArm->m_lDL1RefLevel[m_lCurSlave] = pCInPickArm->m_stMotorZ.GetEncPosn();

		break;

	case 8:
		lOldLevel			= pCInPickArm->m_lDL2RefLevel[m_lCurSlave];

		pCInPickArm->m_lDL2RefLevel[m_lCurSlave] = pCInPickArm->m_stMotorZ.GetEncPosn();

		break;

	case 9:
		lOldLevel			= pCInspOpt->m_lULRefLevel[m_lCurSlave];

		pCInspOpt->m_lULRefLevel[m_lCurSlave] = pCInspOpt->m_stMotorZ.GetEncPosn();

		break;
	}

	CPI9000Stn::ModuleSetupSetLevel(lSetupTitle, lOldLevel, lTempLevel);

	return bResult;
}

BOOL CInspWH::ModuleSetupGoLevel(LONG lSetupTitle)
{
	INT nResult = GMP_SUCCESS;

	CString szTitle = "";
	CString szMsg = "";
	szTitle.Format("GO LEVEL OPERATION");
	szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());

	if (!m_bModSelected)
	{
		HMIMessageBox(MSG_OK, szTitle, szMsg);
		return FALSE;
	}

	switch (lSetupTitle)
	{
	case 0:
		nResult = SetupGoStandbyLevel();
		break;

	case 1:
		nResult = SetupGoPRCalibLevel();
		break;

	case 2:
		nResult = SetupGoULPRLevel(GLASS1);
		break;

	case 3:
		nResult = SetupGoULPRLevel(GLASS2);
		break;

	case 4:
		nResult = SetupGoDLPRCalibLevel();
		break;

	case 5:
		nResult = SetupGoPRLevel(GLASS1);
		break;

	case 6:
		nResult = SetupGoPRLevel(GLASS2);
		break;

	case 7:
		nResult = SetupGoDL1RefLevel();
		break;

	case 8:
		nResult = SetupGoDL2RefLevel();
		break;

	case 9:
		nResult = SetupGoULRefLevel();
		break;

	}
	if (nResult != GMP_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;

}

BOOL CInspWH::ModuleSetupSetPara(PARA stPara)
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	if (g_ulUserGroup < SERVICE &&
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
		m_stWeakBlow2Sol.SetOnDelay(stPara.lPara);
		break;

	case 3:
		m_stWeakBlowSol.SetOffDelay(stPara.lPara);
		m_stWeakBlow2Sol.SetOffDelay(stPara.lPara);
		break;

	case 4:
		m_lCalibPRDelay	= stPara.lPara;
		break;

	case 5:
		m_lPRDelay = stPara.lPara;
		break;

	case 6:
		m_lPRRetryLimit	= stPara.lPara;
		break;

	case 7:
		m_lRejectGlassLimit = stPara.lPara;
		break;

	case 8:
		pCInspOpt->m_dPlacementTolX = stPara.dPara;
		break;

	case 9:
		pCInspOpt->m_dPlacementTolY = stPara.dPara;
		break;

	case 10:
		pCInspOpt->m_dPlacementTolAng = stPara.dPara;
		break;
	}

	return CPI9000Stn::ModuleSetupSetPara(stPara);
}

VOID CInspWH::UpdateModuleSetupLevel()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	INT i = 0;
	INT j = 0;

	// Level Tab
		i = 0;	// 0 
		g_stSetupLevel[i].szLevel.Format("InspOpt Standby Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= MACHINE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInspOpt->m_lStandbyLevel;

		i++;	// 1
		g_stSetupLevel[i].szLevel.Format("InspOpt Calib. PR Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= MACHINE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInspOpt->m_lPRCalibLevel[m_lCurSlave];

		i++;	// 2
		g_stSetupLevel[i].szLevel.Format("UL G1 PR Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= DEVICE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInspOpt->m_lULPRLevel[m_lCurSlave][GLASS1];

		i++;	// 3
		g_stSetupLevel[i].szLevel.Format("UL G2 PR Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= DEVICE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInspOpt->m_lULPRLevel[m_lCurSlave][GLASS2];

		i++;	// 4
		g_stSetupLevel[i].szLevel.Format("INPA Calib. PR Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= MACHINE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInPickArm->m_lPRCalibLevel[m_lCurSlave];

		i++;	// 5
		g_stSetupLevel[i].szLevel.Format("INPA G1 PR Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= DEVICE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInPickArm->m_lPRLevel[m_lCurSlave][GLASS1];

		i++;	// 6
		g_stSetupLevel[i].szLevel.Format("INPA G2 PR Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= DEVICE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInPickArm->m_lPRLevel[m_lCurSlave][GLASS2];

		i++;	// 7
		g_stSetupLevel[i].szLevel.Format("DL1 Ref Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= MACHINE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInPickArm->m_lDL1RefLevel[m_lCurSlave];

		i++;	// 8
		g_stSetupLevel[i].szLevel.Format("DL2 Ref Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= MACHINE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInPickArm->m_lDL2RefLevel[m_lCurSlave];

		i++;	// 9
		g_stSetupLevel[i].szLevel.Format("UL Ref Level");
		g_stSetupLevel[i].bEnable		= TRUE;
		g_stSetupLevel[i].bVisible		= TRUE;
		g_stSetupLevel[i].lType			= MACHINE_PARA;
		g_stSetupLevel[i].lLevelZ		= pCInspOpt->m_lULRefLevel[m_lCurSlave];
}

VOID CInspWH::UpdateModuleSetupPosn()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	INT i = 0;
	INT j = 0;

	// Assign Module Setup Posn/Level/Para
	// Position Tab
	i = 0;
	g_stSetupPosn[i].szPosn.Format("Standby Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_mtrStandbyPosn.y;

	i++;	// 1
	g_stSetupPosn[i].szPosn.Format("DL1 Calib. Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_mtrPRCalibPosn[GLASS1].x;
	g_stSetupPosn[i].lPosnY			= m_mtrPRCalibPosn[GLASS1].y;

	i++;	// 2
	g_stSetupPosn[i].szPosn.Format("Load Glass Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_mtrLoadGlassPosn.y;

#if 1 //20131111
	i++;	// 3
	g_stSetupPosn[i].szPosn.Format("Load Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_mtrLoadGlassOffset.y;
#endif

	i++;	// 4
	g_stSetupPosn[i].szPosn.Format("Unload Glass Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_mtrUnloadGlassPosn.y;

	i++;	// 5
	g_stSetupPosn[i].szPosn.Format("Unload Glass Offset");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= DEVICE_PARA;
	g_stSetupPosn[i].lPosnX			= 0;
	g_stSetupPosn[i].lPosnY			= m_mtrUnloadGlassOffset.y;

	i++;	// 6
	g_stSetupPosn[i].szPosn.Format("InspOpt Standby Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= pCInspOpt->m_mtrStandbyPosn.x;
	g_stSetupPosn[i].lPosnY			= 0;	

	i++;	// 7
	g_stSetupPosn[i].szPosn.Format("InspOpt Pre-PR Posn (WH1)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= pCInspOpt->m_mtrPrePRPosn[WH_1].x;
	g_stSetupPosn[i].lPosnY			= 0;	

	i++;	// 8
	g_stSetupPosn[i].szPosn.Format("WH UL Calib. Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].x;
	g_stSetupPosn[i].lPosnY			= pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].y;	

	i++;	// 9
	g_stSetupPosn[i].szPosn.Format("WH T Standby Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_lStandbyPosnT;
	g_stSetupPosn[i].lPosnY			= 0;

	i++;	// 10
	g_stSetupPosn[i].szPosn.Format("DL2 Calib. Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_mtrPRCalibPosn[GLASS2].x;
	g_stSetupPosn[i].lPosnY			= m_mtrPRCalibPosn[GLASS2].y;

	i++;	// 11
	g_stSetupPosn[i].szPosn.Format("WH DL1 Ref Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_mtrDL1RefPosn.x;
	g_stSetupPosn[i].lPosnY			= m_mtrDL1RefPosn.y;

	i++;	// 12
	g_stSetupPosn[i].szPosn.Format("WH DL2 Ref Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_mtrDL2RefPosn.x;
	g_stSetupPosn[i].lPosnY			= m_mtrDL2RefPosn.y;

	i++;	// 13
	g_stSetupPosn[i].szPosn.Format("WH UL Ref Posn");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= m_mtrULRefPosn.x;
	g_stSetupPosn[i].lPosnY			= m_mtrULRefPosn.y;

	i++;	// 14
	g_stSetupPosn[i].szPosn.Format("InspOpt Pre-PR Posn (WH2)");
	g_stSetupPosn[i].bEnable		= TRUE;
	g_stSetupPosn[i].bVisible		= TRUE;
	g_stSetupPosn[i].lType			= MACHINE_PARA;
	g_stSetupPosn[i].lPosnX			= pCInspOpt->m_mtrPrePRPosn[WH_2].x;
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

//VOID CInspWH::UpdateModuleSetupLevel()
//{
//	INT i = 0;
//	INT j = 0;

	//// Level Tab
	//g_stSetupLevel[i].szLevel.Format("Standby Level");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= MACHINE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lStandbyLevel;

	//i++;	// 1
	//g_stSetupLevel[i].szLevel.Format("Calib. Load Level");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= MACHINE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lCalibLoadLevel;

	//i++;	// 2
	//g_stSetupLevel[i].szLevel.Format("Calib. Unload Level (PickArm)");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= MACHINE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lCalibUnloadLevel;

	//i++;	// 3
	//g_stSetupLevel[i].szLevel.Format("Calib. Unload Level (RejectArm)");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= MACHINE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lUnloadRejGlassLevel;
	
	//i++;	// 4
	//g_stSetupLevel[i].szLevel.Format("Calib. PR Level");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= MACHINE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lPRCalibLevel;

	//i++;	// 5
	//g_stSetupLevel[i].szLevel.Format("Load Level Offset");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= DEVICE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lLoadLevelOffset;

	//i++;	// 6
	//g_stSetupLevel[i].szLevel.Format("Unload Level Offset(PickArm)");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= DEVICE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lUnloadLevelOffset;

	//i++;	// 7
	//g_stSetupLevel[i].szLevel.Format("Unload Level Offset(RejectArm)");
	//g_stSetupLevel[i].bEnable		= TRUE;
	//g_stSetupLevel[i].bVisible		= TRUE;
	//g_stSetupLevel[i].lType			= DEVICE_PARA;
	//g_stSetupLevel[i].lLevelZ		= m_lUnloadRejGlassOffset;

//	i++;	// 8
//	g_stSetupLevel[i].szLevel.Format("PR Level");
//	g_stSetupLevel[i].bEnable		= TRUE;
//	g_stSetupLevel[i].bVisible		= TRUE;
//	g_stSetupLevel[i].lType			= DEVICE_PARA;
//	g_stSetupLevel[i].lLevelZ		= m_lPRLevel[GLASS2][m_lCurGlass];
//
//	i++;	// 9
//	g_stSetupLevel[i].szLevel.Format("UL PR Level");
//	g_stSetupLevel[i].bEnable		= TRUE;
//	g_stSetupLevel[i].bVisible		= TRUE;
//	g_stSetupLevel[i].lType			= DEVICE_PARA;
//	g_stSetupLevel[i].lLevelZ		= m_lULPRLevel[GLASS2];
//
//	for (j = i + 1; j < NUM_OF_LEVEL_Z; j++)
//	{
//		g_stSetupLevel[j].szLevel.Format("Reserved");
//		g_stSetupLevel[j].bEnable		= FALSE;
//		g_stSetupLevel[j].bVisible		= FALSE;
//		g_stSetupLevel[j].lType			= MACHINE_PARA;
//		g_stSetupLevel[j].lLevelZ		= 0;
//	}
//}

VOID CInspWH::UpdateModuleSetupPara()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
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
	g_stSetupPara[i].szPara.Format("Calib PR Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= MACHINE_PARA;
	g_stSetupPara[i].lPara			= m_lCalibPRDelay;
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 5
	g_stSetupPara[i].szPara.Format("PR Delay (ms)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_lPRDelay;
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 6
	g_stSetupPara[i].szPara.Format("PR Retry Limit");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_lPRRetryLimit;
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 7
	g_stSetupPara[i].szPara.Format("Reject Glass Limit");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= m_lRejectGlassLimit;
	g_stSetupPara[i].dPara			= 0.0;

	i++;	// 8
	g_stSetupPara[i].szPara.Format("Insp Placement X Tol. (um)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= 0;
	g_stSetupPara[i].dPara			= pCInspOpt->m_dPlacementTolX;

	i++;	// 9
	g_stSetupPara[i].szPara.Format("Insp Placement Y Tol. (um)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= 0;
	g_stSetupPara[i].dPara			= pCInspOpt->m_dPlacementTolY;

	i++;	// 10
	g_stSetupPara[i].szPara.Format("Insp Placement Angle Tol. (deg)");
	g_stSetupPara[i].bEnable		= TRUE;
	g_stSetupPara[i].bVisible		= TRUE;
	g_stSetupPara[i].lType			= DEVICE_PARA;
	g_stSetupPara[i].lPara			= 0;
	g_stSetupPara[i].dPara			= pCInspOpt->m_dPlacementTolAng;

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

INT CInspWH::SetupGoStandbyPosn()
{
	INT nResult = GMP_SUCCESS;

	if (
		//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
		//m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS ||
		MoveTToStandby(GMP_WAIT) != GMP_SUCCESS ||
		MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoLoadGlassPosn()
{
	INT nResult = GMP_SUCCESS;

	if (
		//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
		//m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS ||
		MoveXY(INSPWH_XY_CTRL_GO_LOAD_POSN, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoUnloadGlassPosn()
{
	INT nResult = GMP_SUCCESS;
	
	if (
		//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
		//m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS ||
		MoveXY(INSPWH_XY_CTRL_GO_UNLOAD_POSN, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoUnloadGlassOffset()
{

	INT nResult = GMP_SUCCESS;
	
	if (
		//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
		//m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS ||
		MoveXY(INSPWH_XY_CTRL_GO_UNLOAD_OFFSET, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoPRCalibPosn(LONG lCurGlass)
{
	INT nResult = GMP_SUCCESS;
	m_lCurGlass = lCurGlass;
	if (
		//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
		//m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS ||
		MoveXYToPRCalibPosn(GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoULPRCalibPosn()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	INT nResult = GMP_SUCCESS;
	pCInspOpt->m_lCurInspWH = m_lCurSlave;

	if (
		pCInspOpt->MoveAbsoluteXY(pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].x, pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].y, GMP_WAIT) != GMP_SUCCESS 
	)
	{
		nResult = GMP_FAIL;
	}
	PRS_DisplayVideo(&pruINSPWH_UL_Calib[m_lCurSlave]);

	return nResult;
}

INT CInspWH::SetupGoTStandbyPosn()
{
	INT nResult = GMP_SUCCESS;
	
	if (
		MoveT(INSPWH_T_CTRL_GO_STANDBY, GMP_WAIT) != GMP_SUCCESS
	   )
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoULRefPosn()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	INT nResult = GMP_SUCCESS;
	pCInspOpt->m_lCurInspWH = m_lCurSlave;
	
	if (
		pCInspOpt->MoveZ(INSPOPT_Z_CTRL_GO_REF_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInspOpt->MoveAbsoluteXY(m_mtrULRefPosn.x, m_mtrULRefPosn.y, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoDL1RefPosn()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	INT nResult = GMP_SUCCESS;
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	if (
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		MoveAbsoluteXY(m_mtrDL1RefPosn.x, m_mtrDL1RefPosn.y, GMP_WAIT) != GMP_SUCCESS ||
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_DL1_REF_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS 
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoDL2RefPosn()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	INT nResult = GMP_SUCCESS;
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	if (
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		MoveAbsoluteXY(m_mtrDL2RefPosn.x, m_mtrDL2RefPosn.y, GMP_WAIT) != GMP_SUCCESS ||
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_DL2_REF_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS 
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

//INT CInspWH::SetupGoRejectG1Posn()
//{
//	INT nResult = GMP_SUCCESS;
//	
//	if (
//		MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//		m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS //||
//		//MoveXY(INSPWH_XY_CTRL_GO_REJECT_G1_POSN, GMP_WAIT) != GMP_SUCCESS
//	)
//	{
//		nResult = GMP_FAIL;
//	}
//
//	return nResult;
//}

//INT CInspWH::SetupGoRejectG2Posn()
//{
//	INT nResult = GMP_SUCCESS;
//	
//	if (
//		MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//		m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS //||
//		//MoveXY(INSPWH_XY_CTRL_GO_REJECT_G2_POSN, GMP_WAIT) != GMP_SUCCESS
//	)
//	{
//		nResult = GMP_FAIL;
//	}
//
//	return nResult;
//}

//INT CInspWH::SetupGoRejectG1Offset()
//{
//	INT nResult = GMP_SUCCESS;
//	
//	if (
//		MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//		m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS //||
//		//MoveXY(INSPWH_XY_CTRL_GO_REJECT_G1_OFFSET, GMP_WAIT) != GMP_SUCCESS
//	)
//	{
//		nResult = GMP_FAIL;
//	}
//
//	return nResult;
//}
//
//INT CInspWH::SetupGoRejectG2Offset()
//{
//	INT nResult = GMP_SUCCESS;
//	
//	if (
//		MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
//		m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//		m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS //||
//		//MoveXY(INSPWH_XY_CTRL_GO_REJECT_G2_OFFSET, GMP_WAIT) != GMP_SUCCESS
//	)
//	{
//		nResult = GMP_FAIL;
//	}
//
//	return nResult;
//}

INT CInspWH::SetupGoInspOptStandbyPosn()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	INT nResult = GMP_SUCCESS;

	
	if (
		pCInspOpt->MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoULRefLevel()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	INT nResult = GMP_SUCCESS;
	pCInspOpt->m_lCurInspWH = m_lCurSlave;
	if (
		pCInspOpt->MoveZ(INSPOPT_Z_CTRL_GO_REF_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInspOpt->m_stMotorZ.Sync() != GMP_SUCCESS
	)
	{
		return GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoStandbyLevel()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	INT nResult = GMP_SUCCESS;
	if (
		pCInspOpt->MoveZ(INSPOPT_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInspOpt->m_stMotorZ.Sync() != GMP_SUCCESS
	)
	{
		return GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoULPRLevel(LONG lCurGlass)
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	INT nResult = GMP_SUCCESS;
	pCInspOpt->m_lCurInspWH = m_lCurSlave;
	pCInspOpt->m_lCurGlass = lCurGlass;
	if (
		pCInspOpt->MoveZ(INSPOPT_Z_CTRL_GO_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInspOpt->m_stMotorZ.Sync() != GMP_SUCCESS
	)
	{
		return GMP_FAIL;
	}
	PRS_DisplayVideo(&pruINSPWH_UL_Calib[m_lCurSlave]);

	return nResult;
}

INT CInspWH::SetupGoPRCalibLevel()
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	INT nResult = GMP_SUCCESS;
	pCInspOpt->m_lCurInspWH = m_lCurSlave;
	if (
		pCInspOpt->MoveZ(INSPOPT_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInspOpt->m_stMotorZ.Sync() != GMP_SUCCESS ||
		pCInspOpt->MoveAbsoluteXY(pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].x, pCInspOpt->m_mtrPRCalibPosn[m_lCurSlave].y, GMP_WAIT) != GMP_SUCCESS
	)
	{
		return GMP_FAIL;
	}
	PRS_DisplayVideo(&pruINSP_DL_Calib[m_lCurGlass][m_lCurSlave]);

	return nResult;
}

INT CInspWH::SetupGoDLPRCalibLevel()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	INT nResult = GMP_SUCCESS;
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	if (
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		MoveAbsoluteXY(m_mtrPRCalibPosn[GLASS1].x, m_mtrPRCalibPosn[GLASS1].y, GMP_WAIT) != GMP_SUCCESS ||
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS 
	)
	{
		return GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoDL1RefLevel()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	INT nResult = GMP_SUCCESS;
	if (
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		MoveAbsoluteXY(m_mtrDL1RefPosn.x, m_mtrDL1RefPosn.y, GMP_WAIT) != GMP_SUCCESS ||
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_DL1_REF_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS 
	)
	{
		return GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoDL2RefLevel()
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	INT nResult = GMP_SUCCESS;
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	if (
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		MoveAbsoluteXY(m_mtrDL2RefPosn.x, m_mtrDL2RefPosn.y, GMP_WAIT) != GMP_SUCCESS ||
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_DL2_REF_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS 
	)
	{
		return GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::SetupGoPRLevel(LONG lCurGlass)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	m_lCurGlass = lCurGlass;
	INT nResult = GMP_SUCCESS;
	if (
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_PR_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS
	)
	{
		return GMP_FAIL;
	}
	PRS_DisplayVideo(&pruINSP_DL_Calib[m_lCurGlass][m_lCurSlave]);

	return nResult;
}

INT CInspWH::SetupGoInspOptPrePRPosn(WHNum lCurWH)
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	INT nResult = GMP_SUCCESS;

	pCInspOpt->m_lCurInspWH = lCurWH;
	if (
		pCInspOpt->MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

INT CInspWH::KeepQuiet()
{
	INT nResult = GMP_SUCCESS;

	if (!m_fHardware)
	{
		return nResult;
	}

	if (!IsGLASS1_VacSensorOn())
	{
		SetGLASS1_VacSol(OFF, GMP_NOWAIT);
	}
	if (!IsGLASS2_VacSensorOn())
	{
		SetGLASS2_VacSol(OFF, GMP_NOWAIT);
	}
	return nResult;
}

VOID CInspWH::PrintMachineSetup(FILE *fp)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str = "";

	if (fp != NULL)
	{
		fprintf(fp, "\n<<<<%s>>>>\n", (const char*)GetStnName());
		fprintf(fp, "======================================================\n");
		fprintf(fp, "<<Posn>>\n");
		fprintf(fp, "%30s\t : %ld, %ld\n", "Standby Posn", 
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.x"], 
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"]);
		fprintf(fp, "%30s\t : %ld, %ld\n", "Load Glass Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.x"],
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.y"]);
		fprintf(fp, "%30s\t : %ld, %ld\n", "Unload Glass Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.x"],
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.y"]);
		fprintf(fp, "%30s\t : %ld, %ld\n", "PR Calib Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrPRCalibPosn.x"],
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrPRCalibPosn.y"]);
		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{
				str.Format("GLASS%ld", i + 1);
				fprintf(fp, "GLASS%d\n", i + 1);
				fprintf(fp, "%30s\t : %ld, %ld\n", "Reject Glass Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassPosn.x"],
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassPosn.y"]);
		}


		fprintf(fp, "<<Level>>\n");
		for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
		{
			str.Format("WHZ_%ld", i + 1);

			fprintf(fp, "WHZ_%d\n", i + 1);
			fprintf(fp, "%30s\t : %ld\n", "Standby Level",
					(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lStandbyLevel"]);
			fprintf(fp, "%30s\t : %ld\n", "Calib. Load Level",
					(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibLoadLevel"]);
			fprintf(fp, "%30s\t : %ld\n", "Calib. Unload Level",
					(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibUnloadLevel"]);
			fprintf(fp, "%30s\t : %ld\n", "Calib. PR Level",
					(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"]);
			fprintf(fp, "%30s\t : %ld\n", "Calib. Reject Unload Level",
					(LONG)pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lUnloadRejGlassLevel"]);
		}
	}

	CPI9000Stn::PrintMachineSetup(fp);
}

VOID CInspWH::PrintDeviceSetup(FILE *fp)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	if (fp != NULL)
	{
		fprintf(fp, "\n<<<<%s>>>>\n", (const char*)GetStnName());
		fprintf(fp, "======================================================\n");
		fprintf(fp, "<<Posn>>\n");

		fprintf(fp, "%30s\t : %ld, %ld\n", "Load Glass Offset",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassOffset.x"],
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassOffset.y"]);
		fprintf(fp, "%30s\t : %ld, %ld\n", "Unload Glass Posn",
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassOffset.x"],
				(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassOffset.y"]);
		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{
			str.Format("GLASS%ld", i + 1);
			fprintf(fp, "%s\n", (const char*)str);
			fprintf(fp, "%30s\t : %ld, %ld\n", "PR1 Posn", 
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrPR1Posn.x"], 
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrPR1Posn.y"]);
			fprintf(fp, "%30s\t : %ld, %ld\n", "PR2 Posn", 
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrPR2Posn.x"], 
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrPR2Posn.y"]);
			fprintf(fp, "%30s\t : %ld, %ld\n", "Reject Glass Offset",
			(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassOffset.x"],
			(LONG)pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassOffset.y"]);
		}

		fprintf(fp, "<<Level>>\n");
		for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
		{
			str.Format("WHZ_%ld", i + 1);

			fprintf(fp, "WHZ_%d\n", i + 1);
			fprintf(fp, "%30s\t : %ld\n", "Load Level Offset",
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lLoadLevelOffset"]);
			fprintf(fp, "%30s\t : %ld\n", "Unload Level Offset",
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadLevelOffset"]);
			fprintf(fp, "%30s\t : %ld\n", "PR Level",
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lPRLevel"]);
			fprintf(fp, "%30s\t : %ld\n", "ULPR Level",
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lULPRLevel"]);
			fprintf(fp, "%30s\t : %ld\n", "Reject Unload Level Offset",
					(LONG)pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadRejGlassOffset"]);
		}

		fprintf(fp, "<<Para>>\n");
		fprintf(fp, "%30s\t : %ld\n", "Vac On Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stGlass1VacSol.GetName()]["lOnDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "Vac Off Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stGlass1VacSol.GetName()]["lOffDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "WeakBlow On Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stWeakBlowSol.GetName()]["lOnDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "WeakBlow Off Delay",
				(LONG)pAppMod->m_smfDevice[GetStnName()]["Delays"][m_stWeakBlowSol.GetName()]["lOffDelay"]);

		fprintf(fp, "%30s\t : %ld\n", "PR Delay",
				(LONG)pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRDelay"]);
		fprintf(fp, "%30s\t : %ld\n", "PR Retry Limit",
				(LONG)pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRRetryLimit"]);
		fprintf(fp, "%30s\t : %ld\n", "Reject Glass Limit",
				(LONG)pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lRejectGlassLimit"]);
	}
	CPI9000Stn::PrintDeviceSetup(fp);
}

VOID CInspWH::PrintCalibSetup(FILE *fp)
{
	return CPRTaskStn::PrintCalibSetup(fp);
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
BOOL CInspWH::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;

	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrPRCalibPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrPRCalibPosn.x"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrPRCalibPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrPRCalibPosn.y"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS1"]["m_mtrRejectGlassPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrRejectGlassPosn[GLASS1].x"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS1"]["m_mtrRejectGlassPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrRejectGlassPosn[GLASS1].y"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS2"]["m_mtrRejectGlassPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrRejectGlassPosn[GLASS2].x"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS2"]["m_mtrRejectGlassPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrRejectGlassPosn[GLASS2].y"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS1"]["m_mtrULRefPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrULRefPosn[GLASS1].x"), szBlank,
			szBlank, szBlank, szBlank, szBlank, szBlank, szBlank, lValue);

	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS1"]["m_mtrULRefPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrULRefPosn[GLASS1].y"), szBlank,
			szBlank, szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS2"]["m_mtrULRefPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrULRefPosn[GLASS2].x"), szBlank,
			szBlank, szBlank, szBlank, szBlank, szBlank, szBlank, lValue);

	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["GLASS1"]["m_mtrULRefPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("m_mtrULRefPosn[GLASS2].y"), szBlank,
			szBlank, szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}

	if (bResult)
	{
		bResult = CWorkHolder::printMachineDeviceInfo();
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
LONG CInspWH::HMI_SetPRSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	SetPRSelected(bMode);

	return 0;
}

LONG CInspWH::HMI_SelectWHNum(IPC_CServiceMessage &svMsg)
{
	ULONG ulWHNum;
	svMsg.GetMsg(sizeof(ULONG), &ulWHNum);

	INT nResult = GMP_SUCCESS;
	CString szMsg = "";
	
	m_lCurInspWH	= (WHNum)ulWHNum;
	m_lCurSlave	= ulWHNum;
	
	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();

	LogAction(__FUNCTION__);
	LogAction("m_lCurInspWH = %ld\t", m_lCurInspWH);
	LogAction("m_lCurSlave = %ld\t", m_lCurSlave);

	return FALSE;
}

////////////////////////////////////////
//PR Setup Page
////////////////////////////////////////


LONG CInspWH::HMI_SelectGlassNum(IPC_CServiceMessage &svMsg)
{
	ULONG ulGlassNum;
	svMsg.GetMsg(sizeof(ULONG), &ulGlassNum);

	INT nResult = GMP_SUCCESS;
	CString szMsg = "";
	
	m_lCurGlass	= ulGlassNum;
	
	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();

	LogAction(__FUNCTION__);
	LogAction("m_lCurGlass = %ld\t", m_lCurGlass);

	return FALSE;
}

LONG CInspWH::HMI_SelectPRU(IPC_CServiceMessage &svMsg)
{
	ULONG ulPRU;
	svMsg.GetMsg(sizeof(ULONG), &ulPRU);
	PRU *pPRU = NULL;
	
	HMI_ulCurPRU = ulPRU;

	SelectPRU(ulPRU);

	pPRU = &PRS_SetCameraPRU(INSPDL_CAM + m_lCurGlass);
	PRS_DisplayVideo(pPRU);
	PRS_ClearScreen(INSPDL_CAM + m_lCurGlass);
	PRS_DrawHomeCursor(INSPDL_CAM + m_lCurGlass, FALSE);

	return 0;
}

LONG CInspWH::HMI_UpdateSetupStatus(IPC_CServiceMessage &svMsg)
{
	UpdateSetupStatus();
	return 0;
}

LONG CInspWH::HMI_SetPRPosn(IPC_CServiceMessage &svMsg)
{	
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CString szOldValue;
	szOldValue.Format("X1 = %ld, Y1 = %ld, X2 = %ld, Y2 = %ld",
		m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x, m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y,
		m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x, m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Position?");
	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
		return 0;
	}
	else if (lAnswer == rMSG_CONTINUE)
	{
		//lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Change UL PR Position as well?");
		switch (HMI_ulCurPRU)
		{
		case 0:
			//if (lAnswer == rMSG_CONTINUE)
			{
				pCInspOpt->m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x += (pCInPickArm->m_stMotorX.GetEncPosn() - m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x) * 100;
				pCInspOpt->m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y += m_stMotorY.GetEncPosn() - m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y;
			}
			m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x = pCInPickArm->m_stMotorX.GetEncPosn();
			m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y = m_stMotorY.GetEncPosn();

			break;
		case 1:
			//if (lAnswer == rMSG_CONTINUE)
			{
				pCInspOpt->m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x += (pCInPickArm->m_stMotorX.GetEncPosn() - m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x) * 100;
				pCInspOpt->m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y += m_stMotorY.GetEncPosn() - m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y;
			}
			m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x = pCInPickArm->m_stMotorX.GetEncPosn();
			m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y = m_stMotorY.GetEncPosn();
			break;
		}
	}

	CString szNewValue;
	szNewValue.Format("X1 = %ld, Y1 = %ld, X2 = %ld, Y2 = %ld",
		m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].x, m_mtrPR1Posn[m_lCurSlave][m_lCurGlass].y,
		m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].x, m_mtrPR2Posn[m_lCurSlave][m_lCurGlass].y);

	CString szParameter;
	szParameter.Format("INSPWH%ld Glass%ld PR Position", m_lCurSlave + 1, m_lCurGlass + 1);
	LogParameter(__FUNCTION__, szParameter, szOldValue, szNewValue);

	return 0;
}

LONG CInspWH::HMI_SetPRLevel(IPC_CServiceMessage &svMsg)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CString szOldValue;
	szOldValue.Format("Z = %ld", pCInPickArm->m_lPRLevel[m_lCurSlave][m_lCurGlass]);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Level?");
	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
		return 0;
	}
	else if (lAnswer == rMSG_CONTINUE)
	{
		pCInPickArm->m_lPRLevel[m_lCurSlave][m_lCurGlass] = pCInPickArm->m_stMotorZ.GetEncPosn();
	}

	CString szNewValue;
	szNewValue.Format("Z = %ld", pCInPickArm->m_lPRLevel[m_lCurSlave][m_lCurSlave]);

	CString szParameter;
	szParameter.Format("INSPWH%ld DL Glass%ld PR Level", m_lCurSlave + 1, m_lCurGlass + 1);
	LogParameter(__FUNCTION__, szParameter, szOldValue, szNewValue);

	return 0;
}
LONG CInspWH::HMI_SearchAp1(IPC_CServiceMessage &svMsg)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp1;
	MTR_POSN mtrTemp;
	PR_COORD coCentre;

	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pruDLPR1[GLASS1].bLoaded || !pruDLPR2[GLASS1].bLoaded || !pruDLPR1[GLASS2].bLoaded || !pruDLPR2[GLASS2].bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!m_stInspWH[m_lCurGlass].bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (m_lCurSlave == WH_2)
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
	}
	else
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
	}

	if( m_lCurGlass == GLASS1 )
	{
		m_lCORCamNum = INSPDL1_COR;
	}
	else
	{
		m_lCORCamNum = INSPDL2_COR;
	}

	PRS_DisplayVideo(&pruDLPR1[m_lCurGlass]);

	m_dCurrAlignAngle = 0.0;

	MoveTToStandby(GMP_WAIT);
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	pCInPickArm->MoveZToPRLevel(GMP_WAIT);
	
	if (!AutoSearchPR1(&pruDLPR1[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

#if 1
	MoveAbsoluteXY((LONG)m_dmtrCurrPR1Posn.x, (LONG)m_dmtrCurrPR1Posn.y, GMP_WAIT);
	Sleep(500);
#endif

	if (!AutoSearchPR2(&pruDLPR2[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

#if 1
	MoveAbsoluteXY((LONG)m_dmtrCurrPR2Posn.x, (LONG)m_dmtrCurrPR2Posn.y, GMP_WAIT);
	Sleep(500);
#endif

	CalcAlignPt((GlassNum)m_lCurGlass);
	mtrTemp1.x = DoubleToLong(m_dmtrCurrPR1Posn.x + m_dmtrCurrAlign1Offset.x);
	mtrTemp1.y = DoubleToLong(m_dmtrCurrPR1Posn.y + m_dmtrCurrAlign1Offset.y);

	MoveAbsoluteXY(mtrTemp1.x, mtrTemp1.y, GMP_WAIT);

	PRS_ClearScreen(INSPDL_CAM + m_lCurGlass);

	if(m_bMoveT)
	{
		DOUBLE dAngle = 1.0 * (m_dCurrAlignAngle);
		m_lTOffset = DistanceToCount(dAngle, m_stMotorT.stAttrib.dDistPerCount);

		mtrTemp1.y = FirstEncToSecondEnc(mtrTemp1.y);
		CalPosnAfterRotate(mtrTemp1, -dAngle, &mtrTemp);
		mtrTemp.y = SecondEncToFirstEnc(mtrTemp.y);

		m_stMotorT.MoveRelative(m_lTOffset, GMP_WAIT);
		MoveAbsoluteXY(mtrTemp.x, mtrTemp.y, GMP_WAIT);
	}

	PRS_DrawCross(INSPDL_CAM + m_lCurGlass, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}


LONG CInspWH::HMI_SearchAp2(IPC_CServiceMessage &svMsg)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp1;
	MTR_POSN mtrTemp;
	PR_COORD coCentre;

	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pruDLPR1[m_lCurGlass].bLoaded || !pruDLPR2[m_lCurGlass].bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!m_stInspWH[m_lCurGlass].bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (m_lCurSlave == WH_2)
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
	}
	else
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
	}

	if( m_lCurGlass == GLASS1 )
	{
		m_lCORCamNum = INSPDL1_COR;
	}
	else
	{
		m_lCORCamNum = INSPDL2_COR;
	}

	PRS_DisplayVideo(&pruDLPR1[m_lCurGlass]);

	m_dCurrAlignAngle = 0.0;
	MoveTToStandby(GMP_WAIT);
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	pCInPickArm->MoveZToPRLevel(GMP_WAIT);
	if (!AutoSearchPR1(&pruDLPR1[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

#if 1
	MoveAbsoluteXY((LONG)m_dmtrCurrPR1Posn.x, (LONG)m_dmtrCurrPR1Posn.y, GMP_WAIT);
	Sleep(500);
#endif

	if (!AutoSearchPR2(&pruDLPR2[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

#if 1
	MoveAbsoluteXY((LONG)m_dmtrCurrPR2Posn.x, (LONG)m_dmtrCurrPR2Posn.y, GMP_WAIT);
	Sleep(500);
#endif
	
	CalcAlignPt((GlassNum)m_lCurGlass);

	mtrTemp1.x = DoubleToLong(m_dmtrCurrPR2Posn.x + m_dmtrCurrAlign2Offset.x);
	mtrTemp1.y = DoubleToLong(m_dmtrCurrPR2Posn.y + m_dmtrCurrAlign2Offset.y);

	MoveAbsoluteXY(mtrTemp1.x, mtrTemp1.y, GMP_WAIT);

	PRS_ClearScreen(INSPDL_CAM + m_lCurGlass);

	if(m_bMoveT)
	{
		DOUBLE dAngle = 1.0 * (m_dCurrAlignAngle);

		m_lTOffset = DistanceToCount(dAngle, m_stMotorT.stAttrib.dDistPerCount);

		mtrTemp1.y = FirstEncToSecondEnc(mtrTemp1.y);
		CalPosnAfterRotate(mtrTemp1, -dAngle, &mtrTemp);
		mtrTemp.y = SecondEncToFirstEnc(mtrTemp.y);

		m_stMotorT.MoveRelative(m_lTOffset, GMP_WAIT);
		MoveAbsoluteXY(mtrTemp.x, mtrTemp.y, GMP_WAIT);
	}

	PRS_DrawCross(INSPDL_CAM + m_lCurGlass, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspWH::HMI_SearchCentre(IPC_CServiceMessage &svMsg)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp;
	mtrTemp.x = 0;
	mtrTemp.y = 0;
	MTR_POSN mtrAlignCenter;
	PR_COORD coCentre;
	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pruDLPR1[m_lCurGlass].bLoaded || !pruDLPR2[m_lCurGlass].bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!m_stInspWH[m_lCurGlass].bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (m_lCurSlave == WH_2)
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
	}
	else
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
	}

	if( m_lCurGlass == GLASS1 )
	{
		m_lCORCamNum = INSPDL1_COR;
	}
	else
	{
		m_lCORCamNum = INSPDL2_COR;
	}

	PRS_DisplayVideo(&pruDLPR1[m_lCurGlass]);

	m_dCurrAlignAngle = 0.0;

	MoveTToStandby(GMP_WAIT);
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	pCInPickArm->MoveZToPRLevel(GMP_WAIT);
	if (!AutoSearchPR1(&pruDLPR1[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

#if 1
	MoveAbsoluteXY((LONG)m_dmtrCurrPR1Posn.x, (LONG)m_dmtrCurrPR1Posn.y, GMP_WAIT);
	Sleep(500);
#endif

	if (!AutoSearchPR2(&pruDLPR2[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

#if 1
	MoveAbsoluteXY((LONG)m_dmtrCurrPR2Posn.x, (LONG)m_dmtrCurrPR2Posn.y, GMP_WAIT);
	Sleep(500);
#endif
	
	CalcAlignPt((GlassNum)m_lCurGlass);

	MoveAbsoluteXY(DoubleToLong(m_dmtrCurrAlignCentre.x), DoubleToLong(m_dmtrCurrAlignCentre.y), GMP_WAIT);

	PRS_ClearScreen(INSPDL_CAM + m_lCurGlass);

	if(m_bMoveT)
	{
		DOUBLE dAngle = 1.0 * (m_dCurrAlignAngle);

		m_lTOffset = DistanceToCount(dAngle, m_stMotorT.stAttrib.dDistPerCount);

		mtrAlignCenter.x = DoubleToLong(m_dmtrCurrAlignCentre.x);
		mtrAlignCenter.y = DoubleToLong(m_dmtrCurrAlignCentre.y);

		mtrAlignCenter.y = FirstEncToSecondEnc(mtrAlignCenter.y);
		CalPosnAfterRotate(mtrAlignCenter, -dAngle, &mtrTemp);
		mtrTemp.y = SecondEncToFirstEnc(mtrTemp.y);

		m_stMotorT.MoveRelative(m_lTOffset, GMP_WAIT);
		MoveAbsoluteXY(mtrTemp.x, mtrTemp.y, GMP_WAIT);
	}

	PRS_DrawCross(INSPDL_CAM + m_lCurGlass, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspWH::HMI_SearchDLnUL(IPC_CServiceMessage &svMsg)
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	BOOL bResult = TRUE;
	pCInspOpt->m_lCurInspWH = m_lCurSlave;
	SearchDLnUL();
	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

BOOL CInspWH::SearchDLnULTest()
{
	//CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	BOOL bResult = TRUE;
	//pCHouseKeeping->m_bStartSearchDLnULTest = TRUE;
	pCInspOpt->m_lCurInspWH = m_lCurSlave;
	pCInspOpt->m_bMoveINPA = FALSE;
	pCInspOpt->m_bMoveWH = TRUE;
	if ( SearchDLnUL() == FALSE )
	{
		m_bStartSearchDLnULTest = FALSE;
		return FALSE;
	}
	
	//svMsg.InitMessage(sizeof(BOOL), &bResult);
	return TRUE;
}

LONG CInspWH::HMI_StartPRCycleTest(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString szMsg = _T("");
	BOOL	bOpDone = TRUE;
	LONG	lResponse;
	LONG	lMethod;

	m_bPRCycle = FALSE;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Only PR Repeatable Test!", GetStnName());
		lResponse = HMIMessageBox(MSG_CONTINUE_CANCEL, "PR CYCLE TEST", szMsg);

		if (lResponse == rMSG_CONTINUE)
		{
			m_lCycleTestMethod = 0;
			m_dCurCalibAng		= 0;
			m_dCurCalibX		= 0;
			m_dCurCalibY		= 0;
			m_bPRCycle = TRUE;
		}
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}

	lMethod = HMISelectionBox("OPTION", "Please select Method", "PR Repeatable Test", "COR Repeatable Test");
	if (lMethod == -1 || lMethod == 11)
	{
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}
	m_lCycleTestMethod = lMethod;
	

	PRS_DisplayVideo(&pruINSP_DL_Calib[m_lCurGlass][m_lCurSlave]);

	if (!pruINSP_DL_Calib[m_lCurGlass][m_lCurSlave].bLoaded)
	{
		HMIMessageBox(MSG_OK, "PR CYCLE TEST", "PR Pattern Not Loaded");
		WriteHMIMess("PR CYCLE TEST FAILED: PR Pattern Not Loaded");
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}
	if (m_lCycleTestMethod == 0)
	{
		if (!HMINumericKeys("Test X (um)", 99999.0, -99999.0, &m_dCurCalibX))
		{
			HMIMessageBox(MSG_OK, "PR CYCLE TEST", "OPERATION STOP");
			svMsg.InitMessage(sizeof(BOOL), &bOpDone);
			return 1;
		}

		if (!HMINumericKeys("Test Y (um)", 99999.0, -99999.0, &m_dCurCalibY))
		{
			HMIMessageBox(MSG_OK, "PR CYCLE TEST", "OPERATION STOP");
			svMsg.InitMessage(sizeof(BOOL), &bOpDone);
			return 1;
		}
	}
	else
	{
		lResponse = HMISelectionBox("OPTION", "Please select Direction", "Only ONE Direction", "Both Direction");
		if (lResponse == -1 || lResponse == 11)
		{
			svMsg.InitMessage(sizeof(BOOL), &bOpDone);
			return 1;
		}
		m_lCycleMoveTDirection = lResponse;
	}

	m_bPRCycle = TRUE;

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);

	return 1;
}

LONG CInspWH::HMI_SetG1XUnloadOffset(IPC_CServiceMessage &svMsg)
{
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Modifying the Unload Offset would affect the glass position on ALL WHs. Continue?");

	if (lAnswer == rMSG_CONTINUE)
	{
		m_lUnloadXOffset[GLASS1] = lDelay;
	}

	return 1;
}

LONG CInspWH::HMI_SetG1YUnloadOffset(IPC_CServiceMessage &svMsg)
{
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Modifying the Unload Offset would affect the glass position on ALL WHs. Continue?");

	if (lAnswer == rMSG_CONTINUE)
	{
		m_lUnloadYOffset[GLASS1] = lDelay;
	}

	return 1;
}

LONG CInspWH::HMI_SetG1TUnloadOffset(IPC_CServiceMessage &svMsg)
{
	DOUBLE dOffset;
	svMsg.GetMsg(sizeof(DOUBLE), &dOffset);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Modifying the Unload Offset would affect the glass position on ALL WHs. Continue?");

	if (lAnswer == rMSG_CONTINUE)
	{
		m_dUnloadTOffset[GLASS1] = dOffset;
	}

	return 1;
}

LONG CInspWH::HMI_SetG2XUnloadOffset(IPC_CServiceMessage &svMsg)
{
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Modifying the Unload Offset would affect the glass position on ALL WHs. Continue?");

	if (lAnswer == rMSG_CONTINUE)
	{
		m_lUnloadXOffset[GLASS2] = lDelay;
	}

	return 1;
}

LONG CInspWH::HMI_SetG2YUnloadOffset(IPC_CServiceMessage &svMsg)
{
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Modifying the Unload Offset would affect the glass position on ALL WHs. Continue?");

	if (lAnswer == rMSG_CONTINUE)
	{
		m_lUnloadYOffset[GLASS2] = lDelay;
	}

	return 1;
}

LONG CInspWH::HMI_SetG2TUnloadOffset(IPC_CServiceMessage &svMsg)
{
	DOUBLE dOffset;
	svMsg.GetMsg(sizeof(DOUBLE), &dOffset);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Modifying the Unload Offset would affect the glass position on ALL WHs. Continue?");

	if (lAnswer == rMSG_CONTINUE)
	{
		m_dUnloadTOffset[GLASS2] = dOffset;
	}

	return 1;
}


LONG CInspWH::HMI_MeasureSetZero(IPC_CServiceMessage &svMsg) //20121016
{
	//CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	if (bMode)
	{
		m_bEnableMeasure = TRUE;

		m_mtrMeasureSetZeroPt.x = GetMotorX().GetEncPosn();
		m_mtrMeasureSetZeroPt.y = m_stMotorY.GetEncPosn();

		m_dMeasuredDistance = 0.0;
		m_dMeasuredAngle = 0.0;
	}
	else
	{
		m_bEnableMeasure = FALSE;
	}

	return 0;
}

LONG CInspWH::HMI_MeasureEnable(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	if (bMode)
	{
		m_bEnableMeasure = TRUE;
	}
	else
	{
		m_bEnableMeasure = FALSE;
	}

	return 0;
}


LONG CInspWH::IPC_SaveMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULPRCalibPosn.x"]		= m_mtrULPRCalibPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULPRCalibPosn.y"]		= m_mtrULPRCalibPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULRefPosn.x"]			= m_mtrULRefPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULRefPosn.y"]			= m_mtrULRefPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL1RefPosn.x"]		= m_mtrDL1RefPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL1RefPosn.y"]		= m_mtrDL1RefPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL2RefPosn.x"]		= m_mtrDL2RefPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL2RefPosn.y"]		= m_mtrDL2RefPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lEncDiff_y"]				= m_lEncDiff_y;

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.x"]			= m_mtrPRCalibPosn[i].x;
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.y"]			= m_mtrPRCalibPosn[i].y;
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassPosn.x"]		= m_mtrRejectGlassPosn[i].x;
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassPosn.y"]		= m_mtrRejectGlassPosn[i].y;

	}
	//for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	//{
	//	str.Format("WHZ_%ld", i + 1);
	//	pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lUnloadRejGlassLevel"]		= m_lUnloadRejGlassLevel[i];
	//}

	return CWorkHolder::IPC_SaveMachineParam();
}

LONG CInspWH::IPC_LoadMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	m_mtrULPRCalibPosn.x	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULPRCalibPosn.x"];
	m_mtrULPRCalibPosn.y	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULPRCalibPosn.y"];
	m_mtrULRefPosn.x		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULRefPosn.x"];
	m_mtrULRefPosn.y		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrULRefPosn.y"];
	m_mtrDL1RefPosn.x		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL1RefPosn.x"];
	m_mtrDL1RefPosn.y		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL1RefPosn.y"];
	m_mtrDL2RefPosn.x		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL2RefPosn.x"];
	m_mtrDL2RefPosn.y		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrDL2RefPosn.y"];
	m_lEncDiff_y			= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lEncDiff_y"];
	
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);
		m_mtrPRCalibPosn[i].x			= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.x"];
		m_mtrPRCalibPosn[i].y			= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.y"];
		m_mtrRejectGlassPosn[i].x		= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassPosn.x"];
		m_mtrRejectGlassPosn[i].y		= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrRejectGlassPosn.y"];

	}
	//for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	//{
	//	str.Format("WHZ_%ld", i + 1);
	//	m_lUnloadRejGlassLevel[i]		= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lUnloadRejGlassLevel"];
	//}
	return CWorkHolder::IPC_LoadMachineParam();
}

LONG CInspWH::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str;
	CString str1;
	

	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrAlignUnloadOffset.x"]	= m_mtrAlignUnloadOffset.x;
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrAlignUnloadOffset.y"]	= m_mtrAlignUnloadOffset.y;
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrLoadGlassOffset.x"]	= m_mtrLoadGlassOffset.x;
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrLoadGlassOffset.y"]	= m_mtrLoadGlassOffset.y; 
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrUnloadGlassOffset.x"]	= m_mtrUnloadGlassOffset.x;
	pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrUnloadGlassOffset.y"]	= m_mtrUnloadGlassOffset.y; 

	pAppMod->m_smfDevice[GetStnName()]["Setting"]["m_bMoveT"]				= m_bMoveT;
	pAppMod->m_smfDevice[GetStnName()]["Setting"]["m_bUseAnvilVacuum"]				= m_bUseAnvilVacuum;


	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);

		pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrRejectGlassOffset.x"]		= m_mtrRejectGlassOffset[i].x;
		pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrRejectGlassOffset.y"]		= m_mtrRejectGlassOffset[i].y;

		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dGlassRefAng"]			= m_dGlassRefAng[i];
		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.x"]		= m_dmtrAlignRef[i].x;
		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.y"]		= m_dmtrAlignRef[i].y;
		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dAlignRefAngle"]		= m_dAlignRefAngle[i];
		//pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lULPRLevel"]				= m_lULPRLevel[i];
		//pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadRejGlassOffset"]		= m_lUnloadRejGlassOffset[i];
		for (INT j = WHZ_1; j < MAX_NUM_OF_WHZ; j++)
		{
			str1.Format("WHZ_%ld", j + 1);
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.x"]				= m_mtrPR1Posn[i][j].x;
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.y"]				= m_mtrPR1Posn[i][j].y;
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.x"]				= m_mtrPR2Posn[i][j].x;
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.y"]				= m_mtrPR2Posn[i][j].y;
			//pAppMod->m_smfDevice[GetStnName()]["Level"][str][str1]["m_lPRLevel"]				= m_lPRLevel[i][j];
		}
	}

	//pAppMod->m_smfDevice[GetStnName()]["Para"]["HMI_bDebugCal"]						= HMI_bDebugCal;
	
	return CWorkHolder::IPC_SaveDeviceParam();
}

LONG CInspWH::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;
	CString str1;
	
	m_mtrAlignUnloadOffset.x		= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrAlignUnloadOffset.x"];
	m_mtrAlignUnloadOffset.y		= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrAlignUnloadOffset.y"];
	m_mtrLoadGlassOffset.x			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrLoadGlassOffset.x"];
	m_mtrLoadGlassOffset.y			= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrLoadGlassOffset.y"];
	m_mtrUnloadGlassOffset.x		= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrUnloadGlassOffset.x"];
	m_mtrUnloadGlassOffset.y		= pAppMod->m_smfDevice[GetStnName()]["Posn"]["m_mtrUnloadGlassOffset.y"];

	m_bMoveT						= (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Setting"]["m_bMoveT"];
	m_bUseAnvilVacuum						= (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Setting"]["m_bUseAnvilVacuum"];
	
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);

		m_mtrRejectGlassOffset[i].x		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrRejectGlassOffset.x"];
		m_mtrRejectGlassOffset[i].y		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str]["m_mtrRejectGlassOffset.y"];

		m_dGlassRefAng[i]		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dGlassRefAng"];
		m_dmtrAlignRef[i].x		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.x"];
		m_dmtrAlignRef[i].y		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.y"];
		m_dAlignRefAngle[i]		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dAlignRefAngle"];
		//m_lULPRLevel[i]			= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lULPRLevel"];
		//m_lUnloadRejGlassOffset[i]		= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadRejGlassOffset"];		

		for (INT j = WHZ_1; j < MAX_NUM_OF_WHZ; j++)
		{
			str1.Format("WHZ_%ld", j + 1);
			m_mtrPR1Posn[i][j].x		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.x"];
			m_mtrPR1Posn[i][j].y		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.y"];
			m_mtrPR2Posn[i][j].x		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.x"];
			m_mtrPR2Posn[i][j].y		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.y"];
			//m_lPRLevel[i][j]			= pAppMod->m_smfDevice[GetStnName()]["Level"][str][str1]["m_lPRLevel"];

		}
	}

	//HMI_bDebugCal = (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Para"]["HMI_bDebugCal"];
	return CWorkHolder::IPC_LoadDeviceParam();
}

LONG CInspWH::IPC_LoadMachineRecord()
{
	if (m_lCurSlave == WH_1)
	{
		// for uploading records
		CString szPath = "";

		for (INT i = 0; i < MAX_NUM_OF_GLASS; i++)
		{
			for (INT j = 0; j < MAX_NUM_OF_WH; j++)
			{
				szPath = "d:\\sw\\PI9000\\Record\\" + pruINSP_DL_Calib[i][j].szFileName + ".zip";
				strcpy((char*) & (pruINSP_DL_Calib[i][j].stDownloadRecordCmd.acFilename[0]), szPath);
			}
		}
	}
	return 1;
}

LONG CInspWH::IPC_SaveMachineRecord()
{
	if (m_lCurSlave == WH_1)
	{
		CString szPath = "";

		for (INT i = 0; i < MAX_NUM_OF_GLASS; i++)
		{
			for (INT j = 0; j < MAX_NUM_OF_WH; j++)
			{
				szPath = "d:\\sw\\PI9000\\Record\\" + pruINSP_DL_Calib[i][j].szFileName + ".zip";
				strcpy((char*) & (pruINSP_DL_Calib[i][j].stUploadRecordCmd.acFilename[0]), szPath);
			}
		}
	}
	return 1;
}

LONG CInspWH::IPC_LoadDeviceRecord()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	if (m_lCurSlave == WH_1)
	{
		CString str = "";

		// for uploading records
		CString szPath = "";
		CString szDevice = pAppMod->m_szDeviceFile;
		INT nFind = szDevice.ReverseFind('.');

		for (INT i = 0; i < MAX_NUM_OF_GLASS; i++)
		{
			szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruDLPR1[i].szFileName + ".zip";
			strcpy((char*) & (pruDLPR1[i].stDownloadRecordCmd.acFilename[0]), szPath);

			szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruDLPR2[i].szFileName + ".zip";
			strcpy((char*) & (pruDLPR2[i].stDownloadRecordCmd.acFilename[0]), szPath);
		}
	}
	return 1;
}

LONG CInspWH::IPC_SaveDeviceRecord()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	if (m_lCurSlave == WH_1)
	{
		CString str = "";

		// for uploading records
		CString szPath = "";
		CString szDevice = pAppMod->m_szDeviceFile;
		INT nFind = szDevice.ReverseFind('.');

		for (INT i = 0; i < MAX_NUM_OF_GLASS; i++)
		{
			szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruDLPR1[i].szFileName + ".zip";
			strcpy((char*) & (pruDLPR1[i].stUploadRecordCmd.acFilename[0]), szPath);

			szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruDLPR2[i].szFileName + ".zip";
			strcpy((char*) & (pruDLPR2[i].stUploadRecordCmd.acFilename[0]), szPath);
		}
	}
	return 1;
}


INT CInspWH::SetupGoLoadGlassOffset() //20131111
{
	INT nResult = GMP_SUCCESS;

	if (
		//MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS || 
		//m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
		//m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS ||
		MoveXY(INSPWH_XY_CTRL_GO_LOAD_OFFSET, GMP_WAIT) != GMP_SUCCESS
	)
	{
		nResult = GMP_FAIL;
	}

	return nResult;
}

BOOL CInspWH::SearchDLnUL()
{
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp;
	mtrTemp.x = 0;
	mtrTemp.y = 0;

	PR_COORD coCentre;
	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pCInPickArm->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pruDLPR1[m_lCurGlass].bLoaded || !pruDLPR2[m_lCurGlass].bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "PR Pattern Not Loaded. Operation Abort!");
		return FALSE;
	}

	if (!pruULGlass1.bLoaded || !pruULGlass2.bLoaded || !pruULCOF1.bLoaded || !pruULCOF2.bLoaded )
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "InspOpt PR Pattern Not Loaded. Operation Abort!");
		return FALSE;
	}

	if (!m_stInspWH[m_lCurGlass].bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Align Pts Not Learned. Operation Abort!");	
		return FALSE;
	}

	if (m_lCurSlave == WH_2)
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
	}
	else
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
	}

	m_dCurrAlignAngle = 0.0;

	MoveTToStandby(GMP_WAIT);
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	pCInPickArm->MoveZToPRLevel(GMP_WAIT);

	PRS_DisplayVideo(&pruDLPR1[m_lCurGlass]);

	if (!AutoSearchPR1(&pruDLPR1[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Search PR1 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	if (!AutoSearchPR2(&pruDLPR2[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Search PR2 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	PRS_ClearScreen(INSPDL_CAM + m_lCurGlass);
	PRS_DisplayVideo(&pruULGlass1);
	
	UpdateInspPRPosn(m_lCurGlass);
	pCInspOpt->m_lCurGlass = m_lCurGlass;
	if (!pCInspOpt->SearchPlacementTest())
	{
		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{

			pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].x = 0.0;
			pCInspOpt->m_dmtrPR1Offset[m_lCurSlave][i].y = 0.0;
			pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][i].x = 0.0;
			pCInspOpt->m_dmtrPR2Offset[m_lCurSlave][i].y = 0.0;
		}
		return FALSE;
	}

	PRS_DisplayVideo(&pruDLPR1[m_lCurGlass]);
	return TRUE;
}

BOOL CInspWH::SearchDLnGoULPR1Posn()
{
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp;
	mtrTemp.x = 0;
	mtrTemp.y = 0;

	PR_COORD coCentre;
	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pCInPickArm->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pCInspOpt->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pruDLPR1[m_lCurGlass].bLoaded || !pruDLPR2[m_lCurGlass].bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "PR Pattern Not Loaded. Operation Abort!");
		return FALSE;
	}

	//if (!pruULGlass1.bLoaded || !pruULGlass2.bLoaded || !pruULCOF1.bLoaded || !pruULCOF2.bLoaded )
	//{
	//	HMIMessageBox(MSG_OK, "SEARCH DL N UL", "InspOpt PR Pattern Not Loaded. Operation Abort!");
	//	return FALSE;
	//}

	if (!m_stInspWH[m_lCurGlass].bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Align Pts Not Learned. Operation Abort!");	
		return FALSE;
	}

	if (m_lCurSlave == WH_2)
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
	}
	else
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
	}

	m_dCurrAlignAngle = 0.0;

	MoveTToStandby(GMP_WAIT);
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	pCInPickArm->MoveZToPRLevel(GMP_WAIT);

	PRS_DisplayVideo(&pruDLPR1[m_lCurGlass]);

	if (!AutoSearchPR1(&pruDLPR1[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Search PR1 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	if (!AutoSearchPR2(&pruDLPR2[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Search PR2 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	PRS_ClearScreen(INSPDL_CAM + m_lCurGlass);
	PRS_DisplayVideo(&pruULGlass1);
	
	UpdateInspPRPosn(m_lCurGlass);
	pCInspOpt->m_lCurGlass = m_lCurGlass;
	if (
		(pCInspOpt->MoveXYToCurrPR1Posn(GMP_WAIT)) != GMP_SUCCESS || 
		(pCInspOpt->MoveZToPRLevel(GMP_WAIT)) != GMP_SUCCESS 
		)
	{
		return FALSE;
	}
	PRS_DrawHomeCursor(INSPUL_CAM, FALSE);

	return TRUE;
}

BOOL CInspWH::SearchDLnGoULPR2Posn()
{
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp;
	mtrTemp.x = 0;
	mtrTemp.y = 0;

	PR_COORD coCentre;
	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pCInPickArm->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pCInspOpt->m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", szMsg);
		return FALSE;
	}

	if (!pruDLPR1[m_lCurGlass].bLoaded || !pruDLPR2[m_lCurGlass].bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "PR Pattern Not Loaded. Operation Abort!");
		return FALSE;
	}

	//if (!pruULGlass1.bLoaded || !pruULGlass2.bLoaded || !pruULCOF1.bLoaded || !pruULCOF2.bLoaded )
	//{
	//	HMIMessageBox(MSG_OK, "SEARCH DL N UL", "InspOpt PR Pattern Not Loaded. Operation Abort!");
	//	return FALSE;
	//}

	if (!m_stInspWH[m_lCurGlass].bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Align Pts Not Learned. Operation Abort!");	
		return FALSE;
	}

	if (m_lCurSlave == WH_2)
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
	}
	else
	{
		pruDLPR1[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pruDLPR2[m_lCurGlass].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
	}

	m_dCurrAlignAngle = 0.0;

	MoveTToStandby(GMP_WAIT);
	pCInPickArm->m_lCurInspWH = m_lCurSlave;
	pCInPickArm->MoveZToPRLevel(GMP_WAIT);

	PRS_DisplayVideo(&pruDLPR1[m_lCurGlass]);

	if (!AutoSearchPR1(&pruDLPR1[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Search PR1 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	if (!AutoSearchPR2(&pruDLPR2[m_lCurGlass]))
	{
		HMIMessageBox(MSG_OK, "SEARCH DL N UL", "Search PR2 Failed!");
		PRS_DisplayText(INSPDL_CAM + m_lCurGlass, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	PRS_ClearScreen(INSPDL_CAM + m_lCurGlass);
	PRS_DisplayVideo(&pruULGlass1);
	
	UpdateInspPRPosn(m_lCurGlass);
	pCInspOpt->m_lCurGlass = m_lCurGlass;
	if (
		(pCInspOpt->MoveXYToCurrPR2Posn(GMP_WAIT)) != GMP_SUCCESS || 
		(pCInspOpt->MoveZToPRLevel(GMP_WAIT)) != GMP_SUCCESS 
		)
	{
		return FALSE;
	}
	PRS_DrawHomeCursor(INSPUL_CAM, FALSE);

	return TRUE;
}

VOID CInspWH::PrintRejectGlassInfo(LONG lGlass)
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	if (HMI_bDebugCal)
	{
		CString szTime = _T("");
		clock_t clkNow;

		clkNow = clock();
		SYSTEMTIME ltime;		// New Command

		// Get time as 64-bit integer.
		GetLocalTime(&ltime);
		szTime.Format("%u-%u-%u %u:%u:%u", 
			ltime.wYear, ltime.wMonth, ltime.wDay, 
			ltime.wHour, ltime.wMinute, ltime.wSecond);

		CString str;
		str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPWH_Reject_Glass_Info_%04u%02u%02u.csv", ltime.wYear, ltime.wMonth, ltime.wDay);

		FILE *fp = fopen(str, "a+");

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			if (ftell(fp) < 10)
			{
				fprintf(fp, "Time\tGlass_Come_From_MBx\tReject_Reason\tX Tol(um)\tY Tol(um)\tAngle Tol(degree)\n");
			}
			fprintf(fp, "%s\t%ld\t%s\t%.2f\t%.2f\t%.2f\n",
					(const char*)szTime,
					(LONG)(2 * m_lCurMBHead + lGlass + 1), 
					(const char*)m_szRejectReason[lGlass],
					pCInspOpt->m_dPlacementTolX,
					pCInspOpt->m_dPlacementTolY,
					pCInspOpt->m_dPlacementTolAng
					);
			fclose(fp);
		}
	
	}
}

LONG CInspWH::HMI_ToggleAlwaysReject(IPC_CServiceMessage &svMsg)
{
	m_bRejectAlways = !m_bRejectAlways;
	return 0;
}

LONG CInspWH::HMI_ToggleMoveT(IPC_CServiceMessage &svMsg)
{
	m_bMoveT = !m_bMoveT;
	return 0;
}

LONG CInspWH::HMI_MoveTest(IPC_CServiceMessage &svMsg)
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	BOOL m_nLastError = GMP_SUCCESS;

	CString szMsg = "";
	CString strLogFile;
	pCHouseKeeping->m_bStopMoveTest = FALSE;

    MTR_POSN mtr1stWHCmdPosn = {0, 0};
    MTR_POSN mtr1stWHEncPosnBeforeDelay = {0, 0};
    MTR_POSN mtr1stWHEncPosnAfterDelay = {0, 0};
    MTR_POSN mtr2ndWHCmdPosn = {0, 0};
    MTR_POSN mtr2ndWHEncPosnBeforeDelay = {0, 0};
    MTR_POSN mtr2ndWHEncPosnAfterDelay = {0, 0};

	m_nLastError = MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
	strLogFile.Format("D:\\sw\\PI9000\\Data\\MoveTest_WH_%ld.txt", m_lCurSlave + 1);
	FILE *fp = fopen(strLogFile, "a+");
	if (fp != NULL) //klocwork fix 20121211
	{
			szMsg.Format("mtr1stWHCmdPosn, mtr1stWHEncPosnBeforeDelay, mtr1stWHEncPosnAfterDelay, mtr2ndWHCmdPosn, mtr2ndWHEncPosnBeforeDelay, mtr2ndWHEncPosnAfterDelay\n");
			fprintf(fp, szMsg);
	}

	for (int i = 0; (!pCHouseKeeping->m_bStopMoveTest && i < 10000); i++ )
	{
		m_nLastError = MoveXY(INSPWH_XY_CTRL_GO_PI_POSN, GMP_WAIT);//move to ul posn
		mtr1stWHCmdPosn.y = m_stMotorY.GetCmdPosn();//get cmd posn
		Sleep(350);
		mtr1stWHEncPosnBeforeDelay.y = m_stMotorY.GetEncPosn();//get enc posn
		Sleep(150);
		mtr1stWHEncPosnAfterDelay.y = m_stMotorY.GetEncPosn();//get enc posn

		m_nLastError = MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);//move to standby
		mtr2ndWHCmdPosn.y = m_stMotorY.GetCmdPosn();//get cmd posn
		Sleep(350);
		mtr2ndWHEncPosnBeforeDelay.y = m_stMotorY.GetEncPosn();//get enc posn
		Sleep(150);
		mtr2ndWHEncPosnAfterDelay.y = m_stMotorY.GetEncPosn();//get enc posn

		if (fp != NULL) //klocwork fix 20121211
		{
				szMsg.Format("%ld, %ld, %ld, %ld, %ld, %ld\n", mtr1stWHCmdPosn.y, mtr1stWHEncPosnBeforeDelay.y, mtr1stWHEncPosnAfterDelay.y, mtr2ndWHCmdPosn.y, mtr2ndWHEncPosnBeforeDelay.y, mtr2ndWHEncPosnAfterDelay.y);
				fprintf(fp, szMsg);
		}

		if ( m_nLastError == GMP_FAIL )
		{
			pCHouseKeeping->m_bStopMoveTest = TRUE;
		}
	}
	fclose(fp);

	return 1;
}

INT CInspWH::Calib2ndEncoderOperation()
{
	ASSERT(m_pAppMod != NULL);
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	CString			szMsg = "";
	ENC_CALIB_DATA	stTempData[PREBOND_T_CALIB_DATA];
	ENC_CALIB_DATA	lPosLmtPosn;
	ENC_CALIB_DATA	lNegLmtPosn;
	LONG			lOffset = 0;
	INT				i = 0;

	PRINT_DATA	stPrintData;

	PRINT_DATA	szPrintTitle;

	for (INT k = 0; k < MAX_PRINT_DATA; k++)
	{
		stPrintData.szTitle[k]	= "";
	}

	stPrintData.szTitle[0]	= "Y Calib";
	stPrintData.szTitle[1]	= "Rough Encoder Posn:";
	stPrintData.szTitle[2]	= "Fine Encoder Posn:";

	INT				nResult = GMP_SUCCESS;

	// Declare the motor encoder calib counts
	for (i = 0; i < PREBOND_T_CALIB_DATA / 2; i++)
	{
		stTempData[PREBOND_T_CALIB_DATA / 2 + i].RotaryPosn = 0;
		stTempData[PREBOND_T_CALIB_DATA / 2 - i].RotaryPosn = 0;
		stTempData[PREBOND_T_CALIB_DATA / 2 + i].LinearPosn = 0;
		stTempData[PREBOND_T_CALIB_DATA / 2 - i].LinearPosn = 0;
	}


	m_stMotorY.MoveAbsolute(m_stMotorY.stProtect.lPosLmt - 1000, SFM_WAIT);
	Sleep(1000);
	lPosLmtPosn.RotaryPosn = m_stMotorY.Get2ndEncPosn();
	lPosLmtPosn.LinearPosn = m_stMotorY.GetEncPosn();

	if (HMI_bDebugSeq)	// 20140826 Yip: Add Debug Message For Pre Bond Theta Calibration
	{
		szMsg.Format("WH%ld Y Calibration Positive Limit (%ld, %ld)", lPosLmtPosn.RotaryPosn, lPosLmtPosn.LinearPosn);
		DisplayMessage(szMsg);
	}

	
	m_stMotorY.MoveAbsolute(m_stMotorY.stProtect.lNegLmt + 1000, SFM_WAIT);
	Sleep(1000);
	lNegLmtPosn.RotaryPosn = stTempData[PREBOND_T_CALIB_DATA - 1].RotaryPosn = m_stMotorY.Get2ndEncPosn();
	lNegLmtPosn.LinearPosn = stTempData[PREBOND_T_CALIB_DATA - 1].LinearPosn = m_stMotorY.GetEncPosn();

	if (HMI_bDebugSeq)	// 20140826 Yip: Add Debug Message For Pre Bond Theta Calibration
	{
		szMsg.Format("WH%ld Y Calibration Negative Limit (%ld, %ld)", lNegLmtPosn.RotaryPosn, lNegLmtPosn.LinearPosn);
		DisplayMessage(szMsg);
	}

	lOffset = DoubleToLong((lPosLmtPosn.LinearPosn - lNegLmtPosn.LinearPosn) / PREBOND_T_CALIB_DATA);

	//SetError(IDS_PB1_T_CALIBRATION_IN_PROGRESS + (1000 * m_lCurSlave));

	for (i = PREBOND_T_CALIB_DATA - 2; i >= 0; i--)
	{
		if (m_stMotorY.MoveRelative(lOffset, SFM_WAIT) == GMP_SUCCESS)
		{
			Sleep(1000);
			m_stMotorY.lHMI_CurrPosn = m_stMotorY.GetEncPosn();
			if (m_lCurSlave == WH_1)
			{
				SetHmiVariable("HMI_lWH1YEncoder");
			}
			else
			{
				SetHmiVariable("HMI_lWH2YEncoder");
			}

			Sleep(30); //20140429
			if (m_bIsOfflineMode)
			{
				stTempData[i].RotaryPosn = lOffset * (PREBOND_T_CALIB_DATA - i);
				stTempData[i].LinearPosn = lOffset * (PREBOND_T_CALIB_DATA - i);
			}
			else
			{
				stTempData[i].RotaryPosn = m_stMotorY.Get2ndEncPosn();
				stTempData[i].LinearPosn = m_stMotorY.GetEncPosn();
			}
		}
		else
		{
			if (HMI_bDebugSeq)	// 20140826 Yip: Add Debug Message For Pre Bond Theta Calibration
			{
				szMsg.Format("WH%ld Y Calibration Failed (Move to Data Sample %d)", (m_lCurSlave + 1), i);
				DisplayMessage(szMsg);
			}
			//CloseAlarm();
			return GMP_FAIL;
		}
	}

	for (i = 0; i < PREBOND_T_CALIB_DATA; i++)
	{
		m_stCalibY[i].LinearPosn = stTempData[i].LinearPosn;
		m_stCalibY[i].RotaryPosn = stTempData[i].RotaryPosn;
		stPrintData.dData[0] = i;
		stPrintData.dData[1] = m_stCalibY[i].RotaryPosn;
		stPrintData.dData[2] = m_stCalibY[i].LinearPosn;
		CString strLogFile;
		strLogFile.Format(/*GetStation().Trim("_") + */" Y Calibration");
		pCHouseKeeping->PrintData(strLogFile, stPrintData);
	}
	m_stMotorY.MoveAbsolute(stTempData[PREBOND_T_CALIB_DATA / 2].LinearPosn, SFM_WAIT);

	//CloseAlarm();
	if (HMI_bDebugSeq)	// 20140826 Yip: Add Debug Message For Pre Bond Theta Calibration
	{
		szMsg.Format("WH%ld Y Calibration Success", (m_lCurSlave + 1));
		DisplayMessage(szMsg);
	}

	return GMP_SUCCESS;
}

LONG CInspWH::HMI_RotaryEncoderCalibration(IPC_CServiceMessage &svMsg)
{
	BOOL	bOpDone = TRUE;
	INT		nResult = GMP_SUCCESS;
	DOUBLE	dAngle	= -15.0;
	CString szTitle, szMsg;
	szTitle.Format("WH%ld Y CALIBRATION", m_lCurSlave + 1);

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, szTitle, szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}

	if (Calib2ndEncoderOperation() != GMP_SUCCESS)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
	}
	else
	{
		
		HMIMessageBox(MSG_OK, "WARNING", "Operation Completed!");
		PrintCalibTData();
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

VOID CInspWH::PrintCalibTData()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");

	PRINT_DATA	stPrintData;
	
	CString strLogFile;

	for (INT i = 0; i < MAX_PRINT_DATA; i++)
	{
		stPrintData.szTitle[i]	= "";
	}

	stPrintData.szTitle[0]	= "Time";
	stPrintData.szTitle[1]	= "Index";
	stPrintData.szTitle[2]	= "Linear";
	stPrintData.szTitle[3]	= "Rotary";

	for (INT i = 0; i < PREBOND_T_CALIB_DATA; i++)
	{
		stPrintData.dData[0] = i;
		stPrintData.dData[1] = m_stCalibY[i].LinearPosn;
		stPrintData.dData[2] = m_stCalibY[i].RotaryPosn;

		strLogFile.Format("Calculation Log\\InspWH_CalibYData_WH_%ld", m_lCurSlave + 1);

		pCHouseKeeping->PrintData(strLogFile, stPrintData);
	}
}

LONG CInspWH::HMI_Find1stEnc2ndEncRelation(IPC_CServiceMessage &svMsg)
{
	BOOL	bOpDone = TRUE;
	INT		nResult = GMP_SUCCESS;
	CString szTitle, szMsg;
	szTitle.Format("WH%ld Y CALIBRATION", m_lCurSlave + 1);

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, szTitle, szMsg);
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}

	Find1stEnc2ndEncRelation();

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

VOID CInspWH::Find1stEnc2ndEncRelation() //Linear
{
	LONG l1stEncCount, l2ndEncCount;
	
	l1stEncCount = m_stMotorY.GetEncPosn();
	l2ndEncCount = m_stMotorY.Get2ndEncPosn();
	
	m_lEncDiff_y = l1stEncCount - l2ndEncCount * 100;//wh1:-1470912139
}

LONG CInspWH::FirstEncToSecondEnc(LONG lMotorCount) //From 1stEnc to 2ndEnc (Linear)
{
	return ((lMotorCount - m_lEncDiff_y)/100);
}

LONG CInspWH::SecondEncToFirstEnc(LONG lMotorCount) //From 2ndEnc to 1stEnc (Linear)
{
	return (m_lEncDiff_y + lMotorCount * 100);
}

LONG CInspWH::HMI_GoCORPosn(IPC_CServiceMessage &svMsg)
{
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	INT nResult = GMP_SUCCESS;
	BOOL	bOpDone = TRUE;
	
	//all go standby
	if (
		//MoveZ(ACF_WH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT) != GMP_SUCCESS || 
		MoveT(INSPWH_T_CTRL_GO_STANDBY, GMP_WAIT) != GMP_SUCCESS ||
		m_stMotorY.MoveAbsolute(SecondEncToFirstEnc((LONG)m_dmtrTableCOR_Neg[m_lCORCamNum][0].y), GMP_WAIT) != GMP_SUCCESS ||
		pCInPickArm->m_stMotorX.MoveAbsolute((LONG)m_dmtrTableCOR_Neg[m_lCORCamNum][0].x, GMP_WAIT) != GMP_SUCCESS
	   )
	{
		nResult = GMP_FAIL;;
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CInspWH::DL1ToUL(LONG lMotorCount) //From INPA to InspOpt (Linear)
{
	m_lULDL1Diff_x = m_mtrULRefPosn.x - m_mtrDL1RefPosn.x * 100;
	return (m_lULDL1Diff_x + lMotorCount * 100);
}

LONG CInspWH::DL2ToUL(LONG lMotorCount) //From INPA to InspOpt (Linear)
{
	m_lULDL2Diff_x = m_mtrULRefPosn.x - m_mtrDL2RefPosn.x * 100;
	return (m_lULDL2Diff_x + lMotorCount * 100);
}

VOID CInspWH::SetDiagnStepsByMicron(ULONG ulSteps)
{
	switch (ulSteps)
	{
	case 0:
		m_lDiagnSteps = 1000;
		break;
	case 1:
		m_lDiagnSteps = 10000;
		break;
	case 2:
		m_lDiagnSteps = 100000;
		break;
	case 3:
		m_lDiagnSteps = 1000000;
		break;
	case 4:
		m_lDiagnSteps = 10000000;
		break;
	case 5:
		m_lDiagnSteps = 100000000;
		break;
	case 6:
		m_lDiagnSteps = 1000000000;
		break;
	default:
		m_lDiagnSteps = 1000000;
	}
}

LONG CInspWH::HMI_SetDiagnStepsByMicron(IPC_CServiceMessage &svMsg)
{
	ULONG ulSteps;
	svMsg.GetMsg(sizeof(ULONG), &ulSteps);

	g_lDiagnSteps = ulSteps;

	SetDiagnStepsByMicron(ulSteps);	
	return 0;
}

LONG CInspWH::HMI_StartSearchDLnULCycle(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;


	m_bStartSearchDLnULTest = TRUE;


	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;

}

#ifdef MB_FORCE_ADC
BOOL CInspWH::GetADCValue(LONG &lForce)
{
	LONG lResult = 0;

	lResult = m_stADCPort.GetDACValue();
	if (lResult != 0)
	{
		lForce = lResult;
		return GMP_SUCCESS;
	}
	else
	{
		return GMP_FAIL;
	}
}

//DOUBLE CInspWH::RawADCToForce(LONG lRawData, DOUBLE dCperLbf)
//{
//	//Assume that the range sol has not change since the ADC is read.
//	//We found that the largest possible value return NU-Motion is 32*1024. So the range return by ADC is +- 32k. But the resolution of ADC is 12bit.
//	//DOUBLE dRangeFactor = 1.0;
//	INT nRange = 2;
//
//	if (dCperLbf == 0.0)
//	{
//		return 0.0;
//	}
//
//	if (m_stMBForceChargeAmpRangeSol.bHMI_Status)
//	{
//		//range II
//		nRange = 2;
//		//dRangeFactor = 1.0;
//	}
//	else
//	{
//		//Range I
//		nRange = 1;
//		//dRangeFactor = 4.0;
//	}
//	
//	//return ((DOUBLE)lRawData / 2048.0 * FORCE_AMP_RANGE_II / dCperLbf / 250.0 * 15.0 * dRangeFactor); //return force unit in lbf
//	return ((DOUBLE)lRawData / (32.0 * 1024.0) * (nRange == 1 ? FORCE_AMP_RANGE_I : FORCE_AMP_RANGE_II ) / dCperLbf); //return force unit in lbf
//}
#endif


