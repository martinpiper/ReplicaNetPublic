/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __ANOBJECT_H__
#define __ANOBJECT_H__

#include "_RO_AnObject.h"

#include "DataBlock_WatchedFloat.h"
#include "NetworkVector3.h"
#include "DataBlock_STL.h"
#include <list>
#include <vector>

class AnObject : _RO_DO_PUBLIC_RO(AnObject)
{
public:
	AnObject();
	virtual ~AnObject();

	// This is the new WatchedFloat DataBlock
	WatchedFloat mWatchedValue;

	// The new DataBlock self registers without using the ROL
	WatchedFloat mWatchedValueSelfRegister;

	// This is just an example of a normal integer that is an NData DataBlock
	int mAnIntValue;

	NetworkVector3 mVector;

	std::list<float> mList;
	DataBlock_STL<std::list<float> > mDataBlockList;

	std::vector<float> mSTLVector;		// This variable is declared in the _RO_AnObject.rol file.

	// This is called by ReplicaNet.
	void UserRegisterDataBlocks(void);
};


#endif
