/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
The Dolphin game object. Every tick the dolphin will move a little using a sin/cos algorithm.
*/
#include "Dolphin.h"
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include <math.h>

extern CD3DMesh*	 gpDolphinMesh;
extern CD3DMesh*	 gpAirplaneMesh;

Dolphin::Dolphin()
{
	mReplica = this;		// Setup our forward reference pointer
	mMesh = gpDolphinMesh;
//	mMesh = gpAirplaneMesh;
	mPosition = D3DXVECTOR4(0,0,0,0);
	mRotation = D3DXVECTOR4(0,D3DXToRadian(180),0,0);

//	mShadow = new ShadowVolume();
}

Dolphin::~Dolphin()
{
}

void Dolphin::PollIt(void)
{
	// Only run the "physics" if we are a master object
	if (!IsMaster())
	{
		return;
	}

	D3DXVECTOR4 velocity = D3DXVECTOR4(0,0,-0.05f,0);
	mRotation.y -= D3DXToRadian(0.1f);

	// Calculate the velocity movement
	D3DXMatrixRotationYawPitchRoll(&m_matObjectMatrix,mRotation.y,mRotation.x,mRotation.z);
	D3DXVECTOR4 trans;
	D3DXVec4Transform(&trans,&velocity,&m_matObjectMatrix);
	mPosition += trans;
}

