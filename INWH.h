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

#pragma once

#include "WorkHolder.h"


class CINWH : public CWorkHolder
{
	DECLARE_DYNCREATE(CINWH)

protected:
	

protected:
	//Update
	virtual VOID RegisterVariables();

private:
	virtual CString GetStation() 
	{
		return "INWH_";
	}

public:
	CINWH();
	virtual	~CINWH();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();
};
