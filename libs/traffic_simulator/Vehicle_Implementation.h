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
			typedef typename Polaris_Component<MasterType,INHERIT(Vehicle_Implementation),Execution_Object>::ComponentType ComponentType;
			
			m_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);
			m_data(bool, is_integrated, NONE, NONE);
			m_data(bool, its_switch, NONE, NONE);

			m_prototype(Null_Prototype<typename MasterType::movement_plan_type>, movement_plan, NONE, NONE);
#ifndef EXCLUDE_DEMAND
			m_prototype(Null_Prototype<typename MasterType::person_type>, traveler, NONE, NONE);
#else
			m_prototype(Null_Prototype<typename MasterType::traveler_type>, traveler, NONE, NONE);
#endif
			m_prototype(Routing_Components::Prototypes::Routing< typename MasterType::routing_type>, router, NONE, NONE);
			m_data(float, distance_to_stop_bar, NONE, NONE);
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

			m_data(Vehicle_Components::Types::Type_Vehicle_Action_keys, suggested_action, NONE, NONE);

			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;
			typedef  Switch_Decision_Data<typename remove_pointer<typename  type_of(switch_decisions_container)::value_type>::type>  _Switch_Decision_Data_Interface;
			typedef  Random_Access_Sequence< type_of(switch_decisions_container), _Switch_Decision_Data_Interface*> _Switch_Decision_Data_Container_Interface;

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
			typedef  Traveler_Components::Prototypes::Traveler< type_of(traveler)> _Traveler_Interface;
			typedef Network_Components::Prototypes::Network<typename MasterType::routable_network_type> _Routable_Network_Interface;
			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(reversed_path_container)::value_type>::type>  _Regular_Link_Interface;
			typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(reversed_path_container), _Regular_Link_Interface*> _Reversed_Path_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Regular_Link_Interface::get_type_of(realtime_replicas_container)::value_type>::type>  _Routable_Link_Interface;
			typedef  Random_Access_Sequence< typename _Regular_Link_Interface::get_type_of(realtime_replicas_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Regular_Network_Interface;
			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Network_Interface::get_type_of(turn_movements_container)::value_type>::type>  _Regular_Movement_Interface;
			typedef  Random_Access_Sequence< typename _Regular_Network_Interface::get_type_of(turn_movements_container), _Regular_Movement_Interface*> _Regular_Movements_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Movement_Interface::get_type_of(realtime_replicas_container)::value_type>::type>  _Routable_Movement_Interface;
			typedef  Random_Access_Sequence< typename _Regular_Movement_Interface::get_type_of(realtime_replicas_container), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;

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
					int current_travel_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() - mp->template departed_time<Time_Seconds>();
					int position_index = mp->template current_trajectory_position<int>();
					_Trajectory_Container_Interface& traj_container = mp->template trajectory_container<_Trajectory_Container_Interface&>();
					_Trajectory_Unit_Interface* current_traj_unit = traj_container[position_index];
					int routed_travel_time = current_traj_unit->template estimated_link_accepting_time<int>();
					float observed_delay_ratio = routed_travel_time > 0 ? (float)current_travel_time / (float)routed_travel_time : 0;
					if (observed_delay_ratio > ((_Scenario_Interface*)_global_scenario)->template minimum_delay_ratio_for_enroute_switching<float>() && current_travel_time - routed_travel_time > ((_Scenario_Interface*)_global_scenario)->template minimum_delay_seconds_for_enroute_switching<float>())
					{
						double r0 = Uniform_RNG.Next_Rand<double>();

						if(r0 <= ((_Scenario_Interface*)_global_scenario)->template enroute_excessive_delay_factor<float>())
						{
							//cout << "vehicle id: " << vehicle->uuid<int>() << ", departure time: " << mp->template departed_time<Time_Seconds>() <<"current trajectory position: " << position_index << ", current_travel_time = " << current_travel_time << ", routed_travel_time = " << routed_travel_time << ", observed_delay_ratio = " << observed_delay_ratio << endl;
							enroute_switching_decision = true;
							cause_for_switching = Scenario_Components::Types::Cause_For_Enroute_Switching::EXCESSIVE_DELAY;
						}
					}
				}
				if (!enroute_switching_decision)
				{

					if (_enroute_information_type == Vehicle_Components::Types::Enroute_Information_Keys::WITH_REALTIME_INFORMATION) 
					{///case 1: with realtime information
						double r1 = Uniform_RNG.Next_Rand<double>();//vehicle->template rng_stream<RNG_Components::RngStream&>().RandU01();
						if (r1 <= _information_compliance_rate) 
						{
							enroute_switching_decision = true;
							cause_for_switching = Scenario_Components::Types::Cause_For_Enroute_Switching::REALTIME_INFORMED;
						}
					}
					else if (_enroute_information_type == Vehicle_Components::Types::Enroute_Information_Keys::NO_REALTIME_INFORMATION) 
					{///case 2: no realtime information
						double r1 = Uniform_RNG.Next_Rand<double>();//vehicle->template rng_stream<RNG_Components::RngStream&>().RandU01();
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

			template<typename TargetType> void unload()
			{
				_simulation_status = Types::Vehicle_Status_Keys::OUT_NETWORK;
				
				int departure_time = ((_Movement_Plan_Interface*)_movement_plan)->template departed_time<Time_Seconds>();
				int current_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				float travel_time = float ((current_time - departure_time)/3600.0f);

				//((_Network_Interface*)_global_network)->template increase_out_network_vht_vehicle_based<NT>(travel_time);
				 
				if (!((_Scenario_Interface*)_global_scenario)->template write_vehicle_trajectory<bool>())
					clear_trajectory<TargetType>();
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
				_Trajectory_Container_Interface& trajectory = ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
				for (int i = 0; i < (int)trajectory.size(); i++)
				{
					Free<typename _Trajectory_Unit_Interface::Component_Type>((typename _Trajectory_Unit_Interface::Component_Type*)trajectory[i]);
				}				
				typedef typename _Trajectory_Container_Interface::Component_Type trajectory_container_type;
				trajectory_container_type().swap((trajectory_container_type&)trajectory);
			}

			template<typename TargetType> void start_agent()
			{
				_suggested_action = Vehicle_Components::Types::DO_NOTHING;
				int first_triggeriteration() = ((iteration() / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()) + 1) * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() - 1;
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

			declare_event(Vehicle_Action)
			{
				((_Vehicle_Interface*)_this)->template take_action<NT>();
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
							adjustment_factor_weather = 1.0f;//min(adjustment_factor_weather,route_link->template speed_adjustment_factor_due_to_weather<float>());
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

				_Switch_Decision_Data_Interface* switch_decision_data = (_Switch_Decision_Data_Interface*)Allocate<typename MasterType::switch_decision_data_type>();
				
				switch_decision_data->template switch_decision_index<int>(int(_switch_decisions_container.size()));

				typename _Trajectory_Container_Interface::iterator itr;

				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
					_Links_Container_Interface& links_container = switch_decision_data->template route_links_container<_Links_Container_Interface&>();
					links_container.push_back(route_link);
				}
				switch_decisions_container<_Switch_Decision_Data_Container_Interface&>().push_back(switch_decision_data);

				((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_switched_decisions<NULLTYPE>(cause_for_switching);
			}

			template<typename TargetType> void enroute_switching(int cause_for_switching)
			{
				int current_route_link_sum_to_destination = 0;
				float current_route_time_to_destination = 0.0f;

				update_eta<NULLTYPE>(current_route_link_sum_to_destination, current_route_time_to_destination);

				///calcualte travel time of the best route
				_Regular_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Regular_Link_Interface*>();
				_Regular_Link_Interface* destination_link = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Regular_Link_Interface*>();
				_router->template routable_origin<_Regular_Link_Interface*>(origin_link);
				_router->template routable_destination<_Regular_Link_Interface*>(destination_link);

				bool use_realtime_travel_time = ((_Scenario_Interface*)_global_scenario)->template use_realtime_travel_time_for_enroute_switching<bool>();;
				_Routable_Network_Interface* routable_network_ptr;
				if (use_realtime_travel_time)
				{
					routable_network_ptr = _router->template realtime_routable_network<_Routable_Network_Interface*>();
				}
				else
				{

					routable_network_ptr = _router->template routable_network<_Routable_Network_Interface*>();
				}

				//float routed_travel_time = router->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr);
				int best_route_link_sum = 0;
				boost::container::vector<float> reversed_arrival_time_container;
				float best_route_time_to_destination = _router->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr, reversed_arrival_time_container);

				///find a new route using shortest path algorithm
				//if (routed_travel_time >= 0.0)
				if (best_route_time_to_destination >= 0.0)
				{	
					if (routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() == 0)
					{
						THROW_WARNING(endl << "Error: path size is: " << routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() << endl);
						THROW_EXCEPTION(endl << "no path between origin link uuid " << origin_link->uuid<int>() << " and destination link uuid " << destination_link->uuid<int>());
					}
					
					typename _Reversed_Path_Container_Interface::iterator itr;
					for(itr = routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin(); itr != routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().end(); itr++)
					{
						_Regular_Link_Interface* link = (_Regular_Link_Interface*)(*itr);
						best_route_link_sum += link->template internal_id<int>();
					}
					_Regular_Link_Interface* regular_destination_link = (_Regular_Link_Interface*)(*(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().begin()));
					_Routable_Link_Interface* routable_destination_link;
					if (use_realtime_travel_time)
					{
						routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]);
					}
					else
					{
						routable_destination_link = (_Routable_Link_Interface*)(regular_destination_link->template replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]);
					}
					//float best_route_time_to_destination = routable_destination_link->template label_cost<float>();

					if (best_route_link_sum != current_route_link_sum_to_destination)
					{
						if (best_route_time_to_destination < current_route_time_to_destination*(1.0 - _relative_indifference_band_route_choice) &&
							best_route_time_to_destination < (current_route_time_to_destination - _minimum_travel_time_saving))
						{
							update_enroute_switch_decisions<  TargetType>(cause_for_switching);
							((_Movement_Plan_Interface*)_movement_plan)->template update_trajectory<_Reversed_Path_Container_Interface>(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>(), reversed_arrival_time_container);

							int current_time = ((_Regular_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>();

							//int current_eta = ((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>();
							((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>(current_time + best_route_time_to_destination);
							//int new_eta = ((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<float>();

							//int routed_travel_time = ((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>();
							//int time_saved = current_route_time_to_destination - best_route_time_to_destination;
							int departure_time = ((_Movement_Plan_Interface*)_movement_plan)->template absolute_departure_time<int>();
							((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>(current_time - departure_time + best_route_time_to_destination);
							//int new_routed_travel_time = ((_Movement_Plan_Interface*)_movement_plan)->template routed_travel_time<float>();

							/////en-route switching
							//if (_enroute_information_type == Vehicle_Components::Types::Enroute_Information_Keys::WITH_REALTIME_INFORMATION)
							//{
								//cout<< "enroute switching...realtime informed vehicle  " << _internal_id << "'s #" << int(_switch_decisions_container.size()) << " switch with " << time_saved/60.0f << " minutes time saving." <<endl;
							//}
							//else
							//{
								//cout<< "enroute switching...realtime uninformed vehicle " << _internal_id << "'s #" << int(_switch_decisions_container.size()) << " switch with " << time_saved/60.0f << " minutes time saving." <<endl;
							//}
							//cout<< "current_time" << convert_seconds_to_hhmmss(current_time) <<endl;
							//cout<< "departure_time" << convert_seconds_to_hhmmss(departure_time) <<endl;
							//cout<< "current_route_time_to_destination = " << current_route_time_to_destination << " seconds, best_route_time_to_destination = " << best_route_time_to_destination << " seconds." <<endl;
							//cout<< "current_travel_time = " << routed_travel_time << " minutes, new_travel_time = " << new_routed_travel_time << " seconds." <<endl;
							//cout<< "current_eta = " << convert_seconds_to_hhmmss(current_eta) << ", new_eta = " << convert_seconds_to_hhmmss(new_eta) << "." <<endl;
						}
					}
				}
			}

			template<typename TargetType> void initialize()
			{
				_is_integrated=false;

				///
				//unsigned long seed = ((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>()+_internal_id+1;
				//unsigned long seed = abs(std::sin(((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>() + (float)_internal_id + 1)*(float)INT_MAX);
				//unsigned long seed = 1;
				//_rng_stream.SetSeed(seed);
				double r1;
				///information capability
				r1 = Uniform_RNG.Next_Rand<double>();//_rng_stream.RandU01();
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
				r1 = Uniform_RNG.Next_Rand<double>();//_rng_stream.RandU01();
				double mean = ((_Scenario_Interface*)_global_scenario)->template relative_indifference_band_route_choice_mean<double>();
				double a = 0.0;
				double b = 2.0*mean;
				double rib = Uniform_RNG.template triangular_random_variate<double>(r1,a,b,mean);//_rng_stream.triangular_random_variate(r1,a,b,mean);
				_relative_indifference_band_route_choice = rib;

				///mtts
				r1 = Uniform_RNG.Next_Rand<double>();//_rng_stream.RandU01();
				mean = ((_Scenario_Interface*)_global_scenario)->template minimum_travel_time_saving_mean<double>();
				a = 0.5 * mean;
				b = a + 2.0*mean;
				double mtts = Uniform_RNG.template triangular_random_variate<double>(r1,a,b,mean);//_rng_stream.triangular_random_variate(r1,a,b,mean);
				_minimum_travel_time_saving = mtts;
				///
				_last_enroute_switching_route_check_time = 0;
				//Initialize<TargetType>();
			}

			template<typename TargetType> void update_eta(int& current_route_link_sum_to_destination, float& current_route_time_to_destination)
			{
	
				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
				typename _Trajectory_Container_Interface::iterator itr;

				_Regular_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Regular_Link_Interface*>();
				_Regular_Link_Interface* destination_link = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Regular_Link_Interface*>();
				
				///calculate travel time of current route

				bool use_realtime_travel_time = ((_Scenario_Interface*)_global_scenario)->template use_realtime_travel_time_for_enroute_switching<bool>();

				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Regular_Link_Interface* route_link = trajectory_unit->template link<_Regular_Link_Interface*>();
					_Routable_Link_Interface* routable_link;
					if (use_realtime_travel_time)
					{
						routable_link = (_Routable_Link_Interface*)(route_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
					}
					else
					{
						routable_link = (_Routable_Link_Interface*)(route_link->template replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
					}
					float link_travel_time = routable_link->template travel_time<float>();

					current_route_link_sum_to_destination += route_link-> template internal_id<int>();
					//current_route_time_to_destination += link_travel_time;
					if (itr < trajectory.end() - 1)
					{
						_Trajectory_Unit_Interface* next_trajectory_unit = (_Trajectory_Unit_Interface*)(*(itr+1));
						_Regular_Link_Interface* next_route_link = next_trajectory_unit->template link<_Regular_Link_Interface*>();

						_Routable_Link_Interface* next_routable_link;
						if (use_realtime_travel_time)
						{
							next_routable_link = (_Routable_Link_Interface*)(next_route_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
						}
						else
						{
							next_routable_link = (_Routable_Link_Interface*)(next_route_link->template replicas_container<_Routable_Links_Container_Interface&>()[__thread_id]); 
						}

						int inbound_link_id = route_link->template internal_id<int>();
						int outbound_link_id = next_route_link->template internal_id<int>();
						typename MasterType::network_type::long_hash_key_type long_hash_key;
						long_hash_key.inbound_link_id = inbound_link_id;
						long_hash_key.outbound_link_id = outbound_link_id;
						typename MasterType::network_type::link_turn_movement_map_type&  link_turn_movement_map = ((_Regular_Network_Interface*)_global_network)->template link_turn_movement_map<typename MasterType::network_type::link_turn_movement_map_type&>();
						_Regular_Movement_Interface* regular_movement = (_Regular_Movement_Interface*)link_turn_movement_map[long_hash_key.movement_id];
						_Routable_Movement_Interface* routable_movement;
						if (use_realtime_travel_time)
						{
							routable_movement = (_Routable_Movement_Interface*)regular_movement->template realtime_replicas_container<_Routable_Movements_Container_Interface&>()[__thread_id]; 
						}
						else
						{
							routable_movement = (_Routable_Movement_Interface*)regular_movement->template replicas_container<_Routable_Movements_Container_Interface&>()[__thread_id]; 
						}
						float link_turn_travel_time = routable_movement->template forward_link_turn_travel_time<float>();
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
