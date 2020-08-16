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
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "RNLobby/Inc/RNLobby.h"
#include "RNLobby/Inc/BackendClient.h"
#include "RNLobby/Inc/AdvertiseClient.h"
#include <set>
#include "Player.h"
#include "Enemy.h"

// This tests the connection to the RNLobby debug server and uses that to advertise game sessions
#define TEST_LOBBY

// This forces the ReplicaNet::Poll() method to be called every frame and ignore the threading model set for the device.
// Some mobile platforms do not have very good scheduling so a forced poll is sometimes needed to make the network responsive.
//#define FORCE_POLLING

// The name to use when advertising the session.
#define EXAMPLE_NAME "MarmaladeExample1"


using namespace RNReplicaNet;

void DrawLine(int x1, int y1, int x2, int y2, int16 color)
{
	int16* ptr = (int16*)s3eSurfacePtr();
	int32 pitch = s3eSurfaceGetInt(S3E_SURFACE_PITCH)/2;
	int ix, iy;
	// if x1 == x2 or y1 == y2, then it does not matter what we set here
	int delta_x = (x2 > x1?(ix = 1, x2 - x1):(ix = -1, x1 - x2)) << 1;
	int delta_y = (y2 > y1?(iy = 1, y2 - y1):(iy = -1, y1 - y2)) << 1;


	int sw = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	int sh = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

	if (y1 > 0 && y1 < sh && x1 >0 && x1 < sw)
		ptr[y1*pitch+x1] = color;


	if (delta_x >= delta_y)
	{
		// error may go below zero
		int error = delta_y - (delta_x >> 1);

		while (x1 != x2)
		{
			if (error >= 0)
			{
				if (error || (ix > 0))
				{
					y1 += iy;
					error -= delta_x;
				}
				// else do nothing
			}
			// else do nothing

			x1 += ix;
			error += delta_y;

			if (y1 > 0 && y1 < sh && x1 >0 && x1 < sw)
				ptr[y1*pitch+x1] = color;
		}
	}
	else
	{
		// error may go below zero
		int error = delta_x - (delta_y >> 1);

		while (y1 != y2)
		{
			if (error >= 0)
			{
				if (error || (iy > 0))
				{
					x1 += ix;
					error -= delta_y;
				}
				// else do nothing
			}
			// else do nothing

			y1 += iy;
			error += delta_x;

			if (y1 > 0 && y1 < sh && x1 >0 && x1 < sw)
				ptr[y1*pitch+x1] = color;
		}
	}
}

void DrawLineRGB(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	DrawLine(x1,y1,x2,y2,(uint16)s3eSurfaceConvertRGB(r,g,b));
}

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
	fillScreen(0, 0, 0);
	s3eDebugPrint(0, 16, message, 0);
	s3eDebugPrint(0, 128, "Press the screen/pointer", 0);
	s3eSurfaceShow();

	while (!s3eDeviceCheckQuitRequest())
	{
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

	ReplicaNet *session = new ReplicaNet;
#ifdef FORCE_POLLING
	// We will manually poll these
	session->SetManualPoll();
	session->SetManualPollXPSession();
	session->SetManualPollXPURL();
#else
	if (PlatformInfo::IsThreaded())
	{
		// When ReplicaNet polls we don't want it to call the poll method for the layers below.
		session->SetPollLayerBelow(false);
		session->SetPollLayerBelowForXPSession(false);
	}
#endif
	// We will choose the best time to update our data blocks by calling ProcessDataBlockUpdate(), not during ReplicaNet's thread.
	session->SetDataBlockUpdatePolicy(false);

#ifndef TEST_LOBBY
	// Start a LAN session find
	session->SessionFind();
#endif

	Player *player = new Player;
	player->Publish();

	int numEnemyToSpawn = (rand() & 3) + 1;
	while(numEnemyToSpawn--)
	{
		Enemy *enemy = new Enemy;
		enemy->Publish();
	}

	int iter = 0;
	bool created = false;
	float lastTick = 0.0f;
	int findTicks = 0;

	SysTime theTime2;
	float lastFrame = (float) theTime2.FloatTime() - 0.1f;
	while (!s3eDeviceCheckQuitRequest())
	{
		lastTick = (float) theTime2.FloatTime();
		if (created)
		{
			// Just discard any late replies
			std::string found = session->SessionEnumerateFound();
		}
		else
		{
#ifndef TEST_LOBBY
			// Do a LAN session find, do several finds just to be sure.
			if ((theTime2.FloatTime() > 0.4f) && (findTicks == 0))
			{
				findTicks++;
				session->SessionFind();
			}
			if ((theTime2.FloatTime() > 0.8f) && (findTicks == 1))
			{
				findTicks++;
				session->SessionFind();
			}
			if ((theTime2.FloatTime() > 1.2f) && (findTicks == 2))
			{
				findTicks++;
				session->SessionFind();
			}
			if ((theTime2.FloatTime() > 1.6f) && (findTicks == 3))
			{
				findTicks++;
				session->SessionFind();
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
					session->SessionJoin(found);
					created = true;
				}
			}
			else
			{
				// No viable session found so create one instead
				session->SetGameChannel(2000);
				session->SessionCreate(EXAMPLE_NAME,"NATUDP@");
				debug += "Created "+session->SessionExportURL()+"\n";
				created = true;
			}
#else
			if (theTime2.FloatTime() < 2.0f)
			{
				std::string found = session->SessionEnumerateFound();
				if (found.find(EXAMPLE_NAME) != std::string::npos)
				{
					// If a session name containing EXAMPLE_NAME was found then join it
					debug += "Joining "+found+"\n";
					session->SessionJoin(found);
					created = true;
				}
			}
			else
			{
				// No viable session found so create one instead
				session->SessionCreate(EXAMPLE_NAME);
				debug += "Created "+session->SessionExportURL()+"\n";
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
		session->ProcessDataBlockUpdate();

		lastTick = (float) theTime2.FloatTime() - lastTick;

#ifdef TEST_LOBBY
		if (session->IsMaster())
		{
			// Advertise the session URL with the advertise server.
			// We do this here because the URL can change over time depending on NAT resolution state of the device.
			advertise1.AdvertiseSession(session->SessionExportURL().c_str(),"Some user data for this example");
		}
#endif

		fillScreen(0, 0, 0);

		// Frame tick calculation
		float now = (float) theTime2.FloatTime();
		float delta = now - lastFrame;
		lastFrame = now;

		// Frame object tick
		session->ObjectListBeginIterate();
		ReplicaObject *object;
		while ((object = session->ObjectListIterate()) != 0)
		{
			BaseObject *real = (BaseObject*) object->GetOpaquePointer();
			real->Tick(delta);
		}
		session->ObjectListFinishIterate();

		// Frame object render
		session->ObjectListBeginIterate();
		while ((object = session->ObjectListIterate()) != 0)
		{
			BaseObject *real = (BaseObject*) object->GetOpaquePointer();
			real->Render();
		}
		session->ObjectListFinishIterate();

		// Draw debug and other Marmalade stuff
		iter++;
		char buffer[500];
#ifdef TEST_LOBBY
		sprintf(buffer,"13it%d t%.2f frm%dms thrd%d conn%d",iter,theTime2.FloatTime(),(int)(lastTick * 1000.0f),PlatformInfo::IsThreaded(),advertise1.GetNumConnections());
#else
		sprintf(buffer,"13it%d t%.2f frm%dms thrd%d",iter,theTime2.FloatTime(),(int)(lastTick * 1000.0f),PlatformInfo::IsThreaded());
#endif
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

	// Delete any remaining objects and free remaining memory
	session->ObjectListBeginIterate();
	ReplicaObject *object;
	while ((object = session->ObjectListIterate()) != 0)
	{
		delete object;
	}
	session->ObjectListFinishIterate();

	delete session;

	fillScreen(0, 0, 0);
	s3eDebugPrint(0, 30, "Exiting!", false);
	s3eSurfaceShow();
	s3eDeviceYield(0);

	PlatformHeap::ForceFree();

	return 0;
}
