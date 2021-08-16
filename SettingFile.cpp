/////////////////////////////////////////////////////////////////
//	SettingFile.cpp : interface of the CSettingFile class
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

#define SETTING_EXTERN
#include "SettingFile.h"

#include "PICKARM.h"
#include "INPICKARM.h"
#include "OUTPICKARM.h"
#include "INSPWH.h"
#include "WH1.h"
#include "WH2.h"
#include "ShBufferWH.h"
#include "InspOpt.h"
#include "RejectArm.h"
#include "RejectBelt.h"
#include "WinEagle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSettingFile, CPI9000Stn)

CSettingFile::CSettingFile()
{
	HMI_bShowIgnoreWindow = FALSE;
	HMI_bShowIgnoreWindowBtn = FALSE;
	HMI_bShowIgnoreWindowNextBtn = FALSE;
}

CSettingFile::~CSettingFile()
{
}

BOOL CSettingFile::InitInstance()
{
	BOOL bTemp;
	bTemp = m_bModSelected;

	CPI9000Stn::InitInstance();
	
	bTemp = m_bModSelected;

	return TRUE;
}

INT CSettingFile::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CSettingFile::Operation()
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

VOID CSettingFile::UpdateOutput()
{
	if ((m_qState == UN_INITIALIZE_Q) || (m_qState == DE_INITIAL_Q) || (m_qState == AUTO_Q) || (m_qState == DEMO_Q))
	{
		return;
	}

	try
	{
		//Update sensor here
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

VOID CSettingFile::RegisterVariables()
{
	try
	{
		// -------------------------------------------------------
		// varibales
		// -------------------------------------------------------
		RegVariable(_T("SF_bWizardEnable"), &g_bWizardEnable);
		RegVariable(_T("SF_szWizardTitle"), &g_szWizardTitle);
		RegVariable(_T("SF_ulWizardSubTitle"), &g_ulWizardSubTitle);
		RegVariable(_T("SF_szWizardStep"), &g_szWizardStep);
		RegVariable(_T("SF_szWizardMess"), &g_szWizardMess);
		RegVariable(_T("SF_szWizardMessPrev"), &g_szWizardMessPrev);
		RegVariable(_T("SF_szWizardMessNext"), &g_szWizardMessNext);
		RegVariable(_T("SF_szWizardBMPath"), &g_szWizardBMPath);
		RegVariable(_T("SF_bWizardEnableBack"), &g_bWizardEnableBack);
		RegVariable(_T("SF_bWizardEnableNext"), &g_bWizardEnableNext);
		RegVariable(_T("SF_bWizardEnableExit"), &g_bWizardEnableExit);
		RegVariable(_T("SF_ulWizardStep"), &g_ulWizardStep);
		RegVariable(_T("SF_lWizardInProgress"), &g_lWizardInProgress);

		RegVariable(_T("SF_bShowIgnoreWindow"), &HMI_bShowIgnoreWindow); //20130319 PR ignore windows
		RegVariable(_T("SF_bShowIgnoreWindowBtn"), &HMI_bShowIgnoreWindowBtn);
		RegVariable(_T("SF_bShowIgnoreWindowNextBtn"), &HMI_bShowIgnoreWindowNextBtn);
		RegVariable(_T("SF_szWizardIgnoreWinTitle"), &g_szWizardIgnoreWinTitle);
		RegVariable(_T("SF_szWizardIgnoreWinSubtitle"), &g_szWizardIgnoreWinSubtitle);

		CString szMsg;
		for (INT i = 0; i < NUM_OF_POSN_XY; i++)
		{
			szMsg.Format("SF_szSetupPosn%ld", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPosn[i].szPosn);
			szMsg.Format("SF_bSetupPosn%ldEnable", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPosn[i].bEnable);
			szMsg.Format("SF_bSetupPosn%ldVisible", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPosn[i].bVisible);
			szMsg.Format("SF_lSetupPosn%ldType", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPosn[i].lType);
			szMsg.Format("SF_lSetupPosn%ldX", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPosn[i].lPosnX);
			szMsg.Format("SF_lSetupPosn%ldY", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPosn[i].lPosnY);

		}

		for (INT i = 0; i < NUM_OF_LEVEL_Z; i++)
		{
			szMsg.Format("SF_szSetupLevel%ld", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupLevel[i].szLevel);
			szMsg.Format("SF_bSetupLevel%ldEnable", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupLevel[i].bEnable);
			szMsg.Format("SF_bSetupLevel%ldVisible", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupLevel[i].bVisible);
			szMsg.Format("SF_lSetupLevel%ldType", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupLevel[i].lType);
			szMsg.Format("SF_lSetupLevel%ldZ", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupLevel[i].lLevelZ);
		}

		for (INT i = 0; i < NUM_OF_PARA; i++)
		{
			szMsg.Format("SF_szSetupPara%ld", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPara[i].szPara);
			szMsg.Format("SF_bSetupPara%ldEnable", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPara[i].bEnable);
			szMsg.Format("SF_bSetupPara%ldVisible", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPara[i].bVisible);
			szMsg.Format("SF_lSetupPara%ldType", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPara[i].lType);
			szMsg.Format("SF_dSetupPara%ldData", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPara[i].dPara);
			szMsg.Format("SF_lSetupPara%ldData", i + 1);
			RegVariable(_T(szMsg),			&g_stSetupPara[i].lPara);
		}

		RegVariable(_T("SF_lSetupInProgress"),		&g_lSetupInProgress);

		// -------------------------------------------------------
		// commands
		// -------------------------------------------------------

		m_comServer.IPC_REG_SERVICE_COMMAND("SF_WizardSetup", HMI_WizardSetup);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_WizardSetupBack", HMI_WizardSetupBack);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_WizardSetupNext", HMI_WizardSetupNext);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_WizardSetupExit", HMI_WizardSetupExit);

		m_comServer.IPC_REG_SERVICE_COMMAND("SF_ModuleSetupPreTask", HMI_ModuleSetupPreTask);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_ModuleSetupPostTask", HMI_ModuleSetupPostTask);

		m_comServer.IPC_REG_SERVICE_COMMAND("SF_ModuleSetupSetPosn", HMI_ModuleSetupSetPosn);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_ModuleSetupGoPosn", HMI_ModuleSetupGoPosn);

		m_comServer.IPC_REG_SERVICE_COMMAND("SF_ModuleSetupSetLevel", HMI_ModuleSetupSetLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_ModuleSetupGoLevel", HMI_ModuleSetupGoLevel);

		m_comServer.IPC_REG_SERVICE_COMMAND("SF_ModuleSetupSetPara", HMI_ModuleSetupSetPara);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_CalcWHZLinearEncoder", HMI_CalcWHZLinearEncoder);

		m_comServer.IPC_REG_SERVICE_COMMAND("SF_WizardSetupPRIgnoreWindows", HMI_WizardSetupPRIgnoreWindows); //20130319 PR ignore windows
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_WizardSetupPRIgnoreWindowsNext", HMI_WizardSetupPRIgnoreWindowsNext);
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_WizardSetupPRIgnoreWindowsExit", HMI_WizardSetupPRIgnoreWindowsExit);

#ifdef PB_THERMAL_CAL
		m_comServer.IPC_REG_SERVICE_COMMAND("SF_GoThermalCalibAp", HMI_GoThermalCalibAp);
#endif
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}


//-----------------------------------------------------------------------------
// HMI command implementation
//-----------------------------------------------------------------------------
//
LONG CSettingFile::HMI_WizardSetup(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
#ifdef PB_THERMAL_CAL
	PRU *pPRU = NULL;
#endif
	typedef struct
	{
		LONG lStation;
		ULONG ulSubTitle;
	} PARA;

	PARA stPara;

	stPara.lStation = 0;
	stPara.ulSubTitle = 0;

	svMsg.GetMsg(sizeof(PARA), &stPara);

	g_bWizardEnable = TRUE;
	SetHmiVariable("SF_bWizardEnable");

	g_lWizardInProgress = stPara.lStation;
	g_ulWizardSubTitle = stPara.ulSubTitle;
	g_ulWizardStep = 0;

	g_bWizardEnableBack = FALSE;
	g_bWizardEnableNext = FALSE;
	g_bWizardEnableExit = FALSE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

	switch (g_lWizardInProgress)
	{
	case SF_WIZARD_IDLE:
		g_szWizardTitle = "";
		DisplayMessage("Kill Sunny");
		break;

	case SF_WINEAGLE:
		g_szWizardTitle = "PR Calibration Setup Procedure";
		SetHmiVariable("SF_szWizardTitle");
		pCWinEagle->SetupWizard();
		break;
	
	case SF_PICKARM:
		break;

	case SF_INSPWH:
		pCWH1->SetupWizard();
		break;

	case SF_INSPOPT:
		pCInspOpt->SetupWizard();
		break;

	}

	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_WizardSetupBack(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	g_bWizardEnableBack = FALSE;
	g_bWizardEnableNext = FALSE;
	g_bWizardEnableExit = FALSE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

	if (g_ulWizardStep > 0)
	{
		g_ulWizardStep--;
	}

	switch (g_lWizardInProgress)
	{
	case SF_WIZARD_IDLE:
		g_szWizardTitle = "";
		DisplayMessage("Kill Sunny");
		break;

	case SF_WINEAGLE:
		g_szWizardTitle = "PR Calibration Setup Procedure";
		SetHmiVariable("SF_szWizardTitle");
		pCWinEagle->SetupWizard();
		break;
	
	case SF_PICKARM:
		break;

	case SF_INSPWH:
		pCWH1->SetupWizard();
		break;

	case SF_INSPOPT:
		pCInspOpt->SetupWizard();
		break;

	}

	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_WizardSetupNext(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	g_bWizardEnableBack = FALSE;
	g_bWizardEnableNext = FALSE;
	g_bWizardEnableExit = FALSE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

	g_ulWizardStep++;

	switch (g_lWizardInProgress)
	{
	case SF_WIZARD_IDLE:
		g_szWizardTitle = "";
		DisplayMessage("Kill Sunny");
		break;

	case SF_WINEAGLE:
		g_szWizardTitle = "PR Calibration Setup Procedure";
		SetHmiVariable("SF_szWizardTitle");
		pCWinEagle->SetupWizard();
		break;
	
	case SF_PICKARM:
		break;

	case SF_INSPWH:
		pCWH1->SetupWizard();
		break;

	case SF_INSPOPT:
		pCInspOpt->SetupWizard();
		break;
	}

	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_WizardSetupExit(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	g_bWizardEnableBack = FALSE;
	g_bWizardEnableNext = FALSE;
	g_bWizardEnableExit = FALSE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

	g_ulWizardStep = 99;

	switch (g_lWizardInProgress)
	{
	case SF_WIZARD_IDLE:
		g_szWizardTitle = "";
		DisplayMessage("Kill Sunny");
		break;

	case SF_WINEAGLE:
		g_szWizardTitle = "PR Calibration Setup Procedure";
		SetHmiVariable("SF_szWizardTitle");
		pCWinEagle->SetupWizard();
		break;
	
	case SF_PICKARM:
		break;

	case SF_INSPWH:
		pCWH1->SetupWizard();
		break;

	case SF_INSPOPT:
		pCInspOpt->SetupWizard();
		break;

	}

	pCWinEagle->HMI_bShowCursorControl	= FALSE; //20130823
	g_bWizardEnable			= FALSE;
	g_szWizardTitle			= "";
	g_ulWizardSubTitle		= 0;
	g_szWizardStep			= "";
	g_szWizardMess			= "";
	g_szWizardMessPrev		= "";
	g_szWizardMessNext		= "";
	g_szWizardBMPath		= "";
	g_bWizardEnableBack		= FALSE;
	g_bWizardEnableNext		= FALSE;
	g_bWizardEnableExit		= FALSE;
	g_ulWizardStep			= 0;
	g_lWizardInProgress		= -1;

	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_WizardSetupPRIgnoreWindows(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	PRU *pPRU = NULL;

	LONG lSetupInProgress = 0;
	svMsg.GetMsg(sizeof(LONG), &lSetupInProgress);

	HMI_bShowIgnoreWindow = TRUE;
	HMI_bShowIgnoreWindowNextBtn = TRUE;
	SetHmiVariable("SF_bShowIgnoreWindow");
	SetHmiVariable("SF_bShowIgnoreWindowNextBtn");

	g_ulWizardIgnoreWindowStep = 0;

	g_szWizardIgnoreWinTitle.Format("PR Ignore Windows Setup");
	g_szWizardIgnoreWinSubtitle.Format("Please Select Ignore Region #%ld", g_ulWizardIgnoreWindowStep + 1);

	//pCWinEagle->SetPRIgnoreWindowBegin();
	switch (/*lSetupInProgress*/g_lWizardInProgress)
	{
	case SF_WIZARD_IDLE:
		g_szWizardTitle = "";
		DisplayMessage("Kill Sunny");
		break;

	case SF_WINEAGLE:
		g_szWizardTitle = "PR Calibration Setup Procedure";
		SetHmiVariable("SF_szWizardTitle");
		pCWinEagle->SetupWizard();
		break;
	
	case SF_PICKARM:
		break;

	case SF_INSPWH:
		pCWH1->SetupWizard();
		break;

	case SF_INSPOPT:
		//pCInspOpt->SetupWizard();
		break;

	case SF_INSP_DL_PR:
		pPRU = &pCWH1->SelectCurPRU(pCWH1->HMI_ulCurPRU);
		if (pPRU != NULL)
		{
			pCWinEagle->SetPRIgnoreWindow(pPRU);
		}
		break;
	
	case SF_INSP_UL_PR:
		pPRU = &pCInspOpt->SelectCurPRU(pCInspOpt->HMI_ulCurPRU);
		if (pPRU != NULL)
		{
			pCWinEagle->SetPRIgnoreWindow(pPRU);
		}
		break;
	}
	return 0;
}

LONG CSettingFile::HMI_WizardSetupPRIgnoreWindowsNext(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	LONG lModuleInProgress = 0;
	PRU *pPRU = NULL;
	CString	szMsg = _T(" ");

	svMsg.GetMsg(sizeof(LONG), &lModuleInProgress);

	if (g_ulWizardIgnoreWindowStep > MAX_PR_IGNORE_WINDOWS - 1)
	{
		szMsg.Format("Error: Number of Ignore Windows exceed %d. Operation Abort!", MAX_PR_IGNORE_WINDOWS);
		DisplayMessage(szMsg);
		szMsg.Format("Number of Ignore Windows exceed %d. Operation Abort!", MAX_PR_IGNORE_WINDOWS);
		HMIMessageBox(MSG_OK, "WARNING", szMsg);
		return 0;
	}
	g_ulWizardIgnoreWindowStep++;
	//no more than 8 window
	if (g_ulWizardIgnoreWindowStep >= MAX_PR_IGNORE_WINDOWS)
	{
	//done
		HMI_bShowIgnoreWindowNextBtn = FALSE;
		SetHmiVariable("SF_bShowIgnoreWindowNextBtn");
		g_szWizardIgnoreWinSubtitle.Format("Max %ld Ignore Region Selected. Done", g_ulWizardIgnoreWindowStep);
	}
	else
	{
		g_szWizardIgnoreWinSubtitle.Format("Please Select Ignore Region #%ld", g_ulWizardIgnoreWindowStep + 1);
	}

	switch (/*lSetupInProgress*/g_lWizardInProgress)
	{
	case SF_WIZARD_IDLE:
		g_szWizardTitle = "";
		DisplayMessage("Kill Sunny");
		break;

	case SF_WINEAGLE:
		g_szWizardTitle = "PR Calibration Setup Procedure";
		SetHmiVariable("SF_szWizardTitle");
		pCWinEagle->SetupWizard();
		break;
	
	case SF_PICKARM:
		break;

	case SF_INSPWH:
		pCWH1->SetupWizard();
		break;

	case SF_INSPOPT:
//		pCInspOpt->SetupWizard();
		break;

	case SF_INSP_DL_PR:
		pPRU = &pCWH1->SelectCurPRU(pCWH1->HMI_ulCurPRU);
		if (pPRU != NULL)
		{
			pCWinEagle->SetPRIgnoreWindowNext(pPRU);
		}
		break;
	
	case SF_INSP_UL_PR:
		pPRU = &pCInspOpt->SelectCurPRU(pCInspOpt->HMI_ulCurPRU);
		if (pPRU != NULL)
		{
			pCWinEagle->SetPRIgnoreWindowNext(pPRU);
		}
		break;
	}
	return 0;
}

LONG CSettingFile::HMI_WizardSetupPRIgnoreWindowsExit(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	LONG lModuleInProgress = 0;
	PRU *pPRU = NULL;

	HMI_bShowIgnoreWindow = FALSE;
	HMI_bShowIgnoreWindowBtn = TRUE;
	HMI_bShowIgnoreWindowNextBtn = TRUE;
	svMsg.GetMsg(sizeof(LONG), &lModuleInProgress);

	switch (/*lSetupInProgress*/g_lWizardInProgress)
	{
	case SF_WIZARD_IDLE:
		g_szWizardTitle = "";
		DisplayMessage("Kill Sunny");
		break;

	case SF_WINEAGLE:
		g_szWizardTitle = "PR Calibration Setup Procedure";
		SetHmiVariable("SF_szWizardTitle");
		pCWinEagle->SetupWizard();
		break;
	
	case SF_PICKARM:
		break;

	case SF_INSPWH:
		pCWH1->SetupWizard();
		break;

	case SF_INSPOPT:
		pCInspOpt->SetupWizard();
		break;

	case SF_INSP_DL_PR:
		pPRU = &pCWH1->SelectCurPRU(pCWH1->HMI_ulCurPRU);
		if (pPRU != NULL)
		{
			pCWinEagle->SetPRIgnoreWindowFinish(pPRU);
		}
		break;
	
	case SF_INSP_UL_PR:
		pPRU = &pCInspOpt->SelectCurPRU(pCInspOpt->HMI_ulCurPRU);
		if (pPRU != NULL)
		{
			pCWinEagle->SetPRIgnoreWindowFinish(pPRU);
		}
		break;
	}
	return 0;
}

LONG CSettingFile::HMI_ModuleSetupPreTask(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CRejectArm *pCRejectArm = (CRejectArm*)m_pModule->GetStation("RejectArm");
	CRejectBelt *pCRejectBelt = (CRejectBelt*)m_pModule->GetStation("RejectBelt");

	PRU *pPRU = NULL;

	LONG lModuleInProgress = 0;
	BOOL bShowLighting = FALSE;
	INT	 nTmpCam = 0;

	svMsg.GetMsg(sizeof(LONG), &lModuleInProgress);

	g_lSetupInProgress = lModuleInProgress;

	switch (g_lSetupInProgress)
	{
	case SCREEN_IDLE:
		DisplayMessage("Kill JASON");
		break;

	case SCREEN_REJARM:
		pCRejectArm->ModuleSetupPreTask();
		break;

	case SCREEN_WINEAGLE:
		pCWinEagle->ModulePRSetupPreTask();
		break;

	case SCREEN_PICKARM:
		//PRS_DisplayVideo(&pruINSP_DL_Calib);
		//PRS_DrawHomeCursor(INSPDL_CAM, FALSE);	
		//pCWinEagle->UpdateLighting(&pruINSP_DL_Calib);

		pCPickArm->ModuleSetupPreTask();
		break;

	case SCREEN_INSPWH:
		PRS_DisplayVideo(&pruINSP_DL_Calib[GLASS1][WH_1]);
		PRS_DrawHomeCursor(INSPDL_CAM, FALSE);	
		pCWinEagle->UpdateLighting(&pruINSP_DL_Calib[GLASS1][WH_1]);

		pCWH1->ModuleSetupPreTask();
		break;

	case SCREEN_SHBUFFER_WH:
		//HMIMessageBox(MSG_OK, "WARNING", "SCREEN_ShBufferWH");
		pCShBufferWH->ModuleSetupPreTask();
		break;

	case SCREEN_REJBELT:
		pCRejectBelt->ModuleSetupPreTask();
		break;

	case SCREEN_INSPOPT:
		break;
	case SCREEN_INPICKARM:
		pCWH1->ModulePRSetupPreTask();
		pCWH2->ModuleSetupPreTask();
		pCInPickArm->ModuleSetupPreTask();
		break;

	case SCREEN_OUTPICKARM:
		pCOutPickArm->ModuleSetupPreTask();
		break;

	case SCREEN_INSPWH1:

		PRS_DisplayVideo(&pruINSP_DL_Calib[GLASS1][WH_1]);
		PRS_DrawHomeCursor(INSPDL_CAM, FALSE);	
		pCWinEagle->UpdateLighting(&pruINSP_DL_Calib[GLASS1][WH_1]);

		pCInspOpt->ModuleSetupPreTask();
		pCInPickArm->ModuleSetupPreTask();
		pCWH1->ModuleSetupPreTask();
		break;

	case SCREEN_INSPWH2:
		PRS_DisplayVideo(&pruINSP_DL_Calib[GLASS1][WH_2]);
		PRS_DrawHomeCursor(INSPDL_CAM, FALSE);	
		pCWinEagle->UpdateLighting(&pruINSP_DL_Calib[GLASS1][WH_2]);

		pCInspOpt->ModuleSetupPreTask();
		pCInPickArm->ModuleSetupPreTask();
		pCWH2->ModuleSetupPreTask();
		break;

	case SCREEN_INSPWH_DOWNLOOK:
		// Show PR Screen
		PRS_DisplayVideo(&pruINSP_DL_Calib[GLASS1][WH_1]);
		PRS_DrawHomeCursor(INSPDL_CAM, FALSE);	
		pCWinEagle->UpdateLighting(&pruINSP_DL_Calib[GLASS1][WH_1]);
	
		if (g_bEnableTrigLighting)
		{
			PRS_EnableHWTrigLighting(pruINSP_DL_Calib[GLASS1][WH_1].nCamera, TRUE);
		}

		pCWH1->ModulePRSetupPreTask();
		pCWH2->ModuleSetupPreTask();
		pCInPickArm->ModuleSetupPreTask();
		break;

	case SCREEN_INSP_UPLOOK:
		// Show PR Screen
		PRS_DisplayVideo(&pruINSPWH_UL_Calib[WH_1]);
		PRS_DrawHomeCursor(INSPUL_CAM, FALSE);	
		pCWinEagle->UpdateLighting(&pruINSPWH_UL_Calib[WH_1]);
	
		if (g_bEnableTrigLighting)
		{
			PRS_EnableHWTrigLighting(pruINSPWH_UL_Calib[WH_1].nCamera, TRUE);
		}

		pCWH1->ModulePRSetupPreTask();
		pCWH2->ModuleSetupPreTask();
		pCInspOpt->ModulePRSetupPreTask();
		break;
	}
	
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_ModuleSetupPostTask(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CRejectArm *pCRejectArm = (CRejectArm*)m_pModule->GetStation("RejectArm");
	CRejectBelt *pCRejectBelt = (CRejectBelt*)m_pModule->GetStation("RejectBelt");
	PRU *pPRU = NULL;

	LONG lModuleInProgress = 0;
	BOOL bShowLighting = FALSE;
	INT	 nTmpCam = 0;

	svMsg.GetMsg(sizeof(LONG), &lModuleInProgress);

	g_lSetupInProgress = lModuleInProgress;

	switch (g_lSetupInProgress)
	{
	case SCREEN_IDLE:
		DisplayMessage("Kill JASON");
		break;

	case SCREEN_REJARM:
		pCRejectArm->ModuleSetupPostTask();
		break;

	case SCREEN_WINEAGLE:
		pCWinEagle->ModulePRSetupPostTask();
		break;

	case SCREEN_PICKARM:
		pCPickArm->ModuleSetupPostTask();
		break;
	
	case SCREEN_INSPWH:
		pCWH1->ModuleSetupPostTask();
		break;

	case SCREEN_SHBUFFER_WH:
		pCShBufferWH->ModuleSetupPostTask();
		break;

	case SCREEN_REJBELT:
		pCRejectBelt->ModuleSetupPostTask();
		break;

	case SCREEN_INSPOPT:
		break;
	case SCREEN_INPICKARM:
		pCInPickArm->ModuleSetupPostTask();
		break;
	case SCREEN_OUTPICKARM:
		pCOutPickArm->ModuleSetupPostTask();
		break;
	case SCREEN_INSPWH1:
		pCWH1->ModuleSetupPostTask();
		break;
	case SCREEN_INSPWH2:
		pCWH2->ModuleSetupPostTask();
		break;

	case SCREEN_INSPWH_DOWNLOOK:
		pCWH1->ModulePRSetupPostTask();
		break;

	case SCREEN_INSP_UPLOOK:
		pCInspOpt->ModulePRSetupPostTask();
		break;
	}
	
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_ModuleSetupSetPosn(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CRejectBelt *pCRejectBelt = (CRejectBelt*)m_pModule->GetStation("RejectBelt");
	CRejectArm *pCRejectArm = (CRejectArm*)m_pModule->GetStation("RejectArm");

	LONG lSetupTitle = 0;
	BOOL bResult = TRUE;

	svMsg.GetMsg(sizeof(LONG), &lSetupTitle);
	
	switch (g_lSetupInProgress)
	{
	case SCREEN_IDLE:
		DisplayMessage("Kill JASON");
		break;

	case SCREEN_REJARM:
		pCRejectArm->ModuleSetupSetPosn(lSetupTitle);
		break;

	case SCREEN_WINEAGLE:
		break;

	case SCREEN_PICKARM:
		pCPickArm->ModuleSetupSetPosn(lSetupTitle);
		break;

	case SCREEN_INSPWH:
		pCInspWH->ModuleSetupSetPosn(lSetupTitle);
		break;

	case SCREEN_SHBUFFER_WH:
		break;

	case SCREEN_INSPOPT:
		break;
	case SCREEN_INPICKARM:
		pCInPickArm->ModuleSetupSetPosn(lSetupTitle);
		break;
	case SCREEN_OUTPICKARM:
		pCOutPickArm->ModuleSetupSetPosn(lSetupTitle);
		break;
	case SCREEN_INSPWH1:
		pCWH1->ModuleSetupSetPosn(lSetupTitle);
		break;
	case SCREEN_INSPWH2:
		pCWH2->ModuleSetupSetPosn(lSetupTitle);
		break;

	}
	
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_ModuleSetupGoPosn(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CRejectArm *pCRejectArm = (CRejectArm*)m_pModule->GetStation("RejectArm");

	LONG lSetupTitle = 0;
	BOOL bResult = TRUE;

	svMsg.GetMsg(sizeof(LONG), &lSetupTitle);

	switch (g_lSetupInProgress)
	{
	case SCREEN_IDLE:
		DisplayMessage("Kill JASON");
		break;

	case SCREEN_REJARM:
		pCRejectArm->ModuleSetupGoPosn(lSetupTitle);
		break;

	case SCREEN_WINEAGLE:
		break;

	case SCREEN_PICKARM:
		pCPickArm->ModuleSetupGoPosn(lSetupTitle);
		break;

	case SCREEN_INSPWH:
		pCInspWH->ModuleSetupGoPosn(lSetupTitle);	
		break;

	case SCREEN_SHBUFFER_WH:
		break;

	case SCREEN_INSPOPT:
		break;
	case SCREEN_INPICKARM:
		pCInPickArm->ModuleSetupGoPosn(lSetupTitle);
		break;
	case SCREEN_OUTPICKARM:
		pCOutPickArm->ModuleSetupGoPosn(lSetupTitle);
		break;
	case SCREEN_INSPWH1:
		pCWH1->ModuleSetupGoPosn(lSetupTitle);	
		break;
	case SCREEN_INSPWH2:
		pCWH2->ModuleSetupGoPosn(lSetupTitle);	
		break;
	}
	
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_ModuleSetupSetLevel(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CRejectArm *pCRejectArm = (CRejectArm*)m_pModule->GetStation("RejectArm");

	LONG lSetupTitle = 0;
	BOOL bResult = TRUE;

	svMsg.GetMsg(sizeof(LONG), &lSetupTitle);

	switch (g_lSetupInProgress)
	{
	case SCREEN_IDLE:
		DisplayMessage("Kill JASON");
		break;

	case SCREEN_REJARM:
		pCRejectArm->ModuleSetupSetLevel(lSetupTitle);
		break;

	case SCREEN_WINEAGLE:
		break;

	case SCREEN_PICKARM:
		break;

	case SCREEN_INSPWH:
		pCInspWH->ModuleSetupSetLevel(lSetupTitle);
		break;

	case SCREEN_SHBUFFER_WH:
		break;

	case SCREEN_INSPOPT:
		break;
	case SCREEN_INPICKARM:
		pCInPickArm->ModuleSetupSetLevel(lSetupTitle);
		break;
	case SCREEN_OUTPICKARM:
		pCOutPickArm->ModuleSetupSetLevel(lSetupTitle);
		break;
	case SCREEN_INSPWH1:
		pCWH1->ModuleSetupSetLevel(lSetupTitle);
		break;
	case SCREEN_INSPWH2:
		pCWH2->ModuleSetupSetLevel(lSetupTitle);
		break;

	}
	
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_ModuleSetupGoLevel(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CRejectArm *pCRejectArm = (CRejectArm*)m_pModule->GetStation("RejectArm");

	LONG lSetupTitle = 0;
	BOOL bResult = TRUE;

	svMsg.GetMsg(sizeof(LONG), &lSetupTitle);

	switch (g_lSetupInProgress)
	{
	case SCREEN_IDLE:
		DisplayMessage("Kill JASON");
		break;

	case SCREEN_REJARM:
		pCRejectArm->ModuleSetupGoLevel(lSetupTitle);
		break;

	case SCREEN_WINEAGLE:
		break;

	case SCREEN_PICKARM:
		break;

	case SCREEN_INSPWH:
		pCInspWH->ModuleSetupGoLevel(lSetupTitle);
		break;

	case SCREEN_SHBUFFER_WH:
		break;
	case SCREEN_INSPOPT:
		break;
	case SCREEN_INPICKARM:
		pCInPickArm->ModuleSetupGoLevel(lSetupTitle);
		break;
	case SCREEN_OUTPICKARM:
		pCOutPickArm->ModuleSetupGoLevel(lSetupTitle);
		break;
	case SCREEN_INSPWH1:
		pCWH1->ModuleSetupGoLevel(lSetupTitle);
		break;
	case SCREEN_INSPWH2:
		pCWH2->ModuleSetupGoLevel(lSetupTitle);
		break;

	}
	
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_ModuleSetupSetPara(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CRejectArm *pCRejectArm = (CRejectArm*)m_pModule->GetStation("RejectArm");

	PARA stPara;

	stPara.lSetupTitle = 0;
	stPara.lPara = 0;
	stPara.dPara = 0;

	LONG lSetupTitle = 0;
	
	CString szDesiredStnName;
	INT nDesiredStn = 0;

	svMsg.GetMsg(sizeof(PARA), &stPara);

	switch (g_lSetupInProgress)
	{
	case SCREEN_IDLE:
		DisplayMessage("Kill JASON");
		break;

	case SCREEN_REJARM:
		pCRejectArm->ModuleSetupSetPara(stPara);
		break;

	case SCREEN_WINEAGLE:
		break;

	case SCREEN_PICKARM:
		pCPickArm->ModuleSetupSetPara(stPara);
		break;

	case SCREEN_INSPWH:
		pCInspWH->ModuleSetupSetPara(stPara);
		break;

	case SCREEN_SHBUFFER_WH:
		pCShBufferWH->ModuleSetupSetPara(stPara);
		break;


	case SCREEN_INSPOPT:
		break;
	case SCREEN_INPICKARM:
		pCInPickArm->ModuleSetupSetPara(stPara);
		break;
	case SCREEN_OUTPICKARM:
		pCOutPickArm->ModuleSetupSetPara(stPara);
		break;
	case SCREEN_INSPWH1:
		pCWH1->ModuleSetupSetPara(stPara);
		break;
	case SCREEN_INSPWH2:
		pCWH2->ModuleSetupSetPara(stPara);
		break;

	}
	
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CSettingFile::HMI_CalcWHZLinearEncoder(IPC_CServiceMessage &svMsg) //whz rotary linear calib
{	
	CWinEagle *pCWinEagle	= (CWinEagle*)m_pModule->GetStation("WinEagle");
	CPickArm *pCPickArm	= (CPickArm*)m_pModule->GetStation("PickArm"); 
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm"); 
	COutPickArm *pCOutPickArm	= (COutPickArm*)m_pModule->GetStation("OutPickArm"); 
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1"); 
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2"); 
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH"); 
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	LONG	lMsgLen = svMsg.GetMsgLen();
	UCHAR	*cName = NULL;
	CString szMsg;

	if (lMsgLen > 0)
	{
		cName = new UCHAR[lMsgLen];
		svMsg.GetMsg(cName, lMsgLen);
	}
	
	if (pAppMod->m_CurrMotor == "INSPWH_Z1")
	{
		pCInspWH->CalcWHZLinearEncoder();
		DisplayMessage("INSPWH_Z1 Cal Completed!!");
	}
	else if (pAppMod->m_CurrMotor == "INSPWH_Z2")
	{
		pCInspWH->CalcWHZLinearEncoder();
		DisplayMessage("INSPWH_Z2 Cal Completed!!");
	}
	else
	{
		szMsg.Format("%s Module Not Found. Operation Abort!", cName == NULL ? "NULL" : (char*)cName);
		HMIMessageBox(MSG_OK, "CALC WHZ LINEAR ENCODER", szMsg);
	}

	if (cName != NULL)
	{
		delete[] cName;
	}
	BOOL bMode = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}