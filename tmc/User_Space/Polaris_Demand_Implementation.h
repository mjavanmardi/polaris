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
		implementation struct Polaris_Demand_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Demand_Implementation),MasterType,Data_Object,ParentType>
		{

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);

			member_component(typename MasterType::network_type, network_reference, none, none);
			
			member_container(vector<typename MasterType::vehicle_type*>, vehicles_container, none, none);

			member_data(int, first_vehicle_departure_time, none, none);

			member_data(int, last_vehicle_departure_time, none, none);
#ifndef EXCLUDE_DEMAND
			typedef typename MasterType::person_type traveler_type;
#else
			typedef typename MasterType::traveler_type traveler_type;
#endif

		};


	}

}