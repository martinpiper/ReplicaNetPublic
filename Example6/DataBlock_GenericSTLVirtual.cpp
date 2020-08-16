/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include "RNPlatform/Inc/MemoryTracking.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "DataBlock_GenericSTLVirtual.h"
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "RNPlatform/Inc/MessageHelper.h"
#include "RNPlatform/Inc/MemoryTracking.h"

using namespace RNReplicaNet;

DataBlock_GenericSTLVirtual::DataBlock_GenericSTLVirtual() : mData(0)
{
	// Because we are going to be updating an stl list or vector then we want to make sure updates are in order.
	SetReliableFlag(true);
}

DataBlock_GenericSTLVirtual::~DataBlock_GenericSTLVirtual()
{
	FreeSessionBlocks();
}

void DataBlock_GenericSTLVirtual::FreeSessionBlocks(void)
{
	std::map<int,SessionBlock *>::iterator st,en;
	st = mPerSessionData.begin();
	en = mPerSessionData.end();

	// Delete the remaining session blocks attached to this datablock
	while(st != en)
	{
		SessionBlock *sessdataptr = (*st++).second;
		delete sessdataptr;
	}
	mPerSessionData.clear();
}

void DataBlock_GenericSTLVirtual::Poll(DataBlockReason *reason,const bool discardOlderIncluded,const unsigned int discardOrder)
{
	if (!reason)
	{
		return;
	}

	switch (reason->mReasonCode)
	{
		case kDataBlockReason_SomeoneJoined:
		{
			if (!reason->mMessageHelper)
			{
				return;
			}
			SessionBlock *sessdataptr = new SessionBlock();
			DoInitialSettingsFor(sessdataptr,GetElementSize());
			// Insert SessionBlock into the std::map with the key reason->mSessionReference
			mPerSessionData[reason->mSessionReference] = sessdataptr;

			// Someone joined so calculate an immediate update
			DataBlock::Poll(reason,true,++(sessdataptr->mDiscardOrder));
			// Number of elements
			size_t numElements = sessdataptr->mOrderedElementData.size();
			reason->mMessageHelper->AddVariable(&numElements,sizeof(numElements));
			// Number of elements changed
			reason->mMessageHelper->AddVariable(&numElements,sizeof(numElements));
			size_t i;
			for (i=0;i<numElements;i++)
			{
				reason->mMessageHelper->AddVariable(&i,sizeof(i));
				reason->mMessageHelper->AddVariable(sessdataptr->mOrderedElementData[i],GetElementSize());
			}

			return;
		}

		case kDataBlockReason_SomeoneLeft:
		{
			std::map<int,SessionBlock *>::iterator found;
			found = mPerSessionData.find(reason->mSessionReference);
			if (found != mPerSessionData.end())
			{
				SessionBlock *sessdataptr = (*found).second;
				assert ((*found).first == reason->mSessionReference);
				mPerSessionData.erase(found);
				delete sessdataptr;
				return;
			}
			break;
		}

		case kDataBlockReason_RecordingNormalPoll:
		{
			// Send the best of what we have so far. This happens to be the current replica state.
			DataBlock::Poll(reason);
			size_t numElements = GetNumberOfElements();
			reason->mMessageHelper->AddVariable(&numElements,sizeof(numElements));
			// Number of elements changed
			reason->mMessageHelper->AddVariable(&numElements,sizeof(numElements));
			size_t i;
			for (i=0;i<numElements;i++)
			{
				reason->mMessageHelper->AddVariable(&i,sizeof(i));
				reason->mMessageHelper->AddVariable(GetElementPointerAt(i),GetElementSize());
			}
			break;
		}

		case kDataBlockReason_NormalPoll:
		{
			if (!reason->mMessageHelper)
			{
				return;
			}

			std::map<int,SessionBlock *>::iterator found;
			found = mPerSessionData.find(reason->mSessionReference);
			if (found != mPerSessionData.end())
			{
				SessionBlock *sessdataptr = (*found).second;
				assert ((*found).first == reason->mSessionReference);

				size_t numElementsChanged = 0;
				bool theSizeChanged = false;
				size_t i;
				// Count the number of elements changed in the existing data
				for (i=0 ; i<GetNumberOfElements() && i < sessdataptr->mOrderedElementData.size() ; i++)
				{
					// If the data is the same then do nothing
					if (memcmp(sessdataptr->mOrderedElementData[i],GetElementPointerAt(i),GetElementSize()) != 0)
					{
						numElementsChanged++;
					}
				}

				// Detect length changes and add on the extra length if it grew
				if (GetNumberOfElements() != sessdataptr->mOrderedElementData.size())
				{
					theSizeChanged = true;
					if (GetNumberOfElements() > sessdataptr->mOrderedElementData.size())
					{
						numElementsChanged += (GetNumberOfElements() - sessdataptr->mOrderedElementData.size());
					}
				}

				// If either the size or the contents have changed then
				if (theSizeChanged || numElementsChanged > 0)
				{
					// Send the update data
					DataBlock::Poll(reason,true,++(sessdataptr->mDiscardOrder));
					// Number of elements
					size_t numElements = GetNumberOfElements();
					reason->mMessageHelper->AddVariable(&numElements,sizeof(numElements));
					// Number of elements changed
					reason->mMessageHelper->AddVariable(&numElementsChanged,sizeof(numElementsChanged));
					for (i=0 ; i<numElements && i < sessdataptr->mOrderedElementData.size() ; i++)
					{
						if (memcmp(sessdataptr->mOrderedElementData[i],GetElementPointerAt(i),GetElementSize()) != 0)
						{
							reason->mMessageHelper->AddVariable(&i,sizeof(i));
							reason->mMessageHelper->AddVariable(GetElementPointerAt(i),GetElementSize());
							numElementsChanged--;
						}
					}
					// Add on the extra length changes if required
					while (numElementsChanged > 0)
					{
						reason->mMessageHelper->AddVariable(&i,sizeof(i));
						reason->mMessageHelper->AddVariable(GetElementPointerAt(i),GetElementSize());
						numElementsChanged--;
						i++;
					}

					// After sending the update then copy what we sent
					DoInitialSettingsFor(sessdataptr,GetElementSize());
				}

				return;
			}
			break;
		}

		default:
			break;
	}
}

void DataBlock_GenericSTLVirtual::ParseMessage(MessageHelper *message)
{
	// Decode our updates
	int newSize;
	int numChanged;
	message->GetVariable(&newSize,sizeof(newSize));
	message->GetVariable(&numChanged,sizeof(numChanged));

	printf("Got update newSize = %d numChanged = %d\n",newSize,numChanged);

	if (!GetDiscardThisUpdate())
	{
		SetNumberOfElements(newSize);
	}
	int i;
	for (i=0;i<numChanged;i++)
	{
		int index;
		message->GetVariable(&index,sizeof(index));
		if (GetDiscardThisUpdate())
		{
			message->SkipData(GetElementSize());
		}
		else
		{
			void *data = GetElementPointerAt(index);
			message->GetVariable(data,GetElementSize());
		}
	}
}

DataBlock_GenericSTLVirtual::SessionBlock::SessionBlock() : mDiscardOrder(0)
{
}

DataBlock_GenericSTLVirtual::SessionBlock::~SessionBlock()
{
	size_t i;
	for (i=0;i<mOrderedElementData.size();i++)
	{
		free(mOrderedElementData[i]);
	}
}

void DataBlock_GenericSTLVirtual::DoInitialSettingsFor(SessionBlock *sessionBlock,const int elementSize)
{
	// First free all of our blocks
	size_t oldSize = sessionBlock->mOrderedElementData.size();
	size_t i;
	// If we want to shrink then free the old blocks
	for (i=0;i<oldSize;i++)
	{
		free (sessionBlock->mOrderedElementData[i]);
		sessionBlock->mOrderedElementData[i] = 0;
	}

	sessionBlock->mOrderedElementData.resize(GetNumberOfElements());

	// Handle the grown elements
	for (i=0;i<sessionBlock->mOrderedElementData.size();i++)
	{
		sessionBlock->mOrderedElementData[i] = malloc(elementSize);
		void *data = GetElementPointerAt(i);
		memcpy(sessionBlock->mOrderedElementData[i],data,elementSize);
	}
}

void DataBlock_GenericSTLVirtual::OwnerChanged(const int sessionID)
{
	DataBlock::OwnerChanged(sessionID);
	FreeSessionBlocks();
}

