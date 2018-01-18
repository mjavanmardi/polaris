#pragma once

#include "Network_Validator_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Routing_Prototype.h"


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
		implementation struct Network_Validation_Implementation : public Polaris_Component<MasterType,INHERIT(Network_Validation_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Network_Validation_Implementation),Execution_Object>::Component_Type ComponentType;

			// reference to the transportation network
			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

			m_data(int, current_index,NONE,NONE);
			m_data(int, num_checks_to_perform,NONE,NONE);
			m_data(bool, do_as_destination,NONE,NONE);

			m_container(std::vector<Network_Validation_Unit<typename MasterType::network_validation_unit_type>*>,validation_units, NONE, NONE);

			typedef Network_Validation_Unit<get_component_type(validation_units_type)> validation_unit_itf;
			typedef Network_Components::Prototypes::Network<type_of(network_reference)> network_itf;
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_itf)> zone_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(links_itf)> link_itf;
			typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
			typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(turns_itf)> turn_itf;
			typedef Random_Access_Sequence<typename validation_unit_itf::get_type_of(origin_routing_results)> routing_results_itf;

			template<typename TargetType> void Initialize(int num_checks)
			{
				// Set the number of checks to perform for each location (i.e. times routing to and from the location)
				_num_checks_to_perform = num_checks;
				_current_index = 0;
				_do_as_destination=false;

				network_itf* network = this->template network_reference<network_itf*>();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();


				// choose a random location for each check
				for (locations_itf::iterator itr = activity_locations->begin(); itr != activity_locations->end(); ++itr)
				{
					// get random origin/destination locations
					validation_unit_itf* validation_unit = (validation_unit_itf*)Allocate<get_component_type(validation_units_type)>();
					validation_unit->network_validator<ComponentType*>(this);
					validation_unit->Initialize<location_itf*>((location_itf*)(*itr));

					_validation_units.push_back(validation_unit);
				}
			
			}

			template<typename TargetType> void Get_Results()
			{
				// Get results for each location
				File_IO::File_Writer fw;
				fw.Open("network_validation_results.xls");
				fw.Write_Line("Location\tOrigin_Fail\tDestination_Fail\tLink_Fail\tTurn_Fail");

				for (validation_units_type::iterator itr = _validation_units.begin(); itr != _validation_units.end(); ++itr)
				{
					validation_unit_itf* v = (validation_unit_itf*)(*itr);
					routing_results_itf* orig_res = v->origin_routing_results<routing_results_itf*>();
					routing_results_itf* dest_res = v->destination_routing_results<routing_results_itf*>();

					bool orig_valid=false;
					for (routing_results_itf::iterator ritr = orig_res->begin(); ritr!=orig_res->end(); ++ritr) if ((*ritr).first==true) orig_valid=true;
					bool dest_valid=false;
					for (routing_results_itf::iterator ritr = dest_res->begin(); ritr!=dest_res->end(); ++ritr) if ((*ritr).first==true) dest_valid=true;

					if (!orig_valid || !dest_valid) 
					{
						fw.Write(v->location<location_itf*>()->uuid<int>());
						if (!orig_valid)				fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as origin, ";
						else							fw.Write("");
						if (!dest_valid)				fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as destination,";
						else							fw.Write("");
						if (v->link_failure<bool>())	fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as origin, ";
						else							fw.Write("");
						if (v->turn_failure<bool>())	fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as destination,";
						else							fw.Write("");
														fw.Write_Line();
					}
				}
			}

		};




		//======================================================================================
		/// Basic skimming implementation
		//--------------------------------------------------------------------------------------
		implementation struct Network_Validation_Unit_Implementation : public Polaris_Component<MasterType,INHERIT(Network_Validation_Unit_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Network_Validation_Unit_Implementation),Execution_Object>::Component_Type ComponentType;

			// reference to the transportation network
			m_prototype(Network_Validator,typename MasterType::network_validation_type, network_validator, NONE, NONE);

			m_prototype(Activity_Location_Components::Prototypes::Activity_Location,typename MasterType::activity_location_type, location, NONE, NONE);
//			typedef strip_modifiers(location_type) location_interface;

			m_prototype(Routing_Components::Prototypes::Routing,typename MasterType::routing_type, router, NONE, NONE);
//			typedef strip_modifiers(router_type) router_interface;

			m_prototype(Movement_Plan,typename router_interface::get_type_of(movement_plan), movement, NONE, NONE);
//			typedef strip_modifiers(movement_type) movement_plan_interface;

			m_data(int, current_index,NONE,NONE);
			m_data(bool, do_as_destination,NONE,NONE);

			m_data(bool, link_failure,NONE,NONE);
			m_data(bool, turn_failure,NONE,NONE);

			m_container(concat(std::vector<pair<bool,location_interface*>>),origin_routing_results, NONE, NONE);
			m_container(concat(std::vector<pair<bool,location_interface*>>),destination_routing_results, NONE, NONE);

			template<typename TargetType> TargetType network_reference()
			{
				return _network_validator->network_reference<TargetType>();
			}
			template<typename TargetType> TargetType num_checks_to_perform()
			{
				return _network_validator->num_checks_to_perform<TargetType>();
			}

			typedef Prototypes::Network_Validation_Unit<ComponentType> validation_unit_itf;
			//typedef Network_Components::Prototypes::Network<typename strip_modifiers(network_validator_type)::get_type_of(network_reference)> network_itf;		
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> network_itf;
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_itf)> zone_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(links_itf)> link_itf;
			typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
			typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(turns_itf)> turn_itf;

			template<typename LocationType> void Initialize(LocationType location, requires(LocationType,check(LocationType,is_pointer) && check(strip_modifiers(LocationType),Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				_Initialize(location);
			}

			void _Initialize(Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type>* loc)
			{
				// Set the number of checks to perform for each location (i.e. times routing to and from the location)
				_current_index = 0;
				_do_as_destination=false;
				_link_failure=false;
				_turn_failure=false;

				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones = network->zones_container<zones_itf*>();

				validation_unit_itf* pthis = (validation_unit_itf*)this;
				pthis->location<location_type>(loc);

				_movement = (movement_type)Allocate<movement_component_type>();
				_movement->network<network_itf*>(network);

				_router = (router_type)Allocate<router_component_type>();	
				_router->movement_plan<movement_type>(_movement);
				_router->network<network_itf*>(network);

				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				
				// choose a random location for each check
				for (int i=0; i<num_checks_to_perform<int>(); i++)
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
				location_itf* orig=nullptr;
				location_itf* dest=nullptr;
				if (_do_as_destination)
				{
					dest = this->_location;
					orig = this->_origin_routing_results[this->_current_index].second;
				}
				else
				{
					orig = this->_location;
					dest = this->_destination_routing_results[this->_current_index].second;
				}

				// If able to successfully update movement plan, then schedule routing
				if (Update_Movement_Plan(orig,dest))_router->Schedule_Route_Computation(iteration()+1);
			}

			template<typename TargetType> void Update_Validator()
			{
				if (!_do_as_destination)
				{
					this->_destination_routing_results[this->_current_index].first=_movement->valid_trajectory<bool>();
					//cout <<"movement from "<<_location->uuid<int>() <<" to "<<this->_destination_routing_results[this->_current_index].second->uuid<int>()<<" was " <<_movement->valid_trajectory<bool>()<<endl;
				}
				else
				{
					this->_origin_routing_results[this->_current_index].first=_movement->valid_trajectory<bool>();
					//cout <<"movement to "<<_location->uuid<int>() <<" from "<<this->_origin_routing_results[this->_current_index].second->uuid<int>()<<" was " <<_movement->valid_trajectory<bool>()<<endl;
				}
				
			}

			template<typename TargetType> void Get_Iteration_Results()
			{
				
			}
			
			template<typename TargetType> bool Update_Movement_Plan(TargetType origin, TargetType destination)
			{
				_movement->template initialize_trajectory<NULLTYPE>();

				location_itf* orig = (location_itf*)origin;
				location_itf* dest = (location_itf*)destination;

				bool return_value=true;

				// Error checking on the origin and destination locations - if the error occurs on the location associated with this validation unit, then set as link_failure
				if (orig == nullptr || dest == nullptr) {THROW_EXCEPTION("Null origin or destination values specified"); return_value=false;}			
				if (orig->template origin_links<links_itf&>().size() == 0) 
				{
					THROW_WARNING("WARNING: movement from " << orig->template uuid<int>() << " to " << dest->template uuid<int>() << ", can not happen as no origin / destination links are available for the locations.");
					return_value=false;
					if (!_do_as_destination) _link_failure=true;
				}
				if (dest->template origin_links<links_itf&>().size() == 0) 
				{
					THROW_WARNING("WARNING: movement from " << orig->template uuid<int>() << " to " << dest->template uuid<int>() << ", can not happen as no origin / destination links are available for the locations.");
					return_value= false;
					if (_do_as_destination) _link_failure=true;
				}

				// add attributes to plan
				_movement->template origin<location_itf*>(orig);
				_movement->template destination<location_itf*>(dest);
				_movement->template origin<link_itf*>(orig->template origin_links<links_itf&>().at(0));
				_movement->template destination<link_itf*>(dest->template origin_links<links_itf&>().at(0));

				// error checking on the turn movements defined at the origin/destination links
				if (_movement->template origin<link_itf*>()->template outbound_turn_movements<turns_itf*>()->size() == 0)
				{
					THROW_WARNING("WARNING: cannot route trip as orig or dest links do not have valid turn movements: [orig_link,dest_link,orig_zone,dest_zone]: "<< ", " <<_movement->template origin<link_itf*>()->uuid<int>() << ", " <<  _movement->template destination<link_itf*>()->uuid<int>() << ", "  << orig->zone<zone_itf*>()->uuid<int>() << ", " << dest->zone<zone_itf*>()->uuid<int>());
					return_value=false;
					if (!_do_as_destination) _turn_failure=true;
				}
				if (_movement->template destination<link_itf*>()->template outbound_turn_movements<turns_itf*>()->size() == 0)
				{
					THROW_WARNING("WARNING: cannot route trip as orig or dest links do not have valid turn movements: [orig_link,dest_link,orig_zone,dest_zone]: "<< ", " <<_movement->template origin<link_itf*>()->uuid<int>() << ", " <<  _movement->template destination<link_itf*>()->uuid<int>() << ", "  << orig->zone<zone_itf*>()->uuid<int>() << ", " << dest->zone<zone_itf*>()->uuid<int>());
					return_value=false;
					if (_do_as_destination) _turn_failure=true;
				}

				// schedule the routing and do routing if requested through scenario, otherwise move to the activity scheduling phase
				_movement->template departed_time<Simulation_Timestep_Increment>(iteration());

				return return_value;
			}
		};



	}

}
