#ifndef _EXAMPLECUSTOMTYPE_H_
#define _EXAMPLECUSTOMTYPE_H_

#include <string>

class ExampleCustomType
{
public:
	float mX,mY,mZ;
	std::string mText;
};

// This header is important to define the FunctionAddVariable and FunctionGetVariable templates
#include "RNReplicaNet/Inc/DataBlock_Function.h"

// Specialise these two templates to allow ExampleCustomType to be used with DataBlock_Function
namespace RNReplicaNet
{
template<>
void FunctionAddVariable<ExampleCustomType>(DynamicMessageHelper &message, ExampleCustomType &variable);

template<>
void FunctionGetVariable<ExampleCustomType>(MessageHelper &message, ExampleCustomType &variable);
}


#endif
