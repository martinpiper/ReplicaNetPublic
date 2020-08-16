/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __DATABLOCK_GENERICSTLVIRTUAL_H__
#define __DATABLOCK_GENERICSTLVIRTUAL_H__

#include "RNReplicaNet/Inc/DataBlock.h"
#include <map>
#include <vector>

/**
 * This class provides, at a generic void* level, element tracking and message handling routines with pure virtual hooks
 * intended for more specific derived container updates.
 */
class DataBlock_GenericSTLVirtual : public RNReplicaNet::DataBlock
{
public:
	RN_CLASS_MINIMAL_RTTI();
	DataBlock_GenericSTLVirtual();

	virtual ~DataBlock_GenericSTLVirtual();

	virtual void Poll(RNReplicaNet::DataBlockReason *reason,const bool discardOlderIncluded = false,const unsigned int discardOrder = 0);

	virtual void ParseMessage(RNReplicaNet::MessageHelper *message);

protected:
	virtual size_t GetNumberOfElements(void) = 0;

	virtual void SetNumberOfElements(const size_t numElements) = 0;

	virtual size_t GetElementSize(void) const = 0;

	virtual void *GetElementPointerAt(const size_t offset) = 0;

	virtual void SetElementDataAt(const void *data,const size_t offset) = 0;

	struct SessionBlock
	{
		SessionBlock();

		virtual ~SessionBlock();

		unsigned int mDiscardOrder;
		std::vector<void *> mOrderedElementData;
	};

// Ctor variables

	void *mData;

// Other variables

	std::map<int,SessionBlock*> mPerSessionData;

	virtual void OwnerChanged(const int sessionID);

private:
	void DoInitialSettingsFor(SessionBlock *sessionBlock,const int elementSize);
	void FreeSessionBlocks(void);
};

#endif
