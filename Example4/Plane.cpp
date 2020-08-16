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
#include "GameDatabase.h"
#include "Network.h"
#include "GameClass.h"

extern CD3DMesh*	 gpAirplaneMesh;

Plane::Plane()
{
	SetReplicaObject(this);		// Setup our forward reference pointer
	mMesh = gpAirplaneMesh;
	mPosition = D3DXVECTOR4(0,0,0,0);
	mRotation = D3DXVECTOR4(0,0,0,0);

	mPlayerNumber = rand() & 7;
	mPlayerColour = rand() & 7;
	mPlayerEnergy = 1.0f;

	mSpeed = 40.0f;
//	mSpeed = 10.0f;
//	mSpeed = 0.0f;
}

Plane::~Plane()
{
	// If this plane is being deleted and it is being used as the camera plane then
	// disconnect it from the application so the camera does not use a hanging pointer.
	if (CMyD3DApplication::GetCMyD3DApplication()->mPlayerConrolledPlane == this)
	{
		CMyD3DApplication::GetCMyD3DApplication()->mPlayerConrolledPlane = 0;
	}
}

void Plane::PostObjectCreate(void)
{
	// If we are playing back a recording and the plane sessionID matches with the recorded sessionID
	// then set the controlled plane pointer so the camera can use it to render with.
	if (Network::mNetwork->GetSessionPlayback() && GetSessionID() == Network::mNetwork->GetSessionID())
	{
		CMyD3DApplication::GetCMyD3DApplication()->mPlayerConrolledPlane = this;
	}
}

void Plane::PollIt(void)
{
	// Only run the "physics" if we are a master object
	if (!IsMaster())
	{
		return;
	}

	if (mSpeed > 0.1f)
	{
	D3DXVECTOR4 velocity = D3DXVECTOR4(0,0,-mSpeed,0);
//	mRotation.y += D3DXToRadian(0.1f);

	// Calculate the velocity movement
	D3DXMatrixRotationYawPitchRoll(&m_matObjectMatrix,mRotation.y,mRotation.x,mRotation.z);
	D3DXVECTOR4 trans;
	D3DXVec4Transform(&trans,&velocity,&m_matObjectMatrix);

	mPosition += trans * GameDatabase::mGameFrameTick;
	GiveDeltaHint(mPosition.x,trans.x);
	GiveDeltaHint(mPosition.y,trans.y);
	GiveDeltaHint(mPosition.z,trans.z);
	}

#define LIMIT (140.0f)

	if (mPosition.x < -LIMIT)
	{
		mPosition.x = -LIMIT;
		GiveDeltaHint(mPosition.x,0);
	}
	if (mPosition.z < -LIMIT)
	{
		mPosition.z = -LIMIT;
		GiveDeltaHint(mPosition.z,0);
	}
	if (mPosition.x > LIMIT)
	{
		mPosition.x = LIMIT;
		GiveDeltaHint(mPosition.x,0);
	}
	if (mPosition.z > LIMIT)
	{
		mPosition.z = LIMIT;
		GiveDeltaHint(mPosition.z,0);
	}
	if (mPosition.y < -6.0f)
	{
		mPosition.y = -6.0f;
		GiveDeltaHint(mPosition.y,0);
	}
	if (mPosition.y > 500.0f)
	{
		mPosition.y = 500.0f;
		GiveDeltaHint(mPosition.y,0);
	}

}

