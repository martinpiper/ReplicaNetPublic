/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
The base class definition for game objects. See GameObjkect.cpp
*/
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "d3dfile.h"
#include <vector>

class GameObject;
namespace RNReplicaNet
{
class ReplicaObject;
}

extern std::vector<GameObject *> gGameObjects;

class GameObject
{
public:
	GameObject();

	virtual ~GameObject();

	virtual void PollIt(void);

	virtual D3DXVECTOR4 GetPosition(void)
	{
		return D3DXVECTOR4(0,0,0,0);
	}


	/**
	 * A delete method for a game object since deletion of a class while in a member function is not very good design
	 */
	void Delete(void);

	bool mDeleteMe;

	enum ObjectType
	{
		kNULL,
		kPlane,
		kCamera
	};

	ObjectType mType;

	/*
	A pointer held by the class that points to the derived class of this object for forward references
	*/
	RNReplicaNet::ReplicaObject *mReplica;
};

#endif
