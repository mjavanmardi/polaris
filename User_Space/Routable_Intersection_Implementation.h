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

		};
	}

}
