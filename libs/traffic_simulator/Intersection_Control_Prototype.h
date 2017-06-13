#pragma once
#include "Traffic_Simulator_Includes.h"
#include "Link_Prototype.h"
//#include "Scenario_Prototype.h"
#include "Turn_Movement_Prototype.h"
extern void* _global_scenario;

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
		prototype struct Phase_Movement ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(movement, NONE, NONE);
			accessor(movement_priority_type, NONE, NONE);
		};

		prototype struct Phase ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(phase_index, NONE, NONE);	// the index counter on all phases in the network
			accessor(phase_sequence_number, NONE, NONE);	// 
				
			//starting time of green, yellow, and red
			accessor(green_starting_time, NONE, NONE);
			accessor(yellow_starting_time, NONE, NONE);
			accessor(red_start_time, NONE, NONE);

			//actuated signal control
			accessor(maximum_green_time, NONE, NONE);
			accessor(minimum_green_time, NONE, NONE);

			//pre-timed signal control
			accessor(offset, NONE, NONE);
			accessor(green_time, NONE, NONE);

			//clearence time
			accessor(yellow_time, NONE, NONE);
			accessor(all_red_time, NONE, NONE);

			//adjusted green and yellow time
			accessor(adjusted_green_time, NONE, NONE);
			accessor(adjusted_yellow_time, NONE, NONE);

			//adjusted max and min green
			accessor(adjusted_maximum_green_time, NONE, NONE);
			accessor(adjusted_minimum_green_time, NONE, NONE);

			//movements
			accessor(turn_movements_in_the_phase_array, NONE, NONE);
		};

		prototype struct Approach ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			accessor(approach_index, NONE, NONE);
			accessor(approach_type, NONE, NONE);
			accessor(inbound_link, NONE, NONE);
			accessor(green_cycle_ratio, NONE, NONE);
		};

		prototype struct Control_Plan ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(control_plan_index, NONE, NONE);

			//duration
			accessor(starting_time, NONE, NONE);
			accessor(ending_time, NONE, NONE);
			
			//type
			accessor(control_type, NONE, NONE);

			//timing plan for signal control nodes
			accessor(offset, NONE, NONE);
			accessor(cycle_length, NONE, NONE);

			accessor(cycle_index, NONE, NONE);
			accessor(cycle_starting_time, NONE, NONE);
			accessor(cycle_ending_time, NONE, NONE);
			accessor(cycle_leftover_time, NONE, NONE);

			accessor(phase_data_array, NONE, NONE);

			//approach data
			accessor(approach_data_array, NONE, NONE);

			//two-way stop and yield sign
			accessor(major_approach_data_array, NONE, NONE);

			accessor(minor_approach_data_array, NONE, NONE);
		};

		using namespace Intersection_Control_Components::Types;
		using namespace Turn_Movement_Components::Types;
		
		prototype struct Intersection_Control ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(intersection, NONE, NONE);
			accessor(current_control_plan, NONE, NONE);
			accessor(control_plan_data_array, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			
			template<typename TargetType> void set_node_control_plan_index(int time)
			{
				// Make interfaces
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				int t_start = time%(24*60*60);

				// Loop over all control plans, if current_time lies within range for the plan, set as current
				for (int i=0;i<(int)control_plan_data_array<_Control_Plans_Container_Interface&>().size();i++)
				{
					_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)control_plan_data_array<_Control_Plans_Container_Interface&>()[i];
					int current_control_plan_start_time = control_plan->template starting_time<int>();
					int current_control_plan_end_time = control_plan->template ending_time<int>();
					if (t_start >= current_control_plan_start_time && t_start < current_control_plan_end_time)
					{
						current_control_plan<_Control_Plan_Interface*>(control_plan);
						break;
					}
				}
			}

			template<typename TargetType> void intersection_control_update()
			{
				// Make Interfaces
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;
				typedef  Intersection_Components::Prototypes::Intersection<typename get_type_of(intersection)> _Intersection_Interface;


				int control_type = current_control_plan<_Control_Plan_Interface*>()->template control_type<int>();

				// Perform Different Update Based on Type
				switch(control_type)
				{
					case NO_CONTROL:
						this->template no_control_state_update<NULLTYPE>();
						break;
					case YIELD_SIGN:
						this->template yield_sign_control_state_update<NULLTYPE>();
						break;
					case ALL_WAY_STOP_SIGN:
						this->template all_way_stop_control_state_update<NULLTYPE>();
						break;
					case TWO_WAY_STOP_SIGN:
						this->template two_way_stop_control_state_update<NULLTYPE>();
						break;
					case PRE_TIMED_SIGNAL_CONTROL:
						this->template pre_timed_signal_control_state_update<NULLTYPE>();
						break;
					case ACTUATED_SIGNAL_CONTROL:
						this->template actuated_signal_control_state_update<NULLTYPE>();
						break;
					case ADAPTIVE_SIGNAL_CONTROL:
						this->template adaptive_signal_control_state_update<NULLTYPE>();
						break;
				}
			}
			
			template<typename TargetType> void no_control_state_update()
			{
				// Make Interfaces
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;


				// Figure out time bounds for the signal vs. the simulation interval
				int t_start = network_reference<_Network_Interface*>()->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				// calculate green time
				this->template calculate_turn_movement_green_time_no_control<NULLTYPE>();

				// end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					// advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};
			
			template<typename TargetType> void calculate_turn_movement_green_time_no_control()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef Intersection_Components::Prototypes::Intersection<typename get_type_of(intersection)> _Intersection_Interface;
				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename _Intersection_Interface::get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Inbound_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Inbound_Movement_Interface*> _Inbound_Movements_Container_Interface;

				typedef  Link_Components::Prototypes::Link< typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference)> _Link_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;

				//green times for each movement are assumed to be the length of the simulation interval
				if((int*)intersection<_Intersection_Interface*>() == nullptr) return;
				_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection<_Intersection_Interface*>()->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				
				// Loop over outbound movements
				for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin();outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);
					_Link_Interface* outbound_link = outbound_inbound_movements->template outbound_link_reference<_Link_Interface*>();
					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;

					// Loop over inbound movements for each outbound movement
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						_Inbound_Movement_Interface* inbound_movement = (_Inbound_Movement_Interface*)(*inbound_movement_itr);

						//turn movement green time
						//allowed
						if (inbound_movement->template movement_rule<int>() == ALLOWED)
						{
							//inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>() / float(inbound_movements_container.size()));
							if (outbound_link->template link_type<int>() != ARTERIAL || inbound_movements_container.size() <=2)
							{
								if (inbound_movement->template movement_type<int>() == THROUGH_TURN)
								{
									inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*1.0f);
								}
								else
								{
									if (outbound_link->template link_type<int>() == ARTERIAL)
									{
										//operation_data.turn_movement_green_time_array[inbound_turn_movement_index] = scenario_data.simulation_interval_length*0.6f;
										inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*1.0f);
									}
									else
									{
										//operation_data.turn_movement_green_time_array[inbound_turn_movement_index] = scenario_data.simulation_interval_length*0.9f;
										inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*1.0f);
									}
								}
							}
							else
							{
								inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*1.0f);

								//if (network_data.turn_movement_data_array[inbound_turn_movement_index].turn_movement_type == THROUGH_TURN)
								//{
								//	operation_data.turn_movement_green_time_array[inbound_turn_movement_index] = scenario_data.simulation_interval_length*1.0f/
								//		((inbound_turn_movement_size)*1.0f);
								//}
								//if (network_data.turn_movement_data_array[inbound_turn_movement_index].turn_movement_type == RIGHT_TURN)
								//{
								//	operation_data.turn_movement_green_time_array[inbound_turn_movement_index] = scenario_data.simulation_interval_length*0.9f/
								//		((inbound_turn_movement_size)*1.0f);
								//}
								//if (network_data.turn_movement_data_array[inbound_turn_movement_index].turn_movement_type == LEFT_TURN)
								//{
								//	operation_data.turn_movement_green_time_array[inbound_turn_movement_index] = scenario_data.simulation_interval_length*0.9f/
								//		((inbound_turn_movement_size)*1.0f);
								//}
								//if (network_data.turn_movement_data_array[inbound_turn_movement_index].turn_movement_type == U_TURN)
								//{
								//	operation_data.turn_movement_green_time_array[inbound_turn_movement_index] = scenario_data.simulation_interval_length*0.8f/
								//		((inbound_turn_movement_size)*1.0f);
								//}
							}
						}

						//prihibited
						if (inbound_movement->template movement_rule<int>() == PROHIBITED)
						{
							// Set green time as 0
							inbound_movement->template green_time<float>(0.0);
						}

						//merge priority
						inbound_movement->template merge_priority<int>(0.0);
					}
				}
			};

			template<typename TargetType> void calculate_turn_movement_green_time_yield_control()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(major_approach_data_array)::value_type>::type>  _Approach_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(major_approach_data_array), _Approach_Interface*> _Major_Approaches_Container_Interface;


				typedef Intersection_Components::Prototypes::Intersection<typename get_type_of(intersection)> _Intersection_Interface;
				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename _Intersection_Interface::get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Inbound_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Inbound_Movement_Interface*> _Inbound_Movements_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface2;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface2*> _Approaches_Container_Interface;

				typedef Link_Components::Prototypes::Link<typename _Approach_Interface::get_type_of(inbound_link)> _Link_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				//green times for each movement are assumed to be the length of the simulation interval
				_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection<_Intersection_Interface*>()->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				
				// Loop over outbound movements
				for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin();outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					
					// Loop over inbound movements for each outbound movement
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						_Inbound_Movement_Interface* inbound_movement = (_Inbound_Movement_Interface*)(*inbound_movement_itr);
						_Link_Interface* inbound_link = inbound_movement->template inbound_link<_Link_Interface*>();
						
						//turn movement green time
						//allowed
						if (inbound_movement->template movement_rule<int>() == ALLOWED)
						{
							// Set green time as interval length
							inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>());
						}

						//prohibited
						if (inbound_movement->template movement_rule<int>() == PROHIBITED)
						{
							// Set green time as 0
							inbound_movement->template green_time<float>(0.0);
						}

						//merge priority
						_Approaches_Container_Interface& approaches_container = current_control_plan<_Control_Plan_Interface*>()->template approach_data_array<_Approaches_Container_Interface&>();
						
						// Loop over all approaches
						for (int approach_index=0;approach_index<(int)approaches_container.size();approach_index++)
						{
							_Approach_Interface* approach = approaches_container[approach_index];
							int approach_link_index = approach->template inbound_link<_Link_Interface*>()->template internal_id<int>();
							
							// Find the matching approach
							if (approach_link_index == inbound_link->template internal_id<int>())
							{
								if (approach->template approach_type<int>() == Types::MAJOR_APPROACH)
								{
									// Set merge priority high
									inbound_movement->template merge_priority<int>(1);
								}
								else
								{
									// Set merge priority low
									inbound_movement->template merge_priority<int>(0.0);
									int num_major_approaches = int(current_control_plan<_Control_Plan_Interface*>()->template major_approach_data_array<_Major_Approaches_Container_Interface&>().size());
									if (inbound_movement->template movement_rule<int>() == ALLOWED)
									{
										if (inbound_movement->template movement_type<int>() == THROUGH_TURN)
										{
											inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*1.0f/
												(num_major_approaches*1.0f));
										}
										if (inbound_movement->template movement_type<int>() == RIGHT_TURN)
										{
											inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*1.0f/
												(num_major_approaches*1.0f));
										}
										if (inbound_movement->template movement_type<int>() == LEFT_TURN)
										{
											inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*1.0f/
												(num_major_approaches*1.0f));
										}
										if (inbound_movement->template movement_type<int>() == U_TURN)
										{
											inbound_movement->template green_time<float>(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()*0.8f/
												(num_major_approaches*1.0f));
										}

									}

								}

								break;
							}
						}

						//highway : mainline and ramp
						if (inbound_link->template link_type<int>() == Link_Components::Types::FREEWAY || inbound_link->template link_type<int>() == Link_Components::Types::FREEWAY || Link_Components::Types::EXPRESSWAY)
						{
							// Set merge priority high
							inbound_movement->template merge_priority<int>(1);
						}

						if (inbound_link->template link_type<int>() == Link_Components::Types::ON_RAMP)
						{
							// Set merge priority low
							inbound_movement->template merge_priority<int>(0.0);
						}
					}
				}
			};

			template<typename TargetType> void yield_sign_control_state_update()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				// Figure out time bounds for the signal vs. the simulation interval

				int t_start = network_reference<_Network_Interface*>()->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				// calculate green time
				this->template calculate_turn_movement_green_time_yield_control<NULLTYPE>();

				// end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					//advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			template<typename TargetType> void all_way_stop_control_state_update()
			{
				// Make Interfaces
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;


				// Figure out time bounds for the signal vs. the simulation interval

				int t_start = network_reference<_Network_Interface*>()->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);;
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				// calculate green time
				this->template calculate_turn_movement_green_time_no_control<NULLTYPE>();

				// end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					//advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			template<typename TargetType> void two_way_stop_control_state_update()
			{
				// Make Interfaces
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;


				// Figure out time bounds for the signal vs. the simulation interval

				int t_start = network_reference<_Network_Interface*>()->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);;
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				// calculate green time
				this->template calculate_turn_movement_green_time_yield_control<NULLTYPE>();

				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					// advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			template<typename TargetType> void pre_timed_signal_control_state_update()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;


				// Figure out time bounds for the signal vs. the simulation interval

				int t_start = network_reference<_Network_Interface*>()->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);;
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				//start of the control plan
				if (t_start == starting_time)
				{
					//calcualte starting times of green, yellow, and red of each phase; green_cycle_ratio
					this->template calculate_green_yellow_red_starting_times_and_green_cycle_ratio<NULLTYPE>();
				}

				//calculate green time
				this->template initialize_node_turn_movement_green_time<NULLTYPE>();
				this->template calculate_turn_movement_green_time<NULLTYPE>();

				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					// advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			template<typename TargetType> void actuated_signal_control_state_update()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;


				// Figure out time bounds for the signal vs. the simulation interval

				int t_start = network_reference<_Network_Interface*>()->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);;
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				int starting_time = current_control_plan<_Control_Plan_Interface*>()->template starting_time<int>();
				int ending_time = current_control_plan<_Control_Plan_Interface*>()->template ending_time<int>();

				//start of the control plan
				if (t_start == starting_time)
				{
					//calculate starting times of green, yellow, and red of each phase; green_cycle_ratio
					this->template calculate_green_yellow_red_starting_times_and_green_cycle_ratio<NULLTYPE>();
				}

				//change signal parameters in response to demand
				this->template actuated_signal_control_response<NULLTYPE>();

				//calculate green time
				this->template initialize_node_turn_movement_green_time<NULLTYPE>();
				this->template calculate_turn_movement_green_time<NULLTYPE>();


				//end of the control plan
				if (t_end == ending_time || (t_start < ending_time && t_end > ending_time))
				{
					//advance control plan
					this->template advance_control_plan_period<NULLTYPE>();
				}
			};

			template<typename TargetType> void actuated_signal_control_response()
			{
				// Make Interfaces
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement< typename _Phase_Movement_Interface::get_type_of(movement)> _Movement_Interface;
				typedef  Link_Components::Prototypes::Link< typename _Movement_Interface::get_type_of(inbound_link)> _Link_Interface;
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;

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

				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>(); // in second
				int t_start_next;
				int new_cycle_leftover_time;

				int next_cycle_leftover_time = cycle_leftover_time - ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				if (next_cycle_leftover_time>=0)
				{//same cycle
					t_start_next = t_start;
				}
				else
				{//different cycle
					t_start_next = t_end;
				}

				float lost_time = 0.0f;
				std::vector<_Link_Interface*> critical_link_array; 
				std::vector<int> critical_link_queue_length_array;
				std::vector<int> critical_link_discharge_time_array;

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

					//determine the extension of the green time to discharge the maximum queue
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
					if (current_critical_link_discharge_time + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() <= 0)
					{
						current_critical_link_discharge_time = 0 - ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
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
					//redefine the starting and ending of green time for all subsequent phases
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

			template<typename TargetType> void adaptive_signal_control_state_update()
			{
				this->template no_control_state_update<NULLTYPE>();
			};

			template<typename TargetType> void calculate_green_yellow_red_starting_times_and_green_cycle_ratio()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement< typename _Phase_Movement_Interface::get_type_of(movement)> _Movement_Interface;
				typedef  Link_Components::Prototypes::Link< typename _Movement_Interface::get_type_of(inbound_link)> _Link_Interface;
				typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;


				int num_phases = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>().size();

				int last_phase = num_phases - 1;
				
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
						_Movement_Interface* movement = phase_movement->template movement<_Movement_Interface*>();
					
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

			template<typename TargetType> void initialize_node_turn_movement_green_time()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef Intersection_Components::Prototypes::Intersection<typename get_type_of(intersection)> _Intersection_Interface;
				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename _Intersection_Interface::get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Inbound_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Inbound_Movement_Interface*> _Inbound_Movements_Container_Interface;


			
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

			template<typename TargetType> void calculate_turn_movement_green_time()
			{
				// Make Interfaces
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement< typename _Phase_Movement_Interface::get_type_of(movement)> _Movement_Interface;
				typedef  Link_Components::Prototypes::Link< typename _Movement_Interface::get_type_of(inbound_link)> _Link_Interface;
				typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;

				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;

				int num_phases = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>().size();
				int offset = current_control_plan<_Control_Plan_Interface*>()->template offset<int>();
				int last_phase = num_phases - 1;
				int cycle_length = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template red_start_time<int>() + 
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template all_red_time<int>();
	
				int cycle_leftover_time = current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>();
				int next_cycle_leftover_time = cycle_leftover_time - ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
	
				int t_start = cycle_length - cycle_leftover_time;
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
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

					if (tmp_green_time>((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())
					{
						tmp_green_time = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
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
						if (phase_movement->template movement_priority_type<int>() == Types::PROTECTED)
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
						movement->template green_time<float&>() +=  tmp_green_time*1.0f;
						//cout << "updating green time " << movement->template green_time<float>();
					}
				}

				if (next_cycle_leftover_time>=0)
				{
					if (next_cycle_leftover_time == 0)
					{
						this->template advance_signal_control_cycle<NULLTYPE>(next_cycle_leftover_time);
					}
					else
					{
						current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>( 
							current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>() - 
							((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
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

						if (tmp_green_time>((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())
						{
							tmp_green_time = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
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
								if (movement->template green_time<float>()>0)
								{
									continue;
								}
							}
							movement->template green_time<float&>() +=  tmp_green_time*1.0f;
							//cout << "updating green time B " << movement->template green_time<float>();
						}
					}

					this->template advance_signal_control_cycle<NULLTYPE>(next_cycle_leftover_time);
				}
			};

			template<typename TargetType> void advance_signal_control_cycle(int cycle_leftover_time)
			{

				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement< typename _Phase_Movement_Interface::get_type_of(movement)> _Movement_Interface;
				typedef  Link_Components::Prototypes::Link< typename _Movement_Interface::get_type_of(inbound_link)> _Link_Interface;
				typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;


				int t_start = network_reference<_Network_Interface*>()->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				int num_phases = (int)current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>().size();


				int last_phase = num_phases - 1;
				int cycle_length = current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template red_start_time<int>() + 
					current_control_plan<_Control_Plan_Interface*>()->template phase_data_array<_Phases_Container_Interface&>()[last_phase]->template all_red_time<int>();

				current_control_plan<_Control_Plan_Interface*>()->template cycle_index<int&>()++;
				current_control_plan<_Control_Plan_Interface*>()->template cycle_starting_time<int>(t_end + cycle_leftover_time);
				current_control_plan<_Control_Plan_Interface*>()->template cycle_ending_time<int>(t_end + cycle_leftover_time + cycle_length);
				current_control_plan<_Control_Plan_Interface*>()->template cycle_leftover_time<int>(cycle_leftover_time + cycle_length);
			};
			
			template<typename TargetType> void advance_control_plan_period()
			{
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement< typename _Phase_Movement_Interface::get_type_of(movement)> _Movement_Interface;
				typedef  Link_Components::Prototypes::Link< typename _Movement_Interface::get_type_of(inbound_link)> _Link_Interface;
				typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;


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

			template<typename TargetType> void Initialize()
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				//TODO
//load_event(ComponentType,Newells_Conditional,Compute_Step_Control,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::CONTROL_SUB_ITERATION,NULLTYPE);
			
				this_component()->template Load_Event<ComponentType>(&Newells_Conditional,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::CONTROL_SUB_ITERATION);
			}

			static void Newells_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Intersection_Control<ComponentType> _Intersection_Control_Interface;
				typedef Intersection_Components::Prototypes::Intersection<typename get_type_of(intersection)> _Intersection_Interface;
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Intersection_Control_Interface* _this_ptr=(_Intersection_Control_Interface*)_this;
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::CONTROL_SUB_ITERATION)
				{
					//_pthis->Swap_Event((Event)&Intersection_Control::Compute_Step_Control<NULLTYPE>);
					
					_this_ptr->template Compute_Step_Control<NULLTYPE>();
					
					//response.result=true;
					response.next._iteration=iteration() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::CONTROL_SUB_ITERATION;

				}
				else
				{
					assert(false);
					cout << "Should never reach here in intersection control conditional!" << endl;
				}
			}

			//declare_event(Compute_Step_Control)
			template<typename TargetType> void Compute_Step_Control()
			{
				typedef Intersection_Control<ComponentType> _Intersection_Control_Interface;
				_Intersection_Control_Interface* _this_ptr=(_Intersection_Control_Interface*)this;
				//step 1: update control
				_this_ptr->template intersection_control_update<NULLTYPE>();
			}
		};
	}
}

using namespace Intersection_Control_Components::Prototypes;
