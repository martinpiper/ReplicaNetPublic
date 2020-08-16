/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
See Enemy.cpp
*/
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "d3dfile.h"

#include "GameObject.h"
#include "Shadow.h"

#include "_RO_Enemy.h"

class Enemy : _RO_DO_PUBLIC_RO(Enemy) , public GameObject
{
public:
	Enemy();
	virtual ~Enemy();

	void Render(void);

	void PollIt(void);

	bool GetPosition(D3DXVECTOR4 &input)
	{
		input = mPosition;
		return true;
	}

	D3DXVECTOR4 GetPosition(void)
	{
		return mPosition;
	}

	void SetPosition(D3DXVECTOR4 pos)
	{
		mPosition = pos;
		GiveDeltaHint(mPosition.x,0);
		GiveDeltaHint(mPosition.y,0);
		GiveDeltaHint(mPosition.z,0);
	}

	D3DXVECTOR4 GetRotation(void)
	{
		return mRotation;
	}

	void SetRotation(D3DXVECTOR4 rot)
	{
		mRotation = rot;
	}

	float GetSpeed(void)
	{
		return mSpeed;
	}

	void SetSpeed(float speed)
	{
		mSpeed = speed;
	}

	/*
	This callback is used by the distance based prediction to calculate the distance to another object in our game
	*/
	float CalculateDistanceToObject(RNReplicaNet::ReplicaObject *object)
	{
		// Since the OpaquePointer is always the GameObject pointer for each object we can do this cast safely
		GameObject *gameobject = (GameObject *) object->GetOpaquePointer();
		return GameObject::CalculateDistanceToObject(gameobject);
	}

	static void SpawnEnemy(void);

	void *GetOpaquePointer(void *data = 0)
	{
		return (GameObject *)(this);
	}

/* These variables are defined in the ROL file for this object */
private:
_RO_DO_ALLOW_FRIEND_RO(Enemy)
	D3DXVECTOR4 mPosition;
	D3DXVECTOR4 mRotation;

	float mSpeed;
};

#endif
