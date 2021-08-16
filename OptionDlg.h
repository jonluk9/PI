/////////////////////////////////////////////////////////////////
//	OptionDlg.h : header file for Dialog box used to configure
//		the option selections in the PI9000
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
// COptionDlg dialog
/////////////////////////////////////////////////////////////////////////////

class COptionDlg : public CDialog
{
public:
	COptionDlg(CWnd *pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_SELECT_OPTIONS };
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedVer1McSelection();
	afx_msg void OnBnClickedVer2McSelection();
	afx_msg void OnBnClickedVer3McSelection();
//	afx_msg void OnBnClickedEcoreSelection();
	afx_msg void OnBnClickedAdamSelection();
	
	BOOL m_bSelectVer1;
	BOOL m_bSelectVer2;
	BOOL m_bSelectVer3;
	BOOL m_bSelectLitec;
	BOOL m_bSelectADAM;
	BOOL m_bSelectHeaterOn;	// 20140905 Yip
	//BOOL m_bSelectEdgeCleaner;
	BOOL m_bSelectCOG902;
	BOOL m_bSelectTA4Camera;
	BOOL m_bSelectNewPBContactSnr; //20141201
	BOOL m_bIsAppRunning; //20120402
	BOOL m_bSelectCreateErrorLog;	// 20150331
	BOOL m_bSelectTeflonEncoder;	// TeflonEncoder
	BOOL m_bSelect120mmFOV;	// 120mmFOV

	CButton m_ctrSelectVer1Mc;
	CButton m_ctrSelectVer2Mc;
	CButton m_ctrSelectVer3Mc;
	CButton m_ctrSelectLitec;
	CButton m_ctrSelectADAM;
	CButton m_ctrSelectHeaterOn;	// 20140905 Yip
	CButton m_ctrSelectNewPBContactSnr; //20141201
	CButton m_ctrSelectEdgeCleaner;
	CButton m_ctrSelectTA4Camera;
	CButton m_ctrSelectCreateErrorLog;	// 20150331
	CButton m_ctrOK; //20120402
	CButton m_ctrSelectTeflonEncoder;	// TeflonEncoder
	CButton m_ctrSelect120mmFOV;	// 120mmFOV

	afx_msg void OnBnClickedLitecSelection();
	afx_msg void OnBnClickedEdgeCleanerSelection();
	afx_msg void OnBnClickedEdgeCleanerSelection2();
	afx_msg void OnBnClickedTa4CameraSelection();
	afx_msg void OnBnClickedHeaterOnSelection();
	afx_msg void OnBnClickedNewPBContactSnrSelection(); //20141201
	afx_msg void OnBnClickedCreateErrorLogSelection();
	afx_msg void OnBnClickedTeflonEncoderSelection();	// TeflonEncoder
	afx_msg void OnBnClicked120mmFOVSelection();	// 120mmFOV
};
