#pragma once
#include "User_Space_Includes.h"
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
			feature_accessor(id, none, none);
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

			feature_accessor(scenario_reference, none, none);

			feature_accessor(network, none, none);
			

			feature_accessor(intersection_simulation_status, none, none);

			feature_prototype void push_vehicle(void* vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}
			
			feature_prototype void accept_vehicle(void* vehicle)
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
				int current_time = scenario->template current_time<int>();


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

						//PRINT(((Vehicle_Interface*)vehicle)->template next_link<Link_Interface*>()->template uuid<int>() << "," << outbound_link->template uuid<int>());
						//PRINT(((Vehicle_Interface*)vehicle)->template current_link<Link_Interface*>()->template uuid<int>() << "," << inbound_link->template uuid<int>());

						if(((_Vehicle_Interface*)vehicle)->template next_link<_Link_Interface*>()==outbound_link && ((_Vehicle_Interface*)vehicle)->template current_link<_Link_Interface*>()==inbound_link)
						{
							
							//_Detector_Interface* detector;
							//detector = inbound_movement->template detector<_Detector_Interface*>();
							//if (detector != NULL) detector->template detect_vehicle<int>();

							_Vehicle_Interface* veh = (_Vehicle_Interface*)vehicle;
							inbound_movement->template vehicles_container<_Vehicles_Container_Interface&>().push_back(veh);
							inbound_movement->template turn_movement_cumulative_arrived_vehicles<int&>()++;

							if (outbound_link->template uuid<int>() == 2 && inbound_link->template uuid<int>() == 1)
							{
								bool test = false;
							}

							char s[100];
#ifdef WINDOWS
							sprintf_s(s, "stop:%i:%i:%i\n", inbound_link->template uuid<int>(), this->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#else
							sprintf(s, "stop:%i:%i:%i\n", inbound_link->template uuid<int>(), this->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#endif
							std::string s1 = s;
							scenario_reference<_Scenario_Interface*>()->template output<NULLTYPE>(s1);
#ifdef WINDOWS							
							sprintf_s(s, "advanced:%i:%i:%i\n", outbound_link->template uuid<int>(), outbound_link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#else
							sprintf(s, "advanced:%i:%i:%i\n", outbound_link->template uuid<int>(), outbound_link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>(), scenario_reference<_Scenario_Interface*>()->template current_time<int>());
#endif
							std::string s2 = s;
							scenario_reference<_Scenario_Interface*>()->template output<NULLTYPE>(s2);
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
				
				//PRINT("\t" << "turn movement capacities of node " << uuid<int>());

				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

					float outbound_link_capacity = (float)(scenario->template simulation_interval_length<int>() * outbound_link->template num_lanes<int>() * (outbound_link->template maximum_flow_rate<float>()/3600.0));

					_Link_Interface* inbound_link;
					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					_Movement_Interface* inbound_movement;
					
					//PRINT("\t\t" << "for outbound link " << outbound_link->template uuid<int>());

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						float inbound_link_capacity = inbound_link->template link_capacity<float>(); 
						
						if(intersection_type<Types::Intersection_Type_Keys>() == Types::Intersection_Type_Keys::NO_CONTROL)
						{
							inbound_movement->template movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}
						else
						{
							inbound_movement->template movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}

						//PRINT("\t\t\t" << "for inbound link " << inbound_link->template uuid<int>() << " is: " << inbound_movement->template movement_capacity<float>());
					}
				}
			}

			feature_prototype void turn_movement_supply_allocation()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();



				// y_capacity(a,t) = q_max(a,t) * delta_t * nlanes(a);
				// y_capacity(a,a',t)
				//	= min{y_capacity(a,t),y_capacity(a',t)} no control
				//	= delta_t * [green(a,a')/C] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 1: no control simulation
				//	= [green(a,a',t)/delta_t] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 2: control simulation
				
				
				vector<float> merge_ratios;
				const int max_inbound_turn_movement_size=20;
				merge_ratios.resize(max_inbound_turn_movement_size);

				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				//PRINT("\t" << "turn movement supplies of node " << uuid<int>());
				size_t size = outbound_links_container.size();
				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					
					//PRINT("\t\t" << "for outbound link " << outbound_link->template uuid<int>());

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					int inbound_turn_movement_size=(int)inbound_links_container.size();
					
					if(inbound_turn_movement_size==1)
					{
						((_Movement_Interface*)inbound_links_container[0])->template movement_supply<float>(outbound_link->template link_supply<float>());
					}
					else
					{
						///simple rule as Daganzo --- need to work on this later
						int num_lanes_outbound_link = outbound_link->template num_lanes<int>();
						int link_type_outbound_link = outbound_link->template link_type<int>();
						int num_merge_lanes = 0;

						int num_through_turns = 0;

						for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
						{
							inbound_movement=(_Movement_Interface*)(*inbound_itr);

							inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

							int num_lanes_inbound_link = inbound_link->template num_lanes<int>();
							int link_type_inbound_link = inbound_link->template link_type<int>();
							
							if (link_type_inbound_link != link_type_outbound_link)
							{
								if (num_merge_lanes<num_lanes_inbound_link)
								{
									num_merge_lanes = num_lanes_inbound_link;
								}
							}
							if (inbound_movement->template movement_type<Types::Turn_Movement_Type_Keys>() == Types::Turn_Movement_Type_Keys::THROUGH_TURN)
							{
								num_through_turns++;
							}
						}
						float major_approach_merge_ratio = 0.6;
						float minor_approach_merge_ratio = 0.4;


						if (num_through_turns>0)
						{
							float through_turn_ratio = 1.0/(float(num_through_turns));
							if (num_merge_lanes<num_lanes_outbound_link)
							{
								float dedicated_lanes = float(num_lanes_outbound_link - num_merge_lanes);
								major_approach_merge_ratio = through_turn_ratio* dedicated_lanes / (float(num_lanes_outbound_link)) + 0.6 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link)));
								minor_approach_merge_ratio = 0.4 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link))) / (float(inbound_turn_movement_size-1));
							}
							else
							{
								major_approach_merge_ratio =through_turn_ratio* 0.6;
								minor_approach_merge_ratio = 0.4 / (float(inbound_turn_movement_size-1));
							}
						}
						else
						{
							if (num_merge_lanes<num_lanes_outbound_link)
							{
								float dedicated_lanes = float(num_lanes_outbound_link - num_merge_lanes);
								major_approach_merge_ratio =  dedicated_lanes / (float(num_lanes_outbound_link)) + 0.5 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link)));
								minor_approach_merge_ratio = 0.5 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link))) / (float(inbound_turn_movement_size-1));
							}
							else
							{
								major_approach_merge_ratio = 0.5;
								minor_approach_merge_ratio = 0.5 / (float(inbound_turn_movement_size-1));
							}
						}

						for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
						{
							inbound_movement=(_Movement_Interface*)(*inbound_itr);

							inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

							int link_type_inbound_link = inbound_link->template link_type<int>();
							float merge_ratio = 0;

							if(link_type_inbound_link == link_type_outbound_link && inbound_movement->template movement_type<Types::Turn_Movement_Type_Keys>() == Types::Turn_Movement_Type_Keys::THROUGH_TURN)
							{
								merge_ratio = major_approach_merge_ratio;
							}
							else
							{
								merge_ratio = minor_approach_merge_ratio;
							}

							inbound_movement->template movement_supply<float>(merge_ratio*outbound_link->template link_supply<float>());
							//PRINT("\t\t\t" << "for inbound link " << inbound_link->template uuid<int>() << " is: " << inbound_movement->template movement_supply<float>());
						}
						///simple rule as Daganzo
					}
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
				int current_time = scenario->template current_time<int>();


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
							inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>(0);
						}
						
						int movement_demand = inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>() - inbound_movement->template turn_movement_cumulative_vehicles<int>();
						
						inbound_movement->template movement_demand<float>(movement_demand);

						if(inbound_movement->template movement_demand<float>() < 0.0) 
						{
							inbound_movement->template movement_demand<float>(0);
						}

						float turn_movement_demand = inbound_movement->template movement_demand<float>();
						float turn_movement_capacity = inbound_movement->template movement_capacity<float>();
						float turn_movement_supply =  inbound_movement->template movement_supply<float>();
						float turn_movement_flow = (float) min(min((double)turn_movement_demand,(double)turn_movement_capacity),(double)turn_movement_supply);
						inbound_movement->template movement_flow<float>(turn_movement_flow);
						//PRINT("\t" << iteration << ": turn movement flow: " << turn_movement_flow);
					}
				}
			}

			feature_prototype void node_transfer()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface,  _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, int, ComponentType);
				typedef Vehicle_Components::Prototypes::Vehicle_Prototype<typename ComponentType::vehicle_type, ComponentType> _Vehicle_Interface;

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();
				int current_time = scenario->template current_time<int>();


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
					outbound_link->template link_upstream_arrived_vehicles<int>(0);
					outbound_link->template link_destination_arrived_vehicles<int>(0);

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

						inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
						inbound_movement->template inbound_link_departed_time_based_experienced_link_turn_travel_delay<float>(0);

						if(outbound_itr==outbound_links_container.begin())
						{
							//initialization
							inbound_link->template link_downstream_departed_vehicles<int>(0);
						}

						//determine number of transfered vehicles
						float transfer_flow_turn_movement = inbound_movement->template movement_flow<float>();
						
						int num_transfer_vehicles_of_turn_movement = (int)transfer_flow_turn_movement;
						transfer_flow_turn_movement = transfer_flow_turn_movement - num_transfer_vehicles_of_turn_movement;
						
						if(transfer_flow_turn_movement > 0.0)
						{
							double rng = (this->template rng_stream<RngStream&>()).RandU01();
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
								int enter_time=vehicle->template current_link_enter_time<int>();
								int delayed_time = (scenario->template current_time<int>() - enter_time) - inbound_link->template link_fftt<float>();
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
								
								outbound_link->template push_vehicle<_Vehicle_Interface*>(vehicle);
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
								inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(FLT_MAX);
							}
							else
							{
								if(inbound_movement->template movement_demand<float>() == 0)
								{//no demand
									inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
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
										inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
									}
								}
							}
						}
					}
				}
				
				//network_cumulative_arrived_vehicles=arrived_vehicles;
				//network_in_network_vehicles=in_network_vehicles;
			}

			feature_prototype void network_state_update()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, int, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Movement_Interface::get_type_of(cached_turn_movement_cumulative_shifted_arrived_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();
				int current_time = scenario->template current_time<int>();



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
						
						//turn movement delayed time update
						inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_cached_delay] = inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<int>();
						
						inbound_movement->template turn_travel_penalty<float>(inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>());
						
						float turn_travel_penalty = 0.0;

						for (int t_cached_time=0;t_cached_time<scenario->template num_simulation_intervals_per_assignment_interval<int>();t_cached_time++)
						{
							turn_travel_penalty += inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_cached_time];
						}

						turn_travel_penalty = (float) ( turn_travel_penalty/((float)scenario->template num_simulation_intervals_per_assignment_interval<int>()) );
						
						inbound_movement->template forward_link_turn_travel_time<float>(outbound_link->template travel_time<float>()+inbound_movement->template turn_travel_penalty<float>());
					}
				}
			}
			
			feature_prototype void Initialize()
			{
				load_event(ComponentType,Newells_Conditional,Compute_Step_Flow,0,NULLTYPE);
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
					
				//PRINT("\n" << iteration << "." << sub_iteration << ":\t" << "visiting intersection: " << _this_ptr->template uuid<int>());
				
				long intersection_current_revision=_pthis->object_current_revision();

				
				typedef Link_Components::Types::Link_Simulation_Status link_simulation_status_type;


				if(intersection_current_revision!=_iteration)
				{
					//first visit this iteration, update status
					_this_ptr->template intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::NONE_COMPLETE);
				}

				if(_this_ptr->template intersection_simulation_status<Types::Intersection_Simulation_Status>()==Types::Intersection_Simulation_Status::NONE_COMPLETE)
				{
					long links_current_revision=_Link_Interface::Component_Type::singleton_reference->type_current_revision();
					//Revision links_current_revision=Execution_Object::allocator_template<link_type>::allocator_reference.type_current_revision();

					if(links_current_revision==_iteration)
					{
						//links visited at least once, link_simulation_status is accurate

						//necessary condition is that all intersection's downstream/upstream links are done

						_Link_Interface* outbound_link;
						_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=_this_ptr->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
						typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

						bool done=true;

						for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
						{
							outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();

							_Link_Interface* inbound_link;
							_Movement_Interface* inbound_movement;
							_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
							
							typename _Movements_Container_Interface::iterator inbound_itr;

							for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
							{
								inbound_movement=(_Movement_Interface*)(*inbound_itr);
								inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();

								done=done &&
									(inbound_link->template link_simulation_status<link_simulation_status_type>()
									 == link_simulation_status_type::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE);

								if(!done) break;
							}
							
							if(!done) break;
						}

						if(done)
						{
							//PRINT("\t" << "Run Compute_Step_Flow, Return This Iteration");
							
							_pthis->Swap_Event((Event)&Intersection_Prototype::Compute_Step_Flow<NULLTYPE>);
							response.result=true;
							response.next=_iteration;
						}
						else
						{
							//PRINT("\t" << "Compute_Step_Flow_Supply_Update Not Finished, Return This Iteration");
							response.result=false;
							response.next=_iteration;
						}
					}
					else
					{
						//link not visited yet

						//PRINT("\t" << "Link Not Visited, Return This Iteration");
						response.result=false;
						response.next=_iteration;
					}
				}
				else if(_this_ptr->template intersection_simulation_status<Types::Intersection_Simulation_Status>()==Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE)
				{
					//although not ideal, simply check whether links are completely done this iteration
					long link_next_revision=_Link_Interface::Component_Type::singleton_reference->type_next_check();
					//Revision link_next_revision=Execution_Object::allocator_template<link_type>::allocator_reference.type_next_check();

					if(link_next_revision>_iteration)
					{
						//PRINT("\t" << "Run Network_State_Update, Return Next Iteration");
						_pthis->Swap_Event((Event)&Intersection_Prototype::Network_State_Update<NULLTYPE>);
						response.result=true;
						response.next=_iteration + _this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();
					}
					else
					{
						//PRINT("\t" << "Compute_Step_Flow_Link_Moving Not Finished, Return This Iteration");
						response.result=false;
						response.next=_iteration;
					}
				}

			}

			declare_feature_event(Compute_Step_Flow)
			{
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;

				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;

				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this_ptr->template turn_movement_capacity_update<NULLTYPE>(); 

				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				_this_ptr->template turn_movement_supply_allocation<NULLTYPE>();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this_ptr->template turn_movement_flow_calculation<NULLTYPE>();

				//step 6: node transfer
				_this_ptr->template node_transfer<NULLTYPE>();

				_this_ptr->template intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE);

				//PRINT("\t\t" << "COMPUTE_STEP_FLOW_COMPLETE");
			}

			declare_feature_event(Network_State_Update)
			{
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;

				Intersection_Prototype* _this_ptr=(Intersection_Prototype*)_this;

				//step 9: intersection network state update
				_this_ptr->template network_state_update<NULLTYPE>();

				_this_ptr->template intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::NETWORK_STATE_UPDATE_COMPLETE);

				//PRINT("\t\t" << "NETWORK_STATE_UPDATE_COMPLETE");
			}
		};
	}
}

using namespace Intersection_Components::Prototypes;