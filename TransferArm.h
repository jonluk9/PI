/////////////////////////////////////////////////////////////////
//	TransferArm.cpp : interface of the CTransferArm class
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

#ifdef TRANSFERARM_EXTERN
extern BOOL					g_bStopCycle;

extern AUTOMODE				AutoMode;
extern BOOL					g_bDiagMode;
extern LONG					g_lDiagnSteps;

extern DOUBLE				g_dGToGOffset; // Glass To Glass Offset

extern ULONG				g_ulUserGroup;

extern SETUP_POSN_XY		g_stSetupPosn[NUM_OF_POSN_XY];
extern SETUP_LEVEL_Z		g_stSetupLevel[NUM_OF_LEVEL_Z];
extern SETUP_PARA			g_stSetupPara[NUM_OF_PARA];

#endif

#include "WorkHolder.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif


class CTransferArm : public CPI9000Stn
{
	DECLARE_DYNCREATE(CTransferArm)
	friend class CInspWH;

protected:
	

protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	virtual VOID RegisterVariables();

public:
	CTransferArm();
	virtual	~CTransferArm();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:

	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------
	CMotorInfo	m_stMotorX;
	CMotorInfo	m_stMotorZ;
	
	CSiInfo		m_stXLmtSnr;
	CSiInfo		m_stZLmtSnr;
	CSiInfo		m_stZHomeSnr;
	CSiInfo		m_stGlass1VacSnr;
	CSiInfo		m_stGlass2VacSnr;

	//TA4&5
	CSiInfo		m_stExtendSnr;
	CSiInfo		m_stRetractSnr;
	CSiInfo		m_stCOF1VacSnr;
	CSiInfo		m_stCOF2VacSnr;

	CSoInfo		m_stCOF1VacSol;
	CSoInfo		m_stCOF2VacSol;

	//TA5
	CSiInfo		m_stLRejectVacSnr;
	CSiInfo		m_stRRejectVacSnr;
	CSiInfo		m_stRGlassDetectSnr;
	CSiInfo		m_stLGlassDetectSnr;

	CSoInfo		m_stGlass1VacSol;
	CSoInfo		m_stGlass2VacSol;
	CSoInfo		m_stWeakBlowSol;
	CSoInfo		m_stGlass2WeakBlowSol;

	// ------------------------------------------------------------------------
	// End of Motor SI/SO
	// ------------------------------------------------------------------------
	
	// ------------------------------------------------------------------------
	// End of CHECK Who Am I
	// ------------------------------------------------------------------------
	
	// Cycle Status
	LONG	m_lPickPlaceRetryCounter;
	LONG	m_lPickPlaceRetryLimit;
	DWORD	m_dwMachineTime;
	DWORD	m_dwStartTime;
	INT		m_nCycleCounter;
	DOUBLE	m_dCycleTime;
	DOUBLE	m_dTempCycleTime[10];
	GlassNum PPAction;
	MainbondType	m_lCurrTargetPosn;

public:
	BOOL	m_bIsVacError; //20121224 vac error
protected:

	// positions
	LONG	m_lStandbyPosn;
	LONG	m_lLeftSafetyPosn;
	LONG	m_lRightSafetyPosn;
	LONG	m_lPickGlassPosn;
	LONG	m_lPlaceGlassPosn;
	
	//Level
	LONG			m_lStandbyLevel;
	LONG			m_lPickLevel;
	LONG			m_lPickLevelOffset;
	LONG			m_lPlaceLevel;
	LONG			m_lPlaceLevelOffset;

	BOOL	m_bUseCOFVacuum;	// 20140619 Yip

	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	// Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	// All Virtual Function from WorkHolder.cpp

	// ------------------------------------------------------------------------
	// END Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	// I/Os
	BOOL IsVacSensorOn(GlassNum lGlassNum);
	INT SetVacSol(GlassNum lGlassNum, BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetWeakBlowSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetG2WeakBlowSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	BOOL IsCOF1_VacSensorOn();	// 20140619 Yip
	BOOL IsCOF2_VacSensorOn();	// 20140619 Yip
	INT SetCOF1_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetCOF2_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	BOOL IsGLASS1_VacSensorOn();
	BOOL IsGLASS2_VacSensorOn();
	INT SetGLASS1_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetGLASS2_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	// 20140619 Yip
	BOOL IsVacStateOn(GLASS_COF_VAC_STATE stVacState);
	BOOL IsVacStateOff(GLASS_COF_VAC_STATE stVacState);
	BOOL IsVacStateMatched(GLASS_COF_VAC_STATE stVacState);
	GLASS_COF_VAC_STATE GetGlass1_VacSensorOnState();
	GLASS_COF_VAC_STATE GetGlass2_VacSensorOnState();
	GLASS_COF_VAC_STATE GetGlassCOF1_VacSensorOnState();
	GLASS_COF_VAC_STATE GetGlassCOF2_VacSensorOnState();
	INT SetGlassCOF1_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetGlassCOF2_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	VOID SetDiagnSteps(ULONG ulSteps);

	GlassNum CalcPickStatus(BOOL bWHStatus[], BOOL bArmStatus[]);
	GlassNum CalcPlaceStatus(BOOL bWHStatus[], BOOL bArmStatus[]);
	INT CheckModulePower(); //20121219 arm crash

	// Add me for setup wizard later
	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------
	// Values are for HMI display only! Users should not use these in coding.

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------
	// Motor
	LONG HMI_PowerOnX(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnZ(IPC_CServiceMessage &svMsg);

	LONG HMI_CommX(IPC_CServiceMessage &svMsg);
	LONG HMI_CommZ(IPC_CServiceMessage &svMsg);
	
	LONG HMI_HomeX(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeZ(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleG1VacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleG2VacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleG1AuxVacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleG2AuxVacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleWeakBlowSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleG2WeakBlowSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleCOF1VacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleCOF2VacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleUseCOFVacuum(IPC_CServiceMessage &svMsg);

	// Setup Page Related
	LONG HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexXPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexXNeg(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZNeg(IPC_CServiceMessage &svMsg);

	LONG HMI_SetPickPlaceRetryLimit(IPC_CServiceMessage &svMsg);
	
	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();
#ifdef EXCEL_MACHINE_DEVICE_INFO
	virtual BOOL printMachineDeviceInfo();
#endif


};
