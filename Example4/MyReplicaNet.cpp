/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This expands ReplicaNet by adding compressed storage of data for recording and playback.
*/
#include <assert.h>
#include "MyReplicaNet.h"
#include "RNXPCompression/Inc/XPCompression.h"
#include "RNPlatform/Inc/MessageHelper.h"

MyReplicaNet::Writer::Writer() : mOutBytesUsed(0) , mFP(0)
{
}

MyReplicaNet::Writer::~Writer()
{
}

void MyReplicaNet::Writer::ForceCompress(void)
{
	// Nothing to compress so don't do anything.
	if (mOutBytesUsed == 0)
	{
		return;
	}

	RNReplicaNet::XPCompression compress;

	// Allocate enough buffer to accept the worst case compressed size plus an extra int for the size of the compressed chunk.
	char *tempBuffer = (char *)malloc(compress.CalculateMaximumMemory(mOutBytesUsed)+sizeof(int));
	assert(tempBuffer);
	int resultLength = 0;
	compress.Compress(mBuffer,mOutBytesUsed,tempBuffer+sizeof(int),&resultLength);
	// The message helper class preserves endian.
	RNReplicaNet::MessageHelper message;
	message.SetBuffer(tempBuffer);
	message.AddInteger(resultLength);

	// Write the compressed data and header.
	fwrite(tempBuffer,1,resultLength+sizeof(int),mFP);

	// Free the working buffer.
	free(tempBuffer);

	// Reset the number of bytes used in the buffer to zero.
	mOutBytesUsed = 0;
}

int MyReplicaNet::Writer::ThreadEntry(void)
{
	ForceCompress();
	return 0;
}

// MyReplicaNet implementation.
MyReplicaNet::MyReplicaNet() : mFP(0) , mAccumulateBytesUsed(0) , mInBytesUsed(0) , mInBufferPosition(0)
{
}

MyReplicaNet::~MyReplicaNet()
{
	// It is safe to call these two functions even if a session is not being recorded or playing back.
	StopSessionRecord();
	StopSessionPlayback();

	// Make sure any remaining data in the buffer is compressed to the file.
	while(mWriter.GetIsRunning())
	{
		Sleep(1);
	}
	mWriter.mOutBytesUsed = mAccumulateBytesUsed;
	memcpy(mWriter.mBuffer,mAccumulateBuffer,mWriter.mOutBytesUsed);
	mWriter.ForceCompress();

	// Close any open recording file if it exists.
	if (mFP)
	{
		fclose(mFP);
		mFP = 0;
	}
}

void MyReplicaNet::SetRecordingFilePointer(FILE *fp)
{
	mFP = fp;
	mWriter.mFP = fp;
}

void MyReplicaNet::Flush(void)
{
	while(mWriter.GetIsRunning())
	{
		Sleep(1);
	}
	mWriter.mOutBytesUsed = mAccumulateBytesUsed;
	memcpy(mWriter.mBuffer,mAccumulateBuffer,mWriter.mOutBytesUsed);
	mAccumulateBytesUsed = 0;
	mWriter.Begin(&mWriter);
}

bool MyReplicaNet::CallbackSessionRecord(const void *data,const int length)
{
	if (!mFP)
	{
		return false;
	}
	if ( (length + mAccumulateBytesUsed) >= kBufferSize)
	{
		Flush();
	}

	// Copy the data in to the buffer.
	memcpy(mAccumulateBuffer + mAccumulateBytesUsed,data,(size_t)length);
	// Increment the number of bytes used in the buffer.
	mAccumulateBytesUsed += length;
	return true;
}

bool MyReplicaNet::CallbackSessionPlayback(void *data,const int length)
{
	if (!mFP)
	{
		return false;
	}

	// If there is no input data then try to get some from the file.
	if (!mInBufferPosition)
	{
		// First reset the input variables to a known state.
		mInBufferPosition = mAccumulateBuffer;
		mInBytesUsed = 0;

		// Read the header size
		size_t nextChunkSize = 0;
		int temp;
		size_t ret = fread(&temp,1,sizeof(temp),mFP);
		// Failed to read the header size so return false.
		if (ret != sizeof(int))
		{
			return false;
		}

		// Read the endian preserved header size
		RNReplicaNet::MessageHelper message;
		message.SetBuffer(&temp);
		nextChunkSize = (size_t)message.GetInteger();

		// Allocate enough buffer for the compressed data
		char *tempBuffer = (char *)malloc(nextChunkSize);
		assert(tempBuffer);
		// Read the compressed data
		ret = fread(tempBuffer,1,nextChunkSize,mFP);
		// Failed to read the header size so return false.
		if (ret != nextChunkSize)
		{
			return false;
		}
		int resultLength = 0;
		// Decompress the data and mInBytesUsed is updated with how much real data there is.
		RNReplicaNet::XPCompression compress;
		compress.Decompress(tempBuffer,nextChunkSize,mAccumulateBuffer,&resultLength,kBufferSize);
		mInBytesUsed = (size_t)resultLength;
		free(tempBuffer);
	}

	// If we have really run out of data then return false.
	if (mInBytesUsed < (size_t)length)
	{
		return false;
	}

	// Check we have a valid input buffer position
	assert(mInBufferPosition);

	// Copy the data from the input buffer.
	memcpy(data,mInBufferPosition,length);

	// Prepare the input buffer length and position for the next time this function is called.
	mInBytesUsed -= length;
	mInBufferPosition += length;
	if (mInBytesUsed == 0)
	{
		// If there is no more data in this buffer then force a fetch of the next chunk.
		mInBufferPosition = 0;
	}
	return true;
}
