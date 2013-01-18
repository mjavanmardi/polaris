#pragma once
#include "User_Space_Includes.h"
#include "Traveler_Prototype.h"

namespace Traveler_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Traveler_Implementation
		{
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);

			member_component(typename MasterType::routing_type, router, none, none);
			member_component(typename MasterType::vehicle_type, vehicle, none, none);
			member_component(typename MasterType::plan_type, plan, none, none);
		};
	}

}