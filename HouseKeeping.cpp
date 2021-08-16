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


#include "stdafx.h"
#include "MarkConstant.h"

#include "PI9000.h"
#include "PI9000_Constant.h"

#define HOUSEKEEPING_EXTERN	 // must pust here 
#include "HouseKeeping.h"

#include "InspWH.h"
#include "WH1.h"
#include "WH2.h"
#include "InspOpt.h"
#include "InPickArm.h"
#include "ShBufferWH.h"

#include "Cal_Util.h"
#include "FileOperations.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern STATUS_TYPE		TRAY_IN_ELEVATOR_Status;
#ifdef GATEWAY
extern GATEWAY_STATUS_TYPE PI9000_Machine_Status;
extern GATEWAY_STATUS_TYPE	COF902_Machine_Status;
extern GATEWAY_STATUS_TYPE COF902_OUTPICKARM_Status;
extern GATEWAY_STATUS_TYPE PI9000_INSPWH_Status;
#endif

IMPLEMENT_DYNCREATE(CHouseKeeping, CPI9000Stn)

CHouseKeeping::CHouseKeeping()
{
	// Si Related

	//dummy Motor for WH-Z rotary-linear calibration
	//m_stMotorEncoderOnlyTmp1.InitMotorInfo("HOUSEKEEPING_ENC_ONLY" , &m_bModSelected); //whz rotary linear calib

	//m_stHTPwrOnSnr			= CSiInfo("HTPwrOnSnr",			"IOMODULE_IN_PORT_2", BIT_7, ACTIVE_LOW);
	//m_mySiList.Add((CSiInfo*)&m_stHTPwrOnSnr); //ok	
	m_stPwrOFF					= CSiInfo("PwrOFF",		"IOMODULE_IN_PORT_2", BIT_16, ACTIVE_LOW);//ACTIVE_HIGH);
	m_mySiList.Add((CSiInfo*)&m_stPwrOFF); //ok	
	m_stEMODetectSnr			= CSiInfo("EMODetectSnr",			"IOMODULE_IN_PORT_2", BIT_17,  ACTIVE_HIGH);//ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stEMODetectSnr); //ok	
	m_stMainPowerFaultSnr		= CSiInfo("MainPowerFaultSnr",		"IOMODULE_IN_PORT_2", BIT_18, ACTIVE_HIGH);
	m_mySiList.Add((CSiInfo*)&m_stMainPowerFaultSnr); //ok	
	m_stFH1Snr					= CSiInfo("FH1",	"IOMODULE_IN_PORT_2", BIT_19, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stFH1Snr); //ok
	m_stFH2Snr					= CSiInfo("FH2",	"IOMODULE_IN_PORT_2", BIT_20, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stFH2Snr); //ok
	m_stFL1Snr					= CSiInfo("FL1",	"IOMODULE_IN_PORT_2", BIT_21, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stFL1Snr); //ok
	m_stFL2Snr					= CSiInfo("FL2",	"IOMODULE_IN_PORT_2", BIT_22, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stFL2Snr); //ok
	m_stSPSnr					= CSiInfo("SP",		"IOMODULE_IN_PORT_2", BIT_23, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stSPSnr); //ok

	m_stStartButtonSnr			= CSiInfo("StartButtonSnr",		"IOMODULE_IN_PORT_1", BIT_0, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stStartButtonSnr);

	m_stStopButtonSnr			= CSiInfo("StopButtonSnr",		"IOMODULE_IN_PORT_1", BIT_1, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stStopButtonSnr);

	m_stResetButtonSnr			= CSiInfo("ResetButtonSnr",		"IOMODULE_IN_PORT_1", BIT_2, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stResetButtonSnr);

	m_stAirSupplySnr			= CSiInfo("AirSupplySnr",		"IOMODULE_IN_PORT_1", BIT_3, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stAirSupplySnr);	//ok

	m_stFrontDoorLockSnr		= CSiInfo("FrontDoorLockSnr01",	"IOMODULE_IN_PORT_1", BIT_4, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stFrontDoorLockSnr);

	m_stFrontDoorLockSnr2		= CSiInfo("FrontDoorLockSnr02",	"IOMODULE_IN_PORT_1", BIT_5, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stFrontDoorLockSnr2);

	m_stBackDoorLockSnr		= CSiInfo("BackDoorLockSnr01",	"IOMODULE_IN_PORT_1", BIT_6, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stBackDoorLockSnr);

	m_stInspLDoorSnrNO			= CSiInfo("InspLDoorSnrNO",		"IOMODULE_IN_PORT_1", BIT_7, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stInspLDoorSnrNO);

	m_stInspLDoorSnrNC			= CSiInfo("InspLDoorSnrNC",		"IOMODULE_IN_PORT_1", BIT_8, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stInspLDoorSnrNC);

	m_stInspRDoorSnrNO			= CSiInfo("InspRDoorSnrNO",		"IOMODULE_IN_PORT_1", BIT_9, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stInspRDoorSnrNO);

	m_stInspRDoorSnrNC			= CSiInfo("InspRDoorSnrNC",		"IOMODULE_IN_PORT_1", BIT_10, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stInspRDoorSnrNC);

	m_stShBufPanelOutBtnSnr		= CSiInfo("ShBufferWHPanelOutSwSnr",		"IOMODULE_IN_PORT_6", BIT_5, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stShBufPanelOutBtnSnr);

	m_stShBufEmptyWhOutBtnSnr		= CSiInfo("ShBufferWHEmptyWHOutSwSnr",		"IOMODULE_IN_PORT_6", BIT_6, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stShBufEmptyWhOutBtnSnr);

	m_stShBufVac1BtnSnr		= CSiInfo("ShBufferWHVac1SwSnr",		"IOMODULE_IN_PORT_6", BIT_7, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stShBufVac1BtnSnr);

	m_stShBufVac2BtnSnr		= CSiInfo("ShBufferWHVac2SwSnr",		"IOMODULE_IN_PORT_6", BIT_8, ACTIVE_LOW);
	m_mySiList.Add((CSiInfo*)&m_stShBufVac2BtnSnr);

	m_stLowerBackDoorSnr01NO			= CSiInfo("LowerBackDoorSnr01NO",		"IOMODULE_IN_PORT_6", BIT_9, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerBackDoorSnr01NO);

	m_stLowerBackDoorSnr01NC			= CSiInfo("LowerBackDoorSnr01NC",		"IOMODULE_IN_PORT_6", BIT_10, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerBackDoorSnr01NC);

	m_stLowerBackDoorSnr02NO			= CSiInfo("LowerBackDoorSnr02NO",		"IOMODULE_IN_PORT_6", BIT_11, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerBackDoorSnr02NO);

	m_stLowerBackDoorSnr02NC			= CSiInfo("LowerBackDoorSnr02NC",		"IOMODULE_IN_PORT_6", BIT_12, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerBackDoorSnr02NC);

	m_stLowerDoorSnr01NO			= CSiInfo("LowerDoorSnr01NO",		"IOMODULE_IN_PORT_6", BIT_13, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerDoorSnr01NO);

	m_stLowerDoorSnr01NC			= CSiInfo("LowerDoorSnr01NC",		"IOMODULE_IN_PORT_6", BIT_14, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerDoorSnr01NC);

	m_stLowerDoorSnr02NO			= CSiInfo("LowerDoorSnr02NO",		"IOMODULE_IN_PORT_6", BIT_15, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerDoorSnr02NO);

	m_stLowerDoorSnr02NC			= CSiInfo("LowerDoorSnr02NC",		"IOMODULE_IN_PORT_6", BIT_16, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stLowerDoorSnr02NC);

	m_stIonizerMaintenanceResult			= CSiInfo("IonizerMaintenanceResult",		"IOMODULE_IN_PORT_4", BIT_22, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stIonizerMaintenanceResult);

	m_stIonizerError			= CSiInfo("IonizerError",		"IOMODULE_IN_PORT_4", BIT_23, ACTIVE_LOW); 
	m_mySiList.Add((CSiInfo*)&m_stIonizerError);


	// SOL
	m_stRedLight				= CSoInfo("RedLight",				"IOMODULE_OUT_PORT_1", BIT_12, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stRedLight);	//ok
	m_stOrangeLight				= CSoInfo("OrangeLight",			"IOMODULE_OUT_PORT_1", BIT_13, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stOrangeLight);	//ok
	m_stGreenLight				= CSoInfo("GreenLight",				"IOMODULE_OUT_PORT_1", BIT_14, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stGreenLight);	//ok
	m_stBuzzer					= CSoInfo("Buzzer",					"IOMODULE_OUT_PORT_1", BIT_15, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stBuzzer);	//ok

	m_stStartButtonSol					= CSoInfo("StartButtonSol",			"IOMODULE_OUT_PORT_1", BIT_0, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stStartButtonSol);	

	m_stResetButtonSol					= CSoInfo("ResetButtonSol",			"IOMODULE_OUT_PORT_1", BIT_2, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stResetButtonSol);	

	m_stFrontDoorLockSol				= CSoInfo("FrontDoorLockSol",			"IOMODULE_OUT_PORT_1", BIT_4, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stFrontDoorLockSol);

	m_stFrontDoorLockSol2				= CSoInfo("FrontDoorLockSol02",			"IOMODULE_OUT_PORT_1", BIT_5, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stFrontDoorLockSol2);	

	m_stBackDoorLockSol				= CSoInfo("BackDoorLockSol",			"IOMODULE_OUT_PORT_1", BIT_6, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stBackDoorLockSol);	

	m_stShBufDoorLockSol				= CSoInfo("ShBufDoorLockSol",			"IOMODULE_OUT_PORT_1", BIT_11, OTHER_SOL, ACTIVE_LOW);
	m_mySoList.Add((CSoInfo*)&m_stShBufDoorLockSol);
	
	m_stShBufPanelOutBtnLamp		= CSoInfo("ShBufferWHPanelOutBtnLamp",		"IOMODULE_OUT_PORT_6", BIT_5, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stShBufPanelOutBtnLamp);

	m_stShBufEmptyWhOutBtnLamp		= CSoInfo("ShBufferWHEmptyWhOutBtnLamp",		"IOMODULE_OUT_PORT_6", BIT_6, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stShBufEmptyWhOutBtnLamp);
	
	m_stIonizerDischargeEnable		= CSoInfo("IonizerDischargeEnable",		"IOMODULE_OUT_PORT_4", BIT_20, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stIonizerDischargeEnable);

	m_stIonizerMaintenanceCheck		= CSoInfo("IonizerMaintenanceCheck",		"IOMODULE_OUT_PORT_4", BIT_21, OTHER_SOL, ACTIVE_HIGH);
	m_mySoList.Add((CSoInfo*)&m_stIonizerMaintenanceCheck);


	m_bDetectAirSupply				= TRUE; //20120305 problem 13
	m_lAirErrorCounter				= 0;
	HMI_bStopProcess				= FALSE; //20121225
	HMI_lAutoMode					= (LONG) AUTO_BOND; //20150211


	m_lHMI_EncoderOnlyTmp1CurrPosn	= 0; //whz rotary linear calib

	m_bBeepSound = TRUE; //20140704

	HMI_bAutoTestStop						= FALSE; //20140923

	m_bDisableSelectAll = FALSE;	// 20150515

	m_bShBufDoorClose = FALSE;
	
	m_bEnableStartBtn	= FALSE;
	m_bEnableResetBtn	= TRUE;
	m_bEnableStopBtn	= FALSE;
	m_bPressBtnToStop	= FALSE;
	
	SetResetBtnSol(ON);
}

CHouseKeeping::~CHouseKeeping()
{
}

BOOL CHouseKeeping::InitInstance()
{
	CPI9000Stn::InitInstance();

	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CGmpSystem *pGmpSys = pAppMod->m_pCGmpSystem;
	CString		szMsg	= _T("");

	INT i = 0;

	try
	{
		// Acutators
		//if (pAppMod->IsInitNuDrive())
		//{
		for (i = 0; i < m_myMotorList.GetSize(); i++)
		{
				// Initi Motor Configuration
			GetmyMotorList().GetAt(i)->SetAppStation(pAppMod);
			GetmyMotorList().GetAt(i)->SetStnName(GetStnName());
			if (pAppMod->IsInitNuDrive())
			{
				GetmyMotorList().GetAt(i)->SetGmpChannel(pGmpSys->AddChannel(m_myMotorList.GetAt(i)->stAttrib.szAxis, m_myMotorList.GetAt(i)->stAttrib.dDistPerCount));
				GetmyMotorList().GetAt(i)->InitMtrAxis();
			}
		}
		//}

#if 0  //whz rotary linear calib
		//m_stMotorEncoderOnlyTmp1.SetAppStation(pAppMod);
		//m_stMotorEncoderOnlyTmp1.SetStnName(GetStnName());
		//if (pAppMod->IsInitNuDrive())
		//{
		//	m_stMotorEncoderOnlyTmp1.SetGmpChannel(pGmpSys->AddChannel(m_stMotorEncoderOnlyTmp1.stAttrib.szAxis, m_stMotorEncoderOnlyTmp1.stAttrib.dDistPerCount));
		//	m_stMotorEncoderOnlyTmp1.InitMtrAxis();
		//	m_stMotorEncoderOnlyTmp1.SetPosition(0);
		//}
#endif

		if (pAppMod->IsInitNuDrive())
		{
			// SI
			for (i = 0; i < m_mySiList.GetSize(); i++)
			{
				GetmySiList().GetAt(i)->SetGmpPort(
					pGmpSys->AddSiPort(m_mySiList.GetAt(i)->GetPortName(), 
									   m_mySiList.GetAt(i)->GetMask(), 
									   m_mySiList.GetAt(i)->GetActiveState()));
			}

			// SO
			for (i = 0; i < m_mySoList.GetSize(); i++)
			{
				GetmySoList().GetAt(i)->SetGmpPort(
					pGmpSys->AddSoPort(m_mySoList.GetAt(i)->GetPortName(), 
									   m_mySoList.GetAt(i)->GetMask(), 
									   m_mySoList.GetAt(i)->GetActiveState()));

				GetmySoList().GetAt(i)->GetGmpPort().Off();
				GetmySoList().GetAt(i)->bHMI_Status = GetmySoList().GetAt(i)->GetGmpPort().IsOn();
				
			}
			////Heat power always on

			SetOrangeSol(ON);	// 20140630 Yip: Yellow Light On After Machine Start Up
			SetResetBtnSol(ON);
		}
#if 1 //20120305 problem 13
		if (pAppMod->IsInitNuDrive() && !IsAirSupplyOn())
		{
			pAppMod->m_bHWInitFatalError = TRUE;
			if (pAppMod->m_bDiagnMode)
			{
				m_bDetectAirSupply = FALSE;
				SetError(IDS_HK_AIR_SUPPLY_ERR);

				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("WARNING: Air Supply Error");
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			}
			else
			{
				pAppMod->m_bHWInitError = TRUE;

				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("ERROR: Air Supply Error");
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			}
		}
#endif

	}
	catch (CAsmException e)
	{
		pAppMod->m_bHWInitError = TRUE;

		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}


	pAppMod->m_bHWInitCheckingPressure = FALSE;
	return TRUE;
}

INT CHouseKeeping::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CHouseKeeping::Operation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	switch (State())
	{		
	case IDLE_Q:
		IdleOperation();

		if (m_bPressBtnToStop)
		{
			m_bPressBtnToStop = FALSE;
			IPC_CServiceMessage svMsg;
			CString szScreen = "Bonding";
			do
			{
				Sleep(50);

			} while (pAppMod->m_szState != "Idle");
			HMI_LightAndBuzzerInIdle(svMsg);
			svMsg.InitMessage(szScreen.GetLength() + 1, szScreen.GetBuffer());
			szScreen.ReleaseBuffer();
			m_comClient.SendRequest("HmiUserService", "HmiSetScreen", svMsg, 0);
		}

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

VOID CHouseKeeping::UpdateOutput()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	INT i = 0;

	BOOL bStopOperation = FALSE;
	LONG lAnswer		= rMSG_TIMEOUT;
	BOOL bShowAirSupplyMessage = FALSE; //20120210

#if 1 //20130410
	if ((m_qState == UN_INITIALIZE_Q) || (m_qState == DE_INITIAL_Q))
#else
	if ((m_qState == UN_INITIALIZE_Q) || (m_qState == DE_INITIAL_Q) || (m_qState == AUTO_Q) || (m_qState == DEMO_Q))
#endif
	{
		return;
	}

	if (pAppMod->m_bHWInitError)
	{
		return;
	}

	if (IsResetBtnOn() && m_bEnableResetBtn)
	{
		m_bEnableStartBtn	= TRUE;
		//m_bEnableResetBtn	= FALSE;
		m_bEnableStopBtn	= FALSE;
		ResetSystem();
	}
 	if (IsStartBtnOn() && m_bEnableStartBtn)
	{
		m_bEnableStartBtn	= FALSE;
		m_bEnableResetBtn	= FALSE;
		m_bEnableStopBtn	= FALSE;
		StartBonding();
		SetResetBtnSol(OFF);
	}
	try
	{
		BOOL bAirSupplyError = FALSE; 

		if (m_fHardware && m_bCheckMotor)
		{
			for (i = 0; i < m_myMotorList.GetSize(); i++)
			{
				GetmyMotorList().GetAt(i)->bHMI_On			= GetmyMotorList().GetAt(i)->IsPowerOn();
				GetmyMotorList().GetAt(i)->lHMI_CurrPosn	= GetmyMotorList().GetAt(i)->GetEncPosn();
			}

#if 1 //whz rotary linear calib
			//if (pAppMod->IsInitNuDrive() && m_bCheckIO)
			{
				m_lHMI_EncoderOnlyTmp1CurrPosn = -1 * m_stMotorEncoderOnlyTmp1.GetEncPosn();
			}
#endif

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
#if 1 //20120305 problem 13 detect air supply
		if (m_fHardware)
		{
			static BOOL bEMODetected = FALSE;

			if (IsEMODetectSnrOn()) //20130829
			{
				if (!bEMODetected)
				{
					// 20140910 Yip: Stop Operation If EMO Is Pressed
					if (State() == AUTO_Q || State() == DEMO_Q || State() == CYCLE_Q)
					{
						bStopOperation = TRUE;
					}
					DisplayMessage("*** EMO is Pressed ***");
					SetError(IDS_HK_EMO_PRESSED_ERR);	// 20140903 Yip
				}
				bEMODetected = TRUE;
			}
			else
			{
				bEMODetected = FALSE;
			}
		}
		// detect air supply
		if (m_bDetectAirSupply && m_fHardware)
		{
			BOOL bONError = FALSE;
			BOOL bOFFError = FALSE;

			bAirSupplyError = !IsAirSupplyOn();

			if (bAirSupplyError || bONError || bOFFError)
			{
				m_lAirErrorCounter++;
			}
			else
			{
				m_lAirErrorCounter = 0;
			}

			// Error?
			if (m_lAirErrorCounter > 5)		
			{
				CWinEagle *pCWinEagle = (CWinEagle*)m_pModule->GetStation("WinEagle");

				if (bAirSupplyError)
				{
					SetError(IDS_HK_AIR_SUPPLY_ERR);
				}

				if (State() == AUTO_Q || State() == DEMO_Q || State() == CYCLE_Q)
				{
					bStopOperation = TRUE;
				}

				{
					if (!(State() == AUTO_Q || State() == DEMO_Q || State() == CYCLE_Q)) //20120210
					{
						lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Continue to Monitor air supply?");
						if (lAnswer == rMSG_CANCEL)
						{
							m_bDetectAirSupply = FALSE;
							SetHmiVariable("Housekeeping_bDetectAirSupply");
							HMIMessageBox(MSG_OK, "WARNING", "Please enable air supply detection manually.");
						}
						else if (lAnswer == rMSG_TIMEOUT)
						{
							m_bDetectAirSupply = FALSE;
							SetHmiVariable("Housekeeping_bDetectAirSupply");
						}

					}
					else
					{
						bShowAirSupplyMessage = TRUE; //20120210
						//m_bDetectAirSupply = FALSE;
					}
				}
				//else
				//{
				//	m_bDetectAirSupply = FALSE;
				//}
			}
		}

		if (bStopOperation)
		{
			INT nConvID;
			IPC_CServiceMessage stMsg;
			LONG lTimeCount = 0;

			typedef struct
			{
				LONG lAxis;
				LONG lAction;
				LONG lUnit;
				BOOL bReply;
			} PARA;

			PARA stPara;

			stPara.lAxis = 0;
			stPara.lAction = 0;
			stPara.lUnit = 0;
			stPara.bReply = FALSE;

			Command(glSTOP_COMMAND);
			State(STOPPING_Q);

			stMsg.InitMessage(sizeof(PARA), &stPara);
			nConvID = m_comClient.SendRequest("PI9000", "Stop", stMsg);
			while (!m_comClient.ScanReplyForConvID(nConvID, 1) && (lTimeCount++ < 8640000))
			{
				Sleep(1);
			}
			m_comClient.ReadReplyForConvID(nConvID, stMsg);
			
			if (bShowAirSupplyMessage) //20120210
			{
				//HMIMessageBox(MSG_OK, "WARNING", "Please enable air supply detection manually.");
				lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Continue to Monitor air supply?");
				if (lAnswer == rMSG_CANCEL)
				{
					m_bDetectAirSupply = FALSE;
					SetHmiVariable("Housekeeping_bDetectAirSupply");
					HMIMessageBox(MSG_OK, "WARNING", "Please enable air supply detection manually.");
				}
				else if (lAnswer == rMSG_TIMEOUT)
				{
					m_bDetectAirSupply = FALSE;
					SetHmiVariable("Housekeeping_bDetectAirSupply");
				}
			}
		}

#endif
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	Sleep(UPDATEOUTPUT_SLEEP);
}

VOID CHouseKeeping::RegisterVariables()
{
	try
	{
		RegVariable(_T("HMI_lEncoderOnlyTmp1CurrPosn"), &m_lHMI_EncoderOnlyTmp1CurrPosn); //whz rotary linear calib
		// -------------------------------------------------------
		// PI9000Stn Variable and Function
		// -------------------------------------------------------		
		RegVariable(_T("HouseKeeping_bCheckIO"), &m_bCheckIO);
		RegVariable(_T("HouseKeeping_bCheckMotor"), &m_bCheckMotor);

		// -------------------------------------------------------
		// varibales
		// -------------------------------------------------------
		RegVariable(_T("ulUserGroup"), &g_ulUserGroup);
		RegVariable(_T("bUserGroupOperator"), &g_bUserGroupOperator); //20150114
		RegVariable(_T("szPassword"), &szUserPassword);
		RegVariable(_T("szEngPassword"), &szEngPassword);
		RegVariable(_T("szSvrPassword"), &szSvrPassword);
		
		RegVariable(_T("lCalAverageFreq"), &g_lCalAverageFreq);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetCalAverageFreq", HMI_SetCalAverageFreq);
		
		RegVariable(_T("HouseKeeping_AutoMode"), &HMI_lAutoMode); //20150211

		RegVariable(_T("HouseKeeping_bSlowMotionIndexing"), &g_bSlowMotionIndexing);
		RegVariable(_T("HouseKeeping_bDetectAirSupply"), &m_bDetectAirSupply);
		RegVariable(_T("Housekeeping_bShowForceInNewton"), &g_bShowForceInNewton);
		RegVariable(_T("HouseKeeping_ulSlowMotionFactor"), &g_ulSlowMotionFactor);

		RegVariable(_T("Housekeeping_bEnableTrigLighting"), &g_bEnableTrigLighting);

		RegVariable(_T("HouseKeeping_dForceCalibratorHeight"), &g_stForceCalibrator.dHeight);

		RegVariable(_T("HouseKeeping_bDisableSelectAll"), &m_bDisableSelectAll);	//20150515


		RegVariable(_T("HMI_bAirSupplySnr"), &m_stAirSupplySnr.bHMI_Status); //20120703
		RegVariable(_T("HMI_bEMODetectSnr"), &m_stEMODetectSnr.bHMI_Status);
		RegVariable(_T("HMI_bMainPowerFaultSnr"), &m_stMainPowerFaultSnr.bHMI_Status);

		RegVariable(_T("HMI_bFrontDoorLockSnr"), &m_stFrontDoorLockSnr.bHMI_Status);
		RegVariable(_T("HMI_bFrontDoorLockSnr2"), &m_stFrontDoorLockSnr2.bHMI_Status);
		RegVariable(_T("HMI_bBackDoorLockSnr"), &m_stBackDoorLockSnr.bHMI_Status);
		RegVariable(_T("HMI_bLowerBackDoorSnr01NC"), &m_stLowerBackDoorSnr01NC.bHMI_Status);
		RegVariable(_T("HMI_bLowerBackDoorSnr01NO"), &m_stLowerBackDoorSnr01NO.bHMI_Status);
		RegVariable(_T("HMI_bLowerBackDoorSnr02NC"), &m_stLowerBackDoorSnr02NC.bHMI_Status);
		RegVariable(_T("HMI_bLowerBackDoorSnr02NO"), &m_stLowerBackDoorSnr02NO.bHMI_Status);
		RegVariable(_T("HMI_bLowerDoorSnr01NC"), &m_stLowerDoorSnr01NC.bHMI_Status);
		RegVariable(_T("HMI_bLowerDoorSnr01NO"), &m_stLowerDoorSnr01NO.bHMI_Status);
		RegVariable(_T("HMI_bLowerDoorSnr02NC"), &m_stLowerDoorSnr02NC.bHMI_Status);
		RegVariable(_T("HMI_bLowerDoorSnr02NO"), &m_stLowerDoorSnr02NO.bHMI_Status);
		RegVariable(_T("HMI_bInspLDoorSnrNO"), &m_stInspLDoorSnrNO.bHMI_Status);
		RegVariable(_T("HMI_bInspLDoorSnrNC"), &m_stInspLDoorSnrNC.bHMI_Status);
		RegVariable(_T("HMI_bInspRDoorSnrNO"), &m_stInspRDoorSnrNO.bHMI_Status);
		RegVariable(_T("HMI_bInspRDoorSnrNC"), &m_stInspRDoorSnrNC.bHMI_Status);
		
		RegVariable(_T("HMI_bFrontDoorLockSol"), &m_stFrontDoorLockSol.bHMI_Status);
		RegVariable(_T("HMI_bFrontDoorLockSol2"), &m_stFrontDoorLockSol2.bHMI_Status);
		RegVariable(_T("HMI_bBackDoorLockSol"), &m_stBackDoorLockSol.bHMI_Status);
		RegVariable(_T("HMI_bShBufDoorLockSol"), &m_stShBufDoorLockSol.bHMI_Status);

		//RegVariable(_T("HMI_bHTPwrOnSnr"), &m_stHTPwrOnSnr.bHMI_Status);
		RegVariable(_T("HMI_bPowerOff"), &m_stPwrOFF.bHMI_Status);


		RegVariable(_T("HMI_bFH1Snr"), &m_stFH1Snr.bHMI_Status);
		RegVariable(_T("HMI_bFH2Snr"), &m_stFH2Snr.bHMI_Status);
		RegVariable(_T("HMI_bFL1Snr"), &m_stFL1Snr.bHMI_Status);
		RegVariable(_T("HMI_bFL2Snr"), &m_stFL2Snr.bHMI_Status);
		RegVariable(_T("HMI_bSPSnr"), &m_stSPSnr.bHMI_Status);

		RegVariable(_T("HMI_bStartButtonSnr"), &m_stStartButtonSnr.bHMI_Status);
		RegVariable(_T("HMI_bStopButtonSnr"), &m_stStopButtonSnr.bHMI_Status);
		RegVariable(_T("HMI_bResetButtonSnr"), &m_stResetButtonSnr.bHMI_Status);
		
		RegVariable(_T("HMI_bStartButtonSol"), &m_stStartButtonSol.bHMI_Status);
		RegVariable(_T("HMI_bResetButtonSol"), &m_stResetButtonSol.bHMI_Status);

		RegVariable(_T("HMI_bShBufferWHPanelOutBtnSnr"), &m_stShBufPanelOutBtnSnr.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHEmptyWhOutBtnSnr"), &m_stShBufEmptyWhOutBtnSnr.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHVac1BtnSnr"), &m_stShBufVac1BtnSnr.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHVac2BtnSnr"), &m_stShBufVac2BtnSnr.bHMI_Status);

		RegVariable(_T("HMI_bRedLight"), &m_stRedLight.bHMI_Status);
		RegVariable(_T("HMI_bOrangeLight"), &m_stOrangeLight.bHMI_Status);
		RegVariable(_T("HMI_bGreenLight"), &m_stGreenLight.bHMI_Status);
		RegVariable(_T("HMI_bBuzzer"), &m_stBuzzer.bHMI_Status);

		RegVariable(_T("HMI_bIonizerDischargeEnable"), &m_stIonizerDischargeEnable.bHMI_Status);
		RegVariable(_T("HMI_bIonizerMaintenanceCheck"), &m_stIonizerMaintenanceCheck.bHMI_Status);
		RegVariable(_T("HMI_bIonizerMaintenanceResult"), &m_stIonizerMaintenanceResult.bHMI_Status);
		RegVariable(_T("HMI_bIonizerError"), &m_stIonizerError.bHMI_Status);

		RegVariable(_T("HMI_bShBufferWHPanelOutBtnLamp"), &m_stShBufPanelOutBtnLamp.bHMI_Status);
		RegVariable(_T("HMI_bShBufferWHEmptyWhOutBtnLamp"), &m_stShBufEmptyWhOutBtnLamp.bHMI_Status);

		// New User enter PR center
		RegVariable(_T("HK_bShowUserSearchPRDone"), &g_bShowUserSearchPRDone);
		// Display the message when bonding
		RegVariable(_T("HK_szBondingMsg"), &g_szBondingMsg);

		// -------------------------------------------------------
		// package information
		// -------------------------------------------------------

		RegVariable(_T("PKG_Glass.szName"), &g_stGlass.szName);
		RegVariable(_T("PKG_Glass.dLength"), &g_stGlass.dLength);
		RegVariable(_T("PKG_Glass.dWidth"), &g_stGlass.dWidth);
		RegVariable(_T("PKG_Glass.dThickness1"), &g_stGlass.dThickness1);
		RegVariable(_T("PKG_Glass.dThickness2"), &g_stGlass.dThickness2);
		RegVariable(_T("PKG_Glass.dThickness3"), &g_stGlass.dThickness3);
		RegVariable(_T("PKG_Glass.dThickness4"), &g_stGlass.dThickness4);

		RegVariable(_T("PKG_Glass.dItoWidth"), &g_stGlass.dItoWidth);
		
		// Rework Mode
		RegVariable(_T("HK_bSelectedReworkMode"), &g_bSelectedReworkMode);
		RegVariable(_T("HK_bIndexTeflonTest"), &HMI_bIndexTeflonTest);

		//Glass input Mode 20120412
		RegVariable(_T("Housekeeping_lGlassInputMode"), &g_lGlassInputMode);
		// LST On Glass Related
		
		// MB Press Shift 

		RegVariable(_T("HMI_bStopProcess"), &HMI_bStopProcess); //20121225
		// -------------------------------------------------------
		// commands
		// -------------------------------------------------------
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleRedSol", HMI_ToggleRedSol);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleOrangeSol", HMI_ToggleOrangeSol);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleGreenSol", HMI_ToggleGreenSol);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleBuzzerSol", HMI_ToggleBuzzerSol);
	
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_AutoTestStop", HMI_PRAutoTestStop); //20140923
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_StopMoveTest", HMI_StopMoveTest);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_StopSearchDLnULTest", HMI_StopSearchDLnULTest); 
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleResetCounterSol", HMI_ToggleResetCounterSol);	
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleStartBtnSol", HMI_ToggleStartBtnSol);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleResetBtnSol", HMI_ToggleResetBtnSol);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleFrontDoorLockSol", HMI_ToggleFrontDoorLockSol);	
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleFrontDoorLockSol2", HMI_ToggleFrontDoorLockSol2);	
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleBackDoorLockSol", HMI_ToggleBackDoorLockSol);	
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleShBufDoorLockSol", HMI_ToggleShBufDoorLockSol);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_TogglePanelOutBtnLamp", HMI_TogglePanelOutBtnLamp);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleEmptyWhOutBtnLamp", HMI_ToggleEmptyWhOutBtnLamp);		
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleIonizerMaintenanceCheckSol", HMI_ToggleIonizerMaintenanceCheckSol);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ToggleIonizerDischargeEnableSol", HMI_ToggleIonizerDischargeEnableSol);		


		// -------------------------------------------------------
		// Setting
		// -------------------------------------------------------
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_BackUpPackages", HMI_BackUpPackages);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ChangePassword", HMI_ChangePassword);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_LangSwitching", HMI_LangSwitching);

		m_comServer.IPC_REG_SERVICE_COMMAND("HK_OperatorLogin", HMI_OperatorLogin); //20150211
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_EngineerLogin", HMI_EngineerLogin);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ServiceLogin", HMI_ServiceLogin);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_DeveloperLogin", HMI_DeveloperLogin);

		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetAutoMode", HMI_SetAutoMode);

		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetDetectCover", HMI_SetDetectCover);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetDetectAirSupply", HMI_SetDetectAirSupply);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetSlowMotionIndexing", HMI_SetSlowMotionIndexing);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetSlowMotionFactor", HMI_SetSlowMotionFactor);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetEnableTrigLighting", HMI_SetEnableTrigLighting);

		m_comServer.IPC_REG_SERVICE_COMMAND("HK_CheckUserGroup", HMI_CheckUserGroup);

		// -------------------------------------------------------
		// package information
		// -------------------------------------------------------
		// Glass Related
		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetGlassName", HMI_SetGlassName);
		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetGlassLength", HMI_SetGlassLength);
		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetGlassWidth", HMI_SetGlassWidth);
		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetReflectorThickness", HMI_SetReflectorThickness);
		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetUpperGlassThickness", HMI_SetUpperGlassThickness);
		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetLowerGlassThickness", HMI_SetLowerGlassThickness);
		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetPolarizerThickness", HMI_SetPolarizerThickness);

		m_comServer.IPC_REG_SERVICE_COMMAND("PKG_SetGlassItoWidth", HMI_SetGlassItoWidth);
		//Rework
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SelectedReworkMode", HMI_SelectedReworkMode);
		
		//Glass input mode 20120412
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_SetGlassInputMode", HMI_SetGlassInputMode);
		
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_LightAndBuzzerInIdle", HMI_LightAndBuzzerInIdle); //20130115

//p20120918
		//Traditional Chinese Language Setting
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_HMILangTraditionalChinese", HMILangTraditionalChinese);
//p20120918:end
#ifdef GATEWAY
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_CheckGatewayConnection", CheckGatewayConnection);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_ProcessGatewayCommand", ProcessGatewayCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_UpdateMachineStatus", GatewayUpdateCOF902MachineStatus);
		m_comServer.IPC_REG_SERVICE_COMMAND("HK_UpdateFX9000OutWHStatus", GatewayUpdateCOF902TA5Status);
#endif
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

/////////////////////////////////////////////////////////////////
//Modified Virtual Operational Functions
/////////////////////////////////////////////////////////////////
VOID CHouseKeeping::PreStartOperation()
{
	m_bErrorExist = FALSE; //20130115
	m_bAlertExist = FALSE;
	m_szLastError = "";	// 20140814 Yip
	SetRedSol(OFF);
	SetOrangeSol(OFF);
	SetGreenSol(ON);
	SetBuzzerSol(OFF);
	m_bShBufDoorClose = (IsInspLDoorSnrOn() /*&& IsInspRDoorSnrOn()*/);
	m_bPanelOutReq = FALSE;
	m_bEmptyOutReq = FALSE;

	g_bStopCycle = FALSE; //20130412
	
	m_bEnableStartBtn	= FALSE;
	m_bEnableResetBtn	= FALSE;
	m_bEnableStopBtn	= TRUE;
	m_bPressBtnToStop   = FALSE;
	
	SetResetBtnSol(OFF);

	Motion(FALSE);
	State(IDLE_Q);
	CPI9000Stn::PreStartOperation();
}

VOID CHouseKeeping::AutoOperation()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	static BOOL bPrevAlertState = FALSE;

	if (Command() == glSTOP_COMMAND || g_bStopCycle)
	{
		m_bEnableStartBtn	= TRUE;
		m_bEnableResetBtn	= TRUE;
		m_bEnableStopBtn	= FALSE;
		State(STOPPING_Q);
		SetResetBtnSol(ON);
	}
	else
	{
		OperationSequence();

		// Handle error
		if (m_bErrorExist)
		{
			bPrevAlertState = FALSE; //20140704
			SetRedSol(ON);
			SetOrangeSol(OFF);
			SetGreenSol(OFF);
			SetBuzzerSol(ON);
		}
		else if (m_bAlertExist)
		{
			if (!bPrevAlertState) //20140704
			{
				bPrevAlertState = TRUE;
				m_bBeepSound = TRUE;
			}
			SetRedSol(ON);
			SetOrangeSol(OFF);
			SetGreenSol(OFF);
			if (m_bBeepSound)
			{
				SetBuzzerSol(ON);
			}
			Sleep(300);

			SetRedSol(OFF);
			SetOrangeSol(OFF);
			SetGreenSol(OFF);
			SetBuzzerSol(OFF);
			Sleep(100);
		}
		else
		{
			bPrevAlertState = FALSE; //20140704
			SetRedSol(OFF);
			SetOrangeSol(OFF);
			SetGreenSol(ON);
			SetBuzzerSol(OFF);
		}

		SetPanelOutLamp(m_nPanelOutLampStatus);
		SetEmptyOutLamp(m_nEmptyOutLampStatus);
		
		Sleep(100);

	}
}

VOID CHouseKeeping::StopOperation()
{
	BOOL bErrorExit = FALSE;
	//Sleep(100);

	bErrorExit = m_bErrorExist;
	m_bErrorExist = FALSE;
	m_bAlertExist = FALSE;
	//m_szLastError = "";	// 20140814 Yip

	if (bErrorExit)
	{
		SetRedSol(ON);
		SetOrangeSol(OFF);
		SetGreenSol(OFF);
		SetBuzzerSol(OFF);
	}
	else
	{
		SetRedSol(OFF);
		SetOrangeSol(ON);
		SetGreenSol(OFF);
		SetBuzzerSol(OFF);
	}
	CPI9000Stn::StopOperation();
}

/////////////////////////////////////////////////////////////////
//Operational Sequences
/////////////////////////////////////////////////////////////////
VOID CHouseKeeping::OperationSequence()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CShBufferWH *pCShBufferWH = (CShBufferWH*)m_pModule->GetStation("ShBufferWH"); 

	if (!IsBurnInDiagBond())
	{
		clock_t clkNow2;
		static clock_t clkToggleVac = 0;
		static clock_t clkToggleVac2 = 0;
		m_bShBufDoorClose = (IsInspLDoorSnrOn() /*&& IsInspRDoorSnrOn()*/);
		if (IsPushShBufVac1BtnOn() && !m_bShBufDoorClose)
		{
			clkNow2 = clock();
			if (((double)(clkNow2 - clkToggleVac) / CLOCKS_PER_SEC) > 1.0)
			{
				clkToggleVac = clock();
				if (pCShBufferWH->m_stGlass1VacSol.bHMI_Status)
				{
					pCShBufferWH->SetGLASS1_VacSol(OFF);
				}
				else
				{
					pCShBufferWH->SetGLASS1_VacSol(ON);
				}
			}
		}


		if (IsPushShBufVac2BtnOn() && !m_bShBufDoorClose)
		{
			clkNow2 = clock();
			if (((double)(clkNow2 - clkToggleVac) / CLOCKS_PER_SEC) > 1.0)
			{
				clkToggleVac2 = clock();
				if (pCShBufferWH->m_stGlass2VacSol.bHMI_Status)
				{
					pCShBufferWH->SetGLASS2_VacSol(OFF);
				}
				else
				{
					pCShBufferWH->SetGLASS2_VacSol(ON);
				}
			}
		}

		if (IsPushShBufPanelOutBtnOn() && !m_bPanelOutReq && !m_bEmptyOutReq)
		{
			m_bPanelOutReq = TRUE;
			//SetPanelOutLamp(LIGHT_ON);
		}
		if (m_bPanelOutReq && !pCShBufferWH->m_bGlass1Exist && !pCShBufferWH->m_bGlass1Exist)
		{
			SetPanelOutLamp(LIGHT_FLASH);
		}
		else if (m_bPanelOutReq && pCShBufferWH->m_bGlass1Exist && pCShBufferWH->m_bGlass1Exist && !m_stShBufPanelOutBtnLamp.bHMI_Status)
		{
			SetPanelOutLamp(LIGHT_ON);
		}

		if (IsPushShBufEmptylOutBtnOn() && !m_bEmptyOutReq  && !m_bPanelOutReq)
		{
			m_bEmptyOutReq = TRUE;
			//SetEmptyOutLamp(LIGHT_ON);
		}
		if (m_bEmptyOutReq && m_stShBufDoorLockSol.bHMI_Status)
		{
			SetEmptyOutLamp(LIGHT_FLASH);
			Sleep(100);
		}
		else if (m_bEmptyOutReq && !m_stShBufEmptyWhOutBtnLamp.bHMI_Status)
		{
			SetEmptyOutLamp(LIGHT_ON);
		}

	}

#if 1 //20121206
	if (m_bDetectCoverStopOp)
	{
		if (!IsFrontDoorLockOn() || !IsFrontDoorLock2On())
		{
			m_bErrorExist = TRUE;
			SetError(IDS_HK_FRONT_COVER_NOT_DETECTED_ERR);
			Command(glSTOP_COMMAND, TRUE);
			g_bStopCycle = TRUE;
			DisplayMessage("FrontCover");
		}
		if (!IsBackDoorLockOn() || !IsLowerBackDoorSnr01On() || !IsLowerBackDoorSnr02On())
		{
			m_bErrorExist = TRUE;
			SetError(IDS_HK_BACK_COVER_NOT_DETECTED_ERR);
			Command(glSTOP_COMMAND, TRUE);
			g_bStopCycle = TRUE;
			DisplayMessage("BackDoor");
		}
		if (!IsLowerDoorSnr01On() || !IsLowerDoorSnr02On())
		{
			m_bErrorExist = TRUE;
			SetError(IDS_HK_LOWER_COVER_NOT_DETECTED_ERR);
			Command(glSTOP_COMMAND, TRUE);
			g_bStopCycle = TRUE;
			DisplayMessage("LowerCover");
		}
	}
#endif

	if (m_bDetectCoverSlowMotion)
	{
		if (!IsFrontDoorLockOn() || !IsFrontDoorLock2On() || !IsBackDoorLockOn() || !IsLowerBackDoorSnr01On() || !IsLowerBackDoorSnr02On() || !IsLowerDoorSnr01On() || !IsLowerDoorSnr02On())
		{
			SetMotionProfileMode(SLOW_MOT_PROF);
		}
		else
		{
			SetMotionProfileMode(NORMAL_MOT_PROF);
		}
	}
	if (IsStopBtnOn() && m_bEnableStopBtn)
	{
		m_bEnableStartBtn	= FALSE;
		m_bEnableResetBtn	= FALSE;
		m_bEnableStopBtn	= FALSE;
		m_bPressBtnToStop	= TRUE;
		StopBonding();
	}
}

VOID CHouseKeeping::SetMotionProfileMode(ULONG ulMode)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	INT i = 0;
	INT j = 0;
	
	for (i = 0; i < pAppMod->GetStationList().GetSize(); i++)
	{
		for (j = 0; j < pAppMod->GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)
		{
			pAppMod->GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stMotionProfile.sMotionProfID = (SHORT)ulMode;
		}
	}
}

/////////////////////////////////////////////////////////////////
//I/O Related
/////////////////////////////////////////////////////////////////
BOOL CHouseKeeping::IsAirSupplyOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stAirSupplySnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}


BOOL CHouseKeeping::IsEMODetectSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif
//return FALSE;  //20140406 debug only
	try
	{
		bResult = m_stEMODetectSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		bResult = TRUE;	// 20140910 Yip: Assume EMO Is Pressed If Read I/O Failed
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsMainPowerFaultSnrOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stMainPowerFaultSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsFrontDoorLockOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stFrontDoorLockSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsFrontDoorLock2On() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stFrontDoorLockSnr2.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsBackDoorLockOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stBackDoorLockSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerDoorSnr01On()
{
	return (!IsLowerDoorSnr01NCOn() && IsLowerDoorSnr01NOOn());
}

BOOL CHouseKeeping::IsLowerDoorSnr02On()
{
	return (!IsLowerDoorSnr02NCOn() && IsLowerDoorSnr02NOOn());
}

BOOL CHouseKeeping::IsLowerBackDoorSnr01On()
{
	return (!IsLowerBackDoorSnr01NCOn() && IsLowerBackDoorSnr01NOOn());
}

BOOL CHouseKeeping::IsLowerBackDoorSnr02On()
{
	return (!IsLowerBackDoorSnr02NCOn() && IsLowerBackDoorSnr02NOOn());
}

BOOL CHouseKeeping::IsInspRDoorSnrOn()
{
	return (!IsInspRDoorSnrNOOn() && IsInspRDoorSnrNCOn());
}

BOOL CHouseKeeping::IsInspLDoorSnrOn()
{
	return (!IsInspLDoorSnrNOOn() && IsInspLDoorSnrNCOn());
}

BOOL CHouseKeeping::IsLowerBackDoorSnr01NCOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerBackDoorSnr01NC.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerBackDoorSnr01NOOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerBackDoorSnr01NO.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerBackDoorSnr02NCOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerBackDoorSnr02NC.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerBackDoorSnr02NOOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerBackDoorSnr02NO.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerDoorSnr01NCOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerDoorSnr01NC.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerDoorSnr01NOOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerDoorSnr01NO.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerDoorSnr02NCOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerDoorSnr02NC.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsLowerDoorSnr02NOOn() //20121206
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stLowerDoorSnr02NO.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsInspLDoorSnrNOOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stInspLDoorSnrNO.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsInspLDoorSnrNCOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stInspLDoorSnrNC.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsInspRDoorSnrNOOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stInspRDoorSnrNO.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsInspRDoorSnrNCOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stInspRDoorSnrNC.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsPushShBufVac1BtnOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stShBufVac1BtnSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsPushShBufVac2BtnOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stShBufVac2BtnSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsPushShBufPanelOutBtnOn() 
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stShBufPanelOutBtnSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsPushShBufEmptylOutBtnOn() 
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stShBufEmptyWhOutBtnSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsStartBtnOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stStartButtonSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsResetBtnOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stResetButtonSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

BOOL CHouseKeeping::IsStopBtnOn()
{
	BOOL bResult = FALSE;

#ifdef OFFLINE
	return bResult;
#endif 

#ifdef _DEBUG	// 20140917 Yip
	if (!theApp.IsInitNuDrive())
	{
		return bResult;
	}
#endif

	try
	{
		bResult = m_stStopButtonSnr.GetGmpPort().IsOn();
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bResult;
}

INT CHouseKeeping::SetRedSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stRedLight;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetOrangeSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stOrangeLight;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetGreenSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stGreenLight;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetBuzzerSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stBuzzer;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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


INT CHouseKeeping::SetResetCounterSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stResetCounterSol;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetStartBtnSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stStartButtonSol;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetResetBtnSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stResetButtonSol;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetShBufDoorLockSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stShBufDoorLockSol;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetFrontDoorLockSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stFrontDoorLockSol;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetFrontDoorLockSol2(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stFrontDoorLockSol2;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetIonizerDischargeEnableSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stIonizerDischargeEnable;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetIonizerMaintenanceCheckSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stIonizerMaintenanceCheck;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetBackDoorLockSol(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;
	CSoInfo *stTempSo = &m_stBackDoorLockSol;

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
			stTempSo->GetGmpPort().On();
			stTempSo->bHMI_Status = TRUE;
		}
		else
		{
			stTempSo->GetGmpPort().Off();
			stTempSo->bHMI_Status = FALSE;
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

INT CHouseKeeping::SetPanelOutBtnLamp(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stShBufPanelOutBtnLamp;

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

INT CHouseKeeping::SetEmptyWhOutBtnLamp(BOOL bMode, BOOL bWait)
{
	INT nResult = GMP_SUCCESS;

	CSoInfo *stTempSo = &m_stShBufEmptyWhOutBtnLamp;

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

VOID CHouseKeeping::SetPanelOutLamp(LampStatus lLampStatus)
{
	switch (lLampStatus)
	{
	case LIGHT_OFF:
		SetPanelOutBtnLamp(OFF);
		break;

	case LIGHT_FLASH:
		SetPanelOutBtnLamp(ON);
		Sleep(300);
		SetPanelOutBtnLamp(OFF);
		Sleep(100);
		break;
	case LIGHT_ON:
		SetPanelOutBtnLamp(ON);
		break;	
	}
}

VOID CHouseKeeping::SetEmptyOutLamp(LampStatus lLampStatus)
{
	switch (lLampStatus)
	{
	case LIGHT_OFF:
		SetEmptyWhOutBtnLamp(OFF);
		break;

	case LIGHT_FLASH:
		SetEmptyWhOutBtnLamp(ON);
		Sleep(300);
		SetEmptyWhOutBtnLamp(OFF);
		Sleep(100);
		break;
	case LIGHT_ON:
		SetEmptyWhOutBtnLamp(ON);
		break;	
	}
}

DOUBLE CHouseKeeping::GetSoftwareVersion(CString Number)
{
	double result;

	result = (double)atof(Number.Right(5));
	
	return result;
}


#ifdef GATEWAY
LONG CHouseKeeping::CheckGatewayConnection(IPC_CServiceMessage &svMsg)
{
#if 1
	LONG lMsgLength = 0;
	CString szResult = _T(" "), szMsg = _T(" ");
	IPC_CServiceMessage svRpyMsg;
	LONG lRpyNum = gnAMS_OK;

	if (getGatewayString(svMsg, lMsgLength, szResult) == gnAMS_OK)
	{
		szMsg.Format("CheckGatewayConnection: Received Msg=%s", szResult);
		DisplayMessage(szMsg);

		//Reply integer
		svRpyMsg.InitMessage(sizeof(LONG), &lRpyNum);
		m_comServer.WriteReply(CString("HK_CheckGatewayConnection"), svRpyMsg);
		return 1;
	}
	return 0;
#else
	CString szMsg = _T("");
	INT nMsgLen = 0;
	IPC_CServiceMessage svRpyMsg;
	LONG lRpyNum = gnAMS_OK;
	char *pBuffer = NULL;
	
	//DisplayMessage("xxxx GatewayUpdateCOF902TA1Status xxxx");
	nMsgLen = svMsg.GetMsgLen();

	if (nMsgLen > 200 || nMsgLen == 0)
	{
		DisplayMessage("CheckGatewayConnection, MsgLen error");
	}
	else
	{
		pBuffer = new char[nMsgLen + 1];

		if (pBuffer != NULL)
		{
			memset(pBuffer, 0, nMsgLen + 1);
			*(pBuffer + nMsgLen) = '\0';
			svMsg.GetMsg(nMsgLen, pBuffer);
			
			szMsg.Format("COF902_CheckConnection String= %s", pBuffer);
			DisplayMessage(szMsg);

			svRpyMsg.InitMessage(sizeof(LONG), &lRpyNum);
			m_comServer.WriteReply(CString("HK_UpdateCOF902TA1Status"), svRpyMsg);

			delete[] pBuffer;
		}
	}

	return 1;	
#endif
}

LONG CHouseKeeping::GatewayUpdateCOF902MachineStatus(IPC_CServiceMessage &svMsg)
{
	CString szMsg = _T("");
	INT nMsgLen = 0;
	IPC_CServiceMessage svRpyMsg;
	//LONG lRpyNum = m_lGatewayReplayNo;
	
	//DisplayMessage("xxxx GatewayUpdateCOF902MachineStatus xxxx");
	nMsgLen = svMsg.GetMsgLen();

	if (nMsgLen != sizeof(LONG))
	{
		DisplayMessage("GatewayUpdateCOF902MachineStatus, MsgLen error");
	}
	else
	{
		svMsg.GetMsg(nMsgLen, &COF902_Machine_Status);

		if (COF902_Machine_Status == ST_GW_MACHINE_READY)
		{
			szMsg.Format("COF902_Machine_Status = ST_GW_MACHINE_READY");
		}
		else if (COF902_Machine_Status == ST_GW_MACHINE_STOP)
		{
			szMsg.Format("COF902_Machine_Status = ST_GW_MACHINE_STOP");
		}
		else
		{
			szMsg.Format("COF902_Machine_Status = ERROR!!");
		}

		DisplayMessage(szMsg);
		if (checkReceivedStatus(COF902_Machine_Status) != gnAMS_OK)
		{
			DisplayMessage("COF902 Machine State Received is invalid");
		}
	}

	return 1;
}

#define LENGTH_GLASSID	16		//20170406

LONG CHouseKeeping::GatewayUpdateCOF902TA5Status(IPC_CServiceMessage &svMsg)
{
	CString szMsg = _T("");
	INT nMsgLen = 0;
	IPC_CServiceMessage svRpyMsg;
	//LONG lRpyNum = m_lGatewayReplayNo;
	
	//DisplayMessage("xxxx GatewayUpdateCOF902TA5Status xxxx");
	nMsgLen = svMsg.GetMsgLen();

#if 1 //20170406
	struct
	{
		LONG lStatus;
		char cGlass1_ID[LENGTH_GLASSID];
		char cCOF1_ID[LENGTH_GLASSID];
		char cGlass2_ID[LENGTH_GLASSID];
		char cCOF2_ID[LENGTH_GLASSID];
	} stGlassInfo;
	for (INT ii = 0; ii < LENGTH_GLASSID; ii++)
	{
		stGlassInfo.cGlass1_ID[ii] = '\0';
		stGlassInfo.cGlass2_ID[ii] = '\0';
		stGlassInfo.cCOF1_ID[ii] = '\0';
		stGlassInfo.cCOF2_ID[ii] = '\0';
	}

	svMsg.GetMsg(nMsgLen, &stGlassInfo);
	COF902_OUTPICKARM_Status = (GATEWAY_STATUS_TYPE) stGlassInfo.lStatus;
	m_stDeviceID.szGlass_ID[GLASS1].Format("%s", stGlassInfo.cGlass1_ID);
	m_stDeviceID.szGlass_ID[GLASS2].Format("%s", stGlassInfo.cGlass2_ID);
	m_stDeviceID.szCOF_ID[GLASS1].Format("%s", stGlassInfo.cCOF1_ID);
	m_stDeviceID.szCOF_ID[GLASS2].Format("%s", stGlassInfo.cCOF2_ID);

	szMsg.Format("COF902_OUTPICKARM_Status = %ld", (LONG) COF902_OUTPICKARM_Status);
	DisplayMessage(szMsg);

	szMsg.Format("Gateway G1_ID = %s, COF1_ID = %s, G2_ID = %s, COF2_ID = %s", 
		m_stDeviceID.szGlass_ID[GLASS1].GetString(),
		m_stDeviceID.szCOF_ID[GLASS1].GetString(),
		m_stDeviceID.szGlass_ID[GLASS2].GetString(),
		m_stDeviceID.szCOF_ID[GLASS2].GetString());
	DisplayMessage(szMsg);
#else
	if (nMsgLen != sizeof(LONG))
	{
		DisplayMessage("GatewayUpdateCOF902TA5Status, MsgLen error");
	}
	else
	{
		svMsg.GetMsg(nMsgLen, &COF902_OUTPICKARM_Status);

		szMsg.Format("COF902_OUTPICKARM_Status = %ld", (LONG) COF902_OUTPICKARM_Status);
		DisplayMessage(szMsg);

	}
#endif
	return 1;
}

INT CHouseKeeping::getGatewayString(IPC_CServiceMessage &svMsg, LONG &lMsgLength, CString &szResult, LONG lMaxMsgLength)
{
	CString szMsg = _T("");
	INT nMsgLen = 0;
	char *pBuffer = NULL;
	
	nMsgLen = svMsg.GetMsgLen();

	if (nMsgLen + 1 > lMaxMsgLength || nMsgLen == 0)
	{
		DisplayMessage("getGatewayString, MsgLen error");
		return gnAMS_NOTOK;
	}
	else
	{
		pBuffer = new char[nMsgLen + 1];

		if (pBuffer != NULL)
		{
			memset(pBuffer, 0, nMsgLen + 1);
			*(pBuffer + nMsgLen) = '\0';
			svMsg.GetMsg(nMsgLen, pBuffer);
			
			//szMsg.Format("getGatewayString String= %s", pBuffer);
			//DisplayMessage(szMsg);
			szResult.Format("%s", pBuffer);

			delete[] pBuffer;
			return gnAMS_OK;
		}
	}
	return gnAMS_NOTOK;
}

INT CHouseKeeping::getGatewayLong(IPC_CServiceMessage &svMsg, LONG &lMsgLength, LONG &lResult)
{
	//CString szMsg = _T("");
	INT nMsgLen = 0;
	
	nMsgLen = svMsg.GetMsgLen();

	if (nMsgLen != sizeof(LONG) || nMsgLen == 0)
	{
		DisplayMessage("getGatewayLong, MsgLen error");
		return gnAMS_NOTOK;
	}
	else
	{
		svMsg.GetMsg(nMsgLen, &lResult);
		//szMsg.Format("getGatewayString String= %s", pBuffer);
		//DisplayMessage(szMsg);
		return gnAMS_OK;
	}
	return gnAMS_NOTOK;
}

LONG CHouseKeeping::ProcessGatewayCommand(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	LONG lMsgLength = 0;
	CString szResult = _T(" "), szMsg = _T(" ");
	IPC_CServiceMessage svRpyMsg;
	LONG lRpyNum = gnAMS_NOTOK;

	if (getGatewayString(svMsg, lMsgLength, szResult) == gnAMS_OK)
	{
		szMsg.Format("ProcessGatewayCommand: Received Msg=%s", szResult);
		DisplayMessage(szMsg);

		if (pCInPickArm->HMI_bStandAlone)
		{
			HMIMessageBox(MSG_OK, "Gateway Command Received", "Gateway Command From Inline, Please Unselect Stand Alone Mode");
			lRpyNum = gnAMS_NOTOK;
			DisplayMessage("***********************************");
			DisplayMessage("Stand Alone Mode: Receive Command From Inline");
			DisplayMessage("***********************************");

			svRpyMsg.InitMessage(sizeof(LONG), &lRpyNum);
			m_comServer.WriteReply(CString("HK_ProcessGatewayCommand"), svRpyMsg);
			return 1;

		}

		if (szResult.GetLength() > 0)
		{
			DisplayMessage("szResult");
			////Pick glass command
			if (szResult == CString("PI902_LoadGlass1Action01"))
			{
				//load req glass 1
				lRpyNum = pAppMod->LoadGlassAction01Gateway(GLASS1);
			}
			else if (szResult == CString("PI902_LoadGlass2Action01"))
			{
				//load req glass 2
				lRpyNum = pAppMod->LoadGlassAction01Gateway(GLASS2);
			}
			else if (szResult == CString("PI902_LoadGlassBothAction01"))
			{
				//load req glass 1 & 2
				lRpyNum = pAppMod->LoadGlassAction01Gateway(MAX_NUM_OF_GLASS);
			}
			///////////////////////////////////////////////
			else if (szResult == CString("PI902_INSPWH_Z1_GoStandbyLevel"))
			{
				//Load glass 1
				lRpyNum = pAppMod->LoadGlassAction02Gateway(GLASS1);
			}
			else if (szResult == CString("PI902_INSPWH_Z2_GoStandbyLevel"))
			{
				//Load glass 2
				lRpyNum = pAppMod->LoadGlassAction02Gateway(GLASS2);
			}
			else if (szResult == CString("PI902_INSPWH_Z1_Z2_GoStandbyLevel"))
			{
				//Load glass 1 & 2
				lRpyNum = pAppMod->LoadGlassAction02Gateway(MAX_NUM_OF_GLASS);
			}

			else
			{
				lRpyNum = gnAMS_NOTOK;
				DisplayMessage("***********************************");
				DisplayMessage("Unknown Remote Command :" + szResult);
				DisplayMessage("***********************************");
			}
		}

		//Reply integer
		svRpyMsg.InitMessage(sizeof(LONG), &lRpyNum);
		m_comServer.WriteReply(CString("HK_ProcessGatewayCommand"), svRpyMsg);
		return 1;
	}
	return 0;
}

#endif

//////////////////////////////////////////////////
// Print Data
//////////////////////////////////////////////////
BOOL CHouseKeeping::PrintHeader(CString szStation, CString szEvent)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;

	FILE *fp = fopen(_T("D:\\sw\\PI9000\\Data\\") + szEvent + _T(".txt"), "a+");
	if (fp != NULL)
	{
		fprintf(fp, "\nSoftware Version\tFile Name\tStation\tEvent\n"); 
		fprintf(fp, "%s\t%s\t%s\t%s\n", (const char*)pAppMod->m_szSoftVersion, (const char*)pAppMod->m_szDeviceFile, (const char*)szStation, (const char*)szEvent);
		fclose(fp);
	}

	return bResult;
}

BOOL CHouseKeeping::PrintTitle(CString szEvent, CString szTitle[])
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;

	CString szPrintTitle = _T("");

	szPrintTitle = _T("Time\t");

	for (INT i = 0; i < MAX_PRINT_DATA; i++)
	{
		if (szTitle[i] == "")
		{
			break;
		}

		szPrintTitle += szTitle[i] + _T("\t");
	}

	FILE *fp = fopen(_T("D:\\sw\\PI9000\\Data\\") + szEvent + _T(".txt"), "a+");
	if (fp != NULL)
	{
		fprintf(fp, szPrintTitle + _T("\n"));
		fclose(fp);
	}

	return bResult;
}


LONG CHouseKeeping::checkReceivedStatus(GATEWAY_STATUS_TYPE lStatus)
{
	if (lStatus >= ST_GW_MAX || lStatus <= ST_GW_MIN)
	{
		return gnAMS_NOTOK;
	}
	else
	{
		return gnAMS_OK;
	}
}

BOOL CHouseKeeping::PrintData(CString szEvent, PRINT_DATA stPrintData)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szMsg = _T("");
	CString szPrintData = _T("");
	CString szTempData = _T("");

	SYSTEMTIME ltime;		// New Command

	// Get time as 64-bit integer.
	GetLocalTime(&ltime);

	// Convert to local time.
	szPrintData.Format("%04u-%02u-%02u %02u:%02u:%02u\t", 
					   ltime.wYear, ltime.wMonth, ltime.wDay, 
					   ltime.wHour, ltime.wMinute, ltime.wSecond);

	for (INT i = 0; i < MAX_PRINT_DATA; i++)
	{
		if (stPrintData.szTitle[i + 1] == "")
		{
			break;
		}

		szTempData.Format("%.3f\t", stPrintData.dData[i]);

		szPrintData += szTempData;
	}

	FILE *fp = fopen(_T("D:\\sw\\PI9000\\Data\\") + szEvent + _T(".txt"), "a+");
	if (fp != NULL)
	{
#if 1 //20130425
		szMsg.Format("PrintData:\\sw\\PI9000\\Data\\%s", szEvent);
		DisplayMessage(szMsg);

		fseek(fp, 0, SEEK_END);
		if (ftell(fp) < 10)
		{
			for (INT i = 0; i < MAX_PRINT_DATA; i++)
			{
				fprintf(fp, stPrintData.szTitle[i] + "\t");
				if (stPrintData.szTitle[i] == "")
				{
					break;
				}
			}
			fprintf(fp, "\n");
		}
#endif
		fprintf(fp, szPrintData + _T("\n"));
		fclose(fp);
	}

	return bResult;
}

BOOL CHouseKeeping::IsDataFileExist(CString szEvent)	// 20140619 Yip
{
	return (CheckPath(_T("D:\\sw\\PI9000\\Data\\") + szEvent + _T(".txt")) != PATH_NOT_FOUND);
}

VOID CHouseKeeping::PrintMachineSetup(FILE *fp)
{
	CPI9000Stn::PrintMachineSetup(fp);
}

VOID CHouseKeeping::PrintDeviceSetup(FILE *fp)
{
	CPI9000Stn::PrintDeviceSetup(fp);
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
BOOL CHouseKeeping::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;
	CString szStr = _T("");

	if (bResult)
	{
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Settings"]["g_lGlassInputMode"];
		bResult = printMachineInfoRow(szModule, CString("Settings"), CString("GlassInputMode"), szBlank, szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
	}
	
	if (bResult)
	{
		bResult = CPI9000Stn::printMachineDeviceInfo();
	}
	return bResult;
}
#endif

//-----------------------------------------------------------------------------
// HMI command implementation
//-----------------------------------------------------------------------------
//SI/SO
LONG CHouseKeeping::HMI_ToggleRedSol(IPC_CServiceMessage &svMsg)
{
	if (m_stRedLight.bHMI_Status)
	{
		SetRedSol(OFF);
	}
	else
	{
		SetRedSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleOrangeSol(IPC_CServiceMessage &svMsg)
{
	if (m_stOrangeLight.bHMI_Status)
	{
		SetOrangeSol(OFF);
	}
	else
	{
		SetOrangeSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleGreenSol(IPC_CServiceMessage &svMsg)
{
	if (m_stGreenLight.bHMI_Status)
	{
		SetGreenSol(OFF);
	}
	else
	{
		SetGreenSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleBuzzerSol(IPC_CServiceMessage &svMsg)
{
	if (m_stBuzzer.bHMI_Status)
	{
		SetBuzzerSol(OFF);
	}
	else
	{
		SetBuzzerSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleResetCounterSol(IPC_CServiceMessage &svMsg)
{
	if (m_stResetCounterSol.bHMI_Status)
	{
		SetResetCounterSol(OFF);
	}
	else
	{
		SetResetCounterSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleStartBtnSol(IPC_CServiceMessage &svMsg)
{
	if (m_stStartButtonSol.bHMI_Status)
	{
		SetStartBtnSol(OFF);
	}
	else
	{
		SetStartBtnSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleResetBtnSol(IPC_CServiceMessage &svMsg)
{
	if (m_stResetButtonSol.bHMI_Status)
	{
		SetResetBtnSol(OFF);
	}
	else
	{
		SetResetBtnSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleFrontDoorLockSol(IPC_CServiceMessage &svMsg)
{
	if (m_stFrontDoorLockSol.bHMI_Status)
	{
		SetFrontDoorLockSol(OFF);
	}
	else
	{
		SetFrontDoorLockSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleFrontDoorLockSol2(IPC_CServiceMessage &svMsg)
{
	if (m_stFrontDoorLockSol2.bHMI_Status)
	{
		SetFrontDoorLockSol2(OFF);
	}
	else
	{
		SetFrontDoorLockSol2(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleBackDoorLockSol(IPC_CServiceMessage &svMsg)
{
	if (m_stBackDoorLockSol.bHMI_Status)
	{
		SetBackDoorLockSol(OFF);
	}
	else
	{
		SetBackDoorLockSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleShBufDoorLockSol(IPC_CServiceMessage &svMsg)
{
	if (m_stShBufDoorLockSol.bHMI_Status)
	{
		SetShBufDoorLockSol(OFF);
	}
	else
	{
		SetShBufDoorLockSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_TogglePanelOutBtnLamp(IPC_CServiceMessage &svMsg)
{
	if (m_stShBufPanelOutBtnLamp.bHMI_Status)
	{
		SetPanelOutBtnLamp(OFF);
	}
	else
	{
		SetPanelOutBtnLamp(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleEmptyWhOutBtnLamp(IPC_CServiceMessage &svMsg)
{
	if (m_stShBufEmptyWhOutBtnLamp.bHMI_Status)
	{
		SetEmptyWhOutBtnLamp(OFF);
	}
	else
	{
		SetEmptyWhOutBtnLamp(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleIonizerMaintenanceCheckSol(IPC_CServiceMessage &svMsg)
{
	if (m_stIonizerMaintenanceCheck.bHMI_Status)
	{
		SetIonizerMaintenanceCheckSol(OFF);
	}
	else
	{
		SetIonizerMaintenanceCheckSol(ON);
	}

	return 0;
}

LONG CHouseKeeping::HMI_ToggleIonizerDischargeEnableSol(IPC_CServiceMessage &svMsg)
{
	if (m_stIonizerDischargeEnable.bHMI_Status)
	{
		SetIonizerDischargeEnableSol(OFF);
	}
	else
	{
		SetIonizerDischargeEnableSol(ON);
	}

	return 0;
}

// ------------------------------------------------------------------------
// Setting
// ------------------------------------------------------------------------
LONG CHouseKeeping::HMI_BackUpPackages(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	//CLitecCtrl *pCLitecCtrl = (CLitecCtrl*)m_pModule->GetStation("EcoreCtrl");

	CString szpassword = szEngPassword;

	if (HMIGetPassword("Please enter password", szpassword))
	{
		CreateDirectory("D:\\sw\\PI9000\\Backup", 0);
		CopyDir("D:\\sw\\PI9000\\Device", "D:\\sw\\PI9000\\Backup");
		CopyDir("D:\\sw\\PI9000\\MarkData", "D:\\sw\\PI9000\\Backup");
		CopyDir("D:\\sw\\PI9000\\Param", "D:\\sw\\PI9000\\Backup");
		CopyDir("D:\\sw\\PI9000\\Record", "D:\\sw\\PI9000\\Backup");
		
		// Release me later
		//pAppMod->PrintMachineInfo();
		//pCLitecCtrl->PrintEcoreInfomation();
		CopyDir("D:\\sw\\PI9000\\Data", "D:\\sw\\PI9000\\Backup");
	}

	BOOL bDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bDone);
	return 1;
}

LONG CHouseKeeping::HMI_ChangePassword(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString	szPassword = "";
	LONG	lResponse;

	lResponse = HMISelectionBox("CHANGE PASSWORD", "Please select user group", "Engineer", "Service Eng");

	if (lResponse == -1 || lResponse == 11)
	{
		HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "Operation Abort!");
		return 0;
	}

	switch (lResponse)
	{
	case 0:
		szPassword = szEngPassword;
		break;

	case 1:
		szPassword = szSvrPassword;
		break;
	}

	if (HMIGetPassword("Please enter the current password", szPassword))
	{
		if (HMIAlphaKeys("Please enter the New Password", &szPassword, TRUE))
		{
			if (szPassword == "")
			{
				HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "Password must contain characters. Please enter again!");
				if (HMIAlphaKeys("Please enter the New Password", &szPassword, TRUE))
				{
					if (szPassword == "")
					{
						HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "Operation Terminated.");
						return 0;
					}
				}
				else
				{
					HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "Operation Abort!");
				}
			}
			if (HMIGetPassword("Please confirm the New Password", szPassword))
			{
				DisplayMessage("Saving Machine file...");

				switch (lResponse)
				{
				case 0:
					szEngPassword = szPassword;
					pAppMod->m_smfMachine["HouseKeeping"]["Password"]["szEngPassword"] = szEngPassword;
					HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "*** Engineer usergroup password changed ***");
					break;

				case 1:
					szSvrPassword = szPassword;
					pAppMod->m_smfMachine["HouseKeeping"]["Password"]["szSvrPassword"] = szSvrPassword;
					HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "*** Service Engineer usergroup password changed ***");
					break;
				}
			}
			else
			{
				HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "Operation Abort!");
			}
		}
		else
		{
			HMIMessageBox(MSG_OK, "CHANGE PASSWORD", "Operation Abort!");
		}
	}
	return 0;

}

LONG CHouseKeeping::HMI_LangSwitching(IPC_CServiceMessage &svMsg)
{
	BOOL bResult;

	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	bResult = pAppMod->HMILangSwitching();

	return 0;
}

LONG CHouseKeeping::HMI_OperatorLogin(IPC_CServiceMessage &svMsg) //20150211
{
	SetAutoMode(AUTO_BOND);
	m_bEnableStartBtn	= TRUE;
	return 0;
}

LONG CHouseKeeping::HMI_EngineerLogin(IPC_CServiceMessage &svMsg)
{
	if (g_ulUserGroup == OPERATOR) //password input fail during engineer login. Then user login as operator.
	{
		HMIMessageBox(MSG_OK, "LOGIN", "Auto Bond is Selected");
		SetAutoMode(AUTO_BOND);
	}
	m_bEnableStartBtn	= TRUE;
	return 0;
}

LONG CHouseKeeping::HMI_ServiceLogin(IPC_CServiceMessage &svMsg)
{
	if (g_ulUserGroup == OPERATOR) //password input fail during engineer login. Then user login as operator.
	{
		HMIMessageBox(MSG_OK, "LOGIN", "Auto Bond is Selected");
		SetAutoMode(AUTO_BOND);
	}
	m_bEnableStartBtn	= TRUE;
	return 0;
}

LONG CHouseKeeping::HMI_DeveloperLogin(IPC_CServiceMessage &svMsg)
{
	if (g_ulUserGroup == OPERATOR) //password input fail during engineer login. Then user login as operator.
	{
		HMIMessageBox(MSG_OK, "LOGIN", "Auto Bond is Selected");
		SetAutoMode(AUTO_BOND);
	}
	m_bEnableStartBtn	= TRUE;
	return 0;
}

LONG CHouseKeeping::SetAutoMode(AUTOMODE eAutoMode)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	HMI_lAutoMode = (LONG) eAutoMode;
	AutoMode = (AUTOMODE)HMI_lAutoMode;

	if (pAppMod == NULL)
	{
		return 0;
	}

	if (AutoMode == INDEXING) //20130710
	{
		pAppMod->UnselectModuleForIndexingMode();
	}

	return 0;
}

LONG CHouseKeeping::HMI_SetAutoMode(IPC_CServiceMessage &svMsg)
{
	LONG lMode;
	svMsg.GetMsg(sizeof(LONG), &lMode);

	if (g_ulUserGroup == OPERATOR &&
		(AUTOMODE) lMode != TEST_BOND && (AUTOMODE) lMode != AUTO_BOND) //20150211
	{
		HMIMessageBox(MSG_OK, "SET BOND MODE", "Operator could Select AUTO bond or TEST bond only. Operation Abort!");
		return 0;
	}

	HMI_lAutoMode = lMode;
	AutoMode = (AUTOMODE)HMI_lAutoMode;

#if 1 //20130710
	if (AutoMode == INDEXING)
	{
		CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
		if (pAppMod != NULL)
		{
			pAppMod->UnselectModuleForIndexingMode();
		}
		m_bDisableSelectAll = TRUE;	// 20150515 Dim select all when use indexing mode
	}
	else	// 20150515
	{
		m_bDisableSelectAll = FALSE;
	}
#endif


	if (AutoMode == BURN_IN)	// 20150515 Unselect all PR module when use burninmode
	{
		CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
		for (int i = 0; i < pAppMod->GetPRStationList().GetSize(); i++)
		{
			pAppMod->GetPRStationList().GetAt(i)->SetPRSelected(FALSE);
		}
	}
	return 0;
}

LONG CHouseKeeping::HMI_SetDetectCover(IPC_CServiceMessage &svMsg)
{
	ULONG ulMode;
	svMsg.GetMsg(sizeof(ULONG), &ulMode);

	m_ulDetectCover = ulMode;

	switch (m_ulDetectCover)
	{
	case 0: //slow
		m_bDetectCoverSlowMotion = TRUE;
		m_bDetectCoverStopOp = FALSE;
		break;
	case 1: //stop
		m_bDetectCoverSlowMotion = FALSE;
		m_bDetectCoverStopOp = TRUE;
		break;
	case 2: //disable
		m_bDetectCoverSlowMotion = FALSE;
		m_bDetectCoverStopOp = FALSE;
		break;
	}

	return 0;
}

LONG CHouseKeeping::HMI_SetDetectAirSupply(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	m_bDetectAirSupply = bMode;

	return 0;
}

LONG CHouseKeeping::HMI_SetSlowMotionIndexing(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	INT i = 0;
	INT j = 0;

	g_bSlowMotionIndexing = bMode;

	if (g_bSlowMotionIndexing)
	{
		SetMotionProfileMode(g_ulSlowMotionFactor);
	}
	else
	{
		SetMotionProfileMode(NORMAL_MOT_PROF);
	}

	return 0;
}

LONG CHouseKeeping::HMI_SetSlowMotionFactor(IPC_CServiceMessage &svMsg)
{

	ULONG ulFactor;
	svMsg.GetMsg(sizeof(ULONG), &ulFactor);

	if (ulFactor < MAX_NUM_OF_MOT_PROF)
	{
		g_ulSlowMotionFactor = ulFactor;

		if (g_bSlowMotionIndexing)
		{
			SetMotionProfileMode(g_ulSlowMotionFactor);
		}
	}

	return 0;
}

LONG CHouseKeeping::HMI_SetEnableTrigLighting(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	g_bEnableTrigLighting = bMode;

	return 0;
}

LONG CHouseKeeping::HMI_SetGlassIndexingDelayFactor(IPC_CServiceMessage &svMsg)
{
	// Add me later
	HMIMessageBox(MSG_OK, "KILL ME", "UNDER CONSTRUCT");
	return 0;
}

LONG CHouseKeeping::HMI_CheckUserGroup(IPC_CServiceMessage &svMsg)
{
	BOOL	bStatus = FALSE;

	if (
		(g_ulUserGroup == ENGINEER) ||
		(g_ulUserGroup == SERVICE) ||
		(g_ulUserGroup == DEVELOPER)
	   )
	{
		bStatus = TRUE;
	}
	else
	{
		HMIMessageBox(MSG_OK, "EDIT DEVICE INFORMATION", "Current User Login Group does not have right to edit Device Information!");
	}
	
	svMsg.InitMessage(sizeof(BOOL), &bStatus);
	return 1;
}

// ------------------------------------------------------------------------
// Package Information
// ------------------------------------------------------------------------
LONG CHouseKeeping::HMI_SetGlassName(IPC_CServiceMessage &svMsg)
{
	LONG	lMsgLen = svMsg.GetMsgLen();
	UCHAR	*cName;

	if (lMsgLen > 0)
	{
		cName = new UCHAR[lMsgLen];
		svMsg.GetMsg(cName, lMsgLen);
		g_stGlass.szName = (CString)cName;
	}

	return 0;
}

LONG CHouseKeeping::HMI_SetGlassLength(IPC_CServiceMessage &svMsg)
{

	DOUBLE dLength;
	svMsg.GetMsg(sizeof(DOUBLE), &dLength);

	g_stGlass.dLength = dLength;

	return 0;
}

LONG CHouseKeeping::HMI_SetGlassWidth(IPC_CServiceMessage &svMsg)
{
	DOUBLE dWidth;
	svMsg.GetMsg(sizeof(DOUBLE), &dWidth);

	g_stGlass.dWidth = dWidth;

	return 0;
}
LONG CHouseKeeping::HMI_SetReflectorThickness(IPC_CServiceMessage &svMsg)
{
	DOUBLE dThickness;
	svMsg.GetMsg(sizeof(DOUBLE), &dThickness);

	g_stGlass.dThickness1 = dThickness;

	return 0;
}

LONG CHouseKeeping::HMI_SetUpperGlassThickness(IPC_CServiceMessage &svMsg)
{
	DOUBLE dThickness;
	svMsg.GetMsg(sizeof(DOUBLE), &dThickness);

	g_stGlass.dThickness2 = dThickness;

	return 0;
}

LONG CHouseKeeping::HMI_SetLowerGlassThickness(IPC_CServiceMessage &svMsg)
{
	DOUBLE dThickness;
	svMsg.GetMsg(sizeof(DOUBLE), &dThickness);

	g_stGlass.dThickness3 = dThickness;

	return 0;
}

LONG CHouseKeeping::HMI_SetPolarizerThickness(IPC_CServiceMessage &svMsg)
{
	DOUBLE dThickness;
	svMsg.GetMsg(sizeof(DOUBLE), &dThickness);

	g_stGlass.dThickness4 = dThickness;

	return 0;
}

LONG CHouseKeeping::HMI_SetGlassItoWidth(IPC_CServiceMessage &svMsg)
{
	DOUBLE dWidth = 0.0;

	svMsg.GetMsg(sizeof(DOUBLE), &dWidth);

	g_stGlass.dItoWidth = dWidth;

	return 0;
}


///////////////////////////////////////
// LSI On Glass Info. Related
///////////////////////////////////////


////////////////////////////////////////////////////////
// MB Press Seq.
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// Rework Mode
////////////////////////////////////////////////////////

LONG CHouseKeeping::HMI_SelectedReworkMode(IPC_CServiceMessage &svMsg)
{
	BOOL bResult;
	svMsg.GetMsg(sizeof(BOOL), &bResult);

	g_bSelectedReworkMode = bResult;

	return 0;
}

////////////////////////////////////////////////////////
// Glass Input Mode
//////////////////////////////////////////////////////// 20120412
LONG CHouseKeeping::HMI_SetGlassInputMode(IPC_CServiceMessage &svMsg)
{
	LONG lMode;
	svMsg.GetMsg(sizeof(LONG), &lMode);

	switch ((GLASS_INPUT_MODE) lMode)
	{
	case GLASS_1_INPUT:
		g_lGlassInputMode = GLASS_1_INPUT;
		break;
	case GLASS_2_INPUT:
		g_lGlassInputMode = GLASS_2_INPUT;
		break;
	case GLASS_1_2_INPUT:
	default:
		g_lGlassInputMode = GLASS_1_2_INPUT;
		break;
	}
	return 0;
}

LONG CHouseKeeping::HMI_LightAndBuzzerInIdle(IPC_CServiceMessage &svMsg) //20130115
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	m_bErrorExist = FALSE;
	m_bAlertExist = FALSE;
	//m_szLastError = "";	// 20140814 Yip

	if (pAppMod->IsInitNuDrive())
	{
		SetRedSol(OFF);
		SetOrangeSol(ON);
		SetGreenSol(OFF);
		SetBuzzerSol(OFF);
	}
	return 0;
}
//p20120918
////////////////////////////////////////////////////////
// Traditional Chinese Switching
////////////////////////////////////////////////////////
LONG CHouseKeeping::HMILangTraditionalChinese(IPC_CServiceMessage &svMsg)
{
	BOOL bResult;

	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	bResult = pAppMod->HMILangTraditionalChinese();

	return 0;
}
//p20120918:end


////////////////////////////////////////////////////////
// Load/Save
////////////////////////////////////////////////////////
// Saving / loading parameters
LONG CHouseKeeping::IPC_SaveMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	pAppMod->m_smfMachine[GetStnName()]["Password"]["szEngPassword"]	= szEngPassword;
	pAppMod->m_smfMachine[GetStnName()]["Password"]["szSvrPassword"]	= szSvrPassword;

	pAppMod->m_smfMachine[GetStnName()]["Tools"]["g_stForceCalibrator.dHeight"]		= g_stForceCalibrator.dHeight;

	pAppMod->m_smfMachine["Lighting"]["g_bEnableTrigLighting"]	= g_bEnableTrigLighting; //20141201

	pAppMod->m_smfMachine[GetStnName()]["Settings"]["g_lCalAverageFreq"]			= g_lCalAverageFreq;

	pAppMod->m_smfMachine[GetStnName()]["Settings"]["m_ulDetectCover"]				= m_ulDetectCover;
	pAppMod->m_smfMachine[GetStnName()]["Settings"]["g_lGlassInputMode"]			= g_lGlassInputMode;
	return CPI9000Stn::IPC_SaveMachineParam();
}

LONG CHouseKeeping::IPC_LoadMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	szEngPassword				= pAppMod->m_smfMachine[GetStnName()]["Password"]["szEngPassword"];
	szSvrPassword				= pAppMod->m_smfMachine[GetStnName()]["Password"]["szSvrPassword"];

	g_stForceCalibrator.dHeight = pAppMod->m_smfMachine[GetStnName()]["Tools"]["g_stForceCalibrator.dHeight"];

	m_ulDetectCover				= (LONG)pAppMod->m_smfMachine[GetStnName()]["Settings"]["m_ulDetectCover"];

	g_lGlassInputMode			= pAppMod->m_smfMachine[GetStnName()]["Settings"]["g_lGlassInputMode"];
	g_bEnableTrigLighting		= (BOOL)(LONG)pAppMod->m_smfMachine["Lighting"]["g_bEnableTrigLighting"];  //20141201

	g_lCalAverageFreq			= (LONG)pAppMod->m_smfMachine[GetStnName()]["Settings"]["g_lCalAverageFreq"];

	if (g_lCalAverageFreq == 0)
	{
		g_lCalAverageFreq = 10;
	}
	SetMotionProfileMode(g_ulSlowMotionFactor);

	//stuff
	switch (m_ulDetectCover)
	{
	case 0:
		m_bDetectCoverSlowMotion = TRUE;
		m_bDetectCoverStopOp = FALSE;
		break;
	case 1:
		m_bDetectCoverSlowMotion = FALSE;
		m_bDetectCoverStopOp = TRUE;
		break;
	case 2:
		m_bDetectCoverSlowMotion = FALSE;
		m_bDetectCoverStopOp = FALSE;
		break;
	}

	if (szEngPassword == "")
	{
		szEngPassword = "PI9000";	//should be cog902
	}

	if (szSvrPassword == "")
	{
		szSvrPassword = "a5s2m2";
	}

	return CPI9000Stn::IPC_LoadMachineParam();
}

LONG CHouseKeeping::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	CString str;

	pAppMod->m_smfDevice["Package"]["g_stGlass.szName"]			= g_stGlass.szName;

	pAppMod->m_smfDevice["Package"]["g_stGlass.dLength"]		= g_stGlass.dLength;
	pAppMod->m_smfDevice["Package"]["g_stGlass.dWidth"]			= g_stGlass.dWidth;
	pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness1"]	= g_stGlass.dThickness1;
	pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness2"]	= g_stGlass.dThickness2;
	pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness3"]	= g_stGlass.dThickness3;
	pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness4"]	= g_stGlass.dThickness4;

	pAppMod->m_smfDevice["Package"]["g_stGlass.dItoWidth"]		= g_stGlass.dItoWidth;


	//Setting
	pAppMod->m_smfDevice[GetStnName()]["Settings"]["g_bSafeIndexing"]		= g_bSafeIndexing;
	pAppMod->m_smfDevice[GetStnName()]["Settings"]["g_bSlowMotionIndexing"]	= g_bSlowMotionIndexing;
	pAppMod->m_smfDevice[GetStnName()]["Settings"]["g_ulSlowMotionFactor"]	= g_ulSlowMotionFactor;
	pAppMod->m_smfDevice[GetStnName()]["Settings"]["m_bNewCoverSnr"]	= m_bNewCoverSnr;	// 20150714

	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["bEnable"]					= pCWH1->m_stInspWH[GLASS1].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["bValid"]					= pCWH1->m_stInspWH[GLASS1].bValid;		
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt1.x"]			= pCWH1->m_stInspWH[GLASS1].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt1.y"]			= pCWH1->m_stInspWH[GLASS1].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt2.x"]			= pCWH1->m_stInspWH[GLASS1].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt2.y"]			= pCWH1->m_stInspWH[GLASS1].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["bAlignPtSet"]				= pCWH1->m_stInspWH[GLASS1].bAlignPtSet;

	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["bEnable"]					= pCWH1->m_stInspWH[GLASS2].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["bValid"]					= pCWH1->m_stInspWH[GLASS2].bValid	;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt1.x"]			= pCWH1->m_stInspWH[GLASS2].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt1.y"]			= pCWH1->m_stInspWH[GLASS2].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt2.x"]			= pCWH1->m_stInspWH[GLASS2].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt2.y"]			= pCWH1->m_stInspWH[GLASS2].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["bAlignPtSet"]				= pCWH1->m_stInspWH[GLASS2].bAlignPtSet;

	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["bEnable"]					= pCWH2->m_stInspWH[GLASS1].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["bValid"]					= pCWH2->m_stInspWH[GLASS1].bValid;		
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt1.x"]			= pCWH2->m_stInspWH[GLASS1].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt1.y"]			= pCWH2->m_stInspWH[GLASS1].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt2.x"]			= pCWH2->m_stInspWH[GLASS1].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt2.y"]			= pCWH2->m_stInspWH[GLASS1].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["bAlignPtSet"]				= pCWH2->m_stInspWH[GLASS1].bAlignPtSet;

	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["bEnable"]					= pCWH2->m_stInspWH[GLASS2].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["bValid"]					= pCWH2->m_stInspWH[GLASS2].bValid	;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt1.x"]			= pCWH2->m_stInspWH[GLASS2].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt1.y"]			= pCWH2->m_stInspWH[GLASS2].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt2.x"]			= pCWH2->m_stInspWH[GLASS2].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt2.y"]			= pCWH2->m_stInspWH[GLASS2].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["bAlignPtSet"]				= pCWH2->m_stInspWH[GLASS2].bAlignPtSet;

	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["bEnable"]					= pCInspOpt->m_stULGlass[GLASS1].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["bValid"]					= pCInspOpt->m_stULGlass[GLASS1].bValid;		
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt1.x"]			= pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt1.y"]			= pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt2.x"]			= pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt2.y"]			= pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["bAlignPtSet"]				= pCInspOpt->m_stULGlass[GLASS1].bAlignPtSet;

	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["bEnable"]					= pCInspOpt->m_stULGlass[GLASS2].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["bValid"]					= pCInspOpt->m_stULGlass[GLASS2].bValid	;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt1.x"]			= pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt1.y"]			= pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt2.x"]			= pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt2.y"]			= pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["bAlignPtSet"]				= pCInspOpt->m_stULGlass[GLASS2].bAlignPtSet;

	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["bEnable"]					= pCInspOpt->m_stULCOF[GLASS1].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["bValid"]					= pCInspOpt->m_stULCOF[GLASS1].bValid;		
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt1.x"]			= pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt1.y"]			= pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt2.x"]			= pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt2.y"]			= pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["bAlignPtSet"]				= pCInspOpt->m_stULCOF[GLASS1].bAlignPtSet;

	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["bEnable"]					= pCInspOpt->m_stULCOF[GLASS2].bEnable;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["bValid"]					= pCInspOpt->m_stULCOF[GLASS2].bValid	;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt1.x"]			= pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt1.x;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt1.y"]			= pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt1.y;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt2.x"]			= pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt2.x;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt2.y"]			= pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt2.y;
	pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["bAlignPtSet"]				= pCInspOpt->m_stULCOF[GLASS2].bAlignPtSet;
	
	return CPI9000Stn::IPC_SaveDeviceParam();
}

LONG CHouseKeeping::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	CString str;

	g_stGlass.szName		= pAppMod->m_smfDevice["Package"]["g_stGlass.szName"];

	g_stGlass.dLength		= pAppMod->m_smfDevice["Package"]["g_stGlass.dLength"];
	g_stGlass.dWidth		= pAppMod->m_smfDevice["Package"]["g_stGlass.dWidth"];
	g_stGlass.dThickness1	= pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness1"];
	g_stGlass.dThickness2	= pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness2"];
	g_stGlass.dThickness3	= pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness3"];
	g_stGlass.dThickness4	= pAppMod->m_smfDevice["Package"]["g_stGlass.dThickness4"];

	g_stGlass.dItoWidth		= pAppMod->m_smfDevice["Package"]["g_stGlass.dItoWidth"];

	//Setting
	g_bSafeIndexing			= (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Settings"]["g_bSafeIndexing"];
	g_bSlowMotionIndexing	= (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Settings"]["g_bSlowMotionIndexing"];
	g_ulSlowMotionFactor	= pAppMod->m_smfDevice[GetStnName()]["Settings"]["g_ulSlowMotionFactor"];
	m_bNewCoverSnr	= (BOOL)(LONG)pAppMod->m_smfDevice[GetStnName()]["Settings"]["m_bNewCoverSnr"];	// 20150714

	pCWH1->m_stInspWH[GLASS1].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["bEnable"];
	pCWH1->m_stInspWH[GLASS1].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["bValid"];
	pCWH1->m_stInspWH[GLASS1].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt1.x"];
	pCWH1->m_stInspWH[GLASS1].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt1.y"];
	pCWH1->m_stInspWH[GLASS1].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt2.x"];
	pCWH1->m_stInspWH[GLASS1].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["dmtrAlignPt2.y"];
	pCWH1->m_stInspWH[GLASS1].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS1"]["bAlignPtSet"];

	pCWH1->m_stInspWH[GLASS2].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["bEnable"];
	pCWH1->m_stInspWH[GLASS2].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["bValid"];
	pCWH1->m_stInspWH[GLASS2].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt1.x"];
	pCWH1->m_stInspWH[GLASS2].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt1.y"];
	pCWH1->m_stInspWH[GLASS2].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt2.x"];
	pCWH1->m_stInspWH[GLASS2].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["dmtrAlignPt2.y"];
	pCWH1->m_stInspWH[GLASS2].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH1"]["GLASS2"]["bAlignPtSet"];

	pCWH2->m_stInspWH[GLASS1].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["bEnable"];
	pCWH2->m_stInspWH[GLASS1].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["bValid"];
	pCWH2->m_stInspWH[GLASS1].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt1.x"];
	pCWH2->m_stInspWH[GLASS1].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt1.y"];
	pCWH2->m_stInspWH[GLASS1].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt2.x"];
	pCWH2->m_stInspWH[GLASS1].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["dmtrAlignPt2.y"];
	pCWH2->m_stInspWH[GLASS1].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS1"]["bAlignPtSet"];

	pCWH2->m_stInspWH[GLASS2].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["bEnable"];
	pCWH2->m_stInspWH[GLASS2].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["bValid"];
	pCWH2->m_stInspWH[GLASS2].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt1.x"];
	pCWH2->m_stInspWH[GLASS2].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt1.y"];
	pCWH2->m_stInspWH[GLASS2].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt2.x"];
	pCWH2->m_stInspWH[GLASS2].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["dmtrAlignPt2.y"];
	pCWH2->m_stInspWH[GLASS2].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stInspWH2"]["GLASS2"]["bAlignPtSet"];

	pCInspOpt->m_stULGlass[GLASS1].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["bEnable"];
	pCInspOpt->m_stULGlass[GLASS1].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["bValid"];
	pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt1.x"];
	pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt1.y"];
	pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt2.x"];
	pCInspOpt->m_stULGlass[GLASS1].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["dmtrAlignPt2.y"];
	pCInspOpt->m_stULGlass[GLASS1].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS1"]["bAlignPtSet"];

	pCInspOpt->m_stULGlass[GLASS2].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["bEnable"];
	pCInspOpt->m_stULGlass[GLASS2].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["bValid"];
	pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt1.x"];
	pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt1.y"];
	pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt2.x"];
	pCInspOpt->m_stULGlass[GLASS2].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["dmtrAlignPt2.y"];
	pCInspOpt->m_stULGlass[GLASS2].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULGlass"]["GLASS2"]["bAlignPtSet"];

	pCInspOpt->m_stULCOF[GLASS1].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["bEnable"];
	pCInspOpt->m_stULCOF[GLASS1].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["bValid"];
	pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt1.x"];
	pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt1.y"];
	pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt2.x"];
	pCInspOpt->m_stULCOF[GLASS1].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["dmtrAlignPt2.y"];
	pCInspOpt->m_stULCOF[GLASS1].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS1"]["bAlignPtSet"];

	pCInspOpt->m_stULCOF[GLASS2].bEnable			= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["bEnable"];
	pCInspOpt->m_stULCOF[GLASS2].bValid				= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["bValid"];
	pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt1.x		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt1.x"];
	pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt1.y		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt1.y"];
	pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt2.x		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt2.x"];
	pCInspOpt->m_stULCOF[GLASS2].dmtrAlignPt2.y		= pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["dmtrAlignPt2.y"];
	pCInspOpt->m_stULCOF[GLASS2].bAlignPtSet		= (BOOL)(LONG)pAppMod->m_smfDevice["Package"]["m_stULCOF"]["GLASS2"]["bAlignPtSet"];
	
#if 1 //20130423
	if (g_bSlowMotionIndexing)
	{
		SetMotionProfileMode(g_ulSlowMotionFactor);
	}
	else
	{
		SetMotionProfileMode(NORMAL_MOT_PROF);
	}
#endif
	return CPI9000Stn::IPC_LoadDeviceParam();
}


LONG CHouseKeeping::HMI_PRAutoTestStop(IPC_CServiceMessage &svMsg)
{
	CPI9000App *m_pAppMod = dynamic_cast<CPI9000App*> (m_pModule);
	if (!HMI_bAutoTestStop)
	{
		HMI_bAutoTestStop = TRUE;
	}
	m_pAppMod->HMIMessageBox(MSG_OK, "AUTO TEST OPERATION", "Auto Test Stopped.");

	return 0;

}

VOID CHouseKeeping::ResetSystem()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	IPC_CServiceMessage svMsg;
	CString szScreen = "Bonding";
	for (INT i = 0; i < g_lErrorCounter; i++)
	{
		CloseAlarm();
	}
	g_lErrorCounter = 0;
	if (AutoMode == AUTO_BOND)
	{
		pAppMod->HMI_SetAllModSelected(svMsg);
	}
	svMsg.InitMessage(szScreen.GetLength() + 1, szScreen.GetBuffer());
	szScreen.ReleaseBuffer();
	m_comClient.SendRequest("HmiUserService", "HmiSetScreen", svMsg, 0);
}

VOID CHouseKeeping::StartBonding()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	IPC_CServiceMessage svMsg;
	CString szScreen = "Operation";
	//pAppMod->HMI_CheckStartBondingAllowed(svMsg);
	svMsg.InitMessage(szScreen.GetLength() + 1, szScreen.GetBuffer());
	szScreen.ReleaseBuffer();
	m_comClient.SendRequest("HmiUserService", "HmiSetScreen", svMsg, 0);
	pAppMod->SrvAutoCommand(svMsg);
}

VOID CHouseKeeping::StopBonding()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	HMI_bStopProcess = FALSE;
	SetHmiVariable("HMI_bStopProcess");
	IPC_CServiceMessage svMsg;
	//CString szScreen = "Bonding";
	pAppMod->SrvStopCommand(svMsg);
	pAppMod->HMI_StatisticsLogFile(svMsg);
	//do
	//{
	//	Sleep(50);

	//} while (pAppMod->m_szState != "Idle");
	//HMI_LightAndBuzzerInIdle(svMsg);
	//svMsg.InitMessage(szScreen.GetLength() + 1, szScreen.GetBuffer());
	//szScreen.ReleaseBuffer();
	//m_comClient.SendRequest("HmiUserService", "HmiSetScreen", svMsg, 0);
}

LONG CHouseKeeping::HMI_SetCalAverageFreq(IPC_CServiceMessage &svMsg)
{
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	LONG lLimit;
	svMsg.GetMsg(sizeof(LONG), &lLimit);

	g_lCalAverageFreq = lLimit;

	for (INT i = INSP_MB1; i < MAX_NUM_OF_MB_TYPE; i++)
	{
		pCInspOpt->m_stdPlacementOffsetX[i].clear();
		pCInspOpt->m_stdPlacementOffsetY[i].clear();
		pCInspOpt->m_stdPlacementAngle[i].clear();
	}
	return 0;
}

VOID CHouseKeeping::LockAllCovers()
{
		SetFrontDoorLockSol(ON);
		SetShBufDoorLockSol(ON);
		SetFrontDoorLockSol2(ON);
		SetBackDoorLockSol(ON);

		Sleep(500);
}

VOID CHouseKeeping::UnLockAllCovers()
{
		SetFrontDoorLockSol(OFF);
		SetShBufDoorLockSol(OFF);
		SetFrontDoorLockSol2(OFF);
		SetBackDoorLockSol(OFF);
		Sleep(500);
}

LONG CHouseKeeping::HMI_StopMoveTest(IPC_CServiceMessage &svMsg)
{
	m_bStopMoveTest = TRUE;
	return 1;
}

LONG CHouseKeeping::HMI_StopSearchDLnULTest(IPC_CServiceMessage &svMsg)
{
	m_bStopSearchDLnULTest = TRUE;
	return 1;
}