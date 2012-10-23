#pragma once
#include "Repository_User_Includes.h"
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
			member_data_basic(int,uuid);

			member_component_basic(typename MasterType::routing_type, router);

			member_component_basic(typename MasterType::vehicle_type, vehicle);

		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Traveler_Component{typedef Polaris_Component<Traveler_Components::Implementations::Polaris_Traveler_Implementation, MasterType> type; };
	}
}