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
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "Test1.h"
#include "RNPlatform/Inc/ThreadClass.h"

using namespace RNReplicaNet;

// This ExtendedObject is used to demonstrate use of ReplicaNet::CallBackAllocate() with custom classIDs.
class ExtendedObject : public Test1
{
public:
	ExtendedObject()
	{
		printf("ExtendedObject()\n");
	}

	virtual const int GetClassID(void)
	{
		return StaticGetClassID();
	}

	const int static StaticGetClassID(void)
	{
		return 1000;
	}

	virtual void PostObjectCreate(void)
	{
		printf("ExtendedObject::PostObjectCreate()\n");
	}

};

/**
 * Create our simple derived class to detect session joiners.
 */
class MyReplicaNet : public ReplicaNet
{
public:
	MyReplicaNet() : mJoined(0) {}

	/**
	 * Access method for the number of joined sessions.
	 */
	int GetNumJoined(void)
	{
		// We use a mutex because we are multi-threaded and the variable may change due to this thread.
		THREADSAFELOCKCLASS(mMutex);
		return mJoined;
	}

	/**
	 * Access method to get the session ID based on an index.
	 */
	int GetSessionIDAt(const int index)
	{
		// We use a mutex because we are multi-threaded and the variable may change due to this thread.
		THREADSAFELOCKCLASS(mMutex);
		return mSessionIDs[index];
	}

private:
	void JoinerSessionIDPost(const int sessionID)
	{
		// We use a mutex because we are multi-threaded and the variable may change due to this thread.
		THREADSAFELOCKCLASS(mMutex);
		mSessionIDs.push_back(sessionID);
		mJoined++;
	}

	int mJoined;

	std::vector<int> mSessionIDs;

	MutexClass mMutex;

	// This detects the custom classID for ExtendedObject and allocates an instance
	ReplicaObject *CallBackAllocate(const int classID,const int sessionID,const int uniqueID)
	{
		if (classID == ExtendedObject::StaticGetClassID())
		{
			return new ExtendedObject();
		}
		return 0;
	}
};


int main(int argc,char **argv)
{
	MyReplicaNet *network;

	// Allocate a ReplicaNet session
	network = new MyReplicaNet();

	// Start to find a session
	printf("Searching for a game...\n");
	network->SessionFind();

	// Sleep for half a second to wait for replies
	CurrentThread::Sleep(500);

	// Enumerate the first found session
	std::string found = network->SessionEnumerateFound();

	if (found != "")
	{
		// Check for the "Example8" string in the URL and set the found URL to be "" if it does not exist
		if (found.find("Example8") == std::string::npos)
		{
			found = "";
		}
	}

	// If no matching session name is found then...
	if (found == "")
	{
		// Create a new session
		network->SessionCreate("Example8");
		printf("New game started '%s'\n",network->SessionExportURL().c_str());
	}
	else
	{
		// Join the found session
		printf("Joining session '%s'\n",found.c_str());
		network->SessionJoin(found);
	}

	// Wait for the session to become stable
	while (!network->IsStable())
	{
		CurrentThread::Sleep(100);
	}

	Test1 *object = 0;
	// If the session is our master session then create an object
	if (network->IsMaster())
	{
		// Now allocate our test object
		object = new ExtendedObject();
		// Publish the object on to the network
		object->Publish();

		printf("Waiting for the other three sessions to join\n");
	}

	while (network->GetNumJoined() < 3)
	{
		CurrentThread::Sleep(200);
	}

	// Go around in an endless loop
	while(1)
	{
		// If this session has an allocated object then we know we just allocated it above
		if (object)
		{
			int twoSessionIDs[2];
			// Get the first and third (0 and 2) session IDs to join the session from our list
			twoSessionIDs[0] = network->GetSessionIDAt(0);
			twoSessionIDs[1] = network->GetSessionIDAt(2);
			// RunTest() is the member function that shows how to call the network shared member functions
			// This time we nominate the two session IDs to call
			object->RunTest(2,twoSessionIDs);
		}

		// Sleep for five seconds
		CurrentThread::Sleep(5000);
	}

	return 0;
}
