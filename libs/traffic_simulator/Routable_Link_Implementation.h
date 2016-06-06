//not used anymore
#pragma once
#include "Link_Prototype.h"

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
		//implementation struct Routable_Link_Implementation:public Polaris_Component<MasterType,INHERIT(Routable_Link_Implementation),Data_Object> 
		//{

		//	typedef typename MasterType::link_type network_link_type;

		//	m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
		//	m_data(int, internal_id, NONE, NONE);
		//	m_data(float, travel_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
		//	m_data(float, f_cost, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
		//	m_data(float, h_cost, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
		//	m_prototype(Null_Prototype,typename MasterType::routable_link_type>, label_pointer, NONE, NONE);
		//	m_data(float, label_cost, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
		//	m_data(Network_Components::Types::Scan_List_Status_Keys, scan_list_status, NONE, NONE);
		//	
		//	m_data(int, scan_list_position, NONE, NONE);
		//	m_data(bool, reset_list_status, NONE, NONE);

		//	m_prototype(Null_Prototype,typename MasterType::link_type>, network_link_reference, NONE, NONE);
		//
		//	m_prototype(Null_Prototype,typename MasterType::routable_intersection_type>, upstream_intersection, NONE, NONE);
		//	m_prototype(Null_Prototype,typename MasterType::routable_intersection_type>, downstream_intersection, NONE, NONE);

		//	template<typename TargetType> void reset_routable_link()
		//	{
		//		_label_cost = INFINITY_FLOAT;
		//		_label_pointer = (label_pointer_interface_type*)this;
		//		_scan_list_status = Network_Components::Types::UNSCANNED;
		//		_reset_list_status = false;
		//		_f_cost = 0.0;
		//		_h_cost = 0.0;
		//	}

		//	template<typename TargetType> void construct_routable_from_regular(TargetType regular_link)
		//	{
		//		typedef Link<typename MasterType::link_type> _Link_Interface;	
		//		_network_link_reference = (network_link_reference_interface_type*)regular_link;
		//		_f_cost = 0.0;
		//		_h_cost = 0.0;
		//		_label_pointer = (label_pointer_interface_type*)this;
		//		_label_cost = INFINITY_FLOAT;
		//		_reset_list_status = false;
		//		_scan_list_status = Network_Components::Types::UNSCANNED;
		//		_uuid = ((_Link_Interface*)regular_link)->template uuid<int>();
		//		_internal_id = ((_Link_Interface*)regular_link)->template internal_id<int>();
		//		_travel_time = ((_Link_Interface*)regular_link)->template travel_time<float>();
		//	}

		//	template<typename TargetType> void construct_realtime_routable_from_regular(TargetType regular_link)
		//	{
		//		typedef Link<typename MasterType::link_type> _Link_Interface;	
		//		_network_link_reference = (network_link_reference_interface_type*)regular_link;
		//		_f_cost = 0.0;
		//		_h_cost = 0.0;
		//		_label_pointer = (label_pointer_interface_type*)this;
		//		_label_cost = INFINITY_FLOAT;
		//		_reset_list_status = false;
		//		_scan_list_status = Network_Components::Types::UNSCANNED;
		//		_uuid = ((_Link_Interface*)regular_link)->template uuid<int>();
		//		_internal_id = ((_Link_Interface*)regular_link)->template internal_id<int>();
		//		_travel_time = ((_Link_Interface*)regular_link)->template realtime_travel_time<float>();
		//	}

		//	m_container(std::vector<typename MasterType::routable_movement_type*>, outbound_turn_movements, NONE, NONE);
		//};
		
	}

}
