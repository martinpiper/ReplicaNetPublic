/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef _NETWORKVECTOR3_H_
#define _NETWORKVECTOR3_H_

namespace RNReplicaNet
{
class ReplicaObject;
}

#include "RNReplicaNet/Inc/DataBlock_Predict_Float.h"

/**
 * This class is to demonstrate how to create a vector that also has networked capabilities. The class is not intended to be included as part of the ROL language but rather directly included in the object and used.<br>
 * When the member variables 
 */
class NetworkVector3
{
public:
	NetworkVector3();

	virtual ~NetworkVector3();

	/**
	 * This function pre-registers the three DataBlock classes with the object
	 */
	void AddAutoRegisterDataBlock(RNReplicaNet::ReplicaObject *object);

	// Normally class members variables shouldn't be public but in this case a vector class can be the exception to the rule
	float mX,mY,mZ;

private:
	RNReplicaNet::DataBlock_Predict_Float mFloats[3];
};

#endif
