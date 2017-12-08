#pragma once

#include "Activity_Simulator_Includes.h"


namespace Platoon_Components
{
	namespace Prototypes
	{
		prototype struct Vehicle_Platooning ADD_DEBUG_INFO
		{
			//tag_as_prototype;
			//accessor(lead_vehicle_id, NONE, NONE); //head vehicle
			//accessor(link, NONE, NONE);
			//accessor(link_arrival_time, NONE, NONE);
			//accessor(node_wait_time, NONE, NONE);

			template<typename TargetType> void Initialize(int start_time);

			//accessor(weather_event_to_process, NONE, NONE);

		};


		////keeps a list of all platooning vehicles in the network. 
		//prototype struct Platoon ADD_DEBUG_INFO
		//{
		//	tag_as_prototype;
		//	accessor(platoon_data_container, NONE, NONE);
		//};



		template<typename ComponentType>
		template<typename TargetType>		
		void Vehicle_Platooning<ComponentType>::Initialize(int start_time)
		{
			this_component()->template Initialize< TargetType>(start_time);
			//((ComponentType*)this)->template Initialize< TargetType>(start_time);
		}


	}
}
using namespace Platoon_Components::Prototypes;