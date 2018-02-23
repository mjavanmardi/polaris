#pragma once
#include "Traffic_Simulator_Includes.h"

namespace Transit_Vehicle_Trip_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}

	namespace Prototypes
	{
		prototype struct Transit_Vehicle_Trip ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(internal_id, NONE, NONE);
			accessor(dbid, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(direction, NONE, NONE);
			accessor(pattern, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(arrival_seconds, NONE, NONE);
			accessor(departure_seconds, NONE, NONE);
			accessor(number_of_stops, NONE, NONE)
			
			//Simulation related
			accessor(people_standing, NONE, NONE);
			accessor(people_seated, NONE, NONE);
			accessor(current_position, NONE, NONE);

			template<typename TargetType> void schdeule_vehicle_movements_in_transit_network(TargetType param)
			{
				this_component()->template schdeule_vehicle_movements_in_transit_network<TargetType>(param);
			}
		};


	}
}

using namespace Transit_Vehicle_Trip_Components::Prototypes;
