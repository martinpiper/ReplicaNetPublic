/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "AnObject.h"

using namespace RNReplicaNet;

int main(int argc,char **argv)
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

	// Check to see if we have found a session for Example6
	if (found.find("Example6") == std::string::npos)
	{
		found = "";
	}

	// If no session name is found then...
	if (found == "")
	{
		// Create a new session
		network->SessionCreate("Example6");
		printf("New game started '%s'\n",network->SessionExportURL().c_str());
	}
	else
	{
		// Join the found session
		printf("Joining session '%s'\n",found.c_str());
		network->SessionJoin(found);

		// Wait until the session is stable
		while (!network->IsStable())
		{
			printf(".");
			// Sleep for a short while
			CurrentThread::Sleep(100);
		}

		printf("\n");
	}

	// Seed our random number generator with our session ID
	srand(network->GetSessionID());

	// Now allocate our test object
	AnObject *object;
	object = new AnObject();

	// THREAD SAFETY: The object has not yet been added to ReplicaNet by publish so it is thread-safe to update it here
	object->mList.push_back(10);
	object->mList.push_back(20);
	object->mList.push_back(30);
	object->mList.push_back(40);

	object->mSTLVector.push_back(110);
	object->mSTLVector.push_back(120);
	object->mSTLVector.push_back(130);
	object->mSTLVector.push_back(140);


	// Publish the object on to the network
	object->Publish();

	// Go around in an endless loop
	while(1)
	{
		// ReplicaNet could update objects here due to multi-threading.
		// Sleep for a short time. Emulate the time taken due to some other work, such as physics updates or rendering.
		CurrentThread::Sleep(250);

		// THREAD SAFETY: Lock the ReplicaObjects so they can be accessed.
		network->LockObjects();
		// Now randomly change the mWatchedValue value for our object to test that the values get transmitted
		if (rand() & 1)
		{
			object->mWatchedValue.SetValue(float(rand()));
		}
		// Update the object STL vector when the local time is greater than ten seconds.
		if (network->GetLocalTime() > 10.0f)
		{
			// After 10 seconds have elapsed on the local time clock delete the first entry in the mSTLVector vector variable each iteration.
			if (!object->mSTLVector.empty())
			{
				object->mSTLVector.erase(object->mSTLVector.begin());
			}
		}
		// THREAD SAFETY: Unlock the ReplicaObjects
		network->UnLockObjects();

		// ReplicaNet could update objects here due to multi-threading.
		// Sleep for a short time. Emulate the time taken due to some other work, such as physics updates or rendering.
		CurrentThread::Sleep(250);

		// Iterate through all objects for this session to display values from those objects.
		// THREAD SAFETY: This also has an implicit LockObjects()
		network->ObjectListBeginIterate();
		ReplicaObject *objects;
		while ( (objects = network->ObjectListIterate()) != 0)
		{
			printf("ReplicaObject %d:%d:%d\n",objects->GetSessionID(),objects->GetUniqueID(),(int)objects->IsMaster());
			// Double check that this ReplicaObject is of a type AnObject
			if (objects->GetClassID() == AnObject::StaticGetClassID())
			{
				AnObject *realObject = (AnObject *) objects;
				if (realObject->mWatchedValue.GetUpdated())
				{
					printf("Object %d:%d has the value updated\n",realObject->GetSessionID(),realObject->GetUniqueID());
				}

				std::list<float>::iterator st,en;
				st = realObject->mList.begin();
				en = realObject->mList.end();
				int index = 0;
				while (st != en)
				{
					printf("mList[%d] = %f\n",index++,*st);
					st++;
				}

				std::vector<float>::iterator st2,en2;
				st2 = realObject->mSTLVector.begin();
				en2 = realObject->mSTLVector.end();
				index = 0;
				while (st2 != en2)
				{
					printf("mSTLVector[%d] = %f\n",index++,*st2);
					st2++;
				}
			}
		}
		// THREAD SAFETY: This also has an implicit UnLockObjects()
		network->ObjectListFinishIterate();

		// ReplicaNet could update objects here due to multi-threading.
		// Sleep for a short time. Emulate the time taken due to some other work, such as physics updates or rendering.
		CurrentThread::Sleep(250);
	}

	return 0;
}
