/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __TEST1_H__
#define __TEST1_H__

#include "ExampleCustomType.h"
#include "_RO_Test1.h"

// ExampleCustomType.h is also included by _RO_Test1.h


class Test1 : _RO_DO_PUBLIC_RO(Test1)
{
public:
	Test1();
	virtual ~Test1();

	void RunTest(void);

	void APublicFunction(int ivalue,float fvalue);

	void APublicFunctionWithCustomTypeByValue(ExampleCustomType thing,int anotherValue);

	void APublicFunctionWithCustomTypeByReference(ExampleCustomType &thing,int anotherValue);

private:
_RO_DO_ALLOW_FRIEND_RO(Test1)

	void APrivateFunction(float fvalue);


	// This is called if or when ReplicaNet changes the owner of this object
	void OwnerChange(void);

	// This function can be called to approve a fault recovery for the object.
	// However the "ApproveFault();" line in _RO_Test1.rol also does this so this function is not needed here
	/*
	bool ApproveFault(void)
	{
		return true;
	}
	*/
};

#endif
