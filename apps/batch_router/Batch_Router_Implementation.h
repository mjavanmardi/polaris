#pragma once

#include "Batch_Router_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Routing_Prototype.h"


namespace Batch_Router_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		//implementation struct Network_Validation_Implementation : public Polaris_Component<MasterType,INHERIT(Network_Validation_Implementation),Execution_Object>
		//{
		//	// Tag as implementation
		//	typedef typename Polaris_Component<MasterType,INHERIT(Network_Validation_Implementation),Execution_Object>::Component_Type ComponentType;

		//	// reference to the transportation network
		//	m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

		//	m_data(int, current_index,NONE,NONE);
		//	m_data(int, num_checks_to_perform,NONE,NONE);
		//	m_data(bool, do_as_destination,NONE,NONE);

		//	m_container(std::vector<Network_Validation_Unit<typename MasterType::network_validation_unit_type>*>,validation_units, NONE, NONE);

		//	typedef Network_Validation_Unit<get_component_type(validation_units_type)> validation_unit_itf;
		//	typedef Network_Components::Prototypes::Network<typename type_of(network_reference)> network_itf;		
		//	typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
		//	typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones_itf)> zone_itf;
		//	typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
		//	typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
		//	typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
		//	typedef Link_Components::Prototypes::Link<typename get_component_type(links_itf)> link_itf;
		//	typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
		//	typedef Turn_Movement_Components::Prototypes::Movement<typename get_component_type(turns_itf)> turn_itf;
		//	typedef Random_Access_Sequence<typename validation_unit_itf::get_type_of(origin_routing_results)> routing_results_itf;

		//	template<typename TargetType> void Initialize(int num_checks)
		//	{
		//		// Set the number of checks to perform for each location (i.e. times routing to and from the location)
		//		_num_checks_to_perform = num_checks;
		//		_current_index = 0;
		//		_do_as_destination=false;

		//		network_itf* network = this->template network_reference<network_itf*>();
		//		locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();


		//		// choose a random location for each check
		//		for (locations_itf::iterator itr = activity_locations->begin(); itr != activity_locations->end(); ++itr)
		//		{
		//			// get random origin/destination locations
		//			validation_unit_itf* validation_unit = (validation_unit_itf*)Allocate<get_component_type(validation_units_type)>();
		//			validation_unit->network_validator<ComponentType*>(this);
		//			validation_unit->Initialize<location_itf*>((location_itf*)(*itr));

		//			_validation_units.push_back(validation_unit);
		//		}
		//	
		//	}

		//	template<typename TargetType> void Get_Results()
		//	{
		//		// Get results for each location
		//		File_IO::File_Writer fw;
		//		fw.Open("network_validation_results.xls");
		//		fw.Write_Line("Location\tOrigin_Fail\tDestination_Fail\tLink_Fail\tTurn_Fail");

		//		for (validation_units_type::iterator itr = _validation_units.begin(); itr != _validation_units.end(); ++itr)
		//		{
		//			validation_unit_itf* v = (validation_unit_itf*)(*itr);
		//			routing_results_itf* orig_res = v->origin_routing_results<routing_results_itf*>();
		//			routing_results_itf* dest_res = v->destination_routing_results<routing_results_itf*>();

		//			bool orig_valid=false;
		//			for (routing_results_itf::iterator ritr = orig_res->begin(); ritr!=orig_res->end(); ++ritr) if ((*ritr).first==true) orig_valid=true;
		//			bool dest_valid=false;
		//			for (routing_results_itf::iterator ritr = dest_res->begin(); ritr!=dest_res->end(); ++ritr) if ((*ritr).first==true) dest_valid=true;

		//			if (!orig_valid || !dest_valid) 
		//			{
		//				fw.Write(v->location<location_itf*>()->uuid<int>());
		//				if (!orig_valid)				fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as origin, ";
		//				else							fw.Write("");
		//				if (!dest_valid)				fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as destination,";
		//				else							fw.Write("");
		//				if (v->link_failure<bool>())	fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as origin, ";
		//				else							fw.Write("");
		//				if (v->turn_failure<bool>())	fw.Write('x');//cout <<v->location<location_itf*>()->uuid<int>()<<", failed as destination,";
		//				else							fw.Write("");
		//												fw.Write_Line();
		//			}
		//		}
		//	}

		//};




		//======================================================================================
		/// Basic skimming implementation
		//--------------------------------------------------------------------------------------

		implementation struct Routed_Trip_Implementation : public Polaris_Component<MasterType,INHERIT(Routed_Trip_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Routed_Trip_Implementation),Execution_Object>::Component_Type ComponentType;

			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

			m_prototype(Routing_Components::Prototypes::Routing,typename MasterType::routing_type, router, NONE, NONE);
			typedef strip_modifiers(router_type) router_interface;

			m_prototype(Activity_Location_Components::Prototypes::Activity_Location,typename MasterType::activity_location_type, origin, NONE, NONE);
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location,typename MasterType::activity_location_type, destination, NONE, NONE);
			typedef strip_modifiers(origin_type) location_interface;

			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, mode, NONE,NONE);

			member_component_and_feature_accessor(departure_time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);

			m_data(bool, link_failure,NONE,NONE);
			m_data(bool, turn_failure,NONE,NONE);

			m_container(concat(std::vector<pair<bool,location_interface*>>),origin_routing_results, NONE, NONE);
			m_container(concat(std::vector<pair<bool,location_interface*>>),destination_routing_results, NONE, NONE);

			typedef strip_modifiers(network_reference_type) network_itf;
			typedef strip_modifiers(router_type) router_itf;
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones_itf)> zone_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links_itf)> link_itf;
			typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
			typedef Turn_Movement_Components::Prototypes::Movement<typename get_component_type(turns_itf)> turn_itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename router_itf::get_type_of(movement_plan)> _Movement_Plan_Interface;
			typedef Random_Access_Sequence<typename _Movement_Plan_Interface::get_type_of(trajectory_container)> _Trajectory_Container_Interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<typename get_component_type(_Trajectory_Container_Interface)> _Trajectory_Unit_Interface;

			template<typename TargetType> void Get_Results()
			{
				_Movement_Plan_Interface* movement = this->_router->movement_plan<_Movement_Plan_Interface*>();

				if (movement->valid_trajectory<bool>())
				{
					cout <<"Travel time for trip was "<< movement->routed_travel_time<float>()<<endl;
					cout <<"Link.dir: travel time"<<endl;
					
					_Trajectory_Container_Interface* trajectory = movement->trajectory_container<_Trajectory_Container_Interface*>();

					for (_Trajectory_Container_Interface::iterator itr = trajectory->begin(); itr != trajectory->end(); ++itr)
					{
						_Trajectory_Unit_Interface* tu = (_Trajectory_Unit_Interface*)*itr;
						link_itf* link = tu->link<link_itf*>();
						cout << link->dbid<int>()<<"."<<link->direction<int>()<<": "<<tu->estimated_link_accepting_time<int>()<<endl;
					}
					/*movement->template valid_trajectory<bool>(true);
					movement->template routed_travel_time<float>(routed_travel_time);
					movement->template estimated_time_of_arrival<Simulation_Timestep_Increment>(_movement_plan->template absolute_departure_time<int>() + routed_travel_time);
					movement->template estimated_travel_time_when_departed<float>(routed_travel_time);
					movement->set_trajectory(path_container, cost_container);*/
					
				}
				else
				{
					cout<<"Error: movement was not routable."<<endl;
				}
				
			}
			
		};



	}

}
