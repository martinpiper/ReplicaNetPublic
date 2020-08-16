/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
The Projectile game object. Every tick the Projectile will move
This object have corresponding ROL files in the ROLFiles folder. The ROL files are compiled by the ROLCompiler.
The CompiledGameObjects folder contains the compiled ROL definition file for this object. The member variables listed
in the ROL file for this class are automatically updated when this object is allocated on another machine
*/
#include "Projectile.h"
#include "Enemy.h"
#include "Network.h"
#include "Camera.h"
#include "GameDatabase.h"

Projectile::Projectile()
{
	SetReplicaObject(this);		// Setup our forward reference pointer
	mMesh = 0;
	mPosition = D3DXVECTOR4(0,0,0,0);
	mRotation = D3DXVECTOR4(0,0,0,0);

	mLifetime = 0.0f;
}

Projectile::~Projectile()
{
}

void Projectile::PollIt(void)
{
	mLifetime += GameDatabase::mGameFrameTick;

#define LIMIT (180.0f)

	if (mLifetime > 4.0f || mPosition.x < -LIMIT  || mPosition.x > LIMIT || mPosition.z < -LIMIT || mPosition.z > LIMIT || mPosition.y < -10.0f || mPosition.y > 500.0f)
	{
		Delete();
		return;
	}

	// Only run the "physics" if we are a master object
	if (!IsMaster())
	{
		return;
	}

#if 0
	for (int i=0;i<gGameObjects.size();i++)
	{
		ReplicaObject *robject;
		// Get the derived class object pointer that contains a base class type of ReplicaObject from the GameObject class
		robject = gGameObjects[i]->mReplica;
		// If there is no object pointer then ignore this object for rendering
		if (!robject)
		{
			continue;
		}
		// Compare the class id of the ReplicaObject with the class ID of the ReplicaObject Plane object
		// If they are the same then we can assume the type cast to a Plane class is valid
		if (robject->GetClassID() == _MAKE_RO(Enemy)::StaticGetClassID())
		{
			D3DXVECTOR4 diff;
			Enemy *proj = (Enemy *) robject;
			diff = GetPosition() - proj->GetPosition();
			float length = D3DXVec4Length(&diff);
			if (length < 10.0f)
			{
				Delete();
				return;
			}
		}
	}
#endif

	D3DXVECTOR4 velocity = D3DXVECTOR4(0,0,-100.05f,0);

	// Calculate the velocity movement
	D3DXMatrixRotationYawPitchRoll(&m_matObjectMatrix,mRotation.y,mRotation.x,mRotation.z);
	D3DXVECTOR4 trans;
	D3DXVec4Transform(&trans,&velocity,&m_matObjectMatrix);
	mPosition += trans * GameDatabase::mGameFrameTick;
//	GiveDeltaHint(mPosition.x,trans.x);
//	GiveDeltaHint(mPosition.y,trans.y);
//	GiveDeltaHint(mPosition.z,trans.z);
}

