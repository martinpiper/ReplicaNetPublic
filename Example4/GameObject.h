/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
The base class definition for game objects. See GameObject.cpp
*/
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "d3dfile.h"
#include <vector>

// These classes are accessed via their pointers
namespace RNReplicaNet
{
class ReplicaObject;
}

/**
 * This base class defines a game object
 */
class GameObject
{
protected:
	/**
	 * Protected ctor means this base class cannot be allocated except with a derived class
	 */
	GameObject();

public:
	/**
	 * Anyone can delete this class
	 */
	virtual ~GameObject();


	/**
	 * A virtual method to allow this object to be polled.
	 */
	virtual void PollIt(void);

	/**
	 * This virtual function can be used to supply a method to return a position for an object.
	 * If an object doesn't know about position then 'false' will be returned.
	 * \param input the D3DXVECTOR4 that will contain the object position, this is not changed if the object has not concept of a position
	 * \return true if the object has a concept of a position or false if the object has no concept of a position
	 */
	virtual bool GetPosition(D3DXVECTOR4 &input)
	{
		return false;
	}

	/**
	 * A delete method for a game object since deletion of a class while in a member function is not very good design.
	 * This delays the real delete of the game object to a time when the object list is not being iterated
	 */
	void Delete(void);

	/**
	 * Access method for mDeleteMe
	 * \return true if the object Delete() method was used
	 */
	bool GetDelete(void) const
	{
		return mDeleteMe;
	}

	/**
	 * The object types known to this example
	 */
	enum ObjectType
	{
		kNULL,
		kPlane,
		kCamera
	};

	/**
	 * This calculates a distance to another GameObject. See GameObject.cpp for more details.
	 * \return The distance to the other object or kReplicaObject_InfiniteDistance
	 */
	float CalculateDistanceToObject(GameObject *object);

	void SetReplicaObject(RNReplicaNet::ReplicaObject *const object)
	{
		mReplica = object;
	}

	RNReplicaNet::ReplicaObject *GetReplicaObject(void) const
	{
		return mReplica;
	}

	void SetType(const ObjectType type)
	{
		mType = type;
	}

	ObjectType GetType(void) const
	{
		return mType;
	}

private:
	/**
	 * When the Delete() function is used this gets set to be true
	 */
	bool mDeleteMe;

	/**
	 * A pointer held by the class that points to the derived class, as a base class pointer, of this object for forward references
	 */
	RNReplicaNet::ReplicaObject *mReplica;

	/**
	 * The type of this object
	 */
	ObjectType mType;
};

#endif
