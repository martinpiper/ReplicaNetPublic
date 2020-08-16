/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This allocates a new ReplicaNet session and also handles the dialog displayed at the start of the application.
The dialog displays a list of found game session for the user to join or allows a new game session to be created.
*/
#include <windows.h>
#include "Network.h"
#include "Resource.h"
#include "RNXPURL/Inc/NetworkEmulation.h"

using namespace RNReplicaNet;

extern HINSTANCE ghInst;
ReplicaNet *gNetwork;

static bool failed_to = true;

static void CreateGame(void)
{
	std::string url;

//	gNetwork->SetGameChannel(2000);
//	gNetwork->SessionCreate("Example1","COMPUDP@");
	gNetwork->SessionCreate("Example1");
	url = gNetwork->SessionExportURL();
	OutputDebugString(url.c_str());
	OutputDebugString("\n");
}

void Network_Init(std::string url)
{
	RNReplicaNet::NetworkEmulation::SetEnabled(true);

	// Allocate a new ReplicaNet session
	gNetwork = new ReplicaNet;
	// Turn on manual Poll() updates for the session to give us slightly better network prediction performance
	gNetwork->SetManualPoll();
	// Disconnect ReplicaNet polling from the session and transport threads.
	gNetwork->SetPollLayerBelow(false);
	gNetwork->SetPollLayerBelowForXPSession(false);


	// Did we have a URL on the command line?
	if (url != "")
	{
		// Try the URL on the command line
		gNetwork->SessionJoin(url);
	}
	else
	{
		// display the network picking dialog
		Network_Dialog();

		if (failed_to)
		{
			gNetwork->SessionFind();
			// Wait a while
			Sleep(500);
			url = gNetwork->SessionEnumerateFound();

			if (url.find("Example1") == std::string::npos)
			{
				url = "";
			}

			if (url != "")
			{
				char buffer[256];
				sprintf(buffer,"Trying to join %s\n",url.c_str());
				OutputDebugString(buffer);
				gNetwork->SessionJoin(url);
			}
			else
			{
				CreateGame();
			}
		}
	}
}

INT_PTR CALLBACK Dlg_JoinOrNew(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::string url;

	switch (msg)
	{
		case WM_INITDIALOG:
			SetTimer(hwndDlg,1,500,0);
			SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_RESETCONTENT, 0, 0);

			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);

//			SetWindowText (GetDlgItem (hwndDlg, IDC_NEWMAPBLOCKWIDTH), textbuf);
			SetFocus(GetDlgItem(hwndDlg,IDC_NEW));
			return 0;
			break;
		case WM_TIMER:
			do
			{
				url = gNetwork->SessionEnumerateFound();
				if (url.find("Example1") == std::string::npos)
				{
					url = "";
				}

				if (url != "")
				{
					SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM) url.c_str());
				}
			} while (url != "");
			break;
		case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_LIST2:
			{
				int sel = SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_GETCURSEL,0,0);
				if (sel != LB_ERR)
				{
					char buffer[256];
					int ret = SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_GETTEXT,sel,(LPARAM) buffer);
					if (ret != LB_ERR)
					{
						SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT1),buffer);
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_SETCURSEL,sel,0);

				}
				break;
			}
			case IDC_EDIT1:
				SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_SETCURSEL, -1, 0);
				break;
			case IDC_REFRESH:
				SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_RESETCONTENT, 0, 0);
				gNetwork->SessionFind();
				break;
			case IDC_JOIN:
			{
				int sel = SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_GETCURSEL,0,0);
				if (sel != LB_ERR)
				{
					char buffer[256];
					int ret = SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_GETTEXT,sel,(LPARAM) buffer);
					if (ret != LB_ERR)
					{
						gNetwork->SessionJoin(std::string(buffer));
						failed_to = false;
						SendMessage (hwndDlg, WM_CLOSE, 0, -1L);
						break;
					}
				}
				char buffer[256];
				int ret = GetWindowText(GetDlgItem(hwndDlg,IDC_EDIT1),buffer,sizeof(buffer));
				if (ret > 0)
				{
					gNetwork->SessionJoin(std::string(buffer));
					failed_to = false;
					SendMessage (hwndDlg, WM_CLOSE, 0, -1L);
					break;
				}
				break;
			}
			case IDC_NEW:
			case IDOK:
				CreateGame();

				failed_to = false;
				SendMessage (hwndDlg, WM_CLOSE, 0, -1L);
				break;
			case IDCANCEL:
				SendMessage (hwndDlg, WM_CLOSE, 0, -1L);
				break;
		}
		break;
		case WM_CLOSE:
			EndDialog (hwndDlg, lParam);
			KillTimer(hwndDlg,1);
			break;
		default:
			return 0;
	}

	return 1;
}

void Network_Dialog(void)
{
	DialogBox(ghInst,MAKEINTRESOURCE(IDD_JOINORNEW),0,&Dlg_JoinOrNew);
}
