/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This file is the base class for any type of game object.
When an object is allocated or deleted the gGameObjects array tracks the list of allocated objects
*/

#include "Network.h"
#include "GameObject.h"

std::vector<GameObject *> gGameObjects;

GameObject::GameObject() : mDeleteMe(false) , mReplica(0)
{
	// Lock our own object list on delete
	gNetwork->LockObjects();

	gGameObjects.push_back(this);

	// UnLock our own object list on delete
	gNetwork->UnLockObjects();
};

GameObject::~GameObject()
{
	// Lock our own object list on delete
	gNetwork->LockObjects();

	std::vector<GameObject *>::iterator st,en;
	st = gGameObjects.begin();
	en = gGameObjects.end();
	while (st != en)
	{
		GameObject *object = *st;
		if (object == this)
		{
			gGameObjects.erase(st);
			break;
		}
		st++;
	}

	// UnLock our own object list on delete
	gNetwork->UnLockObjects();
};

void GameObject::PollIt(void)
{
	printf("Default PollIt()\n");
};

void GameObject::Delete(void)
{
	mDeleteMe = true;
}
