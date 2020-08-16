/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
// ChatDlg.cpp : implementation file
// The chat screen dialog
//

#include "stdafx.h"
#include "Example5.h"
#include "ChatDlg.h"

using namespace RNReplicaNet;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog


CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDlg)
	DDX_Control(pDX, IDC_LIST1, mUsersList);
	DDX_Control(pDX, IDC_EDIT1, mChatEdit);
	DDX_Control(pDX, IDC_RICHTEXTCTRL1, mChatWindow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	//{{AFX_MSG_MAP(CChatDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_WHISPER, OnWhisper)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers

BOOL CChatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	mXPSession = ((CExample5App *)AfxGetApp())->mXPSession;

	mIsStable = false;
	mError = false;

	ShowWindow(SW_SHOW);
	ShowWindow(SW_RESTORE);
	ShowWindow(SW_SHOWDEFAULT);

	SetForegroundWindow();

	SetTimer(1,100,0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CChatDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int sessionid;
	bool change = false;

	char buffer[512];

	while ( (sessionid = mXPSession->GetJoiner()) != kXPSessionUnknownID)
	{
		char buffer[64];
		sprintf(buffer,"%d:",sessionid);
		mUsersList.AddString(buffer);

		mChatText += std::string("SessionID '")+std::string(buffer)+std::string("' Joined\n");
		change = true;

		strcpy(buffer+4,mName);
		((int *)buffer)[0] = kChatCommand_Name;
		mXPSession->DataSendToSession(sessionid,buffer,strlen(buffer+4)+4+1,true);
	}

	while ( (sessionid = mXPSession->GetLeaver()) != kXPSessionUnknownID)
	{
		char buffer[64];
		sprintf(buffer,"%d:",sessionid);
		int sel = mUsersList.FindString(0,buffer);
		if (sel != LB_ERR)
		{
			char full[64];
			mUsersList.GetText(sel,full);
			mUsersList.DeleteString(sel);
			mChatText += std::string("User '")+std::string(full)+std::string("' Left\n");
			change = true;
		}
		else
		{
			mChatText += std::string("Unknown sessionID ");
			mChatText += std::string(buffer);
			mChatText += std::string(" Left\n");
			change = true;
		}
	}

	if (!mIsStable)
	{
		if (mXPSession->IsStable())
		{
			mIsMaster = mXPSession->IsMaster();

			mIsStable = true;
			mChatText.erase();
			if (mXPSession->IsMaster())
			{
				mChatText += std::string("New server started for you '")+std::string(mName)+std::string("'\n");
				mChatText += mXPSession->ExportURL();
				change = true;
			}
			else
			{
				mChatText += std::string("Server joined for you '")+std::string(mName)+std::string("'\n");
				change = true;
			}

			mChatText += std::string("Session params:");
			if (mXPSession->GetCanBecomeMaster())
			{
				mChatText += std::string(" Can become master.");
			}
			if (mXPSession->GetCanSpider())
			{
				mChatText += std::string(" Can spider.");
			}
			mChatText += std::string("\n");

			if (mIsMaster)
			{
				mChatText += std::string("Session: Is master\n");
			}
			else
			{
				mChatText += std::string("Session: Is not master\n");
			}
		}
	}

	if (mXPSession->GetStatus() == XPSession::kXPSession_EERROR && !mError)
	{
		mError = true;
		mChatText += std::string("The master session left and this session is not able to become a master. Chat has ended.\n");
		mUsersList.EnableWindow(false);
	}

	if (mIsMaster != mXPSession->IsMaster())
	{
		mChatText += std::string("Session state changed\n");
		mIsMaster = mXPSession->IsMaster();
		if (mIsMaster)
		{
			mChatText += std::string("Session: Is master\n");
		}
		else
		{
			mChatText += std::string("Session: Is not master\n");
		}
		change = true;
	}

	int length;
	while (mXPSession->DataReceive(&sessionid,buffer,&length))
	{
		int command = ((int *)buffer)[0];
		switch(command)
		{
			case kChatCommand_Name:
			{
				char tempbuffer[64];
				sprintf(tempbuffer,"%d:",sessionid);
				int sel = mUsersList.FindString(0,tempbuffer);
				if (sel != LB_ERR)
				{
					mUsersList.DeleteString(sel);
					sprintf(tempbuffer,"%d:%s",sessionid,buffer+4);
					mUsersList.InsertString(sel,tempbuffer);
					sprintf(tempbuffer,"SessionID %d is now known as '%s'\n",sessionid,buffer+4);
					mChatText += std::string(tempbuffer);
					change = true;
				}				
				break;
			}
			case kChatCommand_Chat:
			{
				char tempbuffer[256];
				sprintf(tempbuffer,"%d:",sessionid);
				int sel = mUsersList.FindString(0,tempbuffer);
				if (sel != LB_ERR)
				{
					char full[64];
					mUsersList.GetText(sel,full);
					sprintf(tempbuffer,"'%s' Said '%s'\n",full,buffer+4);
					mChatText += std::string(tempbuffer);
					change = true;
				}				
				break;
			}
			case kChatCommand_Whisper:
			{
				char tempbuffer[256];
				sprintf(tempbuffer,"%d:",sessionid);
				int sel = mUsersList.FindString(0,tempbuffer);
				if (sel != LB_ERR)
				{
					char full[64];
					mUsersList.GetText(sel,full);
					sprintf(tempbuffer,"'%s' Whispered '%s'\n",full,buffer+4);
					mChatText += std::string(tempbuffer);
					change = true;
				}				
				break;
			}
			default:
				break;
		}
	}


	if (change)
	{
		mChatWindow.SetText(mChatText.c_str());
	}

	CDialog::OnTimer(nIDEvent);
}

void CChatDlg::OnWhisper() 
{
	int sel;
	if ( (sel = mUsersList.GetCurSel()) == LB_ERR)
	{
		return;
	}
	char name[64];
	mUsersList.GetText(sel,name);
	char buffer[256];
	int sessionid;
	mChatEdit.GetWindowText(buffer+4,sizeof(buffer)-4);
	sscanf(name,"%d:",&sessionid);
	((int *)buffer)[0] = kChatCommand_Whisper;
	mXPSession->DataSendToSession(sessionid,buffer,strlen(buffer+4)+4+1,true);

	mChatText += std::string("Whisper to '")+std::string(name)+std::string("' this: '")+std::string(buffer+4)+std::string("'\n");

	mChatWindow.SetText(mChatText.c_str());
}

void CChatDlg::OnChangeEdit1() 
{
}

void CChatDlg::OnButton1() 
{
	char buffer[256];
	mChatEdit.GetWindowText(buffer+4,sizeof(buffer)-4);
	((int *)buffer)[0] = kChatCommand_Chat;
	mXPSession->DataSendToSession(kXPSessionBroadcastID,buffer,strlen(buffer+4)+4+1,true);

	mChatText += std::string("To everyone '")+std::string(buffer+4)+std::string("'\n");

	mChatWindow.SetText(mChatText.c_str());
}
