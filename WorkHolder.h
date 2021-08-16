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

#pragma once

//#include "PRTaskStn.h"
#include "PI9000Stn.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef WORKHOLDER_EXTERN

extern BOOL					g_bDiagMode;
extern LONG					g_lDiagnSteps;

#endif

class CWorkHolder : public CPI9000Stn
{
	DECLARE_DYNCREATE(CWorkHolder)
	friend class CPickArm;
	friend class CInPickArm;
	friend class COutPickArm;

protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	virtual VOID RegisterVariables();

public:
	CWorkHolder();
	virtual	~CWorkHolder();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------
	CMotorInfo	m_stMotorY;
	CMotorInfo	m_stMotorT;
	CMotorInfo	m_stMotorZ[MAX_NUM_OF_WHZ];

	CMotorInfo	m_stMotorX;
	CMotorInfo	m_stMotorX2;
	CMotorInfo	m_stMotorY2;

	CSiInfo		m_stGlass1VacSnr;
	CSiInfo		m_stGlass2VacSnr;
	CSiInfo		m_stGlass1AuxVacSnr;
	CSiInfo		m_stGlass2AuxVacSnr;
	CSiInfo		m_stXLmtSnr;
	CSiInfo		m_stYLmtSnr;
	CSiInfo		m_stSmallXLmtSnr;
	CSiInfo		m_stSmallYLmtSnr;
	CSiInfo		m_stXHomeSnr;
	CSiInfo		m_stYHomeSnr;
	CSiInfo		m_stZ1HomeSnr;
	CSiInfo		m_stZ2HomeSnr;
	CSiInfo		m_stZ1LmtSnr;
	CSiInfo		m_stZ2LmtSnr;
	CSiInfo		m_stTLmtSnr;

	CSoInfo		m_stGlass1VacSol;
	CSoInfo		m_stGlass2VacSol;
	CSoInfo		m_stGlass1AuxVacSol;
	CSoInfo		m_stGlass2AuxVacSol;
	CSoInfo		m_stWeakBlowSol;
	CSoInfo		m_stWeakBlow2Sol;
	// ------------------------------------------------------------------------
	// End of Motor SI/SO
	// ------------------------------------------------------------------------
	DWORD		m_dwStartTime;
	DOUBLE		m_dProcessTime;

	BOOL		m_bZOffDuringBond;
	BOOL		m_bOffWHVacuum;		
	LONG		m_lStaticControlMode;		// 0: Set Ki == 0; 1: Original
	LONG		m_lCurStaticControl[MAX_NUM_OF_WHZ];		// 0: -> Ki == 0; 1 -> Original

	// positions
	MTR_POSN	m_mtrStandbyPosn;
	MTR_POSN	m_mtrLoadGlassPosn;
	MTR_POSN	m_mtrLoadGlassOffset; //20131111
	MTR_POSN	m_mtrUnloadGlassPosn;
	MTR_POSN	m_mtrApproxBondPosn;

	//MTR_POSN	m_mtrLoadGlassOffset;
	MTR_POSN	m_mtrUnloadGlassOffset;

	LONG		m_lStandbyPosnT;

	// levels
	LONG		m_lStandbyLevel[MAX_NUM_OF_WHZ]; // WH_1 = 0; WH_2 = 1;
	LONG		m_lCalibStageLevel[MAX_NUM_OF_WHZ];
	LONG		m_lCalibLoadLevel[MAX_NUM_OF_WHZ];
	LONG		m_lCalibUnloadLevel[MAX_NUM_OF_WHZ];
	LONG		m_lPRCalibLevel[MAX_NUM_OF_WHZ];
	LONG		m_lApproxBondLevel[MAX_NUM_OF_WHZ];

	LONG		m_lRotatery2LinearZ1;
	LONG		m_lRotatery2LinearZ2;

	LONG		m_lLoadLevelOffset[MAX_NUM_OF_WHZ];
	LONG		m_lUnloadLevelOffset[MAX_NUM_OF_WHZ];
	LONG		m_lRejectLevel[MAX_NUM_OF_WHZ];
	LONG		m_lRejectLevelOffset[MAX_NUM_OF_WHZ];
	LONG		m_lPRLevelOffset[MAX_NUM_OF_WHZ];

	BOOL		m_bUseAnvilVacuum;	

	// Measurement Routine
	MTR_POSN	m_mtrMeasureSetZeroPt;
	DOUBLE		m_dMeasuredDistance;
	DOUBLE		m_dMeasuredAngle;
	BOOL		m_bEnableMeasure;

	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------
	
	// ------------------------------------------------------------------------
	// Virtual PRTaskStn Function delcaration
	// ------------------------------------------------------------------------
	// All Virtual Function from PRTaskStn.cpp
	//virtual INT MoveZToPRCalibLevel(BOOL bWait = GMP_WAIT);
	//virtual INT MoveXYToPRCalibPosn(BOOL bWait = GMP_WAIT);
	//virtual INT MoveTToStandby(BOOL bWait = GMP_WAIT);

	//virtual INT MoveZToPRLevel(BOOL bWait = GMP_WAIT);
	//virtual INT MoveXYToCurrPR1Posn(BOOL bWait = GMP_WAIT);
	//virtual INT MoveXYToCurrPR2Posn(BOOL bWait = GMP_WAIT);

	//virtual PRU& SelectCurPRU(ULONG ulPRU);
	
	// ------------------------------------------------------------------------
	// END Virtual PRTaskStn Function delcaration
	// ------------------------------------------------------------------------
	
	// ------------------------------------------------------------------------
	// Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	virtual VOID SetDiagnSteps(ULONG ulSteps, BOOL bFineStep = FALSE);
	virtual BOOL PickGlassOperation(GlassNum lGlassNum);
	virtual INT IndexGlassToTable(GlassNum lGlassNum, BOOL bTeach = TRUE);

	// ------------------------------------------------------------------------
	// END Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	INT SetZControlPara(WHZNum lWHZNum);
	INT SetZ1ControlPara(LONG lAbsoluteCnt, BOOL bWait = GMP_WAIT);
	INT SetZ2ControlPara(LONG lAbsoluteCnt, BOOL bWait = GMP_WAIT);
	INT ResetZControlPara(WHZNum lWHZNum);
	INT ResetZ1ControlPara(LONG lAbsoluteCnt, BOOL bWait = GMP_WAIT);
	INT ResetZ2ControlPara(LONG lAbsoluteCnt, BOOL bWait = GMP_WAIT);

	BOOL IsVacSensorOn(GlassNum lGlassNum);
	INT SetVacSol(GlassNum lGlassNum, BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetWeakBlowSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetWeakBlow2Sol(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	BOOL IsGLASS1_VacSensorOn();
	BOOL IsGLASS2_VacSensorOn();
	INT SetGLASS1_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetGLASS2_VacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	BOOL IsGLASS1_AuxVacSensorOn();
	BOOL IsGLASS2_AuxVacSensorOn();
	INT SetGLASS1_AuxVacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetGLASS2_AuxVacSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	
	WHZ_DATA	m_stWHCalibZ[MAX_NUM_OF_WHZ][MAX_NUM_WHZ_CAL_STEP]; //whz rotary linear calib
	// Add me for setup wizard later

	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------
	// Values are for HMI display only! Users should not use these in coding.	

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------
	// Motor
	LONG HMI_PowerOnY(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnT(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnZ1(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnZ2(IPC_CServiceMessage &svMsg);

	LONG HMI_PowerOnX1(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnX2(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnY1(IPC_CServiceMessage &svMsg);
	LONG HMI_PowerOnY2(IPC_CServiceMessage &svMsg);
//
	LONG HMI_CommY(IPC_CServiceMessage &svMsg);
	LONG HMI_CommT(IPC_CServiceMessage &svMsg);
	LONG HMI_CommZ1(IPC_CServiceMessage &svMsg);
	LONG HMI_CommZ2(IPC_CServiceMessage &svMsg);

	LONG HMI_CommX1(IPC_CServiceMessage &svMsg);
	LONG HMI_CommX2(IPC_CServiceMessage &svMsg);
	LONG HMI_CommY1(IPC_CServiceMessage &svMsg);
	LONG HMI_CommY2(IPC_CServiceMessage &svMsg);
//
	LONG HMI_HomeY(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeT(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeZ1(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeZ2(IPC_CServiceMessage &svMsg);

	LONG HMI_HomeX1(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeX2(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeY1(IPC_CServiceMessage &svMsg);
	LONG HMI_HomeY2(IPC_CServiceMessage &svMsg);


	LONG HMI_ToggleG1VacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleG2VacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleG1AuxVacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleG2AuxVacSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleWeakBlowSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleWeakBlow2Sol(IPC_CServiceMessage &svMsg);

	// Setup Page Related
	LONG HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexX1Pos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexX1Neg(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexX2Pos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexX2Neg(IPC_CServiceMessage &svMsg);

	LONG HMI_IndexYPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexYNeg(IPC_CServiceMessage &svMsg);

	LONG HMI_IndexY1Pos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexY1Neg(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexY2Pos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexY2Neg(IPC_CServiceMessage &svMsg);

	LONG HMI_IndexZ1Pos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZ1Neg(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZ2Pos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexZ2Neg(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexTPos(IPC_CServiceMessage &svMsg);
	LONG HMI_IndexTNeg(IPC_CServiceMessage &svMsg);

	LONG HMI_GoZ1PosLmt(IPC_CServiceMessage &svMsg);
	LONG HMI_GoZ1NegLmt(IPC_CServiceMessage &svMsg);
	LONG HMI_GoZ2PosLmt(IPC_CServiceMessage &svMsg);
	LONG HMI_GoZ2NegLmt(IPC_CServiceMessage &svMsg);

	LONG HMI_SetZStaticControlMode(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleNotUseAnvilVacuum(IPC_CServiceMessage &svMsg);

	BOOL CalcWHZLinearEncoder(); //whz rotary linear calib
	virtual CMotorInfo &GetMotorZ(WHZNum lWHZ = WHZ_NONE); //whz rotary linear calib
	VOID PrintCalibDataToHMI(); //whz rotary linear calib

	// Saving / loading parameters
	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();
#ifdef EXCEL_MACHINE_DEVICE_INFO
	virtual BOOL printMachineDeviceInfo();
#endif

};
