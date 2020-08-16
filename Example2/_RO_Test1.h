/* Auto-generated with RNROLCompiler V3.1.0.0 don't edit */
#ifndef ___RO_Test1_h__
#define ___RO_Test1_h__
#include "RNReplicaNet/Inc/ReplicaObject.h"
#include "RNReplicaNet/Inc/Define_RO.h"
#include "ExampleCustomType.h" // Added by _RO_Test1.rol line 4
_RO_DEF_ALLOCATEFUNC(Test1)
class _MAKE_BASE(Test1);
class _RO_Test1 : public RNReplicaNet::ReplicaObject
{
	_RO_STD_FRAMEWORK(Test1)
	_RO_DO_DEFBLOCK_FUNCTION_VAR(APrivateFunction)
	void Call_APrivateFunction(float);
	typedef void(tDBFV_APrivateFunction)(float);
	_RO_DO_DEFBLOCK_FUNCTION_VAR(APublicFunction)
	void Call_APublicFunction(int,float);
	typedef void(tDBFV_APublicFunction)(int,float);
	_RO_DO_DEFBLOCK_FUNCTION_VAR(APublicFunctionWithCustomTypeByValue)
	void Call_APublicFunctionWithCustomTypeByValue(ExampleCustomType,int);
	typedef void(tDBFV_APublicFunctionWithCustomTypeByValue)(ExampleCustomType,int);
	_RO_DO_DEFBLOCK_FUNCTION_VAR(APublicFunctionWithCustomTypeByReference)
	void Call_APublicFunctionWithCustomTypeByReference(ExampleCustomType&,int);
	typedef void(tDBFV_APublicFunctionWithCustomTypeByReference)(ExampleCustomType&,int);
	_RO_DEF_REGISTERDATABLOCKS(Test1)
_RO_DO_APPROVEFAULT()
};
#endif
