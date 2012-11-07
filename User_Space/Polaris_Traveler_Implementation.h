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
			member_data(int, uuid, none, none);

			member_component(typename MasterType::routing_type, router, none, none);

			member_component(typename MasterType::vehicle_type, vehicle, none, none);

		};
	}

	namespace Components
	{
	}
}