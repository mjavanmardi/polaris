#pragma once

#include "Intersection_Interface.h"

namespace Intersection_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{
		
		struct Routable_Outbound_Inbound_Movements_Base
		{
			typedef Intersection_Components::Interfaces::Movement_Interface<Intersection_Components::Components::Polaris_Movement_Component, NULLTYPE>* outbound_movement_reference_type;
			member_data_basic(outbound_movement_reference_type, outbound_movement_reference);
			member_data_basic(float, forward_link_turn_travel_time);
		};


		struct Routable_Intersection_Base
		{
			member_data_basic(float, x_position);
			member_data_basic(float, y_position);
			typedef Intersection_Components::Components::Routable_Outbound_Inbound_Movements_Component outbound_inbound_movements_element_type;
			typedef Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<outbound_inbound_movements_element_type, NULLTYPE> outbound_inbound_movements_interface;
			typedef vector<outbound_inbound_movements_interface*> outbound_inbound_movements_container_type;
			
			//========================================================================================================
			//start implementation of memeber_data(outbound_inbound_movements_container_type, movement_reference)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_inbound_movements(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_outbound_inbound_movements;
			}
 
			tag_getter(outbound_inbound_movements);

		protected:
			outbound_inbound_movements_container_type outbound_inbound_movements;
			//end
			//--------------------------------------------------------------------------------------------------------
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Intersection_Components::Interfaces::Intersection_Interface, Intersection_Components::Bases::Routable_Intersection_Base> Routable_Intersection_Component;
		typedef Polaris_Component<Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface, Intersection_Components::Bases::Routable_Outbound_Inbound_Movements_Base> Routable_Outbound_Inbound_Movements_Component;
	}	

}
