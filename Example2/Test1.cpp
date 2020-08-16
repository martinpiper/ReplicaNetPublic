/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include <stdio.h>
#include <stdlib.h>
#include "Test1.h"

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
	printf("Test1() Called\n");
}

Test1::~Test1()
{
	REPORTOBJECT();
	printf("~Test1() Called\n");
}

void Test1::RunTest(void)
{
	REPORTOBJECT();
	printf("RunTest() Called\n");
	float random_float;
	int random_int;
	random_float = float(rand()) / float(RAND_MAX);
	/* This calls the function on the master and replicas because this RunTest() function is called on the master copy. */
	GLOBAL_FUNCTION_CALL(APrivateFunction(random_float));

	random_int = rand();
	random_float = float(rand()) / float(RAND_MAX);
	/* This calls the function on the master and replicas because this RunTest() function is called on the master copy. */
	GLOBAL_FUNCTION_CALL(APublicFunction(random_int,random_float));
	/* This calls the function only on the replicas because this RunTest() function is called on the master copy. */
	/* Because APublicFunction is marked as Reliable this shows that Reliable networked functions will propagate updates in the same order they were made. */
	ALL_REPLICAS_FUNCTION_CALL(APublicFunction(1,4.0f));
	ALL_REPLICAS_FUNCTION_CALL(APublicFunction(2,5.0f));
	ALL_REPLICAS_FUNCTION_CALL(APublicFunction(3,6.0f));
	ALL_REPLICAS_FUNCTION_CALL(APublicFunction(4,7.0f));

	ExampleCustomType thing;
	thing.mX = 1.0f;
	thing.mY = 2.0f;
	thing.mZ = 3.0f;
	thing.mText = "Hello world";
	ALL_REPLICAS_FUNCTION_CALL(APublicFunctionWithCustomTypeByValue(thing,4));

	ALL_REPLICAS_FUNCTION_CALL(APublicFunctionWithCustomTypeByReference(thing,4));
}

void Test1::APrivateFunction(float fvalue)
{
	REPORTOBJECT();
	printf("APrivateFunction() : Called with %f\n",fvalue);
}

void Test1::APublicFunction(int ivalue,float fvalue)
{
	REPORTOBJECT();
	printf("APublicFunction() : Called with %d,%f\n",ivalue,fvalue);
}

void Test1::APublicFunctionWithCustomTypeByValue(ExampleCustomType thing,int anotherValue)
{
	REPORTOBJECT();
	printf("APublicFunctionWithCustomTypeByValue() : Called with %f,%f,%f,%s and %d\n",thing.mX,thing.mY,thing.mZ,thing.mText.c_str(),anotherValue);
}

void Test1::APublicFunctionWithCustomTypeByReference(ExampleCustomType &thing,int anotherValue)
{
	REPORTOBJECT();
	printf("APublicFunctionWithCustomTypeByReference() : Called with %f,%f,%f,%s and %d\n",thing.mX,thing.mY,thing.mZ,thing.mText.c_str(),anotherValue);
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
