/////////////////////////////////////////////////////////////////
//	PR_Util.cpp : interface of the PR_Util
//
//	Description:
//		PI9000 Host Software
//
//	Date:		Tue Aug 1 2006
//	Revision:	0.25
//
//	By:			PI9000
//				AAA CSP Group
//
//	Copyright @ ASM Assembly Automation Ltd., 2006.
//	ALL rights reserved.
//
/////////////////////////////////////////////////////////////////

#pragma once

#include "prheader.h"


#include "PI9000Stn.h"

// PR Host System Channel Parameters
//
#define	DPR_PORT					(0)
#define	HOST_SYSTEM_ID				(0x40)
#define	VISION_SYSTEM_ID			(0xa0)
#define	NO_OF_INPUT_CHANNEL			(6)
#define	NO_OF_OUTPUT_CHANNEL		(6)
#define	TOTAL_NO_OF_CHANNEL			((NO_OF_INPUT_CHANNEL)+(NO_OF_OUTPUT_CHANNEL))


// User must following the Vision System Configuration List
// Sender IDs
//
#define	PRS_HOST_SID				(0x40)
#define	PRS_INSPUL_SID				(0x41)
#define	PRS_INSPDL_SID				(0x42)	// In Preciser 
#define	PRS_INSPDL2_SID				(0x43)	// In Preciser 
#define	PRS_MENU_SID				(0x41)	// Handles all the screen, video... functions //VPC

// PR 1 Receiver IDs
//
#define	PRS_HOST_RID				(0xA0)
#define	PRS_INSPUL_RID				(0xA1)
#define	PRS_INSPDL_RID				(0xA2)	// In Preciser 
#define	PRS_INSPDL2_RID				(0xA3)	// In Preciser 
#define	PRS_MENU_RID				(0xA1)	// Handles all the screen, video... functions //VPC

//
// Camera Numbers
//
#define	NO_CAM						(-1)
#define	INSPUL_CAM					(0)
#define	INSPDL_CAM					(1)
#define	INSPDL2_CAM					(2)
#define MAX_CAM						(INSPDL2_CAM)

// PR Size
//
#define	PRS_SMALL_SIZE				((PR_MAX_COORD - PR_MIN_COORD) / 32)
#define	PRS_MEDIUM_SIZE				((PR_MAX_COORD - PR_MIN_COORD) / 16)
#define	PRS_LARGE_SIZE				((PR_MAX_COORD - PR_MIN_COORD) / 8)
#define	PRS_XLARGE_SIZE				((PR_MAX_COORD - PR_MIN_COORD) / 2)

// PR Message Row IDs
//
#define	PRS_MSG_ROW1				(18)
#define	PRS_MSG_ROW2				(17)
#define	PRS_MSG_ROW3				(16)
#define	PRS_MSG_ROW4				(15)
// Common PR Algorithms
//
#define PRS_ALG_INTR				(PR_EDGE_POINTS_MATCHING_ON_DIE_PATTERN)
#define PRS_ALG_EDGE				(PR_EDGE_POINTS_MATCHING_ON_DIE_EDGES_WO_PATTERN_GUIDED)
#define PRS_ALG_L_EDGE				(PR_EDGE_POINTS_MATCHING_ON_LEFT_DIE_EDGE)
#define PRS_ALG_R_EDGE				(PR_EDGE_POINTS_MATCHING_ON_RIGHT_DIE_EDGE)

// Common PR Record Type
//
#define PRS_DIE_TYPE				(PR_RECORD_TYPE_DIE)
#define PRS_SHAPE_TYPE				(PR_RECORD_TYPE_SHAPE_DETECT)
#define PRS_TMPL_TYPE				(PR_RECORD_TYPE_TMPL)
#define PRS_GROUP_TYPE				(PR_RECORD_TYPE_GROUP)

// Fiducial Mark Type
#define PRS_FIDUCIAL_CROSS			(PR_FIDUCIAL_MARK_TYPE_CROSS)
#define PRS_FIDUCIAL_L				(PR_FIDUCIAL_MARK_TYPE_L)
#define PRS_FIDUCIAL_T				(PR_FIDUCIAL_MARK_TYPE_T)

#define PRS_FIDUCIAL_INNER			(PR_FMARK_CENTER_DEF_METHOD_INNER_EDGE)
#define PRS_FIDUCIAL_OUTER			(PR_FMARK_CENTER_DEF_METHOD_OUTER_EDGE)


#define	CAL_LEFT					(0)
#define	CAL_RIGHT					(1)
#define	CAL_UP						(2)
#define	CAL_DOWN					(3)
#define	NO_OF_CAL_PT				(4)

#define	PURPOSE0					(0)
#define	PURPOSE1					(1)
#define MAX_NUM_OF_PURPOSE			(2)


// Transform matrix used with PR to convert coordinates to motor units
//
typedef struct
{
	FLOAT a;
	FLOAT b;
	FLOAT c;
	FLOAT d;

} TRANSFORM;

typedef enum //follows the order of m_myPRUList.Add in the CWinEagle::PRS_InitPRUs()
{
	ID_NONE = 0, 

	ID_DL_GLASS1_PR1,
	ID_DL_GLASS1_PR2,
	ID_DL_GLASS2_PR1,
	ID_DL_GLASS2_PR2,
	
	ID_UL_COF_PR1,
	ID_UL_COF_PR2,
	ID_UL_GLASS_PR1,
	ID_UL_GLASS_PR2,

	//Calibration // Remarks for add Calib PRU, please add at the end
	ID_INSPDL1_WH1_CALIB_PRU,
	ID_INSPDL2_WH1_CALIB_PRU,
	ID_INSPDL1_WH2_CALIB_PRU,
	ID_INSPDL2_WH2_CALIB_PRU,
	ID_INSPWHUL_WH1_CALIB_PRU,
	ID_INSPWHUL_WH2_CALIB_PRU,

	ID_MAX_NUM,
} PRU_ID;

typedef enum
{
	PATTERN_MATCHING = 0, 
	SHAPE_FITTING, 
	EDGE_MATCHING, 
	TMPL_FITTING, 
	FIDUCIAL_FITTING, 
	RECTANGLE_MATCHING,	//Same as EDGE MATCHING
	SUBSTRACTION,

} PR_ALG;


typedef enum
{
	LOG_ALWAYS = 0,
	LOG_FAIL,
	LOG_ALIGN_FAIL,
	LOG_INSP_FAILE,
	LOG_MEASURE_FAILE,

} LOG_CONDITION;

typedef struct
{
	PR_COORD	prCoord;
	LONG		lSize;
	PR_COLOR	lColor;

} PR_CURSOR;

typedef enum{
    //20150213
	CMG_NONE =0,
    CMG_INSPUL_XINSPOPT_YWH1,							//INSPUL, X==INSPOPT, Y==WH1,
	CMG_INSPUL_XINSPOPT_YWH2,							//INSPUL, X==INSPOPT, Y==WH2
	CMG_INSPDL_XINPA_YWH1,								//INSPDL, X==INPA, Y==WH1
    CMG_INSPDL_XINPA_YWH2,								//INSPDL, X==INPA, Y==WH2,

} CAM_MOVEMENT_GROUP_NUM; //Cam and its movement combination each form a group for XY calibration (Theta is excluded)

typedef enum
{
	OBJECT = 0,
	BACKGROUND,
	MAX_NUM_OF_SURFACE,

} ACF_SUBSTRCATION;
typedef struct
{
	PRU_ID		emID;
	PR_OPTIC	stOptic;

	INT			nCamera;

	PR_CAMERA	emCameraNo;
	PR_PURPOSE	emPurpose[MAX_NUM_OF_PURPOSE];
	PR_UBYTE	ubSenderID;
	PR_UBYTE	ubReceiverID;

	BOOL		bCalibrated;
	BOOL		bCalibrated_2; //for mini PBWH X2Y2
	TRANSFORM	m2p_xform;
	TRANSFORM	m2p_xform_2; //for mini PBWH X2Y2
	TRANSFORM	p2m_xform;
	TRANSFORM	p2m_xform_2; //for mini PBWH X2Y2

	PR_UWORD	uwCoaxialLevel;
	PR_UWORD	uwCoaxial1Level;
	PR_UWORD	uwCoaxial2Level;
	PR_UWORD	uwRingLevel;
	PR_UWORD	uwRing1Level;
	PR_UWORD	uwSideLevel;

	PR_EXPOSURE_TIME	ulExposureTime;

	BOOL		bLoaded;

	LONG		lPBMethod;
	
	CString		szStnName;							// Display Only
	CString		szFileName;							// Save/Load Record;
	CString		szErrorMessage;						// 20160203 PR fail reason
	
	//PR_COORD	arcoLrnDieCorners[PR_NO_OF_CORNERS]; //learn reply corners
	PR_RCOORD	arcoLrnDieCorners[PR_NO_OF_CORNERS];	// from PR_COORD to PR_RCOORD for Vision UI
	PR_RCOORD	arcoDieCorners[PR_NO_OF_CORNERS];	//srch reply corners
	PR_RCOORD	rcoDieCentre;
	FLOAT		fAng;
	FLOAT		fScore;
	DOUBLE		dRefAngle;	// 20150804 for 120mm FOV
	FLOAT		fScale;		// 20151201
	PR_RSIZE	rszScale;		

	BOOL		bStatLoaded;
	FLOAT		fStatMean;
	FLOAT		fStatStdev;

	DOUBLE		dPostBdRejectTol;				// reject tolerance in %
	DOUBLE		dTotalDefectArea;				//postbond check
			
	PR_ULWORD				lStationID;		// Keep station and recorded ID.
	PR_ULWORD				lImageID;

	PR_RECORD_TYPE			emRecordType;

	PR_LRN_CMD				stLrnCmd;				// Vision UI
	PR_CREATE_CRITERIA_CMD	stCreateCritCmd;		// Vision UI
	PR_MODIFY_CRITERIA_CMD	stModifyCritCmd;		// Vision UI

	PR_INSP_CMD				stSrchCmd;				// Vision UI

	PR_MANUAL_LRN_DIE_CMD	stManualLrnDieCmd;
	PR_SRCH_DIE_CMD			stSrchDieCmd;

	PR_LRN_SHAPE_CMD		stLrnShapeCmd;
	PR_DETECT_SHAPE_CMD		stDetectShapeCmd;

	PR_LOAD_TMPL_CMD		stLoadTmplCmd;
	PR_SRCH_TMPL_CMD		stSrchTmplCmd;

	PR_IMAGE_STAT_INFO_CMD	stImageStatInfoCmd;
	PR_GET_RECORD_INFO_CMD  stGetRecordInfoCmd; //20180208

	//Subtration
	PR_GRAB_SHARE_IMAGE_CMD		stShareImgCmd;
	PR_GRAB_DISP_IMG_CMD		stDispImgCmd;

	PR_UPLOAD_RECORD_PROCESS_CMD	stUploadRecordCmd;
	PR_DOWNLOAD_RECORD_PROCESS_CMD	stDownloadRecordCmd;

	DOUBLE		dFOV; //20121102 field of view in mm
	BOOL		bUseFirstABCD; //for mini PBWH X2Y2

	CAM_MOVEMENT_GROUP_NUM eCamMoveGroupCurrent;	//current active CamMoveGroup
	CAM_MOVEMENT_GROUP_NUM eCamMoveGroup1;		//for first ABCD
	CAM_MOVEMENT_GROUP_NUM eCamMoveGroup2;		//for 2nd ABCD
	CAM_MOVEMENT_GROUP_NUM eCamMoveGroup3;		//for 3rd ABCD
	CAM_MOVEMENT_GROUP_NUM eCamMoveGroup4;		//for 4th ABCD

} PRU;

extern PRU	pruMenu;							// PRU for menu rotuines only

extern PRU	pruDLPR1[MAX_NUM_OF_GLASS];							// DL Glass PRU (LEFT)		
extern PRU	pruDLPR2[MAX_NUM_OF_GLASS];							// DL Glass PRU (RIGHT)		

extern PRU	pruULCOF1;							// DL COF PRU (LEFT)
extern PRU	pruULCOF2;							// DL COF PRU (RIGHT)
extern PRU	pruULGlass1;							// DL Glass PRU (LEFT)		
extern PRU	pruULGlass2;							// DL Glass PRU (RIGHT)


extern PRU	pruINSP_DL_Calib[MAX_NUM_OF_GLASS][MAX_NUM_OF_WH];										// INSPWH(DL) Calibration PRU
//extern PRU	pruINSP_UL_Calib;										// INSPOPT(UL) Calibration PRU
extern PRU	pruINSPWH_UL_Calib[MAX_NUM_OF_WH];										// INSPWH(DL) Calibration PRU

extern PRU	pruDummy;					// Dummy PRU

extern INT		nCamera;
extern INT		nCurrentCameraID; // to store the ncamera's emCameraNo
extern INT		nAutoModeCam;
extern BOOL		bPRUseMouse;
extern BOOL		bPRUpdateLight;
extern INT		nCurCalPt;

extern BOOL		bChannelInUse[NO_OF_INPUT_CHANNEL];
extern BOOL		bDirtyRow[PR_MAX_ROW_IN_TEXT_COORD + 1];

extern PR_CURSOR			g_stCursor;

VOID PRS_DisplayError(PRU *pPRU, PR_ERROR_TYPE emErrorType, PR_WORD uwErrorCode);

VOID		PRS_InitPRU(PRU *pPRU, LONG lCam, LONG emID, CString szStationName, CString szFileRecordName, LONG lCamMoveGroup1, LONG lCamMoveGroup2, LONG lCamMoveGroup3, LONG lCamMoveGroup4);

// Text Screen and Graphic Functions
//
VOID		PRS_SelectCameraChannel(INT const iCam, PR_UBYTE &ubSenderIDOutput, PR_UBYTE &ubReceiverIDOutput);
PRU			&PRS_SetCameraPRU_FOR_ZOOM(ULONG lCam);
PRU			&PRS_SetCameraPRU(ULONG lCam);

VOID		PRS_ClearScreen(INT const iCam);

VOID		PRS_MenuDisplayText(PR_WORD uwCol, PR_WORD uwRow, char const *Str, ...);
VOID		PRS_MenuClearTextRow(PR_WORD uwRow);

VOID		PRS_DisplayText(INT const iCam, PR_WORD uwCol, PR_WORD uwRow, char const *Str, ...);
VOID		PRS_ClearTextRow(INT const iCam, PR_WORD uwRow);
VOID		PRS_SetReverseMode(INT const iCam, PR_BOOLEAN bMode);
VOID		PRS_PrintPRError(INT const iCam, PR_WORD uwRow, PR_UWORD uwPRStatus);

VOID		PRS_DrawRect(INT const iCam, PR_COORD const coCorner1, PR_COORD const coCorner2, PR_COLOR const emColor);
VOID		PRS_EraseRect(INT const iCam, PR_COORD const coCorner1, PR_COORD const coCorner2);
VOID		PRS_DrawLine(INT const iCam, PR_COORD const coPoint1, PR_COORD const coPoint2, PR_COLOR const emColor);
VOID		PRS_EraseLine(INT const iCam, PR_COORD const coPoint1, PR_COORD const coPoint2);
// ** Add draw die using drawline
VOID		PRS_MenuDrawCross(PR_COORD const coCrossCentre, INT const iCrossSize, PR_COLOR const emColor);
VOID		PRS_DrawCross(INT const iCam, PR_COORD const coCrossCentre, INT const iCrossSize, PR_COLOR const emColor);
VOID		PRS_DrawCross(INT const iCam, PR_RCOORD const coCrossCentre, INT const iCrossSize, PR_COLOR const emColor);
VOID		PRS_EraseCross(INT const iCam, PR_COORD const coCrossCentre, INT const iCrossSize);
VOID		PRS_DrawHomeCursor(INT const iCam, BOOL bShowTmpl);
VOID		PRS_DrawSmallCursor(INT const iCam, PR_COORD const coPosition);
PR_COORD	PRS_ReadSmallCursor(INT const iCam);
VOID		PRS_DrawSrchArea(INT const iCam, PR_WIN const stWin);
VOID		PRS_DrawAndEraseCursor(INT const iCam, PR_COORD stUpperLeftCorner, PR_COORD stLowerRightCorner, BOOL bDraw);
PR_COORD	PRS_GetGraphicsStepSize();

// Mouse Control
VOID		PRS_UseMouseWindow(PRU *pPRU, PR_COORD stUpperLeftCornerPos, PR_COORD stLowerRightCornerPos);
VOID		PRS_GetMouseWindow(PRU *pPRU, PR_COORD *pstUpperLeftCornerPos, PR_COORD *pstLowerRightCornerPos);
VOID		PRS_CancelMouseWindow(PRU *pPRU);

PR_VVOID	PRS_MouseClick(PR_RFUNC_ON_MOUSE_CLICK_IN const *pstInPar, PR_RFUNC_ON_MOUSE_CLICK_OUT *pstOutPar);
PR_VVOID	PRS_MouseDrag(PR_RFUNC_ON_MOUSE_DRAG_IN const *pstInPar, PR_RFUNC_ON_MOUSE_DRAG_OUT *pstOutPar);
PR_VVOID	PRS_InitHookRFuncOnMouseClick();
PR_VVOID	PRS_InitHookRFuncOnMouseDrag();
PR_VVOID	PRS_SetOnMouseClickCmd(BOOL bMode);


// Video Functions
VOID		PRS_DisplayVideo(PRU *pPRU);
VOID		PRS_DisplayBinImg(INT const iCam, BOOL bMode, ULONG Threshold = 100);
VOID		PRS_AutoBondMode(BOOL bMode, BOOL bToggle);
VOID		PRS_DisableDigitalZoom(INT const iCam);
VOID		PRS_DigitalZoom(INT const iCam, PR_ZOOM_FACTOR const emZoomFactor, PR_COORD const coZoomRegionCentre);
VOID		PRS_GetPointFromZoomedImg(PR_ZOOM_FACTOR const emZoomCurrFactor, PR_COORD const coCurrCentreIn1xImg, PR_COORD const coExpectPtInCurrZoomImg, PR_COORD *pcoExpectPtIn1xImg);
VOID		PRS_GetPointInZoomImgFrom1xImg(PR_ZOOM_FACTOR const emExpectZoomFactor, PR_COORD const coExpectZoomCentre, PR_COORD const coExpectZoomPoint, PR_COORD *pcoResultantPointInZoomImg, PR_BOOLEAN *pemIsWithinRange);

// Optics Functions
VOID		PRS_GetOptic(PRU *pPRU);
VOID		PRS_GetCamLighting(PRU *pPRU);
PR_UWORD	PRS_GetLighting(PRU *pPRU, PR_SOURCE const emSource);
VOID		PRS_SetLighting(PRU *pPRU, PR_SOURCE const emSource, PR_UWORD const uwLevel, BOOL bIsSleep = FALSE);
INT			PRS_GetExposureTime(PRU *pPRU);
VOID		PRS_SetExposureTime(PRU *pPRU, INT iExposureTime);

VOID		PRS_EnableHWTrigLighting(INT const iCam, BOOL bMode);

// ** Add Load Optic later for showing learnt info

VOID		PRS_SetChannelInUse(PRU *pPRU, BOOL bInUse);
BOOL		PRS_GetChannelInUse(PRU *pPRU);

// New Learning and Srch Method with vision UI
//learn
VOID		PRS_InitLrnCmd(PRU *pPRU);
BOOL		PRS_ManualLrnCmd(PRU *pPRU);
BOOL		PRS_CreateCriteriaCmd(PRU *pPRU);
BOOL		PRS_ModifyCriteriaCmd(PRU *pPRU);
//search
BOOL		PRS_ManualSrchCmd(PRU *pPRU);
BOOL		PRS_ManualSrchGrabOnly(PRU *pPRU);
BOOL		PRS_ManualSrchProcessOnly(PRU *pPRU);

// Die Alignment Functions

VOID		PRS_InitManualLrnDieCmd(PRU *pPRU);
VOID		PRS_InitSrchDieCmd(PRU *pPRU);
BOOL		PRS_ManualLrnDie(PRU *pPRU);
BOOL		PRS_SrchDie(PRU *pPRU);
BOOL		PRS_SrchDieGrabOnly(PRU *pPRU);
BOOL		PRS_SrchDieProcessOnly(PRU *pPRU);

// Shape Detection Functions
VOID		PRS_InitLrnShapeCmd(PRU *pPRU);
VOID		PRS_InitDetectShapeCmd(PRU *pPRU);
BOOL		PRS_LrnShape(PRU *pPRU);
BOOL		PRS_DetectShape(PRU *pPRU);
BOOL		PRS_DetectShapeGrabOnly(PRU *pPRU);
BOOL		PRS_DetectShapeProcessOnly(PRU *pPRU);

// Template Matching Functions
VOID		PRS_InitLoadTmplCmd(PRU *pPRU);
VOID		PRS_InitSrchTmplCmd(PRU *pPRU);
BOOL		PRS_LoadTmpl(PRU *pPRU);
BOOL		PRS_SrchTmpl(PRU *pPRU);
BOOL		PRS_SrchTmplGrabOnly(PRU *pPRU);
BOOL		PRS_SrchTmplProcessOnly(PRU *pPRU);

VOID		PRS_InitImageStatInfoCmd(PRU *pPRU);
BOOL		PRS_LoadImageStatInfo(PRU *pPRU);
BOOL		PRS_GetImageStatInfo(PRU *pPRU);

//Substration
VOID		PRS_InitGrabShareImgCmd(PRU *pPRU); //learn Background
BOOL		PRS_GrabShareImgCmd(PRU *pPRU);
VOID		PRS_InitGrabDispImgCmd(PRU *pPRU);
BOOL		PRS_GrabDispImgCmd(PRU *pPRU); 



//BOOL		PRS_SrchDie(PRU* pPRU);
//BOOL		PRS_SrchDieGrabOnly(PRU* pPRU);
//BOOL		PRS_SrchDieProcessOnly(PRU* pPRU);

VOID		PR_InitGrabShareImgCmd(PRU *pPRU);

// General Record & Parameter Functions
BOOL		PRS_FreeRecordID(PRU *pPRU, PR_RECORD_TYPE emRecordType, PR_UWORD &uwRecordID);	// 20141024 Yip
BOOL		PRS_FreeRecordID(PRU *pPRU);
VOID		PRS_FreeAllRecords();
VOID		PRS_FreeAllRecordsAllHost();
BOOL		PRS_DispLrnInfo(PRU *pPRU);
VOID		PRS_InitGetRecrodInfoCmd(PRU *pPRU);
BOOL		PRS_GetRecordInfoWithStat(PRU *pPRU, PR_GET_RECORD_INFO_RPY *pstRpy);
BOOL		PRS_GetRecordInfo(PRU *pPRU, PR_GET_RECORD_INFO_RPY *pstRpy);
VOID		PRS_InitUploadRecordProcessCmd(PRU *pPRU);
VOID		PRS_InitDownloadRecordProcessCmd(PRU *pPRU);
BOOL		PRS_UploadRecordProcess(PRU *pPRU, CString *pszErrorMsg = NULL);
BOOL		PRS_DownloadRecordProcess(PRU *pPRU, BOOL bIsOverwriteRecord, LONG &lRetRecordID, CString *pszErrorMsg = NULL);

// Log Functions
VOID		PRS_SetDebugFlag(PR_DEBUG emDebugFlag);
VOID		PRS_SetComLogFlag(PR_COM_LOG emComLogFlag);
VOID		PRS_SetComLogTimeLimit(PR_ULWORD ulTimeLimit);
VOID		PRS_EnableLog();
VOID		PRS_DisableLog();

// Miscellaneous
VOID		PRS_GetHostInterfaceVersionNo(PR_UBYTE	*pubHostITFVersionNo);  //20120106 get version no of interface
BOOL		PRS_GetVersionNo(PR_GET_VERSION_NO_RPY *pstRpy);
VOID		PRS_SaveDispImg(PRU *pPRU, CString szPath);
BOOL		PRS_GrabDispImage(PRU *pPRU);
VOID		PRS_VisionNtDlg(PR_VISIONNT_DLG emDlg);
BOOL		PRS_ProcImgSimpleCmd(PRU *pPRU);

// Calibrations
//VOID		PRS_PRPosToMtrOffset(PRU *pPRU, PR_COORD const coPRPos, MTR_POSN *pMtrOffset);
VOID		PRS_PRPosToMtrOffset(PRU *pPRU, PR_RCOORD const coPRPos, MTR_POSN *pMtrOffset);
VOID		PRS_PRPosToDMtrOffset(PRU *pPRU, PR_RCOORD const coPRPos, D_MTR_POSN *pdMtrOffset);
VOID		PRS_PROffsetToDMtrOffset(PRU *pPRU, PR_COORD const coPROffset, D_MTR_POSN *pdMtrOffset); //20121105
VOID		PRS_MtrOffsetToPRPos(PRU *pPRU, MTR_POSN const coMtrOffset, PR_COORD *pPRPos);
VOID		PRS_MtrOffsetToPRPos(PRU *pPRU, D_MTR_POSN const coMtrOffset, PR_RCOORD *pPRPos);
VOID		PRS_DistanceToPROffset(PRU *pPRU, D_MTR_POSN const dDistance, PR_COORD *pPROffset); //20121102

//Copy Lighting Config
VOID PRS_CopyPurpose(PRU *pPRU);	
//All Display Fucntion
//Remove Background by host
VOID PRS_RemoveShareImg(PRU *pPRUBackground);

////////////////////////////////////////////////////////////////////
// Fucnction Support PR LOG same as the service menu on PR side
////////////////////////////////////////////////////////////////////
BOOL PRS_EnableLogCmd(PRU *pPRU, BOOL bMode, LOG_CONDITION LogCondition); // LogCondition 1: Log Always 2: Log Failure 3: Log Align Failure 4: Log Insp Failure 5: Log Measure Failure

BOOL PRS_CopyUserPurpose(PR_PURPOSE emUserPurpose, PR_PURPOSE emCamPurpose);	// For New PR Auto-Bond Mode Display
