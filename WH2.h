/////////////////////////////////////////////////////////////////
//	WH2.cpp : interface of the CWH2 class
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

#include "InspWH.h"


class CWH2 : public CInspWH
{
	DECLARE_DYNCREATE(CWH2)

protected:
	

protected:
	//Update
	virtual VOID RegisterVariables();

private:
	virtual CString GetStation() 
	{
		return "WH2_";
	}

public:
	CWH2();
	virtual	~CWH2();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();
};
