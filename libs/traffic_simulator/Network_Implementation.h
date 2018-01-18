#pragma once
#include "Network_Prototype.h"
#include "Traffic_Management_Center_Prototype.h"
#include "Network_Event_Prototype.h"
#include "Link_Implementation.h"
#include "Analyze_Link_Group_Implementation.h" 
#include "Network_DB_Reader_Implementations.h"

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

			//std::vector<Link_MOE_Data> link_moe_data_by_type_array;

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

			float network_routed_ttime;
			float network_relative_gap;

		};

		implementation struct Network_Implementation:public Polaris_Component<MasterType,INHERIT(Network_Implementation),Execution_Object>
		{
			typedef int My_Type;

			typedef typename Polaris_Component<MasterType,INHERIT(Network_Implementation),Execution_Object>::Component_Type ComponentType;

			m_data(typename MasterType::network_db_reader_type*, db_reader, NONE, NONE);
			//typedef std::unordered_map<long long,void*> type_of_link_dbid_dir_to_ptr_map;
			//m_data(concat(std::unordered_map<long long,void*>), link_dbid_dir_to_ptr_map, NONE, NONE);
			m_data(concat(std::unordered_map<long long,void*>), link_dbid_dir_to_ptr_map, NONE, NONE);

			m_data(float, max_free_flow_speed, NONE, NONE);

			m_container(std::vector<typename MasterType::intersection_type*>, intersections_container, NONE, NONE);

			m_container(std::vector<typename MasterType::link_type*>, links_container, NONE, NONE);

			m_container(std::vector<typename MasterType::transit_route_type*>, transit_routes_container, NONE, NONE);

			m_container(std::vector<typename MasterType::transit_pattern_type*>, transit_patterns_container, NONE, NONE);

			m_container(std::vector<typename MasterType::transit_vehicle_trip_type*>, transit_vehicle_trips_container, NONE, NONE);
			//m_container(std::vector<Link<typename MasterType::link_type>*>, links_container, NONE, NONE);

			//TODO:ROUTING
			//m_container(std::vector<typename MasterType::routable_network_type*>, routable_networks_container, NONE, NONE);

			//TODO:ROUTING
			//m_container(std::vector<typename MasterType::routable_network_type*>, realtime_routable_networks_container, NONE, NONE);

			//TODO:ROUTING
			//template<typename TargetType>
			//TargetType routable_network(){return (TargetType)(_routable_networks_container[__thread_id]);}tag_getter_as_available(routable_network);

			//TODO:ROUTING
			//template<typename TargetType>
			//TargetType realtime_routable_network(){return (TargetType)(_realtime_routable_networks_container[__thread_id]);}tag_getter_as_available(realtime_routable_network);

			m_container(std::vector<typename MasterType::turn_movement_type*>, turn_movements_container, NONE, NONE);

			m_container(std::vector<typename MasterType::activity_location_type*>, activity_locations_container, NONE, NONE);
			m_container(concat(dense_hash_map<int,int>), activity_location_id_to_idx_container, NONE, NONE);

			m_container(concat(dense_hash_map<int,typename MasterType::zone_type*>), zones_container, NONE, NONE);

			m_container(std::vector<int>, zone_ids_container, NONE, NONE);
			
			m_container(std::vector<typename MasterType::movement_type*>, movements_container, NONE, NONE);

			m_container(std::vector<typename MasterType::ramp_metering_type*>, ramp_metering_container, NONE, NONE);

			//Null_Prototype<typename MasterType::scenario_type>* _scenario_reference;
			//template<typename TargetType> void scenario_reference(TargetType val){}
			//template<typename TargetType> TargetType scenario_reference(){return (TargetType)_scenario_reference;}

			m_prototype(Null_Prototype,typename MasterType::scenario_type, scenario_reference, NONE, NONE);

			m_container(std::vector<typename MasterType::intersection_control_type*>, intersection_controls_container, NONE, NONE);

			m_data(float, network_vmt, NONE, NONE);

			m_data(float, network_vht, NONE, NONE);

            m_data(long,current_cpu_time_in_seconds, NONE, NONE);

			m_data(long,start_cpu_time_in_seconds, NONE, NONE);

			m_data(float, in_network_vht_vehicle_based, NONE, NONE);
			m_data(float, out_network_vht_vehicle_based, NONE, NONE);
			m_data(float, network_vht_vehicle_based, NONE, NONE);
			m_data(_lock, network_vht_vehicle_based_update_lock, NONE, NONE);

			m_data(float, network_vht_in_network_based, NONE, NONE);
			m_data(_lock, network_vht_compensation_update_lock, NONE, NONE);
			m_data(int, network_vht_compensation, NONE, NONE);

			//TODO:ROUTING
			//m_container(std::vector<typename MasterType::routable_network_type*>, network_snapshot_container, NONE, NONE);
			
			m_container(std::vector<typename MasterType::analyze_link_group_type*>, analyze_link_groups_container, NONE, NONE);

			typedef std::unordered_map<int,std::vector<typename MasterType::link_type*>> id_to_links_map_type;
			m_data(id_to_links_map_type, db_id_to_links_map, NONE, NONE);
			
			struct MOE_Data network_moe_data;

			struct MOE_Data realtime_network_moe_data;

			std::vector<struct MOE_Data> td_network_moe_data_array;

			typedef std::unordered_map<int,float> id_to_travel_time_map_type;
			
			//TODO:ROUTING
			//typedef typename MasterType::routable_network_type routable_network_type;
			//typedef std::vector<routable_network_type*> network_snapshot_replicas_container_type;
			//typedef std::unordered_map<int, network_snapshot_replicas_container_type> time_to_snapshot_map_type;
			//m_data(time_to_snapshot_map_type, network_snapshots, NONE, NONE);

			typedef std::vector<int> snapshot_times_container_type;
			m_data(std::vector<int>, snapshot_times, NONE, NONE);

			std::vector<int> ttime_distribution;
			m_data(_lock, ttime_distribution_lock, NONE, NONE);

			typedef union {
				struct
				{
					long inbound_link_id;
					long outbound_link_id;
				};
				long long movement_id;
			} long_hash_key_type;

			typedef std::unordered_map<long long,typename MasterType::turn_movement_type*> type_of_link_turn_movement_map;
			m_data(type_of_link_turn_movement_map, link_turn_movement_map, NONE, NONE);

			m_container(std::vector<Routable_Network<typename MasterType::routable_network_type>*>,routable_networks,NONE,NONE);

			template<typename Routable_Network_Type>
			Routable_Network<Routable_Network_Type>* routable_network(int thread_index){ return _routable_networks[thread_index]; }

			template<typename Link_Type>
			Link<Link_Type>* get_link_ptr(unsigned int uuid)
			{
				Types::Link_ID_Dir link_id_dir;

				link_id_dir.id = uuid / 2;
				link_id_dir.dir = uuid - (uuid / 2)*2;

				if(_link_dbid_dir_to_ptr_map.count(link_id_dir.id_dir))
				{
					Link<Link_Type>* link = (Link<Link_Type>*)_link_dbid_dir_to_ptr_map[link_id_dir.id_dir];

					return link;
				}
				else
				{
					return nullptr;
				}

			}

			template<typename TargetType> void initialize_intersection_control()
			{
				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
				typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Intersection_Control< typename _Intersection_Interface::get_type_of(intersection_control)> _Intersection_Control_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;

				typedef Network<typename MasterType::network_type> _Network_Interface;
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for(intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template network_reference<_Network_Interface*>((_Network_Interface*)this);
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template set_node_control_plan_index<NULLTYPE>(scenario_reference<_Scenario_Interface*>()->template simulation_start_time<int>());
				}
			}

			template<typename TargetType> void simulation_initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				UNLOCK(_network_vht_vehicle_based_update_lock);
				UNLOCK(_network_vht_compensation_update_lock);
				UNLOCK(_ttime_distribution_lock);
				initialize_intersection_control<TargetType>();
				initialize_links<TargetType>();
				initialize_intersections<TargetType>();
				construct_network_cost<TargetType>();
				
				construct_routable_networks();
				
				//TODO:ROUTING_OPERATION
				//construct_realtime_routable_network<TargetType>();
				
				if (((_Scenario_Interface*)_global_scenario)->template read_network_snapshots<bool>())
				{
					read_snapshots();
				}
				if (((_Scenario_Interface*)_global_scenario)->template load_analyze_link_groups_from_file<bool>())
				{
					read_analyze_link_groups<TargetType>();
				}
				_network_vht = 0.0;
				_network_vmt = 0.0;
				_out_network_vht_vehicle_based = 0.0;
				_network_vht_vehicle_based = 0;
				initialize_moe();
				network_moe_data.network_routed_ttime = 0.0f;
				network_moe_data.network_relative_gap = 0.0f;

				initialize_network_agent<TargetType>();
				int num_epoches = 24*60/5 + 1;
				ttime_distribution.resize(num_epoches);
				for (int i = 0; i < (int)ttime_distribution.size(); i++)
				{
					ttime_distribution[i] = 0;
				}
			}

			template<typename TargetType> void update_ttime_distribution(int ttime)
			{
				LOCK(_ttime_distribution_lock);
				//if (ttime < 5) ttime_distribution[0]++;
				//else if (ttime < 10) ttime_distribution[1]++;
				//else if (ttime < 15) ttime_distribution[2]++;
				//else if (ttime < 20) ttime_distribution[3]++;
				//else if (ttime < 25) ttime_distribution[4]++;
				//else if (ttime < 30) ttime_distribution[5]++;
				//else if (ttime < 35) ttime_distribution[6]++;
				//else if (ttime < 40) ttime_distribution[7]++;
				//else if (ttime < 45) ttime_distribution[8]++;
				//else if (ttime < 50) ttime_distribution[9]++;
				//else if (ttime < 55) ttime_distribution[10]++;
				//else if (ttime < 60) ttime_distribution[11]++;
				//else if (ttime < 65) ttime_distribution[12]++;
				//else if (ttime < 70) ttime_distribution[13]++;
				//else if (ttime < 75) ttime_distribution[14]++;
				//else if (ttime < 80) ttime_distribution[15]++;
				//else if (ttime < 85) ttime_distribution[16]++;
				//else if (ttime < 90) ttime_distribution[17]++;
				//else if (ttime < 95) ttime_distribution[18]++;
				//else if (ttime < 100) ttime_distribution[19]++;
				//else if (ttime < 105) ttime_distribution[20]++;
				//else if (ttime < 110) ttime_distribution[21]++;
				//else if (ttime < 115) ttime_distribution[22]++;
				//else if (ttime < 120) ttime_distribution[23]++;
				//else  ttime_distribution[24]++;
				int index = ttime / 5;
				ttime_distribution[index]++;
				UNLOCK(_ttime_distribution_lock);
			}

			void read_snapshots()
			{
				cout << "reading snapshots" << endl;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Link_Components::Prototypes::Link<typename MasterType::link_type> _Link_Interface;
				fstream& input_network_snapshots_file = ((_Scenario_Interface*)_global_scenario)->template input_network_snapshots_file<fstream&>();
				if (!input_network_snapshots_file.is_open())
				{
					THROW_EXCEPTION(endl << "Cannot open snapshots file" << endl);
				}

				string line;
				int iline = 0;
				std::vector<string> tokens;
				int token_size;
				int link_uuid;
				float link_travel_time;
				int movement_uuid;
				float movement_forward_link_turn_travel_time;
				int num_movements;
				float maximum_free_flow_speed;
				Network_Components::Types::Link_ID_Dir link_id_dir;
				std::unordered_map<int,float> link_travel_time_map;
				std::unordered_map<int,float> movement_travel_time_map;
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
							
							//TODO:ROUTING_OPERATION
							//network_snapshot_replicas_container_type network_snapshot_replicas_container;
							//network_snapshot_replicas_container = construct_routables_from_snapshot(current_time, maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
							//_network_snapshots[current_time] = network_snapshot_replicas_container;
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
				//TODO:ROUTING_OPERATION
				//network_snapshot_replicas_container_type network_snapshot_replicas_container;
				//network_snapshot_replicas_container = construct_routables_from_snapshot(current_time, maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
				//_network_snapshots[current_time] = network_snapshot_replicas_container;
				_snapshot_times.push_back(current_time);
				//write_snapshot_from_snapshot_array();
			}

// TODO: This method is not called and does not compile
//			void write_snapshot_from_snapshot_array()
//			{
//				typedef Network<typename MasterType::routable_network_type> _Routable_Network_Interface;
//				typedef Network<typename MasterType::network_type> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(intersections_container)::value_type>::type>  _Routable_Intersection_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(intersections_container), _Routable_Intersection_Interface*> _Routable_Intersections_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Movement_Interface*> _Movements_Container_Interface;
//
//				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
//
//				for (int i = 0; i < (int)_snapshot_times.size(); i++)
//				{
//					int time = _snapshot_times[i];
//					((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>() << time << endl;
//					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)(((network_snapshot_replicas_container_type)(_network_snapshots.find(time)->second))[0]);
//					_Routable_Intersections_Container_Interface& routable_intersections_container = routable_network->template intersections_container<_Routable_Intersections_Container_Interface&>();
//					typename _Routable_Intersections_Container_Interface::iterator intersection_itr;
//					for (intersection_itr = routable_intersections_container.begin(); intersection_itr != routable_intersections_container.end(); intersection_itr++)
//					{
//						_Routable_Intersection_Interface* intersection = (_Routable_Intersection_Interface*)(*intersection_itr);
//						_Inbound_Outbound_Movements_Container_Interface& routable_inbound_outbound_movements_container = intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
//						typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
//						for (inbound_outbound_movements_itr = routable_inbound_outbound_movements_container.begin(); inbound_outbound_movements_itr != routable_inbound_outbound_movements_container.end(); inbound_outbound_movements_itr++)
//						{
//							_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
//							_Routable_Link_Interface* inbound_link = inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>();
//							_Movements_Container_Interface& outbound_movements = inbound_outbound_movements->template outbound_movements<_Movements_Container_Interface&>();
//							((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>()
//								<< inbound_link->template uuid<int>() << "\t"
//								<< inbound_link->template travel_time<float>() << "\t"
//								<< outbound_movements.size()
//								<< endl;
//
//							typename _Movements_Container_Interface::iterator movement_itr;
//							for (movement_itr = outbound_movements.begin(); movement_itr != outbound_movements.end(); movement_itr++)
//							{
//								_Movement_Interface* movement = (_Movement_Interface*)(*movement_itr);
//
//								((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>()
//									<< movement->template uuid<int>() << ","
//									<< movement->template forward_link_turn_travel_time<float>()
//									<< endl;
//							}
//						}
//					}
//				}
//				((_Scenario_Interface*)_global_scenario)->template output_network_snapshots_file<fstream&>().close();
//				exit(0);
//			}

			//TODO:ROUTING_OPERATION
//			network_snapshot_replicas_container_type construct_routables_from_snapshot(int time, float maximum_free_flow_speed, id_to_travel_time_map_type& link_travel_time_map, id_to_travel_time_map_type& movement_travel_time_map)
//			{
//				typedef  Network_Components::Prototypes::Network<typename remove_pointer<typename  type_of(routable_networks_container)::value_type>::type>  _Routable_Network_Interface;
//				typedef  Random_Access_Sequence< type_of(routable_networks_container), _Routable_Network_Interface*> _Routable_Networks_Container_Interface;
//
//				network_snapshot_replicas_container_type network_snapshot_replicas_container;
//				for(unsigned int i=0;i<num_sim_threads()+1;i++)
//				{
//					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename MasterType::routable_network_type>();
////TODO
////					routable_network->template read_snapshot_data<Target_Type<NULLTYPE,void,id_to_travel_time_map_type&>>(maximum_free_flow_speed, link_travel_time_map, movement_travel_time_map);
//					network_snapshot_replicas_container.push_back((typename MasterType::routable_network_type*)routable_network);
//				}
//				return network_snapshot_replicas_container;
//			}

			void initialize_moe()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;

				//network_moe_data.starting_time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>();
				//network_moe_data.ending_time = network_moe_data.starting_time + ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>();
	
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

				//TODO: Check - this has been moved to simulation intialization so it is not reset after every assignment interval, as they are cumulative measures
				/*network_moe_data.network_routed_ttime = 0.0f;
				network_moe_data.network_relative_gap = 0.0f;*/

				network_moe_data.num_arrived_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>();
				network_moe_data.num_departed_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>();
				network_moe_data.num_loaded_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>();


			}
			//agent mostly responsable fr output of results
			template<typename TargetType> void initialize_network_agent()
			{
				typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;
				////TODO
//load_event(ComponentType,ComponentType::template End_Iteration_Conditional,ComponentType::template End_Iteration_Handler, ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::NETWORK_SNAPSHOT_SUB_ITERATION,NULLTYPE);
                
				this->template Load_Event<ComponentType>(&ComponentType::End_Iteration_Conditional,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::NETWORK_SNAPSHOT_SUB_ITERATION);
				_start_cpu_time_in_seconds = (long)get_current_cpu_time_in_seconds();
 			}

			static void End_Iteration_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::NETWORK_SNAPSHOT_SUB_ITERATION)
				{
					//((typename MasterType::network_type*)_this)->Swap_Event((Event)&Snapshot_Subiteration_Handler<NULLTYPE>);
					//response.result=true;
					_this->Snapshot_Subiteration_Handler();
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_COMPUTATION_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_COMPUTATION_SUB_ITERATION)
				{
					//((typename MasterType::network_type*)_this)->Swap_Event((Event)&End_Iteration_Handler<NULLTYPE>);
					//response.result=true;
					_this->End_Iteration_Handler();
					response.next._iteration=iteration() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::NETWORK_SNAPSHOT_SUB_ITERATION;
				} 
				else
				{
					cout << "Should never reach here in network conditional!" << endl;
					assert(false);
				}
			}

			//declare_event(Snapshot_Subiteration_Handler)
			void Snapshot_Subiteration_Handler()
			{
				//typedef Network<typename MasterType::network_type> _Network_Interface;
				//typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;
				//if (((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>() % ((_Scenario_Interface*)_global_scenario)->template snapshot_period<int>() == 0)
				//{
				//	//((typename MasterType::network_type*)_this)->create_snapshot();
				//	if (((_Scenario_Interface*)_global_scenario)->template write_network_snapshots<bool>())
				//	{
				//		cout << "wrting network snapshot" << endl;
				//		((typename MasterType::network_type*)this)->output_snapshot();
				//	}
				//}
				//if (((_Scenario_Interface*)_global_scenario)->template compare_with_historic_moe<bool>() && ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>() % ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				//{
				//	((typename MasterType::network_type*)this)->read_historic_link_moe();
				//}
				//if (((_Scenario_Interface*)_global_scenario)->template read_normal_day_link_moe<bool>() && ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>() % ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				//{
				//	((typename MasterType::network_type*)this)->read_normal_day_link_moe();
				//}
				//if (((((_Network_Interface*)this)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				//{
				//	((typename MasterType::network_type*)this)->_in_network_vht_vehicle_based = 0.0;
				//}
				//((typename MasterType::network_type*)this)->_network_vht_compensation = 0;
			}

			void read_historic_link_moe()
			{
				// do nothing for non-visualized network implementaiton
			}

			void read_normal_day_link_moe()
			{
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				fstream& normal_day_link_moe_file = ((_Scenario_Interface*)_global_scenario)->template normal_day_link_moe_file<fstream&>();
				if (!normal_day_link_moe_file.is_open())
				{
					THROW_EXCEPTION(endl << "Link MOE reference file cannot be opened" << endl);
				}
				struct Link_Components::Implementations::Link_MOE_Data link_moe_data;
				std::vector<string> tokens;
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


			//declare_event(End_Iteration_Handler)
			void End_Iteration_Handler()
			{
				typedef Network<typename MasterType::network_type> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;
				_Network_Interface* _this_ptr = (_Network_Interface*)this;
				if (_this_ptr->template start_of_current_simulation_interval_absolute<int>() > _this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_end_time<int>())
				{
					_this_ptr->template scenario_reference<_Scenario_Interface*>()->template close_files<NULLTYPE>();
					//((typename MasterType::network_type*)_this_ptr)->template add_in_network_to_VHT<NT>();
					//cout << "final vht = " << ((typename MasterType::network_type*)_this_ptr)->_network_vht_in_network_based << endl;
					exit(0);
				}

				((typename MasterType::network_type*)this)->template calculate_moe<NULLTYPE>();
				//((typename MasterType::network_type*)this)->template update_vehicle_locations<NULLTYPE,NULLTYPE,NULLTYPE>();
				((typename MasterType::network_type*)this)->template printResults<NULLTYPE>();
			}

			//template<typename TargetType> void add_in_network_to_VHT()
			//{
			//	typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			//	typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			//	typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
			//	typedef  Random_Access_Sequence< typename _Intersection_Interface::get_type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;

			//	typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Outbound_Movement_Interface;
			//	typedef  Random_Access_Sequence< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Outbound_Movement_Interface*> _Outbound_Movements_Container_Interface;

			//	typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Outbound_Movement_Interface::get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
			//	typedef  Back_Insertion_Sequence< typename _Outbound_Movement_Interface::get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

			//	typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;

			//	typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			//	int current_time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_relative<int>();
			//	typename _Intersections_Container_Interface::iterator intersection_itr;
			//	for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
			//	{
			//		_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);
			//		_Inbound_Outbound_Movements_Container_Interface& _inbound_outbound_movements = intersection->inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
			//		typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
			//		for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
			//		{
			//			_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
			//			_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
			//			typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
			//			for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
			//			{
			//				//num_vehicles_in_link += int(((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>().size());
			//				typename _Vehicles_Container_Interface::iterator vehicle_itr;
			//				_Vehicles_Container_Interface& vehicles = ((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>();

			//				for (vehicle_itr=vehicles.begin();vehicle_itr!=vehicles.end();vehicle_itr++)
			//				{
			//					_Movement_Plan_Interface* mp = ((_Vehicle_Interface*)(*vehicle_itr))->template movement_plan<_Movement_Plan_Interface*>();
			//					float adjustment = float(current_time - mp->departed_time<Time_Seconds>()) / 3600.0f; 
			//					_network_vht_in_network_based += adjustment;
			//				}
			//			}
			//		}
			//	}
			//}


			template<typename TargetType> void update_vehicle_locations()
			{
				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
				typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

				typename _Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					((_Intersection_Interface*)(*intersection_itr))->template update_vehicle_locations<NULLTYPE>();
				}
			}

			template<typename TargetType> void calculate_moe()
			{
				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
				typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				typename _Intersections_Container_Interface::iterator intersection_itr;
				//for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				//{
				//	((_Intersection_Interface*)(*intersection_itr))->template calculate_moe_for_simulation_interval<NULLTYPE>();
				//}
				float vht_increase = float(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() * ((_Scenario_Interface*)_global_scenario)->template network_in_system_vehicles<int>()) / 3600.0f;
				vht_increase += (float)_network_vht_compensation / 3600.0f;
				_network_vht_in_network_based += vht_increase;
				if (((_Scenario_Interface*)_global_scenario)->template calculate_realtime_moe<bool>())
				{
					calculate_realtime_network_moe();
					output_moe_for_simulation_interval<  TargetType>();
				}

				if (((((_Network_Interface*)this)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					//for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
					//{
					//	((_Intersection_Interface*)(*intersection_itr))->template calculate_moe_for_assignment_interval<NULLTYPE>();
					//	((_Intersection_Interface*)(*intersection_itr))->template update_in_network_vehicle_vht<NT>();
					//}
					//_network_vht_vehicle_based = _in_network_vht_vehicle_based + _out_network_vht_vehicle_based;
					link_moe_post_process();
					update_moe_for_assignment_interval_with_links();
					update_moe_for_assignment_interval();
					output_moe_for_assignment_interval<  TargetType>();
					if (((_Scenario_Interface*)_global_scenario)->template write_ttime_distribution_from_network_model<bool>())
					{
						write_ttime_distribution<TargetType>();
					}
					reset_moe_for_assignment_interval();
				}
			}

			void link_moe_post_process()
			{
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
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
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

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
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

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

					_network_vmt += link_component->link_vmt<float>();
					_network_vht += link_component->link_vht<float>();
				}
			}

			void update_moe_for_assignment_interval()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

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
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Turn_Movement_Interface;
				typedef  Random_Access_Sequence< type_of(turn_movements_container), _Turn_Movement_Interface*> _Turn_Movements_Container_Interface;

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

			template<typename TargetType> void initialize_links()
			{
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = _links_container.begin(); links_itr != _links_container.end(); links_itr++)
				{
					((_Link_Interface*)(*links_itr))->template initialize_features<ComponentType*>(this_component());
				}
			}

			template<typename TargetType> void initialize_intersections()
			{
				//determine minimum merge rate
				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
				typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

				typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					((_Intersection_Interface*)(*intersection_itr))->template initialize_features<ComponentType*>(this_component());
				}
			}

			template<typename TargetType> void construct_network_cost()
			{
				// break up between links and movements
				//typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Link_Components::Prototypes::Link<typename MasterType::link_type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typename _Links_Container_Interface::iterator links_itr;
				
				_max_free_flow_speed = -1.0;			
				for (links_itr = _links_container.begin(); links_itr != _links_container.end(); links_itr++)
				{			
					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					Link_Components::Types::Link_Type_Keys facility_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

					if (facility_type == Link_Components::Types::Link_Type_Keys::WALK)
					{		
						float free_flow_speed = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkSpeed<float>();
						free_flow_speed = 3.28084 * free_flow_speed; // feet per second
						float link_travel_time = float(link->template length<float>() / free_flow_speed); // length was already converted to feet
						
						// Link length is defined in feet by "template<typename TargetType> void read_link_data(unique_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)"
						
						link_travel_time = max((float)1.0, link_travel_time);

						link->template travel_time<float>(link_travel_time);
					}
					else if (facility_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
					{
						//TODO: Omer fix later!
						float link_travel_time = 1;
						link->template travel_time<float>(link_travel_time);
					}
					else
					{ 
						float free_flow_speed = (float) ((5280.0) * link->template free_flow_speed<float>()/3600.0); // feet per second
						float link_travel_time = float (link->template length<float>() / free_flow_speed);
					
						_max_free_flow_speed = max(_max_free_flow_speed,free_flow_speed);

						link_travel_time = max((float)1.0,link_travel_time);

						link->template travel_time<float>(link_travel_time);
						//link->template num_lanes<float>(link_travel_time);
					}

				}
		
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Turn_Movement_Interface;
				typedef  Random_Access_Sequence< type_of(turn_movements_container), _Turn_Movement_Interface*> _Turn_Movements_Container_Interface;

				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = _turn_movements_container.begin(); turn_movements_itr != _turn_movements_container.end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* mvmt_itf=(_Turn_Movement_Interface*)(*turn_movements_itr);

					_Link_Interface* inboundLink = mvmt_itf->template inbound_link<_Link_Interface*>();
					_Link_Interface* outboundLink = mvmt_itf->template outbound_link<_Link_Interface*>();

					float turn_travel_penalty = 0.0f;
					//the logic of PROHIBITED is never used in the simulation
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

			void construct_routable_networks()
			{
				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;
				/*typedef  Link_Components::Prototypes::Link<typename MasterType::link_type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typename _Links_Container_Interface::iterator links_itr;*/
				bool multimodal_dijkstra = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::multimodal_dijkstra<bool>();

				if(((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				{
					MasterType::routable_network_type::initialize_moe_data();
				}

				Routable_Network<typename MasterType::routable_network_type>* routable_network = (Routable_Network<typename MasterType::routable_network_type>*)Allocate<typename MasterType::routable_network_type>();
				_routable_networks.push_back(routable_network);
				
				routable_network->initialize();

				routable_network->template construct_routable_network<typename MasterType::network_type>( (Network<typename MasterType::network_type>*)this );

				if (((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>())
				{
					routable_network->template construct_multimodal_routable_network<typename MasterType::network_type>((Network<typename MasterType::network_type>*)this);
				}
				

				if(((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				{
					routable_network->template construct_time_dependent_routable_network<typename MasterType::network_type>( (Network<typename MasterType::network_type>*)this );
				}

				routable_network->finalize();

				if (((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>() && multimodal_dijkstra )
				{					
					routable_network->compute_walk_distance_to_transit<typename MasterType::network_type>((Network<typename MasterType::network_type>*)this); 

					routable_network->compute_drive_fft_to_transit<typename MasterType::network_type>((Network<typename MasterType::network_type>*)this);
					
					routable_network->schedule_dijkstra_network_tree<typename MasterType::network_type>((Network<typename MasterType::network_type>*)this);
				}

				for(uint i=1;i<num_sim_threads();i++)
				{
					routable_network = routable_network->create_copy();
					_routable_networks.push_back( routable_network );
				}

				//
				//if(!((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				//{
				//	time_dependent_routable_network = (Routable_Network<typename MasterType::time_dependent_routable_network_type>*)Allocate<MasterType::time_dependent_routable_network_type>();

				//	time_dependent_routable_network->construct_time_dependent_routable_network<typename MasterType::network_type>( (Network<typename MasterType::network_type>*)this );

				//	for(uint i=1;i<num_sim_threads();i++)
				//	{
				//		time_dependent_routable_network = time_dependent_routable_network->create_copy();
				//		_time_dependent_routable_networks.push_back( time_dependent_routable_network );
				//	}
				//}

			}

			template<typename TargetType> void construct_realtime_routable_network()
			{
				//TODO:ROUTING_OPERATION
				//typedef Network<typename MasterType::network_type> _Regular_Network_Interface;
				//typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::Regular_Network,_Regular_Network_Interface*> Net_IO_Type;
				//typedef  Network_Components::Prototypes::Network<typename remove_pointer<typename  type_of(routable_networks_container)::value_type>::type>  _Routable_Network_Interface;
				//typedef  Random_Access_Sequence< type_of(routable_networks_container), _Routable_Network_Interface*> _Routable_Networks_Container_Interface;

				//for(unsigned int i=0;i<num_sim_threads()+1;i++)
				//{
				//	_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename MasterType::routable_network_type>();
				//	routable_network->template read_realtime_network_data<Net_IO_Type>((_Regular_Network_Interface*)this);
				//	_realtime_routable_networks_container.push_back((typename MasterType::routable_network_type*)routable_network);
				//}
			}

			//==================================================================================================================
			/// Write out intermediate simulation results
			//------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> void printResults() 
			{
						
				typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network<typename MasterType::network_type> _Network_Interface;
				_Scenario_Interface* scenario = scenario_reference<_Scenario_Interface*>();
				_Network_Interface* _this_ptr = (_Network_Interface*)this;

				int iter = _this_ptr->template start_of_current_simulation_interval_absolute<int>();
				//if (iter % 60 == 0)
				//{
					printf("%s, ", convert_seconds_to_hhmmss(_this_ptr->template start_of_current_simulation_interval_absolute<int>()).c_str());
					printf("departed=%7d, arrived=%7d, in_network=%7d, VMT=%7.2f, VHT=%7.2f\n",scenario->template network_cumulative_departed_vehicles<int>(),scenario->template network_cumulative_arrived_vehicles<int>(),scenario->template network_in_network_vehicles<int>(),_network_vmt, _network_vht);
				//}
				if (((_Scenario_Interface*)_global_scenario)->template write_node_control_state<bool>())
				{
					write_node_control_state<TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_vehicle_trajectory<bool>())
				{
					write_vehicle_trajectory<TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_network_link_flow<bool>())
				{
					write_network_link_flow<TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_network_link_turn_time<bool>())
				{
					write_network_link_turn_time<TargetType>();
				}
				if (((_Scenario_Interface*)_global_scenario)->template write_output_summary<bool>())
				{
					write_output_summary<TargetType>();
				}
			}

			template<typename TargetType> void increase_in_network_vht_vehicle_based(float increase)
			{
				LOCK(_network_vht_vehicle_based_update_lock);
				_in_network_vht_vehicle_based += increase;
				UNLOCK(_network_vht_vehicle_based_update_lock);
			}

			template<typename TargetType> void increase_out_network_vht_vehicle_based(float increase)
			{
				LOCK(_network_vht_vehicle_based_update_lock);
				_out_network_vht_vehicle_based += increase;
				UNLOCK(_network_vht_vehicle_based_update_lock);
			}

			template<typename TargetType> void update_network_vht_compensation(int adjustment)
			{
				LOCK(_network_vht_compensation_update_lock);
				_network_vht_compensation += adjustment;
				UNLOCK(_network_vht_compensation_update_lock);
			}
			
			template<typename TargetType> void write_node_control_state();

			template<typename TargetType> void write_vehicle_trajectory();

			template<typename TargetType> void write_network_link_flow();

			template<typename TargetType> void write_network_link_turn_time();
			
			template<typename TargetType> void write_output_summary();

			template<typename TargetType> void read_analyze_link_groups();

			//==================================================================================================================
			/// read from database
			//------------------------------------------------------------------------------------------------------------------		
			template<typename TargetType> void read_network_data(Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				_db_reader = Allocate<typename remove_pointer<type_of(db_reader)>::type>();
				typedef Prototypes::Network_DB_Reader<typename remove_pointer<type_of(db_reader)>::type> _DB_Interface;
				_DB_Interface* db = (_DB_Interface*)_db_reader;
				db->template network_reference<ComponentType*>((ComponentType*)this);

				db->template read_network_data<Network_Components::Types::Network_IO_Maps&>(net_io_maps);
			}

			//==================================================================================================================
			/// output moe
			//------------------------------------------------------------------------------------------------------------------			
			template<typename TargetType> void output_moe_for_simulation_interval();
			template<typename TargetType> void output_moe_for_assignment_interval();
			template<typename TargetType> void write_ttime_distribution();
			//==================================================================================================================
			/// network events
			//------------------------------------------------------------------------------------------------------------------

			m_prototype(Null_Prototype, typename MasterType::network_event_manager_type, network_event_manager, NONE, NONE);

			//==================================================================================================================
			/// traffic management center
			//------------------------------------------------------------------------------------------------------------------
			m_prototype(Null_Prototype, typename MasterType::traffic_management_center_type, traffic_management_center, NONE, NONE);
		};

		implementation struct Integrated_Network_Implementation : public Network_Implementation<MasterType,INHERIT(Integrated_Network_Implementation)>
		{
			typedef typename Network_Implementation<MasterType,INHERIT(Integrated_Network_Implementation)>::Component_Type ComponentType;

			m_prototype(Null_Prototype,typename MasterType::network_skim_type, skimming_faculty, NONE, NONE);

// TODO: this does not compile
//			template<typename TargetType> typename TargetType::ReturnType Get_TTime(typename TargetType::ParamType Origin, typename TargetType::ParamType Destination, typename TargetType::Param2Type Mode_Indicator)
//			{
//				Network_Implementation<MasterType>* base_this = (Network_Implementation<MasterType>*)this;
//
//				typedef (skim_faculty_itf,type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming);
//
//				skim_faculty_itf* skim_faculty = this->skimming_faculty<skim_faculty_itf*>();
//
//				TargetType::ReturnType ret_value = skim_faculty->template Get_TTime<TargetType>(Origin,Destination,Mode_Indicator);
//
//				return ret_value;
//			}
		
		};



	}
}

using namespace Network_Components::Implementations;
