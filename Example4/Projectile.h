/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
See Projectile.cpp
*/
#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "d3dfile.h"

#include "GameObject.h"

#include "_RO_Projectile.h"

class Projectile : _RO_DO_PUBLIC_RO(Projectile) , public GameObject
{
public:
	Projectile();
	virtual ~Projectile();

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
		// Our position is being set by an outside class and since we don't know what our new velocity is yet we give the network layer a hint that it's changed
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
		// Our rotation is being set by an outside class, here we give the network layer a hint that it's changed
		GiveDeltaHint(mRotation.x,0);
		GiveDeltaHint(mRotation.y,0);
		GiveDeltaHint(mRotation.z,0);

		D3DXVECTOR4 velocity = D3DXVECTOR4(0,0,-100.05f,0);

		// Calculate the velocity movement
		D3DXMatrixRotationYawPitchRoll(&m_matObjectMatrix,mRotation.y,mRotation.x,mRotation.z);
		D3DXVECTOR4 trans;
		D3DXVec4Transform(&trans,&velocity,&m_matObjectMatrix);
		GiveDeltaHint(mPosition.x,trans.x);
		GiveDeltaHint(mPosition.y,trans.y);
		GiveDeltaHint(mPosition.z,trans.z);
	}

	CD3DMesh *mMesh;
	D3DXMATRIX	m_matObjectMatrix;

	void OwnerChange(void)
	{
	}

	void *GetOpaquePointer(void *data = 0)
	{
		return (GameObject *)(this);
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

/* These variables are defined in the ROL file for this object */
private:
_RO_DO_ALLOW_FRIEND_RO(Projectile)
	D3DXVECTOR4 mPosition;
	D3DXVECTOR4 mRotation;

	float mLifetime;
};

#endif
