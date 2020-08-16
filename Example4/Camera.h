/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
The camera object. Typically this object is very simple so it does not require a cpp file
*/
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "d3dfile.h"

#include "GameObject.h"
#include "_RO_Camera.h"

class Camera : _RO_DO_PUBLIC_RO(Camera) , public GameObject
{
public:
	Camera()
	{
		SetReplicaObject(this);		// Setup our forward reference pointer
		mPosition = D3DXVECTOR4(0,0,0,0);
	}

	virtual ~Camera()
	{
	}

	void PollIt(void)
	{
//		dprintf("Camera $%p: Master(%d) uniqieID %d sessid %d Pos %f,%f,%f\n",(void*)this,IsMaster(),GetUniqueID(),GetSessionID(),mPosition.x,mPosition.y,mPosition.z);
	}

	bool GetPosition(D3DXVECTOR4 &input)
	{
		input = mPosition;
		return true;
	}

	D3DXVECTOR4 GetPosition(void)
	{
		return mPosition;
	}

	D3DXVECTOR4 mPosition;

	/**
	 * This member function is from the ReplicaObject::GetOpaquePointer() virtual function.
	 */
	void *GetOpaquePointer(void *data = 0)
	{
		return (GameObject *)(this);
	}

	/*
	This virtual callback from the ReplicaObject class is used by the distance based prediction to calculate the distance to another object in our game
	*/
	float CalculateDistanceToObject(RNReplicaNet::ReplicaObject *object)
	{
		// Since the OpaquePointer is always inherited from the GameObject pointer for each object we can do this cast safely
		GameObject *gameobject = (GameObject *) object->GetOpaquePointer();
		return GameObject::CalculateDistanceToObject(gameobject);
	}
};

#endif
