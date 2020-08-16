/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include "NetworkVector3.h"
#include "RNReplicaNet/Inc/ReplicaObject.h"

using namespace RNReplicaNet;

NetworkVector3::NetworkVector3()
{
	mX = 0;
	mY = 0;
	mZ = 0;

	mFloats[0].SetIsAllocated(false);		// When the ReplicaObject is deleted the dtor will not use 'delete' to free this DataBlock
	mFloats[0].RegisterInClassPointer(&mX);

	mFloats[1].SetIsAllocated(false);
	mFloats[1].RegisterInClassPointer(&mY);

	mFloats[2].SetIsAllocated(false);
	mFloats[2].RegisterInClassPointer(&mZ);
}

NetworkVector3::~NetworkVector3()
{
}

void NetworkVector3::AddAutoRegisterDataBlock(ReplicaObject *object)
{
	object->AddAutoRegisterDataBlock(&mFloats[0]);
	object->AddAutoRegisterDataBlock(&mFloats[1]);
	object->AddAutoRegisterDataBlock(&mFloats[2]);
}
