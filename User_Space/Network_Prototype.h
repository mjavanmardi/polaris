#pragma once
#include "User_Space_Includes.h"
#include "../File_IO/utilities.h"
#include "../File_IO/network_data.h"
//using namespace Basic_Units::Data_Structures;
//using namespace Signal_Components::Data_Structures;

namespace Network_Components
{
	namespace Types
	{
		struct ODB_Network{};
		struct File_Network{};
		struct Regular_Network{};
		struct Graphical_Network{};
		struct Network_IO_Maps
		{
			dense_hash_map<int,void*> intersection_id_to_ptr;
			dense_hash_map<long long,void*> link_id_dir_to_ptr;
		};

		template<typename _NetIOType,typename _ParamType>
		struct Network_Initialization_Type
		{
			typedef _NetIOType NetIOType;
			typedef _ParamType ParamType;
		};

		enum Scan_List_Status_Keys
		{
				UNSCANNED=0,
				SCANNED,
				INSELIST,
		};
	}

	namespace Concepts
	{
		concept struct Is_Basic_Network_Prototype
		{
			check_getter(has_intersections, Component_Type::intersections_container);
			check_getter(has_links, Component_Type::links_container);
			check_feature(has_read_function, Component_Type::read_network_data); 
			define_default_check(has_intersections && has_links && has_read_function);
		};

		concept struct Is_Basic_Network
		{
			check_getter(has_intersections, intersections_container);
			check_getter(has_links, links_container);
			check_feature(has_read_function, read_network_data); 

			check_concept(is_basic_network_prototype, Is_Basic_Network_Prototype);
			define_sub_check(is_basic_network, has_intersections && has_links && has_read_function);
			define_default_check(is_basic_network || is_basic_network_prototype);
		};
		
		concept struct Is_Transportation_Network_Prototype
		{
			check_concept(is_basic_network, Is_Basic_Network_Prototype);
			check_getter(has_turns, Component_Type::turn_movements_container);
			check_getter(has_locations, Component_Type::activity_locations_container);
			check_getter(has_zones, Component_Type::zones_container); 
			define_default_check(is_basic_network && has_turns && has_locations && has_zones);
		};

		concept struct Is_Transportation_Network
		{
			check_concept(is_basic_network, Is_Basic_Network);
			check_getter(has_turns, turn_movements_container);
			check_getter(has_locations, activity_locations_container);
			check_getter(has_zones, zones_container); 
			//define_default_check(is_basic_network && has_turns && has_locations && has_zones);
			check_concept(is_transportation_network_prototype, Is_Transportation_Network_Prototype);
			define_sub_check(is_transportation_network, is_basic_network && has_turns && has_locations && has_zones);
			define_default_check(is_transportation_network || is_transportation_network_prototype);
		};
		
		concept struct Is_Routable_Network_Prototype
		{
			check_concept(is_basic_network, Is_Basic_Network_Prototype);
			check_getter(has_routable_network, Component_Type::routable_network);
			check_getter(has_routable_networks_container, Component_Type::routable_networks_container);
			check_getter(has_scan_list, Component_Type::scan_list); 
			define_default_check(is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
		};

		concept struct Is_Routable_Network
		{
			check_concept(is_basic_network, Is_Basic_Network);
			check_getter(has_routable_network, routable_network);
			check_getter(has_routable_networks_container, routable_networks_container);
			check_getter(has_scan_list, scan_list); 
			check_concept(is_routable_network_prototype, Is_Routable_Network_Prototype);
			define_sub_check(is_routable_network, is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
			define_default_check(is_routable_network || is_routable_network_prototype);
		};

		concept struct Is_Simulation_Network_Prototype
		{
			check_concept(is_routable_network, Is_Routable_Network_Prototype);
			check_getter(has_scenario_reference, scenario_reference);
			check_getter(has_max_free_flow_speed, max_free_flow_speed);
			define_default_check(is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Simulation_Network
		{
			check_concept(is_routable_network, Is_Routable_Network);
			check_getter(has_scenario_reference, scenario_reference);
			check_getter(has_max_free_flow_speed, max_free_flow_speed);
			check_concept(is_simulation_network_prototype, Is_Simulation_Network_Prototype);
			define_sub_check(is_simulation_networ, is_routable_network && has_scenario_reference && has_max_free_flow_speed);
			define_default_check(is_simulation_networ || is_simulation_network_prototype);
		};		
		
		concept struct Is_Trasportation_Simulation_Network_Prototype
		{
			check_concept(is_transportation_network, Is_Transportation_Network_Prototype);
			check_concept(is_routable_network, Is_Routable_Network_Prototype);
			check_getter(has_scenario_reference, Component_Type::scenario_reference);
			check_getter(has_max_free_flow_speed, Component_Type::max_free_flow_speed);
			define_default_check(is_transportation_network && is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Trasportation_Simulation_Network
		{
			check_concept(is_transportation_network, Is_Transportation_Network);
			check_concept(is_routable_network, Is_Routable_Network);
			check_getter(has_scenario_reference, scenario_reference);
			check_getter(has_max_free_flow_speed, max_free_flow_speed);

			check_concept(is_transportation_simulation_network_prototype, Is_Trasportation_Simulation_Network_Prototype);
			define_sub_check(is_transportation_simulation_network, is_transportation_network && is_routable_network && has_scenario_reference && has_max_free_flow_speed);
			define_default_check(is_transportation_simulation_network || is_transportation_simulation_network_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Network_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			//==================================================================================================================
			/// basic network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(intersections_container, none, none);
			feature_accessor(links_container, none, none);
			feature_accessor(db_reader, none, none);
			//------------------------------------------------------------------------------------------------------------------
			
			//==================================================================================================================
			/// transportation network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(turn_movements_container, none, none);
			feature_accessor(activity_locations_container, none, none);
			feature_accessor(zones_container, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// routable network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(routable_network, none, none);
			feature_accessor(realtime_routable_network, none, none);
			feature_accessor(routable_networks_container, none, none);
			feature_accessor(realtime_routable_networks_container, none, none);
			feature_accessor(scan_list, none, none);
			feature_accessor(reversed_path_container, none, none);
			feature_accessor(reset_links, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// simulation network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(scenario_reference, none, none);
			feature_accessor(max_free_flow_speed, none, none);	
			feature_accessor(network_vmt, none, none);
			feature_accessor(network_vht, none, none);
			feature_accessor(network_snapshot_container, none, none);
			feature_accessor(link_turn_movement_map, none, none);
			//------------------------------------------------------------------------------------------------------------------


			//==================================================================================================================
			/// graphical network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(network_bounds, none, none);
			feature_accessor(link_lines, none, none);
			feature_accessor(vehicle_coordinates, none, none);
			feature_accessor(canvas, none, none);
			feature_accessor(information_panel, none, none);
			feature_accessor(input_offset, none, none);

			//==================================================================================================================
			/// db-io network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(db_id_to_links_map, none, none);
			feature_accessor(link_dbid_dir_to_ptr_map, none, none );

			//==================================================================================================================
			/// network events
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(network_event_manager, none, none);
			
			//==================================================================================================================
			/// traffic management center
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(traffic_management_center, none, none);

			feature_prototype void push_vehicle_coordinates(void)
			{
				this_component()->template accept_vehicle_coordinates<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void submit_num_vehicles()
			{
				this_component()->template submit_num_vehicles<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void accent_num_vehicles()
			{
				this_component()->template accent_num_vehicles<ComponentType,CallerType,TargetType>();
			}
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// demand compatible network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(skimming_faculty, none, none);
			//feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			//{
			//	define_component_interface(_skim_interface,typename get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			//	_skim_interface* skim = this->skimming_faculty<_skim_interface*>();
			//	return skim->Get_LOS<TargetType>(Origin_ID, Destination_ID, Mode_Indicator);
			//}
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, typename TargetType::Param3Type Start_Time, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
				define_component_interface(_skim_interface,typename get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
				_skim_interface* skim = this->skimming_faculty<_skim_interface*>();
				return skim->Get_LOS<TargetType>(Origin_ID, Destination_ID, Mode_Indicator, Start_Time);
			}
			//------------------------------------------------------------------------------------------------------------------

			feature_prototype void read_network_data(typename TargetType::ParamType data_source, requires(check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same) || check_2(typename TargetType::NetIOType,Types::File_Network,is_same) || check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				this_component()->template read_network_data<ComponentType,CallerType,TargetType>(data_source);
			}
			
			feature_prototype void read_realtime_network_data(typename TargetType::ParamType data_source, requires(check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same) || check_2(typename TargetType::NetIOType,Types::File_Network,is_same) || check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				this_component()->template read_realtime_network_data<ComponentType,CallerType,TargetType>(data_source);
			}

			feature_prototype void read_network_data(requires(!check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same) && !check_2(typename TargetType::NetIOType,Types::File_Network,is_same) && !check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				//assert_check(false,"TargetType::NetIOType is not supported");
				assert_check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(typename TargetType::NetIOType,Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read from file");
				assert_check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same,"TargetType should indicate Regular_Network if you want to create a routable network from a regular network");
			}

			feature_prototype void read_snapshot_data(float maximum_free_flow_speed, typename TargetType::ParamType link_travel_time_map, typename TargetType::ParamType movement_travel_time_map)
			{
				this_component()->template read_snapshot_data<ComponentType,CallerType,TargetType>(maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
			}

			feature_prototype void set_network_bounds()
			{
				this_component()->template set_network_bounds<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void initialize_antares_layers()
			{
				this_component()->template initialize_antares_layers<ComponentType,CallerType,TargetType>();
			}			
			
			feature_prototype void write_network_data(typename TargetType::ParamType data_destination)
			{
				this_component()->template write_network_data<ComponentType,CallerType,TargetType>(data_destination);
			}

			feature_prototype void simulation_initialize()
			{
				this_component()->template simulation_initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void reset_routable_network()
			{
				this_component()->template reset_routable_network<ComponentType,CallerType,TargetType>();
			}

			feature_prototype TargetType current_simulation_interval_index()
			{
				define_component_interface(_Scenario_Interface, typename get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				return (TargetType)(start_of_current_simulation_interval_relative<TargetType>() / scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>());
			}

			feature_prototype TargetType start_of_current_simulation_interval_relative()
			{
				define_component_interface(_Scenario_Interface, typename get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
#ifndef EXCLUDE_DEMAND
				int current_time = int(floor(Simulation_Time.Current_Time<Basic_Units::Time_Variables::Time_Seconds>() + 0.5));
#else
				int current_time = _iteration;
#endif
				if (current_time < scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>() - 1) 
				{
					cout << "_iteration must start from (simulation_interval_length - 1)" << endl;
					exit(1);
				}
				return (TargetType)(_iteration - (scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>() - 1));
			}

			feature_prototype TargetType start_of_current_simulation_interval_absolute()
			{
				define_component_interface(_Scenario_Interface, typename get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				return (TargetType)(start_of_current_simulation_interval_relative<TargetType>() + scenario_reference<_Scenario_Interface*>()->template simulation_start_time<int>());
			}

			feature_prototype TargetType get_routable_network_from_snapshots(int current_time)
			{
				return (TargetType)(this_component()->template get_routable_network_from_snapshots<ComponentType,CallerType,TargetType>(current_time));
			}
		};

	}
}

using namespace Network_Components::Prototypes;