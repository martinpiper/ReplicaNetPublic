/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This object have corresponding ROL files in the ROLFiles folder. The ROL files are compiled by the ROLCompiler.
The CompiledGameObjects folder contains the compiled ROL definition file for this object. The member variables listed
in the ROL file for this class are automatically updated when this object is allocated on another machine
*/
#include "Enemy.h"
#include "Projectile.h"
#include "Camera.h"
#include "RNReplicaNet/Inc/DataBlock.h"
#include "Network.h"
#include "GameDatabase.h"

using namespace RNReplicaNet;

void Enemy::SpawnEnemy(void)
{
	Enemy *enemy;
	enemy = new Enemy();
	enemy->SetPosition(D3DXVECTOR4(100.0f*float(rand())/float(RAND_MAX),0,100.0f*float(rand())/float(RAND_MAX),0));

	enemy->SetSpeed((10.0f*float(rand())/float(RAND_MAX))+0.2f);
	enemy->SetRotation(D3DXVECTOR4(0,D3DXToRadian(360.0f*float(rand())/float(RAND_MAX)),0,0));

	enemy->Publish();
}

Enemy::Enemy()
{
	SetReplicaObject(this);		// Setup our forward reference pointer
	mPosition = D3DXVECTOR4(0,0,0,0);
	mRotation = D3DXVECTOR4(0,0,0,0);

	mSpeed = 0.0f;
}

Enemy::~Enemy()
{
	if (IsMaster())
	{
		SpawnEnemy();
	}
}

void Enemy::PollIt(void)
{
	// Only run the "physics" if we are a master object
	if (!IsMaster())
	{
		return;
	}

	int i;
	for (i=0;i<(int)GameDatabase::mGameObjects.size();i++)
	{
		ReplicaObject *robject;
		// Get the derived class object pointer that contains a base class type of ReplicaObject from the GameObject class
		robject = GameDatabase::mGameObjects[i]->GetReplicaObject();
		// If there is no object pointer then ignore this object
		if (!robject)
		{
			continue;
		}
		// Compare the class id of the ReplicaObject with the class ID of the ReplicaObject Projectile object
		// If they are the same then we can assume the type cast to a Plane class is valid
		if (robject->GetClassID() == _MAKE_RO(Projectile)::StaticGetClassID())
		{
			D3DXVECTOR4 diff;
			Projectile *proj = (Projectile *) robject;
			diff = GetPosition() - proj->GetPosition();
			float length = D3DXVec4Length(&diff);
			if (length < 10.0f)
			{
				Delete();
//				proj->Delete();
				return;
			}
		}
	}

	if (mSpeed > 0.1f)
	{
		D3DXVECTOR4 velocity = D3DXVECTOR4(0,0,-mSpeed,0);
		mRotation.y += D3DXToRadian(1.0f) * GameDatabase::mGameFrameTick;

		// Calculate the velocity movement
		D3DXMATRIX matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix,mRotation.y,mRotation.x,mRotation.z);
		D3DXVECTOR4 trans;
		D3DXVec4Transform(&trans,&velocity,&matrix);

		mPosition += trans * GameDatabase::mGameFrameTick;
		GiveDeltaHint(mPosition.x,trans.x);
		GiveDeltaHint(mPosition.y,trans.y);
		GiveDeltaHint(mPosition.z,trans.z);

//		mRotation.y += 0.5f * GameDatabase::mGameFrameTick;
//		GiveDeltaHint(mRotation.y,0.5f * GameDatabase::mGameFrameTick);
	}

#define LIMIT (140.0f)

	if (mPosition.x < -LIMIT)
	{
		mPosition.x = 0;
		ContinuityBreak(mPosition.x,DataBlock::kTeleport);
		GiveDeltaHint(mPosition.x,0);
	}
	if (mPosition.z < -LIMIT)
	{
		mPosition.z = 0;
		ContinuityBreak(mPosition.z,DataBlock::kTeleport);
		GiveDeltaHint(mPosition.z,0);
	}
	if (mPosition.x > LIMIT)
	{
		mPosition.x = 0;
		ContinuityBreak(mPosition.x,DataBlock::kTeleport);
		GiveDeltaHint(mPosition.x,0);
	}
	if (mPosition.z > LIMIT)
	{
		mPosition.z = 0;
		ContinuityBreak(mPosition.z,DataBlock::kTeleport);
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

