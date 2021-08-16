/////////////////////////////////////////////////////////////////
//	PI9000.cpp : Defines the class behaviors for the application.
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
#include "direct.h"
#define PI9000_EXTERN
#include "PI9000.h"
#include "MainFrm.h"
#include "PI9000Doc.h"
#include "PI9000View.h"
#include "MarkConstant.h"
#include "PI9000_Constant.h"
#include "HardwareDlg.h"
#include "OptionDlg.h"
#include "OptionPasswordDlg.h" //20130916
#include "AppStation.h"
#include "WorkHolder.h"
#include "TransferArm.h"
#include "InspWH.h"
#include "InspOpt.h"
#include "WH1.h"
#include "WH2.h"
#include "InPickArm.h"
#include "OutPickArm.h"
#include "HouseKeeping.h"
#include "WinEagle.h"
#include "SettingFile.h"
#include "TAManager.h"
#include "TimeChart.h"

#include "ShBufferWH.h"
#include "RejectBelt.h"

#include "PI9000Stn.h"
#include "GI_System.h"

#include "PRTaskStn.h"

#include "PR_Util.h"
//#include "Cal_Util.h"
#include "Logging_Util.h"
#include "ExcelMotorInfo_Util.h"
#include "BasicExcel.h"
#include "FileOperations.h"

#include "Psapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////
//	Constant Definitions
/////////////////////////////////////////////////////////////////



const CString gszCOMPANY_NAME			= _T("ASM Assembly Automation Ltd");
const CString gszSOFTWARE_VERSION		= _T("1.00.00");
const CString gszMACHINE_MODEL			= _T("PI9000");
const CString gszSOFTWARE_NUMBER		= _T("10-301880");
const CString gszRELEASE_DATE			= _T("Fri, Apr 07, 2017");

// Define the Registry's Sections
const CString gszPROFILE_SETTING		= _T("Settings");
const CString gszPROFILE_GROUP_ID		= _T("Group Id");
const CString gszPROFILE_AUTO_START		= _T("Auto Start");
const CString gszPROFILE_LOG_MSG		= _T("Log Message");
const CString gszPROFILE_DIR_PATH		= _T("Directory Path");
const CString gszPROFILE_HW_CONFIG		= _T("Hardware Config");
const CString gszPROFILE_EXECUTE_HMI	= _T("Execute HMI");
const CString gszLOG_PATH_SETTING		= _T("Msg Log Path");
const CString gszLOG_FILE_SETTING		= _T("Msg Log File");
const CString gszLOG_BACKUP_SETTING		= _T("Total Backup Files");
const CString gszLOG_FILE_LINE			= _T("Total Number of Lines");
const CString gszSELECT_MC_VER1			= _T("M/C Ver1");
const CString gszSELECT_MC_VER2			= _T("M/C Ver2");
const CString gszSELECT_MC_VER3			= _T("M/C Ver3");
const CString gszSELECT_LITEC			= _T("Select LITEC");
const CString gszSELECT_ADAM			= _T("Select ADAM");
const CString gszSELECT_HEATER_ON		= _T("Select Heater On");	// 20140905 Yip
//const CString gszSELECT_EDGE_CLEANER	= _T("Select Edge Cleaner");
const CString gszSELECT_COG902			= _T("Select COG902");
const CString gszSELECT_TA4_CAMERA		= _T("Select TA4 Camera");
const CString gszSELECT_NEW_PB_CONTACT_SNR = _T("Select New PB Contact Snr"); //20141201
const CString gszSELECT_CREATE_ERROR_LOG	= _T("Select Create Error Log");	// 20150331
const CString gszSELECT_TEFLON_ENCODER	= _T("Select Teflon Encoder");	// TeflonEncoder
const CString gszSELECT_120MM_FOV	    = _T("Select 120mm FOV");	// 120mm FOV


const CString gszENABLE_ALL_HARDWARE	= _T("Enable All Hardware");
const CString gszENABLE_PRS				= _T("Enable PRS");
const CString gszENABLE_TEMP_CTRL		= _T("Enable TempCtrl");
const CString gszENABLE_PICKARM				= _T("Enable PICKARM");
const CString gszENABLE_INSPWH			= _T("Enable INSPWH");
const CString gszENABLE_SHBUFFER_WH			= _T("Enable ShBufferWH");
const CString gszENABLE_INSPOPT			= _T("Enable InspOpt");
const CString gszENABLE_OUT_CONVEYOR	= _T("Enable OutConv");
const CString gszENABLE_DIAGN_MODE		= _T("Enable DiagnMode");

const CString gszENABLE_RejectArm		= _T("Enable RejectArm");
const CString gszENABLE_RejectBelt		= _T("Enable RejectBelt");
const CString gszENABLE_InPickArm		= _T("Enable InPickArm");
const CString gszENABLE_OutPickArm		= _T("Enable OutPickArm");
const CString gszENABLE_WH1		        = _T("Enable WH1");
const CString gszENABLE_WH2		        = _T("Enable WH2");
const CString gszENABLE_INWH	        = _T("Enable INWH");


const CString gszMACHINE_PATH_SETTING	= _T("Machine File Path");
const CString gszDEVICE_PATH_SETTING	= _T("Device File Path");
const CString gszMACHINE_FILE_SETTING	= _T("Machine File");
const CString gszDEVICE_FILE_SETTING	= _T("Device File");

const CString gszMACHINE_ID				= _T("Machine ID");
const CString gszOS_IMAGE				= _T("OS Image Number");

const CString gszVISION_OS_IMAGE		= _T("Vision OS Image Number");

const CString gszCONFIG_EXT				= _T(".cfg");
const CString gszCONFIG_BACK_EXT		= _T(".bcf");

// Define default Registry settings' value
const CString gszMODULE_HW_FILE			= _T("PI9000Hardware");
const CString gszDEFAULT_GROUP_ID		= _T("PI9000");

//Log File
const CString gszLOG_FILE_PATH			= _T("D:\\sw\\PI9000\\LogFileDir");
const CString gszLOG_FILE_NAME			= _T("PI9000MsgLog");

//Application Name
const CString gszPI9000	= _T("PI9000");

//Mark IDE Data File Path
const CString gszMARK_DATA_PATH			= _T("D:\\sw\\PI9000\\MarkData\\");
/*const*/ 
CString gszMARK_DATA_FILE_NAME	= _T("PI9000.dat");

//SCF Files
const CString gszSCF_FLIE_PATH			= _T("D:\\sw\\PI9000\\MarkData\\SCFFile\\PI9000.ini");
const CString gszSCF_ACTI_PATH			= _T("D:\\sw\\PI9000\\MarkData\\SCFFile\\Activation.ini");

const CString gszSCF_FLIE_PATH_2			= _T("D:\\sw\\PI9000\\MarkData\\SCFFile\\PI9000_ver2.ini");
const CString gszSCF_ACTI_PATH_2			= _T("D:\\sw\\PI9000\\MarkData\\SCFFile\\Activation_ver2.ini");

const CString gszSCF_FLIE_PATH_3			= _T("D:\\sw\\PI9000\\MarkData\\SCFFile\\PI9000_ver3.ini");
const CString gszSCF_ACTI_PATH_3			= _T("D:\\sw\\PI9000\\MarkData\\SCFFile\\Activation_ver3.ini");

//Machine and Device File
const CString gszMACHINE_FILE_PATH		= _T("D:\\sw\\PI9000\\Param");
const CString gszDEVICE_FILE_PATH		= _T("D:\\sw\\PI9000\\Device");
const CString gszMACHINE_FILE_NAME		= _T("MachineParam.prm");
const CString gszDEVICE_FILE_NAME		= _T("DeviceParam.prm");

const CString gszMACHINE_ID_NO			= _T("---");
const CString gszOS_IMAGE_NO			= _T("---");

const CString gszOS_VISION_IMAGE_NO		= _T("---");

// Define the IPC Service Commands
const CString gszHARDWARE_INITIAL		= _T("Initialize");
const CString gszSYSTEM_INITIAL			= _T("SystemInit");
const CString gszDIAG_COMMAND			= _T("Diagnostic");
const CString gszAUTO_COMMAND			= _T("AutoCycle");
const CString gszMANUAL_COMMAND			= _T("Manual");
const CString gszCYCLE_COMMAND			= _T("CycleTest");
const CString gszDEMO_COMMAND			= _T("Demonstration");
const CString gszSTOP_COMMAND			= _T("Stop");
const CString gszRESET_COMMAND			= _T("Reset");
const CString gszDE_INITIAL_COMMAND		= _T("DeInitialize");
const CString gszJOG_COMMAND			= _T("Jog");
const CString gszGET_DEVICE				= _T("GetDevice");
const CString gszUPDATE_DEVICE			= _T("UpdateDevice");
const CString gszCHANGE_DEVICE			= _T("ChangeDevice");
const CString gszDELETE_DEVICE			= _T("DeleteDevice"); //20120703

const CString gszNEW_DEVICE				= _T("NewDevice");
const CString gszSAVE_AS_DEVICE			= _T("SaveAsDevice");
const CString gszPRINT_DEVICE			= _T("PrintDevice");


const CString gszGET_MACHINE			= _T("GetMachine");
const CString gszUPDATE_MACHINE			= _T("UpdateMachine");
const CString gszPRINT_MACHINE			= _T("PrintMachine");

HANDLE ghMutex = NULL; //20120614

BEGIN_MESSAGE_MAP(CPI9000App, SFM_CModule)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////
//Construction
/////////////////////////////////////////////////////////////////

CPI9000App::CPI9000App()
{
	HMI_lSWLimitEncoder			= 0; //whz rotary linear calib

	m_fExecuteHmi				= FALSE;
	m_szState					= gszMODULE_STATE[UN_INITIALIZE_Q];
	m_qState					= UN_INITIALIZE_Q;
	m_nOperation				= NULL_OP;
	m_lAction					= glNULL_COMMAND;
	m_dwStopTime				= 0;
	m_bStatusLogged				= FALSE;
	m_bRunState					= FALSE;
	m_fDeleteMarkIDE			= FALSE;

	m_fEnableInitInstanceError	= TRUE;
	m_fSelectVer1				= FALSE;
	m_fSelectCreateErrorLog		= FALSE;	// 20150331

	m_fEnableHardware			= TRUE;
	m_fEnablePRS				= TRUE;


	m_szMachineModel			= gszMACHINE_MODEL;
	m_szMachineID				= "---";
	m_szOSImageNumber			= "10-M00207V1-00-00";
	m_szVisionOSImageNumber		= "---";
	m_szLitecVersion1			= "---";
	m_szLitecVersion2			= "---";
	m_szLitecVersion3			= "---";
	m_szLitecVersion4			= "---";
	m_szSoftwareNumber			= gszSOFTWARE_NUMBER;
	m_szSoftVersion				= gszSOFTWARE_VERSION;
	m_szReleaseDate				= gszRELEASE_DATE;

	m_bHWInitCheckingPressure	= TRUE;
	m_bHWInitFatalError			= FALSE; //20130226
	m_bHWInitFatalErrorUI		= FALSE;

	m_isAppRunning				= FALSE; //20111214
	m_szMotionLibVersion		= "1.0a"; //20121210
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bPerformanceReportEnable	= TRUE;	// 20140730 Yip: Enable Performance Report

	m_clkBondStartTime			= 0; //20120806
	m_clkLastErrorTime			= 0; //20150105

	HMI_lGatewayStatus			= NETWORK_NIL; //0=gnAMS_OK, 1=gnAMS_NOTOK, 2=connecting

	ZeroMemory(&m_stGatewayProcessInfo, sizeof(m_stGatewayProcessInfo));

	InitErrorAlertTable(); //20141124
	setKlocworkTrue(TRUE); //klocwork fix 20121211


}

CPI9000App::~CPI9000App()
{
	if (ghMutex != NULL)
	{
		CloseHandle(ghMutex);
	}
	m_Stations.RemoveAll();
	m_PRStations.RemoveAll();
}

/////////////////////////////////////////////////////////////////
// The one and only CPI9000App object
/////////////////////////////////////////////////////////////////

CPI9000App theApp;

/////////////////////////////////////////////////////////////////
//	Station Threads
/////////////////////////////////////////////////////////////////
//CPickArm			*pCPickArm;
CInspWH				*pCInspWH;
CInspOpt			*pCInspOpt;
CHouseKeeping		*pCHouseKeeping;
CWinEagle			*pCWinEagle;
CSettingFile		*pCSettingFile;
CTimeChart			*pCTimeChart;

CShBufferWH			*pCShBufferWH;
//CRejectArm			*pCRejectArm;
CRejectBelt			*pCRejectBelt;
CInPickArm		    *pCInPickArm;
COutPickArm	        *pCOutPickArm;
CWH1		        *pCWH1;
CWH2			    *pCWH2;
CTAManager			*pCTAManager;
//CINWH			    *pCINWH;

#ifdef EXCEL_MACHINE_DEVICE_INFO
BasicExcelWorksheet		*g_pExcelMachineInfoSheet = NULL;
BasicExcelWorksheet		*g_pExcelDeviceInfoSheet = NULL;
LONG					g_lMachineInfoRowNum = 0;
LONG					g_lDeviceInfoRowNum = 0;
#endif

/////////////////////////////////////////////////////////////////
// CPI9000App initialization
/////////////////////////////////////////////////////////////////

BOOL CPI9000App::InitInstance()
{
	// Create mutex, because there cannot be 2 instances of the same application
	ghMutex = CreateMutex(NULL, FALSE, gszPI9000); 

	// Check if mutex is created succesfully
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//Mutex already exists so there is a running instance of our app.
		MessageDlg dlg;
		//TRACE("### Mutex already exists so there is a running instance of our app.\n");
		dlg.DoModal();
		return false;
	}

#if 1 //20130712
	{
		ULONG ulFreeSpaceMB = 0;
		struct _diskfree_t df = {0};
		if (_getdiskfree((INT)DRIVE_D, &df) == 0)
		{
			ulFreeSpaceMB = df.avail_clusters * df.sectors_per_cluster / 1024 * df.bytes_per_sector / 1024;

			if (ulFreeSpaceMB < 500)
			{
				CString szMsg(" ");
				szMsg.Format("D-Drive free space is %luMB. Free Space is less than 500MB. Please Free More Disk Space for the D-Drive.", ulFreeSpaceMB);
				//HMIMessageBox(MSG_OK, "D-Drive Free Space", szMsg);
				AfxMessageBox((char*)szMsg.GetBuffer(0));
			}
		}

		if (_getdiskfree((INT)DRIVE_C, &df) == 0)
		{
			ulFreeSpaceMB = df.avail_clusters * df.sectors_per_cluster / 1024 * df.bytes_per_sector / 1024;

			if (ulFreeSpaceMB < 500)
			{
				CString szMsg(" ");
				szMsg.Format("C-Drive free space is %luMB. Free Space is less than 500MB. Please Free More Disk Space for the C-Drive.", ulFreeSpaceMB);
				//HMIMessageBox(MSG_OK, "D-Drive Free Space", szMsg);
				AfxMessageBox((char*)szMsg.GetBuffer(0));
			}
		}
	}
#endif

#if 1 //20141210
	{
		char buffer[_MAX_PATH];
		CString szMsg(" ");
		/* Get the current working directory: */
		if (_getcwd(buffer, _MAX_PATH) != NULL)
		{
			//szMsg.Format("Current Working Dir:%s", buffer);
			//AfxMessageBox((char*)szMsg.GetBuffer(0));

			if (
				_stricmp(buffer, "d:\\sw\\PI9000") != 0 &&
#ifdef _DEBUG
				_stricmp(buffer, "d:\\sw\\PI9000\\Debug") != 0
#else
				_stricmp(buffer, "d:\\sw\\PI9000\\Release") != 0
#endif
				)
			{
#ifdef _DEBUG
				szMsg.Format("Current Working Dir Error!\nCurrently:%s\nIt must be D:\\sw\\PI9000\\Debug", buffer);
#else
				szMsg.Format("Current Working Dir Error!\nCurrently:%s\nIt must be D:\\sw\\PI9000\\Release", buffer);
#endif
				AfxMessageBox((char*)szMsg.GetBuffer(0));
				return false;
			}
		}
		else
		{
			//error
		}
	}
#endif

	InitCommonControls();

	SFM_CModule::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

#if 1
	if (m_bPerformanceReportEnable)
	{
		HANDLE hFile;
		hFile = CreateFile("D:\\sw\\PI9000\\Release\\performance_report_config.csv", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL); //20130109

		if (hFile == INVALID_HANDLE_VALUE)		//Philip add
		{
			//use default setting
			SaveSettingReportConfig("8,\n1st_Hour,\n2nd_Hour,\n3rd_Hour,\n4th_Hour,\n5th_Hour,\n6th_Hour,\n7th_Hour,\n8th_Hour,\n9th_Hour,\n10th_Hour,\n11th_Hour,\n12th_Hour,\n13th_Hour,\n14th_Hour,\n15th_Hour,\n16th_Hour,\n17th_Hour,\n18th_Hour,\n19th_Hour,\n20th_Hour,\n21st_Hour,\n22nd_Hour,\n23rd_Hour,\n24th_Hour,");
			LoadSettingReportConfig();
		}
		else
		{
			LoadSettingReportConfig();
		}
		CloseHandle(hFile);


		g_pPerformance_report = new CPerformanceReport("D:\\sw\\PI9000\\Release\\performance_report_config.csv");	//input param: shift config file path
		if (g_pPerformance_report != NULL)
		{
			g_pPerformance_report->StartNewReport();    //create a new report
		}
	}
#endif

	INT nTemp;
	SetRegistryKey(gszCOMPANY_NAME);

	//AutoStart
	nTemp = GetProfileInt(gszPROFILE_SETTING, gszPROFILE_AUTO_START, 0);
	nTemp ? m_fAutoStart = TRUE : m_fAutoStart = FALSE;

	// Initialize the hardware if auto-start is enabled
	if (m_fAutoStart)
	{
		m_bRunState = TRUE;
	}

	m_qState = UN_INITIALIZE_Q;

	//Log File
	nTemp = GetProfileInt(gszPROFILE_SETTING, gszPROFILE_LOG_MSG, 1);
	nTemp ? m_fLogMessage = TRUE : m_fLogMessage = FALSE;

	//Execute Hmi
	nTemp = GetProfileInt(gszPROFILE_SETTING, gszPROFILE_EXECUTE_HMI, 0);
	SetExecuteHmiFlag(nTemp);

	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate *pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPI9000Doc),
		RUNTIME_CLASS(CMainFrame), // main SDI frame window
		RUNTIME_CLASS(CPI9000View));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	//Create InitOperation
	m_pInitOperation	= new CInitOperation;

	//Event
	m_evStart.ResetEvent();

	//Group ID
	m_szGroupID	= GetProfileString(gszPROFILE_SETTING, gszPROFILE_GROUP_ID, gszDEFAULT_GROUP_ID);

	//Retrieve Tot Log File Backup
	nTemp = GetProfileInt(gszPROFILE_SETTING, gszLOG_BACKUP_SETTING, 50); //Default values, 5 Backup files
	if (nTemp <= 5) //20131004
	{
		nTemp = 50;
	}
	SetTotLogBackupFiles(nTemp); 

	//Total Number of line for Log File 
	nTemp = GetProfileInt(gszPROFILE_SETTING, gszLOG_FILE_LINE, 50000); //Default values, 50000 lines
	SetTotLogFileLine(nTemp); 

	// Retrieve Message Log File Path
	if ((m_szLogFilePath = GetProfileString(gszPROFILE_SETTING, gszLOG_PATH_SETTING, _T(""))) == _T(""))
	{
		m_szLogFilePath = gszLOG_FILE_PATH;
		WriteProfileString(gszPROFILE_SETTING, gszLOG_PATH_SETTING, gszLOG_FILE_PATH);
	}

	if ((m_szLogFile = GetProfileString(gszPROFILE_SETTING, gszLOG_FILE_SETTING, _T(""))) == _T(""))
	{
		m_szLogFile = gszLOG_FILE_NAME;
		WriteProfileString(gszPROFILE_SETTING, gszLOG_FILE_SETTING, gszLOG_FILE_NAME);
	}

	m_pInitOperation->SaveLogFilePath(m_szLogFilePath);
	m_pInitOperation->SaveLogFile(m_szLogFile);

	// Retrieve Machine path
	if ((m_szMachinePath = GetProfileString(gszPROFILE_SETTING, gszMACHINE_PATH_SETTING, _T(""))) == _T(""))
	{
		m_szMachinePath = gszMACHINE_FILE_PATH;
		WriteProfileString(gszPROFILE_SETTING, gszMACHINE_PATH_SETTING, gszMACHINE_FILE_PATH);
	}

	// Retrieve Device path
	if ((m_szDevicePath = GetProfileString(gszPROFILE_SETTING, gszDEVICE_PATH_SETTING, _T(""))) == _T(""))
	{
		m_szDevicePath = gszDEVICE_FILE_PATH;
		WriteProfileString(gszPROFILE_SETTING, gszDEVICE_PATH_SETTING, gszDEVICE_FILE_PATH);
	}
	
	// Retrieve Machine File
	if ((m_szMachineFile = GetProfileString(gszPROFILE_SETTING, gszMACHINE_FILE_SETTING, _T(""))) == _T(""))
	{
		m_szMachineFile = gszMACHINE_FILE_NAME;
		WriteProfileString(gszPROFILE_SETTING, gszMACHINE_FILE_SETTING, gszMACHINE_FILE_NAME);
	}

	// Retrieve Default Device File
	if ((m_szDeviceFile = GetProfileString(gszPROFILE_SETTING, gszDEVICE_FILE_SETTING, _T(""))) == _T(""))
	{
		m_szDeviceFile = gszDEVICE_FILE_NAME;
		WriteProfileString(gszPROFILE_SETTING, gszDEVICE_FILE_SETTING, gszDEVICE_FILE_NAME);
	}

	// Retrieve Machine ID
	if ((m_szMachineID = GetProfileString(gszPROFILE_SETTING, gszMACHINE_ID, _T(""))) == _T(""))
	{
		m_szMachineID = gszMACHINE_ID_NO;
		WriteProfileString(gszPROFILE_SETTING, gszMACHINE_ID, gszMACHINE_ID_NO);
	}

	// Retrieve OS Image Number
	if ((m_szOSImageNumber = GetProfileString(gszPROFILE_SETTING, gszOS_IMAGE, _T(""))) == _T(""))
	{
		m_szOSImageNumber = gszOS_IMAGE_NO;
		WriteProfileString(gszPROFILE_SETTING, gszOS_IMAGE, gszOS_IMAGE_NO);
	}

	// Retrieve OS Image Number
	if ((m_szVisionOSImageNumber = GetProfileString(gszPROFILE_SETTING, gszVISION_OS_IMAGE, _T(""))) == _T(""))
	{
		m_szVisionOSImageNumber = gszOS_VISION_IMAGE_NO;
		WriteProfileString(gszPROFILE_SETTING, gszVISION_OS_IMAGE, gszOS_VISION_IMAGE_NO);
	}

	CSingleLock slLock(&m_csDispMsg);
	slLock.Lock();
	m_fCanDisplay = TRUE;
	slLock.Unlock();

	// Backup the Message Log (if program crashes in last run)
	m_pInitOperation->RenameLogFile();
	// Create the Message Log file used for debugging
	m_fLogFileCreated = m_pInitOperation->CreateLogFile();

	m_fSelectVer1				= GetProfileInt(gszPROFILE_HW_CONFIG, gszSELECT_MC_VER1, 1);

	m_fSelectCreateErrorLog		= GetProfileInt(gszPROFILE_HW_CONFIG, gszSELECT_CREATE_ERROR_LOG, 0);	// 20150331


	//Mark IDE Data Path
	LoadData();

	//Retrieve EnableHardware Option
	//Note that EnableAllHardware() should be called after LoadData();
	m_fEnableHardware			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_ALL_HARDWARE, 1);
	m_fEnablePRS				= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_PRS, 1);
	//m_fEnablePICKARM				= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_PICKARM, 1);
	//m_fEnableINSPWH				= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_INSPWH, 1);
	m_fEnableShBufferWH				= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_SHBUFFER_WH, 1);
	m_fEnableInspOpt			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_INSPOPT, 1);
	m_fEnableDiagnMode			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_DIAGN_MODE, 1);

	//m_fEnableRejectArm			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_RejectArm, 1);
	m_fEnableRejectBelt			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_RejectBelt, 1);
	m_fEnableInPickArm			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_InPickArm, 1);
	m_fEnableOutPickArm			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_OutPickArm, 1);
	m_fEnableWH1	     		= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_WH1, 1);
	m_fEnableWH2     			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_WH2, 1);
	//m_fEnableINWH     			= GetProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_INWH, 1);

	m_bHWInitError				= FALSE;
	m_bHWInitFatalError			= FALSE; //20130226
	m_bHWInitFatalErrorUI		= FALSE;
	m_bHWInited					= FALSE;

	m_bAllLitecInited			= FALSE;
	m_bAllAdamInited			= FALSE;

	m_bInvalidInitState			= FALSE;
	m_bInitInstanceError		= FALSE;

	// Station commutated flags
	m_bInspWH1Comm					= FALSE;
	m_bInspWH1Home					= FALSE;
	m_bInspWH2Comm					= FALSE;
	m_bInspWH2Home					= FALSE;
	m_bInPickArmComm				= FALSE;	
	m_bInPickArmHome				= FALSE;
	m_bOutPickArmComm				= FALSE;	
	m_bOutPickArmHome				= FALSE;
	m_bInspOptComm					= FALSE;
	m_bInspOptHome					= FALSE;
	m_bDiagnMode					= FALSE;

	// Diagnostic testing
	bPRRepeatTest				= FALSE;

	// Message windows
	m_szHMIMess					= "The Machine is initialing";
	m_bHMIMess					= FALSE;

	// statistics
	m_ulGlassPicked				= 0;
	m_ulTotalGlassPicked		= 0;


	m_ulGlassRejected						= 0;
	m_ulTotalGlassRejected					= 0;
	m_ulDLG1PRError							= 0;
	m_ulDLG2PRError							= 0;
	m_ulTotalDLPRError						= 0;
	m_ulInspOptPRError						= 0;
	m_ulTotalInspOptPRError					= 0;

	m_ulGlassTolError			= 0;
	m_ulTotalGlassTolError		= 0;

	m_ulUnitsInpected			= 0;
	m_ulTotalGlassPicked		= 0;

//p20120829
	m_lStatisticsLogCount		= 0;
//p20120829:end

	m_dUPH						= 0.0;

	m_ulLangageCode				= 0;		// English

	dDummy						= 0.0;
	stDummy						= "";

	HMI_szMotorDirectionPng		= "";
	HMI_lMotorPosLmt			= 0;
	HMI_lMotorNegLmt			= 0;

#if 1 //20130916
	m_smfMachine.Close(); //Clear File buffer
	if (!m_smfMachine.Open(m_szMachinePath + "\\" + m_szMachineFile, FALSE, TRUE))
	{
		AfxMessageBox("Can not load File \"" + m_szMachinePath + "\\" + m_szMachineFile + "\"");
	}
#endif

	return TRUE;
}

int CPI9000App::ExitInstance() 
{
	if (m_pInitOperation)
	{
		if (m_fLogFileCreated)
		{
			m_pInitOperation->RenameLogFile();
		}

		if (IsInitNuDrive())
		{
			m_pCGmpSystem->FreeSystem();
			DisplayMessage("Nu Drive is uninitialized ...");
		}
		
		delete m_pInitOperation;
		delete m_pCGmpSystem;
	}

	if (g_pPerformance_report != NULL)	// 20140819 Yip: Fix Memory Leak Issue
	{
		delete g_pPerformance_report;
	}
	
	m_smfDevice.Close();
	m_smfMachine.Close();

   // Close process and thread handles.
	if (m_stGatewayProcessInfo.hProcess != NULL)
	{
		TerminateProcess(m_stGatewayProcessInfo.hProcess, 0);
		CloseHandle(m_stGatewayProcessInfo.hProcess);
		CloseHandle(m_stGatewayProcessInfo.hThread);
	}

	return SFM_CModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////
// CPI9000App message handlers
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
//Group ID
/////////////////////////////////////////////////////////////////

CString	CPI9000App::GetModuleID()
{
	return m_szModuleName;
}

CString	CPI9000App::GetGroupID()
{
	return m_szGroupID;
}


/////////////////////////////////////////////////////////////////
//Mark IDE Data Loading
/////////////////////////////////////////////////////////////////

VOID CPI9000App::LoadData()
{
	try
	{
		switch (MachineVersion())
		{
		case VERSION_1:
			gszMARK_DATA_FILE_NAME	= _T("PI9000-1.dat");
			DisplayMessage("PI9000-1 MACHINE");
			break;
			
		case VERSION_2:
			gszMARK_DATA_FILE_NAME	= _T("PI9000-1.dat"); //use the same mark file for another machine, because we are using SCF.ini.
			DisplayMessage("PI9000-2 MACHINE");
			break;	

		case VERSION_3:
			gszMARK_DATA_FILE_NAME	= _T("PI9000-1.dat"); //use the same mark file for another machine, because we are using SCF.ini.
			DisplayMessage("PI9000-3 MACHINE");
			break;	
		}

		//Load data
		m_szDataPath	= gszMARK_DATA_PATH + gszMARK_DATA_FILE_NAME;
		m_pInitOperation->LoadData(m_szDataPath);

		m_szModuleName	= m_pInitOperation->GetModuleID();

		if ((m_szGroupID == gszDEFAULT_GROUP_ID)	&& 
			(m_pInitOperation->GetGroupID() != _T("")))
		{
			m_szGroupID	= m_pInitOperation->GetGroupID();
			SaveGroupId(m_szGroupID);
		}
		else if (m_szGroupID == "")
		{
			(m_pInitOperation->GetGroupID() == _T("")) ? m_szGroupID = gszDEFAULT_GROUP_ID :
																	   m_szGroupID	= m_pInitOperation->GetGroupID();

			SaveGroupId(m_szGroupID);
		}

		CPI9000View *pView = (CPI9000View*)((CMainFrame*) m_pMainWnd)->GetActiveView();
		pView->SetGroupID(m_szGroupID);
		pView->SetModule(m_szModuleName);
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}

DWORD CPI9000App::RunLongTimeFunction(AFX_THREADPROC pfnThreadProc, LPVOID pParam, const CString &szDesc)	// 20140724 Yip
{
	DWORD dwExitCode;

	CWinThread *pThread = AfxBeginThread(pfnThreadProc, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pThread->m_bAutoDelete = FALSE;
	pThread->ResumeThread();

	do
	{
		Sleep(100);
		if (GetExitCodeThread(pThread->m_hThread, &dwExitCode) == 0)
		{
			dwExitCode = 0;
			CString szMsg;
			szMsg.Format("%s Error: %ld", szDesc, GetLastError());
			DisplayMessage(szMsg);
		}
		KeepResponse();
	} while (dwExitCode == STILL_ACTIVE);
	delete pThread;

	return dwExitCode;
}

UINT _LoadMotorInfoParam(LPVOID lpVoid)	// 20140715 Yip
{
	BasicExcel *pExcelDB = static_cast<BasicExcel*>(lpVoid);
	CString szPath = _T("D:\\sw\\PI9000\\Param\\PI9000_MotorInfoParam.xls");
	if (theApp.MachineVersion() >= VERSION_2 && theApp.MachineVersion() < VERSION_3)
	{
		szPath = _T("D:\\sw\\PI9000\\Param\\PI9000_MotorInfoParam_ver2.xls");
	}
	else if (theApp.MachineVersion() >= VERSION_3)
	{
		szPath = _T("D:\\sw\\PI9000\\Param\\PI9000_MotorInfoParam_ver3.xls");
	}
	if (pExcelDB->Load(szPath))
	{
		return 1;
	}
	return 0;
}

typedef struct
{
	CGmpSystem *pCGmpSystem;
	char *cFilePath;
	char *cActiPath;
} GMP_INIT;

UINT _InitGmpSystem(LPVOID lpVoid)	// 20140715 Yip
{
	GMP_INIT *pGmpInit = static_cast<GMP_INIT*>(lpVoid);
	pGmpInit->pCGmpSystem->InitSystem(pGmpInit->cFilePath, pGmpInit->cActiPath, 8000);
	return 0;
}

UINT _SwitchAllCam(LPVOID lpVoid)	// 20140722 Yip
{
	CWinEagle *pCWinEagle = static_cast<CWinEagle*>(lpVoid);
	pCWinEagle->SwitchAllCam();
	return 0;
}

/////////////////////////////////////////////////////////////////
//Hardware Init
/////////////////////////////////////////////////////////////////

BOOL CPI9000App::InitHardware()
{
	BOOL			bInit = FALSE;
	CString			szStn;
	SFM_CStation	*pStn = NULL;
	CAppStation	*pAppStn;

	CString szMsg = "";
	__time64_t ltime;

	try
	{
		//Display Software Ver, Machine model and Release Date
		DisplayMessage("Machine Model: " + m_szMachineModel);
		DisplayMessage("Machine ID: " + m_szMachineID);
		DisplayMessage("Software Version: " + m_szSoftVersion);
		DisplayMessage("Release Date: " + m_szReleaseDate);

		//Setup the IPC comms and Register Variables
		SetupIPC();
		RegisterVariables();

		//Enable/ Disable controller here
		if (!m_fEnableHardware)
		{
			m_pInitOperation->EnableAllHardware(FALSE);
		}

		//Init NuMotion hardware
		//m_pInitOperation->InitializeHipec();
		DisplayMessage("NuMotion Network initialized ...");
		KeepResponse();	// 20140714 Yip

		//Register Station Class
		//ADD_STATION_HERE, Do not remove or modify this comment, it will be used by MARK-ADD-IN to add a Station
		//m_pInitOperation->RegisterStnClass("CPickArm", RUNTIME_CLASS(CPickArm));
		//m_pInitOperation->RegisterStnClass("CInspWH", RUNTIME_CLASS(CInspWH));
		m_pInitOperation->RegisterStnClass("CInspOpt", RUNTIME_CLASS(CInspOpt));
		m_pInitOperation->RegisterStnClass("CHouseKeeping", RUNTIME_CLASS(CHouseKeeping));
		m_pInitOperation->RegisterStnClass("CTAManager", RUNTIME_CLASS(CTAManager));
		m_pInitOperation->RegisterStnClass("CWinEagle", RUNTIME_CLASS(CWinEagle));
		m_pInitOperation->RegisterStnClass("CSettingFile", RUNTIME_CLASS(CSettingFile));
		m_pInitOperation->RegisterStnClass("CTimeChart", RUNTIME_CLASS(CTimeChart));

		m_pInitOperation->RegisterStnClass("CShBufferWH", RUNTIME_CLASS(CShBufferWH));
		//m_pInitOperation->RegisterStnClass("CRejectArm", RUNTIME_CLASS(CRejectArm));
		m_pInitOperation->RegisterStnClass("CRejectBelt", RUNTIME_CLASS(CRejectBelt));
		m_pInitOperation->RegisterStnClass("CWH1", RUNTIME_CLASS(CWH1));
		m_pInitOperation->RegisterStnClass("CWH2", RUNTIME_CLASS(CWH2));
		m_pInitOperation->RegisterStnClass("CInPickArm", RUNTIME_CLASS(CInPickArm));
		m_pInitOperation->RegisterStnClass("COutPickArm", RUNTIME_CLASS(COutPickArm));
		//m_pInitOperation->RegisterStnClass("CINWH", RUNTIME_CLASS(CINWH));

		//Create Station and Sub Station
		m_pInitOperation->CreateStation();
		DisplayMessage("Completed Station setup ...");
		KeepResponse();	// 20140714 Yip

		m_Stations.RemoveAll();
		m_PRStations.RemoveAll();

		//Relay to Stn & Sub Stn: Update Pos flag, Profile flag, and m_fHardware
		for (POSITION pos = m_pStationMap.GetStartPosition(); pos;)
		{
			m_pStationMap.GetNextAssoc(pos, szStn, pStn);
			pAppStn = dynamic_cast<CAppStation*>(pStn);

			if (pAppStn)
			{
				pAppStn->SetUpdatePosition(m_fEnableHardware, TRUE);
				pAppStn->SetUpdateProfile(m_fEnableHardware, TRUE);
				pAppStn->SetHardware(m_fEnableHardware, TRUE);
				m_Stations.Add((CPI9000Stn*)pAppStn);
			}
		}

		//Assign All Station ID 
		for (INT i = 0; i < GetStationList().GetSize(); i++)
		{
			GetStationList().GetAt(i)->SetStationID(i);
		}

		// Assign threads pointers
		pCWinEagle			= dynamic_cast<CWinEagle*>(GetStation("WinEagle"));
		//pCPickArm			= dynamic_cast<CPickArm*>(GetStation("PickArm")); 
		//pCInspWH			= dynamic_cast<CInspWH*>(GetStation("InspWH")); 
		pCInspOpt			= dynamic_cast<CInspOpt*>(GetStation("InspOpt"));

		pCSettingFile		= dynamic_cast<CSettingFile*>(GetStation("SettingFile"));
		pCHouseKeeping		= dynamic_cast<CHouseKeeping*>(GetStation("HouseKeeping"));
		pCTimeChart			= dynamic_cast<CTimeChart*>(GetStation("TimeChart"));
		pCTAManager			= dynamic_cast<CTAManager*>(GetStation("TAManager"));

		pCShBufferWH		= dynamic_cast<CShBufferWH*>(GetStation("ShBufferWH"));
		//pCRejectArm			= dynamic_cast<CRejectArm*>(GetStation("RejectArm"));
		pCRejectBelt		= dynamic_cast<CRejectBelt*>(GetStation("RejectBelt"));

		pCWH1		        = dynamic_cast<CWH1*>(GetStation("WH1"));
		pCWH2				= dynamic_cast<CWH2*>(GetStation("WH2"));
		pCInPickArm			= dynamic_cast<CInPickArm*>(GetStation("InPickArm"));
		pCOutPickArm		= dynamic_cast<COutPickArm*>(GetStation("OutPickArm"));
		//pCINWH				= dynamic_cast<CINWH*>(GetStation("INWH"));

		// For PRTaskStn
		//pCInspWH->m_pAppMod		= this;
		pCWinEagle->m_pAppMod	= this;		
		pCInspOpt->m_pAppMod	= this;		
		pCWH1->m_pAppMod	= this;		
		pCWH2->m_pAppMod	= this;	

		// For Forward Extern
		pCWH1->m_pCOtherWH = pCWH2;
		pCWH2->m_pCOtherWH = pCWH1;	


		//Add all station that required PR System
		//m_PRStations.Add((CPRTaskStn*)pCTrayHolder);
		m_PRStations.Add((CPRTaskStn*)pCWH1);
		m_PRStations.Add((CPRTaskStn*)pCWH2);
		m_PRStations.Add((CPRTaskStn*)pCInspOpt);
		// set hardware enable flag accordingly
		// Use as Init Temp Control

		pCWinEagle->SetHardware(m_fEnablePRS);
		//pCPickArm->SetHardware(m_fEnablePICKARM);
		//pCInspWH->SetHardware(m_fEnableINSPWH);
		pCInspOpt->SetHardware(m_fEnableInspOpt);
		pCShBufferWH->SetHardware(m_fEnableShBufferWH);
		//pCRejectArm->SetHardware(m_fEnableRejectArm);
		pCRejectBelt->SetHardware(m_fEnableRejectBelt);
		pCWH1->SetHardware(m_fEnableWH1);
		pCWH2->SetHardware(m_fEnableWH2);
		pCInPickArm->SetHardware(m_fEnableInPickArm);
		pCOutPickArm->SetHardware(m_fEnableOutPickArm);
		//pCINWH->SetHardware(m_fEnableINWH);
		KeepResponse();	// 20140714 Yip

		m_bDiagnMode = m_fEnableDiagnMode;

		BasicExcel				excelDB;
		BasicExcelWorksheet	*curSheet;
		CString str;

		DisplayMessage("Loading Motor Info...");

		// 20140715 Yip: Create Thread For Loading Motor Info Parameters
		if (RunLongTimeFunction(_LoadMotorInfoParam, static_cast<LPVOID>(&excelDB), "Motor Info Load") == 1)
		{
			for (INT i = 0; i < GetStationList().GetSize(); i++)
			{
				curSheet = excelDB.GetWorksheet(GetStationList().GetAt(i)->GetStnName());
				//only print error when that station do not have motors
				if (curSheet != NULL && GetStationList().GetAt(i)->GetmyMotorList().GetSize() != 0)
				{
					for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)
					{
						if (!LoadMotorInfo(*curSheet, GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)))
						{
							str.Format("Loading Motor Info Error! %s", GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stAttrib.szAxis);

							DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
							DisplayMessage(str);
							DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						}
					}
				}
				else if (curSheet == NULL && GetStationList().GetAt(i)->GetmyMotorList().GetSize() != 0)
				{
					str.Format("Loading Motor Info Error! %s", GetStationList().GetAt(i)->GetStnName());
					DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					DisplayMessage(str);
					DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				}
				KeepResponse();	// 20140714 Yip
			}
			excelDB.Close();
			DisplayMessage("Motor Info Loaded");
		}
		else
		{
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			DisplayMessage("Loading Motor Info Error!");
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		}
		KeepResponse();	// 20140714 Yip

		// Initi Nu Drive Here
		m_pCGmpSystem = new CGmpSystem();

		char cFilePath[50];
		char cActiPath[50];

		switch (MachineVersion())
		{
		case VERSION_1:
			strcpy(cFilePath, gszSCF_FLIE_PATH);
			strcpy(cActiPath, gszSCF_ACTI_PATH);
			DisplayMessage("PI9000-1 MACHINE");
			break;
			
		case VERSION_2:
			strcpy(cFilePath, gszSCF_FLIE_PATH_2);
			strcpy(cActiPath, gszSCF_ACTI_PATH_2);
			DisplayMessage("PI9000-2 MACHINE");
			break;

		case VERSION_3:
			strcpy(cFilePath, gszSCF_FLIE_PATH_3);
			strcpy(cActiPath, gszSCF_ACTI_PATH_3);
			DisplayMessage("PI9000-3 MACHINE");
			break;
		}

		{
		//20120612
			UINT uRev = 0, uBuildNum = 0;
			unsigned char cSubRev[10];
			memset(&cSubRev[0], 0, sizeof(unsigned char) * 10);
			m_pCGmpSystem->GetMotionLibRevision(&uRev, &cSubRev[0], &uBuildNum);
			DisplayMessage("**** Gmp Motion library Revision ****");
			str.Format("Revision: %d.%d %s BuildNum = %d.%d", uRev / 100, uRev % 100, &cSubRev[0], uBuildNum / 100, uBuildNum % 100);
			DisplayMessage(str);
			DisplayMessage("****************************************");
			m_szMotionLibVersion.Format("%d.%d %s", uRev / 100, uRev % 100, &cSubRev[0]); //20121210
		}

		if (IsInitNuDrive())
		{
//			m_pCGmpSystem->InitSystem(cFilePath, cActiPath, 8000);
			DisplayMessage("Initializing Nu System...");
			// 20140715 Yip: Create Thread For Initializing Nu System
			GMP_INIT GmpInit = {m_pCGmpSystem, cFilePath, cActiPath};
			RunLongTimeFunction(_InitGmpSystem, static_cast<LPVOID>(&GmpInit), "Init GMP System");
		}

		DisplayMessage("Initialize Nu system completed");
		KeepResponse();	// 20140714 Yip

		//Load Machine and Device
		LoadSoftwareVersion();  

		BOOL	bLoadFileError = FALSE;

		//Load Machine and Device File
		if (LoadMachineFile())
		{
			_time64(&ltime);
			szMsg = _ctime64(&ltime);

			szMsg += _T(": Machine file Loaded");
		}
		else
		{
			bLoadFileError = TRUE;
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			DisplayMessage("ERROR: Unable to load Machine file");
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		}
		KeepResponse();	// 20140714 Yip

		if (LoadDeviceFile())
		{
			_time64(&ltime);
			szMsg = _ctime64(&ltime);

			szMsg += _T(": Device file Loaded");
		}
		else
		{
			bLoadFileError = TRUE;
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			DisplayMessage("ERROR: Unable to load Device file");
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		}
		KeepResponse();	// 20140714 Yip

		if (!bLoadFileError)
		{
		
			if (pCHouseKeeping->m_dPrevSWVersion < pCHouseKeeping->m_dCurSWVersion)
			{
				//Load Machine and Device File
				if (LoadMachineFile(FALSE))
				{
					DisplayMessage("Machine Data Convertion Finished");
				}
				else
				{
					bLoadFileError = TRUE;
					DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					DisplayMessage("ERROR: Unable to save Machine file");
					DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				}
				KeepResponse();	// 20140714 Yip

				if (LoadDeviceFile(FALSE))
				{
					DisplayMessage("Device Data Convertion Finished");
				}
				else
				{
					bLoadFileError = TRUE;
					DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					DisplayMessage("ERROR: Unable to save Device file");
					DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				}
				KeepResponse();	// 20140714 Yip
			}
		}

		//Save Software
		SaveSoftwareVersion();
		KeepResponse();	// 20140714 Yip

		// Init HMI
		ExecuteHmi();

		m_evStart.SetEvent();
		bInit = TRUE;
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return bInit;
}

VOID CPI9000App::SetupIPC()
{
	if (m_fCreated)
	{
		return;
	}

	try
	{
		Create(m_szModuleName, m_szGroupID);
		DisplayMessage("Communication initialized ...");

		// Register Service Commands
		m_comServer.IPC_REG_SERVICE_COMMAND(gszHARDWARE_INITIAL, SrvInitialize);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszSYSTEM_INITIAL, SrvSystemInitialize);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszDIAG_COMMAND, SrvDiagCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszAUTO_COMMAND, SrvAutoCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszMANUAL_COMMAND, SrvManualCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszCYCLE_COMMAND, SrvCycleCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszDEMO_COMMAND, SrvDemoCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszSTOP_COMMAND, SrvStopCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszRESET_COMMAND, SrvResetCommand);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszDE_INITIAL_COMMAND, SrvDeInitialCommand);

		//Jog Mode
		m_comServer.IPC_REG_SERVICE_COMMAND(gszJOG_COMMAND, SrvJogCommand);
		
		//Device
		m_comServer.IPC_REG_SERVICE_COMMAND(gszUPDATE_DEVICE, SrvUpdateDevice);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszCHANGE_DEVICE, SrvChangeDevice);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszNEW_DEVICE, SrvNewDevice);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszPRINT_DEVICE, SrvPrintDevice);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszDELETE_DEVICE, SrvDeleteDevice); //20120703

		//Machine
		m_comServer.IPC_REG_SERVICE_COMMAND(gszUPDATE_MACHINE, SrvUpdateMachine);
		m_comServer.IPC_REG_SERVICE_COMMAND(gszPRINT_MACHINE, SrvPrintMachine);

		// Additional Service Commands registration here

		DisplayMessage("IPC Services registered ...");
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}


/////////////////////////////////////////////////////////////////
// Display Msg & Exception
/////////////////////////////////////////////////////////////////

VOID CPI9000App::DisplayException(CAsmException &e)
{
	CString	szException, szMsg, szCode;
	CString	szMessage = "	EXCEPTION ENCOUNTERED\n";

	//e.What(szException);

	szMsg = m_szModuleName + _T(" - EXCEPTION ENCOUNTER!");
	DisplayMessage(szMsg);

	e.What(szException);
	szMsg.Format("  FUNCTION:   %s", e.Function());
	DisplayMessage(szMsg);

	szMsg.Format("  PARAMETER:  Error Code Number = %d", e.ErrorID());
	DisplayMessage(szMsg);

	if (e.Parameter() != "")
	{
		szMsg.Format("  ERROR TYPE: %s", e.Parameter());
		DisplayMessage(szMsg);
	}
}

VOID CPI9000App::DisplayMessage(const CString &szMessage)
{
	CSingleLock slLock(&m_csDispMsg);

	if (slLock.Lock(300))
	{
		if (m_fLogMessage && m_fCanDisplay)
		{
			if (m_pMainWnd != NULL)
			{
				CPI9000View *pView = (CPI9000View*)((CMainFrame*) m_pMainWnd)->GetActiveView();
			
				if (pView != NULL)
				{
					pView->DisplayMessage(szMessage);

					if (m_fLogFileCreated)
					{
						m_pInitOperation->WriteLogFile(szMessage);
					}
				}
			}
		}
	}

	slLock.Unlock();
}


/////////////////////////////////////////////////////////////////
// CPI9000App Override of SFM_CModule class functions
/////////////////////////////////////////////////////////////////

VOID CPI9000App::Operation()
{
	static INT	nScanCount = 0;

	// Scan Input Variable at about 100 ms interval
	if ((m_qState != UN_INITIALIZE_Q)	&&
		(m_qState != DE_INITIAL_Q)		&&
		++nScanCount >= 10)
	{
		nScanCount = 0;
		ScanInput();
	}

	switch (m_qState)
	{
	case UN_INITIALIZE_Q:
		UnInitialOperation();
		//if (g_pPerformance_report != NULL)
		//{
		//	g_pPerformance_report->m_lBondingStart = m_ulTotalUnitsBonded;    //Philip performance report for initializing the total bond
		//} 
		break;

	case IDLE_Q:
		IdleOperation();
		break;

	case DIAGNOSTICS_Q:
		DiagOperation();
		break;

	case SYSTEM_INITIAL_Q:
		SystemInitialOperation();
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

	case STOPPING_Q:
		StopOperation();
		break;

	case DE_INITIAL_Q:
		DeInitialOperation();
		break;
	}

	Sleep(10);

	UpdateOutput();
}

VOID CPI9000App::UpdateOutput()
{
	static DWORD dwLastMemoryLogTime = 0;

	m_szState = gszMODULE_STATE[m_qState];

	if ((m_qState == UN_INITIALIZE_Q) ||
		(m_qState == DE_INITIAL_Q))
	{
		return;
	}

	if (
		m_qState != SYSTEM_INITIAL_Q &&
		m_qState != PRESTART_Q
	   )
	{ 
	//20121225
		pCHouseKeeping->HMI_bStopProcess = TRUE;
	}
	if (
		(m_qState == SYSTEM_INITIAL_Q) || (m_qState == PRESTART_Q)
		)
	{		
		pCHouseKeeping->SetStartBtnSol(ON);
		Sleep(300);
		pCHouseKeeping->SetStartBtnSol(OFF);
		Sleep(200);
	}
	if (
		(m_qState == AUTO_Q) || (m_qState == DEMO_Q)
		)
	{
		if (!pCHouseKeeping->m_stStartButtonSol.bHMI_Status)
		{
			pCHouseKeeping->SetStartBtnSol(ON);
		}
	}
	if (
		(m_qState == IDLE_Q)
		)
	{
		if (pCHouseKeeping->m_stStartButtonSol.bHMI_Status)
		{
			pCHouseKeeping->SetStartBtnSol(OFF);
		}
	}

	if (g_pPerformance_report != NULL)
	{
		if (g_pPerformance_report->m_bIsRunning)											// Machine is running at auto or test mode, 
		{
//			g_pPerformance_report->CheckIdleUpdate(m_ulTotalUnitsBonded);    //check if the machine is idle, Idle Definition: no output for over 1 minute
		}						
		else
		{
			g_pPerformance_report->m_lLastUnitBondedTime = clock();    //Reset the clock to current time to avoid idle update 
		}						

		/* 20141028 Yip: Correct Reset Time For New Report
		if (g_pPerformance_report->TimeForNextReport())									//check if it is over 24 for next day, next report
		{
			g_pPerformance_report->m_lBondingEnd = m_ulTotalUnitsBonded;								//Record the number of output for the current report (the end point) update for the UPH
			g_pPerformance_report->UpdateReport(m_szDeviceFile);								//Compute and report all the events from last hr
			g_pPerformance_report->m_lBondingStart = m_ulTotalUnitsBonded;							//Record the number of output for the next report (the start point) update for the UPH
			g_pPerformance_report->CopyCurrentReportToHMI(g_ulDailyPerformanceReportPage);		//refresh the report display for user at HMI
			g_pPerformance_report->EndReport();												//close up the current report and open another new report
		}
		*/
		if (g_pPerformance_report->TimeToUpdate())									//check if it is time for reporting the last hr
		{
			// 20141028 Yip: Correct Reset Time For New Report
			if (g_pPerformance_report->TimeForNextReport())									//check if it is over 24 for next day, next report
			{
				g_pPerformance_report->EndReport();												//close up the current report and open another new report
			}
//			g_pPerformance_report->m_lBondingEnd = m_ulTotalUnitsBonded;								//Record the number of output for the current report (the end point) update for the UPH
			g_pPerformance_report->UpdateReport(m_szDeviceFile);								//Compute and report all the events from last hr
//			g_pPerformance_report->m_lBondingStart = m_ulTotalUnitsBonded;							//Record the number of output for the next report (the start point) update for the UPH
			g_pPerformance_report->CopyCurrentReportToHMI(g_ulDailyPerformanceReportPage);		//refresh the report display for user at HMI
		}
	}

	if (/*m_bHMIMess && */m_astrMsgQueue.GetSize() > 0) 
	{
		m_csMsgLock.Lock();
		Sleep(200);
		m_szHMIMess = m_astrMsgQueue[0];
		m_astrMsgQueue.RemoveAt(0);
		m_bHMIMess = FALSE;
		m_csMsgLock.Unlock();
	}

	if (m_qState == STOPPING_Q && !m_bStatusLogged)
	{
		DWORD dwTime = GetTickCount() - m_dwStopTime;

		if (dwTime > 30000)
		{
			GetAllStatus();
			m_bStatusLogged = TRUE;
		}
	}

	// TODO: Add your variables update here
	if (
		(m_qState == IDLE_Q || m_qState == DEMO_Q || m_qState == AUTO_Q || m_qState == CYCLE_Q) &&
		(labs(GetTickCount() - dwLastMemoryLogTime) / CLOCKS_PER_SEC > 3 * 60)
		)
	{
		LogSystemMemoryStatus();
		LogMemoryCpuStatus(labs(GetTickCount() - dwLastMemoryLogTime)); //for host, vision, hmi process
		dwLastMemoryLogTime = GetTickCount();
	}

	// restart HMI?
	if (m_fRestartHmi)
	{
		Sleep(5000);
		DisplayMessage("Restarting HMI...");
		ExecuteHmi();
	}
	//read encoder for motor Software Limit page
	if (m_pStMotorSWLimit != NULL)
	{
		HMI_lSWLimitEncoder = m_pStMotorSWLimit->GetEncPosn();
	}
}

VOID CPI9000App::ScanInput()
{
	if ((m_qState == UN_INITIALIZE_Q) ||
		(m_qState == DE_INITIAL_Q))
	{
		return;
	}

	// TODO: Add your own variables scanning here
}

INT CPI9000App::State()
{
	return m_qState;
}


/////////////////////////////////////////////////////////////////
//	State Operation Functions
/////////////////////////////////////////////////////////////////

VOID CPI9000App::UnInitialOperation()
{
	if (m_bRunState)
	{
		//ExecuteHmi();

#ifndef _DEBUG	// 20140722 Yip: Do Not Create Error Log When Start Up For Debug Mode To Save Time
		// Logging
		if (m_fSelectCreateErrorLog)	// 20150331
		{
			if (CheckPath("D:\\CreateErrorLog.exe"))
			{
				//			system("D:\\CreateErrorLog.exe");
				ExecuteCreateErrorLog();	// 20140714 Yip: Do Not Show Up Command Prompt For CreateErrorLog.exe
				Sleep(500);
			}
			else
			{
				DisplayMessage("Unable to generate the error log file. File not found: D:\\CreateErrorLog.exe");
			}
		}
#endif
		if (CheckPath("D:\\sw\\KillAll.bat"))	// 20151029 kill all motion exe when starting program
		{
			ExecuteKillAll();
			Sleep(500);
		}
		
		// Check if visionNT.exe is running
		HANDLE hProcessSnap = NULL;
		PROCESSENTRY32 pe32 = {0};
		INT nVisionNTCount	= 0;
		INT nAsmHmiCount	= 0;
		INT nGatewayCount	= 0;

		// Take a snapshot of all processes in the system. 
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hProcessSnap != INVALID_HANDLE_VALUE)
		{
			// Fill in the size of the structure before using it. 
			pe32.dwSize = sizeof(PROCESSENTRY32);

			// Walk the snapshot of the processes, and for each process, display information.
			if (Process32First(hProcessSnap, &pe32))
			{
				do
				{
					CString strName = pe32.szExeFile;
					strName = strName.MakeUpper();

					if (strName.Find("VISIONNT.EXE") >= 0)
					{
						nVisionNTCount++;
					}

					if (strName.Find("ASMHMI.EXE") >= 0)
					{
						nAsmHmiCount++;
					}

					if (strName.Find("SOCKETGATEWAY.EXE") >= 0)
					{
						nGatewayCount++;
					}

				} while (Process32Next(hProcessSnap, &pe32));
			}
		}
		CloseHandle(hProcessSnap);

		if (nGatewayCount <= 0 && nAsmHmiCount == 0 && nVisionNTCount == 0)
		{
			ExecuteGateway();
		}
		if (nVisionNTCount != 0 || nAsmHmiCount != 0 || nGatewayCount != 0)
		{
			if (nVisionNTCount != 0)
			{
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("ERROR: VisionNT.exe is running!");
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("*** Please terminate VisionNT.exe and restart program ***");
			}

			if (nAsmHmiCount != 0)
			{
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("ERROR: AsmHmi.exe is running!");
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("*** Please terminate AsmHmi.exe and restart program ***");
			}
			
			if (nGatewayCount != 0)
			{
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("ERROR: SocketGateway.exe is running!");
				DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				DisplayMessage("*** Please terminate SocketGateway.exe and restart program ***");
			}
		}
		else if (InitHardware() && getKlocworkTrue())
		{
			m_qState		= IDLE_Q;
			m_fDeleteMarkIDE = TRUE;
			DisplayMessage(_T("Hardware Initialized - System in IDLE state"));
		}
		else
		{
			DisplayMessage(_T("Hardware fail to Initialize!!!"));
			DisplayMessage(_T("*** Please restart program ***"));
		}

		m_bRunState = FALSE;
	}
}

VOID CPI9000App::IdleOperation()
{
	CString strInitMessage;

	if (m_fDeleteMarkIDE)
	{
		//Purpose: To Delete all Data Created by MarkIDE
		INT	nCount = 0;

		for (POSITION pos = m_pStationMap.GetStartPosition(); pos;)
		{
			CString	szName;
			SFM_CStation *pStation = NULL;
			m_pStationMap.GetNextAssoc(pos, szName, pStation);

			if (!pStation->IsInitialized())
			{
#if 1 //20130830
				static CString	szNamePrevStation(" ");
				if (szNamePrevStation != szName)
				{
					DisplayMessage("Waiting for initialization of:" + szName);
					szNamePrevStation = szName;
				}
#endif
				return;
			}

			nCount++;
		}

		if (nCount == m_pStationMap.GetCount())
		{
			m_fDeleteMarkIDE = FALSE;
			DisplayMessage(_T("Stations Initialized - Stations in IDLE state"));
			//delete and create a null file
			if (CheckPath("D:\\FRAME IV\\Bin\\MarkIDE.exe"))
			{
				// Delete all the MarkIDE application sw
				RenameDir("D:\\FRAME IV\\Bin\\MarkIDE.exe", "D:\\FRAME IV\\Bin\\ClickMe.bak");
			}
			
			HMILangTraditionalChinese(); //20130227

			if (m_bHWInitFatalError) //20130226
			{
				m_bHWInitFatalErrorUI = TRUE; //20130710
				strInitMessage.Format("Initialization with Fatal Error. Please Restart Program");
				WriteHMIMess(strInitMessage);
				DisplayMessage(_T("*** Initialization with Fatal Error. Please Restart Program***"));

				if (IsInitNuDrive() && !pCHouseKeeping->IsAirSupplyOn())
				{
					SetError(IDS_HK_AIR_SUPPLY_ERR);
				}
				return; //No "OK" button at program start. Force user exit program.
			}

			if (m_bHWInitError)
			{
				if (m_bInvalidInitState)
				{
					m_bHWInitFatalErrorUI = TRUE; //20130710
					strInitMessage.Format("*** Please Remove Glass in the Machine and Restart Program***");
					DisplayMessage(_T("*** Please Remove Glass in the Machine and Restart Program***"));
					return;
				}

				strInitMessage.Format("TEST__INIT ERROR!");
				WriteHMIMess(strInitMessage);
				m_bInitInstanceError = TRUE;
				m_bHWInitError = FALSE; // Must Set, otherwise cannot enter UpdateOutput

				if (m_fEnableInitInstanceError)
				{
					for (int i = 0; i < GetStationList().GetSize(); i++)
					{
						if (GetStationList().GetAt(i)->IsInitInstanceError())
						{
							strInitMessage.Format(GetStationList().GetAt(i)->GetStnName() + "__INIT ERROR!");
							WriteHMIMess(strInitMessage);
						}
					}
					
					WriteHMIMess("Please RE-INIT");
					m_bInitInstanceError = TRUE;
					m_bHWInitError = FALSE; // Must Set, otherwise cannot enter UpdateOutput
				}
				else
				{
					m_bHWInitFatalErrorUI = TRUE; //20130710
					// add turn off all motors
					WriteHMIMess("*** Please restart program ***");
					DisplayMessage(_T("*** Please restart program ***"));
					return;
				}
			}

			// move motors to standby
			if (m_bDiagnMode)
			{
				//// Set E/P regulators
		
				strInitMessage = "Initalization DONE";
				m_bHWInited = TRUE;
			}
			else
			{
				if (!m_bInitInstanceError)
				{
					strInitMessage = "Initalization DONE";

					if (
						pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_NOWAIT) != GMP_SUCCESS ||
						pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
						pCInPickArm->m_stMotorX.Sync() != GMP_SUCCESS ||
						pCInPickArm->m_stMotorZ.Sync() != GMP_SUCCESS ||
						
						pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_NOWAIT) != GMP_SUCCESS ||
						pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, GMP_NOWAIT) != GMP_SUCCESS ||
						pCOutPickArm->m_stMotorX.Sync() != GMP_SUCCESS ||
						pCOutPickArm->m_stMotorZ.Sync() != GMP_SUCCESS ||

						pCWH1->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_NOWAIT) != GMP_SUCCESS ||
						pCWH1->MoveTToStandby() != GMP_SUCCESS ||
						pCWH1->m_stMotorT.Sync() != GMP_SUCCESS ||
						pCWH1->m_stMotorY.Sync() != GMP_SUCCESS ||

						pCWH2->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_NOWAIT) != GMP_SUCCESS ||
						pCWH2->MoveTToStandby() != GMP_SUCCESS ||
						pCWH2->m_stMotorT.Sync() != GMP_SUCCESS ||
						pCWH2->m_stMotorY.Sync() != GMP_SUCCESS ||

						pCInspOpt->MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_NOWAIT) != GMP_SUCCESS ||
						pCInspOpt->MoveZToPRLevel() != GMP_SUCCESS ||
						pCInspOpt->m_stMotorX.Sync() != GMP_SUCCESS ||
						pCInspOpt->m_stMotorZ.Sync() != GMP_SUCCESS 
					   )
					{
						strInitMessage = "Initalization DONE (Move Motor To Standby Error)";
					}
				}

				if (IsInitNuDrive())	//offline 20120302
				{
				}
				// Add me later
				// Motor Move to Standby Pos			


				m_bHWInited = TRUE;
			}

			if (g_bIsVisionNTInited && IsInitNuDrive())	// 20140623 Yip
			{
				// 20140715 Yip: Create Thread For Switching All Cameras
				RunLongTimeFunction(_SwitchAllCam, static_cast<LPVOID>(pCWinEagle), "Switch All Cameras");
			}
		}
	}
}

VOID CPI9000App::DiagOperation()
{
	if (CheckStationMotion() == MOTION_COMPLETE)
	{
		DisplayMessage(_T("Diagnostic operation completed"));
		m_qState = IDLE_Q;
	}
}

VOID CPI9000App::SystemInitialOperation()
{
	// Signal all stations to perform prestart operation
	SFM_CStation *pStation = NULL;
	CAppStation  *pAppStation;
	POSITION	  pos;
	CString		  szKey;
	BOOL		  bError = FALSE;

	if (CheckStationMotion() == MOTION_COMPLETE)
	{
		DisplayMessage(_T("Initialization completed"));

		//check for error during Initialization
		for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
		{
			m_pStationMap.GetNextAssoc(pos, szKey, pStation);
			pAppStation = dynamic_cast<CAppStation*>(pStation);

			if (pAppStation->Result() == gnNOTOK)
			{
				bError = TRUE;
				break;
			}
		}

		if ((m_nOperation != NULL_OP) && (bError != TRUE))
		{
			// Signal all stations to perform prestart operation
			for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);

				if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
				{
					pAppStation->Motion();
					pAppStation->Command(glPRESTART_COMMAND);
					pAppStation->Result(gnOK);
				}
			}

			DisplayMessage(_T("Perform PreStart Command"));
			m_qState = PRESTART_Q;
		}
		else
		{
			m_qState	= IDLE_Q;
			m_nOperation = NULL_OP;
			m_lAction	= glNULL_COMMAND;
		}
	}
}

VOID CPI9000App::PreStartOperation()
{
	//If PreStart Operation Completed, Signal All station to perform: Auto ot Demo
	//Operation (depending on the Command)
	SFM_CStation	*pStation = NULL;
	CAppStation		*pAppStation;
	POSITION		pos;
	CString			szKey;

	if (CheckStationMotion() == MOTION_COMPLETE)
	{
#if 1 //20121124
		__time64_t ltime;
		CString szTime = "";

		_time64(&ltime);
		szTime = _ctime64(&ltime);
		szTime.TrimRight('\n');
#endif
		// reset
		DisplayMessage(_T("PreStart operation completed"));

		if ((m_nOperation == AUTO_OP) || (m_nOperation == DEMO_OP) || (m_nOperation == CYCLE_OP))
		{
			for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);

				if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
				{
					pAppStation->Motion();
					pAppStation->SetAxisAction(0, m_lAction, 0);
					pAppStation->Result(gnOK);

					switch (m_nOperation)
					{
					case AUTO_OP:
						pAppStation->Command(glAUTO_COMMAND);
						break;

					case DEMO_OP:
						pAppStation->Command(glDEMO_COMMAND);
						break;

					case CYCLE_OP:
						pAppStation->Command(glCYCLE_COMMAND);
						break;
					}
				}
			}

			switch (m_nOperation)
			{
			case AUTO_OP:
				if (AutoMode == DIAG_BOND)
				{
					g_bDiagMode = TRUE;
				}
				DisplayMessage(_T("Perform Auto Operation at ") + szTime);
				WriteHMIMess(_T("*** Perform Auto Operation ***") + szTime);

				m_qState = AUTO_Q;
				break;

			case DEMO_OP:
				if (AutoMode == DIAG_BOND)
				{
					g_bDiagMode = TRUE;
				}
#if 1 //20121124
				DisplayMessage(_T("Perform Demo Operation at ") + szTime);
				WriteHMIMess(_T("*** Perform Demo Operation at ") + szTime + _T("***"));
#else
				DisplayMessage(_T("Perform Demo Operation"));
				WriteHMIMess(_T("*** Perform Demo Operation ***"));
#endif
				m_qState = DEMO_Q;
				break;

			case CYCLE_OP:
				DisplayMessage(_T("Perform Cycle Test"));
				WriteHMIMess(_T("*** Perform Cycle Test ***"));
				m_qState = CYCLE_Q;
				break;

			default:
				DisplayMessage(_T("Invalid Operation Encountered!"));
				m_qState = IDLE_Q;
				break;
			}
		}
		else
		{
			m_nOperation	= NULL_OP;
			m_lAction		= glNULL_COMMAND;
			m_qState		= IDLE_Q;
			DisplayMessage(_T("System Initialization completed"));
		}
	}
}

VOID CPI9000App::AutoOperation()
{
	if (CheckStationMotion() == MOTION_COMPLETE)
	{
		DisplayMessage(_T("Auto Operation completed"));

		m_qState	= IDLE_Q;
		m_nOperation = NULL_OP;
		m_lAction	= glNULL_COMMAND;
	}
}

VOID CPI9000App::DemoOperation()
{
	if (CheckStationMotion() == MOTION_COMPLETE)
	{
		DisplayMessage(_T("Demonstration Operation completed"));

		m_qState	= IDLE_Q;
		m_nOperation = NULL_OP;
		m_lAction	= glNULL_COMMAND;
	}
}

VOID CPI9000App::CycleOperation()
{
	if (CheckStationMotion() == MOTION_COMPLETE)
	{
		DisplayMessage(_T("Cycle Test completed"));

		m_qState	= IDLE_Q;
		m_nOperation = NULL_OP;
		m_lAction	= glNULL_COMMAND;
	}
}

VOID CPI9000App::ManualOperation()
{
	if (CheckStationMotion() == MOTION_COMPLETE)
	{
		m_qState	= IDLE_Q;
		m_nOperation = NULL_OP;
		m_lAction	= glNULL_COMMAND;

		DisplayMessage(_T("Manual operation completed"));
	}
}

VOID CPI9000App::StopOperation()
{
	if (CheckStationMotion() == MOTION_COMPLETE)
	{
		if (m_lAction == glSYSTEM_EXIT)
		{
			((CFrameWnd*) m_pMainWnd)->PostMessage(WM_CLOSE);
		}

		if (m_fStopMsg)
		{
			g_bDiagMode = FALSE;

			try
			{
				// 20140728 Yip: Display Time When Operation Stopped
				__time64_t ltime;
				_time64(&ltime);
				CString szTime = _ctime64(&ltime);
				DisplayMessage(_T("[") + szTime + _T("]:") + _T("Operation Stopped!"));

				pCHouseKeeping->UnLockAllCovers();
				DisplayMessage("UnLock all door");

				// 20140801 Yip: Display Bonding Statistics When Operation Stopped
				DisplayMessage("***********************************************");
				DisplayMessage("Bonding Statistics:");
				CString szMsg;
				szMsg.Format("UPH: %.2lf", m_dUPH);
				DisplayMessage(szMsg);
				szMsg.Format("Glass Picked: %ld", m_ulGlassPicked);
				DisplayMessage(szMsg);
				szMsg.Format("Glass Rejected: %ld", m_ulGlassRejected);
				DisplayMessage(szMsg);
				szMsg.Format("Glass Inspected: %ld", m_ulUnitsInpected);
				DisplayMessage(szMsg);
				DisplayMessage("***********************************************");

				TRACE0("\n****** Stop Operation ******\n");
			}
			catch (CAsmException e)
			{
				DisplayException(e);
			}
		}

		m_qState	= IDLE_Q;
		m_nOperation = NULL_OP;
		m_lAction	= glNULL_COMMAND;

		PRS_AutoBondMode(FALSE, FALSE);

		// Add me later
		// save statistics
		m_smfMachine["Stats"]["m_ulTotalGlassPicked"]				= m_ulTotalGlassPicked;
		m_smfMachine["Stats"]["m_ulTotalGlassRejected"]				= m_ulTotalGlassRejected;
		m_smfMachine["Stats"]["m_ulTotalUnitsInpected"]				= m_ulTotalUnitsInpected;
		m_smfMachine["Stats"]["m_ulTotalDLPRError"]					= m_ulTotalDLPRError;
		m_smfMachine["Stats"]["m_ulTotalInspOptPRError"]			= m_ulTotalInspOptPRError;
		m_smfMachine["Stats"]["m_ulTotalGlassTolError"]				= m_ulTotalGlassTolError;


		m_smfMachine.Update();
	}
}

VOID CPI9000App::DeInitialOperation()
{
	SFM_CStation *pStation = NULL;
	CAppStation  *pAppStation;
	POSITION	  pos;
	CString		  szKey;
	CString		  szName;
	INT			  nCount = 0;

	for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
	{
		m_pStationMap.GetNextAssoc(pos, szKey, pStation);
		pAppStation = dynamic_cast<CAppStation*>(pStation);

		if (pAppStation->IsReInitialized())
		{
			nCount++;
		}
		else
		{
			break;
		}
	}

	if (nCount == m_pStationMap.GetCount())
	{
		m_qState = IDLE_Q;
		DisplayMessage("De-Initialize/Re-Initialize Operation Completed, Systems in IDLE state");
	}
}


/////////////////////////////////////////////////////////////////
//	Service Command Functions
/////////////////////////////////////////////////////////////////

LONG CPI9000App::SrvInitialize(IPC_CServiceMessage &svMsg)
{
	SMotCommand	smCommand;
	CString		szDisplay;
	BOOL		bResult = FALSE;

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(SMotCommand), &smCommand);

		// TODO: Process the received command and perform the initialization
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvSystemInitialize(IPC_CServiceMessage &svMsg)
{
	SMotCommand		smCommand;
	CString			szDisplay;
	SFM_CStation	*pStation = NULL;
	CAppStation	*pAppStation;
	POSITION		pos;
	CString			szKey;
	BOOL			bResult = FALSE;

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(smCommand), &smCommand);

		szDisplay.Format(_T("%s - Receive System Initialize command: %d"), m_szModuleName, smCommand.lAction);
		DisplayMessage(szDisplay);

		if (m_qState == IDLE_Q)
		{
			m_lAction = smCommand.lAction;
			m_nOperation = NULL_OP;

			for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);
				pAppStation = dynamic_cast<CAppStation*>(pStation);

				pAppStation->Motion(TRUE);
				pAppStation->Result(gnOK);
				pAppStation->SetAxisAction(smCommand.lAxis, smCommand.lAction, smCommand.lUnit);

				pAppStation->Command(glINITIAL_COMMAND);
			}

			m_qState = SYSTEM_INITIAL_Q;
			bResult = TRUE;
		}
		else
		{
			szDisplay.Format(_T("%s - Initialize command canceled, Systems not in IDLE state"), m_szModuleName);
			DisplayMessage(szDisplay);

			bResult = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvDiagCommand(IPC_CServiceMessage &svMsg)
{
	struct
	{
		LONG			lStation;
		SMotCommand		smCommand;
	} smDiagCommand;

	CString	szDisplay;
	BOOL	bResult = FALSE;

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(smDiagCommand), &smDiagCommand);

		szDisplay.Format(_T("%s - Receive Diagnostic command"), m_szModuleName);
		DisplayMessage(szDisplay);

		szDisplay.Format(_T("   Station: %d, Axis: %d, Action: %d, Unit: %d"),
						 smDiagCommand.lStation, smDiagCommand.smCommand.lAxis,
						 smDiagCommand.smCommand.lAction, smDiagCommand.smCommand.lUnit);
		DisplayMessage(szDisplay);

		if (m_qState == IDLE_Q)
		{
			CAppStation *pStation		= NULL;
			CString		 szStationName	= m_pInitOperation->GetStationName(smDiagCommand.lStation);

			if ((pStation = dynamic_cast<CAppStation*>(GetStation(szStationName))) != NULL)
			{
				pStation->SetAxisAction(smDiagCommand.smCommand.lAxis,
										smDiagCommand.smCommand.lAction, 
										smDiagCommand.smCommand.lUnit);

				pStation->Motion(TRUE);
				pStation->Command(glDIAGNOSTIC_COMMAND);

				m_qState = DIAGNOSTICS_Q;
				bResult = TRUE;
			}
			else
			{
				szDisplay.Format(_T("%s - Diagnostic command canceled, Station Name not found"), m_szModuleName);
				DisplayMessage(szDisplay);
			}

		}
		else
		{
			szDisplay.Format(_T("%s - Diagnostic command canceled, Systems not in IDLE state"), m_szModuleName);
			DisplayMessage(szDisplay);

			bResult = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvAutoCommand(IPC_CServiceMessage &svMsg)
{
	SMotCommand		smCommand;
	POSITION		pos;
	CString			szDisplay	= _T("");
	CString			szKey		= _T("");
	SFM_CStation	*pStation	= NULL;
	CAppStation	*pAppStation	= NULL;
	BOOL			bResult		= FALSE;
	BOOL			bError		= FALSE;
	LONG			lCurMode	= NORMAL_BONDING;

	pCHouseKeeping->HMI_bStopProcess = FALSE; //20121225
	pCHouseKeeping->HMI_bChangeTeflon = FALSE;	// 20140911 Yip

	try
	{
		if (m_qState == IDLE_Q)
		{
			pCHouseKeeping->SetResetBtnSol(OFF);
			pCHouseKeeping->m_bEnableStartBtn	= FALSE;
			pCHouseKeeping->m_bEnableResetBtn	= FALSE;
			pCHouseKeeping->m_bEnableStopBtn	= TRUE;
			pCHouseKeeping->m_bPressBtnToStop   = FALSE;
			pCHouseKeeping->SetResetBtnSol(OFF);
			// Decipher the receive request command
			svMsg.GetMsg(sizeof(SMotCommand), &smCommand);

			__time64_t ltime;
			_time64(&ltime);
			CString szTime = _ctime64(&ltime);
			szDisplay = _T("[") + szTime + _T("]:") + _T("Receive Auto command");

			m_dwOpStartTime = GetTickCount();
#if 1 //20120806 debug only
			m_clkBondStartTime = clock();
#endif

			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			DisplayMessage(szDisplay);
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		


			// Check Air supply mode
			if (!pCHouseKeeping->m_bDetectAirSupply) //20130412
			{
				pCHouseKeeping->SetError(IDS_HK_AIR_SUPPLY_DETECTION_ENABLE_ERR);
				bError = TRUE;
			}
			//check air supply
			if (AutoMode != BURN_IN && !pCHouseKeeping->IsAirSupplyOn())
			{
				pCHouseKeeping->SetError(IDS_HK_AIR_SUPPLY_ERR);
				bError = TRUE;
			}
			
			g_bFlushMode = FALSE;


			if (!bError)   //20130227
			{
				for (INT i = 0; i < GetStationList().GetSize(); i++) //20141204 update the m_bModSelected for all stations
				{
					GetStationList().GetAt(i)->CheckModSelected(FALSE, TRUE);
				}

				// Autobond mode
				if (AutoMode == AUTO_BOND)
				{
					bError = TRUE;

					if (CheckAutoBondSelected(NORMAL_BONDING))
					{
						lCurMode = NORMAL_BONDING;
						bError = FALSE;
					}
					else if (CheckAutoBondSelected(REWORK_PB))
					{
						lCurMode = REWORK_PB;
						bError = FALSE;
					}
					else if (CheckAutoBondSelected(REWORK_MB1))
					{
						lCurMode = REWORK_MB1;
						bError = FALSE;
					}				
					else if (CheckAutoBondSelected(REWORK_MB2))
					{
						lCurMode = REWORK_MB2;
						bError = FALSE;
					}	

					if (bError)
					{
						// Display Error at here
						for (INT i = 0; i < GetStationList().GetSize(); i++)
						{
							GetStationList().GetAt(i)->CheckModSelected(TRUE);
						}

						for (INT i = 0; i < GetPRStationList().GetSize(); i++)
						{
							GetPRStationList().GetAt(i)->CheckPRModSelected(TRUE);
						}
					}
					else 
					{
						switch (lCurMode)
						{
						case REWORK_MB1:
							DisplayMessage("Select Rework MB1");
							break;
						case REWORK_MB2:
							DisplayMessage("Select Rework MB2");
							break;
						case REWORK_PB:
							DisplayMessage("Select Rework PB");
							break;
						}

						if (!CheckPackageStatus((AutoBondRequirment)lCurMode))
						{
							bError = TRUE;
						}
					}
				}
				else
				{
					if (AutoMode == INDEXING && g_ulUserGroup < SERVICE) //20130823
					{
						UnselectModuleForIndexingMode();
					}

					//CheckTrayHolderStatus(); //20130328

	
#if 1 //20130726
					//check all TAx
					if (AutoMode == TEST_BOND || AutoMode == BURN_IN)
					{
						CString szPromptStr = _T(" ");
						BOOL bIsPrompt = FALSE;
						//if (AutoMode == BURN_IN)	// 20150515 Unselect all PR module when use burninmode
						//{
						//	for (int i = 0; i < GetPRStationList().GetSize(); i++)
						//	{
						//		GetPRStationList().GetAt(i)->SetPRSelected(FALSE);
						//	}
						//}
						if (!pCInPickArm->m_bModSelected)
						{
							szPromptStr.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
							bIsPrompt = TRUE;
						}
						if (!pCOutPickArm->m_bModSelected)
						{
							szPromptStr.Format("%s Module Not Selected. Operation Abort!", pCOutPickArm->GetStnName());
							bIsPrompt = TRUE;
						}
						if (bIsPrompt)
						{
							if (AutoMode == TEST_BOND)
							{
								HMIMessageBox(MSG_OK, "WARNING", szPromptStr);
								bError = TRUE;
							}
							if (AutoMode == BURN_IN)
							{
								LONG lAnswer = 0;
								lAnswer = HMIMessageBox(MSG_YES_NO, "BURN-IN", "Not All Transfer Arm Module are Selected, Continue?");
								if (lAnswer == rMSG_TIMEOUT || lAnswer == rMSG_NO)
								{
									bError = TRUE;
								}
							}
						}
					}
#endif
#if 1 //20121226
					if (AutoMode == INDEXING)
					{
						CString szPromptStr = _T("       ");
						BOOL bIsPrompt = FALSE;
						if (!pCInPickArm->m_bModSelected)
						{
							szPromptStr.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
							bIsPrompt = TRUE;
						}
						//if (!pCWH1->m_bModSelected)
						//{
						//	szPromptStr.Format("%s Module Not Selected. Operation Abort!", pCWH1->GetStnName());
						//	bIsPrompt = TRUE;
						//}
						//if (!pCWH2->m_bModSelected)
						//{
						//	szPromptStr.Format("%s Module Not Selected. Operation Abort!", pCWH2->GetStnName());
						//	bIsPrompt = TRUE;
						//}
						if (!pCOutPickArm->m_bModSelected)
						{
							szPromptStr.Format("%s Module Not Selected. Operation Abort!", pCOutPickArm->GetStnName());
							bIsPrompt = TRUE;
						}

						if (bIsPrompt)
						{
							HMIMessageBox(MSG_OK, "WARNING", szPromptStr);
							bError = TRUE;
						}
					}
#endif
				}
			}
			else //20130425
			{
				DisplayMessage("Auto command abort! Since Heater/AirSupply problem");
			}

			if (!bError && m_qState == IDLE_Q && !m_bDiagnMode)
			{
				pCHouseKeeping->LockAllCovers();
				DisplayMessage("Lock all door");
			}
			if (!bError && m_qState == IDLE_Q && !m_bDiagnMode)
			{
#if 1 //20141124
				m_csLogErrorAlertLock.Lock();
				CString szInFilePath2("D:\\sw\\PI9000\\LogFileDir\\");
				CString szInFileName2("ErrorAlarm.csv");
				CheckFileSize(szInFilePath2, szInFileName2, 512); //512KB
				LogErrorAlarm(CString("START_BONDING"));
				m_csLogErrorAlertLock.Unlock();
#endif

				// reset stats
				m_ulGlassPicked = 0;
				m_ulGlassRejected = 0;
				m_ulGlassTolError = 0;
				m_ulUnitsInpected = 0;
				m_ulDLG1PRError = 0;
				m_ulDLG2PRError = 0;
				m_ulInspOptPRError = 0;

				m_dUPH = 0.0;

				m_lAction		= smCommand.lAction;
				m_nOperation	= AUTO_OP;
				m_qState		= SYSTEM_INITIAL_Q;

				// Signal all stations to perform system initialization
				for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
				{
					m_pStationMap.GetNextAssoc(pos, szKey, pStation);

					if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
					{
						pAppStation->Motion();
						pAppStation->Command(glINITIAL_COMMAND);
						pAppStation->Result(gnOK);
					}
				}

				bResult	= TRUE;
			}
		}
		else
		{
			szDisplay.Format(_T("%s - Auto command canceled, Systems not in IDLE state"), m_szModuleName);
			DisplayMessage(szDisplay);

			bResult = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	if (g_pPerformance_report != NULL)
	{
		if (AutoMode == AUTO_BOND)
		{
			g_pPerformance_report->ReportEventTime(MPRT_START, MPRM_AUTO);    //report the event to the temporary memory, a vector
		}		
		else if (AutoMode == TEST_BOND)
		{
			g_pPerformance_report->ReportEventTime(MPRT_START, MPRM_TEST);    //report the event to the temporary memory, a vector
		}		
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvManualCommand(IPC_CServiceMessage &svMsg)
{
	SMotCommand			smCommand;
	CString				szDisplay	= _T("");
	BOOL				bResult		= FALSE;

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(SMotCommand), &smCommand);

		szDisplay.Format(_T("%s - Receive Manual command"), m_szModuleName);
		DisplayMessage(szDisplay);
		
		if (m_qState == IDLE_Q)
		{
			// TODO: Process the received command and perform the Manual operation
			bResult = TRUE;
		}
		else
		{
			szDisplay.Format(_T("%s - Manual command canceled, Systems not in IDLE state"), m_szModuleName);
			DisplayMessage(szDisplay);

			bResult = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvCycleCommand(IPC_CServiceMessage &svMsg)
{
	SMotCommand		smCommand;
	POSITION		pos;
	CString			szDisplay	= _T("");
	CString			szKey		= _T("");
	SFM_CStation	*pStation	= NULL;
	CAppStation	*pAppStation	= NULL;
	BOOL			bResult		= FALSE;

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(SMotCommand), &smCommand);

#if 1 //20130412
		__time64_t ltime;
		_time64(&ltime);
		CString szTime = _ctime64(&ltime);
		CString szMsg = "";
		szMsg.Format(_T("%s - Receive Cycle command"), m_szModuleName);
		szDisplay = _T("[") + szTime + _T("]:") + szMsg; //20130411
#else
		szDisplay.Format(_T("%s - Receive Cycle command"), m_szModuleName);
#endif
		DisplayMessage(szDisplay);
		
		if (m_qState == IDLE_Q && !m_bDiagnMode)
		{
			m_lAction		= smCommand.lAction;
			m_nOperation	= CYCLE_OP;
			m_qState		= SYSTEM_INITIAL_Q;

			// Signal all stations to perform Cycle Test operation
			for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);

				if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
				{
					pAppStation->Motion();
					pAppStation->Result(gnOK);
					pAppStation->SetAxisAction(0, m_lAction, 0);
					pAppStation->Command(glINITIAL_COMMAND);
				}
			}

			bResult	= TRUE;
		}
		else
		{
			if (m_qState != IDLE_Q)
			{
				szDisplay.Format(_T("%s - Cycle command canceled, Systems not in IDLE state"), m_szModuleName);
			}
			else
			{
				HMIMessageBox(MSG_OK, "ERROR", "Systems initialized in DIAGN mode");
				szDisplay.Format(_T("%s - Cycle command canceled, Systems in DIAGN mode"), m_szModuleName);
			}
			DisplayMessage(szDisplay);

			bResult = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvDemoCommand(IPC_CServiceMessage &svMsg)
{
	SMotCommand		smCommand;
	POSITION		pos;
	CString			szDisplay	= _T("");
	CString			szKey		= _T("");
	SFM_CStation	*pStation	= NULL;
	CAppStation	*pAppStation	= NULL;
	BOOL			bResult		= FALSE;

	pCHouseKeeping->HMI_bStopProcess = FALSE; //20121225
	pCHouseKeeping->HMI_bChangeTeflon = FALSE;	// 20140911 Yip

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(SMotCommand), &smCommand);

		szDisplay.Format(_T("%s - Receive Demostration command"), m_szModuleName);
		DisplayMessage(szDisplay);
		
		if (m_qState == IDLE_Q) 
		{
			m_lAction		= smCommand.lAction;
			m_nOperation	= DEMO_OP;
			m_qState		= SYSTEM_INITIAL_Q;

			// Signal all stations to perform Demo Cycle operation
			for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);

				if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
				{
					pAppStation->Motion();
					pAppStation->Result(gnOK);
					pAppStation->SetAxisAction(0, m_lAction, 0);
					pAppStation->Command(glINITIAL_COMMAND);
				}
			}

			bResult			= TRUE;
		}
		else
		{
			szDisplay.Format(_T("%s - Demostration command canceled, Systems not in IDLE state"), m_szModuleName);
			DisplayMessage(szDisplay);

			bResult = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayException(e);
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvStopCommand(IPC_CServiceMessage &svMsg)
{
	SMotCommand		smCommand;
	POSITION		pos;
	CString			szDisplay	= _T("");
	CString			szKey		= _T("");
	SFM_CStation	*pStation	= NULL;
	CAppStation	*pAppStation	= NULL;
	BOOL			bResult		= FALSE;

	pCHouseKeeping->m_bEnableStartBtn	= TRUE;
	pCHouseKeeping->m_bEnableResetBtn	= TRUE;
	pCHouseKeeping->m_bEnableStopBtn	= FALSE;
	try
	{
		if (g_pPerformance_report != NULL)
		{
			g_pPerformance_report->ReportEventTime(MPRT_STOP);    //report the event to the temporary memory, a vector
		}			

		// Decipher the receive request command
		svMsg.GetMsg(sizeof(SMotCommand), &smCommand);
		m_lAction = smCommand.lAction;

		__time64_t ltime;
		_time64(&ltime);
		CString szTime = _ctime64(&ltime);
		CString szMsg = "";
		szMsg.Format(_T("%s - Receive Stop command"), m_szModuleName);
		szDisplay = _T("[") + szTime + _T("]:") + szMsg; //20130411
		//szDisplay.Format(_T("%s - Receive Stop command"), m_szModuleName);
		DisplayMessage(szDisplay);

		if (m_lAction == glCOMPLETE_STOP)
		{
			for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);
				pAppStation = dynamic_cast<CAppStation*>(pStation);

				if (pAppStation->State() == ERROR_Q)
				{
					m_lAction = glCYCLE_STOP;
					DisplayMessage("Auto convert COMPLETE_STOP to CYCLE_STOP");
					break;
				}
			}
		}

		if (m_lAction == glSYSTEM_EXIT)
		{
			SendAlarm(_T("OS011"));
			TRACE0("\n****** Exit System ******\n");

			CSingleLock slLock(&m_csDispMsg);
			slLock.Lock();
			m_fCanDisplay = FALSE;
			slLock.Unlock();
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	try
	{
		// Signal all stations to perform Stop operation
		for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
		{
			m_pStationMap.GetNextAssoc(pos, szKey, pStation);

			if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
			{
				if ((m_lAction == glSYSTEM_EXIT) ||
					(m_lAction == glEMERGENCY_STOP))
				{
					pAppStation->SetAxisAction(0, glABORT_STOP, 0);
				}
				else
				{
					pAppStation->SetAxisAction(0, m_lAction, 0);
				}

				pAppStation->Command(glSTOP_COMMAND);
				pAppStation->Result(gnOK);
			}
		}

		if (m_qState != IDLE_Q)
		{
			m_fStopMsg = TRUE;
		}
		else
		{
			m_fStopMsg = FALSE;
		}

		m_qState = STOPPING_Q;
		m_dwStopTime = GetTickCount();
		m_bStatusLogged = FALSE;
		bResult	= TRUE;
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

#if 1 //20141210
	CString szInFilePath("D:\\sw\\PI9000\\LogFileDir\\");
	DeleteOldFiles(szInFilePath, CString("PI9000MsgLog*.mlg"), 7);	//20141208
	DeleteOldFiles(szInFilePath, CString("PI9000MsgLog*.txt"), 7);
	DeleteOldFiles(szInFilePath, CString("ErrorAlarm_*.csv"), 7);
	DeleteOldFiles(szInFilePath, CString("StationStatus*.log"), 7);
	szInFilePath = "D:\\sw\\PI9000\\Data\\Calculation Log\\";
	DeleteOldFiles(szInFilePath, CString("*.log"), 14);
	DeleteOldFiles(szInFilePath, CString("*.csv"), 14);
#endif
	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

//p20120829: log result onto a text file after pressing stop button
LONG CPI9000App::HMI_StatisticsLogFile(IPC_CServiceMessage &svMsg)
{
	UINT		MAX_NUM_OF_STAT_FILES	= 10;
	BOOL		bOpDone					= TRUE;
	CString		szStatFilePath			= "";

	// 20140814 Yip: Log Statistic Into File By Date Instead Of By File Count
	SYSTEMTIME ltime;
	GetLocalTime(&ltime);
	szStatFilePath.Format("D:\\sw\\PI9000\\Data\\StatisticsLogFile\\StatisticsLogFile_%04u%02u%02u.txt", ltime.wYear, ltime.wMonth, ltime.wDay);
//	szStatFilePath.Format("D:\\sw\\PI9000\\Data\\StatisticsLogFile\\StatisticsLogFile%ld.txt", m_lStatisticsLogCount);
	
	//Create Directory if not exist
	CFileFind fileFind;
	INT nFind = m_szDeviceFile.ReverseFind('.');
	if (!fileFind.FindFile("D:\\sw\\PI9000\\Data\\StatisticsLogFile\\"))
	{
		m_pInitOperation->CreateFileDirectory("D:\\sw\\PI9000\\Data\\StatisticsLogFile\\");
	}

	// Open file to record UpLook PR Position
	FILE *fp = fopen(szStatFilePath, "at");	// 20140814 Yip: Use "at" Instead Of "wt"
	if (fp != NULL)
	{
		clock_t clkNow;
		__time64_t ltime;
		CString szTime = "";

		clkNow = clock();
		_time64(&ltime);
		szTime = _ctime64(&ltime);
		szTime.TrimRight('\n');

		fprintf(fp, "%s\nOperation Time in sec = %.3f\n", (const char*)szTime, (DOUBLE)(clkNow - m_clkBondStartTime) / CLOCKS_PER_SEC);

		fprintf(fp, "******Statistic Log of Most Recent Trial******\n");

		fprintf(fp, "Glass Picked = %lu\n", m_ulGlassPicked);
		fprintf(fp, "Glass Rejected = %lu\n", m_ulGlassRejected);
		fprintf(fp, "Glass Inpected = %lu\n", m_ulUnitsInpected);
		fprintf(fp, "Glass Tol. Error = %lu\n", m_ulGlassTolError);
		fprintf(fp, "DL G1 PR Error = %lu\n", m_ulDLG1PRError);
		fprintf(fp, "DL G2 PR Error = %lu\n", m_ulDLG2PRError);
		fprintf(fp, "InspOpt PR Error = %lu\n", m_ulInspOptPRError);
		
		fprintf(fp, "Last Error Before Stop = %s\n", (char*)(pCHouseKeeping->m_bErrorExist ? pCHouseKeeping->m_szLastError.GetBuffer(0) : "No Error"));	// 20140814 Yip

		fprintf(fp, "\n");
		fprintf(fp, "**********Statistic Log of All Trials**********\n");		

		fprintf(fp, "Total Glass Picked = %lu\n", m_ulTotalGlassPicked);
		fprintf(fp, "Total Glass Rejected = %lu\n", m_ulTotalGlassRejected);
		fprintf(fp, "Total Units Inpected = %lu\n", m_ulTotalUnitsInpected);
		fprintf(fp, "Total Glass Tol. Error = %lu\n", m_ulTotalGlassTolError);
		fprintf(fp, "Total DL PR Error = %lu\n", m_ulTotalDLPRError);
		fprintf(fp, "Total InspOpt PR Error = %lu\n", m_ulTotalInspOptPRError);

		fprintf(fp, "\n\n");
		fprintf(fp, "***********************************************\n");
		fprintf(fp, "\n\n");

		fclose(fp);
		m_lStatisticsLogCount = (m_lStatisticsLogCount + 1) % MAX_NUM_OF_STAT_FILES;
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}
//p20120829:end****************************************************

LONG CPI9000App::SrvResetCommand(IPC_CServiceMessage &svMsg)
{
	struct
	{
		LONG			lStation;
		SMotCommand		smCommand;
	} smResetCommand;

	POSITION	  pos;
	CString		  szDisplay	= _T("");
	CString		  szKey		= _T("");
	SFM_CStation *pStation	= NULL;
	CAppStation  *pAppStation = NULL;
	BOOL		  bResult	= FALSE;

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(smResetCommand), &smResetCommand);

		szDisplay.Format(_T("%s - Receive Reset command"), m_szModuleName);
		DisplayMessage(szDisplay);
		
		if (smResetCommand.smCommand.lAction == glRESET_SINGLE)
		{
			CString szStationName = m_pInitOperation->GetStationName(smResetCommand.lStation);

			if ((pAppStation = dynamic_cast<CAppStation*>(GetStation(szStationName))) != NULL)
			{
				if (pAppStation->State() == ERROR_Q)
				{
					pAppStation->Command(glRESET_COMMAND);
					bResult = TRUE;
				}
			}
			else
			{
				szDisplay.Format(_T("%s - Single Reset command canceled, Station Name not found"), m_szModuleName);
				DisplayMessage(szDisplay);
			}
		}
		else
		{
			for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);
				pAppStation = dynamic_cast<CAppStation*>(pStation);

				if (pAppStation->State() == ERROR_Q)
				{
					pAppStation->Command(glRESET_COMMAND);
				}

				pAppStation->Abort(FALSE);
			}

			bResult = TRUE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvDeInitialCommand(IPC_CServiceMessage &svMsg)
{
	CString			szDisplay, szKey;
	BOOL			bResult;
	SFM_CStation	*pStation = NULL;
	CAppStation	*pAppStation;

	try
	{
		szDisplay.Format(_T("%s - Receive hardware de-initialize command"), m_szModuleName);
		DisplayMessage(szDisplay);
		
		// Decipher the receive request command
		//	svMsg.GetMsg(sizeof(SCommand), &smCommand);

		if (m_qState == IDLE_Q)
		{
			//Hipec hardware DeInitialize
			m_pInitOperation->DeleteContents();
			DisplayMessage("Reset Hipec Ring Network ...");

			//Re-Load Mark IDE Data
			LoadData();

			//Set Controller Type
			m_pInitOperation->SetControllerType(OTHER_CONTROLLER);

			// Signal all stations to ReInitialize
			for (POSITION pos = m_pStationMap.GetStartPosition(); pos != NULL;)
			{
				m_pStationMap.GetNextAssoc(pos, szKey, pStation);

				if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
				{
					pAppStation->Command(glDEINITIAL_COMMAND);
					pAppStation->ReInitialized(FALSE);
					pAppStation->Result(gnOK);
				}
			}

			m_qState = DE_INITIAL_Q;
			bResult = TRUE;
		}
		else
		{
			szDisplay.Format(_T("%s - De-Initialize command canceled, Systems not in IDLE state"), m_szModuleName);
			DisplayMessage(szDisplay);
		
			bResult = FALSE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}

LONG CPI9000App::SrvJogCommand(IPC_CServiceMessage &svMsg)
{
	SMotCommand		smCommand;
	CString			szKey		= _T("");
	SFM_CStation	*pStn		= NULL;
	CAppStation	*pAppStn		= NULL;
	BOOL			bResult		= FALSE;

	try
	{
		// Decipher the receive request command
		svMsg.GetMsg(sizeof(smCommand), &smCommand);
		m_lAction = smCommand.lAction;

		for (POSITION pos = m_pStationMap.GetStartPosition(); pos;)
		{
			m_pStationMap.GetNextAssoc(pos, szKey, pStn);
			pAppStn = dynamic_cast<CAppStation*>(pStn);
			
			switch (m_lAction)
			{
			case glMRK_JOG_ON:
				pAppStn->SetJogMode(TRUE);
				break;

			case glMRK_JOG_OFF:
				pAppStn->SetJogMode(FALSE);
				break;

			case glMRK_STEP:
				pAppStn->Step(TRUE);
				break;
			}

			bResult = TRUE;
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	svMsg.InitMessage(sizeof(BOOL), &bResult);
	return 1;
}


/////////////////////////////////////////////////////////////////
// Start execution utilities
/////////////////////////////////////////////////////////////////

BOOL CPI9000App::AutoStart()
{
	return m_fAutoStart;
}

BOOL CPI9000App::SetRun()
{
	BOOL bResult = FALSE;

	if (m_qState == UN_INITIALIZE_Q)
	{
		m_bRunState = TRUE;
		bResult = TRUE;
	}
	else
	{
		DisplayMessage("Hardware already been initialized!");
	}

	return bResult;
}

VOID CPI9000App::SaveAutoStart(BOOL bOption)
{
	INT nTemp = 0;

	if (bOption)
	{
		nTemp = 1;
	}

	WriteProfileInt(gszPROFILE_SETTING, gszPROFILE_AUTO_START, nTemp);
	m_fAutoStart = bOption;
}


/////////////////////////////////////////////////////////////////
// Option & Hardware Selection
/////////////////////////////////////////////////////////////////

VOID CPI9000App::SetOptions(BOOL isOKEnable)
{
	COptionPasswordDlg dlgPw;
	BOOL bPasswordValid = FALSE;
	CString szSvrPasswrod;

	if (isOKEnable && dlgPw.DoModal() == IDOK)
	{
		szSvrPasswrod = m_smfMachine["HouseKeeping"]["Password"]["szSvrPassword"];
		if (szSvrPasswrod.GetLength() <= 1)
		{
			szSvrPasswrod.Format("a5s2m2");
		}

		if (dlgPw.m_szPassword == szSvrPasswrod)
		{
			bPasswordValid = TRUE;
		}
		else
		{
			bPasswordValid = FALSE;
			DisplayMessage("Option Modification Password Incorrect");
		}
	}
	
	if (bPasswordValid || !isOKEnable)
	{
		COptionDlg	dlg;

		dlg.m_bSelectVer1			= m_fSelectVer1;

		dlg.m_bSelectCreateErrorLog	= m_fSelectCreateErrorLog;		// 20150331
	

		if (!isOKEnable) //20120402
		{
			dlg.m_bIsAppRunning = TRUE;
		}

		// Initialize the Option dialog attributes
		if (dlg.DoModal() == IDOK && isOKEnable) //20111214
		{
			m_fSelectVer1			= dlg.m_bSelectVer1;
			m_fSelectCreateErrorLog	= dlg.m_bSelectCreateErrorLog;	// 20150331
	
			// Obtain the selected options and save them

			//Mark IDE Data Path
			LoadData();

			// Retrieve Options
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszSELECT_MC_VER1, m_fSelectVer1);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszSELECT_CREATE_ERROR_LOG, m_fSelectCreateErrorLog);	// 20150331
		
		}
	}
}

VOID CPI9000App::SelectHardware(BOOL isOKEnable)
{
	COptionPasswordDlg dlgPw;
	BOOL bPasswordValid = FALSE;
	CString szSvrPasswrod;

	if (isOKEnable && dlgPw.DoModal() == IDOK)
	{
		szSvrPasswrod = m_smfMachine["HouseKeeping"]["Password"]["szSvrPassword"];
		if (szSvrPasswrod.GetLength() <= 1)
		{
			szSvrPasswrod.Format("a5s2m2");
		}

		if (dlgPw.m_szPassword == szSvrPasswrod)
		{
			bPasswordValid = TRUE;
		}
		else
		{
			bPasswordValid = FALSE;
			DisplayMessage("Hardware Modification Password Incorrect");
		}
	}
	
	if (bPasswordValid || !isOKEnable)
	{
		CHardwareDlg hwDlg;

		hwDlg.m_bHardware		= m_fEnableHardware;
		hwDlg.m_bPRS			= m_fEnablePRS;
		hwDlg.m_bDiagnMode		= m_fEnableDiagnMode;

		hwDlg.m_bPickArm			= m_fEnablePICKARM;
		hwDlg.m_bInspWH			= m_fEnableINSPWH;
		hwDlg.m_bShBufferWH			= m_fEnableShBufferWH;
		hwDlg.m_bInspOpt		= m_fEnableInspOpt;

		hwDlg.m_bRejectArm		= m_fEnableRejectArm;
		hwDlg.m_bRejectBelt		= m_fEnableRejectBelt;
		hwDlg.m_bWH1		    = m_fEnableWH1;
		hwDlg.m_bWH2		    = m_fEnableWH2;
		hwDlg.m_bInPickArm		= m_fEnableInPickArm;
		hwDlg.m_bOutPickArm		= m_fEnableOutPickArm;
		hwDlg.m_bINWH			= m_fEnableINWH;

		if (!isOKEnable) //20120402
		{
			hwDlg.m_bIsAppRunning = TRUE;
		}

		if (hwDlg.DoModal() == IDOK && isOKEnable) //20111214
		{
			m_fEnableHardware		= hwDlg.m_bHardware;
			m_fEnablePRS			= hwDlg.m_bPRS;
			m_fEnableDiagnMode		= hwDlg.m_bDiagnMode;

			m_fEnablePICKARM			= hwDlg.m_bPickArm;
			m_fEnableINSPWH			= hwDlg.m_bInspWH;
			m_fEnableShBufferWH			= hwDlg.m_bShBufferWH;
			m_fEnableInspOpt		= hwDlg.m_bInspOpt;
			m_fEnableRejectArm = hwDlg.m_bRejectArm;
			m_fEnableRejectBelt = hwDlg.m_bRejectBelt;
			m_fEnableWH1 = hwDlg.m_bWH1;
			m_fEnableWH2 = hwDlg.m_bWH2;
			m_fEnableInPickArm = hwDlg.m_bInPickArm;
			m_fEnableOutPickArm = hwDlg.m_bOutPickArm;
			m_fEnableINWH = hwDlg.m_bINWH;

			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_ALL_HARDWARE, m_fEnableHardware);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_PRS, m_fEnablePRS);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_DIAGN_MODE, m_fEnableDiagnMode);

			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_PICKARM, m_fEnablePICKARM);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_INSPWH, m_fEnableINSPWH);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_SHBUFFER_WH, m_fEnableShBufferWH);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_INSPOPT, m_fEnableInspOpt);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_RejectArm, m_fEnableRejectArm);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_RejectBelt, m_fEnableRejectBelt);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_WH1, m_fEnableWH1);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_WH2, m_fEnableWH2);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_InPickArm, m_fEnableInPickArm);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_OutPickArm, m_fEnableOutPickArm);
			WriteProfileInt(gszPROFILE_HW_CONFIG, gszENABLE_INWH, m_fEnableINWH);
		}
	}

}


/////////////////////////////////////////////////////////////////
// Log Message
/////////////////////////////////////////////////////////////////

BOOL CPI9000App::LogMessage()
{
	return m_fLogMessage;
}

VOID CPI9000App::SetLogMessage(BOOL bEnable)
{
	INT nTemp = 0;

	if (bEnable)
	{
		nTemp = 1;
	}

	WriteProfileInt(gszPROFILE_SETTING, gszPROFILE_LOG_MSG, nTemp);
	m_fLogMessage = bEnable;
}

VOID CPI9000App::SetTotLogBackupFiles(LONG lUnit)
{
	WriteProfileInt(gszPROFILE_SETTING, gszLOG_BACKUP_SETTING, lUnit);
	m_pInitOperation->SetTotLogBackupFiles(lUnit);
}

VOID CPI9000App::SetTotLogFileLine(LONG lUnit)
{
	WriteProfileInt(gszPROFILE_SETTING, gszLOG_FILE_LINE, m_pInitOperation->SetTotLogFileLine(lUnit));
}


/////////////////////////////////////////////////////////////////
// Group ID
/////////////////////////////////////////////////////////////////

VOID CPI9000App::SetGroupId(const CString &szGroup)
{
	m_szGroupID = szGroup;
	WriteProfileString(gszPROFILE_SETTING, gszPROFILE_GROUP_ID, szGroup);
}

VOID CPI9000App::SaveGroupId(const CString &szGroup)
{
	WriteProfileString(gszPROFILE_SETTING, gszPROFILE_GROUP_ID, szGroup);
}


/////////////////////////////////////////////////////////////////
// Check for motion completion
/////////////////////////////////////////////////////////////////

INT CPI9000App::CheckStationMotion()
{
	SFM_CStation *pStation = NULL;
	CAppStation  *pAppStation;
	POSITION	  pos;
	CString		  szKey;
	INT			  nResult = MOTION_COMPLETE;

	for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
	{
		m_pStationMap.GetNextAssoc(pos, szKey, pStation);
		pAppStation = dynamic_cast<CAppStation*>(pStation);

		if (pAppStation->InMotion())
		{
			nResult = MOTION_RUNNING;
			break;
		}
	}

	return nResult;
}


/////////////////////////////////////////////////////////////////
// Execute HMI
/////////////////////////////////////////////////////////////////

VOID CPI9000App::SetExecuteHmiFlag(BOOL bExecute)
{
	INT nTemp = 0;
	
	if (bExecute)
	{
		nTemp = 1;
	}

	WriteProfileInt(gszPROFILE_SETTING, gszPROFILE_EXECUTE_HMI, nTemp);
	m_fExecuteHmi = bExecute;
}

BOOL CPI9000App::IsExecuteHmi()
{
	return m_fExecuteHmi;
}

VOID CPI9000App::ExecuteHmi()
{
	if (m_fExecuteHmi)
	{
		STARTUPINFO startupInfo;
		PROCESS_INFORMATION	processInfo;
	
		memset(&startupInfo, 0, sizeof(STARTUPINFO)); //set memory to 0
		startupInfo.cb = sizeof(STARTUPINFO);
	
		CString strCurDir = "D:\\sw\\PI9000\\AsmHmi";
		CString strHmiExe = "D:\\sw\\PI9000\\AsmHmi\\AsmHmi.exe";
		CString strHmiFile = "D:\\sw\\PI9000\\Hmi\\PI9000.hcf";
		CString strCommandLine = strHmiExe + " " + strHmiFile;
	
		bool bDebug = true;
		char szCommandLine[1024];
		strcpy(szCommandLine, (LPCTSTR)strCommandLine);
	
		CreateProcess(
			/*Application Name*/NULL, 
			/*Command Line*/szCommandLine,
			NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, 
			/*Current Directory*/strCurDir, 
			&startupInfo, &processInfo);
			
		m_fRestartHmi = FALSE;
	}
}

VOID CPI9000App::ExecuteGateway()
{
	STARTUPINFO startupInfo;
	//PROCESS_INFORMATION	processInfo;

	ZeroMemory(&m_stGatewayProcessInfo, sizeof(m_stGatewayProcessInfo));
	memset(&startupInfo, 0, sizeof(STARTUPINFO)); //set memory to 0
	startupInfo.cb = sizeof(STARTUPINFO);

	CString strCurDir = "D:\\sw\\PI9000\\Gateway";
	CString strGatewayExe = "D:\\sw\\PI9000\\Gateway\\SocketGateway.exe";
	//CString strGatewayFile = "D:\\sw\\PI9000\\Gateway\\IPC_Gateway.cfg";
	CString strCommandLine = strGatewayExe; // + " " + strGatewayFile;

	//bool bDebug = true;
	char szCommandLine[1024];
	ZeroMemory(&szCommandLine[0], 1024);
	strcpy(szCommandLine, (LPCTSTR)strCommandLine);

	CreateProcess(
		/*Application Name*/NULL, 
		/*Command Line*/szCommandLine,
		NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, 
		/*Current Directory*/strCurDir, 
		&startupInfo, &m_stGatewayProcessInfo);
}

VOID CPI9000App::ExecuteKillAll()
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION	processInfo;

	memset(&startupInfo, 0, sizeof(STARTUPINFO)); //set memory to 0
	startupInfo.cb = sizeof(STARTUPINFO);

	CString strCurDir = "D:\\sw";
	CString strCommandLine = "D:\\sw\\KillAll.bat";

	char szCommandLine[1024];
	strcpy(szCommandLine, (LPCTSTR)strCommandLine);

	CreateProcess(
					   /*Application Name*/NULL, 
					   /*Command Line*/szCommandLine,
					   NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, 
					   /*Current Directory*/strCurDir, 
					   &startupInfo, &processInfo);
}

BOOL CPI9000App::KeepResponse()	// 20140714 Yip
{
	MSG msgCur;
	while (PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
	{
		// pump message, but quit on WM_QUIT
		if (!PumpMessage())
		{
			return FALSE;
		}
	}
	return TRUE;
}

VOID CPI9000App::ExecuteCreateErrorLog()	// 20140714 Yip
{
	DisplayMessage("Creating Error Log...");

	STARTUPINFO startupInfo;
	PROCESS_INFORMATION	processInfo;

	memset(&startupInfo, 0, sizeof(STARTUPINFO)); //set memory to 0
	startupInfo.cb = sizeof(STARTUPINFO);

	CString strCurDir = "D:";
	CString strCommandLine = "D:\\CreateErrorLog.exe";

	char szCommandLine[1024];
	strcpy(szCommandLine, (LPCTSTR)strCommandLine);

	BOOL bResult = CreateProcess(
					   /*Application Name*/NULL, 
					   /*Command Line*/szCommandLine,
					   NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, 
					   /*Current Directory*/strCurDir, 
					   &startupInfo, &processInfo);

	if (bResult)
	{
		DWORD dwResult;
		do
		{
			KeepResponse();
			Sleep(100);
			dwResult = WaitForSingleObject(processInfo.hProcess, 100);
		} while (dwResult == WAIT_TIMEOUT);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}

	DisplayMessage("Error Log Created");
}

BOOL CPI9000App::ScanVisionNT()
{
	// Check if visionNT.exe is running
	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0};
	INT nCount = 0;

	// Take a snapshot of all processes in the system. 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		// Fill in the size of the structure before using it. 
		pe32.dwSize = sizeof(PROCESSENTRY32);

		// Walk the snapshot of the processes, and for each process, display information.
		if (Process32First(hProcessSnap, &pe32))
		{
			do
			{
				CString strName = pe32.szExeFile;
				strName = strName.MakeUpper();

				if (strName.Find("VISIONNT.EXE") >= 0)
				{
					nCount++;
				}
			} while (Process32Next(hProcessSnap, &pe32));
		}
	}
	CloseHandle(hProcessSnap);

	if (nCount != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#if 1 //20141124
VOID CPI9000App::InitErrorAlertTable()
{
	//auto generate by pyton script. Pls don't input manually
	// For details. Pls refer \\cog900-ws1\SoftwareBackup\Development\Developer\python\Portable Python 2.7.6.1\App\test
	m_mapErrorAlertTable["HK000"] = "HouseKeeping Air Supply Error";
	m_mapErrorAlertTable["HK001"] = "HouseKeeping Operation TimeOut Error";
	m_mapErrorAlertTable["HK002"] = "HouseKeeping Software Handling Error";
	m_mapErrorAlertTable["HK003"] = "HouseKeeping Prebond Air Supply Error";
	m_mapErrorAlertTable["HK004"] = "HouseKeeping Mainbond Air Supply Error";
	m_mapErrorAlertTable["HK005"] = "HouseKeeping Prebond Front Cover Open Error";
	m_mapErrorAlertTable["HK006"] = "HouseKeeping Prebond Back Cover Open Error";
	m_mapErrorAlertTable["HK007"] = "HouseKeeping Mainbond Front Cover Open Error";
	m_mapErrorAlertTable["HK008"] = "HouseKeeping Mainbond Back Cover Open Error";
	m_mapErrorAlertTable["HK009"] = "HouseKeeping Light Curtain Blocked Error";
	m_mapErrorAlertTable["HK010"] = "HouseKeeping Air Supply Detection Enable Error";
	m_mapErrorAlertTable["HK011"] = "Please Clear the ACF Collection Box";
	m_mapErrorAlertTable["HK012"] = "HouseKeeping EMO Pressed";
	m_mapErrorAlertTable["HK013"] = "HouseKeeping Front Cover Open Error";
	m_mapErrorAlertTable["HK014"] = "HouseKeeping Back Cover Open Error";
	m_mapErrorAlertTable["HK015"] = "HouseKeeping Lower Door Open Error";
	m_mapErrorAlertTable["HK016"] = "HouseKeeping In Convey Cover Open Error";
	m_mapErrorAlertTable["HK017"] = "HouseKeeping FPC PA Door Open Error";
	m_mapErrorAlertTable["HK018"] = "HouseKeeping ICH Back Cover Open Error";
	m_mapErrorAlertTable["HP001"] = "Motor Not On Error";
	m_mapErrorAlertTable["HP002"] = "JoyStick Not Off Error";
	m_mapErrorAlertTable["HP003"] = "Limit Sensor Hit";
	m_mapErrorAlertTable["HP004"] = "Search Fail";
	m_mapErrorAlertTable["HP005"] = "DAC Limit Hit";
	m_mapErrorAlertTable["HP006"] = "Software Position Limit Hit";
	m_mapErrorAlertTable["HP007"] = "Driver Fault";
	m_mapErrorAlertTable["HP008"] = "Encoder Fault";
	m_mapErrorAlertTable["HP009"] = "Power Fault";
	m_mapErrorAlertTable["HP010"] = "Position Error Limit Hit";
	m_mapErrorAlertTable["HP011"] = "Temperature Power Fault";
	m_mapErrorAlertTable["HP012"] = "Software Commutation Initialization Fault";
	m_mapErrorAlertTable["HP013"] = "Unknown Channel Fault";
	m_mapErrorAlertTable["HP014"] = "Unknown Fault";
	m_mapErrorAlertTable["HP015"] = "Motor Commutation Error";
	m_mapErrorAlertTable["HP016"] = "Motor Not Home";
	m_mapErrorAlertTable["HP017"] = "Isolation Power Fault";
	m_mapErrorAlertTable["HP018"] = "Motion Abort";
	m_mapErrorAlertTable["HP019"] = "Other Channel Fault";
	m_mapErrorAlertTable["HP020"] = "Event Driven Error";
	m_mapErrorAlertTable["HP021"] = "Motion TimeOut";
	m_mapErrorAlertTable["HP022"] = "FlexiControl Error";
	m_mapErrorAlertTable["HP031"] = "Search Timeout Error";
	m_mapErrorAlertTable["HP032"] = "Exceed Software Limit Error";
	m_mapErrorAlertTable["HP033"] = "Hit Limit Sensor Error";
	m_mapErrorAlertTable["HP034"] = "Encoder Fault Error";
	m_mapErrorAlertTable["HP035"] = "Driver Fault Error";
	m_mapErrorAlertTable["HP036"] = "Motion Abort Error";
	m_mapErrorAlertTable["HP037"] = "Dependent Channel Error";
	m_mapErrorAlertTable["HP038"] = "Motion Control Error";
	m_mapErrorAlertTable["HP039"] = "Brake Lock Error";
	m_mapErrorAlertTable["HP040"] = "Open Wire Fault Error";
	m_mapErrorAlertTable["HP041"] = "Velocity Jump Error";
	m_mapErrorAlertTable["HP042"] = "Emergency Stop Error";
	m_mapErrorAlertTable["HP043"] = "Hit Projected Software Limit Error";
	m_mapErrorAlertTable["INSPOPT000"] = "InspOpt Module Not Selected Error";
	m_mapErrorAlertTable["INSPOPT001"] = "InspOpt Motor X Error";
	m_mapErrorAlertTable["INSPOPT002"] = "InspOpt Motor Y Error";
	m_mapErrorAlertTable["INSPOPT003"] = "InspOpt PR Module Not Selected Error";
	m_mapErrorAlertTable["INSPOPT004"] = "InspOpt Align Point Not Learn Error";
	m_mapErrorAlertTable["INSPOPT005"] = "InspOpt Glass1 UpLook PR1 Error";
	m_mapErrorAlertTable["INSPOPT006"] = "InspOpt Glass2 UpLook PR1 Error";
	m_mapErrorAlertTable["INSPOPT007"] = "InspOpt Glass1 UpLook PR2 Error";
	m_mapErrorAlertTable["INSPOPT008"] = "InspOpt Glass2 UpLook PR2 Error";
	m_mapErrorAlertTable["INSPOPT009"] = "InspOpt COF1 UpLook PR1 Error";
	m_mapErrorAlertTable["INSPOPT010"] = "InspOpt COF2 UpLook PR1 Error";
	m_mapErrorAlertTable["INSPOPT011"] = "InspOpt COF1 UpLook PR2 Error";
	m_mapErrorAlertTable["INSPOPT012"] = "InspOpt COF2 UpLook PR2 Error";
	m_mapErrorAlertTable["INSPOPT013"] = "InspOpt Placement X Tolerance Error";
	m_mapErrorAlertTable["INSPOPT014"] = "InspOpt Placement Y Tolerance Error";
	m_mapErrorAlertTable["INSPOPT015"] = "InspOpt Placement Angle Tolerance Error";
	m_mapErrorAlertTable["INSPOPT016"] = "InspOpt PR Not Loaded Error";
	m_mapErrorAlertTable["INSPWH000"] = "InspWH Module Not Selected Error";
	m_mapErrorAlertTable["INSPWH001"] = "InspWH PR Module Not Selected Error";
	m_mapErrorAlertTable["INSPWH002"] = "InspWH Motor Y Error";
	m_mapErrorAlertTable["INSPWH003"] = "InspWH Motor X Error";
	m_mapErrorAlertTable["INSPWH004"] = "InspWH Motor Z1 Error";
	m_mapErrorAlertTable["INSPWH005"] = "InspWH Motor Z2 Error";
	m_mapErrorAlertTable["INSPWH006"] = "InspWH Calib Not Select Error";
	m_mapErrorAlertTable["INSPWH007"] = "InspWH PR Not Loaded Error";
	m_mapErrorAlertTable["INSPWH008"] = "InspWH Align Point Not Learn Error";
	m_mapErrorAlertTable["INSPWH009"] = "InspWH Glass1 DownLook PR1 Error";
	m_mapErrorAlertTable["INSPWH010"] = "InspWH Glass2 DownLook PR1 Error";
	m_mapErrorAlertTable["INSPWH011"] = "InspWH Glass1 DownLook PR2 Error";
	m_mapErrorAlertTable["INSPWH012"] = "InspWH Glass2 DownLook PR2 Error";
	m_mapErrorAlertTable["INSPWH013"] = "InspWH Glass1 Vac Error ";
	m_mapErrorAlertTable["INSPWH014"] = "InspWH Glass2 Vac Error";
	m_mapErrorAlertTable["INSPWH015"] = "InspWH Reject Glass Limit Hit";
	m_mapErrorAlertTable["INSPWH016"] = "InspWH Gateway Time Out Error";
	m_mapErrorAlertTable["MTR000"] = "Motor Error";
	m_mapErrorAlertTable["PR000"] = "PR Not Selected Error";
	m_mapErrorAlertTable["PR001"] = "DPR Master Initialise Error";
	m_mapErrorAlertTable["PR002"] = "DPR Slave Initialise Error";
	m_mapErrorAlertTable["PR003"] = "DPR Warm Start Error";
	m_mapErrorAlertTable["PR004"] = "Loading PR records from archive...";
	m_mapErrorAlertTable["PICKARM000"] = "PickArm Module Not Selected Error";
	m_mapErrorAlertTable["PICKARM001"] = "PickArm Motor X Error";
	m_mapErrorAlertTable["PICKARM002"] = "PickArm Glass1 Vac Error";
	m_mapErrorAlertTable["PICKARM003"] = "PickArm Glass2 Vac Error";
	m_mapErrorAlertTable["PICKARM004"] = "PickArm COF1 Vac Error";
	m_mapErrorAlertTable["PICKARM005"] = "PickArm COF2 Vac Error";
	m_mapErrorAlertTable["PICKARM006"] = "PickArm Pick Glass From ShBuffer WH Error";
	m_mapErrorAlertTable["PICKARM007"] = "PickArm Place Glass To ShBuffer WH Error";
	m_mapErrorAlertTable["PICKARM008"] = "PickArm Pick Glass From InspWH Error";
	m_mapErrorAlertTable["PICKARM009"] = "PickArm Place Glass Out Error";
	m_mapErrorAlertTable["REJARM000"] = "RejectArm Module Not Selected Error";
	m_mapErrorAlertTable["REJARM001"] = "RejectArm Motor Y Error";
	m_mapErrorAlertTable["REJARM002"] = "RejectArm Motor Z Error";
	m_mapErrorAlertTable["REJARM003"] = "RejectArm Glass Vac Error";
	m_mapErrorAlertTable["REJARM004"] = "RejectArm COF Vac Error";
	m_mapErrorAlertTable["REJARM005"] = "RejectArm Pick Glass Error";
	m_mapErrorAlertTable["REJARM006"] = "RejectArm Place Glass Error";
	m_mapErrorAlertTable["REJARM007"] = "RejectArm Glass Exist On Belt Error";
	m_mapErrorAlertTable["REJBELT000"] = "RejectBelt Module Not Selected Error";
	m_mapErrorAlertTable["REJBELT001"] = "RejectBelt Motor Belt Error";
	m_mapErrorAlertTable["REJBELT002"] = "RejectBelt Belt Full Error";
	m_mapErrorAlertTable["REJBELT003"] = "RejectBelt Belt Move Error";
	m_mapErrorAlertTable["SHBUFFERWH000"] = "ShBufferWH Module Not Selected Error";
	m_mapErrorAlertTable["SHBUFFERWH001"] = "ShBufferWH Y Sol On (Go Load Posn) Error";
	m_mapErrorAlertTable["SHBUFFERWH002"] = "ShBufferWH Y Sol Off (Go Unload Posn) Error";
	m_mapErrorAlertTable["SHBUFFERWH003"] = "ShBufferWH Z Sol On (Go Load Posn) Error";
	m_mapErrorAlertTable["SHBUFFERWH004"] = "ShBufferWH Z Sol Off (Go Unload Posn) Error";
	m_mapErrorAlertTable["SHBUFFERWH005"] = "ShBufferWH Glass1 Vac Error";
	m_mapErrorAlertTable["SHBUFFERWH006"] = "ShBufferWH Glass2 Vac Error";
	m_mapErrorAlertTable["TEMP0000"] = "MB1 Head Heater Error";
	m_mapErrorAlertTable["TEMP0001"] = "MB2 Head Heater Error";
	m_mapErrorAlertTable["TEMP0002"] = "MB1 Base Heater Error";
	m_mapErrorAlertTable["TEMP0003"] = "MB2 Base Heater Error";
	m_mapErrorAlertTable["TEMP0004"] = "ACF1 Heater Error";
	m_mapErrorAlertTable["TEMP0005"] = "PB1 Heater Error";
	m_mapErrorAlertTable["TEMP0006"] = "MB3 Head Heater Error";
	m_mapErrorAlertTable["TEMP0007"] = "MB4 Head Heater Error";
	m_mapErrorAlertTable["TEMP0008"] = "MB3 Base Heater Error";
	m_mapErrorAlertTable["TEMP0009"] = "MB4 Base Heater Error";
	m_mapErrorAlertTable["TEMP0010"] = "ACF2 Heater Error";
	m_mapErrorAlertTable["TEMP0011"] = "PB2 Heater Error";
	m_mapErrorAlertTable["TEMP0050"] = "Temperature Controller Control Error";
	m_mapErrorAlertTable["TEMP0060"] = "Temperature Monitor Error";
	m_mapErrorAlertTable["TEMP0080"] = "Heater Communication Error";
	m_mapErrorAlertTable["TEMP0100"] = "MB1 Head Heater Not On Error";
	m_mapErrorAlertTable["TEMP0101"] = "MB2 Head Heater Not On Error";
	m_mapErrorAlertTable["TEMP0102"] = "MB1 Base Heater Not On Error";
	m_mapErrorAlertTable["TEMP0103"] = "MB2 Base Heater Not On Error";
	m_mapErrorAlertTable["TEMP0104"] = "ACF1 Heater Not On Error";
	m_mapErrorAlertTable["TEMP0105"] = "PB1 Heater Not On Error";
	m_mapErrorAlertTable["TEMP0106"] = "MB3 Head Heater Not On Error";
	m_mapErrorAlertTable["TEMP0107"] = "MB4 Head Heater Not On Error";
	m_mapErrorAlertTable["TEMP0108"] = "MB3 Base Heater Not On Error";
	m_mapErrorAlertTable["TEMP0109"] = "MB4 Base Heater Not On Error";
	m_mapErrorAlertTable["TEMP0110"] = "ACF2 Heater Not On Error";
	m_mapErrorAlertTable["TEMP0111"] = "PB2 Heater Not On Error";
	m_mapErrorAlertTable["TEMP0200"] = "MB1 Head Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0201"] = "MB2 Head Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0202"] = "MB1 Base Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0203"] = "MB2 Base Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0204"] = "ACF1 Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0205"] = "PB1 Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0206"] = "MB3 Head Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0207"] = "MB4 Head Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0208"] = "MB3 Base Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0209"] = "MB4 Base Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0210"] = "ACF2 Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0211"] = "PB2 Heater Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0300"] = "MB1 Head Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0301"] = "MB2 Head Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0302"] = "MB1 Base Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0303"] = "MB2 Base Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0304"] = "ACF1 Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0305"] = "PB1 Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0306"] = "MB3 Head Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0307"] = "MB4 Head Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0308"] = "MB3 Base Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0309"] = "MB4 Base Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0310"] = "ACF2 Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0311"] = "PB2 Temperature Not Ready";
	m_mapErrorAlertTable["TEMP0400"] = "MB1 Head Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0401"] = "MB2 Head Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0402"] = "MB1 Base Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0403"] = "MB2 Base Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0404"] = "ACF1 Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0405"] = "PB1 Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0406"] = "MB3 Head Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0407"] = "MB4 Head Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0408"] = "MB3 Base Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0409"] = "MB4 Base Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0410"] = "ACF2 Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0411"] = "PB2 Monitor Temperature Out of Range Error";
	m_mapErrorAlertTable["TEMP0500"] = "MB1 Head Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0501"] = "MB2 Head Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0502"] = "MB1 Base Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0503"] = "MB2 Base Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0504"] = "ACF1 Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0505"] = "PB1 Head Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0506"] = "MB3 Head Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0507"] = "MB4 Head Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0508"] = "MB3 Base Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0509"] = "MB4 Base Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0510"] = "ACF2 Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0511"] = "PB2 Head Heater Ctrl Error";
	m_mapErrorAlertTable["TEMP0600"] = "MB1 Head Heater Mon Error";
	m_mapErrorAlertTable["TEMP0601"] = "MB2 Head Heater Mon Error";
	m_mapErrorAlertTable["TEMP0602"] = "MB1 Base Heater Mon Error";
	m_mapErrorAlertTable["TEMP0603"] = "MB2 Base Heater Mon Error";
	m_mapErrorAlertTable["TEMP0604"] = "ACF1 Heater Mon Error";
	m_mapErrorAlertTable["TEMP0605"] = "PB1 Heater Mon Error";
	m_mapErrorAlertTable["TEMP0606"] = "MB3 Head Heater Mon Error";
	m_mapErrorAlertTable["TEMP0607"] = "MB4 Head Heater Mon Error";
	m_mapErrorAlertTable["TEMP0608"] = "MB3 Base Heater Mon Error";
	m_mapErrorAlertTable["TEMP0609"] = "MB4 Base Heater Mon Error";
	m_mapErrorAlertTable["TEMP0610"] = "ACF2 Heater Mon Error";
	m_mapErrorAlertTable["TEMP0611"] = "PB2 Heater Mon Error";
	m_mapErrorAlertTable["TEMP0700"] = "Litec Thermocouple Connection Error";
	m_mapErrorAlertTable["TEMP0701"] = "Litec Connection Sensor Connection Error";
	m_mapErrorAlertTable["TEMP0702"] = "Litec Temperature Out of Alarm Band Error";
	m_mapErrorAlertTable["TEMP0703"] = "Litec Temperature Hit Limit";
	m_mapErrorAlertTable["TEMP0704"] = "Litec Heater Open Error";
	m_mapErrorAlertTable["TEMP0705"] = "Litec Protection Triggered!";
	m_mapErrorAlertTable["TEMP0706"] = "Litec Chip ADC Recovery Fail";
	m_mapErrorAlertTable["TEMP0707"] = "Litec Control Noise Protection Triggered!";
	m_mapErrorAlertTable["TEMP0708"] = "Litec Dynamic Protection Triggered!";
	m_mapErrorAlertTable["TEMP0709"] = "Litec Control Static Protection Triggered!";
	m_mapErrorAlertTable["TEMP0710"] = "Litec Control Ramping Protection Triggered!";
	m_mapErrorAlertTable["TEMP0800"] = "MB1 Head Heater Communication Error";
	m_mapErrorAlertTable["TEMP0801"] = "MB2 Head Heater Communication Error";
	m_mapErrorAlertTable["TEMP0802"] = "MB1 Base Heater Communication Error";
	m_mapErrorAlertTable["TEMP0803"] = "MB2 Base Heater Communication Error";
	m_mapErrorAlertTable["TEMP0804"] = "ACF1 Heater Communication Error";
	m_mapErrorAlertTable["TEMP0805"] = "PB1 Heater Communication Error";
	m_mapErrorAlertTable["TEMP0806"] = "MB3 Head Heater Communication Error";
	m_mapErrorAlertTable["TEMP0807"] = "MB4 Head Heater Communication Error";
	m_mapErrorAlertTable["TEMP0808"] = "MB3 Base Heater Communication Error";
	m_mapErrorAlertTable["TEMP0809"] = "MB4 Base Heater Communication Error";
	m_mapErrorAlertTable["TEMP0810"] = "ACF2 Heater Communication Error";
	m_mapErrorAlertTable["TEMP0811"] = "PB2 Heater Communication Error";
	m_mapErrorAlertTable["TEMP0812"] = "Temperture Controller Unit 1 Error (ACF1, ACF2, PB1, PB2)";
	m_mapErrorAlertTable["TEMP0813"] = "Temperture Controller Unit 2 Error (MB1, MB2)";
	m_mapErrorAlertTable["TEMP0814"] = "Temperture Controller Unit 3 Error (MB3, MB4)";
	m_mapErrorAlertTable["TEMP0815"] = "Temperture Controller Unit 4 Error (MB1WH, MB2WH, MB3WH, MB4WH)";
	m_mapErrorAlertTable["TEMP0816"] = "Temperture Controller Unit 5 Error";
	m_mapErrorAlertTable["TEMP0817"] = "Temperture Controller Unit 6 Error";
	m_mapErrorAlertTable["TEMP0818"] = "Temperture Controller Unit 7 Error ";
	m_mapErrorAlertTable["TEMP0819"] = "Temperture Controller Unit 8 Error";
	m_mapErrorAlertTable["TEMP0820"] = "Temperture Controller Unit 9 Error ";
	m_mapErrorAlertTable["TEMP0821"] = "Temperture Controller Unit 10 Error ";
	m_mapErrorAlertTable["TEMP0822"] = "Temperture Controller Unit 11 Error";
	m_mapErrorAlertTable["TEMP0823"] = "Temperture Controller Unit 12 Error";

	return;
}

VOID CPI9000App::LogErrorAlarm(const CString &szError, BOOL bIsError)
{
	// open debugging file
	FILE *fp = fopen("D:\\sw\\PI9000\\LogFileDir\\ErrorAlarm.csv", "a+");

	if (fp != NULL)
	{
		CString szCurrError = szError;
		CString szTime(" "), szDescription(" ");
		char tmpbuf[129];
		struct tm *today;
		__time64_t ltime;

		_time64(&ltime);
		today = _localtime64(&ltime);
		memset(&tmpbuf[0], 0 , 129);
		strftime(tmpbuf, 128, "%Y_%m_%d_%H_%M_%S", today);
		szTime.Format("%s", tmpbuf);
		
		LONG lfpPos = ftell(fp);
		fseek(fp, 0, SEEK_END);
		if (ftell(fp) < 10)
		{
			fprintf(fp, "Machine Model: %s\n", (char*)m_szMachineModel.GetString()); //20150124
			fprintf(fp, "Machine ID: %s\n", (char*)m_szMachineID.GetString());
			fprintf(fp, "SW Version: %s\n", (char*)m_szSoftVersion.GetString());
			fprintf(fp, "Release Date: %s\n",(char*)m_szReleaseDate.GetString());

			fprintf(fp, "Date_Time,Error_ID,Error_Description,Message_Type,Error-To-Start_Duration(Sec),Last_Error\n");
		}
		fseek(fp, lfpPos, SEEK_SET);

		if (szCurrError == "")
		{
			szCurrError = "NIL";
		}

		szDescription = m_mapErrorAlertTable[szError];
		if (szDescription == "")
		{
			szDescription = "Nil";
		}
		
		if (szCurrError == "START_BONDING")
		{
			szDescription = "START BONDING";
		}

		if (szCurrError == "START_BONDING")
		{
			if (pCHouseKeeping->m_szLastError.GetLength() > 0 &&
				(DOUBLE)(m_clkBondStartTime - m_clkLastErrorTime) > 0.0)	//20150105
			{
				fprintf(fp, "%s,%s,%s,START_BONDING,%.2f,%s\n", 
					(char*)szTime.GetBuffer(0), 
					(char*)szCurrError.GetBuffer(0),
					(char*)szDescription.GetBuffer(0), 
					(DOUBLE)(m_clkBondStartTime - m_clkLastErrorTime) / CLOCKS_PER_SEC, 
					(char*)m_mapErrorAlertTable[pCHouseKeeping->m_szLastError].GetBuffer(0));
				pCHouseKeeping->m_szLastError = "";	// 20150326
			}
			else
			{
				fprintf(fp, "%s,%s,%s,START_BONDING,--,--\n", (char*)szTime.GetBuffer(0), (char*)szCurrError.GetBuffer(0),
					(char*)szDescription.GetBuffer(0));
			}
		}
		else
		{
			fprintf(fp, "%s,%s,%s,%s,--,--\n", (char*)szTime.GetBuffer(0), (char*)szCurrError.GetBuffer(0), (char*)szDescription.GetBuffer(0), bIsError ? "Error" : "Alert");
		}

		fclose(fp);
	}

}
#endif

VOID CPI9000App::GetAllStatus(const CString &szError)	// 20140729 Yip: Add Error Description In Status Log
{
	m_csGetAllStatusLock.Lock();	// 20140728 Yip

	SYSTEMTIME ltime;
	GetLocalTime(&ltime);

	CString szFileName;
	szFileName.Format("D:\\sw\\PI9000\\LogFileDir\\StationStatus_%04u%02u%02u.log", ltime.wYear, ltime.wMonth, ltime.wDay);

	// open debugging file
//	FILE *fp = fopen("D:\\sw\\PI9000\\LogFileDir\\StationStatus.log", "a+");
	FILE *fp = fopen(szFileName.GetString(), "a+");	// 20140903 Yip: Separate Station Status Log By Date

	if (fp != NULL)
	{
		__time64_t ltime;
		_time64(&ltime);
		CString szTime = _ctime64(&ltime);
		fprintf(fp, "\nPI9000 Station Status: " + szTime);
		fprintf(fp, "Machine ID: %s\n", (char*)m_szMachineID.GetString());
		fprintf(fp, "SW Version: %s\n", (char*)m_szSoftVersion.GetString());

		if (!szError.IsEmpty())	// 20140729 Yip: Add Error Description In Status Log
		{
			fprintf(fp, "Error: %s Desp:%s\n", (char*)szError.GetString(), (char*)m_mapErrorAlertTable[szError].GetString());
		}
		else
		{
			fprintf(fp, "Error: NIL\n");
		}

		if (m_qState == STOPPING_Q)
		{
			fprintf(fp, "***************************************************************\n");
			fprintf(fp, "AUTOMATIC LOG STATION STATUS\n");
			fprintf(fp, "***************************************************************\n");
		}

		// station states
		fprintf(fp, "Machine State: %d\tBonding Mode: %d\n", State(), AutoMode);

#if 1 //20140220
		SFM_CStation	*pStation = NULL;
		CAppStation	*pAppStation = NULL;
		CString			szKey = _T("");
		POSITION		pos;
		fprintf(fp, "***********************State Map********************************\n");
		for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
		{
			m_pStationMap.GetNextAssoc(pos, szKey, pStation);
			pAppStation = dynamic_cast<CAppStation*>(pStation);
			fprintf(fp, "AppName:%s, State:%s\n", (char*)pAppStation->GetName().GetBuffer(0), (char*)(LPCTSTR)gszMODULE_STATE[pAppStation->State()]);
		}
		fprintf(fp, "***************************************************************\n");
#endif
		fprintf(fp, "INPICKARM Go State: %d\tINPICKARM_Status: %ld\n", pCInPickArm->m_qSubOperation, (LONG)INPICKARM_Status);
		pCInPickArm->PrintDebugSeq(fp); //20131010

		fprintf(fp, "OUTPICKARM Go State: %d\tOUTPICKARM_Status: %ld\n", pCOutPickArm->m_qSubOperation, (LONG)OUTPICKARM_Status);
		pCOutPickArm->PrintDebugSeq(fp); //20131010

		fprintf(fp, "WH1 Go State: %d\tWH1_Status: %ld\n", pCWH1->m_qSubOperation, (LONG)INSPWH_Status[WH_1]);
		pCWH1->PrintDebugSeq(fp); //20131010

		fprintf(fp, "WH2 Go State: %d\tWH2_Status: %ld\n", pCWH2->m_qSubOperation, (LONG)INSPWH_Status[WH_2]);
		pCWH2->PrintDebugSeq(fp); //20131010

		fprintf(fp, "INSPOPT Go State: %d\tINSPOPT_Status: %ld\n", pCInspOpt->m_qSubOperation, (LONG)INSPOPT_Status);
		pCInspOpt->PrintDebugSeq(fp); 

		fprintf(fp, "SHBUFWH Go State: %d\tSHBUFWH_Status: %ld\n", pCShBufferWH->m_qSubOperation, (LONG)BUFFERWH_Status);
		pCShBufferWH->PrintDebugSeq(fp);

		fprintf(fp, "TAManager Go State: %d\tTA_MANAGER_Status: %ld\n", pCTAManager->m_qSubOperation, (LONG)TA_MANAGER_Status);
		pCTAManager->PrintDebugSeq(fp);

		fprintf(fp, "REJBELT Go State: %d\tREJBELT_Status: %ld\n", pCRejectBelt->m_qSubOperation, (LONG)REJECTBELT_Status);
		pCRejectBelt->PrintDebugSeq(fp);
		// autobond stats
		fprintf(fp, "\nMachine Statistics:\n");

		fprintf(fp, "\tGlass Picked:\t%ld\n", m_ulGlassPicked);

		fprintf(fp, "\t\tGlass Rejected:\t%ld\n", m_ulGlassRejected);
		fprintf(fp, "Glass Inpected = %lu\n", m_ulUnitsInpected);
		
		fprintf(fp, "\nAll Glass Status:\n");
		fprintf(fp, "INPICKARM Glass Exist: %ld\t %ld\n", (LONG)pCInPickArm->m_bGlass1Exist, (LONG)pCInPickArm->m_bGlass2Exist);
		fprintf(fp, "OUTPICKARM Glass Exist: %ld\t %ld\n", (LONG)pCOutPickArm->m_bGlass1Exist, (LONG)pCOutPickArm->m_bGlass2Exist);
		fprintf(fp, "WH1 Glass Status: %ld\t %ld\n", (LONG)INSPWH_GLASS_Status[WH_1][GLASS1], (LONG)INSPWH_GLASS_Status[WH_1][GLASS2]);
		fprintf(fp, "WH2 Glass Status: %ld\t %ld\n", (LONG)INSPWH_GLASS_Status[WH_2][GLASS1], (LONG)INSPWH_GLASS_Status[WH_2][GLASS2]);
		fprintf(fp, "SHBUFWH Glass Status: %ld\t %ld\n", (LONG)pCShBufferWH->m_bGlass1Exist, (LONG)pCShBufferWH->m_bGlass2Exist);
	
		fprintf(fp, "\nEnd of Report\n");
		fprintf(fp, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

		fclose(fp);
	}

	m_csGetAllStatusLock.Unlock();	// 20140728 Yip
}

BOOL CALLBACK EnumWndFunc(HWND hWnd, LPARAM lParam)
{
	RECT Rect;
	::GetWindowRect(hWnd, &Rect);

	if (Rect.right - Rect.left == 1286)
	{
		::PostMessage(hWnd, WM_CLOSE, 0, NULL);
	}

	return TRUE;
}

VOID CPI9000App::KillHmi()
{
	DisplayMessage("Killing HMI...");
	EnumThreadWindows(processInfo.dwThreadId, EnumWndFunc, NULL);

	// Check if AsmHmi.exe is running
	INT nCount = 0;

	do
	{
		Sleep(10);

		HANDLE hProcessSnap = NULL;
		PROCESSENTRY32 pe32 = {0};
		nCount = 0;

		// Take a snapshot of all processes in the system. 
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hProcessSnap != INVALID_HANDLE_VALUE)
		{
			// Fill in the size of the structure before using it. 
			pe32.dwSize = sizeof(PROCESSENTRY32);

			// Walk the snapshot of the processes, and for each process, display information.
			if (Process32First(hProcessSnap, &pe32))
			{
				do
				{
					CString strName = pe32.szExeFile;
					strName = strName.MakeUpper();

					if (strName.Find("ASMHMI.EXE") >= 0)
					{
						nCount++;
					}
				} while (Process32Next(hProcessSnap, &pe32));
			}
		}
		CloseHandle(hProcessSnap);

	} while (nCount != 0);

	m_fRestartHmi = TRUE;
}

VOID CPI9000App::SetAlert(UINT unCode)
{
	CString		szCode;
	INT			bResult;

	if ((unCode == gnOK) || (unCode == IDS_SYS_ABORT))
	{
		return;
	}

	pCHouseKeeping->m_bAlertExist = TRUE; //20130115

	if ((bResult = szCode.LoadString(unCode)) != FALSE)
	{
		try
		{
			SendAlarm(szCode, ALM_DONT_LOG, 3000, gszALM_CLASS_ALERT);
#if 1 //20141124
			if ((m_qState == AUTO_Q) || (m_qState == DEMO_Q) || (m_qState == STOPPING_Q))
			{
				m_csLogErrorAlertLock.Lock();
				CString szInFilePath2("D:\\sw\\PI9000\\LogFileDir\\");
				CString szInFileName2("ErrorAlarm.csv");
				CheckFileSize(szInFilePath2, szInFileName2, 512); //512KB
				LogErrorAlarm(szCode, FALSE);
				m_csLogErrorAlertLock.Unlock();
			}
#endif
		}
		catch (CAsmException e)
		{
			TRACE1(_T("SendError Fail - %s\n"), szCode);
		}
	}
	else
	{
		szCode.Format(_T("%u: Alarm Code Not Found! From %s station"), unCode, m_szModuleName);
	}

	szCode = "Error: " + szCode  + " Desp: " + m_mapErrorAlertTable[szCode];
	DisplayMessage(szCode);

	return;
}

VOID CPI9000App::SetError(UINT unCode)
{
	CString		szCode;
	BOOL		bResult;
	LONG		lResponse = 0;

	if (unCode == gnOK)
	{
		return;
	}
	
	pCHouseKeeping->m_bErrorExist = TRUE; //20130115
		
	if ((bResult = szCode.LoadString(unCode)) != FALSE)
	{
		try
		{
			SendAlarm(szCode, ALM_FORCE_LOG, 3000, gszALM_CLASS_ERROR);
		}
		catch (CAsmException e)
		{
			TRACE1(_T("SendError Fail - %s\n"), szCode);
		}
	}
	else
	{
		szCode.Format(_T("%u: Alarm Code Not Found! From %s station"), unCode, m_szModuleName);
	}


	if ((m_qState == AUTO_Q) || (m_qState == DEMO_Q) || (m_qState == STOPPING_Q))
	{
		GetAllStatus(szCode);	// 20140728 Yip
#if 1 //20141124
		m_csLogErrorAlertLock.Lock();
		CString szInFilePath2("D:\\sw\\PI9000\\LogFileDir\\");
		CString szInFileName2("ErrorAlarm.csv");
		CheckFileSize(szInFilePath2, szInFileName2, 512); //512KB
		LogErrorAlarm(szCode);
		m_csLogErrorAlertLock.Unlock();
#endif
		m_clkLastErrorTime = clock(); //20150105
		pCHouseKeeping->m_szLastError = szCode;	// 20140814 Yip
	}

	__time64_t ltime;
	_time64(&ltime);
	CString szTime = _ctime64(&ltime);

	szCode = _T("[") + szTime + _T("]: ") + m_szModuleName + " Error: " + szCode + " Desp: " + m_mapErrorAlertTable[szCode];
	DisplayMessage(szCode);
}

////////////////////////////////////////////////////////////////////////////////////
//Machine - Load machine File
////////////////////////////////////////////////////////////////////////////////////


BOOL CPI9000App::LoadMachineFile(BOOL bLoad)
{
	CSingleLock slLock(&m_csMachine);
	slLock.Lock();

	CFileFind fileFind;

	CString szStationName;
	CString szCommand;
	CString szErrorMsg;

	CString szMsg = "";
	__time64_t ltime;

	//Create Param Directory if not exist
	if (!fileFind.FindFile(m_szMachinePath))
	{
		if (!m_pInitOperation->CreateFileDirectory(m_szMachinePath))
		{
			AfxMessageBox("Can not create Directory");
			slLock.Unlock();

			return FALSE;
		}
	}

	//Open Machine File (Create a new one if not found)
	m_smfMachine.Close(); //Clear File buffer

	if (!m_smfMachine.Open(m_szMachinePath + "\\" + m_szMachineFile, FALSE, TRUE))
	{
		AfxMessageBox("Can not load File \"" + m_szMachinePath + "\\" + m_szMachineFile + "\"");
		slLock.Unlock();

		return FALSE;
	}

	if (bLoad)
	{
		_time64(&ltime);
		szMsg = _ctime64(&ltime);

		szMsg += _T(": Loading Machine file...");
		DisplayMessage(szMsg);
		
		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			szStationName = GetStationList().GetAt(i)->GetStnName();
			GetStationList().GetAt(i)->IPC_LoadMachineParam();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_LoadPRTaskStnMachinePara();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_LoadMachineRecord();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		if (g_bIsVisionNTInited)
		{
			CString szErrMsg = _T("");
			PRU *pPRU = NULL;
			for (INT i = 0; i < pCWinEagle->GetmyCalibPRUList().GetSize(); i++)
			{
				LONG lRetRecordID = 0;
				pPRU = pCWinEagle->GetmyCalibPRUList().GetAt(i);
				if (!PRS_DownloadRecordProcess(pPRU, TRUE, lRetRecordID, &szErrMsg))
				{
					ShowPRDownloadErrorMessage(pPRU, szErrMsg);
				}
				else if (pPRU->bLoaded)
				{
					szMsg.Format("Load Mcahine PR File %s, RetRecordID:%ld", pPRU->stDownloadRecordCmd.acFilename, lRetRecordID);
					DisplayMessage(szMsg);
					pCWinEagle->SetRecordIDToPRUCmd(pPRU, (PRU_ID)lRetRecordID);
				}
				KeepResponse();	// 20140714 Yip
			}
		}
		
		// Add me later 
		// loading statistics
		m_ulTotalGlassPicked					= m_smfMachine["Stats"]["m_ulTotalGlassPicked"];
		m_ulTotalGlassRejected					= m_smfMachine["Stats"]["m_ulTotalGlassRejected"];
		m_ulTotalGlassTolError					= m_smfMachine["Stats"]["m_ulTotalGlassTolError"];
		m_ulTotalUnitsInpected					= m_smfMachine["Stats"]["m_ulTotalUnitsInpected"];
		m_ulTotalDLPRError   					= m_smfMachine["Stats"]["m_ulTotalDLPRError"];
		m_ulTotalInspOptPRError					= m_smfMachine["Stats"]["m_ulTotalInspOptPRError"];
	}
	else
	{
		_time64(&ltime);
		szMsg = _ctime64(&ltime);

		szMsg += _T(": Saving Machine file...");

		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			szStationName = GetStationList().GetAt(i)->GetStnName();
			GetStationList().GetAt(i)->IPC_SaveMachineParam();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_SavePRTaskStnMachinePara();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_SaveMachineRecord();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		if (g_bIsVisionNTInited)
		{
			PRU *pPRU = NULL;
			CString szErrMsg = _T(" ");
			for (INT i = 0; i < pCWinEagle->GetmyCalibPRUList().GetSize(); i++)
			{
				pPRU = pCWinEagle->GetmyCalibPRUList().GetAt(i);
				if (!PRS_UploadRecordProcess(pPRU, &szErrMsg))
				{
					ShowPRUploadErrorMessage(pPRU, szErrMsg); //20130713
				}
				KeepResponse();	// 20140714 Yip
			}
		}
	}

	m_smfMachine.Update();
	slLock.Unlock();

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////
//IPC Service,  Request machine
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
//IPC Service,  Update machine
////////////////////////////////////////////////////////////////////////////////////

LONG CPI9000App::SrvUpdateMachine(IPC_CServiceMessage &svMsg)
{
	CSingleLock slLock(&m_csMachine);
	slLock.Lock();

	BOOL  bResult = TRUE;
	LONG  lStation = -1;
	LONG *pBuffer = NULL, *pData = NULL;

	BOOL bOpDone = TRUE;
	CString szStationName;
	CString szCommand;
	CString szErrorMsg;
	
	CString szMsg = "";
	__time64_t ltime;

	CString szpassword = pCHouseKeeping->szEngPassword;

	if (!HMIGetPassword("Please enter password", szpassword))
	{
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}

	try
	{
		INT	nMsgLen	= svMsg.GetMsgLen();		//in Byte
		pData		= new LONG[nMsgLen];	//The first index, in the message is lStation

		svMsg.GetMsg(nMsgLen, pData); //Decipher message
		pBuffer = pData;
		lStation = *pBuffer++; //pBuffer[0]

		LogAction(__FUNCTION__);
		LogParameter(__FUNCTION__, "Machine File", "Previous File = " + m_szMachineFile, "Updated File = " + m_szMachineFile);

		_time64(&ltime);
		szMsg = _ctime64(&ltime);

		szMsg += _T(": Saving Machine file...");
		DisplayMessage(szMsg);

		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			GetStationList().GetAt(i)->IPC_SaveMachineParam();
			ProcessHMIRequest();
		}
		//pCTrayIndexer->IPC_SaveMachineParam();

		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_SavePRTaskStnMachinePara();
			ProcessHMIRequest();
		}
		// PRUs
		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_SaveMachineRecord();
			ProcessHMIRequest();
		}
	
		if (g_bIsVisionNTInited)
		{
			CString szErrMsg = _T(" ");
			PRU *pPRU = NULL;
			for (INT i = 0; i < pCWinEagle->GetmyCalibPRUList().GetSize(); i++)
			{
				pPRU = pCWinEagle->GetmyCalibPRUList().GetAt(i);
				if (!PRS_UploadRecordProcess(pPRU, &szErrMsg))
				{
					ShowPRUploadErrorMessage(pPRU, szErrMsg); //20130713
				}
			}
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	if (bResult)
	{
		//Inform Stn to update profile
		//CAppStation* pAppStn = dynamic_cast<CAppStation*> (GetStation(m_pInitOperation->GetStationName(lStation)));
		//pAppStn->SetUpdateProfile(TRUE, FALSE);
		m_smfMachine.Update();
		
		HMIMessageBox(MSG_OK, "UPDATE MACHINE", "Machine File Saved");
	}
	else
	{
		HMIMessageBox(MSG_OK, "UPDATE MACHINE", "Machine File Save Error");
	}
	svMsg.InitMessage(sizeof(bResult), &bResult); //Return Values

	if (pData)
	{
		//delete pData; //Free up memory
		delete[] pData;  //Free up memory //klocwork fix 20121211
	}

	slLock.Unlock();
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////
//Device File
////////////////////////////////////////////////////////////////////////////////////
BOOL CPI9000App::LoadDeviceFile(BOOL bLoad, BOOL bDeletePR)
{
	CSingleLock slLock(&m_csDevice);
	slLock.Lock();

	CFileFind			fileFind;
	CPositionAxisObj	*pPos = NULL;
	
	CString szStationName;
	CString szCommand;
	CString szErrorMsg;
	
	CString szMsg = "";
	__time64_t ltime;

	//Create Param Directory if not exist
	if (!fileFind.FindFile(m_szDevicePath))
	{
		if (!m_pInitOperation->CreateFileDirectory(m_szDevicePath))
		{
			AfxMessageBox("Can not create Directory");
			slLock.Unlock();

			return FALSE;
		}
	}

	//Open Device File (Create a new one if not found)
	m_smfDevice.Close(); //Clear File buffer

	if (!m_smfDevice.Open(m_szDevicePath + "\\" + m_szDeviceFile, FALSE, TRUE))
	{
		m_szDeviceFile = "CG144.prm";

		if (!m_smfDevice.Open(m_szDevicePath + "\\" + m_szDeviceFile, FALSE, TRUE))
		{
			AfxMessageBox("Cannot load File \"" + m_szDevicePath + "\\" + m_szDeviceFile + "\"");
			slLock.Unlock();
			return FALSE;
		}
	}

	if (!bLoad)
	{

		_time64(&ltime);
		szMsg = _ctime64(&ltime);

		szMsg += _T(": Creating Device file...");
		DisplayMessage(szMsg);

		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			GetStationList().GetAt(i)->IPC_SaveDeviceParam();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}
	
		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_SavePRTaskStnDevicePara();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		// PRUs
		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_SaveDeviceRecord();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}
	
		if (g_bIsVisionNTInited)
		{
			CString szErrMsg = _T(" ");
			PRU *pPRU = NULL;
			for (INT i = 1; i < pCWinEagle->GetmyPRUList().GetSize(); i++)
			{
				pPRU = pCWinEagle->GetmyPRUList().GetAt(i);
				if (!PRS_UploadRecordProcess(pPRU, &szErrMsg))
				{
					ShowPRUploadErrorMessage(pPRU, szErrMsg); //20130713
				}
				KeepResponse();	// 20140714 Yip
			}
		}

		slLock.Unlock();
		return TRUE;
	}
	else
	{
		_time64(&ltime);
		szMsg = _ctime64(&ltime);

		szMsg += _T(": Loading Device file: [") + m_szDeviceFile + _T("]");
		DisplayMessage(szMsg);

		// Must Load HouseKeeping before other
		pCHouseKeeping->IPC_LoadDeviceParam();

		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			GetStationList().GetAt(i)->IPC_LoadDeviceParam();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_LoadPRTaskStnDevicePara();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}
		// PRUs
		for (int i = 0; i < GetPRStationList().GetSize(); i++)
		{
			GetPRStationList().GetAt(i)->IPC_LoadDeviceRecord();
			ProcessHMIRequest();
			KeepResponse();	// 20140714 Yip
		}

		if (g_bIsVisionNTInited)
		{
			CString szErrMsg = _T(" ");
			PRU *pPRU = NULL;
			for (INT i = 1; i < pCWinEagle->GetmyPRUList().GetSize(); i++)
			{
				LONG lRetRecordID = 0;
				pPRU = pCWinEagle->GetmyPRUList().GetAt(i);
				PRS_FreeRecordID(pPRU); //20180208
				szMsg.Format("FreeRecordID %s, RetRecordID:%ld", pPRU->stDownloadRecordCmd.acFilename, (LONG)pPRU->stSrchCmd.uwRecordID);
				DisplayMessage(szMsg);
				if (!PRS_DownloadRecordProcess(pPRU, TRUE, lRetRecordID, &szErrMsg))
				{
					ShowPRDownloadErrorMessage(pPRU, szErrMsg);
				}
				else if(pPRU->bLoaded)
				{
					szMsg.Format("Load Device PR File %s, RetRecordID:%ld", pPRU->stDownloadRecordCmd.acFilename, lRetRecordID);
					DisplayMessage(szMsg);
					pCWinEagle->SetRecordIDToPRUCmd(pPRU, (PRU_ID)lRetRecordID);
				}
				ProcessHMIRequest();
				KeepResponse();	// 20140714 Yip
			}
		}

		//UpdateSetupStatus()
		//pCInspWH->UpdateSetupStatus();
		pCWH1->UpdateSetupStatus();
		pCWH2->UpdateSetupStatus();
	}



	m_smfDevice.Update();
	slLock.Unlock();

	return TRUE;
}



LONG CPI9000App::SrvUpdateDevice(IPC_CServiceMessage &svMsg)
{
	CSingleLock slLock(&m_csDevice);
	slLock.Lock();

	BOOL  bResult = TRUE;
	
	CString szMsg = "";	
	__time64_t ltime;

	//LONG  lStation = -1;
	//LONG* pBuffer = NULL, *pData = NULL;

	try
	{
		//INT nMsgLen	= svMsg.GetMsgLen();		//in Byte
		//pData		= new LONG[nMsgLen];  //The first index, in the message is lStation

		//svMsg.GetMsg(nMsgLen, pData); //Decipher message
		//pBuffer  = pData;
		//lStation = *pBuffer++; //pBuffer[0]

		//switch (lStation)
		//{
		//}

		//Open Device File
		m_smfDevice.Close(); //Clear File buffer

		if (!m_smfDevice.Open(m_szDevicePath + "\\" + m_szDeviceFile, FALSE, FALSE))
		{
			AfxMessageBox("Device file missing!");
			bResult = FALSE;
		}
		else
		{
			LogAction(__FUNCTION__ + m_szDeviceFile);
			LogParameter(__FUNCTION__, "Device File", "Previous File = " + m_szDeviceFile, "Updated File = " + m_szDeviceFile);

			_time64(&ltime);
			szMsg = _ctime64(&ltime);

			szMsg += _T(": Saving Device file : [") + m_szDeviceFile + _T("]");
			DisplayMessage(szMsg);
						
			for (int i = 0; i < GetStationList().GetSize(); i++)
			{
				GetStationList().GetAt(i)->IPC_SaveDeviceParam();
				ProcessHMIRequest();
			}
			
			for (int i = 0; i < GetPRStationList().GetSize(); i++)
			{
				GetPRStationList().GetAt(i)->IPC_SavePRTaskStnDevicePara();
				ProcessHMIRequest();
			}
			// PRUs
#if 1 //20130812 create path if it is not already exist
			CString szPath = _T(" ");
			CString szDevice = m_szDeviceFile;
			INT nFind = szDevice.ReverseFind('.');

			szPath = "d:\\sw\\PI9000\\Record\\" + szDevice.Left(nFind) + "_";
			CreatePath(szPath);
#endif
			for (int i = 0; i < GetPRStationList().GetSize(); i++)
			{
				GetPRStationList().GetAt(i)->IPC_SaveDeviceRecord();
				ProcessHMIRequest();
			}

			if (g_bIsVisionNTInited)
			{
				CString szErrMsg = _T(" ");
				PRU *pPRU = NULL;
				for (INT i = 1; i < pCWinEagle->GetmyPRUList().GetSize(); i++)
				{
					pPRU = pCWinEagle->GetmyPRUList().GetAt(i);
					
					szMsg.Format("saveRecord:%s", pPRU->stUploadRecordCmd.acFilename);
					DisplayMessage(szMsg);
					if (!PRS_UploadRecordProcess(pPRU, &szErrMsg))
					{
						ShowPRUploadErrorMessage(pPRU, szErrMsg); //20130713
					}
					ProcessHMIRequest();
				}
			}
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		bResult = FALSE;
	}

	BOOL bOpDone = TRUE;

	if (bResult)
	{
		_time64(&ltime);
		szMsg = _ctime64(&ltime);
		szMsg += _T(": Device file Saved");
		DisplayMessage(szMsg);

		//Inform Stn to update position
		//CAppStation* pAppStn = dynamic_cast<CAppStation*> (GetStation(m_pInitOperation->GetStationName(lStation)));
		//pAppStn->SetUpdatePosition(TRUE, FALSE);
		m_smfDevice.Update();

		// 20140813 Yip: Update Device File Path In Registry After Updated Device
		WriteProfileString(gszPROFILE_SETTING, gszDEVICE_FILE_SETTING, m_szDeviceFile);
		WriteProfileString(gszPROFILE_SETTING, gszDEVICE_PATH_SETTING, m_szDevicePath);

		HMIMessageBox(MSG_OK, "UPDATE DEVICE", "Device File Saved");

	}
	else
	{
		_time64(&ltime);
		szMsg = _ctime64(&ltime);
		szMsg += _T(": Save Device file fail");
		DisplayMessage(szMsg);

		HMIMessageBox(MSG_OK, "UPDATE DEVICE", "Device File Save Error");

	}
	//if (pData)
	//{
	//	delete pData;
	//}

	slLock.Unlock();
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);

	return 1;
}

LONG CPI9000App::SrvChangeDevice(IPC_CServiceMessage &svMsg)
{
	BOOL	bResult = TRUE;
	LPTSTR	lpsz	= NULL;
	CString szDevice, szPath, szFile, szExt;
	INT		nFind = 0, nMsgLen = 0;

	// Add me later
	// Check Heater Auto-tune in progress
	
	try
	{
		//Message Structure: char* strNewDeviceFileName  //E.g: C:\abc\yy\NewDevice.dev
		//Get New device file name
		nMsgLen	= svMsg.GetMsgLen();
		lpsz	= new TCHAR[nMsgLen];
		svMsg.GetMsg(lpsz, nMsgLen);	//Decipher message
		
		//Get New File Path and Name
		szDevice = ToString(lpsz); //Convert string char to CString	
		nFind	= szDevice.ReverseFind('\\');
		
		if (nFind == -1)
		{
			//Keyed in file name without directory info (e.g: "Abc")
			//Append current directory with file name
			szPath		= m_szDevicePath;
			szFile		= szDevice;
			szDevice	= szPath + "\\" + szFile;
		}
		else
		{
			//Keyed in file name with directory info (e.g: "C:\Xyz\Abc.prp")
			szPath	= szDevice.Left(nFind);
			szFile = szDevice.Right(szDevice.GetLength() - nFind - 1);
		}

		//Check if device exist
		CFileFind fileFind;
		if (!fileFind.FindFile(szDevice) || szPath.IsEmpty() || szFile.IsEmpty())
		{
			HmiMessage("File does not exist \"" + szDevice + "\"", "CHANGE DEVICE", glHMI_MBX_OK);
			bResult = FALSE;
		}

		if (bResult)
		{
			//Save old device file name and path
			CString szOldPath = m_szDevicePath;
			CString szOldFile = m_szDeviceFile;
			
			//Save new device file name and path
			m_szDeviceFile	= szFile;
			m_szDevicePath	= szPath;

			if (bResult = LoadDeviceFile()) //Load new device
			{
				LogParameter(__FUNCTION__, "Device File", "Previous File = " + szOldFile, "Loaded File = " + m_szDeviceFile);

				//Save Machine and Device and software version
				if (pCHouseKeeping->m_dPrevSWVersion < pCHouseKeeping->m_dCurSWVersion)
				{
					if (LoadDeviceFile(FALSE))
					{
						DisplayMessage("Device Data Convertion Finished");
					}
					else
					{
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						DisplayMessage("ERROR: Unable to save Device file");
						DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						HMIMessageBox(MSG_OK, "LOAD DEVICE", "Device File Load Error");
						
						BOOL bOpDone = TRUE;
						svMsg.InitMessage(sizeof(BOOL), &bOpDone);
						return 1;
					}
				}

				SaveSoftwareVersion();
				
				// Reset all Data
				//pCTrayHolder->m_lCurTrayHolder		= TRAYHOLDER_1;
				INT i = 0;
				INT j = 0;
				//for (i = LSI_TYPE1; i < MAX_NUM_OF_LSI_TYPE; i++)
				//{
				//	//for (j = COL_1; j < pCTrayHolder->m_stTrayHolder[TRAYHOLDER_1].lTrayInCol; j++)
				//	//{
				//	//	if ((LSITypeNum)pCTrayHolder->m_stTrayHolder[TRAYHOLDER_1].lTrayTypeInCol[j] == i)
				//	//	{
				//	//		pCTrayHolder->m_WhCurTray[i].Col	= j + 1;
				//	//		pCTrayHolder->m_WhCurTray[i].Row	= 1;
				//	//		break;
				//	//	}
				//	//}

				//	//if (j == pCTrayHolder->m_stTrayHolder[TRAYHOLDER_1].lTrayInCol)
				//	//{
				//	//	pCTrayHolder->m_WhCurTray[i].Col	= 1;
				//	//	pCTrayHolder->m_WhCurTray[i].Row	= 1;
				//	//}

				//	//pCTrayHolder->m_WhCurPocket[i].Col	= 1;
				//	//pCTrayHolder->m_WhCurPocket[i].Row	= 1;
				//}

				WriteProfileString(gszPROFILE_SETTING, gszDEVICE_FILE_SETTING, m_szDeviceFile);
				WriteProfileString(gszPROFILE_SETTING, gszDEVICE_PATH_SETTING, m_szDevicePath);

				HMIMessageBox(MSG_OK, "LOAD DEVICE", "Device File Loaded");

			}
			else
			{
				//If can not load new device, Revert back to old one
				m_szDevicePath = szOldPath;
				m_szDeviceFile = szOldFile;
				//LoadDeviceFile();
				
				HMIMessageBox(MSG_OK, "LOAD DEVICE", "Device File Load Error");

			}
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	if (lpsz)
	{
		//delete lpsz;
		delete[] lpsz;  //klocwork fix 20121211
	}

	//if (bResult)
	//{
	//	////Inform all stations (and sub stations) to update device
	//	//CString			szStn;
	//	//SFM_CStation*	pStn;
	//	//CAppStation*		pAppStn;

	//	//for (POSITION pos = m_pStationMap.GetStartPosition(); pos;)
	//	//{
	//	//	m_pStationMap.GetNextAssoc(pos, szStn, pStn);
	//	//	pAppStn = dynamic_cast<CAppStation*> (pStn);			
	//	//	(pAppStn) ? pAppStn->SetUpdatePosition(TRUE) : 0;  //Inform all stns & sub stns
	//	//}
	//}

	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}


LONG CPI9000App::SrvNewDevice(IPC_CServiceMessage &svMsg)
{
	BOOL bResult	= TRUE;
	LPTSTR lpsz		= NULL;
	CString			szDevice;
	CString			szPath;
	CString			szFile;
	CString			szExt;
	INT nFind		= 0;
	INT nFindExt	= 0;
	INT nMsgLen		= 0;
	BOOL bOpDone	= TRUE;

	LONG			lAnswer = rMSG_TIMEOUT;
	
	CString szMsg = "";
	__time64_t ltime;

	LogAction(__FUNCTION__ + m_szDeviceFile);

	try
	{
		//Message Structure: char* strDestFileName  //E.g: C:\abc\yy\NewDevice.dev
		//Get New device file name
		nMsgLen	= svMsg.GetMsgLen();
		lpsz	= new TCHAR[nMsgLen];
		svMsg.GetMsg(lpsz, nMsgLen);	//Decipher message
		
		//Get new File Path and Name
		szDevice = ToString(lpsz); //Convert string char to CString	
#if 0 //20121018
		nFind	= szDevice.ReverseFind('\\');

		if (nFind == -1)
		{
#endif
			//Keyed in file name without directory info (e.g: "ABC.prm")
			nFindExt = szDevice.Find('.');
			if (nFindExt == -1)
			{
				//Keyed in file name without extension
				// Avoid space bar at the end
				//if (szDevice.GetAt(szDevice.GetLength()-1) == ' ')
				//{
				//	szDevice = szDevice.Left(szDevice.GetLength() - 1);
				//}
				szDevice += ".prm";
			}
			else // avoid user enter abc.prm.prm.....
			{
				//Keyed in file name with extension
				szDevice = szDevice.Left(nFindExt);
				//if (szDevice.GetAt(szDevice.GetLength()-1) == ' ')
				//{
				//	szDevice = szDevice.Left(szDevice.GetLength() - 1);
				//}
				szDevice += ".prm";
			}

			//Append current directory with file name
			szPath		= m_szDevicePath;
			szFile		= szDevice;
			szDevice	= szPath + "\\" + szFile;
#if 0 //20121018
		}
		else
		{
			//Keyed in file name with directory info (e.g: "D:\SW\PI9000\Device\ABC.prm")
			szPath		= szDevice.Left(nFind);
			szFile		= szDevice.Right(szDevice.GetLength() - nFind - 1);
		}
#endif
		// All that variabel can;t exist as the folder name 
		if (
			szFile.Find('\\') != -1 || szFile.Find('/') != -1 || szFile.Find(':') != -1 || 
			szFile.Find('*') != -1 || szFile.Find('?') != -1 || szFile.Find('"') != -1 ||
			szFile.Find('<') != -1 || szFile.Find('>') != -1 || szFile.Find('|') != -1 ||
			szFile.Find('!') != -1 || szFile.Find('@') != -1 || szFile.Find('#') != -1 || 
			szFile.Find('$') != -1 || szFile.Find('%') != -1 || szFile.Find('^') != -1 ||
			szFile.Find('&') != -1 || szFile.Find('*') != -1 || szFile.Find('(') != -1 ||
			szFile.Find(')') != -1 ||
			szFile.Find('[') != -1 || szFile.Find(']') != -1 || szFile.Find('+') != -1 || //20120104
			szFile.Find('=') != -1 || szFile.Find(' ') != -1
		   )
		{
			HMIMessageBox(MSG_OK, "NEW DEVICE", "File Name cannot include \\//:*?'<>|!@#$%^&*()[]+= characters and [WHITE SPACE]");
			bResult = FALSE;
		}

		if (szFile.GetLength() > 25)
		{
			HMIMessageBox(MSG_OK, "NEW DEVICE", "Length of filename must be less than 20 characters");
			bResult = FALSE;
		}

		
		//Save old device file path and name
		CString szOldPath = m_szDevicePath;
		CString szOldFile = m_szDeviceFile;

		//Save new device file path and name
		m_szDevicePath	= szPath;
		m_szDeviceFile	= szFile;

		if (bResult)
		{
			//Create Directory if not exist
			CFileFind fileFind;
			nFind = m_szDeviceFile.ReverseFind('.');
			if (!fileFind.FindFile("d:\\sw\\PI9000\\Record\\" + m_szDeviceFile.Left(nFind) + "\\"))
			{
				m_pInitOperation->CreateFileDirectory("d:\\sw\\PI9000\\Record\\" + m_szDeviceFile.Left(nFind) + "\\");
			}

			//check if path is valid
			if (szPath != szOldPath)
			{
				HMIMessageBox(MSG_OK, "NEW DEVICE", "Invalid Directory \"" + szPath + "\\\"");
				bResult = FALSE;
			}

			if (bResult)
			{
				//Copy current device file to new device file
				if (!m_smfDevice.SaveAs(szDevice, CStringMapFile::frmBINARY, FALSE))
				{
					//Device name exists!
					HMIMessageBox(MSG_OK, "NEW DEVICE", "Device Name Already Exist");
					bResult = FALSE;
				}
			}
			
			if (bResult)
			{
				//Close current file and open new device
				m_smfDevice.Close();

				if (!m_smfDevice.Open(szDevice, FALSE, FALSE))
				{
					HMIMessageBox(MSG_OK, "NEW DEVICE", "Cannot Load File \"" + szDevice + "\\");
					bResult = FALSE;
				}
			}

			if (bResult)
			{
#if 0 //20130713
				lAnswer = HMIMessageBox(MSG_YES_NO, "NEW DEVICE", "Delete all WorkHolder PR Position?");
				if (lAnswer == rMSG_YES)
				{
				}
				else if (lAnswer == rMSG_TIMEOUT)
				{
					bResult = TRUE;
					//If cannot create a new device, Revert back to old one
					m_szDevicePath = szOldPath;
					m_szDeviceFile = szOldFile;

					HMIMessageBox(MSG_OK, "NEW DEVICE", "Unable to Create New Device File");
				}
#endif
				//Reset some parameters and load the new file
				//
#if 1 //20130713
				bResult = LoadDeviceFile(FALSE, FALSE);
#else
				lAnswer = HMIMessageBox(MSG_YES_NO, "NEW DEVICE", "Copy current PR records to new device file?");
				if (lAnswer == rMSG_YES)
				{
					bResult = LoadDeviceFile(FALSE, FALSE);
				}
				else if (lAnswer == rMSG_NO)
				{
					bResult = LoadDeviceFile(FALSE);
				}
				else 
				{
					bResult = FALSE;
				}
#endif
				if (bResult)
				{
					LogParameter(__FUNCTION__, "Device File", "Previous File = " + szOldFile, "Created File = " + m_szDeviceFile);

					WriteProfileString(gszPROFILE_SETTING, gszDEVICE_FILE_SETTING, m_szDeviceFile);
					WriteProfileString(gszPROFILE_SETTING, gszDEVICE_PATH_SETTING, m_szDevicePath);

					HMIMessageBox(MSG_OK, "NEW DEVICE", "New Device File Created");

					_time64(&ltime);
					szMsg = _ctime64(&ltime);
					szMsg += _T(": Created Device file :") + m_szDeviceFile;
					DisplayMessage(szMsg);
				}
			}
		}

		if (!bResult)
		{
			//If cannot create a new device, Revert back to old one
			m_szDevicePath = szOldPath;
			m_szDeviceFile = szOldFile;

			HMIMessageBox(MSG_OK, "NEW DEVICE", "Unable to Create New Device File");
		}

	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}


	if (lpsz)
	{
		//delete lpsz;
		delete[] lpsz;  //klocwork fix 20121211
	}

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CPI9000App::SrvDeleteDevice(IPC_CServiceMessage &svMsg)
{
	BOOL	bResult = TRUE;
	LPTSTR	lpsz	= NULL;
	CString szDevice, szPath, szFile, szFolder, szTemp, szExt;
	INT		nFind = 0, nMsgLen = 0;

	try
	{
		//Message Structure: char* strNewDeviceFileName (e.g: "D:\SW\PI9000\Device\ABC.prm")
		//Get New device file name
		nMsgLen	= svMsg.GetMsgLen();
		lpsz	= new TCHAR[nMsgLen];
		svMsg.GetMsg(lpsz, nMsgLen);	//Decipher message
		
		//Get New File Path and Name
		szDevice = ToString(lpsz); //Convert string char to CString	
		nFind	= szDevice.ReverseFind('\\');
		
		if (nFind == -1)
		{
			//Keyed in file name without directory info (e.g: "ABC.prm")
			//Append current directory with file name
			szPath		= m_szDevicePath;
			szFile		= szDevice;
			szDevice	= szPath + "\\" + szFile;
		}
		else
		{
			//Keyed in file name with directory info (e.g: "D:\SW\PI9000\Device\ABC.prm")
			szPath		= szDevice.Left(nFind);
			szFile		= szDevice.Right(szDevice.GetLength() - nFind - 1);
		}

		nFind = szFile.ReverseFind('.');
		szFolder = "d:\\sw\\PI9000\\Record\\" + szFile.Left(nFind) + "\\";

		BOOL bOpDone = TRUE;

		//Check if device exist
		CFileFind fileFind;
		if (!fileFind.FindFile(szDevice) || szPath.IsEmpty() || szFile.IsEmpty())
		{
			HMIMessageBox(MSG_OK, "DELETE DEVICE", "File does not exist \"" + szDevice + "\"");
			bResult = FALSE;
		}

		if (bResult)
		{
			if (rMSG_CONTINUE == HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Delete file: " + szFile + "?"))
			{
				if (0 != remove(szDevice))
				{
					HMIMessageBox(MSG_OK, "DELETE DEVICE", "ERROR: Unable to Delete File!");
				}

				CFileOperation fo;
				fo.Delete(szFolder);
			}
			else
			{
				WriteHMIMess("Operation Cancelled!");
			}
		}
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);

	if (lpsz)
	{
		//delete lpsz;
		delete[] lpsz;  //klocwork fix 20121211
	}

	if (bResult)
	{
		//Inform all stations (and sub stations) to update device
		CString			szStn;
		SFM_CStation	*pStn = NULL;
		CAppStation	*pAppStn;

		for (POSITION pos = m_pStationMap.GetStartPosition(); pos;)
		{
			m_pStationMap.GetNextAssoc(pos, szStn, pStn);
			pAppStn = dynamic_cast<CAppStation*>(pStn);
			(pAppStn) ? pAppStn->SetUpdatePosition(TRUE) : 0;  //Inform all stns & sub stns
		}
	}

	return 1;
}

LONG CPI9000App::SrvRestoreMachine(IPC_CServiceMessage &svMsg)
{
	if (rMSG_CONTINUE == HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "All changes to Machine file will be discarded. Continue?"))
	{
		LogAction(__FUNCTION__);
		LogParameter(__FUNCTION__, "Machine File", "Previous File = " + m_szMachineFile, "Restored File = " + m_szMachineFile);

		try
		{
			if (LoadMachineFile()) //Load machine file
			{
				HMIMessageBox(MSG_OK, "LOAD MACHINE", "Machine File Loaded");

			}
			else
			{
				HMIMessageBox(MSG_OK, "LOAD MACHINE", "Machine File Load Error");
			}
		}
		catch (CAsmException e)
		{
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			DisplayException(e);
			DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		}
	}
	else
	{
		HMIMessageBox(MSG_OK, "LOAD MACHINE", "Operation Abort!");
	}

	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);

	return 1;
}

LONG CPI9000App::SrvPrintMachine(IPC_CServiceMessage &svMsg)
{
	__time64_t ltime;
	CString szTime = "";

	_time64(&ltime);
	szTime = _ctime64(&ltime);

	FILE *fp = fopen("D:\\sw\\PI9000\\Data\\machine.txt", "w+");
	if (fp != NULL)
	{
		fprintf(fp, "******************************************************\n");
		fprintf(fp, "*                                                    *\n");
		fprintf(fp, "*     PI9000 High Precision Chip On Glass Bonder     *\n");
		fprintf(fp, "*                                                    *\n");
		fprintf(fp, "******************************************************\n\n");

		fprintf(fp, "Date: " + szTime);

		fprintf(fp, "Machine ID: %s\n", (const char*)m_szMachineID);
		
		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			GetStationList().GetAt(i)->PrintMachineSetup(fp);
			GetStationList().GetAt(i)->PrintCalibSetup(fp);
			GetStationList().GetAt(i)->PrintForceCalibData(fp);
		}

		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			GetStationList().GetAt(i)->PrintPRCalibSetup(fp);
		}

		fclose(fp);
	}

	HMI_PrintMachineDeviceInfoExcel(svMsg);
	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);

	return 1;
}

LONG CPI9000App::SrvPrintDevice(IPC_CServiceMessage &svMsg)
{
	__time64_t ltime;
	CString szTime = "";

	_time64(&ltime);
	szTime = _ctime64(&ltime);

	FILE *fp = fopen("D:\\sw\\PI9000\\Data\\device.txt", "w+");
	if (fp != NULL)
	{
		fprintf(fp, "******************************************************\n");
		fprintf(fp, "*                                                    *\n");
		fprintf(fp, "*     PI9000 High Precision Chip On Glass Bonder     *\n");
		fprintf(fp, "*                                                    *\n");
		fprintf(fp, "******************************************************\n\n");

		fprintf(fp, "Date: " + szTime);

		fprintf(fp, "Machine ID:	%s\n", (const char*)m_szMachineID);
		fprintf(fp, "Device Name:	%s\n", (const char*)m_szDeviceFile);
		
		CString szStationName;
		
		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			szStationName = GetStationList().GetAt(i)->GetStnName(); //20130228
			DisplayMessage("printDeviceSetup:" + szStationName);
			GetStationList().GetAt(i)->PrintDeviceSetup(fp);
		}

		for (int i = 0; i < GetStationList().GetSize(); i++)
		{
			GetStationList().GetAt(i)->PrintPRSetup(fp);
		}
		fclose(fp);
	}

	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);

	return 1;

}
////////////////////////////////////////////////////////////////////////////////////
//Register Hmi Variables
////////////////////////////////////////////////////////////////////////////////////

VOID CPI9000App::RegisterVariables()
{
	try
	{
#if 1	//cal whz
		RegVariable(_T("HMI_bInspWHZ1_Selected"), &HMI_bInspWHZ1_Selected);
		RegVariable(_T("HMI_bInspWHZ2_Selected"), &HMI_bInspWHZ2_Selected);

#endif
#ifdef GATEWAY
		RegVariable(_T("HMI_lGatewayStatus"), &HMI_lGatewayStatus); //20130619
#endif
		//Machine and Device File
		RegVariable(_T("ParamPath"), &m_szMachinePath);
		RegVariable(_T("DevicePath"), &m_szDevicePath);
		RegVariable(_T("DeviceFile"), &m_szDeviceFile);
		RegVariable(_T("MachineFile"), &m_szMachineFile);
		RegVariable(_T("SelectedFile"), &m_szSelectedFile);

		//Lot Info
		RegVariable(_T("LotNumber"), &m_szLotNumber);
		RegVariable(_T("LotStartTime"), &m_szLotStartTime);
		RegVariable(_T("LotEndTime"), &m_szLotEndTime);
		
		//Software/ Machine info
		RegVariable(_T("MachineModel"), &m_szMachineModel);
		RegVariable(_T("MachineID"), &m_szMachineID);
		RegVariable(_T("OSImageNumber"), &m_szOSImageNumber);
		RegVariable(_T("SoftwareNumber"), &m_szSoftwareNumber);
		RegVariable(_T("SoftVersion"), &m_szSoftVersion);
		RegVariable(_T("ReleaseDate"), &m_szReleaseDate);

		RegVariable(_T("VisionOSImageNumber"), &m_szOSImageNumber);
		
		RegVariable(_T("LitecVersion1"), &m_szLitecVersion1);
		RegVariable(_T("LitecVersion2"), &m_szLitecVersion2);
		RegVariable(_T("LitecVersion3"), &m_szLitecVersion3);
		RegVariable(_T("LitecVersion4"), &m_szLitecVersion4);

		RegVariable(_T("MotionLibVersion"), &m_szMotionLibVersion); //20121210

		// S/W status
		RegVariable(_T("CurState"), &m_szState);

		// PR init
		RegVariable(_T("PRInit"), &g_bIsVisionNTInited);
		// Hardware init
		RegVariable(_T("HWInited"), &m_bHWInited);
		// Hardware Error
		RegVariable(_T("HWInitError"), &m_bHWInitError);
		RegVariable(_T("HWInitFatalErrorUI"), &m_bHWInitFatalErrorUI); //20130710
		
		// Invalid Init State
		RegVariable(_T("InvalidInitState"), &m_bInvalidInitState);
		// 
		RegVariable(_T("InitInstanceError"), &m_bInitInstanceError);
		
		RegVariable(_T("EcoreInited"), &m_bAllLitecInited); //20121004

		// Message windows
		RegVariable(_T("szHMIMess"), &m_szHMIMess);
		RegVariable(_T("bHMIMess"), &m_bHMIMess);

		// Diagnostic testing
		RegVariable(_T("bPRRepeatTest"), &bPRRepeatTest);

		// statistics
		RegVariable(_T("Stat_ulGlassPicked"), &m_ulGlassPicked);
		RegVariable(_T("Stat_ulTotalGlassPicked"), &m_ulTotalGlassPicked);

		RegVariable(_T("Stat_ulGlassRejected"), &m_ulGlassRejected);
		RegVariable(_T("Stat_ulTotalGlassRejected"), &m_ulTotalGlassRejected);

		RegVariable(_T("Stat_ulDLG1PRError"), &m_ulDLG1PRError);
		RegVariable(_T("Stat_ulDLG2PRError"), &m_ulDLG2PRError);
		RegVariable(_T("Stat_ulTotalDLPRError"), &m_ulTotalDLPRError);

		RegVariable(_T("Stat_ulInspOptPRError"), &m_ulInspOptPRError);
		RegVariable(_T("Stat_ulTotalInspOptPRError"), &m_ulTotalInspOptPRError);

		RegVariable(_T("Stat_ulGlassTolError"), &m_ulGlassTolError);
		RegVariable(_T("Stat_ulTotalGlassTolError"), &m_ulTotalGlassTolError);
		
		RegVariable(_T("Stat_ulUnitsInpected"), &m_ulUnitsInpected);
		RegVariable(_T("Stat_ulTotalUnitsInpected"), &m_ulTotalUnitsInpected);

		RegVariable(_T("Stat_dUPH"), &m_dUPH);

		RegVariable(_T("MotorPosLmt"), &HMI_lMotorPosLmt);
		RegVariable(_T("MotorNegLmt"), &HMI_lMotorNegLmt);	
		RegVariable(_T("szMotorDirectionPng"), &HMI_szMotorDirectionPng);

		RegVariable(_T("ulSettingPerformanceReportStartTime"), &HMI_ulSettingReportManagerStartTime);
		RegVariable(_T("Stat_ulDailyPerformanceReportPage"), &g_ulDailyPerformanceReportPage);	

		RegVariable(_T("szSettingReportManagerShift1"), &HMI_szSettingReportManagerShift1);
		RegVariable(_T("szSettingReportManagerShift2"), &HMI_szSettingReportManagerShift2);
		RegVariable(_T("szSettingReportManagerShift3"), &HMI_szSettingReportManagerShift3);
		RegVariable(_T("szSettingReportManagerShift4"), &HMI_szSettingReportManagerShift4);
		RegVariable(_T("szSettingReportManagerShift5"), &HMI_szSettingReportManagerShift5);
		RegVariable(_T("szSettingReportManagerShift6"), &HMI_szSettingReportManagerShift6);
		RegVariable(_T("szSettingReportManagerShift7"), &HMI_szSettingReportManagerShift7);
		RegVariable(_T("szSettingReportManagerShift8"), &HMI_szSettingReportManagerShift8);
		RegVariable(_T("szSettingReportManagerShift9"), &HMI_szSettingReportManagerShift9);
		RegVariable(_T("szSettingReportManagerShift10"), &HMI_szSettingReportManagerShift10);
		RegVariable(_T("szSettingReportManagerShift11"), &HMI_szSettingReportManagerShift11);
		RegVariable(_T("szSettingReportManagerShift12"), &HMI_szSettingReportManagerShift12);
		RegVariable(_T("szSettingReportManagerShift13"), &HMI_szSettingReportManagerShift13);
		RegVariable(_T("szSettingReportManagerShift14"), &HMI_szSettingReportManagerShift14);
		RegVariable(_T("szSettingReportManagerShift15"), &HMI_szSettingReportManagerShift15);
		RegVariable(_T("szSettingReportManagerShift16"), &HMI_szSettingReportManagerShift16);
		RegVariable(_T("szSettingReportManagerShift17"), &HMI_szSettingReportManagerShift17);
		RegVariable(_T("szSettingReportManagerShift18"), &HMI_szSettingReportManagerShift18);
		RegVariable(_T("szSettingReportManagerShift19"), &HMI_szSettingReportManagerShift19);
		RegVariable(_T("szSettingReportManagerShift20"), &HMI_szSettingReportManagerShift20);
		RegVariable(_T("szSettingReportManagerShift21"), &HMI_szSettingReportManagerShift21);
		RegVariable(_T("szSettingReportManagerShift22"), &HMI_szSettingReportManagerShift22);
		RegVariable(_T("szSettingReportManagerShift23"), &HMI_szSettingReportManagerShift23);
		RegVariable(_T("szSettingReportManagerShift24"), &HMI_szSettingReportManagerShift24);

		DisplayMessage("Hmi Variables Registered...");

		// commands
		m_comServer.IPC_REG_SERVICE_COMMAND("CloseApp", SrvCloseApp);
		m_comServer.IPC_REG_SERVICE_COMMAND("RestartHMI", SrvRestartHMI);
		m_comServer.IPC_REG_SERVICE_COMMAND("SetMachineID", SetMachineID);
		m_comServer.IPC_REG_SERVICE_COMMAND("CleanOperation", HMI_CleanOperation); //20121112

		m_comServer.IPC_REG_SERVICE_COMMAND("Stat_ResetStat", HMI_ResetStat);
		m_comServer.IPC_REG_SERVICE_COMMAND("GetAllStatus", HMI_GetAllStatus);

		m_comServer.IPC_REG_SERVICE_COMMAND("SetAllModSelected", HMI_SetAllModSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND("CheckAllModSelected", HMI_CheckAllModSelected);
		m_comServer.IPC_REG_SERVICE_COMMAND("SetAllPRSelected", HMI_SetAllPRSelected);

		m_comServer.IPC_REG_SERVICE_COMMAND("SetAllDebugSeq", HMI_SetAllDebugSeq);
		m_comServer.IPC_REG_SERVICE_COMMAND("SetAllLogMotion", HMI_SetAllLogMotion); //20130422

		m_comServer.IPC_REG_SERVICE_COMMAND("RestoreMachine", SrvRestoreMachine);
			
//p20120829
		m_comServer.IPC_REG_SERVICE_COMMAND("StatisticsLogFile", HMI_StatisticsLogFile);
//p20120829:end

		// Set Protection
		m_comServer.IPC_REG_SERVICE_COMMAND("SelectStation", HMI_SelectStation);
		m_comServer.IPC_REG_SERVICE_COMMAND("SelectStationMotor", HMI_SelectStationMotor);
		m_comServer.IPC_REG_SERVICE_COMMAND("SetMotorProtectPosLmt", HMI_SetMotorProtectPosLmt);
		m_comServer.IPC_REG_SERVICE_COMMAND("SetMotorProtectNegLmt", HMI_SetMotorProtectNegLmt);
		m_comServer.IPC_REG_SERVICE_COMMAND("EnterSoftwareLimitPage", HMI_EnterSoftwareLimitPage);
		m_comServer.IPC_REG_SERVICE_COMMAND("LeaveSoftwareLimitPage", HMI_LeaveSoftwareLimitPage);
		m_comServer.IPC_REG_SERVICE_COMMAND("PI9000_SetDiagnSteps", HMI_SetDiagnSteps);
		m_comServer.IPC_REG_SERVICE_COMMAND("PI9000_SWLimitIndexMotorPos", HMI_SWLimitIndexMotorPos);
		m_comServer.IPC_REG_SERVICE_COMMAND("PI9000_SWLimitIndexMotorNeg", HMI_SWLimitIndexMotorNeg);

		//m_comServer.IPC_REG_SERVICE_COMMAND("GetMaxSampleFreq", HMI_GetMaxSampleFreq);		// 20150409

		m_comServer.IPC_REG_SERVICE_COMMAND("PickGlassOperation", HMI_PickGlassOperation);

		m_comServer.IPC_REG_SERVICE_COMMAND("DataLog", HMI_DataLog);
		m_comServer.IPC_REG_SERVICE_COMMAND("HMI_KeepQuiet", HMI_KeepQuiet);
		
		m_comServer.IPC_REG_SERVICE_COMMAND("FlushMode", HMI_FlushMode); //20130403
#ifdef GATEWAY
		m_comServer.IPC_REG_SERVICE_COMMAND("HMI_CheckGatewayConnection", HMI_CheckGatewayConnection); //20130619
#endif
		m_comServer.IPC_REG_SERVICE_COMMAND("SettingReportManagerSaveShiftSchedule", HMI_SettingReportManagerSaveShiftSchedule);		//Philip

#ifdef EXCEL_MACHINE_DEVICE_INFO
		m_comServer.IPC_REG_SERVICE_COMMAND("HMI_PrintMachineDeviceInfoExcel", HMI_PrintMachineDeviceInfoExcel);
#endif
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}


}


////////////////////////////////////////////////////////////////////////////////////
//String Map File Save As
////////////////////////////////////////////////////////////////////////////////////

VOID CPI9000App::PrintMachineInfo()
{
	FILE *fp = fopen("D:\\sw\\PI9000\\Backup\\readme.txt", "wt");
	if (fp != NULL)
	{
		fprintf(fp, "Machine ID\t\t: %s\n", (const char*)gszMACHINE_ID_NO);
		fprintf(fp, "OS Image No\t: %s\n", (const char*)gszOS_IMAGE_NO);
		fprintf(fp, "Vision OS Image No\t: %s\n", (const char*)gszMACHINE_ID_NO);
		fprintf(fp, "\n");

		fprintf(fp, "SWR Number\t\t: %s\n", (const char*)gszSOFTWARE_NUMBER);
		fprintf(fp, "SW Version\t\t: %s\n", (const char*)gszSOFTWARE_VERSION);
		fprintf(fp, "SW Release Date\t\t: %s\n", (const char*)gszRELEASE_DATE);

		fclose(fp);
	}
}

/////////////////////////////////////////////////////////////////
//Startup Sequence
/////////////////////////////////////////////////////////////////
INT CPI9000App::MachineVersion()
{
	if (m_fSelectVer1)
	{
		return VERSION_1;
	}
	else
	{
		DisplayMessage("SEVEN OUT! Unknown Machine Version Detected");
		return MAX_NUM_OF_MACHINE_VERSION;
	}
}

VOID CPI9000App::TC_AddJob(INT emTCObjectID)
{
	if (!pCTimeChart->m_bModSelected || (State() != AUTO_Q && State() != DEMO_Q))
	{
		return;
	}

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;

	stMsg.InitMessage(sizeof(INT), &emTCObjectID);
	nConvID = m_comClient.SendRequest("TimeChart", "TC_AddCmd", stMsg, 0);
}

VOID CPI9000App::TC_DelJob(INT emTCObjectID)
{
	if (!pCTimeChart->m_bModSelected || (State() != AUTO_Q && State() != DEMO_Q))
	{
		return;
	}

	INT nConvID = 0;
	IPC_CServiceMessage stMsg;
	LONG lTimeCount = 0;

	stMsg.InitMessage(sizeof(INT), &emTCObjectID);
	nConvID = m_comClient.SendRequest("TimeChart", "TC_DelCmd", stMsg, 0);
}

BOOL CPI9000App::IsInitNuDrive()
{
	return (m_fEnableHardware);
}

BOOL CPI9000App::IsTempCtrlInited()
{
	return (m_bAllLitecInited);

}

BOOL CPI9000App::IsAnyModuleBusy()
{
	for (int i = 0; i < GetStationList().GetSize(); i++)
	{
		if (GetStationList().GetAt(i)->IsBusy())
		{
			return TRUE;
		}
	}
	return FALSE;

}

BOOL CPI9000App::IsAllServoCommutated()
{
	return (m_bInPickArmComm && m_bOutPickArmComm && m_bInspWH1Comm && m_bInspWH2Comm && m_bInspOptComm);
}

BOOL CPI9000App::IsAllMotorHomed()
{
	return (m_bInPickArmHome && m_bOutPickArmHome && m_bInspWH1Home && m_bInspWH2Home && m_bInspOptHome);
}




// 20140922 Yip: Add Datalog Mode And Stop Condition
INT CPI9000App::StartDataLog(CStringList *pszListPort, GMP_DATALOG_MODE enDatalogMode, GMP_LOGIC enStopLogic, GMP_U32 u32StopPattern)
{
	CStringList szlistPortFinal;

	if (pszListPort)
	{
		szlistPortFinal.AddTail(pszListPort);
	}
	else
	{

		CStdioFile fin;

		if (fin.Open("D:\\sw\\PI9000\\Data\\datalogport.txt", CFile::modeRead))
		{
			CString sMsg;
			while (fin.ReadString(sMsg))
			{
				if (sMsg.GetLength() > 0)
				{
					szlistPortFinal.AddTail(sMsg);
				}
			}
			fin.Close();
		}
		else
		{
			return GMP_FAIL;
		}
	}

	try
	{
		// 20140922 Yip: Add Datalog Mode And Stop Condition
		m_pCGmpSystem->EnableDatalog(&szlistPortFinal, enDatalogMode, enStopLogic, u32StopPattern);
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		SetError(IDS_DATALOG_FAIL);
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		return GMP_FAIL;
	}
	
	return GMP_SUCCESS;
}

INT CPI9000App::StopDataLog(CString szFileName)
{
	try
	{
		m_pCGmpSystem->DisableDatalog(szFileName);
	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		SetError(IDS_DATALOG_FAIL);
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		return GMP_FAIL;
	}

	return GMP_SUCCESS;
}

/////////////////////////////////////////////////////////////////
//Message Windows
/////////////////////////////////////////////////////////////////
VOID CPI9000App::WriteHMIMess(const CString &szMess, BOOL bLog, BOOL bLogTime)
{
	CString	szMessage	= _T("");
	__time64_t ltime;

	_time64(&ltime);
	CString szTime = _ctime64(&ltime);
	szTime = szTime.Trim('\n');

	if (bLogTime)
	{
		szMessage = _T("[") + szTime + _T("]") + _T("   ") + szMess;
	}
	else
	{
		szMessage = szMess;
	}

	m_csMsgLock.Lock();
	m_astrMsgQueue.Add(szMess);
	m_csMsgLock.Unlock();

	if (bLog)
	{
		DisplayMessage(szMess);
	}

}

/////////////////////////////////////////////////////////////////
//HMI Windows
/////////////////////////////////////////////////////////////////

LONG CPI9000App::HMIMessageBox(const INT nBoxType, const CString &szTitle, const CString &szText)
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
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
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

LONG CPI9000App::HMISelectionBox(const CString &szTitle, const CString &szText, const CString &szItemText1, const CString &szItemText2, const CString &szItemText3, const CString &szItemText4, const CString &szItemText5, const CString &szItemText6, const CString &szItemText7, const CString &szItemText8, const CString &szItemText9, const CString &szItemText10)
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
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
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

LONG CPI9000App::HMIConfirmWindow(const CString &szFirstbtn, const CString &szSecondbtn, const CString &szThirdbtn, const CString &szTitle, const CString &szText)
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

	/*CString szCustomText1 = "Yes";
	CString szCustomText2 = "No";
	CString szCustomText3 = "Cancel";*/

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
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
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

BOOL CPI9000App::HMIGetPassword(const CString &szTitle, const CString &szPassword)
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
				pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
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

BOOL CPI9000App::HMIAlphaKeys(const CString &szPrompt, CString *pszKeyIn, BOOL bKeyInPassword)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	BOOL bTRUE = TRUE;
	BOOL bFALSE = FALSE;

	stDummy = "";
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
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
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

BOOL CPI9000App::HMINumericKeys(const CString &szPrompt, const DOUBLE dMaxValue, const DOUBLE dMinValue, DOUBLE *pdValue)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	BOOL bTRUE = TRUE;

	dDummy = 0.0;
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
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
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

BOOL CPI9000App::HMINumericKeys(const CString &szPrompt, const DOUBLE dMaxValue, const DOUBLE dMinValue, LONG *plValue)
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	BOOL bTRUE = TRUE;

	dDummy = 0.0;
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
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
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

BOOL CPI9000App::HMILangSwitching()
{
	IPC_CServiceMessage svMsg;

	switch (m_ulLangageCode)
	{
	case 0:		// English
		m_ulLangageCode = 2;	// to Trad. Chinese
		break;

	case 1:		// Simplify Chinese
		m_ulLangageCode = 0;	// to English
		break;

	case 2:		// Trad. Chinese
		m_ulLangageCode = 1;	// to Simplify Chinese
		break;

	default:
		m_ulLangageCode = 2;	// to English
		break;
	}

	svMsg.InitMessage(sizeof(ULONG), &m_ulLangageCode);
	m_comClient.SendRequest("HmiUserService", "HmiLangSwitching", svMsg, 0);

	return TRUE;
}

//p20120918
BOOL CPI9000App::HMILangTraditionalChinese()
{
	try
	{
		IPC_CServiceMessage svMsg;
		if (m_ulLangageCode != 2)
		{
			m_ulLangageCode = 2;					// Change Language to Traditional Chinese
			svMsg.InitMessage(sizeof(ULONG), &m_ulLangageCode);
			m_comClient.SendRequest("HmiUserService", "HmiLangSwitching", svMsg, 0);
		}
	}
	catch (CAsmException e)	// 20140403 Yip
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
	return TRUE;
}
//p20120918:end


VOID CPI9000App::ProcessHMIRequest()
{
	try
	{
		// perform server request
		if (m_comServer.ScanRequest())
		{
			//if (g_bEnableLog) 
			//{
			//	m_comServer.ProcessRequest(-2);
			//}
			//else 
			//{
			m_comServer.ProcessRequest();
			//}
		}
	}
	catch (CAsmException e)
	{
		CString szMessage;
		e.What(szMessage);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		TRACE1("SFM_CModule::Run - %s\n", szMessage);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

}
////////////////////////////////////////////////////////////////////////////////////
//Close Operation
////////////////////////////////////////////////////////////////////////////////////
BOOL CPI9000App::CheckAutoBondSelected(AutoBondRequirment lMode)
{
	BOOL bError = FALSE;
	INT	i = 0;
	INT j = 0;
	CString szMsg = "";
	LONG	lAnswer = rMSG_TIMEOUT;

	BOOL bPBContinue = FALSE;	// 20140617 Yip
	BOOL bMBContinue = FALSE;	// 20140617 Yip

	switch (lMode)
	{
	case REWORK_MB1:
		for (i = 0; i < GetStationList().GetSize(); i++)
		{
			if (
				GetStationList().GetAt(i)->GetModSelectMode() == GLASS_MOD ||
				GetStationList().GetAt(i)->GetModSelectMode() == MB1_MOD
			   )
			{
				if (!GetStationList().GetAt(i)->IsModSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
			else
			{	
				if (GetStationList().GetAt(i)->IsModSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
		}
		
		// PR Module
		for (i = 0; i < GetPRStationList().GetSize(); i++)
		{
			
			{	
				if (GetPRStationList().GetAt(i)->IsPRSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
		}
		break;

	case REWORK_MB2:
		for (i = 0; i < GetStationList().GetSize(); i++)
		{
			if (
				GetStationList().GetAt(i)->GetModSelectMode() == GLASS_MOD ||
				GetStationList().GetAt(i)->GetModSelectMode() == MB2_MOD
			   )
			{
				if (!GetStationList().GetAt(i)->IsModSelected())
				{
					bError = TRUE;
				}
			}
			else
			{	
				if (GetStationList().GetAt(i)->IsModSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
		}

		// PR Module
		for (i = 0; i < GetPRStationList().GetSize(); i++)
		{
			
			
			{	
				if (GetPRStationList().GetAt(i)->IsPRSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
		}
		break;

	case REWORK_PB:
		for (i = 0; i < GetStationList().GetSize(); i++)
		{
			if (
				GetStationList().GetAt(i)->GetModSelectMode() == GLASS_MOD ||
				GetStationList().GetAt(i)->GetModSelectMode() == MB1_MOD ||
				GetStationList().GetAt(i)->GetModSelectMode() == MB2_MOD ||
				GetStationList().GetAt(i)->GetModSelectMode() == LSI_MOD
			   )
			{
				if (!GetStationList().GetAt(i)->IsModSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
			else
			{	
				if (GetStationList().GetAt(i)->IsModSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
		}

		// PR Module
		for (i = 0; i < GetPRStationList().GetSize(); i++)
		{
			
			if (
				GetPRStationList().GetAt(i)->GetPRModSelectMode() == GLASS_MOD ||
				GetPRStationList().GetAt(i)->GetPRModSelectMode() == MB1_MOD ||
				GetPRStationList().GetAt(i)->GetPRModSelectMode() == MB2_MOD ||
				GetPRStationList().GetAt(i)->GetPRModSelectMode() == LSI_MOD
			   )
			{
				if (!GetPRStationList().GetAt(i)->IsPRSelected())
				{
					bError = TRUE;
				}
			}
			else
			{	
				if (GetPRStationList().GetAt(i)->IsPRSelected())
				{
					CString str;
					str.Format("%s", GetStationList().GetAt(i)->GetStnName());
					bError = TRUE;
				}
			}
		}
		break;

	case NORMAL_BONDING:
		for (i = 0; i < GetStationList().GetSize(); i++)
		{
			if (!GetStationList().GetAt(i)->IsModSelected())
			{
				CString szStationName = GetStationList().GetAt(i)->GetStnName();
				if (szStationName == "TimeChart" )
				{
					LONG lAnswer = rMSG_CANCEL;
					BOOL bContinue = TRUE;
					
					if (bContinue)
					{
						continue;
					}
				}
				else if (
					szStationName == "RejectBelt" 
					)
				{

					continue;
				}
				
				bError = TRUE;
			}
		}

		// PR Module
		for (i = 0; i < GetPRStationList().GetSize(); i++)
		{
			if (!GetPRStationList().GetAt(i)->IsPRSelected())
			{
					bError = TRUE;
			}
		}
		break;
	}

	if (!bError)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



BOOL CPI9000App::CheckPackageStatus(AutoBondRequirment lMode)
{
	BOOL bCheckType1	= FALSE;
	BOOL bCheckType2	= FALSE;
	BOOL bError = FALSE;
	INT	i = 0;
	INT j = 0;
	CString szMsg = "";
	LONG	lAnswer = rMSG_TIMEOUT;


	// Check Setup Status
	for (i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		if (pCWH1->m_stInspWH[i].bEnable)
		{
			if (!pCWH1->m_stInspWH[i].bAlignPtSet)
			{
				SetError(IDS_INSPWH1_ALIGN_NOT_LOADED);
				bError = TRUE;
			}
			if (!pruDLPR1[i].bLoaded || !pruDLPR2[i].bLoaded)
			{
				SetError(IDS_INSPWH1_PR_NOT_LOADED);
				bError = TRUE;
			}
		}
	}
	for (i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		if (pCWH2->m_stInspWH[i].bEnable)
		{
			if (!pCWH2->m_stInspWH[i].bAlignPtSet)
			{
				SetError(IDS_INSPWH2_ALIGN_NOT_LOADED);
				bError = TRUE;
			}
			if (!pruDLPR1[i].bLoaded || !pruDLPR2[i].bLoaded)
			{
				SetError(IDS_INSPWH2_PR_NOT_LOADED);
				bError = TRUE;
			}
		}
	}

	if (lMode == REWORK_PB || lMode == NORMAL_BONDING) 
	{
		for (i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
		{
		}

	}

	if (!bError)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/////////////////////////////////////
//BOOL CPI9000App::CheckAutoBondSelected()
//{
//	return TRUE;
//}

//BOOL CPI9000App::CheckReworkPreBondSelected()
//{
//	return TRUE;
//}
//
//BOOL CPI9000App::CheckReworkMainBond1Selected()
//{
//	return TRUE;
//}
//
//BOOL CPI9000App::CheckReworkMainBond2Selected()
//{
//	return TRUE;
//}

BOOL CPI9000App::CheckGlassIndexingPath(CString StationName, GlassNum lGlassNum)
{

	BOOL bStatus = TRUE;
	CString strMsg;

	if (
		StationName == pCWH1->GetStnName() ||
		StationName == pCWH2->GetStnName() 
	   )
	{
	//InspWH
		if (!pCInPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort!", pCInPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bStatus = FALSE;
		}

		if (!pCOutPickArm->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort!", pCOutPickArm->GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bStatus = FALSE;
		}

		if (!pCWH1->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort!", pCWH1->GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bStatus = FALSE;
		}

		if (!pCWH2->m_bModSelected)
		{
			strMsg.Format("%s Module Not Selected. Operation Abort!", pCWH2->GetStnName());
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", strMsg);
			bStatus = FALSE;
		}

		if (pCInPickArm->IsGLASS1_VacSensorOn())
		{			
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Glass exists on INPICKARM already. Please remove glass and retry again.");
			bStatus = FALSE;
		}

		
		if (
			(lGlassNum == GLASS1 && (pCWH1->SetVacSol(GLASS1, ON, GMP_WAIT) != GMP_SUCCESS || pCWH1->IsGLASS1_VacSensorOn())) || 
			(lGlassNum == GLASS2 && (pCWH1->SetVacSol(GLASS2, ON, GMP_WAIT) != GMP_SUCCESS || pCWH1->IsGLASS2_VacSensorOn()))
		   )
		{			
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Glass exists on WH1 already. Please remove glass and retry again.");
			bStatus = FALSE;
		}
		else if (
			(lGlassNum == GLASS1 && (pCWH2->SetVacSol(GLASS1, ON, GMP_WAIT) != GMP_SUCCESS || pCWH2->IsGLASS1_VacSensorOn())) || 
			(lGlassNum == GLASS2 && (pCWH2->SetVacSol(GLASS2, ON, GMP_WAIT) != GMP_SUCCESS || pCWH2->IsGLASS2_VacSensorOn()))
		   )
		{			
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Glass exists on WH2 already. Please remove glass and retry again.");
			bStatus = FALSE;
		}
		else
		{
			if (lGlassNum == GLASS1)
			{
				pCWH1->SetVacSol(GLASS1, OFF, GMP_WAIT);
				pCWH2->SetVacSol(GLASS1, OFF, GMP_WAIT);
			}
			
			if (lGlassNum == GLASS2)
			{
				pCWH1->SetVacSol(GLASS2, OFF, GMP_WAIT);
				pCWH2->SetVacSol(GLASS2, OFF, GMP_WAIT);
			}
		}
	}

	return bStatus;
}

BOOL CPI9000App::CheckUnitToBondLmt()
{
	return TRUE;
}


BOOL CPI9000App::CheckToCleanLmt()
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//Close Operation
////////////////////////////////////////////////////////////////////////////////////
LONG CPI9000App::SrvCloseApp(IPC_CServiceMessage &svMsg)
{	
	SMotCommand		smCommand;
	
	POSITION		pos;
	SFM_CStation	*pStation = NULL;
	CAppStation	*pAppStation = NULL;
	CString			szKey = _T("");

	BOOL			bResult;
	//BOOL			bDelayedCloseApp = FALSE; //20121231
	//CAppStation*	pAppStationLitec = NULL;


	DisplayMessage("Closing application...");
	svMsg.GetMsg(sizeof(SMotCommand), &smCommand);

	m_lAction = glAMS_SYSTEM_EXIT;

	// 20140617 Yip: Power Off All Heaters When System Exit In Main App Directly Instead Of Through HMI Command

	// Signal all stations to perform Stop operation
	for (pos = m_pStationMap.GetStartPosition(); pos != NULL;)
	{
		m_pStationMap.GetNextAssoc(pos, szKey, pStation);

		if ((pAppStation = dynamic_cast<CAppStation*>(pStation)) != NULL)
		{
#if 1 //20121231
			TRACE0("SrvCloseApp... " + pAppStation->GetName() + "\n");
#endif
			pAppStation->SetAxisAction(0, m_lAction, 0);
			pAppStation->Command(glAMS_STOP_COMMAND);
			pAppStation->Result(GMP_SUCCESS);
		}
	}
	bResult = TRUE;

	m_qState = STOPPING_Q;

	svMsg.InitMessage(sizeof(BOOL), &bResult);

	return 1;
}

LONG CPI9000App::SrvRestartHMI(IPC_CServiceMessage &svMsg)
{
	CString szpassword = pCHouseKeeping->szEngPassword;

	if (HMIGetPassword("Please enter password", szpassword))
	{
		KillHmi();
	}

	return 0;
}

LONG CPI9000App::SetMachineID(IPC_CServiceMessage &svMsg)
{
	CString szpassword = pCHouseKeeping->szSvrPassword;

	if (HMIGetPassword("Please enter service password", szpassword))
	{
		if (HMIAlphaKeys("Please enter Machine ID", &m_szMachineID))
		{
			WriteProfileString(gszPROFILE_SETTING, gszMACHINE_ID, m_szMachineID);
		}
	}

	return 0;
}

LONG CPI9000App::HMI_GetAllStatus(IPC_CServiceMessage &svMsg)
{
	CString szInFilePath("D:\\sw\\PI9000\\LogFileDir\\");
	DeleteOldFiles(szInFilePath, CString("PI9000MsgLog*.mlg"), 7);	//20141208
	DeleteOldFiles(szInFilePath, CString("PI9000MsgLog*.txt"), 7);
	DeleteOldFiles(szInFilePath, CString("ErrorAlarm_*.csv"), 7);
	DeleteOldFiles(szInFilePath, CString("StationStatus*.log"), 7);
	
	szInFilePath = "D:\\sw\\PI9000\\Data\\Calculation Log\\";
	DeleteOldFiles(szInFilePath, CString("*.log"), 14);
	DeleteOldFiles(szInFilePath, CString("*.csv"), 14);
	GetAllStatus();


	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);

	return 1;
}

LONG CPI9000App::HMI_ResetStat(IPC_CServiceMessage &svMsg)
{
	m_ulTotalUnitsInpected = 0;
	m_ulTotalGlassPicked = 0;
	m_ulTotalGlassRejected = 0;
	m_ulTotalDLPRError	= 0;
	m_ulTotalInspOptPRError	= 0;
	m_ulTotalGlassTolError = 0;

	return 0;
}

LONG CPI9000App::HMI_SetAllModSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	for (int i = 0; i < GetStationList().GetSize(); i++)
	{
		if (GetStationList().GetAt(i)->GetStnName() != "TimeChart")
		{
			GetStationList().GetAt(i)->SetModSelected(bMode);
		}
	}

	for (int i = 0; i < GetPRStationList().GetSize(); i++)
	{
		GetPRStationList().GetAt(i)->SetPRSelected(bMode);
	}

	return 0;
}

LONG CPI9000App::HMI_CheckAllModSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	for (int i = 0; i < GetStationList().GetSize(); i++)
	{
		GetStationList().GetAt(i)->CheckModSelected(bMode);
	}

	return 0;
}

LONG CPI9000App::HMI_SetAllPRSelected(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	for (int i = 0; i < GetPRStationList().GetSize(); i++)
	{
		GetPRStationList().GetAt(i)->SetPRSelected(bMode);
	}

	return 0;
}

LONG CPI9000App::HMI_SetAllDebugSeq(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	for (int i = 0; i < GetStationList().GetSize(); i++)
	{
		GetStationList().GetAt(i)->HMI_bDebugSeq = bMode;
	}

	return 0;
}

LONG CPI9000App::HMI_SetAllLogMotion(IPC_CServiceMessage &svMsg) //20130422
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	for (int i = 0; i < GetStationList().GetSize(); i++)
	{
		GetStationList().GetAt(i)->HMI_bLogMotion = bMode;
	}

	return 0;
}

LONG CPI9000App::HMI_PickGlassOperation(IPC_CServiceMessage &svMsg)
{

	BOOL bOpDone;
	LONG lResponse;
	LONG lPlaceGlassStation;
	GlassNum lCurPickStatus			= MAX_NUM_OF_GLASS;
	LONG lCurGlass = GLASS1;


	lResponse = HMISelectionBox("PICK GLASS OPERATION", "Please select where to place glass to:", "WH1", "WH2");
	if (lResponse == -1 || lResponse == 11)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
		bOpDone = TRUE;
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}

	lPlaceGlassStation = lResponse;


	lResponse = HMISelectionBox("PICK GLASS OPERATION", "Pick Which Glass", "Both Glass", "Glass 1", "Glass 2");
	if (lResponse == -1 || lResponse == 11)
	{
		HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
		bOpDone = TRUE;
		svMsg.InitMessage(sizeof(BOOL), &bOpDone);
		return 1;
	}

	if (lResponse == 0)
	{
		lCurPickStatus = MAX_NUM_OF_GLASS;
	}
	else if (lResponse == 1)
	{
		lCurPickStatus = GLASS1;
	}
	else if (lResponse == 2)
	{
		lCurPickStatus = GLASS2;
	}


	switch (lPlaceGlassStation)
	{
	//WH1
	case 0:
		if (
			!CheckGlassIndexingPath(pCWH1->GetStnName(), lCurPickStatus) || 
			!CheckGlassIndexingPath(pCWH2->GetStnName(), lCurPickStatus) || 
			//pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
			pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS || //20121227
			pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS || //20121227
			!pCWH1->PickGlassOperation(lCurPickStatus) ||
			pCWH1->MoveXYToStandbyPosn(GMP_WAIT) != GMP_SUCCESS
		   )
		{
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
			bOpDone = TRUE;
			svMsg.InitMessage(sizeof(BOOL), &bOpDone);
			return 1;
		}
		break;
	//WH2
	case 1:
		if (
			!CheckGlassIndexingPath(pCWH1->GetStnName(), lCurPickStatus) || 
			!CheckGlassIndexingPath(pCWH2->GetStnName(), lCurPickStatus) || 
			//pCInspWH->MoveZToStandbyLevel(MAX_NUM_OF_GLASS, GMP_WAIT) != GMP_SUCCESS ||
			pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS || //20121227
			pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS || //20121227
			!pCWH2->PickGlassOperation(lCurPickStatus) ||
			pCWH2->MoveXYToStandbyPosn(GMP_WAIT) != GMP_SUCCESS
		   )
		{
			HMIMessageBox(MSG_OK, "PICK GLASS OPERATION", "Operation Abort!");
			bOpDone = TRUE;
			svMsg.InitMessage(sizeof(BOOL), &bOpDone);
			return 1;
		}
		break;
	}

	bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	
	return 1;
}

LONG CPI9000App::HMI_CleanOperation(IPC_CServiceMessage &svMsg)
{
	LONG lAnswer = 0;
	lAnswer = HMIMessageBox(MSG_OK_CANCEL, "Clean Operation", "Move modules to Clean Posn?");

	if (lAnswer == rMSG_OK)
	{
		pCInPickArm->m_stMotorX.IsPowerOn();
		pCOutPickArm->m_stMotorX.IsPowerOn();
		pCInPickArm->m_stMotorZ.IsPowerOn();
		pCOutPickArm->m_stMotorZ.IsPowerOn();
		pCWH1->m_stMotorY.IsPowerOn();
		pCWH2->m_stMotorY.IsPowerOn();
		pCWH1->m_stMotorT.IsPowerOn();
		pCWH2->m_stMotorT.IsPowerOn();
		pCInspOpt->m_stMotorX.IsPowerOn();
		pCInspOpt->m_stMotorZ.IsPowerOn();
		//pCInspWH->m_stMotorZ[WHZ_2].IsPowerOn();

		pCInspOpt->MoveZToPRLevel();
		pCInPickArm->MoveZ(INPICKARM_Z_CTRL_GO_STANDBY_LEVEL, SFM_NOWAIT);
		pCOutPickArm->MoveZ(OUTPICKARM_Z_CTRL_GO_STANDBY_LEVEL, SFM_NOWAIT);
		pCWH1->MoveTToStandby(GMP_NOWAIT);
		pCWH2->MoveTToStandby(GMP_NOWAIT);

		pCInspOpt->m_stMotorZ.Sync();
		pCInPickArm->m_stMotorZ.Sync();
		pCOutPickArm->m_stMotorZ.Sync();
		pCWH1->m_stMotorT.Sync();
		pCWH2->m_stMotorT.Sync();
		//pCInspWH->m_stMotorZ[WHZ_2].Sync();

		pCWH1->MoveXYToStandbyPosn(GMP_NOWAIT);
		pCWH2->MoveXYToStandbyPosn(GMP_NOWAIT);
		pCInspOpt->MoveXY(INSPOPT_XY_CTRL_GO_STANDBY_POSN, GMP_NOWAIT);
		pCInPickArm->MoveX(INPICKARM_X_CTRL_GO_STANDBY_POSN, SFM_NOWAIT);
		pCOutPickArm->MoveX(OUTPICKARM_X_CTRL_GO_STANDBY_POSN, SFM_NOWAIT);

		pCWH1->m_stMotorY.Sync();
		pCWH2->m_stMotorY.Sync();
		pCInspOpt->m_stMotorX.Sync();
		pCInPickArm->m_stMotorX.Sync();
		pCOutPickArm->m_stMotorX.Sync();
	}


	BOOL bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CPI9000App::HMI_FlushMode(IPC_CServiceMessage &svMsg)
{
	g_bFlushMode = TRUE;
	WriteHMIMess("Flushing in progress...");
	return 0;
}

#ifdef GATEWAY
LONG CPI9000App::HMI_CheckGatewayConnection(IPC_CServiceMessage &svMsgIn) //20130619
{
	IPC_CServiceMessage svMsg;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	const char szTxt[] = _T("Hello_COF902_How_Are_You");
	char *pBuffer = new char[sizeof(szTxt)];

	INT nIndex = 0;
	clock_t clkNow, clkInit;
	CString szMsg = _T(" ");

	clkInit = clock();
	HMI_lGatewayStatus = NETWORK_CONNECTING;
	memset(&pBuffer[0], 0, sizeof(szTxt));
	memcpy(&pBuffer[0], &szTxt[0], sizeof(szTxt));
	nIndex = sizeof(szTxt);

	DisplayMessage("Network CheckConnection via gateway socket and waiting for reply.");
	svMsg.InitMessage(nIndex, pBuffer);

	try
	{
		nConvID = m_comClient.SendRequest("HK_FX9000GW", "HK_CheckGatewayConnection", svMsg); //"HK_CheckCOF902GatewayConnection", svMsg);
	}
	catch (CAsmException e) //20140609
	{
		CString szMsg;
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		szMsg.Format("Func:%s", __FUNCTION__);
		DisplayMessage(szMsg);
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		szMsg.Format("Gateway Send Data Fail. Pls check gateway connection Setting.");
		DisplayMessage(szMsg);
		HMIMessageBox(MSG_OK, "Network Connection", szMsg);

		delete[] pBuffer;
		return 0;
	}
	while (1)
	{
		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
				clkNow = clock();
				HMI_lGatewayStatus = lReply;

				szMsg.Format("CheckConnection Reply=0x%x", lReply);
				DisplayMessage(szMsg);

				if (lReply == gnAMS_OK) //0
				{
					HMI_lGatewayStatus = NETWORK_GOOD;
				}
				else
				{
					HMI_lGatewayStatus = NETWORK_NIL;
				}
				
				szMsg.Format("COG902 Replay Time: %.4f sec", (DOUBLE)(clkNow - clkInit) / CLOCKS_PER_SEC);
				DisplayMessage(szMsg);
				HMIMessageBox(MSG_OK, "Network Connection", szMsg);
			}
			break;
		}
		else if (lTimeCount++ > 6000) //6 sec timeout limit
		{
			lReply		= glHMI_TIMEOUT;
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
			HMI_lGatewayStatus = NETWORK_NIL;
			DisplayMessage("CheckConnection timeout error");
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(1);
		}
	}

	delete[] pBuffer;

	return 0;
}

LONG CPI9000App::LoadGlassAction01Gateway(GlassNum lGlassNum)
{
	//1. vac on
	//2. INSPWH Z go load level
	//if (pCInspWH->MoveXYToLoadPosn(GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return GMP_FAIL;
	//}
	//if (pCInspWH->SetVacSol(lGlassNum, ON, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return GMP_FAIL;
	//}
	//if (pCInspWH->MoveZToLoadLevel(lGlassNum, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return GMP_FAIL;
	//}
	return GMP_SUCCESS;
}

LONG CPI9000App::LoadGlassAction02Gateway(GlassNum lGlassNum)
{
	//if (pCInspWH->MoveZToStandbyLevel(lGlassNum, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return GMP_FAIL;
	//}
	//if (pCInspWH->MoveXY(INSPWH_XY_CTRL_GO_STANDBY_POSN, GMP_WAIT) != GMP_SUCCESS)
	//{
	//	return GMP_FAIL;
	//}
	return GMP_SUCCESS;
}

LONG CPI9000App::GatewayCmdCOF902TA5GoPlaceGlassPosn(GlassNum lGlassNum)
{
//ask PI902 INSPWH-Z move to load Level and then turn ON WH-Vac.
	IPC_CServiceMessage svMsg;
	LONG lResult = gnAMS_OK;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	INT nIndex = 0;
	CString szMsg = _T(" ");
	const char szTxt1[] = _T("COF902_TA5_GoPlacePosnG1");
	const char szTxt2[] = _T("COF902_TA5_GoPlacePosnG2");
	const char szTxt3[] = _T("COF902_TA5_GoPlacePosnG12");
	char *pCmd = NULL;

	clock_t clkNow, clkInit;

	clkInit = clock();

	if (lGlassNum == GLASS1)
	{
		pCmd = (char*)&szTxt1[0];
		DisplayMessage("Network Ask COF902 TA5 Go PlaceGlass G1 Posn and waiting for reply.");
	}
	else if (lGlassNum == GLASS2)
	{
		pCmd = (char*)&szTxt2[0];
		DisplayMessage("Network Ask COF902 TA5 Go PlaceGlass G2 Posn and waiting for reply.");
	}
	else if (lGlassNum == MAX_NUM_OF_GLASS)
	{
		pCmd = (char*)&szTxt3[0];
		DisplayMessage("Network Ask COF902 TA5 Go PlaceGlass G1G2 Posn and waiting for reply.");
	}

	//nIndex = sizeof(pCmd);
	if (pCmd != NULL)
	{
		nIndex = strlen(pCmd) + 1;
		svMsg.InitMessage(nIndex, pCmd);
	}

	try
	{
		nConvID = m_comClient.SendRequest("HK_COF902GW", "HK_ProcessGatewayCommand", svMsg);
	}
	catch (CAsmException e)
	{
		CString szMsg;
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		szMsg.Format("Func:%s", __FUNCTION__);
		DisplayMessage(szMsg);
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		szMsg.Format("Gateway Send Data Fail. Pls check gateway connection Setting.");
		DisplayMessage(szMsg);
		HMIMessageBox(MSG_OK, "Network Load Glass", szMsg);

		return gnAMS_NOTOK;
	}

	while (1)
	{
		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
				clkNow = clock();

				szMsg.Format("TA5 Go Place Glass Posn Gateway Reply=0x%x", lReply);
				DisplayMessage(szMsg);

				if (lReply == gnAMS_OK) //0
				{
					HMI_lGatewayStatus = NETWORK_GOOD;
					lResult = gnAMS_OK;
				}
				else
				{
					HMI_lGatewayStatus = NETWORK_NIL;
					lResult = gnAMS_NOTOK;
				}
				
				szMsg.Format("COF902 TA5 Gp place Glass Replay Time: %.4f sec", (DOUBLE)(clkNow - clkInit) / CLOCKS_PER_SEC);
				DisplayMessage(szMsg);
				//HMIMessageBox(MSG_OK, "Network Connection", szMsg);
			}
			break;
		}
		else if (lTimeCount++ > 15 * 1000) //15 sec timeout limit
		{
			lReply		= glHMI_TIMEOUT;
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
			HMI_lGatewayStatus = NETWORK_NIL;
			DisplayMessage("Ask COF902 TA5 Go place position Connection timeout error");
			lResult = gnAMS_NOTOK;
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(1);
		}
	}

	return lResult;
}

LONG CPI9000App::GatewayCmdCOF902TA5VacOff(GlassNum lGlassNum)
{
//ask PI902 INSPWH-Z move to load Level and then turn ON WH-Vac.
	IPC_CServiceMessage svMsg;
	LONG lResult = gnAMS_OK;
	INT nConvID = 0;
	LONG lReply = 0;
	LONG lTimeCount = 0;
	INT nIndex = 0;
	CString szMsg = _T(" ");
	const char szTxt1[] = _T("COF902_TA5_VACOff");
	char *pCmd = NULL;

	clock_t clkNow, clkInit;

	clkInit = clock();

	pCmd = (char*)&szTxt1[0];
	DisplayMessage("Network Ask COF902 TA5 VAC Off and waiting for reply.");

	//nIndex = sizeof(pCmd);
	nIndex = strlen(pCmd) + 1;
	svMsg.InitMessage(nIndex, pCmd);

	try
	{
		nConvID = m_comClient.SendRequest("HK_COF902GW", "HK_ProcessGatewayCommand", svMsg);
	}
	catch (CAsmException e)
	{
		CString szMsg;
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		szMsg.Format("Func:%s", __FUNCTION__);
		DisplayMessage(szMsg);
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		szMsg.Format("Gateway Send Data Fail. Pls check gateway connection Setting.");
		DisplayMessage(szMsg);
		HMIMessageBox(MSG_OK, "Network Load Glass", szMsg);

		return gnAMS_NOTOK;
	}

	while (1)
	{
		if (m_comClient.ReadReplyForConvID(nConvID, svMsg, NULL, NULL, 0))
		{
			if (svMsg.GetMsgLen() == sizeof(LONG))
			{
				svMsg.GetMsg(sizeof(LONG), &lReply);
				clkNow = clock();

				szMsg.Format("TA5 VAC Off Gateway Reply=0x%x", lReply);
				DisplayMessage(szMsg);

				if (lReply == gnAMS_OK) //0
				{
					HMI_lGatewayStatus = NETWORK_GOOD;
					lResult = gnAMS_OK;
				}
				else
				{
					HMI_lGatewayStatus = NETWORK_NIL;
					lResult = gnAMS_NOTOK;
				}
				
				szMsg.Format("COF902 TA5 VAC Off Replay Time: %.4f sec", (DOUBLE)(clkNow - clkInit) / CLOCKS_PER_SEC);
				DisplayMessage(szMsg);
				//HMIMessageBox(MSG_OK, "Network Connection", szMsg);
			}
			break;
		}
		else if (lTimeCount++ > 15 * 1000) //15 sec timeout limit
		{
			lReply		= glHMI_TIMEOUT;
			pCHouseKeeping->SetError(IDS_HK_OPERATION_TIMEOUT);
			HMI_lGatewayStatus = NETWORK_NIL;
			DisplayMessage("Ask COF902 TA5 Vac Off. Connection timeout error");
			lResult = gnAMS_NOTOK;
			break;
		}
		else
		{
			while (m_comServer.ProcessRequest())
			{
				;
			}
			Sleep(1);
		}
	}

	return lResult;
}

#endif

INT CPI9000App::UnselectModuleForIndexingMode() //20130710
{
	pCInspOpt->SetPRSelected(FALSE); //20161004
	//pCInspWH->SetPRSelected(FALSE);
	pCWH1->SetPRSelected(FALSE);
	pCWH2->SetPRSelected(FALSE);
	return gnOK;
}

BOOL CPI9000App::ShowPRUploadErrorMessage(PRU *pPRU, CString &szErrorMsg) //20130713
{
	INT nFind = 0;
	CString szTitle = _T("");

	if (pPRU == NULL)
	{
		return FALSE;
	}
	if (CString(pPRU->stUploadRecordCmd.acFilename).GetLength() == 0)
	{
		return FALSE;
	}

	DisplayMessage(CString("PR Upload Record error at file:") + CString(pPRU->stUploadRecordCmd.acFilename) + CString(", Error:") + szErrorMsg);

	szTitle.Format("%s", pPRU->stUploadRecordCmd.acFilename);
	nFind = szTitle.ReverseFind('\\');
	if (nFind != -1)
	{
		szTitle = szTitle.Right(szTitle.GetLength() - nFind - 1);
	}
	HMIMessageBox(MSG_OK, szTitle, 
				  CString("File cannot be saved properly. Please revise and try again. err:") + szErrorMsg);
	
	return TRUE;
}

BOOL CPI9000App::ShowPRDownloadErrorMessage(PRU *pPRU, CString &szErrMsg) //20130713
{
	INT nFind = 0;
	CString szTitle = _T("");

	if (pPRU == NULL)
	{
		return FALSE;
	}

	DisplayMessage(CString("PR Download Record error at file:") + CString(pPRU->stDownloadRecordCmd.acFilename) + CString(" err:") + szErrMsg);
	szTitle.Format("%s", pPRU->stDownloadRecordCmd.acFilename);
	nFind = szTitle.ReverseFind('\\');
	if (nFind != -1)
	{
		szTitle = szTitle.Right(szTitle.GetLength() - nFind - 1);
	}
	HMIMessageBox(MSG_OK, szTitle, 
				  CString("File cannot be loaded properly. Please revise and try again. err:") + szErrMsg);

	return TRUE;
}

LONG CPI9000App::LoadSoftwareVersion()
{
	//Load Device File;
	CSingleLock slLock(&m_csDevice);
	slLock.Lock();

	CFileFind			fileFind;
	
	//Create Param Directory if not exist
	if (!fileFind.FindFile(m_szDevicePath))
	{
		if (!m_pInitOperation->CreateFileDirectory(m_szDevicePath))
		{
			AfxMessageBox("Cannot create Directory");
			slLock.Unlock();

			return FALSE;
		}
	}

	//Open Device File
	m_smfDevice.Close(); //Clear File buffer

	// File not find. Create a new one!
	if (!m_smfDevice.Open(m_szDevicePath + "\\" + m_szDeviceFile, FALSE, FALSE))
	{
		m_szDeviceFile = "CG144.prm";

		if (!m_smfDevice.Open(m_szDevicePath + "\\" + m_szDeviceFile, FALSE, TRUE))
		{
			AfxMessageBox("Cannot load File \"" + m_szDevicePath + "\\" + m_szDeviceFile + "\"");
			slLock.Unlock();
			return FALSE;
		}
	}

	//load the sw version
	pCHouseKeeping->m_dPrevSWVersion	= m_smfDevice["HouseKeeping"]["SW Version"];
	pCHouseKeeping->m_dCurSWVersion		= pCHouseKeeping->GetSoftwareVersion(gszSOFTWARE_VERSION);
	
	m_smfDevice.Update();
	slLock.Unlock();

	return 0;
}

LONG CPI9000App::SaveSoftwareVersion()
{
	//Load Device File;
	CSingleLock slLock(&m_csDevice);
	slLock.Lock();

	CFileFind			fileFind;
	
	//Create Param Directory if not exist
	if (!fileFind.FindFile(m_szDevicePath))
	{
		if (!m_pInitOperation->CreateFileDirectory(m_szDevicePath))
		{
			AfxMessageBox("Cannot create Directory");
			slLock.Unlock();

			return FALSE;
		}
	}

	//Open Device File
	m_smfDevice.Close(); //Clear File buffer

	// File not find. Create a new one!
	if (!m_smfDevice.Open(m_szDevicePath + "\\" + m_szDeviceFile, FALSE, FALSE))
	{
		if (!m_smfDevice.Open(m_szDevicePath + "\\" + m_szDeviceFile, FALSE, TRUE))
		{
			AfxMessageBox("Cannot load File \"" + m_szDevicePath + "\\" + m_szDeviceFile + "\"");
			slLock.Unlock();
			return FALSE;
		}
	}

	//load the sw version
	//m_smfDevice["HouseKeeping"]["SW Version"] = pCHouseKeeping->GetSoftwareVersion(gszSOFTWARE_VERSION);
	
	m_smfDevice.Update();
	slLock.Unlock();

	return 0;
}

///////////////////////////
// LOG
///////////////////////////
BOOL CPI9000App::MotionLog(const CString &szFunction, DOUBLE dVar1, DOUBLE dVar2, BOOL bWait)
{
	BOOL bResult = TRUE;

	CString szPrintData = _T("");
	CString szTempData = _T("");

	szPrintData.Format("%ld\t", GetTickCount());

	szPrintData += szFunction + _T("\t");

	szTempData.Format("%.2f\t", dVar1);

	szPrintData += szTempData;

	szTempData.Format("%.2f\t", dVar2);

	szPrintData += szTempData;

	if (bWait)
	{
		szTempData = "WAIT";
	}
	else
	{
		szTempData = "NO_WAIT";
	}

	szPrintData += szTempData;

	FILE *fp = fopen(_T("D:\\sw\\PI9000\\Data\\MotionLog.txt"), "a+");
	if (fp != NULL)
	{
		fprintf(fp, szPrintData + _T("\n"));
		fclose(fp);
	}

	return bResult;
}

// 20140729 Yip
VOID CPI9000App::LogParameter(const CString &szStation, const CString &szFunction, const CString &szParameter, const CString &szOldValue, const CString &szNewValue)
{
	m_csParameterLogLock.Lock();

	if (bEnableLogging && (szOldValue != szNewValue))
	{
		SYSTEMTIME ltime;
		GetLocalTime(&ltime);

		CString szLogDirectory = "D:\\sw\\PI9000\\Data\\ParameterLog";
		CreateDirectory(szLogDirectory.GetString(), NULL);

		CString szFileName;
		szFileName.Format("%s\\%s_%04u%02u%02u.log", szLogDirectory, m_szMachineID, ltime.wYear, ltime.wMonth, ltime.wDay);

		CFileFind fileFind;
		if (!fileFind.FindFile(szFileName))
		{
			CStdioFile File;
			if (File.Open(szFileName, CFile::modeCreate | CFile::modeWrite))
			{
				File.WriteString("Time\tStation\tFunction\tParameter\tOld Value\tNew Value\n");
				File.Close();
			}
		}

		CString szMsg;
		szMsg = szStation + "\t" + szFunction + "\t" + szParameter + "\t" + szOldValue + "\t" + szNewValue;
		WriteToFile(szFileName, szMsg);
	}

	m_csParameterLogLock.Unlock();
}

// 20140812 Yip
VOID CPI9000App::LogParameter(const CString &szFunction, const CString &szParameter, const CString &szOldValue, const CString &szNewValue)
{
	LogParameter("PI9000App", szFunction, szParameter, szOldValue, szNewValue);
}

LONG CPI9000App::HMI_SelectStation(IPC_CServiceMessage &svMsg)
{
	LONG	lMsgLen = svMsg.GetMsgLen();
	UCHAR	*cName;
	CString strCurrentStation;

	if (lMsgLen > 0)
	{
		cName = new UCHAR[lMsgLen];
		svMsg.GetMsg(cName, lMsgLen);
		strCurrentStation = (CString)cName;
	}
		
	CString szFileName;
	szFileName = "D:\\sw\\PI9000\\Param\\CurrentModuleMotorList.csv";

	if (strCurrentStation == "")
	{
		// Set data to file
		FILE *fp = fopen(szFileName, "wt");
		
		if (fp != NULL)
		{
			fclose(fp);
		}
	}

	// Print The Motor List Name:
	for (INT i = 0; i < GetStationList().GetSize(); i++)
	{
		if (
			GetStationList().GetAt(i)->GetStnName() == strCurrentStation && 
			GetStationList().GetAt(i)->GetmyMotorList().GetSize() != 0
		   )
		{
			
			// Set data to file
			FILE *fp = fopen(szFileName, "wt");

			for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)	
			{
				if (GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stAttrib.bEncoderExist)
				{
					if (fp != NULL)
					{
						fprintf(fp, "%s,\n", (const char*)GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetName());
					}
				}
			}

			if (fp != NULL)
			{
				fclose(fp);
			}
		}
	}
	
	// Reset Display Value
	getStationPicFilename(strCurrentStation, HMI_szMotorDirectionPng); //20120620
	//HMI_szMotorDirectionPng	= "D:\\sw\\PI9000\\Hmi\\images\\modules (png)\\" +  strCurrentStation + ".png";
	HMI_lMotorPosLmt		= 0;
	HMI_lMotorNegLmt		= 0;

	return 0;
}


VOID CPI9000App::getStationPicFilename(CString &szStationName, CString &szPicFileName) //20120620
{
	CString szTmpFileName;
	szTmpFileName	= szStationName;

	szPicFileName	= "D:\\sw\\PI9000\\Hmi\\images\\modules (png)\\" + szTmpFileName + ".png";
	return;
}
LONG CPI9000App::HMI_SelectStationMotor(IPC_CServiceMessage &svMsg)
{
	LONG	lMsgLen = svMsg.GetMsgLen();
	UCHAR	*cName;
	CString temp;
	CString	HMI_Name;
	
	HMI_bPickArmZ_Selected	= FALSE;
	HMI_bInspWHZ1_Selected = FALSE;
	HMI_bInspWHZ2_Selected = FALSE;


	if (lMsgLen > 0)
	{
		cName = new UCHAR[lMsgLen];
		svMsg.GetMsg(cName, lMsgLen);

		// Find the Motor from the actuator name
		for (INT i = 0; i < GetStationList().GetSize(); i++)
		{
			for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)	
			{
				if ((CString)cName == GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetName())
				{	
					m_pStMotorSWLimit = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j);
					m_CurrMotor = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetName();
					HMI_lMotorPosLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lPosLmt;
					HMI_lMotorNegLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lNegLmt;
					break;
				}
			}
		}
	}

	if (m_CurrMotor == "INSPWH_Z1")
	{
		HMI_bInspWHZ1_Selected = TRUE;
	}
	else if (m_CurrMotor == "INSPWH_Z2")
	{
		HMI_bInspWHZ2_Selected = TRUE;
	}
	else if (m_CurrMotor == "PICKARM_Z")
	{
		HMI_bPickArmZ_Selected = TRUE;
	}

	return 0;
}

LONG CPI9000App::HMI_SetMotorProtectPosLmt(IPC_CServiceMessage &svMsg)
{
	LONG	lMsgLen = svMsg.GetMsgLen();
	UCHAR	*cName;
	CString tempMotorName;

	if (lMsgLen > 0)
	{
		cName = new UCHAR[lMsgLen];
		svMsg.GetMsg(cName, lMsgLen);

		// Find the Motor from the actuator name
		for (INT i = 0; i < GetStationList().GetSize(); i++)
		{
			for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)	
			{
				if ((CString)cName == GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetName())
				{
#if 1 //20120620
					BOOL bResult = TRUE;
					if (GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lNegLmt >= GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetEncPosn())
					{
						LONG lAnswer = rMSG_TIMEOUT;

						lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Negative Limit is larger than Positive Limit. Continue?");

						if (lAnswer == rMSG_TIMEOUT || lAnswer == rMSG_CANCEL)
						{
							HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
							bResult = FALSE;
						}
					}
					if (bResult)
					{
						GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lPosLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetEncPosn();
						HMI_lMotorPosLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lPosLmt;
						GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->SetSoftwareLmt();
					}
#else
					//GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lPosLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetEncPosn();
					//HMI_lMotorPosLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lPosLmt;
					//GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->SetSoftwareLmt();
#endif
					return 0;

				}
			}
		}
	}

	HMIMessageBox(MSG_OK, "WARNING", "Please select module and motor first");
	return 0;
}

LONG CPI9000App::HMI_SetMotorProtectNegLmt(IPC_CServiceMessage &svMsg)
{
	// Print The Motor List Name:
	LONG	lMsgLen = svMsg.GetMsgLen();
	UCHAR	*cName;
	CString tempMotorName;

	if (lMsgLen > 0)
	{
		cName = new UCHAR[lMsgLen];
		svMsg.GetMsg(cName, lMsgLen);

		// Find the Motor from the actuator name
		for (INT i = 0; i < GetStationList().GetSize(); i++)
		{
			for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)	
			{
				if ((CString)cName == GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetName())
				{	
#if 1 //20120620
					BOOL bResult = TRUE;
					if (GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lPosLmt <= GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetEncPosn())
					{
						LONG lAnswer = rMSG_TIMEOUT;

						lAnswer = HMIMessageBox(MSG_CONTINUE_CANCEL, "WARNING", "Positive Limit is small than Negative Limit. Continue?");

						if (lAnswer == rMSG_TIMEOUT || lAnswer == rMSG_CANCEL)
						{
							HMIMessageBox(MSG_OK, "WARNING", "Operation Abort!");
							bResult = FALSE;
						}
					}
					if (bResult)
					{
						GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lNegLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetEncPosn();
						HMI_lMotorNegLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lNegLmt;
						GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->SetSoftwareLmt();
					}
#else
					GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lNegLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetEncPosn();
					HMI_lMotorNegLmt = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->stProtect.lNegLmt;
					GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->SetSoftwareLmt();
#endif
					return 0;
				}
			}
		}
	}
	

	HMIMessageBox(MSG_OK, "WARNING", "Please select module and motor first");
	
	return 0;

}

LONG CPI9000App::HMI_EnterSoftwareLimitPage(IPC_CServiceMessage &svMsg)
{
	CString szStation;
	m_pStMotorSWLimit = NULL;

	for (INT i = 0; i < GetStationList().GetSize(); i++)
	{
		szStation = GetStationList().GetAt(i)->GetStnName();
		if (szStation == "PreBondWH" || szStation == "InspWH")
		{
			for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)	
			{
				GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->ModifySWProtection(FALSE);
			}
		}
	}

	return 0;
}

LONG CPI9000App::HMI_LeaveSoftwareLimitPage(IPC_CServiceMessage &svMsg)
{
	CString szStation;
	m_pStMotorSWLimit = NULL;

	for (INT i = 0; i < GetStationList().GetSize(); i++)
	{
		szStation = GetStationList().GetAt(i)->GetStnName();
		if (szStation == "PreBondWH" || szStation == "InspWH")
		{
			for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)	
			{
				//if ((CString)cName == GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetName())
				{	
					GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->ModifySWProtection(TRUE);
				}
			}
		}
	}

	return 0;
}

VOID CPI9000App::SetDiagnSteps(ULONG ulSteps)
{
	switch (ulSteps)
	{
	case 0:
		m_lDiagnSteps = 10;
		break;
	case 1:
		m_lDiagnSteps = 100;
		break;
	case 2:
		m_lDiagnSteps = 1000;
		break;
	case 3:
		m_lDiagnSteps = 10000;
		break;
	case 4:
		m_lDiagnSteps = 100000;
		break;
	case 5:
		m_lDiagnSteps = 1000000;
		break;
	case 6:
		m_lDiagnSteps = 10000000;
		break;
	default:
		m_lDiagnSteps = 10000;
	}
}

LONG CPI9000App::HMI_SetDiagnSteps(IPC_CServiceMessage &svMsg)
{
	ULONG ulSteps;
	svMsg.GetMsg(sizeof(ULONG), &ulSteps);

	SetDiagnSteps(ulSteps);
	return 0;
}

LONG CPI9000App::HMI_SWLimitIndexMotorPos(IPC_CServiceMessage &svMsg)
{
	
	if (m_pStMotorSWLimit != NULL)
	{
		m_pStMotorSWLimit->MoveRelative(m_lDiagnSteps, GMP_WAIT);
	}
	else
	{
		HMIMessageBox(MSG_OK, "Motor Index Pos", "Please Select a Motor");
	}
	return 0;
}

LONG CPI9000App::HMI_SWLimitIndexMotorNeg(IPC_CServiceMessage &svMsg)
{
	if (m_pStMotorSWLimit != NULL)
	{
		m_pStMotorSWLimit->MoveRelative(-m_lDiagnSteps, GMP_WAIT);
	}
	else
	{
		HMIMessageBox(MSG_OK, "Motor Index Neg", "Please Select a Motor");
	}
	return 0;
}

LONG CPI9000App::HMI_DataLog(IPC_CServiceMessage &svMsg)
{
	BOOL bMode;
	svMsg.GetMsg(sizeof(BOOL), &bMode);

	if (bMode)
	{
		// 20140922 Yip: Add Datalog Mode And Stop Condition
		GMP_DATALOG_MODE enDatalogMode = GMP_CONTINUOUS;
		if (rMSG_YES == HMIMessageBox(MSG_YES_NO, "Start Datalog", "Stop datalog when first port not equal to 0?"))
		{
			enDatalogMode = GMP_STOP_EVT;
		}

		HMIMessageBox(MSG_OK, "DEVELOPER", "Start Datalog");
		StartDataLog(NULL, enDatalogMode);
	}
	else
	{
		StopDataLog("D:\\sw\\PI9000\\Data\\NuDataLog.txt");
	}
	return 0;
}


LONG CPI9000App::HMI_KeepQuiet(IPC_CServiceMessage &svMsg)
{
	INT nResult = GMP_SUCCESS;
	BOOL bOpDone = TRUE;

	pCInPickArm->KeepQuiet();
	pCOutPickArm->KeepQuiet();

	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}

LONG CPI9000App::HMI_ShowPerformanceReportPage(IPC_CServiceMessage &svMsg)	//Philip Add
{
	LONG ulPageNum = 0;
	svMsg.GetMsg(sizeof(LONG), &ulPageNum);
	g_ulDailyPerformanceReportPage = ulPageNum;
	return 1;
}

#ifdef EXCEL_MACHINE_DEVICE_INFO
VOID *CPI9000App::GetStnObjPointerByStnName(const CString &szStnName)
{
	//module inside GetStationList
	if (szStnName.GetLength() > 0)
	{
		if (szStnName == "InPickArm")
		{
			return pCInPickArm;
		}
		if (szStnName == "OutPickArm")
		{
			return pCOutPickArm;
		}
		else if (szStnName == "SettingFile")
		{
			return pCSettingFile;
		}
		else if (szStnName == "WH1")
		{
			return pCWH1;
		}
		else if (szStnName == "WH2")
		{
			return pCWH2;
		}
		else if (szStnName == "HouseKeeping")
		{
			return pCHouseKeeping;
		}
		else if (szStnName == "TimeChart")
		{
			return pCTimeChart;
		}
		else if (szStnName == "InspOpt")
		{
			return pCInspOpt;
		}
		else if (szStnName == "WinEagle")
		{
			return pCWinEagle;
		}
		else if (szStnName == "TAManager")
		{
			return pCTAManager;
		}
		else
		{
			return NULL;
		}
	}
	else
	{ 
	//error
		return NULL;
	}
}

LONG CPI9000App::HMI_PrintMachineDeviceInfoExcel(IPC_CServiceMessage &svMsg)
{
	BOOL bOpDone	= TRUE;
	CString szMsg, szDevice;
	const CString szExcelTemplateSrcPath = _T("D:\\sw\\PI9000\\Param\\"), szExcelTemplateDestPath = _T("D:\\sw\\PI9000\\Data\\"), szExcelTemplateFileName = _T("PI9000_Info_Template.xls");
	CString szTime = _T(""), szFileName = _T("");
	__time64_t ltime;

	_time64(&ltime);
	szTime = _ctime64(&ltime);
	szTime.TrimRight('\n');

	char tmpbuf[129];
	struct tm *today;
	today = _localtime64(&ltime);
	memset(&tmpbuf[0], 0 , 129);
	strftime(tmpbuf, 128, "%d_%b_%H_%M_%S", today);
	szFileName = szExcelTemplateDestPath + CString("PI9000_Info_") + CString(tmpbuf) + CString(".xls");

	CFileOperation fo;
	bOpDone = fo.Copy(szExcelTemplateSrcPath + szExcelTemplateFileName, szExcelTemplateDestPath);
	fo.Rename(szExcelTemplateDestPath + szExcelTemplateFileName, szFileName);

	BasicExcel excelDB((const char*)szFileName);

	szDevice = m_szDeviceFile;

	szDevice.TrimLeft("\\");
	g_lMachineInfoRowNum	= 0;
	g_lDeviceInfoRowNum		= 0;

	if (excelDB.Load(szFileName))
	{
		excelDB.New(2);
		excelDB.RenameWorksheet((size_t)0, (const char*)"PI9000_Machine_Info");		///< Rename an Excel worksheet at the given index to the given ANSI name. Index starts from 0. Returns true if successful, false if otherwise.
		excelDB.RenameWorksheet((size_t)1, (const char*)"PI9000_Device_Info");		///< Rename an Excel worksheet at the given index to the given ANSI name. Index starts from 0. Returns true if successful, false if otherwise.

		g_pExcelMachineInfoSheet = excelDB.GetWorksheet((size_t)0);
		g_pExcelDeviceInfoSheet = excelDB.GetWorksheet((size_t)1);

		if (g_pExcelMachineInfoSheet != NULL)
		{
			INT nCol = 0;
			g_pExcelMachineInfoSheet->Cell(g_lMachineInfoRowNum, nCol++)->SetString("PI9000 Machine Data List");
			//g_pExcelMachineInfoSheet->Cell(g_lMachineInfoRowNum, nCol++)->SetString(CString("Device:") + szDevice);
			g_pExcelMachineInfoSheet->Cell(g_lMachineInfoRowNum, nCol++)->SetString(CString("Machine Model: ") + m_szMachineModel);
			g_pExcelMachineInfoSheet->Cell(g_lMachineInfoRowNum, nCol++)->SetString(CString("Machine ID: ") + m_szMachineID);
			g_pExcelMachineInfoSheet->Cell(g_lMachineInfoRowNum, nCol++)->SetString(CString("Software Version: ") + m_szSoftVersion);
			g_pExcelMachineInfoSheet->Cell(g_lMachineInfoRowNum, nCol++)->SetString(CString("SW Release Date: ") + m_szReleaseDate);
			nCol = 10;
			g_pExcelMachineInfoSheet->Cell(g_lMachineInfoRowNum, nCol)->SetString(CString("Date:") + szTime);
			g_lMachineInfoRowNum++;

			nCol = 0;
			g_pExcelDeviceInfoSheet->Cell(g_lDeviceInfoRowNum, nCol++)->SetString("PI9000 Device Data List");
			g_pExcelDeviceInfoSheet->Cell(g_lDeviceInfoRowNum, nCol++)->SetString(CString("Device:") + szDevice);
			nCol = 10;
			g_pExcelMachineInfoSheet->Cell(g_lDeviceInfoRowNum, nCol)->SetString(CString("Date:") + szTime);
			g_lDeviceInfoRowNum++;


			//INT i= 0;
			for (INT i = 0; i < GetStationList().GetSize(); i++)
			{
				void *pStn = NULL;
				pStn = GetStnObjPointerByStnName(GetStationList().GetAt(i)->GetStnName());
				if (pStn != NULL)
				{
					((CPI9000Stn*)pStn)->printMachineDeviceInfo();
				}
#if 1
				for (int jj = 0; jj < GetPRStationList().GetSize(); jj++)
				{
					if (GetPRStationList().GetAt(jj)->m_szPRTaskStnName == GetStationList().GetAt(i)->GetStnName())
					{
						GetPRStationList().GetAt(jj)->printPRTaskStnMachinePara();
					}
				}
#endif
			}

			if (excelDB.Save())
			{
				DisplayMessage("excel save ok");
				szMsg.Format("Total number of Line:%ld", g_lMachineInfoRowNum);
				DisplayMessage(szMsg);
			}
			excelDB.Close();
			g_pExcelMachineInfoSheet	= NULL;
			g_pExcelDeviceInfoSheet		= NULL;
		}
	}

	bOpDone = TRUE;
	svMsg.InitMessage(sizeof(BOOL), &bOpDone);
	return 1;
}
#endif

//Philip add
LONG CPI9000App::HMI_SettingReportManagerSaveShiftSchedule(IPC_CServiceMessage &svMsg)
{
	CString HMISettingReportManagerMessage = "";
	CString szTmpStr = "";
	HMISettingReportManagerMessage.Format("%ld,\n", HMI_ulSettingReportManagerStartTime);
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift1 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift2 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift3 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift4 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift5 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift6 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift7 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift8 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift9 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift10 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift11 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift12 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift13 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift14 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift15 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift16 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift17 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift18 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift19 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift20 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift21 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift22 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift23 + ",\n";
	HMISettingReportManagerMessage += HMI_szSettingReportManagerShift24 + ",\n";

	if (SaveSettingReportConfig(HMISettingReportManagerMessage))
	{
		HMIMessageBox(MSG_OK, "Notice", "New Setting Saved");
	}
	else
	{
		HMIMessageBox(MSG_OK, "Notice", "Cannot Save Setting");
	}

	return 0;
}


BOOL CPI9000App::LoadSettingReportConfig()		//philip add
{
	CFileException ex;

	CString result = "";
	CString tmpStr = "";
	CFile reportSettingConfigFile;
	LONG ulTmp = 0;

	if (reportSettingConfigFile.Open("D:\\sw\\PI9000\\Release\\performance_report_config.csv", CFile::modeRead | CFile::shareDenyNone))
	{
		reportSettingConfigFile.SeekToBegin();
		CArchive reportSettingConfigAr(&reportSettingConfigFile, CArchive::load);
		int counter = 0;

		try
		{
			while (reportSettingConfigAr.ReadString(tmpStr))
			{
				tmpStr.Replace(",", "");
				if (counter == 0)
				{
					sscanf(tmpStr, "%ld", &ulTmp);
					HMI_ulSettingReportManagerStartTime = ulTmp;
				}
				else if (counter == 1)
				{
					HMI_szSettingReportManagerShift1.Format("%s", tmpStr);
				}
				else if (counter == 2)
				{
					HMI_szSettingReportManagerShift2.Format("%s", tmpStr);
				}
				else if (counter == 3)
				{
					HMI_szSettingReportManagerShift3.Format("%s", tmpStr);
				}
				else if (counter == 4)
				{
					HMI_szSettingReportManagerShift4.Format("%s", tmpStr);
				}
				else if (counter == 5)
				{
					HMI_szSettingReportManagerShift5.Format("%s", tmpStr);
				}
				else if (counter == 6)
				{
					HMI_szSettingReportManagerShift6.Format("%s", tmpStr);
				}
				else if (counter == 7)
				{
					HMI_szSettingReportManagerShift7.Format("%s", tmpStr);
				}
				else if (counter == 8)
				{
					HMI_szSettingReportManagerShift8.Format("%s", tmpStr);
				}
				else if (counter == 9)
				{
					HMI_szSettingReportManagerShift9.Format("%s", tmpStr);
				}
				else if (counter == 10)
				{
					HMI_szSettingReportManagerShift10.Format("%s", tmpStr);
				}
				else if (counter == 11)
				{
					HMI_szSettingReportManagerShift11.Format("%s", tmpStr);
				}
				else if (counter == 12)
				{
					HMI_szSettingReportManagerShift12.Format("%s", tmpStr);
				}
				else if (counter == 13)
				{
					HMI_szSettingReportManagerShift13.Format("%s", tmpStr);
				}
				else if (counter == 14)
				{
					HMI_szSettingReportManagerShift14.Format("%s", tmpStr);
				}
				else if (counter == 15)
				{
					HMI_szSettingReportManagerShift15.Format("%s", tmpStr);
				}
				else if (counter == 16)
				{
					HMI_szSettingReportManagerShift16.Format("%s", tmpStr);
				}
				else if (counter == 17)
				{
					HMI_szSettingReportManagerShift17.Format("%s", tmpStr);
				}
				else if (counter == 18)
				{
					HMI_szSettingReportManagerShift18.Format("%s", tmpStr);
				}
				else if (counter == 19)
				{
					HMI_szSettingReportManagerShift19.Format("%s", tmpStr);
				}
				else if (counter == 20)
				{
					HMI_szSettingReportManagerShift20.Format("%s", tmpStr);
				}
				else if (counter == 21)
				{
					HMI_szSettingReportManagerShift21.Format("%s", tmpStr);
				}
				else if (counter == 22)
				{
					HMI_szSettingReportManagerShift22.Format("%s", tmpStr);
				}
				else if (counter == 23)
				{
					HMI_szSettingReportManagerShift23.Format("%s", tmpStr);
				}
				else if (counter == 24)
				{
					HMI_szSettingReportManagerShift24.Format("%s", tmpStr);
				}
				counter++;
			}
		}
		catch (CAsmException e)
		{
			reportSettingConfigAr.Close();
			reportSettingConfigFile.Close();
			return false;
		}
		reportSettingConfigAr.Close();
		reportSettingConfigFile.Close();
		return true;
	}
	return false;
}

BOOL CPI9000App::SaveSettingReportConfig(CString inReportSettingConfig)		//philip add
{
	CFileException ex;
	CFile reportSettingConfigFile;

	if (reportSettingConfigFile.Open("D:\\sw\\PI9000\\Release\\performance_report_config.csv", CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &ex))
	{
		CArchive reportSettingConfigAr(&reportSettingConfigFile, CArchive::store);
			
		reportSettingConfigAr.WriteString(inReportSettingConfig);
		reportSettingConfigAr.Flush();

		reportSettingConfigAr.Close();
		reportSettingConfigFile.Close();
		return true;
	}
	return false;
}

VOID CPI9000App::setKlocworkTrue(BOOL bValue) //klocwork fix 20121211
{
	m_bKlocworkTrue = bValue;
	return;
}
BOOL CPI9000App::getKlocworkTrue()
{
	return m_bKlocworkTrue;
}
BOOL CPI9000App::getKlocworkFalse()
{
	return !m_bKlocworkTrue;
}

BOOL CPI9000App::CheckFileSize(CString &szInFilePath, CString &szInFileName, LONG lFileSizeLimitInKb)
{
	//Rename the file if the file size is larger than the lFileSizeLimit
	//The new name will append the date and time at the end of file name.
	BOOL bFileOp = FALSE;
	CFileOperation fo;

	FILE *fp = fopen(szInFilePath + szInFileName, "r");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		if (ftell(fp) > lFileSizeLimitInKb * 1024)
		{
			bFileOp = TRUE;
		}
		fclose(fp);
	}

	if (bFileOp)
	{
		INT nPos = 0;
		CString szFileName("FileName"), szExt("ext");
		nPos = szInFileName.Find(".");
		szFileName = szInFileName.Left(nPos);
		szExt = szInFileName.Right(szInFileName.GetLength() - nPos - 1);

		CString szTime = _T(""), szDestFilePath = _T("");
		char tmpbuf[129];
		__time64_t ltime;
		struct tm *today;

		_time64(&ltime);
		today = _localtime64(&ltime);
		memset(&tmpbuf[0], 0 , 129);
		strftime(tmpbuf, 128, "_%Y_%m_%d_%H_%M_%S.", today);
		szDestFilePath = szInFilePath + szFileName + CString(tmpbuf) + szExt;
		szTime = tmpbuf;

		//bOpDone = fo.Copy(szInFilePath + szInFileName, szInFilePath);
		if (fo.Rename(szInFilePath + szInFileName, szDestFilePath))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CPI9000App::CalcWHZLinearEncoder(CPI9000Stn *pStation, MOTOR_PROTECTION stProtect, WHZ_DATA *pResult) //whz rotary linear calib
{	

	CString szMsg = "";

	WHZ_DATA	stWhZ_temp[MAX_NUM_WHZ_CAL_STEP];
	INT			i, j, k;
	DOUBLE		WHZ_stepsize;
	LONG		LinearSample[NUM_OF_SAMPLE_DATA];
	LONG		RotarySample[NUM_OF_SAMPLE_DATA];	// WHZ motor
	LONG		SumEncoderData;

	memset(&LinearSample[0], 0, sizeof(LONG) * NUM_OF_SAMPLE_DATA);
	memset(&RotarySample[0], 0, sizeof(LONG) * NUM_OF_SAMPLE_DATA);
	//if (lSWLimitLevel[DOWN] < 500)
	//{
	//	return FALSE;
	//}

	if (
		m_CurrMotor == "INSPWH_Z1"
	   )
	{
		if (pStation->GetMotorZ(WHZ_1).MoveAbsolute(LONG(stProtect.lPosLmt + 500), GMP_WAIT) != GMP_SUCCESS)
		{
			return FALSE;
		}
	}
	else if (
			 m_CurrMotor == "INSPWH_Z2"
			)
	{
		if (pStation->GetMotorZ(WHZ_2).MoveAbsolute(LONG(stProtect.lPosLmt + 500), GMP_WAIT) != GMP_SUCCESS)
		{
			return FALSE;
		}
	}

	for (i = 0; i < MAX_NUM_WHZ_CAL_STEP; i++) // backup data q
	{
		stWhZ_temp[i] = pResult[i];
	}

	WHZ_stepsize = (DOUBLE)((stProtect.lNegLmt - 500) - (stProtect.lPosLmt + 500)) / (DOUBLE)(MAX_NUM_WHZ_CAL_STEP);
	if (
		m_CurrMotor == "INSPWH_Z1"	
	   )
	{
		pResult[0].RotaryPosn = pStation->GetMotorZ(WHZ_1).GetEncPosn();

	}
	else if (
			 m_CurrMotor == "INSPWH_Z2"
			)
	{
		pResult[0].RotaryPosn = pStation->GetMotorZ(WHZ_2).GetEncPosn();

	}
	pResult[0].LinearPosn = -1 * pCHouseKeeping->m_stMotorEncoderOnlyTmp1.GetEncPosn();

	for (i = 1; i < MAX_NUM_WHZ_CAL_STEP; i++) //get linear Encoder data
	{	
		if (
			m_CurrMotor == "INSPWH_Z1"
		   )
		{
			if (pStation->GetMotorZ(WHZ_1).MoveAbsolute(LONG(stProtect.lPosLmt + (WHZ_stepsize * i)), GMP_WAIT) == GMP_SUCCESS)
			{
				Sleep(300);
			}
			else // WHZ move error
			{
				for (k = 0; k < MAX_NUM_WHZ_CAL_STEP; k++) // restore data q
				{
					pResult[k] = stWhZ_temp[k];
				}

				return FALSE;
			}
		}
		else if (
				 m_CurrMotor == "INSPWH_Z2"
				)
		{
			if (pStation->GetMotorZ(WHZ_2).MoveAbsolute(LONG(stProtect.lPosLmt + (WHZ_stepsize * i)), GMP_WAIT) == GMP_SUCCESS)
			{
				Sleep(300);
			}
			else // WHZ move error
			{
				for (k = 0; k < MAX_NUM_WHZ_CAL_STEP; k++) // restore data q
				{
					pResult[k] = stWhZ_temp[k];
				}

				return FALSE;
			}
		}

		if (m_CurrMotor == "INSPWH_Z1" )
		{
			for (j = 0; j < NUM_OF_SAMPLE_DATA; j++) // sample data
			{
				RotarySample[j] = pStation->GetMotorZ(WHZ_1).GetEncPosn();
				LinearSample[j] = -1 * pCHouseKeeping->m_stMotorEncoderOnlyTmp1.GetEncPosn();
				Sleep(200);
			}
		}
		else if (
				 m_CurrMotor == "INSPWH_Z2"
				)
		{
			for (j = 0; j < NUM_OF_SAMPLE_DATA; j++) // sample data
			{
				RotarySample[j] = pStation->GetMotorZ(WHZ_2).GetEncPosn();
				LinearSample[j] = -1 * pCHouseKeeping->m_stMotorEncoderOnlyTmp1.GetEncPosn();
				Sleep(200);
			}
		}



		SumEncoderData = 0; //reset

		for (j = 0; j < NUM_OF_SAMPLE_DATA; j++)
		{
			SumEncoderData += RotarySample[j];
		}
		pResult[i].RotaryPosn = (LONG)(SumEncoderData / NUM_OF_SAMPLE_DATA);

		SumEncoderData = 0; //reset

		for (j = 0; j < NUM_OF_SAMPLE_DATA; j++)
		{
			SumEncoderData += LinearSample[j];
		}
		pResult[i].LinearPosn = (LONG)(SumEncoderData / NUM_OF_SAMPLE_DATA);		
	}

	return TRUE;
}


DWORD CPI9000App::DeleteOldFiles(CString &szInFilePath, CString &szWildCard, LONG lDay) //20141210
{
	//Note: szInFilePath + szWildCard should equal a full path eg D:\sw\COG902\LogFileDir\*.mlg
	//Delete all file pass the file filter and older that the lDay
	const ULONGLONG minutesPerTick = 600000000;
	CString szPath(" "), szMsg(" ");
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH] = "";
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	FILETIME ftWriteTime, ftLimitTime, ftSysTime;
	SYSTEMTIME sysTime;

	if (lDay < 0)
	{
		return -1;
	}

	szPath = szInFilePath + szWildCard;
	strcpy(szDir, szPath.GetString());
	DisplayMessage(szPath);

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		//DisplayMessage("FindFirstFile Error.");
		return -1;
	}

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//szMsg.Format("<DIR>:%s\n", ffd.cFileName);
			//DisplayMessage(szMsg);
			;
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			//szMsg.Format("file :%s, Size:%ld bytes\n", ffd.cFileName, filesize.QuadPart);
			//DisplayMessage(szMsg);

			ftWriteTime.dwHighDateTime = ffd.ftLastWriteTime.dwHighDateTime;
			ftWriteTime.dwLowDateTime = ffd.ftLastWriteTime.dwLowDateTime;
			FileTimeToSystemTime(&ftWriteTime, &sysTime);
			//szMsg.Format("WriteDateTime %d-%d-%d %d %02d:%02d:%02d\n", sysTime.wYear, sysTime.wMonth, sysTime.wDay,
			//	sysTime.wDayOfWeek, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			//DisplayMessage(szMsg);

			ULARGE_INTEGER uli1, uli2;
			GetSystemTimeAsFileTime(&ftSysTime);
			uli1.HighPart = ftSysTime.dwHighDateTime;
			uli1.LowPart = ftSysTime.dwLowDateTime;

			uli2.QuadPart = lDay * 24 * 60 * minutesPerTick;
			uli1.QuadPart -= uli2.QuadPart;

			// Convert back to FILETIME
			ftLimitTime.dwHighDateTime = uli1.HighPart;
			ftLimitTime.dwLowDateTime = uli1.LowPart;

			LONG lResult = 0;
			lResult = CompareFileTime(&ftLimitTime, &ftWriteTime);
			//szMsg.Format("Compare %ld\n", lResult);
			//DisplayMessage(szMsg);
			if (lResult == 1)
			{
				TCHAR pStrDeleteFile[MAX_PATH] = "";
				sprintf(pStrDeleteFile, "%s%s", (char*)szInFilePath.GetBuffer(0), ffd.cFileName);
				szMsg.Format("Delete:%s\n", pStrDeleteFile);
				DisplayMessage(szMsg);
				DeleteFile(pStrDeleteFile);
			}

		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayMessage("FindFirstFile error");
	}

	FindClose(hFind);
	return dwError;
}

//LONG CPI9000App::HMI_GetMaxSampleFreq(IPC_CServiceMessage &svMsg)
//{
//	// Print The Motor List Name:
//	CString tempMotorName;
//	DOUBLE	dMaxSampleFreq;
//	CString szFileName = "";
//	szFileName = "D:\\sw\\PI9000\\Data\\PI9000_GetMaxSampleFreq.txt";
//	// Set data to file
//	FILE *fp = fopen(szFileName, "a+");
//
//	if (fp != NULL)
//	{
//		__time64_t ltime;
//		CString szTime = "";
//
//		_time64(&ltime);
//		szTime = _ctime64(&ltime);
//		szTime.TrimRight('\n');
//
//		fseek(fp, 0, SEEK_END);
//		if (ftell(fp) < 10)
//		{
//			fprintf(fp, "Date: " + szTime + " Motor Channel \tMaxSampleFreq\n");
//		}
//		// Find the Motor from the actuator name
//		for (INT i = 0; i < GetStationList().GetSize(); i++)
//		{
//			for (INT j = 0; j < GetStationList().GetAt(i)->GetmyMotorList().GetSize(); j++)	
//			{
//				tempMotorName = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetName();
//				dMaxSampleFreq = GetStationList().GetAt(i)->GetmyMotorList().GetAt(j)->GetMaxSampleFreq();
//				fprintf(fp, "%s \t%s\t%lf\n",  (const char*)szTime, (const char*)tempMotorName, dMaxSampleFreq);	
//			}
//		}
//		fclose(fp);
//	}
//	return 0;
//
//}

INT CPI9000App::LogMemoryCpuStatus(LONG lLogIntervalInTime)
{
	HANDLE hProcessSnap = NULL;
	HANDLE hProcess = NULL;
	PROCESSENTRY32 pe32 = {0};
	INT nVisionNTCount	= 0;
	INT nAsmHmiCount	= 0;
	INT nGatewayCount	= 0;
	BOOL bResult = TRUE;
	CString szDirectory("D:\\sw\\PI9000\\Data\\");

	// Take a snapshot of all processes in the system. 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		//DeleteOldFiles(szDirectory, CString("MemoryUseageLog*.csv"), 14);

		// Fill in the size of the structure before using it. 
		pe32.dwSize = sizeof(PROCESSENTRY32);

		// Walk the snapshot of the processes, and for each process, display information.
		if (Process32First(hProcessSnap, &pe32))
		{
			do
			{
				CString strName = pe32.szExeFile;
				CString szPath;
				CString szFileNamePrefix(_T("MemoryUsageLog"));
				CString szFileNameSuffix(_T(".csv"));

				strName = strName.MakeUpper();
				szPath = szDirectory;
				if (
					strName.Find("VISIONNT.EXE") >= 0 ||
					strName.Find("ASMHMI.EXE") >= 0 ||
					strName.Find("PI9000.EXE") >= 0 ||
					strName.Find("SOCKETGATEWAY.EXE") >= 0
					)
				{
					PROCESS_MEMORY_COUNTERS stProcessMemoryCnt;

					INT nFind = strName.ReverseFind('.');
					szPath = szPath + szFileNamePrefix + strName.Left(nFind) + szFileNameSuffix;

					//if (strName == "PI9000.EXE")
					{
						hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

						if (hProcess != NULL)
						{
/////////////////////
							static FILETIME ftLastKernelTime[CPU_USAGE_MAX] =  {0,}, ftLastUserTIme[CPU_USAGE_MAX] =  {0,};
							ULONG ulIdx = 0;

							FILETIME ftCreateionTime = {0,}, ftExitTime =  {0,}, ftKernelTime =  {0,}, ftUserTIme =  {0,};
							DOUBLE dKernalCPUPercentage = 0.0, dUserCPUPercentage = 0.0, dTotalCPUPercentage = 0.0;

							//if (strName.Find("VisionProc.exe") >= 0)
							if (strName.Find("VISIONNT.EXE") >= 0)
							{
								ulIdx = CPU_VISIONNT;
							}
							else if (strName.Find("ASMHMI.EXE") >= 0)
							{
								ulIdx = CPU_ASMHMI;
							}
							else if (strName.Find("PI9000.EXE") >= 0)
							{
								ulIdx = CPU_PI9000;
							}
							else if (strName.Find("SOCKETGATEWAY.EXE") >= 0)
							{
								ulIdx = CPU_SOCKETGATEWAY;
							}

							bResult = GetProcessTimes(hProcess,           // handle to process
															&ftCreateionTime, // process creation time
															&ftExitTime,     // process exit time
															&ftKernelTime,   // process kernel-mode time
															&ftUserTIme );     // process user-mode time
							if (!bResult)
							{
								DisplayMessage("GetProcessTimes Error: " + strName);
							}
							else
							{
								DWORD64 l64KernelTime = 0, l64UserTime = 0;
								DWORD64 l64LastKernelTime = 0, l64LastUserTime = 0;
								DWORD64 l64KernelChange = 0, l64UserChange = 0;
								DWORD dwKernel = 0, dwUser = 0; //32bit unsigned
								
								l64KernelTime = (((DWORD64) ftKernelTime.dwHighDateTime) << 32) | ((DWORD64) ftKernelTime.dwLowDateTime);
								l64UserTime = (((DWORD64) ftUserTIme.dwHighDateTime) << 32) | ((DWORD64) ftUserTIme.dwLowDateTime);

								l64LastKernelTime = (((DWORD64) ftLastKernelTime[ulIdx].dwHighDateTime) << 32) | ((DWORD64) ftLastKernelTime[ulIdx].dwLowDateTime);
								l64LastUserTime = (((DWORD64) ftLastUserTIme[ulIdx].dwHighDateTime) << 32) | ((DWORD64) ftLastUserTIme[ulIdx].dwLowDateTime);

								l64KernelChange = l64KernelTime - l64LastKernelTime;
								l64UserChange = l64UserTime - l64LastUserTime;

								dwKernel = (DWORD) (l64KernelChange / (DWORD64)10000);
								dwUser = (DWORD) (l64UserChange / (DWORD64)10000);

								dKernalCPUPercentage = ((DOUBLE) dwKernel / (DOUBLE) lLogIntervalInTime) * 100.0;
								dUserCPUPercentage = ((DOUBLE) dwUser / (DOUBLE) lLogIntervalInTime) * 100.0;
								dTotalCPUPercentage = ((DOUBLE) (dwKernel + dwUser) / (DOUBLE) lLogIntervalInTime) * 100.0;
							}
							ftLastKernelTime[ulIdx] = ftKernelTime;
							ftLastUserTIme[ulIdx] = ftUserTIme;
////////////////
							bResult = GetProcessMemoryInfo(hProcess, &stProcessMemoryCnt, sizeof(stProcessMemoryCnt));
							if (bResult)
							{
								CString szInFileName2("");
								szInFileName2 = szFileNamePrefix + strName.Left(nFind) + szFileNameSuffix;
								CheckFileSize(szDirectory, szInFileName2, 512); //512KB

								FILE *fp = NULL;
								fp = fopen((char *)szPath.GetBuffer(0), "a+");
								if (fp != NULL)
								{
									CString szTime(" ");
									char tmpbuf[129];
									struct tm *today;
									__time64_t ltime;

									_time64(&ltime);
									today = _localtime64(&ltime);
									memset(&tmpbuf[0], 0 , 129);
									strftime(tmpbuf, 128, "%Y-%m-%d %H:%M:%S", today);
									szTime.Format("%s", tmpbuf);

									fseek(fp, 0, SEEK_END);
									if (ftell(fp) < 10)
									{//print header
										fprintf(fp, "Time, Title, PageFaultCount, PeakWorkingSetSize KB, WorkingSetSize KB, QuotaPeakPagedPoolUsage KB, QuotaPagedPoolUsage KB, QuotaPeakNonPagedPoolUsage KB, QuotaNonPagedPoolUsage KB, PagefileUsage KB, PeakPagefileUsage KB, CPU Kernal Usage Percentage, CPU User Usage Percentage, CPU Overall Usage Percentage\n");
									}
									fprintf(fp, "%s, %s, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %.3f, %.3f, %.3f\n",
										(char *)szTime.GetBuffer(0),
										(char *)strName.GetBuffer(0),
										//(LONG)stProcessMemoryCnt.cb, 
										(LONG)stProcessMemoryCnt.PageFaultCount,
										(LONG)stProcessMemoryCnt.PeakWorkingSetSize / 1024,
										(LONG)stProcessMemoryCnt.WorkingSetSize / 1024,
										(LONG)stProcessMemoryCnt.QuotaPeakPagedPoolUsage / 1024,
										(LONG)stProcessMemoryCnt.QuotaPagedPoolUsage / 1024,
										(LONG)stProcessMemoryCnt.QuotaPeakNonPagedPoolUsage / 1024,
										(LONG)stProcessMemoryCnt.QuotaNonPagedPoolUsage /1024,
										(LONG)stProcessMemoryCnt.PagefileUsage /1024,
										(LONG)stProcessMemoryCnt.PeakPagefileUsage / 1024,
										dKernalCPUPercentage,
										dUserCPUPercentage,
										dTotalCPUPercentage
										);
									fclose(fp);
								}
							}
							CloseHandle(hProcess);
						}
					}
				}
			} while (Process32Next(hProcessSnap, &pe32));
		}
	}
	CloseHandle(hProcessSnap);

	return 0;
/////
}

INT CPI9000App::LogSystemMemoryStatus()
{
	BOOL bResult = TRUE;
	CString szDirectory("D:\\sw\\PI9000\\Data\\");
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);
	//DeleteOldFiles(szDirectory, CString("MemoryUseageLog*.csv"), 14); //depend on LogMemoryStatus() to delete file

	if (GlobalMemoryStatusEx(&statex))
	{
		CString szPath;
		CString szInFileName2("MemoryUseageLogSystem.csv");
		FILE *fp = NULL;

		CheckFileSize(szDirectory, szInFileName2, 512); //512KB

		szPath = szDirectory + szInFileName2;
		fp = fopen((char *)szPath.GetBuffer(0), "a+");
		if (fp != NULL)
		{
			CString szTime(" ");
			char tmpbuf[129];
			struct tm *today;
			__time64_t ltime;

			_time64(&ltime);
			today = _localtime64(&ltime);
			memset(&tmpbuf[0], 0 , 129);
			strftime(tmpbuf, 128, "%Y-%m-%d %H:%M:%S", today);
			szTime.Format("%s", tmpbuf);

			fseek(fp, 0, SEEK_END);
			if (ftell(fp) < 10)
			{//print header
				fprintf(fp, "Time, Title, 	Percent_Memory_In_Use, Total_Physical_Memory_KB, Free_Physical_Memory_KB, Total_Paging_File_KB, Free_paging_File_KB, Total_Virtual_Memory_KB, Free_Virtual_Memory_KB, Free_Extended_Memory_KB\n");
			}
			fprintf(fp, "%s, System, %lu, %llu, %llu, %llu, %llu, %llu, %llu, %llu\n",
				(char *)szTime.GetBuffer(0),
				statex.dwMemoryLoad,					//1
				statex.ullTotalPhys / 1024,				//2
				statex.ullAvailPhys / 1024,				//3
				statex.ullTotalPageFile / 1024,
				statex.ullAvailPageFile / 1024,		//5
				statex.ullTotalVirtual / 1024,
				statex.ullAvailVirtual / 1024,
				statex.ullAvailExtendedVirtual / 1024 //8
				);
			fclose(fp);
			
/*
			printf ("%ld percent of memory is in use.\n",
				statex.dwMemoryLoad);
			printf ("There are %*I64d total %sbytes of physical memory.\n",
				WIDTH, statex.ullTotalPhys/DIV, divisor);
			printf ("There are %*I64d free %sbytes of physical memory.\n",
				WIDTH, statex.ullAvailPhys/DIV, divisor);
			printf ("There are %*I64d total %sbytes of paging file.\n",
				WIDTH, statex.ullTotalPageFile/DIV, divisor);
			printf ("There are %*I64d free %sbytes of paging file.\n",
				WIDTH, statex.ullAvailPageFile/DIV, divisor);
			printf ("There are %*I64x total %sbytes of virtual memory.\n",
				WIDTH, statex.ullTotalVirtual/DIV, divisor);
			printf ("There are %*I64x free %sbytes of virtual memory.\n",
				WIDTH, statex.ullAvailVirtual/DIV, divisor);

			// Show the amount of extended memory available.

			printf ("There are %*I64x free %sbytes of extended memory.\n",
				WIDTH, statex.ullAvailExtendedVirtual/DIV, divisor);
*/
		}
	}
	return 0;
}
