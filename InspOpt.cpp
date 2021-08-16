/////////////////////////////////////////////////////////////////
//	InspOpt.cpp : interface of the CInspOpt class
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

#include "HouseKeeping.h"

#define INSPOPT_EXTERN
#include "InspOpt.h"
#include "InspWH.h"
#include "InPickArm.h"
#include "WH1.h"
#include "WH2.h"
#include "WinEagle.h"
#include "HouseKeeping.h"
#include "SettingFile.h"
#include "TimeChart.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CInspOpt, CPI9000Stn)

CInspOpt::CInspOpt()
{
	// CPI9000Srn
	m_lStnErrorCode		= IDS_INSPOPT_MOT_NOT_SELECTED_ERR;
	m_lModSelectMode	= GLASS_MOD;

	m_szPRTaskStnName	= GetStation().Trim('_');
	//m_lPRStnErrorCode	= IDS_INSPOPT_PR_MOD_NOT_SELECTED;
	m_lPRModSelectMode	= GLASS_MOD;

	INT i = 0;

	// Motor Related
	m_stMotorX.InitMotorInfo("INSPOPT_X", &m_bModSelected);
	m_stMotorZ.InitMotorInfo("INSPOPT_Z", &m_bModSelected);

	m_myMotorList.Add((CMotorInfo*)&m_stMotorX);
	m_myMotorList.Add((CMotorInfo*)&m_stMotorZ);

	m_stMotorX.stAttrib.bIsAbsoluteEnc	= TRUE;			
	m_stMotorZ.stAttrib.bIsAbsoluteEnc	= TRUE;			

	// Si Related
	m_stXLmtSnr		= CSiInfo("InspOptXLmtSnr",		"IOMODULE_IN_PORT_6", BIT_18, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stXLmtSnr);
	m_stZLmtSnr		= CSiInfo("InspOptZLmtSnr",		"IOMODULE_IN_PORT_6", BIT_19, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stZLmtSnr);
	// So Related

	m_mtrStandbyPosn.x			= 0;
	m_mtrStandbyPosn.y			= 0;

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{

		m_stULGlass[i].bAlignPtSet		= FALSE;
		m_stULGlass[i].bBonded			= FALSE;
		m_stULGlass[i].bEnable			= TRUE;
		m_stULGlass[i].bValid			= FALSE;
		m_stULGlass[i].dmtrAlignPt1.x	= 0.0;
		m_stULGlass[i].dmtrAlignPt1.y	= 0.0;
		m_stULGlass[i].dmtrAlignPt2.x	= 0.0;
		m_stULGlass[i].dmtrAlignPt2.y	= 0.0;

		m_stULCOF[i].bAlignPtSet		= FALSE;
		m_stULCOF[i].bBonded			= FALSE;
		m_stULCOF[i].bEnable			= TRUE;
		m_stULCOF[i].bValid				= FALSE;
		m_stULCOF[i].dmtrAlignPt1.x	= 0.0;
		m_stULCOF[i].dmtrAlignPt1.y	= 0.0;
		m_stULCOF[i].dmtrAlignPt2.x	= 0.0;
		m_stULCOF[i].dmtrAlignPt2.y	= 0.0;

		m_dAlignAngle[i]			= 0.0;
		m_mtrAlignCentre[i].x		= 0;
		m_mtrAlignCentre[i].y		= 0;
		m_mtrCurrAlignDelta[i].x	= 0;
		m_mtrCurrAlignDelta[i].y	= 0;
		m_dGlassRefAng[i]			= 0.0;
		m_dmtrAlignRef[i].x			= 0.0;
		m_dmtrAlignRef[i].y			= 0.0;
		m_dAlignRefAngle[i]			= 0.0;

		for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
		{
			m_mtrPR1Posn[j][i].x = 0;
			m_mtrPR1Posn[j][i].y = 0;
			m_mtrPR2Posn[j][i].x = 0;
			m_mtrPR2Posn[j][i].y = 0;
			m_lULPRLevel[j][i]	 = 0;

			m_mtrPrePRPosn[j].x	 = 0;
			m_mtrPrePRPosn[j].y	 = 0;

			m_dmtrPR1Offset[j][i].x = 0.0;
			m_dmtrPR1Offset[j][i].y = 0.0;
			m_dmtrPR2Offset[j][i].x = 0.0;
			m_dmtrPR2Offset[j][i].y = 0.0;
		}

		m_dmtrGlassPR1Posn[i].x = 0.0;
		m_dmtrGlassPR1Posn[i].y = 0.0;
		m_dmtrGlassPR2Posn[i].x = 0.0;
		m_dmtrGlassPR2Posn[i].y = 0.0;
		m_dmtrCOFPR1Posn[i].x = 0.0;
		m_dmtrCOFPR1Posn[i].y = 0.0;
		m_dmtrCOFPR2Posn[i].x = 0.0;
		m_dmtrCOFPR2Posn[i].y = 0.0;

		m_dPlacementOffsetX[MAINBOND_1][i] = 0.0;
		m_dPlacementOffsetY[MAINBOND_1][i] = 0.0;
		m_dPlacementAngle[MAINBOND_1][i] = 0.0;
		m_dPlacementOffsetX[MAINBOND_2][i] = 0.0;
		m_dPlacementOffsetY[MAINBOND_2][i] = 0.0;
		m_dPlacementAngle[MAINBOND_2][i] = 0.0;

	}

	for (INT i = INSP_MB1; i < MAX_NUM_OF_MB_TYPE; i++)
	{
		m_dOffsetX[i] = 0.0;
		m_dOffsetY[i] = 0.0;
		m_dOffsetAng[i] = 0.0;

		m_stdPlacementOffsetX[i].clear();
		m_stdPlacementOffsetY[i].clear();
		m_stdPlacementAngle[i].clear();

		m_stdDisplayOffsetX[i].clear();
		m_stdDisplayOffsetY[i].clear();
		m_stdDisplayAngle[i].clear();

		for (INT j = 0; j < NUM_OF_OFFSET_DISPLAY; j++)
		{
			HMI_dOffsetX[i][j]	= 0.0;
			HMI_dOffsetY[i][j]	= 0.0;
			HMI_dOffsetAng[i][j]	= 0.0;
		}
	}


	m_dMeasuredDistance		= 0.0;
	m_dMeasuredAngle		= 0.0;

	m_bEnableMeasure		= FALSE;
	m_mtrMeasureSetZeroPt.x = 0;
	m_mtrMeasureSetZeroPt.y = 0;

	HMI_bXYControl			= FALSE;

	m_dmtrCalibIncludedAngOffset.x	= 0.0; //20121024
	m_dmtrCalibIncludedAngOffset.y	= 0.0;

	HMI_dUplookSpeedFactor	= 1.0; //20150213
	m_lCurInspWH			= WH_1;
	m_lCurGlass				= GLASS1;
	m_lCurMarkType			= GLASS_TYPE;
	m_lCurPRDelay			= 0;

	m_bDoPR2First			= FALSE;
	m_bGoPrePRPosn			= FALSE;
	m_bMoveINPA				= FALSE;
	m_bMoveWH				= TRUE;
	m_bSearchPlacementCycle = FALSE;

	m_dPlacementTolX		= 0.0;	
	m_dPlacementTolY		= 0.0;	
	m_dPlacementTolAng		= 0.0;	

	m_bFeedBackPlacement	= FALSE;
	
	m_lSkipGlass[MAINBOND_1][GLASS1] = 0; //20161123 number of glass skip for each MB
	m_lSkipGlass[MAINBOND_1][GLASS2] = 0;
	m_lSkipGlass[MAINBOND_2][GLASS1] = 0;
	m_lSkipGlass[MAINBOND_2][GLASS2] = 0;
}

CInspOpt::~CInspOpt()
{
}

BOOL CInspOpt::InitInstance()
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

		pAppMod->m_bInspOptComm	= TRUE;

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

			// Home Motor
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

			pAppMod->m_bInspOptHome	= TRUE;
		}
	}
	else
	{
		pAppMod->m_bInspOptHome	= TRUE;
		pAppMod->m_bInspOptComm	= TRUE;
	}

	return TRUE;
}

INT CInspOpt::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CInspOpt::Operation()
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

VOID CInspOpt::UpdateOutput()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
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

			if (m_bEnableMeasure) //20121015
			{
				DOUBLE dx, dy;
				dx = CountToDistance(m_stMotorX.GetEncPosn() - m_mtrMeasureSetZeroPt.x, m_stMotorX.stAttrib.dDistPerCount);
				dy = CountToDistance(pCurInspWH->m_stMotorY.GetEncPosn() - m_mtrMeasureSetZeroPt.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount);
				m_dMeasuredDistance = sqrt(pow(dy, 2.0) + pow(dx, 2.0));
				m_dMeasuredAngle = calcDisplayAngleDegree(dy, dx);
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

VOID CInspOpt::RegisterVariables()
{
	try
	{
		// -------------------------------------------------------
		// PI9000Stn Variable and Function
		// -------------------------------------------------------
		// -------------------------------------------------------
		RegVariable(GetStation() + _T("bCheckMotor"), &m_bCheckMotor);
		RegVariable(GetStation() + _T("bCheckIO"), &m_bCheckIO);

		RegVariable(GetStation() + _T("bModSelected"), &m_bModSelected);
		RegVariable(GetStation() + _T("bShowModSelected"), &m_bShowModSelected);
		RegVariable(GetStation() + _T("bModError"), &m_bModError);
		
		RegVariable(GetStation() + _T("bDebugSeq"), &HMI_bDebugSeq);
		RegVariable(GetStation() + _T("bDebugCal"), &HMI_bDebugCal);
		RegVariable(GetStation() + _T("bLogMotion"), &HMI_bLogMotion);

		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugSeq"), HMI_SetDebugSeq);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDebugCal"), HMI_SetDebugCal);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetLogMotion"), HMI_SetLogMotion);
		// -------------------------------------------------------
		// CPRTaskStn Variable and Function
		// -------------------------------------------------------
		RegVariable(GetStation() + _T("bPRSelected"), &m_bPRSelected);	

		RegVariable(GetStation() + _T("lPRDelay"), &m_lPRDelay);
		RegVariable(GetStation() + _T("lPRDelay2"), &m_lPRDelay2);
		RegVariable(GetStation() + _T("lCalibPRDelay"), &m_lCalibPRDelay);
		RegVariable(GetStation() + _T("lPRRetryLimit"), &m_lPRRetryLimit);
		RegVariable(GetStation() + _T("lRejectGlassLimit"), &m_lRejectGlassLimit);
		RegVariable(GetStation() + _T("lInspCycle"), &m_lInspCycle);

		RegVariable(GetStation() + _T("ulCurPRU"), &HMI_ulCurPRU);
		RegVariable(GetStation() + _T("ulPRID"), &HMI_ulPRID);
		RegVariable(GetStation() + _T("dPRMatchScore"), &HMI_dPRMatchScore);
		RegVariable(GetStation() + _T("dPRAngleRange"), &HMI_dPRAngleRange);
		RegVariable(GetStation() + _T("dPRPercentVar"), &HMI_dPRPercentVar);
		RegVariable(GetStation() + _T("bShowPRSearch"), &HMI_bShowPRSearch);

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

		//// -------------------------------------------------------
		//// varibales
		//// -------------------------------------------------------		
		RegVariable(GetStation() + _T("ulCurGlass"), &m_lCurGlass);
		RegVariable(GetStation() + _T("ulCurMarkType"), &m_lCurMarkType);
		RegVariable(GetStation() + _T("ulCurWH"), &m_lCurInspWH);
		
		RegVariable(GetStation() + _T("bGlassPR1Loaded"),	&pruULGlass1.bLoaded);
		RegVariable(GetStation() + _T("bGlassPR2Loaded"),	&pruULGlass2.bLoaded);
		RegVariable(GetStation() + _T("bCOFPR1Loaded"),	&pruULCOF1.bLoaded);
		RegVariable(GetStation() + _T("bCOFPR2Loaded"),	&pruULCOF2.bLoaded);
		RegVariable(GetStation() + _T("bGlassAlignPtSet"),	&m_stULGlass[GLASS1].bAlignPtSet);
		RegVariable(GetStation() + _T("bGlassValid"),	&m_stULGlass[GLASS1].bValid);
		RegVariable(GetStation() + _T("bCOFAlignPtSet"),	&m_stULCOF[GLASS1].bAlignPtSet);
		RegVariable(GetStation() + _T("bCOFValid"),	&m_stULCOF[GLASS1].bValid);
		//// hardware
		RegVariable(_T("HMI_b") + GetStation().Trim("_") + _T("XOn"), &m_stMotorX.bHMI_On);
		RegVariable(_T("HMI_b") + GetStation().Trim("_") + _T("ZOn"), &m_stMotorZ.bHMI_On);

		RegVariable(_T("HMI_l") + GetStation().Trim("_") + _T("XEncoder"), &m_stMotorX.lHMI_CurrPosn);
		RegVariable(_T("HMI_l") + GetStation().Trim("_") + _T("ZEncoder"), &m_stMotorZ.lHMI_CurrPosn);

		RegVariable(GetStation() + _T("dXDistPerCount"), &m_stMotorX.stAttrib.dDistPerCount);
		RegVariable(GetStation() + _T("dZDistPerCount"), &m_stMotorZ.stAttrib.dDistPerCount);

		RegVariable(GetStation() + _T("bXYControl"), &HMI_bXYControl);
		
		RegVariable(GetStation() + _T("bCursorControl"), &HMI_bCursorControl);
		RegVariable(GetStation() + _T("bShowDigitalZoom"), &HMI_bShowDigitalZoom);
		RegVariable(GetStation() + _T("lInspectionPt"), &HMI_lInspectionPt);

		RegVariable(GetStation() + _T("dPlacementTolX"), &m_dPlacementTolX);
		RegVariable(GetStation() + _T("dPlacementTolY"), &m_dPlacementTolY);
		RegVariable(GetStation() + _T("dPlacementTolAng"), &m_dPlacementTolAng);

		//snr
		RegVariable(_T("HMI_bInspOptXLmtSnr"), &m_stXLmtSnr.bHMI_Status);
		RegVariable(_T("HMI_bInspOptZLmtSnr"), &m_stZLmtSnr.bHMI_Status);

		RegVariable(GetStation() + _T("bDoPR2First"), &m_bDoPR2First);
		RegVariable(GetStation() + _T("bGoPrePRPosn"), &m_bGoPrePRPosn);
		RegVariable(GetStation() + _T("bMoveINPA"), &m_bMoveINPA);
		RegVariable(GetStation() + _T("bMoveWH"), &m_bMoveWH);
		//Meausement Routine
		RegVariable(GetStation() + _T("dMeasuredDistance"), &m_dMeasuredDistance);
		RegVariable(GetStation() + _T("dMeasuredAngle"), &m_dMeasuredAngle);
		RegVariable(GetStation() + _T("bEnableMeasure"), &m_bEnableMeasure);

		RegVariable(GetStation() + _T("dCalibIncludedAngXOffset"), &m_dmtrCalibIncludedAngOffset.x); //20121024
		RegVariable(GetStation() + _T("dCalibIncludedAngYOffset"), &m_dmtrCalibIncludedAngOffset.y);

		RegVariable(GetStation() + _T("dUplookSpeedFactor"), &HMI_dUplookSpeedFactor); //20150213

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleDoPR2First"), HMI_ToggleDoPR2First);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleGoPrePRPosn"), HMI_ToggleGoPrePRPosn);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleMoveWH"), HMI_ToggleMoveWH);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleMoveINPA"), HMI_ToggleMoveINPA);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPR2PosnAsPR1Posn"), HMI_SetPR2PosnAsPR1Posn);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetModSelected"), HMI_SetModSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeAll"), HMI_HomeAll);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnX"), HMI_PowerOnX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("PowerOnZ"), HMI_PowerOnZ);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommX"), HMI_CommX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("CommZ"), HMI_CommZ);
		
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeX"), HMI_HomeX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("HomeZ"), HMI_HomeZ);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetDiagnSteps"), HMI_SetDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXPos"), HMI_IndexXPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexXNeg"), HMI_IndexXNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZPos"), HMI_IndexZPos);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("IndexZNeg"), HMI_IndexZNeg);

		// Param

		//Measurement routine
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MeasureSetZero"), HMI_MeasureSetZero);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("MeasureEnable"), HMI_MeasureEnable); //20121016

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetCalbIncludedAngXOffset"), HMI_SetCalbIncludedAngXOffset); //20121024
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetCalbIncludedAngYOffset"), HMI_SetCalbIncludedAngYOffset); //20121024
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetCustMoveProf"), HMI_SetCustMoveProf); //20150213

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRSelected"), HMI_SetPRSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("UpdateSetupStatus"), HMI_UpdateSetupStatus);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectWHNum"), HMI_SelectWHNum);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectGlassNum"), HMI_SelectGlassNum);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectMarkType"), HMI_SelectMarkType);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SelectPRU"), HMI_SelectPRU);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRPosn"), HMI_SetPRPosn);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPRLevel"), HMI_SetPRLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp1"), HMI_SearchAp1);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp2"), HMI_SearchAp2);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchCentre"), HMI_SearchCentre);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchCOFPlacement"), HMI_SearchCOFPlacement);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("StartSearchPlacementCycle"), HMI_StartSearchPlacementCycle);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchDLnGoULPR1Posn"), HMI_SearchDLnGoULPR1Posn);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchDLnGoULPR2Posn"), HMI_SearchDLnGoULPR2Posn);
		//m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("StartPRCycleTest"), HMI_StartPRCycleTest);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp1AfterDL"), HMI_SearchAp1AfterDL);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchAp2AfterDL"), HMI_SearchAp2AfterDL);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SearchCentreAfterDL"), HMI_SearchCentreAfterDL);

		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPlacementTolX"), HMI_SetPlacementTolX);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPlacementTolY"), HMI_SetPlacementTolY);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("SetPlacementTolAngle"), HMI_SetPlacementTolAngle);

		RegVariable(GetStation() + _T("bFeedBackPlacement"), &m_bFeedBackPlacement);
		m_comServer.IPC_REG_SERVICE_COMMAND(GetStation() + _T("ToggleFeedBackPlacement"), HMI_ToggleFeedBackPlacement);

		CString szMsg;
		for (INT j = INSP_MB1; j < MAX_NUM_OF_MB_TYPE; j++)
		{
			for (INT i = 0; i < NUM_OF_OFFSET_DISPLAY; i++)
			{
				szMsg.Format("InspDisplay_MB%ldOffsetX%ld", j + 1, i + 1);
				RegVariable(_T(szMsg),			&HMI_dOffsetX[j][i]);
				szMsg.Format("InspDisplay_MB%ldOffsetY%ld", j + 1, i + 1);
				RegVariable(_T(szMsg),			&HMI_dOffsetY[j][i]);
				szMsg.Format("InspDisplay_MB%ldAngle%ld", j + 1, i + 1);
				RegVariable(_T(szMsg),			&HMI_dOffsetAng[j][i]);
			}
		}
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
CMotorInfo &CInspOpt::GetMotorX() 
{
	return m_stMotorX;
}

CMotorInfo &CInspOpt::GetMotorZ() 
{
	return m_stMotorZ;
}

CMotorInfo &CInspOpt::GetMotorY()
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
	return pCurInspWH->m_stMotorY;
}

CMotorInfo &CInspOpt::GetMotorT()
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
	return pCurInspWH->m_stMotorT;
}


/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CInspOpt::PreStartOperation()
{
	m_bModError = FALSE;
	m_lRejectGlassCounter = 0;

	if (!m_bModSelected)
	{	
		INSPOPT_Status = ST_MOD_NOT_SELECTED;
		m_qSubOperation = INSPOPT_STOP_Q;
		m_bStopCycle = TRUE;
	}
	else
	{
		if ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT)) != GMP_SUCCESS)
		{
			Command(glSTOP_COMMAND, TRUE);
			INSPOPT_Status = ST_STOP;
			m_qSubOperation = INSPOPT_STOP_Q;
			m_bStopCycle = TRUE;
		}
		else
		{
			INSPOPT_Status = ST_IDLE;
			m_qSubOperation = INSPOPT_IDLE_Q;
			m_bStopCycle = FALSE;
		}
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		//if(AutoMode == BURN_IN)
		//{
		//	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
		//	{
		//		m_dmtrPR1Offset[j][i].x = 0.0;
		//		m_dmtrPR1Offset[j][i].y = 0.0;
		//		m_dmtrPR2Offset[j][i].x = 0.0;
		//		m_dmtrPR2Offset[j][i].y = 0.0;
		//	}
		//}
		m_dmtrGlassPR1Posn[i].x = 0.0;
		m_dmtrGlassPR1Posn[i].y = 0.0;
		m_dmtrGlassPR2Posn[i].x = 0.0;
		m_dmtrGlassPR2Posn[i].y = 0.0;
		m_dmtrCOFPR1Posn[i].x = 0.0;
		m_dmtrCOFPR1Posn[i].y = 0.0;
		m_dmtrCOFPR2Posn[i].x = 0.0;
		m_dmtrCOFPR2Posn[i].y = 0.0;

		m_dPlacementOffsetX[MAINBOND_1][i] = 0.0;
		m_dPlacementOffsetX[MAINBOND_2][i] = 0.0;
		m_dPlacementOffsetY[MAINBOND_1][i] = 0.0;
		m_dPlacementOffsetY[MAINBOND_2][i] = 0.0;

		m_dPlacementAngle[MAINBOND_1][i] = 0.0;
		m_dPlacementAngle[MAINBOND_2][i] = 0.0;
	}

	Motion(FALSE);
	State(IDLE_Q);

	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), INSPOPT_Status);

	CPI9000Stn::PreStartOperation();
}

VOID CInspOpt::AutoOperation()
{
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bStopCycle = TRUE;
	}

	OperationSequence();

	// cycle completed or motor error or abort requested: end operation immediately
	if (
		(m_bModSelected && m_qSubOperation == INSPOPT_STOP_Q) || 
		(!m_bModSelected && m_bStopCycle) || 
		GMP_SUCCESS != m_nLastError || IsAbort() 
	   )
	{
		if (m_fHardware && m_bModSelected)
		{
			MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		}

		if (GMP_SUCCESS != m_nLastError)
		{
			m_bModError = TRUE;
		}

		INSPOPT_Status = ST_STOP;
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
		m_bStopCycle = FALSE;

		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{

		//	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
		//	{
		//		m_dmtrPR1Offset[j][i].x = 0.0;
		//		m_dmtrPR1Offset[j][i].y = 0.0;
		//		m_dmtrPR2Offset[j][i].x = 0.0;
		//		m_dmtrPR2Offset[j][i].y = 0.0;
		//	}
			m_dmtrGlassPR1Posn[i].x = 0.0;
			m_dmtrGlassPR1Posn[i].y = 0.0;
			m_dmtrGlassPR2Posn[i].x = 0.0;
			m_dmtrGlassPR2Posn[i].y = 0.0;
			m_dmtrCOFPR1Posn[i].x = 0.0;
			m_dmtrCOFPR1Posn[i].y = 0.0;
			m_dmtrCOFPR2Posn[i].x = 0.0;
			m_dmtrCOFPR2Posn[i].y = 0.0;

			m_dPlacementOffsetX[MAINBOND_1][i] = 0.0;
			m_dPlacementOffsetX[MAINBOND_2][i] = 0.0;
			m_dPlacementOffsetY[MAINBOND_1][i] = 0.0;
			m_dPlacementOffsetY[MAINBOND_2][i] = 0.0;

			m_dPlacementAngle[MAINBOND_1][i] = 0.0;
			m_dPlacementAngle[MAINBOND_2][i] = 0.0;
		}

		CPI9000Stn::StopOperation();
	}
}

VOID CInspOpt::CycleOperation()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");

	if (!m_bSearchPlacementCycle)
	{
		State(STOPPING_Q);

		return;
	}

	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND)
	{
		m_bSearchPlacementCycle = FALSE;
	}

	PRINT_DATA	stPrintData;
	PRINT_DATA	szPrintTitle;
	CString		szMsg;

	if (m_bSearchPlacementCycle)
	{
		m_bSearchPlacementCycle = SearchPlacementTest();
	}

	if (!m_bSearchPlacementCycle)
	{
		State(STOPPING_Q);
	}
}

CString CInspOpt::GetSequenceString()
{
	switch (m_qSubOperation)
	{
	case INSPOPT_IDLE_Q:			//0
		return "INSPOPT_IDLE_Q";

	case INSPOPT_WAIT_INSPWH_REQ_1_Q:			//1
		return "INSPOPT_WAIT_INSPWH_REQ_1_Q";

	case INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q:			//2
		return "INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q";

	case INSPOPT_DO_GRAB_COF_PR1_INSP_NO_LATCH_Q:			//3
		return "INSPOPT_DO_GRAB_COF_PR1_INSP_NO_LATCH_Q";

	case INSPOPT_DO_SRCH_COF_PR1_INSP_Q:			//4
		return "INSPOPT_DO_SRCH_COF_PR1_INSP_Q";

	case INSPOPT_PR1_TO_NEXT:			//5
		return "INSPOPT_PR1_TO_NEXT";

	case INSPOPT_WAIT_INSPWH_REQ_2_Q:			//6
		return "INSPOPT_WAIT_INSPWH_REQ_2_Q";

	case INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q:			//7
		return "INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q";

	case INSPOPT_DO_GRAB_COF_PR2_INSP_NO_LATCH_Q:			//8
		return "INSPOPT_DO_GRAB_COF_PR2_INSP_NO_LATCH_Q";

	case INSPOPT_DO_SRCH_COF_PR2_INSP_Q:			//9
		return "INSPOPT_DO_SRCH_COF_PR2_INSP_Q";

	case INSPOPT_PR2_TO_NEXT:			//10
		return "INSPOPT_PR2_TO_NEXT";

	case INSPOPT_GET_NEXT_GLASS_Q:			//11
		return "INSPOPT_GET_NEXT_GLASS_Q";

	case INSPOPT_GO_FIRST_PR1_INSPECTION_Q:		//12
		return "INSPOPT_GO_FIRST_PR1_INSPECTION_Q";

	case INSPOPT_GO_FIRST_PR2_INSPECTION_Q:		//13
		return "INSPOPT_GO_FIRST_PR2_INSPECTION_Q";

	case INSPOPT_WAIT_INSPWH_ACK_Q:				//14
		return "INSPOPT_WAIT_INSPWH_ACK_Q";


	case INSPOPT_STOP_Q:			//99
		return "INSPOPT_STOP_Q";

	}

	return "Hi";
}

VOID CInspOpt::OperationSequence()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szMsg;
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

	PRINT_DATA	stPrintData;

	INT i = 0;

	for (i = 0; i < MAX_PRINT_DATA; i++)
	{
		stPrintData.szTitle[i]	= "";
	}
	
		switch (m_qSubOperation)
		{
		case INSPOPT_IDLE_Q:			//0

			m_lCurPRDelay = m_lPRDelay;
			if (!m_bDoPR2First)
			{
				if (
					//(m_bGoPrePRPosn && ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS)) ||
					(/*!m_bGoPrePRPosn &&*/ ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT)) == GMP_SUCCESS))
					//((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_POSTPR1_POSN, GMP_WAIT)) == GMP_SUCCESS) 
					)
				{
					m_qSubOperation = INSPOPT_WAIT_INSPWH_REQ_1_Q;		// go to 1
				}
			}
			else
			{
				if (
					//(m_bGoPrePRPosn && ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS)) ||
					(/*!m_bGoPrePRPosn &&*/ ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT)) == GMP_SUCCESS))
					//((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_POSTPR2_POSN, GMP_WAIT)) == GMP_SUCCESS) 
					)
				{
					m_qSubOperation = INSPOPT_WAIT_INSPWH_REQ_2_Q;		// go to 6
				}
			}

			break;

		case INSPOPT_WAIT_INSPWH_REQ_1_Q:			//1

			if (INSPWH_Status[WH_1] == ST_STOP && INSPWH_Status[WH_2] == ST_STOP)
			{
				m_qSubOperation = INSPOPT_STOP_Q;	// go to 99
			}
			else if (
					INSPWH_Status[WH_1] == ST_INSP_MB12_REQ ||
					INSPWH_Status[WH_1] == ST_INSP_MB34_REQ 
					)
			{
				m_lCurInspWH = WH_1;

				if(INSPWH_GLASS_Status[WH_1][GLASS1] == ST_READY)
				{
					m_lCurGlass = GLASS1;
				}
				else
				{
					m_lCurGlass = GLASS2;
				}

				switch (INSPWH_Status[WH_1])
				{
					case ST_INSP_MB12_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB1;
						}
						else
						{
							m_lCurMBHead = INSP_MB2;
						}
						break;

					case ST_INSP_MB34_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB3;
						}
						else
						{
							m_lCurMBHead = INSP_MB4;
						}

						break;

				}

				if (
					(!m_bGoPrePRPosn || ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS))
					)
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR1_INSPECTION_Q;		// go to 12				
				}

				if(INSPWH_GLASS_Status[WH_1][GLASS1] != ST_READY && INSPWH_GLASS_Status[WH_1][GLASS2] != ST_READY)
				{
					INSPOPT_Status = ST_INSP_WH1_ACK;
					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;
				}

			}
			else if (
					INSPWH_Status[WH_2] == ST_INSP_MB12_REQ ||
					INSPWH_Status[WH_2] == ST_INSP_MB34_REQ 
					)
			{
				m_lCurInspWH = WH_2;

				if(INSPWH_GLASS_Status[WH_2][GLASS1] == ST_READY)
				{
					m_lCurGlass = GLASS1;
				}
				else
				{
					m_lCurGlass = GLASS2;
				}

				switch (INSPWH_Status[WH_2])
				{
					case ST_INSP_MB12_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB1;
						}
						else
						{
							m_lCurMBHead = INSP_MB2;
						}
						break;

					case ST_INSP_MB34_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB3;
						}
						else
						{
							m_lCurMBHead = INSP_MB4;
						}

						break;

				}

				if (
					(!m_bGoPrePRPosn || ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS))
					)
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR1_INSPECTION_Q;		// go to 12
				}

				if(INSPWH_GLASS_Status[WH_2][GLASS1] != ST_READY && INSPWH_GLASS_Status[WH_2][GLASS2] != ST_READY)
				{
					INSPOPT_Status = ST_INSP_WH2_ACK;
					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;
				}
			}
			break;

		case INSPOPT_GO_FIRST_PR1_INSPECTION_Q:			//12
				
				if(m_lCurInspWH == WH_1)
				{
					INSPOPT_Status = ST_INSP_WH1_ACK;
					pruULCOF1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
					pruULCOF2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
					pruULGlass1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
					pruULGlass2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
				}
				else
				{
					INSPOPT_Status = ST_INSP_WH2_ACK;
					pruULCOF1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
					pruULCOF2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
					pruULGlass1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
					pruULGlass2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
				}

				if (
					((m_nLastError = MoveXYToCurrPR1Posn(GMP_NOWAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS) &&
					(!pCurWH->m_bMoveT || ((m_nLastError = pCurWH->m_stMotorT.MoveAbsolute(pCurWH->m_lTInspPosn[m_lCurGlass], GMP_NOWAIT))) == GMP_SUCCESS) &&
					(SyncX()) == GMP_SUCCESS && 
					(SyncY()) == GMP_SUCCESS && 
					(SyncT()) == GMP_SUCCESS && 
					(SyncZ()) == GMP_SUCCESS 

					)
				{
					bPR1_InspDone = FALSE;
					bPR2_InspDone = FALSE;
					m_qSubOperation = INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q;		// go to 2
				}
				else
				{
					//SETERROR
					WriteHMIMess("InspOpt Move Fail: Move to PR1 Posn Fail!", TRUE);
					INSPOPT_Status = ST_ERROR;
					m_qSubOperation = INSPOPT_STOP_Q;
					m_bModError = TRUE;
				}
			
				break;

		case INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q:			//2

			Sleep(m_lCurPRDelay);
			if (AutoSearchPR1GrabOnly(&pruULGlass1, m_lCurGlass, LOOK_GLASS_TYPE))
			{
				if (AutoSearchPR1ProcessOnly(&pruULGlass1, NULL, m_lCurGlass, LOOK_GLASS_TYPE))
				{
					m_dmtrGlassPR1Posn[m_lCurGlass] = m_dmtrCurrPR1Posn;
					m_dmtrCurrPR1Posn.x = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x + m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].x;	
					m_dmtrCurrPR1Posn.y = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y + m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].y;	
					m_qSubOperation = INSPOPT_DO_GRAB_COF_PR1_INSP_NO_LATCH_Q;		// go to 3
				}
				else
				{
					
					m_lPR1_1_ErrorCnt++;
					pAppMod->m_ulInspOptPRError++;
					pAppMod->m_ulTotalInspWHPRError++;
					DisplayMessage("*********************************************");
					if (m_lCurGlass == GLASS1)
					{
						//SetAlert(IDS_INSPOPT_GLASS1_UL_PR1_ERR);
						SetError(IDS_INSPOPT_GLASS1_UL_PR1_ERR);
						szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q: InspOpt Glass1 UL PR1 Process Error.");
						pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass1 UL PR1 Process Error.") + pruULGlass1.szErrorMessage;
						pruULGlass1.szErrorMessage = "";
					}
					else
					{
						SetError(IDS_INSPOPT_GLASS2_UL_PR1_ERR);
						szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q: InspOpt Glass2 UL PR1 Process Error.");
						pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass2 UL PR1 Process Error.") + pruULGlass1.szErrorMessage;
						pruULGlass1.szErrorMessage = "";
					}
					INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");

					INSPOPT_Status = ST_INSP_REJECT;
					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
				m_lPR1_1_ErrorCnt++;
				pAppMod->m_ulInspOptPRError++;
				pAppMod->m_ulTotalInspWHPRError++;
				DisplayMessage("*********************************************");
				if (m_lCurGlass == GLASS1)
				{
					SetError(IDS_INSPOPT_GLASS1_UL_PR1_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q: InspOpt Glass1 UL PR1 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass1 UL PR1 Grab Error.") + pruULGlass1.szErrorMessage;
					pruULGlass1.szErrorMessage = "";
				}
				else
				{
					SetError(IDS_INSPOPT_GLASS2_UL_PR1_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q: InspOpt Glass2 UL PR1 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass2 UL PR1 Grab Error.") + pruULGlass1.szErrorMessage;
					pruULGlass1.szErrorMessage = "";
				}
				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
				INSPOPT_Status = ST_INSP_REJECT;
				m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
			}

			break;

		case INSPOPT_DO_GRAB_COF_PR1_INSP_NO_LATCH_Q:			//3

			if (AutoSearchPR1GrabOnly(&pruULCOF1, m_lCurGlass, LOOK_COF_TYPE))
			{
				if (
					(m_bDoPR2First /*&& ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS)*/) ||
					((!m_bDoPR2First && (m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS) &&
					(!pCurWH->m_bMoveT || (!m_bDoPR2First && (m_nLastError = pCurWH->m_stMotorT.MoveAbsolute(pCurWH->m_lTInspPosn[m_lCurGlass], GMP_NOWAIT)) == GMP_SUCCESS)) &&
					(!m_bDoPR2First && ((m_nLastError = MoveXYToCurrPR2Posn(GMP_NOWAIT)) == GMP_SUCCESS)) &&
					(SyncX()) == GMP_SUCCESS && 
					(SyncY()) == GMP_SUCCESS && 
					(SyncT()) == GMP_SUCCESS && 
					(SyncZ()) == GMP_SUCCESS )
					//((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_POSTPR2_POSN, SFM_NOWAIT)) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = INSPOPT_DO_SRCH_COF_PR1_INSP_Q;		// go to 4
				}
				else
				{
					WriteHMIMess("InspOpt Move Fail: Move to PR1 Posn Fail!", TRUE);
					INSPOPT_Status = ST_ERROR;
					m_qSubOperation = INSPOPT_STOP_Q;
					m_bModError = TRUE;
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
				m_lPR1_2_ErrorCnt++;
				pAppMod->m_ulInspOptPRError++;
				pAppMod->m_ulTotalInspWHPRError++;
				DisplayMessage("*********************************************");
				if (m_lCurGlass == GLASS1)
				{
					SetError(IDS_INSPOPT_COF1_UL_PR1_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_COF_PR1_INSP_NO_LATCH_Q: InspOpt COF1 UL PR1 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF1 UL PR1 Grab Error.") + pruULCOF1.szErrorMessage;
					pruULGlass1.szErrorMessage = "";
				}
				else
				{
					SetError(IDS_INSPOPT_COF2_UL_PR1_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_COF_PR1_INSP_NO_LATCH_Q: InspOpt COF2 UL PR1 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF2 UL PR1 Grab Error.") + pruULCOF1.szErrorMessage;
					pruULGlass1.szErrorMessage = "";
				}
				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
				INSPOPT_Status = ST_INSP_REJECT;
				m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
			}

			break;

		case INSPOPT_DO_SRCH_COF_PR1_INSP_Q:			//4

			if (AutoSearchPR1ProcessOnly(&pruULCOF1, NULL, m_lCurGlass, LOOK_COF_TYPE))
			{
				m_dmtrCOFPR1Posn[m_lCurGlass] = m_dmtrCurrPR1Posn;
				bPR1_InspDone = TRUE;
				m_qSubOperation = INSPOPT_PR1_TO_NEXT;		// go to 5
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
				m_lPR1_2_ErrorCnt++;
				pAppMod->m_ulInspOptPRError++;
				pAppMod->m_ulTotalInspWHPRError++;
				DisplayMessage("*********************************************");
				if (m_lCurGlass == GLASS1)
				{
					SetError(IDS_INSPOPT_COF1_UL_PR1_ERR);
					szMsg.Format("INSPOPT_DO_SRCH_COF_PR1_INSP_Q: InspOpt COF1 UL PR1 Process Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF1 UL PR1 Grab Error.") + pruULCOF1.szErrorMessage;
					pruULGlass1.szErrorMessage = "";
				}
				else
				{
					SetError(IDS_INSPOPT_COF2_UL_PR1_ERR);
					szMsg.Format("INSPOPT_DO_SRCH_COF_PR1_INSP_Q: InspOpt COF2 UL PR1 Process Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF2 UL PR1 Grab Error.") + pruULCOF1.szErrorMessage;
					pruULGlass1.szErrorMessage = "";
				}
				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
				INSPOPT_Status = ST_INSP_REJECT;
				m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
			}

			break;

		case INSPOPT_PR1_TO_NEXT:			//5

			if (bPR1_InspDone && bPR2_InspDone)
			{
				BOOL bSkipGlass = FALSE; //20161123

				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_INSP_DONE;

				Cal_MB_Offset((GlassNum)m_lCurGlass, m_lCurMBHead);
				MainbondType lMBModule = MAINBOND_1;
				switch (m_lCurMBHead)
				{
				case INSP_MB1:
				case INSP_MB2:
					lMBModule = MAINBOND_1;
					break;
				case INSP_MB3:
				case INSP_MB4:
					lMBModule = MAINBOND_2;
					break;

				default:
					lMBModule = MAINBOND_1;
					break;
				}
				
				if (	m_lSkipGlass[lMBModule][m_lCurGlass] > 0) //20161123
				{
					bSkipGlass = TRUE;
					m_lSkipGlass[lMBModule][m_lCurGlass] -= 1;
				}
				else
				{
					bSkipGlass = FALSE;
					m_lSkipGlass[lMBModule][m_lCurGlass] = 0;
				}

				m_stdDisplayOffsetX[m_lCurMBHead].push_back(m_dPlacementOffsetX[lMBModule][m_lCurGlass]);
				m_stdDisplayOffsetY[m_lCurMBHead].push_back(m_dPlacementOffsetY[lMBModule][m_lCurGlass]);
				m_stdDisplayAngle[m_lCurMBHead].push_back(m_dPlacementAngle[lMBModule][m_lCurGlass]);
				m_stszDisplayError[m_lCurMBHead].push_back("Out of Tolerance");

				while (!m_stdDisplayOffsetX[m_lCurMBHead].empty() && (LONG) m_stdDisplayOffsetX[m_lCurMBHead].size() > NUM_OF_OFFSET_DISPLAY)
				{
					m_stdDisplayOffsetX[m_lCurMBHead].pop_front();
					m_stdDisplayOffsetY[m_lCurMBHead].pop_front();
					m_stdDisplayAngle[m_lCurMBHead].pop_front();
					m_stszDisplayError[m_lCurMBHead].pop_front();
				}
				LONG lSize = (LONG)m_stdDisplayOffsetX[m_lCurMBHead].size();

				if (!IsPlacementTolError((GlassNum)m_lCurGlass, m_lCurMBHead))
				{
					m_stszDisplayError[m_lCurMBHead][lSize - 1] = "Normal";
#if 1 //20161123
					if (m_bFeedBackPlacement && !bSkipGlass)
					{
						szMsg.Format("Add placement offset to queue. MB Head %ld, MB Module %ld, Glass %ld",m_lCurMBHead + 1, lMBModule + 1, m_lCurGlass + 1);
						DisplayMessage(szMsg);
						m_stdPlacementOffsetX[m_lCurMBHead].push_back(m_dPlacementOffsetX[lMBModule][m_lCurGlass]);
						m_stdPlacementOffsetY[m_lCurMBHead].push_back(m_dPlacementOffsetY[lMBModule][m_lCurGlass]);
						m_stdPlacementAngle[m_lCurMBHead].push_back(m_dPlacementAngle[lMBModule][m_lCurGlass]);
					}
#else
					if (m_bFeedBackPlacement)
					{
						szMsg.Format("Add placement offset to queue. MB Head %ld, MB Module %ld, Glass %ld",m_lCurMBHead + 1, lMBModule + 1, m_lCurGlass + 1);
						DisplayMessage(szMsg);
						m_stdPlacementOffsetX[m_lCurMBHead].push_back(m_dPlacementOffsetX[lMBModule][m_lCurGlass]);
						m_stdPlacementOffsetY[m_lCurMBHead].push_back(m_dPlacementOffsetY[lMBModule][m_lCurGlass]);
						m_stdPlacementAngle[m_lCurMBHead].push_back(m_dPlacementAngle[lMBModule][m_lCurGlass]);
					}
#endif
					ULONG ulSize = m_stdPlacementOffsetX[m_lCurMBHead].size();
					szMsg.Format("Size of queue: %u", ulSize);
					DisplayMessage(szMsg);
					if (ulSize >= g_lCalAverageFreq)
					{
						szMsg.Format("Calc Offset Average. Frequency: %ld", g_lCalAverageFreq);
						DisplayMessage(szMsg);
						m_dOffsetX[m_lCurMBHead] = -1 * CalDequeAverage(m_stdPlacementOffsetX[m_lCurMBHead]);
						m_dOffsetY[m_lCurMBHead] = -1 * CalDequeAverage(m_stdPlacementOffsetY[m_lCurMBHead]);
						m_dOffsetAng[m_lCurMBHead] = -1 * CalDequeAverage(m_stdPlacementAngle[m_lCurMBHead]);

						if (m_bFeedBackPlacement)
						{
							szMsg.Format("Feed back offset to COF");
							DisplayMessage(szMsg);
							FeedBackOffsetToCOF(m_lCurMBHead);
						}
						//Send the placement result to COF
						m_stdPlacementOffsetX[m_lCurMBHead].clear();
						m_stdPlacementOffsetY[m_lCurMBHead].clear();
						m_stdPlacementAngle[m_lCurMBHead].clear();
						m_lSkipGlass[lMBModule][m_lCurGlass] = NUM_OF_SKIP_GLASS; //20161123
					}
					m_lINSP_GLASS_OK_Cnt++;
					pAppMod->m_ulUnitsInpected++;
					pAppMod->m_ulTotalUnitsInpected++;


					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
				}
				else
				{
					pAppMod->m_ulGlassTolError++;
					pAppMod->m_ulTotalGlassTolError++;
					if (m_bFeedBackPlacement)
					{
						FeedBackOutOfTolToCOF(lMBModule);	// 20160616
					}
					INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
					INSPOPT_Status = ST_INSP_REJECT;
					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
				}
				PrintPlacementDisplay(m_lCurMBHead);
			}
			else
			{
				if (
					((m_nLastError = SyncX()) == GMP_SUCCESS) &&
					((m_nLastError = SyncT()) == GMP_SUCCESS) &&
					((m_nLastError = SyncZ()) == GMP_SUCCESS) &&
					((m_nLastError = SyncY()) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q;		// go to 7
				}
			}

			break;

		case INSPOPT_WAIT_INSPWH_REQ_2_Q:			//6

			if (INSPWH_Status[WH_1] == ST_STOP && INSPWH_Status[WH_2] == ST_STOP)
			{
				m_qSubOperation = INSPOPT_STOP_Q;	// go to 99
			}
			else if (
					INSPWH_Status[WH_1] == ST_INSP_MB12_REQ ||
					INSPWH_Status[WH_1] == ST_INSP_MB34_REQ 
					)
			{
				m_lCurInspWH = WH_1;

				if(INSPWH_GLASS_Status[WH_1][GLASS2] == ST_READY)
				{
					m_lCurGlass = GLASS2;
				}
				else
				{
					m_lCurGlass = GLASS1;
				}

				switch (INSPWH_Status[WH_1])
				{
					case ST_INSP_MB12_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB1;
						}
						else
						{
							m_lCurMBHead = INSP_MB2;
						}
						break;

					case ST_INSP_MB34_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB3;
						}
						else
						{
							m_lCurMBHead = INSP_MB4;
						}

						break;

				}


				if (
					(!m_bGoPrePRPosn || ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS))
					)
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR2_INSPECTION_Q;		// go to 13				
				}
			}
			else if (
					INSPWH_Status[WH_2] == ST_INSP_MB12_REQ ||
					INSPWH_Status[WH_2] == ST_INSP_MB34_REQ 
					)
			{
				m_lCurInspWH = WH_2;

				if(INSPWH_GLASS_Status[WH_2][GLASS2] == ST_READY)
				{
					m_lCurGlass = GLASS2;
				}
				else
				{
					m_lCurGlass = GLASS1;
				}

				switch (INSPWH_Status[WH_2])
				{
					case ST_INSP_MB12_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB1;
						}
						else
						{
							m_lCurMBHead = INSP_MB2;
						}
						break;

					case ST_INSP_MB34_REQ:
						if(m_lCurGlass == GLASS1)
						{
							m_lCurMBHead = INSP_MB3;
						}
						else
						{
							m_lCurMBHead = INSP_MB4;
						}

						break;

				}


				if (
					(!m_bGoPrePRPosn || ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS))
					)
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR2_INSPECTION_Q;		// go to 13
				}
			}
			break;

		case INSPOPT_GO_FIRST_PR2_INSPECTION_Q:					//13

				if(m_lCurInspWH == WH_1)
				{
					INSPOPT_Status = ST_INSP_WH1_ACK;
					pruULCOF1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
					pruULCOF2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
					pruULGlass1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
					pruULGlass2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
				}
				else
				{
					INSPOPT_Status = ST_INSP_WH2_ACK;
					pruULCOF1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
					pruULCOF2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
					pruULGlass1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
					pruULGlass2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
				}

				if (
					((m_nLastError = MoveXYToCurrPR2Posn(GMP_NOWAIT)) == GMP_SUCCESS) &&
					((m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS) &&
					(!pCurWH->m_bMoveT || (m_nLastError = pCurWH->m_stMotorT.MoveAbsolute(pCurWH->m_lTInspPosn[m_lCurGlass], GMP_NOWAIT)) == GMP_SUCCESS) &&
					(SyncX()) == GMP_SUCCESS && 
					(SyncY()) == GMP_SUCCESS && 
					(SyncT()) == GMP_SUCCESS && 
					(SyncZ()) == GMP_SUCCESS
					)
				{
					bPR1_InspDone = FALSE;
					bPR2_InspDone = FALSE;
					m_qSubOperation = INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q;		// go to 7
				}
				else
				{
					//SETERROR
					WriteHMIMess("InspOpt Move Fail: Move to PR2 Posn Fail!", TRUE);
					INSPOPT_Status = ST_ERROR;
					m_qSubOperation = INSPOPT_STOP_Q;
					m_bModError = TRUE;
				}

				break;

		case INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q:			//7

			Sleep(m_lCurPRDelay);
			if (AutoSearchPR2GrabOnly(&pruULGlass2, m_lCurGlass, LOOK_GLASS_TYPE))
			{
				if (AutoSearchPR2ProcessOnly(&pruULGlass2, NULL, m_lCurGlass, LOOK_GLASS_TYPE))
				{
					m_dmtrGlassPR2Posn[m_lCurGlass] = m_dmtrCurrPR2Posn;
					m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x + m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].x;	
					m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y + m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].y;	
					m_qSubOperation = INSPOPT_DO_GRAB_COF_PR2_INSP_NO_LATCH_Q;		// go to 8
				}
				else
				{
					//INSPOPT_Status = ST_INSP_REJECT;
					m_lPR2_1_ErrorCnt++;
					pAppMod->m_ulInspOptPRError++;
					pAppMod->m_ulTotalInspWHPRError++;

					DisplayMessage("*********************************************");
					if (m_lCurGlass == GLASS1)
					{
						SetError(IDS_INSPOPT_GLASS1_UL_PR2_ERR);
						szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q: InspOpt Glass1 UL PR2 Process Error.");
						pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass1 UL PR2 Process Error.") + pruULGlass2.szErrorMessage;
						pruULGlass2.szErrorMessage = "";
					}
					else
					{
						SetError(IDS_INSPOPT_GLASS2_UL_PR2_ERR);
						szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q: InspOpt Glass2 UL PR2 Process Error.");
						pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass2 UL PR2 Process Error.") + pruULGlass2.szErrorMessage;
						pruULGlass2.szErrorMessage = "";
					}
					INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
					DisplayMessage(szMsg);
					DisplayMessage("*********************************************");
					INSPOPT_Status = ST_INSP_REJECT;
					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
				m_lPR2_1_ErrorCnt++;
				pAppMod->m_ulInspOptPRError++;
				pAppMod->m_ulTotalInspWHPRError++;
				DisplayMessage("*********************************************");
				if (m_lCurGlass == GLASS1)
				{
					SetError(IDS_INSPOPT_GLASS1_UL_PR2_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q: InspOpt Glass1 UL PR2 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass1 UL PR2 Grab Error.") + pruULGlass2.szErrorMessage;
					pruULGlass2.szErrorMessage = "";
				}
				else
				{
					SetError(IDS_INSPOPT_GLASS2_UL_PR2_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_N_SRCH_GLASS_PR2_INSP_Q: InspOpt Glass2 UL PR2 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt Glass2 UL PR2 Grab Error.") + pruULGlass2.szErrorMessage;
					pruULGlass2.szErrorMessage = "";
				}
				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
				INSPOPT_Status = ST_INSP_REJECT;
				m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
			}
			break;

		case INSPOPT_DO_GRAB_COF_PR2_INSP_NO_LATCH_Q:			//8
			
			if (AutoSearchPR2GrabOnly(&pruULCOF2, m_lCurGlass, LOOK_COF_TYPE))
			{
				if (
					(!m_bDoPR2First /*&& ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) == GMP_SUCCESS)*/) ||
					((m_bDoPR2First && (m_nLastError = MoveZToPRLevel(GMP_NOWAIT)) == GMP_SUCCESS) &&
					(!pCurWH->m_bMoveT || (m_bDoPR2First && (m_nLastError = pCurWH->m_stMotorT.MoveAbsolute(pCurWH->m_lTInspPosn[m_lCurGlass], GMP_NOWAIT)) == GMP_SUCCESS)) &&
					(m_bDoPR2First && ((m_nLastError = MoveXYToCurrPR1Posn(GMP_NOWAIT)) == GMP_SUCCESS)) &&
					(SyncX()) == GMP_SUCCESS && 
					(SyncY()) == GMP_SUCCESS && 
					(SyncT()) == GMP_SUCCESS && 
					(SyncZ()) == GMP_SUCCESS)
					//((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_POSTPR1_POSN, SFM_NOWAIT)) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = INSPOPT_DO_SRCH_COF_PR2_INSP_Q;		// go to 9
				}
				else
				{
					WriteHMIMess("InspOpt Move Fail: Move to PR1 Posn Fail!", TRUE);
					INSPOPT_Status = ST_ERROR;
					m_qSubOperation = INSPOPT_STOP_Q;
					m_bModError = TRUE;
				}
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
				m_lPR2_2_ErrorCnt++;
				pAppMod->m_ulInspOptPRError++;
				pAppMod->m_ulTotalInspWHPRError++;
				DisplayMessage("*********************************************");
				if (m_lCurGlass == GLASS1)
				{
					SetError(IDS_INSPOPT_COF1_UL_PR2_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_COF_PR2_INSP_NO_LATCH_Q: InspOpt COF1 UL PR2 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF1 UL PR2 Grab Error.") + pruULCOF2.szErrorMessage;
					pruULCOF2.szErrorMessage = "";
				}
				else
				{
					SetError(IDS_INSPOPT_COF2_UL_PR2_ERR);
					szMsg.Format("INSPOPT_DO_GRAB_COF_PR2_INSP_NO_LATCH_Q: InspOpt COF2 UL PR2 Grab Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF2 UL PR2 Grab Error.") + pruULCOF2.szErrorMessage;
					pruULCOF2.szErrorMessage = "";
				}
				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
				INSPOPT_Status = ST_INSP_REJECT;
				m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
			}
			break;

		case INSPOPT_DO_SRCH_COF_PR2_INSP_Q:			//9

			if (AutoSearchPR2ProcessOnly(&pruULCOF2, NULL, m_lCurGlass, LOOK_COF_TYPE))
			{
				m_dmtrCOFPR2Posn[m_lCurGlass] = m_dmtrCurrPR2Posn;
				bPR2_InspDone = TRUE;
				m_qSubOperation = INSPOPT_PR2_TO_NEXT;		// go to 10
			}
			else
			{
				//INSPOPT_Status = ST_INSP_REJECT;
				m_lPR2_2_ErrorCnt++;
				pAppMod->m_ulInspOptPRError++;
				pAppMod->m_ulTotalInspWHPRError++;
				DisplayMessage("*********************************************");
				if (m_lCurGlass == GLASS1)
				{
					SetError(IDS_INSPOPT_COF1_UL_PR2_ERR);
					szMsg.Format("INSPOPT_DO_SRCH_COF_PR2_INSP_Q: InspOpt COF1 UL PR2 Process Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF1 UL PR2 Process Error.") + pruULCOF2.szErrorMessage;
					pruULCOF2.szErrorMessage = "";
				}
				else
				{
					SetError(IDS_INSPOPT_COF2_UL_PR2_ERR);
					szMsg.Format("INSPOPT_DO_SRCH_COF_PR2_INSP_Q: InspOpt COF2 UL PR2 Process Error.");
					pCurWH->m_szRejectReason[m_lCurGlass] = _T("InspOpt COF2 UL PR2 Process Error.") + pruULCOF2.szErrorMessage;
					pruULCOF2.szErrorMessage = "";
				}
				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
				DisplayMessage(szMsg);
				DisplayMessage("*********************************************");
				INSPOPT_Status = ST_INSP_REJECT;
				m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
			}

			break;

		case INSPOPT_PR2_TO_NEXT:			//10

			if (bPR1_InspDone && bPR2_InspDone)
			{
				BOOL bSkipGlass = FALSE; //20161123

				INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_INSP_DONE;

				Cal_MB_Offset((GlassNum)m_lCurGlass, m_lCurMBHead);
				MainbondType lMBModule = MAINBOND_1;
				switch (m_lCurMBHead)
				{
				case INSP_MB1:
				case INSP_MB2:
					lMBModule = MAINBOND_1;
					break;
				case INSP_MB3:
				case INSP_MB4:
					lMBModule = MAINBOND_2;
					break;

				default:
					lMBModule = MAINBOND_1;
					break;
				}

				if (	m_lSkipGlass[lMBModule][m_lCurGlass] > 0) //20161123
				{
					bSkipGlass = TRUE;
					m_lSkipGlass[lMBModule][m_lCurGlass] -= 1;
				}
				else
				{
					bSkipGlass = FALSE;
					m_lSkipGlass[lMBModule][m_lCurGlass] = 0;
				}

				m_stdDisplayOffsetX[m_lCurMBHead].push_back(m_dPlacementOffsetX[lMBModule][m_lCurGlass]);
				m_stdDisplayOffsetY[m_lCurMBHead].push_back(m_dPlacementOffsetY[lMBModule][m_lCurGlass]);
				m_stdDisplayAngle[m_lCurMBHead].push_back(m_dPlacementAngle[lMBModule][m_lCurGlass]);
				m_stszDisplayError[m_lCurMBHead].push_back("Out of Tolerance");

				while (!m_stdDisplayOffsetX[m_lCurMBHead].empty() && (LONG) m_stdDisplayOffsetX[m_lCurMBHead].size() > NUM_OF_OFFSET_DISPLAY)
				{
					m_stdDisplayOffsetX[m_lCurMBHead].pop_front();
					m_stdDisplayOffsetY[m_lCurMBHead].pop_front();
					m_stdDisplayAngle[m_lCurMBHead].pop_front();
					m_stszDisplayError[m_lCurMBHead].pop_front();
				}
				LONG lSize = (LONG)m_stdDisplayOffsetX[m_lCurMBHead].size();
				if (!IsPlacementTolError((GlassNum)m_lCurGlass, m_lCurMBHead))
				{
					m_stszDisplayError[m_lCurMBHead][lSize - 1] = "Normal";
#if 1 //20161123
					if (m_bFeedBackPlacement && !bSkipGlass)
					{
						szMsg.Format("Add placement offset to queue. MB Head %ld, MB Module %ld, Glass %ld",m_lCurMBHead + 1, lMBModule + 1, m_lCurGlass + 1);
						DisplayMessage(szMsg);
						m_stdPlacementOffsetX[m_lCurMBHead].push_back(m_dPlacementOffsetX[lMBModule][m_lCurGlass]);
						m_stdPlacementOffsetY[m_lCurMBHead].push_back(m_dPlacementOffsetY[lMBModule][m_lCurGlass]);
						m_stdPlacementAngle[m_lCurMBHead].push_back(m_dPlacementAngle[lMBModule][m_lCurGlass]);
					}
#else
					if (m_bFeedBackPlacement)
					{
						szMsg.Format("Add placement offset to queue. MB Head %ld, MB Module %ld, Glass %ld",m_lCurMBHead + 1, lMBModule + 1, m_lCurGlass + 1);
						DisplayMessage(szMsg);
						m_stdPlacementOffsetX[m_lCurMBHead].push_back(m_dPlacementOffsetX[lMBModule][m_lCurGlass]);
						m_stdPlacementOffsetY[m_lCurMBHead].push_back(m_dPlacementOffsetY[lMBModule][m_lCurGlass]);
						m_stdPlacementAngle[m_lCurMBHead].push_back(m_dPlacementAngle[lMBModule][m_lCurGlass]);
					}
#endif
					ULONG ulSize = m_stdPlacementOffsetX[m_lCurMBHead].size();
					szMsg.Format("Size of queue: %u", ulSize);
					DisplayMessage(szMsg);
					if (ulSize >= g_lCalAverageFreq)
					{
						szMsg.Format("Calc Offset Average. Frequency: %ld", g_lCalAverageFreq);
						DisplayMessage(szMsg);
						m_dOffsetX[m_lCurMBHead] = -1 * CalDequeAverage(m_stdPlacementOffsetX[m_lCurMBHead]);
						m_dOffsetY[m_lCurMBHead] = -1 * CalDequeAverage(m_stdPlacementOffsetY[m_lCurMBHead]);
						m_dOffsetAng[m_lCurMBHead] = -1 * CalDequeAverage(m_stdPlacementAngle[m_lCurMBHead]);

						if (m_bFeedBackPlacement)
						{
							szMsg.Format("Feed back offset to COF");
							DisplayMessage(szMsg);
							FeedBackOffsetToCOF(m_lCurMBHead);
						}
						//Send the placement result to COF
						m_stdPlacementOffsetX[m_lCurMBHead].clear();
						m_stdPlacementOffsetY[m_lCurMBHead].clear();
						m_stdPlacementAngle[m_lCurMBHead].clear();
						m_lSkipGlass[lMBModule][m_lCurGlass] = NUM_OF_SKIP_GLASS; //20161123
					}
					m_lINSP_GLASS_OK_Cnt++;
					pAppMod->m_ulUnitsInpected++;
					pAppMod->m_ulTotalUnitsInpected++;


					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
				}
				else
				{
					pAppMod->m_ulGlassTolError++;
					pAppMod->m_ulTotalGlassTolError++;
					if (m_bFeedBackPlacement)
					{
						FeedBackOutOfTolToCOF(lMBModule);		// 20160616
					}
					INSPOPT_Status = ST_INSP_REJECT;
					INSPWH_GLASS_Status[m_lCurInspWH][m_lCurGlass] = ST_ERROR;
					m_qSubOperation = INSPOPT_GET_NEXT_GLASS_Q;		// go to 11
				}
				
				PrintPlacementDisplay(m_lCurMBHead);
			}
			else
			{
				if (
					((m_nLastError = SyncX()) == GMP_SUCCESS) &&
					((m_nLastError = SyncT()) == GMP_SUCCESS) &&
					((m_nLastError = SyncZ()) == GMP_SUCCESS) &&
					((m_nLastError = SyncY()) == GMP_SUCCESS)
					)
				{
					m_qSubOperation = INSPOPT_DO_GRAB_N_SRCH_GLASS_PR1_INSP_Q;		// go to 2
				}
			}

			break;


		case INSPOPT_GET_NEXT_GLASS_Q:			//11

			if((m_lCurGlass == GLASS1) && (INSPWH_GLASS_Status[m_lCurInspWH][GLASS2] == ST_READY))
			{
				m_lCurPRDelay = m_lPRDelay2;
				m_lCurGlass = GLASS2;	//next Glass

				if(m_lCurMBHead == INSP_MB1)
				{
					m_lCurMBHead = INSP_MB2;
				}
				else if(m_lCurMBHead == INSP_MB3)
				{
					m_lCurMBHead = INSP_MB4;
				}

				if(!m_bDoPR2First)
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR1_INSPECTION_Q;	//go to 12
				}
				else
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR2_INSPECTION_Q;	//go to 13
				}
			}
			else if((m_lCurGlass == GLASS2) && (INSPWH_GLASS_Status[m_lCurInspWH][GLASS1] == ST_READY))
			{
				m_lCurPRDelay = m_lPRDelay2;
				m_lCurGlass = GLASS1;	//next Glass

				if(m_lCurMBHead == INSP_MB2)
				{
					m_lCurMBHead = INSP_MB1;
				}
				else if(m_lCurMBHead == INSP_MB4)
				{
					m_lCurMBHead = INSP_MB3;
				}

				if(!m_bDoPR2First)
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR1_INSPECTION_Q;	//go to 12
				}
				else
				{
					m_qSubOperation = INSPOPT_GO_FIRST_PR2_INSPECTION_Q;	//go to 13
				}
			}
			else //no more next Glass
			{
				m_lCurPRDelay = m_lPRDelay;
				if ((m_nLastError = MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_NOWAIT)) == GMP_SUCCESS)
				{
					INSPOPT_Status = ST_INSP_DONE;
					m_qSubOperation = INSPOPT_WAIT_INSPWH_ACK_Q;		// go to 14
				}
			}


			break;

		case INSPOPT_WAIT_INSPWH_ACK_Q:			//14
			
			if (INSPWH_Status[WH_1] == ST_STOP && INSPWH_Status[WH_2] == ST_STOP)
			{
				m_qSubOperation = INSPOPT_STOP_Q;	// go to 99
			}
			else if(INSPWH_Status[m_lCurInspWH] == ST_INSP_DONE)
			{
				INSPOPT_Status = ST_IDLE;
				m_qSubOperation = INSPOPT_IDLE_Q;		// go to 0
			}

			break;

		case INSPOPT_STOP_Q:			//99

			break;

		}


	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			CString szFileName;

			if (INSPOPT_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("%s Go State: %s", GetStnName(), GetSequenceString());
			}
			else
			{
				szMsg.Format("%s Go Stop", GetStnName());
			}
			//DisplayMessage(szMsg);
		}
		UpdateDebugSeq(GetSequenceString(), INSPOPT_Status);
		m_qPreviousSubOperation = m_qSubOperation;

		if (HMI_bDebugSeq)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				stPrintData.szTitle[i]	= "";
			}

			stPrintData.szTitle[0].Format("%ld", GetTickCount());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 1].Format("%s", GetSequenceString());
			stPrintData.szTitle[GetStationID() * MAX_STATION_SEQ_DATA + 2].Format("%ld", INSPOPT_Status);

			LogFile->log(stPrintData.szTitle);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// PR Functions and Calculation and Log
//////////////////////////////////////////////////////////////////////////////////////////////

VOID CInspOpt::Cal_MB_Offset(GlassNum lCurGlass, INSP_MB_TYPE lCurMBHead)
{
	m_lInspCycle++;
	//SetHmiVariable("InspOpt_lInspCycle");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCurInspWH;
	PR_RCOORD rcoPR1PixOffset;
	PR_RCOORD rcoPR2PixOffset;
	PR_RCOORD rcoCenterPixOffset;
	D_MTR_POSN dmtrPR1PosnOffset;
	D_MTR_POSN dmtrPR2PosnOffset;
	D_MTR_POSN dmtrCenterPosnOffset;
	DOUBLE dGlassAng, dCOFAng;
	DOUBLE dGlassDist, dCOFDist;
#ifdef ENCODER_BASED
	D_MTR_POSN dmtrPR1PosnOffsetBefore = {0.0, 0.0};
	D_MTR_POSN dmtrPR2PosnOffsetBefore = {0.0, 0.0};
	D_MTR_POSN dmtrCenterPosnOffsetBefore = {0.0, 0.0};

	D_MTR_POSN dmtrPR1PosnOffsetAfter = {0.0, 0.0};
	D_MTR_POSN dmtrPR2PosnOffsetAfter = {0.0, 0.0};
	D_MTR_POSN dmtrCenterPosnOffsetAfter = {0.0, 0.0};
#endif

	LONG lMBModule = MAINBOND_1;

	if(m_lCurInspWH == WH_1)
	{
		pCurInspWH = pCWH1;
	}
	else
	{
		pCurInspWH = pCWH2;
	}

	switch (lCurMBHead)
	{
	case NO_MB_TYPE:
	case INSP_MB1:
	case INSP_MB2:
		lMBModule = MAINBOND_1;
		break;
	case INSP_MB3:
	case INSP_MB4:
		lMBModule = MAINBOND_2;
		break;
	}
	rcoPR1PixOffset.x = pruULCOF1.rcoDieCentre.x - pruULGlass1.rcoDieCentre.x;
	rcoPR1PixOffset.y = pruULCOF1.rcoDieCentre.y - pruULGlass1.rcoDieCentre.y;
	rcoPR2PixOffset.x = pruULCOF2.rcoDieCentre.x - pruULGlass2.rcoDieCentre.x;
	rcoPR2PixOffset.y = pruULCOF2.rcoDieCentre.y - pruULGlass2.rcoDieCentre.y;
	rcoCenterPixOffset.x = (FLOAT)((rcoPR1PixOffset.x + rcoPR2PixOffset.x) / 2.0);
	rcoCenterPixOffset.y = (FLOAT)((rcoPR1PixOffset.y + rcoPR2PixOffset.y) / 2.0);

	dmtrPR1PosnOffset.x = m_dmtrCOFPR1Posn[lCurGlass].x - m_dmtrGlassPR1Posn[lCurGlass].x;
	dmtrPR1PosnOffset.y = m_dmtrCOFPR1Posn[lCurGlass].y - m_dmtrGlassPR1Posn[lCurGlass].y;
	dmtrPR2PosnOffset.x = m_dmtrCOFPR2Posn[lCurGlass].x - m_dmtrGlassPR2Posn[lCurGlass].x;
	dmtrPR2PosnOffset.y = m_dmtrCOFPR2Posn[lCurGlass].y - m_dmtrGlassPR2Posn[lCurGlass].y;
	dmtrCenterPosnOffset.x = (dmtrPR1PosnOffset.x + dmtrPR2PosnOffset.x) / 2.0;
	dmtrCenterPosnOffset.y = (dmtrPR1PosnOffset.y + dmtrPR2PosnOffset.y) / 2.0;
#ifdef ENCODER_BASED
	dmtrPR1PosnOffsetBefore.x = m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_COF_TYPE].x - m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].x;
	dmtrPR1PosnOffsetBefore.y = m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_COF_TYPE].y - m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].y;
	dmtrPR2PosnOffsetBefore.x = m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_COF_TYPE].x - m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].x;
	dmtrPR2PosnOffsetBefore.y = m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_COF_TYPE].y - m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].y;
	dmtrCenterPosnOffsetBefore.x = (dmtrPR1PosnOffsetBefore.x + dmtrPR2PosnOffsetBefore.x) / 2.0;
	dmtrCenterPosnOffsetBefore.y = (dmtrPR1PosnOffsetBefore.y + dmtrPR2PosnOffsetBefore.y) / 2.0;
//after
	dmtrPR1PosnOffsetAfter.x = m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_COF_TYPE].x - m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].x;
	dmtrPR1PosnOffsetAfter.y = m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_COF_TYPE].y - m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].y;
	dmtrPR2PosnOffsetAfter.x = m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_COF_TYPE].x - m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].x;
	dmtrPR2PosnOffsetAfter.y = m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_COF_TYPE].y - m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].y;
	dmtrCenterPosnOffsetAfter.x = (dmtrPR1PosnOffsetAfter.x + dmtrPR2PosnOffsetAfter.x) / 2.0;
	dmtrCenterPosnOffsetAfter.y = (dmtrPR1PosnOffsetAfter.y + dmtrPR2PosnOffsetAfter.y) / 2.0;
#endif

	dGlassAng = CalAngle(m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE], m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE]);
	dCOFAng = CalAngle(m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_COF_TYPE], m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_COF_TYPE]);

	dGlassDist = CalDistance(m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE], m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE], m_stMotorX.stAttrib.dDistPerCount, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount);
	dCOFDist = CalDistance(m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_COF_TYPE], m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_COF_TYPE], m_stMotorX.stAttrib.dDistPerCount, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount);

	m_dPlacementOffsetX[lMBModule][lCurGlass] = CountToDistance(dmtrCenterPosnOffsetAfter.x, m_stMotorX.stAttrib.dDistPerCount);
	m_dPlacementOffsetY[lMBModule][lCurGlass] = -1 * CountToDistance(dmtrCenterPosnOffsetAfter.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount);
	m_dPlacementAngle[lMBModule][lCurGlass] = -1* (dCOFAng - dGlassAng);

	// SendPlacementOffsetToCOF902();

	//if (HMI_bDebugCal)
	{
		CString str;
		CString szTime = _T("");
		clock_t clkNow;

		clkNow = clock();
		SYSTEMTIME ltime;		// New Command

		// Get time as 64-bit integer.
		GetLocalTime(&ltime);
		szTime.Format("%u-%u-%u %u:%u:%u", 
			ltime.wYear, ltime.wMonth, ltime.wDay, 
			ltime.wHour, ltime.wMinute, ltime.wSecond);
		if (lCurMBHead == NO_MB_TYPE)
		{
			str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPOPT_Glass_Placement_Manual.csv");
		}
		else
		{
			str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPOPT_Glass_Placement_Auto_%04u%02u%02u.csv",ltime.wYear, ltime.wMonth, ltime.wDay);
		}
		FILE *fp = fopen(str, "a+");

		if (fp != NULL)
		{

			fseek(fp, 0, SEEK_END);
			if (ftell(fp) < 10)
			{
				fprintf(fp, "Time\tMB_x\tPR1_PlacementOffsetX(um)(Cmd)\tPR1_PlacementOffsetY(um)(Cmd)\tPR2_PlacementOffsetX(um)(Cmd)\tPR2_PlacementOffsetY(um)(Cmd)\tPR1_PlacementOffsetX(um)(BeforePRGrabEnc)\tPR1_PlacementOffsetY(um)(BeforePRGrabEnc)\tPR2_PlacementOffsetX(um)(BeforePRGrabEnc)\tPR2_PlacementOffsetY(um)(BeforePRGrabEnc)\tPR1_PlacementOffsetX(um)(AfterPRGrabEnc)\tPR1_PlacementOffsetY(um)(AfterPRGrabEnc)\tPR2_PlacementOffsetX(um)(AfterPRGrabEnc)\tPR2_PlacementOffsetY(um)(AfterPRGrabEnc)\tGlass_PRPtDistance(um)\tCOF_PRPtDistance(um)\tGlass_Angle\tCOF_Angle\tPlacementOffsetX(um)\tPlacementOffsetY(um)\tPlacementAngle\tGlassID\tCOF_ID\tInspWH_x\n");
			}
			fprintf(fp, "%s\t%ld\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%s\t%s\t%ld\n",
				(const char*)szTime,
				(LONG)(1 + 2 * lMBModule + lCurGlass),
				CountToDistance(dmtrPR1PosnOffset.x, m_stMotorX.stAttrib.dDistPerCount), 
				-1 * CountToDistance(dmtrPR1PosnOffset.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount),
				CountToDistance(dmtrPR2PosnOffset.x, m_stMotorX.stAttrib.dDistPerCount), 
				-1 * CountToDistance(dmtrPR2PosnOffset.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount),
#ifdef ENCODER_BASED
				CountToDistance(dmtrPR1PosnOffsetBefore.x, m_stMotorX.stAttrib.dDistPerCount), 
				-1 * CountToDistance(dmtrPR1PosnOffsetBefore.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount),
				CountToDistance(dmtrPR2PosnOffsetBefore.x, m_stMotorX.stAttrib.dDistPerCount), 
				-1 * CountToDistance(dmtrPR2PosnOffsetBefore.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount),

				CountToDistance(dmtrPR1PosnOffsetAfter.x, m_stMotorX.stAttrib.dDistPerCount), 
				-1 * CountToDistance(dmtrPR1PosnOffsetAfter.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount),
				CountToDistance(dmtrPR2PosnOffsetAfter.x, m_stMotorX.stAttrib.dDistPerCount), 
				-1 * CountToDistance(dmtrPR2PosnOffsetAfter.y, pCurInspWH->m_stMotorY.stAttrib.dDistPerCount),
#endif
				dGlassDist, dCOFDist,
				dGlassAng, dCOFAng,
				m_dPlacementOffsetX[lMBModule][lCurGlass], m_dPlacementOffsetY[lMBModule][lCurGlass],
				m_dPlacementAngle[lMBModule][lCurGlass],
				pCurInspWH->m_stDeviceID.szGlass_ID[lCurGlass].GetString(),
				pCurInspWH->m_stDeviceID.szCOF_ID[lCurGlass].GetString(),
				m_lCurInspWH + 1);

				fclose(fp);

		}
		
		if (lCurMBHead == NO_MB_TYPE)
		{
			str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPOPT_Glass_PRResult_Manual.csv");
		}
		else
		{
			str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPOPT_Glass_PRResult_Auto_%04u%02u%02u.csv",ltime.wYear, ltime.wMonth, ltime.wDay);
		}
		fp = fopen(str, "a+");

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			if (ftell(fp) < 10)
#ifdef ENCODER_BASED
			{
				fprintf(fp, "Time\tMB_x\tGlass_PR1x(Pix)\tGlass_PR1y(Pix)\tGlass_PR2x(Pix)\tGlass_PR2y(Pix)\tGlass_PR1x(Motor)\tGlass_PR1y(Motor)\tGlass_PR2x(Motor)\tGlass_PR2y(Motor)\tGlass_PR1x(Motor_Before)\tGlass_PR1y(Motor_Before)\tGlass_PR2x(Motor_Before)\tGlass_PR2y(Motor_Before)\tGlass_PR1x(Motor_After)\tGlass_PR1y(Motor_After)\tGlass_PR2x(Motor_After)\tGlass_PR2y(Motor_After)\tCOF_PR1x(Pix)\tCOF_PR1y(Pix)\tCOF_PR2x(Pix)\tCOF_PR2y(Pix)\tCOF_PR1x(Motor)\tCOF_PR1y(Motor)\tCOF_PR2x(Motor)\tCOF_PR2y(Motor)\tCOF_PR1x(Motor_Before)\tCOF_PR1y(Motor_Before)\tCOF_PR2x(Motor_Before)\tCOF_PR2y(Motor_Before)\tCOF_PR1x(Motor_After)\tCOF_PR1y(Motor_After)\tCOF_PR2x(Motor_After)\tCOF_PR2y(Motor_After)\tPR1x(Cmd)\tPR1y(Cmd)\tPR2x(Cmd)\tPR2y(Cmd)\tGlass_PR1x(Enc_Before)\tGlass_PR1y(Enc_Before)\tGlass_PR2x(Enc_Before)\tGlass_PR2y(Enc_Before)\tGlass_PR1x(Enc_After)\tGlass_PR1y(Enc_After)\tGlass_PR2x(Enc_After)\tGlass_PR2y(Enc_After)\tCOF_PR1x(Enc_Before)\tCOF_PR1y(Enc_Before)\tCOF_PR2x(Enc_Before)\tCOF_PR2y(Enc_Before)\tCOF_PR1x(Enc_After)\tCOF_PR1y(Enc_After)\tCOF_PR2x(Enc_After)\tCOF_PR2y(Enc_After)\tGlass_PR1t(Enc_Before)\tGlass_PR1t(Enc_After)\tGlass_PR2t(Enc_Before)\tGlass_PR2t(Enc_After)\tCOF_PR1t(Enc_Before)\tCOF_PR1t(Enc_After)\tCOF_PR2t(Enc_Before)\tCOF_PR2t(Enc_After)\tT Posn(Cmd)\tGlass_PR1z(Enc_Before)\tGlass_PR1z(Enc_After)\tGlass_PR2z(Enc_Before)\tGlass_PR2z(Enc_After)\tCOF_PR1z(Enc_Before)\tCOF_PR1z(Enc_After)\tCOF_PR2z(Enc_Before)\tCOF_PR2z(Enc_After)\tZ Posn(Cmd)\tInspWH_x\n");
			}
			fprintf(fp, "%s\t%ld\t%.2f\t%.2f\t%.2f\t%.2f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.2f\t%.2f\t%.2f\t%.2f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n",

#else
			{
				fprintf(fp, "Time\tMB_x\tGlass_PR1x(Pix)\tGlass_PR1y(Pix)\tGlass_PR2x(Pix)\tGlass_PR2y(Pix)\tGlass_PR1x(Motor)\tGlass_PR1y(Motor)\tGlass_PR2x(Motor)\tGlass_PR2y(Motor)\tCOF_PR1x(Pix)\tCOF_PR1y(Pix)\tCOF_PR2x(Pix)\tCOF_PR2y(Pix)\tCOF_PR1x(Motor)\tCOF_PR1y(Motor)\tCOF_PR2x(Motor)\tCOF_PR2y(Motor)\tInspWH_x\n");
			}
			fprintf(fp, "%s\t%ld\t%.2f\t%.2f\t%.2f\t%.2f\t%.0f\t%.0f\t%.0f\t%.0f\t%.2f\t%.2f\t%.2f\t%.2f\t%.0f\t%.0f\t%.0f\t%.0f\t%ld\n",
#endif
					(const char*)szTime,
					(LONG)(1 + 2 * lMBModule + lCurGlass),
					pruULGlass1.rcoDieCentre.x, pruULGlass1.rcoDieCentre.y, 
					pruULGlass2.rcoDieCentre.x, pruULGlass2.rcoDieCentre.y, 
					m_dmtrGlassPR1Posn[lCurGlass].x, m_dmtrGlassPR1Posn[lCurGlass].y,
					m_dmtrGlassPR2Posn[lCurGlass].x, m_dmtrGlassPR2Posn[lCurGlass].y,
#ifdef ENCODER_BASED
					m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].y,
					m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_GLASS_TYPE].y,

					m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].y,
					m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_GLASS_TYPE].y,
#endif
					pruULCOF1.rcoDieCentre.x, pruULCOF1.rcoDieCentre.y, 
					pruULCOF2.rcoDieCentre.x, pruULCOF2.rcoDieCentre.y, 
					m_dmtrCOFPR1Posn[lCurGlass].x, m_dmtrCOFPR1Posn[lCurGlass].y,
					m_dmtrCOFPR2Posn[lCurGlass].x, m_dmtrCOFPR2Posn[lCurGlass].y,
#ifdef ENCODER_BASED
					m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_COF_TYPE].x, m_dmtrCurrPR1BeforePRPosn[lCurGlass][LOOK_COF_TYPE].y,
					m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_COF_TYPE].x, m_dmtrCurrPR2BeforePRPosn[lCurGlass][LOOK_COF_TYPE].y,

					m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_COF_TYPE].x, m_dmtrCurrPR1AfterPRPosn[lCurGlass][LOOK_COF_TYPE].y,
					m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_COF_TYPE].x, m_dmtrCurrPR2AfterPRPosn[lCurGlass][LOOK_COF_TYPE].y,
#endif
#ifdef ENCODER_BASED
					m_mtrPR1Posn[m_lCurInspWH][lCurGlass].x + DoubleToLong(m_dmtrPR1Offset[m_lCurInspWH][lCurGlass].x), m_mtrPR1Posn[m_lCurInspWH][lCurGlass].y + DoubleToLong(m_dmtrPR1Offset[m_lCurInspWH][lCurGlass].y),
					m_mtrPR2Posn[m_lCurInspWH][lCurGlass].x + DoubleToLong(m_dmtrPR2Offset[m_lCurInspWH][lCurGlass].x), m_mtrPR2Posn[m_lCurInspWH][lCurGlass].y + DoubleToLong(m_dmtrPR2Offset[m_lCurInspWH][lCurGlass].y),

					m_mtrCurrPR1BeforeEncPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_mtrCurrPR1BeforeEncPosn[lCurGlass][LOOK_GLASS_TYPE].y,
					m_mtrCurrPR2BeforeEncPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_mtrCurrPR2BeforeEncPosn[lCurGlass][LOOK_GLASS_TYPE].y,

					m_mtrCurrPR1AfterEncPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_mtrCurrPR1AfterEncPosn[lCurGlass][LOOK_GLASS_TYPE].y,
					m_mtrCurrPR2AfterEncPosn[lCurGlass][LOOK_GLASS_TYPE].x, m_mtrCurrPR2AfterEncPosn[lCurGlass][LOOK_GLASS_TYPE].y,

					m_mtrCurrPR1BeforeEncPosn[lCurGlass][LOOK_COF_TYPE].x, m_mtrCurrPR1BeforeEncPosn[lCurGlass][LOOK_COF_TYPE].y,
					m_mtrCurrPR2BeforeEncPosn[lCurGlass][LOOK_COF_TYPE].x, m_mtrCurrPR2BeforeEncPosn[lCurGlass][LOOK_COF_TYPE].y,

					m_mtrCurrPR1AfterEncPosn[lCurGlass][LOOK_COF_TYPE].x, m_mtrCurrPR1AfterEncPosn[lCurGlass][LOOK_COF_TYPE].y,
					m_mtrCurrPR2AfterEncPosn[lCurGlass][LOOK_COF_TYPE].x, m_mtrCurrPR2AfterEncPosn[lCurGlass][LOOK_COF_TYPE].y,

					m_lPR1BeforeTEncPosn[lCurGlass][LOOK_GLASS_TYPE], m_lPR1AfterTEncPosn[lCurGlass][LOOK_GLASS_TYPE],
					m_lPR2BeforeTEncPosn[lCurGlass][LOOK_GLASS_TYPE], m_lPR2AfterTEncPosn[lCurGlass][LOOK_GLASS_TYPE],

					m_lPR1BeforeTEncPosn[lCurGlass][LOOK_COF_TYPE], m_lPR1AfterTEncPosn[lCurGlass][LOOK_COF_TYPE],
					m_lPR2BeforeTEncPosn[lCurGlass][LOOK_COF_TYPE], m_lPR2AfterTEncPosn[lCurGlass][LOOK_COF_TYPE],
#endif
					pCurInspWH->m_lTInspPosn[lCurGlass],
#ifdef ENCODER_BASED
					m_lPR1BeforeZEncPosn[lCurGlass][LOOK_GLASS_TYPE], m_lPR1AfterZEncPosn[lCurGlass][LOOK_GLASS_TYPE],
					m_lPR2BeforeZEncPosn[lCurGlass][LOOK_GLASS_TYPE], m_lPR2AfterZEncPosn[lCurGlass][LOOK_GLASS_TYPE],

					m_lPR1BeforeZEncPosn[lCurGlass][LOOK_COF_TYPE], m_lPR1AfterZEncPosn[lCurGlass][LOOK_COF_TYPE],
					m_lPR2BeforeZEncPosn[lCurGlass][LOOK_COF_TYPE], m_lPR2AfterZEncPosn[lCurGlass][LOOK_COF_TYPE],
#endif
					m_lULPRLevel[m_lCurInspWH][lCurGlass],
					m_lCurInspWH + 1
					);
					
			fclose(fp);
			
		}

	}
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


INT CInspOpt::SyncX()
{
	return m_stMotorX.Sync();
}

INT CInspOpt::SyncZ()
{
	return m_stMotorZ.Sync();
}

INT CInspOpt::SyncY()
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
	return pCurInspWH->m_stMotorY.Sync();
}

INT CInspOpt::SyncT()
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
	return pCurInspWH->m_stMotorT.Sync();
}

///
//Temp

INT CInspOpt::MoveXYToCurPRPosn(LONG lPRU)
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
	INT nResult = GMP_SUCCESS;
	//switch (m_lCurGlass)
	//{
	//case GLASS1:
	//	nResult = pCurInspWH->MoveXY(INSPWH_XY_CTRL_GO_UL_G1_INSP_POSN, GMP_WAIT);
	//	break;
	//case GLASS2:
	//	nResult = pCurInspWH->MoveXY(INSPWH_XY_CTRL_GO_UL_G2_INSP_POSN, GMP_WAIT);
	//	break;
	//}
	if (nResult == GMP_SUCCESS)
	{
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

INT CInspOpt::MoveXY(INSPOPT_XY_CTRL_SIGNAL ctrl, BOOL bWait)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	INT nResult = GMP_SUCCESS;

	MTR_POSN	stMove;

	stMove.x	= 0;
	stMove.y	= 0;
	if (pAppMod->State() == IDLE_Q)
	{
		if (!m_bModSelected)
		{
			HMIMessageBox(MSG_OK, "WARNING", "InspOpt Module Not Selected. Operation Abort");
			return GMP_FAIL;
		}
	}

	switch (ctrl)
	{
	case INSPOPT_XY_CTRL_GO_STANDBY_POSN:
		stMove.x = m_mtrStandbyPosn.x;
		//stMove.y = m_mtrStandbyPosn.y;	
		break;

	case INSPOPT_XY_CTRL_GO_PREPR_POSN:
		stMove.x = m_mtrPrePRPosn[m_lCurInspWH].x;
		//stMove.y = m_mtrPrePRPosn[m_lCurInspWH].y;	
		break;
		break;
	}

	return MoveAbsoluteX(stMove.x, bWait);
}

INT CInspOpt::MoveZ(INSPOPT_Z_CTRL_SIGNAL ctrl, BOOL bWait)
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

	INT		nResult		= GMP_SUCCESS;


	LONG lTempPosn = 0;

	switch (ctrl)
	{
	case INSPOPT_Z_CTRL_GO_STANDBY_LEVEL:
		lTempPosn = m_lStandbyLevel;
		break;

	case INSPOPT_Z_CTRL_GO_CALIB_PR_LEVEL:
		lTempPosn = m_lPRCalibLevel[m_lCurInspWH];
		break;

	case INSPOPT_Z_CTRL_GO_PR_LEVEL:
		lTempPosn = m_lULPRLevel[m_lCurInspWH][m_lCurGlass];
		break;

	case INSPOPT_Z_CTRL_GO_REF_LEVEL:
		lTempPosn = m_lULRefLevel[m_lCurInspWH];
		break;

	}

	return m_stMotorZ.MoveAbsolute(lTempPosn, GMP_WAIT);
}

INT CInspOpt::MoveXYToCurrPR1Posn(BOOL bWait)
{
	CString szMsg = "";
	//MTR_POSN mtrTemp;

	szMsg.Format("MoveXYToCurrPR1Posn : INSPOPT Glass = %ld, m_dCurrAlignAngle = %.3f", m_lCurGlass + 1, m_dCurrAlignAngle);
	DisplayMessage(szMsg);


	m_dmtrCurrPR1Posn.x = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x + m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].x;	
	m_dmtrCurrPR1Posn.y = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y + m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].y;	

	return (MoveAbsoluteXY((LONG)m_dmtrCurrPR1Posn.x, (LONG)m_dmtrCurrPR1Posn.y, bWait) && MoveZToPRLevel(bWait));
}

INT CInspOpt::MoveXYToCurrPR2Posn(BOOL bWait)
{
	CString szMsg = "";
	//MTR_POSN mtrTemp;

	szMsg.Format("MoveXYToCurrPR2Posn : INSPOPT Glass = %ld, m_dCurrAlignAngle = %.3f", m_lCurGlass + 1, m_dCurrAlignAngle);
	DisplayMessage(szMsg);


	m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x + m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].x;	
	m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y + m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].y;	

	return (MoveAbsoluteXY((LONG)m_dmtrCurrPR2Posn.x, (LONG)m_dmtrCurrPR2Posn.y, bWait) && MoveZToPRLevel(bWait));
}

INT CInspOpt::MoveXYToPRCalibPosn(BOOL bWait)
{
	return MoveAbsoluteXY(m_mtrPRCalibPosn[m_lCurInspWH].x, m_mtrPRCalibPosn[m_lCurInspWH].y, bWait);
}

INT CInspOpt::MoveTToStandby(BOOL bWait)
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
	return pCurInspWH->MoveT(INSPWH_T_CTRL_GO_STANDBY, bWait);
}

INT CInspOpt::MoveZToPRCalibLevel(BOOL bWait)
{
	return m_stMotorZ.MoveAbsolute(m_lPRCalibLevel[m_lCurInspWH], bWait);
}

INT CInspOpt::MoveZToPRLevel(BOOL bWait)
{
	return m_stMotorZ.MoveAbsolute(m_lULPRLevel[m_lCurInspWH][m_lCurGlass], bWait);
	//CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	//if (m_lCurGlass == GLASS1)
	//{
	//	return pCInspWH->MoveZ1(INSPWH_Z_CTRL_GO_UL_PR_LEVEL, bWait);
	//}
	//else if (m_lCurGlass == GLASS2)
	//{
	//	return pCInspWH->MoveZ2(INSPWH_Z_CTRL_GO_UL_PR_LEVEL, bWait);
	//}
	//else 
	//{
	//	DisplayMessage(__FUNCTION__);
	//	SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
	//	return GMP_FAIL;
	//}
}

INT CInspOpt::MoveAbsoluteX(LONG lCntX, BOOL bWait)
{
	INT nResultX = GMP_SUCCESS;

	if (
		(nResultX = m_stMotorX.MoveAbsolute(lCntX, GMP_NOWAIT)) == GMP_SUCCESS 
	   )
	{
		if (bWait)
		{
			nResultX = m_stMotorX.Sync();
		}
	}

	return nResultX;
}

INT CInspOpt::MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait)
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

	INT nResultX = GMP_SUCCESS;
	INT nResultY = GMP_SUCCESS;

	if (
		(nResultX = m_stMotorX.MoveAbsolute(lCntX, GMP_NOWAIT)) == GMP_SUCCESS &&
		(nResultY = pCurInspWH->m_stMotorY.MoveAbsolute(lCntY, GMP_NOWAIT)) == GMP_SUCCESS
	   )
	{
		if (bWait)
		{
			nResultX = m_stMotorX.Sync();
			nResultY = pCurInspWH->m_stMotorY.Sync();
		}
	}

	return nResultX | nResultY;
}

INT CInspOpt::MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait)
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

	INT nResultX = GMP_SUCCESS;
	INT nResultY = GMP_SUCCESS;

	if (
		(nResultX = m_stMotorX.MoveRelative(lCntX, GMP_NOWAIT)) == GMP_SUCCESS &&
		(nResultY = pCurInspWH->m_stMotorY.MoveRelative(lCntY, GMP_NOWAIT)) == GMP_SUCCESS
	   )
	{
		if (bWait)
		{
			nResultX = m_stMotorX.Sync();
			nResultY = pCurInspWH->m_stMotorY.Sync();
		}
	}

	return nResultX | nResultY;
}



INT CInspOpt::SetupGoStandbyPosn()
{
	INT nResult = GMP_SUCCESS;

	MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);

	return nResult;
}

LONG CInspOpt::HMI_PowerOnX(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorX.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CInspOpt::HMI_PowerOnZ(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorZ.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

// Commutate
LONG CInspOpt::HMI_CommX(IPC_CServiceMessage &svMsg)
{
	m_stMotorX.Commutate();

	return 0;
}

LONG CInspOpt::HMI_CommZ(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ.Commutate();

	return 0;
}

// Home
LONG CInspOpt::HMI_HomeX(IPC_CServiceMessage &svMsg)
{
	CString szErrMsg("No_Error");
	m_stMotorX.Home(GMP_WAIT, szErrMsg);
	if (szErrMsg != "No_Error")
	{
		HMIMessageBox(MSG_OK, "Home Error", szErrMsg);
		DisplayMessage(szErrMsg);
		//SetError(IDS_MAINBOND_MTR_HOME_ERR + (m_lCurSlave * CONVERT_TO_MAPPING_MB), szErrMsg);
	}
	return 0;
}

LONG CInspOpt::HMI_HomeZ(IPC_CServiceMessage &svMsg)
{
	CString szErrMsg("No_Error");
	m_stMotorZ.Home(GMP_WAIT, szErrMsg);
	if (szErrMsg != "No_Error")
	{
		HMIMessageBox(MSG_OK, "Home Error", szErrMsg);
		DisplayMessage(szErrMsg);
		//SetError(IDS_MAINBOND_MTR_HOME_ERR + (m_lCurSlave * CONVERT_TO_MAPPING_MB), szErrMsg);
	}

	return 0;
}

VOID CInspOpt::SetDiagnSteps(ULONG ulSteps)
{
	switch (ulSteps)
	{
	case 0:
		m_lDiagnSteps = 10;
		break;
	case 1:
		m_lDiagnSteps = 100;
		break;
	case 2:
		m_lDiagnSteps = 1000;
		break;
	case 3:
		m_lDiagnSteps = 10000;
		break;
	case 4:
		m_lDiagnSteps = 100000;
		break;
	case 5:
		m_lDiagnSteps = 1000000;
		break;
	case 6:
		m_lDiagnSteps = 10000000;
		break;
	default:
		m_lDiagnSteps = 10000;
	}

//#if 1 //debug 20160926
//	m_stdDisplayOffsetX[INSP_MB1].clear();
//	m_stdDisplayOffsetY[INSP_MB1].clear();
//	m_stdDisplayAngle[INSP_MB1].clear();
//	m_stszDisplayError[INSP_MB1].clear();
//	//1
//	m_stdDisplayOffsetX[INSP_MB1].push_back(0.2);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.5);
//	m_stdDisplayAngle[INSP_MB1].push_back(-0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//2
//	m_stdDisplayOffsetX[INSP_MB1].push_back(0.15);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.2);
//	m_stdDisplayAngle[INSP_MB1].push_back(-0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//3
//	m_stdDisplayOffsetX[INSP_MB1].push_back(-1.25);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.75);
//	m_stdDisplayAngle[INSP_MB1].push_back(0.02);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//4
//	m_stdDisplayOffsetX[INSP_MB1].push_back(1.1);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-1.0);
//	m_stdDisplayAngle[INSP_MB1].push_back(-0.02);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//5
//	m_stdDisplayOffsetX[INSP_MB1].push_back(-1.3);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-3.6);
//	m_stdDisplayAngle[INSP_MB1].push_back(0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//6
//	m_stdDisplayOffsetX[INSP_MB1].push_back(-0.45);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.6);
//	m_stdDisplayAngle[INSP_MB1].push_back(0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//7
//	m_stdDisplayOffsetX[INSP_MB1].push_back(0.25);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.05);
//	m_stdDisplayAngle[INSP_MB1].push_back(-0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//8
//	m_stdDisplayOffsetX[INSP_MB1].push_back(-0.95);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.75);
//	m_stdDisplayAngle[INSP_MB1].push_back(0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//9
//	m_stdDisplayOffsetX[INSP_MB1].push_back(0.6);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.05);
//	m_stdDisplayAngle[INSP_MB1].push_back(-0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	//10
//	m_stdDisplayOffsetX[INSP_MB1].push_back(0.1);
//	m_stdDisplayOffsetY[INSP_MB1].push_back(-2.9);
//	m_stdDisplayAngle[INSP_MB1].push_back(-0.01);
//	m_stszDisplayError[INSP_MB1].push_back("OK");
//
//	PrintPlacementDisplay(INSP_MB1);
//#endif
}

PRU &CInspOpt::SelectCurPRU(ULONG ulPRU)
{
	if (m_lCurMarkType == GLASS_TYPE)
	{
		switch (ulPRU)
		{
		case 0: // PR1
			return pruULGlass1;

		case 1:	// PR2
			return pruULGlass2;

		default:
			DisplayMessage("INSP UL SelectCurPRU Error");
			return pruULGlass1;
		}
	}
	else if (m_lCurMarkType == COF_TYPE)
	{
		switch (ulPRU)
		{
		case 0: // PR1
			return pruULCOF1;

		case 1:	// PR2
			return pruULCOF2;

		default:
			DisplayMessage("INSP UL SelectCurPRU Error");
			return pruULCOF1;
		}
	}
	DisplayMessage("INSP UL SelectCurPRU Error");
	return pruULGlass1;
	
}
// ------------------------------------------------------------------------
// HMI Command delcaration
// ------------------------------------------------------------------------
LONG CInspOpt::HMI_SetPRSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	SetPRSelected(bMode);

	return 0;
}

LONG CInspOpt::HMI_SelectWHNum(IPC_CServiceMessage &svMsg)
{
	ULONG ulWHNum;
	svMsg.GetMsg(sizeof(ULONG), &ulWHNum);

	INT nResult = GMP_SUCCESS;
	CString szMsg = "";
	
	m_lCurInspWH	= ulWHNum;
	
	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();

	LogAction(__FUNCTION__);
	LogAction("m_lCurInspWH = %ld\t", m_lCurInspWH);

	return FALSE;
}

LONG CInspOpt::HMI_SelectGlassNum(IPC_CServiceMessage &svMsg)
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

LONG CInspOpt::HMI_SelectMarkType(IPC_CServiceMessage &svMsg)
{
	ULONG ulMarkType;
	svMsg.GetMsg(sizeof(ULONG), &ulMarkType);

	INT nResult = GMP_SUCCESS;
	CString szMsg = "";
	
	m_lCurMarkType	= ulMarkType;
	
	UpdateModuleSetupPosn();
	UpdateModuleSetupLevel();
	UpdateModuleSetupPara();

	LogAction(__FUNCTION__);
	LogAction("m_lCurMarkType = %ld\t", m_lCurMarkType);

	return FALSE;
}
////////////////////////////////////////
//PR Setup Page
////////////////////////////////////////

LONG CInspOpt::HMI_SelectPRU(IPC_CServiceMessage &svMsg)
{
	ULONG ulPRU;
	svMsg.GetMsg(sizeof(ULONG), &ulPRU);
	
	HMI_ulCurPRU = ulPRU;

	SelectPRU(ulPRU);

	PRS_ClearScreen(INSPUL_CAM);
	PRS_DrawHomeCursor(INSPUL_CAM, FALSE);

	return 0;
}
LONG CInspOpt::HMI_UpdateSetupStatus(IPC_CServiceMessage &svMsg)
{
	UpdateSetupStatus();
	return 0;
}

LONG CInspOpt::HMI_SetPR2PosnAsPR1Posn(IPC_CServiceMessage &svMsg)
{
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

	CString szOldValue;
	szOldValue.Format("X1 = %ld, Y1 = %ld, X2 = %ld, Y2 = %ld",
		m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y,
		m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Position?");
	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
		return 0;
	}
	else if (lAnswer == rMSG_CONTINUE)
	{
		m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x;
		m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y;
	}

	CString szNewValue;
	szNewValue.Format("X1 = %ld, Y1 = %ld, X2 = %ld, Y2 = %ld",
		m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y,
		m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y);

	CString szParameter;
	szParameter.Format("INSPOPT PR Position");
	LogParameter(__FUNCTION__, szParameter, szOldValue, szNewValue);

	return 0;
}

LONG CInspOpt::HMI_SetPRPosn(IPC_CServiceMessage &svMsg)
{
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

	CString szOldValue;
	szOldValue.Format("X1 = %ld, Y1 = %ld, X2 = %ld, Y2 = %ld",
		m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y,
		m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Position?");
	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
		return 0;
	}
	else if (lAnswer == rMSG_CONTINUE)
	{
		switch (HMI_ulCurPRU)
		{
		case 0:
			m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x = m_stMotorX.GetEncPosn();
			m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y = pCurWH->m_stMotorY.GetEncPosn();

			m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].x = 0;
			m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].y = 0;
			break;
		case 1:
			m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x = m_stMotorX.GetEncPosn();
			m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y = pCurWH->m_stMotorY.GetEncPosn();

			m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].x = 0;
			m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].y = 0;
			break;
		}
	}

	CString szNewValue;
	szNewValue.Format("X1 = %ld, Y1 = %ld, X2 = %ld, Y2 = %ld",
		m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y,
		m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x, m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y);

	CString szParameter;
	szParameter.Format("INSPOPT PR Position");
	LogParameter(__FUNCTION__, szParameter, szOldValue, szNewValue);

	return 0;
}

LONG CInspOpt::HMI_SetPRLevel(IPC_CServiceMessage &svMsg)
{
	CString szOldValue;
	szOldValue.Format("Z = %ld", m_lULPRLevel[m_lCurInspWH][m_lCurGlass]);

	LONG lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Overwrite Level?");
	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
		return 0;
	}
	else if (lAnswer == rMSG_CONTINUE)
	{
		m_lULPRLevel[m_lCurInspWH][m_lCurGlass] = m_stMotorZ.GetEncPosn();
	}

	CString szNewValue;
	szNewValue.Format("Z = %ld", m_lULPRLevel[m_lCurInspWH][m_lCurGlass]);

	CString szParameter;
	szParameter.Format("INSPOPT WH%ld Glass%ld UL PR Level", m_lCurInspWH + 1, m_lCurGlass + 1);
	LogParameter(__FUNCTION__, szParameter, szOldValue, szNewValue);

	return 0;
}

LONG CInspOpt::HMI_SearchAp1(IPC_CServiceMessage &svMsg)
{
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Search Ap1: SW Error");
		DisplayMessage("***********************************************");
		return 1;
	}
	BOOL bResult = TRUE;

	CString szMsg = "";
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

	if (!pPR1PRU->bLoaded || !pPR2PRU->bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pPR_INFO[m_lCurGlass]->bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	m_dCurrAlignAngle = 0.0;

	if (!AutoSearchPR1(pPR1PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!AutoSearchPR2(pPR2PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	PRS_ClearScreen(INSPUL_CAM);
	
	CalcAlignPt((GlassNum)m_lCurGlass, (PRMarkType)m_lCurMarkType);


	mtrTemp.x = DoubleToLong(m_dmtrCurrPR1Posn.x + m_dmtrCurrAlign1Offset.x);
	mtrTemp.y = DoubleToLong(m_dmtrCurrPR1Posn.y + m_dmtrCurrAlign1Offset.y);

	MoveAbsoluteXY(mtrTemp.x, mtrTemp.y, GMP_WAIT);

	PRS_DrawCross(INSPUL_CAM, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);



	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspOpt::HMI_SearchAp2(IPC_CServiceMessage &svMsg)
{
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Search Ap2: SW Error");
		DisplayMessage("***********************************************");
		return 1;
	}
	BOOL bResult = TRUE;

	CString szMsg = "";
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

	if (!pPR1PRU->bLoaded || !pPR2PRU->bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pPR_INFO[m_lCurGlass]->bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	m_dCurrAlignAngle = 0.0;

	if (!AutoSearchPR1(pPR1PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!AutoSearchPR2(pPR2PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	PRS_ClearScreen(INSPUL_CAM);
	
	CalcAlignPt((GlassNum)m_lCurGlass, (PRMarkType)m_lCurMarkType);


	mtrTemp.x = DoubleToLong(m_dmtrCurrPR2Posn.x + m_dmtrCurrAlign2Offset.x);
	mtrTemp.y = DoubleToLong(m_dmtrCurrPR2Posn.y + m_dmtrCurrAlign2Offset.y);

	MoveAbsoluteXY(mtrTemp.x, mtrTemp.y, GMP_WAIT);

	PRS_DrawCross(INSPUL_CAM, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspOpt::HMI_SearchCentre(IPC_CServiceMessage &svMsg)
{
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Search Center: SW Error");
		DisplayMessage("***********************************************");
		return 1;
	}
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp;
	mtrTemp.x = 0;
	mtrTemp.y = 0;

	PR_RCOORD coCentre;
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

	if (!pPR1PRU->bLoaded || !pPR2PRU->bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pPR_INFO[m_lCurGlass]->bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	m_dCurrAlignAngle = 0.0;

	if (!AutoSearchPR1(pPR1PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!AutoSearchPR2(pPR2PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	PRS_ClearScreen(INSPUL_CAM);
	
	CalcAlignPt((GlassNum)m_lCurGlass, (PRMarkType)m_lCurMarkType);

	MoveAbsoluteXY(DoubleToLong(m_dmtrCurrAlignCentre.x), DoubleToLong(m_dmtrCurrAlignCentre.y), GMP_WAIT);

	PRS_DrawCross(INSPUL_CAM, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspOpt::HMI_SearchAp1AfterDL(IPC_CServiceMessage &svMsg)
{
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
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Search Ap1: SW Error");
		DisplayMessage("***********************************************");
		return 1;
	}
	BOOL bResult = TRUE;

	CString szMsg = "";
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

	if (!pPR1PRU->bLoaded || !pPR2PRU->bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pPR_INFO[m_lCurGlass]->bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	m_dCurrAlignAngle = 0.0;

	pCurWH->m_lCurGlass = m_lCurGlass;
	pCurWH->SearchDLnGoULPR1Posn();

	if (!AutoSearchPR1(pPR1PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!AutoSearchPR2(pPR2PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	PRS_ClearScreen(INSPUL_CAM);
	
	CalcAlignPt((GlassNum)m_lCurGlass, (PRMarkType)m_lCurMarkType);


	mtrTemp.x = DoubleToLong(m_dmtrCurrPR1Posn.x + m_dmtrCurrAlign1Offset.x);
	mtrTemp.y = DoubleToLong(m_dmtrCurrPR1Posn.y + m_dmtrCurrAlign1Offset.y);

	MoveAbsoluteXY(mtrTemp.x, mtrTemp.y, GMP_WAIT);

	PRS_DrawCross(INSPUL_CAM, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		m_dmtrPR1Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR1Offset[m_lCurInspWH][i].y = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].y = 0.0;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspOpt::HMI_SearchAp2AfterDL(IPC_CServiceMessage &svMsg)
{
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
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Search Ap2: SW Error");
		DisplayMessage("***********************************************");
		return 1;
	}
	BOOL bResult = TRUE;

	CString szMsg = "";
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

	if (!pPR1PRU->bLoaded || !pPR2PRU->bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pPR_INFO[m_lCurGlass]->bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	m_dCurrAlignAngle = 0.0;

	pCurWH->m_lCurGlass = m_lCurGlass;
	pCurWH->SearchDLnGoULPR1Posn();

	if (!AutoSearchPR1(pPR1PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!AutoSearchPR2(pPR2PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	PRS_ClearScreen(INSPUL_CAM);
	
	CalcAlignPt((GlassNum)m_lCurGlass, (PRMarkType)m_lCurMarkType);


	mtrTemp.x = DoubleToLong(m_dmtrCurrPR2Posn.x + m_dmtrCurrAlign2Offset.x);
	mtrTemp.y = DoubleToLong(m_dmtrCurrPR2Posn.y + m_dmtrCurrAlign2Offset.y);

	MoveAbsoluteXY(mtrTemp.x, mtrTemp.y, GMP_WAIT);

	PRS_DrawCross(INSPUL_CAM, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		m_dmtrPR1Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR1Offset[m_lCurInspWH][i].y = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].y = 0.0;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspOpt::HMI_SearchCentreAfterDL(IPC_CServiceMessage &svMsg)
{
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
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Search Center: SW Error");
		DisplayMessage("***********************************************");
		return 1;
	}
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp;
	mtrTemp.x = 0;
	mtrTemp.y = 0;

	PR_RCOORD coCentre;
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

	if (!pPR1PRU->bLoaded || !pPR2PRU->bLoaded)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!pPR_INFO[m_lCurGlass]->bAlignPtSet)
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Align Pts Not Learned. Operation Abort!");	
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	m_dCurrAlignAngle = 0.0;

	pCurWH->m_lCurGlass = m_lCurGlass;
	pCurWH->SearchDLnGoULPR1Posn();

	if (!AutoSearchPR1(pPR1PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR1 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	if (!AutoSearchPR2(pPR2PRU))
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "Search PR2 Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		svMsg.InitMessage(sizeof(BOOL), &bResult);
		return 1;
	}

	PRS_ClearScreen(INSPUL_CAM);
	
	CalcAlignPt((GlassNum)m_lCurGlass, (PRMarkType)m_lCurMarkType);

	MoveAbsoluteXY(DoubleToLong(m_dmtrCurrAlignCentre.x), DoubleToLong(m_dmtrCurrAlignCentre.y), GMP_WAIT);

	PRS_DrawCross(INSPUL_CAM, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Succeed!");

	szMsg.Format("Align Distance(MTR) = %.3f", m_dCurrAlignDist);
	WriteHMIMess(szMsg);
	Sleep(350);
	szMsg.Format("Align Angle = %.3f", m_dCurrAlignAngle);
	WriteHMIMess(szMsg);

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		m_dmtrPR1Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR1Offset[m_lCurInspWH][i].y = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].y = 0.0;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspOpt::HMI_SearchCOFPlacement(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;

				CString szMsg = "";
	DOUBLE dProcessTime = 0.0;
	DWORD dwStartTime = GetTickCount();

	SearchPlacementTest();


	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR1GrabOnly(&pruULGlass1): %.5f", dProcessTime);
	DisplayMessage(szMsg);

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;

}

LONG CInspOpt::HMI_StartSearchPlacementCycle(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;


	m_bSearchPlacementCycle = TRUE;


	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;

}

LONG CInspOpt::HMI_ToggleDoPR2First(IPC_CServiceMessage &svMsg)
{
	m_bDoPR2First = !m_bDoPR2First;
	return 0;
}

LONG CInspOpt::HMI_ToggleGoPrePRPosn(IPC_CServiceMessage &svMsg)
{
	m_bGoPrePRPosn = !m_bGoPrePRPosn;
	return 0;
}

LONG CInspOpt::HMI_ToggleMoveWH(IPC_CServiceMessage &svMsg)
{
	m_bMoveWH = !m_bMoveWH;
	return 0;
}

LONG CInspOpt::HMI_ToggleMoveINPA(IPC_CServiceMessage &svMsg)
{
	m_bMoveINPA = !m_bMoveINPA;
	return 0;
}

LONG CInspOpt::HMI_SetPlacementTolX(IPC_CServiceMessage &svMsg)
{
	DOUBLE dTol;
	svMsg.GetMsg(sizeof(DOUBLE), &dTol);

	m_dPlacementTolX = dTol;
	return 0;
}

LONG CInspOpt::HMI_SetPlacementTolY(IPC_CServiceMessage &svMsg)
{
	DOUBLE dTol;
	svMsg.GetMsg(sizeof(DOUBLE), &dTol);

	m_dPlacementTolY = dTol;
	return 0;
}

LONG CInspOpt::HMI_SetPlacementTolAngle(IPC_CServiceMessage &svMsg)
{
	DOUBLE dTol;
	svMsg.GetMsg(sizeof(DOUBLE), &dTol);

	m_dPlacementTolAng = dTol;
	return 0;
}


VOID CInspOpt::SetupAlignPt()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
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
	
	BOOL		bResult 	= TRUE;
	CString		szAxis 		= "";
	LONG		lAnswer		= rMSG_TIMEOUT;
	static BOOL	bMovePrevPosn = FALSE;
	MTR_POSN	mtrTemp;

	CString		szMsg;
	PRU *pPR1PRU, *pPR2PRU;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Setup Align Pt: SW Error");
		DisplayMessage("***********************************************");
		bResult = FALSE;
	}

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
			m_lCurGlass = GLASS1;

			if (
				pCurInspWH->IndexGlassToTable((GlassNum)m_lCurGlass) != GMP_SUCCESS ||
				!SetGlassRef(m_lCurGlass, TRUE) || 
				MoveAbsoluteXY(DoubleToLong(m_dmtrCurrPR1Posn.x), DoubleToLong(m_dmtrCurrPR1Posn.y), GMP_WAIT) != GMP_SUCCESS 
			   )
			{
				bResult = FALSE;
			}
			else
			{
				//if (rMSG_YES ==  HMIMessageBox(MSG_YES_NO, "SETUP ALIGN PT", "Auto Set Glass1 PR Posn?"))
				//{
				//	m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR1Posn.x);
				//	m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR1Posn.y);
				//	m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR2Posn.x);
				//	m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR2Posn.y);
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
					mtrTemp.x = (LONG)(m_dmtrCurrPR1Posn.x + pPR_INFO[GLASS1]->dmtrAlignPt1.x);
					mtrTemp.y = (LONG)(m_dmtrCurrPR1Posn.y + pPR_INFO[GLASS1]->dmtrAlignPt1.y);
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
				//m_lCurGlass = GLASS2;
				//pCurInspWH->SetVacSol((GlassNum)m_lCurGlass, ON, GMP_WAIT);
				//if (pCurInspWH->IsVacSensorOn((GlassNum)m_lCurGlass))
				//{
				//	if (rMSG_YES ==  HMIMessageBox(MSG_YES_NO, "SETUP ALIGN PT", "Auto Set Glass2 PR Posn?"))
				//	{
				//		if (!SetGlassRef(m_lCurGlass, TRUE))
				//		{
				//			bResult = FALSE;
				//		}
				//		else
				//		{
				//			m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR1Posn.x);
				//			m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR1Posn.y);
				//			m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x = DoubleToLong(m_dmtrCurrPR2Posn.x);
				//			m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y = DoubleToLong(m_dmtrCurrPR2Posn.y);
				//		}
				//	}
				//}
				PRS_DisplayVideo(pPR1PRU);
				PRS_DrawHomeCursor(INSPUL_CAM, FALSE);

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
			pPR_INFO[GLASS1]->bAlignPtSet = FALSE;

			pPR_INFO[GLASS1]->dmtrAlignPt1.x = ((DOUBLE)m_stMotorX.GetEncPosn() - m_dmtrCurrPR1Posn.x);
			pPR_INFO[GLASS1]->dmtrAlignPt1.y = ((DOUBLE)pCurInspWH->m_stMotorY.GetEncPosn() - m_dmtrCurrPR1Posn.y);
		
			if (bMovePrevPosn)
			{
				mtrTemp.x = (LONG)(m_dmtrCurrPR2Posn.x + pPR_INFO[GLASS1]->dmtrAlignPt2.x);
				mtrTemp.y = (LONG)(m_dmtrCurrPR2Posn.y + pPR_INFO[GLASS1]->dmtrAlignPt2.y);
			}
			else
			{
				mtrTemp.x = (LONG)m_dmtrCurrPR2Posn.x;
				mtrTemp.y = (LONG)m_dmtrCurrPR2Posn.y;
			}
			if (
				MoveAbsoluteXY(mtrTemp.x, mtrTemp.y,  GMP_WAIT) != GMP_SUCCESS
				)
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
		pPR_INFO[GLASS1]->dmtrAlignPt2.x = ((DOUBLE)m_stMotorX.GetEncPosn() - m_dmtrCurrPR2Posn.x);
		pPR_INFO[GLASS1]->dmtrAlignPt2.y = ((DOUBLE)pCurInspWH->m_stMotorY.GetEncPosn() - m_dmtrCurrPR2Posn.y);

		CalcAlignRefPt(m_lCurGlass);
		pPR_INFO[GLASS1]->bAlignPtSet = TRUE;

		pPR_INFO[GLASS2]->bAlignPtSet = pPR_INFO[GLASS1]->bAlignPtSet;
		pPR_INFO[GLASS2]->dmtrAlignPt1.x = pPR_INFO[GLASS1]->dmtrAlignPt1.x;
		pPR_INFO[GLASS2]->dmtrAlignPt1.y = pPR_INFO[GLASS1]->dmtrAlignPt1.y;
		pPR_INFO[GLASS2]->dmtrAlignPt2.x = pPR_INFO[GLASS1]->dmtrAlignPt2.x;
		pPR_INFO[GLASS2]->dmtrAlignPt2.y = pPR_INFO[GLASS1]->dmtrAlignPt2.y;

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

INT CInspOpt::SetGlassRef(LONG lGlass, BOOL bSetAng)
{
	DOUBLE dx,dy;

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

	PRU *pPR1PRU, *pPR2PRU;
	pPR1PRU = &pruULGlass1;
	pPR2PRU = &pruULGlass2;
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR1PRU = &pruULGlass1;
		pPR2PRU = &pruULGlass2;
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR1PRU = &pruULCOF1;
		pPR2PRU = &pruULCOF2;
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Set Glass Ref: SW Error");
		DisplayMessage("***********************************************");
		return 1;
	}

	m_dmtrCurrPR1Posn.x = (DOUBLE)(m_mtrPR1Posn[m_lCurInspWH][lGlass].x + m_dmtrPR1Offset[m_lCurInspWH][lGlass].x);
	m_dmtrCurrPR1Posn.y = (DOUBLE)(m_mtrPR1Posn[m_lCurInspWH][lGlass].y + m_dmtrPR1Offset[m_lCurInspWH][lGlass].y);

	if (
		(MoveAbsoluteXY((LONG)m_dmtrCurrPR1Posn.x, (LONG)m_dmtrCurrPR1Posn.y, GMP_WAIT) != GMP_SUCCESS) //||
		//(pCInspWH->MoveZ((WHZNum)lGlass, INSPWH_Z_CTRL_GO_PR_LEVEL, GMP_WAIT) != GMP_SUCCESS) //20130609
	)
	{
		return FALSE;
	}

	if (!SearchPRCentre(pPR1PRU))
	{
		PRS_DisplayText(pPR1PRU->emCameraNo, 1, PRS_MSG_ROW1, "PR Failed!");
		return FALSE;
	}

	PRS_DrawCross(pPR1PRU->emCameraNo, pPR1PRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	m_dmtrCurrPR1Posn.x = (DOUBLE)m_stMotorX.GetEncPosn();
	m_dmtrCurrPR1Posn.y = (DOUBLE)pCurInspWH->m_stMotorY.GetEncPosn();

	m_dmtrCurrPR2Posn.x = (DOUBLE)(m_mtrPR2Posn[m_lCurInspWH][lGlass].x + m_dmtrPR2Offset[m_lCurInspWH][lGlass].x);
	m_dmtrCurrPR2Posn.y = (DOUBLE)(m_mtrPR2Posn[m_lCurInspWH][lGlass].y + m_dmtrPR2Offset[m_lCurInspWH][lGlass].y);

	if (MoveAbsoluteXY((LONG)m_dmtrCurrPR2Posn.x, (LONG)m_dmtrCurrPR2Posn.y, GMP_WAIT) != GMP_SUCCESS)
	{
		return FALSE;
	}

	if (!SearchPRCentre(pPR2PRU))
	{
		PRS_DisplayText(pPR2PRU->emCameraNo, 1, PRS_MSG_ROW1, "PR Failed!");
		return FALSE;
	}
	
	PRS_DrawCross(pPR2PRU->emCameraNo, pPR2PRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
	
	m_dmtrCurrPR2Posn.x = (DOUBLE)m_stMotorX.GetEncPosn();
	m_dmtrCurrPR2Posn.y = (DOUBLE)pCurInspWH->m_stMotorY.GetEncPosn();

	if (bSetAng)
	{
		dx = 1.0 * (m_dmtrCurrPR2Posn.x - m_dmtrCurrPR1Posn.x);
		dy = 1.0 * (m_dmtrCurrPR2Posn.y - m_dmtrCurrPR1Posn.y);

		if (dx != 0)
		{
			m_dGlassRefAng[lGlass] = (180.0 / PI) * atan((dy / dx));
		}
		else
		{
			m_dGlassRefAng[lGlass] = 0.0;
		}

		CString szMsg = _T("");
		szMsg.Format("Glass Reference Angle Found: %.3f", m_dGlassRefAng[lGlass]);
		WriteHMIMess(szMsg);
		// Debug
		szMsg.Format("PR1 Posn: (%.2f, %.2f), PR2 Posn: (%.2f, %.2f)", m_mtrPR1Posn[m_lCurInspWH][lGlass].x, m_mtrPR1Posn[m_lCurInspWH][lGlass].y, m_mtrPR2Posn[m_lCurInspWH][lGlass].x, m_mtrPR2Posn[m_lCurInspWH][lGlass].y);
		DisplayMessage(szMsg);
	}

	return TRUE;
}

VOID CInspOpt::CalcAlignRefPt(LONG lGlass)
{
	BOOL bResult = TRUE;
	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (m_lCurMarkType == GLASS_TYPE)
	{
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (m_lCurMarkType == COF_TYPE) 
	{
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Calc Align Ref Pt: SW Error");
		DisplayMessage("***********************************************");
		bResult = FALSE;
	}
	if (bResult)
	{
		D_MTR_POSN dmtrCurrAlign1Posn;
		D_MTR_POSN dmtrCurrAlign2Posn;
		DOUBLE dx, dy;

		// current align posn
		dmtrCurrAlign1Posn.x = m_dmtrCurrPR1Posn.x + pPR_INFO[lGlass]->dmtrAlignPt1.x;
		dmtrCurrAlign1Posn.y = m_dmtrCurrPR1Posn.y + pPR_INFO[lGlass]->dmtrAlignPt1.y;

		dmtrCurrAlign2Posn.x = m_dmtrCurrPR2Posn.x + pPR_INFO[lGlass]->dmtrAlignPt2.x;
		dmtrCurrAlign2Posn.y = m_dmtrCurrPR2Posn.y + pPR_INFO[lGlass]->dmtrAlignPt2.y;

		// return align centre
		m_dmtrAlignRef[lGlass].x = (dmtrCurrAlign1Posn.x + dmtrCurrAlign2Posn.x) / 2.0;
		m_dmtrAlignRef[lGlass].y = (dmtrCurrAlign1Posn.y + dmtrCurrAlign2Posn.y) / 2.0;

		// return align angle
		dx = 1.0 * (dmtrCurrAlign2Posn.x - dmtrCurrAlign1Posn.x);
		dy = 1.0 * (dmtrCurrAlign2Posn.y - dmtrCurrAlign1Posn.y);
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
		m_dmtrAlignRef[(lGlass + 1) % MAX_NUM_OF_GLASS].x = m_dmtrAlignRef[lGlass].x;
		m_dmtrAlignRef[(lGlass + 1) % MAX_NUM_OF_GLASS].y = m_dmtrAlignRef[lGlass].y;
	}
}

VOID CInspOpt::CalcAlignPt(GlassNum lGlass,PRMarkType lMarkType)
{
	D_MTR_POSN dmtrCurrAlign1Posn;
	D_MTR_POSN dmtrCurrAlign2Posn;
	
	DOUBLE dx, dy;
	DOUBLE dAngle, dAngleRad, dRefAngleRad;

	INSPWH_INFO *pPR_INFO[MAX_NUM_OF_GLASS];
	pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
	pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	if (lMarkType == GLASS_TYPE)
	{
		pPR_INFO[GLASS1] = &m_stULGlass[GLASS1];
		pPR_INFO[GLASS2] = &m_stULGlass[GLASS2];
	}
	else if (lMarkType == COF_TYPE) 
	{
		pPR_INFO[GLASS1] = &m_stULCOF[GLASS1];
		pPR_INFO[GLASS2] = &m_stULCOF[GLASS2];
	}
	else
	{
		DisplayMessage("***********************************************");
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);				
		DisplayMessage("InspOpt Calc Align Pt: SW Error");
		DisplayMessage("***********************************************");
		return;
	}
	CString str;

	if (lGlass == MAX_NUM_OF_GLASS || lGlass < GLASS1)
	{
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return;
	}

	// if PR not selected, return ref posn
	if (!m_bPRSelected || !pPR_INFO[lGlass]->bAlignPtSet)
	{
		m_dCurrAlignAngle = 0.0;
		m_dCurrAlignDist = 0.0; //20160809
		m_dmtrCurrPR1Posn.x = (DOUBLE)(m_mtrPR1Posn[m_lCurInspWH][lGlass].x + m_dmtrPR1Offset[m_lCurInspWH][lGlass].x);
		m_dmtrCurrPR1Posn.y = (DOUBLE)(m_mtrPR1Posn[m_lCurInspWH][lGlass].y + m_dmtrPR1Offset[m_lCurInspWH][lGlass].y);
		m_dmtrCurrPR2Posn.x = (DOUBLE)(m_mtrPR2Posn[m_lCurInspWH][lGlass].x + m_dmtrPR2Offset[m_lCurInspWH][lGlass].x);
		m_dmtrCurrPR2Posn.y = (DOUBLE)(m_mtrPR2Posn[m_lCurInspWH][lGlass].y + m_dmtrPR2Offset[m_lCurInspWH][lGlass].y);
	}

	dx = 1.0 * (m_dmtrCurrPR2Posn.x - m_dmtrCurrPR1Posn.x);
	dy = 1.0 * (m_dmtrCurrPR2Posn.y - m_dmtrCurrPR1Posn.y);

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
	m_dmtrCurrAlign1Offset.x = (+pPR_INFO[lGlass]->dmtrAlignPt1.x * cos(dRefAngleRad - dAngleRad) 
								+ pPR_INFO[lGlass]->dmtrAlignPt1.y * sin(dRefAngleRad - dAngleRad) 
							   );

	m_dmtrCurrAlign1Offset.y = (-pPR_INFO[lGlass]->dmtrAlignPt1.x * sin(dRefAngleRad - dAngleRad) 
								+ pPR_INFO[lGlass]->dmtrAlignPt1.y * cos(dRefAngleRad - dAngleRad) 
							   );

	m_dmtrCurrAlign2Offset.x = (+pPR_INFO[lGlass]->dmtrAlignPt2.x * cos(dRefAngleRad - dAngleRad) 
								+ pPR_INFO[lGlass]->dmtrAlignPt2.y * sin(dRefAngleRad - dAngleRad) 
							   );

	m_dmtrCurrAlign2Offset.y = (-pPR_INFO[lGlass]->dmtrAlignPt2.x * sin(dRefAngleRad - dAngleRad) 
								+ pPR_INFO[lGlass]->dmtrAlignPt2.y * cos(dRefAngleRad - dAngleRad) 
							   );

	// current align posn
	dmtrCurrAlign1Posn.x = m_dmtrCurrPR1Posn.x + m_dmtrCurrAlign1Offset.x;
	dmtrCurrAlign1Posn.y = m_dmtrCurrPR1Posn.y + m_dmtrCurrAlign1Offset.y;
	dmtrCurrAlign2Posn.x = m_dmtrCurrPR2Posn.x + m_dmtrCurrAlign2Offset.x;
	dmtrCurrAlign2Posn.y = m_dmtrCurrPR2Posn.y + m_dmtrCurrAlign2Offset.y;

	// return align angle
#if 1 //20160826
	dx = 1.0 * (dmtrCurrAlign2Posn.x - dmtrCurrAlign1Posn.x);
	dy = 1.0 * (dmtrCurrAlign2Posn.y - dmtrCurrAlign1Posn.y);
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
		if (lMarkType == GLASS_TYPE)
		{
			str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPOPT_PRPt_WH%ld_Glass%ld_%04u%02u%02u.log", m_lCurInspWH + 1, lGlass + 1, ltime.wYear, ltime.wMonth, ltime.wDay);
		}
		else if (lMarkType == COF_TYPE)
		{
			str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\INSPOPT_PRPt_WH%ld_Glass%ld_COF_%04u%02u%02u.log", m_lCurInspWH + 1, lGlass + 1, ltime.wYear, ltime.wMonth, ltime.wDay);
		}
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
VOID CInspOpt::UpdateSetupStatus()
{
	INT nCurIndex = 0;

	m_stULGlass[GLASS1].bValid = TRUE;
	m_stULGlass[GLASS2].bValid = TRUE;
	m_stULCOF[GLASS1].bValid = TRUE;
	m_stULCOF[GLASS2].bValid = TRUE;

	if (!pruULGlass1.bLoaded || !pruULGlass2.bLoaded || !m_stULGlass[GLASS1].bAlignPtSet)
	{
		m_stULGlass[GLASS1].bValid = FALSE;
		m_stULGlass[GLASS2].bValid = FALSE;
	}
	if (!pruULCOF1.bLoaded || !pruULCOF2.bLoaded || !m_stULCOF[GLASS1].bAlignPtSet)
	{
		m_stULCOF[GLASS1].bValid = FALSE;
		m_stULCOF[GLASS2].bValid = FALSE;
	}
}

VOID CInspOpt::SelectPRU(ULONG ulPRU, BOOL bIsMove) //20121221
{
	CString strMsg;
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspWH *pCInspWH;
	if (m_lCurInspWH == WH_1)
	{
		pCInspWH = pCWH1;
	}
	else
	{
		pCInspWH = pCWH2;
	}

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
		if (!pCInspWH->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInspWH->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			return;
		}
		MoveTToStandby(GMP_WAIT);
		MoveXYToCurPRPosn(ulPRU);
		MoveZToPRLevel(GMP_WAIT);
	}
	UpdateHMIPRButton();
}

VOID CInspOpt::SetupWizard()
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
	CWinEagle *pCWinEagle = (CWinEagle*)m_pModule->GetStation("WinEagle");
	CSettingFile *pCSettingFile = (CSettingFile*)m_pModule->GetStation("SettingFile");

	PRU *pPRU;

	switch (g_ulWizardSubTitle)
	{
	case 0:
		g_szWizardTitle.Format("UL Glass Align Pt Setup Procedure");
		SetHmiVariable("SF_szWizardTitle");
		LogAction("InspOpt: " + g_szWizardTitle);
		pCSettingFile->HMI_bShowIgnoreWindowBtn = FALSE; 
		SetHmiVariable("SF_bShowIgnoreWindowBtn");
		SetupAlignPt();
		break;

	case 1:
		g_szWizardTitle.Format("PR%ld Pattern Setup Procedure", HMI_ulCurPRU + 1);
		LogAction("InspOpt: " + g_szWizardTitle);
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
		LogAction("InspOpt: " + g_szWizardTitle);
		SetHmiVariable("SF_szWizardTitle");
		pPRU = &SelectCurPRU(HMI_ulCurPRU);
		pCWinEagle->SetupPRSrchArea(pPRU);
		break;

	}
}

VOID CInspOpt::SetupPRPatternPreTask(PRU *pPRU)
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
	LONG lAnswer = rMSG_TIMEOUT;
	BOOL bResult = TRUE;
	
	m_dCurrAlignAngle = 0.0;

	switch (g_ulWizardStep)
	{
	case 0:
		if (m_lCurGlass != GLASS1 || m_lCurInspWH != WH_1)
		{
			HMIMessageBox(MSG_OK, "WARNING", "Please use WH1 GLASS1 for the setup!");
			bResult = FALSE;
		}
		else 
		{
			if (
				pCurInspWH->IndexGlassToTable((GlassNum)m_lCurGlass) != GMP_SUCCESS ||
				MoveXYToCurPRPosn(HMI_ulCurPRU) != GMP_SUCCESS ||
				MoveZToPRLevel(m_lCurGlass) != GMP_SUCCESS
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

		for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{
			if (m_lCurMarkType == GLASS_TYPE)
			{
				m_stULGlass[i].bAlignPtSet = FALSE;
			}
			if (m_lCurMarkType == COF_TYPE)
			{
				m_stULCOF[i].bAlignPtSet = FALSE;
			}
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


VOID CInspOpt::SetupPRPatternPostTask()
{
	switch (g_ulWizardStep)
	{
	case 99:
		//pCInspWH->MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		//pCInspWH->MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;
	}
}

LONG CInspOpt::HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg)
{
	ULONG ulSteps;
	svMsg.GetMsg(sizeof(ULONG), &ulSteps);

	SetDiagnSteps(ulSteps);
	return 0;
}

VOID CInspOpt::ModuleSetupPreTask()
{

	m_bCheckMotor		= TRUE;
	m_bCheckIO			= TRUE;
	
	CheckModSelected(TRUE);
	
	SetDiagnSteps(g_lDiagnSteps);

	//UpdateModuleSetupPosn();
	//UpdateModuleSetupLevel();
	//UpdateModuleSetupPara();
}

VOID CInspOpt::ModuleSetupPostTask()
{

	m_bCheckMotor		= FALSE;
	m_bCheckIO			= FALSE;
}

VOID CInspOpt::ModulePRSetupPreTask()
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

	pCurInspWH->m_bCheckMotor		= TRUE;
	pCurInspWH->m_bCheckIO		= TRUE;
	m_bCheckIO = TRUE;
	m_bCheckMotor = TRUE;

	HMI_bXYControl	= TRUE; //20121221

	CheckModSelected(TRUE);
		
	SelectPRU(HMI_ulCurPRU, FALSE); //20121221

	UpdateSetupStatus();

	SetDiagnSteps(g_lDiagnSteps);
}

VOID CInspOpt::ModulePRSetupPostTask()
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

	m_bCheckIO			= FALSE;
	m_bCheckMotor		= FALSE;

	pCurInspWH->m_bCheckMotor		= FALSE;
	pCurInspWH->m_bCheckIO		= FALSE;

	//pCInspWH->MoveZ1(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
	//pCInspWH->MoveZ2(INSPWH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
	MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);

}


LONG CInspOpt::HMI_IndexXPos(IPC_CServiceMessage &svMsg)
{
	//if (m_lDiagnSteps != 10000)
	{
		m_stMotorX.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	}
	//else
	//{
	//	m_stMotorX.MoveRelative(m_lDiagnSteps * 10, GMP_WAIT);
	//}
	return 0;
}

LONG CInspOpt::HMI_IndexXNeg(IPC_CServiceMessage &svMsg)
{

	//if (m_lDiagnSteps != 10000)
	{
		m_stMotorX.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	}
	//else
	//{
	//	m_stMotorX.MoveRelative(-m_lDiagnSteps * 10, GMP_WAIT);
	//}
	return 0;
}

LONG CInspOpt::HMI_IndexZPos(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CInspOpt::HMI_IndexZNeg(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}


LONG CInspOpt::HMI_SetInspectionPt(IPC_CServiceMessage &svMsg)
{
	LONG lOption;
	svMsg.GetMsg(sizeof(LONG), &lOption);

	HMI_lInspectionPt = lOption;

	return 0;
}

LONG CInspOpt::HMI_MeasureSetZero(IPC_CServiceMessage &svMsg)
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
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	if (bMode)
	{
		m_bEnableMeasure = TRUE;

		m_mtrMeasureSetZeroPt.x = m_stMotorX.GetEncPosn();
		m_mtrMeasureSetZeroPt.y = pCurInspWH->m_stMotorY.GetEncPosn();

		m_dMeasuredDistance = 0.0;
		m_dMeasuredAngle = 0.0;
	}
	else
	{
		m_bEnableMeasure = FALSE;
	}

	return 0;
}

LONG CInspOpt::HMI_MeasureEnable(IPC_CServiceMessage &svMsg) //20121016
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

LONG CInspOpt::HMI_SetCalbIncludedAngXOffset(IPC_CServiceMessage &svMsg) //20121024
{
	DOUBLE dOffset;
	svMsg.GetMsg(sizeof(DOUBLE), &dOffset);

	m_dmtrCalibIncludedAngOffset.x = dOffset;

	return 0;
}
LONG CInspOpt::HMI_SetCalbIncludedAngYOffset(IPC_CServiceMessage &svMsg) //20121024
{
	DOUBLE dOffset;
	svMsg.GetMsg(sizeof(DOUBLE), &dOffset);

	m_dmtrCalibIncludedAngOffset.y = dOffset;

	return 0;
}


LONG CInspOpt::HMI_SetCustMoveProf(IPC_CServiceMessage &svMsg) //20150213
{
	DOUBLE dFactor = 1.0;
	svMsg.GetMsg(sizeof(DOUBLE), &dFactor);

	if (dFactor >= 0.01 && dFactor <= 2.0)
	{
		if (
			m_stMotorX.SetCustMoveProf(dFactor) != GMP_SUCCESS 
			)
		{
			HMIMessageBox(MSG_OK, "SET UPLOOK MOTOR SPEED", "Set Motor Speed fail. Operation Abort!");
		}
		else
		{
			HMI_dUplookSpeedFactor = dFactor;
		}
	}
	else
	{
		HMIMessageBox(MSG_OK, "SET UPLOOK MOTOR SPEED", "Valid Speed Range is (0.01 to 2.0). Operation Abort!");
	}

	return 0;
}

LONG CInspOpt::IPC_SaveMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str;

	// saving positions
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.x"]							= m_mtrStandbyPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"]							= m_mtrStandbyPosn.y;

	pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bGoPrePRPosn"]							= m_bGoPrePRPosn;
	pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bDoPR2First"]								= m_bDoPR2First;

	pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bFeedBackPlacement"]						= m_bFeedBackPlacement;

	pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lStandbyLevel"]								= m_lStandbyLevel;

	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
	{
		str.Format("WH_%ld", j + 1);
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.x"]		= m_mtrPRCalibPosn[j].x;
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.y"]		= m_mtrPRCalibPosn[j].y;
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"]		= m_lPRCalibLevel[j];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lULRefLevel"]			= m_lULRefLevel[j];

		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPrePRPosn.x"]		= m_mtrPrePRPosn[j].x;
		pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPrePRPosn.y"]		= m_mtrPrePRPosn[j].y;
	}
	return CPI9000Stn::IPC_SaveMachineParam();
}

LONG CInspOpt::IPC_LoadMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str;

	// saving positions
	m_mtrStandbyPosn.x		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.x"];
	m_mtrStandbyPosn.y		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"];

	m_bGoPrePRPosn			= (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bGoPrePRPosn"];
	m_bDoPR2First			= (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bDoPR2First"];

	m_bFeedBackPlacement	= (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["m_bFeedBackPlacement"];

	m_lStandbyLevel			= pAppMod->m_smfMachine[GetStnName()]["Level"]["m_lStandbyLevel"];

	for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
	{
		str.Format("WH_%ld", j + 1);
		m_mtrPRCalibPosn[j].x		= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.x"];
		m_mtrPRCalibPosn[j].y		= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPRCalibPosn.y"];
		m_lPRCalibLevel[j]			= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"];
		m_lULRefLevel[j]			= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lULRefLevel"];

		m_mtrPrePRPosn[j].x			= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPrePRPosn.x"];
		m_mtrPrePRPosn[j].y			= pAppMod->m_smfMachine[GetStnName()]["Posn"][str]["m_mtrPrePRPosn.y"];
	}
	return CPI9000Stn::IPC_LoadMachineParam();
}

LONG CInspOpt::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str,str1;
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);

		for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
		{
			str1.Format("WH%ld", j + 1);
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.x"]				= m_mtrPR1Posn[j][i].x;
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.y"]				= m_mtrPR1Posn[j][i].y;
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.x"]				= m_mtrPR2Posn[j][i].x;
			pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.y"]				= m_mtrPR2Posn[j][i].y;
			pAppMod->m_smfDevice[GetStnName()]["Level"][str][str1]["m_lULPRLevel"]				= m_lULPRLevel[j][i];
		}
		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dGlassRefAng"]			= m_dGlassRefAng[i];
		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.x"]		= m_dmtrAlignRef[i].x;
		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.y"]		= m_dmtrAlignRef[i].y;
		pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dAlignRefAngle"]		= m_dAlignRefAngle[i];
	}

	pAppMod->m_smfDevice[GetStnName()]["InspParam"]["m_dPlacementTolX"]			= m_dPlacementTolX;	
	pAppMod->m_smfDevice[GetStnName()]["InspParam"]["m_dPlacementTolY"]			= m_dPlacementTolY;	
	pAppMod->m_smfDevice[GetStnName()]["InspParam"]["m_dPlacementTolAng"]		= m_dPlacementTolAng;

	//pAppMod->m_smfDevice[GetStnName()]["Para"]["HMI_bDebugCal"]						= HMI_bDebugCal;


	return CPI9000Stn::IPC_SaveDeviceParam();
}

LONG CInspOpt::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str,str1;

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		str.Format("GLASS%ld", i + 1);

		for (INT j = WH_1; j < MAX_NUM_OF_WH; j++)
		{
			str1.Format("WH%ld", j + 1);

			m_mtrPR1Posn[j][i].x		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.x"];
			m_mtrPR1Posn[j][i].y		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR1Posn.y"];
			m_mtrPR2Posn[j][i].x		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.x"];
			m_mtrPR2Posn[j][i].y		= pAppMod->m_smfDevice[GetStnName()]["Posn"][str][str1]["m_mtrPR2Posn.y"];
			m_lULPRLevel[j][i]  		= pAppMod->m_smfDevice[GetStnName()]["Level"][str][str1]["m_lULPRLevel"];
		}

		m_dGlassRefAng[i]		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dGlassRefAng"];
		m_dmtrAlignRef[i].x		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.x"];
		m_dmtrAlignRef[i].y		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dmtrAlignRef.y"];
		m_dAlignRefAngle[i]		= pAppMod->m_smfDevice[GetStnName()]["AlignParam"][str]["m_dAlignRefAngle"];
	}
	m_dPlacementTolX			= pAppMod->m_smfDevice[GetStnName()]["InspParam"]["m_dPlacementTolX"];	
	m_dPlacementTolY			= pAppMod->m_smfDevice[GetStnName()]["InspParam"]["m_dPlacementTolY"];	
	m_dPlacementTolAng			= pAppMod->m_smfDevice[GetStnName()]["InspParam"]["m_dPlacementTolAng"];

	//HMI_bDebugCal = (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Para"]["HMI_bDebugCal"];
	return CPI9000Stn::IPC_LoadDeviceParam();
}
LONG CInspOpt::IPC_LoadMachineRecord()
{
	// for uploading records
	CString szPath = "";

	for (INT i = 0; i < MAX_NUM_OF_GLASS; i++)
	{
		szPath = "d:\\sw\\PI9000\\Record\\" + pruINSPWH_UL_Calib[i].szFileName + ".zip";
		strcpy((char*) & (pruINSPWH_UL_Calib[i].stDownloadRecordCmd.acFilename[0]), szPath);
	}

	return 1;
}

LONG CInspOpt::IPC_SaveMachineRecord()
{
	CString szPath = "";


	for (INT i = 0; i < MAX_NUM_OF_GLASS; i++)
	{
		szPath = "d:\\sw\\PI9000\\Record\\" + pruINSPWH_UL_Calib[i].szFileName + ".zip";
		strcpy((char*) & (pruINSPWH_UL_Calib[i].stUploadRecordCmd.acFilename[0]), szPath);
	}
	//szPath = "d:\\sw\\PI9000\\Record\\" + pruINSP_UL_Calib.szFileName + ".zip";
	//strcpy((char*) & (pruINSP_UL_Calib.stUploadRecordCmd.acFilename[0]), szPath);

	return 1;
}

LONG CInspOpt::IPC_LoadDeviceRecord()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str = "";

	// for uploading records
	CString szPath = "";
	CString szDevice = pAppMod->m_szDeviceFile;
	INT nFind = szDevice.ReverseFind('.');

	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULGlass1.szFileName + ".zip";
	strcpy((char*) & (pruULGlass1.stDownloadRecordCmd.acFilename[0]), szPath);

	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULGlass2.szFileName + ".zip";
	strcpy((char*) & (pruULGlass2.stDownloadRecordCmd.acFilename[0]), szPath);

	
	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULCOF1.szFileName + ".zip";
	strcpy((char*) & (pruULCOF1.stDownloadRecordCmd.acFilename[0]), szPath);

	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULCOF2.szFileName + ".zip";
	strcpy((char*) & (pruULCOF2.stDownloadRecordCmd.acFilename[0]), szPath);


	return 1;
}

LONG CInspOpt::IPC_SaveDeviceRecord()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str = "";

	// for uploading records
	CString szPath = "";
	CString szDevice = pAppMod->m_szDeviceFile;
	INT nFind = szDevice.ReverseFind('.');

	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULGlass1.szFileName + ".zip";
	strcpy((char*) & (pruULGlass1.stUploadRecordCmd.acFilename[0]), szPath);

	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULGlass2.szFileName + ".zip";
	strcpy((char*) & (pruULGlass2.stUploadRecordCmd.acFilename[0]), szPath);


	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULCOF1.szFileName + ".zip";
	strcpy((char*) & (pruULCOF1.stUploadRecordCmd.acFilename[0]), szPath);

	szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_" + pruULCOF2.szFileName + ".zip";
	strcpy((char*) & (pruULCOF2.stUploadRecordCmd.acFilename[0]), szPath);

	return 1;
}
#ifdef EXCEL_MACHINE_DEVICE_INFO
BOOL CInspOpt::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;
//	INT i;
	CString strTitle, szParamName;

	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrStandbyPosn.x"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrStandbyPosn.y"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}


	if (bResult)
	{
		bResult = CPI9000Stn::printMachineDeviceInfo();
	}
	return bResult;
}
#endif


#if 1 //20120808 debug only
BOOL CInspOpt::LogXYMotor(CString &szLabel, LONG lWhichPB)
{
#if 1
	return TRUE;
#else
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	FILE *fp = NULL;

	//if (AutoMode != AUTO_BOND && AutoMode != TEST_BOND)
	//	return TRUE;

	if (lWhichPB == 0)
	{	
		fp = fopen("d:\\sw\\PI9000\\data\\PB1UL_XY.txt", "a+");
	}
	else if (lWhichPB == 1)
	{	
		fp = fopen("d:\\sw\\PI9000\\data\\PB2UL_XY.txt", "a+");
	}
	else
	{
		DisplayMessage("LogZMotor PBUL error!!");
		return FALSE;
	}

	if (fp != NULL)
	{
		clock_t clkNow;
		__time64_t ltime;
		CString szTime = "";

		clkNow = clock();
		_time64(&ltime);
		szTime = _ctime64(&ltime);
		szTime.TrimRight('\n');

		fseek(fp, 0, SEEK_END);
		if (ftell(fp) < 10)
		{
			fprintf(fp, "Date: " + szTime + " PBUL posn\tX\tY\n");
		}
		if (m_fHardware)
		{
			LONG ii;
			for (ii = 0; ii < 3; ii++)
			{
				fprintf(fp, "%s %s (%.3f)\t%ld\t%ld\n", (const char*)szLabel, (const char*)szTime, 
						(DOUBLE)(clkNow - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC, 
						m_stMotorX.GetEncPosn(), m_stMotorY.GetEncPosn());
				Sleep(5);
			}
		}
		else
		{
			//my test for offline machine
			fprintf(fp, "%s %s (%.3f)\t%ld\t%ld\n", (const char*)szLabel, (const char*)szTime, 
					(DOUBLE)(clkNow - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC, 
					111, 1111);
		}
		fclose(fp);
		return TRUE;
	}
	return FALSE;
#endif
}
#endif

BOOL CInspOpt::SearchPlacementTest()
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspWH *pCurInspWH;

	if(m_lCurInspWH == WH_1)
	{
		pCurInspWH = pCWH1;
		pruULCOF1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
		pruULCOF2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
		pruULGlass1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
		pruULGlass2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
	}
	else
	{
		pCurInspWH = pCWH2;
		pruULCOF1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
		pruULCOF2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
		pruULGlass1.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
		pruULGlass2.eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
	}
	BOOL bResult = TRUE;

	CString szMsg = "";
	MTR_POSN mtrTemp;
	mtrTemp.x = 0;
	mtrTemp.y = 0;

	PR_RCOORD coCentre;
	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		return FALSE;
	}

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
		return FALSE;
	}

	if (!pruULGlass1.bLoaded || !pruULGlass2.bLoaded || !pruULCOF1.bLoaded || !pruULCOF2.bLoaded )
	{
		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", "PR Pattern Not Loaded. Operation Abort!");
		return FALSE;
	}

	m_dCurrAlignAngle = 0.0;

	//if (pCurInspWH->MoveXY((INSPWH_XY_CTRL_SIGNAL)(INSPWH_XY_CTRL_GO_UL_G1_INSP_POSN + m_lCurGlass), GMP_WAIT) != GMP_SUCCESS)
	//{
	//	HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "InspWH Move to Reference Posn Failed!");
	//	return FALSE;
	//}

	if (m_bGoPrePRPosn)
	{
		if ((MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) != GMP_SUCCESS)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to PR1 Posn Failed!");
			return FALSE;
		}
	}

	if((m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x != m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x) && (m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y != m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y))
	{
		if (
			(MoveXYToCurrPR1Posn(GMP_NOWAIT)) != GMP_SUCCESS || 
			(MoveZToPRLevel(GMP_NOWAIT)) != GMP_SUCCESS || 
			(SyncX()) != GMP_SUCCESS || 
			(SyncY()) != GMP_SUCCESS || 
			(SyncZ()) != GMP_SUCCESS 
		)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to PR1 Posn Failed!");
			return FALSE;
		}
	}
	
	Sleep(m_lPRDelay);

	//if(m_bMoveINPA)
	//{
	//	if (!pCInPickArm->m_bModSelected)
	//	{
	//		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
	//		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
	//		return FALSE;
	//	}
	//	if (
	//		(pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_WH1_PLACE_GLASS_POSN,GMP_NOWAIT)) != GMP_SUCCESS
	//	)
	//	{
	//		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Place Posn Failed!");
	//		return FALSE;
	//	}
	//}

	//CString szMsg = "";
	DOUBLE dProcessTime = 0.0;
	DWORD dwStartTime = GetTickCount();

	if (!AutoSearchPR1GrabOnly(&pruULGlass1, m_lCurGlass, LOOK_GLASS_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search Glass PR1 Grab Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR1GrabOnly(&pruULGlass1): %.5f", dProcessTime);
	DisplayMessage(szMsg);

	dwStartTime = GetTickCount();

	if (!AutoSearchPR1ProcessOnly(&pruULGlass1, NULL, m_lCurGlass, LOOK_GLASS_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search Glass PR1 Process Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}
	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR1ProcessOnly(&pruULGlass1): %.5f", dProcessTime);
	DisplayMessage(szMsg);

	m_dmtrGlassPR1Posn[m_lCurGlass] = m_dmtrCurrPR1Posn;
	m_dmtrCurrPR1Posn.x = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x + m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].x;	
	m_dmtrCurrPR1Posn.y = m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y + m_dmtrPR1Offset[m_lCurInspWH][m_lCurGlass].y;	

	dwStartTime = GetTickCount();

	if (!AutoSearchPR1GrabOnly(&pruULCOF1, m_lCurGlass, LOOK_COF_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search COF PR1 Grab Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR1GrabOnly(&pruULCOF1): %.5f", dProcessTime);
	DisplayMessage(szMsg);

	if((m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].x != m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x) && (m_mtrPR1Posn[m_lCurInspWH][m_lCurGlass].y != m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y))
	{
		if (
			(MoveXYToCurrPR2Posn(GMP_NOWAIT)) != GMP_SUCCESS || 
			(MoveZToPRLevel(GMP_NOWAIT)) != GMP_SUCCESS
		)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to PR2 Posn Failed!");
			return FALSE;
		}
	}

	dwStartTime = GetTickCount();

	if (!AutoSearchPR1ProcessOnly(&pruULCOF1, NULL, m_lCurGlass, LOOK_COF_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search COF PR1 Process Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR1ProcessOnly(&pruULCOF1): %.5f", dProcessTime);
	DisplayMessage(szMsg);

	m_dmtrCOFPR1Posn[m_lCurGlass] = m_dmtrCurrPR1Posn;

	if (
		(SyncX()) != GMP_SUCCESS || 
		(SyncY()) != GMP_SUCCESS || 
		(SyncZ()) != GMP_SUCCESS 
	   )
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to PR2 Posn Failed!");
		return FALSE;
	}
	Sleep(m_lPRDelay);

	//if(m_bMoveINPA)
	//{
	//	if (!pCInPickArm->m_bModSelected)
	//	{
	//		szMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
	//		HMIMessageBox(MSG_OK, "SEARCH ALIGN PT1", szMsg);
	//		return FALSE;
	//	}
	//	if (
	//		(pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN,GMP_NOWAIT)) != GMP_SUCCESS
	//	)
	//	{
	//		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to Standby Posn Failed!");
	//		return FALSE;
	//	}
	//}

	dwStartTime = GetTickCount();

	if (!AutoSearchPR2GrabOnly(&pruULGlass2, m_lCurGlass, LOOK_GLASS_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search Glass PR2 Grab Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR2GrabOnly(&pruULGlass2): %.5f", dProcessTime);
	DisplayMessage(szMsg);

	dwStartTime = GetTickCount();

	if (!AutoSearchPR2ProcessOnly(&pruULGlass2, NULL, m_lCurGlass, LOOK_GLASS_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search Glass PR2 Process Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR2ProcessOnly(&pruULGlass2): %.5f", dProcessTime);
	DisplayMessage(szMsg);

	m_dmtrGlassPR2Posn[m_lCurGlass] = m_dmtrCurrPR2Posn;
	m_dmtrCurrPR2Posn.x = m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].x + m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].x;	
	m_dmtrCurrPR2Posn.y = m_mtrPR2Posn[m_lCurInspWH][m_lCurGlass].y + m_dmtrPR2Offset[m_lCurInspWH][m_lCurGlass].y;	

	dwStartTime = GetTickCount();

	if (!AutoSearchPR2GrabOnly(&pruULCOF2, m_lCurGlass, LOOK_COF_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search COF PR2 Grab Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR2GrabOnly(&pruULCOF2): %.5f", dProcessTime);
	DisplayMessage(szMsg);	

	dwStartTime = GetTickCount();

	if (!AutoSearchPR2ProcessOnly(&pruULCOF2, NULL, m_lCurGlass, LOOK_COF_TYPE))
	{
		HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Search COF PR2 Process Failed!");
		PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Failed!");
		return FALSE;
	}

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR2ProcessOnly(&pruULCOF2): %.5f", dProcessTime);
	DisplayMessage(szMsg);	

	m_dmtrCOFPR2Posn[m_lCurGlass] = m_dmtrCurrPR2Posn;

	PRS_ClearScreen(INSPUL_CAM);
	

	dwStartTime = GetTickCount();

	Cal_MB_Offset((GlassNum)m_lCurGlass);

	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	szMsg.Format("PR2GrabOnly(&pruULCOF2): %.5f", dProcessTime);
	DisplayMessage(szMsg);	

	szMsg.Format("Placement Offset X: %.5f, Y: %.5f, T: %.5f", m_dPlacementOffsetX[MAINBOND_1][m_lCurGlass], m_dPlacementOffsetY[MAINBOND_1][m_lCurGlass],m_dPlacementAngle[MAINBOND_1][m_lCurGlass]);
	WriteHMIMess(szMsg);

	PRS_DrawCross(INSPUL_CAM, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
	PRS_DisplayText(INSPUL_CAM, 1, PRS_MSG_ROW1, "Search Succeed!");

	if (m_bGoPrePRPosn)
	{
		if ((MoveXY(INSPOPT_XY_CTRL_GO_PREPR_POSN, GMP_WAIT)) != GMP_SUCCESS)
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "Move to PR1 Posn Failed!");
			return FALSE;
		}
	}

	if (m_bMoveWH)
	{
		if (pCurInspWH->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS ||
			pCurInspWH->MoveT(INSPWH_T_CTRL_GO_STANDBY, GMP_WAIT) != GMP_SUCCESS ||
			MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS) 
		{
			HMIMessageBox(MSG_OK, "SEARCH COF PLACEMENT", "InspWH Move to Standby Posn Failed!");
			return FALSE;
		}
	}

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{

		m_dmtrPR1Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR1Offset[m_lCurInspWH][i].y = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].y = 0.0;

		m_dmtrGlassPR1Posn[i].x = 0.0;
		m_dmtrGlassPR1Posn[i].y = 0.0;
		m_dmtrGlassPR2Posn[i].x = 0.0;
		m_dmtrGlassPR2Posn[i].y = 0.0;
		m_dmtrCOFPR1Posn[i].x = 0.0;
		m_dmtrCOFPR1Posn[i].y = 0.0;
		m_dmtrCOFPR2Posn[i].x = 0.0;
		m_dmtrCOFPR2Posn[i].y = 0.0;

		m_dPlacementOffsetX[MAINBOND_1][i] = 0.0;
		m_dPlacementOffsetX[MAINBOND_2][i] = 0.0;
		m_dPlacementOffsetY[MAINBOND_1][i] = 0.0;
		m_dPlacementOffsetY[MAINBOND_2][i] = 0.0;

		m_dPlacementAngle[MAINBOND_1][i] = 0.0;
		m_dPlacementAngle[MAINBOND_2][i] = 0.0;
	}

	return TRUE;
}

BOOL CInspOpt::IsPlacementTolError(GlassNum lCurGlass, INSP_MB_TYPE lCurMBHead)
{
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
	CString szMsg, szRejectReason;
	BOOL bResult = TRUE;
	LONG lMBModule = MAINBOND_1;

	LONG lSize = m_stszDisplayError[lCurMBHead].size();
	szRejectReason = _T("Placement Offset Out of Tolerance:");
	switch (lCurMBHead)
	{
	case NO_MB_TYPE:
	case INSP_MB1:
	case INSP_MB2:
		lMBModule = MAINBOND_1;
		break;
	case INSP_MB3:
	case INSP_MB4:
		lMBModule = MAINBOND_2;
		break;
	}
	if (
		 (m_dPlacementTolX == 0.0 || fabs(m_dPlacementOffsetX[lMBModule][lCurGlass]) <= m_dPlacementTolX) &&
		 (m_dPlacementTolY == 0.0 || fabs(m_dPlacementOffsetY[lMBModule][lCurGlass]) <= m_dPlacementTolY) &&
		 (m_dPlacementTolAng == 0.0 || fabs(m_dPlacementAngle[lMBModule][lCurGlass]) <= m_dPlacementTolAng)
	)
	{
		bResult = FALSE;
	}
	else
	{
		if (m_dPlacementTolX != 0.0 && fabs(m_dPlacementOffsetX[lMBModule][lCurGlass]) > m_dPlacementTolX)
		{
			DisplayMessage("*********************************************");
			SetError(IDS_INSPOPT_X_TOL_ERR);
			szMsg.Format("INSPOPT PLACEMENT X OUT OF RANGE ERROR.");
			DisplayMessage(szMsg);
			DisplayMessage("*********************************************");
			szRejectReason += szMsg;
			bResult = TRUE;
		}
		if (m_dPlacementTolY != 0.0 && fabs(m_dPlacementOffsetY[lMBModule][lCurGlass]) > m_dPlacementTolY)
		{
			DisplayMessage("*********************************************");
			SetError(IDS_INSPOPT_Y_TOL_ERR);
			szMsg.Format("INSPOPT PLACEMENT Y OUT OF RANGE ERROR.");
			DisplayMessage(szMsg);
			DisplayMessage("*********************************************");
			szRejectReason += szMsg;
			bResult = TRUE;
		}
		if (m_dPlacementTolAng != 0.0 && fabs(m_dPlacementAngle[lMBModule][lCurGlass]) > m_dPlacementTolAng)
		{
			DisplayMessage("*********************************************");
			SetError(IDS_INSPOPT_ANG_TOL_ERR);
			szMsg.Format("INSPOPT PLACEMENT ANGLE OUT OF RANGE ERROR.");
			DisplayMessage(szMsg);
			DisplayMessage("*********************************************");
			szRejectReason += szMsg;
			bResult = TRUE;
		}
	}
	if (bResult)
	{
		pCurWH->m_szRejectReason[m_lCurGlass] = szRejectReason;
	}
	return bResult;
}

VOID CInspOpt::FeedBackOffsetToCOF(INSP_MB_TYPE lCurMBHead)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	FEED_BACK_MSG stFeedBackMsg;

	stFeedBackMsg.lMBNum = lCurMBHead + 1;
	stFeedBackMsg.dOffsetX = m_dOffsetX[lCurMBHead];
	stFeedBackMsg.dOffsetY = m_dOffsetY[lCurMBHead];
	stFeedBackMsg.dOffsetAng = m_dOffsetAng[lCurMBHead];
	
	svMsg.InitMessage(sizeof(FEED_BACK_MSG), &stFeedBackMsg);

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
		str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\InspOpt_Feedback_Msg_to_COF_%04u%02u%02u.csv", ltime.wYear, ltime.wMonth, ltime.wDay);

		FILE *fp = fopen(str, "a+");

		if (fp != NULL)
		{

			fseek(fp, 0, SEEK_END);
			if (ftell(fp) < 10)
			{
				fprintf(fp, "Time\tMBx\tOffsetX\tOffsetY\tOffsetAngle\n");
			}
			fprintf(fp, "%s\t%ld\t%.5f\t%.5f\t%.5f\n",
				(const char*)szTime, stFeedBackMsg.lMBNum,
				stFeedBackMsg.dOffsetX,	stFeedBackMsg.dOffsetY,
				stFeedBackMsg.dOffsetAng
				);
					
			fclose(fp);
		}
	}
	try
	{
		nConvID = m_comClient.SendRequest("HK_COF902GW", "HK_UpdateMBOffset", svMsg, 0);
	}
	catch (CAsmException e)
	{
		CString szMsg;
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		szMsg.Format("Func:%s", __FUNCTION__);
		DisplayMessage(szMsg);
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		szMsg.Format("InspOpt Gateway Send Data Fail. Pls check gateway connection Setting.");
		DisplayMessage(szMsg);
		HMIMessageBox(MSG_OK, "Feed Back MB Offset to COF", szMsg);

		return;
	}

	return;

}

LONG CInspOpt::HMI_ToggleFeedBackPlacement(IPC_CServiceMessage &svMsg)
{
	m_bFeedBackPlacement = !m_bFeedBackPlacement;

	for (INT i = INSP_MB1; i < MAX_NUM_OF_MB_TYPE; i++)
	{
		m_stdPlacementOffsetX[i].clear();
		m_stdPlacementOffsetY[i].clear();
		m_stdPlacementAngle[i].clear();
	}
	return 0;
}

VOID CInspOpt::FeedBackOutOfTolToCOF(MainbondType lMBModule)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	FEED_BACK_MSG stFeedBackMsg;

	stFeedBackMsg.lMBNum = lMBModule * 2 + m_lCurGlass + 1;
	stFeedBackMsg.dOffsetX = m_dPlacementOffsetX[lMBModule][m_lCurGlass];
	stFeedBackMsg.dOffsetY = m_dPlacementOffsetY[lMBModule][m_lCurGlass];
	stFeedBackMsg.dOffsetAng = m_dPlacementAngle[lMBModule][m_lCurGlass];
	
	svMsg.InitMessage(sizeof(FEED_BACK_MSG), &stFeedBackMsg);

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
		str.Format("D:\\sw\\PI9000\\Data\\Calculation Log\\InspOpt_Out_of_Tol_Feedback_to_COF_%04u%02u%02u.csv", ltime.wYear, ltime.wMonth, ltime.wDay);

		FILE *fp = fopen(str, "a+");

		if (fp != NULL)
		{

			fseek(fp, 0, SEEK_END);
			if (ftell(fp) < 10)
			{
				fprintf(fp, "Time\tMBx\tOffsetX\tOffsetY\tOffsetAngle\n");
			}
			fprintf(fp, "%s\t%ld\t%.5f\t%.5f\t%.5f\n",
				(const char*)szTime, stFeedBackMsg.lMBNum,
				stFeedBackMsg.dOffsetX,	stFeedBackMsg.dOffsetY,
				stFeedBackMsg.dOffsetAng
				);
					
			fclose(fp);
		}
	}
	try
	{
		nConvID = m_comClient.SendRequest("HK_COF902GW", "HK_UpdateOutOfTolOffset", svMsg, 0);
	}
	catch (CAsmException e)
	{
		CString szMsg;
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		szMsg.Format("Func:%s", __FUNCTION__);
		DisplayMessage(szMsg);
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		szMsg.Format("InspOpt Gateway Send Data Fail. Pls check gateway connection Setting.");
		DisplayMessage(szMsg);
		HMIMessageBox(MSG_OK, "Feed Back MB Offset to COF", szMsg);

		return;
	}

	return;

}

VOID CInspOpt::PrintPlacementDisplay(INSP_MB_TYPE lCurMBHead)
{
	LONG	lSize = m_stdDisplayOffsetX[lCurMBHead].size();
	CString str = "";	
	str.Format("D:\\sw\\PI9000\\Data\\Display\\MB%ld_PlacementDisplay.csv", lCurMBHead + 1);
	DOUBLE	dMinX = 0.0, dMaxX = 0.0, dSDX = 0.0;
	DOUBLE	dMinY = 0.0, dMaxY = 0.0, dSDY = 0.0;
	DOUBLE	dMinAng = 0.0, dMaxAng = 0.0, dSDAng = 0.0;

	dSDX = CalDequeSD(m_stdDisplayOffsetX[lCurMBHead]);
	CalDequeRange(m_stdDisplayOffsetX[lCurMBHead], dMinX, dMaxX);	
	dSDY = CalDequeSD(m_stdDisplayOffsetY[lCurMBHead]);
	CalDequeRange(m_stdDisplayOffsetY[lCurMBHead], dMinY, dMaxY);
	dSDAng = CalDequeSD(m_stdDisplayAngle[lCurMBHead]);
	CalDequeRange(m_stdDisplayAngle[lCurMBHead], dMinAng, dMaxAng);

	DeleteDir(str);
	FILE *fp = fopen(str, "a+");
	if (fp != NULL)
	{	
		fseek(fp, 0, SEEK_END);
		if (ftell(fp) < 10)
		{
			fprintf(fp, "No.,Offset X,Offset Y,Offset Ang,Glass Status,\n");
		}

		for (INT i = 0; i < lSize; i++)
		{
			fprintf(fp, "%d,%.2f,%.2f,%.2f,%s,\n", 
				(i + 1),
				m_stdDisplayOffsetX[lCurMBHead][i],
				m_stdDisplayOffsetY[lCurMBHead][i],
				m_stdDisplayAngle[lCurMBHead][i],
				(const char*)m_stszDisplayError[lCurMBHead][i]
				);
		}
		fclose(fp);
	}
	
	str.Format("D:\\sw\\PI9000\\Data\\Display\\MB%ld_Placement_MinMaxSD.csv", lCurMBHead + 1);	
	DeleteDir(str);
	fp = fopen(str, "a+");
	if (fp != NULL)
	{	
		fseek(fp, 0, SEEK_END);
		fprintf(fp,"Min:,%.2f,%.2f,%.2f,\t,\n", dMinX, dMinY, dMinAng);
		fprintf(fp,"Max:,%.2f,%.2f,%.2f,\t,\n", dMaxX, dMaxY, dMaxAng);
		fprintf(fp,"SD:,%.3f,%.3f,%.3f,\t,\n", dSDX, dSDY, dSDAng);
		fclose(fp);
	}

	
}

LONG CInspOpt::HMI_SearchDLnGoULPR1Posn(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;
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
	pCurWH->m_lCurGlass = m_lCurGlass;
	pCurWH->SearchDLnGoULPR1Posn();

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		m_dmtrPR1Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR1Offset[m_lCurInspWH][i].y = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].y = 0.0;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CInspOpt::HMI_SearchDLnGoULPR2Posn(IPC_CServiceMessage &svMsg)
{
	BOOL bResult = TRUE;
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

	pCurWH->m_lCurGlass = m_lCurGlass;
	pCurWH->SearchDLnGoULPR2Posn();

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		m_dmtrPR1Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR1Offset[m_lCurInspWH][i].y = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].x = 0.0;
		m_dmtrPR2Offset[m_lCurInspWH][i].y = 0.0;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}