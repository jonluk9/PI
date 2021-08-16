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

#pragma once

#include "PI9000Stn.h"

#ifdef SETTING_EXTERN

extern LONG				g_lDiagnSteps;
extern BOOL				g_bEnableTrigLighting;

	// wizard
extern BOOL				g_bWizardEnable;
extern CString			g_szWizardTitle;
extern ULONG			g_ulWizardSubTitle;
extern CString			g_szWizardStep;
extern CString			g_szWizardMess;
extern CString			g_szWizardMessPrev;
extern CString			g_szWizardMessNext;
extern CString			g_szWizardBMPath;
extern BOOL				g_bWizardEnableBack;
extern BOOL				g_bWizardEnableNext;
extern BOOL				g_bWizardEnableExit;
extern ULONG			g_ulWizardStep;
extern LONG				g_lWizardInProgress;

extern ULONG			g_ulUserGroup;
extern SETUP_POSN_XY	g_stSetupPosn[NUM_OF_POSN_XY];
extern SETUP_LEVEL_Z	g_stSetupLevel[NUM_OF_LEVEL_Z];
extern SETUP_PARA		g_stSetupPara[NUM_OF_PARA];
extern LONG				g_lSetupInProgress;

extern ULONG			g_ulWizardIgnoreWindowStep; //20130319 PR ignore windows
extern CString			g_szWizardIgnoreWinTitle;
extern CString			g_szWizardIgnoreWinSubtitle;

#endif

class CSettingFile : public CPI9000Stn
{
	DECLARE_DYNCREATE(CSettingFile)

protected:
	

protected:
	//Update
	VOID UpdateOutput();

	virtual VOID RegisterVariables();

public:
	BOOL HMI_bShowIgnoreWindow; //20130319 PR ignore windows
	BOOL HMI_bShowIgnoreWindowBtn;
	BOOL HMI_bShowIgnoreWindowNextBtn;

	CSettingFile();
	virtual	~CSettingFile();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------

protected:
	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------


	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------

	LONG HMI_WizardSetup(IPC_CServiceMessage &svMsg);
	LONG HMI_WizardSetupBack(IPC_CServiceMessage &svMsg);
	LONG HMI_WizardSetupNext(IPC_CServiceMessage &svMsg);
	LONG HMI_WizardSetupExit(IPC_CServiceMessage &svMsg);
#ifdef PB_THERMAL_CAL
	LONG HMI_GoThermalCalibAp(IPC_CServiceMessage &svMsg);
#endif

	LONG HMI_ModuleSetupPreTask(IPC_CServiceMessage &svMsg);
	LONG HMI_ModuleSetupPostTask(IPC_CServiceMessage &svMsg);

	LONG HMI_ModuleSetupSetPosn(IPC_CServiceMessage &svMsg);
	LONG HMI_ModuleSetupGoPosn(IPC_CServiceMessage &svMsg);

	LONG HMI_ModuleSetupSetLevel(IPC_CServiceMessage &svMsg);
	LONG HMI_ModuleSetupGoLevel(IPC_CServiceMessage &svMsg);

	LONG HMI_ModuleSetupSetPara(IPC_CServiceMessage &svMsg);
	LONG HMI_CalcWHZLinearEncoder(IPC_CServiceMessage &svMsg); //whz rotary linear calib

	LONG HMI_WizardSetupPRIgnoreWindows(IPC_CServiceMessage &svMsg);
	LONG HMI_WizardSetupPRIgnoreWindowsNext(IPC_CServiceMessage &svMsg);
	LONG HMI_WizardSetupPRIgnoreWindowsExit(IPC_CServiceMessage &svMsg);

};