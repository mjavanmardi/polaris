#pragma once
#include "Plan_Prototype.h"

namespace Plan_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Polaris_Plan_Implementation
		{
			member_component(typename MasterType::movement_plan_type, movement_plan, none, none);
			member_component(typename MasterType::traveler_type, traveler, none, none);
		};
	}
}