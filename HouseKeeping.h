/////////////////////////////////////////////////////////////////
//	CHouseKeeping.cpp : interface of the CHouseKeeping class
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
#include "Package_Util.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef HOUSEKEEPING_EXTERN
extern BOOL					g_bStopCycle;

extern AUTOMODE				AutoMode;
extern BOOL					g_bDiagMode;
extern LONG					g_lDiagnSteps;
extern BOOL					g_bSelectedReworkMode;

extern BOOL					g_bShowForceInNewton;

extern GLASS_INFO			g_stGlass;

extern BOOL					g_bSafeIndexing;
extern BOOL					g_bSlowMotionIndexing;
extern ULONG				g_ulSlowMotionFactor;
extern FORCE_CALIBRATOR		g_stForceCalibrator;
		
extern BOOL					g_bEnableTrigLighting;

extern ULONG				g_ulUserGroup;
extern BOOL					g_bUserGroupOperator; //20150114
extern LONG					g_lGlassInputMode; //20120412
extern LONG					g_lErrorCounter;

extern ULONG					g_lCalAverageFreq;

// 20140815 Yip
extern BOOL					g_bShowUserSearchPRDone;
extern CString				g_szBondingMsg;

//GATEWAY_STATUS_TYPE PI9000_Machine_Status					= ST_GW_MACHINE_STOP;

#endif


typedef struct
{
	CString			szName;

	BOOL			bValid;				// TRUE when PreBond PR learnt && alignment pt calibrated
	DOUBLE			dLength;
	DOUBLE			dWidth;
	DOUBLE			dThickness;
	
	// Bond Pt Related
	D_MTR_POSN		dmtrAlignPt1;
	D_MTR_POSN		dmtrAlignPt2;
	BOOL			bAlignPtSet;		// TRUE when FPC Alignment pts are set
	PRU				*pPBPRU1;
	PRU				*pPBPRU2;

	// Pick Pt Related
	D_MTR_POSN		dmtrAlignPickPt1;
	D_MTR_POSN		dmtrAlignPickPt2;
	BOOL			bAlignPickPtSet;

	OFFSET			stPickOffset;
	PRU				*pFPCPRU1;
	PRU				*pFPCPRU2;

	// ChipPickArm
	D_MTR_POSN		dmtrAlignFPCPt1;
	D_MTR_POSN		dmtrAlignFPCPt2;
	BOOL			bAlignFPCPtSet;

	PRU				*pFPCUPLOOKPRU1;
	PRU				*pFPCUPLOOKPRU2;


} FPC_TYPE_INFO;

class CHouseKeeping : public CPI9000Stn
{
	DECLARE_DYNCREATE(CHouseKeeping)
	friend class CPI9000App;
	friend class CPI9000Stn; //20130121
	friend class CInspOpt;
	friend class CInspWH;
	friend class CWorkHolder;
	//friend class CRejectArm;
	friend class CRejectBelt;
	friend class CPRTaskStn;
	friend class CShBufferWH;
	friend class CInPickArm;

protected:
	

protected:
	//Update
	VOID UpdateOutput();
	VOID UpdateProfile();
	VOID UpdatePosition();

	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();
	virtual VOID StopOperation();

public:
	CHouseKeeping();
	virtual	~CHouseKeeping();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------
	DOUBLE	m_dPrevSWVersion;
	DOUBLE  m_dCurSWVersion;

	// Add NuMotion 
	// 
	CSiInfo		m_stHTPwrOnSnr;
	CSiInfo		m_stPwrOFF;
	CSiInfo		m_stAirSupplySnr;	//20120703
	CSiInfo		m_stMBAirSupplySnr;
	CSiInfo		m_stEMODetectSnr;				// Hit EMO and disconnect with HiPEC
	CSiInfo		m_stMainPowerFaultSnr;			// 3 Phase Power Fault

	CSiInfo		m_stFrontDoorLockSnr;
	CSiInfo		m_stFrontDoorLockSnr2;
	CSiInfo		m_stBackDoorLockSnr;
	CSiInfo		m_stLowerBackDoorSnr01NC;
	CSiInfo		m_stLowerBackDoorSnr01NO;
	CSiInfo		m_stLowerBackDoorSnr02NC;
	CSiInfo		m_stLowerBackDoorSnr02NO;
	CSiInfo		m_stLowerDoorSnr01NC;
	CSiInfo		m_stLowerDoorSnr01NO;
	CSiInfo		m_stLowerDoorSnr02NC;
	CSiInfo		m_stLowerDoorSnr02NO;
	CSiInfo		m_stIonizerMaintenanceResult;
	CSiInfo		m_stIonizerError;

	CSiInfo		m_stFH1Snr;
	CSiInfo		m_stFH2Snr;
	CSiInfo		m_stFL1Snr;
	CSiInfo		m_stFL2Snr;
	CSiInfo		m_stSPSnr;

	CSiInfo		m_stStartButtonSnr;
	CSiInfo		m_stStopButtonSnr;
	CSiInfo		m_stResetButtonSnr;

		
	CSiInfo		m_stShBufPanelOutBtnSnr;	// Panel 
	CSiInfo		m_stShBufEmptyWhOutBtnSnr;
	CSiInfo		m_stShBufVac1BtnSnr;
	CSiInfo		m_stShBufVac2BtnSnr;
	CSiInfo		m_stInspLDoorSnrNO;
	CSiInfo		m_stInspLDoorSnrNC;
	CSiInfo		m_stInspRDoorSnrNO;
	CSiInfo		m_stInspRDoorSnrNC;

	CSoInfo		m_stShBufDoorLockSol;
	CSoInfo		m_stFrontDoorLockSol;
	CSoInfo		m_stFrontDoorLockSol2;
	CSoInfo		m_stBackDoorLockSol;
	CSoInfo		m_stStartButtonSol;
	CSoInfo		m_stResetButtonSol;

	CSoInfo		m_stRedLight;
	CSoInfo		m_stOrangeLight;
	CSoInfo		m_stGreenLight;
	CSoInfo		m_stBuzzer;

	CSoInfo		m_stShBufPanelOutBtnLamp;
	CSoInfo		m_stShBufEmptyWhOutBtnLamp;

	CSoInfo		m_stIonizerDischargeEnable;
	CSoInfo		m_stIonizerMaintenanceCheck;

	//Old Sol
	CSoInfo		m_stResetCounterSol;
	
	//Old Sol

	// ------------------------------------------------------------------------
	// End of Motor SI/SO
	// ------------------------------------------------------------------------

	BOOL	m_bErrorExist;
	BOOL	m_bAlertExist;
	BOOL	m_bBeepSound; //20140704
	CString	m_szLastError;	// 20140814 Yip

	BOOL	m_bPanelOutReq;
	BOOL	m_bEmptyOutReq;
	LampStatus m_nPanelOutLampStatus;
	LampStatus m_nEmptyOutLampStatus;

	ULONG	m_ulDetectCover;
	BOOL	m_bDetectCoverSlowMotion;
	BOOL	m_bDetectCoverStopOp;
	BOOL	m_bDetectAirSupply;
	BOOL	m_bDetectLightCurtainSnr;
	BOOL	m_bDetectEMO;
	BOOL	m_bDetectMainPowerFault;
		
	BOOL	m_bShBufDoorClose;
	LONG	m_lAirErrorCounter;

	BOOL	m_bDisableSelectAll;	// 20150515

#if 1 //20121009 pin detection spring test
	BOOL			m_bPinTest;
#endif
	BOOL HMI_bStopProcess; //20121225
	BOOL HMI_bChangeTeflon;	// 20140911 Yip

	BOOL	m_bNewCoverSnr;	// 20150714
	
	BOOL	m_bEnableStartBtn;
	BOOL	m_bEnableResetBtn;
	BOOL	m_bEnableStopBtn;
	BOOL	m_bPressBtnToStop;
	BOOL	m_bStopMoveTest; //20171206
	BOOL	m_bStopSearchDLnULTest; //20171206
	// Pakage Information

	GLASS_COF_ID m_stDeviceID; //20170406
	
	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------
	VOID OperationSequence();

	VOID SetMotionProfileMode(ULONG ulMode);

	// I/O
	BOOL IsPushShBufVac1BtnOn();
	BOOL IsPushShBufVac2BtnOn();
	BOOL IsPushShBufPanelOutBtnOn();
	BOOL IsPushShBufEmptylOutBtnOn();

	BOOL IsStartBtnOn();
	BOOL IsResetBtnOn();
	BOOL IsStopBtnOn();
	
	BOOL IsAirSupplyOn();
	BOOL IsEMODetectSnrOn();
	BOOL IsMainPowerFaultSnrOn();
	BOOL IsFrontDoorLockOn(); //20121206
	BOOL IsFrontDoorLock2On(); //20121206
	BOOL IsBackDoorLockOn(); //20121206
	BOOL IsLowerBackDoorSnr01On();
	BOOL IsLowerBackDoorSnr01NOOn();
	BOOL IsLowerBackDoorSnr01NCOn();
	BOOL IsLowerBackDoorSnr02On();
	BOOL IsLowerBackDoorSnr02NOOn();
	BOOL IsLowerBackDoorSnr02NCOn();
	BOOL IsLowerDoorSnr01On();
	BOOL IsLowerDoorSnr01NOOn();
	BOOL IsLowerDoorSnr01NCOn();
	BOOL IsLowerDoorSnr02On();
	BOOL IsLowerDoorSnr02NOOn();
	BOOL IsLowerDoorSnr02NCOn();
	BOOL IsInspLDoorSnrOn();
	BOOL IsInspLDoorSnrNOOn();
	BOOL IsInspLDoorSnrNCOn();
	BOOL IsInspRDoorSnrOn();
	BOOL IsInspRDoorSnrNOOn();
	BOOL IsInspRDoorSnrNCOn();

	//BOOL IsICHCoverOn();	// 20141013 Yip: Add ICH Cover Sensor // 20150204
#ifdef MB_FIND_CONTACT_LEVEL_DIFF //20130615
	BOOL IsTemporaryContactSnrOn();
#endif

	INT SetRedSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetOrangeSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetGreenSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetBuzzerSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetResetCounterSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetStartBtnSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetResetBtnSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetFrontDoorLockSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetFrontDoorLockSol2(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetBackDoorLockSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetShBufDoorLockSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetPanelOutBtnLamp(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetEmptyWhOutBtnLamp(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	INT SetIonizerMaintenanceCheckSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT SetIonizerDischargeEnableSol(BOOL bMode, BOOL bWait = GMP_NOWAIT);

	VOID SetPanelOutLamp(LampStatus lLampStatus);
	VOID SetEmptyOutLamp(LampStatus lLampStatus);

	VOID ResetSystem();
	VOID StartBonding();
	VOID StopBonding();

	DOUBLE GetSoftwareVersion(CString Number);
#ifdef GATEWAY
	LONG GatewayUpdateCOF902TA5Status(IPC_CServiceMessage &svMsg);
	LONG CheckGatewayConnection(IPC_CServiceMessage &svMsg);
	LONG GatewayUpdateCOF902MachineStatus(IPC_CServiceMessage &svMsg);
	INT getGatewayString(IPC_CServiceMessage &svMsg, LONG &lMsgLength, CString &szResult, LONG lMaxMsgLength = 128);
	INT getGatewayLong(IPC_CServiceMessage &svMsg, LONG &lMsgLength, LONG &lResult);
	LONG ProcessGatewayCommand(IPC_CServiceMessage &svMsg);
	LONG checkReceivedStatus(GATEWAY_STATUS_TYPE lStatus);
#endif

	//Print Data
	BOOL	PrintHeader(CString szStation, CString szEvent);
	BOOL	PrintTitle(CString szEvent, CString szTitle[]);
	BOOL	PrintData(CString szEvent, PRINT_DATA stPrintData);
	BOOL	IsDataFileExist(CString szEvent);	// 20140619 Yip

	virtual VOID	PrintMachineSetup(FILE *fp);
	virtual VOID	PrintDeviceSetup(FILE *fp);
#ifdef EXCEL_MACHINE_DEVICE_INFO
	virtual BOOL printMachineDeviceInfo();
#endif

protected:
	CMotorInfo	m_stMotorEncoderOnlyTmp1; //whz rotary linear calib
	LONG	m_lHMI_EncoderOnlyTmp1CurrPosn; //whz rotary linear calib
	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------
	CString szUserPassword;
	CString szEngPassword;
	CString szSvrPassword;

	LONG	HMI_lAutoMode;

	BOOL	HMI_bAutoTestStop;//20140923

	BOOL	HMI_bIndexTeflonTest;

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------

	LONG HMI_PRAutoTestStop(IPC_CServiceMessage &svMsg); //20140923
	//SI/SO
	LONG HMI_ToggleRedSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleOrangeSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleGreenSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleBuzzerSol(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleResetCounterSol(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleStartBtnSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleResetBtnSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleFrontDoorLockSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleFrontDoorLockSol2(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleBackDoorLockSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleShBufDoorLockSol(IPC_CServiceMessage &svMsg);
	LONG HMI_TogglePanelOutBtnLamp(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleEmptyWhOutBtnLamp(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleIonizerMaintenanceCheckSol(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleIonizerDischargeEnableSol(IPC_CServiceMessage &svMsg);


	//new sol (total 4)

	// ------------------------------------------------------------------------
	// Setting
	// ------------------------------------------------------------------------
	LONG HMI_BackUpPackages(IPC_CServiceMessage &svMsg);
	LONG HMI_ChangePassword(IPC_CServiceMessage &svMsg);
	LONG HMI_LangSwitching(IPC_CServiceMessage &svMsg);

	LONG HMI_OperatorLogin(IPC_CServiceMessage &svMsg); //20150211
	LONG HMI_EngineerLogin(IPC_CServiceMessage &svMsg);
	LONG HMI_ServiceLogin(IPC_CServiceMessage &svMsg);
	LONG HMI_DeveloperLogin(IPC_CServiceMessage &svMsg);

	LONG HMI_SetCalAverageFreq(IPC_CServiceMessage &svMsg);

	LONG SetAutoMode(AUTOMODE eAutoMode);
	LONG HMI_SetAutoMode(IPC_CServiceMessage &svMsg);

	LONG HMI_SetDetectCover(IPC_CServiceMessage &svMsg);
	LONG HMI_SetDetectLightCurtainSnr(IPC_CServiceMessage &svMsg);
	LONG HMI_SetDetectAirSupply(IPC_CServiceMessage &svMsg);

	LONG HMI_SetSafeIndexing(IPC_CServiceMessage &svMsg);
	LONG HMI_SetSlowMotionIndexing(IPC_CServiceMessage &svMsg);
	LONG HMI_SetSlowMotionFactor(IPC_CServiceMessage &svMsg);
	LONG HMI_SetGlassIndexingDelayFactor(IPC_CServiceMessage &svMsg);
	LONG HMI_SetEnableTrigLighting(IPC_CServiceMessage &svMsg);

	LONG HMI_CheckUserGroup(IPC_CServiceMessage &svMsg);
	LONG HMI_StopMoveTest(IPC_CServiceMessage &svMsg);
	LONG HMI_StopSearchDLnULTest(IPC_CServiceMessage &svMsg);

	// ------------------------------------------------------------------------
	// Package Information
	// ------------------------------------------------------------------------
	// package information
	// Glass Info. Related
	LONG HMI_SetGlassName(IPC_CServiceMessage &svMsg);
	LONG HMI_SetGlassLength(IPC_CServiceMessage &svMsg);
	LONG HMI_SetGlassWidth(IPC_CServiceMessage &svMsg);
	LONG HMI_SetReflectorThickness(IPC_CServiceMessage &svMsg);
	LONG HMI_SetUpperGlassThickness(IPC_CServiceMessage &svMsg);
	LONG HMI_SetLowerGlassThickness(IPC_CServiceMessage &svMsg);
	LONG HMI_SetPolarizerThickness(IPC_CServiceMessage &svMsg);
	LONG HMI_SetGlassItoWidth(IPC_CServiceMessage &svMsg);


	// LSI Type Info. Related
	LONG HMI_SetLSIType1Name(IPC_CServiceMessage &svMsg);
	LONG HMI_SetLSIType1Length(IPC_CServiceMessage &svMsg);
	LONG HMI_SetLSIType1Width(IPC_CServiceMessage &svMsg);
	LONG HMI_SetLSIType1Thickness(IPC_CServiceMessage &svMsg);

	// LSI On Glass Info. Related

	// MB Press Info

	// Rework Mode
	LONG HMI_SelectedReworkMode(IPC_CServiceMessage &svMsg);

	//Glass Input Mode 20120412
	LONG HMI_SetGlassInputMode(IPC_CServiceMessage &svMsg);

	LONG HMI_LightAndBuzzerInIdle(IPC_CServiceMessage &svMsg); //20130115

//p20120918
	// Traditional Chinese Switching
	LONG HMILangTraditionalChinese(IPC_CServiceMessage &svMsg);
//p20120918:end

	VOID LockAllCovers();
	VOID UnLockAllCovers();

	// Saving / loading parameters
	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();

};
