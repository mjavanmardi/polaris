#pragma once

namespace Platoon_Components
{
	namespace Prototypes
	{
		prototype struct Platoon_Data ADD_DEBUG_INFO
		{
			tag_as_prototype;
			accessor(lead_vehicle_id, NONE, NONE); //head vehicle
			accessor(link, NONE, NONE);
			accessor(link_arrival_time, NONE, NONE);
			accessor(node_wait_time, NONE, NONE);
		};


		//keeps a list of all platooning vehicles in the network. 
		prototype struct Platoon ADD_DEBUG_INFO
		{
			tag_as_prototype;
			accessor(platoon_data_container, NONE, NONE);			
		};

	}
}
using namespace Platoon_Components::Prototypes;