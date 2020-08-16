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

#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "RNXPURL/Inc/XPURL.h"
#include "RNPlatform/Inc/PlatformHeap.h"
#include "RNPlatform/Inc/ThreadClass.h"

using namespace RNReplicaNet;

extern HINSTANCE ghInst;

static bool failed_to = true;

static bool canBeMaster = true;
static bool canSpider = true;
static bool canAcceptMigrationObjects = true;
static ReplicaNet::OnError onError = ReplicaNet::kMigrateObjects;

MyReplicaNet *Network::mNetwork = 0;
bool Network::mDedicatedServer = false;
bool Network::mRenderModels = true;
bool Network::mDoRecord = false;
bool Network::mDoPlayback = false;
std::string Network::mRecordingFileName;

// Windows dialog variables
static OPENFILENAME ofn;
static TCHAR sFileName[MAX_PATH];

static void CreateGame(void)
{
	std::string url;

	// Lets set it so that our latency is recalculated a bit quicker just to show we can. :)
	Network::mNetwork->SetLatencyRecalculationDelay(0.5f);

	Network::mNetwork->SetCanBecomeMaster(canBeMaster);
	Network::mNetwork->SetCanSpider(canSpider);
	Network::mNetwork->SetCanAcceptObjects(canAcceptMigrationObjects);

	Network::HandleRecording();
	Network::mNetwork->SessionCreate("Example4");
	url = Network::mNetwork->SessionExportURL();
	OutputDebugString(url.c_str());
	OutputDebugString("\n");
}

// This utility function starts recording or playback if it is configured.
void Network::HandleRecording(void)
{
	// If recording or playback is enabled then configure ReplicaNet
	if (Network::mDoRecord && Network::mRecordingFileName != "")
	{
		FILE *fp = fopen(Network::mRecordingFileName.c_str(),"wb");
		if (fp)
		{
			Network::mNetwork->SetRecordingFilePointer(fp);
			Network::mNetwork->BeginSessionRecord();
		}
	}

	if (Network::mDoPlayback && Network::mRecordingFileName != "")
	{
		FILE *fp = fopen(Network::mRecordingFileName.c_str(),"rb");
		if (fp)
		{
			Network::mNetwork->SetRecordingFilePointer(fp);
			Network::mNetwork->BeginSessionPlayback();
			// We are playing back a recording, so do not try to join or create a game.
			return;
		}
	}
}
void Network::Network_Init(std::string url)
{
	Network::mNetwork = new MyReplicaNet;
	// Allow direct control over when DataBlock updates are applied.
	Network::mNetwork->SetDataBlockUpdatePolicy(false);
	// Allow the ReplicaNet/XPsession/XPURL threads to use automatic scheduling.
	Network::mNetwork->SetPollLayerBelow(false);
	Network::mNetwork->SetPollLayerBelowForXPSession(false);
	Network::mNetwork->SetAutomaticPoll();
	Network::mNetwork->SetAutomaticPollXPSession();
	Network::mNetwork->SetAutomaticPollXPURL();

	if (url != "")
	{
		Network::mNetwork->SessionJoin(url);
	}
	else
	{
		Network::Network_Dialog();

		if (failed_to)
		{
			Network::mNetwork->SetCanBecomeMaster(canBeMaster);
			Network::mNetwork->SetCanSpider(canSpider);
			Network::mNetwork->SetCanAcceptObjects(canAcceptMigrationObjects);

			Network::mNetwork->SessionFind();
			// Wait a while
			Sleep(500);
			url = Network::mNetwork->SessionEnumerateFound();

			// Check for our game name so we get the right game session in the list
			if (url.find("Example4") == std::string::npos)
			{
				url = "";
			}

			if (url != "")
			{
				char buffer[256];
				sprintf(buffer,"Trying to join %s\n",url.c_str());
				OutputDebugString(buffer);
				HandleRecording();
				Network::mNetwork->SessionJoin(url);
			}
			else
			{
				CreateGame();
			}
		}
	}
}

// Windows code to handle the create/join dialog.
static INT_PTR CALLBACK Dlg_JoinOrNew(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::string url;

	switch (msg)
	{
		case WM_INITDIALOG:
			SetTimer(hwndDlg,1,500,0);
			SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_RESETCONTENT, 0, 0);

			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK1), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK2), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK3), BM_SETCHECK, BST_CHECKED, 0);

			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK4), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK6), BM_SETCHECK, BST_UNCHECKED, 0);

			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK7), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK8), BM_SETCHECK, BST_CHECKED, 0);

			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK9), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndDlg, IDC_CHECK10), BM_SETCHECK, BST_UNCHECKED, 0);

			EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT2),FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON2),FALSE);

// This is the address of a test machine which lives behind a DSL connection and firewall that sometimes runs this example application.
// It can be set to display this address in the dialog to make it easier to test the multiplayer code across a realistic internet connection.
//			SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT1),"SESSION://UDP@217.39.30.241:2000/");

			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);

			SetFocus(GetDlgItem(hwndDlg,IDC_NEW));

			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = 0;
			ofn.lpstrFilter = "ReplicaNet recording\0*.repnet\0All Files\0*.*\0\0";
			ofn.lpstrFile = sFileName;
			sFileName[0] =0;
			ofn.nMaxFile = sizeof(sFileName)/sizeof(sFileName[0]);
			return 0;
			break;
		case WM_TIMER:
			do
			{
				url = Network::mNetwork->SessionEnumerateFound();
				// Check for our game name so we get the right game session in the list
				if (url.find("Example4") == std::string::npos)
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
			case IDC_CHECK1:
			{
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK1), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					canSpider = true;
				}
				else
				{
					canSpider = false;
				}
				break;
			}
			case IDC_CHECK2:
			{
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK2), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					canBeMaster = true;
				}
				else
				{
					canBeMaster = false;
				}
				break;
			}
			case IDC_CHECK3:
			{
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK3), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					canAcceptMigrationObjects = true;
				}
				else
				{
					canAcceptMigrationObjects = false;
				}
				break;
			}

			case IDC_CHECK4:
			{
				onError = ReplicaNet::kMigrateObjects;

				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK4), BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK5), BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK6), BM_SETCHECK, BST_UNCHECKED, 0);
				break;
			}

			case IDC_CHECK5:
			{
				onError = ReplicaNet::kDeleteObjects;

				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK4), BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK5), BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK6), BM_SETCHECK, BST_UNCHECKED, 0);
				break;
			}

			case IDC_CHECK6:
			{
				onError = ReplicaNet::kLeaveObjects;

				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK4), BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK5), BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(GetDlgItem(hwndDlg, IDC_CHECK6), BM_SETCHECK, BST_CHECKED, 0);
				break;
			}

			case IDC_CHECK7:
			{
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK7), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					Network::mDedicatedServer = true;
				}
				else
				{
					Network::mDedicatedServer = false;
				}
				break;
			}

			case IDC_CHECK8:
			{
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK8), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					Network::mRenderModels = true;
				}
				else
				{
					Network::mRenderModels = false;
				}
				break;
			}

			case IDC_CHECK9:
			{
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK9), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					Network::mDoRecord = true;
					Network::mDoPlayback = false;
					SendMessage(GetDlgItem(hwndDlg, IDC_CHECK10), BM_SETCHECK, BST_UNCHECKED, 0);

					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT2),TRUE);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON2),TRUE);
				}
				else
				{
					Network::mDoRecord = false;
					Network::mDoPlayback = false;
					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT2),FALSE);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON2),FALSE);
				}
				break;
			}
			case IDC_CHECK10:
			{
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK10), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					Network::mDoRecord = false;
					Network::mDoPlayback = true;
					SendMessage(GetDlgItem(hwndDlg, IDC_CHECK9), BM_SETCHECK, BST_UNCHECKED, 0);

					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT2),TRUE);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON2),TRUE);
				}
				else
				{
					Network::mDoRecord = false;
					Network::mDoPlayback = false;
					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT2),FALSE);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON2),FALSE);
				}
				break;
			}
			case IDC_BUTTON2:
			{
				ofn.Flags = OFN_EXPLORER | OFN_LONGNAMES;
				int value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK9), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
//					ofn.Flags |= OFN_CREATEPROMPT;
					if (GetSaveFileName(&ofn))
					{
						if (strstr(sFileName,".") == 0)
						{
							strcat(sFileName,".repnet");
						}
						Network::mRecordingFileName = sFileName;
						SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT2),sFileName);
					}
				}
				value = SendMessage(GetDlgItem(hwndDlg,IDC_CHECK10), BM_GETCHECK ,0, 0);
				if (value == BST_CHECKED)
				{
					ofn.Flags |= OFN_FILEMUSTEXIST;
					if (GetOpenFileName(&ofn))
					{
						Network::mRecordingFileName = sFileName;
						SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT2),sFileName);
						SendMessage (hwndDlg, WM_CLOSE, 0, -1L);
					}
				}
				break;
			}


			case IDC_EDIT1:
				SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_SETCURSEL, -1, 0);
				break;
			case IDC_REFRESH:
				SendMessage(GetDlgItem(hwndDlg, IDC_LIST2), LB_RESETCONTENT, 0, 0);
				Network::mNetwork->SessionFind();
				break;
			case IDC_JOIN:
			{
				Network::mNetwork->SetCanBecomeMaster(canBeMaster);
				Network::mNetwork->SetCanSpider(canSpider);
				Network::mNetwork->SetCanAcceptObjects(canAcceptMigrationObjects);
				char buffer[256];
				int ret = GetWindowText(GetDlgItem(hwndDlg,IDC_EDIT1),buffer,sizeof(buffer));
				if (ret > 0)
				{
					Network::HandleRecording();
					Network::mNetwork->SessionJoin(std::string(buffer));
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

void Network::Network_Dialog(void)
{
	DialogBox(ghInst,MAKEINTRESOURCE(IDD_JOINORNEW),0,&Dlg_JoinOrNew);
}

void Network::Network_Shutdown(void)
{
	// This demonstrates the nice way to disconnect from a network session
	// None of this is strictly required by ReplicaNet because using delete on the ReplicaNet class also disconnects,
	// however it does perform a very graceful disconnect.

	// First disconnect
	mNetwork->Disconnect();

	// After deleting ReplicaNet it is not a good idea to use ReplicaNet functions on ReplicaObject derived classes
	delete mNetwork;
	mNetwork = 0;

	// We can shutdown the network here. Not strictly necessary but it demonstrates the graceful shutdown procedure.
	RNReplicaNet::XPURL::ShutdownNetwork();

	// This force frees all remaining, one time allocated memory, from the ReplicaNet library. This memory usually exists
	// for the lifetime of the application.
	RNReplicaNet::PlatformHeap::ForceFree();
}

