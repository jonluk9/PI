/////////////////////////////////////////////////////////////////
//	Package_Util.cpp : interface of the Package_Util
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

#include "PI9000Stn.h"


typedef struct
{
	// Dimension Related (all in mm)
	DOUBLE			dLength;
	DOUBLE			dWidth;

	DOUBLE			dThickness1;
	DOUBLE			dThickness2;
	DOUBLE			dThickness3;
	DOUBLE			dThickness4;

	LONG			lSealantPosn;		// 0 - Left, 1 - Right, 2 - Bottom

	DOUBLE			dItoWidth;
	DOUBLE			dItoToDieEdge;
	DOUBLE			dWHOffset;		// Remove Me (May not necessary in New System Design)

	CString			szName;
} GLASS_INFO;

typedef struct
{
	BOOL			bEnable;
	BOOL			bValid;				// TRUE when PR learnt && alignment pt calibrated && PostBond learnt && PostBond pt set
	BOOL			bBonded;			// TRUE when the INSPWH's PR Finish

	D_MTR_POSN		dmtrAlignPt1;
	D_MTR_POSN		dmtrAlignPt2;
	BOOL			bAlignPtSet;		// TRUE when Alignment pts are set
}
INSPWH_INFO;
