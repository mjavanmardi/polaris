#pragma once
#include "User_Space_Includes.h"
#include "Intersection_Control_Prototype.h"
#include "../File_IO/utilities.h"
//#include "Signal_Prototypes.h"

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};
namespace Link_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Link_Prototype;
	}
};
namespace Vehicle_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Vehicle_Prototype;
	}
};
namespace Network_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Network_Prototype;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Control_Prototype;
	}
};
/*------------------------------------------------*/

namespace Intersection_Components
{
	namespace Types
	{
		enum Turn_Movement_Type_Keys
		{
			LEFT_TURN=0,
			THROUGH_TURN,
			RIGHT_TURN,
			U_TURN,
		};

		enum Turn_Movement_Rule_Keys
		{
			PROHIBITED=0,
			ALLOWED,
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
#ifdef WINDOWS
		enum Intersection_Simulation_Status
		{
			NONE_COMPLETE,
			PROCESS_SKIPPED,
			COMPUTE_STEP_CONTROL_COMPLETE,
			COMPUTE_STEP_FLOW_COMPLETE,
			NETWORK_STATE_UPDATE_COMPLETE
		};
#endif
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Movement_Prototype
		{
			tag_as_prototype;

			feature_accessor(detector, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(inbound_link, none, none);
			feature_accessor(outbound_link, none, none);

			feature_accessor(movement_capacity, none, none);
			feature_accessor(movement_supply, none, none);
			feature_accessor(movement_demand, none, none);
			feature_accessor(movement_flow, none, none);
			feature_accessor(movement_transferred, none, none);

			feature_accessor(movement_type, none, none);
			feature_accessor(movement_rule, none, none);

			feature_accessor(turn_travel_penalty, none, none);

			feature_accessor(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, none, none);
			feature_accessor(cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			feature_accessor(cached_outbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			feature_accessor(inbound_link_departed_time_based_experienced_link_turn_travel_delay, none, none);
			feature_accessor(outbound_link_arrived_time_based_experienced_link_turn_travel_delay, none, none);
			
			feature_accessor(turn_movement_cumulative_arrived_vehicles, none, none);
			feature_accessor(turn_movement_cumulative_vehicles, none, none);
			feature_accessor(cached_turn_movement_cumulative_shifted_arrived_vehicles_array, none, none);
			feature_accessor(turn_movement_cumulative_shifted_arrived_vehicles, none, none);
			
			feature_accessor(forward_link_turn_travel_time, none, none);
			
			feature_accessor(replicas_container, none, none);

			feature_accessor(vehicles_container, none, none);
			feature_accessor(upstream_vehicles_container, none, none);

			feature_accessor(merge_priority, none, none);
			feature_accessor(green_time, none, none);
			feature_accessor(inbound_link_green_cycle_ratio, none,none);

			feature_prototype TargetType pull_vehicle()
			{
				return offer_vehicle<TargetType>();
			}
			
			feature_prototype TargetType offer_vehicle()
			{

				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				_Vehicle_Interface* veh=(_Vehicle_Interface*)vehicles_container<_Vehicles_Container_Interface&>().front();

				vehicles_container<_Vehicles_Container_Interface&>().pop_front();

				return (TargetType)veh;
			}

			feature_accessor(minimum_merge_rate, none, none);

			feature_accessor(movement_priority_type, none, none);
		};

		prototype struct Outbound_Inbound_Movements_Prototype
		{
			tag_as_prototype;

			feature_accessor(outbound_link_reference, none, none);
			feature_accessor(inbound_movements, none, none);
		};

		prototype struct Inbound_Outbound_Movements_Prototype
		{
			tag_as_prototype;

			feature_accessor(inbound_link_reference, none, none);
			feature_accessor(lane_groups, none, none);
			feature_accessor(outbound_movements, none, none);
		};

		prototype struct Intersection_Prototype
		{
			tag_as_prototype;

			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(signal, none, none);

			feature_accessor(rng_stream, none, none);

			feature_accessor(inbound_links, none, none);
			feature_accessor(outbound_links, none, none);

			feature_accessor(inbound_outbound_movements, none, none);
			feature_accessor(outbound_inbound_movements, none, none);

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);
			feature_accessor(z_position, none, none);

			feature_accessor(intersection_type, none, none);

			feature_accessor(intersection_simulation_status, none, none);

			feature_accessor(intersection_control, none, none);

			
			feature_accessor(network_reference, none, none);

			feature_accessor(scenario_reference, none, none);

			feature_prototype void push_vehicle(void* vehicle, int num_departed_vehicles, int num_link_origin_departed_vehicles_allowed)
			{
				accept_vehicle<TargetType>(vehicle, num_departed_vehicles, num_link_origin_departed_vehicles_allowed);
			}
			
			feature_prototype void accept_vehicle(void* vehicle, int num_departed_vehicles, int num_link_origin_departed_vehicles_allowed)
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, _Movement_Interface::get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, _Link_Interface::get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype,  ComponentType);
				//define_component_interface(_Detector_Interface,  _Movement_Interface_type::get_type_of(detector), Signal_Components::Prototypes::Detector_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();

				_Link_Interface* outbound_link;

				//PRINT(uuid<int>());

				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					
					
					_Movements_Container_Interface& inbound_movements = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					for(inbound_itr=inbound_movements.begin();inbound_itr!=inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

						if(((_Vehicle_Interface*)vehicle)->template next_link<_Link_Interface*>()==outbound_link && ((_Vehicle_Interface*)vehicle)->template current_link<_Link_Interface*>()==inbound_link)
						{
							
							//_Detector_Interface* detector;
							//detector = inbound_movement->template detector<_Detector_Interface*>();
							//if (detector != NULL) detector->template detect_vehicle<int>();
							inbound_movement->template vehicles_container<_Vehicles_Container_Interface&>().push_back((_Vehicle_Interface*)vehicle);
							inbound_movement->template turn_movement_cumulative_arrived_vehicles<int&>()++;

							char s[100];
#ifdef WINDOWS
							sprintf_s(s, "stop:%i:%i:%i\n", inbound_link->template uuid<int>(), this->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#else
							sprintf(s, "stop:%i:%i:%i\n", inbound_link->template uuid<int>(), this->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#endif
							std::string s1 = s;
							//scenario_reference<_Scenario_Interface*>()->template output<NULLTYPE>(s1);
#ifdef WINDOWS							
							sprintf_s(s, "advanced:%i:%i:%i\n", outbound_link->template uuid<int>(), outbound_link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#else
							sprintf(s, "advanced:%i:%i:%i\n", outbound_link->template uuid<int>(), outbound_link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#endif
							std::string s2 = s;
							//scenario_reference<_Scenario_Interface*>()->template output<NULLTYPE>(s2);
						}
					}
				}
			}

			feature_prototype void turn_movement_capacity_update()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();



			// y_capacity(a,t) = q_max(a,t) * delta_t * nlanes(a);
				// y_capacity(a,a',t)
				//	= min{y_capacity(a,t),y_capacity(a',t)} no control
				//	= delta_t * [green(a,a')/C] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 1: no control simulation
				//	= [green(a,a',t)/delta_t] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 2: control simulation

				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
	
				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

					float outbound_link_capacity = (float)(scenario->template simulation_interval_length<int>() * outbound_link->template num_lanes<int>() * (outbound_link->template maximum_flow_rate<float>()/3600.0));

					_Link_Interface* inbound_link;
					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					_Movement_Interface* inbound_movement;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);


						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						float inbound_link_capacity = inbound_link->template link_capacity<float>(); 
						float green_time = inbound_movement->green_time<float>();
						float green_time_ratio = inbound_movement->green_time<float>()*1.0f/((float)scenario->template simulation_interval_length<int>());
						if(intersection_type<Types::Intersection_Type_Keys>() == Types::Intersection_Type_Keys::NO_CONTROL)
						{
							inbound_movement->template movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity)*green_time_ratio);
						}
						else
						{
							inbound_movement->template movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity)*green_time_ratio);
						}

					}
				}
			}

			feature_prototype void turn_movement_supply_allocation()
			{
				define_component_interface(_Intersection_Control_Interface, get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_component_interface(_Control_Plan_Interface, _Intersection_Control_Interface::get_type_of(current_control_plan), Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);

				Node_Type_Keys control_type = this->intersection_control<_Intersection_Control_Interface*>()->template current_control_plan<_Control_Plan_Interface*>()->template control_type<Node_Type_Keys>();
				switch(control_type)
				{
					case NO_CONTROL:
						this->template supply_allocation_based_on_driving_rule<NULLTYPE>();
						break;
					case YIELD_SIGN:
						this->template supply_allocation_based_on_driving_rule<NULLTYPE>();
						break;
					case ALL_WAY_STOP_SIGN:
						this->template supply_allocation_proportion_to_demand<NULLTYPE>();
						break;
					case TWO_WAY_STOP_SIGN:
						this->template supply_allocation_based_on_driving_rule<NULLTYPE>();
						break;
					case PRE_TIMED_SIGNAL_CONTROL:
						this->template supply_allocation_based_on_driving_rule<NULLTYPE>();
						break;
					case ACTUATED_SIGNAL_CONTROL:
						this->template supply_allocation_based_on_driving_rule<NULLTYPE>();
						break;
					case ADAPTIVE_SIGNAL_CONTROL:
						this->template supply_allocation_based_on_driving_rule<NULLTYPE>();
						break;
				}
			}

			feature_prototype void turn_movement_flow_calculation()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Movement_Interface::get_type_of(cached_turn_movement_cumulative_shifted_arrived_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);


				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();



				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;

					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

						int t_minus_fftt = -1;

						int link_fftt_cached_simulation_interval = inbound_link->template link_fftt_cached_simulation_interval_size<int>();

						if (current_simulation_interval_index >= link_fftt_cached_simulation_interval)
						{
							t_minus_fftt = (current_simulation_interval_index - link_fftt_cached_simulation_interval) % link_fftt_cached_simulation_interval;			
						}

						if (t_minus_fftt > -1)
						{
							int cached_turn_movement_cumulative_shifted_arrived_vehicles=(inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>())[t_minus_fftt];
							inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>(cached_turn_movement_cumulative_shifted_arrived_vehicles);
						}
						else
						{
							inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>(0.0);
						}
					


						int movement_demand = inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>() - inbound_movement->template turn_movement_cumulative_vehicles<int>();

						inbound_movement->template movement_demand<float>(movement_demand);

						if(inbound_movement->template movement_demand<float>() < 0.0) 
						{
							inbound_movement->template movement_demand<float>(0.0);
						}

						float turn_movement_demand = inbound_movement->template movement_demand<float>();
						float turn_movement_capacity = inbound_movement->template movement_capacity<float>();
						float turn_movement_supply =  inbound_movement->template movement_supply<float>();
						float turn_movement_flow = (float) min(min((double)turn_movement_demand,(double)turn_movement_capacity),(double)turn_movement_supply);
						inbound_movement->template movement_flow<float>(turn_movement_flow);
					}
				}
			}

			feature_prototype void node_transfer()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface,  _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				typedef Vehicle_Components::Prototypes::Vehicle_Prototype<typename ComponentType::vehicle_type, ComponentType> _Vehicle_Interface;

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();



				// apply transfers of y(a,a''t) for each turn;
				//int arrived_vehicles = this->template network_cumulative_arrived_vehicles;
				//int in_network_vehicles = this->template network_in_network_vehicles;
				
				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;

					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					
					//initialization ->template  move to network state update
					outbound_link->template link_upstream_arrived_vehicles<int>(0.0);
					outbound_link->template link_destination_arrived_vehicles<int>(0.0);

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0.0);
						inbound_movement->template inbound_link_departed_time_based_experienced_link_turn_travel_delay<float>(0.0);

						if(outbound_itr==outbound_links_container.begin())
						{
							//initialization
							inbound_link->template link_downstream_departed_vehicles<int>(0.0);
						}

						//determine number of transfered vehicles
						float transfer_flow_turn_movement = inbound_movement->template movement_flow<float>();
						
						int num_transfer_vehicles_of_turn_movement = (int)transfer_flow_turn_movement;
						transfer_flow_turn_movement = transfer_flow_turn_movement - num_transfer_vehicles_of_turn_movement;
						
						if(transfer_flow_turn_movement > 0.0)
						{
							double rng = (this->template rng_stream<User_Space::RngStream&>()).RandU01();
							if(rng<=transfer_flow_turn_movement)
							{//partial vehicle, incomplete implementation
								++num_transfer_vehicles_of_turn_movement;
							}
						}

						inbound_movement->template movement_transferred<int>(num_transfer_vehicles_of_turn_movement);
						
						if (num_transfer_vehicles_of_turn_movement>0)
						{

							
							//move vehicles for this turn movement
							for (int iv=0;iv<num_transfer_vehicles_of_turn_movement;iv++)
							{
								_Vehicle_Interface* vehicle=inbound_movement->template pull_vehicle<_Vehicle_Interface*>();

								//update vehicle state: transfer to next link
								int enter_time=vehicle->template get_current_link_enter_time<NULLTYPE>();
								int delayed_time = int((scenario->template current_simulation_time<int>() - enter_time) - inbound_link->template link_fftt<float>());
								int enter_interval_index = vehicle->template current_link_enter_interval_index<int>();
								int delayed_interval = current_simulation_interval_index - enter_interval_index;

								//update inbound link state: N(a',L,t)
								inbound_link->template link_downstream_cumulative_vehicles<int&>()++;
								inbound_link->template link_downstream_departed_vehicles<int&>()++;

								outbound_link->template link_upstream_arrived_vehicles<int&>()++;
								outbound_link->template link_upstream_cumulative_arrived_vehicles<int&>()++;
								//update turn movement state: N(a',a,t)
								inbound_movement->template turn_movement_cumulative_vehicles<int&>()++;

								//update link_turn_travel_delay
								inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float&>()+=delayed_time;

								//find the departed time
								int departed_time_position = -1;
								
								int num_intervals=scenario->template num_simulation_intervals_per_assignment_interval<int>();

								if (current_simulation_interval_index>=num_intervals)
								{
									departed_time_position = (current_simulation_interval_index+num_intervals-delayed_interval)%num_intervals;
								}
								else
								{
									departed_time_position = enter_interval_index;
								}

								inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[departed_time_position]+=delayed_interval*simulation_interval_length;
								
								outbound_link->template push_vehicle<_Vehicle_Interface*>(vehicle, delayed_time);
							}
						}

						if (num_transfer_vehicles_of_turn_movement>0)
						{
							float delay=inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>()/((float)num_transfer_vehicles_of_turn_movement);
							inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(delay);
						}
						else
						{
							if(inbound_movement->template movement_rule<Types::Turn_Movement_Rule_Keys>() == Types::Turn_Movement_Rule_Keys::PROHIBITED)
							{
								inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(INFINITY_FLOAT);
							}
							else
							{
								if(inbound_movement->template movement_demand<float>() == 0)
								{//no demand
									inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0.0);
								}
								else
								{//no supply for this turn -- need a prediction -- need to work it later
									int t_minus_one = -1;
									if (current_simulation_interval_index>0)
									{
										t_minus_one = (current_simulation_interval_index-1)%scenario->template num_simulation_intervals_per_assignment_interval<int>();

										float delay = simulation_interval_length +
											inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_minus_one];

										inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(delay);
									}
									else
									{
										inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0.0);
									}
								}
							}
						}
					}
				}
			}
			
			feature_prototype void intersection_control_update()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				intersection_control<_Intersection_Control_Interface*>()->template intersection_control_update<NULLTYPE>(scenario);
			}

			

			feature_prototype void network_state_update()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Movement_Interface::get_type_of(cached_turn_movement_cumulative_shifted_arrived_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				define_component_interface(_Intersection_Control_Interface, get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();


				

				//network_data.network_cumulative_arrived_vehicles = this->template network_cumulative_arrived_vehicles;
				//network_data.network_cumulative_departed_vehicles = this->template network_cumulative_departed_vehicles;
				//network_data.network_cumulative_loaded_vehicles = this->template network_cumulative_loaded_vehicles;
				//network_data.network_in_network_vehicles = this->template network_in_network_vehicles;


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					
					outbound_link->template travel_time<float>(outbound_link->template link_fftt<float>());

					int t_fftt = -1;
					int t_bwtt = -1;
					int t_cached_delay = -1;

					if (current_simulation_interval_index>0)
					{
						t_fftt = (current_simulation_interval_index)%outbound_link->template link_fftt_cached_simulation_interval_size<int>();
						t_bwtt = (current_simulation_interval_index)%outbound_link->template link_bwtt_cached_simulation_interval_size<int>();
						t_cached_delay = (current_simulation_interval_index)%scenario->template num_simulation_intervals_per_assignment_interval<int>();
					}
					else
					{
						t_fftt = 0;
						t_bwtt = 0;
						t_cached_delay = 0;
					}

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;

					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);
						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

						int t_plus_fftt = -1;

						if (current_simulation_interval_index>0)
						{
							t_plus_fftt = (current_simulation_interval_index) % (inbound_link->template link_fftt_cached_simulation_interval_size<int>());
						}
						else
						{
							t_plus_fftt = 0;
						}

						//cached cumulative shifted arrived vehicles

						inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>()[t_plus_fftt] = inbound_movement->template turn_movement_cumulative_arrived_vehicles<int>();

						inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_cached_delay] = inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>();

						if (((current_simulation_interval_index+1)*simulation_interval_length)%scenario->template assignment_interval_length<int>() == 0)
						{	

							float turn_travel_penalty = 0.0;
							for (int t_cached_time=0;t_cached_time<scenario->template num_simulation_intervals_per_assignment_interval<int>();t_cached_time++)
							{
								turn_travel_penalty += inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_cached_time];
							}

							turn_travel_penalty = (float) ( turn_travel_penalty/((float)scenario->template num_simulation_intervals_per_assignment_interval<int>()) );
							inbound_movement->template turn_travel_penalty<float>(turn_travel_penalty);
							inbound_movement->template forward_link_turn_travel_time<float>(outbound_link->template travel_time<float>()+inbound_movement->template turn_travel_penalty<float>());
						}
					}
				}
			}
			
			feature_prototype void origin_link_loading()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Movement_Interface::get_type_of(cached_turn_movement_cumulative_shifted_arrived_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				define_component_interface(_Intersection_Control_Interface, get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Origin_Container_Interface, _Vehicle_Interface,_Link_Interface::get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_interface(_Vehicle_Origin_Queue_Interface, _Link_Interface::get_type_of(link_origin_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();


				

				//network_data.network_cumulative_arrived_vehicles = this->template network_cumulative_arrived_vehicles;
				//network_data.network_cumulative_departed_vehicles = this->template network_cumulative_departed_vehicles;
				//network_data.network_cumulative_loaded_vehicles = this->template network_cumulative_loaded_vehicles;
				//network_data.network_in_network_vehicles = this->template network_in_network_vehicles;


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

					_Vehicle_Interface* vehicle;
					outbound_link->template link_origin_departed_vehicles<int>(0.0);
					outbound_link->template link_origin_loaded_vehicles<int>(0.0);
					//arrived vehicles at current interval
					int current_position = outbound_link->template link_origin_vehicle_current_position<int>();

					if(current_position<(int)outbound_link->template link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>().size())
					{
						for(int iv=current_position;iv<(int)outbound_link->template link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>().size();iv++)
						{
							//update current position
							outbound_link->template link_origin_vehicle_current_position<int&>()++;

							vehicle=(_Vehicle_Interface*)outbound_link->template link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>()[iv];
						
							int departure_interval=vehicle->template departed_simulation_interval_index<int>();

							outbound_link->template link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().push_back(vehicle);
							outbound_link->template link_origin_arrived_vehicles<int&>()++;
							outbound_link->template link_origin_loaded_vehicles<int&>()++;
							outbound_link->template link_origin_cumulative_arrived_vehicles<int&>()++;

								scenario->template network_cumulative_loaded_vehicles<int&>()++;
						}
					}

					//have demand
					if(outbound_link->template link_origin_arrived_vehicles<int>()>0)
					{
						//check available spaces
						float link_available_spaces = outbound_link->template link_supply<float>() - outbound_link->template link_upstream_arrived_vehicles<float>();
						int num_link_origin_departed_vehicles_allowed = (int) link_available_spaces;
						float link_origin_departed_flow_allowed = (float) (link_available_spaces - num_link_origin_departed_vehicles_allowed);
					
						if (link_origin_departed_flow_allowed>0.0)
						{//partial vehicle
							double rng = rng_stream<User_Space::RngStream&>().RandU01();
							if(rng<=link_origin_departed_flow_allowed)
							{//partial vehicle, incomplete implementation
								++num_link_origin_departed_vehicles_allowed;
							}
						}

						if(num_link_origin_departed_vehicles_allowed>0)
						{//
							int num_departed_vehicles = min(outbound_link->template link_origin_arrived_vehicles<int>(),num_link_origin_departed_vehicles_allowed);
							for (int iv=0;iv<num_departed_vehicles;iv++)
							{//
								vehicle=(_Vehicle_Interface*)outbound_link->template link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().front();
								outbound_link->template link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().pop_front();

								//update vehicle state
								vehicle->template load_to_origin_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
							
								//update link state
								outbound_link->template link_origin_cumulative_departed_vehicles<int&>()++;
								outbound_link->template link_origin_departed_vehicles<int&>()++;

								outbound_link->template link_origin_arrived_vehicles<int&>()--;

								outbound_link->template push_vehicle<_Vehicle_Interface*>(vehicle, 0.0);

								scenario->template network_cumulative_departed_vehicles<int&>()++;
								scenario->template network_in_network_vehicles<int&>()++;
							}
						}
					}
				}
			}



			feature_prototype void Initialize()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);

				if (outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().size() == 0 || inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>().size() == 0)
				{
					// no outbound or no inbound
					this->template intersection_simulation_status<Intersection_Components::Types::Intersection_Simulation_Status>(Intersection_Components::Types::Intersection_Simulation_Status::PROCESS_SKIPPED);
				}
				else {
					network_reference<_Network_Interface*>()->template increment_intersection_finish_target<NULLTYPE>();
					load_event(ComponentType,Newells_Conditional,Compute_Step_Flow,scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>()-1,NULLTYPE);
					intersection_control<_Intersection_Control_Interface*>()->template Initialize<NULLTYPE>();
				}
			}

			declare_feature_conditional(Newells_Conditional)
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;

				ComponentType* _pthis = (ComponentType*)_this;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;

				long intersection_current_revision=_pthis->object_current_revision();

				
				typedef Link_Components::Types::Link_Simulation_Status link_simulation_status_type;

				if(_sub_iteration == Scenario_Components::Types::Type_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_ITERATION)
				{
					_pthis->Swap_Event((Event)&Intersection_Prototype::Compute_Step_Flow<NULLTYPE>);
					response.result=true;
					response.next=_iteration;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Iteration_keys::INTERSECTION_NETWORK_STATE_UPDATE)
				{

					_pthis->Swap_Event((Event)&Intersection_Prototype::Network_State_Update<NULLTYPE>);
					response.result=true;
					response.next=_iteration + _this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();
				} 
				else
				{
					response.result=false;
					response.next=_iteration;
				}
			}

			declare_feature_event(Compute_Step_Flow)
			{
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				
				double calculation_time_start = ::get_current_cpu_time_in_seconds();
				
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;

				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this_ptr->template turn_movement_capacity_update<NULLTYPE>(); 

				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				_this_ptr->template turn_movement_supply_allocation<NULLTYPE>();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this_ptr->template turn_movement_flow_calculation<NULLTYPE>();

				//step 6: node transfer
				_this_ptr->template node_transfer<NULLTYPE>();

				_this_ptr->template origin_link_loading<NULLTYPE>();

				_this_ptr->template intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE);

				double calculation_time_end = ::get_current_cpu_time_in_seconds();
				_this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_time_in_seconds<double&>() += calculation_time_end - calculation_time_start;
			}

			declare_feature_event(Network_State_Update)
			{
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				
				double calculation_time_start = ::get_current_cpu_time_in_seconds();
				
				Intersection_Prototype* _this_ptr=(Intersection_Prototype*)_this;

				//step 9: intersection network state update
				_this_ptr->template network_state_update<NULLTYPE>();

				_this_ptr->template intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::NETWORK_STATE_UPDATE_COMPLETE);
				_this_ptr->template network_reference<_Network_Interface*>()->template increment_intersection_finish_counter<NULLTYPE>();

				double calculation_time_end = ::get_current_cpu_time_in_seconds();
				_this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_time_in_seconds<double&>() += calculation_time_end - calculation_time_start;
			}

			feature_prototype string getStatus()
			{
				switch(intersection_simulation_status<Types::Intersection_Simulation_Status>())
				{
				case Types::Intersection_Simulation_Status::NONE_COMPLETE:
					return "NONE_COMPLETE";
				case Types::Intersection_Simulation_Status::PROCESS_SKIPPED:
					return "PROCESS_SKIPPED";
				case Types::Intersection_Simulation_Status::COMPUTE_STEP_CONTROL_COMPLETE:
					return "COMPUTE_STEP_CONTROL_COMPLETE";
				case Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE:
					return "COMPUTE_STEP_FLOW_COMPLETE";
				case Types::Intersection_Simulation_Status::NETWORK_STATE_UPDATE_COMPLETE:
					return "NETWORK_STATE_UPDATE_COMPLETE";
				}
			}

			feature_prototype void supply_allocation_based_on_driving_rule()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				size_t size = outbound_links_container.size();
				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					int inbound_turn_movement_size=(int)inbound_links_container.size();
					if(inbound_turn_movement_size >= 1)
					{
						if (inbound_turn_movement_size == 1)
						{
							inbound_links_container[0]->template movement_supply<float>(outbound_link->template link_supply<float>());
						}
						else
						{//divide space based on merging policy
							///driving rule with priority : protected movement v.s. permitted movements
							///only one movement can be specified as protected movement for merging to a link, other movements should be yielded the protected movement
							///there will be capacity lose for such driving rule for permitted movements
							///if there is no protected movement, then all the movements are all permitted and thus share the capacity based on demand
							int num_allowed_movements = 0;
							float total_transfer_demand = 0.0f;
							_Movement_Interface* protected_turn_movement = nullptr;

							for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
							{
								inbound_movement=(_Movement_Interface*)(*inbound_itr);

								inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

								total_transfer_demand += inbound_movement->template movement_demand<float>();
					
								if (inbound_movement->template merge_priority<int>() == 1)
								{
									protected_turn_movement = inbound_movement;
								}
								if (inbound_movement->template movement_rule<int>() == ALLOWED)
								{
									num_allowed_movements++;
								}
							}

							if (total_transfer_demand>0)
							{
								if (total_transfer_demand<=outbound_link->template link_supply<float>())
								{//case 1: total_transfer_demand<= link_supply_array
									for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
									{
										inbound_movement = inbound_links_container[inbound_turn_movement];
										inbound_movement->template movement_supply<float>(inbound_movement->template movement_demand<float>());
									}
								}
								else
								{//case 2: total_transfer_demand<= link_supply_array
									if (protected_turn_movement != nullptr)
									{
										if (protected_turn_movement->template movement_demand<float>() >= outbound_link->link_supply<float>())
										{
											protected_turn_movement->movement_supply<float>(outbound_link->link_supply<float>());
											for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
											{
												inbound_movement = inbound_links_container[inbound_turn_movement];
												if (inbound_movement->template internal_id<int>() != protected_turn_movement->template internal_id<int>())
												{
													inbound_movement->template movement_supply<float>(0.0);
												}
											}
											
										}
										else
										{
											protected_turn_movement->template movement_supply<float>(protected_turn_movement->template movement_demand<float>());
								
											total_transfer_demand -= protected_turn_movement->template movement_demand<float>();
											float link_supply_leftover = outbound_link->template link_supply<float>() - protected_turn_movement->template movement_supply<float>();
								
											for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
											{
												inbound_movement = inbound_links_container[inbound_turn_movement];
												if (inbound_movement->template internal_id<int>() != protected_turn_movement->template internal_id<int>())
												{
													inbound_movement->template movement_supply<float>(link_supply_leftover * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
												}
											}
										}
					
									}
									else
									{
										for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
										{
											inbound_movement = inbound_links_container[inbound_turn_movement];
											inbound_movement->template movement_supply<float>(outbound_link->template link_supply<float>() * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
										}
										
									}
								}
							}
							else
							{
								for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
								{
									inbound_movement = inbound_links_container[inbound_turn_movement];
						
									if (inbound_movement->template movement_rule<int>() == ALLOWED)
									{
										inbound_movement->movement_supply<float>(outbound_link->template link_supply<float>() / (num_allowed_movements*1.0f));
									}
									else
									{
										inbound_movement->movement_supply<float>(0.0f);
									}

								}
							}

						}
					}
				}
			}

			feature_prototype void supply_allocation_proportion_to_demand()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				size_t size = outbound_links_container.size();
				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					
					_Movement_Interface* inbound_movement;

					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					int inbound_turn_movement_size=(int)inbound_links_container.size();
					if (inbound_turn_movement_size == 1)
					{//only one inbound turn movement - allocate full space of outbound link to inbound turn movement
						inbound_movement = inbound_links_container[0];
						inbound_movement->movement_supply<float>(outbound_link->template link_supply<float>());
					}
					else
					{//divide space based on merging policy
						///propotional to demand
						float total_transfer_demand = 0.0f;
						for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
						{
							inbound_movement = inbound_links_container[inbound_turn_movement];
							total_transfer_demand += inbound_movement->template movement_demand<float>();
							inbound_movement->template movement_supply<float>(outbound_link->link_supply<float>() / (inbound_turn_movement_size*1.0f));
						}

						if (total_transfer_demand>0)
						{
									
							if (total_transfer_demand<=outbound_link->template link_supply<float>())
							{//case 1: total_transfer_demand<= link_supply_array
								for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
								{
									inbound_movement = inbound_links_container[inbound_turn_movement];
									inbound_movement->template movement_supply<float>(inbound_movement->template movement_demand<float>());
								}
							}
							else
							{//case 2: total_transfer_demand<= link_supply_array
								for (int inbound_turn_movement = 0; inbound_turn_movement < inbound_turn_movement_size; inbound_turn_movement++)
								{
									inbound_movement = inbound_links_container[inbound_turn_movement];
									inbound_movement->template movement_supply<float>(outbound_link->template link_supply<float>() * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
								}
							}
						}
					}
				}
			}
		};
	}
}

using namespace Intersection_Components::Prototypes;