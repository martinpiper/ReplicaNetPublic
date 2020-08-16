/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This file is the base class for any type of game object.
When an object is allocated or deleted the GameDatabase::mGameObjects array tracks the list of allocated objects
*/

#include "Network.h"
#include "GameObject.h"
#include "GameDatabase.h"
#include "RNReplicaNet/Inc/ReplicaObject.h"

using namespace RNReplicaNet;

GameObject::GameObject() : mDeleteMe(false) , mReplica(0)
{
	// Lock our own object list on delete
	Network::mNetwork->LockObjects();

	GameDatabase::mGameObjects.push_back(this);

	// UnLock our own object list on delete
	Network::mNetwork->UnLockObjects();
};

GameObject::~GameObject()
{
	// Lock our own object list on delete
	Network::mNetwork->LockObjects();

	std::vector<GameObject *>::iterator st,en;
	st = GameDatabase::mGameObjects.begin();
	en = GameDatabase::mGameObjects.end();
	while (st != en)
	{
		GameObject *object = *st;
		if (object == this)
		{
			GameDatabase::mGameObjects.erase(st);
			break;
		}
		st++;
	}

	// UnLock our own object list on delete
	Network::mNetwork->UnLockObjects();
};

void GameObject::PollIt(void)
{
	printf("Default PollIt()\n");
};

void GameObject::Delete(void)
{
	mDeleteMe = true;
}

/**
 * This calculates a distance to another GameObject.
 * If the distance cannot be calculated, since one or both objects do not know about position,
 * then kReplicaObject_InfiniteDistance is returned.
 * This works because every GameObject::GetPosition() is able to return the correct position (or false)
 * from the allocated derived class.
 * A good place to look is Camera.h to see how the classes are structured.
 */
float GameObject::CalculateDistanceToObject(GameObject *gameobject)
{
	if (gameobject)
	{
		D3DXVECTOR4 pos;
		D3DXVECTOR4 otherpos;
		bool ret = gameobject->GetPosition(otherpos);

		if (ret)
		{
			ret = GetPosition(pos);
			if (ret)
			{
				pos.x = otherpos.x - pos.x;
				pos.y = otherpos.y - pos.y;
				pos.z = otherpos.z - pos.z;
				return sqrtf( (pos.x*pos.x) + (pos.y*pos.y) + (pos.z*pos.z));
			}
		}
	}

	return kReplicaObject_InfiniteDistance;
}

