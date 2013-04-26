#pragma once
#include "Vehicle_Prototype.h"

namespace Vehicle_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Polaris_Switch_Decision_Data_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Switch_Decision_Data_Implementation),MasterType,Data_Object,ParentType>
		{
			member_data(int, switch_decision_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_container(vector<typename MasterType::link_type*>, route_links_container, none, none);
		};

		implementation struct Polaris_Vehicle_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Vehicle_Implementation),MasterType,Execution_Object,ParentType>
		{
			member_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, none, none);
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			
			member_component(typename MasterType::movement_plan_type, movement_plan, none, none);
#ifndef EXCLUDE_DEMAND
			member_component(typename MasterType::person_type, traveler, none, none);
#else
			member_component(typename MasterType::traveler_type, traveler, none, none);
#endif
			member_data(float, distance_to_stop_bar, none, none);
			member_data(float, local_speed, none, none);

			member_container(vector<typename MasterType::switch_decision_data_type*>, switch_decisions_container, none, none);
			member_data(RNG_Components::RngStream, rng_stream, none, none);
			member_data(Vehicle_Components::Types::Enroute_Information_Keys, enroute_information_type, none, none);
			member_data(double, information_compliance_rate, none, none);
			member_data(double, relative_indifference_bound_route_choice, none, none);
			member_data(double, minimum_travel_time_saving, none, none);

			feature_implementation void load(requires(check_2(TargetType,typename Types::Load_To_Entry_Queue,is_same)))
			{
				_simulation_status = Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE;
			}

			feature_implementation void load(requires(check_2(TargetType,typename Types::Load_To_Origin_Link,is_same)))
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type,ComponentType> _Movement_Plan_Interface;
				_simulation_status =  Types::Vehicle_Status_Keys::IN_NETWORK;
				((_Movement_Plan_Interface*)_movement_plan)->template initialize_trajectory<NULLTYPE>();
				((typename MasterType::vehicle_type*)this)->template load_register<ComponentType,CallerType,TargetType>();
			}

			feature_implementation void load(requires(!check_2(TargetType,typename Types::Load_To_Origin_Link,is_same) && !check_2(TargetType,typename Types::Load_To_Entry_Queue,is_same)))
			{
                assert_check_2(TargetType,typename Vehicle_Components::Types::Load_To_Origin_Link,is_same,"no match");
                assert_check_2(TargetType,typename Vehicle_Components::Types::Load_To_Entry_Queue,is_same,"no match");
			}

			feature_implementation void unload()
			{
				_simulation_status = Types::Vehicle_Status_Keys::OUT_NETWORK;
			}

			feature_implementation void load_register()
			{
				// do nothing for polaris vehicle
			}

			feature_implementation bool exploit_events_set(TargetType events_set)
			{
				define_component_interface(_Movement_Plan_Interface, type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, _Movement_Plan_Interface::get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				typedef Network_Event<typename MasterType::base_network_event_type> _Base_Event_Interface;
				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();

				_Trajectory_Container_Interface::iterator itr;

				bool event_found_flag = false;
				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
					
					///weather
					_Base_Event_Interface* weather_event = route_link->current_weather_event<_Base_Event_Interface*>();
					if (weather_event != nullptr)
					{
						if (events_set.find(weather_event) != events_set.end())
						{
							event_found_flag = true;
							break;
						}
					}

					///accident
					_Base_Event_Interface* accident_event = route_link->current_accident_event<_Base_Event_Interface*>();
					if (accident_event != nullptr)
					{
						if (events_set.find(accident_event) != events_set.end())
						{
							event_found_flag = true;
							break;
						}
					}

				}

				return event_found_flag;
			}

			feature_implementation void update_enroute_switch_decisions()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				define_component_interface(_Movement_Plan_Interface, type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, _Movement_Plan_Interface::get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Switch_Decision_Data_Container_Interface, _Switch_Decision_Data_Interface, typename type_of(switch_decisions_container), Random_Access_Sequence_Prototype, Switch_Decision_Data_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Switch_Decision_Data_Interface::get_type_of(route_links_container), Random_Access_Sequence_Prototype, Link_Prototype, ComponentType);

				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();

				_Switch_Decision_Data_Interface* switch_decision_data = (_Switch_Decision_Data_Interface*)Allocate<typename MasterType::switch_decision_data_type>();
				
				switch_decision_data->template switch_decision_index<int>(int(_switch_decisions_container.size()));

				_Trajectory_Container_Interface::iterator itr;

				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
					_Links_Container_Interface& links_container = switch_decision_data->template route_links_container<_Links_Container_Interface&>();
					links_container.push_back(route_link);
				}
				switch_decisions_container<ComponentType,CallerType,_Switch_Decision_Data_Container_Interface&>().push_back(switch_decision_data);

				((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_switched_decisions<NULLTYPE>();
			}

			feature_implementation void enroute_switching()
			{
				define_component_interface(_Traveler_Interface, type_of(traveler), Traveler_Components::Prototypes::Traveler_Prototype, ComponentType);
				define_component_interface(_Routing_Interface, _Traveler_Interface::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);

				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::routable_network_type> _Routable_Network_Interface;
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, _Routable_Network_Interface::get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);

				_Routing_Interface* router = traveler<ComponentType,CallerType,_Traveler_Interface*>()->template router<_Routing_Interface*>();

				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, typename _Regular_Link_Interface::get_type_of(realtime_replicas_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface2, typename MasterType::routable_network_type::get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, _Movement_Plan_Interface::get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Regular_Network_Interface;
				define_container_and_value_interface(_Regular_Movements_Container_Interface, _Regular_Movement_Interface, typename _Regular_Network_Interface::get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Movements_Container_Interface, _Routable_Movement_Interface, typename _Regular_Movement_Interface::get_type_of(realtime_replicas_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Links_Container_Interface, _Regular_Link_Interface3, typename _Regular_Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				
				_Regular_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Regular_Link_Interface*>();
				_Regular_Link_Interface* destination_link = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Regular_Link_Interface*>();
				
				///calculate travel time of current route
				float current_route_time_to_destination = 0.0f; 
				int current_route_link_sum_to_destination = 0;

				bool use_realtime_travel_time = true;

				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
				_Trajectory_Container_Interface::iterator itr;

				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Regular_Link_Interface* route_link = trajectory_unit->template link<_Regular_Link_Interface*>();
					_Routable_Link_Interface* routable_link;
					if (use_realtime_travel_time)
					{
						routable_link = (_Routable_Link_Interface*)(route_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[_thread_id]); 
					}
					else
					{
						routable_link = (_Routable_Link_Interface*)(route_link->template replicas_container<_Routable_Links_Container_Interface&>()[_thread_id]); 
					}
					float link_travel_time = routable_link->template travel_time<float>();

					current_route_link_sum_to_destination += route_link-> template internal_id<int>();
					current_route_time_to_destination += link_travel_time;
					if (itr < trajectory.end() - 1)
					{
						_Trajectory_Unit_Interface* next_trajectory_unit = (_Trajectory_Unit_Interface*)(*(itr+1));
						_Regular_Link_Interface* next_route_link = next_trajectory_unit->template link<_Regular_Link_Interface*>();

						_Routable_Link_Interface* next_routable_link;
						if (use_realtime_travel_time)
						{
							next_routable_link = (_Routable_Link_Interface*)(next_route_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[_thread_id]); 
						}
						else
						{
							next_routable_link = (_Routable_Link_Interface*)(next_route_link->template replicas_container<_Routable_Links_Container_Interface&>()[_thread_id]); 
						}

						int inbound_link_id = route_link->template internal_id<int>();
						int outbound_link_id = next_route_link->template internal_id<int>();
						typename MasterType::network_type::long_hash_key_type long_hash_key;
						long_hash_key.inbound_link_id = inbound_link_id;
						long_hash_key.outbound_link_id = outbound_link_id;
						typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();
						_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];
						_Routable_Movement_Interface* routable_movement;
						if (use_realtime_travel_time)
						{
							routable_movement = (_Routable_Movement_Interface*)regular_movement->template realtime_replicas_container<_Routable_Movements_Container_Interface&>()[_thread_id]; 
						}
						else
						{
							routable_movement = (_Routable_Movement_Interface*)regular_movement->template replicas_container<_Routable_Movements_Container_Interface&>()[_thread_id]; 
						}
						float turn_penalty = routable_movement->template forward_link_turn_travel_time<float>();
						current_route_time_to_destination += turn_penalty;
					}
				}

				///calcualte travel time of the best route
				router->template routable_origin<_Regular_Link_Interface*>(origin_link);
				router->template routable_destination<_Regular_Link_Interface*>(destination_link);

				_Routable_Network_Interface* routable_network_ptr;
				if (use_realtime_travel_time)
				{
					routable_network_ptr = router->template realtime_routable_network<_Routable_Network_Interface*>();
				}
				else
				{
					routable_network_ptr = router->template routable_network<_Routable_Network_Interface*>();
				}

				float routed_travel_time = router->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr);
				
				///find a new route using shortest path algorithm
				if (routed_travel_time >= 0.0)
				{	
					if (routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() == 0)
					{
						THROW_WARNING(endl << "Error: path size is: " << routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() << endl);
						THROW_EXCEPTION(endl << "no path between origin link uuid " << origin_link->uuid<int>() << " and destination link uuid " << destination_link->uuid<int>());
					}
					
					int best_route_link_sum = 0;
					_Reversed_Path_Container_Interface::iterator itr;
					for(itr = routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin(); itr != routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().end(); itr++)
					{
						_Regular_Link_Interface* link = (_Regular_Link_Interface*)(*itr);
						best_route_link_sum += link->template internal_id<int>();
					}
					_Regular_Link_Interface* regular_destination_link = (_Regular_Link_Interface*)(*(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin()));
					_Routable_Link_Interface* routable_destination_link;
					if (use_realtime_travel_time)
					{
						routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[_thread_id]);
					}
					else
					{
						routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template replicas_container<_Routable_Links_Container_Interface&>()[_thread_id]);
					}
					float best_route_time_to_destination = routable_destination_link->template label_cost<float>();

					if (best_route_link_sum != current_route_link_sum_to_destination)
					{
						if (best_route_time_to_destination < current_route_time_to_destination*(1.0 - _relative_indifference_bound_route_choice) &&
							best_route_time_to_destination < (current_route_time_to_destination - _minimum_travel_time_saving))
						{
							update_enroute_switch_decisions<ComponentType, CallerType, TargetType>();
							((_Movement_Plan_Interface*)_movement_plan)->template update_trajectory<_Reversed_Path_Container_Interface>(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>());
							///en-route switching
							cout<< "enroute switching..." <<endl;
							int current_time = ((_Regular_Network_Interface*)_global_network)->start_of_current_simulation_interval_absolute<int>();
							((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>(current_time + best_route_time_to_destination);
							int routed_travel_time = ((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>();
							((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>(routed_travel_time - current_route_time_to_destination + best_route_time_to_destination);
						}
					}
				}
			}

			feature_implementation void initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				///
				unsigned long seed = ((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>()+_internal_id+1;
				_rng_stream.SetSeed(seed);
				double r1;
				///information capability
				r1 = _rng_stream.RandU01();
				if (r1 <= ((_Scenario_Interface*)_global_scenario)->template realtime_informed_vehicle_market_share<double>())
				{
					_enroute_information_type = Vehicle_Components::Types::Enroute_Information_Keys::WITH_REALTIME_INFORMATION;
				}
				else
				{
					_enroute_information_type = Vehicle_Components::Types::Enroute_Information_Keys::NO_REALTIME_INFORMATION;
				}

				/// information compliance rate
				r1 = _rng_stream.RandU01();
				_information_compliance_rate = r1;

				///rib
				r1 = _rng_stream.RandU01();
				double mean = ((_Scenario_Interface*)_global_scenario)->template relative_indifference_bound_route_choice_mean<double>();
				double a = 0.0;
				double b = 2.0*mean;
				double rib = _rng_stream.triangular_random_variate(r1,a,b,mean);
				_relative_indifference_bound_route_choice = rib;

				///mtts
				r1 = _rng_stream.RandU01();
				mean = ((_Scenario_Interface*)_global_scenario)->template minimum_travel_time_saving_mean<double>();
				a = 0.5 * mean;
				b = a + 2.0*mean;
				double mtts = _rng_stream.triangular_random_variate(r1,a,b,mean);
				_minimum_travel_time_saving = mtts;
				///
			}

		};
	}

}