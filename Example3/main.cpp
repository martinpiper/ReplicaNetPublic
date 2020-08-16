/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This example shows how to use the reliable UDP transport on its own
*/
#include <windows.h>
#include "RNXPURL/Inc/XPURL.h"
#include "RNPlatform/Inc/PlatformHeap.h"

using namespace RNReplicaNet;

int main(int argc,char **argv)
{
	// Make sure the default transports are registered otherwise FindTransport may not find the transport we are looking for
	XPURL::RegisterDefaultTransports();

	// The XPURL class enables us to find the transport we want
	XPURL xpurl;

	// Some transport pointers
	Transport *transa = 0,*transb = 0;

	// Test to make sure we have the UDP transport loaded
	if (!xpurl.FindTransport("UDP@"))
	{
		// If not loaded then report an error
		printf("Cannot find a transport to use\n");
		exit(-1);
	}

	// Create our reliable UDP listen transport
	transa = xpurl.FindTransport("UDP@")->Allocate();
	transa->Listen();		// We could force a port number to be used here e.g. transa->Listen(4444); but we let the transport pick a port number
	// Query what the transport is listening as
	std::string url = transa->ExportURL();
	printf("The listening transport URL is '%s'\n",url.c_str());

	// Create our reliable UDP transport to connect with
	transb = xpurl.FindTransport("UDP@")->Allocate();

	// Try to connect to our listen transport
	transb->Connect(url);

	// Sleep a short while to make sure the connection is established
	Sleep(50);

	// A buffer for the data we are going to send
	char buffer[256];

	// Accept the transport that connected
	Transport *ret = transa->Accept();

	// Sleep for a little while
	Sleep(50);

	// If we couldn't accept the connection then report an error
	if (!ret)
	{
		printf("UDP transa couldn't accept\n");
		delete transb;
		delete transa;
		PlatformHeap::ForceFree();
		exit(-1);
	}

	int i;
	// Send 100 packets on both transports with the reliable method
	for (i=0;i<100;i++)
	{
		ret->SendReliable(buffer,8);
		transb->SendReliable(buffer,8);
	}

	// Sleep to give the packets time to get there...
	Sleep(50);

	int gota=0,gotb=0;

	// Total up the number of packet received from each transport
	while(ret->Recv(buffer,256) > 0)
	{
		gota++;
	}

	while(transb->Recv(buffer,256) > 0)
	{
		gotb++;
	}

	// Print our results
	printf("Reliable gota = %d   gotb = %d\n",gota,gotb);


	// Send 100 packets on both transports with the certain method
	for (i=0;i<100;i++)
	{
		ret->SendCertain(buffer,8);
		transb->SendCertain(buffer,8);
	}

	// Sleep to give the packets time to get there...
	Sleep(50);

	int cgota=0,cgotb=0;

	// Total up the number of packet received from each transport
	while(ret->Recv(buffer,256) > 0)
	{
		cgota++;
	}

	while(transb->Recv(buffer,256) > 0)
	{
		cgotb++;
	}

	// Print our results
	printf("Certain gota = %d   gotb = %d\n",gota,gotb);



	// Now send 100 packets as before but using the unreliable method
	for (i=0;i<100;i++)
	{
		ret->Send(buffer,8);
		transb->Send(buffer,8);
	}

	Sleep(50);

	int ugota=0,ugotb=0;

	while( ret->Recv(buffer,256) > 0)
	{
		ugota++;
	}

	while( transb->Recv(buffer,256) > 0)
	{
		ugotb++;
	}

	// Print out the results
	printf("Unreliable gota = %d   gotb = %d\n",ugota,ugotb);

	// Finish the example
	if (gota != 100 || gotb != 100 || cgota != 100 || cgotb != 100 || ugota != 100 || ugotb != 100)
	{
		printf("Somewhere some packets got lost\n");
	}
	else
	{
		printf("All packets were received\n");
	}

	// Delete the transports
	delete ret;
	delete transb;
	delete transa;

	// Force free all internal memory and threads.
	PlatformHeap::ForceFree();

	return 0;
}
