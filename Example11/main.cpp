/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This example shows the use of networked member functions in classes
*/
#include "RNPlatform/Inc/MemoryTracking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "RNPlatform/Inc/PlatformHeap.h"
#include "RNXPURL/Inc/XPURL.h"
#include "RNLobby/Inc/BackendClient.h"
#include "RNLobby/Inc/NATDetectClient.h"
#include "RNLobby/Inc/AdvertiseClient.h"
#include "RNLobby/Inc/RNLobby.h"
#include "Test1.h"

using namespace RNReplicaNet;

static void RunTest(void);

int main(int argc,char **argv)
{
	RunTest();

	// Force free all internal memory and threads.
	PlatformHeap::ForceFree();

	return 0;
}

// Extend NATDetectClient to access the callback we are interested in.
class NATDetectClient : public RNLobby::NATDetectClient
{
	void CallbackCompleted(void)
	{
		switch (GetReply())
		{
			case RNLobby::NATDetectServer::kReply_Timeout:
				printf("NATDetectClient: kReply_Timeout\n");
				return;
				break;
			case RNLobby::NATDetectServer::kReply_Success:
				printf("NATDetectClient: kReply_Success\n");
				break;
		}
		switch (GetNATType())
		{
			case kOpen:
				printf("NATDetectClient: NAT Type is Open\n");
				break;
			case kModerate:
				printf("NATDetectClient: NAT Type is Moderate\n");
				break;
			default:
			case kStrict:
				printf("NATDetectClient: NAT Type is Strict\n");
				break;
		}
	}
};

// Extend AdvertiseClient to access the callbacks we are interested in.
class MyAdvertiseClient : public RNLobby::AdvertiseClient
{
public:
	volatile bool mListingCompleted;
	std::list<std::string> mURLsFound;
	void CallbackListingsEntry(const int serverIndex,const int hostLoginID,const char *url, const char *userData, const float age, const XPAddress &address)
	{
		// Ignore any replies that are not complete
		if (!url)
		{
			return;
		}
		printf("AdvertiseClient: CallbackListingsEntry with '%s' from addr %s\n",url,address.Export().c_str());
		// Save each URL and discard the rest of the data since it isn't used.
		mURLsFound.push_back(url);
	}

	void CallbackListingsBegin(void)
	{
		printf("AdvertiseClient: CallbackListingsBegin\n");
	}

	void CallbackListingsComplete(void)
	{
		printf("AdvertiseClient: CallbackListingsComplete\n");
		mListingCompleted = true;
	}

	void CallbackDisconnected(Transport *transport)
	{
		printf("AdvertiseClient: CallbackDisconnected\n");
	}
};

static void RunTest(void)
{
	// RNLobby - Authenticate with the Backend server.
	// Before any other RNLobby client classes are started this authentication must complete.
	printf("Authenticate with BackendServer\n");
	// Without this backend check enabled this example will need localhost servers running.
#if 1
	RNLobby::BackendClient backend;
	backend.UseReleaseServers(true);
	// TitleID 11 and "Example11Key" corresponds to this example only.
	// When using BackendClient in your own development and test applications use TitleID 1002 and key "ngjfyt64ert" instead.
	// In a real world application it is a good idea to obfuscate the encryption key.
	backend.Start(11,"Example11Key");

	// Wait for any reply before continuing.
	while (!backend.GetCompleted())
	{
		printf(".");
		CurrentThread::Sleep(100);
	}
	printf("\n");
	// Display suitable error messages for the possible replies.
	if (backend.GetReply() == RNLobby::BackendServer::kReply_InvalidTitleID)
	{
		printf("The titleID is invalid.\n");
		return;
	}
	if (backend.GetReply() == RNLobby::BackendServer::kReply_TitleIDSupportPeriodExpired)
	{
		printf("The titleID support period has expired.\n");
		return;
	}
	if (backend.GetReply() == RNLobby::BackendServer::kReply_ServerDownForMaintanence)
	{
		printf("The server is down for maintanence, please try again later.\n");
		return;
	}
	if (backend.GetReply() == RNLobby::BackendServer::kReply_ServerCannotBeContacted)
	{
		printf("The server cannot be contacted, please check your internet settings and try again.\n");
		return;
	}
#endif
	// Must have received a kReply_ValidTitleID reply so continue to use other RNLobby functions.

	// Start detecting the type of NAT used for out internet connection.
	// This step is not strictly required but it may be of interest to users who want to test their NAT/Firewall.
	// Like most RNLobby services this is asynchronous so it can be left to complete.
	NATDetectClient natDetect;
	natDetect.Start();

	// Start the connection to the advertise server. This is then used to search for or advertise sessions.
	MyAdvertiseClient advertise;
	advertise.Start();

	// At this point a ReplicaNet instance can be allocated and objects published if required.
	// If using the NATUDP transport then call RNLobby::RegisterDefaultTransports() before allocating a ReplicaNet instance.
	RNLobby::RegisterDefaultTransports();
	// The ReplicaNet instance has not been attached to any session yet until we get the result of querying the session server.
	ReplicaNet *network;
	// Allocate a ReplicaNet session
	network = new ReplicaNet();
	// Now allocate our test object
	Test1 *object;
	object = new Test1();

	// Publish the object on to the network
	object->Publish();

	// Get a list of sessions from the server. Any returned sessions are only for this TitleID.
	printf("Getting list of sessions\n");
	advertise.mListingCompleted = false;
	advertise.mURLsFound.clear();
	advertise.GetListings();
	while (!advertise.mListingCompleted)
	{
		printf(".");
		CurrentThread::Sleep(100);
	}
	printf("\n");
	if (advertise.mURLsFound.empty())
	{
		// No sessions found so create one
		network->SessionCreate("Example11","NATUDP@");
		printf("New game started '%s'\n",network->SessionExportURL().c_str());
	}
	else
	{
		// Pick the first session in the list
		std::string found = *advertise.mURLsFound.begin();
		// Join the found session
		printf("Joining session '%s'\n",found.c_str());
		network->SessionJoin(found);
	}

	// Go around in a loop for a while
	int i;
	for (i=0;i<50;i++)
	{
		// Keep on advertising the session if we are the session master, the URL can change over time as the NAT resolution continues working.
		// This will also advertise the new session host if this ReplicaNet instance becomes the session master.
		if (network->IsMaster())
		{
			advertise.AdvertiseSession(network->SessionExportURL().c_str());
		}

		if (i % 2 == 0)
		{
			network->ObjectListBeginIterate();
			ReplicaObject *objects;
			while ( (objects = network->ObjectListIterate()) != 0)
			{
				printf("ReplicaObject %d:%d:%d\n",objects->GetSessionID(),objects->GetUniqueID(),(int)objects->IsMaster());
				// Double check that this ReplicaObject is of a type Test1
				if (objects->GetClassID() == Test1::StaticGetClassID())
				{
					Test1 *realObject = (Test1 *) objects;

					realObject->ReportTest1();
				}
			}
			network->ObjectListFinishIterate();
		}

		// Sleep for one second
		CurrentThread::Sleep(1000);
	}

	// Stop the advertise of this session
	advertise.StopAdvertiseSession();

	// Demonstrate how to shutdown ReplicaNet
	// First disconnect this ReplicaNet instance from the network session.
	network->Disconnect();

	// Tidy any game objects allocated and registered to ReplicaNet.
	ReplicaObject *robject;
	network->ObjectListBeginIterate();
	while ( (robject = network->ObjectListIterate()) != 0)
	{
		delete robject;
	}
	network->ObjectListFinishIterate();

	// Delete the ReplicaNet instance which stops the ReplicaNet threads
	delete network;

	// XPURL threads and other internal threads are still running at this point.

	// The RNLobby services will go out of scope when the function returns and this will stop the threads associated with those services.
}
