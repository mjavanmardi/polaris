//not used anymore
#pragma once

#include "Intersection_Prototype.h"

namespace Intersection_Components
{
	//namespace Types
	//{
	//
	//}

	//namespace Concepts
	//{
	//}
	//
	//namespace Implementations
	//{
	//	

	//	implementation struct Routable_Movement_Implementation:public Polaris_Component<MasterType,INHERIT(Routable_Movement_Implementation),Data_Object>
	//	{
	//		m_data(int, uuid, NONE, NONE);
	//		m_data(float, forward_link_turn_travel_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
	//		m_prototype(Null_Prototype,typename MasterType::routable_link_type>, inbound_link, NONE, NONE);
	//		m_prototype(Null_Prototype,typename MasterType::routable_link_type>, outbound_link, NONE, NONE);
	//	};


	//	implementation struct Routable_Inbound_Outbound_Movements_Implementation:public Polaris_Component<MasterType,INHERIT(Routable_Inbound_Outbound_Movements_Implementation),Data_Object>
	//	{
	//		m_prototype(Null_Prototype,typename MasterType::routable_link_type>, inbound_link_reference, NONE, NONE);
	//		m_container(std::vector<typename MasterType::routable_movement_type*>, outbound_movements, NONE, NONE);
	//	};

	//	
	//	implementation struct Routable_Outbound_Inbound_Movements_Implementation:public Polaris_Component<MasterType,INHERIT(Routable_Outbound_Inbound_Movements_Implementation),Data_Object>
	//	{
	//		m_prototype(Null_Prototype,typename MasterType::routable_link_type>, outbound_link_reference, NONE, NONE);
	//		m_container(std::vector<typename MasterType::routable_movement_type*>, inbound_movements, NONE, NONE);
	//	};

	//	implementation struct Routable_Intersection_Implementation:public Polaris_Component<MasterType,INHERIT(Routable_Intersection_Implementation),Data_Object>
	//	{
	//		m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
	//		m_data(int, internal_id, NONE, NONE);
	//		m_data(float, x_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
	//		m_data(float, y_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
	//		m_data(float, z_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

	//		typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<MasterType> outbound_inbound_movements_container_element_type;
	//		m_container(std::vector<outbound_inbound_movements_container_element_type*>, outbound_inbound_movements, NONE, NONE);
	//		typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<MasterType> inbound_outbound_movements_container_element_type;
	//		m_container(std::vector<inbound_outbound_movements_container_element_type*>, inbound_outbound_movements, NONE, NONE);

	//		//template<typename TargetType> void construct_routable_from_regular(typename TargetType::ParamType regular_intersection, typename TargetType::Param2Type linksMap)
	//		template<typename RegularIntersectionParamType,typename LinksMapParamType> void construct_routable_from_regular(RegularIntersectionParamType regular_intersection, LinksMapParamType linksMap)
	//		{
	//			typedef Intersection<typename MasterType::intersection_type> _Regular_Intersection_Interface;
	//			typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Regular_Outbound_Inbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), _Regular_Outbound_Inbound_Movements_Interface*> _Regular_Outbound_Inbound_Movements_Container_Interface;

	//			typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Regular_Inbound_Outbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), _Regular_Inbound_Outbound_Movements_Interface*> _Regular_Inbound_Outbound_Movements_Container_Interface;

	//			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Regular_Movement_Interface;
	//			typedef  Random_Access_Sequence< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Regular_Movement_Interface*> _Regular_Movements_Container_Interface;

	//			typedef  Link_Components::Prototypes::Link< typename _Regular_Inbound_Outbound_Movements_Interface::get_type_of(inbound_link_reference)> _Regular_Link_Interface;

	//			typedef Intersection<typename MasterType::routable_intersection_type> _Routable_Intersection_Interface;
	//			typedef Outbound_Inbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Routable_Outbound_Inbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), _Routable_Outbound_Inbound_Movements_Interface*> _Routable_Outbound_Inbound_Movements_Container_Interface;

	//			typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Routable_Inbound_Outbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Routable_Inbound_Outbound_Movements_Interface*> _Routable_Inbound_Outbound_Movements_Container_Interface;

	//			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Routable_Movement_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;

	//			typedef  Link_Components::Prototypes::Link< typename _Routable_Inbound_Outbound_Movements_Interface::get_type_of(inbound_link_reference)> _Routable_Link_Interface;

	//			_x_position = regular_intersection->template x_position<float>();
	//			_y_position = regular_intersection->template y_position<float>();
	//			_uuid = regular_intersection->template uuid<int>();
	//			_internal_id = ((_Regular_Intersection_Interface*)regular_intersection)->template internal_id<int>();
	//				
	//			// create inbound_outbound_movements
	//			_Regular_Inbound_Outbound_Movements_Container_Interface& regular_inbound_outbound_movements_container = ((_Regular_Intersection_Interface*)regular_intersection)->template inbound_outbound_movements<_Regular_Inbound_Outbound_Movements_Container_Interface&>();
	//			typename _Regular_Inbound_Outbound_Movements_Container_Interface::iterator regular_inbound_outbound_movements_itr;
	//				
	//			for(regular_inbound_outbound_movements_itr=regular_inbound_outbound_movements_container.begin(); regular_inbound_outbound_movements_itr!=regular_inbound_outbound_movements_container.end(); regular_inbound_outbound_movements_itr++)
	//			{
	//				_Regular_Inbound_Outbound_Movements_Interface* regular_inbound_outbound_movements = (_Regular_Inbound_Outbound_Movements_Interface*)(*regular_inbound_outbound_movements_itr);
	//				_Routable_Inbound_Outbound_Movements_Interface* routable_inbound_outbound_movements = (_Routable_Inbound_Outbound_Movements_Interface*)Allocate<typename _Routable_Inbound_Outbound_Movements_Interface::Component_Type>();
	//				_Regular_Link_Interface* regular_link = regular_inbound_outbound_movements->template inbound_link_reference<_Regular_Link_Interface*>();
	//				_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)linksMap.find(regular_link->template internal_id<int>())->second;
	//				routable_inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>(routable_link);
	//				//another level of loop
	//				_Regular_Movements_Container_Interface& regular_outbound_movements_container = regular_inbound_outbound_movements->template outbound_movements<_Regular_Movements_Container_Interface&>();
	//				typename _Regular_Movements_Container_Interface::iterator regular_outbound_movement_itr;
	//				for(regular_outbound_movement_itr=regular_outbound_movements_container.begin(); regular_outbound_movement_itr!=regular_outbound_movements_container.end(); regular_outbound_movement_itr++)
	//				{
	//					_Regular_Movement_Interface* regular_outbound_movement = (_Regular_Movement_Interface*)(*regular_outbound_movement_itr);
	//					_Routable_Movement_Interface* routable_outbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
	//					routable_outbound_movement->template uuid<int>(regular_outbound_movement->template uuid<int>());
	//					routable_outbound_movement->template forward_link_turn_travel_time<float>(regular_outbound_movement->template forward_link_turn_travel_time<float>());
	//					_Regular_Link_Interface* regular_outbound_link = regular_outbound_movement->template outbound_link<_Regular_Link_Interface*>();
	//					_Routable_Link_Interface* routable_outbound_link = (_Routable_Link_Interface*)linksMap.find(regular_outbound_link->template internal_id<int>())->second;
	//					routable_outbound_movement->template outbound_link<_Routable_Link_Interface*>(routable_outbound_link);
	//					routable_outbound_movement->template inbound_link<_Routable_Link_Interface*>(routable_link);
	//					regular_outbound_movement->template replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
	//					routable_inbound_outbound_movements->template outbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
	//					
	//					routable_link->template outbound_turn_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
	//				}
	//				inbound_outbound_movements<_Routable_Inbound_Outbound_Movements_Container_Interface&>().push_back(routable_inbound_outbound_movements);
	//			}
	//		}
	//		template<typename TargetType> void construct_realtime_routable_from_regular(typename TargetType::ParamType regular_intersection, typename TargetType::Param2Type linksMap)
	//		{
	//			typedef Intersection<typename MasterType::intersection_type> _Regular_Intersection_Interface;
	//			typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Regular_Outbound_Inbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), _Regular_Outbound_Inbound_Movements_Interface*> _Regular_Outbound_Inbound_Movements_Container_Interface;

	//			typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Regular_Inbound_Outbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), _Regular_Inbound_Outbound_Movements_Interface*> _Regular_Inbound_Outbound_Movements_Container_Interface;

	//			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Regular_Movement_Interface;
	//			typedef  Random_Access_Sequence< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Regular_Movement_Interface*> _Regular_Movements_Container_Interface;

	//			typedef  Link_Components::Prototypes::Link< typename _Regular_Inbound_Outbound_Movements_Interface::get_type_of(inbound_link_reference)> _Regular_Link_Interface;

	//			typedef Intersection<typename MasterType::routable_intersection_type> _Routable_Intersection_Interface;
	//			typedef Outbound_Inbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Routable_Outbound_Inbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), _Routable_Outbound_Inbound_Movements_Interface*> _Routable_Outbound_Inbound_Movements_Container_Interface;

	//			typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Routable_Inbound_Outbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Routable_Inbound_Outbound_Movements_Interface*> _Routable_Inbound_Outbound_Movements_Container_Interface;

	//			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Routable_Movement_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;

	//			typedef  Link_Components::Prototypes::Link< typename _Routable_Inbound_Outbound_Movements_Interface::get_type_of(inbound_link_reference)> _Routable_Link_Interface;

	//			_x_position = regular_intersection->template x_position<float>();
	//			_y_position = regular_intersection->template y_position<float>();
	//			_uuid = regular_intersection->template uuid<int>();
	//			_internal_id = ((_Regular_Intersection_Interface*)regular_intersection)->template internal_id<int>();
	//				
	//			// create inbound_outbound_movements
	//			_Regular_Inbound_Outbound_Movements_Container_Interface& regular_inbound_outbound_movements_container = ((_Regular_Intersection_Interface*)regular_intersection)->template inbound_outbound_movements<_Regular_Inbound_Outbound_Movements_Container_Interface&>();
	//			typename _Regular_Inbound_Outbound_Movements_Container_Interface::iterator regular_inbound_outbound_movements_itr;
	//				
	//			for(regular_inbound_outbound_movements_itr=regular_inbound_outbound_movements_container.begin(); regular_inbound_outbound_movements_itr!=regular_inbound_outbound_movements_container.end(); regular_inbound_outbound_movements_itr++)
	//			{
	//				_Regular_Inbound_Outbound_Movements_Interface* regular_inbound_outbound_movements = (_Regular_Inbound_Outbound_Movements_Interface*)(*regular_inbound_outbound_movements_itr);
	//				_Routable_Inbound_Outbound_Movements_Interface* routable_inbound_outbound_movements = (_Routable_Inbound_Outbound_Movements_Interface*)Allocate<typename _Routable_Inbound_Outbound_Movements_Interface::Component_Type>();
	//				_Regular_Link_Interface* regular_link = regular_inbound_outbound_movements->template inbound_link_reference<_Regular_Link_Interface*>();
	//				_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)linksMap.find(regular_link->template internal_id<int>())->second;
	//				routable_inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>(routable_link);
	//				//another level of loop
	//				_Regular_Movements_Container_Interface& regular_outbound_movements_container = regular_inbound_outbound_movements->template outbound_movements<_Regular_Movements_Container_Interface&>();
	//				typename _Regular_Movements_Container_Interface::iterator regular_outbound_movement_itr;
	//				for(regular_outbound_movement_itr=regular_outbound_movements_container.begin(); regular_outbound_movement_itr!=regular_outbound_movements_container.end(); regular_outbound_movement_itr++)
	//				{
	//					_Regular_Movement_Interface* regular_outbound_movement = (_Regular_Movement_Interface*)(*regular_outbound_movement_itr);
	//					_Routable_Movement_Interface* routable_outbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
	//					routable_outbound_movement->template uuid<int>(regular_outbound_movement->template uuid<int>());
	//					routable_outbound_movement->template forward_link_turn_travel_time<float>(regular_outbound_movement->template realtime_forward_link_turn_travel_time<float>());
	//					_Regular_Link_Interface* regular_outbound_link = regular_outbound_movement->template outbound_link<_Regular_Link_Interface*>();
	//					_Routable_Link_Interface* routable_outbound_link = (_Routable_Link_Interface*)linksMap.find(regular_outbound_link->template internal_id<int>())->second;
	//					routable_outbound_movement->template outbound_link<_Routable_Link_Interface*>(routable_outbound_link);
	//					routable_outbound_movement->template inbound_link<_Routable_Link_Interface*>(routable_link);
	//					regular_outbound_movement->template realtime_replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
	//					routable_inbound_outbound_movements->template outbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
	//					
	//					routable_link->template outbound_turn_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
	//				}
	//				inbound_outbound_movements<_Routable_Inbound_Outbound_Movements_Container_Interface&>().push_back(routable_inbound_outbound_movements);
	//			}
	//		}
	//		
	//		template<typename TargetType> void set_forward_link_turn_travel_time(TargetType movement_travel_time_map)
	//		{
	//			typedef Intersection<typename MasterType::routable_intersection_type> _Routable_Intersection_Interface;
	//			typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Routable_Inbound_Outbound_Movements_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Routable_Inbound_Outbound_Movements_Interface*> _Routable_Inbound_Outbound_Movements_Container_Interface;

	//			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Routable_Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Routable_Movement_Interface;
	//			typedef  Random_Access_Sequence< typename _Routable_Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;


	//			typename _Routable_Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
	//			for (inbound_outbound_movements_itr = _inbound_outbound_movements.begin(); inbound_outbound_movements_itr != _inbound_outbound_movements.end(); inbound_outbound_movements_itr++)
	//			{
	//				_Routable_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Routable_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
	//				_Routable_Movements_Container_Interface& outbound_movements = inbound_outbound_movements->template outbound_movements<_Routable_Movements_Container_Interface&>();
	//				typename _Routable_Movements_Container_Interface::iterator movement_itr;
	//				for (movement_itr = outbound_movements.begin(); movement_itr != outbound_movements.end(); movement_itr++)
	//				{
	//					_Routable_Movement_Interface* movement = (_Routable_Movement_Interface*)(*movement_itr);
	//					int movement_uuid = movement->template uuid<int>();
	//					float forward_link_turn_travel_time = movement_travel_time_map.find(movement_uuid)->second;
	//					movement->template 	forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
	//				}
	//			}
	//		}
	//	};
	//}

}

