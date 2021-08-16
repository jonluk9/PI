/////////////////////////////////////////////////////////////////
//	WH1.cpp : interface of the CWH1 class
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


class CWH1 : public CInspWH
{
	DECLARE_DYNCREATE(CWH1)

protected:
	

protected:
	//Update
	virtual VOID RegisterVariables();

private:
	virtual CString GetStation() 
	{
		return "WH1_";
	}

public:
	CWH1();
	virtual	~CWH1();

	virtual	BOOL InitInstance();
	virtual	INT	ExitInstance();

	VOID Operation();
};
