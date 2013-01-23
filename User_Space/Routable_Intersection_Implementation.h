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
		

		implementation struct Routable_Movement_Implementation:public Polaris_Component_Class<Routable_Movement_Implementation,MasterType,Data_Object,ParentType>
		{
			member_data(float, forward_link_turn_travel_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_component(typename MasterType::routable_link_type, inbound_link, none, none);
			member_component(typename MasterType::routable_link_type, outbound_link, none, none);

		};


		implementation struct Routable_Inbound_Outbound_Movements_Implementation:public Polaris_Component_Class<Routable_Inbound_Outbound_Movements_Implementation,MasterType,Data_Object,ParentType>
		{
			member_component(typename MasterType::routable_link_type, inbound_link_reference, none, none);

			member_container(vector<typename MasterType::routable_movement_type*>, outbound_movements, none, none);

		};

		
		implementation struct Routable_Outbound_Inbound_Movements_Implementation:public Polaris_Component_Class<Routable_Outbound_Inbound_Movements_Implementation,MasterType,Data_Object,ParentType>
		{
			member_component(typename MasterType::routable_link_type, outbound_link_reference, none, none);
			member_container(vector<typename MasterType::routable_movement_type*>, inbound_movements, none, none);

		};

		implementation struct Routable_Intersection_Implementation:public Polaris_Component_Class<Routable_Intersection_Implementation,MasterType,Data_Object,ParentType>
		{
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, z_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<MasterType> outbound_inbound_movements_container_element_type;
			member_container(vector<outbound_inbound_movements_container_element_type*>, outbound_inbound_movements, none, none);
			typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<MasterType> inbound_outbound_movements_container_element_type;
			member_container(vector<inbound_outbound_movements_container_element_type*>, inbound_outbound_movements, none, none);

			feature_implementation void construct_routable_from_regular(typename TargetType::ParamType regular_intersection, typename TargetType::Param2Type linksMap)
			{
				typedef Intersection_Prototype<typename MasterType::intersection_type> _Regular_Intersection_Interface;
				define_container_and_value_interface(_Regular_Outbound_Inbound_Movements_Container_Interface, _Regular_Outbound_Inbound_Movements_Interface, _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Inbound_Outbound_Movements_Container_Interface, _Regular_Inbound_Outbound_Movements_Interface, _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Movements_Container_Interface, _Regular_Movement_Interface, _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Regular_Link_Interface, _Regular_Inbound_Outbound_Movements_Interface::get_type_of(inbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType); 

				typedef Intersection_Prototype<typename MasterType::routable_intersection_type> _Routable_Intersection_Interface;
				define_container_and_value_interface(_Routable_Outbound_Inbound_Movements_Container_Interface, _Routable_Outbound_Inbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype,Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Inbound_Outbound_Movements_Container_Interface, _Routable_Inbound_Outbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Movements_Container_Interface, _Routable_Movement_Interface, _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Routable_Link_Interface, _Routable_Inbound_Outbound_Movements_Interface::get_type_of(inbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType); 

				//dense_hash_map<int, _Routable_Link_Interface*>& linksMap = linksMap_param;
				//_Regular_Intersection_Interface* regular_intersection = regular_intersection_param;

				_x_position = regular_intersection->template x_position<float>();
				_y_position = regular_intersection->template y_position<float>();
				_uuid = regular_intersection->template uuid<int>();
				_internal_id = ((_Regular_Intersection_Interface*)regular_intersection)->template internal_id<int>();
					
				// create inbound_outbound_movements
				_Regular_Inbound_Outbound_Movements_Container_Interface& regular_inbound_outbound_movements_container = ((_Regular_Intersection_Interface*)regular_intersection)->template inbound_outbound_movements<_Regular_Inbound_Outbound_Movements_Container_Interface&>();
				typename _Regular_Inbound_Outbound_Movements_Container_Interface::iterator regular_inbound_outbound_movements_itr;
					
				for(regular_inbound_outbound_movements_itr=regular_inbound_outbound_movements_container.begin(); regular_inbound_outbound_movements_itr!=regular_inbound_outbound_movements_container.end(); regular_inbound_outbound_movements_itr++)
				{
					_Regular_Inbound_Outbound_Movements_Interface* regular_inbound_outbound_movements = (_Regular_Inbound_Outbound_Movements_Interface*)(*regular_inbound_outbound_movements_itr);
					_Routable_Inbound_Outbound_Movements_Interface* routable_inbound_outbound_movements = (_Routable_Inbound_Outbound_Movements_Interface*)Allocate<typename _Routable_Inbound_Outbound_Movements_Interface::Component_Type>();
					_Regular_Link_Interface* regular_link = regular_inbound_outbound_movements->template inbound_link_reference<_Regular_Link_Interface*>();
					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)linksMap.find(regular_link->template internal_id<int>())->second;
					routable_inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>(routable_link);
					//another level of loop
					_Regular_Movements_Container_Interface& regular_outbound_movements_container = regular_inbound_outbound_movements->template outbound_movements<_Regular_Movements_Container_Interface&>();
					typename _Regular_Movements_Container_Interface::iterator regular_outbound_movement_itr;
					for(regular_outbound_movement_itr=regular_outbound_movements_container.begin(); regular_outbound_movement_itr!=regular_outbound_movements_container.end(); regular_outbound_movement_itr++)
					{
						_Regular_Movement_Interface* regular_outbound_movement = (_Regular_Movement_Interface*)(*regular_outbound_movement_itr);
						_Routable_Movement_Interface* routable_outbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
						routable_outbound_movement->template forward_link_turn_travel_time<float>(regular_outbound_movement->template forward_link_turn_travel_time<float>());
						_Regular_Link_Interface* regular_outbound_link = regular_outbound_movement->template outbound_link<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_outbound_link = (_Routable_Link_Interface*)linksMap.find(regular_outbound_link->template internal_id<int>())->second;
						routable_outbound_movement->template outbound_link<_Routable_Link_Interface*>(routable_outbound_link);
						regular_outbound_movement->template replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
						routable_inbound_outbound_movements->template outbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
					}
					inbound_outbound_movements<ComponentType,CallerType,_Routable_Inbound_Outbound_Movements_Container_Interface&>().push_back(routable_inbound_outbound_movements);
				}
					
				// create outbound_inbound_movements
				_Regular_Outbound_Inbound_Movements_Container_Interface& regular_outbound_inbound_movements_container = regular_intersection->template outbound_inbound_movements<_Regular_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Regular_Outbound_Inbound_Movements_Container_Interface::iterator regular_outbound_inbound_movements_itr;
					
				for(regular_outbound_inbound_movements_itr=regular_outbound_inbound_movements_container.begin(); regular_outbound_inbound_movements_itr!=regular_outbound_inbound_movements_container.end(); regular_outbound_inbound_movements_itr++)
				{
					_Regular_Outbound_Inbound_Movements_Interface* regular_outbound_inbound_movements = (_Regular_Outbound_Inbound_Movements_Interface*)(*regular_outbound_inbound_movements_itr);
					_Routable_Outbound_Inbound_Movements_Interface* routable_outbound_inbound_movements = (_Routable_Outbound_Inbound_Movements_Interface*)Allocate<typename _Routable_Outbound_Inbound_Movements_Interface::Component_Type>();
					_Regular_Link_Interface* regular_link = regular_outbound_inbound_movements->template outbound_link_reference<_Regular_Link_Interface*>();
					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)linksMap.find(regular_link->template internal_id<int>())->second;
					routable_outbound_inbound_movements->template outbound_link_reference<_Routable_Link_Interface*>(routable_link);
					//another level of loop
					_Regular_Movements_Container_Interface& regular_inbound_movements_container = regular_outbound_inbound_movements->template inbound_movements<_Regular_Movements_Container_Interface&>();
					typename _Regular_Movements_Container_Interface::iterator regular_inbound_movement_itr;
					for(regular_inbound_movement_itr=regular_inbound_movements_container.begin(); regular_inbound_movement_itr!=regular_inbound_movements_container.end(); regular_inbound_movement_itr++)
					{
						_Regular_Movement_Interface* regular_inbound_movement = (_Regular_Movement_Interface*)(*regular_inbound_movement_itr);
						_Routable_Movement_Interface* routable_inbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
						routable_inbound_movement->template forward_link_turn_travel_time<float>(regular_inbound_movement->template forward_link_turn_travel_time<float>());
						_Regular_Link_Interface* regular_inbound_link = regular_inbound_movement->template inbound_link<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_inbound_link = (_Routable_Link_Interface*)linksMap.find(regular_inbound_link->template internal_id<int>())->second;
						routable_inbound_movement->template inbound_link<_Routable_Link_Interface*>(routable_inbound_link);
						regular_inbound_movement->template replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
						routable_outbound_inbound_movements->template inbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
					}
					outbound_inbound_movements<ComponentType,CallerType,_Routable_Outbound_Inbound_Movements_Container_Interface&>().push_back(routable_outbound_inbound_movements);
				}
			}
		};
	}

}
