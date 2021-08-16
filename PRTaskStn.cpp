/////////////////////////////////////////////////////////////////
//	PRTaskStn.cpp : interface of the CWorkHolder class
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
#include "PI9000.h"

#define PRTASKSTN_EXTERN
#include "PRTaskStn.h"
#include <typeinfo.h>

#include "FileOperations.h"
#include "ExcelMotorInfo_Util.h"
#include "BasicExcel.h"
#include "WinEagle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPRTaskStn::CPRTaskStn()
{
	INT i = 0;

	m_szPRTaskStnName				= "";
	m_lPRModSelectMode				= GLASS_MOD;

	m_bPRSelected					= TRUE;
	m_bPRError						= FALSE;

	m_bPRCycle						= FALSE;
	m_ulPRCycle						= 0;

	m_lAddJobPR_GRAB				= 0;		
	m_lAddJobPR_PROCESS				= 0;

	m_lPRRetryCounter				= 0;
	m_lPRRetryLimit					= 0;
	m_lRejectGlassCounter			= 0;
	m_lRejectGlassLimit				= 0;
	m_lPRDelay						= 0;
	m_lPRDelay2			= 0;
	m_lCalibPRDelay					= 0;
	m_lPRErrorLimit					= 10;

	m_dmtrCurrPR1Posn.x				= 0.0;
	m_dmtrCurrPR1Posn.y				= 0.0;
	m_dmtrCurrPR2Posn.x				= 0.0;
	m_dmtrCurrPR2Posn.y				= 0.0;
	m_dmtrCurrAlign1Offset.x		= 0.0;
	m_dmtrCurrAlign1Offset.y		= 0.0;
	m_dmtrCurrAlign2Offset.x		= 0.0;
	m_dmtrCurrAlign2Offset.y		= 0.0;
	m_dmtrCurrAlignCentre.x			= 0.0;
	m_dmtrCurrAlignCentre.y			= 0.0;
	m_dCurrAlignAngle				= 0.0;
	m_dCurrPreciseAlginAngle		= 0.0;
	m_dCurrAlignDist				= 0.0;

	m_nPRInProgress					= ST_PR_IDLE;

	//PostBond Only
	for (INT i = 0; i < PR_NO_OF_CORNERS; i++)
	{
		m_PRCurrPBPR1Corner[i].x			= 0;
		m_PRCurrPBPR1Corner[i].y			= 0;
		m_PRCurrPBPR2Corner[i].x			= 0;
		m_PRCurrPBPR2Corner[i].y			= 0;
	}

	//Auto Calib parameters
	for (INT i = 0; i < NO_OF_CAL_PT; i++)
	{
		m_stPRCalibPts[i].x					= 0;
		m_stPRCalibPts[i].y					= 0;
		m_stMtrCalibPts[i].x				= 0;
		m_stMtrCalibPts[i].y				= 0;
	}

	//// COR parameters
	for (INT j = 0; j < MAX_NUM_OF_TA4_COR; j++)
	{
		for (INT i = 0; i < COR_ARRAY_SIZE; i++)
		{
			m_stCORMtrCalibPts[j][i].x				= 0;
			m_stCORMtrCalibPts[j][i].y				= 0;
			m_dmtrTableCOR_Pos[j][i].x				= 0;
			m_dmtrTableCOR_Pos[j][i].y				= 0;
			m_dmtrTableCOR_Neg[j][i].x				= 0;
			m_dmtrTableCOR_Neg[j][i].y				= 0;
			m_dCORLUT[j][i]						= 0;
		}
	}
	
#ifdef ENCODER_BASED //use encoder value instead of command motor count
	for (INT ii = (INT)GLASS1; ii < (INT)MAX_NUM_OF_GLASS; ii++)
	{
		for (INT jj = (INT)LOOK_GLASS_TYPE; jj < (INT)MAX_NUM_OF_OBJ_LOOK; jj++)
		{
			m_mtrCurrPR1BeforeEncPosn[ii][jj].x = 0;
			m_mtrCurrPR1BeforeEncPosn[ii][jj].y = 0;

			m_mtrCurrPR2BeforeEncPosn[ii][jj].x = 0;
			m_mtrCurrPR2BeforeEncPosn[ii][jj].y = 0;

			m_mtrCurrPR1AfterEncPosn[ii][jj].x = 0;
			m_mtrCurrPR1AfterEncPosn[ii][jj].y = 0;

			m_mtrCurrPR2AfterEncPosn[ii][jj].x = 0;
			m_mtrCurrPR2AfterEncPosn[ii][jj].y = 0;

			m_dmtrCurrPR1BeforePRPosn[ii][jj].x = 0.0;
			m_dmtrCurrPR1BeforePRPosn[ii][jj].y = 0.0;

			m_dmtrCurrPR1AfterPRPosn[ii][jj].x = 0.0;
			m_dmtrCurrPR1AfterPRPosn[ii][jj].y = 0.0;

			m_dmtrCurrPR2BeforePRPosn[ii][jj].x = 0.0;
			m_dmtrCurrPR2BeforePRPosn[ii][jj].y = 0.0;

			m_dmtrCurrPR2AfterPRPosn[ii][jj].x = 0.0;
			m_dmtrCurrPR2AfterPRPosn[ii][jj].y = 0.0;

			m_dmtrCurrAlign1OffsetBefore[ii][jj].x = 0.0;
			m_dmtrCurrAlign1OffsetBefore[ii][jj].y = 0.0;
			
			m_dmtrCurrAlign1OffsetAfter[ii][jj].x = 0.0;
			m_dmtrCurrAlign1OffsetAfter[ii][jj].y = 0.0;

			m_dmtrCurrAlign2OffsetBefore[ii][jj].x = 0.0;
			m_dmtrCurrAlign2OffsetBefore[ii][jj].y = 0.0;

			m_dmtrCurrAlign2OffsetAfter[ii][jj].x = 0.0;
			m_dmtrCurrAlign2OffsetAfter[ii][jj].y = 0.0;

			m_dmtrCurrAlignCentreBefore[ii][jj].x = 0.0;
			m_dmtrCurrAlignCentreBefore[ii][jj].y = 0.0;

			m_dmtrCurrAlignCentreAfter[ii][jj].x = 0.0;
			m_dmtrCurrAlignCentreAfter[ii][jj].y = 0.0;

			m_dCurrAlignAngleBefore[ii][jj] = 0.0;
			m_dCurrAlignAngleAfter[ii][jj] = 0.0;
		}
	}
#endif

	for (INT j = 0; j < MAX_NUM_OF_TA4_COR; j++)
	{
		m_bCORCalibrated[j]					= FALSE;
	}
	//m_dCalibCORRange					= 5.0;
	m_dCalibCORRange					= 20.0; //20130318

	m_mtrThermalCalibRefPosn.x			= 0; //20140627 PBDL cal
	m_mtrThermalCalibRefPosn.y			= 0;
	m_mtrThermalCalibOffset.x			= 0; //20140707 thermal compensation
	m_mtrThermalCalibOffset.y			= 0;
#ifdef PB_THERMAL_CAL
	m_mtrThermalCalibAlignOffset.x = 0; //20151009
	m_mtrThermalCalibAlignOffset.y = 0;
#endif

	HMI_ulCurPRU						= 0;
	HMI_ulPRID							= 0;
	HMI_dPRMatchScore					= 85;
	HMI_dPRAngleRange					= 3.0;
	HMI_dPRPercentVar					= 5.0;
	HMI_bShowPRSearch					= FALSE;
	HMI_dPostBdRejectTol				= 5.0;
	HMI_ulPRPostBdAttrib				= PR_DEFECT_ATTRIBUTE_BRIGHT;
	HMI_lSegThreshold					= 0;
	HMI_lDefectThreshold				= 0;

	// Basic
	m_lIsAlign							= 0;
	m_lAlignAlg							= 0;

	HMI_bShowSegThreshold				= FALSE;
	HMI_bShowBasicAlg					= FALSE;
	HMI_bShowAdvanceAlg					= FALSE;	

	m_motorDummy.stAttrib.szAxis = ""; //klocwork fix 20121211

}

CPRTaskStn::~CPRTaskStn()
{
}
/////////////////////////////////////////////////////////////////
// Virtual Move
/////////////////////////////////////////////////////////////////
// Fix me
CMotorInfo &CPRTaskStn::GetMotorX() // Suppose the child will be used
{
#if 1 //klocwork fix 20121211
	return m_motorDummy;
#else
	//Create A DUMMY CMotorInfo;
	CMotorInfo stDummyMotor;
	stDummyMotor.stAttrib.szAxis = "";
	return stDummyMotor;
#endif
}

CMotorInfo &CPRTaskStn::GetMotorY()
{
#if 1 //klocwork fix 20121211
	return m_motorDummy;
#else
	//Create A DUMMY CMotorInfo;
	CMotorInfo stDummyMotor;
	stDummyMotor.stAttrib.szAxis = "";
	return stDummyMotor;
#endif
}

CMotorInfo &CPRTaskStn::GetMotorT()
{
#if 1 //klocwork fix 20121211
	return m_motorDummy;
#else
	//Create A DUMMY CMotorInfo;
	CMotorInfo stDummyMotor;
	stDummyMotor.stAttrib.szAxis = "";
	return stDummyMotor;
#endif
}

CMotorInfo &CPRTaskStn::GetMotorZ()
{
#if 1 //klocwork fix 20121211
	return m_motorDummy;
#else
	//Create A DUMMY CMotorInfo;
	CMotorInfo stDummyMotor;
	stDummyMotor.stAttrib.szAxis = "";
	return stDummyMotor;
#endif
}

INT CPRTaskStn::SyncX()
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::SyncY()
{

	return GMP_SUCCESS;
}

INT CPRTaskStn::SyncZ()
{

	return GMP_SUCCESS;
}

INT CPRTaskStn::SyncT()
{

	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveZToPRCalibLevel(BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveXYToPRCalibPosn(BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveTToStandby(BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveXYToCurrPR1Posn(BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveZToPRLevel(BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveXYToCurrPR2Posn(BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait, BOOL bMoveToSafeX)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait, BOOL bMoveToSafeX)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveAbsoluteT(LONG lMtrCnt, BOOL bWait)
{
	return GMP_SUCCESS;
}

INT CPRTaskStn::MoveRelativeT(LONG lMtrCnt, BOOL bWait)
{
	return GMP_SUCCESS;
}

PRU &CPRTaskStn::SelectCurPRU(ULONG ulPRU)
{
	return pruDummy;
}

PRU &CPRTaskStn::SelectCurPBBackground(ULONG ulPRU)
{
	return pruDummy;
}

/////////////////////////////////////////////////////////////////
// END Virtual Move
/////////////////////////////////////////////////////////////////

VOID CPRTaskStn::SetPRSelected(BOOL bMode)
{
	if (!g_bIsVisionNTInited)
	{
		m_bPRSelected = FALSE;
		m_pAppMod->HMIMessageBox(MSG_OK, "MODULE NOT SELECTED", "PRS Not Initialized!");
	}
	else
	{
		m_bPRSelected = bMode;
	}
}

BOOL CPRTaskStn::IsPRSelected()
{
	return m_bPRSelected;
}

VOID CPRTaskStn::CheckPRModSelected(BOOL bMode)
{
	if (g_bIsVisionNTInited)
	{
		if (bMode)
		{
			if (!m_bPRSelected)
			{
				m_pAppMod->SetAlert(m_lPRStnErrorCode);
			}
		}
	}
}

LONG CPRTaskStn::GetPRModSelectMode()
{
	return m_lPRModSelectMode;
}

LONG CPRTaskStn::SleepWithCalibDelay()
{
	Sleep(m_lCalibPRDelay);

	return GMP_SUCCESS;
}
LONG CPRTaskStn::GetPRDelay()
{
	return m_lPRDelay;
}

LONG CPRTaskStn::GetPRCalibDelay()
{
	return m_lCalibPRDelay;
}

/////////////////////////////////////////
// Learn Function
////////////////////////////////////////
BOOL CPRTaskStn::ManualLrnDie_New(PRU *pPRU)	// Vision UI Learn
{
	// learn die
	pPRU->bLoaded = FALSE;

	if (PRS_ManualLrnCmd(pPRU))
	{


		CString		szMsg = _T(" ");
		szMsg.Format("PRS_ManualLrnCmd %s, RetRecordID:%ld", pPRU->szFileName, (LONG)pPRU->stSrchCmd.uwRecordID);
		m_pAppMod->DisplayMessage(szMsg);

		if (PRS_CreateCriteriaCmd(pPRU))
		{

			pPRU->bLoaded = TRUE;

			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Learnt");

			CString szMsg = _T("");
			szMsg.Format("Pattern Learn Succeed [ID: %d]", pPRU->stSrchDieCmd.auwRecordID[0]);
			m_pAppMod->WriteHMIMess(szMsg);

#if 0 //20180208
			PRS_InitGetRecrodInfoCmd(pPRU);
			PR_GET_RECORD_INFO_RPY	stRpy;
			if (PRS_GetRecordInfoWithStat(pPRU, &stRpy))
			{
				;
			}
#endif
			return TRUE;
		}
		else
		{
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Learn Failed!");
			m_pAppMod->WriteHMIMess("Pattern Learn Failed!");

			return FALSE;
		}
		
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Learn Failed!");
		m_pAppMod->WriteHMIMess("Pattern Learn Failed!");

		return FALSE;
	}
}

VOID CPRTaskStn::ModifyPRCriteria(PRU *pPRU)
{
	if (!pPRU->bLoaded)
	{
		m_pAppMod->WriteHMIMess("Pattern Search Error: Please load pattern first.");
		return;
	}

	if (PRS_ModifyCriteriaCmd(pPRU))
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Criteria Modified");

	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Modify Criteria Failed!");
		m_pAppMod->WriteHMIMess("Modify Criteria Failed!");
	}
}

VOID CPRTaskStn::SrchPR_New(PRU *pPRU)
{
	if (!pPRU->bLoaded)
	{
		m_pAppMod->WriteHMIMess("Pattern Search Error: Please load pattern first.");
		return;
	}

	if (PRS_ManualSrchCmd(pPRU))
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Found");

		CString szMsg = _T("");
		m_pAppMod->WriteHMIMess("Pattern Search Succeed!");
		szMsg.Format("XY: (%.2f,%.2f), Ang: %.2f, Score: %.2f, Scale: x:%.2f, y: %.2f", 
					 pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, pPRU->fAng, pPRU->fScore, pPRU->rszScale.x, pPRU->rszScale.y);	
		m_pAppMod->WriteHMIMess(szMsg);

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Search Failed!");
		m_pAppMod->WriteHMIMess("Pattern Search Failed!");
	}
}

BOOL CPRTaskStn::ManualLrnDie(PRU *pPRU)
{
	// learn die
	pPRU->bLoaded = FALSE;

	if (PRS_ManualLrnDie(pPRU))
	{
		pPRU->bLoaded = TRUE;

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Learnt");

		CString szMsg = _T("");
		szMsg.Format("Pattern Learn Succeed [ID: %d]", pPRU->stSrchDieCmd.auwRecordID[0]);
		m_pAppMod->WriteHMIMess(szMsg);

		return TRUE;
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Learn Failed!");
		m_pAppMod->WriteHMIMess("Pattern Learn Failed!");

		return FALSE;
	}
}

VOID CPRTaskStn::SrchPRDie(PRU *pPRU)
{
	if (!pPRU->bLoaded)
	{
		m_pAppMod->WriteHMIMess("Pattern Search Error: Please load pattern first.");
		return;
	}

	if (PRS_SrchDie(pPRU))
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Found");

		CString szMsg = _T("");
		m_pAppMod->WriteHMIMess("Pattern Search Succeed!");
		szMsg.Format("XY: (%.2f,%.2f), Ang: %.2f, Score: %.2f, Scale: %.2f", 
					 pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, pPRU->fAng, pPRU->fScore, pPRU->fScale);	// 20151201 add scale
		m_pAppMod->WriteHMIMess(szMsg);

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Search Failed!");
		m_pAppMod->WriteHMIMess("Pattern Search Failed!");
	}
}

VOID CPRTaskStn::SetPRDieSrchArea(PRU *pPRU, PR_COORD coCorner1, PR_COORD coCorner2)
{
	CString szOldValue;
	szOldValue.Format("ID = %ld, Upper Left Corner = (%d, %d), Lower Right Corner = (%d, %d)",
					  pPRU->emID,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.x,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.y,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.x,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.y);

	pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1 = coCorner1;
	pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2 = coCorner2;
	pPRU->stSrchDieCmd.stDieAlign.stStreet.coProbableDieCentre.x = (coCorner1.x + coCorner2.x) / 2;	// 20150901
	pPRU->stSrchDieCmd.stDieAlign.stStreet.coProbableDieCentre.y = (coCorner1.y + coCorner2.y) / 2;	// 20150901

	CString szNewValue;
	szNewValue.Format("ID = %ld, Upper Left Corner = (%d, %d), Lower Right Corner = (%d, %d)",
					  pPRU->emID,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.x,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.y,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.x,
					  pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.y);

	LogPRParameter("PRTaskStn", __FUNCTION__, "Search Area", szOldValue, szNewValue);

	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Pattern Search Area Updated");
	m_pAppMod->WriteHMIMess("Pattern Search Area Updated");
}

// PR Menu routines: Shape
BOOL CPRTaskStn::LearnPRShape(PRU *pPRU)
{
	// learn shape
	pPRU->bLoaded = FALSE;

	if (PRS_LrnShape(pPRU))
	{
		pPRU->bLoaded = TRUE;

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Shape Learnt");

		CString szMsg = _T("");
		szMsg.Format("Shape Learn Succeed [ID: %d]", pPRU->stDetectShapeCmd.uwRecordID);
		m_pAppMod->WriteHMIMess(szMsg);
		
		return TRUE;
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Shape Learn Failed!");
		m_pAppMod->WriteHMIMess("Shape Learn Failed!");

		return FALSE;
	}
}

VOID CPRTaskStn::DetectPRShape(PRU *pPRU)
{
	if (!pPRU->bLoaded)
	{
		m_pAppMod->WriteHMIMess("Shape Search Error: Please load pattern first.");
		return;
	}

	if (PRS_DetectShape(pPRU))
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Shape Found");

		CString szMsg = _T("");
		m_pAppMod->WriteHMIMess("Shape Search Succeed!");
		szMsg.Format("XY: (%.2f,%.2f), Score: %.2f", 
					 pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, pPRU->fScore);
		m_pAppMod->WriteHMIMess(szMsg);

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Shape Search Failed!");
		m_pAppMod->WriteHMIMess("Shape Search Failed!");
	}
}

VOID CPRTaskStn::SetPRShapeSrchArea(PRU *pPRU, PR_COORD coCorner1, PR_COORD coCorner2)
{
	CString szOldValue;
	szOldValue.Format("ID = %ld, Upper Left Corner = (%d, %d), Lower Right Corner = (%d, %d)",
					  pPRU->emID,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner1.x,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner1.y,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner2.x,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner2.y);

	pPRU->stDetectShapeCmd.stInspWin.coCorner1	= coCorner1;
	pPRU->stDetectShapeCmd.stInspWin.coCorner2	= coCorner2;

	CString szNewValue;
	szNewValue.Format("ID = %ld, Upper Left Corner = (%d, %d), Lower Right Corner = (%d, %d)",
					  pPRU->emID,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner1.x,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner1.y,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner2.x,
					  pPRU->stDetectShapeCmd.stInspWin.coCorner2.y);

	LogPRParameter("PRTaskStn", __FUNCTION__, "Search Area", szOldValue, szNewValue);

	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Shape Search Area Updated");
	m_pAppMod->WriteHMIMess("Shape Search Area Updated");
}
	

// PR Menu routines: Tmpl
BOOL CPRTaskStn::LoadPRTmpl(PRU *pPRU)
{

	// load template
	pPRU->bLoaded = FALSE;

	if (PRS_LoadTmpl(pPRU))
	{
		pPRU->bLoaded = TRUE;

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Tmpl Loaded");

		CString szMsg = _T("");
		szMsg.Format("Tmpl Load Succeed [ID: %d]", pPRU->stSrchTmplCmd.uwRecordID);
		m_pAppMod->WriteHMIMess(szMsg);
		
		return TRUE;
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Tmpl Load Failed!");
		m_pAppMod->WriteHMIMess("Tmpl Load Failed!");

		return FALSE;
	}
}

VOID CPRTaskStn::SrchPRTmpl(PRU *pPRU)
{
	if (!pPRU->bLoaded)
	{
		m_pAppMod->WriteHMIMess("Tmpl Search Error: Please load Tmpl first.");
		return;
	}

	if (PRS_SrchTmpl(pPRU))
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Tmpl Found");

		CString szMsg = _T("");
		m_pAppMod->WriteHMIMess("Tmp Search Succeed!");
		szMsg.Format("XY: (%.2f,%.2f), Ang: %.2f", 
					 pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, pPRU->fAng);
		m_pAppMod->WriteHMIMess(szMsg);

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Search Tmpl Failed!");
		m_pAppMod->WriteHMIMess("Tmpl Search Failed!");
	}
}

VOID CPRTaskStn::SetPRTmplSrchArea(PRU *pPRU, PR_COORD coCorner1, PR_COORD coCorner2)
{
	CString szOldValue;
	szOldValue.Format("ID = %ld, Upper Left Corner = (%d, %d), Lower Right Corner = (%d, %d)",
					  pPRU->emID,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner1.x,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner1.y,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner2.x,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner2.y);

	pPRU->stSrchTmplCmd.stSrchWin.coCorner1 = coCorner1;
	pPRU->stSrchTmplCmd.stSrchWin.coCorner2 = coCorner2;

	CString szNewValue;
	szNewValue.Format("ID = %ld, Upper Left Corner = (%d, %d), Lower Right Corner = (%d, %d)",
					  pPRU->emID,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner1.x,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner1.y,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner2.x,
					  pPRU->stSrchTmplCmd.stSrchWin.coCorner2.y);

	LogPRParameter("PRTaskStn", __FUNCTION__, "Search Area", szOldValue, szNewValue);

	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Tmpl Search Area Updated");
	m_pAppMod->WriteHMIMess("Tmpl Search Area Updated");
}

BOOL CPRTaskStn::LrnBackgroundCmd(PRU *pPRU, PRU *pPRUBackground)
{
	PR_COORD coCentre;
	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	// learn die
	pPRUBackground->bLoaded	= FALSE;

	if (PRS_GrabShareImgCmd(pPRUBackground))
	{
		pPRUBackground->bLoaded = TRUE;


		PRS_DisplayText(pPRUBackground->nCamera, 1, PRS_MSG_ROW1, "Pattern Learnt");

		return TRUE;
	}
	else
	{
		PRS_DisplayText(pPRUBackground->nCamera, 1, PRS_MSG_ROW1, "Pattern Learn Failed!");
		m_pAppMod->WriteHMIMess("Pattern Learn Failed!");

		return FALSE;
	}
}

/////////////////////////////////////////
// Search Function
////////////////////////////////////////
BOOL CPRTaskStn::SearchPR(PRU *pPRU, BOOL bIsWait)
{
	BOOL bResult = FALSE;

	if (bIsWait)
	{
		Sleep(m_lPRDelay);
	}

	if (!m_bPRSelected)
	{
		return TRUE;
	}

	if (!pPRU->bLoaded)
	{
		pPRU->rcoDieCentre.x	= PR_DEF_CENTRE_X;
		pPRU->rcoDieCentre.y	= PR_DEF_CENTRE_Y;
		pPRU->fAng			= 0.0;
		pPRU->fScore		= 0.0;
		pPRU->fScale		= 0.0;	// 20151201
		pPRU->rszScale.x	= 0.0;
		pPRU->rszScale.y	= 0.0;
		
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "PR Pattern Not Loaded");

		return FALSE;
	}

	m_pAppMod->TC_AddJob(m_lAddJobPR_GRAB);
	m_pAppMod->TC_AddJob(m_lAddJobPR_PROCESS);

#if 1	// Vision UI
	bResult = PRS_ManualSrchCmd(pPRU);
#else
	switch (pPRU->emRecordType)
	{
	// Pattern Matching or Edge Matching
	case PRS_DIE_TYPE:
		bResult = PRS_SrchDie(pPRU);
		break;

	// Shape Fitting
	case PRS_SHAPE_TYPE:
		bResult = PRS_DetectShape(pPRU);
		break;

	// Tmpl Fitting
	case PRS_TMPL_TYPE:
		bResult = PRS_SrchTmpl(pPRU);
		break;
	}
#endif
	
	m_pAppMod->TC_DelJob(m_lAddJobPR_GRAB);
	m_pAppMod->TC_DelJob(m_lAddJobPR_PROCESS);

	return bResult;
}

BOOL CPRTaskStn::SearchPRGrabOnly(PRU *pPRU)
{
	BOOL bResult = FALSE;

	if (!pPRU->bLoaded)
	{
		pPRU->rcoDieCentre.x	= PR_DEF_CENTRE_X;
		pPRU->rcoDieCentre.y	= PR_DEF_CENTRE_Y;
		pPRU->fAng			= 0.0;
		pPRU->fScore		= 0.0;
		pPRU->fScale		= 0.0;	// 20151201
		pPRU->rszScale.x	= 0.0;
		pPRU->rszScale.y	= 0.0;

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "PR Pattern Not Loaded");

		return FALSE;
	}

	m_pAppMod->TC_AddJob(m_lAddJobPR_GRAB);

#if 1
	bResult = PRS_ManualSrchGrabOnly(pPRU);
#else
	switch (pPRU->emRecordType)
	{
	// Pattern Matching or Edge Matching
	case PRS_DIE_TYPE:
		bResult = PRS_SrchDieGrabOnly(pPRU);
		break;

	// Shape Fitting
	case PRS_SHAPE_TYPE:
		bResult = PRS_DetectShapeGrabOnly(pPRU);
		break;

	// Tmpl Fitting
	case PRS_TMPL_TYPE:
		bResult = PRS_SrchTmplGrabOnly(pPRU);
		break;
	
	}
#endif

	m_pAppMod->TC_DelJob(m_lAddJobPR_GRAB);

	return bResult;
}

BOOL CPRTaskStn::SearchPRProcessOnly(PRU *pPRU)
{
	BOOL bResult = FALSE;

	if (!pPRU->bLoaded)
	{
		pPRU->rcoDieCentre.x	= PR_DEF_CENTRE_X;
		pPRU->rcoDieCentre.y	= PR_DEF_CENTRE_Y;
		pPRU->fAng			= 0.0;
		pPRU->fScore		= 0.0;
		pPRU->fScale		= 0.0;	// 20151201
		pPRU->rszScale.x	= 0.0;
		pPRU->rszScale.y	= 0.0;

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "PR Pattern Not Loaded");

		return FALSE;
	}

	m_pAppMod->TC_AddJob(m_lAddJobPR_PROCESS);

#if 1
	bResult = PRS_ManualSrchProcessOnly(pPRU);
#else
	switch (pPRU->emRecordType)
	{
	// Pattern Matching or Edge Matching
	case PRS_DIE_TYPE:
		bResult = PRS_SrchDieProcessOnly(pPRU);
		break;

	// Shape Fitting
	case PRS_SHAPE_TYPE:
		bResult = PRS_DetectShapeProcessOnly(pPRU);
		break;

	// Tmpl Fitting
	case PRS_TMPL_TYPE:
		bResult = PRS_SrchTmplProcessOnly(pPRU);
		break;

	}
#endif
	m_pAppMod->TC_DelJob(m_lAddJobPR_PROCESS);

	return bResult;
}

BOOL CPRTaskStn::SearchPRCentre(PRU *pPRU, BOOL bIsInvert) //20120613
{
	MTR_POSN mtr_offset;
	BOOL bResult = FALSE;

	if (!m_bPRSelected)
	{
		return TRUE;
	}

	Sleep(m_lPRDelay);
	
	if (!pPRU->bLoaded)
	{
		pPRU->rcoDieCentre.x	= PR_DEF_CENTRE_X;
		pPRU->rcoDieCentre.y	= PR_DEF_CENTRE_Y;
		pPRU->fAng			= 0.0;
		pPRU->fScore		= 0.0;
		pPRU->fScale		= 0.0;	// 20151201
		pPRU->rszScale.x	= 0.0;
		pPRU->rszScale.y	= 0.0;

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "PR Pattern Not Loaded");

		return FALSE;
	}

	m_pAppMod->TC_AddJob(m_lAddJobPR_GRAB);
	m_pAppMod->TC_AddJob(m_lAddJobPR_PROCESS);

	bResult = PRS_ManualSrchCmd(pPRU);
	//switch (pPRU->emRecordType)
	//{
	//// Pattern Matching or Edge Matching
	//case PRS_DIE_TYPE:
	//	bResult = PRS_SrchDie(pPRU);
	//	break;

	//// Shape Fitting
	//case PRS_SHAPE_TYPE:
	//	bResult = PRS_DetectShape(pPRU);
	//	break;

	//// Tmpl Fitting
	//case PRS_TMPL_TYPE:
	//	bResult = PRS_SrchTmpl(pPRU);
	//	break;

	//}

	m_pAppMod->TC_DelJob(m_lAddJobPR_GRAB);
	m_pAppMod->TC_DelJob(m_lAddJobPR_PROCESS);

	// Die found. Align table to centre
	if (bResult)
	{
		PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);

		if (bIsInvert) //20120613
		{
			//mtr_offset.x = -1* mtr_offset.x;
			mtr_offset.y = -1 * mtr_offset.y;
		}

		if (MoveRelativeXY(mtr_offset.x, mtr_offset.y, GMP_WAIT) != GMP_SUCCESS)
		{
			bResult = FALSE;
		}
	}

	return bResult;
}

VOID CPRTaskStn::SinglePRCycleOperation(PRU *pPRU, DOUBLE dTestAng, DOUBLE dTestX, DOUBLE	dTestY, PRINT_DATA	*stPrintData)
{	
	MTR_POSN	mtrCurPosn;
	INT i = 0;

	//DOUBLE		dData[20];

	if (!m_bPRCycle)
	{
		return;
	}

	do
	{	
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);
		
		if (
			MoveXYToPRCalibPosn() != GMP_SUCCESS ||
			MoveZToPRCalibLevel() != GMP_SUCCESS
		   )
		{
			m_bPRCycle = FALSE;
			break;
		}

		Sleep(m_lPRDelay);

		stPrintData->dData[0] = GetMotorX().GetEncPosn();
		stPrintData->dData[1] = GetMotorY().GetEncPosn();
		stPrintData->dData[2] = GetMotorT().GetEncPosn();

		if (!SearchPRCentre(pPRU))
		{
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
			m_bPRCycle = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		Sleep(m_lPRDelay);

		mtrCurPosn.x = GetMotorX().GetEncPosn();
		mtrCurPosn.y = GetMotorY().GetEncPosn();

		stPrintData->dData[3] = GetMotorX().GetEncPosn();
		stPrintData->dData[4] = GetMotorY().GetEncPosn();
		stPrintData->dData[5] = GetMotorT().GetEncPosn();
		stPrintData->dData[6] = pPRU->rcoDieCentre.x;
		stPrintData->dData[7] = pPRU->rcoDieCentre.y;

		LONG lTestAngStep	= AngleToCount(dTestAng, GetMotorT().stAttrib.dDistPerCount);
		LONG lTestXStep		= DistanceToCount(dTestX * 1000 , GetMotorX().stAttrib.dDistPerCount);
		LONG lTestYStep		= DistanceToCount(dTestY * 1000, GetMotorY().stAttrib.dDistPerCount);
	
		// Forward
		if (
			(GetMotorT().MoveRelative(lTestAngStep, GMP_WAIT) != GMP_SUCCESS) ||
			(GetMotorX().MoveRelative(lTestXStep, GMP_WAIT) != GMP_SUCCESS) ||
			(GetMotorY().MoveRelative(lTestYStep, GMP_WAIT) != GMP_SUCCESS)
		   )
		{
			m_bPRCycle = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		stPrintData->dData[8] = GetMotorX().GetEncPosn();
		stPrintData->dData[9] = GetMotorY().GetEncPosn();
		stPrintData->dData[10] = GetMotorT().GetEncPosn();

		// Backward
		if (
			(GetMotorT().MoveRelative(-lTestAngStep, GMP_WAIT) != GMP_SUCCESS) ||
			(GetMotorX().MoveRelative(-lTestXStep, GMP_WAIT) != GMP_SUCCESS) ||
			(GetMotorY().MoveRelative(-lTestYStep, GMP_WAIT) != GMP_SUCCESS)
		   )
		{
			m_bPRCycle = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		stPrintData->dData[11] = GetMotorX().GetEncPosn();
		stPrintData->dData[12] = GetMotorY().GetEncPosn();
		stPrintData->dData[13] = GetMotorT().GetEncPosn();

		if (!SearchPR(pPRU))
		{
			m_bPRCycle = FALSE;
			break;
		}

		stPrintData->dData[14] = pPRU->rcoDieCentre.x;
		stPrintData->dData[15] = pPRU->rcoDieCentre.y;

		CString szMsg = "";
		m_ulPRCycle++;
		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
		szMsg.Format("Cycle: %ld", m_ulPRCycle);
		PRS_DisplayText(pPRU->nCamera, 2, 0, szMsg);

	} while (0);
		
}

/////////////////////////////////////////////////////////////////
// COR Calc
/////////////////////////////////////////////////////////////////
VOID CPRTaskStn::CalDPosnAfterRotate(D_MTR_POSN dmtrTableCOR, MTR_POSN mtrCurPosn, DOUBLE dAngle, D_MTR_POSN *p_dmtrResultPosn)
{
	DOUBLE dAngleRad;

	 //COR not calibrated
	if (!m_bCORCalibrated[m_lCORCamNum])
	{
		p_dmtrResultPosn->x = mtrCurPosn.x;
		p_dmtrResultPosn->y = mtrCurPosn.y;
		return;
	}

	dAngleRad = (PI / 180.0) * dAngle;

	p_dmtrResultPosn->x = (+((DOUBLE)mtrCurPosn.x - dmtrTableCOR.x) * cos(dAngleRad)
						   - ((DOUBLE)mtrCurPosn.y - dmtrTableCOR.y) * sin(dAngleRad)
						   + dmtrTableCOR.x);

	p_dmtrResultPosn->y = (+((DOUBLE)mtrCurPosn.x - dmtrTableCOR.x) * sin(dAngleRad)
						   + ((DOUBLE)mtrCurPosn.y - dmtrTableCOR.y) * cos(dAngleRad)
						   + dmtrTableCOR.y);

	return;
}

VOID CPRTaskStn::CalPosnAfterRotate(MTR_POSN mtrCurPosn, DOUBLE dAngle, MTR_POSN *p_mtrResultPosn)
{
	DOUBLE dAngleRad;
	D_MTR_POSN dmtrTableCOR;

	if (!m_bCORCalibrated[m_lCORCamNum] || dAngle == 0.0)
	{
		p_mtrResultPosn->x = mtrCurPosn.x;
		p_mtrResultPosn->y = mtrCurPosn.y;
		return;
	}

	dAngleRad = (PI / 180.0) * dAngle;

	INT i = 0;
	INT nIndex = 0;

	if (fabs(dAngle) > m_dCORLUT[m_lCORCamNum][COR_ARRAY_SIZE - 1])
	{
		nIndex = COR_ARRAY_SIZE - 1;
	}
	else
	{
		for (i = 0; i < COR_ARRAY_SIZE; i++)
		{
			if (fabs(dAngle) <= m_dCORLUT[m_lCORCamNum][i])
			{
				nIndex = i;
				break;
			}
		}
	}

	// negative angle
	if (dAngle < 0.0)
	{
		dmtrTableCOR.x = m_dmtrTableCOR_Neg[m_lCORCamNum][nIndex].x;
		dmtrTableCOR.y = m_dmtrTableCOR_Neg[m_lCORCamNum][nIndex].y;
	}
	// positive ange
	else
	{
		dmtrTableCOR.x = m_dmtrTableCOR_Pos[m_lCORCamNum][nIndex].x;
		dmtrTableCOR.y = m_dmtrTableCOR_Pos[m_lCORCamNum][nIndex].y;
	}

	p_mtrResultPosn->x = DoubleToLong(+((DOUBLE)mtrCurPosn.x - dmtrTableCOR.x) * cos(dAngleRad)
									  - ((DOUBLE)mtrCurPosn.y - dmtrTableCOR.y) * sin(dAngleRad)
									  + dmtrTableCOR.x);

	p_mtrResultPosn->y = DoubleToLong(+((DOUBLE)mtrCurPosn.x - dmtrTableCOR.x) * sin(dAngleRad)
									  + ((DOUBLE)mtrCurPosn.y - dmtrTableCOR.y) * cos(dAngleRad)
									  + dmtrTableCOR.y);

	return;
}


#if 1 //20140627 PBDL cal
BOOL CPRTaskStn::SetThermalCalibRefPosn(PRU *pPRU)
{
	CString szTitle		= "SET THERMAL REF POSN";
	CString szText		= "";
	CString szMsg		= "";
	BOOL bResult		= TRUE;

	if (!m_bPRSelected)
	{
		szText.Format("Module PR Not Selected. Operation Abort!");
		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, szText);
		return FALSE;
	}
	if (!pPRU->bCalibrated)
	{
		szText.Format("XY Calibration NOT Done. Operation Abort!");
		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, szText);
		szText.Format("Please Finish XY Calibration First.");
		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, szText);
		return FALSE;
	}

	m_pAppMod->WriteHMIMess("Set PBDL Calibration");

	PRS_ClearScreen(pPRU->nCamera);

	// operation frame (with 0.1m calibration window)
	do
	{
		if (
			MoveZToPRCalibLevel() != GMP_SUCCESS ||
			MoveXYToPRCalibPosn() != GMP_SUCCESS
		   )
		{
			m_pAppMod->WriteHMIMess("Calibration Abort!");
			return FALSE;
		}

		Sleep(m_lCalibPRDelay);

		if (!SearchPRCentre(pPRU))
		{
			m_pAppMod->DisplayMessage("SetThermalCalibRefPosn: PR Search error.");
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		m_mtrThermalCalibRefPosn.x = GetMotorX().GetEncPosn();
		m_mtrThermalCalibRefPosn.y = GetMotorY().GetEncPosn();

		szMsg.Format("SetThermalCalibRefPosn: m_lCalibPRDelay=%ld", m_lCalibPRDelay);
		m_pAppMod->DisplayMessage(szMsg);

		szMsg.Format("m_mtrThermalCalibRefPosn=(%ld,%ld)", m_mtrThermalCalibRefPosn.x, m_mtrThermalCalibRefPosn.y);
		m_pAppMod->DisplayMessage(szMsg);

		//if (
		//	(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
		//)
		//{
		//	bResult = FALSE;
		//	return FALSE;
		//}
		//Sleep(m_lCalibPRDelay);

		if (!SearchPR(pPRU))
		{
			m_pAppMod->DisplayMessage("SetThermalCalibRefPosn: PR Search error2.");
			bResult = FALSE;
			break;
		}
		
		szMsg.Format("SetThermalCalibRefPosn: Error Offset=(%ld,%ld)", 
					 pPRU->rcoDieCentre.x - PR_DEF_CENTRE_X, 
					 pPRU->rcoDieCentre.y - PR_DEF_CENTRE_Y);
		m_pAppMod->DisplayMessage(szMsg);
		
	} while (0);

	return bResult;
}
#endif

/////////////////////////////////////////////////////////////////
// X-Y Calibration
/////////////////////////////////////////////////////////////////
BOOL CPRTaskStn::AutoCalibration(PRU *pPRU)
{

	CString szTitle		= "AUTO CALIBRATION";
	CString szText		= "";
	CString szMsg		= "";
	BOOL bResult		= TRUE;
	LONG lStepTimes		= 1;	// 20150804 120mm FOV

	MTR_POSN mtr_oldPosn;
	MTR_POSN mtr_oldPosn2ndEnc;

	if (!m_bPRSelected)
	{
		szText.Format("Module PR Not Selected. Operation Abort!");
		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, szText);
		return FALSE;
	}

	m_pAppMod->WriteHMIMess("Starting Auto Calibration...");

	PRS_ClearScreen(pPRU->nCamera);

	if (pPRU->emCameraNo != PR_CAMERA_1)
	{
		lStepTimes = 1;
	}
	else
	{
		lStepTimes = 100;
	}

	// operation frame (with 0.1m calibration window)
	do
	{
		if (pPRU->bCalibrated && !SearchPRCentre(pPRU))
		{
			bResult = FALSE;
			break;
		}

		mtr_oldPosn.x = GetMotorX().GetEncPosn();
		mtr_oldPosn.y = GetMotorY().GetEncPosn();
		if (pPRU->emCameraNo != PR_CAMERA_1)
		{
			mtr_oldPosn2ndEnc.y = GetMotorY().Get2ndEncPosn();
		}
		// left
		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS || 
			(MoveAbsoluteXY(mtr_oldPosn.x - 100 * lStepTimes, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			return FALSE;
		}
		Sleep(m_lCalibPRDelay);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_LEFT].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_LEFT].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_LEFT].x		= mtr_oldPosn.x - 100 * lStepTimes;
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			m_stMtrCalibPts[CAL_LEFT].y		= mtr_oldPosn.y;
		}
		else
		{
			m_stMtrCalibPts[CAL_LEFT].y		= mtr_oldPosn2ndEnc.y;
		}
		// right
		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS || 
			(MoveAbsoluteXY(mtr_oldPosn.x + 100 * lStepTimes, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		PRS_ClearScreen(pPRU->nCamera);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_RIGHT].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_RIGHT].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_RIGHT].x	= mtr_oldPosn.x + 100 * lStepTimes;
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			m_stMtrCalibPts[CAL_RIGHT].y		= mtr_oldPosn.y;
		}
		else
		{
			m_stMtrCalibPts[CAL_RIGHT].y		= mtr_oldPosn2ndEnc.y;
		}

		// up
		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS ||  
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y + 10000, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		PRS_ClearScreen(pPRU->nCamera);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_UP].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_UP].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_UP].x		= mtr_oldPosn.x;
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			m_stMtrCalibPts[CAL_UP].y		= mtr_oldPosn.y + 10000;
		}
		else
		{
			m_stMtrCalibPts[CAL_UP].y		= mtr_oldPosn2ndEnc.y + 100;
		}

		// down
		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS || 
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y - 10000, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		PRS_ClearScreen(pPRU->nCamera);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_DOWN].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_DOWN].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_DOWN].x		= mtr_oldPosn.x;
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			m_stMtrCalibPts[CAL_DOWN].y		= mtr_oldPosn.y - 10000;
		}
		else
		{
			m_stMtrCalibPts[CAL_DOWN].y		= mtr_oldPosn2ndEnc.y - 100;
		}

		// back to origin
		if (MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS)
		{
			bResult = FALSE;
			break;
		}

	} while (0);

	PRS_DisplayVideo(pPRU);

	if (!bResult || !FindAutoCalibMatrix(pPRU, m_stMtrCalibPts, m_stPRCalibPts))
	{

		pPRU->bCalibrated	= FALSE;

		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "Calibration Failed!");
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Calibration Failed!");
		m_pAppMod->WriteHMIMess("Auto Calibration Failed!");

		return FALSE;
	}

	// operation frame (with PR_DEF_CENTRE_X/2 [2048x2048] calibration window)
	PR_RCOORD pr_diesize;
	PR_RCOORD pr_coord;
	MTR_POSN mtr_offset;
	pr_diesize.x	= 0.0;
	pr_diesize.y	= 0.0;
	pr_coord.x		= 0.0;
	pr_coord.y		= 0.0;
	mtr_offset.x	= 0;
	mtr_offset.y	= 0;

	pr_diesize.x = pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].x - pPRU->arcoLrnDieCorners[PR_UPPER_LEFT].x;
	pr_diesize.y = pPRU->arcoLrnDieCorners[PR_LOWER_RIGHT].y - pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].y;
	//switch (pPRU->emRecordType)
	//{
	//// Pattern Matching or Edge Matching
	//case PRS_DIE_TYPE:
	//	pr_diesize.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x - pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
	//	pr_diesize.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y - pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
	//	break;

	//// Shape Fitting
	//case PRS_SHAPE_TYPE:
	//	pr_diesize.x = pPRU->stLrnShapeCmd.stEncRect.coCorner2.x - pPRU->stLrnShapeCmd.stEncRect.coCorner1.x;
	//	pr_diesize.y = pPRU->stLrnShapeCmd.stEncRect.coCorner2.y - pPRU->stLrnShapeCmd.stEncRect.coCorner1.y;
	//	break;

	//// Tmpl Fitting
	//case PRS_TMPL_TYPE:
	//	pr_diesize.x = pPRU->stLoadTmplCmd.szTmplSize.x * 2;
	//	pr_diesize.y = pPRU->stLoadTmplCmd.szTmplSize.y * 2;
	//	break;
	//}

	do
	{
		// back to origin
		if (!SearchPRCentre(pPRU))
		{
			bResult = FALSE;
			break;
		}

		mtr_oldPosn.x = GetMotorX().GetEncPosn();
		mtr_oldPosn.y = GetMotorY().GetEncPosn();
		if (pPRU->emCameraNo != PR_CAMERA_1)
		{
			mtr_oldPosn2ndEnc.y = GetMotorY().Get2ndEncPosn();
		}

		// left
		pr_coord.x = (PR_MAX_COORD + 1) / 8 * 7 - pr_diesize.x / 2;
		pr_coord.y = PR_DEF_CENTRE_Y;
		PRS_PRPosToMtrOffset(pPRU, pr_coord, &mtr_offset);
	
		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS || 
			(MoveAbsoluteXY(mtr_oldPosn.x + mtr_offset.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		PRS_ClearScreen(pPRU->nCamera);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_LEFT].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_LEFT].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_LEFT].x		= mtr_oldPosn.x + mtr_offset.x;
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			m_stMtrCalibPts[CAL_LEFT].y		= mtr_oldPosn.y;
		}
		else
		{
			m_stMtrCalibPts[CAL_LEFT].y		= mtr_oldPosn2ndEnc.y;
		}

		// right
		pr_coord.x = (PR_MAX_COORD + 1) / 8 * 1 + pr_diesize.x / 2;
		pr_coord.y = PR_DEF_CENTRE_Y;
		PRS_PRPosToMtrOffset(pPRU, pr_coord, &mtr_offset);
		
		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS || 
			(MoveAbsoluteXY(mtr_oldPosn.x + mtr_offset.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		PRS_ClearScreen(pPRU->nCamera);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_RIGHT].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_RIGHT].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_RIGHT].x	= mtr_oldPosn.x + mtr_offset.x;
		m_stMtrCalibPts[CAL_RIGHT].y	= mtr_oldPosn.y;

		// up
		pr_coord.x = PR_DEF_CENTRE_X;
		pr_coord.y = (PR_MAX_COORD + 1) / 8 * 7 - pr_diesize.y / 2;
		PRS_PRPosToMtrOffset(pPRU, pr_coord, &mtr_offset);

		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS || 
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y + mtr_offset.y, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		PRS_ClearScreen(pPRU->nCamera);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_UP].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_UP].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_UP].x		= mtr_oldPosn.x;
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			m_stMtrCalibPts[CAL_UP].y		= mtr_oldPosn.y + mtr_offset.y;
		}
		else
		{
			m_stMtrCalibPts[CAL_UP].y		= mtr_oldPosn2ndEnc.y + mtr_offset.y/100;
		}
		//m_stMtrCalibPts[CAL_UP].y		= mtr_oldPosn.y + mtr_offset.y;

		// down
		pr_coord.x = PR_DEF_CENTRE_X;
		pr_coord.y = (PR_MAX_COORD + 1) / 8 * 1 + pr_diesize.y / 2;
		PRS_PRPosToMtrOffset(pPRU, pr_coord, &mtr_offset);

		if (
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
			SleepWithCalibDelay() != GMP_SUCCESS || 
			(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y + mtr_offset.y, GMP_WAIT) != GMP_SUCCESS) 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		PRS_ClearScreen(pPRU->nCamera);

		if (!SearchPR(pPRU))
		{
			bResult = FALSE;
			break;
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		m_stPRCalibPts[CAL_DOWN].x		= pPRU->rcoDieCentre.x;
		m_stPRCalibPts[CAL_DOWN].y		= pPRU->rcoDieCentre.y;
		m_stMtrCalibPts[CAL_DOWN].x		= mtr_oldPosn.x;
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			m_stMtrCalibPts[CAL_DOWN].y		= mtr_oldPosn.y + mtr_offset.y;
		}
		else
		{
			m_stMtrCalibPts[CAL_DOWN].y		= mtr_oldPosn2ndEnc.y + mtr_offset.y/100;
		}
		//m_stMtrCalibPts[CAL_DOWN].y		= mtr_oldPosn.y + mtr_offset.y;

		// back to origin
		if (MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS)
		{
			bResult = FALSE;
			break;
		}

		FILE *fp = fopen("D:\\sw\\PI9000\\Data\\AUTOCALIBRATION.txt", "a+");

		if (fp != NULL) //klocwork fix 20121211
		{
			for (INT i = 0; i < NO_OF_CAL_PT; i++)
			{
				szMsg.Format("%d:    PR(%ld, %ld), MTR(%ld, %ld)\n", i, m_stPRCalibPts[i].x, m_stPRCalibPts[i].y, m_stMtrCalibPts[i].x, m_stMtrCalibPts[i].y);
				fprintf(fp, szMsg);
			}

			fclose(fp);
		}

	} while (0);

	PRS_DisplayVideo(pPRU);

	// Release Me with ASAP
	if (bResult && FindAutoCalibMatrix(pPRU, m_stMtrCalibPts, m_stPRCalibPts))
	{

		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//return FALSE; //klocwork fix 20121211
}


/////////////////////////////////////////////////////////////////
// COR Calibration
/////////////////////////////////////////////////////////////////
BOOL CPRTaskStn::CheckPRCentre(PRU *pPRU, BOOL bIsInvert) //20120613
{
	CString szMsg;
	MTR_POSN mtr_offset;

	PRS_DisplayVideo(pPRU);

	if (!m_bPRSelected)
	{
		szMsg.Format("PR Module Not Selected. Operation Abort!");
		m_pAppMod->HMIMessageBox(MSG_OK, "CHECK PR CENTRE OPERATION", szMsg);
		return 0;
	}

	if (!SearchPRCentre(pPRU, bIsInvert)) //20120613
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "PR SEARCH", "PR Failed!");
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
		return 0;
	}

	szMsg.Format("BEFORE MOVE: PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
	m_pAppMod->WriteHMIMess(szMsg);

	PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	Sleep(m_lCalibPRDelay);
	
	PRS_ClearScreen(pPRU->nCamera);

	if (!SearchPR(pPRU))
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "PR SEARCH", "PR Failed!");
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");

		return 0;
	}


	PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	szMsg.Format("AFTER MOVE: PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
	m_pAppMod->WriteHMIMess(szMsg);

	Sleep(500);

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);

	szMsg.Format("Away From Centre (PR): [%.2f, %.2f] (POSN): [%ld,%ld]", 
				 pPRU->rcoDieCentre.x - PR_DEF_CENTRE_X, pPRU->rcoDieCentre.y - PR_DEF_CENTRE_Y, 
				 mtr_offset.x, mtr_offset.y);
	m_pAppMod->WriteHMIMess(szMsg);

	return 0;
}


BOOL CPRTaskStn::CORCalibration(PRU *pPRU, DOUBLE dInitalStepAngle, BOOL bUseRotaryEnc, BOOL bInvertAngle)
{
	INT i;
	CString szTitle;
	CString szMsg;
	LONG lStepTimes		= 1;	// 20180111

	if (pPRU->emCameraNo == PR_CAMERA_1)
	{
		lStepTimes = 1;
	}
	else
	{
		lStepTimes = 100;
	}

	szTitle.Format("COR CALIBRATION");

	if (!m_bPRSelected)
	{
		szMsg.Format("PR Module Not Selected. Operation Abort!") ;
		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, szMsg);
		return FALSE;
	}
	
	m_pAppMod->WriteHMIMess("Starting COR Calibration...");
	PRS_DisplayVideo(pPRU);
	PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

	if (
		MoveZToPRCalibLevel() != GMP_SUCCESS ||
		MoveXYToPRCalibPosn() != GMP_SUCCESS ||
		MoveTToStandby() != GMP_SUCCESS 
	   )
	{
		m_pAppMod->WriteHMIMess("Calibration Abort!");
		return FALSE;
	}

	if (m_dCalibCORRange == 0.0)
	{
		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "Parameter Error! Calibration Range must not be 0");
		m_pAppMod->WriteHMIMess("Calibration Abort!");

		return FALSE;
	}

	for (i = 0; i < COR_ARRAY_SIZE; i++)
	{
		m_stCORMtrCalibPts[m_lCORCamNum][i].x = 0;
		m_stCORMtrCalibPts[m_lCORCamNum][i].y = 0;
	}

	// pull to centre first
	if (!SearchPRCentre(pPRU))
	{
		m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Calibration Failed!");
		m_pAppMod->WriteHMIMess("Calibration Abort!");

		return FALSE;
	}

	D_MTR_POSN dmtr_tempCOR = {0.0, 0.0};

	BOOL bResult = TRUE;
	INT nCounter = 0;
	LONG lStep = 0;

	// main frame 1: Find approximate table COR
	//do
	//{
	//	PRS_ClearScreen(pPRU->nCamera);

	//	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "COR Calibration: %ld/10", nCounter + 1);
	//	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Pattern Angle: %.2f", pPRU->fAng);

	//	// get initial calibration point
	//	if (nCounter == 0)
	//	{
	//		m_stCORMtrCalibPts[m_lCORCamNum][nCounter].x = GetMotorX().GetEncPosn();
	//		m_stCORMtrCalibPts[m_lCORCamNum][nCounter].y = GetMotorY().GetEncPosn();
	//		if (pPRU->emCameraNo != PR_CAMERA_1)
	//		{
	//			m_stCORMtrCalibPts2ndEnc[m_lCORCamNum][nCounter].y = GetMotorY().Get2ndEncPosn();
	//		}
	//	}
	//	else
	//	{
	//		m_stCORMtrCalibPts[m_lCORCamNum][nCounter].x = m_stCORMtrCalibPts[m_lCORCamNum][nCounter - 1].x;
	//		m_stCORMtrCalibPts[m_lCORCamNum][nCounter].y = m_stCORMtrCalibPts[m_lCORCamNum][nCounter - 1].y;
	//		if (pPRU->emCameraNo != PR_CAMERA_1)
	//		{
	//			m_stCORMtrCalibPts2ndEnc[m_lCORCamNum][nCounter].y = m_stCORMtrCalibPts2ndEnc[m_lCORCamNum][nCounter - 1].y;
	//		}
	//	}

	//	// move theta
	//	lStep = AngleToCount(dInitalStepAngle/*0.2*//*0.5*/, bUseRotaryEnc ? GetMotorT().stAttrib.dDistPerCountAux : GetMotorT().stAttrib.dDistPerCount);

	//	if (
	//		(bUseRotaryEnc && (MoveRelativeT(lStep, GMP_WAIT) != GMP_SUCCESS)) ||
	//		(!bUseRotaryEnc && (GetMotorT().MoveRelative(lStep, GMP_WAIT) != GMP_SUCCESS))
	//	   )
	//	{
	//		m_pAppMod->DisplayMessage("COR move T fail");
	//		bResult = FALSE;
	//		break;
	//	}

	//	Sleep(m_lCalibPRDelay);

	//	// align pattern centre to cam centre after rotation
	//	if (!SearchPR(pPRU))
	//	{
	//		Sleep(m_lCalibPRDelay);

	//		// retry #1
	//		if (!SearchPR(pPRU))
	//		{
	//			Sleep(m_lCalibPRDelay);

	//			// retry #2
	//			if (!SearchPR(pPRU))
	//			{
	//				m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
	//				bResult = FALSE;
	//				break;
	//			}
	//		}
	//	}

	//	PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	//	// update calibration data
	//	MTR_POSN mtrTempOffset;
	//	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtrTempOffset);

	//	m_stCORMtrCalibPts[m_lCORCamNum][nCounter].x += mtrTempOffset.x;
	//	m_stCORMtrCalibPts[m_lCORCamNum][nCounter].y += mtrTempOffset.y;
	//	if (pPRU->emCameraNo != PR_CAMERA_1)
	//	{
	//		m_stCORMtrCalibPts2ndEnc[m_lCORCamNum][nCounter].y += mtrTempOffset.y/lStepTimes;
	//	}

	//	if (MoveAbsoluteXY(m_stCORMtrCalibPts[m_lCORCamNum][nCounter].x, m_stCORMtrCalibPts[m_lCORCamNum][nCounter].y, GMP_WAIT) != GMP_SUCCESS)
	//	{
	//		bResult = FALSE;
	//		break;
	//	}

	//	// do PR and check if needed to force table to centre
	//	do
	//	{
	//		Sleep(m_lCalibPRDelay);

	//		PRS_ClearScreen(pPRU->nCamera);

	//		if (!SearchPR(pPRU))
	//		{
	//			Sleep(m_lCalibPRDelay);

	//			// retry #1
	//			if (!SearchPR(pPRU))
	//			{
	//				Sleep(m_lCalibPRDelay);

	//				// retry #2
	//				if (!SearchPR(pPRU))
	//				{
	//					m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
	//					bResult = FALSE;
	//					break;
	//				}
	//			}
	//		}

	//		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	//		if (
	//			fabs(pPRU->rcoDieCentre.x - PR_DEF_CENTRE_X) > 1 || 
	//			fabs(pPRU->rcoDieCentre.y - PR_DEF_CENTRE_Y) > 1 
	//		   )
	//		{
	//			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "fine tuning...(%.2f,%.2f)", 
	//							pPRU->rcoDieCentre.x - PR_DEF_CENTRE_X, 
	//							pPRU->rcoDieCentre.y - PR_DEF_CENTRE_Y);

	//			Sleep(m_lCalibPRDelay);

	//			// correction
	//			if (!SearchPR(pPRU))
	//			{
	//				Sleep(m_lCalibPRDelay);

	//				// retry #1
	//				if (!SearchPR(pPRU))
	//				{
	//					Sleep(m_lCalibPRDelay);

	//					// retry #2
	//					if (!SearchPR(pPRU))
	//					{
	//						m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
	//						bResult = FALSE;
	//						break;
	//					}
	//				}
	//			}

	//			PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	//			// update calibration point
	//			MTR_POSN mtrTempOffset;
	//			PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtrTempOffset);

	//			m_stCORMtrCalibPts[m_lCORCamNum][nCounter].x += mtrTempOffset.x;
	//			m_stCORMtrCalibPts[m_lCORCamNum][nCounter].y += mtrTempOffset.y;
	//			if (pPRU->emCameraNo != PR_CAMERA_1)
	//			{
	//				m_stCORMtrCalibPts2ndEnc[m_lCORCamNum][nCounter].y += mtrTempOffset.y/lStepTimes;
	//			}

	//			if (MoveAbsoluteXY(m_stCORMtrCalibPts[m_lCORCamNum][nCounter].x, m_stCORMtrCalibPts[m_lCORCamNum][nCounter].y, GMP_WAIT) != GMP_SUCCESS)
	//			{
	//				bResult = FALSE;
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			// result = ok
	//			break;
	//		}
	//	} while (1);

	//	nCounter++;

	//} while (bResult && nCounter < COR_ARRAY_SIZE);

	//if (!bResult || m_pAppMod->getKlocworkFalse()) //klocwork fix 20121211
	//{
	//	m_pAppMod->WriteHMIMess("Calibration Abort!");
	//	return FALSE;
	//}

	//if (pPRU->emCameraNo != PR_CAMERA_1)
	//{
	//	for (i = 0; i < COR_ARRAY_SIZE; i++)
	//	{
	//		m_stCORMtrCalibPts[m_lCORCamNum][i].y = m_stCORMtrCalibPts2ndEnc[m_lCORCamNum][i].y;
	//	}
	//}

	//if (!FindCORCentre(nCounter, m_stCORMtrCalibPts[m_lCORCamNum], &dmtr_tempCOR))
	//{
	//	m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "Calibration Failed!");
	//	PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);
	//	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "COR Calibration Failed!");

	//	m_pAppMod->WriteHMIMess("Unable to find COR Centre with data. Calibration Abort!");
	//	return FALSE;
	//}
	// Done finding approximate table COR

	m_pAppMod->WriteHMIMess("Starting to fine-tuning COR...");
	PRS_DisplayVideo(pPRU);
	PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

	//if (
	//	MoveXYToPRCalibPosn() != GMP_SUCCESS ||
	//	MoveTToStandby() != GMP_SUCCESS 
	//	)
	//{
	//	m_pAppMod->WriteHMIMess("Calibration Abort!");
	//	return FALSE;
	//}

	MTR_POSN mtr_Temp = {0, 0};
	D_MTR_POSN dmtr_TempResult = {0.0, 0.0};
	D_MTR_POSN dmtr_TempOffset = {0.0, 0.0};
	DOUBLE dRefineAng = 0.0;

	D_MTR_POSN	dmtrTempPos[COR_ARRAY_SIZE];
	D_MTR_POSN	dmtrTempNeg[COR_ARRAY_SIZE];
	DOUBLE		dTempLUT[COR_ARRAY_SIZE];

	bResult = TRUE;
	nCounter = 0;
	lStep = 0;

	for (i = 0; i < COR_ARRAY_SIZE; i++)
	{
		dmtrTempPos[i].x = 0.0;
		dmtrTempPos[i].y = 0.0;
		dmtrTempNeg[i].x = 0.0;
		dmtrTempNeg[i].y = 0.0;

		dTempLUT[i] = (i + 1) * m_dCalibCORRange / (2.0 * 10.0);
	}
	dmtr_tempCOR.x = GetMotorX().GetEncPosn();
	dmtr_tempCOR.y = GetMotorY().Get2ndEncPosn();
	// main frame 2: Fine-tunning table COR (neg array)
	do
	{
		PRS_ClearScreen(pPRU->nCamera);
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Fine-Tuning COR(neg): %ld/%ld", nCounter + 1, 2 * COR_ARRAY_SIZE);

		// get initial approximate COR
		if (nCounter == 0)
		{
			dmtrTempNeg[nCounter].x = dmtr_tempCOR.x;
			dmtrTempNeg[nCounter].y = dmtr_tempCOR.y;
		}
		else
		{
			dmtrTempNeg[nCounter].x = dmtrTempNeg[nCounter - 1].x;
			dmtrTempNeg[nCounter].y = dmtrTempNeg[nCounter - 1].y;
		}

		if (
			MoveXYToPRCalibPosn() != GMP_SUCCESS ||
			MoveTToStandby() != GMP_SUCCESS 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		if (!SearchPRCentre(pPRU))
		{
			m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Calibration Failed!");
			m_pAppMod->WriteHMIMess("Calibration Abort!");

			bResult = FALSE;
			break;
		}

		mtr_Temp.x = GetMotorX().GetEncPosn();
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			mtr_Temp.y = GetMotorY().GetEncPosn();
		}
		else
		{
			mtr_Temp.y = GetMotorY().Get2ndEncPosn();
		}

		// move theta
		if (nCounter == 0)
		{
			dRefineAng = (-1.0) * (dTempLUT[0] / 2.0);
		}
		else
		{
			dRefineAng = (-1.0) * (dTempLUT[nCounter - 1] + (dTempLUT[0] / 2.0));
		}

		lStep = AngleToCount(dRefineAng, bUseRotaryEnc ? GetMotorT().stAttrib.dDistPerCountAux : GetMotorT().stAttrib.dDistPerCount);

		CalDPosnAfterRotate(dmtrTempNeg[nCounter], mtr_Temp, dRefineAng, &dmtr_TempResult);

		if (
			(MoveAbsoluteXY(DoubleToLong(dmtr_TempResult.x), GetMotorY().GetEncPosn() + (DoubleToLong(dmtr_TempResult.y) - mtr_Temp.y)* lStepTimes, GMP_WAIT) != GMP_SUCCESS) || 
			(bUseRotaryEnc && (MoveRelativeT(lStep, GMP_WAIT) != GMP_SUCCESS)) ||
			(!bUseRotaryEnc && (GetMotorT().MoveRelative(lStep, GMP_WAIT) != GMP_SUCCESS))
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		// align pattern centre to cam centre after rotation
		if (!SearchPR(pPRU))
		{
			Sleep(m_lCalibPRDelay);

			// retry #1
			if (!SearchPR(pPRU))
			{
				Sleep(m_lCalibPRDelay);

				// retry #2
				if (!SearchPR(pPRU))
				{
					m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
					bResult = FALSE;
					break;
				}
			}
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		PRS_PRPosToDMtrOffset(pPRU, pPRU->rcoDieCentre, &dmtr_TempOffset);

		dmtr_TempResult.x += dmtr_TempOffset.x;
		dmtr_TempResult.y += dmtr_TempOffset.y;
#if 1 //20140702
		RefineCORCentre(mtr_Temp, bInvertAngle ? -dRefineAng : dRefineAng, dmtr_TempResult, &dmtrTempNeg[nCounter]);
#else
		RefineCORCentre(mtr_Temp, dRefineAng, dmtr_TempResult, &dmtrTempNeg[nCounter]);
#endif
		nCounter++;

	} while (bResult && nCounter < COR_ARRAY_SIZE);

	if (!bResult)
	{
		m_pAppMod->WriteHMIMess("Calibration Abort!");
		return FALSE;
	}

	nCounter = COR_ARRAY_SIZE;

	// main frame 3: Fine-tunning table COR (neg array) part 2
	do
	{
		PRS_ClearScreen(pPRU->nCamera);

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Fine-Tuning COR(neg): %ld/%ld", nCounter + 1, 2 * COR_ARRAY_SIZE);

		// get approximate COR
		if (nCounter > COR_ARRAY_SIZE)
		{
			dmtrTempNeg[2 * COR_ARRAY_SIZE - nCounter - 1].x = dmtrTempNeg[2 * COR_ARRAY_SIZE - nCounter].x;
			dmtrTempNeg[2 * COR_ARRAY_SIZE - nCounter - 1].y = dmtrTempNeg[2 * COR_ARRAY_SIZE - nCounter].y;
		}

		if (
			MoveXYToPRCalibPosn() != GMP_SUCCESS ||
			MoveTToStandby() != GMP_SUCCESS 			
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		if (!SearchPRCentre(pPRU))
		{
			m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Calibration Failed!");
			m_pAppMod->WriteHMIMess("Calibration Abort!");

			bResult = FALSE;
			break;
		}


		mtr_Temp.x = GetMotorX().GetEncPosn();
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			mtr_Temp.y = GetMotorY().GetEncPosn();
		}
		else
		{
			mtr_Temp.y = GetMotorY().Get2ndEncPosn();
		}

		// move theta
		dRefineAng = (-1.0) * (dTempLUT[2 * COR_ARRAY_SIZE - nCounter - 1] - (dTempLUT[0] / 2.0));

		lStep = AngleToCount(dRefineAng, bUseRotaryEnc ? GetMotorT().stAttrib.dDistPerCountAux : GetMotorT().stAttrib.dDistPerCount);

		CalDPosnAfterRotate(dmtrTempNeg[2 * COR_ARRAY_SIZE - nCounter - 1], mtr_Temp, dRefineAng, &dmtr_TempResult);

		if (
			(MoveAbsoluteXY(DoubleToLong(dmtr_TempResult.x), GetMotorY().GetEncPosn() + (DoubleToLong(dmtr_TempResult.y) - mtr_Temp.y)* lStepTimes, GMP_WAIT) != GMP_SUCCESS) || 
			(bUseRotaryEnc && (MoveRelativeT(lStep, GMP_WAIT) != GMP_SUCCESS)) ||
			(!bUseRotaryEnc && (GetMotorT().MoveRelative(lStep, GMP_WAIT) != GMP_SUCCESS))
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		// align pattern centre to cam centre after rotation
		if (!SearchPR(pPRU))
		{
			Sleep(m_lCalibPRDelay);

			// retry #1
			if (!SearchPR(pPRU))
			{
				Sleep(m_lCalibPRDelay);

				// retry #2
				if (!SearchPR(pPRU))
				{
					m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
					bResult = FALSE;
					break;
				}
			}
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		PRS_PRPosToDMtrOffset(pPRU, pPRU->rcoDieCentre, &dmtr_TempOffset);

		dmtr_TempResult.x += dmtr_TempOffset.x;
		dmtr_TempResult.y += dmtr_TempOffset.y;

#if 1 //20140702
		RefineCORCentre(mtr_Temp, bInvertAngle ? -dRefineAng : dRefineAng, dmtr_TempResult, &dmtrTempNeg[2 * COR_ARRAY_SIZE - nCounter - 1]);
#else
		RefineCORCentre(mtr_Temp, dRefineAng, dmtr_TempResult, &dmtrTempNeg[2 * COR_ARRAY_SIZE - nCounter - 1]);
#endif
		nCounter++;

	} while (bResult && nCounter < 2 * COR_ARRAY_SIZE);

	if (!bResult)
	{
		m_pAppMod->WriteHMIMess("Calibration Abort!");
		return FALSE;
	}

	nCounter = 0;

	// main frame 4: Fine-tunning table COR (pos array)
	do
	{
		PRS_ClearScreen(pPRU->nCamera);

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Fine-Tuning COR(pos): %ld/%ld", nCounter + 1, 2 * COR_ARRAY_SIZE);

		// get initial approximate COR
		if (nCounter == 0)
		{
			dmtrTempPos[nCounter].x = dmtrTempNeg[nCounter].x;
			dmtrTempPos[nCounter].y = dmtrTempNeg[nCounter].y;
		}
		else
		{
			dmtrTempPos[nCounter].x = dmtrTempPos[nCounter - 1].x;
			dmtrTempPos[nCounter].y = dmtrTempPos[nCounter - 1].y;
		}

		if (
			MoveXYToPRCalibPosn() != GMP_SUCCESS ||
			MoveTToStandby() != GMP_SUCCESS ||
			m_pAppMod->getKlocworkFalse() //klocwork fix 20121211
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		if (!SearchPRCentre(pPRU) || m_pAppMod->getKlocworkFalse()) //klocwork fix 20121211
		{
			m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Calibration Failed!");
			m_pAppMod->WriteHMIMess("Calibration Abort!");

			bResult = FALSE;
			break;
		}

		mtr_Temp.x = GetMotorX().GetEncPosn();
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			mtr_Temp.y = GetMotorY().GetEncPosn();
		}
		else
		{
			mtr_Temp.y = GetMotorY().Get2ndEncPosn();
		}

		// move theta
		if (nCounter == 0)
		{
			dRefineAng = (1.0) * (dTempLUT[0] / 2.0);
		}
		else
		{
			dRefineAng = (1.0) * (dTempLUT[nCounter - 1] + (dTempLUT[0] / 2.0));
		}

		lStep = AngleToCount(dRefineAng, bUseRotaryEnc ? GetMotorT().stAttrib.dDistPerCountAux : GetMotorT().stAttrib.dDistPerCount);

		CalDPosnAfterRotate(dmtrTempPos[nCounter], mtr_Temp, dRefineAng, &dmtr_TempResult);

	
		if (
			(MoveAbsoluteXY(DoubleToLong(dmtr_TempResult.x), GetMotorY().GetEncPosn() + (DoubleToLong(dmtr_TempResult.y) - mtr_Temp.y)* lStepTimes, GMP_WAIT) != GMP_SUCCESS) || 
			(bUseRotaryEnc && (MoveRelativeT(lStep, GMP_WAIT) != GMP_SUCCESS)) ||
			(!bUseRotaryEnc && (GetMotorT().MoveRelative(lStep, GMP_WAIT) != GMP_SUCCESS))
			|| m_pAppMod->getKlocworkFalse() //klocwork fix 20121211
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		// align pattern centre to cam centre after rotation
		if (!SearchPR(pPRU))
		{
			Sleep(m_lCalibPRDelay);

			// retry #1
			if (!SearchPR(pPRU))
			{
				Sleep(m_lCalibPRDelay);

				// retry #2
				if (!SearchPR(pPRU) || m_pAppMod->getKlocworkFalse()) //klocwork fix 20121211
				{
					m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
					bResult = FALSE;
					break;
				}
			}
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		PRS_PRPosToDMtrOffset(pPRU, pPRU->rcoDieCentre, &dmtr_TempOffset);

		dmtr_TempResult.x += dmtr_TempOffset.x;
		dmtr_TempResult.y += dmtr_TempOffset.y;

#if 1 //20140702
		RefineCORCentre(mtr_Temp, bInvertAngle ? -dRefineAng : dRefineAng, dmtr_TempResult, &dmtrTempPos[nCounter]);
#else
		RefineCORCentre(mtr_Temp, dRefineAng, dmtr_TempResult, &dmtrTempPos[nCounter]);
#endif
		nCounter++;

	} while (bResult && nCounter < COR_ARRAY_SIZE);

	if (!bResult || m_pAppMod->getKlocworkFalse()) //klocwork fix 20121211
	{
		m_pAppMod->WriteHMIMess("Calibration Abort!");
		return FALSE;
	}

	nCounter = COR_ARRAY_SIZE;

	// main frame 5: Fine-tunning table COR (pos array) part 2
	do
	{
		PRS_ClearScreen(pPRU->nCamera);

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Fine-Tuning COR(pos): %ld/%ld", nCounter + 1, 2 * COR_ARRAY_SIZE);

		// get approximate COR
		if (nCounter > COR_ARRAY_SIZE)
		{
			dmtrTempPos[2 * COR_ARRAY_SIZE - nCounter - 1].x = dmtrTempPos[2 * COR_ARRAY_SIZE - nCounter].x;
			dmtrTempPos[2 * COR_ARRAY_SIZE - nCounter - 1].y = dmtrTempPos[2 * COR_ARRAY_SIZE - nCounter].y;
		}

		if (
			MoveXYToPRCalibPosn() != GMP_SUCCESS ||
			MoveTToStandby() != GMP_SUCCESS 
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		if (!SearchPRCentre(pPRU))
		{
			m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Calibration Failed!");
			m_pAppMod->WriteHMIMess("Calibration Abort!");

			bResult = FALSE;
			break;
		}

		mtr_Temp.x = GetMotorX().GetEncPosn();
		if (pPRU->emCameraNo == PR_CAMERA_1)
		{
			mtr_Temp.y = GetMotorY().GetEncPosn();
		}
		else
		{
			mtr_Temp.y = GetMotorY().Get2ndEncPosn();
		}

		// move theta
		dRefineAng = (1.0) * (dTempLUT[2 * COR_ARRAY_SIZE - nCounter - 1] - (dTempLUT[0] / 2.0));

		lStep = AngleToCount(dRefineAng, bUseRotaryEnc ? GetMotorT().stAttrib.dDistPerCountAux : GetMotorT().stAttrib.dDistPerCount);

		CalDPosnAfterRotate(dmtrTempPos[2 * COR_ARRAY_SIZE - nCounter - 1], mtr_Temp, dRefineAng, &dmtr_TempResult);

		if (
			(MoveAbsoluteXY(DoubleToLong(dmtr_TempResult.x), GetMotorY().GetEncPosn() + (DoubleToLong(dmtr_TempResult.y) - mtr_Temp.y)* lStepTimes, GMP_WAIT) != GMP_SUCCESS) || 
			(bUseRotaryEnc && (MoveRelativeT(lStep, GMP_WAIT) != GMP_SUCCESS)) ||
			(!bUseRotaryEnc && (GetMotorT().MoveRelative(lStep, GMP_WAIT) != GMP_SUCCESS))
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		// align pattern centre to cam centre after rotation
		if (!SearchPR(pPRU))
		{
			Sleep(m_lCalibPRDelay);

			// retry #1
			if (!SearchPR(pPRU))
			{
				Sleep(m_lCalibPRDelay);

				// retry #2
				if (!SearchPR(pPRU))
				{
					m_pAppMod->HMIMessageBox(MSG_OK, szTitle, "PR Search Error!");
					bResult = FALSE;
					break;
				}
			}
		}

		PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

		PRS_PRPosToDMtrOffset(pPRU, pPRU->rcoDieCentre, &dmtr_TempOffset);

		dmtr_TempResult.x += dmtr_TempOffset.x;
		dmtr_TempResult.y += dmtr_TempOffset.y;

#if 1 //20140702
		RefineCORCentre(mtr_Temp, bInvertAngle ? -dRefineAng : dRefineAng, dmtr_TempResult, &dmtrTempPos[2 * COR_ARRAY_SIZE - nCounter - 1]);
#else
		RefineCORCentre(mtr_Temp, dRefineAng, dmtr_TempResult, &dmtrTempPos[2 * COR_ARRAY_SIZE - nCounter - 1]);
#endif
		nCounter++;

	} while (bResult && nCounter < 2 * COR_ARRAY_SIZE);

	if (!bResult)
	{
		m_pAppMod->WriteHMIMess("Calibration Abort!");
		return FALSE;
	}

	for (i = 0; i < COR_ARRAY_SIZE; i++)
	{
		m_dmtrTableCOR_Pos[m_lCORCamNum][i].x = dmtrTempPos[i].x;
		m_dmtrTableCOR_Pos[m_lCORCamNum][i].y = dmtrTempPos[i].y;
		m_dmtrTableCOR_Neg[m_lCORCamNum][i].x = dmtrTempNeg[i].x;
		m_dmtrTableCOR_Neg[m_lCORCamNum][i].y = dmtrTempNeg[i].y;

		m_dCORLUT[m_lCORCamNum][i] = dTempLUT[i];
	}

	// Adjustment
	D_MTR_POSN dmtrTemp = {0.0, 0.0};
	D_MTR_POSN dmtrDiff = {0.0, 0.0};
	for (i = 1; i < COR_ARRAY_SIZE - 1; i++)
	{
		dmtrTemp.x = fabs(m_dmtrTableCOR_Pos[m_lCORCamNum][i + 1].x - m_dmtrTableCOR_Pos[m_lCORCamNum][i].x);
		if (dmtrTemp.x > dmtrDiff.x)
		{
			dmtrDiff.x = dmtrTemp.x;
		}
		dmtrTemp.y = fabs(m_dmtrTableCOR_Pos[m_lCORCamNum][i + 1].y - m_dmtrTableCOR_Pos[m_lCORCamNum][i].y);
		if (dmtrTemp.y > dmtrDiff.y)
		{
			dmtrDiff.y = dmtrTemp.y;
		}
	}
	dmtrTemp.x = fabs(m_dmtrTableCOR_Pos[m_lCORCamNum][1].x - m_dmtrTableCOR_Pos[m_lCORCamNum][0].x);
	dmtrTemp.y = fabs(m_dmtrTableCOR_Pos[m_lCORCamNum][1].y - m_dmtrTableCOR_Pos[m_lCORCamNum][0].y);
	if (dmtrTemp.x > dmtrDiff.x || dmtrTemp.y > dmtrDiff.y)
	{
		m_dmtrTableCOR_Pos[m_lCORCamNum][0].x = m_dmtrTableCOR_Pos[m_lCORCamNum][1].x;
		m_dmtrTableCOR_Pos[m_lCORCamNum][0].y = m_dmtrTableCOR_Pos[m_lCORCamNum][1].y;
	}

	dmtrTemp.x = 0.0;
	dmtrTemp.y = 0.0;
	dmtrDiff.x = 0.0;
	dmtrDiff.y = 0.0;
	for (i = 1; i < COR_ARRAY_SIZE - 1; i++)
	{
		dmtrTemp.x = fabs(m_dmtrTableCOR_Neg[m_lCORCamNum][i + 1].x - m_dmtrTableCOR_Neg[m_lCORCamNum][i].x);
		if (dmtrTemp.x > dmtrDiff.x)
		{
			dmtrDiff.x = dmtrTemp.x;
		}
		dmtrTemp.y = fabs(m_dmtrTableCOR_Neg[m_lCORCamNum][i + 1].y - m_dmtrTableCOR_Neg[m_lCORCamNum][i].y);
		if (dmtrTemp.y > dmtrDiff.y)
		{
			dmtrDiff.y = dmtrTemp.y;
		}
	}
	dmtrTemp.x = fabs(m_dmtrTableCOR_Neg[m_lCORCamNum][1].x - m_dmtrTableCOR_Neg[m_lCORCamNum][0].x);
	dmtrTemp.y = fabs(m_dmtrTableCOR_Neg[m_lCORCamNum][1].y - m_dmtrTableCOR_Neg[m_lCORCamNum][0].y);
	if (dmtrTemp.x > dmtrDiff.x || dmtrTemp.y > dmtrDiff.y)
	{
		m_dmtrTableCOR_Neg[m_lCORCamNum][0].x = m_dmtrTableCOR_Neg[m_lCORCamNum][1].x;
		m_dmtrTableCOR_Neg[m_lCORCamNum][0].y = m_dmtrTableCOR_Neg[m_lCORCamNum][1].y;
	}

	PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);
	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "COR Calibration Succeed!");

	m_bCORCalibrated[m_lCORCamNum] = TRUE;

	MoveXYToPRCalibPosn();
	MoveTToStandby();

	return TRUE;
}

BOOL CPRTaskStn::FindCORResultRange(DOUBLE *pdMaxX, DOUBLE *pdMinX, DOUBLE *pdMaxY, DOUBLE *pdMinY)
{
	INT i;
	DOUBLE dNegXMax = 0.0, dNegXMin = 0.0, dNegYMax = 0.0, dNegYMin = 0.0;
	DOUBLE dXMax = 0.0, dXMin = 0.0;
	DOUBLE dYMax = 0.0, dYMin = 0.0;

	dNegXMax = m_dmtrTableCOR_Neg[m_lCORCamNum][0].x;
	dNegXMin = m_dmtrTableCOR_Neg[m_lCORCamNum][0].x;

	dNegYMax = m_dmtrTableCOR_Neg[m_lCORCamNum][0].y;
	dNegYMin = m_dmtrTableCOR_Neg[m_lCORCamNum][0].y;

	for (i = 1; i < COR_ARRAY_SIZE; i++)
	{
		if (m_dmtrTableCOR_Neg[m_lCORCamNum][i].x > dNegXMax)
		{
			dNegXMax = m_dmtrTableCOR_Neg[m_lCORCamNum][i].x;
		}
		if (m_dmtrTableCOR_Neg[m_lCORCamNum][i].x < dNegXMin)
		{
			dNegXMin = m_dmtrTableCOR_Neg[m_lCORCamNum][i].x;
		}

		if (m_dmtrTableCOR_Neg[m_lCORCamNum][i].y > dNegYMax)
		{
			dNegYMax = m_dmtrTableCOR_Neg[m_lCORCamNum][i].y;
		}
		if (m_dmtrTableCOR_Neg[m_lCORCamNum][i].y < dNegYMin)
		{
			dNegYMin = m_dmtrTableCOR_Neg[m_lCORCamNum][i].y;
		}
	}
	
	//Positive
	dXMax = dNegXMax;
	dXMin = dNegXMin;
	dYMax = dNegYMax;
	dYMin = dNegYMin;
	for (i = 0; i < COR_ARRAY_SIZE; i++)
	{
		if (m_dmtrTableCOR_Pos[m_lCORCamNum][i].x > dXMax)
		{
			dXMax = m_dmtrTableCOR_Pos[m_lCORCamNum][i].x;
		}
		if (m_dmtrTableCOR_Pos[m_lCORCamNum][i].x < dXMin)
		{
			dXMin = m_dmtrTableCOR_Pos[m_lCORCamNum][i].x;
		}

		if (m_dmtrTableCOR_Pos[m_lCORCamNum][i].y > dYMax)
		{
			dYMax = m_dmtrTableCOR_Pos[m_lCORCamNum][i].y;
		}
		if (m_dmtrTableCOR_Pos[m_lCORCamNum][i].y < dYMin)
		{
			dYMin = m_dmtrTableCOR_Pos[m_lCORCamNum][i].y;
		}
	}
	*pdMaxX = dXMax;
	*pdMinX = dXMin;
	*pdMaxY = dYMax;
	*pdMinY = dYMin;

	return TRUE;
}

BOOL CPRTaskStn::SetAllCORData(DOUBLE dXPosn, DOUBLE dYPosn)
{
	INT ii;

	for (ii = 0; ii < COR_ARRAY_SIZE; ii++)
	{
		m_dmtrTableCOR_Neg[m_lCORCamNum][ii].x = dXPosn;
		m_dmtrTableCOR_Pos[m_lCORCamNum][ii].x = dXPosn;

		m_dmtrTableCOR_Neg[m_lCORCamNum][ii].y = dYPosn;
		m_dmtrTableCOR_Pos[m_lCORCamNum][ii].y = dYPosn;
	}
	m_bCORCalibrated[m_lCORCamNum] = TRUE;

	return TRUE;
}

BOOL CPRTaskStn::CheckCORCentre(PRU *pPRU, DOUBLE dTestAng, BOOL bUseRotaryEnc, BOOL bInvertAngle)
{

	CString szMsg = "";
	MTR_POSN mtrCurPosn;
	MTR_POSN mtrResultPosn;
	MTR_POSN mtrOffset;
	LONG lStepTimes		= 1;	// 20180111

	if (pPRU->emCameraNo == PR_CAMERA_1)
	{
		lStepTimes = 1;
	}
	else
	{
		lStepTimes = 100;
	}

	CString str;

	PRS_DisplayVideo(pPRU);

	if (!m_bCORCalibrated[m_lCORCamNum])
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "CHECK COR CENTRE OPERATION", "Have not been Calibrated, Please calib first and retry");
		return FALSE;
	}

	//Move to Calib Posn 
	//Move to the Calib Posn with T is standby Posn
	if (
		MoveTToStandby() != GMP_SUCCESS ||
		MoveXYToPRCalibPosn() != GMP_SUCCESS ||
		MoveZToPRCalibLevel() != GMP_SUCCESS 
	   )
	{
		return FALSE;
	}
	
	Sleep(m_lCalibPRDelay);

	if (!SearchPRCentre(pPRU))
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "CHECK COR CENTRE OPERATION", "COR Test Failed!");
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
		m_pAppMod->WriteHMIMess("PreBond PR Failed!");
		return 0;
	}

	PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	mtrCurPosn.x = GetMotorX().GetEncPosn();
	if (pPRU->emCameraNo == PR_CAMERA_1)
	{
		mtrCurPosn.y = GetMotorY().GetEncPosn();
	}
	else
	{
		mtrCurPosn.y = GetMotorY().Get2ndEncPosn();
	}
	//mtrCurPosn.y = GetMotorY().GetEncPosn();
	
	if (!SearchPR(pPRU))
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "CHECK COR CENTRE OPERATION", "COR Test Failed!");
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
		m_pAppMod->WriteHMIMess("PreBond PR Failed!");
		return 0;
	}

	LONG lTestAngStep = AngleToCount(dTestAng, bUseRotaryEnc ? GetMotorT().stAttrib.dDistPerCountAux : GetMotorT().stAttrib.dDistPerCount);
	if (
		(bUseRotaryEnc && (MoveRelativeT(lTestAngStep, GMP_WAIT) != GMP_SUCCESS)) ||
		(!bUseRotaryEnc && (GetMotorT().MoveRelative(lTestAngStep, GMP_WAIT) != GMP_SUCCESS))
	   )
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "CHECK COR CENTRE OPERATION", "COR Test Failed!");
		return 0;
	}

#if 1 //20140702
	CalPosnAfterRotate(mtrCurPosn, bInvertAngle ? -dTestAng : dTestAng, &mtrResultPosn);
#else
	CalPosnAfterRotate(mtrCurPosn, dTestAng, &mtrResultPosn);
#endif

	mtrOffset.x = mtrResultPosn.x - mtrCurPosn.x;
	mtrOffset.y = mtrResultPosn.y - mtrCurPosn.y;



	MoveRelativeXY(mtrOffset.x, mtrOffset.y * lStepTimes, GMP_WAIT);
	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Offset (%ld,%ld)", mtrOffset.x, mtrOffset.y);

	Sleep(m_lCalibPRDelay);

	if (!SearchPR(pPRU))
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "CHECK COR CENTRE OPERATION", "COR Test Failed!");
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
		m_pAppMod->WriteHMIMess("PreBond PR Failed!");
		return 0;
	}


	PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

	szMsg.Format("PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
	m_pAppMod->WriteHMIMess(szMsg);

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtrOffset);

	szMsg.Format("Away From Centre (PR): [%.2f, %.2f] (POSN): [%ld,%ld]", 
				 pPRU->rcoDieCentre.x - PR_DEF_CENTRE_X, pPRU->rcoDieCentre.y - PR_DEF_CENTRE_Y, 
				 mtrOffset.x, mtrOffset.y);
	m_pAppMod->WriteHMIMess(szMsg);

	if (m_bPRCycle)
	{
		m_ulPRCycle++;
		szMsg.Format("Cycle: %ld", m_ulPRCycle);
		PRS_DisplayText(pPRU->nCamera, 2, 0, szMsg);
	}

	return TRUE;
}
/*BOOL CPRTaskStn::GetPRCentreMap(PRU *pPRU)
{
	BOOL bResult = TRUE;
	LONG lDiection = 0;		// 0 - X, 1 - Y

	MTR_POSN mtrPosn = {0, 0};
	INT i = 0;
	CString szMsg = "";

	LONG lSelection = m_pAppMod->HMISelectionBox("OPTION", "Please select move direction", "X", "Y");


	if (lSelection == -1 || lSelection == 11)
	{
		bResult = FALSE;
	}
	else
	{
		FILE *fp = fopen("D:\\sw\\PI9000\\Data\\GetPRCentreMap.txt", "a+");
		if (lSelection == 0)
		{
			mtrPosn.x = 10;
		}
		else
		{
			mtrPosn.y = 10;
		}

		while (1)
		{
			if (!SearchPR(pPRU))
			{
				break;
			}

			Sleep(m_lCalibPRDelay);

			if (MoveRelativeXY(mtrPosn.x, mtrPosn.y, GMP_WAIT) != GMP_SUCCESS)
			{
				break;
			}

			m_pAppMod->ProcessHMIRequest();

			szMsg.Format("Point%d\tENC\t%ld\t%ld\tPR\t%ld\t%ld\n", i, GetMotorX().GetEncPosn(), GetMotorY().GetEncPosn(), pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
			fprintf(fp, szMsg);

			i++;
		}
		fclose(fp);
	}



	return bResult;
}*/

BOOL CPRTaskStn::GetPRCentreMap(PRU *pPRU)
{
	BOOL bResult = TRUE;
	LONG lDiection = 0;		// 0 - X, 1 - Y
	LONG lNumOfCntX = 0;
	LONG lNumOfCntY = 0;

	MTR_POSN mtrPosn = {0, 0};
	INT i = 0;
	CString szMsg = "";

	MTR_POSN mtrStartPosn;
	MTR_POSN mtrEndPosn;
	mtrStartPosn.x	= GetMotorX().GetEncPosn();
	mtrStartPosn.y	= GetMotorY().GetEncPosn();
	mtrEndPosn.x	= GetMotorX().GetEncPosn();
	mtrEndPosn.y	= GetMotorY().GetEncPosn();

	//LONG lSelection = m_pAppMod->HMISelectionBox("OPTION", "Please select move direction", "X", "Y", "X&Y");

	//if (lSelection == -1 || lSelection == 11)
	//{
	//	bResult = FALSE;
	//}
	//else
	//{
	FILE *fp = fopen("D:\\sw\\PI9000\\Data\\GetPRCentreMap.txt", "a+");

		//if (lSelection == 0)
		//{
		//	mtrPosn.x = 10;
		//}
		//else if (lSelection == 1)
		//{
		//	mtrPosn.y = 10;
		//}
		//else if (lSelection == 2)
		//{
	if (
		!m_pAppMod->HMINumericKeys("Set X Limit(cnt)", 9999999.0, 0, &lNumOfCntX) ||
		!m_pAppMod->HMINumericKeys("Set Y Limit(cnt)", 9999999.0, 0, &lNumOfCntY)
	   )
	{
		bResult = FALSE;
	}
	else
	{
		mtrEndPosn.x = mtrStartPosn.x + lNumOfCntX;
		mtrEndPosn.y = mtrStartPosn.y + lNumOfCntY;
		mtrPosn.x = 10;
		mtrPosn.y = 10;
	}
		//}

	while (bResult)
	{
		if (fp != NULL) //klocwork fix 20121211
		{
			fprintf(fp, "START\n");
		}
		if (
			GetMotorX().MoveAbsolute(mtrStartPosn.x, GMP_WAIT) != GMP_SUCCESS ||
			GetMotorY().MoveAbsolute(mtrStartPosn.y, GMP_WAIT) != GMP_SUCCESS
		   )
		{
			bResult = FALSE;
			break;
		}

		Sleep(m_lCalibPRDelay);

		while (GetMotorY().GetEncPosn() <= mtrEndPosn.y)
		{
			while (GetMotorX().GetEncPosn() <= mtrEndPosn.x)
			{
				if (!SearchPR(pPRU))
				{
					bResult = FALSE;
					break;
				}
				else
				{
					Sleep(m_lCalibPRDelay);

					if (GetMotorX().MoveRelative(mtrPosn.x, GMP_WAIT) != GMP_SUCCESS)
					{
						bResult = FALSE;
						break;
					}
				}

				szMsg.Format("\tPoint%d\tENC\t%ld\t%ld\tPR\t%.2f\t%.2f\n", i, GetMotorX().GetEncPosn(), GetMotorY().GetEncPosn(), pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
				if (fp != NULL) //klocwork fix 20121211
				{
					fprintf(fp, szMsg);
				}
				i++;
					
				m_pAppMod->ProcessHMIRequest();
			}

			if (
				!bResult ||
				GetMotorX().MoveAbsolute(mtrStartPosn.x, GMP_WAIT) != GMP_SUCCESS ||
				GetMotorY().MoveRelative(mtrPosn.y, GMP_WAIT) != GMP_SUCCESS
			   )
			{
				bResult = FALSE;
				break;
			}

			Sleep(m_lCalibPRDelay);
		}
	}
	if (fp != NULL) //klocwork fix 20121211
	{
		fclose(fp);
	}
	//}

	return bResult;
}

/////////////////////////////////////////////////////////////////
//PR Related
/////////////////////////////////////////////////////////////////
BOOL CPRTaskStn::AutoSearchPR1(PRU *pPRU, CString *pszStr)
{
	MTR_POSN mtr_offset;


	if (
		(MoveXYToCurrPR1Posn(GMP_NOWAIT)) != GMP_SUCCESS || 
		(MoveZToPRLevel(GMP_NOWAIT)) != GMP_SUCCESS || 
		(SyncX()) != GMP_SUCCESS || 
		(SyncY()) != GMP_SUCCESS || 
		(SyncZ()) != GMP_SUCCESS 
	   )
	{
		return FALSE;
	}

	Sleep(m_lPRDelay); //20150318

	if (!m_bPRSelected)
	{
		return TRUE;
	}
	
	if (!SearchPR(pPRU))
	{
		return FALSE;
	}

	//Test

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);
#if 1 //20140414 debug only
	if (pszStr != NULL)
	{
		pszStr->Format("CurrPR1Posn: %.5f %.5f mtr_offset=%ld %ld", m_dmtrCurrPR1Posn.x, m_dmtrCurrPR1Posn.y, mtr_offset.x, mtr_offset.y);
	}
#endif
	m_dmtrCurrPR1Posn.x += (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR1Posn.y += (DOUBLE)mtr_offset.y;

	return TRUE;
}

BOOL CPRTaskStn::AutoSearchPR1GrabOnly(PRU *pPRU, LONG lGlass, CamObjType emCamObjType)
{
	// debug time
	//DOUBLE dProcessTime = 0.0;
	//DWORD dwStartTime = GetTickCount();

	m_nPRInProgress = ST_PR1_IN_PROGRESS;

	if (!m_bPRSelected)
	{
		return TRUE;
	}

#ifdef ENCODER_BASED
	m_mtrCurrPR1BeforeEncPosn[lGlass][(LONG)emCamObjType].x = GetMotorX().GetEncPosn();
	m_mtrCurrPR1BeforeEncPosn[lGlass][(LONG)emCamObjType].y = GetMotorY().GetEncPosn();
	m_lPR1BeforeTEncPosn[lGlass][(LONG)emCamObjType] = GetMotorT().GetEncPosn();
	m_lPR1BeforeZEncPosn[lGlass][(LONG)emCamObjType] = GetMotorZ().GetEncPosn();
#endif

	if (!SearchPRGrabOnly(pPRU))
	{
		m_nPRInProgress = ST_PR_IDLE;
		return FALSE;
	}

#ifdef ENCODER_BASED
	m_mtrCurrPR1AfterEncPosn[lGlass][(LONG)emCamObjType].x = GetMotorX().GetEncPosn();
	m_mtrCurrPR1AfterEncPosn[lGlass][(LONG)emCamObjType].y = GetMotorY().GetEncPosn();
	m_lPR1AfterTEncPosn[lGlass][(LONG)emCamObjType] = GetMotorT().GetEncPosn();
	m_lPR1AfterZEncPosn[lGlass][(LONG)emCamObjType] = GetMotorZ().GetEncPosn();
#endif

	// debug time
	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	//CString szMsg = "";
	//szMsg.Format("%s PR1 Grab Time: %.5f", pPRU->szFileName, dProcessTime);
	//m_pAppMod->DisplayMessage(szMsg);

	return TRUE;
}

#if 1 //20140414
BOOL CPRTaskStn::AutoSearchPR1ProcessOnly(PRU *pPRU, CString *pszMsgOut, LONG lGlass, CamObjType emCamObjType)
{
	// debug time
	//DOUBLE dProcessTime = 0.0;
	//DWORD dwStartTime = GetTickCount();

	MTR_POSN mtr_offset;

	if (!m_bPRSelected)
	{
		m_nPRInProgress = ST_PR_IDLE;
		return TRUE;
	}

	if (!SearchPRProcessOnly(pPRU))
	{
		m_nPRInProgress = ST_PR_IDLE;
		return FALSE;
	}

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);

	if (pszMsgOut != NULL)
	{
		pszMsgOut->Format("PR1Posn=%.3f %.3f offset=%ld %ld", m_dmtrCurrPR1Posn.x, m_dmtrCurrPR1Posn.y, mtr_offset.x, mtr_offset.y);
	}

	m_dmtrCurrPR1Posn.x += (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR1Posn.y += (DOUBLE)mtr_offset.y;

#ifdef ENCODER_BASED
	m_dmtrCurrPR1BeforePRPosn[lGlass][(LONG)emCamObjType].x = (DOUBLE)m_mtrCurrPR1BeforeEncPosn[lGlass][(LONG)emCamObjType].x + (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR1BeforePRPosn[lGlass][(LONG)emCamObjType].y = (DOUBLE)m_mtrCurrPR1BeforeEncPosn[lGlass][(LONG)emCamObjType].y + (DOUBLE)mtr_offset.y;

	m_dmtrCurrPR1AfterPRPosn[lGlass][(LONG)emCamObjType].x = (DOUBLE)m_mtrCurrPR1AfterEncPosn[lGlass][(LONG)emCamObjType].x + (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR1AfterPRPosn[lGlass][(LONG)emCamObjType].y = (DOUBLE)m_mtrCurrPR1AfterEncPosn[lGlass][(LONG)emCamObjType].y + (DOUBLE)mtr_offset.y;
#endif
	m_nPRInProgress = ST_PR_IDLE;

	// debug time
	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	//CString szMsg = "";
	//szMsg.Format("%s PR1 Process Time: %.5f", GetStnName(), dProcessTime);
	//DisplayMessage(szMsg);

	return TRUE;
}
#else
BOOL CPRTaskStn::AutoSearchPR1ProcessOnly(PRU *pPRU)
{
	// debug time
	DOUBLE dProcessTime = 0.0;
	DWORD dwStartTime = GetTickCount();

	MTR_POSN mtr_offset;

	if (!m_bPRSelected)
	{
		m_nPRInProgress = ST_PR_IDLE;
		return TRUE;
	}

	if (!SearchPRProcessOnly(pPRU))
	{
		m_nPRInProgress = ST_PR_IDLE;
		return FALSE;
	}

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);

	m_dmtrCurrPR1Posn.x += (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR1Posn.y += (DOUBLE)mtr_offset.y;

	m_nPRInProgress = ST_PR_IDLE;

	// debug time
	dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	CString szMsg = "";
	szMsg.Format("%s PR1 Process Time: %.5f", pPRU->szFileName, dProcessTime);
	m_pAppMod->DisplayMessage(szMsg);

	return TRUE;
}
#endif

BOOL CPRTaskStn::AutoSearchPR2(PRU *pPRU, CString *pszStr)
{
	MTR_POSN mtr_offset;

	if (
		(MoveXYToCurrPR2Posn(GMP_NOWAIT)) != GMP_SUCCESS || 
		(MoveZToPRLevel(GMP_NOWAIT)) != GMP_SUCCESS || 
		(SyncX()) != GMP_SUCCESS || 
		(SyncY()) != GMP_SUCCESS || 
		(SyncZ()) != GMP_SUCCESS 
	   )
	{
		return FALSE;
	}

	Sleep(m_lPRDelay); //20150318

	if (!m_bPRSelected)
	{
		return TRUE;
	}

	if (!SearchPR(pPRU))
	{
		return FALSE;
	}

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);
#if 1 //20140414 debug only
	if (pszStr != NULL)
	{
		pszStr->Format("CurrPR2Posn: %.5f %.5f mtr_offset=%ld %ld", m_dmtrCurrPR2Posn.x, m_dmtrCurrPR2Posn.y, mtr_offset.x, mtr_offset.y);
	}
#endif

	m_dmtrCurrPR2Posn.x += (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR2Posn.y += (DOUBLE)mtr_offset.y;

	return TRUE;
}

BOOL CPRTaskStn::AutoSearchPR2GrabOnly(PRU *pPRU, LONG lGlass, CamObjType emCamObjType)
{
	// debug time
	//DOUBLE dProcessTime = 0.0;
	//DWORD dwStartTime = GetTickCount();

	m_nPRInProgress = ST_PR2_IN_PROGRESS;

	if (!m_bPRSelected)
	{
		return TRUE;
	}

#ifdef ENCODER_BASED
	m_mtrCurrPR2BeforeEncPosn[lGlass][(LONG)emCamObjType].x = GetMotorX().GetEncPosn();
	m_mtrCurrPR2BeforeEncPosn[lGlass][(LONG)emCamObjType].y = GetMotorY().GetEncPosn();
	m_lPR2BeforeTEncPosn[lGlass][(LONG)emCamObjType] = GetMotorT().GetEncPosn();
	m_lPR2BeforeZEncPosn[lGlass][(LONG)emCamObjType] = GetMotorZ().GetEncPosn();
#endif

	if (!SearchPRGrabOnly(pPRU))
	{
		m_nPRInProgress = ST_PR_IDLE;
		return FALSE;
	}

#ifdef ENCODER_BASED
	m_mtrCurrPR2AfterEncPosn[lGlass][(LONG)emCamObjType].x = GetMotorX().GetEncPosn();
	m_mtrCurrPR2AfterEncPosn[lGlass][(LONG)emCamObjType].y = GetMotorY().GetEncPosn();
	m_lPR2AfterTEncPosn[lGlass][(LONG)emCamObjType] = GetMotorT().GetEncPosn();
	m_lPR2AfterZEncPosn[lGlass][(LONG)emCamObjType] = GetMotorZ().GetEncPosn();
#endif

	// debug time
	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	//CString szMsg = "";
	//szMsg.Format("%s PR2 Grab Time: %.5f", pPRU->szFileName, dProcessTime);
	//m_pAppMod->DisplayMessage(szMsg);

	return TRUE;
}

#if 1 //20140414
BOOL CPRTaskStn::AutoSearchPR2ProcessOnly(PRU *pPRU, CString *pszMsgOut, LONG lGlass, CamObjType emCamObjType)
{
	// debug time
	//DOUBLE dProcessTime = 0.0;
	//DWORD dwStartTime = GetTickCount();

	MTR_POSN mtr_offset;

	if (!m_bPRSelected)
	{
		m_nPRInProgress = ST_PR_IDLE;
		return TRUE;
	}

	if (!SearchPRProcessOnly(pPRU))
	{
		m_nPRInProgress = ST_PR_IDLE;
		return FALSE;
	}

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);

	if (pszMsgOut != NULL)
	{
		pszMsgOut->Format("PR2Posn=%.3f %.3f offset=%ld %ld", m_dmtrCurrPR2Posn.x, m_dmtrCurrPR2Posn.y, mtr_offset.x, mtr_offset.y);
	}

	m_dmtrCurrPR2Posn.x += (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR2Posn.y += (DOUBLE)mtr_offset.y;
#ifdef ENCODER_BASED
	m_dmtrCurrPR2BeforePRPosn[lGlass][(LONG)emCamObjType].x = (DOUBLE)m_mtrCurrPR2BeforeEncPosn[lGlass][(LONG)emCamObjType].x + (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR2BeforePRPosn[lGlass][(LONG)emCamObjType].y = (DOUBLE)m_mtrCurrPR2BeforeEncPosn[lGlass][(LONG)emCamObjType].y + (DOUBLE)mtr_offset.y;

	m_dmtrCurrPR2AfterPRPosn[lGlass][(LONG)emCamObjType].x = (DOUBLE)m_mtrCurrPR2AfterEncPosn[lGlass][(LONG)emCamObjType].x + (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR2AfterPRPosn[lGlass][(LONG)emCamObjType].y = (DOUBLE)m_mtrCurrPR2AfterEncPosn[lGlass][(LONG)emCamObjType].y + (DOUBLE)mtr_offset.y;
#endif

	m_nPRInProgress = ST_PR_IDLE;

	// debug time
	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	//CString szMsg = "";
	//szMsg.Format("%s PR2 Process Time: %.5f", pPRU->szFileName, dProcessTime);
	//m_pAppMod->DisplayMessage(szMsg);

	return TRUE;
}
#else
BOOL CPRTaskStn::AutoSearchPR2ProcessOnly(PRU *pPRU)
{
	// debug time
	//DOUBLE dProcessTime = 0.0;
	//DWORD dwStartTime = GetTickCount();

	MTR_POSN mtr_offset;

	if (!m_bPRSelected)
	{
		m_nPRInProgress = ST_PR_IDLE;
		return TRUE;
	}

	if (!SearchPRProcessOnly(pPRU))
	{
		m_nPRInProgress = ST_PR_IDLE;
		return FALSE;
	}

	PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);

	m_dmtrCurrPR2Posn.x += (DOUBLE)mtr_offset.x;
	m_dmtrCurrPR2Posn.y += (DOUBLE)mtr_offset.y;

	m_nPRInProgress = ST_PR_IDLE;

	// debug time
	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
	//CString szMsg = "";
	//szMsg.Format("%s PR2 Process Time: %.5f", GetStnName(), dProcessTime);
	//DisplayMessage(szMsg);

	return TRUE;
}
#endif

//// PostBond
//BOOL CPRTaskStn::AutoSearchPBPR1(PRU *pPRU)
//{
//	if (!m_bPRSelected)
//	{
//		Sleep(m_lPRDelay);
//		return TRUE;
//	}
//
//	if (!SearchPR(pPRU))
//	{
//		return FALSE;
//	}
//	
//	m_PRCurrPBPR1Corner[PR_UPPER_LEFT].x	= pPRU->arcoDieCorners[PR_UPPER_LEFT].x;
//	m_PRCurrPBPR1Corner[PR_UPPER_LEFT].y	= pPRU->arcoDieCorners[PR_UPPER_LEFT].y;
//	m_PRCurrPBPR1Corner[PR_LOWER_LEFT].x	= pPRU->arcoDieCorners[PR_LOWER_LEFT].x;
//	m_PRCurrPBPR1Corner[PR_LOWER_LEFT].y	= pPRU->arcoDieCorners[PR_LOWER_LEFT].y;
//	m_PRCurrPBPR1Corner[PR_UPPER_RIGHT].x	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].x;
//	m_PRCurrPBPR1Corner[PR_UPPER_RIGHT].y	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].y;
//	m_PRCurrPBPR1Corner[PR_LOWER_RIGHT].x	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].x;
//	m_PRCurrPBPR1Corner[PR_LOWER_RIGHT].y	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].y;
//
//	return TRUE;
//}
//
//
//BOOL CPRTaskStn::AutoSearchPBPR1GrabOnly(PRU *pPRU)
//{
//	// debug time
//	//DOUBLE dProcessTime = 0.0;
//	//DWORD dwStartTime = GetTickCount();
//
//	m_nPRInProgress = ST_PB_PR1_IN_PROGRESS;
//
//	if (!m_bPRSelected)
//	{
//		return TRUE;
//	}
//
//	if (!SearchPRGrabOnly(pPRU))
//	{
//		m_nPRInProgress = ST_PR_IDLE;
//		return FALSE;
//	}
//
//	// debug time
//	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
//	//CString szMsg = "";
//	//szMsg.Format("%s PR1 Grab Time: %.5f", GetStnName(), dProcessTime);
//	//DisplayMessage(szMsg);
//
//	return TRUE;
//}
//
//BOOL CPRTaskStn::AutoSearchPBPR1ProcessOnly(PRU *pPRU)
//{
//	// debug time
//	//DOUBLE dProcessTime = 0.0;
//	//DWORD dwStartTime = GetTickCount();
//
//	if (!m_bPRSelected)
//	{
//		m_nPRInProgress = ST_PR_IDLE;
//		return TRUE;
//	}
//
//	if (!SearchPRProcessOnly(pPRU))
//	{
//		m_nPRInProgress = ST_PR_IDLE;
//		return FALSE;
//	}
//	
//	m_PRCurrPBPR1Corner[PR_UPPER_LEFT].x	= pPRU->arcoDieCorners[PR_UPPER_LEFT].x;
//	m_PRCurrPBPR1Corner[PR_UPPER_LEFT].y	= pPRU->arcoDieCorners[PR_UPPER_LEFT].y;
//	m_PRCurrPBPR1Corner[PR_LOWER_LEFT].x	= pPRU->arcoDieCorners[PR_LOWER_LEFT].x;
//	m_PRCurrPBPR1Corner[PR_LOWER_LEFT].y	= pPRU->arcoDieCorners[PR_LOWER_LEFT].y;
//	m_PRCurrPBPR1Corner[PR_UPPER_RIGHT].x	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].x;
//	m_PRCurrPBPR1Corner[PR_UPPER_RIGHT].y	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].y;
//	m_PRCurrPBPR1Corner[PR_LOWER_RIGHT].x	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].x;
//	m_PRCurrPBPR1Corner[PR_LOWER_RIGHT].y	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].y;
//
//	m_nPRInProgress = ST_PR_IDLE;
//
//	// debug time
//	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
//	//CString szMsg = "";
//	//szMsg.Format("%s PR1 Process Time: %.5f", GetStnName(), dProcessTime);
//	//DisplayMessage(szMsg);
//
//	return TRUE;
//}
//
//BOOL CPRTaskStn::AutoSearchPBPR2(PRU *pPRU)
//{
//	if (!m_bPRSelected)
//	{
//		Sleep(m_lPRDelay);
//		return TRUE;
//	}
//
//	if (!SearchPR(pPRU))
//	{
//		return FALSE;
//	}
//	
//	m_PRCurrPBPR2Corner[PR_UPPER_LEFT].x	= pPRU->arcoDieCorners[PR_UPPER_LEFT].x;
//	m_PRCurrPBPR2Corner[PR_UPPER_LEFT].y	= pPRU->arcoDieCorners[PR_UPPER_LEFT].y;
//	m_PRCurrPBPR2Corner[PR_LOWER_LEFT].x	= pPRU->arcoDieCorners[PR_LOWER_LEFT].x;
//	m_PRCurrPBPR2Corner[PR_LOWER_LEFT].y	= pPRU->arcoDieCorners[PR_LOWER_LEFT].y;
//	m_PRCurrPBPR2Corner[PR_UPPER_RIGHT].x	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].x;
//	m_PRCurrPBPR2Corner[PR_UPPER_RIGHT].y	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].y;
//	m_PRCurrPBPR2Corner[PR_LOWER_RIGHT].x	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].x;
//	m_PRCurrPBPR2Corner[PR_LOWER_RIGHT].y	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].y;
//
//	return TRUE;
//}
//
//
//BOOL CPRTaskStn::AutoSearchPBPR2GrabOnly(PRU *pPRU)
//{
//	// debug time
//	//DOUBLE dProcessTime = 0.0;
//	//DWORD dwStartTime = GetTickCount();
//
//	m_nPRInProgress = ST_PB_PR2_IN_PROGRESS;
//
//	if (!m_bPRSelected)
//	{
//		return TRUE;
//	}
//
//	if (!SearchPRGrabOnly(pPRU))
//	{
//		m_nPRInProgress = ST_PR_IDLE;
//		return FALSE;
//	}
//
//	// debug time
//	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
//	//CString szMsg = "";
//	//szMsg.Format("%s PR2 Grab Time: %.5f", GetStnName(), dProcessTime);
//	//DisplayMessage(szMsg);
//
//	return TRUE;
//}
//
//BOOL CPRTaskStn::AutoSearchPBPR2ProcessOnly(PRU *pPRU)
//{
//	// debug time
//	//DOUBLE dProcessTime = 0.0;
//	//DWORD dwStartTime = GetTickCount();
//
//	if (!m_bPRSelected)
//	{
//		m_nPRInProgress = ST_PR_IDLE;
//		return TRUE;
//	}
//
//	if (!SearchPRProcessOnly(pPRU))
//	{
//		m_nPRInProgress = ST_PR_IDLE;
//		return FALSE;
//	}
//	
//	m_PRCurrPBPR2Corner[PR_UPPER_LEFT].x	= pPRU->arcoDieCorners[PR_UPPER_LEFT].x;
//	m_PRCurrPBPR2Corner[PR_UPPER_LEFT].y	= pPRU->arcoDieCorners[PR_UPPER_LEFT].y;
//	m_PRCurrPBPR2Corner[PR_LOWER_LEFT].x	= pPRU->arcoDieCorners[PR_LOWER_LEFT].x;
//	m_PRCurrPBPR2Corner[PR_LOWER_LEFT].y	= pPRU->arcoDieCorners[PR_LOWER_LEFT].y;
//	m_PRCurrPBPR2Corner[PR_UPPER_RIGHT].x	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].x;
//	m_PRCurrPBPR2Corner[PR_UPPER_RIGHT].y	= pPRU->arcoDieCorners[PR_UPPER_RIGHT].y;
//	m_PRCurrPBPR2Corner[PR_LOWER_RIGHT].x	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].x;
//	m_PRCurrPBPR2Corner[PR_LOWER_RIGHT].y	= pPRU->arcoDieCorners[PR_LOWER_RIGHT].y;
//
//	m_nPRInProgress = ST_PR_IDLE;
//
//	// debug time
//	//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
//	//CString szMsg = "";
//	//szMsg.Format("%s PR2 Process Time: %.5f", GetStnName(), dProcessTime);
//	//DisplayMessage(szMsg);
//
//	return TRUE;
//}

VOID CPRTaskStn::UpdateHMIPRButton()
{
	PRU *pPRU;

	//Normal PR
	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);
	pPRU = &pThis->SelectCurPRU(pThis->HMI_ulCurPRU);

	pThis->HMI_ulPRID = pPRU->stSrchCmd.uwRecordID;
//	switch (pPRU->emRecordType)
//	{
//	// Fiducial Fitting or Pattern Matching or Edge Matching
//	case PRS_DIE_TYPE:
//		pThis->HMI_ulPRID = pPRU->stSrchDieCmd.auwRecordID[0];
//		pThis->HMI_dPRMatchScore = pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore;
//		pThis->HMI_dPRAngleRange = pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle;
//		pThis->HMI_dPRPercentVar = pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale - 100.0; // Hard-set
//		break;
//
//	// Shape Fitting
//	case PRS_SHAPE_TYPE:
//		pThis->HMI_ulPRID = pPRU->stDetectShapeCmd.uwRecordID;
//		pThis->HMI_dPRMatchScore = pPRU->stDetectShapeCmd.rDetectScore;
//#if 1 //20130103
//		pThis->HMI_dPRAngleRange = pPRU->stDetectShapeCmd.rEndAngle;
//#else
//		pThis->HMI_dPRAngleRange = 0.0;
//#endif
//		pThis->HMI_dPRPercentVar = pPRU->stDetectShapeCmd.rSizePercentVar;
//		break;
//
//	// Tmpl Fitting
//	case PRS_TMPL_TYPE:
//		pThis->HMI_ulPRID = pPRU->stSrchTmplCmd.uwRecordID;
//		pThis->HMI_dPRMatchScore = pPRU->stSrchTmplCmd.ubPassScore;
//		pThis->HMI_dPRAngleRange = 0.0;
//		pThis->HMI_dPRPercentVar = 5.0;
//		break;
//
//	}

	pThis->HMI_bShowPRSearch = TRUE; //pPRU->bLoaded;
}

VOID CPRTaskStn::PrintCalibSetup(FILE *fp)
{
	CString str, str1, str2 = "";

	if (fp != NULL)
	{
		fprintf(fp, "\n%s COR Calibration\n", (const char*)m_szPRTaskStnName);
		fprintf(fp, "======================================================\n");

		
		
		for (INT j = 0; j < MAX_NUM_OF_TA4_COR; j++)
		{
			str.Format("m_bCORCalibrated[%ld]", j);
			if ((BOOL)(LONG)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][str])
			{
				fprintf(fp, "COR Calibration			: Calibrated\n");
			}
			else
			{
				fprintf(fp, "COR Calibration			: Not Calibrated\n");
			}
			for (INT i = 0; i < COR_ARRAY_SIZE; i++)
			{
				str.Format("m_dCORLUT[%ld][%ld]", j, i);
				str1.Format("m_dmtrTableCOR_Pos[%ld][%ld].x", j, i);
				str2.Format("m_dmtrTableCOR_Pos[%ld][%ld].y", j, i);
				fprintf(fp, "%30s\t : %.2f, %.2f, %.2f\n", "COR Angle (+)", 
					(DOUBLE)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][str], 
					(DOUBLE)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][str1], 
					(DOUBLE)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][str2]);
			}


			for (INT i = 0; i < COR_ARRAY_SIZE; i++)
			{
				str.Format("m_dCORLUT[%ld][%ld]", j, i);
				str1.Format("m_dmtrTableCOR_Neg[%ld][%ld].x", j, i);
				str2.Format("m_dmtrTableCOR_Neg[%ld][%ld].y", j, i);
				fprintf(fp, "%30s\t : %.2f, %.2f, %.2f\n", "COR Angle (-)", 
					(DOUBLE)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][str],  
					(DOUBLE)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][str1], 
					(DOUBLE)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][str2]);

			}
		}
	}
}
/////////////////////////////////////////////////////////////////
//HMI Function
/////////////////////////////////////////////////////////////////
LONG CPRTaskStn::HMI_SetPRMatchScore(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	DOUBLE dMatchScore;
	svMsg.GetMsg(sizeof(DOUBLE), &dMatchScore);

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);
	pPRU = &pThis->SelectCurPRU(pThis->HMI_ulCurPRU);

	CString szOldValue;
	szOldValue.Format("ID = %ld, Value = %.2f", pPRU->emID, pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore);
		
	pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore	= (PR_REAL)dMatchScore;
	pPRU->stSrchDieCmd.rCfmPatternMatchScore		= (PR_REAL)dMatchScore; // Comfirmation Pattern 	
	pPRU->stDetectShapeCmd.rDetectScore				= (PR_REAL)dMatchScore;
	pPRU->stSrchTmplCmd.ubPassScore					= (PR_UBYTE)dMatchScore;

	CString szNewValue;
	szNewValue.Format("ID = %ld, Value = %.2f", pPRU->emID, pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore);

	LogPRParameter(__FUNCTION__, "PR Match Score", szOldValue, szNewValue);

	UpdateHMIPRButton();

	return 0;
}

LONG CPRTaskStn::HMI_SetPRAngRange(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	DOUBLE dAngRange;
	svMsg.GetMsg(sizeof(DOUBLE), &dAngRange);

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);
	pPRU = &pThis->SelectCurPRU(pThis->HMI_ulCurPRU);

	CString szOldValue;
	szOldValue.Format("ID = %ld, Value = %.2f", pPRU->emID, pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle);

	switch (pPRU->emRecordType)
	{
	// Pattern Matching or Edge Matching
	case PRS_DIE_TYPE:
		pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle = -(PR_REAL)dAngRange;
		pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle = (PR_REAL)dAngRange;
		break;

	// Shape Fitting
	case PRS_SHAPE_TYPE:
		pPRU->stDetectShapeCmd.rStartAngle = -(PR_REAL)dAngRange; //20130103
		pPRU->stDetectShapeCmd.rEndAngle = (PR_REAL)dAngRange;
		break;

	// Tmpl Fitting
	case PRS_TMPL_TYPE:
		break;

	}

	CString szNewValue;
	szNewValue.Format("ID = %ld, Value = %.2f", pPRU->emID, pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle);

	LogPRParameter(__FUNCTION__, "PR Angle Range", szOldValue, szNewValue);

	UpdateHMIPRButton();

	return 0;
}

LONG CPRTaskStn::HMI_SetPRPercentVar(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	DOUBLE dVar;
	svMsg.GetMsg(sizeof(DOUBLE), &dVar);

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);
	pPRU = &pThis->SelectCurPRU(pThis->HMI_ulCurPRU);

	CString szOldValue;
	szOldValue.Format("ID = %ld, Value = %.2f", pPRU->emID, pPRU->stDetectShapeCmd.rSizePercentVar);

	switch (pPRU->emRecordType)
	{
	case PRS_DIE_TYPE:
		pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale	= (PR_UBYTE)(100.0 + dVar); //20130103
		pPRU->stSrchDieCmd.stDieAlignPar.ubMinScale	= (PR_UBYTE)(100.0 - dVar);
		break;

	case PRS_SHAPE_TYPE:
		pPRU->stDetectShapeCmd.rSizePercentVar		= (PR_REAL)dVar;
		break;

	case PRS_TMPL_TYPE:
		break;

	}

	CString szNewValue;
	szNewValue.Format("ID = %ld, Value = %.2f", pPRU->emID, pPRU->stDetectShapeCmd.rSizePercentVar);

	LogPRParameter(__FUNCTION__, "PR Percentage Variation", szOldValue, szNewValue);

	UpdateHMIPRButton();

	return 0;
}

VOID CPRTaskStn::MachineStartup_SrchPR(PRU *pPRU)	// 20140623 Yip
{
	if (!pPRU->bLoaded)
	{
		return;
	}

	PRS_DisplayVideo(pPRU);

#if 1 //Vision UI
	PRS_ManualSrchCmd(pPRU);
#else
	switch (pPRU->emRecordType)
	{
	// Pattern Matching or Edge Matching
	case PRS_DIE_TYPE:
		PRS_SrchDie(pPRU);
		break;

	// Shape Fitting
	case PRS_SHAPE_TYPE:
		PRS_DetectShape(pPRU);
		break;

	// Tmpl Fitting
	case PRS_TMPL_TYPE:
		PRS_SrchTmpl(pPRU);
		break;
	}
#endif
	return;
}

//PR Action
LONG CPRTaskStn::HMI_ModifyPRCriteria(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);
	pPRU = &pThis->SelectCurPRU(pThis->HMI_ulCurPRU);

	PRS_DisplayVideo(pPRU);

	ModifyPRCriteria(pPRU);	// Vision UI

	UpdateHMIPRButton();

	BOOL bResult = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

#if 1 // Vision UI
LONG CPRTaskStn::HMI_SrchPR(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);
	pPRU = &pThis->SelectCurPRU(pThis->HMI_ulCurPRU);

	PRS_DisplayVideo(pPRU);

	SrchPR_New(pPRU);	// Vision UI

	UpdateHMIPRButton();

	BOOL bResult = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}
#else
LONG CPRTaskStn::HMI_SrchPR(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);
	pPRU = &pThis->SelectCurPRU(pThis->HMI_ulCurPRU);

	PRS_DisplayVideo(pPRU);

	switch (pPRU->emRecordType)
	{
	// Pattern Matching or Edge Matching
	case PRS_DIE_TYPE:
		SrchPRDie(pPRU);
		break;

	// Shape Fitting
	case PRS_SHAPE_TYPE:
		DetectPRShape(pPRU);
		break;

	// Tmpl Fitting
	case PRS_TMPL_TYPE:
		SrchPRTmpl(pPRU);
		break;
	}

	UpdateHMIPRButton();

	BOOL bResult = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}
#endif


LONG CPRTaskStn::HMI_SetPRRetryLimit(IPC_CServiceMessage &svMsg)
{
	LONG lLimit;
	svMsg.GetMsg(sizeof(LONG), &lLimit);
	
	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);

	SetAndLogPRParameter(__FUNCTION__, "PR Retry Limit", pThis->m_lPRRetryLimit, lLimit);

	return 0;
}

LONG CPRTaskStn::HMI_SetRejectGlassLimit(IPC_CServiceMessage &svMsg)
{
	LONG lLimit;
	svMsg.GetMsg(sizeof(LONG), &lLimit);

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);

	SetAndLogPRParameter(__FUNCTION__, "Reject Glass Limit", pThis->m_lRejectGlassLimit, lLimit);

	return 0;
}

LONG CPRTaskStn::HMI_SetPRDelay(IPC_CServiceMessage &svMsg)
{
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);

	SetAndLogPRParameter(__FUNCTION__, "PR Delay", pThis->m_lPRDelay, lDelay);

	return 0;
}

LONG CPRTaskStn::HMI_SetPRDelay2(IPC_CServiceMessage &svMsg)
{
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);

	SetAndLogPRParameter(__FUNCTION__, "PR Delay", pThis->m_lPRDelay2, lDelay);

	return 0;
}

LONG CPRTaskStn::HMI_SetCalibPRDelay(IPC_CServiceMessage &svMsg)
{
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	CPRTaskStn *pThis = dynamic_cast<CPRTaskStn*>(pGF);

	SetAndLogPRParameter(__FUNCTION__, "Calib PR Delay", pThis->m_lCalibPRDelay, lDelay);

	return 0;
}

LONG CPRTaskStn::IPC_SavePRTaskStnMachinePara()
{
	CString szParamName;

	m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["m_lCalibPRDelay"]					= m_lCalibPRDelay;
	m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["m_lPRErrorLimit"]					= m_lPRErrorLimit;	//20141003

	// COR
	

	for (INT j = 0; j < MAX_NUM_OF_TA4_COR; j++)
	{
		szParamName.Format("m_bCORCalibrated[%ld]", j);
		m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName]			= m_bCORCalibrated[j];
		for (INT i = 0; i < COR_ARRAY_SIZE; i++)
		{
			szParamName.Format("m_dmtrTableCOR_Pos[%ld][%ld].x", j, i);
			m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName]					= m_dmtrTableCOR_Pos[j][i].x;
			szParamName.Format("m_dmtrTableCOR_Pos[%ld][%ld].y", j, i);
			m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName]				= m_dmtrTableCOR_Pos[j][i].y;
			szParamName.Format("m_dmtrTableCOR_Neg[%ld][%ld].x", j, i);
			m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName]				= m_dmtrTableCOR_Neg[j][i].x;
			szParamName.Format("m_dmtrTableCOR_Neg[%ld][%ld].y", j, i);
			m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName]				= m_dmtrTableCOR_Neg[j][i].y;
			szParamName.Format("m_dCORLUT[%ld][%ld]", j, i);
			m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName]				= m_dCORLUT[j][i];
		}
	}
	return 1;
}

LONG CPRTaskStn::IPC_LoadPRTaskStnMachinePara()
{
	CString szParamName;

	m_lCalibPRDelay			= m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["m_lCalibPRDelay"];
	m_lPRErrorLimit			= m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["m_lPRErrorLimit"];

	// COR

	for (INT j = 0; j < MAX_NUM_OF_TA4_COR; j++)
	{
		szParamName.Format("m_bCORCalibrated[%ld]", j);
		m_bCORCalibrated[j]		= (BOOL)(LONG)m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];

		for (INT i = 0; i < COR_ARRAY_SIZE; i++)
		{
			szParamName.Format("m_dmtrTableCOR_Pos[%ld][%ld].x", j, i);
			m_dmtrTableCOR_Pos[j][i].x		= m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			szParamName.Format("m_dmtrTableCOR_Pos[%ld][%ld].y", j, i);
			m_dmtrTableCOR_Pos[j][i].y		= m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			szParamName.Format("m_dmtrTableCOR_Neg[%ld][%ld].x", j, i);
			m_dmtrTableCOR_Neg[j][i].x		= m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			szParamName.Format("m_dmtrTableCOR_Neg[%ld][%ld].y", j, i);
			m_dmtrTableCOR_Neg[j][i].y		= m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			szParamName.Format("m_dCORLUT[%ld][%ld]", j, i);
			m_dCORLUT[j][i]				= m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
		}
	}
	return 1;
}


LONG CPRTaskStn::IPC_SavePRTaskStnDevicePara()
{
	m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRRetryLimit"]			= m_lPRRetryLimit;
	m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lRejectGlassLimit"]		= m_lRejectGlassLimit;

	m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRDelay"]		= m_lPRDelay;
	m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRDelay2"]		= m_lPRDelay2;
	
	return 1;
}	

LONG CPRTaskStn::IPC_LoadPRTaskStnDevicePara()
{
	m_lPRRetryLimit		= m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRRetryLimit"];
	m_lRejectGlassLimit = m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lRejectGlassLimit"];

	m_lPRDelay			= m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRDelay"];
	m_lPRDelay2			= m_pAppMod->m_smfDevice[m_szPRTaskStnName]["PRTaskStn"]["m_lPRDelay2"];
	
	return 1;
}	

LONG CPRTaskStn::IPC_SaveMachineRecord()
{

	return 1;
}

LONG CPRTaskStn::IPC_LoadMachineRecord()
{
	return 1;
}

LONG CPRTaskStn::IPC_SaveDeviceRecord()
{
	return 1;
}

LONG CPRTaskStn::IPC_LoadDeviceRecord()
{
	return 1;
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
LONG CPRTaskStn::printPRTaskStnMachinePara()
{
	//CFX9000App *pAppMod = dynamic_cast<CFX9000App*> (m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	//CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;
	CString szParamName;

	if (bResult)
	{
		lValue = (LONG) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["m_lCalibPRDelay"];
		bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("m_lCalibPRDelay"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	if (bResult)
	{
		lValue = (LONG) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["m_lPRErrorLimit"];
		bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("m_lPRErrorLimit"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	// COR
	for(INT j = 0; j < MAX_NUM_OF_TA4_COR; j++)
	{
		if (bResult)
		{
			szParamName.Format("m_bCORCalibrated[%ld]", j);
			lValue = (LONG) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("COR"), szParamName, szBlank,
				szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		}

		for (INT i = 0; i < COR_ARRAY_SIZE; i++)
		{
			szParamName.Format("m_dmtrTableCOR_Pos[%ld][%ld].x", j, i);
			dValue = (DOUBLE) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("COR"), szParamName, szBlank,
				szBlank, szBlank, szBlank, szBlank, szBlank, dValue);
			if (!bResult)
			{
				break;
			}

			szParamName.Format("m_dmtrTableCOR_Pos[%ld][%ld].y", j, i);
			dValue = (DOUBLE) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("COR"), szParamName, szBlank,
				szBlank, szBlank, szBlank, szBlank, szBlank, dValue);
			if (!bResult)
			{
				break;
			}

			szParamName.Format("m_dmtrTableCOR_Neg[%ld][%ld].x", j, i);
			dValue = (DOUBLE) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("COR"), szParamName, szBlank,
				szBlank, szBlank, szBlank, szBlank, szBlank, dValue);
			if (!bResult)
			{
				break;
			}

			szParamName.Format("m_dmtrTableCOR_Neg[%ld][%ld].y", j, i);
			dValue = (DOUBLE) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("COR"), szParamName, szBlank,
				szBlank, szBlank, szBlank, szBlank, szBlank, dValue);
			if (!bResult)
			{
				break;
			}

			szParamName.Format("m_dCORLUT[%ld][%ld]", j, i);
			dValue = (DOUBLE) m_pAppMod->m_smfMachine[m_szPRTaskStnName]["PRTaskStn"]["COR"][szParamName];
			bResult = printMachineInfoRow(m_szPRTaskStnName, CString("PRTaskStn"), CString("COR"), szParamName, szBlank,
				szBlank, szBlank, szBlank, szBlank, szBlank, dValue);
			if (!bResult)
			{
				break;
			}
		}
	}
	return bResult;
}
#endif

VOID CPRTaskStn::SetAndLogPRParameter(const CString &szFunction, const CString &szParameter, LONG &lParameter, LONG lNewValue)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("Value = %ld", lParameter);

	lParameter = lNewValue;

	CString szNewValue;
	szNewValue.Format("Value = %ld", lNewValue);

	LogPRParameter(szFunction, szParameter, szOldValue, szNewValue);
}

VOID CPRTaskStn::LogPRParameter(const CString &szStation, const CString &szFunction, const CString &szParameter, const CString &szOldValue, const CString &szNewValue)	// 20140729 Yip
{
	LogAction(szFunction);
	LogAction("%s\t%s", szParameter, szOldValue);
	LogAction("%s\t%s", szParameter, szNewValue);
	theApp.LogParameter(szStation, szFunction, szParameter, szOldValue, szNewValue);
}

VOID CPRTaskStn::LogPRParameter(const CString &szFunction, const CString &szParameter, const CString &szOldValue, const CString &szNewValue)	// 20140729 Yip
{
	SFM_CStation *pGF = (SFM_CStation*)(void*)this; // grand-grand-father
	LogPRParameter(pGF->GetName(), szFunction, szParameter, szOldValue, szNewValue);
}

BOOL CPRTaskStn::AutoTest(PRU *pPRU)
{
	CWinEagle *pCWinEagle = (CWinEagle*)m_pAppMod->GetStation("WinEagle");

	CString szMsg;
	MTR_POSN mtr_offset;
	BOOL bResult		= TRUE;
	PRS_DisplayVideo(pPRU);
	int count	= 4;
	int i;
	FLOAT fTargetPointX[4], fTargetPointY[4];
	CString szMoveOut[4], szMoveIn[4];

	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!", m_szPRTaskStnName);
		m_pAppMod->HMIMessageBox(MSG_OK, "PR AUTO TEST OPERATION", szMsg);
		return FALSE;
	}

	m_pAppMod->WriteHMIMess("Starting Auto Test...");
				
	//Movement 
	PRS_ClearScreen(pPRU->nCamera);


	MTR_POSN mtr_oldPosn;
	PR_RCOORD pr_diesize;
	PR_RCOORD pr_coord;
	pr_diesize.x	= 0.0;
	pr_diesize.y	= 0.0;
	pr_coord.x		= 0.0;
	pr_coord.y		= 0.0;
	FLOAT errorX		= 0.0;
	FLOAT errorY		= 0.0; 
	FLOAT	targetX		= 0.0;
	FLOAT	targetY		= 0.0;

	pr_diesize.x = pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].x - pPRU->arcoLrnDieCorners[PR_UPPER_LEFT].x;
	pr_diesize.y = pPRU->arcoLrnDieCorners[PR_LOWER_RIGHT].y - pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].y;
	//switch (pPRU->emRecordType)
	//{
	//// Pattern Matching or Edge Matching
	//case PRS_DIE_TYPE:
	//	pr_diesize.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x - pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
	//	pr_diesize.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y - pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
	//	break;

	//// Shape Fitting
	//case PRS_SHAPE_TYPE:
	//	pr_diesize.x = pPRU->stLrnShapeCmd.stEncRect.coCorner2.x - pPRU->stLrnShapeCmd.stEncRect.coCorner1.x;
	//	pr_diesize.y = pPRU->stLrnShapeCmd.stEncRect.coCorner2.y - pPRU->stLrnShapeCmd.stEncRect.coCorner1.y;
	//	break;

	//// Tmpl Fitting
	//case PRS_TMPL_TYPE:
	//	pr_diesize.x = pPRU->stLoadTmplCmd.szTmplSize.x * 2;
	//	pr_diesize.y = pPRU->stLoadTmplCmd.szTmplSize.y * 2;
	//	break;
	//}

	//Excel file 
	BOOL bOpDone	= TRUE;
	LONG m_lCaliInfoRowNum	= 0;
	INT  nCol				= 0;
	const CString  szDestPath = _T("D:\\sw\\PI9000\\Data\\AutoTest\\");
	CString szTime = _T(""), szFileName = _T("");
	
	SYSTEMTIME ltime;		// New Command

	// Get time as 64-bit integer.
	GetLocalTime(&ltime);

	// Convert to local time.
	szTime.Format("%u-%u-%u %u:%u:%u\t", 
				  ltime.wYear, ltime.wMonth, ltime.wDay, 
				  ltime.wHour, ltime.wMinute, ltime.wSecond);

	
	
	szFileName = szDestPath + CString("PI9000_PRCaliInfo_") + m_szPRTaskStnName + CString(".csv");
	FILE *fp = fopen(szFileName, "a+");

	do
	{
		fTargetPointX[0] = ((PR_MAX_COORD + 1) / 8 * 7 - pr_diesize.x / 2);
		fTargetPointX[1] = ((PR_MAX_COORD + 1) / 8 * 1 + pr_diesize.x / 2);
		fTargetPointX[2] = ((PR_MAX_COORD + 1) / 8 * 1 + pr_diesize.x / 2);
		fTargetPointX[3] = ((PR_MAX_COORD + 1) / 8 * 7 - pr_diesize.x / 2);

		fTargetPointY[0] = ((PR_MAX_COORD + 1) / 8 * 7 - pr_diesize.x / 2);
		fTargetPointY[1] = ((PR_MAX_COORD + 1) / 8 * 7 - pr_diesize.x / 2);
		fTargetPointY[2] = ((PR_MAX_COORD + 1) / 8 * 1 + pr_diesize.x / 2);
		fTargetPointY[3] = ((PR_MAX_COORD + 1) / 8 * 1 + pr_diesize.x / 2);

		szMoveOut[0] = "Move to Upper-Left Point";
		szMoveOut[1] = "Move to Upper-Right Point";
		szMoveOut[2] = "Move to Lower-Right Point";
		szMoveOut[3] = "Move to Lower-Left Point";

		szMoveIn[0] = "Move back from Upper-Left Point";
		szMoveIn[1] = "Move back from Upper-Right Point";
		szMoveIn[2] = "Move back from Lower-Right Point";
		szMoveIn[3] = "Move back from Lower-Left Point";
		
		

		DOUBLE thetaX	= 180 * atan(pPRU->p2m_xform.b / pPRU->p2m_xform.a) / 3.1415926;
		DOUBLE thetaY	= 180 * atan(-1 * (pPRU->p2m_xform.c / pPRU->p2m_xform.d)) / 3.1415926;

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			fprintf(fp, "%s,PR Transform Angle X (degree): %.3f,PR Transform Angle Y (degree): %.3f,Difference (Theta Y-Theta X): %.3f,\n", (const char*)szTime, thetaX, thetaY, (thetaY - thetaX)); 
			fprintf(fp, "%s,Movement,Target Point X:,Target Point Y:,Before Move PR X:,Before Move PR Y:,Before Move Motor Posn X:,Before Move Motor Posn Y:,After Move PR X:,After Move PR Y:,After Move Motor Posn X:,After Move Motor Posn Y:,POSN X:,POSN Y:,Away from Target Point X:,Away from Target Point Y:\n", (const char*)szTime); 
			//fclose(fp);
		}											 
														
// back to origin
		if (!SearchPRCentre(pPRU))
		{
			bResult = FALSE;
			break;
		}
		

		mtr_oldPosn.x = GetMotorX().GetEncPosn();
		mtr_oldPosn.y = GetMotorY().GetEncPosn();

		

		for (i = 0; i < count; i++)
		{
			//Move Out
			PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);
			szMsg.Format("Move to Out Point");
			m_pAppMod->WriteHMIMess(szMsg);
			szMsg.Format("BEFORE MOVE: PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
			m_pAppMod->WriteHMIMess(szMsg);
			pr_coord.x = fTargetPointX[i];
			pr_coord.y = fTargetPointY[i];
			PRS_PRPosToMtrOffset(pPRU, pr_coord, &mtr_offset);
			targetX	= PR_MAX_COORD + 1 - pr_coord.x;
			targetY	= PR_MAX_COORD + 1 - pr_coord.y;

			if (fp != NULL)
			{
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%s,%s,%.2f,%.2f,%.2f,%.2f,%ld,%ld,", (const char*)szTime, (const char*)szMoveOut[i], targetX, targetY, pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, mtr_oldPosn.x, mtr_oldPosn.y); 
			}
			//Move
		
			if (
				(MoveAbsoluteXY(mtr_oldPosn.x, mtr_oldPosn.y, GMP_WAIT) != GMP_SUCCESS) || 
				SleepWithCalibDelay() != GMP_SUCCESS || 
				(MoveAbsoluteXY(mtr_oldPosn.x + mtr_offset.x, mtr_oldPosn.y + mtr_offset.y, GMP_WAIT) != GMP_SUCCESS) 
			   )
			{
				bResult = FALSE;
				break;
			}
	
			Sleep(m_lCalibPRDelay);

			PRS_ClearScreen(pPRU->nCamera);

			if (!SearchPR(pPRU))
			{
				bResult = FALSE;
				break;
			}

			PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);


			mtr_oldPosn.x = GetMotorX().GetEncPosn();
			mtr_oldPosn.y = GetMotorY().GetEncPosn();


			szMsg.Format("AFTER MOVE: PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
			m_pAppMod->WriteHMIMess(szMsg);

			errorX = pPRU->rcoDieCentre.x - targetX;
			errorY = pPRU->rcoDieCentre.y - targetY;

			pr_coord.x = errorX + PR_DEF_CENTRE_X;
			pr_coord.y = errorY + PR_DEF_CENTRE_Y;

			PRS_PRPosToMtrOffset(pPRU, pr_coord, &mtr_offset);

			szMsg.Format("Away From Upper-Left Point (PR) : [%.2f, %.2f], (POSN): [%ld,%ld]", errorX, errorY, mtr_offset.x, mtr_offset.y);
			m_pAppMod->WriteHMIMess(szMsg);

			if (fp != NULL)
			{
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%.2f,%.2f,%ld,%ld,%ld,%ld,%.2f,%.2f\n", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, mtr_oldPosn.x, mtr_oldPosn.y, mtr_offset.x, mtr_offset.y, errorX, errorY); 
			}																															

			if (abs((INT)errorX) > pCWinEagle->HMI_lErrorLimit || abs((INT)errorY) > pCWinEagle->HMI_lErrorLimit)
			{
				szMsg.Format("%s, Error Is Out Of Range", szMoveOut[i]);
				m_pAppMod->WriteHMIMess(szMsg);
			}

			// Move In
			// back to origin
			if (!SearchPRCentre(pPRU))
			{
				bResult = FALSE;
				break;
			}


			szMsg.Format("Move Back from Out Point");
			m_pAppMod->WriteHMIMess(szMsg);
			szMsg.Format("BEFORE MOVE: PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
			m_pAppMod->WriteHMIMess(szMsg);

			if (fp != NULL)
			{
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%s,%s,%d,%d,%.2f,%.2f,%ld,%ld,", (const char*)szTime, (const char*)szMoveIn[i], PR_DEF_CENTRE_X, PR_DEF_CENTRE_Y, pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, mtr_oldPosn.x, mtr_oldPosn.y); 
			}

			Sleep(m_lCalibPRDelay);

			PRS_ClearScreen(pPRU->nCamera);
			PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);

			if (!SearchPR(pPRU))
			{
				bResult = FALSE;
				break;
			}
			PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

			szMsg.Format("AFTER MOVE: PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
			m_pAppMod->WriteHMIMess(szMsg);

			PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtr_offset);
			errorX = pPRU->rcoDieCentre.x - PR_DEF_CENTRE_X;
			errorY = pPRU->rcoDieCentre.y - PR_DEF_CENTRE_Y;

			
			mtr_oldPosn.x = GetMotorX().GetEncPosn();
			mtr_oldPosn.y = GetMotorY().GetEncPosn();
			

			szMsg.Format("Away From Center (PR):[%.2f,%.2f], (POSN): [%ld,%ld]", errorX, errorY, mtr_offset.x, mtr_offset.y);
			m_pAppMod->WriteHMIMess(szMsg);

			if (fp != NULL)
			{
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%.2f,%.2f,%ld,%ld,%ld,%ld,%.2f,%.2f\n", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, mtr_oldPosn.x, mtr_oldPosn.y, mtr_offset.x, mtr_offset.y, errorX, errorY); 
			}	

			if (abs((INT)errorX) > pCWinEagle->HMI_lErrorLimit || abs((INT)errorY) > pCWinEagle->HMI_lErrorLimit)
			{
				szMsg.Format("%s, Error Is Out Of Range", szMoveIn[i]);
				m_pAppMod->WriteHMIMess(szMsg);
			}

		}

	} while (0);

	//if (!bOpDone)
	//{
	//	m_pAppMod->HMIMessageBox(MSG_OK, "Calibration file fail", "Calibration file might be opened, please close it and try again.");
	//	//bOpDone = fo.Delete(szExcelTemplateDestPath + szExcelTemplateFileName);
	//	return FALSE;
	//}
	//else
	//{
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			fprintf(fp, "\n"); 
			fclose(fp);
		}
		return TRUE;
	//}
}

BOOL CPRTaskStn::CORAutoTest(PRU *pPRU, LONG lAutoTestCount, BOOL bUseRotaryEnc, BOOL bInvertAngle)
{
	CWinEagle *pCWinEagle = (CWinEagle*)m_pAppMod->GetStation("WinEagle");
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pAppMod->GetStation("HouseKeeping");
	CString szMsg = "";
	MTR_POSN mtrCurPosn;
	MTR_POSN mtrResultPosn;
	MTR_POSN mtrOffset;
	BOOL bResult = TRUE;
	DOUBLE dAng[8] = {-1.5, -1.0, -0.5, -0.1, 0.1, 0.5, 1.0, 1.5};
	DOUBLE dTestAng;
	LONG lErrorX[100] = {0};
	LONG lErrorY[100] = {0};
	LONG  lEminX(0), lEmaxX(0), lEminY(0), lEmaxY(0), lErrorSumX(0), lErrorSumY(0);
	DOUBLE dStDevX(0), dStDevY(0), dAverageX(0), dAverageY(0);
	double sx(0), sy(0);
	LONG lAnswer = rMSG_TIMEOUT;
	//LONG lAutoTestCount = 3;
	LONG lAngCount = 8;
	LONG i , j;
	LONG lStepTimes		= 1;	// 20180111

	if (pPRU->emCameraNo == PR_CAMERA_1)
	{
		lStepTimes = 1;
	}
	else
	{
		lStepTimes = 100;
	}

	CString str;

	PRS_DisplayVideo(pPRU);


	if (!m_bPRSelected)
	{
		szMsg.Format("%s PR Module Not Selected. Operation Abort!" , m_szPRTaskStnName);
		m_pAppMod->HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", szMsg);
		return FALSE;
	}

	if (!m_bCORCalibrated[m_lCORCamNum])
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", "This Module have not been Calibrated, Please calib first and retry");
		return FALSE;
	}


	//Move to Calib Posn 
	//Move to the Calib Posn with T is standby Posn
	if (
		MoveTToStandby() != GMP_SUCCESS ||
		MoveXYToPRCalibPosn() != GMP_SUCCESS ||
		MoveZToPRCalibLevel() != GMP_SUCCESS
	   )
	{
		return FALSE;
	}
	//Excel file 
	BOOL bOpDone	= TRUE;
	//const CString szExcelTemplateSrcPath = _T("D:\\sw\\PI9000\\Param\\"), szExcelTemplateDestPath = _T("D:\\sw\\PI9000\\Data\\"), szExcelTemplateFileName = _T("PI9000_Info_Template.xls");
	const CString  szDestPath = _T("D:\\sw\\PI9000\\Data\\AutoTest\\");
	CString szTime = _T(""), szFileName = _T("");
	
	SYSTEMTIME ltime;		// New Command

	// Get time as 64-bit integer.
	GetLocalTime(&ltime);

	// Convert to local time.
	szTime.Format("%u-%u-%u %u:%u:%u\t", 
				  ltime.wYear, ltime.wMonth, ltime.wDay, 
				  ltime.wHour, ltime.wMinute, ltime.wSecond);

	
	
	szFileName = szDestPath + CString("PI9000_CORCaliInfo_") + m_szPRTaskStnName + CString(".csv");
	FILE *fp = fopen(szFileName, "a+");
	do
	{

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			fprintf(fp, "%s,TestAngle,Away from Centre X(pixel):,Away from Centre Y(pixel):,Away from Centre POSN X:,Away from Centre POSN Y:,Error range X (min):,Error range X (max):,Error range Y (min):,Error range Y (max):,Error SD X:,Error SD Y:,\n", (const char*)szTime); 
			//fclose(fp);
		}	

		for (i = 0; i < lAngCount; i++)
		{
			lErrorSumX	= 0;
			lErrorSumY	= 0;
			dStDevX		= 0.0;
			dStDevY		= 0.0;
			dAverageX	= 0.0;
			dAverageY	= 0.0;

			if (pCHouseKeeping->HMI_bAutoTestStop)
			{
				pCHouseKeeping->HMI_bAutoTestStop	= FALSE;
				m_pAppMod->WriteHMIMess("COR Auto Test Stop!");
				bResult = FALSE;
				break;
			}
			
			dTestAng = dAng[i];


			for (j = 0; j < lAutoTestCount; j++)
			{
				if (pCHouseKeeping->HMI_bAutoTestStop || theApp.getKlocworkFalse())
				{
					pCHouseKeeping->HMI_bAutoTestStop = FALSE;
					m_pAppMod->WriteHMIMess("COR Auto Test Stop!");
					bResult = FALSE;
					break;
				}
		
					//Move to Calib Posn 
					//Move to the Calib Posn with T is standby Posn
				if (
					MoveTToStandby() != GMP_SUCCESS ||
					MoveXYToPRCalibPosn() != GMP_SUCCESS ||
					MoveZToPRCalibLevel() != GMP_SUCCESS
				   )
				{
					bResult = FALSE;
					break;
				}

				Sleep(m_lCalibPRDelay);

				if (!SearchPRCentre(pPRU))
				{
					m_pAppMod->HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", "COR Auto Test Failed!");
					PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
					m_pAppMod->WriteHMIMess("COR Auto Test SearchPRCentre Failed!");
					bResult = FALSE;
					break;

				}

				PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

				mtrCurPosn.x = GetMotorX().GetEncPosn();
				if (pPRU->emCameraNo == PR_CAMERA_1)
				{
					mtrCurPosn.y = GetMotorY().GetEncPosn();
				}
				else
				{
					mtrCurPosn.y = GetMotorY().Get2ndEncPosn();
				}
				//mtrCurPosn.y = GetMotorY().GetEncPosn();

				Sleep(m_lCalibPRDelay);

				if (!SearchPR(pPRU, FALSE))
				{
					m_pAppMod->HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", "COR Auto Test Failed!");
					PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
					m_pAppMod->WriteHMIMess(" COR Auto Test SearchPR Failed!");
					bResult = FALSE;
					break;
				}

				LONG lTestAngStep = AngleToCount(dTestAng, bUseRotaryEnc ? GetMotorT().stAttrib.dDistPerCountAux : GetMotorT().stAttrib.dDistPerCount);
				if (
					(bUseRotaryEnc && (MoveRelativeT(lTestAngStep, GMP_WAIT) != GMP_SUCCESS)) ||
					(!bUseRotaryEnc && (GetMotorT().MoveRelative(lTestAngStep, GMP_WAIT) != GMP_SUCCESS))
					)
				{
					m_pAppMod->HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", "COR Auto Test Failed!");
					m_pAppMod->WriteHMIMess(" COR Auto Test MoveRelative T Failed!");
					bResult = FALSE;
					break;
				}

				CalPosnAfterRotate(mtrCurPosn, bInvertAngle ? -dTestAng : dTestAng, &mtrResultPosn);

				mtrOffset.x = mtrResultPosn.x - mtrCurPosn.x;
				mtrOffset.y = mtrResultPosn.y - mtrCurPosn.y;



				MoveRelativeXY(mtrOffset.x, mtrOffset.y * lStepTimes, GMP_WAIT);
				PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Offset (%ld,%ld)", mtrOffset.x, mtrOffset.y * lStepTimes);

				Sleep(m_lCalibPRDelay);
				if (!SearchPR(pPRU, FALSE))
				{
					m_pAppMod->HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", "COR Auto Test Failed!");
					PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "PR Failed!");
					m_pAppMod->WriteHMIMess("COR Auto Test SearchPR2 Failed!");
					bResult = FALSE;
					break;
				}


				PRS_DrawCross(pPRU->nCamera, pPRU->rcoDieCentre, PRS_MEDIUM_SIZE, PR_COLOR_YELLOW);

				szMsg.Format("PR XY: [%.2f,%.2f]", pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y);
				m_pAppMod->WriteHMIMess(szMsg);

				PRS_PRPosToMtrOffset(pPRU, pPRU->rcoDieCentre, &mtrOffset);
				lErrorX[j] = (LONG)(pPRU->rcoDieCentre.x - PR_DEF_CENTRE_X);
				lErrorY[j] = (LONG)(pPRU->rcoDieCentre.y - PR_DEF_CENTRE_Y);

				szMsg.Format("Away From Centre (PR): [%ld, %ld] (POSN): [%ld,%ld]", lErrorX[j], lErrorY[j], mtrOffset.x, mtrOffset.y);
				m_pAppMod->WriteHMIMess(szMsg);

				lErrorX[j] = mtrOffset.x;
				lErrorY[j] = mtrOffset.y;

				lErrorSumX += lErrorX[j];
				lErrorSumY += lErrorY[j];

				if (fp != NULL)
				{
					fseek(fp, 0, SEEK_END);
					fprintf(fp, "%s,%.3f,%ld,%ld,%ld,%ld,", (const char*)szTime, dTestAng, lErrorX[j], lErrorY[j], mtrOffset.x, mtrOffset.y); 
					if (j != (lAutoTestCount - 1))
					{
						fseek(fp, 0, SEEK_END);
						fprintf(fp, "\n");
					}
				}

				PRS_ClearScreen(pPRU->nCamera);

				if (abs(lErrorX[j]) > pCWinEagle->HMI_lErrorLimit || abs(lErrorY[j]) > pCWinEagle->HMI_lErrorLimit)
				{
					szMsg.Format(" Test Ang:%.1lf,  Error Is Out Of Range", dTestAng);
					m_pAppMod->WriteHMIMess(szMsg);

				}
			}
			if (!bResult) 
			{
				break;
			}

//			m_lCaliInfoRowNum--;
			dAverageX = ((double)lErrorSumX) / lAutoTestCount;
			dAverageY = ((double)lErrorSumY) / lAutoTestCount;
			lEminX = lErrorX[0];
			lEmaxX = lErrorX[0];
			lEminY = lErrorY[0];
			lEmaxY = lErrorY[0];

			for (j = 0; j < lAutoTestCount; j++)
			{
					
				lEminX = lEminX < lErrorX[j] ? lEminX : lErrorX[j];
				lEminY = lEminY < lErrorY[j] ? lEminY : lErrorY[j];
				lEmaxX = lEmaxX < lErrorX[j] ? lErrorX[j] : lEmaxX ;
				lEmaxY = lEmaxY < lErrorY[j] ? lErrorY[j] : lEmaxY ;

				sx += (lErrorX[j] - dAverageX) * (lErrorX[j] - dAverageX);
				sy += (lErrorY[j] - dAverageY) * (lErrorY[j] - dAverageY);
			}
			dStDevX = sqrt(sx / lAutoTestCount);
			dStDevY = sqrt(sy / lAutoTestCount);

			if (fp != NULL)
			{
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%ld,%ld,%ld,%ld,%.5f,%.5f,\n", lEminX, lEmaxX, lEminY, lEmaxY, dStDevX, dStDevY); 

			}
		
		}		
	} while (0);

	if (!bResult)
	{
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			fprintf(fp, "\n"); 
			fclose(fp);
		}
		m_pAppMod->HMIMessageBox(MSG_OK, "Calibration file Save", "Calibration file Save");
		return FALSE;
	}
	else
	{
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			fprintf(fp, "\n"); 
			fclose(fp);
		}
		return TRUE;
	}

}

#if 0
BOOL CPRTaskStn::ExcelTest(LONG lCount)
{
	CWinEagle *pCWinEagle = (CWinEagle*)m_pAppMod->GetStation("WinEagle");
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pAppMod->GetStation("HouseKeeping");
	CString szMsg = "";
	MTR_POSN mtrCurPosn;
	//MTR_POSN mtrResultPosn;
	MTR_POSN mtrOffset;
	BOOL bResult = TRUE;
	DOUBLE dAng[8] = {-5.0, -3.0, -1.0, -0.5, 0.5, 1.0, 3.0, 5.0};
	DOUBLE dTestAng;
	LONG lErrorX[100] = {0,};
	LONG lErrorY[100] = {0,};
	LONG  lEminX(0), lEmaxX(0), lEminY(0), lEmaxY(0), lErrorSumX(0), lErrorSumY(0);
	DOUBLE dStDevX(0), dStDevY(0), dAverageX(0), dAverageY(0);
	double sx(0), sy(0);
	LONG lAnswer = rMSG_TIMEOUT;
	LONG lAutoTestCount = 3;
	LONG lAngCount = 8;
	LONG i = 0 , j = 0;

	CString str;

	//Excel file 
	BOOL bOpDone	= TRUE;
	const CString szExcelTemplateSrcPath = _T("D:\\sw\\PI9000\\Param\\"), szExcelTemplateDestPath = _T("D:\\sw\\PI9000\\Data\\"), szExcelTemplateFileName = _T("PI9000_Info_Template.xls");
	CString szTime = _T(""), szFileName = _T("");
	
	SYSTEMTIME ltime;		// New Command

	// Get time as 64-bit integer.
	GetLocalTime(&ltime);

	// Convert to local time.
	szTime.Format("%u-%u-%u %u:%u:%u\n", 
				  ltime.wYear, ltime.wMonth, ltime.wDay, 
				  ltime.wHour, ltime.wMinute, ltime.wSecond);

	
	
	szFileName = szExcelTemplateDestPath + CString("PI9000_CORCaliInfo") + CString(".xls");
	BasicExcel *pExcelDB = NULL;
	pExcelDB = new BasicExcel((const char*)szFileName);
	BasicExcelWorksheet		*m_pExcelCaliInfoSheet = NULL;
	CFileOperation fo;

	if (pExcelDB == NULL)
	{
		return FALSE;
	}
	do
	{

		//Excel file 

		if (!(pExcelDB->Load((const char*)szFileName)))
		{		
			bOpDone = fo.Copy(szExcelTemplateSrcPath + szExcelTemplateFileName, szExcelTemplateDestPath);
			if (!(fo.Rename(szExcelTemplateDestPath + szExcelTemplateFileName, szFileName)))
			{
				bOpDone = FALSE;
				break;
			}

			if (pExcelDB->Load(szFileName))
			{
				pExcelDB->New(5);
				pExcelDB->RenameWorksheet((size_t)0, (const char*)"InspWH");	
				pExcelDB->RenameWorksheet((size_t)1, (const char*)"PBDL1");
				pExcelDB->RenameWorksheet((size_t)2, (const char*)"PBDL2");
				pExcelDB->RenameWorksheet((size_t)3, (const char*)"PB1");
				pExcelDB->RenameWorksheet((size_t)4, (const char*)"PB2");
			}
		}
		else
		{
				pExcelDB->New(5);
				pExcelDB->RenameWorksheet((size_t)0, (const char*)"InspWH");	
				pExcelDB->RenameWorksheet((size_t)1, (const char*)"PBDL1");
				pExcelDB->RenameWorksheet((size_t)2, (const char*)"PBDL2");
				pExcelDB->RenameWorksheet((size_t)3, (const char*)"PB1");
				pExcelDB->RenameWorksheet((size_t)4, (const char*)"PB2");
		}
		LONG m_lCaliInfoRowNum	= 0;
		INT  nCol				= 0;

		CString szTask("InspWH");

		m_pExcelCaliInfoSheet = pExcelDB->GetWorksheet(szTask);

		m_lCaliInfoRowNum	= m_pExcelCaliInfoSheet->GetTotalRows();
		//m_lCaliInfoRowNum++;

		if (m_pExcelCaliInfoSheet != NULL)
		{
			for (i = 0; i < 6000; i++)
			{
				m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum++, nCol)->SetString((const char*)"abc");
			}
#if 0
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString(szTime);
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum++, nCol++)->SetString(szTask);
			nCol	= 0;
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString(szTime);
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("TestAngle");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("Away from Centre POSN X:");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("Away from Centre POSN Y:");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("Error range X (min):");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("Error range X (max):");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("Error range Y (min):");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("Error range Y (max):");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString("Error SD X:");
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum++, nCol++)->SetString("Error SD Y:");				
#endif
		}

#if 0
		for (i = 0; i < lAngCount; i++)
		{
			lErrorSumX	= 0;
			lErrorSumY	= 0;
			dStDevX		= 0.0;
			dStDevY		= 0.0;
			dAverageX	= 0.0;
			dAverageY	= 0.0;

			if (pCHouseKeeping->HMI_bAutoTestStop)
			{
				pCHouseKeeping->HMI_bAutoTestStop	= FALSE;
				m_pAppMod->WriteHMIMess("COR Auto Test Stop!");
				bResult = FALSE;
				break;
			}
			
			dTestAng = dAng[i];


			for (j = 0; j < lAutoTestCount; j++)
			{
				if (pCHouseKeeping->HMI_bAutoTestStop)
				{
					pCHouseKeeping->HMI_bAutoTestStop = FALSE;
					m_pAppMod->WriteHMIMess("COR Auto Test Stop!");
					bResult = FALSE;
					break;
				}
		
				mtrCurPosn.x = 0;
				mtrCurPosn.y = 0;
				mtrOffset.x = 0;
				mtrOffset.y = 0;

				nCol	= 0;
				m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString(szTime);
//					m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetString(szEdge);
				m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetDouble(dTestAng);
					//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetInteger(lErrorX[j]);
					//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetInteger(lErrorY[j]);
				m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetInteger(mtrOffset.x);
				m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum++, nCol++)->SetInteger(mtrOffset.y);

			}
			if (!bResult) 
			{
				break;
			}

			m_lCaliInfoRowNum--;
			dAverageX = ((double)lErrorSumX) / lAutoTestCount;
			dAverageY = ((double)lErrorSumY) / lAutoTestCount;
			lEminX = lErrorX[0];
			lEmaxX = lErrorX[0];
			lEminY = lErrorY[0];
			lEmaxY = lErrorY[0];

			for (j = 0; j < lAutoTestCount; j++)
			{
					
				lEminX = lEminX < lErrorX[j] ? lEminX : lErrorX[j];
				lEminY = lEminY < lErrorY[j] ? lEminY : lErrorY[j];
				lEmaxX = lEmaxX < lErrorX[j] ? lErrorX[j] : lEmaxX ;
				lEmaxY = lEmaxY < lErrorY[j] ? lErrorY[j] : lEmaxY ;

				sx += (lErrorX[j] - dAverageX) * (lErrorX[j] - dAverageX);
				sy += (lErrorY[j] - dAverageY) * (lErrorY[j] - dAverageY);
			}
			dStDevX = sqrt(sx / lAutoTestCount);
			dStDevY = sqrt(sy / lAutoTestCount);
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetInteger(lEminX);
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetInteger(lEmaxX);
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetInteger(lEminY);
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetInteger(lEmaxY);
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetDouble(dStDevX);
			m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum++, nCol++)->SetDouble(dStDevY);
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum, nCol++)->SetDouble(dAverageX);
			//m_pExcelCaliInfoSheet->Cell(m_lCaliInfoRowNum++, nCol++)->SetDouble(dAverageY);


			
		}		
#endif
	} while (0);

	if (!bOpDone)
	{
		m_pAppMod->HMIMessageBox(MSG_OK, "Calibration file fail", "Calibration file might be opened, please close it and try again.");
		bOpDone = fo.Delete(szExcelTemplateDestPath + szExcelTemplateFileName);
		if (pExcelDB != NULL)
		{
			delete pExcelDB;
		}
		return FALSE;
	}
	else if (!bResult)
	{
		pExcelDB->Save();
		pExcelDB->Close();
		m_pAppMod->HMIMessageBox(MSG_OK, "Calibration file Save", "Calibration file Save");
		m_pExcelCaliInfoSheet	= NULL;

		if (pExcelDB != NULL)
		{
			delete pExcelDB;
		}
		return FALSE;
	}
	else
	{
		if (pExcelDB->Save())
		{
			m_pAppMod->DisplayMessage("excel save ok");
		}
		pExcelDB->Close();
		m_pExcelCaliInfoSheet	= NULL;

		if (pExcelDB != NULL)
		{
			delete pExcelDB;
		}
		return TRUE;
	}
}
#endif