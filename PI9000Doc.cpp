/////////////////////////////////////////////////////////////////
//	PI9000Doc.cpp : implementation of the CPI9000Doc class
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

#include "PI9000Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////
// CPI9000Doc
/////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPI9000Doc, CDocument)


BEGIN_MESSAGE_MAP(CPI9000Doc, CDocument)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////
// CPI9000Doc construction/destruction
/////////////////////////////////////////////////////////////////

CPI9000Doc::CPI9000Doc()
{
	// TODO: add one-time construction code here

}

CPI9000Doc::~CPI9000Doc()
{
}

BOOL CPI9000Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


/////////////////////////////////////////////////////////////////
// CPI9000Doc serialization
/////////////////////////////////////////////////////////////////

void CPI9000Doc::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


/////////////////////////////////////////////////////////////////
// CPI9000Doc diagnostics
/////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CPI9000Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPI9000Doc::Dump(CDumpContext &dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////
// CPI9000Doc commands
/////////////////////////////////////////////////////////////////
