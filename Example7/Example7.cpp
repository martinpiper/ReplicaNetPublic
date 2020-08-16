/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "RNXPSession/Inc/XPSession.h"
#include "RNPlatform/Inc/ThreadClass.h"
#include "RNPlatform/Inc/PlatformInfo.h"
#include "RNXPURL/Inc/XPURL.h"

using namespace RNReplicaNet;

char buffer_classcreate[]="This is a typcial class create message which can be quite large and contain a number of bytes";
char buffer_datablock[]="A data block message which is small";

// This forces the session to be polled a few times if the program is not operating with multi-threading
// If multi-threading is active then it is not absolutely needed to force a poll
static void ForcePoll(XPSession *aSession)
{
	// If the platform is not multithreaded then call the XPSession::Poll() function a few times
	if (!PlatformInfo::IsThreaded())
	{
		int i;
		for (i=0;i<20;i++)
		{
			aSession->Poll();
		}
	}
}

// The example starts here
int main(int argc,char **argv)
{
	XPSession *aSession;

	aSession = XPSession::Allocate();

	aSession->SetCanSpider(true);
	aSession->SetCanBecomeMaster(true);
	aSession->SetEncryption(true);

	printf("Searching for a session\n");

	int normalsessid = aSession->GetSessionID();

	aSession->Find();

	ForcePoll(aSession);

	/* Pause for a while for any other sessions to respond. A game can do anything while waiting for replies. */
	CurrentThread::Sleep(2000);

	ForcePoll(aSession);

	std::string resfind;

	do
	{
		resfind = aSession->EnumerateFound();
		if (resfind=="")
		{
			break;
		}
		printf("Found a session '%s'\n",resfind.c_str());
		// If a found session string contains 'Example7' then we have found a session to join.
		if (resfind.find("Example7") != std::string::npos)
		{
			break;
		}
	} while (resfind != "");

	if (resfind != "")
	{
		printf("Trying to join the session '%s'\n",resfind.c_str());
		aSession->Join(resfind);
	}
	else
	{
		printf("Find session failed to find a suitable session\n");
		aSession->Create("Example7");
	}

	aSession->Poll();

	std::string url = aSession->ExportURL();

	printf("My URL is '%s'\n",url.c_str());

	aSession->Poll();

	while(aSession->GetStatus() == XPSession::kXPSession_EOK)
	{
		aSession->Poll();

		if (normalsessid != aSession->GetSessionID())
		{
			normalsessid = aSession->GetSessionID();
			printf("\nI am now session ID %d\n",normalsessid);
			aSession->DataSendToSession(0,buffer_classcreate,sizeof(buffer_classcreate),true);
		}

		if (aSession->IsMaster())
		{
			printf("MasterSession ID = %d time %f Latency %f packets resent %d\r",aSession->GetSessionID(),aSession->GetTime(),aSession->GetLatencyToMasterSession(),aSession->GetNetworkPacketsLost());
		}
		else
		{
			printf("Session ID = %d time %f Latency %f packets resent %d\r",aSession->GetSessionID(),aSession->GetTime(),aSession->GetLatencyToMasterSession(),aSession->GetNetworkPacketsLost());
		}

		// Now send some packets randomly
		// With the 'unreliable' method
		if ( (rand() & 3) == 0)
		{
			aSession->DataSendToSessionUnreliable(rand()&3,buffer_classcreate,sizeof(buffer_classcreate));
		}
		if ( (rand() & 3) == 0)
		{
			aSession->DataSendToSessionUnreliable(rand()&3,buffer_datablock,sizeof(buffer_datablock));
		}

		// With the 'certain' method
		if ( (rand() & 3) == 0)
		{
			aSession->DataSendToSessionCertain(rand()&3,buffer_classcreate,sizeof(buffer_classcreate));
		}
		if ( (rand() & 3) == 0)
		{
			aSession->DataSendToSessionCertain(rand()&3,buffer_datablock,sizeof(buffer_datablock));
		}

		// With the 'reliable' method
		if ( (rand() & 3) == 0)
		{
			aSession->DataSendToSessionReliable(rand()&3,buffer_classcreate,sizeof(buffer_classcreate));
		}
		if ( (rand() & 3) == 0)
		{
			aSession->DataSendToSessionReliable(rand()&3,buffer_datablock,sizeof(buffer_datablock));
		}

		// Now check for packet data to arrive
		char data[512];
		int len;
		int sessionid;
		unsigned char type;
		bool gotPacket = false;
		while(aSession->DataReceive(&sessionid,data,&len,&type))
		{
			data[len] = 0;
			printf("\nGot %d bytes from %d with type %d",len,sessionid,(int)type);
			gotPacket = true;
		}
		// Produce a tidy output if we got a packet this time
		if (gotPacket)
		{
			printf("\n");
		}

		// Also check for session ID numbers joining or leaving the session
		int sessid;
		while ( (sessid = aSession->GetJoiner()) != kXPSessionUnknownID)
		{
			printf("\nSession %d Joined\n\n",sessid);
		}
		while ( (sessid = aSession->GetLeaver()) != kXPSessionUnknownID)
		{
			printf("\nSession %d Left\n\n",sessid);
		}

		CurrentThread::Sleep(100);

		// Quit the session after 20 seconds local time
		if (aSession->GetLocalTime() > 20.0f)
		{
			break;
		}
	}

	delete aSession;
	printf("\nQuit the test\n");

	return 0;
}
