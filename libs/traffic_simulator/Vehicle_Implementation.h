#pragma once
#include "Vehicle_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "../Repository/RNG_Implementations.h"

namespace Vehicle_Components
{
	namespace Types
	{
		enum Type_Vehicle_Action_keys
		{
			MOVE_TO_NEXT_LINK = 0,
			MOVE_TO_ORIGIN_LINK,
			DO_NOTHING,
		};
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Switch_Decision_Data_Implementation:public Polaris_Component<MasterType,INHERIT(Switch_Decision_Data_Implementation),Data_Object>
		{
			m_data(int, switch_decision_index, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_container(boost::container::vector<typename MasterType::link_type*>, route_links_container, NONE, NONE);
		};

		implementation struct Vehicle_Implementation:public Polaris_Component<MasterType,INHERIT(Vehicle_Implementation),Execution_Object>
		{

			m_data(bool, is_integrated, NONE, NONE);
			m_data(bool, its_switch, NONE, NONE);
			m_data(Vehicle_Components::Types::Type_Vehicle_Action_keys, suggested_action, NONE, NONE);

			m_data(bool, write_trajectory,NONE,NONE);

			typedef typename Polaris_Component<MasterType,INHERIT(Vehicle_Implementation),Execution_Object>::ComponentType ComponentType;
			
			m_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			m_data(int, internal_id, NONE, NONE);
			m_data(float, distance_to_stop_bar, NONE, NONE);

			m_prototype(Movement_Plan_Components::Prototypes::Movement_Plan,typename MasterType::movement_plan_type, movement_plan, NONE, NONE);
#ifndef EXCLUDE_DEMAND
			m_prototype(Null_Prototype,typename MasterType::person_type, traveler, NONE, NONE);
#else
			m_prototype(Null_Prototype,typename MasterType::traveler_type, traveler, NONE, NONE);
#endif
			m_prototype(Routing_Components::Prototypes::Routing, typename MasterType::routing_type, router, NONE, NONE);

			m_data(float, local_speed, NONE, NONE);
			m_data(int, downstream_preferred_departure_time, NONE, NONE);

			m_container(boost::container::vector<typename MasterType::switch_decision_data_type*>, switch_decisions_container, NONE, NONE);
			
			//m_data(RNG_Components::RngStream, rng_stream, NONE, NONE);
			m_data(Vehicle_Components::Types::Enroute_Information_Keys, enroute_information_type, NONE, NONE);
			m_data(float, information_compliance_rate, NONE, NONE);
			
			m_data(float, relative_indifference_band_route_choice, NONE, NONE);
			m_data(float, minimum_travel_time_saving, NONE, NONE);
			
			//m_data(bool, enroute_updated, NONE, NONE);
			m_data(int, last_enroute_switching_route_check_time, NONE, NONE);
			m_data(int, entry_queue_length, NONE, NONE);


			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;
			typedef  Switch_Decision_Data<typename remove_pointer<typename  type_of(switch_decisions_container)::value_type>::type>  _Switch_Decision_Data_Interface;
			typedef  Random_Access_Sequence<typename type_of(switch_decisions_container), _Switch_Decision_Data_Interface*> _Switch_Decision_Data_Container_Interface;

			typedef  Link<typename remove_pointer< typename _Switch_Decision_Data_Interface::get_type_of(route_links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< typename _Switch_Decision_Data_Interface::get_type_of(route_links_container), _Link_Interface*> _Links_Container_Interface;

			typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Link_Interface::get_type_of(link_origin_vehicle_queue)::value_type>::type>  _Vehicle_Interface;
			typedef  Back_Insertion_Sequence< typename _Link_Interface::get_type_of(link_origin_vehicle_queue), _Vehicle_Interface*> _Vehicle_Origin_Queue_Interface;

			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef  Trajectory_Unit<typename remove_pointer< typename _Movement_Plan_Interface::get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename _Movement_Plan_Interface::get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type>  _Movement_Interface;
			typedef  Random_Access_Sequence< typename _Link_Interface::get_type_of(outbound_turn_movements), _Movement_Interface*> _Movements_Container_Interface;

			typedef Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type> _Network_Event_Interface;
			typedef  Traveler_Components::Prototypes::Traveler<typename type_of(traveler)> _Traveler_Interface;
			
			//TODO:ROUTING
			//typedef Network_Components::Prototypes::Network<typename MasterType::routable_network_type> _Routable_Network_Interface;
			//typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(reversed_path_container)::value_type>::type>  _Regular_Link_Interface;
			//typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(reversed_path_container), _Regular_Link_Interface*> _Reversed_Path_Container_Interface;
			//typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Regular_Link_Interface::get_type_of(realtime_replicas_container)::value_type>::type>  _Routable_Link_Interface;
			//typedef  Random_Access_Sequence< typename _Regular_Link_Interface::get_type_of(realtime_replicas_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
			//typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Movement_Interface::get_type_of(realtime_replicas_container)::value_type>::type>  _Routable_Movement_Interface;
			//typedef  Random_Access_Sequence< typename _Regular_Movement_Interface::get_type_of(realtime_replicas_container), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Regular_Network_Interface;
			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Network_Interface::get_type_of(turn_movements_container)::value_type>::type>  _Regular_Movement_Interface;
			typedef  Random_Access_Sequence< typename _Regular_Network_Interface::get_type_of(turn_movements_container), _Regular_Movement_Interface*> _Regular_Movements_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Regular_Network_Interface::get_type_of(links_container)::value_type>::type>  _Regular_Link_Interface3;
			typedef  Random_Access_Sequence< typename _Regular_Network_Interface::get_type_of(links_container), _Regular_Link_Interface3*> _Regular_Links_Container_Interface;

			typedef Network_Event<typename MasterType::base_network_event_type> _Base_Event_Interface;

			template<typename TargetType> void load(requires(TargetType,check_2(TargetType,typename Types::Load_To_Entry_Queue,is_same)))
			{
				_simulation_status = Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE;
				_Link_Interface* origin_link=movement_plan<_Movement_Plan_Interface*>()->template origin<_Link_Interface*>();
				_entry_queue_length = (int)origin_link->template link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().size();
				//update_network_vht_compensation<TargetType>();
				if (((_Scenario_Interface*)_global_scenario)->template vehicle_taking_action<bool>())
				{
					start_agent<TargetType>();
				}
				//TODO: Remove this code abomination
#ifdef ANTARES
				else
				{
					Load_Event<ComponentType>(&ComponentType::Vehicle_Action_Condition,iteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION);
					//TODO
					//load_event(ComponentType,ComponentType::template Vehicle_Action_Condition,ComponentType::template compute_vehicle_position,iteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION,NULLTYPE);
					((ComponentType*)this)->route_being_displayed = false;
				}
#endif
				_its_switch = false;
			}

			template<typename TargetType> void update_network_vht_compensation()
			{
				int current_time;
				if (iteration() < ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() - 1)
				{
					current_time = 0;
				}
				else
				{
					current_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>();
				}

				((_Scenario_Interface*)_global_scenario)->template increase_network_in_system_vehicles<NULLTYPE>();
				_Movement_Plan_Interface* mp = (_Movement_Plan_Interface*)_movement_plan;
				mp->template entry_time<int>(current_time);
				int adjustment = current_time - mp->template departed_time<Time_Seconds>();
				((_Network_Interface*)_global_network)->template update_network_vht_compensation<NT>(adjustment);
			}

			template<typename TargetType> void load(requires(TargetType,!check_2(TargetType,typename Types::Load_To_Entry_Queue,is_same)))
			{
                assert_check_2(TargetType,typename Vehicle_Components::Types::Load_To_Entry_Queue,is_same,"no match");
			}

			template<typename TargetType> void take_action()
			{
				switch(_suggested_action)
				{
				case Vehicle_Components::Types::MOVE_TO_NEXT_LINK:
					move_to_next_link<TargetType>();
					_suggested_action = Vehicle_Components::Types::DO_NOTHING;
					break;
				case Vehicle_Components::Types::MOVE_TO_ORIGIN_LINK:
					move_to_origin_link<TargetType>();
					_suggested_action = Vehicle_Components::Types::DO_NOTHING;
					break;
				case Vehicle_Components::Types::DO_NOTHING:
					// do nothing
					break;
				default:
					cout << "unrecognized vehicle action" << endl;
					assert(false);
				}
			}

			template<typename TargetType> void move_to_origin_link()
			{
				_Movement_Plan_Interface* mp = (_Movement_Plan_Interface*)_movement_plan;
				mp->template initialize_trajectory<NULLTYPE>();
				_Link_Interface* link = mp->template origin<_Link_Interface*>();
				move_to_link<_Link_Interface*>((_Link_Interface*)link);
				_simulation_status = Types::Vehicle_Status_Keys::IN_NETWORK;

				update_network_vht_compensation<TargetType>();
			}

			template<typename TargetType> void move_to_next_link()
			{
				_Movement_Plan_Interface* mp = (_Movement_Plan_Interface*)_movement_plan;
				_Link_Interface* link = mp->template next_link<_Link_Interface*>();

				if (link == nullptr)
				{
					cout << "invalid next link in move_to_next_link of vehicle implementation" << endl;
					assert(false);
				}
				move_to_link<_Link_Interface*>((_Link_Interface*)link);
			}

			template<typename TargetType> void move_to_link(TargetType link)
			{


				_distance_to_stop_bar = ((_Link_Interface*)link)->template length<float>();

				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				_Movement_Plan_Interface* mp = (_Movement_Plan_Interface*)_movement_plan;
				float a_delayed_time;

				if (mp->template current_trajectory_position<int>() == -1)
				{
					a_delayed_time = 0.0;
				}
				else
				{
					a_delayed_time = max(0.0f, float((int)((((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() - mp->template get_current_link_enter_time<int>()) - ((_Link_Interface*)link)->template link_fftt<float>())));
				}

				if (mp->template trajectory_size<int>() == 0)
				{
					THROW_EXCEPTION("Error, empty trajectory for vehicle " << _uuid);
				}
				mp->template transfer_to_next_link<NULLTYPE>(a_delayed_time);


				//GLOBALS::Simulation_Time.Current_Time<Time_Seconds>()

				///enroute switching
				//if (((_Scenario_Interface*)_global_scenario)->template use_realtime_travel_time_for_enroute_switching<bool>())
				//{
				//	_enroute_updated = false;
				//	update_eta<NULLTYPE>();
				//}
				//else
				//{
				//	if (((((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				//	{
				//		_enroute_updated = false;
				//		update_eta<NULLTYPE>();
				//	}
				//}

				if (((_Scenario_Interface*)_global_scenario)->template enroute_switching_enabled<bool>())
				{
					check_enroute_switching<TargetType>((_Link_Interface*)link);
				}

				if(((_Link_Interface*)link)->template internal_id<int>() == (mp->template destination<_Link_Interface*>())->template internal_id<int>())
				{
					((_Vehicle_Interface*)this)->template unload<NULLTYPE>();
				}
				((_Link_Interface*)link)->template push_vehicle_from_network<_Vehicle_Interface*>((_Vehicle_Interface*)this);
			}

			template<typename TargetType> void check_enroute_switching(_Link_Interface* link)
			{
				if (int(((_Link_Interface*)link)->template outbound_turn_movements<_Movements_Container_Interface&>().size()) <= 1)
				{
					return;
				}
				int last_enroute_switching_route_check_time_assignment_index = _last_enroute_switching_route_check_time / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				int current_assignment_index = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>() / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				// check enroute switching only if the current assignment index and the assignment index of the last enroute switching check are different which means that the routable network has been updated
				if (!(current_assignment_index > last_enroute_switching_route_check_time_assignment_index))
				{
					return;
				}
				
				bool enroute_switching_decision = false;
				int cause_for_switching = -1;
				_Movement_Plan_Interface* mp = (_Movement_Plan_Interface*)_movement_plan;
				
				

				if (((_Scenario_Interface*)_global_scenario)->template enroute_switching_on_excessive_delay<bool>())
				{
					float next_link_time = 0.0f;
					float next_routed_time = 0.0f;

					int current_travel_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() - mp->template departed_time<Time_Seconds>();
					int position_index = mp->template current_trajectory_position<int>();
					_Trajectory_Container_Interface& traj_container = mp->template trajectory_container<_Trajectory_Container_Interface&>();
					_Trajectory_Unit_Interface* current_traj_unit = traj_container[position_index];
					int routed_travel_time = current_traj_unit->template estimated_link_accepting_time<int>();
									
					// check to make sure that next link in the trajectory is not jammed
					//_Link_Interface* next_link = mp->next_link<_Link_Interface*>();
					if (position_index+2<traj_container.size())
					{
						_Trajectory_Unit_Interface* next_traj_unit = traj_container[position_index+2];
						_Link_Interface* next_link = next_traj_unit->link<_Link_Interface*>();
						int next_routed_travel_time = next_traj_unit->template estimated_link_accepting_time<int>();
						
						int diff = next_routed_travel_time - routed_travel_time;
						routed_travel_time = next_routed_travel_time;

						current_travel_time += next_link_travel_time<float>();
						//if (current_travel_time >= 36000) cout <<"Next link id: " << next_link->template uuid<int>()<<". Next link travel time: " << next_link_travel_time<float>()<<", routed time = "<<diff<<", delay ratio: " << (float)current_travel_time / (float)routed_travel_time<<endl;
					}

					float observed_delay_ratio = routed_travel_time > 0 ? (float)current_travel_time / (float)routed_travel_time : 0;
					

					/*
					{
						_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
						typename _Trajectory_Container_Interface::iterator itr;

						///calculate travel time of current route

						for (itr = (trajectory.begin() + (((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()+1)); itr < trajectory.end(); itr++)
						{
							// this is next
							_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
							_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();

							if (itr < trajectory.end() - 1)
							{
								_Trajectory_Unit_Interface* next_trajectory_unit = (_Trajectory_Unit_Interface*)(*(itr+1));
								_Link_Interface* next_route_link = next_trajectory_unit->template link<_Link_Interface*>();

								int inbound_link_id = route_link->template uuid<int>();
								int outbound_link_id = next_route_link->template uuid<int>();

								typename MasterType::network_type::long_hash_key_type long_hash_key;
								long_hash_key.inbound_link_id = inbound_link_id;
								long_hash_key.outbound_link_id = outbound_link_id;
								typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->template link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();
								_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];

								float link_turn_travel_time = regular_movement->template forward_link_turn_travel_time<float>();
								
								next_link_time += link_turn_travel_time;

								next_routed_time = trajectory_unit->template estimated_link_accepting_time<int>();
							}

							break;
						}
					}

					// check one link ahead
					int current_travel_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() - mp->template departed_time<Time_Seconds>() + next_link_time;
					int position_index = mp->template current_trajectory_position<int>();
					_Trajectory_Container_Interface& traj_container = mp->template trajectory_container<_Trajectory_Container_Interface&>();
					_Trajectory_Unit_Interface* current_traj_unit = traj_container[position_index];
					
					int routed_travel_time;

					if(next_routed_time != 0.0f)
					{
						routed_travel_time = next_routed_time;
					}
					else
					{
						routed_travel_time = current_traj_unit->template estimated_link_accepting_time<int>();
					}
					
					float observed_delay_ratio = routed_travel_time > 0 ? (float)current_travel_time / (float)routed_travel_time : 0;
					
					float would_be_current_travel_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() - mp->template departed_time<Time_Seconds>();
					float would_be_routed_travel_time = current_traj_unit->template estimated_link_accepting_time<int>();
					float would_be_delay_ratio = would_be_routed_travel_time > 0 ? (float)would_be_current_travel_time / (float)would_be_routed_travel_time : 0;

					//cout << "switched: " << current_travel_time << " vs " << would_be_current_travel_time << " and " << routed_travel_time << " vs " << would_be_routed_travel_time << endl;

					if(
						(observed_delay_ratio > ((_Scenario_Interface*)_global_scenario)->template minimum_delay_ratio_for_enroute_switching<float>() && would_be_delay_ratio <= ((_Scenario_Interface*)_global_scenario)->template minimum_delay_ratio_for_enroute_switching<float>()) ||
						((current_travel_time - routed_travel_time) > ((_Scenario_Interface*)_global_scenario)->template minimum_delay_seconds_for_enroute_switching<float>() && (would_be_current_travel_time - would_be_routed_travel_time) <= ((_Scenario_Interface*)_global_scenario)->template minimum_delay_seconds_for_enroute_switching<float>())
						)
					{
						cout << "look ahead caused switch: " << current_travel_time << " vs " << would_be_current_travel_time << " and " << routed_travel_time << " vs " << would_be_routed_travel_time << endl;
					}
					*/

					// three conditions:
					// - a person's perceived delay in arriving to their destination
					// - a sufficient amount of time spent delayed
					// - sufficiently far from their destination in absolute terms

					if (observed_delay_ratio > ((_Scenario_Interface*)_global_scenario)->template minimum_delay_ratio_for_enroute_switching<float>() &&
						(current_travel_time - routed_travel_time) > ((_Scenario_Interface*)_global_scenario)->template minimum_delay_seconds_for_enroute_switching<float>() /*&&
						( mp->template estimated_time_of_arrival<float>() - iteration() ) < ((_Scenario_Interface*)_global_scenario)->template minimum_seconds_from_arrival_for_enroute_switching<float>()*/)
					{
						//cout << "switched: " << current_travel_time << " vs " << routed_travel_time << endl;

						double r0 = Uniform_RNG.template Next_Rand<double>();

						if(r0 <= ((_Scenario_Interface*)_global_scenario)->template enroute_excessive_delay_factor<float>())
						{
							//cout << "vehicle id: " << uuid<int>() << ", departure time: " << mp->template departed_time<Time_Seconds>() <<"current trajectory position: " << position_index << ", current_travel_time = " << current_travel_time << ", routed_travel_time = " << routed_travel_time << ", observed_delay_ratio = " << observed_delay_ratio << endl;
							enroute_switching_decision = true;
							cause_for_switching = Scenario_Components::Types::Cause_For_Enroute_Switching::EXCESSIVE_DELAY;
						}
					}
				}

				if (!enroute_switching_decision)
				{

					if (_enroute_information_type == Vehicle_Components::Types::Enroute_Information_Keys::WITH_REALTIME_INFORMATION) 
					{///case 1: with realtime information
						double r1 = Uniform_RNG.template Next_Rand<double>();//vehicle->template rng_stream<RNG_Components::RngStream&>().RandU01();
						if (r1 <= _information_compliance_rate) 
						{
							enroute_switching_decision = true;
							cause_for_switching = Scenario_Components::Types::Cause_For_Enroute_Switching::REALTIME_INFORMED;
						}
					}
					else if (_enroute_information_type == Vehicle_Components::Types::Enroute_Information_Keys::NO_REALTIME_INFORMATION) 
					{///case 2: no realtime information
						double r1 = Uniform_RNG.template Next_Rand<double>();//vehicle->template rng_stream<RNG_Components::RngStream&>().RandU01();
						if (r1 <= _information_compliance_rate)
						{

							/// case 2.3: Accident
							if (((_Link_Interface*)link)->template current_accident_event<_Network_Event_Interface*>() != nullptr)
							{
								//enroute_switching_decision = true;
								//cout<< "uninformed vehicle switching...accident" <<endl;
							}
							else
							{
								boost::unordered::unordered_set<_Network_Event_Interface*> events_set;
								
								/// case 2.1: VMS
								((_Link_Interface*)link)->template get_events_from_vms<boost::unordered::unordered_set<_Network_Event_Interface*>&>(events_set);

								bool vms = false;
								int vms_event_size = int(events_set.size());
								if (vms_event_size>0)
								{
									_its_switch = true;
									vms = true;
								}
								/// case 2.2: HAR
								((_Link_Interface*)link)->template get_events_from_har<boost::unordered::unordered_set<_Network_Event_Interface*>&>(events_set);
								int har_event_size = int(events_set.size()) - vms_event_size;
								bool har = false;
								if (har_event_size>0)
								{
									har = true;
								}
								/// exploit
								if (vms || har)
								{
									enroute_switching_decision = exploit_events_set<boost::unordered::unordered_set<_Network_Event_Interface*>&>(events_set);
									if (enroute_switching_decision)	cause_for_switching = Scenario_Components::Types::Cause_For_Enroute_Switching::ITS_INFORMED;
								}
							}
						}
					}
				}
				if (enroute_switching_decision)
				{
					enroute_switching<NULLTYPE>(cause_for_switching);
					_last_enroute_switching_route_check_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>(); 
				}
			}

			template<typename TargetType> TargetType next_link_travel_time()
			{
				//_Movement_Plan_Interface* mp = (_Movement_Plan_Interface*)_movement_plan;
				//
				//_Link_Interface* origin_link = mp->template current_link<_Link_Interface*>();

				//int position_index = mp->template current_trajectory_position<int>();

				//_Trajectory_Container_Interface& traj_container = mp->template trajectory_container<_Trajectory_Container_Interface&>();
				//_Trajectory_Unit_Interface* current_traj_unit = traj_container[position_index+1];
				//
				/////calculate travel time of current route
				//_Link_Interface* route_link = mp->template current_link<_Link_Interface*>();
				//_Link_Interface* next_route_link = mp->next_link<_Link_Interface*>();
				//// return 0 if we are at the end of the trajectory
				//if (next_route_link==nullptr) return 0.0;

				//int inbound_link_id = route_link->template uuid<int>();
				//int outbound_link_id = next_route_link->template uuid<int>();
				//typename MasterType::network_type::long_hash_key_type long_hash_key;
				//long_hash_key.inbound_link_id = inbound_link_id;
				//long_hash_key.outbound_link_id = outbound_link_id;
				//typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->template link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();
				//_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];
				//return regular_movement->template forward_link_turn_travel_time<float>();

				float current_travel_time = 0.0;

				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
				typename _Trajectory_Container_Interface::iterator itr;

				_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Link_Interface*>();
				
				///calculate travel time of current route
				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr <= (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()) + 2; itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();

					if (itr < trajectory.end() - 1)
					{
						_Trajectory_Unit_Interface* next_trajectory_unit = (_Trajectory_Unit_Interface*)(*(itr+1));
						_Link_Interface* next_route_link = next_trajectory_unit->template link<_Link_Interface*>();

						int inbound_link_id = route_link->template uuid<int>();
						int outbound_link_id = next_route_link->template uuid<int>();

						typename MasterType::network_type::long_hash_key_type long_hash_key;
						long_hash_key.inbound_link_id = inbound_link_id;
						long_hash_key.outbound_link_id = outbound_link_id;
						typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->template link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();
						_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];
						float link_turn_travel_time = regular_movement->template forward_link_turn_travel_time<float>();
						current_travel_time += link_turn_travel_time;
					}
				}

				return current_travel_time;
			}

			template<typename TargetType> void unload()
			{
				_simulation_status = Types::Vehicle_Status_Keys::OUT_NETWORK;
				
				//int departure_time = ((_Movement_Plan_Interface*)_movement_plan)->template departed_time<Time_Seconds>();
				//int current_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				//float travel_time = float ((current_time - departure_time)/3600.0f);

				//((_Network_Interface*)_global_network)->template increase_out_network_vht_vehicle_based<NT>(travel_time);
				 
				//if (!((_Scenario_Interface*)_global_scenario)->template write_vehicle_trajectory<bool>())
					//clear_trajectory<TargetType>();
			}

			template<typename TargetType> void update_vht()
			{
				int departure_time = ((_Movement_Plan_Interface*)_movement_plan)->template departed_time<Time_Seconds>();
				int current_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				float travel_time = float(current_time - departure_time) / 3600.0f;

				((_Network_Interface*)_global_network)->template increase_in_network_vht_vehicle_based<NT>(travel_time);
			}

			template<typename TargetType> void clear_trajectory()
			{
				//_Trajectory_Container_Interface& trajectory = ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
				//for (int i = 0; i < (int)trajectory.size(); i++)
				//{
				//	Free<typename _Trajectory_Unit_Interface::Component_Type>((typename _Trajectory_Unit_Interface::Component_Type*)trajectory[i]);
				//}
				//trajectory.clear();
				//typedef typename _Trajectory_Container_Interface::Component_Type trajectory_container_type;
				//trajectory_container_type().swap((trajectory_container_type&)trajectory);

			}

			template<typename TargetType> void start_agent()
			{
				_suggested_action = Vehicle_Components::Types::DO_NOTHING;
				int first_trigger_iteration = ((iteration() / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()) + 1) * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() - 1;
				//TODO
//load_event(ComponentType,ComponentType::template Vehicle_Action_Condition,ComponentType::template Vehicle_Action,first_triggeriteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION,NULLTYPE);
			}

			static void Vehicle_Action_Condition(ComponentType* _this,Event_Response& response)
			{
				if (((_Vehicle_Interface*)_this)->template simulation_status<Types::Vehicle_Status_Keys>() == Types::Vehicle_Status_Keys::OUT_NETWORK)
				{
					response.result=false;
					response.next._iteration=END;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION)
				{
					//((typename MasterType::vehicle_type*)_this)->Swap_Event((Event)&Vehicle_Action<NULLTYPE>);
					response.result=true;
					if (((_Vehicle_Interface*)_this)->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Types::Vehicle_Status_Keys::IN_NETWORK)
					{
						response.next._iteration=_iteration;
						response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_TRANSFER_SUB_ITERATION;
					}
					else
					{
						response.next._iteration=_iteration + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
						response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION;
					}
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_TRANSFER_SUB_ITERATION)
				{
					//((typename MasterType::vehicle_type*)_this)->Swap_Event((Event)&Vehicle_Action<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_TRANSFER_SUB_ITERATION;
				}
				else
				{
					cout << "Should never reach here in vehicle action conditional!" << endl;
					assert(false);				
				}
			}

			//declare_event(Vehicle_Action)
			void Vehicle_Action()
			{
				take_action<NT>();
			}

			template<typename TargetType> bool exploit_events_set(TargetType events_set)
			{

				typedef Network_Event<typename MasterType::base_network_event_type> _Base_Event_Interface;
				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();

				typename _Trajectory_Container_Interface::iterator itr;

				bool event_found_flag = false;
				float route_adjustment_factor = 1.0f;
				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
					
					float adjustment_factor = 1.0f;

					///weather
					_Base_Event_Interface* weather_event = route_link->template current_weather_event<_Base_Event_Interface*>();
					if (weather_event != nullptr)
					{
						float adjustment_factor_weather = 1.0f;
						if (events_set.find(weather_event) != events_set.end())
						{
							//adjustment_factor_weather = 1.0f;
							adjustment_factor_weather = min(adjustment_factor_weather,route_link->template speed_adjustment_factor_due_to_weather<float>());
							//event_found_flag = true;
							//break;
						}
						adjustment_factor *= adjustment_factor_weather;
					}

					///accident
					_Base_Event_Interface* accident_event = route_link->template current_accident_event<_Base_Event_Interface*>();
					if (accident_event != nullptr)
					{
						float adjustment_factor_accident = 1.0f;
						if (events_set.find(accident_event) != events_set.end())
						{
							adjustment_factor_accident = min(adjustment_factor_accident,route_link->template capacity_adjustment_factor_due_to_accident<float>()); // speed factor is not used because it is 1.0 according to the Guidebook
							//event_found_flag = true;
							//break;
						}
						adjustment_factor *= adjustment_factor_accident;
					}

					route_adjustment_factor = min(route_adjustment_factor, adjustment_factor);
				}

				if (route_adjustment_factor<0.9f)
				{
					event_found_flag = true;
				}

				return event_found_flag;
			}

			template<typename TargetType> void update_enroute_switch_decisions(int cause_for_switching)
			{
				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();

				//_Switch_Decision_Data_Interface* switch_decision_data = (_Switch_Decision_Data_Interface*)Allocate<typename MasterType::switch_decision_data_type>();
				
				//switch_decision_data->template switch_decision_index<int>(int(_switch_decisions_container.size()));

				//typename _Trajectory_Container_Interface::iterator itr;

				//for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				//{
				//	_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
				//	_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
				//	_Links_Container_Interface& links_container = switch_decision_data->template route_links_container<_Links_Container_Interface&>();
				//	links_container.push_back(route_link);
				//}
				//switch_decisions_container<_Switch_Decision_Data_Container_Interface&>().push_back(switch_decision_data);

				((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_switched_decisions<NULLTYPE>(cause_for_switching);
			}

			template<typename TargetType> void enroute_switching(int cause_for_switching)
			{
				//TODO:ROUTING_OPERATION
				/////calculate travel time of the best route
				//_Regular_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Regular_Link_Interface*>();
				//_Regular_Link_Interface* destination_link = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Regular_Link_Interface*>();
				//_router->template routable_origin<_Regular_Link_Interface*>(origin_link);
				//_router->template routable_destination<_Regular_Link_Interface*>(destination_link);


				//TODO:ROUTING_OPERATION
				//bool use_realtime_travel_time = ((_Scenario_Interface*)_global_scenario)->template use_realtime_travel_time_for_enroute_switching<bool>();;
				//_Routable_Network_Interface* routable_network_ptr;
				//if (use_realtime_travel_time)
				//{
				//	routable_network_ptr = _router->template realtime_routable_network<_Routable_Network_Interface*>();
				//}
				//else
				//{

				//	routable_network_ptr = _router->template routable_network<_Routable_Network_Interface*>();
				//}

				//already commented out
				////float routed_travel_time = router->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr);

				//TODO:ROUTING_OPERATION
				//float best_route_time_to_destination = _router->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr, reversed_arrival_time_container);

				float current_route_time_to_destination = 0.0f;

				update_eta<NULLTYPE>(current_route_time_to_destination);
				_Network_Interface* net = (_Network_Interface*)_global_network;

				Routable_Network<typename MasterType::routable_network_type>* routable_net = net->routable_network<typename MasterType::routable_network_type>();
				
				_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Link_Interface*>();
				_Link_Interface* destination_link = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Link_Interface*>();

				boost::container::deque<global_edge_id> path_container;
				boost::container::deque<float> cumulative_cost_container;

				int best_route_link_sum = 0;

				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;

				float best_route_time_to_destination = 0.0f;

				//if(!((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				//{
					best_route_time_to_destination = routable_net->compute_static_network_path(origin_link->template uuid<int>(),destination_link->template uuid<int>(),path_container,cumulative_cost_container);
				//}
				//else
				//{
				//	best_route_time_to_destination = routable_net->compute_time_dependent_network_path(origin_link->template uuid<int>(),destination_link->template uuid<int>(),iteration(),path_container,cumulative_cost_container);
				//}



				if (best_route_time_to_destination >= 0.0)
				{	
					
					if (path_container.size() == 0)
					{
						//THROW_WARNING(endl << "Error: path size is: " << routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() << endl);
						THROW_EXCEPTION("no path between origin link uuid " << origin_link->template uuid<int>() << " and destination link uuid " << destination_link->template uuid<int>() << endl);
					}
					
					//This code doesn't reliably verify that the path is new
					//typename _Reversed_Path_Container_Interface::iterator itr;
					//for(itr = routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin(); itr != routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().end(); itr++)
					//{
					//	_Regular_Link_Interface* link = (_Regular_Link_Interface*)(*itr);
					//	best_route_link_sum += link->template internal_id<int>();
					//}
					//_Regular_Link_Interface* regular_destination_link = (_Regular_Link_Interface*)(*(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin()));
					//_Routable_Link_Interface* routable_destination_link;
					//if (use_realtime_travel_time)
					//{
					//	routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]);
					//}
					//else
					//{
					//	routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]);
					//}

					if (best_route_time_to_destination < current_route_time_to_destination)
					{
						if (best_route_time_to_destination < current_route_time_to_destination*(1.0 - _relative_indifference_band_route_choice) &&
							best_route_time_to_destination < (current_route_time_to_destination - _minimum_travel_time_saving))
						{
							update_enroute_switch_decisions<  TargetType>(cause_for_switching);
							((_Movement_Plan_Interface*)_movement_plan)->update_trajectory(path_container, cumulative_cost_container);

							int current_time = ((_Regular_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>();
							((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>(current_time + best_route_time_to_destination);
							
							int departure_time = ((_Movement_Plan_Interface*)_movement_plan)->template absolute_departure_time<int>();
							((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>(current_time - departure_time + best_route_time_to_destination);
							///cout << "decided to switch" << endl;
						}
						else
						{
							//cout << "decided not to switch: " << best_route_time_to_destination << "," << current_route_time_to_destination << "," << current_route_time_to_destination*(1.0 - _relative_indifference_band_route_choice) << "," << (current_route_time_to_destination - _minimum_travel_time_saving) << "," << _minimum_travel_time_saving << "," << _relative_indifference_band_route_choice << endl;
						}
					}
					else
					{

						if(best_route_time_to_destination != current_route_time_to_destination)
						{
							THROW_WARNING("eta error, best route time greater than current route time: " << best_route_time_to_destination << "," << current_route_time_to_destination);

						//	_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
						//	
						//	float cost_sum = 0.0f;
						//	
						//	cout << "Routed cost container: " << endl;

						//	for(boost::container::deque<float>::iterator itr = cumulative_cost_container.begin(); itr!=cumulative_cost_container.end();itr++)
						//	{
						//		cout << *itr << endl;
						//	}
						//	cout << "Routed cost profile: " << endl;

						//	for(boost::container::deque<global_edge_id>::iterator itr = path_container.begin();itr!=path_container.end();)
						//	{
						//		_Link_Interface* link = net->get_link_ptr< typename MasterType::link_type >( itr->edge_id );

						//		++itr;

						//		if(itr!=path_container.end())
						//		{
						//			_Link_Interface* next_link = net->get_link_ptr< typename MasterType::link_type >( itr->edge_id );

						//			//int inbound_link_id = link->template internal_id<int>();
						//			//int outbound_link_id = next_link->template internal_id<int>();
						//			int inbound_link_id = link->template uuid<int>();
						//			int outbound_link_id = next_link->template uuid<int>();

						//			typename MasterType::network_type::long_hash_key_type long_hash_key;
						//			long_hash_key.inbound_link_id = inbound_link_id;
						//			long_hash_key.outbound_link_id = outbound_link_id;
						//			typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->template link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();

						//			cout << link_turn_movement_map.count(long_hash_key.movement_id) << ": " << inbound_link_id << "," << outbound_link_id << "," << endl;

						//			_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];
						//		
						//			float current = regular_movement->template forward_link_turn_travel_time<float>();
						//		
						//			cost_sum+=current;

						//			cout << link->template uuid<int>() << " to " << next_link->template uuid<int>() << "," << cost_sum << "," << current << endl;

						//			cout << regular_movement->inbound_link<_Link_Interface*>()->template uuid<int>() << " to " << regular_movement->outbound_link<_Link_Interface*>()->template uuid<int>() << "," << regular_movement->template forward_link_turn_travel_time<float>() << endl;

						//			cout << "Outbound turn movements summary: " << endl;

						//			boost::container::vector<typename MasterType::turn_movement_type*>* movements = link->outbound_turn_movements<boost::container::vector<typename MasterType::turn_movement_type*>*>();

						//			for(boost::container::vector<typename MasterType::turn_movement_type*>::iterator itr = movements->begin();itr!=movements->end();itr++)
						//			{
						//				Movement<typename MasterType::turn_movement_type>* current_movement = (Movement<typename MasterType::turn_movement_type>*) *itr;

						//				cout << current_movement->inbound_link<_Link_Interface*>()->template uuid<int>() << "," << current_movement->outbound_link<_Link_Interface*>()->template uuid<int>() << "," << current_movement->template forward_link_turn_travel_time<float>() << endl;
						//			}
						//		}
						//	}

						//	cost_sum = 0.0f;

						//	cout << "Current cost profile: " << endl;
						//
						//	//_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
						//	typename _Trajectory_Container_Interface::iterator itr;

						//	for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
						//	{
						//		_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
						//		_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();

						//		if (itr < trajectory.end() - 1)
						//		{
						//			_Trajectory_Unit_Interface* next_trajectory_unit = (_Trajectory_Unit_Interface*)(*(itr+1));
						//			_Link_Interface* next_route_link = next_trajectory_unit->template link<_Link_Interface*>();

						//			//int inbound_link_id = route_link->template internal_id<int>();
						//			//int outbound_link_id = next_route_link->template internal_id<int>();
						//			int inbound_link_id = route_link->template uuid<int>();
						//			int outbound_link_id = next_route_link->template uuid<int>();

						//			typename MasterType::network_type::long_hash_key_type long_hash_key;
						//			long_hash_key.inbound_link_id = inbound_link_id;
						//			long_hash_key.outbound_link_id = outbound_link_id;
						//			typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->template link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();
						//			_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];

						//			float current = regular_movement->template forward_link_turn_travel_time<float>();
						//		
						//			cost_sum+=current;

						//			cout << route_link->template uuid<int>() << " to " << next_route_link->template uuid<int>() << "," << cost_sum << "," << current << endl;
						//		}
						//	}
						}
					}
				}

				///find a new route using shortest path algorithm
				
				
				//already commented out
				////if (routed_travel_time >= 0.0)

				////TODO:ROUTING_OPERATION
				//if (best_route_time_to_destination >= 0.0)
				//{	
				//	
				//	if (routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() == 0)
				//	{
				//		THROW_WARNING(endl << "Error: path size is: " << routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() << endl);
				//		THROW_EXCEPTION(endl << "no path between origin link uuid " << origin_link->template uuid<int>() << " and destination link uuid " << destination_link->template uuid<int>());
				//	}
				//	
				//	typename _Reversed_Path_Container_Interface::iterator itr;
				//	for(itr = routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin(); itr != routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().end(); itr++)
				//	{
				//		_Regular_Link_Interface* link = (_Regular_Link_Interface*)(*itr);
				//		best_route_link_sum += link->template internal_id<int>();
				//	}
				//	_Regular_Link_Interface* regular_destination_link = (_Regular_Link_Interface*)(*(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin()));
				//	_Routable_Link_Interface* routable_destination_link;
				//	if (use_realtime_travel_time)
				//	{
				//		routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]);
				//	}
				//	else
				//	{
				//		routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]);
				//	}

				//	if (best_route_link_sum != current_route_link_sum_to_destination)
				//	{
				//		if (best_route_time_to_destination < current_route_time_to_destination*(1.0 - _relative_indifference_band_route_choice) &&
				//			best_route_time_to_destination < (current_route_time_to_destination - _minimum_travel_time_saving))
				//		{
				//			update_enroute_switch_decisions<  TargetType>(cause_for_switching);
				//			((_Movement_Plan_Interface*)_movement_plan)->template update_trajectory<_Reversed_Path_Container_Interface>(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>(), reversed_arrival_time_container);

				//			int current_time = ((_Regular_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>();
				//			((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>(current_time + best_route_time_to_destination);
				//			int departure_time = ((_Movement_Plan_Interface*)_movement_plan)->template absolute_departure_time<int>();
				//			((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>(current_time - departure_time + best_route_time_to_destination);
				//		}
				//	}
				//}
			}

			template<typename TargetType> void initialize()
			{
				//_is_integrated=false;
				
				_simulation_status=Types::Vehicle_Status_Keys::UNLOADED;

				///
				//unsigned long seed = ((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>()+_internal_id+1;
				//unsigned long seed = abs(std::sin(((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>() + (float)_internal_id + 1)*(float)INT_MAX);
				//unsigned long seed = 1;
				//_rng_stream.SetSeed(seed);
				double r1;
				///information capability
				r1 = Uniform_RNG.template Next_Rand<double>();//_rng_stream.RandU01();
				if (r1 <= ((_Scenario_Interface*)_global_scenario)->template realtime_informed_vehicle_market_share<double>())
				{
					_enroute_information_type = Vehicle_Components::Types::Enroute_Information_Keys::WITH_REALTIME_INFORMATION;
				}
				else
				{
					_enroute_information_type = Vehicle_Components::Types::Enroute_Information_Keys::NO_REALTIME_INFORMATION;
				}

				/// information compliance rate
				r1 = ((_Scenario_Interface*)_global_scenario)->template information_compliance_rate_mean<double>();
				_information_compliance_rate = r1;

				///rib
				r1 = Uniform_RNG.template Next_Rand<double>();//_rng_stream.RandU01();
				double mean = ((_Scenario_Interface*)_global_scenario)->template relative_indifference_band_route_choice_mean<double>();
				double a = 0.0;
				double b = 2.0*mean;
				double rib = Uniform_RNG.template triangular_random_variate<double>(r1,a,b,mean);//_rng_stream.triangular_random_variate(r1,a,b,mean);
				_relative_indifference_band_route_choice = rib;

				///mtts
				r1 = Uniform_RNG.template Next_Rand<double>();//_rng_stream.RandU01();
				mean = ((_Scenario_Interface*)_global_scenario)->template minimum_travel_time_saving_mean<double>();
				a = 0.5 * mean;
				b = a + 2.0*mean;
				double mtts = Uniform_RNG.template triangular_random_variate<double>(r1,a,b,mean);//_rng_stream.triangular_random_variate(r1,a,b,mean);
				_minimum_travel_time_saving = mtts;
				///
				_last_enroute_switching_route_check_time = 0;
				//Initialize<TargetType>();

				// determine if we track this vehicle for trajectory writing
				_write_trajectory = false;
				if (((_Scenario_Interface*)_global_scenario)->use_vehicle_tracking_list<bool>())
				{
					std::unordered_set<int>& tracking_list = ((_Scenario_Interface*)_global_scenario)->vehicle_tracking_list<std::unordered_set<int>&>();
					std::unordered_set<int>::iterator itr = tracking_list.find(_uuid);
					if (itr != tracking_list.end())
					{
						_write_trajectory = true;
					}
				}
				else
				{
					float x = ((_Scenario_Interface*)_global_scenario)->template vehicle_trajectory_sample_rate<float>();
					if (x < 0.99999999)
					{
						float r = GLOBALS::Uniform_RNG. template Next_Rand<float>();
						if (r < x) _write_trajectory = true;
					}
					else
					{
						_write_trajectory = true;
					}
				}
			}

			template<typename TargetType> void update_eta(float& current_route_time_to_destination)
			{
				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
				typename _Trajectory_Container_Interface::iterator itr;

				_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Link_Interface*>();
				_Link_Interface* destination_link = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Link_Interface*>();
				
				///calculate travel time of current route

				bool use_realtime_travel_time = ((_Scenario_Interface*)_global_scenario)->template use_realtime_travel_time_for_enroute_switching<bool>();

				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
					//_Routable_Link_Interface* routable_link;
					//if (use_realtime_travel_time)
					//{
					//	routable_link = (_Routable_Link_Interface*)(route_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
					//}
					//else
					//{
					//	routable_link = (_Routable_Link_Interface*)(route_link->template replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
					//}

					if (itr < trajectory.end() - 1)
					{
						_Trajectory_Unit_Interface* next_trajectory_unit = (_Trajectory_Unit_Interface*)(*(itr+1));
						_Link_Interface* next_route_link = next_trajectory_unit->template link<_Link_Interface*>();

						//_Routable_Link_Interface* next_routable_link;
						//if (use_realtime_travel_time)
						//{
						//	next_routable_link = (_Routable_Link_Interface*)(next_route_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
						//}
						//else
						//{
						//	next_routable_link = (_Routable_Link_Interface*)(next_route_link->template replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
						//}

						//int inbound_link_id = route_link->template internal_id<int>();
						//int outbound_link_id = next_route_link->template internal_id<int>();
						int inbound_link_id = route_link->template uuid<int>();
						int outbound_link_id = next_route_link->template uuid<int>();

						typename MasterType::network_type::long_hash_key_type long_hash_key;
						long_hash_key.inbound_link_id = inbound_link_id;
						long_hash_key.outbound_link_id = outbound_link_id;
						typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->template link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();
						_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];
						//_Routable_Movement_Interface* routable_movement;
						//if (use_realtime_travel_time)
						//{
						//	routable_movement = (_Routable_Movement_Interface*)regular_movement->template realtime_replicas_container<_Routable_Movements_Container_Interface&>()[__thread_id]; 
						//}
						//else
						//{
						//	routable_movement = (_Routable_Movement_Interface*)regular_movement->template replicas_container<_Routable_Movements_Container_Interface&>()[__thread_id]; 
						//}
						float link_turn_travel_time = regular_movement->template forward_link_turn_travel_time<float>();
						current_route_time_to_destination += link_turn_travel_time;
					}
				}

				int current_time = ((_Regular_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>();
				int departure_time = ((_Movement_Plan_Interface*)_movement_plan)->template absolute_departure_time<int>();
				float current_eta = ((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>();
				((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>(current_time + current_route_time_to_destination);
				float arrival_time_diff = ((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>() - current_eta;
				int routed_travel_time = ((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>();
				((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>(current_time - departure_time + current_route_time_to_destination);
			}
		};
	}

}
