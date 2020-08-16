/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include "RNPlatform/Inc/MemoryTracking.h"
#include "Test1.h"

#define REPORTOBJECT()	\
if (IsMaster())	\
{	\
	printf("Test1 master  $%p SessionID %d UniqueID %d ClassID %d \n\n",this,GetSessionID(),GetUniqueID(),GetClassID());	\
}	\
else	\
{	\
	printf("Test1 replica $%p SessionID %d UniqueID %d ClassID %d \n\n",this,GetSessionID(),GetUniqueID(),GetClassID());	\
}



Test1::Test1()
{
	REPORTOBJECT();
	printf("Test1() Called\n");
}

Test1::~Test1()
{
	REPORTOBJECT();
	printf("~Test1() Called\n");
}

void Test1::OwnerChange(void)
{
	REPORTOBJECT();
	printf("Owner changed to be sessid %d\n",GetSessionID());
	if (IsMaster())
	{
		printf("as a master\n");
	}
	else
	{
		printf("as a replica\n");
	}
}

void Test1::ReportTest1()
{
	REPORTOBJECT();
}
