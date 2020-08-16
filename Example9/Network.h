/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
See Network.cpp
*/
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <string>
#include "RNReplicaNet/Inc/ReplicaNet.h"

extern RNReplicaNet::ReplicaNet *gNetwork;

extern void Network_Init(std::string url);
extern void Network_Dialog(void);

#endif
