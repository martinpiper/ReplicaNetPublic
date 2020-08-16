/* Auto-generated with RNROLCompiler V3.1.0.0 don't edit */
#include "_RO_Plane.h"
#include "Plane.h"
_RO_DO_STD_FRAMEWORK(Plane)
#include "RNReplicaNet/Inc/DataBlock_Predict_Float.h"
#include "RNReplicaNet/Inc/DataBlock_NData.h"
#include "RNReplicaNet/Inc/DataBlock_Function.h"
_RO_DO_MEMBERFUNC_PRESTAGE_DEF(Plane,AMemberFunction)
float value1
_RO_DO_MEMBERFUNC_PRESTAGE(AMemberFunction)
_RO_DO_MEMBERFUNC_ADDVAR2(value1,float)
_RO_DO_MEMBERFUNC_POSTSTAGE(AMemberFunction)
_RO_DO_MEMBERFUNC_CRACK_START(Plane,AMemberFunction)
float value1;
_RO_DO_MEMBERFUNC_MESSAGE_CRACK(value1)
_RO_DO_MEMBERFUNC_CRACK_END(Plane,AMemberFunction)
value1
_RO_DO_MEMBERFUNC_CRACK_END2()
_RO_DO_BEGINREGISTERDATABLOCKS(Plane)
_RO_DO_REGISTERBLOCK_FUNCTION_VAR(AMemberFunction)
_RO_DO_DATABLOCK_MAKERELIABLE()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_VAR(mPosition.x)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINERROR(0.5f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINDISTANCE(50.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXERROR(10.5f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXDISTANCE(200.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETQUADRATIC()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETSNAPTODIFFERENCE(10.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETDELTASHORTFLOAT()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_VAR(mPosition.y)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINERROR(0.5f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINDISTANCE(50.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXERROR(10.5f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXDISTANCE(200.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETQUADRATIC()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETSNAPTODIFFERENCE(10.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETDELTASHORTFLOAT()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_VAR(mPosition.z)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINERROR(0.5f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINDISTANCE(50.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXERROR(10.5f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXDISTANCE(200.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETQUADRATIC()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETSNAPTODIFFERENCE(10.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETDELTASHORTFLOAT()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_VAR(mRotation.x)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINERROR(0.01f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINDISTANCE(0.1f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXERROR(0.15f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXDISTANCE(50.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETQUADRATIC()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETSNAPTODIFFERENCE(0.1f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETDELTASHORTFLOAT()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_VAR(mRotation.y)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINERROR(0.01f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINDISTANCE(0.1f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXERROR(0.15f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXDISTANCE(50.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETQUADRATIC()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETSNAPTODIFFERENCE(0.1f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETDELTASHORTFLOAT()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_VAR(mRotation.z)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINERROR(0.01f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMINDISTANCE(0.1f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXERROR(0.15f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETMAXDISTANCE(50.0f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETQUADRATIC()
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETSNAPTODIFFERENCE(0.1f)
_RO_DO_REGISTERBLOCK_PREDICT_FLOAT_SETDELTASHORTFLOAT()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_NDATA_VAR(mPlayerNumber)
_RO_DO_DATABLOCK_MAKERELIABLE()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_NDATA_VAR(mPlayerColour)
_RO_DO_DATABLOCK_MAKERELIABLE()
_RO_DO_REGISTERBLOCK_END()
_RO_DO_REGISTERBLOCK_NDATA_VAR(mPlayerEnergy)
_RO_DO_REGISTERBLOCK_END()
_RO_DO_ENDREGISTERDATABLOCKS(Plane)