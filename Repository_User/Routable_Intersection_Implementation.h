#pragma once

#include "Intersection_Prototype.h"

namespace Intersection_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		

		implementation struct Routable_Movement_Implementation
		{
			member_data_basic(float, forward_link_turn_travel_time);
			
			member_component_basic(typename MasterType::routable_link_type, inbound_link);
			member_component_basic(typename MasterType::routable_link_type, outbound_link);

		};


		implementation struct Routable_Inbound_Outbound_Movements_Implementation
		{
			member_component_basic(typename MasterType::routable_link_type, inbound_link_reference);

			member_container_basic(vector<typename MasterType::routable_movement_type*>, outbound_movements);

		};

		template<typename MasterType>
		struct Routable_Outbound_Inbound_Movements_Implementation
		{
			member_component_basic(typename MasterType::routable_link_type, outbound_link_reference);
			member_container_basic(vector<typename MasterType::routable_movement_type*>, inbound_movements);

		};

		template<typename MasterType>
		struct Routable_Intersection_Implementation
		{
			member_data_basic(int, uuid);
			member_data_basic(float, x_position);
			member_data_basic(float, y_position);
			member_data_basic(float, z_position);

			typedef Polaris_Component<Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation, MasterType> outbound_inbound_movements_container_element_type;
			member_container_basic(vector<outbound_inbound_movements_container_element_type*>, outbound_inbound_movements);
			typedef Polaris_Component<Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation, MasterType> inbound_outbound_movements_container_element_type;
			member_container_basic(vector<inbound_outbound_movements_container_element_type*>, inbound_outbound_movements);

		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Routable_Intersection_Component{typedef Polaris_Component<Intersection_Components::Implementations::Routable_Intersection_Implementation, MasterType> type; };

		template<typename MasterType>
		struct Routable_Movement_Component{typedef Polaris_Component<Intersection_Components::Implementations::Routable_Movement_Implementation, MasterType> type; };
		template<typename MasterType>
		struct Routable_Inbound_Outbound_Movements_Component{typedef Polaris_Component<Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation, MasterType> type; };
		template<typename MasterType>
		struct Routable_Outbound_Inbound_Movements_Component{typedef Polaris_Component<Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation, MasterType> type; };
	}	

}
