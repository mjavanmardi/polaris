#pragma once
#include "Demand_Prototype.h"

namespace Demand_Components
{
	namespace Types
	{


	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		template<typename MasterType>
		struct Polaris_Demand_Implementation
		{

			member_component_basic(typename MasterType::scenario_type, scenario_reference);

			member_component_basic(typename MasterType::network_type, network_reference);

			typedef typename MasterType::traveler_type traveler_type;
		};


	}
	
	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Demand_Component{ typedef Polaris_Component<Demand_Components::Implementations::Polaris_Demand_Implementation, MasterType> type; };
	}	
}