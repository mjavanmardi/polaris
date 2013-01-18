#pragma once

#include "Polaris_Link_Implementation.h"
#include "Routing_Prototype.h"
#include "Network_Prototype.h"

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
		implementation struct Routable_Link_Implementation {

			typedef typename MasterType::link_type network_link_type;

			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(float, travel_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, f_cost, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, h_cost, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_component(typename MasterType::routable_link_type, label_pointer, none, none);
			member_data(float, label_cost, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(Network_Components::Types::Scan_List_Status_Keys, scan_list_status, none, none);

			member_component(typename MasterType::link_type, network_link_reference, none, none);
		
			member_component(typename MasterType::routable_intersection_type, upstream_intersection, none, none);
			member_component(typename MasterType::routable_intersection_type, downstream_intersection, none, none);

		};
		
	}

}