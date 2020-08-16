/* Auto-generated with RNROLCompiler V3.1.0.0 don't edit */
#include "_RO_Test1.h"
#include "Test1.h"
_RO_DO_STD_FRAMEWORK(Test1)
#include "RNReplicaNet/Inc/DataBlock_Function.h"
_RO_DO_MEMBERFUNC_PRESTAGE_DEF(Test1,APublicFunction)
int value1,
float value2
_RO_DO_MEMBERFUNC_PRESTAGE(APublicFunction)
_RO_DO_MEMBERFUNC_ADDVAR2(value1,int)
_RO_DO_MEMBERFUNC_ADDVAR2(value2,float)
_RO_DO_MEMBERFUNC_POSTSTAGE(APublicFunction)
_RO_DO_MEMBERFUNC_CRACK_START(Test1,APublicFunction)
int value1;
float value2;
_RO_DO_MEMBERFUNC_MESSAGE_CRACK(value1)
_RO_DO_MEMBERFUNC_MESSAGE_CRACK(value2)
_RO_DO_MEMBERFUNC_CRACK_END(Test1,APublicFunction)
value1,
value2
_RO_DO_MEMBERFUNC_CRACK_END2()
_RO_DO_BEGINREGISTERDATABLOCKS(Test1)
_RO_DO_REGISTERBLOCK_FUNCTION_VAR(APublicFunction)
_RO_DO_REGISTERBLOCK_END()
_RO_DO_ENDREGISTERDATABLOCKS(Test1)
