/////////////////////////////////////////////////////////////////
//	WinEagle.cpp : interface of the CWinEagle class
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
#include "math.h"
#include "PI9000.h"
#include "PI9000_Constant.h"
#include "PR_Util.h"
#include "Cal_Util.h"

#define WINEAGLE_EXTERN
#include "WinEagle.h"

#include "INSPWH.h"
#include "WH1.h"
#include "WH2.h"
#include "PICKARM.h"
#include "InPickArm.h"
#include "OutPickArm.h"
#include "InspOpt.h"
#include "PI9000Stn.h"
#include "SettingFile.h"
#include <io.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CWinEagle, CPI9000Stn)

CWinEagle::CWinEagle()
{ 
	//PR Util
	nCamera						= INSPDL_CAM;
	nAutoModeCam				= INSPDL_CAM;
	bPRUseMouse					= FALSE;
	bPRUpdateLight				= TRUE;
	nCurCalPt					= CAL_LEFT;

	for (INT i = 0; i < NO_OF_INPUT_CHANNEL; i++)
	{
		bChannelInUse[i] = FALSE;
	}
	
	for (INT i = 0; i < PR_MAX_ROW_IN_TEXT_COORD + 1; i++)
	{
		bDirtyRow[i]			= FALSE;
	}

	// WinEagle
	m_bPRFailed					= FALSE;
	m_bPRDebug					= FALSE;

	m_szVisionNumber			= "---";
	m_szVisionVersion			= "---";
//	m_szVisionLibVersion		= "3.32.01";
	m_szVisionLibVersion		= "xx.xx.xxxx"; //20120106 get version no of interface

	m_bSwitchAutoMode			= FALSE;
	m_lAutoBondMode				= 0;

	m_bMoveToPosn				= FALSE;
	
	ulShowPRID					= 0;
	ulShowValidID				= 0;
	ulShowRecordType			= 0;
	ulShowOwnerID				= 0;
	
	ulTestCounter				= 0;
	ulTestDelay					= 0;

	ulShowExposureTime			= 0;
	
	bShowPRCoaxialCtrl			= FALSE;
	bShowPRCoaxial1Ctrl			= FALSE;
	bShowPRCoaxial2Ctrl			= FALSE;
	bShowPRRingCtrl				= FALSE;
	bShowPRRing1Ctrl			= FALSE;
	bShowPRSideCtrl				= FALSE;
	bShowPRSide1Ctrl			= FALSE;	
	bShowPRBackCtrl				= FALSE;

	ulHMICoaxialLevel			= 0;
	ulHMICoaxial1Level			= 0;
	ulHMICoaxial2Level			= 0;
	ulHMIRingLevel				= 0;
	ulHMIRing1Level				= 0;
	ulHMISideLevel				= 0;
	ulHMISide1Level				= 0;

	m_prZoomCentre.x			= PR_DEF_CENTRE_X;
	m_prZoomCentre.y			= PR_DEF_CENTRE_Y;
	g_stCursor.prCoord.x		= PR_DEF_CENTRE_X;
	g_stCursor.prCoord.y		= PR_DEF_CENTRE_Y;
	g_stCursor.lSize			= PRS_XLARGE_SIZE;	//PRS_LARGE_SIZE;
	g_stCursor.lColor			= PR_COLOR_GREEN;
	m_stCentreCursor.prCoord.x	= PR_DEF_CENTRE_X;
	m_stCentreCursor.prCoord.y	= PR_DEF_CENTRE_Y;
	m_stCentreCursor.lSize		= PRS_XLARGE_SIZE; //PRS_MEDIUM_SIZE;
	m_stCentreCursor.lColor		= PR_COLOR_RED;
	m_lCursorDiagnSteps			= 500;
	m_lCurrZoomFactor			= PR_ZOOM_FACTOR_1X;

	m_PRZoomedPRPt_a.x			= PR_DEF_CENTRE_X;
	m_PRZoomedPRPt_a.y			= PR_DEF_CENTRE_Y;
	ZoomCentre.x				= PR_DEF_CENTRE_X;
	ZoomCentre.y				= PR_DEF_CENTRE_Y;

#if 1 //20130906
	m_lIncludeAnglePosDeltaX	= 0;
	m_lIncludeAnglePosDeltaY	= 0;
	m_lIncludeAngleNegDeltaX	= 0;
	m_lIncludeAngleNegDeltaY	= 0;
#endif
	
	HMI_bCalibPRModSelected		= FALSE;
	HMI_bShowCalibPRModSelected	= FALSE;
	HMI_bCalibModSelected		= FALSE;
	HMI_bShowCalibModSelected	= FALSE;

	HMI_bPRUCalibrated			= FALSE;
	HMI_bCORCalibrated			= FALSE;

	HMI_dIncludedAnglePos		= 0.0; //20130530
	HMI_dIncludedAngleNeg		= 0.0;
	HMI_bShowIncludeAngle		= FALSE;

	HMI_lCalibPRDelay			= 0;
	HMI_dCalibCORRange			= 0.0;
	HMI_dTestCORAng				= 0.0;
	HMI_bShowAutoCal			= FALSE;
	HMI_bShowCORCal				= FALSE;
	HMI_bShowThermalRef			= FALSE; //20140627 PBDL calib

	HMI_bInspDLTableTab			= FALSE;
	HMI_bInspULTableTab			= FALSE;
	HMI_bVACSnr					= FALSE; //20130318

	HMI_ulPRAlg					= 0;
	HMI_ulPRFMarkType			= 0;

	HMI_bShowPRAlg				= FALSE;
	HMI_bShowFiducial			= FALSE;
	HMI_bShowCursorControl		= FALSE;
	HMI_bShowDigitalZoom		= FALSE;

	HMI_szCurPRStation			= "";
	HMI_lCurRecord				= 0;

	HMI_lErrorLimit				= 10; //20140923

	PRS_InitPRUs();				// must add here
}

CWinEagle::~CWinEagle()
{
}

BOOL CWinEagle::InitInstance()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CPI9000Stn::InitInstance();

	CString szMsg = _T("");
	INT nCount = 0;
	PRU *pPRU = NULL;

	//20120106 get version no of interface
	PR_UBYTE tmpVer[40];
	PRS_GetHostInterfaceVersionNo(&tmpVer[0]);
	m_szVisionLibVersion.Format("%s", tmpVer);
	szMsg.Format("vision interface version=%s", m_szVisionLibVersion);
	DisplayMessage(szMsg);

	// PR not initalized
	if (!g_bIsVisionNTInited && !m_bPRFailed)
	{
		BOOL bStatus = FALSE;

		if (!m_fHardware)
		{
			g_bIsVisionNTInited = TRUE;
			szMsg = _T("   ") + GetName() + " --- Not Enabled";
			DisplayMessage(szMsg);
		}
		else //if (pAppMod->m_bHWInited && pAppMod->m_bPRInit)
		{
//			do 
//			{
//				Sleep(1000);
//				nCount++;
//
//			} while (!pAppMod->ScanVisionNT());
//
//			if (g_bIsVisionNTInited || getKlocworkFalse())	// 20140722 Yip: Show Download PR Record Message After Vision Inited
//			{
//				SetAlert(IDS_WAIT_PR_DOWNLOAD_RECORD);
//			}
//
//#ifdef _DEBUG	// 20140714 Yip: Close "No grabber board detected" Dialog Automatically
//			HWND hHandle = NULL;
//			INT nTime = 0;
//			do 
//			{
//				Sleep(500);
//				hHandle = FindWindow("#32770", "ASM Vision Group");
//				nTime += 500;
//			} while ((hHandle == NULL) && (nTime < 30000));
//			if (hHandle != NULL)
//			{
//				SendMessage(hHandle, WM_SYSCOMMAND, SC_CLOSE, 0);
//			}
//#endif
//
//			szMsg.Format("Time Used for HMI Initialization = %ds", nCount);
//			DisplayMessage(szMsg);

			//bStatus = PRS_Init();
			bStatus = PRS_ConnectToVision();
			m_bPRFailed = !bStatus;

			if (!m_bPRFailed)
			{
				CString szMsg = "";
				DOUBLE dProcessTime = 0.0;
				DWORD dwStartTime = GetTickCount();

				PRS_InitOptics();

				dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
				szMsg.Format("PRS_InitOptics: %.5f", dProcessTime);
				DisplayMessage(szMsg);

				dwStartTime = GetTickCount();

				PRS_DisplayVideo(&pruINSP_DL_Calib[GLASS1][WH_1]);

				dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
				szMsg.Format("PRS_DisplayVideo: %.5f", dProcessTime);
				DisplayMessage(szMsg);

				szMsg = _T("   ") + GetName() + " --- PR Init Completed.";
				DisplayMessage(szMsg);

				PR_GET_VERSION_NO_RPY stRpy;

				if (PRS_GetVersionNo(&stRpy))
				{
					m_szVisionNumber = stRpy.aubSoftwareNo;
					m_szVisionVersion = stRpy.aubVersionNo;
				}

				//dProcessTime = 0.0;
				//dwStartTime = GetTickCount();
				FreeAllRecords();
				PRS_InitHookRFuncOnMouseClick();
				
				// debug time
				//dProcessTime = (DOUBLE)(GetTickCount() - dwStartTime);
				//szMsg.Format("Free All Record: %.5f", dProcessTime);
				//DisplayMessage(szMsg);
				
				//dProcessTime = 0.0;
				//dwStartTime = GetTickCount();
				//PR_PURPOSE emUserPurpose = PR_PURPOSE_USER_DEFINED_1;	// For New PR Auto-Bond Mode Display
				for (INT i = 1; i < GetmyPRUList().GetSize(); i++)
				{
					LONG lRetRecordID = 0;
					pPRU = GetmyPRUList().GetAt(i);;
					//emUserPurpose = (PR_PURPOSE)(PR_PURPOSE_USER_DEFINED_1 + i - 1);
					//if (PRS_CopyUserPurpose(emUserPurpose, pPRU->emPurpose[PURPOSE0]))	// For New PR Auto-Bond Mode Display
					//{
					//	pPRU->emPurpose[PURPOSE1] = pPRU->emPurpose[PURPOSE0];
					//	pPRU->emPurpose[PURPOSE0] = emUserPurpose;
					//}
					//////////
					if (::strlen((char*)&pPRU->stDownloadRecordCmd.acFilename[0]) == 0)
					{
						szMsg.Format("PR Load nothing. i=%d", i);
						DisplayMessage(szMsg);
					}
					else
					{
						PRS_DownloadRecordProcess(pPRU, TRUE, lRetRecordID);
						if (pPRU->bLoaded && lRetRecordID != 0)
						{
							szMsg.Format("PRFile %s, RecID:%ld", pPRU->stDownloadRecordCmd.acFilename, lRetRecordID);
							DisplayMessage(szMsg);
							SetRecordIDToPRUCmd(pPRU, (PRU_ID)lRetRecordID);
						}
						else
						{
							pPRU->bLoaded = FALSE;
							szMsg.Format("PRFile ERROR %s", pPRU->stDownloadRecordCmd.acFilename);
							DisplayMessage(szMsg);
						}
					}
					PRS_EnableHWTrigLighting(pPRU->nCamera, g_bEnableTrigLighting); //20141201
				}
				for (INT i = 0; i < GetmyCalibPRUList().GetSize(); i++)
				{
					LONG lRetRecordID = 0;
					pPRU = GetmyCalibPRUList().GetAt(i);
					if (::strlen((char*)&pPRU->stDownloadRecordCmd.acFilename[0]) == 0)
					{
						szMsg.Format("PR Calib Load nothing. i=%d", i);
						DisplayMessage(szMsg);
					}
					else
					{
						PRS_DownloadRecordProcess(pPRU, TRUE, lRetRecordID);
						if (pPRU->bLoaded)
						{
							szMsg.Format("CalPR File %s, RecID:%ld", pPRU->stDownloadRecordCmd.acFilename, lRetRecordID);
							DisplayMessage(szMsg);
							SetRecordIDToPRUCmd(pPRU, (PRU_ID)lRetRecordID);
						}
					}
				}


				CloseAlarm();		//If crash here. Please make sure that Group ID is correct.
				g_bIsVisionNTInited = bStatus;
				szMsg = _T("   ") + GetName() + " --- PR Records Downloaded.";
				DisplayMessage(szMsg);


			}
		}
	}

	return TRUE;
}


INT CWinEagle::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CPI9000Stn::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CWinEagle::Operation()
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
VOID CWinEagle::UpdateLighting(PRU *pPRU)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	if (pPRU == NULL)
	{
		return;
	}

	switch (pPRU->emCameraNo)
	{
	case PR_CAMERA_1:	//INSPWH UL
		bShowPRCoaxialCtrl 	= TRUE;
		bShowPRCoaxial1Ctrl = FALSE;
		bShowPRCoaxial2Ctrl = FALSE;
		bShowPRRingCtrl 	= TRUE;
		bShowPRRing1Ctrl 	= FALSE;
		bShowPRSideCtrl		= FALSE;
		bShowPRSide1Ctrl	= FALSE;
		bShowPRBackCtrl		= FALSE; //20150709
		break;

	case PR_CAMERA_2:	//INSPWH DL 1
		bShowPRCoaxialCtrl 	= TRUE;
		bShowPRCoaxial1Ctrl = FALSE;
		bShowPRCoaxial2Ctrl = FALSE;
		bShowPRRingCtrl 	= FALSE;
		bShowPRRing1Ctrl 	= FALSE;
		bShowPRSideCtrl		= TRUE;
		bShowPRSide1Ctrl	= TRUE;
		bShowPRBackCtrl		= FALSE; //20150709
		break;

	case PR_CAMERA_3:	//INSPWH DL 2
		bShowPRCoaxialCtrl 	= TRUE;
		bShowPRCoaxial1Ctrl = FALSE;
		bShowPRCoaxial2Ctrl = FALSE;
		bShowPRRingCtrl 	= FALSE;
		bShowPRRing1Ctrl 	= FALSE;
		bShowPRSideCtrl		= TRUE;
		bShowPRSide1Ctrl	= TRUE;
		bShowPRBackCtrl		= FALSE; //20150709
		break;

	}
}

VOID CWinEagle::UpdateOutput()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	if ((m_qState == UN_INITIALIZE_Q) || (m_qState == DE_INITIAL_Q))
	{
		return;
	}

	if (pAppMod->m_bHWInitError)
	{
		return;
	}

	if (!g_bIsVisionNTInited)
	{
		return;
	}

	PRU *pPRU = NULL;

	try
	{
		// update lighting
		if (bPRUpdateLight)
		{
			bPRUpdateLight = FALSE;

			if (nCamera != NO_CAM)
			{
				pPRU = &PRS_SetCameraPRU(nCamera);
		
				if (m_fHardware)
				{
					ulHMICoaxialLevel	= (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT);
					ulHMICoaxial1Level	= (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_1);
					ulHMICoaxial2Level	= (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_2);
					ulHMIRingLevel		= (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT);
					ulHMIRing1Level		= (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT_1);
					ulHMISideLevel		= (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT);
					ulHMISide1Level		= (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT_1);
					ulHMIBackLevel		= (ULONG)PRS_GetLighting(pPRU, PR_BACK_LIGHT);
				}
				else
				{
					ulHMICoaxialLevel	= 50;
					ulHMICoaxial1Level	= 60;
					ulHMICoaxial2Level	= 70;
					ulHMIRingLevel		= 80;
					ulHMIRing1Level		= 90;
					ulHMISideLevel		= 100;
					ulHMISide1Level		= 100;
					ulHMIBackLevel		= 50;
				}

			}
			else
			{
				ulHMICoaxialLevel	= 0;
				ulHMICoaxial1Level	= 0;
				ulHMICoaxial2Level	= 0;
				ulHMIRingLevel		= 0;
				ulHMIRing1Level		= 0;
				ulHMISideLevel		= 0;
				ulHMISide1Level		= 0;
			}
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

VOID CWinEagle::RegisterVariables()
{
	try
	{

		// -------------------------------------------------------
		// CPRTaskStn Variable and Function
		// -------------------------------------------------------
		RegVariable(_T("PR_ulCurPRU"), &HMI_ulCurPRU);
		RegVariable(_T("PR_ulPRID"), &HMI_ulPRID);
		RegVariable(_T("PR_dPRMatchScore"), &HMI_dPRMatchScore);
		RegVariable(_T("PR_dPRAngleRange"), &HMI_dPRAngleRange);
		RegVariable(_T("PR_dPRPercentVar"), &HMI_dPRPercentVar);
		RegVariable(_T("PR_bShowPRSearch"), &HMI_bShowPRSearch);

		RegVariable(_T("PR_lThermalRefPosnX"), &m_mtrThermalCalibRefPosn.x); //20140627 PBDL calib
		RegVariable(_T("PR_lThermalRefPosnY"), &m_mtrThermalCalibRefPosn.y);

		m_comServer.IPC_REG_SERVICE_COMMAND(_T("PR_SetPRMatchScore"), HMI_SetPRMatchScore);
		m_comServer.IPC_REG_SERVICE_COMMAND(_T("PR_SetPRAngRange"), HMI_SetPRAngRange);
		m_comServer.IPC_REG_SERVICE_COMMAND(_T("PR_SetPRPercentVar"), HMI_SetPRPercentVar);

		m_comServer.IPC_REG_SERVICE_COMMAND(_T("PR_SrchPR"), HMI_SrchPR);
		m_comServer.IPC_REG_SERVICE_COMMAND(_T("PR_ModifyPRCriteria"), HMI_ModifyPRCriteria);
		// -------------------------------------------------------
		// varibales
		// -------------------------------------------------------
		RegVariable(_T("PR_bSwitchAutoMode"), &m_bSwitchAutoMode);
		RegVariable(_T("PR_lAutoBondMode"), &m_lAutoBondMode);
		
		RegVariable(_T("VisionNumber"), &m_szVisionNumber);
		RegVariable(_T("VisionVersion"), &m_szVisionVersion);
		RegVariable(_T("VisionLibVersion"), &m_szVisionLibVersion);
		
		RegVariable(_T("PR_ulShowPRID"), &ulShowPRID);
		RegVariable(_T("PR_ulShowValidID"), &ulShowValidID);
		RegVariable(_T("PR_ulShowRecordType"), &ulShowRecordType);
		RegVariable(_T("PR_ulShowOwnerID"), &ulShowOwnerID);

		RegVariable(_T("PR_ulTestCounter"), &ulTestCounter);
		RegVariable(_T("PR_ulTestDelay"), &ulTestDelay);

		// Control Lighting
		RegVariable(_T("PR_ulShowExposureTime"), &ulShowExposureTime);

		RegVariable(_T("PR_bShowPRCoaxialCtrl"), &bShowPRCoaxialCtrl);
		RegVariable(_T("PR_bShowPRCoaxial1Ctrl"), &bShowPRCoaxial1Ctrl);
		RegVariable(_T("PR_bShowPRCoaxial2Ctrl"), &bShowPRCoaxial2Ctrl);
		RegVariable(_T("PR_bShowPRRingCtrl"), &bShowPRRingCtrl);
		RegVariable(_T("PR_bShowPRRing1Ctrl"), &bShowPRRing1Ctrl);
		RegVariable(_T("PR_bShowPRSideCtrl"), &bShowPRSideCtrl);
		RegVariable(_T("PR_bShowPRSide1Ctrl"), &bShowPRSide1Ctrl);
		RegVariable(_T("PR_bShowPRBackCtrl"), &bShowPRBackCtrl);	// 20150709

		RegVariable(_T("PR_ulHMICoaxialLevel"), &ulHMICoaxialLevel);
		RegVariable(_T("PR_ulHMICoaxial1Level"), &ulHMICoaxial1Level);
		RegVariable(_T("PR_ulHMICoaxial2Level"), &ulHMICoaxial2Level);
		RegVariable(_T("PR_ulHMIRingLevel"), &ulHMIRingLevel);
		RegVariable(_T("PR_ulHMIRing1Level"), &ulHMIRing1Level);
		RegVariable(_T("PR_ulHMISideLevel"), &ulHMISideLevel);
		RegVariable(_T("PR_ulHMISide1Level"), &ulHMISide1Level);
		RegVariable(_T("PR_ulHMIBackLevel"), &ulHMIBackLevel);	// 20150709

		//Calib Setting
		RegVariable(_T("PR_bCalibPRModSelected"), &HMI_bCalibPRModSelected);
		RegVariable(_T("PR_bShowCalibPRModSelected"), &HMI_bShowCalibPRModSelected);
		RegVariable(_T("PR_bCalibModSelected"), &HMI_bCalibModSelected);
		RegVariable(_T("PR_bShowCalibModSelected"), &HMI_bShowCalibModSelected);
		RegVariable(_T("PR_bShowThermalRef"), &HMI_bShowThermalRef); //20140627 PBDL calib

		RegVariable(_T("PR_bPRUCalibrated"), &HMI_bPRUCalibrated);
		RegVariable(_T("PR_bCORCalibrated"), &HMI_bCORCalibrated);
		RegVariable(_T("PR_lCalibPRDelay"), &HMI_lCalibPRDelay);
		RegVariable(_T("PR_dCalibCORRange"), &HMI_dCalibCORRange);
		RegVariable(_T("PR_dTestCORAng"), &HMI_dTestCORAng);
		RegVariable(_T("PR_bShowAutoCal"), &HMI_bShowAutoCal);
		RegVariable(_T("PR_bShowCORCal"), &HMI_bShowCORCal);

		RegVariable(_T("PR_dIncludedAnglePos"), &HMI_dIncludedAnglePos); //20130530
		RegVariable(_T("PR_dIncludedAngleNeg"), &HMI_dIncludedAngleNeg);
		RegVariable(_T("PR_bShowIncludeAngle"), &HMI_bShowIncludeAngle);

		RegVariable(_T("PR_bInspDLTableTab"), &HMI_bInspDLTableTab);
		RegVariable(_T("PR_bInspULTableTab"), &HMI_bInspULTableTab);

		// Setup Learn
		RegVariable(_T("PR_ulPRAlg"), &HMI_ulPRAlg);
		RegVariable(_T("PR_ulPRFMarkType"), &HMI_ulPRFMarkType);

		RegVariable(_T("PR_bShowPRAlg"), &HMI_bShowPRAlg);
		RegVariable(_T("PR_bShowFiducial"), &HMI_bShowFiducial);
		RegVariable(_T("PR_bShowCursorControl"), &HMI_bShowCursorControl);
		RegVariable(_T("PR_bShowDigitalZoom"), &HMI_bShowDigitalZoom);

		RegVariable(_T("PR_bVACSnr"), &HMI_bVACSnr); //20130318
	
		RegVariable(_T("PR_lCurRecord"), &HMI_lCurRecord);

		RegVariable(_T("PR_lErrorLimit"), &HMI_lErrorLimit); //20140923
	
		// commands
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISwitchCam", HMISwitchCam);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SelectCurStation", HMI_SelectCurStation);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SelectCurRecord", HMI_SelectCurRecord);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIUpdateDiagnPRScreen", HMIUpdateDiagnPRScreen);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIUpdateServicePRScreen", HMIUpdateServicePRScreen);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIClearPRRecord", HMIClearPRRecord);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIStartRepeatTest", HMIStartRepeatTest);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISetTestCounter", HMISetTestCounter);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISetTestDelay", HMISetTestDelay);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrExposureTime", HMIIncrExposureTime);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrExposureTime", HMIDecrExposureTime);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrCoaxialLevel", HMIIncrCoaxialLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrCoaxialLevel", HMIDecrCoaxialLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrCoaxialLevel10", HMIIncrCoaxialLevel10);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrCoaxialLevel10", HMIDecrCoaxialLevel10);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrCoaxial1Level", HMIIncrCoaxial1Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrCoaxial1Level", HMIDecrCoaxial1Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrCoaxial1Level10", HMIIncrCoaxial1Level10);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrCoaxial1Level10", HMIDecrCoaxial1Level10);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrCoaxial2Level", HMIIncrCoaxial2Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrCoaxial2Level", HMIDecrCoaxial2Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrCoaxial2Level10", HMIIncrCoaxial2Level10);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrCoaxial2Level10", HMIDecrCoaxial2Level10);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrRingLevel", HMIIncrRingLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrRingLevel", HMIDecrRingLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrRingLevel10", HMIIncrRingLevel10);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrRingLevel10", HMIDecrRingLevel10);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrRing1Level", HMIIncrRing1Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrRing1Level", HMIDecrRing1Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrRing1Level10", HMIIncrRing1Level10);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrRing1Level10", HMIDecrRing1Level10);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrSideLevel", HMIIncrSideLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrSideLevel", HMIDecrSideLevel);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrSideLevel10", HMIIncrSideLevel10);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrSideLevel10", HMIDecrSideLevel10);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrSide1Level", HMIIncrSide1Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrSide1Level", HMIDecrSide1Level);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrSide1Level10", HMIIncrSide1Level10);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrSide1Level10", HMIDecrSide1Level10);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrBackLevel", HMIIncrBackLevel);	// 20150709
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrBackLevel", HMIDecrBackLevel);	// 20150709
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIIncrBackLevel10", HMIIncrBackLevel10);	// 20150709
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIDecrBackLevel10", HMIDecrBackLevel10);	// 20150709

		// Cursor Related
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetCursorDiagnSteps", HMI_SetCursorDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_IndexCursorXPos", HMI_IndexCursorXPos);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_IndexCursorXNeg", HMI_IndexCursorXNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_IndexCursorYPos", HMI_IndexCursorYPos);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_IndexCursorYNeg", HMI_IndexCursorYNeg);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_ZoomIn", HMI_ZoomIn);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_ZoomOut", HMI_ZoomOut);
		
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIFreeAllRecords", HMIFreeAllRecords);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMIOnOffLogMode", HMIOnOffLogMode);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISetDebugFlag", HMISetDebugFlag);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISetComLogFlag", HMISetComLogFlag);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISetComLogTimeLimit", HMISetComLogTimeLimit);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISetVisionNtDlgToService", HMISetVisionNtDlgToService);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_HMISetVisionNtDlgToVision", HMISetVisionNtDlgToVision);

		// Caibration Setup Page
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetCalibPRSelected", HMI_SetCalibPRSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetCalibModSelected", HMI_SetCalibModSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SelectCalibPRU", HMI_SelectCalibPRU);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SelectPRAlg", HMI_SelectPRAlg);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SelectFMarkType", HMI_SelectFMarkType);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetCalibPRDelay", HMI_SetCalibPRDelay);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetCalibCORRange", HMI_SetCalibCORRange);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetTestCORAng", HMI_SetTestCORAng);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_AutoCal", HMI_AutoCal);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_CORCal", HMI_CORCal);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_AutoTest", HMI_AutoTest);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_CORAutoTest", HMI_CORAutoTest);//20190923

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetPRErrorLimit", HMI_SetPRErrorLimit);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_CheckPRCentre", HMI_CheckPRCentre);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetCORCentre", HMI_SetCORCentre);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_CheckCORCentre", HMI_CheckCORCentre);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_GetPRCentreMap", HMI_GetPRCentreMap);

		m_comServer.IPC_REG_SERVICE_COMMAND("PR_ToggleVAC", HMI_ToggleVAC); //20130318
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_ResetIncludedAngle", HMI_ResetIncludedAngle); //20130530

		// 20140815 Yip
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetUserSearchPRContinue", HMI_SetUserSearchPRContinue);
		m_comServer.IPC_REG_SERVICE_COMMAND("PR_SetUserRejectPRTask", HMI_SetUserRejectPRTask);

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
VOID CWinEagle::PreStartOperation()
{
	if (m_fHardware && m_lAutoBondMode)
	{
		m_bSwitchAutoMode = FALSE;
		PRS_AutoBondMode(TRUE, FALSE);
	}

#ifdef MOUSE_CLICK_PR
	m_bUserRejectPRTask = FALSE;
	m_bUserSearchPRDone = FALSE;

	WINEAGLE_Status = ST_IDLE;
	m_qSubOperation = WINEAGLE_IDLE_Q;
	m_bStopCycle = FALSE;
	g_bShowUserSearchPRDone = FALSE;
#endif
	Motion(FALSE);
	State(IDLE_Q);
#ifdef MOUSE_CLICK_PR
	m_deqDebugSeq.clear(); //20131010
	m_qPreviousSubOperation = m_qSubOperation;
	UpdateDebugSeq(GetSequenceString(), WINEAGLE_Status);
	
#endif

	CPI9000Stn::PreStartOperation();
}

VOID CWinEagle::AutoOperation()
{
	if (m_fHardware && m_bSwitchAutoMode && m_lAutoBondMode)
	{
		m_bSwitchAutoMode = FALSE;
		PRS_AutoBondMode(TRUE, TRUE);
	}

	if (m_fHardware && m_bSwitchAutoMode && !m_lAutoBondMode)
	{
		m_bSwitchAutoMode = FALSE;
		PRS_AutoBondMode(FALSE, FALSE);
	}
#ifdef MOUSE_CLICK_PR
	OperationSequence(); //20150813
#endif
	// stop command received: end operation after cycle completed
	if (Command() == glSTOP_COMMAND || IsAbort() || g_bStopCycle)
	{
		Command(glSTOP_COMMAND);
		State(STOPPING_Q);
	
#ifdef MOUSE_CLICK_PR
		WINEAGLE_Status = ST_STOP;
		m_bStopCycle = FALSE;	
		CPI9000Stn::StopOperation();
#endif
	}
}

STATUS_TYPE *CWinEagle::GetRequestClickStatus()	// 20140815 Yip
{
	return &WINEAGLE_Status;	// NEED to change when enable click PR
}

CString CWinEagle::GetSequenceString()	// 20140815 Yip
{
	switch (m_qSubOperation)
	{
	case WINEAGLE_IDLE_Q:
		return "WINEAGLE_IDLE_Q";

	case WINEAGLE_WAIT_REQUEST_Q:
		return "WINEAGLE_WAIT_REQUEST_Q";

	case WINEAGLE_WAIT_USER_CLICK_PR_Q:
		return "WINEAGLE_WAIT_USER_CLICK_PR_Q";

	case WINEAGLE_WAIT_STATION_RELEASE_Q:
		return "WINEAGLE_WAIT_STATION_RELEASE_Q";

	case WINEAGLE_STOP_Q:
		return "WINEAGLE_STOP_Q";
	}

	return "Hi";
}

VOID CWinEagle::OperationSequence()	// 20140815 Yip: Add WinEagle Auto Bond Operation Sequence
{
	STATUS_TYPE *emStatus = NULL;
	switch (m_qSubOperation)
	{
	case WINEAGLE_IDLE_Q:
		if (m_bStopCycle)
		{
			m_qSubOperation = WINEAGLE_STOP_Q;
		}
		else
		{
			m_qSubOperation = WINEAGLE_IDLE_Q;
			//m_qSubOperation = WINEAGLE_WAIT_REQUEST_Q;
		}
		break;

	case WINEAGLE_WAIT_REQUEST_Q:
		if (m_bStopCycle)
		{
			m_qSubOperation = WINEAGLE_STOP_Q;
		}
		else if ((emStatus = GetRequestClickStatus()) != NULL)
		{
			// Turn Off AutoBond First
			if (m_lAutoBondMode) // Reset Display Video
			{
				PRS_AutoBondMode(FALSE, FALSE);
			}
			*emStatus = ST_ACK_CLICK_PR;

			WINEAGLE_Status = ST_BUSY;

			m_qSubOperation = WINEAGLE_WAIT_USER_CLICK_PR_Q;
		}
		break;

	case WINEAGLE_WAIT_USER_CLICK_PR_Q:
		if (m_bStopCycle)	// User Cancel Operation
		{
			m_qSubOperation = WINEAGLE_STOP_Q;
		}
		else if (m_bUserSearchPRDone) // Waiting Finish
		{
			// NOT AutoBond Mode
			if (!m_lAutoBondMode) // Reset Display Video
			{
				PRU *pPRU;
				pPRU = &PRS_SetCameraPRU(nCamera);
				PRS_DisplayVideo(pPRU);
			}
			else
			{
				PRS_AutoBondMode(TRUE, FALSE);
			}

			WINEAGLE_Status = ST_DONE;

			m_bUserSearchPRDone = FALSE;
			m_qSubOperation = WINEAGLE_WAIT_STATION_RELEASE_Q;
		}
		else if (m_bUserRejectPRTask)
		{
			// NOT AutoBond Mode
			if (!m_lAutoBondMode) // Reset Display Video
			{
				PRU *pPRU;
				pPRU = &PRS_SetCameraPRU(nCamera);
				PRS_DisplayVideo(pPRU);
			}
			else
			{
				PRS_AutoBondMode(TRUE, FALSE);
			}

			WINEAGLE_Status = ST_USER_REJECT;

			m_bUserRejectPRTask = FALSE;
			m_qSubOperation = WINEAGLE_WAIT_STATION_RELEASE_Q;

		}
		break;

	case WINEAGLE_WAIT_STATION_RELEASE_Q:
		if (WINEAGLE_Status == ST_RELEASE)
		{
			WINEAGLE_Status = ST_IDLE;
			m_qSubOperation = WINEAGLE_WAIT_REQUEST_Q;
		}
		break;

	case WINEAGLE_STOP_Q:
		break;
	}

	if (m_qPreviousSubOperation != m_qSubOperation)
	{
		if (HMI_bDebugSeq)
		{
			CString szMsg;
			if (WINEAGLE_STOP_Q != m_qSubOperation)
			{
				szMsg.Format("%s Go State: %ld", GetStnName(), m_qSubOperation);
			}
			else
			{
				szMsg.Format("%s Go Stop", GetStnName());
			}
			DisplayMessage(szMsg);

		}
		UpdateDebugSeq(GetSequenceString(), WINEAGLE_Status);
		m_qPreviousSubOperation = m_qSubOperation;
	}
}


// My derivation
BOOL CWinEagle::PRS_Init()
{
	unsigned short		Channel[TOTAL_NO_OF_CHANNEL];
	//unsigned short		Channel[TOTAL_NO_OF_CHANNEL];
	PR_WARM_START_RPY	stCommRpy;
	PR_UWORD			uwStatus = PR_COMM_NOERR;

	CString szMsg = _T("");
	BOOL bResult = FALSE;

	//INT	i;
	INT	j;

//	do //klocwork fix 20121211
	while (TRUE)
	{
		open_EmuSI_drvr(TRUE);

		for (INT i = 0; i < TOTAL_NO_OF_CHANNEL; ++i)
		{
			Channel[i] = 0x200;
		}

		//Channel[0] = 0x200;
		//Channel[1] = 0x200;
		//Channel[2] = 0x200;
		//Channel[3] = 0x80;
		//Channel[4] = 0x80;
		//Channel[5] = 0x80;
		//Channel[6] = 0x80;
		//Channel[7] = 0x80;

		//Channel[8] = 0x80;
		//Channel[9] = 0x80;
		//Channel[10] = 0x80;
		//Channel[11] = 0x80;

		//Channel[0] = 0x200;
		//Channel[1] = 0x200;
		//Channel[2] = 0x200;
		//Channel[3] = 0x200;
		//Channel[4] = 0x200;
		//Channel[5] = 0x200;
		//Channel[6] = 0x200;
		//Channel[7] = 0x80;

		//Channel[8] = 0x80;
		//Channel[9] = 0x80;
		//Channel[10] = 0x80;
		//Channel[11] = 0x80;

		//Channel[12] = 0x80;
		//Channel[13] = 0x80;
		//Channel[14] = 0x80;
		//Channel[15] = 0x80;
		//Channel[16] = 0x80;
		//Channel[17] = 0x80;
		//Channel[18] = 0x80;
		//Channel[19] = 0x80;
		//Channel[20] = 0x80;
		//Channel[21] = 0x80;
		//Channel[22] = 0x80;
		//Channel[23] = 0x80;
		//Channel[24] = 0x80;
		//Channel[25] = 0x80;

		// Initialize the SI commnication
		m_pDPR = Dpr_Master_Init_Ext(DPR_PORT, NO_OF_INPUT_CHANNEL, NO_OF_OUTPUT_CHANNEL, 
									 HOST_SYSTEM_ID, Channel, "PI9000 Host", DPR_EMULATED_MODE);

		if (NULL == m_pDPR)
		{
			SetError(IDS_DPR_MASTER_INIT_ERR);
			DisplayMessage("ERROR: DPR Master Init failed!");
			break;
		}
		szMsg = _T("   ") + GetName() + " --- DPR Master Init success.";
		DisplayMessage(szMsg);

		// Wait for Slave to be initialized (300 sec timeout)
		j = 0;
		do
		{
			Sleep(1000);
			j++;
		} while (1 == Dpr_Slave_Initialized(m_pDPR) && j < 300);

		//szMsg.Format("Time Used for DPR Slave Initialization = %ds", j);
		//DisplayMessage(szMsg);

		if (j >= 300)
		{
			CloseAlarm();
			SetError(IDS_DPR_SLAVE_INIT_ERR);
			DisplayMessage("ERROR: DPR Slave Init timeout. Process terminated!");
			break;
		}
		szMsg = _T("   ") + GetName() + " --- DPR Slave Init success.";
		DisplayMessage(szMsg);

		// Install the interupt callback function
		Dpr_Master_InstallInterrupt(m_pDPR, PR_SetSICallBack, NULL);

		if (Dpr_IsInterruptEnabled(m_pDPR))
		{
			szMsg = _T("   ") + GetName() + " --- Master with interrupt ready.";
			DisplayMessage(szMsg);
		}
		else
		{
			szMsg = _T("   ") + GetName() + " --- Master without interrupt ready.";
			DisplayMessage(szMsg);
		}

		Sleep(1000);	// Ensure interrupt handler is installed before sending command

		// Initialize the communication in vision host interface
		PR_InitMultiSI(m_pDPR, HOST_SYSTEM_ID, VISION_SYSTEM_ID);

		// Call warm start to initalize the vision system
		PR_WarmStartCmd(PRS_INSPUL_SID, PRS_INSPUL_RID, &stCommRpy);

		if (PR_COMM_NOERR != stCommRpy.uwCommunStatus)
		{
			uwStatus = stCommRpy.uwCommunStatus;
		}
		else if (PR_ERR_NOERR != stCommRpy.uwPRStatus)
		{
			uwStatus = stCommRpy.uwPRStatus;
		}

		if (PR_ERR_NOERR != uwStatus)
		{
			SetError(IDS_PR_WARM_START_ERR);
			DisplayMessage("ERROR: PR warm start failed!");
			break;
		}

		// Initalized sucessfully
		bResult = TRUE;
		break;
	}
	//} while(TRUE); //klocwork fix 20121211

	return bResult;
}

BOOL CWinEagle::PRS_ConnectToVision()
{
#ifdef FAKE_VISION
	return TRUE;
#else
	PR_CONNECT_TO_VISION_STRUCT	stConnectToVisionStruct;
	PR_COMMON_RPY				stCommRpy;
	PR_UWORD					uwStatus = PR_COMM_NOERR;
	INT i = 0;

	PR_InitConnectToVisionStruct(&stConnectToVisionStruct);

	stConnectToVisionStruct.emIsMasterInit = PR_TRUE; //VPC
	stConnectToVisionStruct.emCommMedia = PR_COMM_MEDIA_SI; //VPC
	stConnectToVisionStruct.ubHostID = HOST_SYSTEM_ID;
	stConnectToVisionStruct.ubPRID = VISION_SYSTEM_ID;

	stConnectToVisionStruct.ubNumOfHostInCh = NO_OF_INPUT_CHANNEL;
	stConnectToVisionStruct.ubNumOfHostOutCh = NO_OF_OUTPUT_CHANNEL;

	for (i = 0; i < NO_OF_INPUT_CHANNEL; i++)
	{
		stConnectToVisionStruct.auwHostInChSize[i] = 64; 
		stConnectToVisionStruct.auwRPCInChSize[i] = 64; 
	}

	for (i = 0; i < NO_OF_OUTPUT_CHANNEL; i++)
	{
		stConnectToVisionStruct.auwHostOutChSize[i] = 64;
		stConnectToVisionStruct.auwRPCOutChSize[i] = 64;
	}

	stConnectToVisionStruct.ubNumOfRPCInCh	= 2;
	stConnectToVisionStruct.ubNumOfRPCOutCh = 2;
	stConnectToVisionStruct.ulDprBaseAddress= 0; 
    stConnectToVisionStruct.ubNumOfHostInCh = NO_OF_INPUT_CHANNEL;

	stConnectToVisionStruct.ulTimeout = 300000;		// 5 Mins
	PR_ConnectToVision(&stConnectToVisionStruct, &stCommRpy);

	if (PR_COMM_NOERR != stCommRpy.uwCommunStatus)
	{
		uwStatus = stCommRpy.uwCommunStatus;
	}
	else if (PR_ERR_NOERR != stCommRpy.uwPRStatus)
	{
		uwStatus = stCommRpy.uwPRStatus;
	}

	if (PR_ERR_NOERR != uwStatus)
	{
		SetError(IDS_PR_WARM_START_ERR);
		DisplayMessage("ERROR: PR start failed!");
		return FALSE;
	}
	else
	{
		// Initalized sucessfully
		return TRUE;
	}
#endif
}	



VOID CWinEagle::PRS_InitOptics()
{
	INT		i = 0;
	PRU	*pPRU = NULL;

	for (i = 0; i < GetmyPRUList().GetSize(); i++)
	{
		pPRU = GetmyPRUList().GetAt(i);
		PRS_GetOptic(pPRU);
	}

	for (i = 0; i < GetmyCalibPRUList().GetSize(); i++)
	{
		pPRU = GetmyCalibPRUList().GetAt(i);
		PRS_GetOptic(pPRU);
	}
}

VOID CWinEagle::PRS_InitPRUs()
{
	INT		i = 0;
	CString szFileName;

	m_myPRUList.RemoveAll();
	m_myCalibPRUList.RemoveAll();
	///////////////////////
	// Calib PRU
	// INSPWH Cam with INSPWH Calibration PRU

	szFileName.Format("InspWH1_DL1_CalibPRU");
	PRS_InitPRU(&pruINSP_DL_Calib[GLASS1][WH_1], INSPDL_CAM, ID_INSPDL1_WH1_CALIB_PRU + i, "InspWH_DL1", szFileName, CMG_INSPDL_XINPA_YWH1, CMG_NONE, CMG_NONE, CMG_NONE);
	m_myCalibPRUList.Add(&pruINSP_DL_Calib[GLASS1][WH_1]);

	szFileName.Format("InspWH2_DL1_CalibPRU");
	PRS_InitPRU(&pruINSP_DL_Calib[GLASS1][WH_2], INSPDL_CAM, ID_INSPDL1_WH2_CALIB_PRU, "InspWH_DL1", szFileName, CMG_NONE, CMG_INSPDL_XINPA_YWH2, CMG_NONE, CMG_NONE);
	m_myCalibPRUList.Add(&pruINSP_DL_Calib[GLASS1][WH_2]);
	
	szFileName.Format("InspWH1_DL2_CalibPRU");
	PRS_InitPRU(&pruINSP_DL_Calib[GLASS2][WH_1], INSPDL2_CAM, ID_INSPDL2_WH1_CALIB_PRU, "InspWH_DL2", szFileName, CMG_INSPDL_XINPA_YWH1, CMG_NONE, CMG_NONE, CMG_NONE);
	m_myCalibPRUList.Add(&pruINSP_DL_Calib[GLASS2][WH_1]);
	
	szFileName.Format("InspWH2_DL2_CalibPRU");
	PRS_InitPRU(&pruINSP_DL_Calib[GLASS2][WH_2], INSPDL2_CAM, ID_INSPDL2_WH2_CALIB_PRU, "InspWH_DL2", szFileName, CMG_NONE, CMG_INSPDL_XINPA_YWH2, CMG_NONE, CMG_NONE);
	m_myCalibPRUList.Add(&pruINSP_DL_Calib[GLASS2][WH_2]);
	
	szFileName.Format("InspWH_UL_WH1_CalibPRU");
	PRS_InitPRU(&pruINSPWH_UL_Calib[WH_1], INSPUL_CAM, ID_INSPWHUL_WH1_CALIB_PRU, "InspWH_UL", szFileName, CMG_INSPUL_XINSPOPT_YWH1, CMG_NONE, CMG_NONE, CMG_NONE);
	m_myCalibPRUList.Add(&pruINSPWH_UL_Calib[WH_1]);
	
	szFileName.Format("InspWH_UL_WH2_CalibPRU");
	PRS_InitPRU(&pruINSPWH_UL_Calib[WH_2], INSPUL_CAM, ID_INSPWHUL_WH2_CALIB_PRU, "InspWH_UL", szFileName, CMG_NONE, CMG_INSPUL_XINSPOPT_YWH2, CMG_NONE, CMG_NONE);
	m_myCalibPRUList.Add(&pruINSPWH_UL_Calib[WH_2]);

	// Menu PRU
	pruMenu.emID			= ID_NONE;
	PRS_InitPRU(&pruMenu, NO_CAM, ID_NONE , "", "", CMG_NONE, CMG_NONE, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruMenu);

	szFileName.Format("InspWH_Glass1_PR1");
	PRS_InitPRU(&pruDLPR1[GLASS1], INSPDL_CAM, ID_DL_GLASS1_PR1, "InspWH DL1", szFileName, CMG_INSPDL_XINPA_YWH1, CMG_INSPDL_XINPA_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruDLPR1[GLASS1]);
	szFileName.Format("InspWH_Glass1_PR2");
	PRS_InitPRU(&pruDLPR2[GLASS1], INSPDL_CAM, ID_DL_GLASS1_PR2, "InspWH DL1", szFileName, CMG_INSPDL_XINPA_YWH1, CMG_INSPDL_XINPA_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruDLPR2[GLASS1]);

	szFileName.Format("InspWH_Glass2_PR1");
	PRS_InitPRU(&pruDLPR1[GLASS2], INSPDL2_CAM, ID_DL_GLASS2_PR1, "InspWH DL2", szFileName, CMG_INSPDL_XINPA_YWH1, CMG_INSPDL_XINPA_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruDLPR1[GLASS2]);
	szFileName.Format("InspWH_Glass2_PR2");
	PRS_InitPRU(&pruDLPR2[GLASS2], INSPDL2_CAM, ID_DL_GLASS2_PR2, "InspWH DL2", szFileName, CMG_INSPDL_XINPA_YWH1, CMG_INSPDL_XINPA_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruDLPR2[GLASS2]);

	szFileName.Format("InspOPT_COF_PR1");
	PRS_InitPRU(&pruULCOF1, INSPUL_CAM, ID_UL_COF_PR1, "InspOpt", szFileName, CMG_INSPUL_XINSPOPT_YWH1, CMG_INSPUL_XINSPOPT_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruULCOF1);


	szFileName.Format("InspOPT_COF_PR2");
	PRS_InitPRU(&pruULCOF2, INSPUL_CAM, ID_UL_COF_PR2, "InspOpt", szFileName, CMG_INSPUL_XINSPOPT_YWH1, CMG_INSPUL_XINSPOPT_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruULCOF2);

	szFileName.Format("InspOPT_Glass_PR1");
	PRS_InitPRU(&pruULGlass1, INSPUL_CAM, ID_UL_GLASS_PR1, "InspOpt", szFileName, CMG_INSPUL_XINSPOPT_YWH1, CMG_INSPUL_XINSPOPT_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruULGlass1);

	szFileName.Format("InspOPT_Glass_PR2");
	PRS_InitPRU(&pruULGlass2, INSPUL_CAM, ID_UL_GLASS_PR2, "InspOpt", szFileName, CMG_INSPUL_XINSPOPT_YWH1, CMG_INSPUL_XINSPOPT_YWH2, CMG_NONE, CMG_NONE);
	m_myPRUList.Add((PRU*)&pruULGlass2);

	LONG ltemp;
	ltemp = GetmyPRUList().GetSize();
	ltemp = GetmyCalibPRUList().GetSize();
	//protection the total num of myPRUList must equal to the last ID...
	if (GetmyPRUList().GetSize() + GetmyCalibPRUList().GetSize() > ID_MAX_NUM/*ID_SHUTTLE2_CALIB+1*/) // Must Use Afx to Pump Our Error
	{
		CString szError;
		AfxMessageBox("Error:: InitPRU");
		szError.Format("myPRUList.GetSize() = %ld", GetmyPRUList().GetSize());
		AfxMessageBox(szError);
		szError.Format("myCalibPRUList.GetSize() = %ld", GetmyCalibPRUList().GetSize());
		AfxMessageBox(szError);
	}

	// Dummy PRU
	//pruDummy.emID						= ID_NONE;
	//PRS_InitPRU(&pruDummy, NO_CAM);
	
}

VOID CWinEagle::FreeAllRecords()
{
	PRU *pPRU = NULL;
	//PRS_FreeAllRecords();
	PRS_FreeAllRecordsAllHost();

	for (INT i = 1; i < GetmyPRUList().GetSize(); i++)
	{
		pPRU = GetmyPRUList().GetAt(i);
		pPRU->bLoaded						= FALSE;
		pPRU->stSrchCmd.uwRecordID			= 0;//pPRU->emID;
		pPRU->stSrchDieCmd.auwRecordID[0]	= 0;//pPRU->emID;
		//pPRU->stChangeRecordCmd.uwRecordID	= 0;//pPRU->emID;	//20150109
		pPRU->stDetectShapeCmd.uwRecordID	= 0;//pPRU->emID;
		pPRU->stSrchTmplCmd.uwRecordID		= 0;//pPRU->emID;
		//pPRU->stSrchACFCmd.uwRecordID		= 0;//pPRU->emID;
		//pPRU->stModifyCritCmd.uwRecordID	= pPRU->emID; //20180208
		pPRU->stLrnCmd.uwRecordID			= 0; //pPRU->emID; //20180208 Let vision assign Record ID
	}

	for (INT i = 0; i < GetmyCalibPRUList().GetSize(); i++)
	{
		pPRU = GetmyCalibPRUList().GetAt(i);
		pPRU->bLoaded						= FALSE;
		pPRU->stSrchCmd.uwRecordID			= 0;//pPRU->emID;
		pPRU->stSrchDieCmd.auwRecordID[0]	= 0;//pPRU->emID;
		//pPRU->stChangeRecordCmd.uwRecordID	= 0;//pPRU->emID;	//20150109
		pPRU->stDetectShapeCmd.uwRecordID	= 0;//pPRU->emID;
		pPRU->stSrchTmplCmd.uwRecordID		= 0;//pPRU->emID;
		//pPRU->stSrchACFCmd.uwRecordID		= 0;//pPRU->emID;
		//pPRU->stModifyCritCmd.uwRecordID	= pPRU->emID; //20180208
		pPRU->stLrnCmd.uwRecordID			= 0;//pPRU->emID; //20180208 Let vision assign Record ID
	}
}

VOID CWinEagle::SetRecordIDToPRUCmd(PRU *pPRU, PRU_ID emID)
{
	if (pPRU->bLoaded)
	{
		pPRU->stSrchCmd.uwRecordID			= emID;
		pPRU->stSrchDieCmd.auwRecordID[0]	= emID;
		//pPRU->stChangeRecordCmd.uwRecordID	= emID;
		pPRU->stDetectShapeCmd.uwRecordID	= emID;
		pPRU->stSrchTmplCmd.uwRecordID		= emID;
		//pPRU->stSrchACFCmd.uwRecordID		= emID;
		pPRU->stModifyCritCmd.uwRecordID = emID; //20180208
		pPRU->stLrnCmd.uwRecordID			= 0;
	}
}

PRU &CWinEagle::SelectCurPRU(ULONG ulPRU)
{
	switch (ulPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		return pruINSP_DL_Calib[GLASS1][WH_1];

	case INSPDL2_WH1_CALIB_PRU:
		return pruINSP_DL_Calib[GLASS2][WH_1];

	case INSPDL1_WH2_CALIB_PRU:
		return pruINSP_DL_Calib[GLASS1][WH_2];

	case INSPDL2_WH2_CALIB_PRU:
		return pruINSP_DL_Calib[GLASS2][WH_2];
	
	case INSPWHUL_WH1_CALIB_PRU:
		return pruINSPWH_UL_Calib[WH_1];
	
	case INSPWHUL_WH2_CALIB_PRU:
		return pruINSPWH_UL_Calib[WH_2];

	default:
		DisplayMessage("SelectCurPRU Error");
		return pruINSP_DL_Calib[GLASS1][WH_1];
	}
}

VOID CWinEagle::StartRepeatTest(PRU *pPRU)
{
	BOOL	bSrch = FALSE;
	CString szMsg = _T("");
	CString szPrintTitle = _T("PR_Test");
	INT		nCnt = ulTestCounter;

	if (!pPRU->bLoaded)
	{
		HMIMessageBox(MSG_OK, "PR REPEATABILITY TEST", "PR Pattern Not Loaded!");
		WriteHMIMess("Test Failed: Please re-learn pattern");
		return;
	}

	WriteHMIMess("PR Test in progress...");

	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	// start test	
	FILE *fp = fopen("D:\\sw\\PI9000\\Data\\PR_Test.txt", "wt");
	if (fp != NULL)
	{
		fprintf(fp, "N:\tX:\tY:\tAng:\tScore:\n");
		fclose(fp);
	}
	//else //klocwork fix 20121211
	//{
	//	if (fp != NULL)
	//	{
	//		fprintf(fp, "Record %d, PR Test", pPRU->emID);
	//	}
	//}

	while (pAppMod->bPRRepeatTest && nCnt > 0)
	{
		bSrch = PRS_ManualSrchCmd(pPRU);
		//switch (pPRU->emRecordType)
		//{
		//// Pattern Matching or Edge Matching
		//case PRS_DIE_TYPE:
		//	bSrch = PRS_SrchDie(pPRU);
		//	break;

		//// Shape Fitting
		//case PRS_SHAPE_TYPE:
		//	bSrch = PRS_DetectShape(pPRU);
		//	break;

		//// Tmpl Fitting
		//case PRS_TMPL_TYPE:
		//	bSrch = PRS_SrchTmpl(pPRU);
		//	break;
		//}

		fp = fopen("D:\\sw\\PI9000\\Data\\PR_Test.txt", "a");

		if (bSrch)
		{
			szMsg.Format("Cycle %ld XY: (%.2f,%.2f), Ang: %.2f, Score: %.2f", 
						 ulTestCounter, pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, pPRU->fAng, pPRU->fScore);

			if (fp != NULL)
			{
				fprintf(fp, "%lu\t%.2f\t%.2f\t%.2f\t%.2f\n", ulTestCounter, pPRU->rcoDieCentre.x, pPRU->rcoDieCentre.y, pPRU->fAng, pPRU->fScore);
				fclose(fp);
			}
		}
		else
		{
			szMsg.Format("Cycle %ld Search Failed", ulTestCounter);
			if (fp != NULL)
			{
				fprintf(fp, "SEARCH FAIL\n");
				fclose(fp);
			}
		}

		nCnt--;

		WriteHMIMess(szMsg);
		Sleep(ulTestDelay);
	}

	if (!pAppMod->bPRRepeatTest)
	{
		WriteHMIMess("PR Test Cancelled");
	}
	else
	{
		WriteHMIMess("PR Test Completed");
		pAppMod->bPRRepeatTest = FALSE;
	}
}





///////////////////////////////////////////////////////
// Updated Page
//////////////////////////////////////////////////////
VOID CWinEagle::UpdateHMIDiagnPRScreen(BOOL bOperation)
{
	PRU	*pPRU = NULL;
		
	pPRU	= GetmyPRUList().GetAt(HMI_ulCurPRU);
	
	PRS_DisplayVideo(pPRU);

	if (!PRS_DispLrnInfo(pPRU))
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "No Pattern Learnt");
	}
	else
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Displaying Learnt Pattern");
	}

	ulShowExposureTime	= (ULONG)PRS_GetExposureTime(pPRU);

	// pattern loaded: get record info
	if (pPRU->bLoaded)
	{
		ulShowPRID = pPRU->stSrchCmd.uwRecordID;
		//switch (pPRU->emRecordType)
		//{
		//// Pattern Matching or Edge Matching
		//case PRS_DIE_TYPE:
		//	ulShowPRID = pPRU->stSrchDieCmd.auwRecordID[0];
		//	break;

		//// Shape Fitting
		//case PRS_SHAPE_TYPE:
		//	ulShowPRID = pPRU->stDetectShapeCmd.uwRecordID;
		//	break;

		//// Tmpl Fitting
		//case PRS_TMPL_TYPE:
		//	ulShowPRID = pPRU->stSrchTmplCmd.uwRecordID;
		//	break;

		//}

		PR_GET_RECORD_INFO_RPY	stRpy;
		if (PRS_GetRecordInfo(pPRU, &stRpy))
		{
			ulShowValidID		= stRpy.emValidID;
			ulShowOwnerID		= stRpy.ulOwnerID;

			switch (stRpy.emRecordType)
			{
			case PRS_DIE_TYPE:
				ulShowRecordType = 0;
				break;

			case PRS_SHAPE_TYPE:
				ulShowRecordType = 1;
				break;

			case PRS_TMPL_TYPE:
				ulShowRecordType = 2;
				break;
			
			}
		}
		else
		{
			ulShowValidID		= 0;		// no valid!
			ulShowRecordType	= 4;
			ulShowOwnerID		= 0;
		}
	}

	else
	{
		ulShowPRID			= 0;
		ulShowValidID		= 2;
		ulShowRecordType	= 5;
		ulShowOwnerID		= 0;
	}

	// set lighting control
	UpdateLighting(pPRU);
}

VOID CWinEagle::UpdateHMICalibPRButton(BOOL bOperation)
{
	PRU *pPRU;

	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CInPickArm *pCInPickArm	= (CInPickArm*)m_pModule->GetStation("InPickArm");

	pPRU = &SelectCurPRU(HMI_ulCurPRU);

	switch (HMI_ulCurPRU)
	{

	case INSPDL1_WH1_CALIB_PRU:
		HMI_bCalibPRModSelected		= pCWH1->m_bPRSelected;
		HMI_bShowCalibPRModSelected	= TRUE;
		HMI_bCalibModSelected		= pCInPickArm->m_bModSelected && pCWH1->m_bModSelected; 
		HMI_bShowCalibModSelected	= pCWH1->m_bShowModSelected;
		HMI_lCalibPRDelay			= pCWH1->m_lCalibPRDelay;
		HMI_lErrorLimit				= pCWH1->m_lPRErrorLimit;
		HMI_bCORCalibrated			= pCWH1->m_bCORCalibrated[pCWH1->m_lCORCamNum% MAX_NUM_OF_INSPDL_COR];
		HMI_dCalibCORRange			= pCWH1->m_dCalibCORRange;
		HMI_dIncludedAnglePos		= 0.0; //20130530
		HMI_dIncludedAngleNeg		= 0.0; //20130530
		HMI_bShowIncludeAngle		= FALSE;

		HMI_bInspULTableTab			= FALSE;
		HMI_bInspDLTableTab			= TRUE;
		HMI_bVACSnr					= pCWH1->m_stGlass1VacSol.bHMI_Status;

		break;

	case INSPDL2_WH1_CALIB_PRU:
		HMI_bCalibPRModSelected		= pCWH1->m_bPRSelected;
		HMI_bShowCalibPRModSelected	= TRUE;
		HMI_bCalibModSelected		= pCInPickArm->m_bModSelected && pCWH1->m_bModSelected; 
		HMI_bShowCalibModSelected	= pCWH1->m_bShowModSelected;
		HMI_lCalibPRDelay			= pCWH1->m_lCalibPRDelay;
		HMI_lErrorLimit				= pCWH1->m_lPRErrorLimit;
		HMI_bCORCalibrated			= pCWH1->m_bCORCalibrated[pCWH1->m_lCORCamNum% MAX_NUM_OF_INSPDL_COR];
		HMI_dCalibCORRange			= pCWH1->m_dCalibCORRange;
		HMI_dIncludedAnglePos		= 0.0; //20130530
		HMI_dIncludedAngleNeg		= 0.0; //20130530
		HMI_bShowIncludeAngle		= FALSE;

		HMI_bInspULTableTab			= FALSE;
		HMI_bInspDLTableTab			= TRUE;
		HMI_bVACSnr					= pCWH1->m_stGlass1VacSol.bHMI_Status;

		break;

	case INSPDL1_WH2_CALIB_PRU:
		HMI_bCalibPRModSelected		= pCWH2->m_bPRSelected;
		HMI_bShowCalibPRModSelected	= TRUE;
		HMI_bCalibModSelected		= pCInPickArm->m_bModSelected && pCWH2->m_bModSelected; 
		HMI_bShowCalibModSelected	= pCWH2->m_bShowModSelected;
		HMI_lCalibPRDelay			= pCWH2->m_lCalibPRDelay;
		HMI_lErrorLimit				= pCWH2->m_lPRErrorLimit;
		HMI_bCORCalibrated			= pCWH2->m_bCORCalibrated[pCWH2->m_lCORCamNum% MAX_NUM_OF_INSPDL_COR];
		HMI_dCalibCORRange			= pCWH2->m_dCalibCORRange;
		HMI_dIncludedAnglePos		= 0.0; //20130530
		HMI_dIncludedAngleNeg		= 0.0; //20130530
		HMI_bShowIncludeAngle		= FALSE;

		HMI_bInspULTableTab			= FALSE;
		HMI_bInspDLTableTab			= TRUE;
		HMI_bVACSnr					= pCWH2->m_stGlass1VacSol.bHMI_Status;

		break;

	case INSPDL2_WH2_CALIB_PRU:
		HMI_bCalibPRModSelected		= pCWH2->m_bPRSelected;
		HMI_bShowCalibPRModSelected	= TRUE;
		HMI_bCalibModSelected		= pCInPickArm->m_bModSelected && pCWH2->m_bModSelected; 
		HMI_bShowCalibModSelected	= pCWH2->m_bShowModSelected;
		HMI_lCalibPRDelay			= pCWH2->m_lCalibPRDelay;
		HMI_lErrorLimit				= pCWH2->m_lPRErrorLimit;
		HMI_bCORCalibrated			= pCWH2->m_bCORCalibrated[pCWH2->m_lCORCamNum% MAX_NUM_OF_INSPDL_COR];
		HMI_dCalibCORRange			= pCWH2->m_dCalibCORRange;
		HMI_dIncludedAnglePos		= 0.0; //20130530
		HMI_dIncludedAngleNeg		= 0.0; //20130530
		HMI_bShowIncludeAngle		= FALSE;

		HMI_bInspULTableTab			= FALSE;
		HMI_bInspDLTableTab			= TRUE;
		HMI_bVACSnr					= pCWH2->m_stGlass1VacSol.bHMI_Status;

		break;

	case INSPWHUL_WH1_CALIB_PRU:
		HMI_bCalibPRModSelected		= pCInspOpt->m_bPRSelected;
		HMI_bShowCalibPRModSelected	= TRUE;
		HMI_bCalibModSelected		= pCInspOpt->m_bModSelected && pCWH1->m_bModSelected; 
		HMI_bShowCalibModSelected	= pCInspOpt->m_bShowModSelected;
		HMI_lCalibPRDelay			= pCInspOpt->m_lCalibPRDelay;
		HMI_lErrorLimit				= pCInspOpt->m_lPRErrorLimit;
		HMI_bCORCalibrated			= pCInspOpt->m_bCORCalibrated[pCInspOpt->m_lCORCamNum% MAX_NUM_OF_INSPDL_COR];
		HMI_dCalibCORRange			= pCInspOpt->m_dCalibCORRange;
		HMI_dIncludedAnglePos		= 0.0; //20130530
		HMI_dIncludedAngleNeg		= 0.0; //20130530
		HMI_bShowIncludeAngle		= FALSE;

		HMI_bInspDLTableTab			= FALSE;
		HMI_bInspULTableTab			= TRUE;
		HMI_bVACSnr					= pCWH1->m_stGlass1VacSol.bHMI_Status;

		break;

	case INSPWHUL_WH2_CALIB_PRU:
		HMI_bCalibPRModSelected		= pCInspOpt->m_bPRSelected;
		HMI_bShowCalibPRModSelected	= TRUE;
		HMI_bCalibModSelected		= pCInspOpt->m_bModSelected && pCWH2->m_bModSelected; 
		HMI_bShowCalibModSelected	= pCInspOpt->m_bShowModSelected;
		HMI_lCalibPRDelay			= pCInspOpt->m_lCalibPRDelay;
		HMI_lErrorLimit				= pCInspOpt->m_lPRErrorLimit;
		HMI_bCORCalibrated			= pCInspOpt->m_bCORCalibrated[pCInspOpt->m_lCORCamNum% MAX_NUM_OF_INSPDL_COR];
		HMI_dCalibCORRange			= pCInspOpt->m_dCalibCORRange;
		HMI_dIncludedAnglePos		= 0.0; //20130530
		HMI_dIncludedAngleNeg		= 0.0; //20130530
		HMI_bShowIncludeAngle		= FALSE;

		HMI_bInspDLTableTab			= FALSE;
		HMI_bInspULTableTab			= TRUE;
		HMI_bVACSnr					= pCWH2->m_stGlass1VacSol.bHMI_Status;

		break;

	}

	if (HMI_bVACSnr) //20130327
	{
		DisplayMessage("VAC Sol On");
	}
	else
	{
		DisplayMessage("VAC Sol Off");
	}

	SetHmiVariable("PR_bVACSnr"); //20130318

	HMI_bPRUCalibrated 			= pPRU->bCalibrated;
	HMI_bShowAutoCal			= pPRU->bLoaded;
	

	HMI_bShowCORCal				= TRUE;


	CPRTaskStn::UpdateHMIPRButton();

	// set lighting control and Display Video
	UpdateLighting(pPRU);
	PRS_DisplayVideo(pPRU);
	PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

}

LONG CWinEagle::SelectCalibPRU(ULONG ulPRU, BOOL bCheckModSelected)	// 20140915 Yip: Add bCheckModSelected
{
	CWH1 *pCWH1	= (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2	= (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	COutPickArm *pCOutPickArm = (COutPickArm*)m_pModule->GetStation("OutPickArm");

	CString szMsg(" ");
	PRU *pPRU;
	LONG lAnswer = rMSG_TIMEOUT;

	// protection
	if (HMI_ulCurPRU >= MAX_NUM_OF_CALIB_PRU)
	{
		HMI_ulCurPRU = INSPDL1_WH1_CALIB_PRU;
	}

	pPRU = &SelectCurPRU(HMI_ulCurPRU);

	switch (HMI_ulCurPRU)
	{

	case INSPDL1_WH1_CALIB_PRU:
		pCInPickArm->m_lCurInspWH = WH_1;
		pCWH1->m_lCurGlass = GLASS1;
		pCWH1->m_lCORCamNum = INSPDL1_COR;

		if (m_bMoveToPosn)
		{
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
			pCWH1->MoveAbsoluteXY(pCWH1->m_mtrPRCalibPosn[GLASS1].x, pCWH1->m_mtrPRCalibPosn[GLASS1].y, GMP_WAIT);
			pCWH1->MoveTToStandby();
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_WAIT);
		}
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCInPickArm->m_lCurInspWH = WH_1;
		pCWH1->m_lCurGlass = GLASS2;
		pCWH1->m_lCORCamNum = INSPDL2_COR;

		if (m_bMoveToPosn)
		{
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
			pCWH1->MoveAbsoluteXY(pCWH1->m_mtrPRCalibPosn[GLASS2].x, pCWH1->m_mtrPRCalibPosn[GLASS2].y, GMP_WAIT);
			pCWH1->MoveTToStandby();
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_WAIT);
		}
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCInPickArm->m_lCurInspWH = WH_2;
		pCWH2->m_lCurGlass = GLASS1;
		pCWH2->m_lCORCamNum = INSPDL1_COR;

		if (m_bMoveToPosn)
		{
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
			pCWH2->MoveAbsoluteXY(pCWH2->m_mtrPRCalibPosn[GLASS1].x, pCWH2->m_mtrPRCalibPosn[GLASS1].y, GMP_WAIT);
			pCWH2->MoveTToStandby();
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_WAIT);
		}
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCInPickArm->m_lCurInspWH = WH_2;
		pCWH2->m_lCurGlass = GLASS2;
		pCWH2->m_lCORCamNum = INSPDL2_COR;

		if (m_bMoveToPosn)
		{
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
			pCWH2->MoveAbsoluteXY(pCWH2->m_mtrPRCalibPosn[GLASS2].x, pCWH2->m_mtrPRCalibPosn[GLASS2].y, GMP_WAIT);
			pCWH2->MoveTToStandby();
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_WAIT);
		}
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCInspOpt->m_lCurInspWH = WH_1;
		pCInspOpt->m_lCORCamNum = INSPDL1_COR;
		if (m_bMoveToPosn)
		{
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCInspOpt->MoveAbsoluteXY(pCInspOpt->m_mtrPRCalibPosn[WH_1].x, pCInspOpt->m_mtrPRCalibPosn[WH_1].y, GMP_WAIT);
			pCWH1->MoveTToStandby();
			pCInspOpt->MoveZ(INSPOPT_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_WAIT);
		}
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCInspOpt->m_lCurInspWH = WH_2;
		pCInspOpt->m_lCORCamNum = INSPDL2_COR;
		if (m_bMoveToPosn)
		{
			pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
			pCInspOpt->MoveAbsoluteXY(pCInspOpt->m_mtrPRCalibPosn[WH_2].x, pCInspOpt->m_mtrPRCalibPosn[WH_2].y, GMP_WAIT);
			pCWH2->MoveTToStandby();
			pCInspOpt->MoveZ(INSPOPT_Z_CTRL_GO_CALIB_PR_LEVEL, GMP_WAIT);
		}
		break;

	}
	
	PRS_DisplayVideo(pPRU);
	PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

	return TRUE;
}


BOOL CWinEagle::CalcCentreCursorPt(PRU *pPRU, BOOL bDraw, PR_COORD const PreCursor, PR_COORD *pprCentreCursor)
{
	PR_COORD	prTempPt;
	PR_BOOLEAN	TempResultantCoordStatus;

	if (bDraw)
	{
		// draw PRTempAlignPt_a cursor
		PRS_GetPointInZoomImgFrom1xImg(m_lCurrZoomFactor, m_prZoomCentre, PreCursor, &prTempPt, &TempResultantCoordStatus);
		PRS_DrawCross(pPRU->nCamera, prTempPt, g_stCursor.lSize, g_stCursor.lColor);
	}
	
	PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, m_prZoomCentre, g_stCursor.prCoord, &prTempPt);

	PR_COORD prTempCentre;

	prTempCentre.x = (PreCursor.x + prTempPt.x) / 2;
	prTempCentre.y = (PreCursor.y + prTempPt.y) / 2;

	PRS_GetPointInZoomImgFrom1xImg(m_lCurrZoomFactor, m_prZoomCentre, prTempCentre, pprCentreCursor, &TempResultantCoordStatus);


	return TempResultantCoordStatus;
}

VOID CWinEagle::SetPRIgnoreWindow(PRU *pPRU)
{
	PR_COORD	coUpperLeft;
	PR_COORD	coLowerRight;
	CString		szMsg = _T(" ");

	szMsg.Format("PR ignore windows Setup...");
	DisplayMessage(szMsg);

	PRS_CancelMouseWindow(pPRU);

	coUpperLeft.x = 8192 * 2 / 5;
	coUpperLeft.y = 8192 * 2 / 5;
	coLowerRight.x = 8192 * 3 / 5;
	coLowerRight.y = 8192 * 3 / 5;

	PRS_ClearScreen(pPRU->emCameraNo);
	PRS_DisplayText(pPRU->emCameraNo, 1, PRS_MSG_ROW1, "Set Ignore Window");
	//PRS_CancelMouseWindow(pPRU);
	PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
	PRS_DrawHomeCursor(pPRU->emCameraNo, FALSE);
	return;
}

VOID CWinEagle::SetPRIgnoreWindowFinish(PRU *pPRU)
{
	PR_COORD	coUpperLeft;
	PR_COORD	coLowerRight;
	CString		szMsg = _T(" ");
	//stCmd.stAlign.stStreet.stDetectIgnoreWin.uwNIgnoreWin
	if (g_ulWizardIgnoreWindowStep >= MAX_PR_IGNORE_WINDOWS)
	{
		szMsg.Format("Error: Total ignore windows exceed %d. Operation Abort!", MAX_PR_IGNORE_WINDOWS);
		DisplayMessage(szMsg);
		return;
	}
	pPRU->stManualLrnDieCmd.stAlign.stStreet.stDetectIgnoreWin.uwNIgnoreWin = (PR_UWORD) g_ulWizardIgnoreWindowStep;
	szMsg.Format("Number of ignore windows: %ld.", g_ulWizardIgnoreWindowStep);
	HMIMessageBox(MSG_OK, "IGNORE WINDOW", szMsg);
	szMsg.Format("Number of ignore windows is %ld.", g_ulWizardIgnoreWindowStep);
	DisplayMessage(szMsg);

	//restore mouse rectangle selection
	switch (HMI_ulPRAlg)
	{
	case PATTERN_MATCHING:
	case EDGE_MATCHING:
	case RECTANGLE_MATCHING:
		coUpperLeft.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
		coUpperLeft.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
		coLowerRight.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
		coLowerRight.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
		break;

	case SHAPE_FITTING:
		coUpperLeft.x = pPRU->stLrnShapeCmd.stEncRect.coCorner1.x;
		coUpperLeft.y = pPRU->stLrnShapeCmd.stEncRect.coCorner1.y;
		coLowerRight.x = pPRU->stLrnShapeCmd.stEncRect.coCorner2.x;
		coLowerRight.y = pPRU->stLrnShapeCmd.stEncRect.coCorner2.y;
		break;

	case TMPL_FITTING:
		coUpperLeft.x = pPRU->stLoadTmplCmd.coTmplCentre.x - pPRU->stLoadTmplCmd.szTmplSize.x;
		coUpperLeft.y = pPRU->stLoadTmplCmd.coTmplCentre.y - pPRU->stLoadTmplCmd.szTmplSize.y;
		coLowerRight.x = pPRU->stLoadTmplCmd.coTmplCentre.x + pPRU->stLoadTmplCmd.szTmplSize.x;
		coLowerRight.y = pPRU->stLoadTmplCmd.coTmplCentre.y + pPRU->stLoadTmplCmd.szTmplSize.y;
		break;

	case FIDUCIAL_FITTING:
		coUpperLeft.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
		coUpperLeft.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
		coLowerRight.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
		coLowerRight.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
		break;
	}
	PRS_ClearTextRow(pPRU->emCameraNo, PRS_MSG_ROW1); //20150716
	PRS_CancelMouseWindow(pPRU);
	//PRS_ClearScreen(pPRU->emCameraNo);
	PRS_DisplayText(pPRU->emCameraNo, 1, PRS_MSG_ROW1, "Ready to learn pattern");

	PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
	PRS_DrawHomeCursor(pPRU->emCameraNo, FALSE);
	
	return;
}

VOID CWinEagle::SetPRIgnoreWindowNext(PRU *pPRU)
{
	//stCmd.stAlign.stStreet.stDetectIgnoreWin.astIgnoreWin[i]
	//stCmd.stAlign.stStreet.stDetectIgnoreWin.uwNIgnoreWin
	//PR_COORD        coCorner1;      /* Upper-left corner */
	//PR_COORD        coCorner2;      /* Lower-right corner */
/*
typedef enum {	PR_COLOR_TRANSPARENT = 0, PR_COLOR_RED = 1, PR_COLOR_YELLOW = 2,
				PR_COLOR_BLUE = 3, PR_COLOR_GREEN = 4,
				PR_COLOR_CYAN = 5, PR_COLOR_MAGENTA = 6,
				PR_COLOR_GREY = 7, PR_COLOR_BLACK = 8, PR_COLOR_FOREGROUND, 
				PR_COLOR_RESTORE_FOREGROUND,
				PR_COLOR_END = 15} PR_COLOR;
*/
	PR_COORD	coUpperLeft;
	PR_COORD	coLowerRight;
	ULONG		ulIndex = 0;
	CString		szMsg = _T(" ");

	if (g_ulWizardIgnoreWindowStep > MAX_PR_IGNORE_WINDOWS)
	{
		szMsg.Format("Error: Number of Ignore Windows exceed %d. Operation Abort!", MAX_PR_IGNORE_WINDOWS);
		DisplayMessage(szMsg);
		szMsg.Format("Number of Ignore Windows exceed %d. Operation Abort!", MAX_PR_IGNORE_WINDOWS);
		HMIMessageBox(MSG_OK, "WARNING", szMsg);
		return;
	}
	szMsg.Format("Current ignore windows %ld.", g_ulWizardIgnoreWindowStep);
	DisplayMessage(szMsg);

	ulIndex = (g_ulWizardIgnoreWindowStep - 1) % MAX_PR_IGNORE_WINDOWS;
	PRS_GetMouseWindow(pPRU, &coUpperLeft, &coLowerRight);
	PRS_CancelMouseWindow(pPRU);
	
	PRS_DrawRect(pPRU->emCameraNo, coUpperLeft, coLowerRight, PR_COLOR_RED/*(PR_COLOR)(PR_COLOR_RED + ulIndex)*/);

	pPRU->stManualLrnDieCmd.stAlign.stStreet.stDetectIgnoreWin.astIgnoreWin[ulIndex].coCorner1.x = coUpperLeft.x;
	pPRU->stManualLrnDieCmd.stAlign.stStreet.stDetectIgnoreWin.astIgnoreWin[ulIndex].coCorner1.y = coUpperLeft.y;
	pPRU->stManualLrnDieCmd.stAlign.stStreet.stDetectIgnoreWin.astIgnoreWin[ulIndex].coCorner2.x = coLowerRight.x;
	pPRU->stManualLrnDieCmd.stAlign.stStreet.stDetectIgnoreWin.astIgnoreWin[ulIndex].coCorner2.y = coLowerRight.y;

	szMsg.Format("coUpperLeft=(%ld, %ld) coLowerRight=(%ld, %ld).", (LONG)coUpperLeft.x, (LONG)coUpperLeft.y, (LONG)coLowerRight.x, (LONG)coLowerRight.y);
	DisplayMessage(szMsg);

	coUpperLeft.x = 8192 * 2 / 5;
	coUpperLeft.y = 8192 * 2 / 5;
	coLowerRight.x = 8192 * 3 / 5;
	coLowerRight.y = 8192 * 3 / 5;
	//PRS_CancelMouseWindow(pPRU);
	PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
	PRS_DrawHomeCursor(pPRU->emCameraNo, FALSE);
	return;
}

/////////////////////////////////////////////////////////////////
//General PR Setup Related 
/////////////////////////////////////////////////////////////////
VOID CWinEagle::SetupPRSrchArea(PRU *pPRU)
{
	BOOL bResult = TRUE;
	PR_COORD	coUpperLeft;
	PR_COORD	coLowerRight;

	if (g_ulWizardStep > 1 && g_ulWizardStep != 99)
	{
		g_ulWizardStep = 1;
	}

	HMI_bShowPRAlg			= FALSE;
	HMI_bShowFiducial		= FALSE;

	g_bWizardEnableBack = FALSE;
	g_bWizardEnableNext = FALSE;
	g_bWizardEnableExit = TRUE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

	SetHmiVariable("PR_bShowPRAlg");
	SetHmiVariable("PR_bShowFiducial");

	//PRS_DisplayVideo(pPRU);

	// Perform Operations
	switch (g_ulWizardStep)
	{
	case 99:
		PRS_CancelMouseWindow(pPRU);

		g_szWizardStep = "";
		g_szWizardMess = "";
		g_szWizardBMPath = "";

		g_bWizardEnableBack		= FALSE;
		g_bWizardEnableNext		= FALSE;
		g_bWizardEnableExit		= TRUE;

		break;

	case 0:
		PRS_DisplayVideo(pPRU);
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

		switch (pPRU->emRecordType)
		{
		// Pattern Matching or Edge Matching
		case PRS_DIE_TYPE:
			PRS_UseMouseWindow(pPRU, 
							   pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1, 
							   pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2);
			break;

		// Shape Fitting
		case PRS_SHAPE_TYPE:
			PRS_UseMouseWindow(pPRU, 
							   pPRU->stDetectShapeCmd.stInspWin.coCorner1, 
							   pPRU->stDetectShapeCmd.stInspWin.coCorner2);
			break;

		// Tmpl Fitting
		case PRS_TMPL_TYPE:
			PRS_UseMouseWindow(pPRU, 
							   pPRU->stSrchTmplCmd.stSrchWin.coCorner1, 
							   pPRU->stSrchTmplCmd.stSrchWin.coCorner2);
			break;
		}

		g_szWizardStep = "Step 1/1";
		g_szWizardMessPrev = "";
		g_szWizardMess = "1. Define Pattern Search Area with Mouse...";
		g_szWizardMessNext = "";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;

		break;

	case 1:
		PRS_GetMouseWindow(pPRU, &coUpperLeft, &coLowerRight);

		switch (pPRU->emRecordType)
		{
		// Pattern Matching or Edge Matching
		case PRS_DIE_TYPE:
			SetPRDieSrchArea(pPRU, coUpperLeft, coLowerRight);
			break;

		// Shape Fitting
		case PRS_SHAPE_TYPE:
			SetPRShapeSrchArea(pPRU, coUpperLeft, coLowerRight);
			break;

		// Tmpl Fitting
		case PRS_TMPL_TYPE:
			SetPRTmplSrchArea(pPRU, coUpperLeft, coLowerRight);
			break;
		}

		PRS_CancelMouseWindow(pPRU);

		g_szWizardStep = "DONE";
		g_szWizardMess = "DONE";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = FALSE;
		g_bWizardEnableExit = TRUE;

		break;
	}

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

}
#if 1
VOID CWinEagle::SetupPRPattern(PRU *pPRU)
{
	BOOL		bResult = TRUE;
	PR_COORD	coUpperLeft;
	PR_COORD	coLowerRight;

	PR_COORD	acoDieCorners[4];
	PR_SIZE		szTmplSize;
	PR_COORD	coTmplCentre;

	CString		str = "";

	// g_ulWizardStep =98 // skip the follow step
	if (g_ulWizardStep > 6 && g_ulWizardStep != 99 && g_ulWizardStep != 98)
	{
		g_ulWizardStep = 6;
	}

	HMI_bShowPRAlg			= FALSE;
	HMI_bShowFiducial		= FALSE;
	HMI_bShowCursorControl	= FALSE;
	HMI_bShowDigitalZoom	= FALSE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

	SetHmiVariable("PR_bShowPRAlg");
	SetHmiVariable("PR_bShowFiducial");
	SetHmiVariable("PR_bShowCursorControl");
	SetHmiVariable("PR_bShowDigitalZoom");

	// 20140815 Yip: Add Manual Learn Die Ignore Region
	PR_DETECT_IGNORE_WIN *pstDetectIgnoreWin = &pPRU->stManualLrnDieCmd.stAlign.stStreet.stDetectIgnoreWin;

	switch (g_ulWizardStep)
	{
	case 99:
		// Reset Digital Zoom
		m_lCurrZoomFactor = PR_ZOOM_FACTOR_1X;
		//PRS_DisableDigitalZoom(pPRU->nCamera);
		g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
		g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);
	
		PRS_DisplayVideo(pPRU);
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);
		PRS_CancelMouseWindow(pPRU);
		PRS_SetOnMouseClickCmd(OFF);
		PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);

		g_szWizardStep			= "";
		g_szWizardMess			= "";
		g_szWizardBMPath		= "";

		g_bWizardEnableBack		= FALSE;
		g_bWizardEnableNext		= FALSE;
		g_bWizardEnableExit		= TRUE;

		HMI_bShowPRAlg			= FALSE;
		HMI_bShowFiducial		= FALSE;
		HMI_bShowCursorControl	= FALSE;
		HMI_bShowDigitalZoom	= FALSE;

		break;
	
	case 0:
		PRS_DisplayVideo(pPRU);
		//if (PRS_CopyUserPurpose(pPRU->emPurpose[PURPOSE0], pPRU->emPurpose[PURPOSE1]))
		//{
		//	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Ready to learn pattern");
		//	g_szWizardStep = "Step 1/2";
		//	g_szWizardMess.Format("1. Align Pattern with camera centre");
		//	g_szWizardMessNext.Format("2. Learn PR");
		//	g_szWizardBMPath = "";
		//}
		//else
		//{
		//	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Copy user purpose fail!");
		//	bResult = FALSE;
		//	break;
		//}
		
		switch (HMI_ulPRAlg)
		{
		case PATTERN_MATCHING:
		case EDGE_MATCHING:
		case RECTANGLE_MATCHING:
			HMI_bShowFiducial = FALSE;
			coUpperLeft.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			coUpperLeft.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			coLowerRight.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			coLowerRight.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
			break;

		case SHAPE_FITTING:
			HMI_bShowFiducial = FALSE;
			coUpperLeft.x = pPRU->stLrnShapeCmd.stEncRect.coCorner1.x;
			coUpperLeft.y = pPRU->stLrnShapeCmd.stEncRect.coCorner1.y;
			coLowerRight.x = pPRU->stLrnShapeCmd.stEncRect.coCorner2.x;
			coLowerRight.y = pPRU->stLrnShapeCmd.stEncRect.coCorner2.y;
			break;

		case TMPL_FITTING:
			HMI_bShowFiducial = FALSE;
			coUpperLeft.x = pPRU->stLoadTmplCmd.coTmplCentre.x - pPRU->stLoadTmplCmd.szTmplSize.x;
			coUpperLeft.y = pPRU->stLoadTmplCmd.coTmplCentre.y - pPRU->stLoadTmplCmd.szTmplSize.y;
			coLowerRight.x = pPRU->stLoadTmplCmd.coTmplCentre.x + pPRU->stLoadTmplCmd.szTmplSize.x;
			coLowerRight.y = pPRU->stLoadTmplCmd.coTmplCentre.y + pPRU->stLoadTmplCmd.szTmplSize.y;
			break;

		case FIDUCIAL_FITTING:
			HMI_bShowFiducial = TRUE;
			HMI_ulPRFMarkType = pPRU->stManualLrnDieCmd.emFiducialMarkType;
			coUpperLeft.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			coUpperLeft.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			coLowerRight.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			coLowerRight.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
			switch (HMI_ulPRFMarkType)
			{
			case 0:
				g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType1.png";
				SetHmiVariable("SF_szWizardBMPath");

				break;
			case 1:
				g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType2.png";
				SetHmiVariable("SF_szWizardBMPath");

				break;
			case 2:
				g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType3.png";
				SetHmiVariable("SF_szWizardBMPath");

				break;
			}
			break;
		}

		// 20140815 Yip: Add Manual Learn Die Ignore Region
		pstDetectIgnoreWin->uwNIgnoreWin = 0;

		HMI_bShowPRAlg	= TRUE;
		SetHmiVariable("PR_bShowPRAlg");
		SetHmiVariable("PR_bShowFiducial");

		PRS_CancelMouseWindow(pPRU);
		PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);
		
		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;
		break;

	case 1:
		PRS_GetMouseWindow(pPRU, &coUpperLeft, &coLowerRight);
		PRS_CancelMouseWindow(pPRU);

		acoDieCorners[PR_UPPER_RIGHT].x	= coLowerRight.x;
		acoDieCorners[PR_UPPER_RIGHT].y	= coUpperLeft.y;
		acoDieCorners[PR_UPPER_LEFT].x	= coUpperLeft.x;
		acoDieCorners[PR_UPPER_LEFT].y	= coUpperLeft.y;
		acoDieCorners[PR_LOWER_LEFT].x	= coUpperLeft.x;
		acoDieCorners[PR_LOWER_LEFT].y	= coLowerRight.y;
		acoDieCorners[PR_LOWER_RIGHT].x	= coLowerRight.x;
		acoDieCorners[PR_LOWER_RIGHT].y	= coLowerRight.y;

		szTmplSize.x = (coLowerRight.x - coUpperLeft.x) / 2;
		szTmplSize.y = (coLowerRight.y - coUpperLeft.y) / 2;
		coTmplCentre.x = (coLowerRight.x + coUpperLeft.x) / 2;
		coTmplCentre.y = (coLowerRight.y + coUpperLeft.y) / 2;

		if (HMI_ulPRAlg == RECTANGLE_MATCHING)
		{
			PRS_SetOnMouseClickCmd(ON);
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Upper Left Corner");
			
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);

			g_szWizardStep = "Step 1/2";
			g_szWizardMess.Format("1. Locate Upper Left Corner");
			g_szWizardMessNext.Format("2. Locate Upper Right Corner");
			g_szWizardBMPath = "";

			HMI_bShowCursorControl			= TRUE;
			SetHmiVariable("PR_bShowCursorControl");


			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
			break;
		}
		//else if (HMI_ulPRAlg == FIDUCIAL_FITTING)
		//{
		//	pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_TRUE;
		//	pPRU->stManualLrnDieCmd.emFiducialMarkType = (PR_FIDUCIAL_MARK_TYPE)HMI_ulPRFMarkType;

		//	for (INT i = PR_UPPER_RIGHT; i <= PR_LOWER_RIGHT; i++)
		//	{
		//		pPRU->stManualLrnDieCmd.acoDieCorners[i] = acoDieCorners[i];
		//	}

		//	pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;

		//	PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
		//	PRS_SetOnMouseClickCmd(ON);

		//	g_szWizardStep = "Step 2/3";
		//	g_szWizardMessPrev = "1. Align Glass Pattern centre with camera centre";
		//	g_szWizardMess = "2. Locate Corner1";
		//	g_szWizardMessNext = "3. Locate Corner2";	
		//	g_szWizardBMPath = "";
		//	
		//	PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Please Set Pt 1");

		//	HMI_bShowCursorControl			= TRUE;
		//	HMI_bShowDigitalZoom			= TRUE;

		//	SetHmiVariable("PR_bShowCursorControl");
		//	SetHmiVariable("PR_bShowDigitalZoom");

		//	g_bWizardEnableBack = FALSE;
		//	g_bWizardEnableNext = TRUE;
		//	g_bWizardEnableExit = TRUE;
		//}
		else  // Other
		{
			LONG lResponse;

			switch (HMI_ulPRAlg)
			{
			case PATTERN_MATCHING:
			case FIDUCIAL_FITTING:
				pPRU->emRecordType = PRS_DIE_TYPE;
				pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_INTR;
				break;

			case EDGE_MATCHING:
				lResponse = HMISelectionBox("LEARN PR", "Please Select EDGE:matching algorithm:", "4-Edge", "Left-Edge", "Right-Edge");

				if (lResponse == -1 || lResponse == 11)
				{
					bResult = FALSE;
				}
				if (lResponse == 0)
				{
					pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_EDGE;
					pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_EDGE;
				}
				else if (lResponse == 1)
				{
					pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_L_EDGE;
					pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_L_EDGE;
				}
				else if (lResponse == 2)
				{
					pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_R_EDGE;
					pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_R_EDGE;
				}

				pPRU->emRecordType = PRS_DIE_TYPE;
				pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_FALSE;
				pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
				pPRU->stManualLrnDieCmd.emObjectType = PR_OBJ_TYPE_IC_DIE;
				break;

			case SHAPE_FITTING:
				//lResponse = HMISelectionBox("LEARN PR", "Please select circle attribute:", "Bright", "Dark");

				//if (lResponse == -1 || lResponse == 11)
				//{
				//	bResult = FALSE;
				//}
				//if (lResponse == 0)
				//{
				//	pPRU->stLrnShapeCmd.emObjAttribute = PR_OBJ_ATTRIBUTE_BRIGHT;
				//}
				//else if (lResponse == 1)
				//{
				//	pPRU->stLrnShapeCmd.emObjAttribute = PR_OBJ_ATTRIBUTE_DARK;
				//}

				pPRU->stLrnShapeCmd.emShapeType				= PR_SHAPE_TYPE_CIRCLE;
				pPRU->stLrnShapeCmd.emDetectAlg				= PR_SHAPE_DETECT_ALG_EDGE_PTS;

				pPRU->stLrnShapeCmd.stEncRect.coCorner1.x = coUpperLeft.x;
				pPRU->stLrnShapeCmd.stEncRect.coCorner1.y = coUpperLeft.y;
				pPRU->stLrnShapeCmd.stEncRect.coCorner2.x = coLowerRight.x;
				pPRU->stLrnShapeCmd.stEncRect.coCorner2.y = coLowerRight.y;
				
				pPRU->emRecordType = PRS_SHAPE_TYPE;
				pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
				break;

			case TMPL_FITTING:
				pPRU->emRecordType = PRS_TMPL_TYPE;
				pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
				break;
			
			}
			switch (pPRU->emRecordType)
			{
			// Fiducial Fitting or Pattern Matching or Edge Matching
			case PRS_DIE_TYPE:
				if (HMI_ulPRAlg == EDGE_MATCHING)
				{
					for (INT i = PR_UPPER_RIGHT; i <= PR_LOWER_RIGHT; i++)
					{
						pPRU->stManualLrnDieCmd.acoDieCorners[i] = acoDieCorners[i];
					}
					bResult = ManualLrnDie(pPRU);
				}
				else
				{
					PRS_FreeRecordID(pPRU); //20180208
					bResult = ManualLrnDie_New(pPRU);	// Vision UI
				}
				break;

			// Shape Fitting
			case PRS_SHAPE_TYPE:
				//bResult = LearnPRShape(pPRU);
				PRS_FreeRecordID(pPRU); //20180208
				bResult = ManualLrnDie_New(pPRU);	// Vision UI
				break;

			// Tmpl Fitting
			case PRS_TMPL_TYPE:
				pPRU->stLoadTmplCmd.szTmplSize = szTmplSize;
				pPRU->stLoadTmplCmd.coTmplCentre = coTmplCentre;
				bResult = LoadPRTmpl(pPRU);
				break;
			}

			if (bResult)
			{
				// Sucess help to set Pre-Reuqired for search
				pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore	= (PR_REAL)85.0;
				pPRU->stSrchDieCmd.rCfmPatternMatchScore		= (PR_REAL)85.0; // Comfirmation Pattern 	
				pPRU->stDetectShapeCmd.rDetectScore				= (PR_REAL)85.0;
				pPRU->stSrchTmplCmd.ubPassScore					= (PR_UBYTE)85.0;
				
				pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle	= -(PR_REAL)3.0;
				pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle		= (PR_REAL)3.0;

				pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale	= (PR_UBYTE)105.0;
				pPRU->stSrchDieCmd.stDieAlignPar.ubMinScale	= (PR_UBYTE)95.0;
				pPRU->stDetectShapeCmd.rSizePercentVar		= (PR_REAL)5.0;

				g_szWizardStep = "2/2";
				g_szWizardMessPrev = "2. Learn PR";
				g_szWizardMess = "DONE";
				g_szWizardMessNext = "";
				g_szWizardBMPath = "";

				g_bWizardEnableBack = FALSE;
				g_bWizardEnableNext = FALSE;
				g_bWizardEnableExit = TRUE;
			}
		}
		break;

	case 2: //Get point 1
		if (HMI_ulPRAlg == RECTANGLE_MATCHING)
		{
			if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
			{
				m_PRZoomedPRPt_a.x = g_stCursor.prCoord.x;
				m_PRZoomedPRPt_a.y = g_stCursor.prCoord.y;
			}
			else
			{
				PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &m_PRZoomedPRPt_a);
			}
			
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x	= m_PRZoomedPRPt_a.x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y	= m_PRZoomedPRPt_a.y;

			PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, PR_COLOR_RED);
					
			g_szWizardStep = "2/2";
			g_szWizardMessPrev = "1. Locate Upper Left Corner";
			g_szWizardMess = "2. Locate Lower Right Corner";
			g_szWizardMessNext = "DONE";
			g_szWizardBMPath = "";
			
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Lower Right Corner");

			HMI_bShowCursorControl			= TRUE;
			SetHmiVariable("PR_bShowCursorControl");

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
		}
		else
		{
			g_szWizardStep = "DONE";
			g_szWizardMess = "DONE";
			g_szWizardBMPath = "";

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = FALSE;
			g_bWizardEnableExit = TRUE;
		}
		break;

	case 3:
		if (HMI_ulPRAlg == RECTANGLE_MATCHING)
		{
			if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
			{
				m_PRZoomedPRPt_a.x = g_stCursor.prCoord.x;
				m_PRZoomedPRPt_a.y = g_stCursor.prCoord.y;
			}
			else
			{
				PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &m_PRZoomedPRPt_a);
			}
			
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x	= m_PRZoomedPRPt_a.x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y	= m_PRZoomedPRPt_a.y;

			PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, PR_COLOR_RED);
		

			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_RIGHT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_RIGHT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_LEFT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_LEFT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;

			pPRU->emRecordType = PRS_DIE_TYPE;
			pPRU->stManualLrnDieCmd.emAlignAlg = PR_EDGE_POINTS_MATCHING_ON_DIE_EDGES_WO_PATTERN_GUIDED;
			pPRU->stSrchDieCmd.emAlignAlg = PR_EDGE_POINTS_MATCHING_ON_DIE_EDGES_WO_PATTERN_GUIDED;
			pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_FALSE;
			pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
			pPRU->stManualLrnDieCmd.emObjectType = PR_OBJ_TYPE_IC_DIE;

			g_szWizardStep = "3/4";
			g_szWizardMessPrev = "2. Locate Lower Right Corner";
			g_szWizardMess = "DONE";
			g_szWizardMessNext = "DONE";
			g_szWizardBMPath = "";
			
			bResult = ManualLrnDie(pPRU);
		}

		if (bResult)
		{
			PRS_SetOnMouseClickCmd(OFF);

			// Sucess help to set Pre-Reuqired for search
			pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore	= (PR_REAL)85.0;
			pPRU->stSrchDieCmd.rCfmPatternMatchScore		= (PR_REAL)85.0; // Comfirmation Pattern 	
			pPRU->stDetectShapeCmd.rDetectScore				= (PR_REAL)85.0;
			pPRU->stSrchTmplCmd.ubPassScore					= (PR_UBYTE)85.0;
			
			pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle	= -(PR_REAL)3.0;
			pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle		= (PR_REAL)3.0;
			pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale	= (PR_UBYTE)105.0;
			pPRU->stSrchDieCmd.stDieAlignPar.ubMinScale	= (PR_UBYTE)95.0;
			pPRU->stDetectShapeCmd.rSizePercentVar		= (PR_REAL)5.0;

			// 20140815 Yip: Add Manual Learn Die Ignore Region
			if (rMSG_YES != HMIMessageBox(MSG_YES_NO, "Learn PR", "Do all lines indicate edges?\nYes - Done\nNo - Add Ignore Region"))
			{
				coUpperLeft = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT];
				coLowerRight = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT];
				for (int i = 0; i < PR_MAX_DETECT_IGNORE_WIN; ++i)
				{
					pstDetectIgnoreWin->astIgnoreWin[i].coCorner1 = coUpperLeft;
					pstDetectIgnoreWin->astIgnoreWin[i].coCorner2 = coLowerRight;
				}
				pstDetectIgnoreWin->uwNIgnoreWin = 0;

				PRS_ClearScreen(pPRU->nCamera);
				PRS_CancelMouseWindow(pPRU);
				PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
				PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

				g_szWizardStep = "4/4";
				g_szWizardMessPrev = "3. Locate Corner2";
				g_szWizardMess.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
				g_szWizardMessNext = "4.2. Adjust Upper Left Corner";
				g_szWizardBMPath = "";

				g_bWizardEnableBack = FALSE;
				g_bWizardEnableNext = TRUE;
				g_bWizardEnableExit = TRUE;
			}
			else
			{
				g_szWizardStep = "4/4";
				g_szWizardMessPrev = "3. Locate Corner2";
				g_szWizardMess = "DONE";
				g_szWizardMessNext = "";
				g_szWizardBMPath = "";

				g_bWizardEnableBack = FALSE;
				g_bWizardEnableNext = FALSE;
				g_bWizardEnableExit = TRUE;
			}
		}
		break;

	case 4:	// 20140815 Yip: Add Manual Learn Die Ignore Region
		PRS_GetMouseWindow(pPRU, &coUpperLeft, &coLowerRight);
		PRS_CancelMouseWindow(pPRU);

		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1 = coUpperLeft;
		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2 = coLowerRight;

		m_lCurrZoomFactor = PR_ZOOM_FACTOR_2X;

		ZoomCentre = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1;
		g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
		g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
		PRS_DigitalZoom(nCurrentCameraID, m_lCurrZoomFactor, ZoomCentre);
		PRS_DrawCross(nCurrentCameraID, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Adjust Upper Left Corner");

		HMI_bShowCursorControl = TRUE;
		HMI_bShowDigitalZoom = TRUE;

		SetHmiVariable("PR_bShowCursorControl");
		SetHmiVariable("PR_bShowDigitalZoom");

		PRS_SetOnMouseClickCmd(ON);

		g_szWizardStep = "4/4";
		g_szWizardMessPrev.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
		g_szWizardMess = "4.2. Adjust Upper Left Corner";
		g_szWizardMessNext = "4.3. Adjust Lower Right Corner";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;

		break;

	case 5:	// 20140815 Yip: Add Manual Learn Die Ignore Region
		if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
		{
			coUpperLeft = g_stCursor.prCoord;
		}
		else
		{
			PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &coUpperLeft);
		}
		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1 = coUpperLeft;

		PRS_DisableDigitalZoom(pPRU->nCamera);

		ZoomCentre = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2;
		g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
		g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
		PRS_DigitalZoom(nCurrentCameraID, m_lCurrZoomFactor, ZoomCentre);
		PRS_DrawCross(nCurrentCameraID, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Adjust Lower Right Corner");

		HMI_bShowCursorControl = TRUE;
		HMI_bShowDigitalZoom = TRUE;

		SetHmiVariable("PR_bShowCursorControl");
		SetHmiVariable("PR_bShowDigitalZoom");

		PRS_SetOnMouseClickCmd(ON);

		g_szWizardStep = "4/4";
		g_szWizardMessPrev = "4.2. Adjust Upper Left Corner";
		g_szWizardMess = "4.3. Adjust Lower Right Corner";
		g_szWizardMessNext = "DONE";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;
		break;

	case 6:	// 20140815 Yip: Add Manual Learn Die Ignore Region
		if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
		{
			coLowerRight = g_stCursor.prCoord;
		}
		else
		{
			PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &coLowerRight);
			m_lCurrZoomFactor = PR_ZOOM_FACTOR_1X;
			PRS_DisableDigitalZoom(pPRU->nCamera);
		}
		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2 = coLowerRight;

		PRS_SetOnMouseClickCmd(OFF);
		PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);

		HMI_bShowCursorControl	= FALSE;
		HMI_bShowDigitalZoom	= FALSE;

		SetHmiVariable("PR_bShowCursorControl");
		SetHmiVariable("PR_bShowDigitalZoom");

		++pstDetectIgnoreWin->uwNIgnoreWin;

		for (int i = 0; i < pstDetectIgnoreWin->uwNIgnoreWin; ++i)
		{
			PRS_DrawRect(pPRU->nCamera, pstDetectIgnoreWin->astIgnoreWin[i].coCorner1, pstDetectIgnoreWin->astIgnoreWin[i].coCorner2, (PR_COLOR)(i + 1));
		}

		if (
			(pstDetectIgnoreWin->uwNIgnoreWin < PR_MAX_DETECT_IGNORE_WIN) &&
			(rMSG_YES == HMIMessageBox(MSG_YES_NO, "Learn PR", "Add another ignore region?"))
		   )
		{
			coUpperLeft = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1;
			coLowerRight = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2;

			PRS_CancelMouseWindow(pPRU);
			PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
			PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

			g_szWizardStep = "4/4";
			g_szWizardMessPrev = "3. Locate Corner2";
			g_szWizardMess.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
			g_szWizardMessNext = "4.2. Adjust Upper Left Corner";
			g_szWizardBMPath = "";

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
		}
		else
		{
			bResult = ManualLrnDie(pPRU);

			if (bResult)
			{
				if (rMSG_YES != HMIMessageBox(MSG_YES_NO, "Learn PR", "Do all lines indicate edges?\nYes - Done\nNo - Modify Ignore Region"))
				{
					pstDetectIgnoreWin->uwNIgnoreWin = 0;
					coUpperLeft = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1;
					coLowerRight = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2;

					PRS_ClearScreen(pPRU->nCamera);
					PRS_CancelMouseWindow(pPRU);
					PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
					PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

					g_szWizardStep = "4/4";
					g_szWizardMessPrev = "3. Locate Corner2";
					g_szWizardMess.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
					g_szWizardMessNext = "4.2. Adjust Upper Left Corner";
					g_szWizardBMPath = "";

					g_bWizardEnableBack = FALSE;
					g_bWizardEnableNext = TRUE;
					g_bWizardEnableExit = TRUE;
				}
				else
				{
					g_szWizardStep = "4/4";
					g_szWizardMessPrev = "3. Locate Corner2";
					g_szWizardMess = "DONE";
					g_szWizardMessNext = "";
					g_szWizardBMPath = "";

					g_bWizardEnableBack = FALSE;
					g_bWizardEnableNext = FALSE;
					g_bWizardEnableExit = TRUE;
				}
			}
		}
		g_ulWizardStep = 3;
		break;
	}

	if (!bResult)
	{
		PRS_CancelMouseWindow(pPRU);
		PRS_SetOnMouseClickCmd(OFF);
		PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);

		g_szWizardStep = "FAIL";
		g_szWizardMess = "OPERATION FAIL!";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = FALSE;
		g_bWizardEnableExit = TRUE;
	}

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");
}

#else
VOID CWinEagle::SetupPRPattern(PRU *pPRU)
{
	BOOL		bResult = TRUE;
	PR_COORD	coUpperLeft;
	PR_COORD	coLowerRight;

	PR_COORD	acoDieCorners[4];
	PR_SIZE		szTmplSize;
	PR_COORD	coTmplCentre;

	CString		str = "";

	// g_ulWizardStep =98 // skip the follow step
	if (g_ulWizardStep > 6 && g_ulWizardStep != 99 && g_ulWizardStep != 98)
	{
		g_ulWizardStep = 6;
	}

	HMI_bShowPRAlg			= FALSE;
	HMI_bShowFiducial		= FALSE;
	HMI_bShowCursorControl	= FALSE;
	HMI_bShowDigitalZoom	= FALSE;

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");

	SetHmiVariable("PR_bShowPRAlg");
	SetHmiVariable("PR_bShowFiducial");
	SetHmiVariable("PR_bShowCursorControl");
	SetHmiVariable("PR_bShowDigitalZoom");

	// 20140815 Yip: Add Manual Learn Die Ignore Region
	PR_DETECT_IGNORE_WIN *pstDetectIgnoreWin = &pPRU->stManualLrnDieCmd.stAlign.stStreet.stDetectIgnoreWin;

	switch (g_ulWizardStep)
	{
	case 99:
		// Reset Digital Zoom
		m_lCurrZoomFactor = PR_ZOOM_FACTOR_1X;
		PRS_DisableDigitalZoom(pPRU->nCamera);
		g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
		g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);
	
		PRS_DisplayVideo(pPRU);
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);
		PRS_CancelMouseWindow(pPRU);
		PRS_SetOnMouseClickCmd(OFF);
		PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);

		g_szWizardStep			= "";
		g_szWizardMess			= "";
		g_szWizardBMPath		= "";

		g_bWizardEnableBack		= FALSE;
		g_bWizardEnableNext		= FALSE;
		g_bWizardEnableExit		= TRUE;

		HMI_bShowPRAlg			= FALSE;
		HMI_bShowFiducial		= FALSE;
		HMI_bShowCursorControl	= FALSE;
		HMI_bShowDigitalZoom	= FALSE;

		break;
	
	case 0:
		PRS_DisplayVideo(pPRU);

		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Ready to learn pattern");
		g_szWizardStep = "Step 1/2";
		g_szWizardMess.Format("1. Align Pattern with camera centre");
		g_szWizardMessNext.Format("2. Learn PR");
		g_szWizardBMPath = "";
		
		switch (HMI_ulPRAlg)
		{
		case PATTERN_MATCHING:
		case EDGE_MATCHING:
		case RECTANGLE_MATCHING:
			HMI_bShowFiducial = FALSE;
			coUpperLeft.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			coUpperLeft.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			coLowerRight.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			coLowerRight.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
			break;

		case SHAPE_FITTING:
			HMI_bShowFiducial = FALSE;
			coUpperLeft.x = pPRU->stLrnShapeCmd.stEncRect.coCorner1.x;
			coUpperLeft.y = pPRU->stLrnShapeCmd.stEncRect.coCorner1.y;
			coLowerRight.x = pPRU->stLrnShapeCmd.stEncRect.coCorner2.x;
			coLowerRight.y = pPRU->stLrnShapeCmd.stEncRect.coCorner2.y;
			break;

		case TMPL_FITTING:
			HMI_bShowFiducial = FALSE;
			coUpperLeft.x = pPRU->stLoadTmplCmd.coTmplCentre.x - pPRU->stLoadTmplCmd.szTmplSize.x;
			coUpperLeft.y = pPRU->stLoadTmplCmd.coTmplCentre.y - pPRU->stLoadTmplCmd.szTmplSize.y;
			coLowerRight.x = pPRU->stLoadTmplCmd.coTmplCentre.x + pPRU->stLoadTmplCmd.szTmplSize.x;
			coLowerRight.y = pPRU->stLoadTmplCmd.coTmplCentre.y + pPRU->stLoadTmplCmd.szTmplSize.y;
			break;

		case FIDUCIAL_FITTING:
			HMI_bShowFiducial = TRUE;
			HMI_ulPRFMarkType = pPRU->stManualLrnDieCmd.emFiducialMarkType;
			coUpperLeft.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			coUpperLeft.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			coLowerRight.x = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			coLowerRight.y = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
			switch (HMI_ulPRFMarkType)
			{
			case 0:
				g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType1.png";
				SetHmiVariable("SF_szWizardBMPath");

				break;
			case 1:
				g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType2.png";
				SetHmiVariable("SF_szWizardBMPath");

				break;
			case 2:
				g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType3.png";
				SetHmiVariable("SF_szWizardBMPath");

				break;
			}
			break;
		}

		// 20140815 Yip: Add Manual Learn Die Ignore Region
		pstDetectIgnoreWin->uwNIgnoreWin = 0;

		HMI_bShowPRAlg	= TRUE;
		SetHmiVariable("PR_bShowPRAlg");
		SetHmiVariable("PR_bShowFiducial");

		PRS_CancelMouseWindow(pPRU);
		PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
		PRS_DrawHomeCursor(pPRU->nCamera, FALSE);
		
		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;
		break;

	case 1:
		PRS_GetMouseWindow(pPRU, &coUpperLeft, &coLowerRight);
		PRS_CancelMouseWindow(pPRU);

		acoDieCorners[PR_UPPER_RIGHT].x	= coLowerRight.x;
		acoDieCorners[PR_UPPER_RIGHT].y	= coUpperLeft.y;
		acoDieCorners[PR_UPPER_LEFT].x	= coUpperLeft.x;
		acoDieCorners[PR_UPPER_LEFT].y	= coUpperLeft.y;
		acoDieCorners[PR_LOWER_LEFT].x	= coUpperLeft.x;
		acoDieCorners[PR_LOWER_LEFT].y	= coLowerRight.y;
		acoDieCorners[PR_LOWER_RIGHT].x	= coLowerRight.x;
		acoDieCorners[PR_LOWER_RIGHT].y	= coLowerRight.y;

		szTmplSize.x = (coLowerRight.x - coUpperLeft.x) / 2;
		szTmplSize.y = (coLowerRight.y - coUpperLeft.y) / 2;
		coTmplCentre.x = (coLowerRight.x + coUpperLeft.x) / 2;
		coTmplCentre.y = (coLowerRight.y + coUpperLeft.y) / 2;

		if (HMI_ulPRAlg == RECTANGLE_MATCHING)
		{
			PRS_SetOnMouseClickCmd(ON);
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Upper Left Corner");
			
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);

			g_szWizardStep = "Step 1/2";
			g_szWizardMess.Format("1. Locate Upper Left Corner");
			g_szWizardMessNext.Format("2. Locate Upper Right Corner");
			g_szWizardBMPath = "";

			HMI_bShowCursorControl			= TRUE;
			SetHmiVariable("PR_bShowCursorControl");


			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
			break;
		}
		else if (HMI_ulPRAlg == FIDUCIAL_FITTING)
		{
			pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_TRUE;
			pPRU->stManualLrnDieCmd.emFiducialMarkType = (PR_FIDUCIAL_MARK_TYPE)HMI_ulPRFMarkType;

			for (INT i = PR_UPPER_RIGHT; i <= PR_LOWER_RIGHT; i++)
			{
				pPRU->stManualLrnDieCmd.acoDieCorners[i] = acoDieCorners[i];
			}

			pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;

			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
			PRS_SetOnMouseClickCmd(ON);

			g_szWizardStep = "Step 2/3";
			g_szWizardMessPrev = "1. Align Glass Pattern centre with camera centre";
			g_szWizardMess = "2. Locate Corner1";
			g_szWizardMessNext = "3. Locate Corner2";	
			g_szWizardBMPath = "";
			
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Please Set Pt 1");

			HMI_bShowCursorControl			= TRUE;
			HMI_bShowDigitalZoom			= TRUE;

			SetHmiVariable("PR_bShowCursorControl");
			SetHmiVariable("PR_bShowDigitalZoom");

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
		}
		else  // Other
		{
			LONG lResponse;

			switch (HMI_ulPRAlg)
			{
			case PATTERN_MATCHING:
				pPRU->emRecordType = PRS_DIE_TYPE;
				pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_INTR;
				pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_INTR;
				pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_FALSE;
				pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
				pPRU->stManualLrnDieCmd.emObjectType = PR_OBJ_TYPE_IC_DIE;
				break;

			case EDGE_MATCHING:
				lResponse = HMISelectionBox("LEARN PR", "Please Select EDGE:matching algorithm:", "4-Edge", "Left-Edge", "Right-Edge");

				if (lResponse == -1 || lResponse == 11)
				{
					bResult = FALSE;
				}
				if (lResponse == 0)
				{
					pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_EDGE;
					pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_EDGE;
				}
				else if (lResponse == 1)
				{
					pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_L_EDGE;
					pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_L_EDGE;
				}
				else if (lResponse == 2)
				{
					pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_R_EDGE;
					pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_R_EDGE;
				}

				pPRU->emRecordType = PRS_DIE_TYPE;
				pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_FALSE;
				pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
				pPRU->stManualLrnDieCmd.emObjectType = PR_OBJ_TYPE_IC_DIE;
				break;

			case SHAPE_FITTING:
				lResponse = HMISelectionBox("LEARN PR", "Please select circle attribute:", "Bright", "Dark");

				if (lResponse == -1 || lResponse == 11)
				{
					bResult = FALSE;
				}
				if (lResponse == 0)
				{
					pPRU->stLrnShapeCmd.emObjAttribute = PR_OBJ_ATTRIBUTE_BRIGHT;
				}
				else if (lResponse == 1)
				{
					pPRU->stLrnShapeCmd.emObjAttribute = PR_OBJ_ATTRIBUTE_DARK;
				}

				//lResponse = HMISelectionBox("LEARN PR", "Please select method:", "Circle", "Rectangle");
				//
				//if (lResponse == -1 || lResponse == 11)
				//{
				//	bResult = FALSE;
				//}
				//if (lResponse == 0)
				//{
				pPRU->stLrnShapeCmd.emShapeType				= PR_SHAPE_TYPE_CIRCLE;
				pPRU->stLrnShapeCmd.emDetectAlg				= PR_SHAPE_DETECT_ALG_EDGE_PTS;

				pPRU->stLrnShapeCmd.stEncRect.coCorner1.x = coUpperLeft.x;
				pPRU->stLrnShapeCmd.stEncRect.coCorner1.y = coUpperLeft.y;
				pPRU->stLrnShapeCmd.stEncRect.coCorner2.x = coLowerRight.x;
				pPRU->stLrnShapeCmd.stEncRect.coCorner2.y = coLowerRight.y;
				//}
				//else if (lResponse == 1)
				//{
				//	pPRU->stLrnShapeCmd.emShapeType				= PR_SHAPE_TYPE_HORIZONTAL_RECTANGLE;
				//	pPRU->stLrnShapeCmd.emDetectAlg				= PR_SHAPE_DETECT_ALG_SHA_SYNTHETIC_LRN;

				//	pPRU->stLrnShapeCmd.emRectMode				= PR_RECT_MODE_ENCLOSE;
				//	pPRU->stLrnShapeCmd.uwNumOfLine				= 4;

				//	pPRU->stLrnShapeCmd.astLineCoord[0].coPoint1		= acoDieCorners[PR_UPPER_RIGHT];
				//	pPRU->stLrnShapeCmd.astLineCoord[0].coPoint2		= acoDieCorners[PR_UPPER_LEFT];

				//	pPRU->stLrnShapeCmd.astLineCoord[1].coPoint1		= acoDieCorners[PR_UPPER_LEFT];
				//	pPRU->stLrnShapeCmd.astLineCoord[1].coPoint2		= acoDieCorners[PR_LOWER_RIGHT];
				//
				//	pPRU->stLrnShapeCmd.astLineCoord[2].coPoint1		= acoDieCorners[PR_LOWER_RIGHT];
				//	pPRU->stLrnShapeCmd.astLineCoord[2].coPoint2		= acoDieCorners[PR_LOWER_LEFT];
				//	
				//	pPRU->stLrnShapeCmd.astLineCoord[3].coPoint1		= acoDieCorners[PR_LOWER_LEFT];
				//	pPRU->stLrnShapeCmd.astLineCoord[3].coPoint2		= acoDieCorners[PR_UPPER_RIGHT];
				//	
				//}	
				
				pPRU->emRecordType = PRS_SHAPE_TYPE;
				pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
				break;

			case TMPL_FITTING:
				pPRU->emRecordType = PRS_TMPL_TYPE;
				pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
				break;
			
			//case FIDUCIAL_FITTING: //klocwork fix 20121211
			//	pPRU->emRecordType = PRS_DIE_TYPE;
			//	pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_INTR;
			//	pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_INTR;
			//	pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_TRUE;
			//	pPRU->stManualLrnDieCmd.emFiducialMarkType = (PR_FIDUCIAL_MARK_TYPE)HMI_ulPRFMarkType;
			//	pPRU->stManualLrnDieCmd.emObjectType = PR_OBJ_TYPE_FIDUCIAL_MARK;
			//	break;
			}

			switch (pPRU->emRecordType)
			{
			// Fiducial Fitting or Pattern Matching or Edge Matching
			case PRS_DIE_TYPE:
				for (INT i = PR_UPPER_RIGHT; i <= PR_LOWER_RIGHT; i++)
				{
					pPRU->stManualLrnDieCmd.acoDieCorners[i] = acoDieCorners[i];
				}
				bResult = ManualLrnDie(pPRU);
				break;

			// Shape Fitting
			case PRS_SHAPE_TYPE:
				bResult = LearnPRShape(pPRU);
				break;

			// Tmpl Fitting
			case PRS_TMPL_TYPE:
				pPRU->stLoadTmplCmd.szTmplSize = szTmplSize;
				pPRU->stLoadTmplCmd.coTmplCentre = coTmplCentre;
				bResult = LoadPRTmpl(pPRU);
				break;
			}

			if (bResult)
			{
				// Sucess help to set Pre-Reuqired for search
				pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore	= (PR_REAL)85.0;
				pPRU->stSrchDieCmd.rCfmPatternMatchScore		= (PR_REAL)85.0; // Comfirmation Pattern 	
				pPRU->stDetectShapeCmd.rDetectScore				= (PR_REAL)85.0;
				pPRU->stSrchTmplCmd.ubPassScore					= (PR_UBYTE)85.0;
				
				pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle	= -(PR_REAL)3.0;
				pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle		= (PR_REAL)3.0;

				pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale	= (PR_UBYTE)105.0;
				pPRU->stSrchDieCmd.stDieAlignPar.ubMinScale	= (PR_UBYTE)95.0;
				pPRU->stDetectShapeCmd.rSizePercentVar		= (PR_REAL)5.0;

				g_szWizardStep = "2/2";
				g_szWizardMessPrev = "2. Learn PR";
				g_szWizardMess = "DONE";
				g_szWizardMessNext = "";
				g_szWizardBMPath = "";

				g_bWizardEnableBack = FALSE;
				g_bWizardEnableNext = FALSE;
				g_bWizardEnableExit = TRUE;
			}
		}
		break;

	case 2: //Get point 1
		if (HMI_ulPRAlg == RECTANGLE_MATCHING)
		{
			if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
			{
				m_PRZoomedPRPt_a.x = g_stCursor.prCoord.x;
				m_PRZoomedPRPt_a.y = g_stCursor.prCoord.y;
			}
			else
			{
				PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &m_PRZoomedPRPt_a);
			}
			
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x	= m_PRZoomedPRPt_a.x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y	= m_PRZoomedPRPt_a.y;

			PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, PR_COLOR_RED);
					
			g_szWizardStep = "2/2";
			g_szWizardMessPrev = "1. Locate Upper Left Corner";
			g_szWizardMess = "2. Locate Lower Right Corner";
			g_szWizardMessNext = "DONE";
			g_szWizardBMPath = "";
			
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Lower Right Corner");

			HMI_bShowCursorControl			= TRUE;
			SetHmiVariable("PR_bShowCursorControl");

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
		}
		else if (HMI_ulPRAlg == FIDUCIAL_FITTING)
		{
			if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
			{
				m_PRZoomedPRPt_a.x = g_stCursor.prCoord.x;
				m_PRZoomedPRPt_a.y = g_stCursor.prCoord.y;
			}
			else
			{
				PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &m_PRZoomedPRPt_a);
			}

			pPRU->stManualLrnDieCmd.acoFMarkInputCorners[0].x = m_PRZoomedPRPt_a.x;
			pPRU->stManualLrnDieCmd.acoFMarkInputCorners[0].y = m_PRZoomedPRPt_a.y;
			PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, PR_COLOR_RED);

			// Reset Cursor to PR Centre
			//g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
			//g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
			//PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
		
			g_szWizardStep = "3/3";
			g_szWizardMessPrev = "2. Locate Corner1";
			g_szWizardMess = "3. Locate Corner2";
			g_szWizardMessNext = "DONE";
			g_szWizardBMPath = "";
			
			PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Please Set Pt 2");

			HMI_bShowCursorControl			= TRUE;
			SetHmiVariable("PR_bShowCursorControl");

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
		}
		else
		{
			g_szWizardStep = "DONE";
			g_szWizardMess = "DONE";
			g_szWizardBMPath = "";

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = FALSE;
			g_bWizardEnableExit = TRUE;
		}
		break;

	case 3:
		if (HMI_ulPRAlg == RECTANGLE_MATCHING)
		{
			if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
			{
				m_PRZoomedPRPt_a.x = g_stCursor.prCoord.x;
				m_PRZoomedPRPt_a.y = g_stCursor.prCoord.y;
			}
			else
			{
				PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &m_PRZoomedPRPt_a);
			}
			
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x	= m_PRZoomedPRPt_a.x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y	= m_PRZoomedPRPt_a.y;

			PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, PR_COLOR_RED);
		

			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_RIGHT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_RIGHT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_LEFT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_LEFT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
			pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y		= pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;

			pPRU->emRecordType = PRS_DIE_TYPE;
			pPRU->stManualLrnDieCmd.emAlignAlg = PR_EDGE_POINTS_MATCHING_ON_DIE_EDGES_WO_PATTERN_GUIDED;
			pPRU->stSrchDieCmd.emAlignAlg = PR_EDGE_POINTS_MATCHING_ON_DIE_EDGES_WO_PATTERN_GUIDED;
			pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_FALSE;
			pPRU->stManualLrnDieCmd.emIsLrnCfmPattern = PR_FALSE;
			pPRU->stManualLrnDieCmd.emObjectType = PR_OBJ_TYPE_IC_DIE;

			g_szWizardStep = "3/4";
			g_szWizardMessPrev = "2. Locate Lower Right Corner";
			g_szWizardMess = "DONE";
			g_szWizardMessNext = "DONE";
			g_szWizardBMPath = "";
			
			bResult = ManualLrnDie(pPRU);
		}
		else if (HMI_ulPRAlg == FIDUCIAL_FITTING)
		{
			if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
			{
				m_PRZoomedPRPt_a.x = g_stCursor.prCoord.x;
				m_PRZoomedPRPt_a.y = g_stCursor.prCoord.y;
			}
			else
			{
				PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &m_PRZoomedPRPt_a);
				m_lCurrZoomFactor = PR_ZOOM_FACTOR_1X;
				PRS_DisableDigitalZoom(pPRU->nCamera);
			}

			pPRU->stManualLrnDieCmd.acoFMarkInputCorners[1].x = m_PRZoomedPRPt_a.x;
			pPRU->stManualLrnDieCmd.acoFMarkInputCorners[1].y = m_PRZoomedPRPt_a.y;
			PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, PR_COLOR_RED);

			pPRU->emRecordType = PRS_DIE_TYPE;
			pPRU->stManualLrnDieCmd.emAlignAlg = PRS_ALG_INTR;
			pPRU->stSrchDieCmd.emAlignAlg = PRS_ALG_INTR;
			pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle = PR_TRUE;
			pPRU->stManualLrnDieCmd.emFiducialMarkType = (PR_FIDUCIAL_MARK_TYPE)HMI_ulPRFMarkType;
			pPRU->stManualLrnDieCmd.emObjectType = PR_OBJ_TYPE_FIDUCIAL_MARK;
			bResult = ManualLrnDie(pPRU);
		}

		if (bResult)
		{
			PRS_SetOnMouseClickCmd(OFF);

			// Sucess help to set Pre-Reuqired for search
			pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore	= (PR_REAL)85.0;
			pPRU->stSrchDieCmd.rCfmPatternMatchScore		= (PR_REAL)85.0; // Comfirmation Pattern 	
			pPRU->stDetectShapeCmd.rDetectScore				= (PR_REAL)85.0;
			pPRU->stSrchTmplCmd.ubPassScore					= (PR_UBYTE)85.0;
			
			pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle	= -(PR_REAL)3.0;
			pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle		= (PR_REAL)3.0;
			pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale	= (PR_UBYTE)105.0;
			pPRU->stSrchDieCmd.stDieAlignPar.ubMinScale	= (PR_UBYTE)95.0;
			pPRU->stDetectShapeCmd.rSizePercentVar		= (PR_REAL)5.0;

			// 20140815 Yip: Add Manual Learn Die Ignore Region
			if (rMSG_YES != HMIMessageBox(MSG_YES_NO, "Learn PR", "Do all lines indicate edges?\nYes - Done\nNo - Add Ignore Region"))
			{
				coUpperLeft = pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT];
				coLowerRight = pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT];
				for (int i = 0; i < PR_MAX_DETECT_IGNORE_WIN; ++i)
				{
					pstDetectIgnoreWin->astIgnoreWin[i].coCorner1 = coUpperLeft;
					pstDetectIgnoreWin->astIgnoreWin[i].coCorner2 = coLowerRight;
				}
				pstDetectIgnoreWin->uwNIgnoreWin = 0;

				PRS_ClearScreen(pPRU->nCamera);
				PRS_CancelMouseWindow(pPRU);
				PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
				PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

				g_szWizardStep = "4/4";
				g_szWizardMessPrev = "3. Locate Corner2";
				g_szWizardMess.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
				g_szWizardMessNext = "4.2. Adjust Upper Left Corner";
				g_szWizardBMPath = "";

				g_bWizardEnableBack = FALSE;
				g_bWizardEnableNext = TRUE;
				g_bWizardEnableExit = TRUE;
			}
			else
			{
				g_szWizardStep = "4/4";
				g_szWizardMessPrev = "3. Locate Corner2";
				g_szWizardMess = "DONE";
				g_szWizardMessNext = "";
				g_szWizardBMPath = "";

				g_bWizardEnableBack = FALSE;
				g_bWizardEnableNext = FALSE;
				g_bWizardEnableExit = TRUE;
			}
		}
		break;

	case 4:	// 20140815 Yip: Add Manual Learn Die Ignore Region
		PRS_GetMouseWindow(pPRU, &coUpperLeft, &coLowerRight);
		PRS_CancelMouseWindow(pPRU);

		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1 = coUpperLeft;
		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2 = coLowerRight;

		m_lCurrZoomFactor = PR_ZOOM_FACTOR_2X;

		ZoomCentre = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1;
		g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
		g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
		PRS_DigitalZoom(nCurrentCameraID, m_lCurrZoomFactor, ZoomCentre);
		PRS_DrawCross(nCurrentCameraID, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Adjust Upper Left Corner");

		HMI_bShowCursorControl = TRUE;
		HMI_bShowDigitalZoom = TRUE;

		SetHmiVariable("PR_bShowCursorControl");
		SetHmiVariable("PR_bShowDigitalZoom");

		PRS_SetOnMouseClickCmd(ON);

		g_szWizardStep = "4/4";
		g_szWizardMessPrev.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
		g_szWizardMess = "4.2. Adjust Upper Left Corner";
		g_szWizardMessNext = "4.3. Adjust Lower Right Corner";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;

		break;

	case 5:	// 20140815 Yip: Add Manual Learn Die Ignore Region
		if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
		{
			coUpperLeft = g_stCursor.prCoord;
		}
		else
		{
			PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &coUpperLeft);
		}
		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1 = coUpperLeft;

		PRS_DisableDigitalZoom(pPRU->nCamera);

		ZoomCentre = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2;
		g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
		g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
		PRS_DigitalZoom(nCurrentCameraID, m_lCurrZoomFactor, ZoomCentre);
		PRS_DrawCross(nCurrentCameraID, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW1, "Adjust Lower Right Corner");

		HMI_bShowCursorControl = TRUE;
		HMI_bShowDigitalZoom = TRUE;

		SetHmiVariable("PR_bShowCursorControl");
		SetHmiVariable("PR_bShowDigitalZoom");

		PRS_SetOnMouseClickCmd(ON);

		g_szWizardStep = "4/4";
		g_szWizardMessPrev = "4.2. Adjust Upper Left Corner";
		g_szWizardMess = "4.3. Adjust Lower Right Corner";
		g_szWizardMessNext = "DONE";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = TRUE;
		g_bWizardEnableExit = TRUE;
		break;

	case 6:	// 20140815 Yip: Add Manual Learn Die Ignore Region
		if (m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
		{
			coLowerRight = g_stCursor.prCoord;
		}
		else
		{
			PRS_GetPointFromZoomedImg(m_lCurrZoomFactor, ZoomCentre, g_stCursor.prCoord, &coLowerRight);
			m_lCurrZoomFactor = PR_ZOOM_FACTOR_1X;
			PRS_DisableDigitalZoom(pPRU->nCamera);
		}
		pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2 = coLowerRight;

		PRS_SetOnMouseClickCmd(OFF);
		PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);

		HMI_bShowCursorControl	= FALSE;
		HMI_bShowDigitalZoom	= FALSE;

		SetHmiVariable("PR_bShowCursorControl");
		SetHmiVariable("PR_bShowDigitalZoom");

		++pstDetectIgnoreWin->uwNIgnoreWin;

		for (int i = 0; i < pstDetectIgnoreWin->uwNIgnoreWin; ++i)
		{
			PRS_DrawRect(pPRU->nCamera, pstDetectIgnoreWin->astIgnoreWin[i].coCorner1, pstDetectIgnoreWin->astIgnoreWin[i].coCorner2, (PR_COLOR)(i + 1));
		}

		if (
			(pstDetectIgnoreWin->uwNIgnoreWin < PR_MAX_DETECT_IGNORE_WIN) &&
			(rMSG_YES == HMIMessageBox(MSG_YES_NO, "Learn PR", "Add another ignore region?"))
		   )
		{
			coUpperLeft = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1;
			coLowerRight = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2;

			PRS_CancelMouseWindow(pPRU);
			PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
			PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

			g_szWizardStep = "4/4";
			g_szWizardMessPrev = "3. Locate Corner2";
			g_szWizardMess.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
			g_szWizardMessNext = "4.2. Adjust Upper Left Corner";
			g_szWizardBMPath = "";

			g_bWizardEnableBack = FALSE;
			g_bWizardEnableNext = TRUE;
			g_bWizardEnableExit = TRUE;
		}
		else
		{
			bResult = ManualLrnDie(pPRU);

			if (bResult)
			{
				if (rMSG_YES != HMIMessageBox(MSG_YES_NO, "Learn PR", "Do all lines indicate edges?\nYes - Done\nNo - Modify Ignore Region"))
				{
					pstDetectIgnoreWin->uwNIgnoreWin = 0;
					coUpperLeft = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner1;
					coLowerRight = pstDetectIgnoreWin->astIgnoreWin[pstDetectIgnoreWin->uwNIgnoreWin].coCorner2;

					PRS_ClearScreen(pPRU->nCamera);
					PRS_CancelMouseWindow(pPRU);
					PRS_UseMouseWindow(pPRU, coUpperLeft, coLowerRight);
					PRS_DrawHomeCursor(pPRU->nCamera, FALSE);

					g_szWizardStep = "4/4";
					g_szWizardMessPrev = "3. Locate Corner2";
					g_szWizardMess.Format("4.1. Add Ignore Region %d", pstDetectIgnoreWin->uwNIgnoreWin + 1);
					g_szWizardMessNext = "4.2. Adjust Upper Left Corner";
					g_szWizardBMPath = "";

					g_bWizardEnableBack = FALSE;
					g_bWizardEnableNext = TRUE;
					g_bWizardEnableExit = TRUE;
				}
				else
				{
					g_szWizardStep = "4/4";
					g_szWizardMessPrev = "3. Locate Corner2";
					g_szWizardMess = "DONE";
					g_szWizardMessNext = "";
					g_szWizardBMPath = "";

					g_bWizardEnableBack = FALSE;
					g_bWizardEnableNext = FALSE;
					g_bWizardEnableExit = TRUE;
				}
			}
		}
		g_ulWizardStep = 3;
		break;
	}

	if (!bResult)
	{
		PRS_CancelMouseWindow(pPRU);
		PRS_SetOnMouseClickCmd(OFF);
		PRS_ClearTextRow(pPRU->nCamera, PRS_MSG_ROW1);

		g_szWizardStep = "FAIL";
		g_szWizardMess = "OPERATION FAIL!";
		g_szWizardBMPath = "";

		g_bWizardEnableBack = FALSE;
		g_bWizardEnableNext = FALSE;
		g_bWizardEnableExit = TRUE;
	}

	SetHmiVariable("SF_bWizardEnable");
	SetHmiVariable("SF_szWizardStep");
	SetHmiVariable("SF_szWizardMess");
	SetHmiVariable("SF_szWizardMessPrev");
	SetHmiVariable("SF_szWizardMessNext");
	SetHmiVariable("SF_szWizardBMPath");
	SetHmiVariable("SF_bWizardEnableBack");
	SetHmiVariable("SF_bWizardEnableNext");
	SetHmiVariable("SF_bWizardEnableExit");
}
#endif


/////////////////////////////////////////////////////////////////
//Setup Related
/////////////////////////////////////////////////////////////////
VOID CWinEagle::SetupWizard()
{
	CSettingFile *pCSettingFile = (CSettingFile*)m_pModule->GetStation("SettingFile");
	PRU *pPRU;

	switch (g_ulWizardSubTitle)
	{
	case 0:
		LogAction("WinEagle: Camera Calibration Learn PR pattern");
		pPRU = &SelectCurPRU(HMI_ulCurPRU);
		SetupPRPatternPreTask();
		SetupPRPattern(pPRU);
		SetupPRPatternPostTask();
		break;

	case 1:
		LogAction("WinEagle: Camera Calibration PR Search Area");
		pPRU = &SelectCurPRU(HMI_ulCurPRU);
		SetupPRSrchArea(pPRU);
		break;

	}
}

VOID CWinEagle::SetupPRPatternPreTask()
{
	//Do Every thing before each Step 
}


VOID CWinEagle::SetupPRPatternPostTask()
{
	//Do Every thing after each Step
	switch (g_ulWizardStep)
	{
	case 99:
		UpdateHMICalibPRButton(FALSE);
		break;
	}
}

VOID CWinEagle::ModulePRSetupPreTask()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
		
	//Protection for PreBond
	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCWH1->CheckModSelected(TRUE);
		pCInPickArm->CheckModSelected(TRUE);
		if (pCWH1->m_bModSelected && pCInPickArm->m_bModSelected)
		{
			m_bMoveToPosn = TRUE;
		}
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCWH1->CheckModSelected(TRUE);
		pCInPickArm->CheckModSelected(TRUE);
		if (pCWH1->m_bModSelected && pCInPickArm->m_bModSelected)
		{
			m_bMoveToPosn = TRUE;
		}
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCWH2->CheckModSelected(TRUE);
		pCInPickArm->CheckModSelected(TRUE);
		if (pCWH2->m_bModSelected && pCInPickArm->m_bModSelected)
		{
			m_bMoveToPosn = TRUE;
		}
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCWH2->CheckModSelected(TRUE);
		pCInPickArm->CheckModSelected(TRUE);
		if (pCWH2->m_bModSelected && pCInPickArm->m_bModSelected)
		{
			m_bMoveToPosn = TRUE;
		}
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCWH1->CheckModSelected(TRUE);
		pCInspOpt->CheckModSelected(TRUE);

		if (pCWH1->m_bModSelected && pCInspOpt->m_bModSelected)
		{
			m_bMoveToPosn = TRUE;
		}
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCWH2->CheckModSelected(TRUE);
		pCInspOpt->CheckModSelected(TRUE);

		if (pCWH2->m_bModSelected && pCInspOpt->m_bModSelected)
		{
			m_bMoveToPosn = TRUE;
		}
		break;
	}

	m_bMoveToPosn = FALSE; //20130405
	//if (m_bMoveToPosn)
	//{
	//	SelectCalibPRU(HMI_ulCurPRU);
	//}

	UpdateHMICalibPRButton(HMI_ulCurPRU);

}

// Add me later
VOID CWinEagle::ModulePRSetupPostTask()
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	COutPickArm *pCOutPickArm = (COutPickArm*)m_pModule->GetStation("OutPickArm");

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCWH1->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCWH1->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCWH2->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCWH2->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCWH1->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCInspOpt->MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_WAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCWH2->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		pCInspOpt->MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT);
		break;
	}


}

LONG CWinEagle::SwitchAllCam()	// 20140623 Yip
{
	LONG lLoopCnt = 0;
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");

	PRU *pPRU = NULL;
	DisplayMessage(CString(__FUNCTION__) + CString(" Begin..."));


	DisplayMessage(CString(__FUNCTION__) + CString(" Finished."));
	return 0;
}

//-----------------------------------------------------------------------------
// HMI command implementation
//-----------------------------------------------------------------------------
//

LONG CWinEagle::HMISwitchCam(IPC_CServiceMessage &svMsg)
{
	CHouseKeeping *pCHouseKeeping = (CHouseKeeping*)m_pModule->GetStation("HouseKeeping");

	PRU *pPRU = NULL;
	PR_COORD coCentre;
	coCentre.x = PR_DEF_CENTRE_X;
	coCentre.y = PR_DEF_CENTRE_Y;

	if (!m_lAutoBondMode || State() != AUTO_Q)
	{
		pPRU = &PRS_SetCameraPRU(nCamera);

		if (g_bEnableTrigLighting)
		{
			PRS_EnableHWTrigLighting(pPRU->nCamera, FALSE);
		}

		if (nCamera >= MAX_CAM)
		{
			nCamera = INSPUL_CAM;
		}
		else
		{
			nCamera++;
		}

		pPRU = &PRS_SetCameraPRU(nCamera);

		if (g_bEnableTrigLighting)
		{
			PRS_EnableHWTrigLighting(pPRU->nCamera, TRUE);
		}

		PRS_DisplayVideo(pPRU);

		if (nCamera != NO_CAM)
		{
			//for (int dx = -m_lCrosshairWidth ; dx <= m_lCrosshairWidth ; dx++)
			//{
			//	coCentre.x = PR_DEF_CENTRE_X + dx;
			//	coCentre.y = PR_DEF_CENTRE_Y + dx;

			//	//cmdDrawRect.stWin.coCorner1.x = stWin.coCorner1.x + dx;
			//	//cmdDrawRect.stWin.coCorner1.y = stWin.coCorner1.y + dx;
			//	//BoundPRCoord(cmdDrawRect.stWin.coCorner1);
			//	//cmdDrawRect.stWin.coCorner2.x = stWin.coCorner2.x + dx;
			//	//cmdDrawRect.stWin.coCorner2.y = stWin.coCorner2.y + dx;
			//	//BoundPRCoord(cmdDrawRect.stWin.coCorner2);
			//	//PR_DrawRectCmd(&cmdDrawRect, m_sid, m_rid, &rpyDrawRect); //from CPL
			//}
			PRS_DrawHomeCursor(nCurrentCameraID, FALSE);
			//PRS_DrawCross(nCamera, coCentre, PRS_XLARGE_SIZE, PR_COLOR_RED);
		}

		bPRUpdateLight = TRUE;

		// updated lighting
		UpdateLighting(pPRU);
	}
	return 0;
}

LONG CWinEagle::HMI_SelectCurStation(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	// Remarks for the HMI Display PRStaion seq must equal to the add m_PRStations at PI9000.h
	LONG	lMsgLen = svMsg.GetMsgLen();
	unsigned char cArray[10] = " ";
	UCHAR	*cName = &cArray[0];
	CString temp;

	if (lMsgLen > 0)
	{
		cName = new UCHAR[lMsgLen];
		if (cName == NULL) //klocwork fix 20121211
		{
			return 0;
		}
		svMsg.GetMsg(cName, lMsgLen);
	}

	HMI_szCurPRStation = cName;
	HMI_lCurRecord	= 0;

	//To find the record
	LONG lTemp = 0;
	CString szCurStaionName;
	szCurStaionName = HMI_szCurPRStation;

	
	for (INT i = 0; i < GetmyPRUList().GetSize(); i++)
	{
		if (GetmyPRUList().GetAt(i)->szStnName == szCurStaionName)
		{
			if (lTemp == HMI_lCurRecord)
			{
				HMI_ulCurPRU = GetmyPRUList().GetAt(i)->emID;
				break;
			}
			lTemp++;
		}
	}

	//HMI_ulCurPRU = 0;

	UpdateHMIDiagnPRScreen(FALSE);
	if (cName != NULL) //klocwork fix 20121211
	{
		delete[] cName;
	}
		//delete cName;

	return 0;
}

LONG CWinEagle::HMI_SelectCurRecord(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	LONG lRecord;
	svMsg.GetMsg(sizeof(LONG), &lRecord);

	HMI_lCurRecord = lRecord;

	//To find the record
	LONG lTemp = 0;
	CString szCurStaionName;
	szCurStaionName = HMI_szCurPRStation;

	for (INT i = 0; i < GetmyPRUList().GetSize(); i++)
	{
		if (GetmyPRUList().GetAt(i)->szStnName == szCurStaionName)
		{
			if (lTemp == lRecord)
			{
				HMI_ulCurPRU = GetmyPRUList().GetAt(i)->emID;
				break;
			}
			lTemp++;
		}
	}

	//HMI_ulCurPRU = 0;

	UpdateHMIDiagnPRScreen(FALSE);
	return 0;
}

LONG CWinEagle::HMIUpdateDiagnPRScreen(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	// Reset all Data
	//HMI_szCurPRStation = "TrayHolder";
	HMI_lCurRecord	= 0;

	//To find the record
	LONG lTemp = 0;
	CString szCurStaionName;
	szCurStaionName = HMI_szCurPRStation;

	
	for (INT i = 0; i < GetmyPRUList().GetSize(); i++)
	{
		if (GetmyPRUList().GetAt(i)->szStnName == szCurStaionName)
		{
			if (lTemp == HMI_lCurRecord)
			{
				HMI_ulCurPRU = GetmyPRUList().GetAt(i)->emID;
				break;
			}
			lTemp++;
		}
	}

	UpdateHMIDiagnPRScreen(FALSE);
	return 0;
}

LONG CWinEagle::HMIUpdateServicePRScreen(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	// Reset all Data
	//HMI_szCurPRStation = "TrayHolder";
	HMI_lCurRecord	= 0;

	//To find the record
	LONG lTemp = 0;
	CString szCurStaionName;
	szCurStaionName = HMI_szCurPRStation;

	
	for (INT i = 0; i < GetmyPRUList().GetSize(); i++)
	{
		if (GetmyPRUList().GetAt(i)->szStnName == szCurStaionName)
		{
			if (lTemp == HMI_lCurRecord)
			{
				HMI_ulCurPRU = GetmyPRUList().GetAt(i)->emID;
				break;
			}
			lTemp++;
		}
	}

	UpdateHMIDiagnPRScreen(FALSE);
	return 0;
}


LONG CWinEagle::HMIClearPRRecord(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	LONG lAnswer = rMSG_TIMEOUT;

	lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Operation is irreversible. Continue?");
	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
	}
	
	else if (lAnswer == rMSG_CONTINUE)
	{
		pPRU	= GetmyPRUList().GetAt(HMI_ulCurPRU);
	
		if (PRS_FreeRecordID(pPRU))
		{
			WriteHMIMess("Free PR Record ID Completed");
		}
		else
		{
			WriteHMIMess("Free PR Record ID Failed!");
		}
	}
	else
	{
		WriteHMIMess("Operation Cancelled!");
	}

	UpdateHMIDiagnPRScreen(FALSE);
	return 0;
}

LONG CWinEagle::HMIStartRepeatTest(IPC_CServiceMessage &svMsg)
{

	PRU *pPRU;
	CString szMsg = _T("");
	BOOL	bSrch = FALSE;

	pPRU	= GetmyPRUList().GetAt(HMI_ulCurPRU);
	
	StartRepeatTest(pPRU);

	return 0;
	
}

LONG CWinEagle::HMISetTestCounter(IPC_CServiceMessage &svMsg)
{
	ULONG ulCounter;
	svMsg.GetMsg(sizeof(ULONG), &ulCounter);

	ulTestCounter = ulCounter;
	return 0;
}

LONG CWinEagle::HMISetTestDelay(IPC_CServiceMessage &svMsg)
{
	ULONG ulDelay;
	svMsg.GetMsg(sizeof(ULONG), &ulDelay);

	ulTestDelay = ulDelay;
	return 0;
}

LONG CWinEagle::HMIIncrExposureTime(IPC_CServiceMessage &svMsg)
{

	INT		i = 0;
	ULONG	ulExposureTime;
	PRU	*pPRU;

	pPRU	= GetmyPRUList().GetAt(HMI_ulCurPRU);

	ulExposureTime = (ULONG)PRS_GetExposureTime(pPRU);
	
	ulExposureTime++;
	if (ulExposureTime > 10)
	{
		ulExposureTime = 10;
	}

	ulShowExposureTime = ulExposureTime;

	PRS_SetExposureTime(pPRU, (INT)ulExposureTime);

	return 0;
}

LONG CWinEagle::HMIDecrExposureTime(IPC_CServiceMessage &svMsg)
{
	INT		i = 0;
	ULONG	ulExposureTime;
	PRU	*pPRU;
	
	pPRU	= GetmyPRUList().GetAt(HMI_ulCurPRU);

	ulExposureTime = (ULONG)PRS_GetExposureTime(pPRU);

	if (ulExposureTime > 0)
	{
		ulExposureTime--;
	}
	ulShowExposureTime = ulExposureTime;

	PRS_SetExposureTime(pPRU, (INT)ulExposureTime);

	return 0;
}

LONG CWinEagle::HMIIncrCoaxialLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT);
	
	ulLightLevel++;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrCoaxialLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT);

	if (ulLightLevel > 0)
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrCoaxialLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT);
	
	ulLightLevel += 10;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrCoaxialLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrCoaxial1Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_1);
	
	ulLightLevel++;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrCoaxial1Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_1);

	if (ulLightLevel > 0)
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrCoaxial1Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_1);
	
	ulLightLevel += 10;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrCoaxial1Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_1);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrCoaxial2Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_2);
	
	ulLightLevel++;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_2, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrCoaxial2Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_2);

	if (ulLightLevel > 0)
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_2, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrCoaxial2Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_2);
	
	ulLightLevel += 10;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_2, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrCoaxial2Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_COAXIAL_LIGHT_2);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_COAXIAL_LIGHT_2, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrRingLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT);
	

	ulLightLevel++;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_RING_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrRingLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT);

	if (ulLightLevel > 0)
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_RING_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrRingLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT);
	
	ulLightLevel += 10;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_RING_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrRingLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_RING_LIGHT, (PR_UWORD)ulLightLevel);

	return 0;
}

LONG CWinEagle::HMIIncrRing1Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT_1);
	
	ulLightLevel++;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}
	PRS_SetLighting(pPRU, PR_RING_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrRing1Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT_1);

	if (ulLightLevel > 0)
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_RING_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrRing1Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT_1);
	
	ulLightLevel += 10;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_RING_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrRing1Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_RING_LIGHT_1);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_RING_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrSide1Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT_1);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrSide1Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT_1);

	if (ulLightLevel < 1)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrSide1Level(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT_1);

	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}
	else
	{
		ulLightLevel++;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrSide1Level10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT_1);

	if (ulLightLevel > 90)
	{
		ulLightLevel = 100;
	}
	else
	{
		ulLightLevel += 10;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT_1, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrSideLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrSideLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT);

	if (ulLightLevel < 1)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrSideLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT);

	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}
	else
	{
		ulLightLevel++;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrSideLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_SIDE_LIGHT);

	if (ulLightLevel > 90)
	{
		ulLightLevel = 100;
	}
	else
	{
		ulLightLevel += 10;
	}

	PRS_SetLighting(pPRU, PR_SIDE_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}
//20150709
LONG CWinEagle::HMIIncrBackLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_BACK_LIGHT);
	
	ulLightLevel++;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_BACK_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrBackLevel(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_BACK_LIGHT);

	if (ulLightLevel > 0)
	{
		ulLightLevel--;
	}

	PRS_SetLighting(pPRU, PR_BACK_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIIncrBackLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_BACK_LIGHT);
	
	ulLightLevel += 10;
	if (ulLightLevel > 100)
	{
		ulLightLevel = 100;
	}

	PRS_SetLighting(pPRU, PR_BACK_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}

LONG CWinEagle::HMIDecrBackLevel10(IPC_CServiceMessage &svMsg)
{
	ULONG	ulLightLevel;
	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	// fix me! issue with PRU lighting (use HMI_ulCurPRU?)
	pPRU = &PRS_SetCameraPRU(nCamera);

	ulLightLevel = (ULONG)PRS_GetLighting(pPRU, PR_BACK_LIGHT);

	if (ulLightLevel < 10)
	{
		ulLightLevel = 0;
	}
	else
	{
		ulLightLevel -= 10;
	}

	PRS_SetLighting(pPRU, PR_BACK_LIGHT, (PR_UWORD)ulLightLevel);
	return 0;
}//20150709

LONG CWinEagle::HMI_SetCursorDiagnSteps(IPC_CServiceMessage &svMsg)
{
	ULONG ulSteps;
	svMsg.GetMsg(sizeof(ULONG), &ulSteps);

	switch (ulSteps)
	{
	case 0:
		m_lCursorDiagnSteps = 5;
		break;
	case 1:
		m_lCursorDiagnSteps = 10;
		break;
	case 2:
		m_lCursorDiagnSteps = 50;
		break;
	case 3:
		m_lCursorDiagnSteps = 100;
		break;
	case 4:
		m_lCursorDiagnSteps = 500;
		break;
	case 5:
		m_lCursorDiagnSteps = 1000;
		break;
	default:
		m_lCursorDiagnSteps = 1000;
	}
	return 0;
}

// Add me later
LONG CWinEagle::HMI_IndexCursorXPos(IPC_CServiceMessage &svMsg)
{

	PR_BOOLEAN	TempResultantCoordStatus	= PR_FALSE;

	PR_COORD	prTempAlignPt_a;
	//PR_COORD	prTempCentre;
	//BOOL		bDrawCentreCursor			= FALSE;

	prTempAlignPt_a.x	= PR_DEF_CENTRE_X;
	prTempAlignPt_a.y	= PR_DEF_CENTRE_Y;

	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	pPRU = &PRS_SetCameraPRU(nCamera);
	

	//Check drawing centre cursor is required (Set PreBond and PreBond WH alignment pt)
	//if (
	//	(g_lWizardInProgress == SF_PREBOND_WH && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 2 || g_ulWizardStep == 4))||
	//	(g_lWizardInProgress == SF_PREBOND && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 3 || g_ulWizardStep == 5))
	//	)
	//{
	//	if (g_lWizardInProgress == SF_PREBOND)
	//	{
	//		prTempAlignPt_a.x = pCPreBond->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBond->m_PRZoomedAlignPt_a.y;
	//	}
	//	else if (g_lWizardInProgress == SF_PREBOND_WH)
	//	{
	//		prTempAlignPt_a.x = pCPreBondWH->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBondWH->m_PRZoomedAlignPt_a.y;
	//	}

	//	bDrawCentreCursor = TRUE;
	//}

	// Check cursor is out of boundary
	if (g_stCursor.prCoord.x + (PR_WORD)m_lCursorDiagnSteps > PR_MAX_COORD)
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Out of Boundary");
	}
	else
	{
		// Erase old cursors
		PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);

		//if (bDrawCentreCursor)
		//{
		//	CalcCentreCursorPt(pPRU, FALSE, prTempAlignPt_a, &prTempCentre);
		//	PRS_EraseCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize);
		//}

		// draw new cursors
		g_stCursor.prCoord.x += (PR_WORD)m_lCursorDiagnSteps;
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Cursor Coord: (%ld,%ld)", g_stCursor.prCoord.x, g_stCursor.prCoord.y);
		PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);

		//if (bDrawCentreCursor)
		//{
		//	PR_COORD prTempCentre;
		//	
		//	if (!CalcCentreCursorPt(pPRU, TRUE, prTempAlignPt_a, &prTempCentre))
		//	{
		//		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "out of zoomed region");
		//	}
		//	else
		//	{
		//		PRS_DrawCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize, m_stCentreCursor.lColor);
		//	}
		//}
	}

	return 0;
}

// Add me later
LONG CWinEagle::HMI_IndexCursorXNeg(IPC_CServiceMessage &svMsg)
{

	PR_BOOLEAN	TempResultantCoordStatus	= PR_FALSE;

	PR_COORD	prTempAlignPt_a;
	//PR_COORD	prTempCentre;
	//BOOL		bDrawCentreCursor	= FALSE;

	prTempAlignPt_a.x			= PR_DEF_CENTRE_X;
	prTempAlignPt_a.y			= PR_DEF_CENTRE_Y;

	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	pPRU = &PRS_SetCameraPRU(nCamera);

	//if (
	//	(g_lWizardInProgress == SF_PREBOND_WH && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 2 || g_ulWizardStep == 4))||
	//	(g_lWizardInProgress == SF_PREBOND && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 3 || g_ulWizardStep == 5))
	//	)
	//{
	//	if (g_lWizardInProgress == SF_PREBOND)
	//	{
	//		prTempAlignPt_a.x = pCPreBond->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBond->m_PRZoomedAlignPt_a.y;
	//	}
	//	else if (g_lWizardInProgress == SF_PREBOND_WH)
	//	{
	//		prTempAlignPt_a.x = pCPreBondWH->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBondWH->m_PRZoomedAlignPt_a.y;
	//	}

	//	bDrawCentreCursor = TRUE;
	//}

	// Check cursor is out of boundary
	if (g_stCursor.prCoord.x - (PR_WORD)m_lCursorDiagnSteps < 0)
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Out of Boundary");
	}
	else
	{
		// Erase old cursors
		PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);

		//if (bDrawCentreCursor)
		//{
		//	CalcCentreCursorPt(pPRU, FALSE, prTempAlignPt_a, &prTempCentre);
		//	PRS_EraseCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize);
		//}

		// draw new cursors
		g_stCursor.prCoord.x -= (PR_WORD)m_lCursorDiagnSteps;
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Cursor Coord: (%ld,%ld)", g_stCursor.prCoord.x, g_stCursor.prCoord.y);
		PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);

		//if (bDrawCentreCursor)
		//{
		//	if (!CalcCentreCursorPt(pPRU, TRUE, prTempAlignPt_a, &prTempCentre))
		//	{
		//		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "out of zoomed region");
		//	}
		//	else
		//	{
		//		PRS_DrawCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize, m_stCentreCursor.lColor);
		//	}
		//}
	}

	return 0;
}

// Add me later
LONG CWinEagle::HMI_IndexCursorYPos(IPC_CServiceMessage &svMsg)
{
	PR_BOOLEAN	TempResultantCoordStatus	= PR_FALSE;

	PR_COORD	prTempAlignPt_a;
	//PR_COORD	prTempCentre;
	//BOOL		bDrawCentreCursor	= FALSE;

	prTempAlignPt_a.x			= PR_DEF_CENTRE_X;
	prTempAlignPt_a.y			= PR_DEF_CENTRE_Y;

	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	pPRU = &PRS_SetCameraPRU(nCamera);

	//Check drawing centre cursor is required (Set PreBond and PreBond WH alignment pt)
	//if (
	//	(g_lWizardInProgress == SF_PREBOND_WH && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 2 || g_ulWizardStep == 4))||
	//	(g_lWizardInProgress == SF_PREBOND && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 3 || g_ulWizardStep == 5))
	//	)
	//{
	//	if (g_lWizardInProgress == SF_PREBOND)
	//	{
	//		prTempAlignPt_a.x = pCPreBond->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBond->m_PRZoomedAlignPt_a.y;
	//	}
	//	else if (g_lWizardInProgress == SF_PREBOND_WH)
	//	{
	//		prTempAlignPt_a.x = pCPreBondWH->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBondWH->m_PRZoomedAlignPt_a.y;
	//	}
	//	bDrawCentreCursor = TRUE;
	//}

	// Check cursor is out of boundary
	if (g_stCursor.prCoord.y + (PR_WORD)m_lCursorDiagnSteps > PR_MAX_COORD)
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Out of Boundary");
	}
	else
	{
		// Erase old cursors
		PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);

		//if (bDrawCentreCursor)
		//{
		//	CalcCentreCursorPt(pPRU, FALSE, prTempAlignPt_a, &prTempCentre);
		//	PRS_EraseCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize);
		//}

		// draw new cursors
		g_stCursor.prCoord.y += (PR_WORD)m_lCursorDiagnSteps;
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Cursor Coord: (%ld,%ld)", g_stCursor.prCoord.x, g_stCursor.prCoord.y);
		PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);

		//if (bDrawCentreCursor)
		//{
		//	if (!CalcCentreCursorPt(pPRU, TRUE, prTempAlignPt_a, &prTempCentre))
		//	{
		//		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "out of zoomed region");
		//	}
		//	else
		//	{
		//		PRS_DrawCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize, m_stCentreCursor.lColor);
		//	}
		//}
	}

	return 0;
}

// Add me later
LONG CWinEagle::HMI_IndexCursorYNeg(IPC_CServiceMessage &svMsg)
{

	PR_BOOLEAN	TempResultantCoordStatus	= PR_FALSE;

	PR_COORD	prTempAlignPt_a;
	//PR_COORD	prTempCentre;
	//BOOL		bDrawCentreCursor	= FALSE;

	prTempAlignPt_a.x			= PR_DEF_CENTRE_X;
	prTempAlignPt_a.y			= PR_DEF_CENTRE_Y;

	PRU	*pPRU;

	if (nCamera == NO_CAM)
	{
		return 0;
	}

	pPRU = &PRS_SetCameraPRU(nCamera);

	//Check drawing centre cursor is required (Set PreBond and PreBond WH alignment pt)
	//if (
	//	(g_lWizardInProgress == SF_PREBOND_WH && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 2 || g_ulWizardStep == 4))||
	//	(g_lWizardInProgress == SF_PREBOND && g_ulWizardSubTitle == 0 && (g_ulWizardStep == 3 || g_ulWizardStep == 5))
	//	)
	//{
	//	if (g_lWizardInProgress == SF_PREBOND)
	//	{
	//		prTempAlignPt_a.x = pCPreBond->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBond->m_PRZoomedAlignPt_a.y;
	//	}
	//	else if (g_lWizardInProgress == SF_PREBOND_WH)
	//	{
	//		prTempAlignPt_a.x = pCPreBondWH->m_PRZoomedAlignPt_a.x;
	//		prTempAlignPt_a.y = pCPreBondWH->m_PRZoomedAlignPt_a.y;
	//	}
	//	bDrawCentreCursor = TRUE;
	//}

	// Check cursor is out of boundary
	if (g_stCursor.prCoord.y - (PR_WORD)m_lCursorDiagnSteps < 0)
	{
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Out of Boundary");
	}
	else
	{
		// Erase old cursors
		PRS_EraseCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize);

		//if (bDrawCentreCursor)
		//{
		//	CalcCentreCursorPt(pPRU, FALSE, prTempAlignPt_a, &prTempCentre);
		//	PRS_EraseCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize);
		//}

		// draw new cursors
		g_stCursor.prCoord.y -= (PR_WORD)m_lCursorDiagnSteps;
		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "Cursor Coord: (%ld,%ld)", g_stCursor.prCoord.x, g_stCursor.prCoord.y);
		PRS_DrawCross(pPRU->nCamera, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);

		//if (bDrawCentreCursor)
		//{
		//	if (!CalcCentreCursorPt(pPRU, TRUE, prTempAlignPt_a, &prTempCentre))
		//	{
		//		PRS_DisplayText(pPRU->nCamera, 1, PRS_MSG_ROW2, "out of zoomed region");
		//	}
		//	else
		//	{
		//		PRS_DrawCross(pPRU->nCamera, prTempCentre, m_stCentreCursor.lSize, m_stCentreCursor.lColor);
		//	}
		//}
	}
	return 0;
}

LONG CWinEagle::HMIFreeAllRecords(IPC_CServiceMessage &svMsg)
{
	LONG lAnswer = rMSG_TIMEOUT;

	lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Operation is irreversible. Continue?");

	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
	}
	else if (lAnswer == rMSG_CONTINUE)
	{
		FreeAllRecords();
		PRS_InitPRUs();
		WriteHMIMess("*** All PR Record Freed ***");
	}
	else
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
	}

	return 0;
}

////////////////////////////////////////////
// Advance Log Function
////////////////////////////////////////////
LONG CWinEagle::HMIOnOffLogMode(IPC_CServiceMessage &svMsg)
{
	if (!m_bPRDebug)
	{
		PRS_EnableLog();
		WriteHMIMess("*** PR Log Enabled ***");
		m_bPRDebug = TRUE;
	}
	else
	{
		PRS_DisableLog();
		WriteHMIMess("*** PR Log Disabled ***");
		m_bPRDebug = FALSE;
	}

	return 0;
}

LONG CWinEagle::HMISetDebugFlag(IPC_CServiceMessage &svMsg)
{
	PR_DEBUG ulDebugFlag;

	CString sDebugFlag;
	LPTSTR lpsz = NULL;
	INT nMsgLen = 0;

	nMsgLen = svMsg.GetMsgLen();
	lpsz = new TCHAR[nMsgLen];
	svMsg.GetMsg(lpsz, nMsgLen);
	sDebugFlag = ToString(lpsz); // convert string char to CString

	if ("Disable" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_DISABLE;
	}
	else if ("Show Data" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_SHOW_DATA;
	}
	else if ("Show Time" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_SHOW_TIME;
	}
	else if ("2 Points" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_2_POINTS;
	}
	else if ("Golden Template" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_GOLDEN_TMPL;
	}
	else if ("Show Permanent Mem" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_PERMANENT_MEM;
	}
	else if ("Log Data" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_LOG_DATA;
	}
	else if ("Log Result" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_LOG_RESULT;
	}
	else if ("Log Auto Test" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_LOG_AUTO_TEST;
	}
	else if ("Log Valid Parameters" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_VALID_PAR;
	}
	else if ("BGA" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_BGA;
	}
	else if ("Log Failure" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_LOG_FAILURE;
	}
	else if ("Log Min Failure" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_LOG_MIN_FAILURE;
	}
	else if ("Log Alg Time" == sDebugFlag)
	{
		ulDebugFlag = PR_DEBUG_LOG_ALG_TIME;
	}
	else
	{
		ulDebugFlag = PR_DEBUG_DISABLE;
	}

	PRS_SetDebugFlag(ulDebugFlag);

	CString szMsg = "*** PR Debug Mode Set To " + sDebugFlag + " ***";
	WriteHMIMess(szMsg);

	return 0;
}

LONG CWinEagle::HMISetComLogFlag(IPC_CServiceMessage &svMsg)
{
	PR_COM_LOG ulComLogFlag = PR_COM_LOG_DISABLE;

	CString sComLogFlag;
	LPTSTR lpsz = NULL;
	INT nMsgLen = 0;

	nMsgLen = svMsg.GetMsgLen();
	lpsz = new TCHAR[nMsgLen];
	svMsg.GetMsg(lpsz, nMsgLen);
	sComLogFlag = ToString(lpsz); // convert string char to CString

	if ("Disable" == sComLogFlag)
	{
		ulComLogFlag = PR_COM_LOG_DISABLE;
	}
	else if ("Log All" == sComLogFlag)
	{
		ulComLogFlag = PR_COM_LOG_ALL;
	}
	else if ("Log Long Com Time" == sComLogFlag)
	{
		ulComLogFlag = PR_COM_LOG_LONG_COM_TIME;
	}

	PRS_SetComLogFlag(ulComLogFlag);

	CString szMsg = "*** PR Comm Log Flag Set To " + sComLogFlag + " ***";
	WriteHMIMess(szMsg);

	return 0;
}

LONG CWinEagle::HMISetComLogTimeLimit(IPC_CServiceMessage &svMsg)
{
	CString szMsg = _T("");

	ULONG ulComLogTimeLimit;
	svMsg.GetMsg(sizeof(ULONG), &ulComLogTimeLimit);

	PRS_SetComLogTimeLimit((PR_ULWORD)ulComLogTimeLimit);

	szMsg.Format("*** PR comm log time limit set to %ld ***", ulComLogTimeLimit);
	WriteHMIMess(szMsg);

	return 0;
}

LONG CWinEagle::HMISetVisionNtDlgToService(IPC_CServiceMessage &svMsg)
{
	PRS_VisionNtDlg(PR_VISIONNT_DLG_SERVICE);

	return 0;
}

LONG CWinEagle::HMISetVisionNtDlgToVision(IPC_CServiceMessage &svMsg)
{
	PRS_VisionNtDlg(PR_VISIONNT_DLG_DIAGNOSTIC);

	return 0;
}

/////////////////////////////////////
// Caibration Setup Page
/////////////////////////////////////
LONG CWinEagle::HMI_SetCalibPRSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCWH1->SetPRSelected(bMode);
		pCInPickArm->SetPRSelected(bMode);
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCWH1->SetPRSelected(bMode);
		pCInPickArm->SetPRSelected(bMode);
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCWH2->SetPRSelected(bMode);
		pCInPickArm->SetPRSelected(bMode);
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCWH2->SetPRSelected(bMode);
		pCInPickArm->SetPRSelected(bMode);
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCWH1->SetPRSelected(bMode);
		pCInspOpt->SetPRSelected(bMode);
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCWH2->SetPRSelected(bMode);
		pCInspOpt->SetPRSelected(bMode);
		break;
	}

	UpdateHMICalibPRButton(FALSE);

	return 0;
}

LONG CWinEagle::HMI_SetCalibModSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCWH1->SetModSelected(bMode);
		pCInPickArm->SetModSelected(bMode);
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCWH1->SetModSelected(bMode);
		pCInPickArm->SetModSelected(bMode);
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCWH2->SetModSelected(bMode);
		pCInPickArm->SetModSelected(bMode);
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCWH2->SetModSelected(bMode);
		pCInPickArm->SetModSelected(bMode);
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCWH1->SetModSelected(bMode);
		pCInspOpt->SetModSelected(bMode);
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCWH2->SetModSelected(bMode);
		pCInspOpt->SetModSelected(bMode);
		break;
	}

	UpdateHMICalibPRButton(FALSE);

	return 0;
}

LONG CWinEagle::HMI_SelectCalibPRU(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	ULONG ulPRU;
	svMsg.GetMsg(sizeof(ULONG), &ulPRU);
	
	LONG lAnswer = rMSG_TIMEOUT;
	HMI_ulCurPRU = ulPRU;
	
	lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Move Modules to Calibration Posns?");

	if (lAnswer == rMSG_TIMEOUT)
	{
		HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
		return 0;
	}		
	else if (lAnswer == rMSG_CONTINUE)
	{
		m_bMoveToPosn = TRUE;
	}
	else
	{
		m_bMoveToPosn = FALSE;
	}

	SelectCalibPRU(ulPRU);

	UpdateHMICalibPRButton(FALSE);

	return 0;
}

LONG CWinEagle::HMI_SelectPRAlg(IPC_CServiceMessage &svMsg)
{
	CSettingFile *pCSettingFile = (CSettingFile*)m_pModule->GetStation("SettingFile");
	BOOL bOpDone = FALSE;

	ULONG ulAlg;
	svMsg.GetMsg(sizeof(ULONG), &ulAlg);

	HMI_ulPRAlg = ulAlg;

	switch (HMI_ulPRAlg) //Update HMI only
	{
	case FIDUCIAL_FITTING:
		HMI_bShowFiducial = TRUE;
		SetHmiVariable("PR_bShowFiducial");
		switch (HMI_ulPRFMarkType)
		{
		case 0:
			g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType1.png";
			SetHmiVariable("SF_szWizardBMPath");

			break;
		case 1:
			g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType2.png";
			SetHmiVariable("SF_szWizardBMPath");

			break;
		case 2:
			g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType3.png";
			SetHmiVariable("SF_szWizardBMPath");

			break;
		}
		break;

	default:
		HMI_bShowFiducial = FALSE;
		SetHmiVariable("PR_bShowFiducial");
		g_szWizardBMPath = "";
		SetHmiVariable("SF_szWizardBMPath");	
		break;
	}

	switch (HMI_ulPRAlg) //Update HMI only
	{
	case PATTERN_MATCHING:
	case EDGE_MATCHING:
	case FIDUCIAL_FITTING:
	case RECTANGLE_MATCHING:
		pCSettingFile->HMI_bShowIgnoreWindowBtn = TRUE;
		break;
	case SHAPE_FITTING:
	case TMPL_FITTING:
		pCSettingFile->HMI_bShowIgnoreWindowBtn = FALSE;
		break;
	}
	SetHmiVariable("SF_bShowIgnoreWindowBtn");

	bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CWinEagle::HMI_SelectFMarkType(IPC_CServiceMessage &svMsg)
{
	ULONG ulType;
	svMsg.GetMsg(sizeof(ULONG), &ulType);

	HMI_ulPRFMarkType = ulType;
	switch (HMI_ulPRFMarkType)
	{
	case 0:
		g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType1.png";
		SetHmiVariable("SF_szWizardBMPath");

		break;
	case 1:
		g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType2.png";
		SetHmiVariable("SF_szWizardBMPath");

		break;
	case 2:
		g_szWizardBMPath = "d:\\sw\\PI9000\\Hmi\\images\\Wizard\\PR\\FMarkType3.png";
		SetHmiVariable("SF_szWizardBMPath");

		break;
	}
	return 0;
}

LONG CWinEagle::HMI_SetCalibPRDelay(IPC_CServiceMessage &svMsg)
{
	//CTrayHolder *pCTrayHolder = (CTrayHolder*)m_pModule->GetStation("TrayHolder");
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	LONG lDelay;
	svMsg.GetMsg(sizeof(LONG), &lDelay);

	HMI_lCalibPRDelay = lDelay;

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCWH1->m_lCalibPRDelay = lDelay;
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCWH1->m_lCalibPRDelay = lDelay;
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCWH2->m_lCalibPRDelay = lDelay;
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCWH2->m_lCalibPRDelay = lDelay;
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCInspOpt->m_lCalibPRDelay = lDelay;
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCInspOpt->m_lCalibPRDelay = lDelay;
		break;
	}

	UpdateHMICalibPRButton(FALSE);
	return 0;
}

LONG CWinEagle::HMI_SetCalibCORRange(IPC_CServiceMessage &svMsg)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	DOUBLE dRange;
	svMsg.GetMsg(sizeof(DOUBLE), &dRange);

	if (dRange == 0)
	{
		HMIMessageBox(MSG_OK, "SET COR CALIBRATION RANGE", "Parameter Error! Calibration Range must not be 0");
		return 0;
	}

	switch (HMI_ulCurPRU)
	{

	case INSPDL1_WH1_CALIB_PRU:		
		pCWH1->m_dCalibCORRange	= dRange;
		break;

	case INSPDL2_WH1_CALIB_PRU:		
		pCWH1->m_dCalibCORRange	= dRange;
		break;

	case INSPDL1_WH2_CALIB_PRU:		
		pCWH2->m_dCalibCORRange	= dRange;
		break;

	case INSPDL2_WH2_CALIB_PRU:		
		pCWH2->m_dCalibCORRange	= dRange;
		break;

	case INSPWHUL_WH1_CALIB_PRU:		
		pCInspOpt->m_dCalibCORRange	= dRange;
		break;

	case INSPWHUL_WH2_CALIB_PRU:		
		pCInspOpt->m_dCalibCORRange	= dRange;
		break;
	}

	HMI_dCalibCORRange = dRange;

	return 0;
}

LONG CWinEagle::HMI_SetTestCORAng(IPC_CServiceMessage &svMsg)
{
	DOUBLE dAng;
	svMsg.GetMsg(sizeof(DOUBLE), &dAng);

	HMI_dTestCORAng = dAng;

	return 0;
}


LONG CWinEagle::HMI_AutoCal(IPC_CServiceMessage &svMsg)
{	
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	BOOL bResult = TRUE;
	PRU *pPRU;
	CString strMsg;
	LONG lAnswer = -1;

	pPRU = &SelectCurPRU(HMI_ulCurPRU);

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pPRU->eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
			if (!pCWH1->AutoCalibration(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPDL2_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pPRU->eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
			if (!pCWH1->AutoCalibration(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPDL1_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pPRU->eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
			if (!pCWH2->AutoCalibration(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPDL2_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pPRU->eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
			if (!pCWH2->AutoCalibration(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPWHUL_WH1_CALIB_PRU:		
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInspOpt->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pPRU->eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH1;
			if (!pCInspOpt->AutoCalibration(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPWHUL_WH2_CALIB_PRU:		
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}	
		else if (!pCInspOpt->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pPRU->eCamMoveGroupCurrent = CMG_INSPUL_XINSPOPT_YWH2;
			if (!pCInspOpt->AutoCalibration(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	}

	// Set Calib PRU First
	if (!bResult)
	{
		pPRU->bCalibrated = FALSE;
	}
	else
	{
		pPRU->bCalibrated = TRUE;
	}

	//Set bCalibrated for all relative this pPRU

	for (INT i = 1; i < GetmyPRUList().GetSize(); i++)
	{
		PRU *pTempPRU = GetmyPRUList().GetAt(i);
		if (pTempPRU->eCamMoveGroup1 == pPRU->eCamMoveGroupCurrent) //first abcd
		{
			if (!bResult)
			{
				pTempPRU->bCalibrated	= FALSE;
			}
			else
			{
				pTempPRU->bCalibrated	= TRUE;

				pTempPRU->m2p_xform.a	= pPRU->m2p_xform.a;
				pTempPRU->m2p_xform.b	= pPRU->m2p_xform.b;
				pTempPRU->m2p_xform.c	= pPRU->m2p_xform.c;
				pTempPRU->m2p_xform.d	= pPRU->m2p_xform.d;

				pTempPRU->p2m_xform.a	= pPRU->p2m_xform.a;
				pTempPRU->p2m_xform.b	= pPRU->p2m_xform.b;
				pTempPRU->p2m_xform.c	= pPRU->p2m_xform.c;
				pTempPRU->p2m_xform.d	= pPRU->p2m_xform.d;
			}
		}
		else if (pTempPRU->eCamMoveGroup2 == pPRU->eCamMoveGroupCurrent)
		{
			if (!bResult)
			{
				pTempPRU->bCalibrated_2	= FALSE;
			}
			else
			{
				pTempPRU->bCalibrated_2	= TRUE;

				pTempPRU->m2p_xform_2.a	= pPRU->m2p_xform_2.a;
				pTempPRU->m2p_xform_2.b	= pPRU->m2p_xform_2.b;
				pTempPRU->m2p_xform_2.c	= pPRU->m2p_xform_2.c;
				pTempPRU->m2p_xform_2.d	= pPRU->m2p_xform_2.d;

				pTempPRU->p2m_xform_2.a	= pPRU->p2m_xform_2.a;
				pTempPRU->p2m_xform_2.b	= pPRU->p2m_xform_2.b;
				pTempPRU->p2m_xform_2.c	= pPRU->p2m_xform_2.c;
				pTempPRU->p2m_xform_2.d	= pPRU->p2m_xform_2.d;
			}
		}
	}

	for (INT i = 0; i < GetmyCalibPRUList().GetSize(); i++)
	{
		PRU *pTempPRU = GetmyCalibPRUList().GetAt(i);
		if (pTempPRU->eCamMoveGroup1 == pPRU->eCamMoveGroupCurrent) //first abcd
		{
			if (!bResult)
			{
				pTempPRU->bCalibrated	= FALSE;
			}
			else
			{
				pTempPRU->bCalibrated	= TRUE;

				pTempPRU->m2p_xform.a	= pPRU->m2p_xform.a;
				pTempPRU->m2p_xform.b	= pPRU->m2p_xform.b;
				pTempPRU->m2p_xform.c	= pPRU->m2p_xform.c;
				pTempPRU->m2p_xform.d	= pPRU->m2p_xform.d;

				pTempPRU->p2m_xform.a	= pPRU->p2m_xform.a;
				pTempPRU->p2m_xform.b	= pPRU->p2m_xform.b;
				pTempPRU->p2m_xform.c	= pPRU->p2m_xform.c;
				pTempPRU->p2m_xform.d	= pPRU->p2m_xform.d;
			}
		}
		else if (pTempPRU->eCamMoveGroup2 == pPRU->eCamMoveGroupCurrent)
		{
			if (!bResult)
			{
				pTempPRU->bCalibrated_2	= FALSE;
			}
			else
			{
				pTempPRU->bCalibrated_2	= TRUE;

				pTempPRU->m2p_xform_2.a	= pPRU->m2p_xform_2.a;
				pTempPRU->m2p_xform_2.b	= pPRU->m2p_xform_2.b;
				pTempPRU->m2p_xform_2.c	= pPRU->m2p_xform_2.c;
				pTempPRU->m2p_xform_2.d	= pPRU->m2p_xform_2.d;

				pTempPRU->p2m_xform_2.a	= pPRU->p2m_xform_2.a;
				pTempPRU->p2m_xform_2.b	= pPRU->p2m_xform_2.b;
				pTempPRU->p2m_xform_2.c	= pPRU->p2m_xform_2.c;
				pTempPRU->p2m_xform_2.d	= pPRU->p2m_xform_2.d;
			}
		}
	}

	if (bResult)
	{
		HMIMessageBox(MSG_OK, "AUTO CAL", "OPERATION SUCCESS");
	}
	else
	{
		HMIMessageBox(MSG_OK, "AUTO CAL", "OPERATION FAIL");
	}

	UpdateHMICalibPRButton(FALSE);

	BOOL bCalInProgress = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bCalInProgress);


	return 1;
}

LONG CWinEagle::HMI_CORCal(IPC_CServiceMessage &svMsg)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	BOOL bCalInProgress = TRUE;
	LONG lAnswer = rMSG_TIMEOUT;
	BOOL bResult = TRUE;
	CString strMsg;
	lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Operation is irreversible. Continue?");

	if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT)
	{
		svMsg.InitMessage(sizeof(BOOL), &bCalInProgress);
		bCalInProgress = FALSE;
		UpdateHMICalibPRButton(FALSE);
		return 1;
	}

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pruINSP_DL_Calib[GLASS1][WH_1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
			pCWH1->m_lCORCamNum = INSPDL1_COR;
			if (!pCWH1->CORCalibration(&pruINSP_DL_Calib[GLASS1][WH_1], 0.2, FALSE, TRUE))
			{
				bResult = FALSE;
			}
			//else
			//{
			//	DOUBLE dMaxX = 0.0, dMaxY = 0.0, dMinX = 0.0, dMinY = 0.0;
			//	pCINWH->FindCORResultRange(&dMaxX, &dMinX, &dMaxY, &dMinY);
			//	strMsg.Format("COR X Max:%.2f Min:%.2f Range:%.2f", dMaxX, dMinX, fabs(dMaxX - dMinX));
			//	WriteHMIMess(strMsg);
			//	strMsg.Format("COR Y Max:%.2f Min:%.2f Range:%.2f", dMaxY, dMinY, fabs(dMaxY - dMinY));
			//	WriteHMIMess(strMsg);
			//}
		}
		break;
		
	case INSPDL2_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pruINSP_DL_Calib[GLASS2][WH_1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
			pCWH1->m_lCORCamNum = INSPDL2_COR;
			if (!pCWH1->CORCalibration(&pruINSP_DL_Calib[GLASS2][WH_1], 0.2, FALSE, TRUE))
			{
				bResult = FALSE;
			}
			//else
			//{
			//	DOUBLE dMaxX = 0.0, dMaxY = 0.0, dMinX = 0.0, dMinY = 0.0;
			//	pCINWH->FindCORResultRange(&dMaxX, &dMinX, &dMaxY, &dMinY);
			//	strMsg.Format("COR X Max:%.2f Min:%.2f Range:%.2f", dMaxX, dMinX, fabs(dMaxX - dMinX));
			//	WriteHMIMess(strMsg);
			//	strMsg.Format("COR Y Max:%.2f Min:%.2f Range:%.2f", dMaxY, dMinY, fabs(dMaxY - dMinY));
			//	WriteHMIMess(strMsg);
			//}
		}
		break;
		
	case INSPDL1_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pruINSP_DL_Calib[GLASS1][WH_2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
			pCWH2->m_lCORCamNum = INSPDL1_COR;
			if (!pCWH2->CORCalibration(&pruINSP_DL_Calib[GLASS1][WH_2], 0.2, FALSE, TRUE))
			{
				bResult = FALSE;
			}
			//else
			//{
			//	DOUBLE dMaxX = 0.0, dMaxY = 0.0, dMinX = 0.0, dMinY = 0.0;
			//	pCINWH->FindCORResultRange(&dMaxX, &dMinX, &dMaxY, &dMinY);
			//	strMsg.Format("COR X Max:%.2f Min:%.2f Range:%.2f", dMaxX, dMinX, fabs(dMaxX - dMinX));
			//	WriteHMIMess(strMsg);
			//	strMsg.Format("COR Y Max:%.2f Min:%.2f Range:%.2f", dMaxY, dMinY, fabs(dMaxY - dMinY));
			//	WriteHMIMess(strMsg);
			//}
		}
		break;
		
	case INSPDL2_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pruINSP_DL_Calib[GLASS2][WH_2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
			pCWH2->m_lCORCamNum = INSPDL2_COR;
			if (!pCWH2->CORCalibration(&pruINSP_DL_Calib[GLASS2][WH_2], 0.2, FALSE, TRUE))
			{
				bResult = FALSE;
			}
			//else
			//{
			//	DOUBLE dMaxX = 0.0, dMaxY = 0.0, dMinX = 0.0, dMinY = 0.0;
			//	pCINWH->FindCORResultRange(&dMaxX, &dMinX, &dMaxY, &dMinY);
			//	strMsg.Format("COR X Max:%.2f Min:%.2f Range:%.2f", dMaxX, dMinX, fabs(dMaxX - dMinX));
			//	WriteHMIMess(strMsg);
			//	strMsg.Format("COR Y Max:%.2f Min:%.2f Range:%.2f", dMaxY, dMinY, fabs(dMaxY - dMinY));
			//	WriteHMIMess(strMsg);
			//}
		}
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInspOpt->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pCInspOpt->m_lCORCamNum = INSPDL1_COR;
			if (!pCInspOpt->CORCalibration(&pruINSPWH_UL_Calib[WH_1], 0.2))
			{
				bResult = FALSE;
			}
			//else
			//{
			//	DOUBLE dMaxX = 0.0, dMaxY = 0.0, dMinX = 0.0, dMinY = 0.0;
			//	pCINWH->FindCORResultRange(&dMaxX, &dMinX, &dMaxY, &dMinY);
			//	strMsg.Format("COR X Max:%.2f Min:%.2f Range:%.2f", dMaxX, dMinX, fabs(dMaxX - dMinX));
			//	WriteHMIMess(strMsg);
			//	strMsg.Format("COR Y Max:%.2f Min:%.2f Range:%.2f", dMaxY, dMinY, fabs(dMaxY - dMinY));
			//	WriteHMIMess(strMsg);
			//}
		}
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInspOpt->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			pCInspOpt->m_lCORCamNum = INSPDL2_COR;
			if (!pCInspOpt->CORCalibration(&pruINSPWH_UL_Calib[WH_2], 0.2))
			{
				bResult = FALSE;
			}
			//else
			//{
			//	DOUBLE dMaxX = 0.0, dMaxY = 0.0, dMinX = 0.0, dMinY = 0.0;
			//	pCINWH->FindCORResultRange(&dMaxX, &dMinX, &dMaxY, &dMinY);
			//	strMsg.Format("COR X Max:%.2f Min:%.2f Range:%.2f", dMaxX, dMinX, fabs(dMaxX - dMinX));
			//	WriteHMIMess(strMsg);
			//	strMsg.Format("COR Y Max:%.2f Min:%.2f Range:%.2f", dMaxY, dMinY, fabs(dMaxY - dMinY));
			//	WriteHMIMess(strMsg);
			//}
		}
		break;
	}

	if (bResult)
	{
		HMIMessageBox(MSG_OK, "AUTO CAL", "OPERATION SUCCESS");
	}
	else
	{
		//HMIMessageBox(MSG_OK, "AUTO CAL", "OPERATION FAIL");
	}
	svMsg.InitMessage(sizeof(BOOL), &bCalInProgress);

	UpdateHMICalibPRButton(FALSE);

	return 1;
}

LONG CWinEagle::HMI_CheckPRCentre(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;
	LONG lAnswer = -1;

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCWH1->CheckPRCentre(&pruINSP_DL_Calib[GLASS1][WH_1]);
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCWH1->CheckPRCentre(&pruINSP_DL_Calib[GLASS2][WH_1]);
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCWH2->CheckPRCentre(&pruINSP_DL_Calib[GLASS1][WH_2]);
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCWH2->CheckPRCentre(&pruINSP_DL_Calib[GLASS2][WH_2]);
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCInspOpt->CheckPRCentre(&pruINSPWH_UL_Calib[WH_1]);
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCInspOpt->CheckPRCentre(&pruINSPWH_UL_Calib[WH_2]);
		break;
	}

	return 0;
}

LONG CWinEagle::HMI_SetCORCentre(IPC_CServiceMessage &svMsg)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;
	CString strMsg(" ");

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		break;

	case INSPDL2_WH1_CALIB_PRU:
		break;

	case INSPDL1_WH2_CALIB_PRU:
		break;

	case INSPDL2_WH2_CALIB_PRU:
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		break;
	}

	return 0;
}

LONG CWinEagle::HMI_CheckCORCentre(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;
	CString strMsg(" ");

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pruINSP_DL_Calib[GLASS1][WH_1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pCWH1->m_lCORCamNum = INSPDL1_COR;
		pCWH1->CheckCORCentre(&pruINSP_DL_Calib[GLASS1][WH_1], HMI_dTestCORAng, FALSE, TRUE);
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pruINSP_DL_Calib[GLASS2][WH_1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
		pCWH1->m_lCORCamNum = INSPDL1_COR;
		pCWH1->CheckCORCentre(&pruINSP_DL_Calib[GLASS2][WH_1], HMI_dTestCORAng, FALSE, TRUE);
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pruINSP_DL_Calib[GLASS1][WH_2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pCWH2->m_lCORCamNum = INSPDL1_COR;
		pCWH2->CheckCORCentre(&pruINSP_DL_Calib[GLASS1][WH_2], HMI_dTestCORAng, FALSE, TRUE);
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pruINSP_DL_Calib[GLASS2][WH_2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
		pCWH2->m_lCORCamNum = INSPDL2_COR;
		pCWH2->CheckCORCentre(&pruINSP_DL_Calib[GLASS2][WH_2], HMI_dTestCORAng, FALSE, TRUE);
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCInspOpt->CheckCORCentre(&pruINSPWH_UL_Calib[WH_1], HMI_dTestCORAng);
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCInspOpt->CheckCORCentre(&pruINSPWH_UL_Calib[WH_2], HMI_dTestCORAng);
		break;
	}

	return 0;
}

LONG CWinEagle::HMI_GetPRCentreMap(IPC_CServiceMessage &svMsg)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);

	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;

	if (g_ulUserGroup < SERVICE)
	{
		HMIMessageBox(MSG_OK, "WARNING", "YOU ARE NOT AUTHORIZED TO PERFORM THIS OPERATION!");	
	}
	else
	{
		switch (HMI_ulCurPRU)
		{
		case INSPDL1_WH1_CALIB_PRU:
			break;

		case INSPDL2_WH1_CALIB_PRU:
			break;

		case INSPDL1_WH2_CALIB_PRU:
			break;

		case INSPDL2_WH2_CALIB_PRU:
			break;

		case INSPWHUL_WH1_CALIB_PRU:
			break;

		case INSPWHUL_WH2_CALIB_PRU:
			break;
		}
	}

	HMIMessageBox(MSG_OK, "WARNING", "OPERATION TERMINATED!");

	return 0;
}

//Digital Zoom
LONG CWinEagle::HMI_ZoomIn(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle = (CWinEagle*)m_pModule->GetStation("WinEagle");

	g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
	g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;

	if (pCWinEagle->m_lCurrZoomFactor != PR_ZOOM_FACTOR_8X)
	{
		if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_1X)
		{
			pCWinEagle->m_lCurrZoomFactor = PR_ZOOM_FACTOR_2X;
		}
		else if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_2X)
		{
			pCWinEagle->m_lCurrZoomFactor = PR_ZOOM_FACTOR_4X;
		}
		else if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_4X)
		{
			pCWinEagle->m_lCurrZoomFactor = PR_ZOOM_FACTOR_8X;
		}

		PRS_DigitalZoom(nCurrentCameraID, pCWinEagle->m_lCurrZoomFactor, ZoomCentre);
		PRS_DrawCross(nCurrentCameraID, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);
	
		if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_2X)
		{
			PRS_DisplayText(nCurrentCameraID, 2, 0, "Cam: 2X Image");
		}
		else if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_4X)
		{
			PRS_DisplayText(nCurrentCameraID, 2, 0, "Cam: 4X Image");
		}
		else if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_8X)
		{
			PRS_DisplayText(nCurrentCameraID, 2, 0, "Cam: 8X Image");
		}

		HMI_bShowCursorControl = TRUE;

	}
	
	switch (g_ulWizardStep)
	{
	case 2:
		PRS_DisplayText(nCurrentCameraID, 1, PRS_MSG_ROW1, "Please Set Align Pt 1");
		break;

	case 3:
		PRS_DisplayText(nCurrentCameraID, 1, PRS_MSG_ROW1, "Please Set Align Pt 2");
		break;
	}

	return 0;
}

LONG CWinEagle::HMI_ZoomOut(IPC_CServiceMessage &svMsg)
{
	CWinEagle *pCWinEagle = (CWinEagle*)m_pModule->GetStation("WinEagle");

	g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
	g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
	
	if (pCWinEagle->m_lCurrZoomFactor != PR_ZOOM_FACTOR_2X && pCWinEagle->m_lCurrZoomFactor != PR_ZOOM_FACTOR_1X)
	{
		if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_8X)
		{
			pCWinEagle->m_lCurrZoomFactor = PR_ZOOM_FACTOR_4X;
		}
		else if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_4X)
		{
			pCWinEagle->m_lCurrZoomFactor = PR_ZOOM_FACTOR_2X;
		}
	
		PRS_DigitalZoom(nCurrentCameraID, pCWinEagle->m_lCurrZoomFactor, ZoomCentre);
		PRS_DrawCross(nCurrentCameraID, g_stCursor.prCoord, g_stCursor.lSize, g_stCursor.lColor);

		if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_4X)
		{
			PRS_DisplayText(nCurrentCameraID, 2, 0, "Cam: 4X Image");
		}
		else if (pCWinEagle->m_lCurrZoomFactor == PR_ZOOM_FACTOR_2X)
		{
			PRS_DisplayText(nCurrentCameraID, 2, 0, "Cam: 2X Image");
		}
	}
	else
	{
		if (pCWinEagle->m_lCurrZoomFactor != PR_ZOOM_FACTOR_1X)
		{
			pCWinEagle->m_lCurrZoomFactor = PR_ZOOM_FACTOR_1X;
			PRS_DisableDigitalZoom(nCurrentCameraID);
			PRS_DrawHomeCursor(nCurrentCameraID, FALSE);

			g_stCursor.prCoord.x = PR_DEF_CENTRE_X;
			g_stCursor.prCoord.y = PR_DEF_CENTRE_Y;
		}
	}

	
	switch (g_ulWizardStep)
	{
	case 2:
		PRS_DisplayText(nCurrentCameraID, 1, PRS_MSG_ROW1, "Please Set Align Pt 1");
		break;

	case 3:
		PRS_DisplayText(nCurrentCameraID, 1, PRS_MSG_ROW1, "Please Set Align Pt 2");
		break;
	}

	return 0;
}

LONG CWinEagle::HMI_ToggleVAC(IPC_CServiceMessage &svMsg)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");

	// protection
	if (HMI_ulCurPRU >= MAX_NUM_OF_CALIB_PRU)
	{
		HMI_ulCurPRU = INSPDL1_WH1_CALIB_PRU;
	}

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCWH1->HMI_ToggleG1VacSol(svMsg);
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCWH1->HMI_ToggleG2VacSol(svMsg);
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCWH2->HMI_ToggleG1VacSol(svMsg);
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCWH2->HMI_ToggleG2VacSol(svMsg);
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCWH1->HMI_ToggleG1VacSol(svMsg);
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCWH2->HMI_ToggleG1VacSol(svMsg);
		break;
	}

	UpdateHMICalibPRButton(FALSE);
	return 0;
}

LONG CWinEagle::HMI_ResetIncludedAngle(IPC_CServiceMessage &svMsg)
{
	PRU *pPRU;

	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CPickArm *pCPickArm = (CPickArm*)m_pModule->GetStation("PickArm");

	pPRU = &SelectCurPRU(HMI_ulCurPRU);


	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		break;

	case INSPDL2_WH1_CALIB_PRU:
		break;

	case INSPDL1_WH2_CALIB_PRU:
		break;

	case INSPDL2_WH2_CALIB_PRU:
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		break;
	}
	UpdateHMICalibPRButton(FALSE);
	return 0;
}

LONG CWinEagle::HMI_SetUserSearchPRContinue(IPC_CServiceMessage &svMsg)	// 20140815 Yip
{
	m_bUserSearchPRDone = TRUE;
	g_bShowUserSearchPRDone = FALSE;
	return 0;
}

LONG CWinEagle::HMI_SetUserRejectPRTask(IPC_CServiceMessage &svMsg)	// 20140815 Yip
{
	m_bUserRejectPRTask = TRUE;
	g_bShowUserSearchPRDone = FALSE;

	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

VOID CWinEagle::PrintPRCalibSetup(FILE *fp)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str = "";
	
	if (fp != NULL)
	{
		fprintf(fp, "\n<<X-Y Calibration>>\n");

		for (INT i = 0; i < GetmyCalibPRUList().GetSize(); i++) //20130109 CArray is start from zero. myCalibPRUList is start from zero. But myPRUList is start from 1, because there is a dummy item at index zero (pruMenu)
		{
			if ((BOOL)(LONG)pAppMod->m_smfMachine[GetmyCalibPRUList().GetAt(i)->szStnName]["PRU"][GetmyCalibPRUList().GetAt(i)->szFileName]["bCalibrated"])
			{
				fprintf(fp, "\n%s CALIBRATION\n", (const char*)GetmyCalibPRUList().GetAt(i)->szFileName);
			}
			else
			{
				fprintf(fp, "\n%s NOT CALIBRATION\n", (const char*)GetmyCalibPRUList().GetAt(i)->szFileName);
			}
			fprintf(fp, "======================================================\n");
		}
	}
}

VOID CWinEagle::PrintPRSetup(FILE *fp)
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CString str = "";
	
	if (fp != NULL)
	{
		fprintf(fp, "\n<<PR Record>>\n");

		for (INT i = 1; i < GetmyPRUList().GetSize(); i++)
		{
			if ((BOOL)(LONG)pAppMod->m_smfDevice[GetmyPRUList().GetAt(i)->szStnName]["PRU"][GetmyPRUList().GetAt(i)->szFileName]["bLoaded"])
			{
				fprintf(fp, "\n%s\n", (const char*)GetmyPRUList().GetAt(i)->szFileName);
				fprintf(fp, "======================================================\n");
				switch ((LONG)pAppMod->m_smfDevice[GetmyPRUList().GetAt(i)->szStnName]["PRU"][GetmyPRUList().GetAt(i)->szFileName]["emRecordType"])
				{
				case 0:
					fprintf(fp, "Alignment Algorithm		: Pattern Matching");
					break;
				case 1:
					fprintf(fp, "Alignment Algorithm		: Edge Matching");
					break;
				case 2:
					fprintf(fp, "Alignment Algorithm		: Circle(Shape) Fitting");
					break;
				case 3:
					fprintf(fp, "Alignment Algorithm		:Template Fitting");
					break;
				case 4:
					fprintf(fp, "Alignment Algorithm		:Fidiciual Mark");
					break;
				case 5:
					fprintf(fp, "Alignment Algorithm		:Rectangle");
					break;
				}

				fprintf(fp, "%30s\t : %.2f\n",	"Match Score",
						(DOUBLE)pAppMod->m_smfDevice[GetmyPRUList().GetAt(i)->szStnName]["PRU"][GetmyPRUList().GetAt(i)->szFileName]["stSrchDieCmd"]["rMatchScore"]);
				fprintf(fp, "%30s\t : %.2f\n",	"Angle Range",
						(DOUBLE)pAppMod->m_smfDevice[GetmyPRUList().GetAt(i)->szStnName]["PRU"][GetmyPRUList().GetAt(i)->szFileName]["stSrchDieCmd"]["rEndAngle"]);
			}
			else
			{
				fprintf(fp, "\n%s NOT LOADED\n", (LPCTSTR)GetmyPRUList().GetAt(i)->szFileName); //klocwork fix 20121211
			}

		}
	}
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
BOOL CWinEagle::printMachineDeviceInfo()
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;

	for (INT i = 0; i < GetmyCalibPRUList().GetSize(); i++) //20130109 CArray is start from zero. myCalibPRUList is start from zero. But myPRUList is start from 1, because there is a dummy item at index zero (pruMenu)
	{
		printPRPara(GetmyCalibPRUList().GetAt(i)->szStnName, GetmyCalibPRUList().GetAt(i), GetmyCalibPRUList().GetAt(i)->szFileName, TRUE);
	}

	//for(INT i = 1; i<GetmyPRUList().GetSize(); i++) //device
	//{
	//	printPRPara(GetmyPRUList().GetAt(i)->szStnName, GetmyPRUList().GetAt(i), GetmyPRUList().GetAt(i)->szFileName, FALSE);
	//}

	if (bResult)
	{
		bResult = CPI9000Stn::printMachineDeviceInfo();
	}
	return bResult;
}

LONG CWinEagle::printPRPara(CString szStation, PRU *pPRU, CString szPRU, BOOL IsMachineData) // The print excel
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	CStringMapFile *pMap;

	BOOL bResult = TRUE;
	CString szBlank("--");
	CString szModule = GetStnName();
	LONG lValue = 0;
	DOUBLE dValue = 0.0;

	if (IsMachineData)
	{
		pMap = &pAppMod->m_smfMachine;
	}
	else
	{
		pMap = &pAppMod->m_smfDevice;
	}

	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("uwCoaxialLevel"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["uwCoaxialLevel"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("uwCoaxial1Level"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["uwCoaxial1Level"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("uwCoaxial2Level"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["uwCoaxial2Level"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("uwRingLevel"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["uwRingLevel"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("uwRing1Level"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["uwRing1Level"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("uwSideLevel"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["uwSideLevel"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("bLoaded"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["bLoaded"]	
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("bStatLoaded"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["bStatLoaded"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("lPBMethod"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["lPBMethod"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("dPostBdRejectTol"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["dPostBdRejectTol"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("emRecordType"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["emRecordType"]
					   );

	// calibration PRU
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_UPPER_RIGHT].x"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_RIGHT].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_UPPER_RIGHT].y"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_RIGHT].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_UPPER_LEFT].x"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_LEFT].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_UPPER_LEFT].y"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_LEFT].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_LOWER_RIGHT].x"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_RIGHT].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_LOWER_RIGHT].y"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_RIGHT].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_LOWER_LEFT].x"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_LEFT].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("arcoLrnDieCorners[PR_LOWER_LEFT].y"),
						szBlank, szBlank, szBlank, szBlank, szBlank, (LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_LEFT].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("acoDieCorners[PR_UPPER_LEFT].x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_UPPER_LEFT].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("acoDieCorners[PR_UPPER_LEFT].Y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_UPPER_LEFT].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("acoDieCorners[PR_LOWER_RIGHT].x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_LOWER_RIGHT].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("acoDieCorners[PR_LOWER_RIGHT].y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_LOWER_RIGHT].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("emAlignAlg"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emAlignAlg"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[0].coCorner1.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner1.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[0].coCorner1.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner1.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[0].coCorner2.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner2.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[0].coCorner2.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner2.y"]
					   );

	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[1].coCorner1.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner1.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[1].coCorner1.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner1.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[1].coCorner2.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner2.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("astChipWindows[1].coCorner2.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner2.y"]
					   );

	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("leChipIntrusiveLength"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["leChipIntrusiveLength"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("emIsAutoLearnFMarkCenterAngle"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emIsAutoLearnFMarkCenterAngle"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("emFiducialMarkType"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emFiducialMarkType"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stManualLrnDieCmd"),
						CString("ubThreshold"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["ubThreshold"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("emAlignAlg"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["emAlignAlg"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stLargeSrchWin.coCorner1.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner1.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stLargeSrchWin.coCorner1.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner1.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stLargeSrchWin.coCorner2.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner2.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stLargeSrchWin.coCorner2.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner2.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("rMatchScore"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rMatchScore"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("rStartAngle"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rStartAngle"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("rEndAngle"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rEndAngle"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stDieInsp"), CString("stTmpl"), CString("emDefectAttribute"), szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieInsp"]["stTmpl"]["emDefectAttribute"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stDieInsp"), CString("stTmpl"), CString("ubDefectThreshold"), szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieInsp"]["stTmpl"]["ubDefectThreshold"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("coProbableDieCentre.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["coProbableDieCentre.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("coProbableDieCentre.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["coProbableDieCentre.y"]
					   );

	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stDieAlignPar"), CString("ubMaxScale"), szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieAlignPar"]["ubMaxScale"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stDieAlignPar"), CString("ubMinScale"), szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieAlignPar"]["ubMinScale"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stPaLn.emIsCoarseSrch"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.emIsCoarseSrch"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stPaLn.stLargeSrchWin.coCorner1.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner1.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stPaLn.stLargeSrchWin.coCorner1.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner1.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stPaLn.stLargeSrchWin.coCorner2.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner2.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("stPaLn.stLargeSrchWin.coCorner2.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner2.y"]
					   );

	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchDieCmd"),
						CString("rCfmPatternMatchScore"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rCfmPatternMatchScore"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLrnShapeCmd"),
						CString("coCorner1.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner1.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLrnShapeCmd"),
						CString("coCorner1.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner1.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLrnShapeCmd"),
						CString("coCorner2.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner2.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLrnShapeCmd"),
						CString("coCorner2.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner2.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLrnShapeCmd"),
						CString("emObjAttribute"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["emObjAttribute"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("stInspWin.coCorner1.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner1.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("stInspWin.coCorner1.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner1.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("stInspWin.coCorner2.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner2.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("stInspWin.coCorner2.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner2.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("rDetectScore"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rDetectScore"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("rSizePercentVar"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rSizePercentVar"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("rStartAngle"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rStartAngle"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stDetectShapeCmd"),
						CString("rEndAngle"), szBlank, szBlank, szBlank, szBlank, 
						(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rEndAngle"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLoadTmplCmd"),
						CString("coTmplCentre.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["coTmplCentre.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLoadTmplCmd"),
						CString("coTmplCentre.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["coTmplCentre.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLoadTmplCmd"),
						CString("szTmplSize.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["szTmplSize.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLoadTmplCmd"),
						CString("szTmplSize.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["szTmplSize.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stLoadTmplCmd"),
						CString("ubPassScore"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["ubPassScore"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchTmplCmd"),
						CString("stSrchWin.coCorner1.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner1.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchTmplCmd"),
						CString("stSrchWin.coCorner1.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner1.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchTmplCmd"),
						CString("stSrchWin.coCorner2.x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner2.x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stSrchTmplCmd"),
						CString("stSrchWin.coCorner2.y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner2.y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[0].x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[0].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[0].y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[0].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[1].x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[1].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[1].y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[1].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[2].x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[2].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[2].y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[2].y"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[3].x"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[3].x"]
					   );
	printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("stImageStatInfoCmd"),
						CString("acoCorners[3].y"), szBlank, szBlank, szBlank, szBlank, 
						(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[3].y"]
					   );
	if (IsMachineData)
	{
		// calibration data
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("bCalibrated"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(LONG)(*pMap)[szStation]["PRU"][szPRU]["bCalibrated"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("m2p_xform.a"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.a"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("m2p_xform.b"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.b"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("m2p_xform.c"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.c"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("m2p_xform.d"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.d"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("p2m_xform.a"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.a"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("p2m_xform.b"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.b"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("p2m_xform.c"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.c"]
						   );
		printMachineInfoRow(szModule, szStation, CString("PRU"), szPRU, CString("p2m_xform.d"),
							szBlank, szBlank, szBlank, szBlank, szBlank, 
							(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.d"]
						   );
	}
	return 1;
}
#endif

LONG CWinEagle::SavePRPara(CString szStation, PRU *pPRU, CString szPRU, BOOL IsMachineData) // The save file Name
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	
	CStringMapFile *pMap;
	if (IsMachineData)
	{
		pMap = &pAppMod->m_smfMachine;
	}
	else
	{
		pMap = &pAppMod->m_smfDevice;
	}

	// calibration PRU
	(*pMap)[szStation]["PRU"][szPRU]["uwCoaxialLevel"]	= (LONG)pPRU->uwCoaxialLevel;
	(*pMap)[szStation]["PRU"][szPRU]["uwCoaxial1Level"]	= (LONG)pPRU->uwCoaxial1Level;
	(*pMap)[szStation]["PRU"][szPRU]["uwCoaxial2Level"]	= (LONG)pPRU->uwCoaxial2Level;
	(*pMap)[szStation]["PRU"][szPRU]["uwRingLevel"]		= (LONG)pPRU->uwRingLevel;
	(*pMap)[szStation]["PRU"][szPRU]["uwRing1Level"]	= (LONG)pPRU->uwRing1Level;
	(*pMap)[szStation]["PRU"][szPRU]["uwSideLevel"]		= (LONG)pPRU->uwSideLevel;
	(*pMap)[szStation]["PRU"][szPRU]["bLoaded"]			= (BOOL)pPRU->bLoaded;
	(*pMap)[szStation]["PRU"][szPRU]["bStatLoaded"]		= (BOOL)pPRU->bStatLoaded;
	(*pMap)[szStation]["PRU"][szPRU]["lPBMethod"]		= (LONG)pPRU->lPBMethod;

	(*pMap)[szStation]["PRU"][szPRU]["dPostBdRejectTol"] = (DOUBLE)pPRU->dPostBdRejectTol;
	(*pMap)[szStation]["PRU"][szPRU]["emRecordType"]	= (LONG)pPRU->emRecordType;

	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_RIGHT].x"]	= pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].x;
	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_RIGHT].y"]	= pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].y;
	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_LEFT].x"]	= pPRU->arcoLrnDieCorners[PR_UPPER_LEFT].x;
	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_LEFT].y"]	= pPRU->arcoLrnDieCorners[PR_UPPER_LEFT].y;
	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_RIGHT].x"]	= pPRU->arcoLrnDieCorners[PR_LOWER_RIGHT].x;
	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_RIGHT].y"]	= pPRU->arcoLrnDieCorners[PR_LOWER_RIGHT].y;
	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_LEFT].x"]	= pPRU->arcoLrnDieCorners[PR_LOWER_LEFT].x;
	(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_LEFT].y"]	= pPRU->arcoLrnDieCorners[PR_LOWER_LEFT].y;

	(*pMap)[szStation]["PRU"][szPRU]["stSrchCmd"]["uwRecordID"] 			= pPRU->stSrchCmd.uwRecordID;	// Vision UI
	(*pMap)[szStation]["PRU"][szPRU]["stSrchCmd"]["emVisualTask"] 			= pPRU->stSrchCmd.emVisualTask;
	(*pMap)[szStation]["PRU"][szPRU]["stModifyCritCmd"]["uwRecordID"] 		= pPRU->stModifyCritCmd.uwRecordID;
	(*pMap)[szStation]["PRU"][szPRU]["stModifyCritCmd"]["emVisualTask"] 		= pPRU->stModifyCritCmd.emVisualTask;


	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_UPPER_LEFT].x"]	= (LONG)pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_UPPER_LEFT].y"]	= (LONG)pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_LOWER_RIGHT].x"]	= (LONG)pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_LOWER_RIGHT].y"]	= (LONG)pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emAlignAlg"]						= (LONG)pPRU->stManualLrnDieCmd.emAlignAlg;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner1.x"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner1.x;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner1.y"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner1.y;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner2.x"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner2.x;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner2.y"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner2.y;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner1.x"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner1.x;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner1.y"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner1.y;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner2.x"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner2.x;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner2.y"]	= (LONG)pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner2.y;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["leChipIntrusiveLength"]				= (LONG)pPRU->stManualLrnDieCmd.leChipIntrusiveLength;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emIsAutoLearnFMarkCenterAngle"]	= (BOOL)pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle;
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emFiducialMarkType"]				= (LONG)pPRU->stManualLrnDieCmd.emFiducialMarkType;
	//(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["auwRecordID[0]"]						= (LONG)pPRU->stSrchDieCmd.auwRecordID[0];
	(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["ubThreshold"] = (LONG)pPRU->stManualLrnDieCmd.stAlign.stStreet.ubThreshold;

	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["emAlignAlg"]				= (LONG)pPRU->stSrchDieCmd.emAlignAlg;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner1.x"] = (LONG)pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.x;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner1.y"] = (LONG)pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.y;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner2.x"] = (LONG)pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.x;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner2.y"] = (LONG)pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.y;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rMatchScore"]				= (DOUBLE)pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rStartAngle"]				= (DOUBLE)pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rEndAngle"]					= (DOUBLE)pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieInsp"]["stTmpl"]["emDefectAttribute"]	= (LONG)pPRU->stSrchDieCmd.stDieInsp.stTmpl.emDefectAttribute;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieInsp"]["stTmpl"]["ubDefectThreshold"]	= (LONG)pPRU->stSrchDieCmd.stDieInsp.stTmpl.ubDefectThreshold;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["coProbableDieCentre.x"]		= (LONG)pPRU->stSrchDieCmd.coProbableDieCentre.x;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["coProbableDieCentre.y"]		= (LONG)pPRU->stSrchDieCmd.coProbableDieCentre.y;
	
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieAlignPar"]["ubMaxScale"]	= (DOUBLE)pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale; //20130103
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieAlignPar"]["ubMinScale"] = (DOUBLE)pPRU->stSrchDieCmd.stDieAlignPar.ubMinScale;

	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.emIsCoarseSrch"]	= (BOOL)(LONG)pPRU->stSrchDieCmd.stDieAlign.stPaLn.emIsCoarseSrch;	 // new
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner1.x"]	= (LONG)pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner1.x;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner1.y"]	= (LONG)pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner1.y;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner2.x"]	= (LONG)pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner2.x;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner2.y"]	= (LONG)pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner2.y;
	//Confimation Pattern
	(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rCfmPatternMatchScore"]		= (DOUBLE)pPRU->stSrchDieCmd.rCfmPatternMatchScore;


	(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner1.x"]				= (LONG)pPRU->stLrnShapeCmd.stEncRect.coCorner1.x;
	(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner1.y"]				= (LONG)pPRU->stLrnShapeCmd.stEncRect.coCorner1.y;
	(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner2.x"]				= (LONG)pPRU->stLrnShapeCmd.stEncRect.coCorner2.x;
	(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner2.y"]				= (LONG)pPRU->stLrnShapeCmd.stEncRect.coCorner2.y;
	(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["emObjAttribute"]			= (LONG)pPRU->stLrnShapeCmd.emObjAttribute;
	//(*pMap)[szStation]["PRU"][szPRU][szStation"]["stDetectShapeCmd"]["uwRecordID"]			= (LONG)pPRU->stDetectShapeCmd.uwRecordID;
	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner1.x"]	= (LONG)pPRU->stDetectShapeCmd.stInspWin.coCorner1.x;
	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner1.y"]	= (LONG)pPRU->stDetectShapeCmd.stInspWin.coCorner1.y;
	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner2.x"]	= (LONG)pPRU->stDetectShapeCmd.stInspWin.coCorner2.x;
	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner2.y"]	= (LONG)pPRU->stDetectShapeCmd.stInspWin.coCorner2.y;
	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rDetectScore"]			= (DOUBLE)pPRU->stDetectShapeCmd.rDetectScore;
	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rSizePercentVar"]			= (DOUBLE)pPRU->stDetectShapeCmd.rSizePercentVar;

	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rStartAngle"]			= (DOUBLE)pPRU->stDetectShapeCmd.rStartAngle;
	(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rEndAngle"]			= (DOUBLE)pPRU->stDetectShapeCmd.rEndAngle;

	(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["coTmplCentre.x"]			= (LONG)pPRU->stLoadTmplCmd.coTmplCentre.x;
	(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["coTmplCentre.y"]			= (LONG)pPRU->stLoadTmplCmd.coTmplCentre.y;
	(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["szTmplSize.x"]				= (LONG)pPRU->stLoadTmplCmd.szTmplSize.x;
	(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["szTmplSize.y"]				= (LONG)pPRU->stLoadTmplCmd.szTmplSize.y;
	//(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["uwRecordID"]				= (LONG)pPRU->stSrchTmplCmd.uwRecordID;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["ubPassScore"]				= (LONG)pPRU->stSrchTmplCmd.ubPassScore;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner1.x"]	= (LONG)pPRU->stSrchTmplCmd.stSrchWin.coCorner1.x;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner1.y"]	= (LONG)pPRU->stSrchTmplCmd.stSrchWin.coCorner1.y;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner2.x"]	= (LONG)pPRU->stSrchTmplCmd.stSrchWin.coCorner2.x;
	(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner2.y"]	= (LONG)pPRU->stSrchTmplCmd.stSrchWin.coCorner2.y;

	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[0].x"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[0].x;
	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[0].y"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[0].y;
	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[1].x"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[1].x;
	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[1].y"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[1].y;
	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[2].x"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[2].x;
	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[2].y"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[2].y;
	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[3].x"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[3].x;
	(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[3].y"]	= (LONG)pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[3].y;


	
	if (IsMachineData)
	{
		// calibration data
		(*pMap)[szStation]["PRU"][szPRU]["bCalibrated"]	= pPRU->bCalibrated;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.a"]	= pPRU->m2p_xform.a;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.b"]	= pPRU->m2p_xform.b;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.c"]	= pPRU->m2p_xform.c;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.d"]	= pPRU->m2p_xform.d;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.a"]	= pPRU->p2m_xform.a;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.b"]	= pPRU->p2m_xform.b;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.c"]	= pPRU->p2m_xform.c;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.d"]	= pPRU->p2m_xform.d;
#if 1 //20140416
		(*pMap)[szStation]["PRU"][szPRU]["bCalibrated_2"]	= pPRU->bCalibrated_2;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.a"]	= pPRU->m2p_xform_2.a;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.b"]	= pPRU->m2p_xform_2.b;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.c"]	= pPRU->m2p_xform_2.c;
		(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.d"]	= pPRU->m2p_xform_2.d;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.a"]	= pPRU->p2m_xform_2.a;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.b"]	= pPRU->p2m_xform_2.b;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.c"]	= pPRU->p2m_xform_2.c;
		(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.d"]	= pPRU->p2m_xform_2.d;
#endif
	}

	return 1;
}

LONG CWinEagle::LoadPRPara(CString szStation, PRU *pPRU, CString szPRU, BOOL IsMachineData)  // using the wizard station
{
	CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
		
	CStringMapFile *pMap;
	if (IsMachineData)
	{
		pMap = &pAppMod->m_smfMachine;
	}
	else
	{
		pMap = &pAppMod->m_smfDevice;
	}

	// calibration PRU
	pPRU->uwCoaxialLevel		= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["uwCoaxialLevel"];
	pPRU->uwCoaxial1Level		= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["uwCoaxial1Level"];
	pPRU->uwCoaxial2Level		= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["uwCoaxial2Level"];
	pPRU->uwRingLevel			= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["uwRingLevel"];
	pPRU->uwRing1Level			= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["uwRing1Level"];
	pPRU->uwSideLevel			= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["uwSideLevel"];

	pPRU->bLoaded				= (BOOL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["bLoaded"];
	pPRU->bStatLoaded			= (BOOL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["bStatLoaded"];
	pPRU->emRecordType			= (PR_RECORD_TYPE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["emRecordType"];
	pPRU->dPostBdRejectTol		= (DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["dPostBdRejectTol"];
	pPRU->lPBMethod				= (LONG)(*pMap)[szStation]["PRU"][szPRU]["lPBMethod"];

	pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].x	= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_RIGHT].x"];
	pPRU->arcoLrnDieCorners[PR_UPPER_RIGHT].y	= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_RIGHT].y"];
	pPRU->arcoLrnDieCorners[PR_UPPER_LEFT].x		= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_LEFT].x"];
	pPRU->arcoLrnDieCorners[PR_UPPER_LEFT].y		= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_UPPER_LEFT].y"];
	pPRU->arcoLrnDieCorners[PR_LOWER_RIGHT].x	= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_RIGHT].x"];
	pPRU->arcoLrnDieCorners[PR_LOWER_RIGHT].y	= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_RIGHT].y"];
	pPRU->arcoLrnDieCorners[PR_LOWER_LEFT].x		= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_LEFT].x"];
	pPRU->arcoLrnDieCorners[PR_LOWER_LEFT].y		= (PR_REAL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["arcoLrnDieCorners[PR_LOWER_LEFT].y"];

	pPRU->stSrchCmd.uwRecordID				= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchCmd"]["uwRecordID"];	// Vision UI
	pPRU->stSrchCmd.emVisualTask			= (PR_VISUAL_TASK)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchCmd"]["emVisualTask"];
	pPRU->stModifyCritCmd.uwRecordID		= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stModifyCritCmd"]["uwRecordID"];	// Vision UI
	pPRU->stModifyCritCmd.emVisualTask      = (PR_VISUAL_TASK)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stModifyCritCmd"]["emVisualTask"];

	pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_UPPER_LEFT].x"];
	pPRU->stManualLrnDieCmd.acoDieCorners[PR_UPPER_LEFT].y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_UPPER_LEFT].y"];
	pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_LOWER_RIGHT].x"];
	pPRU->stManualLrnDieCmd.acoDieCorners[PR_LOWER_RIGHT].y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["acoDieCorners[PR_LOWER_RIGHT].y"];
	pPRU->stManualLrnDieCmd.emAlignAlg						= (PR_DIE_ALIGN_ALG)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emAlignAlg"];
	pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner1.x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner1.x"];
	pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner1.y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner1.y"];
	pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner2.x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner2.x"];
	pPRU->stManualLrnDieCmd.astChipWindows[0].coCorner2.y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[0].coCorner2.y"];
	pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner1.x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner1.x"];
	pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner1.y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner1.y"];
	pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner2.x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner2.x"];
	pPRU->stManualLrnDieCmd.astChipWindows[1].coCorner2.y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["astChipWindows[1].coCorner2.y"];
	pPRU->stManualLrnDieCmd.leChipIntrusiveLength			= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["leChipIntrusiveLength"];
	pPRU->stManualLrnDieCmd.emIsAutoLearnFMarkCenterAngle	= (PR_BOOLEAN)(BOOL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emIsAutoLearnFMarkCenterAngle"];
	pPRU->stManualLrnDieCmd.emFiducialMarkType				= (PR_FIDUCIAL_MARK_TYPE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["emFiducialMarkType"];
	//pPRU->stSrchDieCmd.auwRecordID[0]						= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["auwRecordID[0]"];
	pPRU->stManualLrnDieCmd.stAlign.stStreet.ubThreshold	= (PR_UBYTE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stManualLrnDieCmd"]["ubThreshold"];

	pPRU->stSrchDieCmd.emAlignAlg										= (PR_DIE_ALIGN_ALG)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["emAlignAlg"];
	pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner1.x"];
	pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner1.y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner1.y"];
	pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.x	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner2.x"];
	pPRU->stSrchDieCmd.stDieAlign.stStreet.stLargeSrchWin.coCorner2.y	= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stLargeSrchWin.coCorner2.y"];
	pPRU->stSrchDieCmd.stDieAlignPar.rMatchScore						= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rMatchScore"];
	pPRU->stSrchDieCmd.stDieAlignPar.rStartAngle						= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rStartAngle"];
	pPRU->stSrchDieCmd.stDieAlignPar.rEndAngle							= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rEndAngle"];
	pPRU->stSrchDieCmd.stDieInsp.stTmpl.emDefectAttribute				= (PR_DEFECT_ATTRIBUTE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieInsp"]["stTmpl"]["emDefectAttribute"];
	pPRU->stSrchDieCmd.stDieInsp.stTmpl.ubDefectThreshold				= (PR_UBYTE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieInsp"]["stTmpl"]["ubDefectThreshold"];
	pPRU->stSrchDieCmd.coProbableDieCentre.x							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["coProbableDieCentre.x"];
	pPRU->stSrchDieCmd.coProbableDieCentre.y							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["coProbableDieCentre.y"];
	
	pPRU->stSrchDieCmd.stDieAlignPar.ubMaxScale							= (PR_UBYTE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieAlignPar"]["ubMaxScale"];
	pPRU->stSrchDieCmd.stDieAlignPar.ubMinScale							= (PR_UBYTE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stDieAlignPar"]["ubMinScale"];

	pPRU->stSrchDieCmd.stDieAlign.stPaLn.emIsCoarseSrch					= (PR_BOOLEAN)(BOOL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.emIsCoarseSrch"];
	pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner1.x		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner1.x"];
	pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner1.y		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner1.y"];
	pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner2.x		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner2.x"];
	pPRU->stSrchDieCmd.stDieAlign.stPaLn.stLargeSrchWin.coCorner2.y		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["stPaLn.stLargeSrchWin.coCorner2.y"];
	//Confimation Pattern
	pPRU->stSrchDieCmd.rCfmPatternMatchScore							= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchDieCmd"]["rCfmPatternMatchScore"];


	pPRU->stLrnShapeCmd.stEncRect.coCorner1.x							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner1.x"];
	pPRU->stLrnShapeCmd.stEncRect.coCorner1.y							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner1.y"];
	pPRU->stLrnShapeCmd.stEncRect.coCorner2.x							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner2.x"];
	pPRU->stLrnShapeCmd.stEncRect.coCorner2.y							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["coCorner2.y"];
	pPRU->stLrnShapeCmd.emObjAttribute									= (PR_OBJ_ATTRIBUTE)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLrnShapeCmd"]["emObjAttribute"];
	//pPRU->stDetectShapeCmd.uwRecordID									= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["uwRecordID"];
	pPRU->stDetectShapeCmd.stInspWin.coCorner1.x						= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner1.x"];
	pPRU->stDetectShapeCmd.stInspWin.coCorner1.y						= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner1.y"];
	pPRU->stDetectShapeCmd.stInspWin.coCorner2.x						= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner2.x"];
	pPRU->stDetectShapeCmd.stInspWin.coCorner2.y						= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["stInspWin.coCorner2.y"];
	pPRU->stDetectShapeCmd.rDetectScore									= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rDetectScore"];
	pPRU->stDetectShapeCmd.rSizePercentVar								= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rSizePercentVar"];
#if 1 //20130103
	PR_REAL rTmpStartAngle, rTmpEndAngle;
	rTmpStartAngle = pPRU->stDetectShapeCmd.rStartAngle;
	rTmpEndAngle = pPRU->stDetectShapeCmd.rEndAngle;
	pPRU->stDetectShapeCmd.rStartAngle							= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rStartAngle"];
	pPRU->stDetectShapeCmd.rEndAngle							= (PR_REAL)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stDetectShapeCmd"]["rEndAngle"];
	if (pPRU->stDetectShapeCmd.rStartAngle == 0.0 && pPRU->stDetectShapeCmd.rEndAngle == 0.0)
	{
		pPRU->stDetectShapeCmd.rStartAngle					= rTmpStartAngle;
		pPRU->stDetectShapeCmd.rEndAngle					= rTmpEndAngle;
	}
#endif

	pPRU->stLoadTmplCmd.coTmplCentre.x									= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["coTmplCentre.x"];
	pPRU->stLoadTmplCmd.coTmplCentre.y									= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["coTmplCentre.y"];
	pPRU->stLoadTmplCmd.szTmplSize.x									= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["szTmplSize.x"];
	pPRU->stLoadTmplCmd.szTmplSize.y									= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stLoadTmplCmd"]["szTmplSize.y"];
	//pPRU->stSrchTmplCmd.uwRecordID									= (PR_UWORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["uwRecordID"];
	pPRU->stSrchTmplCmd.ubPassScore										= (PR_UBYTE)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["ubPassScore"];
	pPRU->stSrchTmplCmd.stSrchWin.coCorner1.x							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner1.x"];
	pPRU->stSrchTmplCmd.stSrchWin.coCorner1.y							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner1.y"];
	pPRU->stSrchTmplCmd.stSrchWin.coCorner2.x							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner2.x"];
	pPRU->stSrchTmplCmd.stSrchWin.coCorner2.y							= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stSrchTmplCmd"]["stSrchWin.coCorner2.y"];

	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[0].x		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[0].x"];
	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[0].y		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[0].y"];
	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[1].x		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[1].x"];
	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[1].y		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[1].y"];
	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[2].x		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[2].x"];
	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[2].y		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[2].y"];
	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[3].x		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[3].x"];
	pPRU->stImageStatInfoCmd.stDetectIgnoreRegion.astDetectRegion[0].acoCorners[3].y		= (PR_WORD)(LONG)(*pMap)[szStation]["PRU"][szPRU]["stImageStatInfoCmd"]["acoCorners[3].y"];


	if (IsMachineData)
	{
		// calibration data
		pPRU->bCalibrated		= (BOOL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["bCalibrated"];
		pPRU->m2p_xform.a		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.a"];
		pPRU->m2p_xform.b		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.b"];
		pPRU->m2p_xform.c		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.c"];
		pPRU->m2p_xform.d		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform.d"];
		pPRU->p2m_xform.a		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.a"];
		pPRU->p2m_xform.b		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.b"];
		pPRU->p2m_xform.c		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.c"];
		pPRU->p2m_xform.d		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform.d"];
#if 1 //20140416
		pPRU->bCalibrated_2		= (BOOL)(LONG)(*pMap)[szStation]["PRU"][szPRU]["bCalibrated_2"];
		pPRU->m2p_xform_2.a		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.a"];
		pPRU->m2p_xform_2.b		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.b"];
		pPRU->m2p_xform_2.c		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.c"];
		pPRU->m2p_xform_2.d		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["m2p_xform_2.d"];
		pPRU->p2m_xform_2.a		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.a"];
		pPRU->p2m_xform_2.b		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.b"];
		pPRU->p2m_xform_2.c		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.c"];
		pPRU->p2m_xform_2.d		= (FLOAT)(DOUBLE)(*pMap)[szStation]["PRU"][szPRU]["p2m_xform_2.d"];
#endif
	}

	return 1;
}

LONG CWinEagle::IPC_SaveMachineParam()
{
	//for(INT i = 1; i<GetmyCalibPRUList().GetSize(); i++)
	for (INT i = 0; i < GetmyCalibPRUList().GetSize(); i++) //20130109 CArray is start from zero. myCalibPRUList is start from zero. But myPRUList is start from 1, because there is a dummy item at index zero (pruMenu)
	{
		SavePRPara(GetmyCalibPRUList().GetAt(i)->szStnName, GetmyCalibPRUList().GetAt(i), GetmyCalibPRUList().GetAt(i)->szFileName, TRUE);
	}

	return CPI9000Stn::IPC_SaveMachineParam();
}

LONG CWinEagle::IPC_LoadMachineParam()
{
	//for(INT i = 1; i<GetmyCalibPRUList().GetSize(); i++)
	for (INT i = 0; i < GetmyCalibPRUList().GetSize(); i++) //20130109 CArray is start from zero. myCalibPRUList is start from zero. But myPRUList is start from 1, because there is a dummy item at index zero (pruMenu)
	{
		LoadPRPara(GetmyCalibPRUList().GetAt(i)->szStnName, GetmyCalibPRUList().GetAt(i), GetmyCalibPRUList().GetAt(i)->szFileName, TRUE);

		// Reset all calib Data;
		for (INT j = 1; j < GetmyPRUList().GetSize(); j++)
		{
			//First ABCD
			if (	(GetmyPRUList().GetAt(j)->eCamMoveGroup1 == GetmyCalibPRUList().GetAt(i)->eCamMoveGroup1) &&
				GetmyCalibPRUList().GetAt(i)->eCamMoveGroup1 != CMG_NONE)
			{
				GetmyPRUList().GetAt(j)->bCalibrated	= GetmyCalibPRUList().GetAt(i)->bCalibrated;
				
				GetmyPRUList().GetAt(j)->m2p_xform.a	= GetmyCalibPRUList().GetAt(i)->m2p_xform.a;
				GetmyPRUList().GetAt(j)->m2p_xform.b	= GetmyCalibPRUList().GetAt(i)->m2p_xform.b;
				GetmyPRUList().GetAt(j)->m2p_xform.c	= GetmyCalibPRUList().GetAt(i)->m2p_xform.c;
				GetmyPRUList().GetAt(j)->m2p_xform.d	= GetmyCalibPRUList().GetAt(i)->m2p_xform.d;

				GetmyPRUList().GetAt(j)->p2m_xform.a	= GetmyCalibPRUList().GetAt(i)->p2m_xform.a;
				GetmyPRUList().GetAt(j)->p2m_xform.b	= GetmyCalibPRUList().GetAt(i)->p2m_xform.b;
				GetmyPRUList().GetAt(j)->p2m_xform.c	= GetmyCalibPRUList().GetAt(i)->p2m_xform.c;
				GetmyPRUList().GetAt(j)->p2m_xform.d	= GetmyCalibPRUList().GetAt(i)->p2m_xform.d;
			}

			//Second ABCD
			if ((GetmyPRUList().GetAt(j)->eCamMoveGroup2 == GetmyCalibPRUList().GetAt(i)->eCamMoveGroup2) &&
				GetmyCalibPRUList().GetAt(i)->eCamMoveGroup2 != CMG_NONE)
			{
				GetmyPRUList().GetAt(j)->bCalibrated_2	= GetmyCalibPRUList().GetAt(i)->bCalibrated_2;
				
				GetmyPRUList().GetAt(j)->m2p_xform_2.a	= GetmyCalibPRUList().GetAt(i)->m2p_xform_2.a;
				GetmyPRUList().GetAt(j)->m2p_xform_2.b	= GetmyCalibPRUList().GetAt(i)->m2p_xform_2.b;
				GetmyPRUList().GetAt(j)->m2p_xform_2.c	= GetmyCalibPRUList().GetAt(i)->m2p_xform_2.c;
				GetmyPRUList().GetAt(j)->m2p_xform_2.d	= GetmyCalibPRUList().GetAt(i)->m2p_xform_2.d;

				GetmyPRUList().GetAt(j)->p2m_xform_2.a	= GetmyCalibPRUList().GetAt(i)->p2m_xform_2.a;
				GetmyPRUList().GetAt(j)->p2m_xform_2.b	= GetmyCalibPRUList().GetAt(i)->p2m_xform_2.b;
				GetmyPRUList().GetAt(j)->p2m_xform_2.c	= GetmyCalibPRUList().GetAt(i)->p2m_xform_2.c;
				GetmyPRUList().GetAt(j)->p2m_xform_2.d	= GetmyCalibPRUList().GetAt(i)->p2m_xform_2.d;
			}
		}
	}

	return CPI9000Stn::IPC_LoadMachineParam();
}

LONG CWinEagle::IPC_SaveDeviceParam()
{
	for (INT i = 1; i < GetmyPRUList().GetSize(); i++)
	{
		SavePRPara(GetmyPRUList().GetAt(i)->szStnName, GetmyPRUList().GetAt(i), GetmyPRUList().GetAt(i)->szFileName);
	}

	return CPI9000Stn::IPC_SaveDeviceParam();
}

LONG CWinEagle::IPC_LoadDeviceParam()
{
	for (INT i = 1; i < GetmyPRUList().GetSize(); i++)
	{
		LoadPRPara(GetmyPRUList().GetAt(i)->szStnName, GetmyPRUList().GetAt(i), GetmyPRUList().GetAt(i)->szFileName);
	}

	return CPI9000Stn::IPC_LoadDeviceParam();
}



LONG CWinEagle::HMI_AutoTest(IPC_CServiceMessage &svMsg)
{	
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	BOOL bResult = TRUE;
	PRU *pPRU;
	CString strMsg;
	LONG lAnswer = -1;

	pPRU = &SelectCurPRU(HMI_ulCurPRU);

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			if (!pCWH1->AutoTest(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPDL2_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			if (!pCWH1->AutoTest(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPDL1_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			if (!pCWH2->AutoTest(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPDL2_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			if (!pCWH2->AutoTest(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInspOpt->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			if (!pCInspOpt->AutoTest(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else if (!pCInspOpt->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
			HMIMessageBox(MSG_OK, "WARNING", strMsg);
			bResult = FALSE;
		}
		else
		{
			if (!pCInspOpt->AutoTest(pPRU))
			{
				bResult = FALSE;
			}	
		}
		break;

	}

	return 1;
}

LONG CWinEagle::HMI_CORAutoTest(IPC_CServiceMessage &svMsg)
{
	CInspWH *pCInspWH = (CInspWH*)m_pModule->GetStation("InspWH");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInPickArm *pCInPickArm = (CInPickArm*)m_pModule->GetStation("InPickArm");

	BOOL bOpDone = TRUE;
	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;
	
	LONG lAnswer = rMSG_TIMEOUT;
	LONG lAutoTestCount = 0;
	LONG lResponse = 0;

	BOOL bResult = TRUE;
	CString strMsg;

	do
	{
		lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "OPTION", "Auto Test with COR Angle: -1.5, -1, -0.5, -0.1, 0.1, 0.5, 1, 1.5?");

		if (lAnswer == rMSG_CONTINUE)
		{
			if (!HMINumericKeys("Set Test Repeat Time", 30, 0, &lAutoTestCount))
			{
				HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", "OPERATION STOP");
				svMsg.InitMessage(sizeof(BOOL), &bOpDone);
				return 1;
				break;
			}
			if (lAutoTestCount < 0 || lAutoTestCount == 0 || lAutoTestCount > 100)
			{
				HMIMessageBox(MSG_OK, "COR AUTO TEST OPERATION", "OPERATION STOP");
				svMsg.InitMessage(sizeof(BOOL), &bOpDone);
				return 1;
				break;
			}

			switch (HMI_ulCurPRU)
			{
			case INSPDL1_WH1_CALIB_PRU:
				if (!pCWH1->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else if (!pCInPickArm->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else
				{
					pruINSP_DL_Calib[GLASS1][WH_1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
					pCWH1->m_lCORCamNum = INSPDL1_COR;
					if (!pCWH1->CORAutoTest(&pruINSP_DL_Calib[GLASS1][WH_1], lAutoTestCount))
					{
						bResult = FALSE;
					}

				}
				break;

			case INSPDL2_WH1_CALIB_PRU:
				if (!pCWH1->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else if (!pCInPickArm->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else
				{
					pruINSP_DL_Calib[GLASS2][WH_1].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH1;
					pCWH1->m_lCORCamNum = INSPDL2_COR;
					if (!pCWH1->CORAutoTest(&pruINSP_DL_Calib[GLASS2][WH_1], lAutoTestCount))
					{
						bResult = FALSE;
					}

				}
				break;

			case INSPDL1_WH2_CALIB_PRU:
				if (!pCWH2->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else if (!pCInPickArm->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else
				{
					pruINSP_DL_Calib[GLASS1][WH_2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
					pCWH2->m_lCORCamNum = INSPDL1_COR;
					if (!pCWH2->CORAutoTest(&pruINSP_DL_Calib[GLASS1][WH_2], lAutoTestCount))
					{
						bResult = FALSE;
					}

				}
				break;

			case INSPDL2_WH2_CALIB_PRU:
				if (!pCWH2->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else if (!pCInPickArm->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCInPickArm->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else
				{
					pruINSP_DL_Calib[GLASS2][WH_2].eCamMoveGroupCurrent = CMG_INSPDL_XINPA_YWH2;
					pCWH2->m_lCORCamNum = INSPDL2_COR;
					if (!pCWH2->CORAutoTest(&pruINSP_DL_Calib[GLASS2][WH_2], lAutoTestCount))
					{
						bResult = FALSE;
					}

				}
				break;

			case INSPWHUL_WH1_CALIB_PRU:
				pCInspOpt->m_lCurInspWH = WH_1;
				if (!pCWH1->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCWH1->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else if (!pCInspOpt->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else
				{
					if (!pCInspOpt->CORAutoTest(&pruINSPWH_UL_Calib[WH_1], lAutoTestCount))
					{
						bResult = FALSE;
					}

				}
				break;

			case INSPWHUL_WH2_CALIB_PRU:
				pCInspOpt->m_lCurInspWH = WH_2;
				if (!pCWH2->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCWH2->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else if (!pCInspOpt->m_bModSelected)
				{
					strMsg.Format("%s Module Not Selected. Operation Abort", pCInspOpt->GetStnName());
					HMIMessageBox(MSG_OK, "WARNING", strMsg);
					bResult = FALSE;
				}
				else
				{
					if (!pCInspOpt->CORAutoTest(&pruINSPWH_UL_Calib[WH_2], lAutoTestCount))
					{
						bResult = FALSE;
					}

				}
				break;
			}
		}
		else if (lAnswer == rMSG_CANCEL || lAnswer == rMSG_TIMEOUT)
		{
			return 1;
			break;
		}
	} while (0);

	if (bResult)
	{
		HMIMessageBox(MSG_OK, "COR AUTO TEST", "OPERATION SUCCESS");
	}
	else
	{
		//HMIMessageBox(MSG_OK, "COR AUTO TEST", "OPERATION FAIL");
	}

	return 1;
}

LONG CWinEagle::HMI_SetPRErrorLimit(IPC_CServiceMessage &svMsg)
{
	CWH1 *pCWH1 = (CWH1*)m_pModule->GetStation("WH1");
	CWH2 *pCWH2 = (CWH2*)m_pModule->GetStation("WH2");
	CInspOpt *pCInspOpt = (CInspOpt*)m_pModule->GetStation("InspOpt");

	LONG lLimit;

	svMsg.GetMsg(sizeof(LONG), &lLimit);

	HMI_lErrorLimit = lLimit;

	switch (HMI_ulCurPRU)
	{
	case INSPDL1_WH1_CALIB_PRU:
		pCWH1->m_lPRErrorLimit = lLimit;
		break;

	case INSPDL2_WH1_CALIB_PRU:
		pCWH1->m_lPRErrorLimit = lLimit;
		break;

	case INSPDL1_WH2_CALIB_PRU:
		pCWH2->m_lPRErrorLimit = lLimit;
		break;

	case INSPDL2_WH2_CALIB_PRU:
		pCWH2->m_lPRErrorLimit = lLimit;
		break;

	case INSPWHUL_WH1_CALIB_PRU:
		pCInspOpt->m_lPRErrorLimit = lLimit;
		break;

	case INSPWHUL_WH2_CALIB_PRU:
		pCInspOpt->m_lPRErrorLimit = lLimit;
		break;
	}


	return 0;
}