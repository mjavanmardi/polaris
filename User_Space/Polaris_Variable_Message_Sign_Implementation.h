#pragma once
#include "Variable_Message_Sign_Prototype.h"

namespace Variable_Message_Sign_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Variable_Speed_Sign:public Polaris_Component_Class<Variable_Speed_Sign,MasterType,Execution_Object>
		{

		};

		implementation struct Variable_Word_Sign:public Polaris_Component_Class<Variable_Word_Sign,MasterType,Execution_Object>
		{

		};
	}
}

using namespace Variable_Message_Sign_Components::Implementations;