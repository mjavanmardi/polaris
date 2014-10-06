#pragma once

#include "Network_Validator_Prototype.h"


namespace Network_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		//======================================================================================
		/// Basic skimming implementation
		//--------------------------------------------------------------------------------------
		implementation struct Network_Validation_Unit_Implementation : public Polaris_Component<MasterType,INHERIT(Network_Validation_Unit_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Network_Validation_Unit_Implementation),Data_Object>::Component_Type ComponentType;

			// reference to the transportation network
			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

			m_prototype(Activity_Location_Components::Prototypes::Activity_Location,typename MasterType::activity_location_type, location, NONE, NONE);
			typedef strip_modifiers(location_type) location_interface;

			m_prototype(Routing_Components::Prototypes::Routing,typename MasterType::routing_type, router, NONE, NONE);
			typedef strip_modifiers(router_type) router_interface;

			m_data(int, current_iteration,NONE,NONE);
			m_data(int, num_checks_to_perform,NONE,NONE);

			m_container(concat(std::vector<pair<bool,location_interface*>>),origin_routing_results, NONE, NONE);
			m_container(concat(std::vector<pair<bool,location_interface*>>),destination_routing_results, NONE, NONE);

			typedef Movement_Plan<typename router_interface::get_type_of(movement_plan)> movement_plan_interface;
			typedef Prototypes::Network_Validation_Unit<ComponentType> validation_unit_itf;
			typedef Network_Components::Prototypes::Network<typename type_of(network_reference)> network_itf;		
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones_itf)> zone_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			typedef Link_Components::Prototypes::Link<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;


			template<typename LocationType> void Initialize(LocationType location, requires(LocationType,check(LocationType,is_pointer) && check(strip_modifiers(LocationType),Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				_Initialize(location);
			}

			void _Initialize(Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type>* loc)
			{
				// Set the number of checks to perform for each location (i.e. times routing to and from the location)
				_num_checks_to_perform = 5;
				_current_iteration = 0;

				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones = network->zones_container<zones_itf*>();

				validation_unit_itf* pthis = (validation_unit_itf*)this;
				pthis->location<location_type>(loc);

				router_type router = (router_type)Allocate<router_component_type>();

				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				
				// choose a random location for each check
				for (int i=0; i<_num_checks_to_perform; i++)
				{
					// get random origin/destination locations
					location_itf* orig = network->get_random_location<location_itf*>(loc);
					_origin_routing_results.push_back(pair<bool,location_itf*>(false,orig));
					location_itf* dest = network->get_random_location<location_itf*>(loc);
					_destination_routing_results.push_back(pair<bool,location_itf*>(false,dest));
				}
			
			}

			template<typename TargetType> void Initialize_Iteration()
			{
				movement_plan_interface* movement = (movement_plan_interface*)Allocate<router_type::get_type_of(movement_plan)>();
				
			}
			template<typename TargetType> void Get_Iteration_Results()
			{
				
			}
			
		};



	}

}
