/////////////////////////////////////////////////////////////////
//	INWH.cpp : interface of the CINWH class
//
//	Description:
//		PPI9000 Application Software
//
//	Date:		Thursday, September 09, 2010
//	Revision:	1.00
//
//	By:			PPI9000
//				CSP
//
//	Copyright @ ASM Assembly Automation Ltd., 2010.
//	ALL rights reserved.
//
/////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PI9000.h"
#include "PI9000_Constant.h"
#include "INWH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CINWH, CWorkHolder)

CINWH::CINWH()
{
	// CPPI9000Srn
	//m_lStnErrorCode		= IDS_MAINBOND_WH_NOT_SELECTED_ERR;
	//m_lModSelectMode	= MB12_MOD;

	//m_lCurSlave = MAINBOND_1;
	//m_lCurSlaveGlass = GLASS1;

	// Motor Related

}

CINWH::~CINWH()
{
}

BOOL CINWH::InitInstance()
{
	CWorkHolder::InitInstance();

	return TRUE;
}

INT CINWH::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWorkHolder::ExitInstance();
}


/////////////////////////////////////////////////////////////////
//State Operation
/////////////////////////////////////////////////////////////////

VOID CINWH::Operation()
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

VOID CINWH::RegisterVariables()
{
	try
	{
		// -------------------------------------------------------
		// PPI9000Stn Variable and Function
		// -------------------------------------------------------

		// -------------------------------------------------------
		// CWorkHolder Variable and Function
		// -------------------------------------------------------


	}
	catch (CAsmException e)
	{
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		DisplayException(e);
		DisplayMessage("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}
}
