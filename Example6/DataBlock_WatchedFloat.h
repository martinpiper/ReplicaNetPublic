/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __DATABLOCK_WATHCEDFLOAT_H__
#define __DATABLOCK_WATHCEDFLOAT_H__

#include "RNReplicaNet/Inc/DataBlock_Predict_Float.h"

/**
 * This class is an example of how to expand the standard DataBlock derived classes to provide a custom interface.
 */
class WatchedFloat : public RNReplicaNet::DataBlock_Predict_Float
{
public:
	WatchedFloat()
	{
		// This class is not designed to be newed in the normal way. It is designed to be included as part of an object as a member variable
		SetIsAllocated(false);		// When the ReplicaObject is deleted the dtor will not use 'delete' to free this DataBlock
		RegisterInClassPointer(&mValue);
		mUpdated = false;
	}

	~WatchedFloat()
	{
	}

	// Because we get a message from ReplicaNet the value of this DataBlock is about to change
	virtual void ParseMessage(RNReplicaNet::MessageHelper *message)
	{
		if (!GetDiscardThisUpdate())
		{
			mUpdated = true;
		}
		// Now call the real DataBlock below us to parse the message
		DataBlock_Predict_Float::ParseMessage(message);
	}

	/**
	 * This reads the updated flag and resets the flag back to false
	 * \return returns the changed flag
	 */
	bool GetUpdated(void)
	{
		bool retVal = mUpdated;
		mUpdated = false;
		return retVal;
	}

	/**
	 * Gets the value
	 * \return the value
	 */
	float GetValue(void) const
	{
		return mValue;
	}

	/**
	 * Sets the value
	 * \param value the value to set
	 */
	void SetValue(const float value)
	{
		mValue = value;
	}

private:
	bool mUpdated;
	float mValue;
};

// These two macros are used by the ROL compiled object to reference the DataBlock class and to register it with ReplicaNet.
#define EXTENSION_WATCHEDFLOATCTOR(x)	\
	RegisterDataBlock(&(basepoint->x));

#define EXTENSION_WATCHEDFLOATDTOR()

#endif
