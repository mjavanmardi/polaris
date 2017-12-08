#pragma once

#include "Person_Platooning_Implementations.h"

namespace Platoon_Components
{
	namespace Prototypes
	{
		prototype struct Coordinated_Platooning ADD_DEBUG_INFO
		{
			//tag_as_prototype;
			//accessor(lead_vehicle_id, NONE, NONE); //head vehicle
			//accessor(link, NONE, NONE);
			//accessor(link_arrival_time, NONE, NONE);
			//accessor(node_wait_time, NONE, NONE);

			template<typename TargetType> void Initialize(int start_time);

			//accessor(weather_event_to_process, NONE, NONE);
			accessor(Person_Platooning_Faculty, NONE, NONE);

		};


		////keeps a list of all platooning vehicles in the network. 
		//prototype struct Platoon ADD_DEBUG_INFO
		//{
		//	tag_as_prototype;
		//	accessor(platoon_data_container, NONE, NONE);
		//};



		template<typename ComponentType>
		template<typename TargetType>		
		void Coordinated_Platooning<ComponentType>::Initialize(int start_time)
		{
			//this_component()->template Initialize< TargetType>(start_time);
			((ComponentType*)this)->template Initialize< TargetType>(start_time);
		}


	}
}
using namespace Platoon_Components::Prototypes;