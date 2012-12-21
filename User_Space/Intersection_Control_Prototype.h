#pragma once
#include "User_Space_Includes.h"

namespace Intersection_Control_Components
{
	namespace Types
	{
			enum Movement_Priority_Type_Keys
			{
				PROTECTED=1,  //high priority
				PERMITTED,    //yield the right-of-way to conflicting movements
			};
			enum Approach_Type_Keys
			{
				MAJOR_APPROACH=1,
				MINOR_APPROACH,
			};
			enum Intersection_Type_Keys
			{
				NO_CONTROL=0,
				YIELD_SIGN,
				ALL_WAY_STOP_SIGN,
				TWO_WAY_STOP_SIGN,
				PRE_TIMED_SIGNAL_CONTROL, 
				ACTUATED_SIGNAL_CONTROL, 
				ADAPTIVE_SIGNAL_CONTROL,
			};

	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Phase_Movement_Prototype
		{
			tag_as_prototype;

			feature_accessor(movement, none, none);
			feature_accessor(movement_priority_type, none, none);
		};

		prototype struct Phase_Prototype
		{
			tag_as_prototype;

			feature_accessor(phase_index, none, none);	// the index counter on all phases in the network
			feature_accessor(phase_sequence_number, none, none);	// 
				
			//starting time of green, yellow, and red
			feature_accessor(green_starting_time, none, none);
			feature_accessor(yellow_starting_time, none, none);
			feature_accessor(red_start_time, none, none);

			//actuated signal control
			feature_accessor(maximum_green_time, none, none);
			feature_accessor(minimum_green_time, none, none);

			//pre-timed signal control
			feature_accessor(offset, none, none);
			feature_accessor(green_time, none, none);

			//clearence time
			feature_accessor(yellow_time, none, none);
			feature_accessor(all_red_time, none, none);

			//adjusted green and yellow time
			feature_accessor(adjusted_green_time, none, none);
			feature_accessor(adjusted_yellow_time, none, none);

			//adjusted max and min green
			feature_accessor(adjusted_maximum_green_time, none, none);
			feature_accessor(adjusted_minimum_green_time, none, none);

			//movements
			feature_accessor(turn_movements_in_the_phase_array, none, none);
		};

		prototype struct Approach_Prototype
		{
			tag_as_prototype;
			
			feature_accessor(approach_index, none, none);
			feature_accessor(approach_type, none, none);
			feature_accessor(inbound_link, none, none);
			feature_accessor(green_cycle_ratio, none, none);
		};

		prototype struct Control_Plan_Prototype
		{
			tag_as_prototype;

			feature_accessor(control_plan_index, none, none);

			//duration
			feature_accessor(starting_time, none, none);
			feature_accessor(ending_time, none, none);
				
			//type
			feature_accessor(control_type, none, none);

			//timing plan for signal control nodes
			feature_accessor(offset, none, none);
			feature_accessor(cycle_length, none, none);

			feature_accessor(cycle_index, none, none);
			feature_accessor(cycle_starting_time, none, none);
			feature_accessor(cycle_ending_time, none, none);
			feature_accessor(cycle_leftover_time, none, none);

			feature_accessor(phase_data_array, none, none);

			//approach data
			feature_accessor(approach_data_array, none, none);

			//two-way stop and yield sign
			feature_accessor(major_approach_data_array, none, none);

			feature_accessor(minor_approach_data_array, none, none);
		};

		prototype struct Intersection_Control_Prototype
		{
			tag_as_prototype;

			feature_accessor(intersection, none, none);
			feature_accessor(current_control_plan, none, none);
			feature_accessor(control_plan_data_array, none, none);
			
			feature_prototype void set_node_current_control_plan_index(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);

				int current_time = ((_Scenario_Interface*)scenario)->template current_time<long>()%(24*60*60);

				Component_Type* this_monitor=this_component();

				typename _Control_Plans_Container_Interface::Component_Type* monitor=control_plan_data_array<_Control_Plans_Container_Interface::Component_Type*>();


				for (int i=0;i<(int)control_plan_data_array<_Control_Plans_Container_Interface&>().size();i++)
				{
					_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)control_plan_data_array<_Control_Plans_Container_Interface&>()[i];
					int current_control_plan_start_time = control_plan->template starting_time<int>();
					int current_control_plan_end_time = control_plan->template ending_time<int>();
					if (current_time >=current_control_plan_start_time && current_time<current_control_plan_end_time)
					{
						current_control_plan<_Control_Plan_Interface*>(control_plan);
						break;
					}
				}
			}

			feature_prototype void intersection_control_update(void* scenario)
			{

				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				int control_type = current_control_plan<_Control_Plan_Interface*>()->template control_type<int>();

				switch(control_type)
				{
					case NO_CONTROL:
						this->template no_control_state_update<NULLTYPE>(scenario);
						break;
					case YIELD_SIGN:
						this->template yield_sign_control_state_update<NULLTYPE>(scenario);
						break;
					case ALL_WAY_STOP_SIGN:
						this->template all_way_stop_control_state_update<NULLTYPE>(scenario);
						break;
					case TWO_WAY_STOP_SIGN:
						this->template two_way_stop_control_state_update<NULLTYPE>(scenario);
						break;
					case PRE_TIMED_SIGNAL_CONTROL:
						this->template pre_timed_signal_control_state_update<NULLTYPE>(scenario);
						break;
					case ACTUATED_SIGNAL_CONTROL:
						this->template actuated_signal_control_state_update<NULLTYPE>(scenario);
						break;
					case ADAPTIVE_SIGNAL_CONTROL:
						this->template adaptive_signal_control_state_update<NULLTYPE>(scenario);
						break;
				}
			}
			
			feature_prototype void no_control_state_update(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);
	
				int t_start = current_time;
				int t_end = t_start + ((_Scenario_Interface*)scenario)->simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();



				// calculate green time
				this->template calculate_turn_movement_green_time_no_control<NULLTYPE>(scenario);

				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					//advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};
			feature_prototype void calculate_turn_movement_green_time_no_control(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface,get_type_of(intersection),Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Movements_Container_Interface, _Inbound_Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);
				
				//green times for each movement are assumed to be the length of the simulation interval
				_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection<_Intersection_Interface*>()->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin();outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						_Inbound_Movement_Interface* inbound_movement = (_Inbound_Movement_Interface*)(*inbound_movement_itr);

						//turn movement green time
						//allowed
						if (inbound_movement->template movement_rule<int>() == ALLOWED)
						{
							inbound_movement->template green_time<float>(((_Scenario_Interface*)scenario)->template simulation_interval_length<int>()*1.0f);
						}

						//prihibited
						if (inbound_movement->template movement_rule<int>() == PROHIBITED)
						{
							inbound_movement->template green_time<float>(0.0);
						}

						//merge priority
						inbound_movement->template merge_priority<int>(0.0);
					}
				}
			};

			feature_prototype void calculate_turn_movement_green_time_yield_control(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface,get_type_of(intersection),Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Movements_Container_Interface, _Inbound_Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Approaches_Container_Interface, _Approach_Interface, _Control_Plan_Interface::get_type_of(approach_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Approach_Prototype, ComponentType);
				define_component_interface(_Link_Interface,_Approach_Interface::get_type_of(inbound_link),Link_Components::Prototypes::Link_Prototype, ComponentType);

				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);

				//green times for each movement are assumed to be the length of the simulation interval
				_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection<_Intersection_Interface*>()->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin();outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						_Inbound_Movement_Interface* inbound_movement = (_Inbound_Movement_Interface*)(*inbound_movement_itr);
						_Link_Interface* inbound_link = inbound_movement->template inbound_link<_Link_Interface*>();
						//turn movement green time
						//allowed
						if (inbound_movement->template movement_rule<int>() == ALLOWED)
						{
							inbound_movement->template green_time<float>(((_Scenario_Interface*)scenario)->template simulation_interval_length<int>()*1.0f);
						}

						//prihibited
						if (inbound_movement->template movement_rule<int>() == PROHIBITED)
						{
							inbound_movement->template green_time<float>(0.0);
						}

						//merge priority
						_Approaches_Container_Interface& approaches_container = current_control_plan<_Control_Plan_Interface*>()->template approach_data_array<_Approaches_Container_Interface&>();
						for (int approach_index=0;approach_index<(int)approaches_container.size();approach_index++)
						{
							_Approach_Interface* approach = approaches_container[approach_index];
							int approach_link_index = approach->template inbound_link<_Link_Interface*>()->template internal_id<int>();
							if (approach_link_index == inbound_link->template internal_id<int>())
							{
								if (approach->template approach_type<int>() == MAJOR_APPROACH)
								{
									inbound_movement->template merge_priority<int>(1);
								}
								else
								{
									inbound_movement->template merge_priority<int>(0.0);
								}

								break;
							}
						}

						//highway : mainline and ramp
						if (inbound_link->template link_type<int>() == FREEWAY || inbound_link-> template link_type<int>() == FREEWAY || EXPRESSWAY)
						{
							inbound_movement->template merge_priority<int>(1);
						}

						if (inbound_link->template link_type<int>() == ON_RAMP)
						{
							inbound_movement->template merge_priority<int>(0.0);
						}
					}
				}
			};

			feature_prototype void yield_sign_control_state_update(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);


				int t_start = current_time;
				int t_end = t_start + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();


				//calculate green time
				this->template calculate_turn_movement_green_time_yield_control<NULLTYPE>(scenario);

				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					//advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			feature_prototype void all_way_stop_control_state_update(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);

				int t_start = current_time;
				int t_end = t_start + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				//calculate green time
				this->template calculate_turn_movement_green_time_no_control<NULLTYPE>(scenario);

				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					//advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			feature_prototype void two_way_stop_control_state_update(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);

				int t_start = current_time;
				int t_end = t_start + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				// calculate green time
				this->template calculate_turn_movement_green_time_yield_control<NULLTYPE>(scenario);

				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					// advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			feature_prototype void pre_timed_signal_control_state_update(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);

				int t_start = current_time;
				int t_end = t_start + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				//start of the control plan
				if (t_start == starting_time)
				{
					//calcualte starting times of green, yellow, and red of each phase; green_cycle_ratio
					this->template calculate_green_yellow_red_starting_times_and_green_cycle_ratio<NULLTYPE>(scenario);
				}

				//calculate green time
				this->template initialize_node_turn_movement_green_time<NULLTYPE>();
				this->template calculate_turn_movement_green_time<NULLTYPE>(scenario);

				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					// advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			feature_prototype void actuated_signal_control_state_update(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				int current_time = ((_Scenario_Interface*)scenario)->current_time<long>()%(24*60*60);

				int t_start = current_time;
				int t_end = t_start + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				//start of the control plan
				if (t_start == starting_time)
				{
					//calcualte starting times of green, yellow, and red of each phase; green_cycle_ratio
					this->template calculate_green_yellow_red_starting_times_and_green_cycle_ratio<NULLTYPE>(scenario);
				}

				//change signal parameters in response to demand
				this->template actuated_signal_control_response<NULLTYPE>(scenario);


				//calculate green time
				this->template initialize_node_turn_movement_green_time<NULLTYPE>();
				this->template calculate_turn_movement_green_time<NULLTYPE>(scenario);


				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					//advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			feature_prototype void actuated_signal_control_response(void* scenario)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Phases_Container_Interface, _Phase_Interface, _Control_Plan_Interface::get_type_of(phase_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Prototype, ComponentType);
				define_container_and_value_interface(_Phase_Movements_Container_Interface, _Phase_Movement_Interface, _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Movement_Prototype, ComponentType);
				define_component_interface(_Movement_Interface, _Phase_Movement_Interface::get_type_of(movement), Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Movement_Interface::get_type_of(inbound_link), Link_Components::Prototypes::Link_Prototype, ComponentType);

				int current_time = ((_Scenario_Interface*)scenario)->template current_time<long>()%(24*60*60);

				int num_phases = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>().size();
				int last_phase = num_phases - 1;
				int cycle_length = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template red_start_time<int>() + 
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template all_red_time<int>();

				int cycle_leftover_time = current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>();

				int t_start = cycle_length - cycle_leftover_time;
	
				if (t_start >=current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template yellow_starting_time<int>())
				{//end of last phase
					return;
				}

				int t_end = t_start + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>(); // in second
				int t_start_next;
				int new_cycle_leftover_time;

				int next_cycle_leftover_time = cycle_leftover_time - ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
	
				if (next_cycle_leftover_time>=0)
				{//same cycle
					t_start_next = t_start;
				}
				else
				{//different cycle
					t_start_next = t_end;
				}

				float lost_time = 0.0f;
				vector<_Link_Interface*> critical_link_array; 
				vector<int> critical_link_queue_length_array;
				vector<int> critical_link_discharge_time_array;

				critical_link_array.resize(num_phases);
				critical_link_queue_length_array.resize(num_phases);
				critical_link_discharge_time_array.resize(num_phases);

				int current_phase = -1;
				for (int iphase=0;iphase<num_phases;iphase++){
					//find critical link, which is the link with the maximum length of vehicle queue during each phase
					critical_link_queue_length_array[iphase] = 0;
					_Phase_Movements_Container_Interface& phase_movements = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>();
					for (int turn_movement=0;turn_movement< phase_movements.size();turn_movement++)
					{
						_Movement_Interface* movement = phase_movements[turn_movement]->template movement<_Movement_Interface*>();
						_Link_Interface* inbound_link = movement->template inbound_link<_Link_Interface*>();;
						if (critical_link_queue_length_array[iphase]<=inbound_link->template link_num_vehicles_in_queue<int>())
						{
							critical_link_queue_length_array[iphase] = inbound_link->template link_num_vehicles_in_queue<int>();
							critical_link_array[iphase] = inbound_link;
						}
					}

					//determine the extenstion of the green time to discharge the maximum queue
					float maximum_flow_rate_per_second = critical_link_array[iphase]->template maximum_flow_rate<float>() * critical_link_array[iphase]->template num_lanes<int>() / 3600.0f;
					critical_link_discharge_time_array[iphase] = (int) ((critical_link_queue_length_array[iphase] * 1.0f) / maximum_flow_rate_per_second);

					//find current phase index
					int yellow_starting_time = 0;
					if (current_phase == -1)
					{
						yellow_starting_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>();
						if (t_start<=yellow_starting_time)
						{
							current_phase = iphase;
						}
					}
				}

				//only apply it to current phase
				int current_critical_link_discharge_time = critical_link_discharge_time_array[current_phase];
				int current_yellow_starting_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template yellow_starting_time<int>();
				int current_green_starting_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template green_starting_time<int>();
				int maximum_green_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template adjusted_maximum_green_time<int>();
				int minimum_green_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template adjusted_minimum_green_time<int>();
				int extended_green_ending_time = t_start + current_critical_link_discharge_time;

				if (extended_green_ending_time>=current_yellow_starting_time)
				{
					//check for max_green
					if ((current_yellow_starting_time + current_critical_link_discharge_time - current_green_starting_time) > maximum_green_time)
					{
						current_critical_link_discharge_time = maximum_green_time + current_green_starting_time - current_yellow_starting_time;
					}
		
					//check for min_green 
					if ((current_yellow_starting_time + current_critical_link_discharge_time - current_green_starting_time) < minimum_green_time)
					{
						current_critical_link_discharge_time = minimum_green_time + current_green_starting_time - current_yellow_starting_time;
					}
				}
				else
				{//
					current_critical_link_discharge_time = extended_green_ending_time - current_yellow_starting_time;
					if (current_critical_link_discharge_time + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>() <= 0)
					{
						current_critical_link_discharge_time = 0 - ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
					}

					//check for min_green 
					if ((current_yellow_starting_time + current_critical_link_discharge_time - current_green_starting_time) < minimum_green_time)
					{
						current_critical_link_discharge_time = minimum_green_time + current_green_starting_time - current_yellow_starting_time;
					}

					//check for max_green
					if ((current_yellow_starting_time + current_critical_link_discharge_time - current_green_starting_time) > maximum_green_time)
					{
						current_critical_link_discharge_time = maximum_green_time + current_green_starting_time - current_yellow_starting_time;
					}
				}

				if (current_critical_link_discharge_time != 0)
				{
					//redefine the starting and ending of green time for all consequent phases
			
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template yellow_starting_time<int>(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template yellow_starting_time<int>() + current_critical_link_discharge_time);
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template red_start_time<int>(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template red_start_time<int>() + current_critical_link_discharge_time);
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template adjusted_green_time<int>( 
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template yellow_starting_time<int>() -
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[current_phase]->template green_starting_time<int>());

					for (int iphase=current_phase+1;iphase<num_phases;iphase++)
					{
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>() + current_critical_link_discharge_time);
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>() + current_critical_link_discharge_time);
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template red_start_time<int>(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template red_start_time<int>() + current_critical_link_discharge_time);
					}

					current_control_plan<_Control_Plan_Interface*>()->template cycle_ending_time<int&>() += current_critical_link_discharge_time;
			
					cycle_length = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template red_start_time<int>() + 
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template all_red_time<int>();
			
					new_cycle_leftover_time = cycle_leftover_time + current_critical_link_discharge_time;
					current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>(new_cycle_leftover_time);
					current_control_plan<_Control_Plan_Interface*>()->template cycle_length<int>(cycle_length);
				}
			};

			feature_prototype void adaptive_signal_control_state_update(void* scenario)
			{

				this->template no_control_state_update<NULLTYPE>(scenario);
			};

			feature_prototype void calculate_green_yellow_red_starting_times_and_green_cycle_ratio(void* scenario)
			{

				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Phases_Container_Interface, _Phase_Interface, _Control_Plan_Interface::get_type_of(phase_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Prototype, ComponentType);
				define_container_and_value_interface(_Phase_Movements_Container_Interface, _Phase_Movement_Interface, _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Movement_Prototype, ComponentType);
				define_component_interface(_Movement_Interface, _Phase_Movement_Interface::get_type_of(movement), Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Movement_Interface::get_type_of(inbound_link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Approaches_Container_Interface, _Approach_Interface, _Control_Plan_Interface::get_type_of(approach_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Approach_Prototype, ComponentType);

				int current_time = ((_Scenario_Interface*)scenario)->template current_time<long>()%(24*60*60);

				int num_phases = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>().size();

				int last_phase = num_phases - 1;
				
int ayt;
int agt;
int yt, gt;
int agt_max, agt_min, gt_max, gt_min;
				//calculate adjusted green and yellow times
				for (int iphase=0;iphase<num_phases;iphase++)
				{
					if (current_control_plan<_Control_Plan_Interface*>()->template control_type<int>() == PRE_TIMED_SIGNAL_CONTROL)
					{
						//adjusted green and yellow time
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>(
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>() - 
							int(0.5*current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>()));

						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_green_time<int>(
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_time<int>() + 
							(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>()- 
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>()));
					}

					if (current_control_plan<_Control_Plan_Interface*>()->template control_type<int>() == ACTUATED_SIGNAL_CONTROL)
					{

						//adjusted minimum and maximum green time, and yellow time
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>(
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>() - 
							int(0.5*current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>()));
								
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_maximum_green_time<int>(
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template maximum_green_time<int>() + 
							(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>() - 
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>()));

						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_minimum_green_time<int>(
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template minimum_green_time<int>() + 
							(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>() - 
							current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>()));
					}
					ayt = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>();
					agt = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_green_time<int>();
					yt = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_time<int>();
					gt = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_time<int>();
					agt_max = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_maximum_green_time<int>();
					agt_min = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_minimum_green_time<int>();
					gt_max = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template maximum_green_time<int>();
					gt_min = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template minimum_green_time<int>();

				}


				int cycle_length = current_control_plan<_Control_Plan_Interface*>()->template cycle_length<int>();
				if (current_control_plan<_Control_Plan_Interface*>()->template control_type<int>() == ACTUATED_SIGNAL_CONTROL)
				{
					cycle_length = 0;
					for (int iphase=0;iphase<num_phases;iphase++)
					{

						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_green_time<int>( 
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_minimum_green_time<int>());
						cycle_length += current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_green_time<int>();
						cycle_length += current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>();
						cycle_length += current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template all_red_time<int>();
					}
				}
				int offset = current_control_plan<_Control_Plan_Interface*>()->template offset<int>();
				int cycle_index = 0;
				current_control_plan<_Control_Plan_Interface*>()->template cycle_index<int>(cycle_index);

				int cycle_starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int cycle_ending_time = cycle_starting_time + cycle_length;
	
				current_control_plan<_Control_Plan_Interface*>()->template cycle_starting_time<int>(cycle_starting_time);
				current_control_plan<_Control_Plan_Interface*>()->template cycle_ending_time<int>(cycle_ending_time);

				int num_approaches = (int)current_control_plan<_Control_Plan_Interface*>()->template approach_data_array<_Approaches_Container_Interface&>().size();

				//initialize green_cycle_raio
				for (int approach_index = 0;approach_index<num_approaches;approach_index++)
				{
					current_control_plan<_Control_Plan_Interface*>()->template approach_data_array<_Approaches_Container_Interface&>()[approach_index]->template green_cycle_ratio<float>(0.0);
				}

				//
				int cycle_start_time = cycle_length;
				int tmp_time = -1*cycle_length;

				for (int iphase=0;iphase<num_phases;iphase++)
				{
					//green starting time
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>(
						cycle_start_time +
						tmp_time);
			
					//yellow starting time
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>( 
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>() +
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_green_time<int>());
				
					//red starting time
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template red_start_time<int>( 
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>() +
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_yellow_time<int>());
			
					//leftover time
					tmp_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template red_start_time<int>() + 
						current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template all_red_time<int>() - cycle_length;

					//green_cycle_ratio calculation
					int num_turn_movements_in_the_phase = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>().size();
					for (int turn_movement=0;turn_movement<num_turn_movements_in_the_phase;turn_movement++)
					{
						_Phase_Movement_Interface* phase_movement = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>()[turn_movement];
						_Movement_Interface* movement = phase_movement->movement<_Movement_Interface*>();
					
						// we need to distinguish protected and permitted here
						_Link_Interface* inbound_link = movement->template inbound_link<_Link_Interface*>();
				
						_Approach_Interface* approach = inbound_link->template approach<_Approach_Interface*>();
						approach->template green_cycle_ratio<float>(approach->template green_cycle_ratio<float>() +
							(current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template adjusted_green_time<int>()*1.0f)/(cycle_length*1.0f));
						//
						movement->template inbound_link_green_cycle_ratio<float>(approach->template green_cycle_ratio<float>());
					}
				}

				current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>(cycle_length);
			};

			feature_prototype void initialize_node_turn_movement_green_time()
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface,get_type_of(intersection),Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Movements_Container_Interface, _Inbound_Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

			
				//green times for each movement are assumed to be the length of the simulation interval
				_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection<_Intersection_Interface*>()->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin();outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						_Inbound_Movement_Interface* inbound_movement = (_Inbound_Movement_Interface*)(*inbound_movement_itr);
						inbound_movement->template green_time<float>(0.0);
					}
				}
			}

			feature_prototype void calculate_turn_movement_green_time(void* scenario)
			{

				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Phases_Container_Interface, _Phase_Interface, _Control_Plan_Interface::get_type_of(phase_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Prototype, ComponentType);
				define_container_and_value_interface(_Phase_Movements_Container_Interface, _Phase_Movement_Interface, _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Movement_Prototype, ComponentType);
				define_component_interface(_Movement_Interface, _Phase_Movement_Interface::get_type_of(movement), Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Movement_Interface::get_type_of(inbound_link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Approaches_Container_Interface, _Approach_Interface, _Control_Plan_Interface::get_type_of(approach_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Approach_Prototype, ComponentType);

				int current_time = ((_Scenario_Interface*)scenario)->template current_time<long>()%(24*60*60);
				int num_phases = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>().size();
				int offset = current_control_plan<_Control_Plan_Interface*>()->template offset<int>();
				int last_phase = num_phases - 1;
				int cycle_length = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template red_start_time<int>() + 
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template all_red_time<int>();
	
				int cycle_leftover_time = current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>();
				int next_cycle_leftover_time = cycle_leftover_time - ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
	
				int t_start = cycle_length - cycle_leftover_time;
				int t_end = t_start + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
				int t_start_next;
	
				if (next_cycle_leftover_time >=0)
				{//same cycle
					t_start_next = t_end;
				}
				else
				{//two cycles
					t_start_next = cycle_length;
				}

				//case 1: same cycle
				for (int iphase=0;iphase<num_phases;iphase++)
				{
					//calculate green in the phase
					int tmp_green_time = 0;
					if (t_start >= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>() &&
						t_start_next <= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>())
					{//same phase - green
						tmp_green_time = t_start_next - t_start;
					}
					else if (t_start <= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>() &&
						t_start_next > current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>())
					{//same phase - yellow
						tmp_green_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>() - t_start;
					}
					else if (t_start < current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>() &&
						t_start_next >= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>())
					{//
						tmp_green_time = t_start_next - current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>();
					}

					if (tmp_green_time>((_Scenario_Interface*)scenario)->template simulation_interval_length<int>())
					{
						tmp_green_time = ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
					}

					if (tmp_green_time < 0.05)
					{
						tmp_green_time = 0;
					}

					//allocate green to each movement in the phase
					for (int turn_movement=0;turn_movement<(int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>().size();turn_movement++)
					{
						_Phase_Movement_Interface* phase_movement = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>()[turn_movement];
						_Movement_Interface* movement = phase_movement->template movement<_Movement_Interface*>();
									
						// we need to distinguish protected and permitted here
						if (phase_movement->template movement_priority_type<int>() == PROTECTED)
						{
							movement->template merge_priority<int>(1);
						}
						else
						{
							movement->template merge_priority<int>(0.0);
						}

						if (iphase>0)
						{
							if (movement->template green_time<float>()>0)
							{
								continue;
							}
						}
						movement->green_time<float&>() +=  tmp_green_time*1.0f;
					}
				}

				if (next_cycle_leftover_time>=0)
				{
					if (next_cycle_leftover_time == 0)
					{
						this->template advance_signal_control_cycle<NULLTYPE>(scenario,next_cycle_leftover_time);
					}
					else
					{
						current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>( 
							current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>() - 
							((_Scenario_Interface*)scenario)->template simulation_interval_length<int>());
					}
				}
				else
				{//end of a cycle - new cycle
					t_start = 0;
					t_start_next = t_end - cycle_length;

					//assign the leftover time
					for (int iphase=0;iphase<num_phases;iphase++)
					{
						//calculate green in the phase
						int tmp_green_time = 0;
						if (t_start >= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>() &&
							t_start_next <= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>())
						{
							tmp_green_time = t_start_next - t_start;
						}
						else if (t_start<= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>() &&
							t_start_next>current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>())
						{
							tmp_green_time = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template yellow_starting_time<int>() - t_start;
						}
						else if (t_start < current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>() &&
							t_start_next >= current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>())
						{
							tmp_green_time = t_start_next - current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template green_starting_time<int>();
						}

						if (tmp_green_time>((_Scenario_Interface*)scenario)->template simulation_interval_length<int>())
						{
							tmp_green_time = ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>();
						}

						if (tmp_green_time < 0.05)
						{
							tmp_green_time = 0;
						}

						//allocate green to each movement in the phase
						for (int turn_movement=0;turn_movement<current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>().size();turn_movement++)
						{
							_Phase_Movement_Interface* phase_movement = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[iphase]->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>()[turn_movement];
							_Movement_Interface* movement = phase_movement->template movement<_Movement_Interface*>();
			
							// we need to distinguish protected and permitted here
							if (iphase>0)
							{
								if (movement->green_time<float>()>0)
								{
									continue;
								}
							}
							movement->green_time<float&>() +=  tmp_green_time*1.0f;
						}
					}

					this->template advance_signal_control_cycle<NULLTYPE>(scenario,next_cycle_leftover_time);
				}
			};

			feature_prototype void advance_signal_control_cycle(void* scenario, int cycle_leftover_time)
			{

				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Phases_Container_Interface, _Phase_Interface, _Control_Plan_Interface::get_type_of(phase_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Prototype, ComponentType);
				define_container_and_value_interface(_Phase_Movements_Container_Interface, _Phase_Movement_Interface, _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Movement_Prototype, ComponentType);
				define_component_interface(_Movement_Interface, _Phase_Movement_Interface::get_type_of(movement), Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Movement_Interface::get_type_of(inbound_link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Approaches_Container_Interface, _Approach_Interface, _Control_Plan_Interface::get_type_of(approach_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Approach_Prototype, ComponentType);

				int current_time = ((_Scenario_Interface*)scenario)->template current_time<long>()%(24*60*60);
				int num_phases = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>().size();


				int last_phase = num_phases - 1;
				int cycle_length = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template red_start_time<int>() + 
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template all_red_time<int>();

				current_control_plan<_Control_Plan_Interface*>()->template cycle_index<int&>()++;
				current_control_plan<_Control_Plan_Interface*>()->template cycle_starting_time<int>(current_time + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>() + cycle_leftover_time);
				current_control_plan<_Control_Plan_Interface*>()->template cycle_ending_time<int>(current_time + ((_Scenario_Interface*)scenario)->template simulation_interval_length<int>() + cycle_leftover_time + cycle_length);
				current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>(cycle_leftover_time + cycle_length);
			};

			
			feature_prototype void advance_control_plan_period()
			{

				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Phases_Container_Interface, _Phase_Interface, _Control_Plan_Interface::get_type_of(phase_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Prototype, ComponentType);
				define_container_and_value_interface(_Phase_Movements_Container_Interface, _Phase_Movement_Interface, _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Movement_Prototype, ComponentType);
				define_component_interface(_Movement_Interface, _Phase_Movement_Interface::get_type_of(movement), Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Movement_Interface::get_type_of(inbound_link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Approaches_Container_Interface, _Approach_Interface, _Control_Plan_Interface::get_type_of(approach_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Approach_Prototype, ComponentType);

				int current_control_plan_index = current_control_plan<_Control_Plan_Interface*>()->template control_plan_index<int>();
				if (current_control_plan_index < (int)this->template control_plan_data_array<_Control_Plans_Container_Interface&>().size() - 1 )
				{
					current_control_plan_index++;
				}
				else
				{
					current_control_plan_index = 0;
				}
				current_control_plan<_Control_Plan_Interface*>(control_plan_data_array<_Control_Plans_Container_Interface&>()[current_control_plan_index]);
			};		
		};
	}
}

using namespace Intersection_Control_Components::Prototypes;