/////////////////////////////////////////////////////////////////
//	PI9000View.cpp : implementation of the CPI9000View class
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
#include "PI9000View.h"
#include "MarkConstant.h"
#include "PI9000_Constant.h"
#include "StringEntryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////
//	Constant Definitions
/////////////////////////////////////////////////////////////////

const INT gnMAX_MESSAGE_LINES		= 1000;


/////////////////////////////////////////////////////////////////
// CPI9000View
/////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPI9000View, CFormView)


BEGIN_MESSAGE_MAP(CPI9000View, CFormView)
	ON_BN_CLICKED(IDC_GROUP_ID, OnGroupId)
	ON_BN_CLICKED(IDC_AUTO_START, OnAutoStart)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_SELECT_HARDWARE, OnSelectHardware)
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	ON_BN_CLICKED(IDC_LOG_MSG, OnLogMsg)
	ON_BN_CLICKED(IDC_EXEC_HMI, OnExecuteHmi)
	ON_LBN_SELCHANGE(IDC_MSG_LIST, OnLbnSelchangeMsgList)
	ON_BN_CLICKED(IDC_MODULE_NAME, OnBnClickedModuleName)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////
// CPI9000View construction/destruction
/////////////////////////////////////////////////////////////////

CPI9000View::CPI9000View()
	: CFormView(CPI9000View::IDD)
{
	// TODO: add construction code here
}

CPI9000View::~CPI9000View()
{
}

void CPI9000View::DoDataExchange(CDataExchange *pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_LIST, m_lbMessage);
	DDX_Control(pDX, IDC_GROUP_ID, m_btnGroupId);
	DDX_Control(pDX, IDC_MODULE_NAME, m_btnModule);
	DDX_Control(pDX, IDC_AUTO_START, m_btnAutoStart);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_LOG_MSG, m_btnLogMessage);
	DDX_Control(pDX, IDC_EXEC_HMI, m_btnExecHmi);
}

BOOL CPI9000View::PreCreateWindow(CREATESTRUCT &cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

void CPI9000View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CPI9000App *pApp = (CPI9000App*) AfxGetApp();
	m_btnGroupId.SetWindowText(pApp->GetGroupID());

	if (pApp->AutoStart())
	{
		m_btnAutoStart.SetCheck(1);
		m_btnStart.EnableWindow(FALSE);
	}
	else
	{
		m_btnAutoStart.SetCheck(0);
	}

	m_btnLogMessage.SetCheck(pApp->LogMessage());
	m_btnExecHmi.SetCheck(pApp->IsExecuteHmi());
}


/////////////////////////////////////////////////////////////////
// CPI9000View diagnostics
/////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CPI9000View::AssertValid() const
{
	CFormView::AssertValid();
}

void CPI9000View::Dump(CDumpContext &dc) const
{
	CFormView::Dump(dc);
}

CPI9000Doc *CPI9000View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPI9000Doc)));
	return (CPI9000Doc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CPI9000View message handlers
/////////////////////////////////////////////////////////////////////////////

void CPI9000View::OnGroupId() 
{
#if 1 //20120320 lock groupID
	CPI9000App *pApp	= (CPI9000App*) AfxGetApp();
	if (pApp->State() != UN_INITIALIZE_Q)
	{
		CString szMessage = _T("Group ID cannot be changed while appln is Initialized");
		DisplayMessage(szMessage);

		return;
	}
	pApp->SetGroupId("COG");
	m_btnGroupId.SetWindowText("COG");
	DisplayMessage("Set Group ID to COG");
#else
	CStringEntryDlg				dlg;
	CPI9000App *pApp	= (CPI9000App*) AfxGetApp();

	if (pApp->State() != UN_INITIALIZE_Q)
	{
		CString szMessage = _T("Group ID cannot be changed while appln is Initialized");
		DisplayMessage(szMessage);

		return;
	}
	
	dlg.m_szTitle = _T("Group ID");
	m_btnGroupId.GetWindowText(dlg.m_szString);

	if (dlg.DoModal() == IDOK)
	{
		pApp->SetGroupId(dlg.m_szString);
		m_btnGroupId.SetWindowText(dlg.m_szString);
	}
#endif
}

void CPI9000View::OnAutoStart() 
{
	CPI9000App *pApp = (CPI9000App*) AfxGetApp();
	INT nState = m_btnAutoStart.GetCheck();
	
	BOOL bState = FALSE;

	if (nState == 1)
	{
		bState = TRUE;
	}

	pApp->SaveAutoStart(bState);

	if ((nState == 1) && (pApp->State() == UN_INITIALIZE_Q))
	{
		m_btnStart.EnableWindow(FALSE);
		pApp->SetRun();
	}
}

void CPI9000View::OnStart() 
{
	CPI9000App *pApp = (CPI9000App*) AfxGetApp();

	m_btnStart.EnableWindow(FALSE);
	pApp->m_isAppRunning = TRUE; //20111214
	pApp->SetRun();
}

void CPI9000View::OnSelectHardware() 
{
	CPI9000App *pApp = (CPI9000App*) AfxGetApp();
	if (pApp->m_isAppRunning) //20111214
	{
		pApp->SelectHardware(FALSE);
	}
	else
	{
		pApp->SelectHardware(TRUE);
	}
}

void CPI9000View::OnOptions() 
{
	CPI9000App *pApp = (CPI9000App*) AfxGetApp();
	if (pApp->m_isAppRunning) //20111214
	{
		pApp->SetOptions(FALSE);
	}
	else
	{
		pApp->SetOptions(TRUE);
	}
}

void CPI9000View::OnLogMsg() 
{
	CPI9000App *pApp	= (CPI9000App*) AfxGetApp();
	INT nState					= m_btnLogMessage.GetCheck();
	BOOL bState					= FALSE;

	if (nState == 1)
	{
		bState = TRUE;
	}

	pApp->SetLogMessage(bState);
}

void CPI9000View::OnExecuteHmi()
{
	CPI9000App *pApp	= (CPI9000App*) AfxGetApp();
	INT nState					= m_btnExecHmi.GetCheck();
	BOOL bState					= FALSE;

	if (nState == 1)
	{
		bState = TRUE;
	}

	pApp->SetExecuteHmiFlag(bState);
}

VOID CPI9000View::SetGroupID(CString &szGroupID)
{
	m_btnGroupId.SetWindowText(szGroupID);
}

VOID CPI9000View::SetModule(CString &szModule)
{
	m_btnModule.SetWindowText(szModule);
}


/////////////////////////////////////////////////////////////////////////////
// Message Display
/////////////////////////////////////////////////////////////////////////////

VOID CPI9000View::DisplayMessage(const CString &szMessage)
{
	INT nIndex = m_lbMessage.AddString(szMessage);
	m_lbMessage.PostMessage(LB_SETCURSEL, nIndex, 0);
	m_lbMessage.PostMessage(LB_SETCURSEL, -1, 0);

	if (m_lbMessage.GetCount() > gnMAX_MESSAGE_LINES)
	{
		m_lbMessage.DeleteString(0);
	}
}

void CPI9000View::OnLbnSelchangeMsgList()
{
	// TODO: Add your control notification handler code here
}

void CPI9000View::OnBnClickedModuleName()
{
	// TODO: Add your control notification handler code here
}
