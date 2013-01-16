#pragma once
#include "Traffic_Management_Center_Prototype.h"

namespace Traffic_Management_Center_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Simple_TMC:public Polaris_Component_Class<Simple_TMC,MasterType,Execution_Object>
		{

		};
	}
}

using namespace Traffic_Management_Center_Components::Implementations;