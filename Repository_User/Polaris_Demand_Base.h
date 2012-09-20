#pragma once
#include "Demand_Interface.h"

namespace Demand_Components
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
		struct Polaris_Demand_Base
		{
			typedef typename MasterType::scenario_type scenario_type;
			typedef typename MasterType::network_type network_type;
			typedef typename MasterType::link_type links_container_element_type;
			typedef vector<void*> links_container_type;
			typedef typename MasterType::activity_location_type activity_locations_container_element_type;
			typedef vector<void*> activity_locations_container_type;
			typedef typename MasterType::traveler_type traveler_type;
			typedef typename MasterType::vehicle_type vehicle_type;
			typedef typename MasterType::routing_type routing_type;


			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType scenario_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_scenario_reference);}
			tag_setter(scenario_reference);
			template<typename ThisType, typename CallerType, typename TargetType>
			void scenario_reference(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_scenario_reference = (void*)set_value;}
			tag_getter(scenario_reference);
			
			void* _scenario_reference;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType network_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_network_reference);}
			tag_setter(network_reference);
			template<typename ThisType, typename CallerType, typename TargetType>
			void network_reference(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_network_reference = (void*)set_value;}
			tag_getter(network_reference);
			
			void* _network_reference;

		};


	}
	
	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Demand_Component
		{
			typedef Polaris_Component<Demand_Components::Interfaces::Demand_Interface, Demand_Components::Bases::Polaris_Demand_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}	
}