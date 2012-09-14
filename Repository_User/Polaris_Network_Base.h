#pragma once
#include "Network_Interface.h"
#include "Polaris_Intersection_Base.h"

namespace Network_Components
{
	namespace Types
	{


	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{
		struct Polaris_Network_Base
		{
			member_data_basic(float, max_free_flow_speed);

			typedef Intersection_Components::Components::Polaris_Intersection_Component intersections_element_type;
			typedef Intersection_Components::Interfaces::Intersection_Interface<intersections_element_type, NULLTYPE> intersection_interface_type;
			typedef vector<intersection_interface_type*> intersections_container_type;
			//========================================================================================================
			//start implementation of memeber_data(vector<Intersection_Interface*>, intersections)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType intersections(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_intersections;
			}

			tag_getter(intersections);
			
		protected:
			intersections_container_type _intersections;
			//end
			//--------------------------------------------------------------------------------------------------------
			
			typedef Link_Components::Components::Polaris_Link_Component links_element_type;
			typedef Link_Components::Interfaces::Link_Interface<links_element_type, NULLTYPE> link_interface_type;
			typedef vector<link_interface_type*> links_container_type;
			//========================================================================================================
			//start implementation of memeber_data(vector<Link_Interface*>, links)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType links(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_links;
			}

			tag_getter(links);
			
		protected:
			links_container_type _links;
			//end
			//--------------------------------------------------------------------------------------------------------	

			typedef Link_Components::Components::Polaris_Turn_Movement_Component turn_movements_element_type;
			typedef Link_Components::Interfaces::Turn_Movement_Interface<turn_movements_element_type, NULLTYPE> turn_movement_interface_type;
			typedef vector<turn_movement_interface_type*> turn_movements_container_type;
			//========================================================================================================
			//start implementation of memeber_data(vector<Turn_Movement_Interface*>, turn_movements)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType turn_movements(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_turn_movements;
			}

			tag_getter(turn_movements);
			
		protected:
			turn_movements_container_type _turn_movements;
			//end
			//--------------------------------------------------------------------------------------------------------			
		};

		typedef Intersection_Components::Components::Polaris_Outbound_Inbound_Movements_Component outbound_inbound_movements_element_type;
		typedef Intersection_Components::Components::Polaris_Inbound_Outbound_Movements_Component inbound_outbound_movements_element_type;
		typedef Intersection_Components::Bases::Polaris_Intersection_Base::inbound_outbound_movements_container_type inbound_outbound_movements_container_type;
		typedef Intersection_Components::Bases::Polaris_Intersection_Base::outbound_inbound_movements_container_type outbound_inbound_movements_container_type;
		typedef Intersection_Components::Bases::movements_container_type movements_container_type;
		typedef Intersection_Components::Bases::movements_element_type movements_element_type;

	}
	
	namespace Components
	{
		typedef Polaris_Component<Network_Components::Interfaces::Network_Interface, Network_Components::Bases::Polaris_Network_Base> Polaris_Network_Component;
	}	
}