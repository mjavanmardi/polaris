#pragma once
#include "User_Space_Includes.h"
#include "Activity_Location_Prototype.h"
#include "Zone_Prototype.h"
//#include "../File_IO/utilities.h"
//#include "../File_IO/network_data.h"
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

	namespace Prototypes
	{
		prototype struct Network
		{
			tag_as_prototype;

			//==================================================================================================================
			/// basic network
			//------------------------------------------------------------------------------------------------------------------
			accessor(intersections_container, NONE, NONE);
			accessor(links_container, NONE, NONE);
			accessor(db_reader, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------
			
			//==================================================================================================================
			/// transportation network
			//------------------------------------------------------------------------------------------------------------------
			accessor(turn_movements_container, NONE, NONE);
			accessor(activity_locations_container, NONE, NONE);
			accessor(zones_container, NONE, NONE);
			accessor(zone_ids_container, NONE, NONE);
			accessor(ramp_metering_container, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// routable network
			//------------------------------------------------------------------------------------------------------------------
			accessor(routable_network, NONE, NONE);
			accessor(realtime_routable_network, NONE, NONE);
			accessor(routable_networks_container, NONE, NONE);
			accessor(realtime_routable_networks_container, NONE, NONE);
			accessor(scan_list, NONE, NONE);
			accessor(reversed_path_container, NONE, NONE);
			accessor(reset_links, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// simulation network
			//------------------------------------------------------------------------------------------------------------------
			accessor(scenario_reference, NONE, NONE);
			accessor(max_free_flow_speed, NONE, NONE);	
			accessor(network_vmt, NONE, NONE);
			accessor(network_vht, NONE, NONE);
			accessor(network_snapshot_container, NONE, NONE);
			accessor(link_turn_movement_map, NONE, NONE);
			accessor(in_network_vht_vehicle_based, NONE, NONE);
			accessor(out_network_vht_vehicle_based_update_lock, NONE, NONE);
			accessor(network_vht_vehicle_based, NONE, NONE);
			accessor(analyze_link_groups_container, NONE, NONE);

			//------------------------------------------------------------------------------------------------------------------


			//==================================================================================================================
			/// graphical network
			//------------------------------------------------------------------------------------------------------------------
			accessor(network_bounds, NONE, NONE);
			accessor(link_lines, NONE, NONE);
			accessor(vehicle_coordinates, NONE, NONE);
			accessor(canvas, NONE, NONE);
			accessor(information_panel, NONE, NONE);
			accessor(input_offset, NONE, NONE);

			//==================================================================================================================
			/// db-io network
			//------------------------------------------------------------------------------------------------------------------
			accessor(db_id_to_links_map, NONE, NONE);
			accessor(link_dbid_dir_to_ptr_map, NONE, NONE );

			//==================================================================================================================
			/// network events
			//------------------------------------------------------------------------------------------------------------------
			accessor(network_event_manager, NONE, NONE);
			
			//==================================================================================================================
			/// traffic management center
			//------------------------------------------------------------------------------------------------------------------
			accessor(traffic_management_center, NONE, NONE);

			template<typename TargetType> void push_vehicle_coordinates(void)
			{
				this_component()->template accept_vehicle_coordinates<TargetType>();
			}
			template<typename TargetType> void submit_num_vehicles()
			{
				this_component()->template submit_num_vehicles<TargetType>();
			}
			template<typename TargetType> void accent_num_vehicles()
			{
				this_component()->template accent_num_vehicles<TargetType>();
			}
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// demand compatible network
			//------------------------------------------------------------------------------------------------------------------
			accessor(skimming_faculty, NONE, NONE);
			template<typename TargetType> typename TargetType::ReturnType Get_TTime(typename TargetType::ParamType Origin, typename TargetType::ParamType Destination, typename TargetType::Param2Type Mode_Indicator, typename TargetType::Param3Type Start_Time, 
				requires(TargetType,check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value) /*&& check(typename TargetType::ParamType, Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)*/))
			{			
				typedef Network_Skimming_Components::Prototypes::Network_Skimming<typename get_type_of(skimming_faculty)> _skim_interface;
				//typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> _location_interface;
				//typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> _zone_interface;
				//_location_interface* orig = (_location_interface*)Origin;
				//_location_interface* dest = (_location_interface*)Destination;

				////if (orig == dest) return 0;

				//int Origin_ID, Destination_ID;
				//Origin_ID = orig->template zone<_zone_interface*>()->template uuid<int>();
				//Destination_ID = dest->template zone<_zone_interface*>()->template uuid<int>();

				_skim_interface* skim = this->skimming_faculty<_skim_interface*>();
//TODO
//				typename TargetType::ReturnType ret_value = skim->template Get_TTime<Target_Type<NT,typename TargetType::ReturnType, typename TargetType::ParamType, typename TargetType::Param2Type, typename TargetType::Param3Type>>(Origin, Destination, Mode_Indicator, Start_Time);
				
				return ret_value;
			}

			template<typename TargetType> typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin, typename TargetType::ParamType Destination, typename TargetType::Param2Type Start_Time /*, requires(TargetType,check(typename TargetType::ReturnType, Network_Skimming_Components::Concepts::Is_LOS_Prototype) && check(typename TargetType::ParamType, Activity_Location_Components::Concepts::Is_Activity_Location_Prototype))*/)
			{		
				typedef Network_Skimming_Components::Prototypes::Network_Skimming<typename get_type_of(skimming_faculty)> _skim_interface;
				_skim_interface* skim = this->skimming_faculty<_skim_interface*>();
				/*typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> _zone_interface;
				_location_interface* orig = (_location_interface*)Origin;
				_location_interface* dest = (_location_interface*)Destination;

				int Origin_ID, Destination_ID;
				Origin_ID = orig->template zone<_zone_interface*>()->template uuid<int>();
				Destination_ID = dest->template zone<_zone_interface*>()->template uuid<int>();*/

//TODO
//				typename TargetType::ReturnType ret_value = skim->Get_LOS<Target_Type<NT,typename TargetType::ReturnType, typename TargetType::ParamType, typename TargetType::Param2Type>>(Origin, Destination, Start_Time);
				
				return ret_value;
			}
			
			template<typename TargetType> typename TargetType::ReturnType Get_TTime(typename TargetType::ParamType Origin, typename TargetType::ParamType Destination, typename TargetType::Param2Type Mode_Indicator, typename TargetType::Param3Type Start_Time, 
				requires(TargetType,!check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{			
				assert_check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value, "Return type must be a time value.");
				assert_check(typename TargetType::ReturnType, Zone_Components::Concepts::Is_Zone_Prototype, "Origin/Destination must be specified as a zone_prototype.");
				assert_check(typename TargetType::ParamType, Activity_Location_Components::Concepts::Is_Activity_Location_Prototype, " Or Origin/Destination must be specified as an activity_location_prototype.");
			}
			
			//------------------------------------------------------------------------------------------------------------------

			template<typename TargetType> void read_network_data(typename TargetType::ParamType data_source, requires(TargetType,check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same) || check_2(typename TargetType::NetIOType,Types::File_Network,is_same) || check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				this_component()->template read_network_data<TargetType>(data_source);
			}
			
			template<typename TargetType> void read_realtime_network_data(typename TargetType::ParamType data_source, requires(TargetType,check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same) || check_2(typename TargetType::NetIOType,Types::File_Network,is_same) || check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				this_component()->template read_realtime_network_data<TargetType>(data_source);
			}

			template<typename TargetType> void read_network_data(requires(TargetType,!check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same) && !check_2(typename TargetType::NetIOType,Types::File_Network,is_same) && !check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				//assert_check(false,"TargetType::NetIOType is not supported");
				assert_check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(typename TargetType::NetIOType,Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read from file");
				assert_check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same,"TargetType should indicate Regular_Network if you want to create a routable network from a regular network");
			}

			template<typename TargetType> void read_snapshot_data(float maximum_free_flow_speed, typename TargetType::ParamType link_travel_time_map, typename TargetType::ParamType movement_travel_time_map)
			{
				this_component()->template read_snapshot_data<TargetType>(maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
			}

			template<typename TargetType> void set_network_bounds()
			{
				this_component()->template set_network_bounds<TargetType>();
			}

			template<typename TargetType> void initialize_antares_layers()
			{
				this_component()->template initialize_antares_layers<TargetType>();
			}			
			
			template<typename TargetType> void write_network_data(typename TargetType::ParamType data_destination)
			{
				this_component()->template write_network_data<TargetType>(data_destination);
			}

			template<typename TargetType> void simulation_initialize()
			{
				this_component()->template simulation_initialize<TargetType>();
			}

			template<typename TargetType> void reset_routable_network()
			{
				this_component()->template reset_routable_network<TargetType>();
			}

			template<typename TargetType> TargetType current_simulation_interval_index()
			{
				typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;
				return (TargetType)(start_of_current_simulation_interval_relative<TargetType>() / scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>());
			}

			template<typename TargetType> TargetType start_of_current_simulation_interval_relative()
			{
				typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;
#ifndef EXCLUDE_DEMAND
				int current_time = int(floor(Simulation_Time.Current_Time<Basic_Units::Time_Variables::Time_Seconds>() + 0.5));
#else
				int current_time = iteration();
#endif
				if (current_time < scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>() - 1) 
				{
					cout << "iteration() must start from (simulation_interval_length - 1)" << endl;
					exit(1);
				}
				return (TargetType)(iteration() - (scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>() - 1));
			}

			template<typename TargetType> TargetType start_of_current_simulation_interval_absolute()
			{
				typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;
				return (TargetType)(start_of_current_simulation_interval_relative<TargetType>() + scenario_reference<_Scenario_Interface*>()->template simulation_start_time<int>());
			}

			template<typename TargetType> TargetType get_routable_network_from_snapshots(int current_time)
			{
				return (TargetType)(this_component()->template get_routable_network_from_snapshots<TargetType>(current_time));
			}

			template<typename TargetType> void increase_in_network_vht_vehicle_based(float increase)
			{
				this_component()->template increase_in_network_vht_vehicle_based<TargetType>(increase);
			}

			template<typename TargetType> void increase_out_network_vht_vehicle_based(float increase)
			{
				this_component()->template increase_out_network_vht_vehicle_based<TargetType>(increase);
			}
			template<typename TargetType> void update_network_vht_compensation(int adjustment)
			{
				this_component()->template update_network_vht_compensation<TargetType>(adjustment);
			}
			template<typename TargetType> void update_ttime_distribution(int ttime)
			{
				this_component()->template update_ttime_distribution<TargetType>(ttime);
			}

			template<typename TargetType> TargetType get_random_zone(boost::container::vector<TargetType>* available_zones = nullptr, requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Zone_Components::Concepts::Is_Zone_Prototype)))
			{
				typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
				typedef  Pair_Associative_Container< typename get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;

				typedef  Random_Access_Sequence< typename get_type_of(zone_ids_container),int> _Zone_Ids_Interface;
				_Zone_Interface* zone;
				_Zones_Container_Interface::iterator zone_itr;
				int zone_index, zone_id, zone_count;

				if (available_zones == nullptr)
				{
					_Zones_Container_Interface* zones = this->zones_container<_Zones_Container_Interface*>();
					_Zone_Ids_Interface& zone_ids = this->zone_ids_container<_Zone_Ids_Interface&>();

					zone_count = zones->size();

					zone_index = (int)((Uniform_RNG.template Next_Rand<float>()*0.999999)*zone_count);
					zone_id = zone_ids[zone_index];
					zone_itr = zones->find(zone_id);

					if (zone_itr == zones->end()) {THROW_EXCEPTION("Error: zone id '" << zone_id << "' was not found");}
					zone = (_Zone_Interface*)zone_itr->second;
				}

				else
				{
					zone_count = available_zones->size();
					zone_index = (int)((Uniform_RNG.template Next_Rand<float>()*0.999999)*zone_count);
					zone = (_Zone_Interface*)(available_zones->at(zone_index));
				}

				return (TargetType)zone;
			}
		};

	}
}

using namespace Network_Components::Prototypes;
