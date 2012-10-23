#pragma once

#include "Polaris_Link_Implementation.h"
#include "Routing_Prototype.h"

namespace Link_Components
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
		struct Routable_Link_Implementation {

			typedef typename MasterType::link_type network_link_type;

			member_data_basic(int, uuid);
			member_data_basic(float, travel_time);
			member_data_basic(float, f_cost);
			member_data_basic(float, h_cost);
			member_component_basic(typename MasterType::routable_link_type, label_pointer);
			member_data_basic(float, label_cost);
			member_data_basic(Scan_List_Status_Keys, scan_list_status);

			member_component_basic(typename MasterType::link_type, network_link_reference);
		
			member_component_basic(typename MasterType::routable_intersection_type, upstream_intersection);
			member_component_basic(typename MasterType::routable_intersection_type, downstream_intersection);

		};
		
	}

	namespace Components
	{
		template<typename MasterType>
		struct Routable_Link_Component{typedef Polaris_Component<Link_Components::Implementations::Routable_Link_Implementation, MasterType> type; };
	}	

}