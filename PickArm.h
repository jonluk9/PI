/////////////////////////////////////////////////////////////////
//	PICKARM.cpp : interface of the CPickArm class
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

#define TRANSFERARM_EXTERN
#include "TransferArm.h"

#ifdef EXCEL_MACHINE_DEVICE_INFO
#include "ExcelMachineInfo_Util.h"
#endif

#ifdef PICKARM_EXTERN
extern BOOL					g_bStopCycle;
	
extern BOOL					g_bFlushMode;

extern STATUS_TYPE			PICKARM_Status;
extern STATUS_TYPE			IN_CONVEYOR_Status;
extern STATUS_TYPE			INSPWH_Status[MAX_NUM_OF_WH];
extern STATUS_TYPE			INSPWH_GLASS_Status[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
extern STATUS_TYPE			BUFFERWH_Status;

extern GLASS_INFO			g_stGlass;
extern LONG					g_lGlassInputMode;

extern BOOL					GlassOnInBelt;
extern LONG					g_lGlassInputMode; //20120412

extern BOOL					g_bIsPerformCleanEdge;
extern LONG					g_lGatewayPICKARMStatus; //20130919

#endif

typedef enum
{
	PICKARM_X_CTRL_NOTHING = 0, 
	PICKARM_X_CTRL_GO_HOME, 
	PICKARM_X_CTRL_GO_STANDBY_POSN, 
	PICKARM_X_CTRL_GO_PICK_GLASS_POSN,
	PICKARM_X_CTRL_GO_PICK_GLASS_OFFSET,
	PICKARM_X_CTRL_GO_PLACE_GLASS_POSN,
	PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_POSN, 
	PICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET, 
	PICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET,

} PICKARM_X_CTRL_SIGNAL;


// States
//
typedef enum
{
	PICKARM_IDLE_Q = 0, 

	PICKARM_WAIT_BUFFER_UNLOAD_Q,
	PICKARM_PICK_GLASS_FROM_BUFFER_Q,
	PICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q,
	PICKARM_WAIT_INSPWH_UNLOAD_Q,
	PICKARM_PICK_GLASS_FROM_INSPWH_Q,		//5

	PICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q,
	PICKARM_UNLOAD_TO_BUFFER_WH_Q,
	PICKARM_PLACE_GLASS_TO_BUFFER_Q,
	PICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q,
	PICKARM_UNLOAD_TO_FLIPPER_WH_Q,		//10

	PICKARM_WAIT_FLIPPER_WH_ACK_Q, 
	PICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q,
	PICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q,
	PICKARM_CHECK_PLACE_GLASS_Q,	//14

	PICKARM_STOP_Q = 99,

} PICKARM_STATE;

class CPickArm : public CTransferArm
{
	DECLARE_DYNCREATE(CPickArm)
	friend class CPI9000App;
	friend class CSettingFile;
	friend class CWinEagle;
	friend class CInspWH;
	//friend class CPreBondWH;
	//friend class CMainBondWH;
	//friend class CInspection;
	//friend class CInSHWH;
	friend class CShBufferWH;
	friend class CRejectArm;

protected:
	

protected:
	VOID UpdateOutput();

	virtual VOID RegisterVariables();
	virtual VOID PreStartOperation();
	virtual VOID AutoOperation();

	virtual BOOL GetGlass1Status() 
	{
		return m_bGlass1Exist;
	}


private:
	virtual CString		GetStation() 
	{
		return "PickArm_";
	}

public:
	CPickArm();
	virtual	~CPickArm();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();

protected:
	// ------------------------------------------------------------------------
	// Variable delcaration
	// ------------------------------------------------------------------------

	CSiInfo		m_stXLmtSnr;

	CSiInfo		m_stRecieveUpStreamInLine;
	CSiInfo		m_stRecieveReady;
	CSiInfo		m_stRecieving;
	CSiInfo		m_stRecieveStageMotion;
	CSiInfo		m_stRecieveStageInPosition;
	CSiInfo		m_stRecieveStageVacOK;
	CSiInfo		m_stRecieveG1VacOff;
	CSiInfo		m_stRecieveG2VacOff;
	CSiInfo		m_stRecieveG1VacError;
	CSiInfo		m_stRecieveG2VacError;
	CSiInfo		m_stRecieveCancel;
	CSiInfo		m_stInterLockFront;

	CSoInfo		m_stSendUpStreamInLine;
	CSoInfo		m_stSendReady;
	CSoInfo		m_stSendStageMotion;
	CSoInfo		m_stSendStageInPosition;
	CSoInfo		m_stSendStageVacOn;
	CSoInfo		m_stSending;
	CSoInfo		m_stSendG1Exist;
	CSoInfo		m_stSendG2Exist;
	CSoInfo		m_stSendCancel;
	CSoInfo		m_stInterLockBack;
	CSoInfo		m_stPAEMO;

	// 
	// ------------------------------------------------------------------------
	// End of Motor SI/SO
	// ------------------------------------------------------------------------

	// cycle status
	BOOL		m_bIsDefaultIndex;	// 1: Place Glass2 first 0: Place Glass1 first
	LONG		m_lCurrAssignPlace; // Try to pretend place to which Glass1 or Glass2
	
	//Posn
	LONG		m_lPickGlassOffset;
	LONG		m_lPlaceGlassOffset; 
	LONG		m_lShBufPlaceGlassPosn;
	LONG		m_lShBufPlaceGlassOffset;

	BOOL		HMI_bStandAlone;
	BOOL		m_bInlineInBack;
	// ------------------------------------------------------------------------
	// Function delcaration
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	// Virtual Transfer Function delcaration
	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	// END Virtual WorkHolder Function delcaration
	// ------------------------------------------------------------------------
	CString GetSequenceString();
	//VOID OperationSequence();

	GlassNum AssignPickFromWhere();
	GlassNum AssignPickFromWhere_ShBuf();
	BOOL IsFlushStop();
	GlassNum AssignPlaceToWhere();
	GlassNum AssignPlaceToWhere_edge_cleaner();

	INT PickGlassFromBuffer(GlassNum lGlassNum);
	INT PickGlassFromInspWH(GlassNum lGlassNum);
	INT PlaceGlassToBuffer(GlassNum lGlassNum);
	INT PlaceGlassOnOutBelt();
	BOOL IsPickGlassFromBuffer_OK(GlassNum lGlassNum);
	BOOL IsPickGlassFromInspWH_OK(GlassNum lGlassNum);
	BOOL IsPlaceGlassToBuffer_OK();
	BOOL IsUnloadToFlipperWH();
	BOOL CheckPlaceStatus();

	///////////////////////////////////////
	// HandShaking Signals
	///////////////////////////////////////
	VOID HS_PickArmSelectPanelEnable();
	VOID HS_PickArmSendPanelReady();
	BOOL HS_IsFlipperSendPanelReadyACK();	
	BOOL HS_IsFlipperRequestArmVacOff();	
	VOID HS_SetPanelDisabled();
	BOOL HS_IsFlipperReceivedPanel();	
	VOID HS_PickArmSendPanelNotReady();
	BOOL HS_IsHandshakeCanel();
	VOID HS_ClearAllHandShakeSignal();

	INT SetHandShakeBit(CSoInfo *stTempSo, BOOL bMode);
	BOOL IsHandShakeBitOn(CSiInfo stTempSi);

	////////////////////////////////////////

	INT PickGlass_DOWN();
	INT PickGlass_UP();
	INT PlaceGlass(GlassNum lGlassNum);
	
	BOOL PickGlassOperation();
	BOOL PickGlassFromInspWHOperation(GlassNum lGlassNum);
	BOOL PickGlassFromShbufOperation(GlassNum lGlassNum);
	BOOL PlaceGlassOperation();
	BOOL PlaceGlassToShbufOperation(GlassNum lGlassNum);
	BOOL PlaceGlassToOutBeltOperation(GlassNum lGlassNum);

	//Motor Related
	INT MoveX(PICKARM_X_CTRL_SIGNAL ctrl, BOOL bWait);


	// Module Setup Related
	VOID ModuleSetupPreTask();
	VOID ModuleSetupPostTask();
	
	BOOL ModuleSetupSetPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupGoPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupSetPara(PARA stPara);

	VOID UpdateModuleSetupPosn();
	VOID UpdateModuleSetupPara();

	INT SetupGoStandbyPosn();
	INT SetupGoPickGlassPosn();
	INT SetupGoPickGlassOffset();
	INT SetupGoPlaceGlassPosn();
	INT SetupGoPlaceGlass1Posn();
	INT SetupGoShBufPlaceGlassPosn();
	INT SetupGoPlaceGlassOffset(); //20121011
	INT SetupGoShBufPlaceGlassOffset();

	INT	SetSendUpStreamInLine(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSendReady(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSendStageMotion(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSendStageInPosition(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSendStageVacOn(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSending(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSendG1Exist(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSendG2Exist(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetSendCancel(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetInterLockBack(BOOL bMode, BOOL bWait = GMP_NOWAIT);
	INT	SetPAEMO(BOOL bMode, BOOL bWait = GMP_NOWAIT);


	INT MoveRelatedModuleToStandby();	// 20150327

	INT KeepQuiet();

	virtual VOID PrintMachineSetup(FILE *fp);
	virtual VOID PrintDeviceSetup(FILE *fp);
#ifdef EXCEL_MACHINE_DEVICE_INFO
	virtual BOOL printMachineDeviceInfo();
#endif


protected:
	// ------------------------------------------------------------------------
	// HMI Variable delcaration
	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	// HMI Command delcaration
	// ------------------------------------------------------------------------

	LONG HMI_PickGlassOperation(IPC_CServiceMessage &svMsg);
	LONG HMI_PlaceGlassOperation(IPC_CServiceMessage &svMsg);
	
	LONG HMI_SetPlaceGlassPrority(IPC_CServiceMessage &svMsg);
	LONG HMI_SetStandAlone(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleSendUpStreamInLine(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSendReady(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSendStageMotion(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSendStageInPosition(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSendStageVacOn(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSending(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSendG1Exist(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSendG2Exist(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleSendCancel(IPC_CServiceMessage &svMsg);
	LONG HMI_ToggleInterLockBack(IPC_CServiceMessage &svMsg);
	LONG HMI_TogglePAEMO(IPC_CServiceMessage &svMsg);

	LONG HMI_ToggleInlineInBack(IPC_CServiceMessage &svMsg);

	// Saving / loading parameters
	virtual LONG IPC_SaveMachineParam();
	virtual LONG IPC_LoadMachineParam();
	virtual LONG IPC_SaveDeviceParam();
	virtual LONG IPC_LoadDeviceParam();


};
