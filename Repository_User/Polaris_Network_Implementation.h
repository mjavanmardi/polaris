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
			member_data_basic(float, max_free_flow_speed);

			member_container_basic(vector<typename MasterType::intersection_type*>, intersections_container);

			member_container_basic(vector<typename MasterType::link_type*>, links_container);

			member_container_basic(vector<typename MasterType::routable_network_type*>, routable_networks_container);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_network(){return (TargetType)(_routable_networks_container[thread_id]);}tag_getter(routable_network);

			member_container_basic(vector<typename MasterType::turn_movement_type*>, turn_movements_container);


			member_container_basic(vector<typename MasterType::activity_location_type*>, activity_locations_container);

			
			member_container_basic(vector<typename MasterType::movement_type*>, movements_container);

			typedef typename MasterType::scenario_type scenario_type;
		};


	}
	
	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Network_Component{typedef Polaris_Component<Network_Components::Implementations::Polaris_Network_Implementation, MasterType> type; };
	}	
}