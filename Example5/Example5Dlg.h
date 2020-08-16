/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
// Example5Dlg.h : header file
//

#if !defined(AFX_EXAMPLE5DLG_H__67DD6E7B_B15D_4169_BE0D_0D9DAF405951__INCLUDED_)
#define AFX_EXAMPLE5DLG_H__67DD6E7B_B15D_4169_BE0D_0D9DAF405951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNXPSession/Inc/XPSession.h"

/////////////////////////////////////////////////////////////////////////////
// CExample5Dlg dialog

class CExample5Dlg : public CDialog
{
// Construction
public:
	CExample5Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExample5Dlg)
	enum { IDD = IDD_EXAMPLE5_DIALOG };
	CButton	m_AllowSpider;
	CButton	m_CanBeMaster;
	CEdit	mChatHandle;
	CEdit	mJoinEdit;
	CListBox	mServerListControl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExample5Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExample5Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnSelchangeList1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	RNReplicaNet::XPSession *mXPSession;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLE5DLG_H__67DD6E7B_B15D_4169_BE0D_0D9DAF405951__INCLUDED_)
