/////////////////////////////////////////////////////////////////
//	Cal_Util.cpp : interface of the Cal_Util
//
//	Description:
//		
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

#include "stdafx.h"
#include "math.h"
#include "prheader.h"

#include "Cal_Util.h"

#include "FileOperations.h"

#include "PI9000.h"

#define NUM_OF_BACKUP 5

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LONG DoubleToLong(DOUBLE dInput)
{
	LONG lOutput;

	if (dInput < 0)
	{
		lOutput = (LONG)(dInput - 0.5);
	}
	else
	{
		lOutput = (LONG)(dInput + 0.5);
	}

	return lOutput;
}

DOUBLE SetDecimalPlace(DOUBLE dInput)
{
	LONG	lTempOutput;
	LONG	lTest;
	DOUBLE dOutput;

	dInput *= 1000.0;
	lTempOutput = (LONG)dInput;
	lTest = lTempOutput;
	dOutput = (DOUBLE)lTempOutput / 1000.0;


	return dOutput;
}

BOOL FindManualCalibMatrix(PRU *pPRU, MTR_POSN MtrPts[], PR_COORD PRPts[])
{

	if (pPRU->eCamMoveGroup1 == pPRU->eCamMoveGroupCurrent)
	{
		// calculate p2m transform (0 = Left, 1 = Right, 2 = Up, 3 = Down)
		pPRU->p2m_xform.a = -(FLOAT)(MtrPts[1].x - MtrPts[0].x) / (FLOAT)(PRPts[1].x - PRPts[0].x);
		pPRU->p2m_xform.b = -(FLOAT)(MtrPts[3].x - MtrPts[2].x) / (FLOAT)(PRPts[3].y - PRPts[2].y);
		pPRU->p2m_xform.c = -(FLOAT)(MtrPts[1].y - MtrPts[0].y) / (FLOAT)(PRPts[1].x - PRPts[0].x);
		pPRU->p2m_xform.d = -(FLOAT)(MtrPts[3].y - MtrPts[2].y) / (FLOAT)(PRPts[3].y - PRPts[2].y);

		// calculate m2p transform
		FLOAT fDet;
		fDet = (FLOAT)((-1.0) * (pPRU->p2m_xform.b) * (pPRU->p2m_xform.c) + (pPRU->p2m_xform.a) * (pPRU->p2m_xform.d));

		if (fDet == 0.0)
		{
			pPRU->bCalibrated = FALSE;
			return FALSE;
		}

		pPRU->m2p_xform.a = (FLOAT)pPRU->p2m_xform.d / fDet;
		pPRU->m2p_xform.b = -(FLOAT)pPRU->p2m_xform.b / fDet;
		pPRU->m2p_xform.c = -(FLOAT)pPRU->p2m_xform.c / fDet;
		pPRU->m2p_xform.d = (FLOAT)pPRU->p2m_xform.a / fDet;

		pPRU->bCalibrated = TRUE;
	}
	else
	{
		// calculate p2m transform (0 = Left, 1 = Right, 2 = Up, 3 = Down)
		pPRU->p2m_xform_2.a = -(FLOAT)(MtrPts[1].x - MtrPts[0].x) / (FLOAT)(PRPts[1].x - PRPts[0].x);
		pPRU->p2m_xform_2.b = -(FLOAT)(MtrPts[3].x - MtrPts[2].x) / (FLOAT)(PRPts[3].y - PRPts[2].y);
		pPRU->p2m_xform_2.c = -(FLOAT)(MtrPts[1].y - MtrPts[0].y) / (FLOAT)(PRPts[1].x - PRPts[0].x);
		pPRU->p2m_xform_2.d = -(FLOAT)(MtrPts[3].y - MtrPts[2].y) / (FLOAT)(PRPts[3].y - PRPts[2].y);

		// calculate m2p transform
		FLOAT fDet;
		fDet = (FLOAT)((-1.0) * (pPRU->p2m_xform_2.b) * (pPRU->p2m_xform_2.c) + (pPRU->p2m_xform_2.a) * (pPRU->p2m_xform_2.d));

		if (fDet == 0.0)
		{
			pPRU->bCalibrated_2 = FALSE;
			return FALSE;
		}

		pPRU->m2p_xform_2.a = (FLOAT)pPRU->p2m_xform_2.d / fDet;
		pPRU->m2p_xform_2.b = -(FLOAT)pPRU->p2m_xform_2.b / fDet;
		pPRU->m2p_xform_2.c = -(FLOAT)pPRU->p2m_xform_2.c / fDet;
		pPRU->m2p_xform_2.d = (FLOAT)pPRU->p2m_xform_2.a / fDet;

		pPRU->bCalibrated_2 = TRUE;
	}
	return TRUE;
}

BOOL FindAutoCalibMatrix(PRU *pPRU, MTR_POSN MtrPts[], PR_RCOORD PRPts[])
{
	if (pPRU->eCamMoveGroup1 == pPRU->eCamMoveGroupCurrent)
	{
		// calculate p2m transform (0 = Left, 1 = Right, 2 = Up, 3 = Down)
		pPRU->m2p_xform.a = -(FLOAT)(PRPts[1].x - PRPts[0].x) / (FLOAT)(MtrPts[1].x - MtrPts[0].x);
		pPRU->m2p_xform.b = -(FLOAT)(PRPts[3].x - PRPts[2].x) / (FLOAT)(MtrPts[3].y - MtrPts[2].y);
		pPRU->m2p_xform.c = -(FLOAT)(PRPts[1].y - PRPts[0].y) / (FLOAT)(MtrPts[1].x - MtrPts[0].x);
		pPRU->m2p_xform.d = -(FLOAT)(PRPts[3].y - PRPts[2].y) / (FLOAT)(MtrPts[3].y - MtrPts[2].y);

		// calculate m2p transform
		FLOAT fDet;
		fDet = (FLOAT)((-1.0) * (pPRU->m2p_xform.b) * (pPRU->m2p_xform.c) + (pPRU->m2p_xform.a) * (pPRU->m2p_xform.d));

		if (fDet == 0.0)
		{
			pPRU->bCalibrated = FALSE;
			return FALSE;
		}

		pPRU->p2m_xform.a = (FLOAT)pPRU->m2p_xform.d / fDet;
		pPRU->p2m_xform.b = -(FLOAT)pPRU->m2p_xform.b / fDet;
		pPRU->p2m_xform.c = -(FLOAT)pPRU->m2p_xform.c / fDet;
		pPRU->p2m_xform.d = (FLOAT)pPRU->m2p_xform.a / fDet;

		pPRU->bCalibrated = TRUE;
	}
	else
	{
		//for PBWH X2Y2 (X1Y1 use first set of data)
		// calculate p2m transform (0 = Left, 1 = Right, 2 = Up, 3 = Down)
		pPRU->m2p_xform_2.a = -(FLOAT)(PRPts[1].x - PRPts[0].x) / (FLOAT)(MtrPts[1].x - MtrPts[0].x);
		pPRU->m2p_xform_2.b = -(FLOAT)(PRPts[3].x - PRPts[2].x) / (FLOAT)(MtrPts[3].y - MtrPts[2].y);
		pPRU->m2p_xform_2.c = -(FLOAT)(PRPts[1].y - PRPts[0].y) / (FLOAT)(MtrPts[1].x - MtrPts[0].x);
		pPRU->m2p_xform_2.d = -(FLOAT)(PRPts[3].y - PRPts[2].y) / (FLOAT)(MtrPts[3].y - MtrPts[2].y);

		// calculate m2p transform
		FLOAT fDet;
		fDet = (FLOAT)((-1.0) * (pPRU->m2p_xform_2.b) * (pPRU->m2p_xform_2.c) + (pPRU->m2p_xform_2.a) * (pPRU->m2p_xform_2.d));

		if (fDet == 0.0)
		{
			pPRU->bCalibrated_2 = FALSE;
			return FALSE;
		}

		pPRU->p2m_xform_2.a = (FLOAT)pPRU->m2p_xform_2.d / fDet;
		pPRU->p2m_xform_2.b = -(FLOAT)pPRU->m2p_xform_2.b / fDet;
		pPRU->p2m_xform_2.c = -(FLOAT)pPRU->m2p_xform_2.c / fDet;
		pPRU->p2m_xform_2.d = (FLOAT)pPRU->m2p_xform_2.a / fDet;

		pPRU->bCalibrated_2 = TRUE;
	}
	return TRUE;
}

BOOL FindCORCentre(INT nSample, MTR_POSN MtrPts[], D_MTR_POSN *p_dmtrCORPosn)
{
	// calculate the COR with modified least squares methods
	DOUBLE a, b, c, d, e;

	DOUBLE dTmp1, dTmp2, dTmp3, dTmp4, dTmp5;
	INT i;

	// calculate a
	dTmp1 = 0.0;
	dTmp2 = 0.0;

	for (i = 0; i < nSample; i++)
	{
		dTmp1 += (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].x;
		dTmp2 += (DOUBLE) MtrPts[i].x;
	}

	a = (nSample * dTmp1) - (dTmp2 * dTmp2);

	// calculate b
	dTmp1 = 0.0;
	dTmp2 = 0.0;
	dTmp3 = 0.0;

	for (i = 0; i < nSample; i++)
	{
		dTmp1 += (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].y;
		dTmp2 += (DOUBLE) MtrPts[i].x;
		dTmp3 += (DOUBLE) MtrPts[i].y;
	}

	b = (nSample * dTmp1) - (dTmp2 * dTmp3);

	// calculate c
	dTmp1 = 0.0;
	dTmp2 = 0.0;

	for (i = 0; i < nSample; i++)
	{
		dTmp1 += (DOUBLE) MtrPts[i].y * (DOUBLE) MtrPts[i].y;
		dTmp2 += (DOUBLE) MtrPts[i].y;
	}

	c = (nSample * dTmp1) - (dTmp2 * dTmp2);

	// calculate d
	dTmp1 = 0.0;
	dTmp2 = 0.0;
	dTmp3 = 0.0;
	dTmp4 = 0.0;
	dTmp5 = 0.0;

	for (i = 0; i < nSample; i++)
	{
		dTmp1 += (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].y * (DOUBLE) MtrPts[i].y;
		dTmp2 += (DOUBLE) MtrPts[i].x;
		dTmp3 += (DOUBLE) MtrPts[i].y * (DOUBLE) MtrPts[i].y;
		dTmp4 += (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].x;
		dTmp5 += (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].x;
	}

	d = ((nSample * dTmp1) - (dTmp2 * dTmp3) + (nSample * dTmp4) - (dTmp2 * dTmp5)) / 2.0;

	// calculate e
	dTmp1 = 0.0;
	dTmp2 = 0.0;
	dTmp3 = 0.0;
	dTmp4 = 0.0;
	dTmp5 = 0.0;

	for (i = 0; i < nSample; i++)
	{
		dTmp1 += (DOUBLE) MtrPts[i].y * (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].x;
		dTmp2 += (DOUBLE) MtrPts[i].y;
		dTmp3 += (DOUBLE) MtrPts[i].x * (DOUBLE) MtrPts[i].x;
		dTmp4 += (DOUBLE) MtrPts[i].y * (DOUBLE) MtrPts[i].y * (DOUBLE) MtrPts[i].y;
		dTmp5 += (DOUBLE) MtrPts[i].y * (DOUBLE) MtrPts[i].y;
	}

	e = ((nSample * dTmp1) - (dTmp2 * dTmp3) + (nSample * dTmp4) - (dTmp2 * dTmp5)) / 2.0;

	// calculate the centre point
	DOUBLE dDen;
	dDen = (a * c) - (b * b);

	if (dDen == 0.0)
	{
		p_dmtrCORPosn->x = 0.0;
		p_dmtrCORPosn->y = 0.0;

		return FALSE;
	}

	p_dmtrCORPosn->x = ((d * c) - (b * e)) / dDen;
	p_dmtrCORPosn->y = ((a * e) - (b * d)) / dDen;

	return TRUE;
}

BOOL RefineCORCentre(MTR_POSN mtrCurPosn, DOUBLE dAngle, D_MTR_POSN dmtrResultPosn, D_MTR_POSN *p_dmtrCORPosn)
{
	DOUBLE dAngleRad;

	dAngleRad = (PI / 180.0) * dAngle;

	if (dAngleRad == 0.0)
	{
		return FALSE;
	}

	DOUBLE dDeterminant;

	dDeterminant = 1.0 / ((cos(dAngleRad) - 1.0) * (cos(dAngleRad) - 1.0) + sin(dAngleRad) * sin(dAngleRad));

	p_dmtrCORPosn->x = dDeterminant * (dmtrResultPosn.x * (1.0 - cos(dAngleRad)) - dmtrResultPosn.y * sin(dAngleRad) 
									   + mtrCurPosn.x * (1.0 - cos(dAngleRad)) + mtrCurPosn.y * sin(dAngleRad));

	p_dmtrCORPosn->y = dDeterminant * (dmtrResultPosn.x * sin(dAngleRad) + dmtrResultPosn.y * (1.0 - cos(dAngleRad))
									   - mtrCurPosn.x * sin(dAngleRad) + mtrCurPosn.y * (1.0 - cos(dAngleRad)));

	return TRUE;
}

BOOL VectorRotate(D_MTR_POSN dmtrIn, DOUBLE dAngleRad, D_MTR_POSN *p_dmtrOut) //20120810
{
	D_MTR_POSN dmtrTmp;

	dmtrTmp.x = dmtrIn.x;
	dmtrTmp.y = dmtrIn.y;

	p_dmtrOut->x = (dmtrTmp.x * cos(dAngleRad)) - (dmtrTmp.y * sin(dAngleRad));
	p_dmtrOut->y = (dmtrTmp.x * sin(dAngleRad)) + (dmtrTmp.y * cos(dAngleRad));
	return TRUE;
}

BOOL VectorRotate(MTR_POSN mtrIn, DOUBLE dAngleRad, MTR_POSN *p_mtrOut) //20120810
{
	D_MTR_POSN dmtrTmp;

	dmtrTmp.x = mtrIn.x;
	dmtrTmp.y = mtrIn.y;

	p_mtrOut->x = DoubleToLong((dmtrTmp.x * cos(dAngleRad)) - (dmtrTmp.y * sin(dAngleRad)));
	p_mtrOut->y = DoubleToLong((dmtrTmp.x * sin(dAngleRad)) + (dmtrTmp.y * cos(dAngleRad)));
	return TRUE;
}

DOUBLE calcAngle(D_MTR_POSN dmtrP1, D_MTR_POSN dmtrP2) //20120924
{
	DOUBLE dx, dy;

	dx = 1.0 * (dmtrP2.x - dmtrP1.x);
	dy = 1.0 * (dmtrP2.y - dmtrP1.y);

	if (dx != 0)
	{
		return atan(dy / dx);
	}
	else
	{
		return 0.0;
	}
}

DOUBLE calcAngleDegree(D_MTR_POSN dmtrP1, D_MTR_POSN dmtrP2)
{
	DOUBLE dx, dy;

	dx = 1.0 * (dmtrP2.x - dmtrP1.x);
	dy = 1.0 * (dmtrP2.y - dmtrP1.y);

	if (dx != 0)
	{
		return (180.0 / PI) * atan(dy / dx);
	}
	else
	{
		return 0.0;
	}
}

DOUBLE calcAngle(MTR_POSN mtrP1, MTR_POSN mtrP2)
{
	DOUBLE dx, dy;

	dx = 1.0 * (mtrP2.x - mtrP1.x);
	dy = 1.0 * (mtrP2.y - mtrP1.y);

	if (dx != 0)
	{
		return atan(dy / dx);
	}
	else
	{
		return 0.0;
	}
}

DOUBLE calcAngleDegree(MTR_POSN mtrP1, MTR_POSN mtrP2)
{
	DOUBLE dx, dy;

	dx = 1.0 * (mtrP2.x - mtrP1.x);
	dy = 1.0 * (mtrP2.y - mtrP1.y);

	if (dx != 0)
	{
		return (180.0 / PI) * atan(dy / dx);
	}
	else
	{
		return 0.0;
	}
}

DOUBLE calcDisplayAngleDegree(DOUBLE dy, DOUBLE dx)
{ 
//range -180 to 180 degree
	DOUBLE dRtn;
	if (dx != 0.0)
	{
		if (dy == 0.0)
		{
			if (dx > 0)
			{
				dRtn = 0.0;
			}
			else
			{
				dRtn = 180.0;
			}
		}
		else if (dy > 0 && dx < 0)
		{
			dRtn = 180.0 + (atan(dy / dx) * 180.0 / PI);
		}
		else if (dy < 0 && dx < 0)
		{
			dRtn = 180.0 - (atan(dy / dx) * 180.0 / PI);
		}
		else
		{
			dRtn = atan(dy / dx) * 180.0 / PI;
		}
	}
	else if (dy == 0.0)
	{
		dRtn = 0.0;
	}
	else if (dy > 0.0)
	{
		dRtn = 90.0;
	}
	else
	{
		dRtn = -90.0;
	}
	return dRtn;
}

BOOL ForceToDac(DOUBLE dTargetForce, FORCE_DATA stForceCalib[], LONG *p_lOpenDacValue)
{
	BOOL bStatus = FALSE;

	// min force, dead weight of the bond head
	if (dTargetForce < stForceCalib[0].Force_kgf)
	{
		*p_lOpenDacValue = stForceCalib[0].OpenDacValue;
		bStatus = FALSE;
	}
	else
	{
		INT i = 0;

		for (i = 0; i < MAX_FORCE_DATA - 1; i++)
		{
			if ((dTargetForce == stForceCalib[i].Force_kgf) /*&& (stForceCalib[i].Force_kgf == stForceCalib[i+1].Force_kgf)*/)
			{
				*p_lOpenDacValue = stForceCalib[i].OpenDacValue;
				bStatus = TRUE;
				break;
			}

			if ((dTargetForce >= stForceCalib[i].Force_kgf) && (dTargetForce < stForceCalib[i + 1].Force_kgf))
			{
				*p_lOpenDacValue = DoubleToLong((DOUBLE)(stForceCalib[i + 1].OpenDacValue)
												- (stForceCalib[i + 1].Force_kgf - dTargetForce) 
												* (DOUBLE)(stForceCalib[i + 1].OpenDacValue - stForceCalib[i].OpenDacValue) 
												/ (stForceCalib[i + 1].Force_kgf - stForceCalib[i].Force_kgf));
				bStatus = TRUE;
				break;
			}

			if (dTargetForce == stForceCalib[i + 1].Force_kgf)
			{
				*p_lOpenDacValue = stForceCalib[i + 1].OpenDacValue;
				bStatus = TRUE;
				break;
			}
		}
	}

	// max force, projection
	if (!bStatus)
	{
		if (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - stForceCalib[MAX_FORCE_DATA - 2].Force_kgf == 0)
		{
			*p_lOpenDacValue = 0;
		}
		else
		{
			*p_lOpenDacValue = DoubleToLong((DOUBLE)(stForceCalib[MAX_FORCE_DATA - 1].OpenDacValue)
											- (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - dTargetForce) 
											* (DOUBLE)(stForceCalib[MAX_FORCE_DATA - 1].OpenDacValue - stForceCalib[MAX_FORCE_DATA - 2].OpenDacValue) 
											/ (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - stForceCalib[MAX_FORCE_DATA - 2].Force_kgf));
		}
	}

	//if (*p_lOpenDacValue < 0.0)
	//{
	//	*p_lOpenDacValue = 0;
	//}
	return bStatus;
}

BOOL DacToForce(DOUBLE dTargetDacValue, FORCE_DATA stForceCalib[], DOUBLE *p_dForce)
{
	BOOL bStatus = FALSE;

	// min pressure
	if (dTargetDacValue < stForceCalib[0].OpenDacValue)
	{
		*p_dForce = stForceCalib[0].Force_kgf;
		bStatus = FALSE;
	}
	else
	{
		INT i = 0;

		for (i = 0; i < MAX_FORCE_DATA - 1; i++)
		{
			if ((dTargetDacValue == stForceCalib[i].OpenDacValue) /*&& (stForceCalib[i].OpenDacValue == stForceCalib[i+1].OpenDacValue)*/)
			{
				*p_dForce = stForceCalib[i].Force_kgf;
				bStatus = TRUE;
				break;
			}

			if ((dTargetDacValue >= stForceCalib[i].OpenDacValue) && (dTargetDacValue < stForceCalib[i + 1].OpenDacValue))
			{
				*p_dForce = stForceCalib[i + 1].Force_kgf 
							- (stForceCalib[i + 1].OpenDacValue - dTargetDacValue) 
							* (stForceCalib[i + 1].Force_kgf - stForceCalib[i].Force_kgf) 
							/ (stForceCalib[i + 1].OpenDacValue - stForceCalib[i].OpenDacValue);
				bStatus = TRUE;
				break;
			}

			if (dTargetDacValue == stForceCalib[i + 1].OpenDacValue)
			{
				*p_dForce = stForceCalib[i + 1].Force_kgf;
				bStatus = TRUE;
				break;
			}

		}
	}

	// max force, projection
	if (!bStatus)
	{
		if (stForceCalib[MAX_FORCE_DATA - 1].OpenDacValue - stForceCalib[MAX_FORCE_DATA - 2].OpenDacValue == 0)
		{
			*p_dForce = 0;
		}
		else
		{
			*p_dForce = stForceCalib[MAX_FORCE_DATA - 1].Force_kgf 
						- (stForceCalib[MAX_FORCE_DATA - 1].OpenDacValue - dTargetDacValue) 
						* (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - stForceCalib[MAX_FORCE_DATA - 2].Force_kgf) 
						/ (stForceCalib[MAX_FORCE_DATA - 1].OpenDacValue - stForceCalib[MAX_FORCE_DATA - 2].OpenDacValue);
		}
	}

	if (*p_dForce < 0.0)
	{
		*p_dForce = 0.0;
	}
	return bStatus;
}

BOOL ForceToPressure(DOUBLE dTargetForce, FORCE_DATA stForceCalib[], DOUBLE *p_dPressure)
{
	BOOL bStatus = FALSE;

	// min force, dead weight of the bond head
	if (dTargetForce < stForceCalib[0].Force_kgf)
	{
		*p_dPressure = stForceCalib[0].Pressure_bar;
		bStatus = FALSE;
	}
	else
	{
		INT i = 0;

		for (i = 0; i < MAX_FORCE_DATA - 1; i++)
		{
			if ((dTargetForce == stForceCalib[i].Force_kgf) && (stForceCalib[i].Force_kgf == stForceCalib[i + 1].Force_kgf))
			{
				*p_dPressure = stForceCalib[i].Pressure_bar;
				bStatus = TRUE;
				break;
			}

			if ((dTargetForce >= stForceCalib[i].Force_kgf) && (dTargetForce < stForceCalib[i + 1].Force_kgf))
			{
				*p_dPressure = stForceCalib[i + 1].Pressure_bar 
							   - (stForceCalib[i + 1].Force_kgf - dTargetForce) 
							   * (stForceCalib[i + 1].Pressure_bar - stForceCalib[i].Pressure_bar) 
							   / (stForceCalib[i + 1].Force_kgf - stForceCalib[i].Force_kgf);
				bStatus = TRUE;
				break;
			}
		}
	}

	// max force, projection
	if (!bStatus)
	{
		if (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - stForceCalib[MAX_FORCE_DATA - 2].Force_kgf == 0)
		{
			*p_dPressure = 0;
		}
		else
		{
			*p_dPressure = stForceCalib[MAX_FORCE_DATA - 1].Pressure_bar 
						   - (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - dTargetForce) 
						   * (stForceCalib[MAX_FORCE_DATA - 1].Pressure_bar - stForceCalib[MAX_FORCE_DATA - 2].Pressure_bar) 
						   / (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - stForceCalib[MAX_FORCE_DATA - 2].Force_kgf);
		}
	}

	if (*p_dPressure < 0.0)
	{
		*p_dPressure = 0.0;
	}
	return bStatus;
}

BOOL PressureToForce(DOUBLE dTargetPressure, FORCE_DATA stForceCalib[], DOUBLE *p_dForce)
{
	BOOL bStatus = FALSE;

	// min pressure
	if (dTargetPressure < stForceCalib[0].Pressure_bar)
	{
		*p_dForce = stForceCalib[0].Force_kgf;
		bStatus = FALSE;
	}
	else
	{
		INT i = 0;

		for (i = 0; i < MAX_FORCE_DATA - 1; i++)
		{
			if ((dTargetPressure == stForceCalib[i].Pressure_bar) && (stForceCalib[i].Pressure_bar == stForceCalib[i + 1].Pressure_bar))
			{
				*p_dForce = stForceCalib[i].Force_kgf;
				bStatus = TRUE;
				break;
			}

			if ((dTargetPressure >= stForceCalib[i].Pressure_bar) && (dTargetPressure < stForceCalib[i + 1].Pressure_bar))
			{
				*p_dForce = stForceCalib[i + 1].Force_kgf 
							- (stForceCalib[i + 1].Pressure_bar - dTargetPressure) 
							* (stForceCalib[i + 1].Force_kgf - stForceCalib[i].Force_kgf) 
							/ (stForceCalib[i + 1].Pressure_bar - stForceCalib[i].Pressure_bar);
				bStatus = TRUE;
				break;
			}
		}
	}

	// max force, projection
	if (!bStatus)
	{
		if (stForceCalib[MAX_FORCE_DATA - 1].Pressure_bar - stForceCalib[MAX_FORCE_DATA - 2].Pressure_bar == 0)
		{
			*p_dForce = 0;
		}
		else
		{
			*p_dForce = stForceCalib[MAX_FORCE_DATA - 1].Force_kgf 
						- (stForceCalib[MAX_FORCE_DATA - 1].Pressure_bar - dTargetPressure) 
						* (stForceCalib[MAX_FORCE_DATA - 1].Force_kgf - stForceCalib[MAX_FORCE_DATA - 2].Force_kgf) 
						/ (stForceCalib[MAX_FORCE_DATA - 1].Pressure_bar - stForceCalib[MAX_FORCE_DATA - 2].Pressure_bar);
		}
	}

	if (*p_dForce < 0.0)
	{
		*p_dForce = 0.0;
	}
	return bStatus;
}


BOOL RotaryToLinearEnc(LONG lTargetRotaryEnc, WHZ_DATA stWhZ_q[], LONG *p_lLinearEnc)
{
	BOOL bStatus = FALSE;
	LONG lResult;

	// WHZ lowest Calib posn
	if (lTargetRotaryEnc > stWhZ_q[0].RotaryPosn)
	{
		*p_lLinearEnc = stWhZ_q[0].LinearPosn;
		bStatus = FALSE;
	}
	else if (lTargetRotaryEnc < stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].RotaryPosn) // over highest calib posn
	{
		*p_lLinearEnc = stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].LinearPosn;
		bStatus = FALSE;
	}
	else if (lTargetRotaryEnc == stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].RotaryPosn) // highest calib posn
	{
		*p_lLinearEnc = stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].LinearPosn;
		bStatus = TRUE;
	}
	else // search Enc 
	{
		INT i = 0;

		for (i = 0; i < MAX_NUM_WHZ_CAL_STEP - 1; i++)
		{
			if (lTargetRotaryEnc == stWhZ_q[i].RotaryPosn) 
			{
				*p_lLinearEnc = stWhZ_q[i].LinearPosn;
				bStatus = TRUE;
				break;
			}

			if ((lTargetRotaryEnc < stWhZ_q[i].RotaryPosn) && (lTargetRotaryEnc > stWhZ_q[i + 1].RotaryPosn))
			{
				lResult = stWhZ_q[i + 1].LinearPosn 
						  - ((stWhZ_q[i + 1].RotaryPosn - lTargetRotaryEnc) 
							 * (stWhZ_q[i + 1].LinearPosn - stWhZ_q[i].LinearPosn) 
							 / (stWhZ_q[i + 1].RotaryPosn - stWhZ_q[i].RotaryPosn));	//debug

				*p_lLinearEnc = stWhZ_q[i + 1].LinearPosn 
								- ((stWhZ_q[i + 1].RotaryPosn - lTargetRotaryEnc) 
								   * (stWhZ_q[i + 1].LinearPosn - stWhZ_q[i].LinearPosn) 
								   / (stWhZ_q[i + 1].RotaryPosn - stWhZ_q[i].RotaryPosn));
				bStatus = TRUE;
				break;
			}
		}
	}

	return bStatus;
}


BOOL LinearToRotaryEnc(LONG lTargetLinearEnc, WHZ_DATA stWhZ_q[], LONG *p_lRotaryEnc)
{
	BOOL bStatus = FALSE;

	// WHZ lowest Calib posn
	if (lTargetLinearEnc > stWhZ_q[0].LinearPosn)
	{
		*p_lRotaryEnc = stWhZ_q[0].RotaryPosn;
		bStatus = FALSE;
	}
	else if (lTargetLinearEnc < stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].LinearPosn) // over highest calib posn
	{
		*p_lRotaryEnc = stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].RotaryPosn;
		bStatus = FALSE;
	}
	else if (lTargetLinearEnc == stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].LinearPosn) // highest calib posn
	{
		*p_lRotaryEnc = stWhZ_q[MAX_NUM_WHZ_CAL_STEP - 1].RotaryPosn;
		bStatus = TRUE;
	}
	else // search Enc 
	{
		INT i = 0;

		for (i = 0; i < MAX_NUM_WHZ_CAL_STEP - 1; i++)
		{
			if (lTargetLinearEnc == stWhZ_q[i].LinearPosn) 
			{
				*p_lRotaryEnc = stWhZ_q[i].RotaryPosn;
				bStatus = TRUE;
				break;
			}

			if ((lTargetLinearEnc < stWhZ_q[i].LinearPosn) && (lTargetLinearEnc > stWhZ_q[i + 1].LinearPosn))
			{
				*p_lRotaryEnc = stWhZ_q[i + 1].RotaryPosn 
								- ((stWhZ_q[i + 1].LinearPosn - lTargetLinearEnc) 
								   * (stWhZ_q[i + 1].RotaryPosn - stWhZ_q[i].RotaryPosn) 
								   / (stWhZ_q[i + 1].LinearPosn - stWhZ_q[i].LinearPosn));
				bStatus = TRUE;
				break;
			}
		}
	}

	return bStatus;
}

BOOL RotaryToLinearEnc(LONG lTargetRotaryEnc, ENC_CALIB_DATA stWhZ_q[], LONG *p_lLinearEnc)
{
	BOOL bStatus = FALSE;

	// WHZ lowest Calib posn
	if (lTargetRotaryEnc > stWhZ_q[0].RotaryPosn)
	{
		*p_lLinearEnc = stWhZ_q[0].LinearPosn;
		bStatus = FALSE;
	}
	else if (lTargetRotaryEnc < stWhZ_q[PREBOND_T_CALIB_DATA - 1].RotaryPosn) // over highest calib posn
	{
		*p_lLinearEnc = stWhZ_q[PREBOND_T_CALIB_DATA - 1].LinearPosn;
		bStatus = FALSE;
	}
	else if (lTargetRotaryEnc == stWhZ_q[PREBOND_T_CALIB_DATA - 1].RotaryPosn) // highest calib posn
	{
		*p_lLinearEnc = stWhZ_q[PREBOND_T_CALIB_DATA - 1].LinearPosn;
		bStatus = TRUE;
	}
	else // search Enc 
	{
		INT i = 0;

		for (i = 0; i < PREBOND_T_CALIB_DATA - 1; i++)
		{
			if (lTargetRotaryEnc == stWhZ_q[i].RotaryPosn) 
			{
				*p_lLinearEnc = stWhZ_q[i].LinearPosn;
				bStatus = TRUE;
				break;
			}

			if ((lTargetRotaryEnc < stWhZ_q[i].RotaryPosn) && (lTargetRotaryEnc > stWhZ_q[i + 1].RotaryPosn))
			{
				*p_lLinearEnc = stWhZ_q[i + 1].LinearPosn
								- ((stWhZ_q[i + 1].RotaryPosn - lTargetRotaryEnc) 
								   * (stWhZ_q[i + 1].LinearPosn - stWhZ_q[i].LinearPosn) 
								   / (stWhZ_q[i + 1].RotaryPosn - stWhZ_q[i].RotaryPosn));
				bStatus = TRUE;
				break;
			}
		}
	}

	return bStatus;
}


BOOL LinearToRotaryEnc(LONG lTargetLinearEnc, ENC_CALIB_DATA stWhZ_q[], LONG *p_lRotaryEnc)
{
	BOOL bStatus = FALSE;

	// WHZ lowest Calib posn
	if (lTargetLinearEnc > stWhZ_q[0].LinearPosn)
	{
		*p_lRotaryEnc = stWhZ_q[0].RotaryPosn;
		bStatus = FALSE;
	}
	else if (lTargetLinearEnc < stWhZ_q[PREBOND_T_CALIB_DATA - 1].LinearPosn) // over highest calib posn
	{
		*p_lRotaryEnc = stWhZ_q[PREBOND_T_CALIB_DATA - 1].RotaryPosn;
		bStatus = FALSE;
	}
	else if (lTargetLinearEnc == stWhZ_q[PREBOND_T_CALIB_DATA - 1].LinearPosn) // highest calib posn
	{
		*p_lRotaryEnc = stWhZ_q[PREBOND_T_CALIB_DATA - 1].RotaryPosn;
		bStatus = TRUE;
	}
	else // search Enc 
	{
		INT i = 0;

		for (i = 0; i < PREBOND_T_CALIB_DATA - 1; i++)
		{
			if (lTargetLinearEnc == stWhZ_q[i].LinearPosn) 
			{
				*p_lRotaryEnc = stWhZ_q[i].RotaryPosn;
				bStatus = TRUE;
				break;
			}

			if ((lTargetLinearEnc < stWhZ_q[i].LinearPosn) && (lTargetLinearEnc > stWhZ_q[i + 1].LinearPosn))
			{
				*p_lRotaryEnc = stWhZ_q[i + 1].RotaryPosn 
								- ((stWhZ_q[i + 1].LinearPosn - lTargetLinearEnc) 
								   * (stWhZ_q[i + 1].RotaryPosn - stWhZ_q[i].RotaryPosn) 
								   / (stWhZ_q[i + 1].LinearPosn - stWhZ_q[i].LinearPosn));
				bStatus = TRUE;
				break;
			}
		}
	}

	return bStatus;
}

BOOL CalcWHAbsoluteLevel(LONG lRefLevel, LONG lOffset, WHZ_DATA stData[MAX_NUM_OF_WHZ][MAX_NUM_WHZ_CAL_STEP], LONG *p_lAbsoluteLevel, LONG WHZ)
{
	//CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
	LONG lLevelInLinear = 0;
	LONG lLevelInRotary	= 0;
	BOOL bResult = TRUE;
	if (
		WHZ == WHZ_2
	   )
	{
		// Transform Reference Level from Motor Count to Linear
		if (!RotaryToLinearEnc(lRefLevel, stData[WHZ_2], &lLevelInLinear))
		{
			lLevelInRotary = lRefLevel;
			bResult = FALSE;
		}
		else
		{
			lLevelInLinear += DistanceToCount(lOffset, WHZ_CAL_DISTPERCNT);

			if (!LinearToRotaryEnc(lLevelInLinear, stData[WHZ_2], &lLevelInRotary))
			{
				lLevelInRotary = lRefLevel;
				bResult = FALSE;
			}
		}
	}
	else
	{
	// Transform Reference Level from Motor Count to Linear
		if (!RotaryToLinearEnc(lRefLevel, stData[WHZ_1], &lLevelInLinear))
		{
			lLevelInRotary = lRefLevel;
			bResult = FALSE;
		}
		else
		{
			lLevelInLinear += DistanceToCount(lOffset, WHZ_CAL_DISTPERCNT);

			if (!LinearToRotaryEnc(lLevelInLinear, stData[WHZ_1], &lLevelInRotary))
			{
				lLevelInRotary = lRefLevel;
				bResult = FALSE;
			}
		}
	}
	*p_lAbsoluteLevel = lLevelInRotary;

	return bResult;
}

//BOOL CalcTAbsoluteLevel(LONG lRefLevel, LONG lOffset, ENC_CALIB_DATA stData[PREBOND_T_CALIB_DATA], LONG *p_lAbsoluteLevel)
//{
//	//CPI9000App *pAppMod = dynamic_cast<CPI9000App*>(m_pModule);
//	LONG lLevelInLinear = 0;
//	LONG lLevelInRotary	= 0;
//	BOOL bResult = TRUE;
//	{
//		// Transform Reference Level from Motor Count to Linear
//		if (!RotaryToLinearEnc(lRefLevel, stData, &lLevelInLinear))
//		{
//			lLevelInRotary = lRefLevel;
//			bResult = FALSE;
//		}
//		else
//		{
//			lLevelInLinear += DistanceToCount(lOffset, WHZ_CAL_DISTPERCNT);
//
//			if (!LinearToRotaryEnc(lLevelInLinear, stData[WHZ_2], &lLevelInRotary))
//			{
//				lLevelInRotary = lRefLevel;
//				bResult = FALSE;
//			}
//		}
//	}
//	*p_lAbsoluteLevel = lLevelInRotary;
//
//	return bResult;
//}

BOOL CalcWHLinearOffset(LONG lRefLevel, LONG lCurrLevel , WHZ_DATA stData[], LONG *p_lLinearOffset)
{
	LONG lRefLevelInLinear	= 0;
	LONG lCurrLevelInLinear	= 0;
	BOOL bResult = TRUE;

	// Transform Reference Level from Motor Count to Linear
	if (!RotaryToLinearEnc(lRefLevel, stData, &lRefLevelInLinear))
	{
		*p_lLinearOffset = 0;
		return FALSE;
	}
	if (!RotaryToLinearEnc(lCurrLevel, stData, &lCurrLevelInLinear))
	{
		*p_lLinearOffset = 0;
		return FALSE;
	}

	*p_lLinearOffset = DoubleToLong(CountToDistance((DOUBLE)lCurrLevelInLinear - (DOUBLE)lRefLevelInLinear, WHZ_CAL_DISTPERCNT));

	return TRUE;
}


LONG DistanceToCount(DOUBLE dDistance, DOUBLE dDistPerCount) //From um to count
{
	if (dDistPerCount != 0.0)
	{
		return DoubleToLong(dDistance / (dDistPerCount));
	}
	else
	{
		return 0;
	}
}

DOUBLE CountToDistance(DOUBLE dMotorCount, DOUBLE dDistPerCount)
{
	return dMotorCount * (dDistPerCount);
}

LONG AngleToCount(DOUBLE dAngle, DOUBLE dDistPerCount)
{
	if (dDistPerCount != 0.0)
	{
		return DoubleToLong(dAngle / dDistPerCount);
	}
	else
	{
		return 0;
	}
}

DOUBLE	CountToAngle(DOUBLE dMotorCount, DOUBLE dDistPerCount)
{
	if (dDistPerCount != 0.0)
	{
		return dMotorCount * dDistPerCount;
	}
	else
	{
		return 0.0;
	}
}

DOUBLE DistanceInHipec(LONG lDistance, DOUBLE dDistPerCount)
{
	DOUBLE dTemp = (DOUBLE)lDistance * (dDistPerCount / 1000.0);

	return dTemp;
}

DOUBLE VelocityInHipec(LONG lVelocity, DOUBLE dDistPerCount)
{
	DOUBLE dTemp = lVelocity * (dDistPerCount / 1000.0) * HIPEC_FREQ / 64.0;

	return dTemp;
}

LONG HipecInDistance(DOUBLE dHipecDist, DOUBLE dDistPerCount)
{
	LONG lTemp = DoubleToLong(dHipecDist / (dDistPerCount / 1000.0));

	return lTemp;
}

LONG HipecInVelocity(DOUBLE dVelocity, DOUBLE dDistPerCount)
{
	LONG lTemp = DoubleToLong(dVelocity / ((dDistPerCount / 1000.0) * HIPEC_FREQ) * 64.0);

	return lTemp;
}

VOID CalcPosnAfterTransform(D_MTR_POSN dmtrCurrOffset, DOUBLE dCurrDistPerCnt, DOUBLE dResultDistPerCnt, DOUBLE dAngleBtnAxis, D_MTR_POSN *p_dmtrResultOffset)
{
	D_MTR_POSN dmtrTempPosn;
		
	dmtrTempPosn.x = dmtrCurrOffset.x * (dCurrDistPerCnt / dResultDistPerCnt);
	dmtrTempPosn.y = dmtrCurrOffset.y * (dCurrDistPerCnt / dResultDistPerCnt);

	dAngleBtnAxis = (PI / 180.0) * dAngleBtnAxis;

	p_dmtrResultOffset->x = (+dmtrTempPosn.x * cos(dAngleBtnAxis) - dmtrTempPosn.y * sin(dAngleBtnAxis));
	p_dmtrResultOffset->y = (+dmtrTempPosn.x * sin(dAngleBtnAxis) + dmtrTempPosn.y * cos(dAngleBtnAxis));
}

VOID SortTempCalibData(TEMP_DATA stTempCalib[])
{
	INT i, j;
	INT min;

	TEMP_DATA stTmpData[MAX_TEMP_DATA];

	for (i = 0; i < MAX_TEMP_DATA; i++)
	{
		stTmpData[i].SetTemp = stTempCalib[i].SetTemp;
		stTmpData[i].BondTemp = stTempCalib[i].BondTemp;

		stTempCalib[i].SetTemp = 0.0;
		stTempCalib[i].BondTemp = 0.0;
	}

	for (i = 0; i < MAX_TEMP_DATA; i++)
	{
		min = 0;

		for (j = 0; j < MAX_TEMP_DATA; j++)
		{
			if (stTmpData[j].SetTemp != 0)
			{
				if (stTmpData[min].SetTemp == 0.0 || stTmpData[j].SetTemp < stTmpData[min].SetTemp)
				{
					min = j;
				}
			}
		}

		stTempCalib[i].SetTemp = stTmpData[min].SetTemp;
		stTempCalib[i].BondTemp = stTmpData[min].BondTemp;

		stTmpData[min].SetTemp = 0.0;
		stTmpData[min].BondTemp = 0.0;
	}
}

VOID SortForceCalibData(FORCE_DATA stForceCalib[]) //20130416
{
	INT i, j;
	INT min;

	FORCE_DATA stTmpData[MAX_FORCE_DATA];

	for (i = 0; i < MAX_FORCE_DATA; i++)
	{
		stTmpData[i].Force_kgf		= stForceCalib[i].Force_kgf;
		stTmpData[i].Pressure_bar	= stForceCalib[i].Pressure_bar;		
		stTmpData[i].OpenDacValue	= stForceCalib[i].OpenDacValue;

		stForceCalib[i].Pressure_bar	= 0.0;
		stForceCalib[i].Force_kgf		= 0.0;
		stForceCalib[i].OpenDacValue	= 0;
	}

	for (i = 0; i < MAX_FORCE_DATA; i++)
	{
		min = 0;

		for (j = 0; j < MAX_FORCE_DATA; j++)
		{
			if (stTmpData[j].Pressure_bar != 0)
			{
				if (stTmpData[min].Pressure_bar == 0.0 || stTmpData[j].Pressure_bar < stTmpData[min].Pressure_bar)
				{
					min = j;
				}
			}
		}

		stForceCalib[i].Pressure_bar	= stTmpData[min].Pressure_bar;
		stForceCalib[i].Force_kgf		= stTmpData[min].Force_kgf;

		stTmpData[min].Pressure_bar		= 0.0;
		stTmpData[min].Force_kgf		= 0.0;
	}
}

BOOL SetTempToBondTemp(DOUBLE dSetTemp, TEMP_DATA stTempCalib[], DOUBLE *p_dBondTemp)
{
	BOOL bStatus = FALSE;

	// stTempCalib[] is empty
	if (stTempCalib[0].SetTemp == 0.0)
	{
		*p_dBondTemp = dSetTemp;
		bStatus = FALSE;
	}
	// stTempCalib[] is not empty
	else
	{
		// min temp
		if (dSetTemp < stTempCalib[0].SetTemp)
		{
			//if (dSetTemp < stTempCalib[0].BondTemp)
			//{
			//	*p_dBondTemp = dSetTemp;
			//}
			//else
			//{
			//	*p_dBondTemp = stTempCalib[0].BondTemp;
			//}
			if (stTempCalib[0].SetTemp != 0)
			{
				*p_dBondTemp = stTempCalib[0].BondTemp 
							   - (stTempCalib[0].SetTemp - dSetTemp) 
							   * (stTempCalib[0].BondTemp - 0.0) 
							   / (stTempCalib[0].SetTemp - 0.0);
			}
			else
			{
				*p_dBondTemp = dSetTemp;
			}
			bStatus = FALSE;
		}
		else
		{
			INT i = 0;

			for (i = 0; i < MAX_TEMP_DATA - 1; i++)
			{
				if (dSetTemp == stTempCalib[i].SetTemp)
				{
					*p_dBondTemp = stTempCalib[i].BondTemp;
					bStatus = TRUE;
					return bStatus;
				}

				if ((dSetTemp > stTempCalib[i].SetTemp) && (dSetTemp < stTempCalib[i + 1].SetTemp))
				{
					*p_dBondTemp = stTempCalib[i + 1].BondTemp 
								   - (stTempCalib[i + 1].SetTemp - dSetTemp) 
								   * (stTempCalib[i + 1].BondTemp - stTempCalib[i].BondTemp) 
								   / (stTempCalib[i + 1].SetTemp - stTempCalib[i].SetTemp);
					bStatus = TRUE;
					return bStatus;
				}

				if ((dSetTemp > stTempCalib[i].SetTemp) && (stTempCalib[i + 1].SetTemp == 0))
				{
					if (i == 0)
					{
						*p_dBondTemp = stTempCalib[i].BondTemp 
									   - (stTempCalib[i].SetTemp - dSetTemp) 
									   * (stTempCalib[i].BondTemp - 0.0) 
									   / (stTempCalib[i].SetTemp - 0.0);
						bStatus = FALSE;
						return bStatus;
					}
					else
					{
						*p_dBondTemp = stTempCalib[i].BondTemp 
									   - (stTempCalib[i].SetTemp - dSetTemp) 
									   * (stTempCalib[i].BondTemp - stTempCalib[i - 1].BondTemp) 
									   / (stTempCalib[i].SetTemp - stTempCalib[i - 1].SetTemp);
						bStatus = FALSE;
						return bStatus;
					}
				}

				if (dSetTemp == stTempCalib[i + 1].SetTemp)
				{
					*p_dBondTemp = stTempCalib[i + 1].BondTemp;
					bStatus = TRUE;
					return bStatus;
				}
			}
			// max Temp and project compare with all 5 Data 
			if (dSetTemp > stTempCalib[MAX_TEMP_DATA - 1].SetTemp)
			{
				*p_dBondTemp = stTempCalib[MAX_TEMP_DATA - 1].BondTemp 
							   - (stTempCalib[MAX_TEMP_DATA - 1].SetTemp - dSetTemp) 
							   * (stTempCalib[MAX_TEMP_DATA - 1].BondTemp - stTempCalib[MAX_TEMP_DATA - 2].BondTemp) 
							   / (stTempCalib[MAX_TEMP_DATA - 1].SetTemp - stTempCalib[MAX_TEMP_DATA - 2].SetTemp);
				bStatus = FALSE;
				return bStatus;
			}
		}
	}

	return bStatus;
}

BOOL BondTempToSetTemp(DOUBLE dBondTemp, TEMP_DATA stTempCalib[], DOUBLE *p_dSetTemp)
{
	BOOL bStatus = FALSE;

	// stTempCalib[] is empty
	if (stTempCalib[0].SetTemp == 0.0)
	{
		*p_dSetTemp = dBondTemp;
		bStatus = FALSE;
	}
	// stTempCalib[] is not empty
	else
	{
		// min temp
		if (dBondTemp < stTempCalib[0].BondTemp)
		{
			//if (dBondTemp < stTempCalib[0].SetTemp)
			//{
			//	*p_dSetTemp = dBondTemp;
			//}
			//else
			//{
			//	*p_dSetTemp = stTempCalib[0].SetTemp;
			//}

			if (stTempCalib[0].BondTemp != 0)
			{
				*p_dSetTemp = stTempCalib[0].SetTemp 
							  - (stTempCalib[0].BondTemp - dBondTemp) 
							  * (stTempCalib[0].SetTemp - 0.0) 
							  / (stTempCalib[0].BondTemp - 0.0);
			}
			else
			{
				*p_dSetTemp = dBondTemp;
			}
			bStatus = FALSE;
		}
		else
		{
			INT i = 0;

			for (i = 0; i < MAX_TEMP_DATA - 1; i++)
			{
				if (dBondTemp == stTempCalib[i].BondTemp)
				{
					*p_dSetTemp = stTempCalib[i].SetTemp;
					bStatus = TRUE;
					break;
				}

				if ((dBondTemp > stTempCalib[i].BondTemp) && (dBondTemp < stTempCalib[i + 1].BondTemp))
				{
					*p_dSetTemp = stTempCalib[i + 1].SetTemp 
								  - (stTempCalib[i + 1].BondTemp - dBondTemp) 
								  * (stTempCalib[i + 1].SetTemp - stTempCalib[i].SetTemp) 
								  / (stTempCalib[i + 1].BondTemp - stTempCalib[i].BondTemp);
					bStatus = TRUE;
					break;
				}

				if ((dBondTemp > stTempCalib[i].BondTemp) && (stTempCalib[i + 1].BondTemp == 0))
				{
					if (i == 0)
					{
						*p_dSetTemp = stTempCalib[i].SetTemp 
									  - (stTempCalib[i].BondTemp - dBondTemp) 
									  * (stTempCalib[i].SetTemp - 0.0) 
									  / (stTempCalib[i].BondTemp - 0.0);
						bStatus = FALSE;
						break;
					}
					else
					{
						*p_dSetTemp = stTempCalib[i].SetTemp 
									  - (stTempCalib[i].BondTemp - dBondTemp) 
									  * (stTempCalib[i].SetTemp - stTempCalib[i - 1].SetTemp) 
									  / (stTempCalib[i].BondTemp - stTempCalib[i - 1].BondTemp);
						bStatus = FALSE;
						break;
					}
				}

				if (dBondTemp == stTempCalib[i + 1].BondTemp)
				{
					*p_dSetTemp = stTempCalib[i + 1].SetTemp;
					bStatus = TRUE;
					break;
				}
			}
			// max Temp and project compare with all 5 Data 
			if (dBondTemp > stTempCalib[MAX_TEMP_DATA - 1].BondTemp)
			{
				*p_dSetTemp = stTempCalib[MAX_TEMP_DATA - 1].SetTemp 
							  - (stTempCalib[MAX_TEMP_DATA - 1].BondTemp - dBondTemp) 
							  * (stTempCalib[MAX_TEMP_DATA - 1].SetTemp - stTempCalib[MAX_TEMP_DATA - 2].SetTemp) 
							  / (stTempCalib[MAX_TEMP_DATA - 1].BondTemp - stTempCalib[MAX_TEMP_DATA - 2].BondTemp);
				bStatus = FALSE;
				return bStatus;
			}

		}
	}

	return bStatus;
}

LSITypeNum GetLSITypeNum(HandOverStatusNum lHandOverStatusNum)
{
	switch (lHandOverStatusNum)
	{
	case HANDOVER_LSI_TYPE1_SH1:
	case HANDOVER_LSI_TYPE1_SH2:
		return LSI_TYPE1;

	case HANDOVER_LSI_TYPE2_SH1:
	case HANDOVER_LSI_TYPE2_SH2:
		return LSI_TYPE2;

	default:
		return NO_LSI_TYPE;
	}
}

ShuttleUnitNum GetShuttleUnit(HandOverStatusNum lHandOverStatusNum)
{
	switch (lHandOverStatusNum)
	{
	case HANDOVER_LSI_TYPE1_SH1:
	case HANDOVER_LSI_TYPE2_SH1:
		return SHUTTLE_1;

	case HANDOVER_LSI_TYPE1_SH2:
	case HANDOVER_LSI_TYPE2_SH2:
		return SHUTTLE_2;

	default:
		return SHUTTLE_NONE;
	}
}

BOOL SetHandOverStatus(LSITypeNum lLSIType, ShuttleUnitNum lShuttleUnit, HandOverStatusNum *lHandOverStatus) 
{
	LONG  lTemp;
	lTemp = lShuttleUnit + lLSIType * (MAX_NUM_OF_LSI_TYPE);

	if (
		lTemp >= MAX_NUM_OF_HANDOVER || 
		lTemp <= NO_HANDOVER
	   )
	{
		return FALSE;
	}
	else
	{
		*lHandOverStatus = (HandOverStatusNum)lTemp;
		return TRUE;
	}
}

STATUS_TYPE SetPickLSIStatus(HandOverStatusNum lHandOverStatus)
{
	if (lHandOverStatus == HANDOVER_LSI_TYPE1_SH1)
	{
		return ST_REQ_PICK_TYPE1_SH1;
	}
	else if (lHandOverStatus == HANDOVER_LSI_TYPE1_SH2)
	{
		return ST_REQ_PICK_TYPE1_SH2;
	}
	else if (lHandOverStatus == HANDOVER_LSI_TYPE2_SH1)
	{
		return ST_REQ_PICK_TYPE2_SH1;
	}
	else if (lHandOverStatus == HANDOVER_LSI_TYPE2_SH2)
	{
		return ST_REQ_PICK_TYPE2_SH2;
	}

	return ST_ERROR;
}

STATUS_TYPE SetPlaceLSIStatus(HandOverStatusNum lHandOverStatus)
{
	if (lHandOverStatus == HANDOVER_LSI_TYPE1_SH1)
	{
		return ST_REQ_PLACE_TYPE1_SH1;
	}
	else if (lHandOverStatus == HANDOVER_LSI_TYPE1_SH2)
	{
		return ST_REQ_PLACE_TYPE1_SH2;
	}
	else if (lHandOverStatus == HANDOVER_LSI_TYPE2_SH1)
	{
		return ST_REQ_PLACE_TYPE2_SH1;
	}
	else if (lHandOverStatus == HANDOVER_LSI_TYPE2_SH2)
	{
		return ST_REQ_PLACE_TYPE2_SH2;
	}

	return ST_ERROR;
}

HandOverStatusNum GetHandOverStatus(STATUS_TYPE stStationStatus)
{
	if (stStationStatus == ST_REQ_PLACE_TYPE1_SH1)
	{
		return HANDOVER_LSI_TYPE1_SH1;
	}
	else if (stStationStatus == ST_REQ_PLACE_TYPE1_SH2)
	{
		return HANDOVER_LSI_TYPE1_SH2;
	}
	else if (stStationStatus == ST_REQ_PLACE_TYPE2_SH1)
	{
		return HANDOVER_LSI_TYPE2_SH1;
	}	
	else if (stStationStatus == ST_REQ_PLACE_TYPE2_SH2)
	{
		return HANDOVER_LSI_TYPE2_SH2;
	}

	return NO_HANDOVER;
}

LONG GetMaxValue(LONG lValue1, LONG lValue2, LONG lValue3, LONG lValue4)
{
	LONG lMaxValue = lValue1;

	if (lValue2 > lMaxValue)
	{
		lMaxValue = lValue2;
	}

	if (lValue3 > lMaxValue)
	{
		lMaxValue = lValue3;
	}

	if (lValue4 > lMaxValue)
	{
		lMaxValue = lValue4;
	}

	return lMaxValue;
}



LONG SetGlassStatus(STATUS_TYPE StationStatus, STATUS_TYPE *pGLASS_Status)
{
	switch (StationStatus)
	{
	case ST_PLACE_READY1_EMPTY2:
		pGLASS_Status[GLASS1] = ST_READY;
		pGLASS_Status[GLASS2] = ST_EMPTY;
		break;

	case ST_PLACE_READY1_ERR2:
		pGLASS_Status[GLASS1] = ST_READY;
		pGLASS_Status[GLASS2] = ST_ERROR;
		break;

	case ST_PLACE_EMPTY1_READY2:
		pGLASS_Status[GLASS1] = ST_EMPTY;
		pGLASS_Status[GLASS2] = ST_READY;
		break;

	case ST_PLACE_EMPTY1_ERR2:
		pGLASS_Status[GLASS1] = ST_EMPTY;
		pGLASS_Status[GLASS2] = ST_ERROR;
		break;

	case ST_PLACE_ERR1_READY2:
		pGLASS_Status[GLASS1] = ST_ERROR;
		pGLASS_Status[GLASS2] = ST_READY;
		break;

	case ST_PLACE_ERR1_ERR2:
		pGLASS_Status[GLASS1] = ST_ERROR;
		pGLASS_Status[GLASS2] = ST_ERROR;
		break;

	case ST_PLACE_ERR1_EMPTY2:
		pGLASS_Status[GLASS1] = ST_ERROR;
		pGLASS_Status[GLASS2] = ST_EMPTY;
		break;

	case ST_PLACE_READY1_READY2:
		pGLASS_Status[GLASS1] = ST_READY;
		pGLASS_Status[GLASS2] = ST_READY;
		break;

	default:
		return GMP_FAIL;
	}

	return GMP_SUCCESS;
}

LONG SetStationStatus(STATUS_TYPE GLASS_Status[])
{
	LONG lTempStatus = ST_PICK_EMPTY1_EMPTY2;

	for (INT i = GLASS1; i < MAX_NUM_OF_GLASS; i++)
	{
		switch (GLASS_Status[i])
		{
		case ST_READY:
		case ST_READY_1_LEFT:
		case ST_READY_1_RIGHT:
		case ST_READY_2_LEFT:
		case ST_READY_2_RIGHT:
		case ST_READY_1:
		case ST_READY_2:
			if (i == GLASS1)
			{
				lTempStatus = ST_PICK_READY1_READY2;
			}
			else
			{
				lTempStatus += 0;
			}
			break;

		case ST_ERROR:
			if (i == GLASS1)
			{
				lTempStatus = ST_PICK_ERR1_READY2;
			}
			else
			{
				lTempStatus += 1;
			}
			break;

		case ST_EMPTY:
			if (i == GLASS1)
			{
				lTempStatus = ST_PICK_EMPTY1_READY2;
			}
			else
			{
				lTempStatus += 2;
			}
			break;

		default:
			return ST_ERROR;
		}
	}

	return lTempStatus;

}

VOID CalcDataMean(DATA_SET *p_stDataSet)
{
	INT		i = 0;
	DOUBLE	dDataSum = 0.0;

	for (i = 0; i < MAX_RECORD_DATA; i++)
	{
		dDataSum += p_stDataSet->dDataSet[i];
	}

	p_stDataSet->dMean = dDataSum / MAX_RECORD_DATA;
}

VOID CalcDataRange(DATA_SET *p_stDataSet)
{
	INT i = 0;

	for (i = 0; i < MAX_RECORD_DATA; i++)
	{
		if (p_stDataSet->dDataSet[i] < p_stDataSet->dMin)
		{
			p_stDataSet->dMin = p_stDataSet->dDataSet[i];
		}
		else if (p_stDataSet->dDataSet[i] > p_stDataSet->dMax)
		{
			p_stDataSet->dMax = p_stDataSet->dDataSet[i];
		}
	}
}


BOOL CopyDir(const CString szSource, const CString szDest)
{
	CFileOperation fo;
	
	fo.SetOverwriteMode(TRUE);

	if (!fo.CheckPath(szSource))
	{
		//fo.ShowError();
		return FALSE;
	}
	
	//if (!fo.CheckPath(szDest))
	//{
	//	CreateDirectory(szDest, NULL);
	//	return FALSE;
	//}

	if (!fo.Copy(szSource, szDest))
	{
		fo.ShowError();
		return FALSE;
	}

	return TRUE;
}

BOOL DeleteDir(const CString szDest)
{
	CFileOperation fo;
	
	fo.SetOverwriteMode(TRUE);

	if (!fo.Delete(szDest))
	{
		//fo.ShowError();
		return FALSE;
	}

	return TRUE;
}

BOOL CheckPath(const CString szDest)
{
	CFileOperation fo;
	
	if (!fo.CheckPath(szDest))
	{
		//fo.ShowError();
		return FALSE;
	}

	return TRUE;
	
}

VOID CreatePath(const CString szPath)
{
	if (CheckPath(szPath) == PATH_NOT_FOUND) 
	{
		CreateDirectory(szPath, NULL);
	}
}

BOOL RenameDir(const CString szSource, const CString szDest)
{
	CFileOperation fo;
	
	fo.SetOverwriteMode(TRUE);

	if (!fo.Rename(szSource, szDest))
	{
		//fo.ShowError();
		return FALSE;
	}

	return TRUE;
}


BOOL IsSplitFile(CString szFileName, ULONG ulSizeInByte)
{
	CFileFind	FileFind;

	ULONG ulFileSize = szFileName.GetLength();

	BOOL bWorking = FileFind.FindFile(szFileName);

	if (!bWorking)
	{
		return TRUE;
	}

	while (bWorking)
	{
		bWorking = FileFind.FindNextFile();
		ulFileSize = (ULONG)FileFind.GetLength();
	}

	if (ulFileSize >= ulSizeInByte)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL BackupFile(CString szFileName)
{
	CFileFind	FileFind;
	CString		szBackupFile[NUM_OF_BACKUP];
	CString		szExtension;
	BOOL		bResult = TRUE;

	INT			i = 0;


	if (szFileName == "")
	{
		return FALSE;
	}

	szBackupFile[0] = szFileName;
	szFileName.TrimRight(".");
	for (i = 1; i < NUM_OF_BACKUP; i++)
	{
		szExtension.Format("00%ld", i + 1);
		szBackupFile[i] = szFileName.TrimRight(".") + szExtension;
	}
	

	for (i = NUM_OF_BACKUP - 1; i >= 0; i--)
	{
		// Delete the oldest File
		if ((i == NUM_OF_BACKUP - 1))
		{
			if (FileFind.FindFile(szBackupFile[i]))
			{
				DeleteDir(szBackupFile[i]);
			}
		}
		else
		{
			if (FileFind.FindFile(szBackupFile[i]))
			{
				bResult = RenameDir(szBackupFile[i], szBackupFile[i + 1]);
			}
		}
	}

	return bResult;
}

DOUBLE CalDequeAverage(std::deque <LONG> &stlDeque) //20130425
{
	DOUBLE dReturn = 0.0;
	std::deque <LONG>::size_type size;

	size = stlDeque.size();
	if (size == 0)
	{
		return 0.0;
	}

	for (INT ii = 0; ii < (INT)size; ii++)
	{
		dReturn += stlDeque[ii];
	}
	dReturn /= size;
	return dReturn;
}

DOUBLE CalDequeAverage(std::deque <DOUBLE> &stdDeque) //20151105
{
	DOUBLE dReturn = 0.0;
	std::deque <DOUBLE>::size_type size;

	size = stdDeque.size();
	if (size == 0)
	{
		return 0.0;
	}

	for (INT ii = 0; ii < (INT)size; ii++)
	{
		dReturn += stdDeque[ii];
	}
	dReturn /= size;
	return dReturn;
}

DOUBLE CalDequeSD(std::deque <DOUBLE> &stdDeque) //20160603
{
	DOUBLE dReturn = 0.0;
	std::deque <DOUBLE>::size_type size;

	size = stdDeque.size();
	if (size == 0)
	{
		return 0.0;
	}

	DOUBLE dMean = CalDequeAverage(stdDeque);
	for (INT ii = 0; ii < (INT)size; ii++)
	{
		dReturn += pow((stdDeque[ii] - dMean), 2.0);
	}
	dReturn = sqrt(dReturn / (size - 1));
	return dReturn;
}

VOID CalDequeRange(std::deque <DOUBLE> &stdDeque, DOUBLE &dMin, DOUBLE &dMax) //20160623
{
	DOUBLE dReturn = 0.0;
	std::deque <DOUBLE>::size_type size;
	//dMin = dMax = 0.0;
	size = stdDeque.size();
	if (size == 0)
	{
		return;
	}
	dMin = stdDeque[0];
	dMax = stdDeque[0];

	for (INT ii = 0; ii < (INT)size; ii++)
	{
		if (stdDeque[ii] < dMin)
		{
			dMin = stdDeque[ii];
		}
		if (stdDeque[ii] > dMax)
		{
			dMax = stdDeque[ii];
		}
	}
}

DOUBLE CalAngle(D_MTR_POSN dmtrPt1, D_MTR_POSN dmtrPt2)
{
	DOUBLE dx,dy;

	dx = dmtrPt2.x - dmtrPt1.x;
	dy = dmtrPt2.y - dmtrPt1.y;

	if (dx != 0)
	{
		return (180.0 / PI) * atan((dy / dx));	
	}
	else
	{
		return 0.0;
	}

}

DOUBLE	CalDistance(D_MTR_POSN dmtrPt1, D_MTR_POSN dmtrPt2, DOUBLE dDistPerCountX, DOUBLE dDistPerCountY)
{
	DOUBLE dx,dy;

	dx = dmtrPt2.x - dmtrPt1.x;
	dy = dmtrPt2.y - dmtrPt1.y;


	return sqrt(pow(dx * dDistPerCountX, 2.0) + pow(dy * dDistPerCountY, 2.0));	

}
