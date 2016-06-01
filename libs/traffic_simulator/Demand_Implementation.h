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
		implementation struct Demand_Implementation:public Polaris_Component<MasterType,INHERIT(Demand_Implementation),Data_Object>
		{

			m_prototype(Null_Prototype,typename MasterType::scenario_type, scenario_reference, NONE, NONE);

			m_prototype(Null_Prototype,typename MasterType::network_type, network_reference, NONE, NONE);
			
			m_container(std::vector<typename MasterType::vehicle_type*>, vehicles_container, NONE, NONE);

			m_container(std::vector<typename MasterType::vehicle_characteristics_type*>, vehicle_types_container, NONE, NONE);

			m_data(int, first_vehicle_departure_time, NONE, NONE);

			m_data(int, last_vehicle_departure_time, NONE, NONE);

			typedef typename MasterType::traveler_type traveler_type;

		};


	}

}
