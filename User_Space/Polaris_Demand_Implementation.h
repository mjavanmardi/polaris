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
		implementation struct Polaris_Demand_Implementation
		{

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);

			member_component(typename MasterType::network_type, network_reference, none, none);

			typedef typename MasterType::traveler_type traveler_type;
		};


	}

}