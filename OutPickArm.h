/////////////////////////////////////////////////////////////////
//	OUTPICKARM.cpp : interface of the COutPickArm class
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

#ifdef OUTPICKARM_EXTERN
extern BOOL					g_bStopCycle;
	
extern BOOL					g_bFlushMode;

extern STATUS_TYPE			OUTPICKARM_Status;
extern STATUS_TYPE			IN_CONVEYOR_Status;
extern STATUS_TYPE			INSPWH_Status[MAX_NUM_OF_WH];
extern STATUS_TYPE			INSPWH_GLASS_Status[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];
extern STATUS_TYPE			OUTPICKARM_GLASS_Status[MAX_NUM_OF_GLASS];	
extern STATUS_TYPE			BUFFERWH_Status;
extern STATUS_TYPE			TA_MANAGER_Status;

extern GLASS_INFO			g_stGlass;
extern LONG					g_lGlassInputMode;

extern BOOL					g_bGlassOnRejBelt;
extern LONG					g_lGlassInputMode; //20120412

extern BOOL					g_bIsPerformCleanEdge;
extern LONG					g_lGatewayOUTPICKARMStatus; //20130919

extern PAPosnType			OUTPICKARM_ARM_Posn;

#endif

typedef enum
{
	OUTPICKARM_Z_CTRL_NOTHING = 0, 
	OUTPICKARM_Z_CTRL_GO_HOME, 
	OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, 
	OUTPICKARM_Z_CTRL_GO_PICK_GLASS_LEVEL,
	OUTPICKARM_Z_CTRL_GO_PICK_GLASS_OFFSET,
	OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_LEVEL,
	OUTPICKARM_Z_CTRL_GO_SHBUF_PLACE_GLASS_LEVEL, 
	OUTPICKARM_Z_CTRL_GO_REJBELT_PLACE_GLASS_LEVEL, 
	OUTPICKARM_Z_CTRL_GO_PLACE_GLASS_OFFSET, 
	OUTPICKARM_Z_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET, 
	OUTPICKARM_Z_CTRL_GO_REJBELT_PLACE_GLASS_OFFSET, 
	OUTPICKARM_Z_CTRL_GO_SHBUF_PICK_GLASS_LEVEL, 
	OUTPICKARM_Z_CTRL_GO_SHBUF_PICK_GLASS_OFFSET, 

} OUTPICKARM_Z_CTRL_SIGNAL;
typedef enum
{
	OUTPICKARM_X_CTRL_NOTHING = 0, 
	OUTPICKARM_X_CTRL_GO_HOME, 
	OUTPICKARM_X_CTRL_GO_STANDBY_POSN, 
	OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_POSN,
	OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_POSN,
	OUTPICKARM_X_CTRL_GO_WH1_PICK_GLASS_OFFSET,
	OUTPICKARM_X_CTRL_GO_WH2_PICK_GLASS_OFFSET,
	OUTPICKARM_X_CTRL_GO_PLACE_GLASS_POSN,
	OUTPICKARM_X_CTRL_GO_PLACE_GLASS_OFFSET,
	OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_POSN,
	OUTPICKARM_X_CTRL_GO_SHBUF_PLACE_GLASS_OFFSET,
 	OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_POSN, 
	OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_POSN, 
	OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS1_OFFSET,
	OUTPICKARM_X_CTRL_GO_REJBELT_PLACE_GLASS2_OFFSET,


} OUTPICKARM_X_CTRL_SIGNAL;


// States
//
typedef enum
{
	OUTPICKARM_IDLE_Q = 0, 

	OUTPICKARM_WAIT_BUFFER_UNLOAD_Q,
	OUTPICKARM_PICK_GLASS_FROM_BUFFER_Q,
	OUTPICKARM_CHECK_PICK_BUFFER_GLASS_OK_Q,
	OUTPICKARM_WAIT_INSPWH_UNLOAD_Q,
	OUTPICKARM_PICK_GLASS_FROM_INSPWH_Q,		//5

	OUTPICKARM_CHECK_PICK_INSPWH_GLASS_OK_Q,
	OUTPICKARM_UNLOAD_TO_BUFFER_WH_Q,
	OUTPICKARM_PLACE_GLASS_TO_BUFFER_Q,
	OUTPICKARM_CHECK_PLACE_BUFFER_GLASS_OK_Q,
	OUTPICKARM_UNLOAD_TO_FLIPPER_WH_Q,			//10

	OUTPICKARM_WAIT_FLIPPER_WH_ACK_Q, 
	OUTPICKARM_WAIT_FLIPPER_WH_REQUEST_PA_VAC_OFF_Q,
	OUTPICKARM_WAIT_FLIPPER_WH_RECEIVE_PANEL_ACK_Q,
	OUTPICKARM_CHECK_PLACE_GLASS_Q,	
	OUTPICKARM_ANY_ERROR_GLASS_PICKED_Q,		//15

	OUTPICKARM_WAIT_REJBELT_READY_Q,			//16

	OUTPICKARM_STOP_Q = 99,

} OUTPICKARM_STATE;

class COutPickArm : public CTransferArm
{
	DECLARE_DYNCREATE(COutPickArm)
	friend class CPI9000App;
	friend class CSettingFile;
	friend class CWinEagle;
	friend class CInspWH;
	friend class CInPickArm;
	//friend class CPreBondWH;
	//friend class CMainBondWH;
	//friend class CInspection;
	//friend class CInSHWH;
	friend class CShBufferWH;
	friend class CRejectBelt;

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
		return "OutPickArm_";
	}

public:
	COutPickArm();
	virtual	~COutPickArm();

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
	LONG		m_lCurInspWH;
	
	//Posn
	LONG		m_lPickGlassPosn[MAX_NUM_OF_WH];
	LONG		m_lPickGlassOffset[MAX_NUM_OF_WH];
	LONG		m_lPlaceGlassPosn; 
	LONG		m_lPlaceGlassOffset; 
	LONG		m_lShBufPlaceGlassPosn;
	LONG		m_lShBufPlaceGlassOffset;
	LONG		m_lRejBeltPlaceGlassPosn[MAX_NUM_OF_GLASS];
	LONG		m_lRejBeltPlaceGlassOffset[MAX_NUM_OF_GLASS];

	//Level
	LONG			m_lShBufPickLevel;
	LONG			m_lShBufPickLevelOffset;
	LONG			m_lShBufPlaceLevel;
	LONG			m_lShBufPlaceLevelOffset;
	LONG			m_lRejBeltPlaceLevel;
	LONG			m_lRejBeltPlaceLevelOffset;

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
	VOID OperationSequence();

	GlassNum AssignPickFromWhere(LONG lCurWH);
	GlassNum AssignPickFromWhere_ShBuf();
//	BOOL IsFlushStop();
	GlassNum AssignPlaceToWhere(LONG lCurWH);
//	GlassNum AssignPlaceToWhere_edge_cleaner();

	INT PickGlassFromBuffer(GlassNum lGlassNum);
	INT PickGlassFromInspWH(LONG lCurWH, GlassNum lGlassNum);
	INT PlaceGlassToBuffer(GlassNum lGlassNum);
	INT PlaceGlassOnOutBelt();
	INT PlaceGlassOnRejBelt(GlassNum lGlassNum);
	BOOL IsPickGlassFromBuffer_OK(GlassNum lGlassNum);
	BOOL IsPickGlassFromInspWH_OK(LONG lCurInspWH, GlassNum lGlassNum);
	BOOL IsPlaceGlassToBuffer_OK();
	BOOL IsUnloadToFlipperWH();
	BOOL CheckPlaceStatus();
	VOID TransferBufferGlassStatusToPA(GlassNum lGlassNum);
	VOID TransferInspWHGlassStatusToPA(LONG lCurWH, GlassNum lGlassNum);

	///////////////////////////////////////
	// HandShaking Signals
	///////////////////////////////////////
	VOID HS_OUTPICKARMSelectPanelEnable();
	VOID HS_OUTPICKARMSendPanelReady();
	BOOL HS_IsFlipperSendPanelReadyACK();	
	BOOL HS_IsFlipperRequestArmVacOff();	
	VOID HS_SetPanelDisabled();
	BOOL HS_IsFlipperReceivedPanel();	
	VOID HS_OUTPICKARMSendPanelNotReady();
	BOOL HS_IsHandshakeCanel();
	VOID HS_ClearAllHandShakeSignal();

	INT SetHandShakeBit(CSoInfo *stTempSo, BOOL bMode);
	BOOL IsHandShakeBitOn(CSiInfo stTempSi);

	////////////////////////////////////////
//
//	INT PickGlass_DOWN();
//	INT PickGlass_UP();
//	INT PlaceGlass(GlassNum lGlassNum);
//	
	BOOL PickGlassOperation();
	BOOL PickGlassFromInspWHOperation(LONG lCurWH, GlassNum lGlassNum);
	BOOL PickGlassFromShbufOperation(GlassNum lGlassNum);
	BOOL PlaceGlassOperation();
	BOOL PlaceGlassToShbufOperation(GlassNum lGlassNum);
	BOOL PlaceGlassToOutBeltOperation(GlassNum lGlassNum);
	BOOL PlaceGlassToRejBeltOperation(GlassNum lGlassNum);
//
	//Motor Related
	INT MoveX(OUTPICKARM_X_CTRL_SIGNAL ctrl, BOOL bWait);
	INT MoveZ(OUTPICKARM_Z_CTRL_SIGNAL ctrl, BOOL bWait);


//	// Module Setup Related
	VOID ModuleSetupPreTask();
	VOID ModuleSetupPostTask();
	
	BOOL ModuleSetupSetPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupGoPosn(LONG lSetupTitle = 0);
	BOOL ModuleSetupSetLevel(LONG lSetupTitle = 0);
	BOOL ModuleSetupGoLevel(LONG lSetupTitle = 0);
	BOOL ModuleSetupSetPara(PARA stPara);

	VOID UpdateModuleSetupPosn();
	VOID UpdateModuleSetupLevel();
	VOID UpdateModuleSetupPara();

	INT SetupGoStandbyPosn();
	INT SetupGoPickGlassPosn(LONG lCurWH);
	INT SetupGoPickGlassOffset(LONG lCurWH);
	INT SetupGoPlaceGlassPosn();
	INT SetupGoShBufPlaceGlassPosn();
	INT SetupGoPlaceGlassOffset(); //20121011
	INT SetupGoShBufPlaceGlassOffset();
	INT SetupGoPlaceRejGlassPosn(LONG lCurGlass);
	INT SetupGoPlaceRejGlassOffset(LONG lCurGlass);

	INT SetupGoStandbyLevel();
	INT SetupGoPickLevel();
	INT SetupGoPickLevelOffset();
	INT SetupGoPlaceLevel();
	INT SetupGoPlaceLevelOffset();
	INT SetupGoBufferPlaceLevel();
	INT SetupGoBufferPlaceLevelOffset();
	INT SetupGoRejBeltPlaceLevel();
	INT SetupGoRejBeltPlaceLevelOffset();

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
//	
//	LONG HMI_SetPlaceGlassPrority(IPC_CServiceMessage &svMsg);
//	LONG HMI_SetStandAlone(IPC_CServiceMessage &svMsg);

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
