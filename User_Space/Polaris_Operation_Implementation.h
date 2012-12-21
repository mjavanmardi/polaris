#pragma once
#include "User_Space_Includes.h"
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
		implementation struct Polaris_Operation_Implementation
		{
			member_component(typename MasterType::network_type, network_reference, none, none);
		};
	}

}