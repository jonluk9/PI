/////////////////////////////////////////////////////////////////
// PI9000View.h : interface of the CPI9000View class
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


class CPI9000View : public CFormView
{
protected: // create from serialization only
	CPI9000View();

	DECLARE_DYNCREATE(CPI9000View)

public:
	enum { IDD = IDD_PI9000_FORM };

	CListBox m_lbMessage;
	CButton	 m_btnGroupId;
	CButton	 m_btnModule;
	CButton	 m_btnAutoStart;
	CButton	 m_btnStart;
	CButton	 m_btnLogMessage;
	CButton  m_btnExecHmi;

// Attributes
public:
	CPI9000Doc *GetDocument() const;

// Operations
public:
	VOID DisplayMessage(const CString &szMessage);
	VOID SetGroupID(CString &szGroupID);
	VOID SetModule(CString &szGroupID);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);

protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CPI9000View();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

// Generated message map functions
protected:
	afx_msg void OnGroupId();
	afx_msg void OnAutoStart();
	afx_msg void OnStart();
	afx_msg void OnSelectHardware();
	afx_msg void OnOptions();
	afx_msg void OnLogMsg();
	afx_msg void OnExecuteHmi();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeMsgList();
	afx_msg void OnBnClickedModuleName();
};

#ifndef _DEBUG  // debug version in PI9000View.cpp
inline CPI9000Doc *CPI9000View::GetDocument() const
{ 
	return reinterpret_cast<CPI9000Doc*>(m_pDocument); 
}
#endif
