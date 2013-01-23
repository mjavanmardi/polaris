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
		implementation struct Routable_Link_Implementation:public Polaris_Component_Class<Routable_Link_Implementation,MasterType,Data_Object,ParentType> 
		{

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

			feature_implementation void reset_routable_link()
			{
				_label_cost = INFINITY_FLOAT;
				_label_pointer = this;
				_scan_list_status = Network_Components::Types::UNSCANNED;
				_f_cost = 0.0;
				_h_cost = 0.0;
			}

			feature_implementation void construct_routable_from_regular(TargetType regular_link)
			{
				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;	
				_network_link_reference = (typename MasterType::link_type*)regular_link;
				_f_cost = 0.0;
				_h_cost = 0.0;
				_label_pointer = this;
				_label_cost = 0.0;
				_uuid = ((_Link_Interface*)regular_link)->template uuid<int>();
				_internal_id = ((_Link_Interface*)regular_link)->template internal_id<int>();
				_travel_time = ((_Link_Interface*)regular_link)->template travel_time<float>();
			}
		};
		
	}

}