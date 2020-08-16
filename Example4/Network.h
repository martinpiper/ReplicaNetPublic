/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
See Network.cpp
*/
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <string>
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "MyReplicaNet.h"

/**
 * Handles the network dialog and allocation of the ReplicaNet session
 */
class Network
{
private:
	Network();		// Not intended for allocation

public:
	static void Network_Init(std::string url);
	static void Network_Shutdown(void);
	static void Network_Dialog(void);
	static void HandleRecording(void);

	static MyReplicaNet *mNetwork;
	static bool mDedicatedServer;
	static bool mRenderModels;
	static bool mDoRecord;
	static bool mDoPlayback;
	static std::string mRecordingFileName;
};

#endif
