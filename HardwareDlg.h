/////////////////////////////////////////////////////////////////
//	Hardware.h : header file for Dialog box used to configure
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

#pragma once


/////////////////////////////////////////////////////////////////////////////
// CHardwareDlg dialog
/////////////////////////////////////////////////////////////////////////////

class CHardwareDlg : public CDialog
{
public:
	CHardwareDlg(CWnd *pParent = NULL);

	// Dialog Data
	//{{AFX_DATA(CHardwareDlg)
	enum { IDD = IDD_HARDWARE };
	BOOL	m_bHardware;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardwareDlg)
protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CHardwareDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedHwSelection();

	BOOL	m_bPRS;
	BOOL	m_bTempCtrl;
	BOOL	m_bDiagnMode;
	BOOL	m_bPickArm;
	BOOL	m_bInspWH;
	BOOL	m_bShBufferWH;
	BOOL	m_bInspOpt;
	BOOL	m_bIsAppRunning;

	BOOL	m_bRejectArm;
	BOOL	m_bRejectBelt;
	BOOL	m_bWH1;
	BOOL	m_bWH2;
	BOOL	m_bInPickArm;
	BOOL	m_bOutPickArm;
	BOOL	m_bINWH;

	CButton m_ctrHardware; //20120402
	CButton m_ctrPRS;
	CButton m_ctrDiagnMode;
	CButton m_ctrPICKARM;
	CButton m_ctrINSPWH;
	CButton m_ctrInspOpt;
	CButton m_ctrShBufferWH;
	CButton m_ctrRejectArm;
	CButton m_ctrRejectBelt;
	CButton m_ctrWH1;
	CButton m_ctrWH2;
	CButton m_ctrInPickArm;
	CButton m_ctrOutPickArm;
	CButton m_ctrINWH;

	afx_msg void OnBnClickedDiagnModeSel();
	afx_msg void OnBnClickedOk();
};
