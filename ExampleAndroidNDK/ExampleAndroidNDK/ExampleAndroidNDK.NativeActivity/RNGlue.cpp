#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>

#include "RNPlatform/Inc/PlatformInfo.h"
#include "RNPlatform/Inc/PlatformHeap.h"
#include "RNLobby/Inc/BackendClient.h"
#include "RNLobby/Inc/AdvertiseClient.h"
#include "RNLobby/Inc/UserClient.h"
#include "RNXPURL/Inc/TransportConfig.h"

#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "RNReplicaNet/Inc/RO_RegistryBlock.h"
#include "RNReplicaNet/Inc/Define_RO.h"

#include "RNGlue.h"
#include "Font.h"

_RO_DO_PREREGISTER_FUNC()
_RO_DO_PREREGISTER_FUNC_END()

#include <android/log.h>
#define  LOG_TAG    "replicanet"

using namespace RNReplicaNet;
using namespace RNLobby;

static BackendClient *backend = 0;
static AdvertiseClient *advertise = 0;
static UserClient *user = 0;

class MyAdvertiseClient : public AdvertiseClient
{
public:
	virtual void CallbackGotServerIndex(const int serverIndex)
	{
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "CallbackGotServerIndex %d\n", serverIndex);
	}

	/// This is called at the start when the listings are refreshed.
	virtual void CallbackListingsBegin(void)
	{
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "CallbackListingsBegin\n");
	}

	virtual void CallbackListingsEntry(const int serverIndex, const int hostLoginID, const char *url, const char *userData, const float age, const XPAddress &address)
	{
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "CallbackListingsEntry %d %d '%s' '%s' %f %s\n", serverIndex, hostLoginID, url, userData, age, address.Export().c_str());
	}


	virtual void CallbackListingsComplete(void)
	{
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "CallbackListingsComplete\n");
	}
};

class MyUserClient : public RNReplicaNet::RNLobby::UserClient
{
public:
	MyUserClient()
	{
	}

	virtual ~MyUserClient()
	{
	}

protected:
	virtual void CallbackUserLogin(const int loginID)
	{
		EnterChatRoom(0);
	}
};

// This is called when starting the application
extern "C" void TestRN(void)
{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "IsThreaded %d\n", RNReplicaNet::PlatformInfo::IsThreaded());

	backend = new BackendClient();

	// Start an asynchronous backend authentication check
	// Use the example 12 backend information for now.
	backend->Start(12, "Example12Key");
	//	backend->Start(0,"");	// Use locally hosted servers
}

static SysTime sTime;

// This is called every frame so the debug can be rendered
extern "C" void TestRN2(void)
{
	char buffer[1024];

	// If there is no advertise client allocated then assume we are still in the backend authentication state
	if (!advertise)
	{
		if (!backend)
		{
			FontDraw(8, 64, "No backend allocated, oops!");
			return;
		}

		if (!backend->GetCompleted())
		{
			sprintf(buffer, "Backend authentication check %f\nsent %d bytes/sec %d\nrecv %d bytes/sec %d\n", sTime.FloatTime(), asocket_packets_sent, (int)asocket_packets_sent_size_per_second, asocket_packets_got, (int)asocket_packets_got_size_per_second);
			FontDraw(8, 64, buffer);
			return;
		}

		if (!RNReplicaNet::RNLobby::BackendClient::IsAuthenticated())
		{
			// This indicates there was a problem contacting the server
			sprintf(buffer, "Backend should be authenticated but isn't\nError code %d\n", (int)backend->GetReply());
			FontDraw(8, 64, buffer);
			return;
		}

		// Backend authentication completed. so initialise the rest of the application
		RNReplicaNet::RNLobby::RegisterDefaultTransports();

		// Don't need the backend authentication now, so delete it.
		delete backend;
		backend = 0;

		advertise = new MyAdvertiseClient();
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "1");
		advertise->Start();
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "2");
		user = new MyUserClient();
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "3");
		user->Start();
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "4");
		user->LoginUser("Example12User", "Example12Password");
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "5");
		advertise->AdvertiseSession("Native android", "user data");
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "6");
	}


	if (!advertise || !user)
	{
		FontDraw(8, 64, "No advertise or user client allocated, oops!");
		return;
	}

	sprintf(buffer, "ExampleAndroidNDK Test 1\nsent %d bytes/sec %d\nrecv %d bytes/sec %d\nconn %d\n", asocket_packets_sent, (int)asocket_packets_sent_size_per_second, asocket_packets_got, (int)asocket_packets_got_size_per_second, advertise->GetNumConnections() + user->GetNumConnections());
	FontDraw(8, 64, buffer);

	if (sTime.FloatTime() > 5.0f)
	{
		user->ChatRoomSay(buffer);
		advertise->GetListings();
		sTime.Reset();
	}
}

// This is called when the application loses focus.
extern "C" void TestRN3(void)
{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Free everything");

	delete backend;
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "FE1");

	delete user;
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "FE2");

	delete advertise;
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "FE3");

	PlatformHeap::ForceFree();
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "FE4");

	exit(0);
	// This FE5 message should not be displayed
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "FE5");
}
