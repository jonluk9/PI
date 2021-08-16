/////////////////////////////////////////////////////////////////
//	WorkHolder.cpp : interface of the CWorkHolder class
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

#include "HouseKeeping.h"
#include "Cal_Util.h"

#define WORKHOLDER_EXTERN
#include "WorkHolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CWorkHolder, CPI9000Stn)

CWorkHolder::CWorkHolder()
{
	m_dwStartTime				= 0;
	m_dProcessTime				= 0.0;

	m_bUseAnvilVacuum			= FALSE;
	m_bZOffDuringBond			= FALSE;
	m_bOffWHVacuum				= FALSE;
	m_lStaticControlMode		= FALSE;
	m_lCurStaticControl[WHZ_1]	= 1; //20120618 // 0: Set Ki == 0; 1: Normal
	m_lCurStaticControl[WHZ_2]	= 1; //20120618 // 0: Set Ki == 0; 1: Normal

	m_mtrStandbyPosn.x			= 0;
	m_mtrStandbyPosn.y			= 0;
	m_mtrLoadGlassPosn.x		= 0;
	m_mtrLoadGlassPosn.y		= 0;
	m_mtrUnloadGlassPosn.x		= 0;
	m_mtrUnloadGlassPosn.y		= 0;
	m_mtrApproxBondPosn.x		= 0;
	m_mtrApproxBondPosn.y		= 0;

	m_lStandbyPosnT				= 0;

	for (INT i = 0; i < MAX_NUM_OF_WHZ; i++)
	{
		m_lStandbyLevel[i]			= 0;
		m_lCalibStageLevel[i]		= 0;
		m_lCalibLoadLevel[i]		= 0;
		m_lCalibUnloadLevel[i]		= 0;
		m_lPRCalibLevel[i]			= 0;
		m_lApproxBondLevel[i]		= 0;
		m_lLoadLevelOffset[i]		= 0;

		m_lUnloadLevelOffset[i]		= 0;
		m_lPRLevelOffset[i]			= 0;
	}	

	for (int j = 0; j < MAX_NUM_OF_WHZ; j++)
	{
		for (i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++) //whz rotary linear calib
		{
			m_stWHCalibZ[j][i].LinearPosn = 0;
			m_stWHCalibZ[j][i].RotaryPosn = 0;
		}
	}
	
	m_mtrMeasureSetZeroPt.x	= 0;
	m_mtrMeasureSetZeroPt.y = 0;
	m_dMeasuredDistance		= 0.0;
	m_dMeasuredAngle		= 0.0;
	m_bEnableMeasure		= FALSE;
}

CWorkHolder::~CWorkHolder()
{
}

BOOL CWorkHolder::InitInstance()
{
	CPI9000Stn::InitInstance();

	return TRUE;
}

INT CWorkHolder::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CWorkHolder::Operation()
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

VOID CWorkHolder::UpdateOutput()
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
#if 1 //def ENC_ONLY //20160216
				if (GetmyMotorList().GetAt(i)->b2ndEncoderExist)
				{
					GetmyMotorList().GetAt(i)->lHMI_Curr2ndPosn	= GetmyMotorList().GetAt(i)->Get2ndEncPosn();
				}
#endif 
				GetmyMotorList().GetAt(i)->bHMI_On			= GetmyMotorList().GetAt(i)->IsPowerOn();
				GetmyMotorList().GetAt(i)->lHMI_CurrPosn	= GetmyMotorList().GetAt(i)->GetEncPosn();
				
			}

			//RotaryToLinearEnc(m_stMotorZ[WHZ_1].GetEncPosn(), m_stWHCalibZ[WHZ_1], &m_lRotatery2LinearZ1);
			//RotaryToLinearEnc(m_stMotorZ[WHZ_2].GetEncPosn(), m_stWHCalibZ[WHZ_2], &m_lRotatery2LinearZ2);
			

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

VOID CWorkHolder::RegisterVariables()
{
	try
	{
		//Register Sensors here 
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

// ------------------------------------------------------------------------
// Virtual WorkHolder Function delcaration
// ------------------------------------------------------------------------

VOID CWorkHolder::SetDiagnSteps(ULONG ulSteps, BOOL bFineStep)
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
}


BOOL CWorkHolder::PickGlassOperation(GlassNum lGlassNum)
{
	DisplayMessage(__FUNCTION__);
	SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
	return FALSE;
}

INT CWorkHolder::IndexGlassToTable(GlassNum lGlassNum, BOOL bTeach)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	LONG lAnswer = rMSG_TIMEOUT;

	CString strMsg;

	if (m_bIsOfflineMode)
	{
		return GMP_SUCCESS;
	}

	SetVacSol(lGlassNum, ON, GMP_WAIT);

	if (IsVacSensorOn(lGlassNum))
	{
		strMsg.Format("Glass found on %s. Use current Glass for the setup?", GetStnName());
		lAnswer = HMIMessageBox(MSG_YES_NO, "Index Glass to WH", strMsg);
		if (lAnswer == rMSG_YES)
		{
			return GMP_SUCCESS;
		}
		else if (lAnswer == rMSG_TIMEOUT)
		{
			return GMP_FAIL;
		}

		do
		{
			SetVacSol(lGlassNum, OFF);
			
			strMsg.Format("Please remove Glass from %s.", GetStnName());
			lAnswer = HMIMessageBox(MSG_OK_CANCEL, "Index Glass to WH", strMsg);
			
			if (lAnswer == rMSG_OK)
			{
				SetVacSol(lGlassNum, ON, GMP_WAIT);
			}
			else if (lAnswer == rMSG_CANCEL)
			{
				SetVacSol(lGlassNum, ON, GMP_WAIT);

				if (!IsVacSensorOn(lGlassNum))
				{
					SetVacSol(lGlassNum, OFF);
				}

				HMIMessageBox(MSG_OK, "Index Glass to WH", "Operation Abort!");
				return GMP_FAIL;
			}
			else
			{
				SetVacSol(lGlassNum, ON, GMP_WAIT);
				return GMP_FAIL;
			}

		} while (IsVacSensorOn(lGlassNum));
	}

//	if (!pAppMod->CheckGlassIndexingPath(GetStnName(), lGlassNum))
//	{
//		return GMP_FAIL;
//	}
//
//		lAnswer = HMIMessageBox(MSG_OK_CANCEL, "Index Glass to WH", "Please place glass onto InConveyor.");
////p20121022
//	if (lAnswer == MSG_OK)
//	{
////p20121022:end
//		if (!PickGlassOperation(lGlassNum))
//		{
//			return GMP_FAIL;
//		}
////p20121022
//	}
//	else
//	{
//		return GMP_FAIL;
//	}
//p20121022:end

	return GMP_SUCCESS;
}


// ------------------------------------------------------------------------
// END Virtual WorkHolder Function delcaration
// ------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//Operational Sequences
/////////////////////////////////////////////////////////////////

// Motor Related
INT CWorkHolder::SetZControlPara(WHZNum lWHZNum)
{
	INT nResult = GMP_SUCCESS; 
#if 1
	if (!m_bModSelected || m_bIsOfflineMode)
	{
		m_lCurStaticControl[WHZ_1] = 0;
		m_lCurStaticControl[WHZ_2] = 0;
		return GMP_SUCCESS;
	}
	
	CString szParamBlk;
	if (m_lStaticControlMode == NORMAL_MODE)
	{
		szParamBlk = "PL_Static";
	}
	else if (m_lStaticControlMode == KI_0_MODE)
	{
		szParamBlk = "PL_Dynamic1";
	}
	else
	{
		return GMP_SUCCESS;
	}
	
	switch (lWHZNum)
	{
	case WHZ_1:
		if (
			m_stMotorZ[WHZ_1].SelectStaticCtrlPara(szParamBlk) == GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_WAIT) == GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_1] = 0;
		break;

	case WHZ_2:
		if (
			m_stMotorZ[WHZ_2].SelectStaticCtrlPara(szParamBlk) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_WAIT) != GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_2] = 0;
		break;

	case MAX_NUM_OF_WHZ:
		if (
			m_stMotorZ[WHZ_1].SelectStaticCtrlPara(szParamBlk) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].SelectStaticCtrlPara(szParamBlk) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_1] = 0;
		m_lCurStaticControl[WHZ_2] = 0;
		break;
	}

#else
	if (!m_bModSelected || m_bIsOfflineMode)
	{
		switch (lWHZNum)
		{
		case WHZ_1:
			m_lCurStaticControl[WHZ_1] = 0;
			break;

		case WHZ_2:
			m_lCurStaticControl[WHZ_2] = 0;
			break;

		case MAX_NUM_OF_WHZ:
			m_lCurStaticControl[WHZ_1] = 0;
			m_lCurStaticControl[WHZ_2] = 0;
			break;
		}
		return GMP_SUCCESS;
	}

	if (
		(lWHZNum == WHZ_1 && m_lCurStaticControl[WHZ_1] == 0) || 
		(lWHZNum == WHZ_2 && m_lCurStaticControl[WHZ_2] == 0) || 
		(lWHZNum == MAX_NUM_OF_WHZ && m_lCurStaticControl[WHZ_1] == 0 && m_lCurStaticControl[WHZ_2] == 0) ||
		m_lStaticControlMode == OFF_MOTOR_MODE || 
		m_lStaticControlMode == NORMAL_MODE
	   )
	{
		return GMP_SUCCESS;
	}

	switch (lWHZNum)
	{
	case WHZ_1:
		if (
			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Dynamic1") == GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_WAIT) == GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_1] = 0;
		break;

	case WHZ_2:
		if (
			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Dynamic1") != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_WAIT) != GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_2] = 0;
		break;

	case MAX_NUM_OF_WHZ:
		if (
			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Dynamic1") != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Dynamic1") != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_1] = 0;
		m_lCurStaticControl[WHZ_2] = 0;
		break;
	}
#endif
	return nResult;
}

INT CWorkHolder::SetZ1ControlPara(LONG lAbsoluteCnt, BOOL bWait)
{
	INT nResult = GMP_SUCCESS; 

	//if (!m_bModSelected || m_bIsOfflineMode)
	//{
	//	m_lCurStaticControl[WHZ_1] = 0;
	//	return GMP_SUCCESS;
	//}

	//if (
	//	m_lCurStaticControl[WHZ_1] == 0 || 
	//	m_lStaticControlMode == OFF_MOTOR_MODE || 
	//	m_lStaticControlMode == NORMAL_MODE
	//)
	//{
	if (m_stMotorZ[WHZ_1].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS)
	{
		return GMP_SUCCESS;
	}
	//}


	//if (
	//	//m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Dynamic1") == GMP_SUCCESS ||
	//	//m_stMotorZ[WHZ_1].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS
	//	m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Dynamic1") != GMP_SUCCESS || //20130609
	//	m_stMotorZ[WHZ_1].MoveAbsolute(lAbsoluteCnt, bWait) != GMP_SUCCESS
	//)
	//{
	//	nResult = GMP_FAIL;
	//	DisplayMessage("SetZ1ControlPara error!");
	//}

	//m_lCurStaticControl[WHZ_1] = 0;
	return nResult;
}

INT CWorkHolder::SetZ2ControlPara(LONG lAbsoluteCnt, BOOL bWait)
{
	INT nResult = GMP_SUCCESS; 

	//if (!m_bModSelected || m_bIsOfflineMode)
	//{
	//	m_lCurStaticControl[WHZ_2] = 0;
	//	return GMP_SUCCESS;
	//}

	//if (
	//	m_lCurStaticControl[WHZ_2] == 0 || 
	//	m_lStaticControlMode == OFF_MOTOR_MODE || 
	//	m_lStaticControlMode == NORMAL_MODE
	//)
	//{
	if (m_stMotorZ[WHZ_2].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS)
	{
		return GMP_SUCCESS;
	}
	//}


	//if (
	//	//m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Dynamic1") == GMP_SUCCESS ||
	//	//m_stMotorZ[WHZ_2].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS
	//	m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Dynamic1") != GMP_SUCCESS || //20130609
	//	m_stMotorZ[WHZ_2].MoveAbsolute(lAbsoluteCnt, bWait) != GMP_SUCCESS
	//)
	//{
	//	nResult = GMP_FAIL;
	//	DisplayMessage("SetZ2ControlPara error!");
	//}

	//m_lCurStaticControl[WHZ_2] = 0;
	return nResult;
}

INT CWorkHolder::ResetZControlPara(WHZNum lWHZNum)
{
	INT nResult = GMP_SUCCESS; 
//#if 0
//	if (!m_bModSelected || m_bIsOfflineMode)
//	{
//		return GMP_SUCCESS;
//	}
//	if (m_lStaticControlMode == OFF_MOTOR_MODE)
//	{
//		return GMP_SUCCESS;
//	}
//	switch (lWHZNum)
//	{
//	case WHZ_1:
//		if (
//			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_WAIT) != GMP_SUCCESS
//		)
//		{
//			nResult = GMP_FAIL;
//		}
//		m_lCurStaticControl[WHZ_1] = 1;
//		break;
//
//	case WHZ_2:
//		if (
//			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_WAIT) != GMP_SUCCESS
//		)
//		{
//			nResult = GMP_FAIL;
//		}
//		m_lCurStaticControl[WHZ_2] = 1;
//		break;
//
//	case MAX_NUM_OF_WHZ:
//		if (
//			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//		)
//		{
//			nResult = GMP_FAIL;
//		}
//		m_lCurStaticControl[WHZ_1] = 1;
//		m_lCurStaticControl[WHZ_2] = 1;
//		break;
//	}
//
//#else
//	if (!m_bModSelected || m_bIsOfflineMode)
//	{
//		switch (lWHZNum)
//		{
//		case WHZ_1:
//			m_lCurStaticControl[WHZ_1] = 1;
//			break;
//
//		case WHZ_2:
//			m_lCurStaticControl[WHZ_2] = 1;
//			break;
//
//		case MAX_NUM_OF_WHZ:
//			m_lCurStaticControl[WHZ_1] = 1;
//			m_lCurStaticControl[WHZ_2] = 1;
//			break;
//		}
//		return GMP_SUCCESS;
//	}
//	if (
//		(lWHZNum == WHZ_1 && m_lCurStaticControl[WHZ_1] == 1) || 
//		(lWHZNum == WHZ_2 && m_lCurStaticControl[WHZ_2] == 1) || 
//		(lWHZNum == MAX_NUM_OF_WHZ && m_lCurStaticControl[WHZ_1] == 1 && m_lCurStaticControl[WHZ_2] == 1) ||
//		m_lStaticControlMode == OFF_MOTOR_MODE || 
//		m_lStaticControlMode == NORMAL_MODE
//	)
//	{
//		return GMP_SUCCESS;
//	}
//
//	switch (lWHZNum)
//	{
//	case WHZ_1:
//		if (
//			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_WAIT) != GMP_SUCCESS
//		)
//		{
//			nResult = GMP_FAIL;
//		}
//		m_lCurStaticControl[WHZ_1] = 1;
//		break;
//
//	case WHZ_2:
//		if (
//			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_WAIT) != GMP_SUCCESS
//		)
//		{
//			nResult = GMP_FAIL;
//		}
//		m_lCurStaticControl[WHZ_2] = 1;
//		break;
//
//	case MAX_NUM_OF_WHZ:
//		if (
//			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
//			m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
//		)
//		{
//			nResult = GMP_FAIL;
//		}
//		m_lCurStaticControl[WHZ_1] = 1;
//		m_lCurStaticControl[WHZ_2] = 1;
//		break;
//	}
//#endif
	return nResult;
}

INT CWorkHolder::ResetZ1ControlPara(LONG lAbsoluteCnt, BOOL bWait)
{
	INT nResult = GMP_SUCCESS; 

	if (!m_bModSelected || m_bIsOfflineMode)
	{
		m_lCurStaticControl[WHZ_1] = 1;
		return GMP_SUCCESS;
	}

	if (
		m_lCurStaticControl[WHZ_1] == 1 || 
		m_lStaticControlMode == OFF_MOTOR_MODE || 
		m_lStaticControlMode == NORMAL_MODE
	   )
	{
		if (m_stMotorZ[WHZ_1].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS)
		{
			return GMP_SUCCESS;
		}
	}

	if (
		//m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Static") == GMP_SUCCESS ||
		//m_stMotorZ[WHZ_1].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS
		m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS || //20120910
		m_stMotorZ[WHZ_1].MoveAbsolute(lAbsoluteCnt, bWait) != GMP_SUCCESS
	   )
	{
		nResult = GMP_FAIL;
	}

	m_lCurStaticControl[WHZ_1] = 1;
	return nResult;
}

INT CWorkHolder::ResetZ2ControlPara(LONG lAbsoluteCnt, BOOL bWait)
{
	INT nResult = GMP_SUCCESS; 

	if (!m_bModSelected || m_bIsOfflineMode)
	{
		m_lCurStaticControl[WHZ_2] = 1;
		return GMP_SUCCESS;
	}

	if (
		m_lCurStaticControl[WHZ_2] == 1 || 
		m_lStaticControlMode == OFF_MOTOR_MODE || 
		m_lStaticControlMode == NORMAL_MODE
	   )
	{
		if (m_stMotorZ[WHZ_2].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS)
		{
			return GMP_SUCCESS;
		}
	}

	if (
		//m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Static") == GMP_SUCCESS ||
		//m_stMotorZ[WHZ_2].MoveAbsolute(lAbsoluteCnt, bWait) == GMP_SUCCESS
		m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS || //20120910
		m_stMotorZ[WHZ_2].MoveAbsolute(lAbsoluteCnt, bWait) != GMP_SUCCESS
	   )
	{
		nResult = GMP_FAIL;
	}

	m_lCurStaticControl[WHZ_2] = 1;
	return nResult;
}

/////////////////////////////////////////////////////////////////
//I/O Related
/////////////////////////////////////////////////////////////////
BOOL CWorkHolder::IsVacSensorOn(GlassNum lGlassNum)
{
	if (lGlassNum == GLASS1)
	{
		if(!m_bUseAnvilVacuum)
		{
			return IsGLASS1_VacSensorOn();
		}
		else
		{
			return IsGLASS1_VacSensorOn() && IsGLASS1_AuxVacSensorOn();
		}
	}
	else if (lGlassNum == GLASS2)
	{
		if(!m_bUseAnvilVacuum)
		{
			return IsGLASS2_VacSensorOn();
		}
		else
		{
			return IsGLASS2_VacSensorOn() && IsGLASS2_AuxVacSensorOn();
		}
	}
	else
	{
		SetError(IDS_HK_SOFTWARE_HANDLING_ERR);
		return FALSE;
	}
}

INT CWorkHolder::SetVacSol(GlassNum lGlassNum, BOOL bMode, BOOL bWait)
{
	if (lGlassNum == GLASS1)
	{ 
		if(!m_bUseAnvilVacuum)
		{
			return SetGLASS1_VacSol(bMode, bWait);
		}
		else
		{
			return SetGLASS1_VacSol(bMode, bWait) && SetGLASS1_AuxVacSol(bMode, bWait);
		}
	}
	else if (lGlassNum == GLASS2)
	{
		if(!m_bUseAnvilVacuum)
		{
			return SetGLASS2_VacSol(bMode, bWait);
		}
		else
		{
			return SetGLASS2_VacSol(bMode, bWait) && SetGLASS2_AuxVacSol(bMode, bWait);
		}
	}
	else
	{
		if(!m_bUseAnvilVacuum)
		{
			if (
				SetGLASS1_VacSol(bMode, GMP_NOWAIT) != GMP_SUCCESS ||
				SetGLASS2_VacSol(bMode, GMP_NOWAIT) != GMP_SUCCESS
			)
			{
				return GMP_FAIL;
			}
		}
		else
		{			
			if (
				SetGLASS1_VacSol(bMode, GMP_NOWAIT) != GMP_SUCCESS ||
				SetGLASS1_AuxVacSol(bMode, GMP_NOWAIT) != GMP_SUCCESS ||
				SetGLASS2_AuxVacSol(bMode, GMP_NOWAIT) != GMP_SUCCESS ||
				SetGLASS2_VacSol(bMode, GMP_NOWAIT) != GMP_SUCCESS
			)
			{
				return GMP_FAIL;
			}
		}
		if (bWait == GMP_WAIT)
		{
			if (bMode)
			{
				Sleep(m_stGlass1VacSol.GetOnDelay());
			}
			else
			{
				Sleep(m_stGlass1VacSol.GetOffDelay());
			}
		}

		return GMP_SUCCESS;
	}
	return GMP_FAIL;
}	

INT CWorkHolder::SetWeakBlowSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stWeakBlowSol;

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

INT CWorkHolder::SetWeakBlow2Sol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stWeakBlow2Sol;

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

BOOL CWorkHolder::IsGLASS1_VacSensorOn()
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
		bResult = m_stGlass1VacSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CWorkHolder::IsGLASS2_VacSensorOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return GetGlass2Status();
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return GetGlass2Status();
	}
#endif

	try
	{
		bResult = m_stGlass2VacSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CWorkHolder::IsGLASS1_AuxVacSensorOn()
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
		bResult = m_stGlass1AuxVacSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CWorkHolder::IsGLASS2_AuxVacSensorOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return GetGlass2Status();
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return GetGlass2Status();
	}
#endif

	try
	{
		bResult = m_stGlass2AuxVacSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

INT CWorkHolder::SetGLASS1_VacSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stGlass1VacSol;

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

			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
			}

			TC_DelJob(stTempSo->GetTCObjectID());
		}
		else
		{
			TC_AddJob(stTempSo->GetTCObjectID());

			if (!g_bDiagMode)
			{
				stTempSo->GetGmpPort().Off();
				stTempSo->bHMI_Status = FALSE;
			}
			else
			{
				stTempSo->GetGmpPort().On();
				stTempSo->bHMI_Status = TRUE;
			}
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

INT CWorkHolder::SetGLASS2_VacSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stGlass2VacSol;

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

			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
			}

			TC_DelJob(stTempSo->GetTCObjectID());
		}
		else
		{
			TC_AddJob(stTempSo->GetTCObjectID());

			if (!g_bDiagMode)
			{
				stTempSo->GetGmpPort().Off();
				stTempSo->bHMI_Status = FALSE;
			}
			else
			{
				stTempSo->GetGmpPort().On();
				stTempSo->bHMI_Status = TRUE;
			}
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


INT CWorkHolder::SetGLASS1_AuxVacSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stGlass1AuxVacSol;

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

			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
			}

			TC_DelJob(stTempSo->GetTCObjectID());
		}
		else
		{
			TC_AddJob(stTempSo->GetTCObjectID());

			if (!g_bDiagMode)
			{
				stTempSo->GetGmpPort().Off();
				stTempSo->bHMI_Status = FALSE;
			}
			else
			{
				stTempSo->GetGmpPort().On();
				stTempSo->bHMI_Status = TRUE;
			}
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

INT CWorkHolder::SetGLASS2_AuxVacSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stGlass2AuxVacSol;

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

			if (bWait)
			{
				Sleep(stTempSo->GetOnDelay());
			}

			TC_DelJob(stTempSo->GetTCObjectID());
		}
		else
		{
			TC_AddJob(stTempSo->GetTCObjectID());

			if (!g_bDiagMode)
			{
				stTempSo->GetGmpPort().Off();
				stTempSo->bHMI_Status = FALSE;
			}
			else
			{
				stTempSo->GetGmpPort().On();
				stTempSo->bHMI_Status = TRUE;
			}
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

// ------------------------------------------------------------------------
// HMI Command delcaration
// ------------------------------------------------------------------------
LONG CWorkHolder::HMI_PowerOnY(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorY.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CWorkHolder::HMI_PowerOnT(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorT.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}


LONG CWorkHolder::HMI_PowerOnZ1(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorZ[WHZ_1].PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CWorkHolder::HMI_PowerOnZ2(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorZ[WHZ_2].PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CWorkHolder::HMI_PowerOnX1(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorX.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CWorkHolder::HMI_PowerOnX2(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorX2.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CWorkHolder::HMI_PowerOnY1(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorY.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CWorkHolder::HMI_PowerOnY2(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_stMotorY2.PowerOn(bMode);

	bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}



// Commutate
LONG CWorkHolder::HMI_CommY(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.Commutate();

	return 0;
}

LONG CWorkHolder::HMI_CommT(IPC_CServiceMessage &svMsg)
{
	m_stMotorT.Commutate();

	return 0;
}

LONG CWorkHolder::HMI_CommZ1(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ[WHZ_1].Commutate();

	return 0;
}

LONG CWorkHolder::HMI_CommZ2(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ[WHZ_2].Commutate();

	return 0;
}

LONG CWorkHolder::HMI_CommX1(IPC_CServiceMessage &svMsg)
{
	m_stMotorX.Commutate();

	return 0;
}

LONG CWorkHolder::HMI_CommX2(IPC_CServiceMessage &svMsg)
{
	m_stMotorX2.Commutate();

	return 0;
}

LONG CWorkHolder::HMI_CommY1(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.Commutate();

	return 0;
}

LONG CWorkHolder::HMI_CommY2(IPC_CServiceMessage &svMsg)
{
	m_stMotorY2.Commutate();

	return 0;
}

// Home
LONG CWorkHolder::HMI_HomeY(IPC_CServiceMessage &svMsg)
{
	CString szErrMsg("No_Error");
	m_stMotorY.Home(GMP_WAIT, szErrMsg);
	if (szErrMsg != "No_Error")
	{
		HMIMessageBox(MSG_OK, "Home Error", szErrMsg);
		DisplayMessage(szErrMsg);
		//SetError(IDS_MAINBOND_MTR_HOME_ERR + (m_lCurSlave * CONVERT_TO_MAPPING_MB), szErrMsg);
	}

	return 0;
}

LONG CWorkHolder::HMI_HomeZ1(IPC_CServiceMessage &svMsg)
{

	m_stMotorZ[WHZ_1].Home(GMP_WAIT);
	m_stMotorZ[WHZ_1].DisableProtection(GMP_PROT_TYPE_SW_LMT);
	return 0;
}

LONG CWorkHolder::HMI_HomeZ2(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ[WHZ_2].Home(GMP_WAIT);
	m_stMotorZ[WHZ_2].DisableProtection(GMP_PROT_TYPE_SW_LMT);

	return 0;
}

LONG CWorkHolder::HMI_HomeX1(IPC_CServiceMessage &svMsg)
{
	m_stMotorX.Home(GMP_WAIT);

	return 0;
}

LONG CWorkHolder::HMI_HomeX2(IPC_CServiceMessage &svMsg)
{
	m_stMotorX2.Home(GMP_WAIT);

	return 0;
}

LONG CWorkHolder::HMI_HomeY1(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.Home(GMP_WAIT);

	return 0;
}

LONG CWorkHolder::HMI_HomeY2(IPC_CServiceMessage &svMsg)
{
	m_stMotorY2.Home(GMP_WAIT);

	return 0;
}

LONG CWorkHolder::HMI_HomeT(IPC_CServiceMessage &svMsg)
{
	LogAction(__FUNCTION__);

	m_stMotorT.Home(GMP_WAIT);

	return 0;
}

LONG CWorkHolder::HMI_ToggleG1VacSol(IPC_CServiceMessage &svMsg)
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

LONG CWorkHolder::HMI_ToggleG2VacSol(IPC_CServiceMessage &svMsg)
{
	if (m_stGlass2VacSol.bHMI_Status)
	{
		SetGLASS2_VacSol(OFF);
	}
	else
	{
		SetGLASS2_VacSol(ON);
	}

	return 0;
}

LONG CWorkHolder::HMI_ToggleG1AuxVacSol(IPC_CServiceMessage &svMsg)
{
	if (m_stGlass1AuxVacSol.bHMI_Status)
	{
		SetGLASS1_AuxVacSol(OFF);
	}
	else
	{
		SetGLASS1_AuxVacSol(ON);
	}

	return 0;
}

LONG CWorkHolder::HMI_ToggleG2AuxVacSol(IPC_CServiceMessage &svMsg)
{
	if (m_stGlass2AuxVacSol.bHMI_Status)
	{
		SetGLASS2_AuxVacSol(OFF);
	}
	else
	{
		SetGLASS2_AuxVacSol(ON);
	}

	return 0;
}

LONG CWorkHolder::HMI_ToggleWeakBlowSol(IPC_CServiceMessage &svMsg)
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

LONG CWorkHolder::HMI_ToggleWeakBlow2Sol(IPC_CServiceMessage &svMsg)
{
	if (m_stWeakBlow2Sol.bHMI_Status)
	{
		SetWeakBlow2Sol(OFF);
	}
	else
	{
		SetWeakBlow2Sol(ON);
	}

	return 0;
}

// Setup Page Related
LONG CWorkHolder::HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg)
{
	struct
	{
		ULONG ulSteps;
		BOOL bFineStep;
	} stSetting;
	svMsg.GetMsg(sizeof(stSetting), &stSetting);

	SetDiagnSteps(stSetting.ulSteps, stSetting.bFineStep);
	return 0;
}

LONG CWorkHolder::HMI_IndexX1Pos(IPC_CServiceMessage &svMsg)
{
	m_stMotorX.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexX1Neg(IPC_CServiceMessage &svMsg)
{
	m_stMotorX.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexX2Pos(IPC_CServiceMessage &svMsg)
{
	m_stMotorX2.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexX2Neg(IPC_CServiceMessage &svMsg)
{
	m_stMotorX2.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexYPos(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexYNeg(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexY1Pos(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexY1Neg(IPC_CServiceMessage &svMsg)
{
	m_stMotorY.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexY2Pos(IPC_CServiceMessage &svMsg)
{
	m_stMotorY2.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexY2Neg(IPC_CServiceMessage &svMsg)
{
	m_stMotorY2.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexZ1Pos(IPC_CServiceMessage &svMsg)
{
	//m_stMotorZ[WHZ_1].DisableProtection(GMP_PROT_TYPE_SW_LMT);
#if 0 //whz rotary linear calib
	LONG lResultLevel = 0;
	if (CalcWHAbsoluteLevel(m_stMotorZ[WHZ_1].GetEncPosn(), m_lDiagnSteps, m_stWHCalibZ, &lResultLevel, WHZ_1))
	{
		
		if (
			(lResultLevel < m_stMotorZ[WHZ_1].stProtect.lNegLmt || 
			 lResultLevel > m_stMotorZ[WHZ_1].stProtect.lPosLmt)
		   )
		{
			HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
			return 0;
		}

		//m_stMotorZ[WHZ_1].DisableProtection(GMP_PROT_TYPE_SW_LMT);
		m_stMotorZ[WHZ_1].MoveAbsolute(lResultLevel, GMP_WAIT);
	}
	else
	{
		HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
	}
#else
	m_stMotorZ[WHZ_1].MoveRelative(m_lDiagnSteps, GMP_WAIT);
#endif
	//m_stMotorZ[WHZ_1].DisableProtection(GMP_PROT_TYPE_SW_LMT);
	return 0;
}

LONG CWorkHolder::HMI_IndexZ1Neg(IPC_CServiceMessage &svMsg)
{
#if 0
	LONG lResultLevel = 0;
	if (CalcWHAbsoluteLevel(m_stMotorZ[WHZ_1].GetEncPosn(), -m_lDiagnSteps, m_stWHCalibZ, &lResultLevel, WHZ_1))
	{
		
		if (
			(lResultLevel < m_stMotorZ[WHZ_1].stProtect.lNegLmt || 
			 lResultLevel > m_stMotorZ[WHZ_1].stProtect.lPosLmt)
		   )
		{
			HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
			return 0;
		}

		m_stMotorZ[WHZ_1].MoveAbsolute(lResultLevel, GMP_WAIT);
	}
	else
	{
		HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
	}
#else
	m_stMotorZ[WHZ_1].MoveRelative(-m_lDiagnSteps, GMP_WAIT);
#endif
	return 0;
}

LONG CWorkHolder::HMI_IndexZ2Pos(IPC_CServiceMessage &svMsg)
{
#if 0 //whz rotary linear calib
	LONG lResultLevel = 0;
	if (CalcWHAbsoluteLevel(m_stMotorZ[WHZ_2].GetEncPosn(), m_lDiagnSteps, m_stWHCalibZ, &lResultLevel, WHZ_2))
	{
		
		if (
			(lResultLevel < m_stMotorZ[WHZ_2].stProtect.lNegLmt || 
			 lResultLevel > m_stMotorZ[WHZ_2].stProtect.lPosLmt)
		   )
		{
			HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
			return 0;
		}

		m_stMotorZ[WHZ_2].MoveAbsolute(lResultLevel, GMP_WAIT);
	}
	else
	{
		HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
	}
#else
	m_stMotorZ[WHZ_2].MoveRelative(m_lDiagnSteps, GMP_WAIT);
#endif
	return 0;
}

LONG CWorkHolder::HMI_IndexZ2Neg(IPC_CServiceMessage &svMsg)
{
#if 0
	LONG lResultLevel = 0;
	if (CalcWHAbsoluteLevel(m_stMotorZ[WHZ_2].GetEncPosn(), -m_lDiagnSteps, m_stWHCalibZ, &lResultLevel, WHZ_2))
	{
		
		if (
			(lResultLevel < m_stMotorZ[WHZ_2].stProtect.lNegLmt || 
			 lResultLevel > m_stMotorZ[WHZ_2].stProtect.lPosLmt)
		   )
		{
			HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
			return 0;
		}

		m_stMotorZ[WHZ_2].MoveAbsolute(lResultLevel, GMP_WAIT);
	}
	else
	{
		HMIMessageBox(MSG_OK, "INDEX Z", "Software Position Limit Hit. Operation Abort!");
	}
#else
	m_stMotorZ[WHZ_2].MoveRelative(-m_lDiagnSteps, GMP_WAIT);
#endif
	return 0;
}

LONG CWorkHolder::HMI_IndexTPos(IPC_CServiceMessage &svMsg)
{
	m_stMotorT.MoveRelative(m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_IndexTNeg(IPC_CServiceMessage &svMsg)
{
	m_stMotorT.MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_SetZStaticControlMode(IPC_CServiceMessage &svMsg)
{
	LONG lMode;
	svMsg.GetMsg(sizeof(LONG), &lMode);

	m_lStaticControlMode = lMode;
#if 0 //20120618
	INT nResult = GMP_SUCCESS;
	if (m_lStaticControlMode == KI_0_MODE)
	{
		if (
			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Dynamic1") != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Dynamic1") != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_1] = 0;
		m_lCurStaticControl[WHZ_2] = 0;
	}
	else if (m_lStaticControlMode == NORMAL_MODE)
	{
		if (
			m_stMotorZ[WHZ_1].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].SelectStaticCtrlPara("PL_Static") != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].MoveRelative(0, GMP_NOWAIT) != GMP_SUCCESS ||
			m_stMotorZ[WHZ_1].Sync() != GMP_SUCCESS ||
			m_stMotorZ[WHZ_2].Sync() != GMP_SUCCESS
		   )
		{
			nResult = GMP_FAIL;
		}
		m_lCurStaticControl[WHZ_1] = 1;
		m_lCurStaticControl[WHZ_2] = 1;
	}
#endif
	return 0;
}

LONG CWorkHolder::IPC_SaveMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.x"]				= m_mtrStandbyPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"]				= m_mtrStandbyPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.x"]				= m_mtrLoadGlassPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.y"]				= m_mtrLoadGlassPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.x"]			= m_mtrUnloadGlassPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.y"]			= m_mtrUnloadGlassPosn.y;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrApproxBondPosn.x"]			= m_mtrApproxBondPosn.x;
	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrApproxBondPosn.y"]			= m_mtrApproxBondPosn.y;

	pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lStandbyPosnT"]					= m_lStandbyPosnT;

	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lStandbyLevel"]		= m_lStandbyLevel[i];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibStageLevel"]		= m_lCalibStageLevel[i];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibLoadLevel"]		= m_lCalibLoadLevel[i];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibUnloadLevel"]	= m_lCalibUnloadLevel[i];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"]		= m_lPRCalibLevel[i];
		pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lApproxBondLevel"]		= m_lApproxBondLevel[i];

	}
#if 1
	for (int j = 0; j < MAX_NUM_OF_WHZ; j++)
	{
		str.Format("WHZ_%ld", j + 1);
		for (i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++) //whz rotary linear calib
		{
			pAppMod->m_smfMachine[GetStnName()]["CalibZ"][str][i]["LinearPosn"]				= m_stWHCalibZ[j][i].LinearPosn;
			pAppMod->m_smfMachine[GetStnName()]["CalibZ"][str][i]["RotaryPosn"]				= m_stWHCalibZ[j][i].RotaryPosn;
		}
	}
#else
	for (int j = 0; j < MAX_NUM_OF_WHZ; j++)
	{
		for (i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++) //whz rotary linear calib
		{
			pAppMod->m_smfMachine[GetStnName()]["CalibZ"][j][i]["LinearPosn"]				= m_stWHCalibZ[j][i].LinearPosn;
			pAppMod->m_smfMachine[GetStnName()]["CalibZ"][j][i]["RotaryPosn"]				= m_stWHCalibZ[j][i].RotaryPosn;
		}
	}
#endif
	return CPI9000Stn::IPC_SaveMachineParam();
}

LONG CWorkHolder::IPC_LoadMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	m_mtrStandbyPosn.x		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.x"];
	m_mtrStandbyPosn.y		= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrStandbyPosn.y"];
	m_mtrLoadGlassPosn.x	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.x"];
	m_mtrLoadGlassPosn.y	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.y"];
	m_mtrUnloadGlassPosn.x	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.x"];
	m_mtrUnloadGlassPosn.y	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.y"];
	m_mtrApproxBondPosn.x	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrApproxBondPosn.x"];
	m_mtrApproxBondPosn.y	= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrApproxBondPosn.y"];

	m_lStandbyPosnT			= pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lStandbyPosnT"];

	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		m_lStandbyLevel[i]		= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lStandbyLevel"];
		m_lCalibStageLevel[i]	= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibStageLevel"];
		m_lCalibLoadLevel[i]	= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibLoadLevel"];
		m_lCalibUnloadLevel[i]	= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibUnloadLevel"];
		m_lPRCalibLevel[i]		= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"];
		m_lApproxBondLevel[i]	= pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lApproxBondLevel"];

	}
#if 1
	for (int j = 0; j < MAX_NUM_OF_WHZ; j++)
	{
		str.Format("WHZ_%ld", j + 1);
		for (i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++) //whz rotary linear calib
		{
			m_stWHCalibZ[j][i].LinearPosn =	pAppMod->m_smfMachine[GetStnName()]["CalibZ"][str][i]["LinearPosn"];
			m_stWHCalibZ[j][i].RotaryPosn =	pAppMod->m_smfMachine[GetStnName()]["CalibZ"][str][i]["RotaryPosn"];
		}
	}
#else
	for (int j = 0; j < MAX_NUM_OF_WHZ; j++)
	{
		for (i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++) //whz rotary linear calib
		{
			m_stWHCalibZ[j][i].LinearPosn =	pAppMod->m_smfMachine[GetStnName()]["CalibZ"][j][i]["LinearPosn"];
			m_stWHCalibZ[j][i].RotaryPosn =	pAppMod->m_smfMachine[GetStnName()]["CalibZ"][j][i]["RotaryPosn"];
		}
	}
#endif

	return CPI9000Stn::IPC_LoadMachineParam();
}

LONG CWorkHolder::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;
	

	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lLoadLevelOffset"]			= m_lLoadLevelOffset[i];
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadLevelOffset"]		= m_lUnloadLevelOffset[i];
		pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lPRLevelOffset"]			= m_lPRLevelOffset[i];

	}

	pAppMod->m_smfDevice[GetStnName()]["Para"]["m_lStaticControlMode"]				= m_lStaticControlMode;

	return CPI9000Stn::IPC_SaveDeviceParam();
}

LONG CWorkHolder::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str;

	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		m_lLoadLevelOffset[i]		= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lLoadLevelOffset"];
		m_lUnloadLevelOffset[i]		= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lUnloadLevelOffset"];
		m_lPRLevelOffset[i]			= pAppMod->m_smfDevice[GetStnName()]["Level"][str]["m_lPRLevelOffset"];
	}

	m_lStaticControlMode		= pAppMod->m_smfDevice[GetStnName()]["Para"]["m_lStaticControlMode"];

	if (GetStnName() == "PreBondWH")
	{
		m_lStaticControlMode = NORMAL_MODE;
	}

	return CPI9000Stn::IPC_LoadDeviceParam();
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
BOOL CWorkHolder::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;
	CString str;

	if (bResult)
	{
		////Standby
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
		////Load Glass
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrLoadGlassPosn.x"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrLoadGlassPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrLoadGlassPosn.y"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	////Unload Glass
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrUnloadGlassPosn.x"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrUnloadGlassPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrUnloadGlassPosn.y"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	////Approx Bond
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrApproxBondPosn.x"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrApproxBondPosn.x"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_mtrApproxBondPosn.y"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("mtrApproxBondPosn.y"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	//Theta
	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Posn"]["m_lStandbyPosnT"];
		bResult = printMachineInfoRow(szModule, CString("Posn"), CString("StandbyPosnT"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	//Level
	for (INT i = WHZ_1; i < MAX_NUM_OF_WHZ; i++)
	{
		str.Format("WHZ_%ld", i + 1);
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lStandbyLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), str, CString("StandbyLevel"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibStageLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), str, CString("CalibStageLevel"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibLoadLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), str, CString("CalibLoadLevel"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lCalibUnloadLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), str, CString("CalibUnloadLevel"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lPRCalibLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), str, CString("PRCalibLevel"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Level"][str]["m_lApproxBondLevel"];
		bResult = printMachineInfoRow(szModule, CString("Level"), str, CString("ApproxBondLevel"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
	}
	if (bResult)
	{
		bResult = CPI9000Stn::printMachineDeviceInfo();
	}

	return bResult;
}
#endif

CMotorInfo &CWorkHolder::GetMotorZ(WHZNum lWHZNum)  //whz rotary linear calib
{
	if (lWHZNum == WHZ_1)
	{
		return m_stMotorZ[WHZ_1];
	}
	else if (lWHZNum == WHZ_2)
	{
		return m_stMotorZ[WHZ_2];
	}
	else
	{
		return m_stMotorZ[WHZ_1];
	}
}

VOID CWorkHolder::PrintCalibDataToHMI() //whz rotary linear calib
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szFileName;
	szFileName = "D:\\sw\\PI9000\\Param\\" + pAppMod->m_CurrMotor + ".csv";	//e.g.: INSPWH_Z1.csv
	// Set data to file
	FILE *fp = fopen(szFileName, "wt");
	if (fp != NULL)
	{
		if (
			pAppMod->m_CurrMotor == "INSPWH_Z2"	||
			pAppMod->m_CurrMotor == "PBWH_Z2"
		   )
		{
			for (INT i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++)
			{
				fprintf(fp, "%ld,%ld\n", m_stWHCalibZ[WHZ_2][i].LinearPosn, m_stWHCalibZ[WHZ_2][i].RotaryPosn);
			}
		}
		else
		{
			for (INT i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++)
			{
				fprintf(fp, "%ld,%ld\n", m_stWHCalibZ[WHZ_1][i].LinearPosn, m_stWHCalibZ[WHZ_1][i].RotaryPosn);
			}
		}
		fclose(fp);
	}
}

BOOL CWorkHolder::CalcWHZLinearEncoder() //whz rotary linear calib
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");

	PRINT_DATA	stPrintData;
	CString		szLogFileName;
	CString		szMsg;
	
	LogAction(__FUNCTION__);

	CString szpassword	= pCHouseKeeping->szEngPassword;

	if (!HMIGetPassword("Please enter Engineer Password", szpassword))
	{
		return FALSE;
	}

	if (!m_bModSelected)
	{
		szMsg.Format("%s Module Not Selected. Operation Abort!", GetStnName());
		HMIMessageBox(MSG_OK, "CALC WH Z LINEAR ENCODER", szMsg);
		return FALSE;
	}
	if (
		pAppMod->m_CurrMotor == "INSPWH_Z1"	||
		pAppMod->m_CurrMotor == "PBWH_Z1"	||
		pAppMod->m_CurrMotor == "MB1WH_Z"	||
		pAppMod->m_CurrMotor == "MB3WH_Z"	||
		pAppMod->m_CurrMotor == "MB2WH_Z"	||
		pAppMod->m_CurrMotor == "MB4WH_Z"
	   )
	{
		if (!pAppMod->CalcWHZLinearEncoder(this, m_stMotorZ[WHZ_1].stProtect, &m_stWHCalibZ[WHZ_1][0]))
		{
			HMIMessageBox(MSG_OK, "CALC WH Z LINEAR ENCODER", "Calibration Fail!");
			return FALSE;
		}
	}
	else if (
			 pAppMod->m_CurrMotor == "INSPWH_Z2"	||
		 
			 pAppMod->m_CurrMotor == "PBWH_Z2"
			)
	{
		if (!pAppMod->CalcWHZLinearEncoder(this, m_stMotorZ[WHZ_2].stProtect, &m_stWHCalibZ[WHZ_2][0]))
		{
			HMIMessageBox(MSG_OK, "CALC WH Z LINEAR ENCODER", "Calibration Fail!");
			return FALSE;
		}
	}

	stPrintData.szTitle[0]	= "Time";
	stPrintData.szTitle[1]	= "No";
	stPrintData.szTitle[2]	= "Linear Encoder";
	stPrintData.szTitle[3]	= "Rotary Encoder";
	szLogFileName = "Calculation Log\\" + GetStnName() + "_CalWhZEncoderLog";
		
	pCHouseKeeping->PrintHeader(GetStnName(), szLogFileName);
	pCHouseKeeping->PrintTitle(szLogFileName, stPrintData.szTitle);



	if (
		pAppMod->m_CurrMotor == "INSPWH_Z1"	||
		pAppMod->m_CurrMotor == "PBWH_Z1"	||
		pAppMod->m_CurrMotor == "MB1WH_Z"	||
		pAppMod->m_CurrMotor == "MB3WH_Z"	||
		pAppMod->m_CurrMotor == "MB2WH_Z"	||
		pAppMod->m_CurrMotor == "MB4WH_Z"
	   )
	{
		for (INT i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++)
		{
			stPrintData.dData[0] = i;
			stPrintData.dData[1] = m_stWHCalibZ[WHZ_1][i].LinearPosn;
			stPrintData.dData[2] = m_stWHCalibZ[WHZ_1][i].RotaryPosn;
			pCHouseKeeping->PrintData(szLogFileName, stPrintData);
		}
		m_stMotorZ[WHZ_1].MoveAbsolute(m_stMotorZ[WHZ_1].stProtect.lNegLmt, GMP_WAIT);

	}
	else if (
			 pAppMod->m_CurrMotor == "INSPWH_Z2"	||
			 pAppMod->m_CurrMotor == "PBWH_Z2"
			)
	{		
		for (INT i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++)
		{
			stPrintData.dData[0] = i;
			stPrintData.dData[1] = m_stWHCalibZ[WHZ_2][i].LinearPosn;
			stPrintData.dData[2] = m_stWHCalibZ[WHZ_2][i].RotaryPosn;
			pCHouseKeeping->PrintData(szLogFileName, stPrintData);
		}
		m_stMotorZ[WHZ_2].MoveAbsolute(m_stMotorZ[WHZ_2].stProtect.lNegLmt, GMP_WAIT);

	}
	//MoveZ(WHZ_1, MAINBOND_WH_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
	PrintCalibDataToHMI();

	HMIMessageBox(MSG_OK, "CALC WH Z LINEAR ENCODER", "Calibration Sucess!");

	return TRUE;
}

LONG CWorkHolder::HMI_GoZ1PosLmt(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ[WHZ_1].MoveAbsolute(m_stMotorZ[WHZ_1].stProtect.lPosLmt, GMP_WAIT);
	return 0;
}
LONG CWorkHolder::HMI_GoZ1NegLmt(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ[WHZ_1].MoveAbsolute(m_stMotorZ[WHZ_1].stProtect.lNegLmt, GMP_WAIT);
	return 0;
}
LONG CWorkHolder::HMI_GoZ2PosLmt(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ[WHZ_2].MoveAbsolute(m_stMotorZ[WHZ_2].stProtect.lPosLmt, GMP_WAIT);
	return 0;
}
LONG CWorkHolder::HMI_GoZ2NegLmt(IPC_CServiceMessage &svMsg)
{
	m_stMotorZ[WHZ_2].MoveAbsolute(m_stMotorZ[WHZ_2].stProtect.lNegLmt, GMP_WAIT);
	return 0;
}

LONG CWorkHolder::HMI_ToggleNotUseAnvilVacuum(IPC_CServiceMessage &svMsg)
{
	m_bUseAnvilVacuum = !m_bUseAnvilVacuum;
	return 0;
}