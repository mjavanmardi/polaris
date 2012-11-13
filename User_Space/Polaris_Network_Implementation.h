#pragma once
#include "Network_Prototype.h"
#include "Polaris_Intersection_Implementation.h"

namespace Network_Components
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
		struct Polaris_Network_Implementation
		{
			member_data(float, max_free_flow_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(vector<typename MasterType::intersection_type*>, intersections_container, none, none);

			member_container(vector<typename MasterType::link_type*>, links_container, none, none);

			member_container(vector<typename MasterType::routable_network_type*>, routable_networks_container, none, none);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_network(){return (TargetType)(_routable_networks_container[_thread_id]);}tag_getter_as_available(routable_network);

			member_container(vector<typename MasterType::turn_movement_type*>, turn_movements_container, none, none);


			member_container(vector<typename MasterType::activity_location_type*>, activity_locations_container, none, none);

			
			member_container(vector<typename MasterType::movement_type*>, movements_container, none, none);

			typedef typename MasterType::scenario_type scenario_type;
		};


	}

}