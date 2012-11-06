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

			member_data(int, uuid, none, none);
			member_data(float, travel_time, none, none);
			member_data(float, f_cost, none, none);
			member_data(float, h_cost, none, none);
			member_component(typename MasterType::routable_link_type, label_pointer, none, none);
			member_data(float, label_cost, none, none);
			member_data(Scan_List_Status_Keys, scan_list_status, none, none);

			member_component(typename MasterType::link_type, network_link_reference, none, none);
		
			member_component(typename MasterType::routable_intersection_type, upstream_intersection, none, none);
			member_component(typename MasterType::routable_intersection_type, downstream_intersection, none, none);

		};
		
	}

	namespace Components
	{
	}	

}