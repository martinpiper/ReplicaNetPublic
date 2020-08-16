#include "s3e.h"
#include "s3eThread.h"
#include <assert.h>
#include "pthread.h"
#include "unistd.h"
#include "RNPlatform/Inc/SysTime.h"
#include "RNXPURL/Inc/TransportConfig.h"
#include "RNXPURL/Inc/NetworkEmulation.h"
#include "RNPlatform/Inc/SysTime.h"
#include "RNPlatform/Inc/PlatformHeap.h"
#include "RNPlatform/Inc/PlatformInfo.h"
#include "RNXPSession/Inc/XPSession.h"
#include "RNLobby/Inc/RNLobby.h"
#include "RNLobby/Inc/BackendClient.h"
#include "RNLobby/Inc/AdvertiseClient.h"

// This tests the connection to the RNLobby debug server and uses that to advertise game sessions
#define TEST_LOBBY

// This forces the XPSession::Poll() method to be called every frame and ignore the threading model set for the device.
// Some mobile platforms do not have very good scheduling so a forced poll is sometimes needed to make the network responsive.
//#define FORCE_POLLING

// The name to use when advertising the session.
#define EXAMPLE_NAME "MarmaladeExample2"


using namespace RNReplicaNet;

void fillScreen(uint8 r, uint8 g, uint8 b)
{
	uint16* screen = (uint16*)s3eSurfacePtr();
	uint16 colour = (uint16)s3eSurfaceConvertRGB(r, g, b);
	int32 width = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	int32 height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	int32 pitch = s3eSurfaceGetInt(S3E_SURFACE_PITCH);

	int y;
	int x;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			screen[y * pitch / 2 + x] = colour;
		}
	}
}

class MyAdvertiseClient : public RNReplicaNet::RNLobby::AdvertiseClient
{
public:
	virtual void CallbackListingsEntry(const int serverIndex,const int hostLoginID,const char *url, const char *userData, const float age, const XPAddress &address)
	{
		if (strstr(url,EXAMPLE_NAME))
		{
			THREADSAFELOCK();
			mGoodServerFound = url;
		}
	}

	std::string GetGoodServerFound(void)
	{
		THREADSAFELOCK();
		return mGoodServerFound;
	}

private:
	std::string mGoodServerFound;
};

void DebugWait(const char *message)
{
	int frame = 0;

	while (!s3eDeviceCheckQuitRequest())
	{
		fillScreen(0, 0, 0);
		s3eDebugPrint(0, 16, message, 0);
		char text[128];
		sprintf( text , "Press the screen/pointer (frame %d)" , frame++);
		s3eDebugPrint(0, 128, text, 0);
		s3eSurfaceShow();

		s3eDeviceYield(0);
		s3eKeyboardUpdate();
		s3ePointerUpdate();

		if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_PRESSED)
		{
			fillScreen(0, 0, 0);
			s3eSurfaceShow();
			return;
		}

		if (s3eKeyboardAnyKey())
		{
			break;
		}
	}

	fillScreen(0, 0, 0);
	s3eDebugPrint(0, 16, "Need to exit", 0);
	s3eSurfaceShow();
	exit(-1);
}

int main()
{
	s3eDebugTraceLine("in main");
	fillScreen(0, 0, 0);
	s3eDebugPrint(0, 16, "Please wait...", 0);
	s3eSurfaceShow();
	s3eDeviceYield(0);

#if 0
	// Configure some emulated packet loss to test network stability
	NetworkEmulation::SetPacketLoss(10.0f);
	NetworkEmulation::SetAverageLatency(0.100f);
	NetworkEmulation::SetJitter(0.050f);
	NetworkEmulation::SetEnabled(true);
#endif

#ifdef TEST_LOBBY
	if (!PlatformInfo::IsThreaded())
	{
		DebugWait("Device should be multi-threaded, but isn't");
	}

	//	fillScreen(100, 0, 0);
	//	s3eSurfaceShow();

	RNReplicaNet::RNLobby::BackendClient backend;
	// Use the example 12 backend information for now.
	backend.Start(12,"Example12Key");
	//	backend.Start(0,"");	// Use locally hosted servers
	while(!backend.GetCompleted())
	{
		printf(".");
		RNReplicaNet::CurrentThread::Sleep(100);
	}
	if (!RNReplicaNet::RNLobby::BackendClient::IsAuthenticated())
	{
		DebugWait("Backend should be authenticated but isn't");
	}

	// The BackendClient is authenticated so it is now the correct time register the NATUDP transport
	RNReplicaNet::RNLobby::RegisterDefaultTransports();

	fillScreen(0, 100, 0);
	s3eSurfaceShow();

	MyAdvertiseClient advertise1;
	advertise1.Start();

	// Try to get some listing from the advertise server
	advertise1.GetListings();
#endif


	std::string debug;

	XPSession *session = XPSession::Allocate();
#ifdef FORCE_POLLING
	// We will manually poll these
	session->SetManualPoll();
	session->SetManualPollXPSession();
	session->SetManualPollXPURL();
#else
	if (PlatformInfo::IsThreaded())
	{
		// When XPSession polls we don't want it to call the poll method for the layers below.
		session->SetPollLayerBelow(false);
	}
#endif

#ifndef TEST_LOBBY
	// Start a LAN session find
	session->Find();
#endif

	int iter = 0;
	bool created = false;
	float lastTick = 0.0f;
	int findTicks = 0;

	SysTime theTime2;
	float lastFrame = (float) theTime2.FloatTime() - 0.1f;

	SysTime toSendWhen;
	while (!s3eDeviceCheckQuitRequest())
	{
		lastTick = (float) theTime2.FloatTime();
		if (created)
		{
			// Just discard any late replies
			std::string found = session->EnumerateFound();
		}
		else
		{
#ifndef TEST_LOBBY
			// Do a LAN session find, do several finds just to be sure.
			if ((theTime2.FloatTime() > 0.4f) && (findTicks == 0))
			{
				findTicks++;
				session->Find();
			}
			if ((theTime2.FloatTime() > 0.8f) && (findTicks == 1))
			{
				findTicks++;
				session->Find();
			}
			if ((theTime2.FloatTime() > 1.2f) && (findTicks == 2))
			{
				findTicks++;
				session->Find();
			}
			if ((theTime2.FloatTime() > 1.6f) && (findTicks == 3))
			{
				findTicks++;
				session->Find();
			}

#endif
#ifdef TEST_LOBBY
			if (theTime2.FloatTime() < 2.0f)
			{
				std::string found = advertise1.GetGoodServerFound();
				if (!found.empty())
				{
					// If the advertise server returned a viable session then join it
					debug += "Joining "+found+"\n";
					session->Join(found);
					created = true;
				}
			}
			else
			{
				// No viable session found so create one instead
				session->SetGameChannel(2000);
				session->Create(EXAMPLE_NAME,"NATUDP@");
				debug += "Created "+session->ExportURL()+"\n";
				created = true;
			}
#else
			if (theTime2.FloatTime() < 2.0f)
			{
				std::string found = session->EnumerateFound();
				if (found.find(EXAMPLE_NAME) != std::string::npos)
				{
					// If a session name containing EXAMPLE_NAME was found then join it
					debug += "Joining "+found+"\n";
					session->Join(found);
					created = true;
				}
			}
			else
			{
				// No viable session found so create one instead
				session->Create(EXAMPLE_NAME);
				debug += "Created "+session->ExportURL()+"\n";
				created = true;
			}
#endif
		}
#ifdef FORCE_POLLING
		session->Poll();
#else
		if (!PlatformInfo::IsThreaded())
		{
			session->Poll();
		}
#endif

		lastTick = (float) theTime2.FloatTime() - lastTick;

#ifdef TEST_LOBBY
		if (session->IsMaster())
		{
			// Advertise the session URL with the advertise server.
			// We do this here because the URL can change over time depending on NAT resolution state of the device.
			advertise1.AdvertiseSession(session->ExportURL().c_str(),"Some user data for this example");
		}
#endif

		fillScreen(0, 0, 0);

		// Frame tick calculation
		float now = (float) theTime2.FloatTime();
		float delta = now - lastFrame;
		lastFrame = now;

		// Frame tick
		if (toSendWhen.FloatTime() > 0.1f)
		{
			// Send some data every 0.1 of a second to a random-ish session ID
			toSendWhen.Reset();
			const char *someData = "ReplicaNet and RNLobby are brilliant!";
			session->DataSendToSessionUnreliable(rand()&3,someData,strlen(someData));
		}
		// Get any delivered packets
		char buffer[1024];
		int fromSessionID;
		int length;
		while (	session->DataReceive(&fromSessionID,buffer,&length) )
		{
			buffer[length] = 0;
			printf("Got '%s' from sessionID %d\n",buffer,fromSessionID);
		}

		// Draw debug and other Marmalade stuff
		iter++;
		sprintf(buffer,"it%d t%.2f frm%dms thrd%d",iter,theTime2.FloatTime(),(int)(lastTick * 1000.0f),(int)PlatformInfo::IsThreaded());
		s3eDebugPrint(0, 16, buffer, 0);
		int psent,precv;
		session->GetMergedStatistics(&psent,&precv);
		sprintf(buffer,"MID%d ID%d s%.2f r%.2f ms%d mr%d pl%d ltm%dms",session->GetMasterSessionID(),session->GetSessionID(),
			session->GetNetworkSendRate(),session->GetNetworkReceiveRate(),psent,precv,
			session->GetNetworkPacketsLost(),(int)(session->GetLatencyToMasterSession()*1000.0f)
			);

		s3eDebugPrint(0, 32, buffer, 0);

		s3eDebugPrint(0, 48, debug.c_str(), true);

		s3eSurfaceShow();
		s3eDeviceYield(0);
		s3eKeyboardUpdate();
		s3ePointerUpdate();

		if (s3eKeyboardAnyKey())
			break;
	}

#ifdef TEST_LOBBY
	// Stop advertising
	advertise1.StopAdvertiseSession();
	advertise1.Stop();
#endif

	// Gracefully disconnect the ReplicaNet session.
	// This gives the packet layer beneath ReplicaNet some time to send packets before the process exits.
	SysTime finish;
	session->Disconnect();
	while (finish.FloatTime() < 1.0f)
	{
		fillScreen(0, 0, 0);
		iter++;
		char buffer[500];
		sprintf(buffer,"iter = %d time %f",iter,theTime2.FloatTime());
		s3eDebugPrint(0, 0, buffer, 0);
		s3eDebugPrint(0, 30, "Disconnecting", false);
		s3eSurfaceShow();
		s3eDeviceYield(0);

#ifdef FORCE_POLLING
		session->Poll();
#else
		if (!PlatformInfo::IsThreaded())
		{
			session->Poll();
		}
#endif
	}

	delete session;

	fillScreen(0, 0, 0);
	s3eDebugPrint(0, 30, "Exiting!", false);
	s3eSurfaceShow();
	s3eDeviceYield(0);

	PlatformHeap::ForceFree();

	return 0;
}
