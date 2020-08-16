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
#include "Dolphin.h"

using namespace RNReplicaNet;

extern CD3DMesh*	 gpAirplaneMesh;

Plane::Plane()
{
	mReplica = this;		// Setup our forward reference pointer
	mMesh = gpAirplaneMesh;
	mPosition = D3DXVECTOR4(0,0,0,0);
	mRotation = D3DXVECTOR4(0,0,0,0);

	// Enable the session propagation mechanism for this object
	SetSessionPropagationFilter(true);

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

	D3DXVECTOR4 velocity = D3DXVECTOR4(0,0,-0.05f,0);
	mRotation.y += D3DXToRadian(0.1f);

	// Calculate the velocity movement
	D3DXMatrixRotationYawPitchRoll(&m_matObjectMatrix,mRotation.y,mRotation.x,mRotation.z);
	D3DXVECTOR4 trans;
	D3DXVec4Transform(&trans,&velocity,&m_matObjectMatrix);
	mPosition += trans;


	// Now look for dolphin classes and update if we are to propagate to these
	int i;
	for (i=0;i<(int)gGameObjects.size();i++)
	{
		ReplicaObject *robject;

		// Get the derived class object pointer that contains a base class type of ReplicaObject from the GameObject class
		robject = gGameObjects[i]->mReplica;
		// If there is an object pointer
		if (robject)
		{
			// Compare the class id of the ReplicaObject with the class ID of the ReplicaObject Dolphin object
			if (robject->GetClassID() == _MAKE_RO(Dolphin)::StaticGetClassID())
			{
				D3DXVECTOR4 temp = GetPosition() - gGameObjects[i]->GetPosition();
				float distance = D3DXVec4Length(&temp);
				// Check the distance
				if (distance < 30.0f)
				{
					// It is within the distance so we try to propagate to the object's sessionID
					int sessionID = robject->GetSessionID();
					if (SessionPropagationAddSession(sessionID))
					{
						char buffer[256];
						sprintf(buffer,"Added %d:%d to %d\n",GetSessionID(),GetUniqueID(),sessionID);
						OutputDebugString(buffer);
					}
				}
				else
				{
					// It is outside the distance so we remove the propagate using object's sessionID
					int sessionID = robject->GetSessionID();
					if (SessionPropagationRemoveSession(sessionID))
					{
						char buffer[256];
						sprintf(buffer,"Removed %d:%d from %d\n",GetSessionID(),GetUniqueID(),sessionID);
						OutputDebugString(buffer);
					}
				}
			}
		}
	}
}
