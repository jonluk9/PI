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

#pragma once

#include "PI9000Stn.h"
#include "math.h"
#include "prheader.h"
#include "PR_Util.h"
#include "Cal_Util.h"
#include "HouseKeeping.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef PRTASKSTN_EXTERN
extern CString	g_szWizardBMPath;
extern BOOL	g_bIsVisionNTInited;
#endif

typedef enum
{
	BASIC = 0,
	ADVANCE,
	MAX_NUM_OF_PB_METHOD,

} PB_Method;

typedef enum
{
	PR1,
	PR2,
	MAX_NUM_OF_PR,

} PRNum;


class CPRTaskStn
{
	//DECLARE_DYNCREATE(CPRTaskStn)
public:
	CString		m_szPRTaskStnName;
protected:
	CPI9000App *m_pAppMod;

	LONG		m_lPRStnErrorCode;		// IDS_xxxxx_NOT_SELECTED_ERR

	LONG		m_lPRModSelectMode;		

	BOOL		m_bPRSelected;
	BOOL		m_bPRError;

	BOOL		m_bPRCycle;
	ULONG		m_ulPRCycle;

	LONG		m_lAddJobPR_GRAB;
	LONG		m_lAddJobPR_PROCESS;

	LONG		m_lPRRetryCounter;
	LONG		m_lPRRetryLimit;
	LONG		m_lRejectGlassCounter;
	LONG		m_lRejectGlassLimit;
	LONG		m_lPRDelay;
	LONG		m_lPRDelay2;
	LONG		m_lCalibPRDelay;
	LONG		m_lPRErrorLimit;//20141003

#ifdef ENCODER_BASED //use encoder value instead of command motor count
	LONG	m_lPR1BeforeZEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK]; //before grab PR
	LONG	m_lPR2BeforeZEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];

	LONG	m_lPR1AfterZEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK]; //before grab PR
	LONG	m_lPR2AfterZEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];

	LONG	m_lPR1BeforeTEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK]; //before grab PR
	LONG	m_lPR2BeforeTEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];

	LONG	m_lPR1AfterTEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK]; //before grab PR
	LONG	m_lPR2AfterTEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];

	MTR_POSN	m_mtrCurrPR1BeforeEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK]; //before grab PR
	MTR_POSN	m_mtrCurrPR2BeforeEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];

	MTR_POSN	m_mtrCurrPR1AfterEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK]; //after grab PR
	MTR_POSN	m_mtrCurrPR2AfterEncPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];

	D_MTR_POSN	m_dmtrCurrPR1BeforePRPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];
	D_MTR_POSN	m_dmtrCurrPR1AfterPRPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];

	D_MTR_POSN	m_dmtrCurrPR2BeforePRPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];
	D_MTR_POSN	m_dmtrCurrPR2AfterPRPosn[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];
//////
	D_MTR_POSN	m_dmtrCurrAlign1OffsetBefore[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];						// current align1 offsets
	D_MTR_POSN	m_dmtrCurrAlign1OffsetAfter[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];						// current align1 offsets

	D_MTR_POSN	m_dmtrCurrAlign2OffsetBefore[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];						// current align2 offsets
	D_MTR_POSN	m_dmtrCurrAlign2OffsetAfter[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];						// current align2 offsets

	D_MTR_POSN	m_dmtrCurrAlignCentreBefore[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];						// current final die centre
	D_MTR_POSN	m_dmtrCurrAlignCentreAfter[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];						// current final die centre

	DOUBLE		m_dCurrAlignAngleBefore[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];							// current final die angle	
	DOUBLE		m_dCurrAlignAngleAfter[MAX_NUM_OF_GLASS][MAX_NUM_OF_OBJ_LOOK];							// current final die angle	
#endif

	D_MTR_POSN	m_dmtrCurrPR1Posn;
	D_MTR_POSN	m_dmtrCurrPR2Posn;
	D_MTR_POSN	m_dmtrCurrAlign1Offset;						// current align1 offsets
	D_MTR_POSN	m_dmtrCurrAlign2Offset;						// current align2 offsets
	D_MTR_POSN	m_dmtrCurrAlignCentre;						// current final die centre
	DOUBLE		m_dCurrAlignAngle;							// current final die angle	
	DOUBLE		m_dCurrPreciseAlginAngle;					// Record the Corrected angle in precise mode
	DOUBLE		m_dCurrAlignDist;							// distance btw align pts

	MTR_POSN	m_mtrCurrPRCalibPosn;
	LONG		m_lCurrPRCalibLevel;

	INT			m_nPRInProgress;

	//PostBond Only
	PR_COORD	m_PRCurrPBPR1Corner[PR_NO_OF_CORNERS];
	PR_COORD	m_PRCurrPBPR2Corner[PR_NO_OF_CORNERS];

	//Auto Calib parameters
	//PR_COORD	m_stPRCalibPts[NO_OF_CAL_PT];
	PR_RCOORD	m_stPRCalibPts[NO_OF_CAL_PT];
	MTR_POSN	m_stMtrCalibPts[NO_OF_CAL_PT];
	MTR_POSN	m_stCORMtrCalibPts[MAX_NUM_OF_TA4_COR][COR_ARRAY_SIZE];
	MTR_POSN	m_stCORMtrCalibPts2ndEnc[MAX_NUM_OF_TA4_COR][COR_ARRAY_SIZE];
	MTR_POSN	m_mtrThermalCalibRefPosn; //20140627 PBDL calib
	MTR_POSN	m_mtrThermalCalibOffset; //20140707 thermal compensation
#ifdef PB_THERMAL_CAL
	MTR_POSN	m_mtrCurrCalibGlassPosn;
	MTR_POSN	m_mtrThermalCalibAlignOffset; //20151009
#endif

	//// COR parameters
	D_MTR_POSN	m_dmtrTableCOR_Pos[MAX_NUM_OF_TA4_COR][COR_ARRAY_SIZE];
	D_MTR_POSN	m_dmtrTableCOR_Neg[MAX_NUM_OF_TA4_COR][COR_ARRAY_SIZE];
	DOUBLE		m_dCORLUT[MAX_NUM_OF_TA4_COR][COR_ARRAY_SIZE];
	BOOL		m_bCORCalibrated[MAX_NUM_OF_TA4_COR];
	DOUBLE		m_dCalibCORRange;
	LONG		m_lCORCamNum;

	ULONG		HMI_ulCurPRU;
	ULONG		HMI_ulPRID;
	DOUBLE		HMI_dPRMatchScore;
	DOUBLE		HMI_dPRAngleRange;
	DOUBLE		HMI_dPRPercentVar;
	BOOL		HMI_bShowPRSearch;
	DOUBLE		HMI_dPostBdRejectTol;
	ULONG		HMI_ulPRPostBdAttrib;
	LONG		HMI_lSegThreshold;
	LONG		HMI_lDefectThreshold;

	// Advance
	LONG		HMI_lPBMethod;		// 0: Basic; 1: Advance

	// Basic
	LONG		m_lIsAlign;			// 0: NO Align
	LONG		m_lAlignAlg;		// 0:Parallel; 1: Three Line

	BOOL		HMI_bShowSegThreshold;
	BOOL		HMI_bShowPBMethod;
	BOOL		HMI_bShowBasicAlg;
	BOOL		HMI_bShowAdvanceAlg;

	CMotorInfo	m_motorDummy; //klocwork fix 20121211
protected:


public:
	CPRTaskStn();
	virtual	~CPRTaskStn();

public:
	VOID SetPRSelected(BOOL bMode);
	BOOL IsPRSelected();
	VOID CheckPRModSelected(BOOL bMode);
	LONG GetPRModSelectMode();

private:
	LONG SleepWithCalibDelay();
	LONG GetPRDelay();
	LONG GetPRCalibDelay();

protected:
	BOOL ManualLrnDie_New(PRU *pPRU);	// Vision UI
	VOID SrchPR_New(PRU *pPRU);			// Vision UI
	VOID ModifyPRCriteria(PRU *pPRU);	
	BOOL ManualLrnDie(PRU *pPRU);
	VOID SrchPRDie(PRU *pPRU);
	VOID SetPRDieSrchArea(PRU *pPRU, PR_COORD coCorner1, PR_COORD coCorner2);
	BOOL LearnPRShape(PRU *pPRU);
	VOID DetectPRShape(PRU *pPRU);
	VOID SetPRShapeSrchArea(PRU *pPRU, PR_COORD coCorner1, PR_COORD coCorner2);
	BOOL LoadPRTmpl(PRU *pPRU);
	VOID SrchPRTmpl(PRU *pPRU);
	VOID SetPRTmplSrchArea(PRU *pPRU, PR_COORD coCorner1, PR_COORD coCorner2);
	BOOL LrnBackgroundCmd(PRU *pPRU, PRU *pPRUBackground);

	BOOL SearchPR(PRU *pPRU, BOOL bIsWait = TRUE);
	BOOL SearchPRGrabOnly(PRU *pPRU);
	BOOL SearchPRProcessOnly(PRU *pPRU);

public:
	BOOL SearchPRCentre(PRU *pPRU, BOOL bIsInvert = FALSE); //20120613
	VOID SinglePRCycleOperation(PRU *pPRU, DOUBLE dTestAng, DOUBLE dTestX, DOUBLE dTestY, PRINT_DATA *stPrintData);

protected:
	// Calc Related
	VOID CalDPosnAfterRotate(D_MTR_POSN dmtrTableCOR, MTR_POSN mtrCurPosn, DOUBLE dAngle, D_MTR_POSN *p_dmtrResultPosn);
	VOID CalPosnAfterRotate(MTR_POSN mtrCurPosn, DOUBLE dAngle, MTR_POSN *p_mtrResultPosn);

	BOOL CheckPRCentre(PRU *pPRU, BOOL bIsInvert = FALSE); //20120613
	BOOL AutoCalibration(PRU *pPRU);
	BOOL SetThermalCalibRefPosn(PRU *pPRU); //20140627 PBDL calib
	
	BOOL AutoTest(PRU *pPRU);  //20140923
	BOOL CORAutoTest(PRU *pPRU, LONG lAutoTestCount, BOOL bUseRotaryEnc = FALSE, BOOL bInvertAngle = FALSE);
	//BOOL ExcelTest(LONG lCount); //20141119


	BOOL CORCalibration(PRU *pPRU, DOUBLE dInitalStepAngle = 0.5, BOOL bUseRotaryEnc = FALSE, BOOL bInvertAngle = FALSE);
	BOOL FindCORResultRange(DOUBLE *pdMaxX, DOUBLE *pdMinX, DOUBLE *pdMaxY, DOUBLE *pdMinY);
	BOOL SetAllCORData(DOUBLE dXPosn, DOUBLE dYPosn);
	BOOL CheckCORCentre(PRU *pPRU, DOUBLE dTestAng, BOOL bUseRotaryEnc = FALSE, BOOL bInvertAngle = FALSE);
	BOOL GetPRCentreMap(PRU *pPRU);

	virtual BOOL AutoSearchPR1(PRU *pPRU, CString *pszMsgOut = NULL);
#ifdef ENCODER_BASED
	BOOL AutoSearchPR1GrabOnly(PRU *pPRU, LONG lGlass = 0, CamObjType emCamObjType = LOOK_GLASS_TYPE);
	BOOL AutoSearchPR1ProcessOnly(PRU *pPRU, CString *pszMsgOut = NULL, LONG lGlass = 0, CamObjType emCamObjType = LOOK_GLASS_TYPE);
#else
	BOOL AutoSearchPR1GrabOnly(PRU *pPRU);
	BOOL AutoSearchPR1ProcessOnly(PRU *pPRU, CString *pszMsgOut = NULL);
#endif
	virtual BOOL AutoSearchPR2(PRU *pPRU, CString *pszMsgOut = NULL); //20140414
#ifdef ENCODER_BASED
	BOOL AutoSearchPR2GrabOnly(PRU *pPRU, LONG lGlass = 0, CamObjType emCamObjType = LOOK_GLASS_TYPE);
	BOOL AutoSearchPR2ProcessOnly(PRU *pPRU, CString *pszMsgOut = NULL, LONG lGlass = 0, CamObjType emCamObjType = LOOK_GLASS_TYPE);
#else
	BOOL AutoSearchPR2GrabOnly(PRU *pPRU);
	BOOL AutoSearchPR2ProcessOnly(PRU *pPRU, CString *pszMsgOut = NULL);
#endif

	//BOOL AutoSearchPBPR1(PRU *pPRU);
	//BOOL AutoSearchPBPR1GrabOnly(PRU *pPRU);
	//BOOL AutoSearchPBPR1ProcessOnly(PRU *pPRU);
	//BOOL AutoSearchPBPR2(PRU *pPRU);
	//BOOL AutoSearchPBPR2GrabOnly(PRU *pPRU);
	//BOOL AutoSearchPBPR2ProcessOnly(PRU *pPRU);

public:
	virtual CMotorInfo &GetMotorX();
	virtual CMotorInfo &GetMotorY();
	virtual CMotorInfo &GetMotorZ();
	virtual CMotorInfo &GetMotorT();

	virtual INT		SyncX();
	virtual INT		SyncY();
	virtual INT		SyncZ();
	virtual INT		SyncT();

	virtual INT MoveZToPRCalibLevel(BOOL bWait = GMP_WAIT); //Only FOR COR Calib and COR Check
	virtual INT MoveXYToPRCalibPosn(BOOL bWait = GMP_WAIT); //Only FOR COR Calib and COR Check
	virtual INT MoveTToStandby(BOOL bWait = GMP_WAIT);

	virtual INT MoveZToPRLevel(BOOL bWait = GMP_WAIT);
	virtual INT MoveXYToCurrPR1Posn(BOOL bWait = GMP_WAIT);
	virtual INT MoveXYToCurrPR2Posn(BOOL bWait = GMP_WAIT);
	
	virtual INT MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait);
	virtual INT MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait);

	virtual INT MoveAbsoluteXY(LONG lCntX, LONG lCntY, BOOL bWait, BOOL bMoveToSaveX);
	virtual INT MoveRelativeXY(LONG lCntX, LONG lCntY, BOOL bWait, BOOL bMoveToSaveX);

	virtual INT MoveAbsoluteT(LONG lMtrCnt, BOOL bWait);
	virtual INT MoveRelativeT(LONG lMtrCnt, BOOL bWait);

	virtual PRU &SelectCurPRU(ULONG ulPRU);
	virtual PRU &SelectCurPBBackground(ULONG ulPRU);
		
protected:
	VOID UpdateHMIPRButton();
	VOID DisplaySrchArea(PRU *pPRU);
	VOID DisplayPBNonAlignSrchArea(PRU *pPRU);
	VOID DisplayPBSrchArea(PRU *pPRU, LONG lSearchPixel = 20);
	VOID DisplayNormalSrchArea(PRU *pPRU);

	// 20140729 Yip
	VOID SetAndLogPRParameter(const CString &szFunction, const CString &szParameter, LONG &lParameter, LONG lNewValue);
	VOID LogPRParameter(const CString &szStation, const CString &szFunction, const CString &szParameter, const CString &szOldValue, const CString &szNewValue);
	VOID LogPRParameter(const CString &szFunction, const CString &szParameter, const CString &szOldValue, const CString &szNewValue);

public:
	VOID PrintCalibSetup(FILE *fp);

protected:
	LONG HMI_SetPRMatchScore(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRAngRange(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRPercentVar(IPC_CServiceMessage &svMsg);

	// Only For PostBond
	LONG HMI_IncrSegThresholdLevel10(IPC_CServiceMessage &svMsg);
	LONG HMI_IncrSegThresholdLevel(IPC_CServiceMessage &svMsg);
	LONG HMI_DecrSegThresholdLevel10(IPC_CServiceMessage &svMsg);
	LONG HMI_DecrSegThresholdLevel(IPC_CServiceMessage &svMsg);
	LONG HMI_SetSegThreshold(IPC_CServiceMessage &svMsg);
	LONG HMI_SetDefectThreshold(IPC_CServiceMessage &svMsg);
	LONG HMI_SetDefectAttribute(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPostBdRejectTol(IPC_CServiceMessage &svMsg);
	
	LONG HMI_SetIsAlign(IPC_CServiceMessage &svMsg);
	LONG HMI_SetAlignAlg(IPC_CServiceMessage &svMsg);

	LONG HMI_ModifyPRCriteria(IPC_CServiceMessage &svMsg);	// Vision UI
	VOID MachineStartup_SrchPR(PRU *pPRU);	// 20140623 Yip
	LONG HMI_SrchPR(IPC_CServiceMessage &svMsg);

	LONG HMI_SetPRRetryLimit(IPC_CServiceMessage &svMsg);
	LONG HMI_SetRejectGlassLimit(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRDelay(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPRDelay2(IPC_CServiceMessage &svMsg);
	LONG HMI_SetCalibPRDelay(IPC_CServiceMessage &svMsg);
	

public:
	LONG IPC_SavePRTaskStnMachinePara();
	LONG IPC_LoadPRTaskStnMachinePara();
	LONG IPC_SavePRTaskStnDevicePara();
	LONG IPC_LoadPRTaskStnDevicePara();
	
	virtual LONG IPC_SaveMachineRecord();
	virtual LONG IPC_LoadMachineRecord();
	virtual LONG IPC_SaveDeviceRecord();
	virtual LONG IPC_LoadDeviceRecord();
#ifdef EXCEL_MACHINE_DEVICE_INFO
	virtual LONG printPRTaskStnMachinePara();
#endif


};