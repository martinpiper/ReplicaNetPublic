/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
See MyReplicaNet.cpp
*/
#ifndef _MYREPLICANET_H_
#define _MYREPLICANET_H_
#include <stdio.h>
#include "RNReplicaNet/Inc/ReplicaNet.h"
#include "RNPlatform/Inc/Thread.h"
#include "RNPlatform/Inc/ThreadClass.h"

// Create our own ReplicaNet derived class to implement compressed recordings.
class MyReplicaNet : public RNReplicaNet::ReplicaNet
{
public:
	MyReplicaNet();
	virtual ~MyReplicaNet();

	/// Sets the file pointer used for the recording.
	void SetRecordingFilePointer(FILE *fp);

	void Flush(void);

	/// The recording callbacks from ReplicaNet
	virtual bool CallbackSessionRecord(const void *data,const int length);
	virtual bool CallbackSessionPlayback(void *data,const int length);

	enum
	{
		kBufferSize = 64 * 1024			// 64K of buffer
	};

	class Writer : public RNReplicaNet::Thread , public RNReplicaNet::ThreadClass
	{
	public:
		Writer();
		virtual ~Writer();

		/// Forces any data in the buffer to be compressed and saved to the file.
		void ForceCompress(void);

		virtual int ThreadEntry(void);

		size_t mOutBytesUsed;				// The number of output bytes used in the buffer.
		char mBuffer[kBufferSize];
		FILE *mFP;
	};

	Writer mWriter;

private:
	FILE *mFP;
	char mAccumulateBuffer[kBufferSize];
	size_t mAccumulateBytesUsed;				// The number of output bytes used in the buffer.
	size_t mInBytesUsed;				// The number of output bytes used in the buffer.
	char *mInBufferPosition;			// The input buffer position pointer.
};

#endif
