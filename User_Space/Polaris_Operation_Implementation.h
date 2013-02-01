#pragma once
#include "Operation_Prototype.h"

namespace Operation_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Operation_Implementation:public Polaris_Component_Class<Polaris_Operation_Implementation,MasterType,Data_Object,ParentType>
		{
			member_component(typename MasterType::network_type, network_reference, none, none);
		};
	}

}