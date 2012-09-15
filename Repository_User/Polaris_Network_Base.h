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
		template<typename MasterType>
		struct Polaris_Network_Base
		{
			member_data_basic(float, max_free_flow_speed);

			typedef typename MasterType::intersection_type intersection_type;
			typedef intersection_type intersections_container_element_type;
			typedef vector<void*> intersections_container_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType intersections_container(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_intersections;} tag_getter(intersections_container);
			
			intersections_container_type _intersections;
			


			typedef typename MasterType::link_type link_type;
			typedef link_type links_container_element_type;
			typedef vector<void*> links_container_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType links_container(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_links_container;} tag_getter(links_container);
			
			links_container_type _links_container;


			typedef typename MasterType::turn_movement_type turn_movement_type;
			typedef turn_movement_type turn_movements_element_type;
			typedef vector<void*> turn_movements_container_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType turn_movements_container(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_turn_movements_container;}
			tag_getter(turn_movements_container);
			



			typedef typename intersection_type::outbound_inbound_movements_container_element_type outbound_inbound_movements_container_element_type;
			typedef typename intersection_type::outbound_inbound_movements_container_type outbound_inbound_movements_container_type;

			typedef typename intersection_type::inbound_outbound_movements_container_element_type inbound_outbound_movements_container_element_type;
			typedef typename intersection_type::inbound_outbound_movements_container_type inbound_outbound_movements_container_type;
			

			typedef typename outbound_inbound_movements_container_element_type::movements_element_type movements_type;
			
			turn_movements_container_type _turn_movements_container;
		};

		//typedef Intersection_Components::Components::Polaris_Outbound_Inbound_Movements_Component outbound_inbound_movements_element_type;
		//typedef Intersection_Components::Components::Polaris_Inbound_Outbound_Movements_Component inbound_outbound_movements_element_type;
		//typedef Intersection_Components::Bases::Polaris_Intersection_Base::inbound_outbound_movements_container_type inbound_outbound_movements_container_type;
		//typedef Intersection_Components::Bases::Polaris_Intersection_Base::outbound_inbound_movements_container_type outbound_inbound_movements_container_type;
		//typedef Intersection_Components::Bases::movements_container_type movements_container_type;
		//typedef Intersection_Components::Bases::movements_element_type movements_element_type;

	}
	
	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Network_Component
		{
			typedef Polaris_Component<Network_Components::Interfaces::Network_Interface, Network_Components::Bases::Polaris_Network_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}	
}