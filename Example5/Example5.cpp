/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
// Example5.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Example5.h"
#include "Example5Dlg.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace RNReplicaNet;


/////////////////////////////////////////////////////////////////////////////
// CExample5App

BEGIN_MESSAGE_MAP(CExample5App, CWinApp)
	//{{AFX_MSG_MAP(CExample5App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//	DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExample5App construction

CExample5App::CExample5App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExample5App object

CExample5App theApp;

/////////////////////////////////////////////////////////////////////////////
// CExample5App initialization

BOOL CExample5App::InitInstance()
{
	AfxEnableControlContainer();

	mXPSession = XPSession::Allocate();

#if _MFC_VER < 0x0700
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif
	// Open the server select dialog
	CExample5Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	delete mXPSession;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
