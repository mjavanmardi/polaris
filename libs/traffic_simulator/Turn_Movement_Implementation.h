#pragma once
#include "Turn_Movement_Prototype.h"
#include "../repository/RNG_Implementations.h"

namespace Turn_Movement_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		struct Movement_MOE_Data
		{
			float movement_flow_rate; 
			float movement_flow_rate_prev; //TODO: Omer
			float intervals_with_demand; //TODO: Omer
			float turn_penalty;
			float turn_penalty_standard_deviation;
			float inbound_link_turn_time;
			float outbound_link_turn_time;
		};

		implementation struct Movement_Implementation :public Polaris_Component<MasterType,INHERIT(Movement_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Movement_Implementation),Data_Object>::Component_Type ComponentType;
			
			m_data(float, turn_travel_penalty, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_data(float, movement_capacity, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, movement_supply, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, movement_demand, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, movement_flow, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, movement_transferred, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, movement_capacity_leftover, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_data(Turn_Movement_Components::Types::Turn_Movement_Type_Keys, movement_type, NONE, NONE);
			m_data(Turn_Movement_Components::Types::Turn_Movement_Rule_Keys, movement_rule, NONE, NONE);

			m_data(int, num_turn_lanes, NONE, NONE);

			m_data(float, minimum_merge_rate, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_data(float, forward_link_turn_travel_time, NONE, NONE);
			m_data(float, realtime_forward_link_turn_travel_time, NONE, NONE);

			m_container(std::vector<float>, cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, NONE, NONE);
			m_data(float, outbound_link_arrived_time_based_experienced_link_turn_travel_delay, NONE, NONE);

			//TODO: Omer: 2018.01.25 added for time-dependent reporting by entry time
			//----------------------------------------------------------------------------------
			m_container(std::vector<int>, vehicles_processed_by_entry_time, NONE, NONE);
			m_container(std::vector<float>, turn_delay_by_entry_time, NONE, NONE);
			m_container(std::vector<float>, add_delay_by_entry_time, NONE, NONE);
			//----------------------------------------------------------------------------------

			m_container(std::deque<typename MasterType::vehicle_type*>, vehicles_container, NONE, NONE);

			m_prototype(Link_Components::Prototypes::Link,typename MasterType::link_type, inbound_link, NONE, NONE);

			m_prototype(Link_Components::Prototypes::Link,typename MasterType::link_type, outbound_link, NONE, NONE);

			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);
			m_data(int, merge_priority, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, green_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, inbound_link_green_cycle_ratio, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			m_data(_lock,mvmt_lock, NONE, NONE);

			struct Movement_MOE_Data movement_moe_data;

			struct Movement_MOE_Data realtime_movement_moe_data;

			
			typedef  Link_Components::Prototypes::Link<type_of(inbound_link)> _Link_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer<typename  type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
			typedef  Back_Insertion_Sequence<type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Turn_Movement_Components::Prototypes::Movement<typename MasterType::movement_type> _Turn_Movement_Interface;
			//typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;
			typedef typename MasterType::link_type _link_component_type;

			Movement_Implementation()
			{
				UNLOCK(_mvmt_lock);
			}

			template<typename TargetType> void update_capacity(Intersection_Components::Types::Intersection_Type_Keys intersection_type)
			{
				float outbound_link_capacity = (float)(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() * ((_Link_Interface*)_outbound_link)->template num_lanes<int>() * ((_Link_Interface*)_outbound_link)->template maximum_flow_rate<float>()/3600.0);
				float inbound_link_capacity = ((_Link_Interface*)_inbound_link)->template link_capacity<float>(); 
				float green_time_ratio = _green_time*1.0f/((float)((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
				if(intersection_type == Intersection_Components::Types::Intersection_Type_Keys::NO_CONTROL)
				{
					//_movement_capacity = min(inbound_link_capacity,outbound_link_capacity)*green_time_ratio;
					//_movement_capacity = min(inbound_link_capacity,outbound_link_capacity);
					_movement_capacity = inbound_link_capacity;
				}
				else
				{
					//if(green_time_ratio < 0.8f)
					if(green_time_ratio <= 0.0f)
					{
						//_movement_capacity = min(inbound_link_capacity,outbound_link_capacity)*green_time_ratio;
						_movement_capacity = 0.0f;
					}
					else
					{
						//_movement_capacity = min(inbound_link_capacity,outbound_link_capacity)*green_time_ratio;
						//_movement_capacity = min(inbound_link_capacity,outbound_link_capacity);
						_movement_capacity = inbound_link_capacity;
					}
				}
			}

			template<typename TargetType> void update_demand()
			{
				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				///count vehicles ready for transferring to next link
				_movement_demand -= _movement_transferred;
				if (_movement_demand < 0)
				{
					cout << "movement demand cannot be negative! " << endl;
					assert(false);
				}
				typename _Vehicles_Container_Interface::iterator vehicle_itr;
				int current_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>();

				for (vehicle_itr = _vehicles_container.begin() + _movement_demand; vehicle_itr != _vehicles_container.end(); vehicle_itr++)
				{
					_Vehicle_Interface* vehicle = (_Vehicle_Interface*)(*vehicle_itr);
					int pdt = vehicle->template downstream_preferred_departure_time<int>();

					if (pdt <= current_time)
					{
						_movement_demand++;
					}
					else
					{
						break;
					}
				}

				if(_movement_demand < 0.0) 
				{
					_movement_demand = 0.0;
				}
			}

			template<typename TargetType> void update_flow()
			{
				if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
				{
					_movement_flow = (float) min(min((double)_movement_demand,(double)_movement_capacity),(double)_movement_supply);
				}
				else
				{
					_Link_Interface* lnk = (_Link_Interface*)_inbound_link;
					typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;

					_Intersection_Interface* itx = lnk->template downstream_intersection<_Intersection_Interface*>();



					if(itx->template intersection_type<Intersection_Components::Types::Intersection_Type_Keys>() == Intersection_Components::Types::Intersection_Type_Keys::NO_CONTROL)
					{
						_movement_flow = (float) min((double)_movement_demand,(double)_movement_supply);
					}
					else
					{
						if(_movement_capacity == 0.0f)
						{
							//_movement_flow = (float) min(min((double)_movement_demand,(double)_movement_capacity),(double)_movement_supply);
							_movement_flow = 0.0f;
						}
						else
						{
							_movement_flow = (float) min((double)_movement_demand,(double)_movement_supply);
						}
					}

					//if(_movement_capacity < _movement_demand && _movement_capacity < _movement_supply)
					//{
					//	int inbound_link_type = ((_Link_Interface*)_inbound_link)->template link_type<int>();

					//	if (inbound_link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || inbound_link_type == Link_Components::Types::Link_Type_Keys::LOCAL)
					//	{
					//		

					//		if(_movement_flow > 20) cout << "letting through: " << _movement_flow << " vs " << _movement_capacity << endl;
					//	}
					//	
					//}
				}

				if (((_Scenario_Interface*)_global_scenario)->template rng_type<int>() == Scenario_Components::Types::RNG_Type_Keys::DETERMINISTIC)
				{
					float total_movement_flow = _movement_flow + _movement_capacity_leftover;
					if (total_movement_flow < 0.0f)
					{
						_movement_flow = 0.0f;
						_movement_capacity_leftover = total_movement_flow;
					}
					else
					{
						int num_transfer_vehicles_of_turn_movement = (int)(total_movement_flow);
						// borrow from next simulation interval
						if (total_movement_flow - num_transfer_vehicles_of_turn_movement > 0.005)
							_movement_flow = num_transfer_vehicles_of_turn_movement + 1;
						else
							_movement_flow = num_transfer_vehicles_of_turn_movement;
						_movement_flow = (float)min((double)_movement_flow, (double)_movement_demand);
						_movement_capacity_leftover = total_movement_flow - _movement_flow;
					}
				}
				if (_movement_flow > _vehicles_container.size())
				{
					_movement_flow = float(_vehicles_container.size());
				}
			}

			template<typename TargetType> void transfer_vehicles()
			{
				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();

				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = 0.0;

				//determine number of transfered vehicles
				float transfer_flow_turn_movement = _movement_flow;
						
				int num_transfer_vehicles_of_turn_movement = (int)transfer_flow_turn_movement;
				transfer_flow_turn_movement = transfer_flow_turn_movement - num_transfer_vehicles_of_turn_movement;
				//TODO: Omer: Re-enable the condition later
				//if (((_Scenario_Interface*)_global_scenario)->template rng_type<int>() != Scenario_Components::Types::RNG_Type_Keys::DETERMINISTIC)
				{
					if(transfer_flow_turn_movement > 0.0)
					{
						double rng = Uniform_RNG.template Next_Rand<double>();//rng_stream.RandU01();
						if(rng<=transfer_flow_turn_movement)
						{//partial vehicle, incomplete implementation
							++num_transfer_vehicles_of_turn_movement;
						}
					}
				}

				if (num_transfer_vehicles_of_turn_movement > (int)_vehicles_container.size())
				{
					num_transfer_vehicles_of_turn_movement = (int)_vehicles_container.size();
				}

				_movement_transferred = num_transfer_vehicles_of_turn_movement;
						
				if (num_transfer_vehicles_of_turn_movement>0)
				{
					push_vehicles_to_outbound_link<NULLTYPE>(num_transfer_vehicles_of_turn_movement);
					//float delay=_outbound_link_arrived_time_based_experienced_link_turn_travel_delay/((float)num_transfer_vehicles_of_turn_movement);
					//_outbound_link_arrived_time_based_experienced_link_turn_travel_delay= delay;

					// reset vehicle intersection delays for the link to zero, once it starts moving again
					for (auto itr =_vehicles_container.begin(); itr != _vehicles_container.end(); ++itr)
					{
						_Vehicle_Interface* vehicle=(_Vehicle_Interface*)*itr;
						vehicle->template movement_plan<_Movement_Plan_Interface*>()->template set_current_link_intersection_delay<int>(0);
					}
				}
				else
				{
					if(_movement_rule == Types::Turn_Movement_Rule_Keys::PROHIBITED/* || _movement_capacity == 0.0f*/)
					{
						_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = INFINITY_FLOAT;
					}
					else
					{
						if(_movement_demand == 0)
						{//no demand
							_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = 0.0;
						}
						else
						{//no supply for this turn -- need a prediction -- need to work it later
							int t_minus_one = -1;
							if (current_simulation_interval_index>0)
							{
								t_minus_one = (current_simulation_interval_index-1)%((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();
								float delay = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() + _cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[t_minus_one];
								_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = delay;

								// update vehicles currently being delayed by the signal
								
								//TODO: Omer: 2018.01.25 added for time-dependent reporting by entry time
								//----------------------------------------------------------------------------------
								int assignment_index = current_simulation_interval_index / ((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();
								_add_delay_by_entry_time[assignment_index] += ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
								//----------------------------------------------------------------------------------

								for (auto itr =_vehicles_container.begin(); itr != _vehicles_container.end(); ++itr)
								{
									_Vehicle_Interface* vehicle=(_Vehicle_Interface*)*itr;
									vehicle->template movement_plan<_Movement_Plan_Interface*>()->template update_current_link_intersection_delay<int>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
								}
							}
							else
							{
								_outbound_link_arrived_time_based_experienced_link_turn_travel_delay= 0.0;
							}
						}
					}
				}
			}

			template<typename TargetType> void push_vehicles_to_outbound_link(int num_transfer_vehicles_of_turn_movement)
			{
				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				float total_delay_time = 0.0f;
				for (int iv=0;iv<num_transfer_vehicles_of_turn_movement;iv++)
				{
					_Vehicle_Interface* vehicle=((_Turn_Movement_Interface*)this)->template pull_vehicle<_Vehicle_Interface*>();

					//update vehicle state: transfer to next link
					int enter_time=vehicle->template movement_plan<_Movement_Plan_Interface*>()->template get_current_link_enter_time<int>();
					// set the intersection delay experience by the vehicle
					//vehicle->template movement_plan<_Movement_Plan_Interface*>()->template set_current_link_intersection_delay<int>(_outbound_link_arrived_time_based_experienced_link_turn_travel_delay);

					//int delayed_time = max(0, int((((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() - enter_time) - ((_Link_Interface*)_inbound_link)->template link_fftt<float>()));
					//int delayed_time = max(0, (int)((float)iteration() - (float)enter_time - ((_Link_Interface*)_inbound_link)->template link_fftt<float>()));
					int delayed_time = max(0,    (int)ceil(((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<float>() - (float)enter_time - ((_Link_Interface*)_inbound_link)->template link_fftt<float>())   );
					int enter_interval_index = enter_time / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					int delayed_interval = current_simulation_interval_index - enter_interval_index;

					//TODO: Omer: 2018.01.25 added for time-dependent reporting by entry time
					//----------------------------------------------------------------------------------
					int assignment_index = enter_time / (((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());
					
					_vehicles_processed_by_entry_time[assignment_index]++;
					_turn_delay_by_entry_time[assignment_index] += delayed_time;
					//----------------------------------------------------------------------------------

					//update inbound link state: N(a',L,t)
					((_Link_Interface*)_inbound_link)->template link_downstream_cumulative_vehicles<int&>()++;
					((_Link_Interface*)_inbound_link)->template link_downstream_departed_vehicles<int&>()++;
					((_Link_Interface*)_inbound_link)->template cacc_count<int&>()--;

					((_Link_Interface*)_outbound_link)->template link_upstream_arrived_vehicles<int&>()++;
					((_Link_Interface*)_outbound_link)->template link_upstream_cumulative_arrived_vehicles<int&>()++;
					((_Link_Interface*)_outbound_link)->template cacc_count<int&>()++;

					//update link_turn_travel_delay
					total_delay_time += delayed_time;

					//vehicle->template transfer_to_link<_Link_Interface*>((_Link_Interface*)_outbound_link);
					if (((_Scenario_Interface*)_global_scenario)->template vehicle_taking_action<bool>())
					{
						vehicle->template make_suggestion<Vehicle_Components::Types::Type_Vehicle_Action_keys>(Vehicle_Components::Types::MOVE_TO_NEXT_LINK);
					}
					else
					{
						vehicle->template move_to_next_link<NT>();
					}
					//vehicle->template distance_to_stop_bar<float>(((_Link_Interface*)_outbound_link)->template length<float>());
					//((_Link_Interface*)_outbound_link)->template push_vehicle<_Vehicle_Interface*>(vehicle);
				}
				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = total_delay_time / (float)num_transfer_vehicles_of_turn_movement;
			}


			template<typename TargetType> void update_state()
			{

				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				int t_cached_delay = -1;
				if (current_simulation_interval_index>0)
				{
					t_cached_delay = (current_simulation_interval_index)%((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();
				}
				else
				{
					t_cached_delay = 0;
				}

				//=========================================================================================================
				//TODO: Omer - we are removing the add signal penalty as it should already be accounted for from simulation
				//JOSH: only do this when there is no observed delay on the link, so that router has some basis for assuming the link performance.  Added a new 'sign_penalty' function, which just assumes 6 second delay at intersection
				_Link_Interface* lnk = (_Link_Interface*)_inbound_link;
				if (_outbound_link_arrived_time_based_experienced_link_turn_travel_delay == 0 && _movement_demand == 0)
				{				
					typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;
					_Intersection_Interface* itx = lnk->template downstream_intersection<_Intersection_Interface*>();
					if (itx->intersection_type<Intersection_Components::Types::Intersection_Type_Keys>() == Intersection_Components::Types::PRE_TIMED_SIGNAL_CONTROL ||
						itx->intersection_type<Intersection_Components::Types::Intersection_Type_Keys>() == Intersection_Components::Types::ACTUATED_SIGNAL_CONTROL ||
						itx->intersection_type<Intersection_Components::Types::Intersection_Type_Keys>() == Intersection_Components::Types::ADAPTIVE_SIGNAL_CONTROL)
					{
						add_signal_penalty<TargetType>();
					}
					if (itx->intersection_type<Intersection_Components::Types::Intersection_Type_Keys>() == Intersection_Components::Types::TWO_WAY_STOP_SIGN ||
						itx->intersection_type<Intersection_Components::Types::Intersection_Type_Keys>() == Intersection_Components::Types::ALL_WAY_STOP_SIGN)
					{
						add_sign_penalty<TargetType>();
					}
				}

				//TODO: Omer: 2018.02.20
				/*if (t_cached_delay>0)
				{ 
					_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = max(_outbound_link_arrived_time_based_experienced_link_turn_travel_delay, _cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[t_cached_delay-1] - ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>());
				}*/

				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = max(_outbound_link_arrived_time_based_experienced_link_turn_travel_delay, _movement_demand * 2.0f);

				// if the current probe vehicle travel time estimate (_outbound_link_arrived....) is greater than the assignment update period, use the delay based on the turn flow rate instead.
				if (movement_moe_data.movement_flow_rate_prev > 0 && _outbound_link_arrived_time_based_experienced_link_turn_travel_delay > ((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>()* ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())
				{
					float estimated_per_vehicle_delay = 3600.0f / movement_moe_data.movement_flow_rate_prev;
					_outbound_link_arrived_time_based_experienced_link_turn_travel_delay =_movement_demand*estimated_per_vehicle_delay;
				}

				//------------------------------------------------------------------------------
				_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[t_cached_delay] = _outbound_link_arrived_time_based_experienced_link_turn_travel_delay;
				//------------------------------------------------------------------------------				

				if (((_Scenario_Interface*)_global_scenario)->template use_realtime_travel_time_for_enroute_switching<bool>())
				{
					realtime_forward_link_turn_travel_time<float>(((_Link_Interface*)_inbound_link)->template link_fftt<float>()+_outbound_link_arrived_time_based_experienced_link_turn_travel_delay);
				}

				//==============================================================================================================
				// UPDATE THE ASSIGNMENT INTERVAL TRAVEL DELAY
				//TODO: Check - JA, 01/25/18 restored the update to only occur on assignment intervals.....
				//if (((current_simulation_interval_index+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				//{	
					_turn_travel_penalty = _outbound_link_arrived_time_based_experienced_link_turn_travel_delay;

					
					//==============================================================================================================
					forward_link_turn_travel_time<float>(((_Link_Interface*)_inbound_link)->template link_fftt<float>()+_turn_travel_penalty);
				//}
			}

            template<typename TargetType> void add_signal_penalty()
            {
				typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;
				typedef Intersection_Control_Components::Prototypes::Intersection_Control<_Intersection_Interface::get_type_of(intersection_control)> _Intersection_Control_Interface;
				typedef Intersection_Control_Components::Prototypes::Control_Plan<_Intersection_Control_Interface::get_type_of(current_control_plan)> _Interscetion_Control_Plan_Interface;
				
				_Link_Interface* lnk = (_Link_Interface*)_inbound_link;		
				_Intersection_Interface* itx = lnk->template downstream_intersection<_Intersection_Interface*>();
				_Intersection_Control_Interface* signal = itx->intersection_control<_Intersection_Control_Interface*>();
				_Interscetion_Control_Plan_Interface* signal_control_plan = signal->current_control_plan<_Interscetion_Control_Plan_Interface*>();

                int outbound_link_type = ((_Link_Interface*)_outbound_link)->template link_type<int>();
                int inbound_link_type = ((_Link_Interface*)_inbound_link)->template link_type<int>();
                bool eligible_for_signal_penalty = false;
				float signal_control_penalty = 0.0;
                if (inbound_link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || inbound_link_type == Link_Components::Types::Link_Type_Keys::LOCAL)
				{
                    eligible_for_signal_penalty = true;
				}
                else
				{
                    if (inbound_link_type == Link_Components::Types::Link_Type_Keys::ON_RAMP && (outbound_link_type != Link_Components::Types::Link_Type_Keys::FREEWAY && outbound_link_type != Link_Components::Types::Link_Type_Keys::EXPRESSWAY))
					{
                        eligible_for_signal_penalty = true;
					}
                    else
					{
                        eligible_for_signal_penalty = false;
					}
				}

				if (!eligible_for_signal_penalty)
				{
                    signal_control_penalty = 0.0;
				}
                else
                {
                    //float vc = ((_link_component_type*)_inbound_link)->link_moe_data.link_out_flow_ratio;
					float vc = ((typename MasterType::link_type*)_inbound_link)->link_moe_data.link_out_flow_ratio;

					//float vc = 1.0f;


                    float cycle= signal_control_plan->cycle_length<int>(); // 75 seconds

                    /*if (_movement_type == Turn_Movement_Components::Types::LEFT_TURN) green=5;
                    else green=30;*/

                    float Du=6.8 * vc - 0.39 * _green_time + 0.35 * cycle - 4.5;
                    Du = max(Du, 0.0f);
                    float Di = 2.7 * pow(vc,8) - 7.3 * (_green_time / cycle) + 3.4;
                    Di = max(Di, 0.0f);
                    signal_control_penalty = max(8.0f, Du + Di);
					//if(((_link_component_type*)_inbound_link)->template uuid<int>() == 104) cout << _movement_transferred << endl;
                }

				//TODO: JA - 18-2-15: add the additional turn penalty from the signal to the experienced travel time (this is only called when no cars have experienced the turn movement, so it will always be 0 before adding the penalty
				//_turn_travel_penalty += signal_control_penalty;
				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay += signal_control_penalty;

            }

			template<typename TargetType> void add_sign_penalty()
			{
				typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;
				typedef Intersection_Control_Components::Prototypes::Intersection_Control<_Intersection_Interface::get_type_of(intersection_control)> _Intersection_Control_Interface;
				typedef Intersection_Control_Components::Prototypes::Control_Plan<_Intersection_Control_Interface::get_type_of(current_control_plan)> _Interscetion_Control_Plan_Interface;

				_Link_Interface* lnk = (_Link_Interface*)_inbound_link;
				_Intersection_Interface* itx = lnk->template downstream_intersection<_Intersection_Interface*>();
				_Intersection_Control_Interface* signal = itx->intersection_control<_Intersection_Control_Interface*>();
				_Interscetion_Control_Plan_Interface* signal_control_plan = signal->current_control_plan<_Interscetion_Control_Plan_Interface*>();

				int outbound_link_type = ((_Link_Interface*)_outbound_link)->template link_type<int>();
				int inbound_link_type = ((_Link_Interface*)_inbound_link)->template link_type<int>();
				bool eligible_for_signal_penalty = false;
				float signal_control_penalty = 0.0;
				if (inbound_link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || inbound_link_type == Link_Components::Types::Link_Type_Keys::LOCAL)
				{
					eligible_for_signal_penalty = true;
				}
				else
				{
					if (inbound_link_type == Link_Components::Types::Link_Type_Keys::ON_RAMP && (outbound_link_type != Link_Components::Types::Link_Type_Keys::FREEWAY && outbound_link_type != Link_Components::Types::Link_Type_Keys::EXPRESSWAY))
					{
						eligible_for_signal_penalty = true;
					}
					else
					{
						eligible_for_signal_penalty = false;
					}
				}

				if (!eligible_for_signal_penalty)
				{
					signal_control_penalty = 0.0;
				}
				else
				{
					//float vc = ((_link_component_type*)_inbound_link)->link_moe_data.link_out_flow_ratio;
					float vc = ((typename MasterType::link_type*)_inbound_link)->link_moe_data.link_out_flow_ratio;

					//float vc = 1.0f;


					float cycle = signal_control_plan->cycle_length<int>(); // 75 seconds

																			/*if (_movement_type == Turn_Movement_Components::Types::LEFT_TURN) green=5;
																			else green=30;*/

					float Du = 6.8 * vc - 0.39 * _green_time + 0.35 * cycle - 4.5;
					Du = max(Du, 0.0f);
					float Di = 2.7 * pow(vc, 8) - 7.3 * (_green_time / cycle) + 3.4;
					Di = max(Di, 0.0f);
					signal_control_penalty = max(8.0f, Du + Di);
					//if(((_link_component_type*)_inbound_link)->template uuid<int>() == 104) cout << _movement_transferred << endl;
				}

				//TODO: JA - 18-2-15: add the additional turn penalty from the signal to the experienced travel time (this is only called when no cars have experienced the turn movement, so it will always be 0 before adding the penalty
				//_turn_travel_penalty += signal_control_penalty;
				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay += signal_control_penalty;
			}


			template<typename TargetType> void accept_vehicle(void* vehicle)
			{

				LOCK(_mvmt_lock);

				vehicles_container<_Vehicles_Container_Interface&>().push_back((_Vehicle_Interface*)vehicle);
				
				UNLOCK(_mvmt_lock);
			}

			template<typename TargetType> void initialize_features()
			{
				_movement_capacity = 0.0;
				_movement_demand = 0.0;
				_movement_supply = 0.0;
				_movement_flow = 0.0;
				_movement_capacity_leftover = 0.0;
				_movement_transferred = 0;

				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = 0.0;

				_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array.clear();
				_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array.resize(((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());

				//TODO: Omer: 2018.01.25 added for time-dependent reporting by entry time
				//----------------------------------------------------------------------------------
				int num_of_assignment_intervals_in_a_day = (int)((float)((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals<int>() / (float)((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());

				_vehicles_processed_by_entry_time.clear();
				_vehicles_processed_by_entry_time.resize(num_of_assignment_intervals_in_a_day);

				_turn_delay_by_entry_time.clear();
				_turn_delay_by_entry_time.resize(num_of_assignment_intervals_in_a_day);

				_add_delay_by_entry_time.clear();
				_add_delay_by_entry_time.resize(num_of_assignment_intervals_in_a_day);
				//----------------------------------------------------------------------------------

				int j;

				for (j=0;j<((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();j++)
				{
					_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[j] = 0.0;

				}

				//TODO: Omer: 2018.01.25 added for time-dependent reporting by entry time
				//----------------------------------------------------------------------------------
				for (j = 0; j < num_of_assignment_intervals_in_a_day; j++)
				{
					_vehicles_processed_by_entry_time[j] = 0;
					_turn_delay_by_entry_time[j] = 0.0;
					_add_delay_by_entry_time[j] = 0.0;
				}
				//----------------------------------------------------------------------------------

				if (_movement_type == Turn_Movement_Components::Types::THROUGH_TURN)
				{
					_minimum_merge_rate= 1.0;
				}
				else
				{
					_minimum_merge_rate = 0.2;
				}

				initialize_moe();
			}

			void initialize_moe()
			{
				movement_moe_data.inbound_link_turn_time = 0.0f;
				movement_moe_data.outbound_link_turn_time = 0.0f;
				movement_moe_data.turn_penalty = 0.0f;
				movement_moe_data.turn_penalty_standard_deviation = 0.0f;
				movement_moe_data.movement_flow_rate_prev = movement_moe_data.movement_flow_rate; //TODO: Omer
				movement_moe_data.intervals_with_demand = 0.0f; //TODO: Omer
				movement_moe_data.movement_flow_rate = 0.0f;
			}

			template<typename TargetType> void calculate_moe_for_simulation_interval_from_outbound_link()
			{
				realtime_movement_moe_data.turn_penalty = _outbound_link_arrived_time_based_experienced_link_turn_travel_delay / 60.0f;
				realtime_movement_moe_data.movement_flow_rate = (float)_movement_transferred;			
				((_link_component_type*)_outbound_link)->realtime_link_moe_data.link_in_volume += (float)_movement_transferred;

				//TODO: Omer
				if (_movement_demand > 0)
				{
					movement_moe_data.intervals_with_demand++;
				}
				//------------------------------------------------

				movement_moe_data.movement_flow_rate += (float)_movement_transferred;
				
				
				((_link_component_type*)_outbound_link)->link_moe_data.link_in_volume += (float)_movement_transferred;

				realtime_movement_moe_data.outbound_link_turn_time = _outbound_link_arrived_time_based_experienced_link_turn_travel_delay / 60.0f + ((_link_component_type*)_outbound_link)->link_fftt<float>() / 60.0f;
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval_from_outbound_link()
			{
				float _tmp_turn_travel_penalty;
				float _tmp_turn_travel_penalty_standard_divation;
				//TODO: Omer replacing this with what the route is really seeing
				/*::calculate_mean_standard_deviation
					(_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array,
					_tmp_turn_travel_penalty,
					_tmp_turn_travel_penalty_standard_divation);
				movement_moe_data.turn_penalty = _tmp_turn_travel_penalty / 60.0f;
				movement_moe_data.turn_penalty_standard_deviation = _tmp_turn_travel_penalty_standard_divation / 60.0f;*/				
				//TODO: Omer Change the above back to the old one ASAP
				_tmp_turn_travel_penalty = _outbound_link_arrived_time_based_experienced_link_turn_travel_delay;
				_tmp_turn_travel_penalty_standard_divation = 0.0f;
				//--------------------------------------------------------------------------------------------------------------------------------------------------------
				movement_moe_data.turn_penalty = _tmp_turn_travel_penalty / 60.0f;
				movement_moe_data.turn_penalty_standard_deviation = _tmp_turn_travel_penalty_standard_divation / 60.0f;
				//-------------------------------------------------------------------------------------------------------------------------------------------------------
				
				movement_moe_data.outbound_link_turn_time = (((_link_component_type*)_outbound_link)->link_fftt<float>() + _tmp_turn_travel_penalty) / 60.0f;

				//TODO:Omer
				movement_moe_data.movement_flow_rate = movement_moe_data.movement_flow_rate * 3600.0f / (movement_moe_data.intervals_with_demand * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>());
				//movement_moe_data.movement_flow_rate = movement_moe_data.movement_flow_rate * 3600.0f / (((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>());

				//------------------------------------------------------------------------------------------------------------------
			}

			template<typename TargetType> void calculate_moe_for_simulation_interval_from_inbound_link()
			{
				typedef typename MasterType::link_type _link_component_type;
				((_link_component_type*)_inbound_link)->realtime_link_moe_data.link_out_volume += (float)_movement_transferred;
				((_link_component_type*)_inbound_link)->link_moe_data.link_out_volume += (float)_movement_transferred;

				realtime_movement_moe_data.inbound_link_turn_time = _outbound_link_arrived_time_based_experienced_link_turn_travel_delay / 60.0f + ((_link_component_type*)_inbound_link)->link_fftt<float>() / 60.0f;
				
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval_from_inbound_link()
			{
				movement_moe_data.inbound_link_turn_time = (((_link_component_type*)_inbound_link)->link_fftt<float>()/60.0f + movement_moe_data.turn_penalty);
			}

			//TODO: Omer: 2018.01.25 added for time-dependent reporting by entry time
			//----------------------------------------------------------------------------------
			template<typename TargetType> void calculate_moe_for_assignment_interval_from_outbound_link_end()
			{
				int num_of_assignment_intervals_in_a_day = (int)((float)((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals<int>() / (float)((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());

				/*for (int j = 1; j < num_of_assignment_intervals_in_a_day; j++)
				{
					if (_add_delay_by_entry_time[j] > 0)
					{
						_add_delay_by_entry_time[j] += _add_delay_by_entry_time[j - 1];
					}
				}*/
				
				for (int j = 0; j < num_of_assignment_intervals_in_a_day; j++)
				{
					if (_vehicles_processed_by_entry_time[j] > 0)
					{
						_turn_delay_by_entry_time[j] = _turn_delay_by_entry_time[j] / _vehicles_processed_by_entry_time[j];
						/*_turn_delay_by_entry_time[j] += std::min(_turn_delay_by_entry_time[j],_add_delay_by_entry_time[j]);*/
						_turn_delay_by_entry_time[j] += _add_delay_by_entry_time[j];

						if (j > 0)
						{						
							_turn_delay_by_entry_time[j] = std::max(_turn_delay_by_entry_time[j], _turn_delay_by_entry_time[j - 1] - (float)((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>()*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>());
						}

						_turn_delay_by_entry_time[j] = std::max(6.0f, _turn_delay_by_entry_time[j]);
					}
					else
					{
						_turn_delay_by_entry_time[j] = _add_delay_by_entry_time[j];
						
						if (j > 0)
						{
							_turn_delay_by_entry_time[j] = std::max(_turn_delay_by_entry_time[j], _turn_delay_by_entry_time[j - 1] - (float)((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>()*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>());
						}

						_turn_delay_by_entry_time[j] = std::max(6.0f, _turn_delay_by_entry_time[j]);
					}

				}
			}
			//----------------------------------------------------------------------------------

		};
	}
}

