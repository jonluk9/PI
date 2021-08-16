/////////////////////////////////////////////////////////////////
// PI9000Stn.cpp : interface of the PI9000Stn class
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
#include "PI9000Stn.h"
#include "PI9000_Constant.h"
#include "Package_Util.h"

#include "TimeChart.h"
#include "HouseKeeping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
FPC_TYPE_INFO		g_stFPCType[MAX_NUM_OF_FPC_TYPE];

COG902_Glass_Info g_COG902GlassInfo[MAX_GLASS];

BOOL g_bStopCycle						= FALSE;

AUTOMODE AutoMode						= AUTO_BOND;
BOOL g_bDiagMode						= FALSE;
BOOL g_bFlushMode						= FALSE;
BOOL g_bSelectedReworkMode				= FALSE;
LONG g_lDiagnSteps						= 4;

LONG					g_ulDailyPerformanceReportPage = 0;
CPerformanceReport		*g_pPerformance_report = NULL;		//Philip Add 2012/12/7

CMotorInfo	g_stDummyMotor;  //whz rotary linear calib
// Station status
STATUS_TYPE INPICKARM_Status								= ST_MOD_NOT_SELECTED;
STATUS_TYPE OUTPICKARM_Status								= ST_MOD_NOT_SELECTED;
STATUS_TYPE PICKARM_Status									= ST_MOD_NOT_SELECTED;
STATUS_TYPE INSPWH_Status[MAX_NUM_OF_WH]					= { ST_MOD_NOT_SELECTED, ST_MOD_NOT_SELECTED };
STATUS_TYPE INSPOPT_Status									= ST_MOD_NOT_SELECTED;
STATUS_TYPE REJECTARM_Status								= ST_MOD_NOT_SELECTED;
STATUS_TYPE REJECTBELT_Status								= ST_MOD_NOT_SELECTED;
STATUS_TYPE BUFFERWH_Status									= ST_MOD_NOT_SELECTED;
STATUS_TYPE TA_MANAGER_Status								= ST_MOD_NOT_SELECTED;


STATUS_TYPE GLASSPR_Status[MAX_NUM_OF_PREBOND]				= { ST_MOD_NOT_SELECTED, ST_MOD_NOT_SELECTED };

STATUS_TYPE	INSPWH_GLASS_Status[MAX_NUM_OF_WH][MAX_NUM_OF_GLASS];

STATUS_TYPE	INPICKARM_GLASS_Status[MAX_NUM_OF_GLASS]			= {ST_EMPTY, ST_EMPTY};
STATUS_TYPE	OUTPICKARM_GLASS_Status[MAX_NUM_OF_GLASS]			= {ST_EMPTY, ST_EMPTY};

STATUS_TYPE	WINEAGLE_Status			= ST_MOD_NOT_SELECTED;

// Arm Posn
PAPosnType INPICKARM_ARM_Posn;
PAPosnType OUTPICKARM_ARM_Posn;

//Glass Status on REJBELT
BOOL g_bGlassOnRejBelt				= FALSE;

// Vision
BOOL	g_bEnableTrigLighting	= FALSE;
BOOL	g_bIsVisionNTInited		= FALSE; // Replaced pCWinEagle->m_bPRInit

// Bonding, user can enter PR center Manually
BOOL	g_bIsUserSearchPRDone	= FALSE; 
BOOL	g_bShowUserSearchPRDone = FALSE;

// Warning Message when bonding
CString g_szBondingMsg = "";

// Package Info
GLASS_INFO			g_stGlass;
DOUBLE				g_dGToGOffset = 170; // Machine Hard set mm // GlassToGlass Offset //PI9000 = 170mm
LONG				g_lGlassInputMode = GLASS_1_2_INPUT;

// Housekeeping
BOOL	g_bLogIndex				= FALSE;

BOOL	g_bSafeIndexing			= TRUE;
BOOL	g_bSlowMotionIndexing	= FALSE;
ULONG	g_ulSlowMotionFactor	= 0;
BOOL	g_bShowForceInNewton	= FALSE;

BOOL	g_lFlipperMode			= FLIP_MODE;

FORCE_CALIBRATOR	g_stForceCalibrator = { 0.0 };

//Login by
ULONG	g_ulUserGroup;
BOOL	g_bUserGroupOperator; //20150114 to disable items in HMI

LONG	g_lErrorCounter = 0;
ULONG	g_lCalAverageFreq = 10;

// Wizard Setup Related
BOOL	g_bWizardEnable		= FALSE;
CString	g_szWizardTitle		= "";
ULONG	g_ulWizardSubTitle	= 0;
CString	g_szWizardStep		= "";
CString	g_szWizardMess		= "";
CString	g_szWizardMessPrev	= "";
CString	g_szWizardMessNext	= "";
//CString	g_szWizardBMPath	= "";
CString	g_szWizardBMPath(" ", 50); //20121126
BOOL	g_bWizardEnableBack	= FALSE;
BOOL	g_bWizardEnableNext	= FALSE;
BOOL	g_bWizardEnableExit	= TRUE;
ULONG	g_ulWizardStep		= 0;
LONG	g_lWizardInProgress	= -1;

SETUP_POSN_XY	g_stSetupPosn[NUM_OF_POSN_XY];
SETUP_LEVEL_Z	g_stSetupLevel[NUM_OF_LEVEL_Z];
SETUP_PARA		g_stSetupPara[NUM_OF_PARA];
LONG			g_lSetupInProgress	= -1;

ULONG	g_ulWizardIgnoreWindowStep = 0; //20130319 PR ignore windows
CString	g_szWizardIgnoreWinTitle	= "";
CString	g_szWizardIgnoreWinSubtitle	= "";

#ifdef GATEWAY
GATEWAY_STATUS_TYPE COF902_OUTPICKARM_Status						= ST_GW_TA5_IDLE;
GATEWAY_STATUS_TYPE PI9000_INSPWH_Status					= ST_GW_INSPWH_IDLE;

GATEWAY_STATUS_TYPE COF902_Machine_Status					= ST_GW_MACHINE_STOP;
GATEWAY_STATUS_TYPE PI9000_Machine_Status					= ST_GW_MACHINE_STOP;
#endif

// Message
CCriticalSection	m_csMsgLock; 
CStringArray		m_astrMsgQueue;

// Get All Status
CCriticalSection	m_csGetAllStatusLock;	// 20140728 Yip

CCriticalSection	m_csLogErrorAlertLock;	// 20141124

// Parameter Log
CCriticalSection	m_csParameterLogLock;	// 20140729 Yip

// Update Statistic
CCriticalSection	m_csUpdateStatisticLock;	// 20140903 Yip

//Sequence related
BOOL	g_bIsTA3Ready = FALSE; //20120321

// For Dummy Motor 
const CString gszDummyMotor			= _T("DUMMY_MOTOR");

LONG g_lGatewayPICKARMStatus = ST_STOP; //20130919

#ifdef MB_FORCE_ADC
BOOL m_bMBForceLogRun[MAX_NUM_OF_MAINBOND];
#endif

IMPLEMENT_DYNCREATE(CPI9000Stn, CAppStation)

CPI9000Stn::CPI9000Stn()
{
	m_qState				= IDLE_Q;
	m_qSubOperation			= 0;
	m_qPreviousSubOperation	= 0;

	INT i = 0;

	// Wizard Setup Related
	for (i = 0; i < NUM_OF_POSN_XY; i++)
	{
		g_stSetupPosn[i].szPosn.Format("Position %d", i);
		g_stSetupPosn[i].bEnable			= TRUE;
		g_stSetupPosn[i].bVisible			= TRUE;
		g_stSetupPosn[i].lType				= MACHINE_PARA;
		g_stSetupPosn[i].lPosnX				= 0;
		g_stSetupPosn[i].lPosnY				= 0;
	}

	for (i = 0; i < NUM_OF_LEVEL_Z; i++)
	{
		g_stSetupLevel[i].szLevel.Format("Level %d", i);
		g_stSetupLevel[i].bEnable			= TRUE;
		g_stSetupLevel[i].bVisible			= TRUE;
		g_stSetupLevel[i].lType				= MACHINE_PARA;
		g_stSetupLevel[i].lLevelZ			= 0;
	}

	for (i = 0; i < NUM_OF_PARA; i++)
	{
		g_stSetupPara[i].szPara.Format("Para %d", i);
		g_stSetupPara[i].bEnable			= TRUE;
		g_stSetupPara[i].bVisible			= TRUE;
		g_stSetupPara[i].lType				= MACHINE_PARA;
		g_stSetupPara[i].dPara				= 0.0;
		g_stSetupPara[i].lPara				= 0;
	}

	m_lModSelectMode		= GLASS_MOD;

	m_bInitInstanceError	= FALSE;

	m_bCheckMotor			= FALSE;
	m_bCheckIO				= FALSE;

	m_bModSelected			= TRUE;
	m_bShowModSelected		= TRUE;
	//m_bPRSelected			= FALSE;
	m_bModError				= FALSE;
	//m_bPRError			= FALSE;
	m_bType1Exist			= FALSE;
	m_bType2Exist			= FALSE;
	m_bGlass1Exist			= FALSE;
	m_bGlass2Exist			= FALSE;
	m_bIsOfflineMode		= FALSE;
	m_bIsBusy				= FALSE;

	m_lDiagnSteps			= 1000;

	m_bPressCycle			= FALSE;
	
	m_bStopCycle			= FALSE;

	HMI_bDebugSeq			= FALSE;
	HMI_bDebugCal			= FALSE;
	HMI_bLogMotion			= FALSE; //20130422 //TRUE;
	setKlocworkTrue(TRUE); //klocwork fix 20121211

}

CPI9000Stn::~CPI9000Stn()
{
	//if (LogFile != NULL)	// 20140819 Yip: Fix Memory Leak Issue
	//{
	//	delete LogFile;
	//}
}

BOOL CPI9000Stn::InitInstance()
{
	try
	{
		Create();

		CSingleLock aLock(m_pModule->StartEvent());
		aLock.Lock();

		if (m_fHardware)
		{
			Initialize();
		}

		//Get Device & Machine File pointer from the Module
		CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
		m_psmfMachine = &(pAppMod->m_smfMachine);
		m_psmfDevice = &(pAppMod->m_smfDevice);

		//Register Variables
		RegisterVariables();

		// create the Station Log
		LogFile = new CLogFile("D:\\sw\\PI9000\\Data\\StationLog\\" + this->GetStnName() + ".log", CLogFile::NEW_ALWAYS | CLogFile::SAVE_ALWAYS | CLogFile::WRITE_TIME);
		LogFile->log("Station ID = %ld", this->GetStationID());

	}
	catch (CAsmException e)
	{
		DisplayException(e);
	}

	return TRUE;
}

INT CPI9000Stn::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if (LogFile != NULL)	// 20140819 Yip: Fix Memory Leak Issue
	{
		delete LogFile;
	}
	return CAppStation::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////
VOID CPI9000Stn::IdleOperation()
{
	switch (Command())
	{
	case glNULL_COMMAND:
		Sleep(20);
		break;

	case glDIAGNOSTIC_COMMAND:
		State(DIAGNOSTICS_Q);
		Command(glNULL_COMMAND);
		break;

	case glINITIAL_COMMAND:
		State(SYSTEM_INITIAL_Q);
		Command(glNULL_COMMAND);
		break;

	case glPRESTART_COMMAND:
		State(PRESTART_Q);
		Command(glNULL_COMMAND);
		break;

	case glAUTO_COMMAND:
		State(AUTO_Q);
		Command(glNULL_COMMAND);
		break;

	case glMANUAL_COMMAND:
		State(MANUAL_Q);
		Command(glNULL_COMMAND);
		break;

	case glDEMO_COMMAND:
		State(DEMO_Q);
		Command(glNULL_COMMAND);
		break;

	case glCYCLE_COMMAND:
		State(CYCLE_Q);
		Command(glNULL_COMMAND);
		break;

	case glDEINITIAL_COMMAND:
		State(DE_INITIAL_Q);
		Command(glNULL_COMMAND);
		break;

	case glSTOP_COMMAND:
	case glRESET_COMMAND:
		State(IDLE_Q);
		Motion(FALSE);
		Command(glNULL_COMMAND);
		break;

	default:
		Command(glNULL_COMMAND);
		State(IDLE_Q);
		Result(gnNOTOK);
		SetAlert(IDS_SYS_INVALID_COMD);
		break;
	}
}

VOID CPI9000Stn::DiagOperation()
{
	INT		m_nLastError	= gnOK;
	LONG	lAxis, lAction, lUnit;
	
	AxisAction(lAxis, lAction, lUnit); 
	
	if (m_fHardware)
	{
		try
		{
			CString	szAxisID	= GetAxisID(lAxis);

			switch (lAction)
			{
			case glPOSITION_ACTION:
				if (lUnit == HOME_POS)
				{
					m_nLastError = MoveHome(szAxisID); //lUnit = 0;
				}
				else
				{
					m_nLastError = MoveActuator(szAxisID, GetPositionID(szAxisID, NORMAL_POS, lUnit)); //lUnit = 1,2,3, etc
				}
				break;
			case glRELATIVE_ACTION:
				//m_nLastError	 = MoveRelative(szAxisID, lUnit, GMP_WAIT, 0);
				break;
			case glABSOLUTE_ACTION:
				//m_nLastError	 = MoveAbsolute(szAxisID, lUnit, GMP_WAIT, 0);
				break;
			case  glFAST_HOME_ACTION:
				m_nLastError	= MoveActuator(szAxisID, GetPositionID(szAxisID, FAST_HOME_POS, lUnit));  //lUnit = 1,2,3,etc
				break;
			case glSEARCH_ACTION:
				break;
			case glINDEX_ACTION:
				break;
			case glPOWER_ON_ACTION:
				PowerOnAxis(szAxisID);
				break;
			case glPOWER_OFF_ACTION:
				PowerOffAxis(szAxisID);
				break;			
			}
		}
		catch (CAsmException e)
		{
			DisplayException(e);
		}
	}
	
	Motion(FALSE);
	State(IDLE_Q);
}

VOID CPI9000Stn::InitOperation()
{
	CString szMsg = _T("   ") + GetName() + " --- InitOperation Completed";
	DisplayMessage(szMsg);
	
	Motion(FALSE);
	State(IDLE_Q);
}

VOID CPI9000Stn::PreStartOperation()
{
	CString szMsg = _T("   ") + GetName() + " --- PreStartOperation Completed";
	DisplayMessage(szMsg);

	Motion(FALSE);
	State(IDLE_Q);
}

VOID CPI9000Stn::AutoOperation()
{
	if (Command() == glSTOP_COMMAND)
	{
		State(STOPPING_Q);
	}
	else
	{
		Sleep(100);
	}
}

VOID CPI9000Stn::DemoOperation()
{
	if (Command() == glSTOP_COMMAND)
	{
		State(STOPPING_Q);
	}
	else
	{
		Sleep(100);
	}
}

VOID CPI9000Stn::CycleOperation()
{
	if (Command() == glSTOP_COMMAND)
	{
		State(STOPPING_Q);
	}
	else
	{
		Sleep(100);
	}
}

VOID CPI9000Stn::ManualOperation()
{
	if (Command() == glSTOP_COMMAND)
	{
		State(STOPPING_Q);
	}
	else
	{
		Sleep(100);
	}
}

VOID CPI9000Stn::ErrorOperation()
{
	if (Command() == glSTOP_COMMAND)
	{
		State(STOPPING_Q);
	}
	else if (Command() == glRESET_COMMAND) 
	{
		Command(glNULL_COMMAND);
		RestoreState();
	}
}

VOID CPI9000Stn::StopOperation()
{
	Sleep(100);
	CString szMsg = _T("   ") + GetName() + "--- Operation Stop";
	DisplayMessage(szMsg);

	State(IDLE_Q);
	Command(glNULL_COMMAND);
	Motion(FALSE);
}

VOID CPI9000Stn::DeInitialOperation()
{
	SFM_CStation *pStation;
	CAppStation  *pAppStn;
	CString		  szKey;

	try
	{
		//De-Initialize
		DeleteContents();
		DisplayMessage(_T("    Reset station --- ") + GetName());
			
		//Re-Initialize
		Initialize();
		DisplayMessage(_T("    Re-Initialized --- ") + GetName());

		//DeInitialise All Sub Stn
		for (POSITION pos = m_pStationMap.GetStartPosition(); pos;)
		{
			m_pStationMap.GetNextAssoc(pos, szKey, pStation);

			if ((pAppStn = dynamic_cast<CAppStation*>(pStation)) != NULL)
			{
				pAppStn->Command(glDEINITIAL_COMMAND);
				pAppStn->ReInitialized(FALSE);
				pAppStn->Result(gnOK);
			}
		}
	}
	catch (CAsmException e)
	{
		DisplayException(e);
	}

	//Set Reinitialize flag to true if all sub stn have been ReInitialized
	INT	nCount = 0;
	while (nCount != m_pStationMap.GetCount())
	{
		nCount = 0;
		for (POSITION pos = m_pStationMap.GetStartPosition(); pos;)
		{
			m_pStationMap.GetNextAssoc(pos, szKey, pStation);
			pAppStn = dynamic_cast<CAppStation*>(pStation);

			if (!pAppStn->IsReInitialized())
			{
				break;
			}

			nCount++;
		}
	}

	ReInitialized(TRUE);
	Motion(FALSE);
	State(IDLE_Q);
}


/////////////////////////////////////////////////////////////////
//Station Signalling
/////////////////////////////////////////////////////////////////
BOOL CPI9000Stn::Signal(INT nSignal)
{
	IPC_CServiceMessage svMsg;
	BOOL				bResult = FALSE;

	if (m_szModuleName != _T(""))
	{
		try
		{
			CString szService = _T("SrvSig");
			szService += m_szStationName;

			svMsg.InitMessage(sizeof(INT), &nSignal);
			m_comClient.SendRequest(m_szModuleName, szService, svMsg, 0);
			bResult = TRUE;
		}
		catch (CAsmException e)
		{
			DisplayException(e);
		}
	}

	return bResult;
}


/////////////////////////////////////////////////////////////////
//Alarm
/////////////////////////////////////////////////////////////////
BOOL CPI9000Stn::SetAlert(UINT unCode, const CString &szMessage)
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping"); //20130121
	CString		szCode;
	INT			bResult;

	g_lErrorCounter++;
	if ((unCode == gnOK) || (unCode == IDS_SYS_ABORT))
	{
		return TRUE;
	}

	pCHouseKeeping->m_bAlertExist = TRUE; //20130121

	if ((bResult = szCode.LoadString(unCode)) != FALSE)
	{
		try
		{
			if (szMessage.IsEmpty())
			{
				SendAlarm(szCode, ALM_DONT_LOG, 3000, gszALM_CLASS_ALERT);
			}
			else	// 20140707 Yip: Add Error/Alert With Specific Message
			{
				SendAlarm(szCode, ALM_DONT_LOG, 3000, gszALM_CLASS_ALERT, glHMI_MSG_MODAL, NULL, szMessage);
			}
		}
		catch (CAsmException e)
		{
			TRACE1(_T("SendAlert Fail - %s\n"), szCode);
		}
#if 1 //20141124
		if ((m_qState == AUTO_Q) || (m_qState == DEMO_Q) || (m_qState == STOPPING_Q))
		{
			m_csLogErrorAlertLock.Lock();
			CString szInFilePath2("D:\\sw\\PI9000\\LogFileDir\\");
			CString szInFileName2("ErrorAlarm.csv");
			theApp.CheckFileSize(szInFilePath2, szInFileName2, 512); //512KB
			theApp.LogErrorAlarm(szCode, FALSE);
			m_csLogErrorAlertLock.Unlock();

		}
#endif
	}
	else
	{
		szCode.Format(_T("%u: Alarm Code Not Found! From %s station"), unCode, m_szStationName);
	}

	if (szMessage.IsEmpty())
	{
		szCode = "Alert Code: " + szCode + " Desp: " + theApp.m_mapErrorAlertTable[szCode]; //20141124
	}
	else	// 20140707 Yip: Add Error/Alert With Specific Message
	{
		szCode = "Alert Code: " + szCode + " - " + szMessage;
	}
	DisplayMessage(szCode);

	return bResult;
}

BOOL CPI9000Stn::SetError(UINT unCode, const CString &szMessage)
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping"); //20130121
	CString		szCode;
	BOOL		bResult;

	g_lErrorCounter++;
	if (unCode == gnOK)
	{
		return TRUE;
	}

	if (g_pPerformance_report != NULL)
	{
		//switch (unCode)
		//{
		//default:
		g_pPerformance_report->ReportEventTime(MPRT_ERROR);
		//	break;
		//}
	}

	pCHouseKeeping->m_bErrorExist = TRUE; //20130121

	if ((bResult = szCode.LoadString(unCode)) != FALSE)
	{
		try
		{
			if (szMessage.IsEmpty())
			{
				SendAlarm(szCode, ALM_FORCE_LOG, 3000, gszALM_CLASS_ERROR);
			}
			else	// 20140707 Yip: Add Error/Alert With Specific Message
			{
				SendAlarm(szCode, ALM_FORCE_LOG, 3000, gszALM_CLASS_ERROR, glHMI_MSG_MODAL, NULL, szMessage);
			}
		}
		catch (CAsmException e)
		{
			TRACE1(_T("SendError Fail - %s\n"), szCode);
		}
	}
	else
	{
		szCode.Format(_T("%u: Alarm Code Not Found! From %s station"), unCode, m_szStationName);
	}


	if ((m_qState == AUTO_Q) || (m_qState == DEMO_Q) || (m_qState == STOPPING_Q))
	{
		theApp.GetAllStatus(szCode);	// 20140728 Yip
#if 1 //20141124
		m_csLogErrorAlertLock.Lock();
		CString szInFilePath2("D:\\sw\\PI9000\\LogFileDir\\");
		CString szInFileName2("ErrorAlarm.csv");
		theApp.CheckFileSize(szInFilePath2, szInFileName2, 512); //512KB
		theApp.LogErrorAlarm(szCode);
		m_csLogErrorAlertLock.Unlock();
#endif
		theApp.m_clkLastErrorTime = clock(); //20150105
		pCHouseKeeping->m_szLastError = szCode;	// 20140814 Yip
	}

	if (szMessage.IsEmpty())
	{
		szCode = "Error Code: " + szCode + " Desp: " + theApp.m_mapErrorAlertTable[szCode]; //20141124
	}
	else	// 20140707 Yip: Add Error/Alert With Specific Message
	{
		szCode = "Error Code: " + szCode + " - " + szMessage;
	}
	DisplayMessage(szCode);

	return bResult;
}

BOOL CPI9000Stn::SetSolError(const CString &szMessage)	// 20140707 Yip
{
	return SetError(IDS_SOL_ERR, szMessage);
}

BOOL CPI9000Stn::SetMotorError(const CString &szMessage)	// 20140707 Yip
{
	return SetError(IDS_MTR_ERR, szMessage);
}

BOOL CPI9000Stn::CloseDisplayAlert(UINT unCode)
{
	//CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");
		
	CString		szCode;
	INT			bResult;

	if ((unCode == gnOK) || (unCode == IDS_SYS_ABORT))
	{
		return TRUE;
	}

	//pCHouseKeeping->m_bAlertExist = TRUE;
	//g_lAlertCounter++;

	if ((bResult = szCode.LoadString(unCode)) != FALSE)
	{
		try
		{
			//SendAlarm(szCode, ALM_DONT_LOG, 3000, gszALM_CLASS_ALERT);
			CloseAlarm(szCode);
			
			Sleep(500);
			if (m_qState != IDLE_Q)
			{
				IPC_CServiceMessage svMsg;
				INT nConvID = 0;
				LONG lReply = 0;
				LONG lTimeCount = 0;

				CString  str = "SetHMILowerLayer";
				char *pBuffer = new char[str.GetLength() + 1];
				INT nIndex = 0;

				memcpy(&pBuffer[nIndex], (LPCTSTR)str, str.GetLength() + 1);
				nIndex += str.GetLength() + 1;

				try
				{
					svMsg.InitMessage(nIndex, pBuffer);
					nConvID = m_comClient.SendRequest("HmiUserService", "HmiExecuteRoutine", svMsg, 0);
				}
				catch (CAsmException e)
				{
					AfxMessageBox("Application process not found!");
				}
			}
		}
		catch (CAsmException e)
		{
			TRACE1(_T("CloseDisplayAlert Fail - %s\n"), szCode);
		}
	}
	else
	{
		szCode.Format(_T("%u: Alarm Code Not Found! From %s station"), unCode, m_szStationName);
	}

	//szCode = m_szStationName+ " Alert: " + szCode;
	//DisplayMessage(szCode);

	return bResult;
}

/////////////////////////////////////////////////////////////////
//Update Profile
/////////////////////////////////////////////////////////////////
VOID CPI9000Stn::UpdateSearchProfile(const CString &szAxis, const CString &szProfile)
{
	//Note: Critical section locking is performed by UpdateProfile();
	CMapElement		&rElement = (*m_psmfMachine)[m_szStationName][szAxis][gszMRK_SRCH_PROF][szProfile];
	
	//Set search profile here
}

VOID CPI9000Stn::UpdateMotionProfile(const CString &szAxis, const CString &szProfile)
{
	//Note: Critical section locking is performed by UpdateProfile();
	CMapElement	&rElement	= (*m_psmfMachine)[m_szStationName][szAxis][gszMRK_MOT_PROF][szProfile];
	
	//Set search profile here
}

VOID CPI9000Stn::UpdatePosUnit(const CString &szAxis, const CString &szPos)
{
	//Syntax: m_smfDevice[szStation][szAxis][gszMRK_POS][szPos]
	//for Contour position: m_smfDevice[szStation][szAxis][gszMRK_POS][szPos][nIndex]
	CMapElement	&rElement = (*m_psmfDevice)[m_szStationName][szAxis][gszMRK_POS][szPos];
	
	//Set position here
}

VOID CPI9000Stn::RegisterVariables()
{ 
	/*Virtual*/ 
}


INT CPI9000Stn::SleepWithReturn(DWORD dwMilliseconds)
{
	if (dwMilliseconds > 0)
	{
		Sleep(dwMilliseconds);
	}

	return GMP_SUCCESS;
}

/////////////////////////////////////////////////////////////////
//TimeChart Logging
/////////////////////////////////////////////////////////////////
// Help Me

VOID CPI9000Stn::TC_AddJob(INT emTCObjectID)
{
	CTimeChart *pCTimeChart = (CTimeChart*)m_pModule->GetStation("TimeChart");

	if (!pCTimeChart->m_bModSelected || (State() != AUTO_Q && State() != DEMO_Q))
	{
		return;
	}

	//Debug, Test Time Chart
	//DisplayMessage("TC_AddJob: not fail");

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	//LONG lTimeCount = 0;

	if ((emTCObjectID < 0) || (emTCObjectID >= NUM_OF_TC_OBJECT))	// 20140718 Yip
	{
		CString szMsg;
		szMsg.Format("%s: TC_AddJob with Invalid ID %d", GetStnName(), emTCObjectID);
		DisplayMessage(szMsg);
	}

	stMsg.InitMessage(sizeof(INT), &emTCObjectID);
	nConvID = m_comClient.SendRequest("TimeChart", "TC_AddCmd", stMsg, 0);
}

VOID CPI9000Stn::TC_DelJob(INT emTCObjectID)
{
	CTimeChart *pCTimeChart = (CTimeChart*)m_pModule->GetStation("TimeChart");

	if (!pCTimeChart->m_bModSelected || (State() != AUTO_Q && State() != DEMO_Q))
	{
		return;
	}

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	//LONG lTimeCount = 0;

	if ((emTCObjectID < 0) || (emTCObjectID >= NUM_OF_TC_OBJECT))	// 20140718 Yip
	{
		CString szMsg;
		szMsg.Format("%s: TC_DelJob with Invalid ID %d", GetStnName(), emTCObjectID);
		DisplayMessage(szMsg);
	}

	stMsg.InitMessage(sizeof(INT), &emTCObjectID);
	nConvID = m_comClient.SendRequest("TimeChart", "TC_DelCmd", stMsg, 0);

}

VOID CPI9000Stn::TC_ResetTime()
{
	CTimeChart *pCTimeChart = (CTimeChart*)m_pModule->GetStation("TimeChart");

	if (!pCTimeChart->m_bModSelected || (State() != AUTO_Q && State() != DEMO_Q))
	{
		return;
	}

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;

	stMsg.InitMessage(sizeof(LONG), &lTimeCount); //20130709
	nConvID = m_comClient.SendRequest("TimeChart", "TC_ResetTimer", stMsg, 0);

}

/////////////////////////////////////////////////////////////////
//Message Windows and Logging
/////////////////////////////////////////////////////////////////
VOID CPI9000Stn::WriteHMIMess(const CString &szMess, BOOL bLog, BOOL bLogTime)
{
	CString	szMessage	= _T("");
	__time64_t ltime;

	_time64(&ltime);
	CString szTime = _ctime64(&ltime);
	szTime = szTime.Trim('\n');

	if (bLog)
	{
		DisplayMessage(szMess);
	}

	if (bLogTime)
	{
		szMessage = _T("[") + szTime + _T("]") + _T("   ") + szMess;
	}
	else
	{
		szMessage = szMess;
	}
	
	m_csMsgLock.Lock();
	m_astrMsgQueue.Add(szMessage);
	m_csMsgLock.Unlock();
}

LONG CPI9000Stn::HMIMessageBox(const INT nBoxType, const CString &szTitle, const CString &szText)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;

	LONG	lType	= nBoxType;
	LONG	lAlign	= 3;			// align centre
	LONG	lWidth	= 400;
	LONG	lHeight	= 280;

	char *pBuffer = new char[szText.GetLength() + 1 + szTitle.GetLength() + 1 + 4 * sizeof(LONG)];

	INT nIndex = 0;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szText, szText.GetLength() + 1);
	nIndex += szText.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szTitle, szTitle.GetLength() + 1);
	nIndex += szTitle.GetLength() + 1;
	memcpy(&pBuffer[nIndex], &lType, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lAlign, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lWidth, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lHeight, sizeof(LONG));
	nIndex += sizeof(LONG);

	svMsg.InitMessage(nIndex, pBuffer);
	delete[] pBuffer;

	nConvID = m_comClient.SendRequest("HmiUserService", "HmiMessageBox", svMsg);
	while (1)
	{
		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
			}
			break;
		}
		else if (lTimeCount++ > 30000)
		{
			lReply		= glHMI_TIMEOUT;
			SetError(IDS_HK_OPERATION_TIMEOUT);
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(10);
		}
	}

	return lReply;
}

LONG CPI9000Stn::HMISelectionBox(const CString &szTitle, const CString &szText, const CString &szItemText1, const CString &szItemText2, const CString &szItemText3, const CString &szItemText4, const CString &szItemText5, const CString &szItemText6, const CString &szItemText7, const CString &szItemText8, const CString &szItemText9, const CString &szItemText10)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	INT i = 0;
	INT j = 0;


	LONG	lSelection = 0;					//A long variable indicating the default selection to be displayed.
	LONG	lNumSelection = 0;				//A long variable indicating the total number of selections (maximum of 7 items).
	
	CString szItemText[11];

	szItemText[0] = szItemText1;
	szItemText[1] = szItemText2;
	szItemText[2] = szItemText3;
	szItemText[3] = szItemText4;
	szItemText[4] = szItemText5;
	szItemText[5] = szItemText6;
	szItemText[6] = szItemText7;
	szItemText[7] = szItemText8;
	szItemText[8] = szItemText9;
	szItemText[9] = szItemText10;
	szItemText[10] = "";

	while (szItemText[j] != "")
	{
		lNumSelection++;
		j++;
	}

	char *pBuffer = new char[szText.GetLength() + 1 + szTitle.GetLength() + 1 + 2 * sizeof(LONG) + szItemText[0].GetLength() + 1 + szItemText[1].GetLength() + 1 + szItemText[2].GetLength() + 1 + szItemText[3].GetLength() + 1 + szItemText[4].GetLength() + 1 + szItemText[5].GetLength() + 1 + szItemText[6].GetLength() + 1 + szItemText[7].GetLength() + 1 + szItemText[8].GetLength() + 1 + szItemText[9].GetLength() + 1];

	INT nIndex = 0;

	memcpy(&pBuffer[nIndex], (LPCTSTR)szText, szText.GetLength() + 1);
	nIndex += szText.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szTitle, szTitle.GetLength() + 1);
	nIndex += szTitle.GetLength() + 1;
	memcpy(&pBuffer[nIndex], &lSelection, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lNumSelection, sizeof(LONG));
	nIndex += sizeof(LONG);

	for (i = 0; i < j; i++)
	{
		memcpy(&pBuffer[nIndex], (LPCTSTR)szItemText[i], szItemText[i].GetLength() + 1);
		nIndex += szItemText[i].GetLength() + 1;
	}

	svMsg.InitMessage(nIndex, pBuffer);
	delete[] pBuffer;

	nConvID = m_comClient.SendRequest("HmiUserService", "HmiSelectionBox", svMsg);
	while (1)
	{
		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
			}
			break;
		}
		else if (lTimeCount++ > 30000)
		{
			lReply		= glHMI_TIMEOUT;
			SetError(IDS_HK_OPERATION_TIMEOUT);
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(10);
		}
	}

	return lReply;
}

LONG CPI9000Stn::HMIConfirmWindow(const CString &szFirstbtn, const CString &szSecondbtn, const CString &szThirdbtn, const CString &szTitle, const CString &szText)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;

	LONG	lType	= 209;			// 3 buttons
	LONG	lAlign	= 3;			// align centre
	LONG	lWidth	= 400;
	LONG	lHeight	= 280;
	LONG	lTimeout = 0;
	CString szBitmapFile = NULL;

	CString szCustomText1 = szFirstbtn;
	CString szCustomText2 = szSecondbtn;
	CString szCustomText3 = szThirdbtn;

	//CString szCustomText1 = "Yes";
	//CString szCustomText2 = "No";
	//CString szCustomText3 = "Cancel";

	Response MessageReply = BUTTON_1;
	LONG lResponse	= 0;

	char *pBuffer = new char[szText.GetLength() + 1 + szTitle.GetLength() + 1 + 5 * sizeof(LONG) + szBitmapFile.GetLength() + 1 + szCustomText1.GetLength() + 1 + szCustomText2.GetLength() + 1 + szCustomText3.GetLength() + 1];
	
	INT nIndex = 0;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szText, szText.GetLength() + 1);
	nIndex += szText.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szTitle, szTitle.GetLength() + 1);
	nIndex += szTitle.GetLength() + 1;
	memcpy(&pBuffer[nIndex], &lType, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lAlign, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lWidth, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lHeight, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], &lTimeout, sizeof(LONG));
	nIndex += sizeof(LONG);
	memcpy(&pBuffer[nIndex], (LPCTSTR)szBitmapFile, szBitmapFile.GetLength() + 1);
	nIndex += szBitmapFile.GetLength() + 1;

	memcpy(&pBuffer[nIndex], (LPCTSTR)szCustomText1, szCustomText1.GetLength() + 1);
	nIndex += szCustomText1.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szCustomText2, szCustomText2.GetLength() + 1);
	nIndex += szCustomText2.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szCustomText3, szCustomText3.GetLength() + 1);
	nIndex += szCustomText3.GetLength() + 1;

	svMsg.InitMessage(nIndex, pBuffer);
	delete[] pBuffer;

	nConvID = m_comClient.SendRequest("HmiUserService", "HmiMessageBox", svMsg);
	while (1)
	{
		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lResponse);
			}
			break;
		}
		else if (lTimeCount++ > 30000)
		{
			MessageReply		= BUTTON_3; // Alaways at the Cancel
			SetError(IDS_HK_OPERATION_TIMEOUT);
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(10);
		}
	}

	if (lResponse == 1)
	{
		MessageReply = BUTTON_1;
	}
	else if (lResponse == 5)
	{
		MessageReply = BUTTON_2;
	}
	else if (lResponse == 8)
	{
		MessageReply = BUTTON_3;
	}

	return MessageReply;
}

BOOL CPI9000Stn::HMIGetPassword(const CString &szTitle, const CString &szPassword)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;

	PCHAR	pBuffer		= NULL;
	INT		nLength		= 0;
	INT		nMsgIndex	= 0;
	INT		nTCharSize	= sizeof(TCHAR);
	BOOL	bReply		= FALSE;

	nLength = (szTitle.GetLength() + 1) * nTCharSize;
	nLength += (szPassword.GetLength() + 1) * nTCharSize;
	pBuffer = new CHAR[nLength];

	_tcsncpy(pBuffer, szTitle, szTitle.GetLength() + 1);
	nMsgIndex = szTitle.GetLength() + 1;
	_tcsncpy(pBuffer + nMsgIndex, szPassword, szPassword.GetLength() + 1);

	try
	{
		svMsg.InitMessage(nLength, pBuffer);
		delete[] pBuffer;
		pBuffer = NULL;

		nConvID = m_comClient.SendRequest("HmiUserService", "HmiPassword", svMsg);
		while (1)
		{
			MSG msgCur;
			while (::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
			{
				CWinApp *pApp = AfxGetApp();
				// pump message, but quit on WM_QUIT
				if (!pApp->PumpMessage())
				{
					return bReply;
				}
			}

			if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
			{
				if (svMsg.GetMsgLen() == sizeof(BOOL))
				{
					svMsg.GetMsg(sizeof(BOOL), &bReply);
				}
				break;
			}
			else if (lTimeCount++ > 30000)
			{
				lReply		= glHMI_TIMEOUT;
				SetError(IDS_HK_OPERATION_TIMEOUT);
				break;
			}
			else
			{
				while (m_comServer.ProcessRequest())
				{
					;
				}
				Sleep(10);
			}
		}
	}
	catch (CAsmException e)
	{
		if (pBuffer != NULL)
		{
			delete[] pBuffer;
			pBuffer = NULL;
		}
	}

	return bReply;
}

BOOL CPI9000Stn::HMIAlphaKeys(const CString &szPrompt, CString *pszKeyIn, BOOL bKeyInPassword)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	BOOL bTRUE = TRUE;
	BOOL bFALSE = FALSE;

	pAppMod->stDummy = "";
	CString szName = "stDummy";

	CHAR *pBuffer = new CHAR[szName.GetLength() + 1 + szPrompt.GetLength() + 1 + sizeof(BOOL) + sizeof(BOOL)];
	BOOL bReply = FALSE;

	INT nIndex = 0;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szName, szName.GetLength() + 1);
	nIndex += szName.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szPrompt, szPrompt.GetLength() + 1);
	nIndex += szPrompt.GetLength() + 1;
	memcpy(&pBuffer[nIndex], &bKeyInPassword, sizeof(BOOL));
	nIndex += sizeof(BOOL);
	memcpy(&pBuffer[nIndex], &bTRUE, sizeof(BOOL));
	nIndex += sizeof(BOOL);

	svMsg.InitMessage(nIndex, pBuffer);
	delete[] pBuffer;

	nConvID = m_comClient.SendRequest("HmiUserService", "HmiAlphaKeys", svMsg);
	//while (!m_comClient.ScanReplyForConvID(nConvID, 1) && (lTimeCount++ < 8640000))
	//{
	//	MSG msgCur;
	//	while(::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
	//	{
	//		CWinApp* pApp = AfxGetApp();
	//		// pump message, but quit on WM_QUIT
	//		if (!pApp->PumpMessage())
	//		{
	//			return bReply;
	//		}
	//	}
	//	Sleep(10);
	//	ProcessHMIRequest();
	//}

	//if (lTimeCount < 8640000)
	//{
	//	m_comClient.ReadReplyForConvID(nConvID, svMsg);
	//	if (svMsg.GetMsgLen() == sizeof(BOOL))
	//	{
	//		svMsg.GetMsg(sizeof(BOOL), &bReply);
	//	}
	//	else
	//	{
	//		CHAR* pReply = new CHAR[svMsg.GetMsgLen()];
	//		svMsg.GetMsg(svMsg.GetMsgLen(), pReply);

	//		CString stReply = &pReply[sizeof(BOOL)];
	//		*pszKeyIn = stReply;
	//		
	//		bReply = TRUE;
	//	}
	//}

	while (1)
	{
		MSG msgCur;
		while (::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
			CWinApp *pApp = AfxGetApp();
			// pump message, but quit on WM_QUIT
			if (!pApp->PumpMessage())
			{
				return bReply;
			}
		}

		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
			}
			else
			{
				CHAR *pReply = new CHAR[svMsg.GetMsgLen()];
				svMsg.GetMsg(svMsg.GetMsgLen(), pReply);

				CString stReply = &pReply[sizeof(BOOL)];
				*pszKeyIn = stReply;
				
				bReply = TRUE;
			}
			break;
		}
		else if (lTimeCount++ > 30000)
		{
			lReply		= glHMI_TIMEOUT;
			SetError(IDS_HK_OPERATION_TIMEOUT);
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(10);
		}
	}

	return bReply;
}

BOOL CPI9000Stn::HMINumericKeys(const CString &szPrompt, const DOUBLE dMaxValue, const DOUBLE dMinValue, DOUBLE *pdValue)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	BOOL bTRUE = TRUE;

	pAppMod->dDummy = 0.0;
	CString szVariable = "dDummy";

	CHAR *pBuffer = new CHAR[szVariable.GetLength() + 1 + szPrompt.GetLength() + 1 + 2 * sizeof(DOUBLE) + sizeof(BOOL)];
	BOOL bReply = FALSE;

	INT nIndex = 0;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szVariable, szVariable.GetLength() + 1);
	nIndex += szVariable.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szPrompt, szPrompt.GetLength() + 1);
	nIndex += szPrompt.GetLength() + 1;
	memcpy(&pBuffer[nIndex], &dMaxValue, sizeof(DOUBLE));
	nIndex += sizeof(DOUBLE);
	memcpy(&pBuffer[nIndex], &dMinValue, sizeof(DOUBLE));
	nIndex += sizeof(DOUBLE);
	memcpy(&pBuffer[nIndex], &bTRUE, sizeof(BOOL));
	nIndex += sizeof(BOOL);

	svMsg.InitMessage(nIndex, pBuffer);
	delete[] pBuffer;

	nConvID = m_comClient.SendRequest("HmiUserService", "HmiNumericKeys", svMsg);
	//while (!m_comClient.ScanReplyForConvID(nConvID, 1) && (lTimeCount++ < 8640000))
	//{
	//	MSG msgCur;
	//	while(::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
	//	{
	//		CWinApp* pApp = AfxGetApp();
	//		// pump message, but quit on WM_QUIT
	//		if (!pApp->PumpMessage())
	//		{
	//			return bReply;
	//		}
	//	}
	//	Sleep(10);
	//	ProcessHMIRequest();
	//}

	//if (lTimeCount < 8640000)
	//{
	//	CString szMsg = "";

	//	m_comClient.ReadReplyForConvID(nConvID, svMsg);

	//	if (svMsg.GetMsgLen() == sizeof(BOOL))
	//	{
	//		svMsg.GetMsg(sizeof(BOOL), &bReply);
	//	}
	//	else
	//	{
	//		CHAR* pReply = new CHAR[svMsg.GetMsgLen()];
	//		svMsg.GetMsg(svMsg.GetMsgLen(), pReply);

	//		CString stReply = &pReply[sizeof(BOOL)];
	//		*pdValue = atof(stReply);

	//		bReply = TRUE;
	//	}
	//}

	while (1)
	{
		MSG msgCur;
		while (::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
			CWinApp *pApp = AfxGetApp();
			// pump message, but quit on WM_QUIT
			if (!pApp->PumpMessage())
			{
				return bReply;
			}
		}

		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
			}
			else
			{
				CHAR *pReply = new CHAR[svMsg.GetMsgLen()];
				svMsg.GetMsg(svMsg.GetMsgLen(), pReply);

				CString stReply = &pReply[sizeof(BOOL)];
				*pdValue = atof(stReply);				
				bReply = TRUE;
			}
			break;
		}
		else if (lTimeCount++ > 30000)
		{
			lReply		= glHMI_TIMEOUT;
			SetError(IDS_HK_OPERATION_TIMEOUT);
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(10);
		}
	}
	return bReply;
}

BOOL CPI9000Stn::HMINumericKeys(const CString &szPrompt, const DOUBLE dMaxValue, const DOUBLE dMinValue, LONG *plValue)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	BOOL bTRUE = TRUE;

	pAppMod->dDummy = 0.0;
	CString szVariable = "dDummy";

	CHAR *pBuffer = new CHAR[szVariable.GetLength() + 1 + szPrompt.GetLength() + 1 + 2 * sizeof(DOUBLE) + sizeof(BOOL)];
	BOOL bReply = FALSE;

	INT nIndex = 0;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szVariable, szVariable.GetLength() + 1);
	nIndex += szVariable.GetLength() + 1;
	memcpy(&pBuffer[nIndex], (LPCTSTR)szPrompt, szPrompt.GetLength() + 1);
	nIndex += szPrompt.GetLength() + 1;
	memcpy(&pBuffer[nIndex], &dMaxValue, sizeof(DOUBLE));
	nIndex += sizeof(DOUBLE);
	memcpy(&pBuffer[nIndex], &dMinValue, sizeof(DOUBLE));
	nIndex += sizeof(DOUBLE);
	memcpy(&pBuffer[nIndex], &bTRUE, sizeof(BOOL));
	nIndex += sizeof(BOOL);

	svMsg.InitMessage(nIndex, pBuffer);
	delete[] pBuffer;

	nConvID = m_comClient.SendRequest("HmiUserService", "HmiNumericKeys", svMsg);
	//while (!m_comClient.ScanReplyForConvID(nConvID, 1) && (lTimeCount++ < 8640000))
	//{
	//	MSG msgCur;
	//	while(::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
	//	{
	//		CWinApp* pApp = AfxGetApp();
	//		// pump message, but quit on WM_QUIT
	//		if (!pApp->PumpMessage())
	//		{
	//			return bReply;
	//		}
	//	}
	//	Sleep(10);
	//	ProcessHMIRequest();
	//}

	//if (lTimeCount < 8640000)
	//{
	//	CString szMsg = "";

	//	m_comClient.ReadReplyForConvID(nConvID, svMsg);

	//	if (svMsg.GetMsgLen() == sizeof(BOOL))
	//	{
	//		svMsg.GetMsg(sizeof(BOOL), &bReply);
	//	}
	//	else
	//	{
	//		CHAR* pReply = new CHAR[svMsg.GetMsgLen()];
	//		svMsg.GetMsg(svMsg.GetMsgLen(), pReply);

	//		CString stReply = &pReply[sizeof(BOOL)];
	//		*plValue = atol(stReply);

	//		bReply = TRUE;
	//	}
	//}
	while (1)
	{
		MSG msgCur;
		while (::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
			CWinApp *pApp = AfxGetApp();
			// pump message, but quit on WM_QUIT
			if (!pApp->PumpMessage())
			{
				return bReply;
			}
		}

		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
			}
			else
			{
				CHAR *pReply = new CHAR[svMsg.GetMsgLen()];
				svMsg.GetMsg(svMsg.GetMsgLen(), pReply);

				CString stReply = &pReply[sizeof(BOOL)];
				*plValue = atol(stReply);
				bReply = TRUE;
			}
			break;
		}
		else if (lTimeCount++ > 30000)
		{
			lReply		= glHMI_TIMEOUT;
			SetError(IDS_HK_OPERATION_TIMEOUT);
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(10);
		}
	}
	return bReply;
}

VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq)	// 20140619 Yip
{
	m_csGetAllStatusLock.Lock();	// 20150612
	m_deqDebugSeq.push_back(szSeq);
	while (!m_deqDebugSeq.empty() && (LONG) m_deqDebugSeq.size() > SEQ_DEQUE_LENGTH)
	{
		m_deqDebugSeq.pop_front();
	}
	m_csGetAllStatusLock.Unlock();	// 20150612
}

VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq, const STATUS_TYPE &stStatus)	// 20140619 Yip
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szStatus;
	szStatus.Format(", Status: %ld", (LONG)stStatus);

	CString szTime;
	szTime.Format(" Time:%.3f", (DOUBLE)(clock() - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC);

	UpdateDebugSeq(szSeq + szStatus + szTime);
}

VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq, const STATUS_TYPE &stStatus, const CString &szRelatedStatus, const LONG &lRelatedStatus)	// 20140619 Yip
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szStatus;
	szStatus.Format(", Status: %ld, %s: %ld", (LONG)stStatus, szRelatedStatus, lRelatedStatus);
	
	CString szTime;
	szTime.Format(" Time:%.3f", (DOUBLE)(clock() - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC);

	UpdateDebugSeq(szSeq + szStatus + szTime);
}

VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq, const STATUS_TYPE &stStatus, const STATUS_TYPE &stGlassStatus)	// 20140619 Yip
{
	UpdateDebugSeq(szSeq, stStatus, "Glass Status", (LONG)stGlassStatus);
}

VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq, const STATUS_TYPE &stStatus, const STATUS_TYPE stGlassStatus[])	// 20140619 Yip
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szStatus;
	szStatus.Format(", Status: %ld", (LONG)stStatus);
	CString szGlassStatus;
	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		szGlassStatus.Format(", Glass%d Status: %ld", i + 1, (LONG)stGlassStatus[i]);
		szStatus += szGlassStatus;
	}
	CString szTime;
	szTime.Format(" Time:%.3f", (DOUBLE)(clock() - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC);

	UpdateDebugSeq(szSeq + szStatus + szTime);
}

VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq, const STATUS_TYPE &stStatus, BOOL bExist)	// 20140619 Yip
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szStatus;
	szStatus.Format(", Status: %ld, Exist: %ld", (LONG)stStatus, (LONG)bExist);
	CString szTime;
	szTime.Format(" Time:%.3f", (DOUBLE)(clock() - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC);

	UpdateDebugSeq(szSeq + szStatus + szTime);
}

VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq, const STATUS_TYPE &stStatus, BOOL bExist1, BOOL bExist2)	// 20140619 Yip
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szStatus;
	szStatus.Format(", Status: %ld, Exist1: %ld, Exist2: %ld", (LONG)stStatus, (LONG)bExist1, (LONG)bExist2);
	
	CString szTime;
	szTime.Format(" Time:%.3f", (DOUBLE)(clock() - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC);

	UpdateDebugSeq(szSeq + szStatus + szTime);
}
VOID CPI9000Stn::UpdateDebugSeq(const CString &szSeq, const STATUS_TYPE &stStatus, BOOL bExist1, BOOL bExist2, BOOL bAllowCollect1, BOOL bAllowCollect2)	// 20151119
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString szStatus;
	szStatus.Format(", Status: %ld, Exist1: %ld, Exist2: %ld, AllowCollect1: %ld, AllowCollect2: %ld", (LONG)stStatus, (LONG)bExist1, (LONG)bExist2, (LONG)bAllowCollect1, (LONG)bAllowCollect2);
	CString szTime;
	szTime.Format(" Time:%.3f", (DOUBLE)(clock() - pAppMod->m_clkBondStartTime) / CLOCKS_PER_SEC);

	UpdateDebugSeq(szSeq + szStatus + szTime);
}

//////////////////////////////////
// API
//////////////////////////////////
//public Used:
BOOL CPI9000Stn::IsInitInstanceError()
{
	return m_bInitInstanceError;
}

BOOL CPI9000Stn::IsBusy()
{
	return m_bIsBusy;
}

BOOL CPI9000Stn::IsModSelected()
{
	return m_bModSelected;
}

//BOOL CPI9000Stn::IsPRSelected()
//{
//	return m_bPRSelected;
//}

BOOL CPI9000Stn::IsOfflineMode()
{
	return m_bIsOfflineMode;
}

BOOL CPI9000Stn::IsPressCycle()
{
	return m_bPressCycle;
}

BOOL CPI9000Stn::IsBurnInDiagBond()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	return ((AutoMode == BURN_IN || AutoMode == DIAG_BOND) && (pAppMod->State() != IDLE_Q));
}

VOID CPI9000Stn::PrintMachineSetup(FILE *fp)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str;

	if (fp != NULL)
	{
		fprintf(fp, "<Software Protection>\n");
		
		for (INT i = 0; i < GetmyMotorList().GetSize(); i++)
		{
			str = GetmyMotorList().GetAt(i)->stAttrib.szAxis;

			fprintf(fp, "Axis			: %s\n",	(const char*)str);
			fprintf(fp, "lNegLmt		: %ld\n",	(LONG)pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lNegLmt"]);
			fprintf(fp, "lPosLmt		: %ld\n",	(LONG)pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lPosLmt"]);
		}
	}
}

VOID CPI9000Stn::PrintDeviceSetup(FILE *fp)
{
}

VOID CPI9000Stn::PrintDebugSeq(FILE *fp) //20131010
{
	std::deque <LONG>::size_type size;

	size = m_deqDebugSeq.size();
	if (size == 0)
	{
		return;
	}

	for (INT ii = 0; ii < (INT)size; ii++)
	{
		fprintf(fp, "\t\t%s\n", (char*) m_deqDebugSeq[ii].GetBuffer(0));
	}
	fprintf(fp, "\n");
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
BOOL CPI9000Stn::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;
	CString str;

	for (INT i = 0; i < GetmyMotorList().GetSize(); i++)
	{
		str = GetmyMotorList().GetAt(i)->stAttrib.szAxis;

		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lNegLmt"];
		bResult = printMachineInfoRow(szModule, CString("Protection"), str, CString("NegLmt"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
		lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lPosLmt"];
		bResult = printMachineInfoRow(szModule, CString("Protection"), str, CString("PosLmt"), szBlank,
									  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
		if (!bResult)
		{
			break;
		}
	}

	for (INT i = 0; i < GetmySoList().GetSize(); i++)
	{
		if (GetmySoList().GetAt(i)->IsMachineData())
		{
			str = GetmySoList().GetAt(i)->GetName();

			lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOnDelay"];
			bResult = printMachineInfoRow(szModule, CString("Delays"), str, CString("OnDelay"), szBlank,
										  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
			if (!bResult)
			{
				break;
			}

			lValue = (LONG) pAppMod->m_smfMachine[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOffDelay"];
			bResult = printMachineInfoRow(szModule, CString("Delays"), str, CString("OffDelay"), szBlank,
										  szBlank, szBlank, szBlank, szBlank, szBlank, lValue);
			if (!bResult)
			{
				break;
			}
		}
	}
	return bResult;
}
#endif

VOID CPI9000Stn::PrintPRSetup(FILE *fp)
{
}

VOID CPI9000Stn::PrintPRCalibSetup(FILE *fp)
{
}

VOID CPI9000Stn::PrintCalibSetup(FILE *fp)
{
}

VOID CPI9000Stn::PrintForceCalibData(FILE *fp)
{
}


LONG CPI9000Stn::IPC_SaveMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str;

	// Motor List
	for (INT i = 0; i < GetmyMotorList().GetSize(); i++)
	{
		str = GetmyMotorList().GetAt(i)->stAttrib.szAxis;
		pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lNegLmt"]	= GetmyMotorList().GetAt(i)->stProtect.lNegLmt;
		pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lPosLmt"]	= GetmyMotorList().GetAt(i)->stProtect.lPosLmt;

		pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lAbsoluteEncLastHome"] = GetmyMotorList().GetAt(i)->stAttrib.lAbsoluteEncLastHome;
	}

	// So List
	for (INT i = 0; i < GetmySoList().GetSize(); i++)
	{
		if (GetmySoList().GetAt(i)->IsMachineData())
		{
			pAppMod->m_smfMachine[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOnDelay"] = GetmySoList().GetAt(i)->GetOnDelay();
			pAppMod->m_smfMachine[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOffDelay"] = GetmySoList().GetAt(i)->GetOffDelay();
			
		}
	}

	pAppMod->m_smfMachine[GetStnName()]["Setting"]["HMI_bDebugSeq"] = HMI_bDebugSeq;	
	pAppMod->m_smfMachine[GetStnName()]["Setting"]["HMI_bDebugCal"]	= HMI_bDebugCal;
	return 1;
}

LONG CPI9000Stn::IPC_LoadMachineParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CString str;
	LONG lDelay;

	// Motor List
	for (INT i = 0; i < GetmyMotorList().GetSize(); i++)
	{
		str = GetmyMotorList().GetAt(i)->stAttrib.szAxis;
		GetmyMotorList().GetAt(i)->stProtect.lNegLmt		= pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lNegLmt"];
		GetmyMotorList().GetAt(i)->stProtect.lPosLmt		= pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lPosLmt"];

		GetmyMotorList().GetAt(i)->stAttrib.lAbsoluteEncLastHome		= pAppMod->m_smfMachine[GetStnName()]["Protection"][str]["lAbsoluteEncLastHome"];
	}

	// So List
	for (INT i = 0; i < GetmySoList().GetSize(); i++)
	{
		if (GetmySoList().GetAt(i)->IsMachineData())
		{
			lDelay = pAppMod->m_smfMachine[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOnDelay"];
			GetmySoList().GetAt(i)->SetOnDelay(lDelay);
			lDelay = pAppMod->m_smfMachine[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOffDelay"];
			GetmySoList().GetAt(i)->SetOffDelay(lDelay);
		}
	}
	
	HMI_bDebugSeq = (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["HMI_bDebugSeq"];	
	HMI_bDebugCal = (BOOL)(LONG)pAppMod->m_smfMachine[GetStnName()]["Setting"]["HMI_bDebugCal"];

	return 1;
}

LONG CPI9000Stn::IPC_SaveDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	// So List
	for (INT i = 0; i < GetmySoList().GetSize(); i++)
	{
		if (!GetmySoList().GetAt(i)->IsMachineData())
		{
			pAppMod->m_smfDevice[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOnDelay"] = GetmySoList().GetAt(i)->GetOnDelay();
			pAppMod->m_smfDevice[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOffDelay"] = GetmySoList().GetAt(i)->GetOffDelay();
			
		}
	}

	return 1;
}

LONG CPI9000Stn::IPC_LoadDeviceParam()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	LONG lDelay;

	// So List
	for (INT i = 0; i < GetmySoList().GetSize(); i++)
	{
		if (!GetmySoList().GetAt(i)->IsMachineData())
		{
			lDelay = pAppMod->m_smfDevice[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOnDelay"];
			GetmySoList().GetAt(i)->SetOnDelay(lDelay);
			lDelay = pAppMod->m_smfDevice[GetStnName()]["Delays"][GetmySoList().GetAt(i)->GetName()]["lOffDelay"];
			GetmySoList().GetAt(i)->SetOffDelay(lDelay);
		}
	}

	return 1;
}

VOID CPI9000Stn::SetBusy(BOOL bMode)
{
	m_bIsBusy = bMode;
}

VOID CPI9000Stn::SetOfflineMode(BOOL bMode)
{
	m_bIsOfflineMode = bMode;
}

VOID CPI9000Stn::SetModSelected(BOOL bMode)
{
	if (!m_fHardware)
	{
		m_bModSelected = bMode;
	}
	else
	{
		if (bMode)
		{
			for (INT i = 0 ; i < GetmyMotorList().GetSize(); i++)
			{
				if (GetmyMotorList().GetAt(i)->PowerOn(ON) != GMP_SUCCESS)
				{
					m_bModSelected = FALSE;
					SetAlert(m_lStnErrorCode);
					break;
				}
			}

#if 1 //20130124
			for (INT i = 0 ; i < GetmyMotorList().GetSize(); i++)
			{
				GetmyMotorList().GetAt(i)->GetEncPosn(); //update encoder count
				Sleep(2);
				GetmyMotorList().GetAt(i)->IsPowerOn();
			}
#endif
			m_bModSelected = TRUE;
		}
		else
		{
			m_bModSelected = FALSE;
		}
	}
}

VOID CPI9000Stn::CheckModSelected(BOOL bMode, BOOL bNoPopup)
{
	if (m_fHardware)
	{
		for (INT i = 0 ; i < GetmyMotorList().GetSize(); i++)
		{
			if (!GetmyMotorList().GetAt(i)->IsPowerOn())
			{
				m_bModSelected = FALSE;
				if (!bNoPopup)
				{
					SetAlert(m_lStnErrorCode);
				}
				return;
			}
		}
		if (bMode)
		{
			if (!m_bModSelected)
			{
				SetAlert(m_lStnErrorCode);
			}
		}
	}
}

LONG CPI9000Stn::GetModSelectMode()
{
	return m_lModSelectMode;
}


VOID CPI9000Stn::SetStationID(LONG lStationID)
{
	m_lStationID = lStationID;
}

LONG CPI9000Stn::GetStationID()
{
	return m_lStationID;
}

////////////////////////////
//HMI Function
///////////////////////////
LONG CPI9000Stn::HMI_SetModSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction(__FUNCTION__);

	SetModSelected(bMode);

	return 0;
}

LONG CPI9000Stn::HMI_HomeAll(IPC_CServiceMessage &svMsg)
{
	BOOL	bMode = TRUE;
	LogAction(__FUNCTION__);

	for (INT i = 0; i < m_myMotorList.GetSize(); i++)
	{
		if (GetmyMotorList().GetAt(i)->Home(GMP_WAIT) != GMP_SUCCESS)
		{
			break;
		}
	}

	svMsg.InitMessage(sizeof(BOOL), &bMode);
	return 1;
}

LONG CPI9000Stn::HMI_SetDebugSeq(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction(__FUNCTION__);

	HMI_bDebugSeq = bMode;

	return 0;
}

LONG CPI9000Stn::HMI_SetDebugCal(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction(__FUNCTION__);

	HMI_bDebugCal = bMode;

	return 0;
}

LONG CPI9000Stn::HMI_SetLogMotion(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);
	LogAction(__FUNCTION__);

	HMI_bLogMotion = bMode;

	return 0;
}

VOID CPI9000Stn::setKlocworkTrue(BOOL bValue) //klocwork fix 20121211
{
	m_bKlocworkTrue = bValue;
	return;
}

BOOL CPI9000Stn::getKlocworkTrue()
{
	return m_bKlocworkTrue;
}
BOOL CPI9000Stn::getKlocworkFalse()
{
	return !m_bKlocworkTrue;
}

CMotorInfo &CPI9000Stn::GetMotorZ(WHZNum lWHZ) //whz rotary linear calib
{
	return g_stDummyMotor;
}

VOID CPI9000Stn::UpdateModuleSetupPosn()	// 20140729 Yip
{
}

VOID CPI9000Stn::UpdateModuleSetupLevel()	// 20140729 Yip
{
}

VOID CPI9000Stn::UpdateModuleSetupPara()	// 20140729 Yip
{
}

VOID CPI9000Stn::ModuleSetupSetPosn(LONG lSetupTitle, const CString &szAxis, LONG lOldPosn, LONG lNewPosn)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("%s = %ld", szAxis, lOldPosn);

	CString szNewValue;
	szNewValue.Format("%s = %ld", szAxis, lNewPosn);

	LogParameter(__FUNCTION__, g_stSetupPosn[lSetupTitle].szPosn, szOldValue, szNewValue);

	UpdateModuleSetupPosn();
}

VOID CPI9000Stn::ModuleSetupSetPosn(LONG lSetupTitle, MTR_POSN mtrOldPosn, MTR_POSN mtrNewPosn)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("X = %ld, Y = %ld", mtrOldPosn.x, mtrOldPosn.y);

	CString szNewValue;
	szNewValue.Format("X = %ld, Y = %ld", mtrNewPosn.x, mtrNewPosn.y);

	LogParameter(__FUNCTION__, g_stSetupPosn[lSetupTitle].szPosn, szOldValue, szNewValue);

	UpdateModuleSetupPosn();
}

VOID CPI9000Stn::ModuleSetupSetLevel(LONG lSetupTitle, LONG lOldLevel, LONG lNewLevel)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("Z = %ld", lOldLevel);

	CString szNewValue;
	szNewValue.Format("Z = %ld", lNewLevel);

	LogParameter(__FUNCTION__, g_stSetupLevel[lSetupTitle].szLevel, szOldValue, szNewValue);

	UpdateModuleSetupLevel();
}

BOOL CPI9000Stn::ModuleSetupSetPara(PARA stPara)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("LONG = %ld, DOUBLE = %.5f", g_stSetupPara[stPara.lSetupTitle].lPara, g_stSetupPara[stPara.lSetupTitle].dPara);

	UpdateModuleSetupPara();

	CString szNewValue;
	szNewValue.Format("LONG = %ld, DOUBLE = %.5f", g_stSetupPara[stPara.lSetupTitle].lPara, g_stSetupPara[stPara.lSetupTitle].dPara);

	LogParameter(__FUNCTION__, g_stSetupPara[stPara.lSetupTitle].szPara, szOldValue, szNewValue);

	return TRUE;
}

VOID CPI9000Stn::SetAndLogParameter(const CString &szFunction, const CString &szParameter, LONG &lParameter, LONG lNewValue)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("Value = %ld", lParameter);

	lParameter = lNewValue;

	CString szNewValue;
	szNewValue.Format("Value = %ld", lNewValue);

	LogParameter(szFunction, szParameter, szOldValue, szNewValue);
}

VOID CPI9000Stn::SetAndLogParameter(const CString &szFunction, const CString &szParameter, BOOL &bParameter, BOOL bNewValue)	// 20141118
{
	CString szOldValue;
	szOldValue.Format("Value = %d", (INT)bParameter);

	bParameter = bNewValue;

	CString szNewValue;
	szNewValue.Format("Value = %d", (INT)bNewValue);

	LogParameter(szFunction, szParameter, szOldValue, szNewValue);
}

VOID CPI9000Stn::SetAndLogParameter(const CString &szFunction, const CString &szParameter, ULONG &ulParameter, ULONG ulNewValue)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("Value = %lu", ulParameter);

	ulParameter = ulNewValue;

	CString szNewValue;
	szNewValue.Format("Value = %lu", ulNewValue);

	LogParameter(szFunction, szParameter, szOldValue, szNewValue);
}

VOID CPI9000Stn::SetAndLogParameter(const CString &szFunction, const CString &szParameter, DOUBLE &dParameter, DOUBLE dNewValue)	// 20140729 Yip
{
	CString szOldValue;
	szOldValue.Format("Value = %.5lf", dParameter);

	dParameter = dNewValue;

	CString szNewValue;
	szNewValue.Format("Value = %.5lf", dNewValue);

	LogParameter(szFunction, szParameter, szOldValue, szNewValue);
}

VOID CPI9000Stn::LogParameter(const CString &szFunction, const CString &szParameter, const CString &szOldValue, const CString &szNewValue)	// 20140729 Yip
{
	LogAction(szFunction);
	LogAction("%s\t%s", szParameter, szOldValue);
	LogAction("%s\t%s", szParameter, szNewValue);
	theApp.LogParameter(GetName(), szFunction, szParameter, szOldValue, szNewValue);
}
