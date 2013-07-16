#pragma once
#include "Network_Prototype.h"
#include "Traffic_Management_Center_Prototype.h"
#include "Network_Event_Prototype.h"
#include "Polaris_Link_Implementation.h"

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
		struct MOE_Data
		{
			//int starting_time;
			//int ending_time;

			double assignment_calculation_time;
			double simulation_calculation_time;
			double operation_calculation_time;
			double output_calculation_time;

			//vector<Link_MOE_Data> link_moe_data_by_type_array;

			int num_generated_vehicles;
			int num_loaded_vehicles;
			int num_departed_vehicles;
			int num_arrived_vehicles;
			float network_avg_link_travel_time;
			float network_avg_link_travel_delay;
			float network_avg_link_speed;
			float network_avg_link_density;
			float network_avg_link_in_flow_rate;
			float network_avg_link_out_flow_rate;
			float network_avg_link_in_volume;
			float network_avg_link_out_volume;
			float network_avg_link_queue_length;

			float network_avg_link_speed_ratio;
			float network_avg_link_in_flow_ratio;
			float network_avg_link_out_flow_ratio;
			float network_avg_link_density_ratio;
			float network_avg_link_travel_time_ratio;
		};

		implementation struct Polaris_Network_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Network_Implementation),MasterType,Execution_Object,ParentType>
		{
			typedef typename Polaris_Component<APPEND_CHILD(Polaris_Network_Implementation),MasterType,Execution_Object,ParentType>::Component_Type ComponentType;

			member_data_component(typename MasterType::network_db_reader_type, db_reader, none,none);
			typedef unordered_map<long long,void*> type_of_link_dbid_dir_to_ptr_map;
			member_data(type_of_link_dbid_dir_to_ptr_map, link_dbid_dir_to_ptr_map, none, none);

			member_data(float, max_free_flow_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(vector<typename MasterType::intersection_type*>, intersections_container, none, none);

			member_container(vector<typename MasterType::link_type*>, links_container, none, none);

			member_container(vector<typename MasterType::routable_network_type*>, routable_networks_container, none, none);

			member_container(vector<typename MasterType::routable_network_type*>, realtime_routable_networks_container, none, none);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_network(){return (TargetType)(_routable_networks_container[_thread_id]);}tag_getter_as_available(routable_network);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType realtime_routable_network(){return (TargetType)(_realtime_routable_networks_container[_thread_id]);}tag_getter_as_available(realtime_routable_network);

			member_container(vector<typename MasterType::turn_movement_type*>, turn_movements_container, none, none);

			member_container(vector<typename MasterType::activity_location_type*>, activity_locations_container, none, none);

			member_associative_container(concat(dense_hash_map<int,typename MasterType::zone_type*>), zones_container, none, none);
			
			member_container(vector<typename MasterType::movement_type*>, movements_container, none, none);

			member_container(vector<typename MasterType::ramp_metering_type*>, ramp_metering_container, none, none);

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);

			member_container(vector<typename MasterType::intersection_control_type*>, intersection_controls_container, none, none);

			member_data(float, network_vmt, none, none);

			member_data(float, network_vht, none,none);

            member_data(long,current_cpu_time_in_seconds,none,none);

			member_data(long,start_cpu_time_in_seconds,none,none);

			member_data(float, in_network_vht_vehicle_based, none, none);
			member_data(float, out_network_vht_vehicle_based, none, none);
			member_data(float, network_vht_vehicle_based, none, none);
			member_data(_lock, network_vht_vehicle_based_update_lock, none, none);

			member_container(vector<typename MasterType::routable_network_type*>, network_snapshot_container, none, none);

			typedef unordered_map<int,vector<typename MasterType::link_type*>> id_to_links_map_type;
			member_data(id_to_links_map_type, db_id_to_links_map, none, none);
			
			struct MOE_Data network_moe_data;

			struct MOE_Data realtime_network_moe_data;

			vector<struct MOE_Data> td_network_moe_data_array;

			typedef unordered_map<int,float> id_to_travel_time_map_type;
			typedef typename MasterType::routable_network_type routable_network_type;
			typedef vector<routable_network_type*> network_snapshot_replicas_container_type;
			typedef unordered_map<int, network_snapshot_replicas_container_type> time_to_snapshot_map_type;
			typedef vector<int> snapshot_times_container_type;
			member_data(time_to_snapshot_map_type, network_snapshots, none, none);
			member_data(vector<int>, snapshot_times, none, none);

			typedef union {
				struct
				{
					long inbound_link_id;
					long outbound_link_id;
				};
				long long movement_id;
			} long_hash_key_type;

			typedef unordered_map<long,typename MasterType::turn_movement_type*> type_of_link_turn_movement_map;
			member_data(type_of_link_turn_movement_map, link_turn_movement_map, none, none);

			feature_implementation void initialize_intersection_control()
			{
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, typename _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for(intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template network_reference<_Network_Interface*>((_Network_Interface*)this);
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template set_node_control_plan_index<NULLTYPE>(scenario_reference<ComponentType,CallerType,_Scenario_Interface*>()->template simulation_start_time<int>());
				}
			}

			feature_implementation void simulation_initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				UNLOCK(_network_vht_vehicle_based_update_lock);
				initialize_intersection_control<ComponentType,CallerType,TargetType>();
				initialize_links<ComponentType,CallerType,TargetType>();
				initialize_intersections<ComponentType,CallerType,TargetType>();
				construct_network_cost<ComponentType,CallerType,TargetType>();
				construct_routable_network<ComponentType,CallerType,TargetType>();
				construct_realtime_routable_network<ComponentType,CallerType,TargetType>();
				if (((_Scenario_Interface*)_global_scenario)->template read_network_snapshots<bool>())
				{
					read_snapshots();
				}
				_network_vht = 0.0;
				_network_vmt = 0.0;
				_out_network_vht_vehicle_based = 0.0;
				initialize_moe();
				initialize_network_agent<ComponentType,CallerType,TargetType>();
			}

			void read_snapshots()
			{
				cout << "reading snapshots" << endl;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Link_Components::Prototypes::Link_Prototype<typename MasterType::link_type> _Link_Interface;
				fstream& input_network_snapshots_file = ((_Scenario_Interface*)_global_scenario)->template input_network_snapshots_file<fstream&>();
				if (!input_network_snapshots_file.is_open())
				{
					THROW_EXCEPTION(endl << "Cannot open snapshots file" << endl);
				}

				string line;
				int iline = 0;
				vector<string> tokens;
				int token_size;
				int link_uuid;
				float link_travel_time;
				int movement_uuid;
				float movement_forward_link_turn_travel_time;
				int num_movements;
				float maximum_free_flow_speed;
				Network_Components::Types::Link_ID_Dir link_id_dir;
				unordered_map<int,float> link_travel_time_map;
				unordered_map<int,float> movement_travel_time_map;
				_Link_Interface* link;
				while (input_network_snapshots_file.good())
				{
					getline(input_network_snapshots_file,line);
					iline = iline + 1;
					if(iline == 3) // skip the first three lines
					{
						break;
					}
				}
				bool first = true;
				int current_time;

				while(input_network_snapshots_file.good())
				{
					getline(input_network_snapshots_file, line);
					string_split(tokens, line);
					if (tokens.size() == 2)
					{
						// starting a new snapshot
						if (first)
						{
							first = false;
						}
						else 
						{
							// construct routable network using collected data
							network_snapshot_replicas_container_type network_snapshot_replicas_container;
							network_snapshot_replicas_container = construct_routables_from_snapshot(current_time, maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
							_network_snapshots[current_time] = network_snapshot_replicas_container;
							_snapshot_times.push_back(current_time);
						}
						link_travel_time_map.clear();
						movement_travel_time_map.clear();
						current_time = stoi(tokens[0]);
						maximum_free_flow_speed = stof(tokens[1]);
					}
					else if (tokens.size() == 5)
					{
						// starting link 
						//token_size = 3;
						//string_split(tokens, line, token_size);
						link_uuid = stoi(tokens[0]);
						link_id_dir.id = stol(tokens[1]);
						link_id_dir.dir = stol(tokens[2]);
						link_travel_time = stof(tokens[3]);
						link = (_Link_Interface*)(_link_dbid_dir_to_ptr_map[link_id_dir.id_dir]);
						link_travel_time_map[link->template uuid<int>()] = link_travel_time;
						num_movements = stoi(tokens[4]);
						// starting movements
						for (int i = 0; i < num_movements; i++)
						{
							getline(input_network_snapshots_file, line);
							token_size = 2;
							string_split(tokens, line, token_size);
							movement_uuid = stoi(tokens[0]);
							movement_forward_link_turn_travel_time = stof(tokens[1]);
							movement_travel_time_map[movement_uuid] = movement_forward_link_turn_travel_time;
						}
					}
					else
					{
						cout << "snapshot file format wrong" << endl;
					}
				}
				// construct routable network using collected data
				network_snapshot_replicas_container_type network_snapshot_replicas_container;
				network_snapshot_replicas_container = construct_routables_from_snapshot(current_time, maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
				_network_snapshots[current_time] = network_snapshot_replicas_container;
				_snapshot_times.push_back(current_time);
				//write_snapshot_from_snapshot_array();
			}

			void write_snapshot_from_snapshot_array()
			{
				typedef Network_Prototype<typename MasterType::routable_network_type> _Routable_Network_Interface;
				typedef Network_Prototype<typename MasterType::network_type> _Regular_Network_Interface;
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, typename _Routable_Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, typename MasterType::network_type);
				define_container_and_value_interface(_Routable_Intersections_Container_Interface, _Routable_Intersection_Interface, typename _Routable_Network_Interface::get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, typename MasterType::network_type);
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, typename MasterType::network_type);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, typename MasterType::network_type);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;

				for (int i = 0; i < (int)_snapshot_times.size(); i++)
				{
					int time = _snapshot_times[i];
					((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>() << time << endl;
					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)(((network_snapshot_replicas_container_type)(_network_snapshots.find(time)->second))[0]);
					_Routable_Intersections_Container_Interface& routable_intersections_container = routable_network->template intersections_container<_Routable_Intersections_Container_Interface&>();
					typename _Routable_Intersections_Container_Interface::iterator intersection_itr;
					for (intersection_itr = routable_intersections_container.begin(); intersection_itr != routable_intersections_container.end(); intersection_itr++)
					{
						_Routable_Intersection_Interface* intersection = (_Routable_Intersection_Interface*)(*intersection_itr);
						_Inbound_Outbound_Movements_Container_Interface& routable_inbound_outbound_movements_container = intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
						typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
						for (inbound_outbound_movements_itr = routable_inbound_outbound_movements_container.begin(); inbound_outbound_movements_itr != routable_inbound_outbound_movements_container.end(); inbound_outbound_movements_itr++)
						{
							_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
							_Routable_Link_Interface* inbound_link = inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>();
							_Movements_Container_Interface& outbound_movements = inbound_outbound_movements->template outbound_movements<_Movements_Container_Interface&>();
							((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>()
								<< inbound_link->template uuid<int>() << "\t"
								<< inbound_link->template travel_time<float>() << "\t"
								<< outbound_movements.size()
								<< endl;

							typename _Movements_Container_Interface::iterator movement_itr;
							for (movement_itr = outbound_movements.begin(); movement_itr != outbound_movements.end(); movement_itr++)
							{
								_Movement_Interface* movement = (_Movement_Interface*)(*movement_itr);
							
								((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>()
									<< movement->template uuid<int>() << ","
									<< movement->template forward_link_turn_travel_time<float>()
									<< endl;
							}
						}
					}
				}
				((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>().close();
				exit(0);
			}

			network_snapshot_replicas_container_type construct_routables_from_snapshot(int time, float maximum_free_flow_speed, id_to_travel_time_map_type& link_travel_time_map, id_to_travel_time_map_type& movement_travel_time_map)
			{
				define_container_and_value_interface_unqualified_container(_Routable_Networks_Container_Interface, _Routable_Network_Interface, type_of(routable_networks_container), Random_Access_Sequence_Prototype, Network_Components::Prototypes::Network_Prototype, typename MasterType::network_type);
				network_snapshot_replicas_container_type network_snapshot_replicas_container;
				for(int i=0;i<_num_threads;i++)
				{
					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename MasterType::routable_network_type>();
					routable_network->template read_snapshot_data<Target_Type<NULLTYPE,void,id_to_travel_time_map_type&>>(maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
					network_snapshot_replicas_container.push_back((typename MasterType::routable_network_type*)routable_network);
				}
				return network_snapshot_replicas_container;
			}

			void initialize_moe()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Network_Interface;

				//network_moe_data.starting_time = ((_Network_Interface*)this)->start_of_current_simulation_interval_absolute<int>();
				//network_moe_data.ending_time = network_moe_data.starting_time + ((_Scenario_Interface*)_global_scenario)->assignment_interval_length<int>();
	
				network_moe_data.assignment_calculation_time = 0.0;
				network_moe_data.simulation_calculation_time = 0.0f;
				network_moe_data.operation_calculation_time = 0.0f;
				network_moe_data.output_calculation_time = 0.0f;

				network_moe_data.num_arrived_vehicles = 0;
				network_moe_data.num_departed_vehicles = 0;
				network_moe_data.num_generated_vehicles = 0;
				network_moe_data.num_loaded_vehicles = 0;
	
				network_moe_data.network_avg_link_density = 0.0f;
				network_moe_data.network_avg_link_density_ratio = 0.0f;
				network_moe_data.network_avg_link_in_flow_rate = 0.0f;
				network_moe_data.network_avg_link_in_flow_ratio = 0.0f;
				network_moe_data.network_avg_link_out_flow_rate = 0.0f;
				network_moe_data.network_avg_link_out_flow_ratio = 0.0f;
				network_moe_data.network_avg_link_in_volume = 0.0f;
				network_moe_data.network_avg_link_out_volume = 0.0f;
				network_moe_data.network_avg_link_queue_length = 0.0f;
				network_moe_data.network_avg_link_speed = 0.0f;
				network_moe_data.network_avg_link_speed_ratio = 0.0f;
				network_moe_data.network_avg_link_travel_delay = 0.0f;
				network_moe_data.network_avg_link_travel_time = 0.0f;
				network_moe_data.network_avg_link_travel_time_ratio = 0.0f;

				network_moe_data.num_arrived_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>();
				network_moe_data.num_departed_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>();
				network_moe_data.num_loaded_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>();
			}

			feature_implementation void initialize_network_agent()
			{
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				load_event(ComponentType,ComponentType::template End_Iteration_Conditional,ComponentType::template End_Iteration_Handler, ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::NETWORK_SNAPSHOT_SUB_ITERATION,NULLTYPE);
                _start_cpu_time_in_seconds = (long)get_current_cpu_time_in_seconds();
 			}

			declare_feature_conditional(End_Iteration_Conditional)
			{
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, typename MasterType::network_type);
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::NETWORK_SNAPSHOT_SUB_ITERATION)
				{
					((typename MasterType::network_type*)_this)->Swap_Event((Event)&Snapshot_Subiteration_Handler<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_COMPUTATION_SUB_ITERATION;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_COMPUTATION_SUB_ITERATION)
				{
					((typename MasterType::network_type*)_this)->Swap_Event((Event)&End_Iteration_Handler<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::NETWORK_SNAPSHOT_SUB_ITERATION;
				} 
				else
				{
					cout << "Should never reach here in network conditional!" << endl;
					assert(false);
				}
			}

			declare_feature_event(Snapshot_Subiteration_Handler)
			{
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, typename MasterType::network_type);
				if (((_Network_Interface*)_this)->template start_of_current_simulation_interval_absolute<int>() % ((_Scenario_Interface*)_global_scenario)->template snapshot_period<int>() == 0)
				{
					//((typename MasterType::network_type*)_this)->create_snapshot();
					if (((_Scenario_Interface*)_global_scenario)->template write_network_snapshots<bool>())
					{
						cout << "wrting network snapshot" << endl;
						((typename MasterType::network_type*)_this)->output_snapshot();
					}
				}
				if (((_Scenario_Interface*)_global_scenario)->template compare_with_historic_moe<bool>() && ((_Network_Interface*)_this)->template start_of_current_simulation_interval_absolute<int>() % ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					((typename MasterType::network_type*)_this)->read_historic_link_moe();
				}
				if (((_Scenario_Interface*)_global_scenario)->template read_normal_day_link_moe<bool>() && ((_Network_Interface*)_this)->template start_of_current_simulation_interval_absolute<int>() % ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					((typename MasterType::network_type*)_this)->read_normal_day_link_moe();
				}
				if (((((_Network_Interface*)_this)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					((typename MasterType::network_type*)_this)->_in_network_vht_vehicle_based = 0.0;
				}
			}

			void read_historic_link_moe()
			{
				// do nothing for non-visualized network implementaiton
			}

			void read_normal_day_link_moe()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;

				fstream& normal_day_link_moe_file = ((_Scenario_Interface*)_global_scenario)->template normal_day_link_moe_file<fstream&>();
				if (!normal_day_link_moe_file.is_open())
				{
					THROW_EXCEPTION(endl << "Link MOE reference file cannot be opened" << endl);
				}
				struct Link_Components::Implementations::Link_MOE_Data link_moe_data;
				vector<string> tokens;
				string line;
				int current_time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>();
				int record_time;
				typename MasterType::link_type* link;
				Network_Components::Types::Link_ID_Dir link_id_dir;
				for (int i = 0; i < (int)_links_container.size(); i++)
				{
					getline(normal_day_link_moe_file, line); 
					string_split(tokens, line);
					if (tokens.size() == 0)
					{
						break;
					}
					record_time = stoi(tokens[1]);
					if (record_time < current_time)
					{
						// skip until current time
						while(normal_day_link_moe_file.good())
						{
							getline(normal_day_link_moe_file, line);
							string_split(tokens, line);
							record_time = stoi(tokens[1]);
							if (record_time >= current_time)
							{
								break;
							}
						}
					}
					link_id_dir.id = stol(tokens[3]);
					link_id_dir.dir = stol(tokens[4]);
					link = (typename MasterType::link_type*)(_link_dbid_dir_to_ptr_map[link_id_dir.id_dir]);
					int offset = 8;
					link_moe_data.link_travel_time = stof(tokens[offset++]);
					link_moe_data.link_travel_time_standard_deviation = stof(tokens[offset++]);
					link_moe_data.link_queue_length = stof(tokens[offset++]);
					link_moe_data.link_travel_delay = stof(tokens[offset++]);
					link_moe_data.link_travel_delay_standard_deviation = stof(tokens[offset++]);
					link_moe_data.link_speed = stof(tokens[offset++]);
					link_moe_data.link_density = stof(tokens[offset++]);
					link_moe_data.link_in_flow_rate = stof(tokens[offset++]);
					link_moe_data.link_out_flow_rate = stof(tokens[offset++]);
					link_moe_data.link_in_volume = stof(tokens[offset++]);
					link_moe_data.link_out_volume = stof(tokens[offset++]);
					link_moe_data.link_speed_ratio = stof(tokens[offset++]);
					link_moe_data.link_in_flow_ratio = stof(tokens[offset++]);
					link_moe_data.link_out_flow_ratio = stof(tokens[offset++]);
					link_moe_data.link_density_ratio = stof(tokens[offset++]);
					link_moe_data.link_travel_time_ratio = stof(tokens[offset++]);
					link_moe_data.num_vehicles_in_link = stof(tokens[offset++]);
					link->normal_day_link_moe_data = link_moe_data;
				}
			}

			void create_snapshot()
			{
				typedef Network_Prototype<typename MasterType::network_type> _Regular_Network_Interface;
				cout << "creating network snapshot at " << ((_Regular_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>() << endl;
				typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::Regular_Network,_Regular_Network_Interface*> Net_IO_Type;
				define_container_and_value_interface_unqualified_container(_Routable_Networks_Container_Interface, _Routable_Network_Interface, type_of(network_snapshot_container), Random_Access_Sequence_Prototype, Network_Components::Prototypes::Network_Prototype, typename MasterType::network_type);
				_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename MasterType::routable_network_type>();
				routable_network->template read_network_data<Net_IO_Type>((_Regular_Network_Interface*)this);
				_network_snapshot_container.push_back((typename MasterType::routable_network_type*)routable_network);
			}


			void output_snapshot()
			{
				typedef Network_Prototype<typename MasterType::routable_network_type> _Routable_Network_Interface;
				typedef Network_Prototype<typename MasterType::network_type> _Regular_Network_Interface;
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, typename _Routable_Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, typename MasterType::network_type);
				define_container_and_value_interface(_Routable_Intersections_Container_Interface, _Routable_Intersection_Interface, typename _Routable_Network_Interface::get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, typename MasterType::network_type);
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, typename MasterType::network_type);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, typename MasterType::network_type);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				define_component_interface(_Regular_Link_Interface, typename _Routable_Link_Interface::get_type_of(network_link_reference), Link_Components::Prototypes::Link_Prototype, typename MasterType::network_type);

				_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)_routable_networks_container[0];
				((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>() << endl << ((_Regular_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>() << "\t" << routable_network->template max_free_flow_speed<float>();

				_Routable_Intersections_Container_Interface& routable_intersections_container = routable_network->template intersections_container<_Routable_Intersections_Container_Interface&>();
				typename _Routable_Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = routable_intersections_container.begin(); intersection_itr != routable_intersections_container.end(); intersection_itr++)
				{
					_Routable_Intersection_Interface* intersection = (_Routable_Intersection_Interface*)(*intersection_itr);
					_Inbound_Outbound_Movements_Container_Interface& routable_inbound_outbound_movements_container = intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
					typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
					for (inbound_outbound_movements_itr = routable_inbound_outbound_movements_container.begin(); inbound_outbound_movements_itr != routable_inbound_outbound_movements_container.end(); inbound_outbound_movements_itr++)
					{
						_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
						_Routable_Link_Interface* inbound_link = inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>();
						_Movements_Container_Interface& outbound_movements = inbound_outbound_movements->template outbound_movements<_Movements_Container_Interface&>();
						((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>()
							<< endl
							<< inbound_link->template uuid<int>() << "\t"
							<< inbound_link->template network_link_reference<_Regular_Link_Interface*>()->template dbid<int>() << "\t"
							<< inbound_link->template network_link_reference<_Regular_Link_Interface*>()->template direction<int>() << "\t"
							<< inbound_link->template travel_time<float>() << "\t"
							<< outbound_movements.size();

						typename _Movements_Container_Interface::iterator movement_itr;
						for (movement_itr = outbound_movements.begin(); movement_itr != outbound_movements.end(); movement_itr++)
						{
							_Movement_Interface* movement = (_Movement_Interface*)(*movement_itr);
							
							((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>()
								<< endl
								<< movement->template uuid<int>() << ","
								<< movement->template forward_link_turn_travel_time<float>();
						}
					}
				}
			}

			feature_implementation TargetType get_routable_network_from_snapshots(int current_time)
			{
				typedef Network_Prototype<routable_network_type> _Routable_Network_Interface;
				typename vector<int>::iterator time_itr;
				int closest_time = -1;
				for (time_itr = _snapshot_times.begin(); time_itr != _snapshot_times.end(); time_itr++)
				{
					int time = (int)(*time_itr);
					if (current_time >= time)
					{
						closest_time = time;
						continue;
					}
					else
					{
						break;
					}
				}

				routable_network_type* routable_network = ((network_snapshot_replicas_container_type)(_network_snapshots.find(closest_time)->second))[_thread_id];
				return (TargetType)routable_network;
			}

			declare_feature_event(End_Iteration_Handler)
			{
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, typename MasterType::network_type);
				_Network_Interface* _this_ptr = (_Network_Interface*)_this;
				if (_this_ptr->template start_of_current_simulation_interval_absolute<int>() > _this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_end_time<int>())
				{
					_this_ptr->template scenario_reference<_Scenario_Interface*>()->template close_files<NULLTYPE>();
					exit(0);
				}

				((typename MasterType::network_type*)_this)->template calculate_moe<NULLTYPE,NULLTYPE,NULLTYPE>();
				//((typename MasterType::network_type*)_this)->template update_vehicle_locations<NULLTYPE,NULLTYPE,NULLTYPE>();
				((typename MasterType::network_type*)_this)->template printResults<NULLTYPE,NULLTYPE,NULLTYPE>();
			}

			feature_implementation void update_vehicle_locations()
			{
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					((_Intersection_Interface*)(*intersection_itr))->template update_vehicle_locations<NULLTYPE>();
				}
			}

			feature_implementation void calculate_moe()
			{
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type, ComponentType> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;

				typename _Intersections_Container_Interface::iterator intersection_itr;
				//for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				//{
				//	((_Intersection_Interface*)(*intersection_itr))->template calculate_moe_for_simulation_interval<NULLTYPE>();
				//}
				
				if (((_Scenario_Interface*)_global_scenario)->template calculate_realtime_moe<bool>())
				{
					calculate_realtime_network_moe();
					output_moe_for_simulation_interval<ComponentType, CallerType, TargetType>();
				}

				if (((((_Network_Interface*)this)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					//for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
					//{
					//	((_Intersection_Interface*)(*intersection_itr))->template calculate_moe_for_assignment_interval<NULLTYPE>();
					//	((_Intersection_Interface*)(*intersection_itr))->template update_in_network_vehicle_vht<NT>();
					//}
					_network_vht_vehicle_based = _in_network_vht_vehicle_based + _out_network_vht_vehicle_based;
					link_moe_post_process();
					update_moe_for_assignment_interval_with_links();
					update_moe_for_assignment_interval();
					output_moe_for_assignment_interval<ComponentType, CallerType, TargetType>();
					reset_moe_for_assignment_interval();
				}
			}

			void link_moe_post_process()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Network_Interface;
				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;
				int end_time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				int start_time = end_time - ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>();
				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_link_component_type* link_component = (_link_component_type*)(*link_itr);
					link_component->link_moe_data.start_time = start_time;
					link_component->link_moe_data.end_time = end_time;
					link_component->non_volatile_link_moe_data = link_component->link_moe_data;
				}
			}

			void calculate_realtime_network_moe()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;

				realtime_network_moe_data.network_avg_link_density = 0.0;
				realtime_network_moe_data.network_avg_link_out_flow_rate = 0.0;
				realtime_network_moe_data.network_avg_link_in_flow_rate = 0.0;
				realtime_network_moe_data.network_avg_link_out_volume = 0.0;
				realtime_network_moe_data.network_avg_link_in_volume = 0.0;
				realtime_network_moe_data.network_avg_link_queue_length = 0.0;
				realtime_network_moe_data.network_avg_link_speed = 0.0;
				realtime_network_moe_data.network_avg_link_travel_time = 0.0;
				realtime_network_moe_data.network_avg_link_travel_delay = 0.0;

				realtime_network_moe_data.network_avg_link_density_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_out_flow_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_in_flow_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_speed_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_travel_time_ratio = 0.0;

				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_link_component_type* link_component = (_link_component_type*)(*link_itr);

					realtime_network_moe_data.network_avg_link_density += link_component->realtime_link_moe_data.link_density;
					realtime_network_moe_data.network_avg_link_out_flow_rate += link_component->realtime_link_moe_data.link_out_flow_rate;
					realtime_network_moe_data.network_avg_link_in_flow_rate += link_component->realtime_link_moe_data.link_in_flow_rate;
					realtime_network_moe_data.network_avg_link_out_volume += link_component->realtime_link_moe_data.link_out_volume;
					realtime_network_moe_data.network_avg_link_in_volume += link_component->realtime_link_moe_data.link_in_volume;
					realtime_network_moe_data.network_avg_link_queue_length += link_component->realtime_link_moe_data.link_queue_length;
					realtime_network_moe_data.network_avg_link_speed += link_component->realtime_link_moe_data.link_speed;
					realtime_network_moe_data.network_avg_link_travel_time += link_component->realtime_link_moe_data.link_travel_time;
					realtime_network_moe_data.network_avg_link_travel_delay += link_component->realtime_link_moe_data.link_travel_delay;

					realtime_network_moe_data.network_avg_link_density_ratio += link_component->realtime_link_moe_data.link_density_ratio;
					realtime_network_moe_data.network_avg_link_out_flow_ratio += link_component->realtime_link_moe_data.link_out_flow_ratio;
					realtime_network_moe_data.network_avg_link_in_flow_ratio += link_component->realtime_link_moe_data.link_in_flow_ratio;
					realtime_network_moe_data.network_avg_link_speed_ratio += link_component->realtime_link_moe_data.link_speed_ratio;
					realtime_network_moe_data.network_avg_link_travel_time_ratio += link_component->realtime_link_moe_data.link_travel_time_ratio;
				}

				realtime_network_moe_data.network_avg_link_density /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_out_flow_rate /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_in_flow_rate /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_out_volume /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_in_volume /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_queue_length /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_speed /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_travel_time /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_travel_delay /= float(_links_container.size());

				realtime_network_moe_data.network_avg_link_density_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_out_flow_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_in_flow_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_speed_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_travel_time_ratio /= float(_links_container.size());

				realtime_network_moe_data.num_arrived_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>();
				realtime_network_moe_data.num_departed_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>();
				realtime_network_moe_data.num_loaded_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>();
			}


			void update_moe_for_assignment_interval_with_links()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;

				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
						
					_link_component_type* link_component = (_link_component_type*)(*link_itr);
					network_moe_data.network_avg_link_travel_delay += link_component->link_moe_data.link_travel_delay;
					network_moe_data.network_avg_link_travel_time += link_component->link_moe_data.link_travel_time;
					network_moe_data.network_avg_link_speed += link_component->link_moe_data.link_speed;
					network_moe_data.network_avg_link_density += link_component->link_moe_data.link_density;
					network_moe_data.network_avg_link_in_flow_rate += link_component->link_moe_data.link_in_flow_rate;
					network_moe_data.network_avg_link_out_flow_rate += link_component->link_moe_data.link_out_flow_rate;
					network_moe_data.network_avg_link_in_volume += link_component->link_moe_data.link_in_volume;
					network_moe_data.network_avg_link_out_volume += link_component->link_moe_data.link_out_volume;
					network_moe_data.network_avg_link_density_ratio +=link_component->link_moe_data.link_density_ratio;
					network_moe_data.network_avg_link_in_flow_ratio += link_component->link_moe_data.link_in_flow_ratio;
					network_moe_data.network_avg_link_out_flow_ratio += link_component->link_moe_data.link_out_flow_ratio;
					network_moe_data.network_avg_link_speed_ratio += link_component->link_moe_data.link_speed_ratio;
					network_moe_data.network_avg_link_travel_time_ratio += link_component->link_moe_data.link_travel_time_ratio;

					_network_vmt += link_component->_link_vmt;
					_network_vht += link_component->_link_vht;
				}
			}

			void update_moe_for_assignment_interval()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;

				network_moe_data.num_arrived_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>() - network_moe_data.num_arrived_vehicles;
				network_moe_data.num_departed_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>() - network_moe_data.num_departed_vehicles;
				network_moe_data.num_loaded_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>() - network_moe_data.num_loaded_vehicles;

				//performance
				network_moe_data.network_avg_link_travel_time /= float(_links_container.size()); 
				network_moe_data.network_avg_link_speed /= float(_links_container.size()); 
				network_moe_data.network_avg_link_density /= float(_links_container.size()); 
				network_moe_data.network_avg_link_in_flow_rate  /= float(_links_container.size()); 
				network_moe_data.network_avg_link_out_flow_rate  /= float(_links_container.size()); 
				network_moe_data.network_avg_link_in_volume  /= float(_links_container.size()); 
				network_moe_data.network_avg_link_out_volume  /= float(_links_container.size()); 
				network_moe_data.network_avg_link_queue_length  /= float(_links_container.size()); 

				network_moe_data.network_avg_link_travel_time_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_speed_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_density_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_in_flow_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_out_flow_ratio /= float(_links_container.size());

				td_network_moe_data_array.push_back(network_moe_data);
			}

			void reset_moe_for_assignment_interval()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				define_container_and_value_interface_unqualified_container(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, NULLTYPE);
				// reset network moe				
				initialize_moe();
				// reset link moe
				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;
				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_link_component_type* link_component = (_link_component_type*)(*link_itr);
					link_component->initialize_moe();
				}
				// reset turn movement moe
				typename _Turn_Movements_Container_Interface::iterator movement_itr;
				typedef typename MasterType::turn_movement_type _movement_component_type;
				for (movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
				{
					_movement_component_type* movement_component = (_movement_component_type*)(*movement_itr);
					movement_component->initialize_moe();
				}
			}

			feature_implementation void initialize_links()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = _links_container.begin(); links_itr != _links_container.end(); links_itr++)
				{
					((_Link_Interface*)(*links_itr))->template initialize_features<ComponentType*>(this_component());
				}
			}

			feature_implementation void initialize_intersections()
			{
				//determine minimum merge rate
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					((_Intersection_Interface*)(*intersection_itr))->template initialize_features<ComponentType*>(this_component());
				}
			}

			feature_implementation void construct_network_cost()
			{
				// break up between links and movements
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				_max_free_flow_speed = -1.0;			
				for (links_itr = _links_container.begin(); links_itr != _links_container.end(); links_itr++)
				{			
					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					float free_flow_speed = (float) ((5280.0) * link->template free_flow_speed<float>()/3600.0); // feet per second
					float link_travel_time = float (link->template length<float>() / free_flow_speed);
					
					_max_free_flow_speed = max(_max_free_flow_speed,free_flow_speed);

					link_travel_time = max((float)1.0,link_travel_time);
					link->template travel_time<float>(link_travel_time);
				}
		
				define_container_and_value_interface_unqualified_container(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = _turn_movements_container.begin(); turn_movements_itr != _turn_movements_container.end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* mvmt_itf=(_Turn_Movement_Interface*)(*turn_movements_itr);

					_Link_Interface* inboundLink = mvmt_itf->template inbound_link<_Link_Interface*>();
					_Link_Interface* outboundLink = mvmt_itf->template outbound_link<_Link_Interface*>();

					float turn_travel_penalty = 0.0f;
					if (mvmt_itf->template movement_rule<int>() == Turn_Movement_Components::Types::PROHIBITED)
					{
						turn_travel_penalty = INFINITY_FLOAT;
					}
					float forward_link_turn_travel_time = inboundLink->template travel_time<float>() + turn_travel_penalty;
					
					mvmt_itf->template turn_travel_penalty<float>(turn_travel_penalty);
					mvmt_itf->template forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
				}
			}

			// call it from initilization; move to implementation
			feature_implementation void construct_routable_network()
			{
				typedef Network_Prototype<typename MasterType::network_type> _Regular_Network_Interface;
				typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::Regular_Network,_Regular_Network_Interface*> Net_IO_Type;
				define_container_and_value_interface_unqualified_container(_Routable_Networks_Container_Interface, _Routable_Network_Interface, type_of(routable_networks_container), Random_Access_Sequence_Prototype, Network_Components::Prototypes::Network_Prototype, ComponentType);
				for(int i=0;i<_num_threads;i++)
				{
					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename MasterType::routable_network_type>();
					routable_network->template read_network_data<Net_IO_Type>((_Regular_Network_Interface*)this);
					_routable_networks_container.push_back((typename MasterType::routable_network_type*)routable_network);
				}
			}

			feature_implementation void construct_realtime_routable_network()
			{
				typedef Network_Prototype<typename MasterType::network_type> _Regular_Network_Interface;
				typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::Regular_Network,_Regular_Network_Interface*> Net_IO_Type;
				define_container_and_value_interface_unqualified_container(_Routable_Networks_Container_Interface, _Routable_Network_Interface, type_of(routable_networks_container), Random_Access_Sequence_Prototype, Network_Components::Prototypes::Network_Prototype, ComponentType);
				for(int i=0;i<_num_threads;i++)
				{
					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename MasterType::routable_network_type>();
					routable_network->template read_realtime_network_data<Net_IO_Type>((_Regular_Network_Interface*)this);
					_realtime_routable_networks_container.push_back((typename MasterType::routable_network_type*)routable_network);
				}
			}

			//==================================================================================================================
			/// Write out intermediate simulation results
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation void printResults() 
			{
						
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				_Scenario_Interface* scenario = scenario_reference<ComponentType,CallerType,_Scenario_Interface*>();
				_Network_Interface* _this_ptr = (_Network_Interface*)this;
				printf("%s, ", convert_seconds_to_hhmmss(_this_ptr->template start_of_current_simulation_interval_absolute<int>()).c_str());
				//printf("loaded=%7d, departed=%7d, arrived=%7d, in_network=%7d, VMT=%7f, VHT=%7f\n",scenario->template network_cumulative_loaded_vehicles<int>(),scenario->template network_cumulative_departed_vehicles<int>(),scenario->template network_cumulative_arrived_vehicles<int>(),scenario->template network_in_network_vehicles<int>(),_network_vmt, _network_vht);
				printf("departed=%7d, arrived=%7d, in_network=%7d, VMT=%7.2f, VHT=%7.2f\n",scenario->template network_cumulative_departed_vehicles<int>(),scenario->template network_cumulative_arrived_vehicles<int>(),scenario->template network_in_network_vehicles<int>(),_network_vmt, _network_vht);
				if (((_Scenario_Interface*)_global_scenario)->template write_node_control_state<bool>())
				{
					write_node_control_state<ComponentType,CallerType,TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_vehicle_trajectory<bool>())
				{
					write_vehicle_trajectory<ComponentType,CallerType,TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_network_link_flow<bool>())
				{
					write_network_link_flow<ComponentType,CallerType,TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_network_link_turn_time<bool>())
				{
					write_network_link_turn_time<ComponentType,CallerType,TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_output_summary<bool>())
				{
					write_output_summary<ComponentType,CallerType,TargetType>();
				}
			}

			feature_implementation void increase_in_network_vht_vehicle_based(float increase)
			{
				LOCK(_network_vht_vehicle_based_update_lock);
				_in_network_vht_vehicle_based += increase;
				UNLOCK(_network_vht_vehicle_based_update_lock);
			}

			feature_implementation void increase_out_network_vht_vehicle_based(float increase)
			{
				LOCK(_network_vht_vehicle_based_update_lock);
				_out_network_vht_vehicle_based += increase;
				UNLOCK(_network_vht_vehicle_based_update_lock);
			}
			
			feature_implementation void write_node_control_state();

			feature_implementation void write_vehicle_trajectory();

			feature_implementation void write_network_link_flow();

			feature_implementation void write_network_link_turn_time();
			
			feature_implementation void write_output_summary();

			//==================================================================================================================
			/// Convert network data from Polaris structure to C++ data structure to be written to a file
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation void write_network_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_intersection_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_link_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_turn_movement_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_activity_location_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_zone_data(network_models::network_information::network_data_information::NetworkData& network_data);

			//==================================================================================================================
			/// read from database
			//------------------------------------------------------------------------------------------------------------------		
			feature_implementation void read_network_data(Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				define_component_interface(_DB_Interface,type_of(db_reader),Prototypes::Network_DB_Reader_Prototype,ComponentType);
				_DB_Interface* db = (_DB_Interface*)&_db_reader;
				db->template network_reference<ComponentType*>((ComponentType*)this);
				db->template read_network_data<Network_Components::Types::Network_IO_Maps&>(net_io_maps);
			}


			//==================================================================================================================
			/// Convert network data from C++ data structure to Plaris structure
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation void read_network_data(network_models::network_information::network_data_information::NetworkData& network_data); tag_feature_as_available(read_network_data);

			feature_implementation void read_intersection_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_link_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_turn_movement_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_activity_location_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_zone_data(network_models::network_information::network_data_information::NetworkData& network_data);

			//==================================================================================================================
			/// output moe
			//------------------------------------------------------------------------------------------------------------------			
			feature_implementation void output_moe_for_simulation_interval();
			feature_implementation void output_moe_for_assignment_interval();

			//==================================================================================================================
			/// network events
			//------------------------------------------------------------------------------------------------------------------

			member_prototype(Network_Event_Manager, network_event_manager, typename MasterType::network_event_manager_type, none, none);

			//==================================================================================================================
			/// traffic management center
			//------------------------------------------------------------------------------------------------------------------
			member_prototype(Traffic_Management_Center, traffic_management_center, typename type_of(MasterType::traffic_management_center), none, none);

		};

		implementation struct Integrated_Polaris_Network_Implementation : public Polaris_Network_Implementation<MasterType,ParentType,APPEND_CHILD(Integrated_Polaris_Network_Implementation)>
		{

			member_component(typename MasterType::network_skim_type, skimming_faculty,none,none);

			feature_implementation typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin, typename TargetType::ParamType Destination, typename TargetType::Param2Type Mode_Indicator)
			{
				Polaris_Network_Implementation<MasterType,ParentType>* base_this = (Polaris_Network_Implementation<MasterType,ParentType>*)this;
				
				define_component_interface(skim_faculty_itf,type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,CallerType);

				skim_faculty_itf* skim_faculty = this->skimming_faculty<ComponentType,CallerType,skim_faculty_itf*>();

				return (TargetType::ReturnType)(skim_faculty->template Get_Current_LOS<TargetType>(Origin,Destination,Mode_Indicator));
			}
		
		};



	}
}

using namespace Network_Components::Implementations;