/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
// Example5.h : main header file for the EXAMPLE5 application
//

#if !defined(AFX_EXAMPLE5_H__24F84513_48FB_447D_85BA_BC5FC204464B__INCLUDED_)
#define AFX_EXAMPLE5_H__24F84513_48FB_447D_85BA_BC5FC204464B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "RNXPSession/Inc/XPSession.h"


/////////////////////////////////////////////////////////////////////////////
// CExample5App:
// See Example5.cpp for the implementation of this class
//

const int kChatCommand_Name = 0;
const int kChatCommand_Chat = 1;
const int kChatCommand_Whisper = 2;

class CExample5App : public CWinApp
{
public:
	CExample5App();

	RNReplicaNet::XPSession *mXPSession;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExample5App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CExample5App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//	DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLE5_H__24F84513_48FB_447D_85BA_BC5FC204464B__INCLUDED_)
