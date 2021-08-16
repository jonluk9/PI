/////////////////////////////////////////////////////////////////
//	Hardware.cpp : implementation file for Dialog box used to configure
//		the controller hardware in the PI9000 
//		application
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
#include "resource.h"
#include "HardwareDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CHardwareDlg dialog
/////////////////////////////////////////////////////////////////////////////

CHardwareDlg::CHardwareDlg(CWnd *pParent) : CDialog(CHardwareDlg::IDD, pParent)
	, m_bPRS(false)
	, m_bDiagnMode(false)
	, m_bWH1(false)
	, m_bWH2(false)
	, m_bInspOpt(false)
	, m_bIsAppRunning(false)
	, m_bShBufferWH(false)
	, m_bRejectBelt(false)
	, m_bInPickArm(false)
	, m_bOutPickArm(false)
{
	//{{AFX_DATA_INIT(CHardware)
	m_bHardware = FALSE;
	//}}AFX_DATA_INIT
}

void CHardwareDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardwareDlg)
	DDX_Check(pDX, IDC_HW_SELECTION, m_bHardware);
	DDX_Check(pDX, IDC_PRS_SEL, m_bPRS);
	DDX_Check(pDX, IDC_DIAGN_MODE_SEL, m_bDiagnMode);
	DDX_Check(pDX, IDC_WH1_SEL, m_bWH1);
	DDX_Check(pDX, IDC_WH2_SEL, m_bWH2);
	DDX_Check(pDX, IDC_INSP_OPT_SEL, m_bInspOpt);
	DDX_Check(pDX, IDC_SHBUFFER_WH_SEL, m_bShBufferWH);
	DDX_Check(pDX, IDC_REJECTBELT_SEL, m_bRejectBelt);
	DDX_Check(pDX, IDC_INPICKARM_SEL, m_bInPickArm);
	DDX_Check(pDX, IDC_OUTPICKARM_SEL, m_bOutPickArm);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_HW_SELECTION, m_ctrHardware);
	DDX_Control(pDX, IDC_PRS_SEL, m_ctrPRS);
	DDX_Control(pDX, IDC_DIAGN_MODE_SEL, m_ctrDiagnMode);
	DDX_Control(pDX, IDC_WH1_SEL, m_ctrWH1);
	DDX_Control(pDX, IDC_WH2_SEL, m_ctrWH2);
	DDX_Control(pDX, IDC_INSP_OPT_SEL, m_ctrInspOpt);
	DDX_Control(pDX, IDC_SHBUFFER_WH_SEL, m_ctrShBufferWH);
	DDX_Control(pDX, IDC_REJECTBELT_SEL, m_ctrRejectBelt);
	DDX_Control(pDX, IDC_INPICKARM_SEL, m_ctrInPickArm);
	DDX_Control(pDX, IDC_OUTPICKARM_SEL, m_ctrOutPickArm);
}

BOOL CHardwareDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (!m_bHardware || m_bIsAppRunning)
	{
		if (!m_bIsAppRunning) //20121114
		{
			m_bDiagnMode = FALSE;
			m_ctrDiagnMode.EnableWindow(FALSE);

			m_bWH1			= FALSE;
			m_bWH2			= FALSE;
			m_bInspOpt		= FALSE;
			m_bShBufferWH	= FALSE;
			m_bRejectBelt	= FALSE;
			m_bInPickArm	= FALSE;
			m_bOutPickArm	= FALSE;
		}

		if (m_bIsAppRunning) //20120402
		{
			m_ctrDiagnMode.EnableWindow(FALSE);
			m_ctrHardware.EnableWindow(FALSE);
			m_ctrPRS.EnableWindow(FALSE);
		}
		m_ctrWH1.EnableWindow(FALSE);
		m_ctrWH2.EnableWindow(FALSE);
		m_ctrInspOpt.EnableWindow(FALSE);
		m_ctrDiagnMode.EnableWindow(FALSE);
		m_ctrShBufferWH.EnableWindow(FALSE);
		m_ctrRejectBelt.EnableWindow(FALSE);
		m_ctrInPickArm.EnableWindow(FALSE);
		m_ctrOutPickArm.EnableWindow(FALSE);
	}
	else
	{
		if (m_bDiagnMode)
		{
			m_ctrWH1.EnableWindow(FALSE);
			m_ctrWH2.EnableWindow(FALSE);
			m_ctrInspOpt.EnableWindow(FALSE);

			m_ctrShBufferWH.EnableWindow(FALSE);
			m_ctrRejectBelt.EnableWindow(FALSE);
			m_ctrInPickArm.EnableWindow(FALSE);
			m_ctrOutPickArm.EnableWindow(FALSE);
		}
		else
		{
			m_ctrWH1.EnableWindow(TRUE);
			m_ctrWH2.EnableWindow(TRUE);
			m_ctrInspOpt.EnableWindow(TRUE);
			m_ctrShBufferWH.EnableWindow(TRUE);
			m_ctrRejectBelt.EnableWindow(TRUE);
			m_ctrInPickArm.EnableWindow(TRUE);
			m_ctrOutPickArm.EnableWindow(TRUE);
		}
	}

	UpdateData(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CHardwareDlg, CDialog)
	//{{AFX_MSG_MAP(CHardwareDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HW_SELECTION, OnBnClickedHwSelection)
	ON_BN_CLICKED(IDC_DIAGN_MODE_SEL, OnBnClickedDiagnModeSel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHardwareDlg message handlers
/////////////////////////////////////////////////////////////////////////////

void CHardwareDlg::OnBnClickedHwSelection()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_bHardware)
	{
		m_bWH1		        = TRUE;
		m_bWH2		        = TRUE;
		m_bInspOpt		    = TRUE;
		m_bShBufferWH		= TRUE;
		m_bRejectBelt	    = TRUE;
		m_bInPickArm	    = TRUE;
		m_bOutPickArm	    = TRUE;
		m_bDiagnMode		= FALSE;

		m_ctrWH1.EnableWindow(TRUE);
		m_ctrWH2.EnableWindow(TRUE);
		m_ctrInspOpt.EnableWindow(TRUE);
		m_ctrDiagnMode.EnableWindow(TRUE);
		m_ctrShBufferWH.EnableWindow(TRUE);
		m_ctrRejectBelt.EnableWindow(TRUE);
		m_ctrInPickArm.EnableWindow(TRUE);
		m_ctrOutPickArm.EnableWindow(TRUE);
	}
	else
	{
		m_bWH1		        = FALSE;
		m_bWH1		        = FALSE;
		m_bWH2		        = FALSE;
		m_bInspOpt			= FALSE;
		m_bShBufferWH		= FALSE;
		m_bRejectBelt		= FALSE;
		m_bInPickArm	    = FALSE;
		m_bOutPickArm	    = FALSE;
		m_bDiagnMode		= FALSE;

		m_ctrWH1.EnableWindow(FALSE);
		m_ctrWH2.EnableWindow(FALSE);
		m_ctrInspOpt.EnableWindow(FALSE);
		m_ctrDiagnMode.EnableWindow(FALSE);

		m_ctrShBufferWH.EnableWindow(FALSE);
		m_ctrRejectBelt.EnableWindow(FALSE);
		m_ctrInPickArm.EnableWindow(FALSE);
		m_ctrOutPickArm.EnableWindow(FALSE);
	}

	UpdateData(FALSE);

}

void CHardwareDlg::OnBnClickedDiagnModeSel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// diagn mode enabled
	if (m_bDiagnMode)
	{
		m_bWH1		        = TRUE;
		m_bWH2		        = TRUE;
		m_bInspOpt			= TRUE;
		m_bShBufferWH		= TRUE;
		m_bRejectBelt		= TRUE;
		m_bInPickArm	    = TRUE;
		m_bOutPickArm	    = TRUE;

		m_ctrWH1.EnableWindow(FALSE);
		m_ctrWH2.EnableWindow(FALSE);
		m_ctrInspOpt.EnableWindow(FALSE);
		m_ctrShBufferWH.EnableWindow(FALSE);
		m_ctrRejectBelt.EnableWindow(FALSE);
		m_ctrInPickArm.EnableWindow(FALSE);
		m_ctrOutPickArm.EnableWindow(FALSE);
	}
	else
	{
		m_bWH1		        = TRUE;
		m_bWH2		        = TRUE;
		m_bInspOpt			= TRUE;
		m_bShBufferWH		= TRUE;
		m_bRejectBelt		= TRUE;
		m_bInPickArm	    = TRUE;
		m_bOutPickArm	    = TRUE;

		m_ctrWH1.EnableWindow(TRUE);
		m_ctrWH2.EnableWindow(TRUE);
		m_ctrInspOpt.EnableWindow(TRUE);
		m_ctrShBufferWH.EnableWindow(TRUE);
		m_ctrRejectBelt.EnableWindow(TRUE);
		m_ctrInPickArm.EnableWindow(TRUE);
		m_ctrOutPickArm.EnableWindow(TRUE);
	}
}

void CHardwareDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


