/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
The Plane game object. Every tick the plane will move a little furthur around in a circle.
This object have corresponding ROL files in the ROLFiles folder. The ROL files are compiled by the ROLCompiler.
The CompiledGameObjects folder contains the compiled ROL definition file for this object. The member variables listed
in the ROL file for this class are automatically updated when this object is allocated on another machine
*/
#include "Plane.h"
#include "Camera.h"

extern CD3DMesh*	 gpAirplaneMesh;

Plane::Plane()
{
	mReplica = this;		// Setup our forward reference pointer
	mMesh = gpAirplaneMesh;
	mPosition = D3DXVECTOR4(0,0,0,0);
	mRotation = D3DXVECTOR4(0,0,0,0);

	mPlayerNumber = rand() & 7;
	mPlayerColour = rand() & 7;
	mPlayerEnergy = 1.0f;
	mFunctionXRot = 0.0f;

//	mShadow = new ShadowVolume();
}

Plane::~Plane()
{
}

void Plane::PollIt(void)
{
	// Only run the "physics" if we are a master object
	if (!IsMaster())
	{
		return;
	}

	// Deliberately no frame rate compensation here, to make the example easier to follow.
	D3DXVECTOR4 velocity = D3DXVECTOR4(0,0,-0.5f,0);
	mRotation.y += D3DXToRadian(0.5f);
	mRotation.y += D3DXToRadian(0.5f);

	// Calculate the velocity movement
	D3DXMatrixRotationYawPitchRoll(&m_matObjectMatrix,mRotation.y,mRotation.x,mRotation.z);
	D3DXVECTOR4 trans;
	D3DXVec4Transform(&trans,&velocity,&m_matObjectMatrix);
	mPosition += trans;
}

/*
This callback is used by the distance based prediction to calculate the distance to another object in our game
*/
float Plane::CalculateDistanceToObject(ReplicaObject *object)
{
	// This could be part of the GameObject base class
	if (object->GetClassID() == _RO_Camera::StaticGetClassID())
	{
		Camera *gameobject = (Camera *) object;

		D3DXVECTOR4 pos = gameobject->GetPosition();

		pos.x = mPosition.x - pos.x;
		pos.y = mPosition.y - pos.y;
		pos.z = mPosition.z - pos.z;
		return sqrtf( (pos.x*pos.x) + (pos.y*pos.y) + (pos.z*pos.z));
	}
	return kReplicaObject_InfiniteDistance;
}

/*
This callback returns true which means if a session quits then the fault tolerant
mechanism makes the plane get transfered from the session that left to another session
*/
bool Plane::ApproveFault(void)
{
	return true;
}

/*
This callback lets us know if the plane object changes who owns the object
*/
void Plane::OwnerChange(void)
{
}
