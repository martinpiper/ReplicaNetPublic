/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include "AnObject.h"
using namespace RNReplicaNet;

AnObject::AnObject()
{
	mWatchedValue.SetValue(0);
	mWatchedValueSelfRegister.SetValue(0);
	mAnIntValue = 0;

	// Register the extended DataBlock variable with ReplicaNet without using ROL.
	AddAutoRegisterDataBlock(&mWatchedValueSelfRegister);

	// This function call on the DataBlock uses the ReplicaObject pointer to pre-register the three datablocks used for the three vector components.
	mVector.AddAutoRegisterDataBlock(this);

	mDataBlockList.SetIsAllocated(false);
	// This ensures that any changes in this STL list are done in order
	mDataBlockList.RegisterInClassPointer(&mList);
//	AddAutoRegisterDataBlock(&mDataBlockList);
}

AnObject::~AnObject()
{
}

void AnObject::UserRegisterDataBlocks(void)
{
	RegisterDataBlock(&mDataBlockList);
}
