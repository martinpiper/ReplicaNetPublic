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
#include <assert.h>
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "RNPlatform/Inc/PlatformHeap.h"
#include "RNPlatform/Inc/MemoryTracking.h"
#include "RNXPURL/Inc/XPURL.h"
#include "Test1.h"

using namespace RNReplicaNet;

namespace RNReplicaNet
{
template<>
void FunctionAddVariable<ExampleCustomType>(DynamicMessageHelper &message, ExampleCustomType &variable)
{
	message << variable.mX;
	message << variable.mY;
	message << variable.mZ;
	message << variable.mText;
}

template<>
void FunctionGetVariable<ExampleCustomType>(MessageHelper &message, ExampleCustomType &variable)
{
	message >> variable.mX;
	message >> variable.mY;
	message >> variable.mZ;
	message >> variable.mText;
}
}


void RunTest(void)
{
	ReplicaNet *network;

	// Allocate a ReplicaNet session
	network = new ReplicaNet();

	// Start to find a session
	printf("Searching for a game...\n");
	network->SessionFind();

	// Sleep for half a second to wait for replies
	CurrentThread::Sleep(500);

	// Enumerate the first found session
	std::string found = network->SessionEnumerateFound();

	// If no session name is found then...
	if (found == "")
	{
		// Create a new session
		network->SessionCreate("Example2");
		printf("New game started '%s'\n",network->SessionExportURL().c_str());
	}
	else
	{
		// Join the found session
		printf("Joining session '%s'\n",found.c_str());
		network->SessionJoin(found);
	}

	// Now allocate our test object
	Test1 *object;
	object = new Test1();
	// Publish the object on to the network
	object->Publish();

	// Go around in loop for ten times
	int i;
	for (i=0;i<10;i++)
	{
		// RunTest() is the member function that shows how to call the network shared member functions
		object->RunTest();

		// Sleep for five seconds
		CurrentThread::Sleep(5000);
	}


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
}

int main(int argc,char **argv)
{
	RunTest();

	// Force free all internal memory and threads.
	PlatformHeap::ForceFree();

	return 0;
}
