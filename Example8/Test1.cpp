/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include "Test1.h"
#include "RNReplicaNet/Inc/DataBlock_Function.h"

using namespace RNReplicaNet;

#define REPORTOBJECT()	\
if (IsMaster())	\
{	\
	printf("Test1 master  $%p SessionID %d UniqueID %d ClassID %d ",(void*)this,GetSessionID(),GetUniqueID(),GetClassID());	\
}	\
else	\
{	\
	printf("Test1 replica $%p SessionID %d UniqueID %d ClassID %d ",(void*)this,GetSessionID(),GetUniqueID(),GetClassID());	\
}



Test1::Test1()
{
	REPORTOBJECT();
	printf("Test1() created\n");
}

Test1::~Test1()
{
	REPORTOBJECT();
	printf("~Test1() deleted\n");
}

bool first = true;

void Test1::RunTest(const int numSessionIDs,const int *sessionIDs)
{
	REPORTOBJECT();
	printf("RunTest() Called\n");
	float random_float;
	int random_int;

	random_int = rand();
	random_float = float(rand()) / float(RAND_MAX);
	/* This calls the function only on the nominated replicas */
	DataBlock_Function::SetSessionIDFilter(numSessionIDs,sessionIDs);
	ALL_REPLICAS_FUNCTION_CALL(APublicFunction(random_int,random_float));

	// The same functionality is offered by using the macro
	NOMINATED_REPLICAS_FUNCTION_CALL(numSessionIDs,sessionIDs,APublicFunction(random_int,random_float));
}

void Test1::APublicFunction(int ivalue,float fvalue)
{
	REPORTOBJECT();
	printf("APublicFunction() : Called with %d,%f\n",ivalue,fvalue);
}
