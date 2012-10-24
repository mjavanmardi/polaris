#pragma once
#include "Repository_User_Includes.h"
#include "Signal_Prototypes.h"
#include "Link_Prototype.h"
#include "Vehicle_Prototype.h"

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

		enum Intersection_Simulation_Status
		{
			NONE_COMPLETE,
			COMPUTE_STEP_FLOW_COMPLETE,
			NETWORK_STATE_UPDATE_COMPLETE
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		template<typename ComponentType,typename CallerType>
		struct Movement_Prototype
		{
			feature_accessor(detector);
			feature_accessor(id);
			feature_accessor(inbound_link);
			feature_accessor(outbound_link);

			feature_accessor(movement_capacity);
			feature_accessor(movement_supply);
			feature_accessor(movement_demand);
			feature_accessor(movement_flow);
			feature_accessor(movement_transferred);

			feature_accessor(movement_type);
			feature_accessor(movement_rule);

			feature_accessor(turn_travel_penalty);

			feature_accessor(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array);
			feature_accessor(cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array);
			feature_accessor(cached_outbound_link_departed_time_based_experienced_link_turn_travel_delay_array);
			feature_accessor(inbound_link_departed_time_based_experienced_link_turn_travel_delay);
			feature_accessor(outbound_link_arrived_time_based_experienced_link_turn_travel_delay);
			
			feature_accessor(turn_movement_cumulative_arrived_vehicles);
			feature_accessor(turn_movement_cumulative_vehicles);
			feature_accessor(cached_turn_movement_cumulative_shifted_arrived_vehicles_array);
			feature_accessor(turn_movement_cumulative_shifted_arrived_vehicles);
			
			feature_accessor(forward_link_turn_travel_time);
			
			feature_accessor(replicas_container);

			feature_accessor(vehicles_container);


			feature TargetType pull_vehicle()
			{
				return offer_vehicle<TargetType>();
			}
			
			feature TargetType offer_vehicle()
			{

				define_container_and_value_interface_local(Polaris_Back_Insertion_Sequence_Prototype, vehicles_container, _Vehicles_Container_Interface, Vehicle_Prototype, _Vehicle_Interface, ComponentType);

				_Vehicle_Interface* veh=(_Vehicle_Interface*)vehicles_container<_Vehicles_Container_Interface&>().front();
				vehicles_container<_Vehicles_Container_Interface&>().pop_front();

				return (TargetType)veh;
			}

			feature_accessor(minimum_merge_rate);
		};

		template<typename ComponentType,typename CallerType>
		struct Outbound_Inbound_Movements_Prototype
		{
			feature_accessor(outbound_link_reference);
			feature_accessor(inbound_movements);
		};

		template<typename ComponentType,typename CallerType>
		struct Inbound_Outbound_Movements_Prototype
		{
			feature_accessor(inbound_link_reference);
			feature_accessor(lane_groups);
			feature_accessor(outbound_movements);
		};

		template<typename ComponentType,typename CallerType>
		struct Intersection_Prototype
		{
			typedef ComponentType Component_Type;
			feature_accessor(uuid);

			feature_accessor(signal);

			feature_accessor(rng_stream);

			feature_accessor(inbound_links);
			feature_accessor(outbound_links);

			feature_accessor(inbound_outbound_movements);
			feature_accessor(outbound_inbound_movements);

			feature_accessor(x_position);
			feature_accessor(y_position);
			feature_accessor(z_position);

			feature_accessor(intersection_type);

			feature_accessor(scenario_reference);

			feature_accessor(network);
			

			feature_accessor(intersection_simulation_status);

			//feature TargetType pull_vehicle()
			//{
			//	//PTHIS(ComponentType)->offer_vehicle<Dispatch<ComponentType>,TargetType>();
			//}

			feature void push_vehicle(void* vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}
			
			feature void accept_vehicle(void* vehicle)
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);

				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype,outbound_inbound_movements,_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Outbound_Inbound_Movements_Interface,ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Outbound_Inbound_Movements_Interface_type::outbound_link_reference, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Outbound_Inbound_Movements_Interface_type::inbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);
				define_container_and_value_interface(Polaris_Back_Insertion_Sequence_Prototype, _Movement_Interface_type::vehicles_container, _Vehicles_Container_Interface, Vehicle_Prototype, _Vehicle_Interface, ComponentType);
				define_component_interface(_Intersection_Interface, Intersection_Prototype, _Link_Interface_type::upstream_intersection, ComponentType);
				define_component_interface(_Detector_Interface, Signal_Components::Prototypes::Detector_Prototype, _Outbound_Inbound_Movements_Interface_type::detector, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();


				_Link_Interface* outbound_link;

				//PRINT(uuid<int>());

				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->outbound_link_reference<_Link_Interface*>();

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;

					_Movements_Container_Interface& inbound_movements = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->inbound_movements<_Movements_Container_Interface&>();
					_Movements_Container_Interface::iterator inbound_itr;

					for(inbound_itr=inbound_movements.begin();inbound_itr!=inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<_Link_Interface*>();

						//PRINT(((Vehicle_Interface*)vehicle)->next_link<Link_Interface*>()->uuid<int>() << "," << outbound_link->uuid<int>());
						//PRINT(((Vehicle_Interface*)vehicle)->current_link<Link_Interface*>()->uuid<int>() << "," << inbound_link->uuid<int>());

						if(((_Vehicle_Interface*)vehicle)->next_link<_Link_Interface*>()==outbound_link && ((_Vehicle_Interface*)vehicle)->current_link<_Link_Interface*>()==inbound_link)
						{
							
							_Detector_Interface* detector;
							detector = inbound_movement->detector<_Detector_Interface*>();
							if (detector != NULL) detector->detect_vehicle<int>();

							_Vehicle_Interface* veh = (_Vehicle_Interface*)vehicle;
							inbound_movement->vehicles_container<_Vehicles_Container_Interface&>().push_back(veh);
							inbound_movement->turn_movement_cumulative_arrived_vehicles<int&>()++;

							if (outbound_link->uuid<int>() == 2 && inbound_link->uuid<int>() == 1)
							{
								bool test = false;
							}

							char s[100];
							sprintf_s(s, "stop:%i:%i:%i\n", inbound_link->uuid<int>(), this->uuid<int>(), scenario_reference<_Scenario_Interface*>()->current_time<int>());
							std::string s1 = s;
							scenario_reference<_Scenario_Interface*>()->output<NULLTYPE>(s1);
							sprintf_s(s, "advanced:%i:%i:%i\n", outbound_link->uuid<int>(), outbound_link->downstream_intersection<_Intersection_Interface*>()->uuid<int>(), scenario_reference<_Scenario_Interface*>()->current_time<int>());
							std::string s2 = s;
							scenario_reference<_Scenario_Interface*>()->output<NULLTYPE>(s2);
						}
					}
				}
			}

			feature void turn_movement_capacity_update()
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype,outbound_inbound_movements,_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Outbound_Inbound_Movements_Interface,ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Outbound_Inbound_Movements_Interface_type::outbound_link_reference, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Outbound_Inbound_Movements_Interface_type::inbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);

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
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->outbound_link_reference<_Link_Interface*>();

					float outbound_link_capacity = (float)(scenario->simulation_interval_length<int>() * outbound_link->num_lanes<int>() * (outbound_link->maximum_flow_rate<float>()/3600.0));

					_Link_Interface* inbound_link;
					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					_Movement_Interface* inbound_movement;
					
					//PRINT("\t\t" << "for outbound link " << outbound_link->uuid<int>());

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<_Link_Interface*>();
						float inbound_link_capacity = inbound_link->link_capacity<float>(); 
						
						if(intersection_type<Types::Intersection_Type_Keys>() == Types::Intersection_Type_Keys::NO_CONTROL)
						{
							inbound_movement->movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}
						else
						{
							inbound_movement->movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}

						//PRINT("\t\t\t" << "for inbound link " << inbound_link->uuid<int>() << " is: " << inbound_movement->movement_capacity<float>());
					}
				}
			}

			feature void turn_movement_supply_allocation()
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype,outbound_inbound_movements,_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Outbound_Inbound_Movements_Interface,ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Outbound_Inbound_Movements_Interface_type::outbound_link_reference, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Outbound_Inbound_Movements_Interface_type::inbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);

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
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->outbound_link_reference<_Link_Interface*>();
					
					//PRINT("\t\t" << "for outbound link " << outbound_link->uuid<int>());

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					int inbound_turn_movement_size=(int)inbound_links_container.size();
					
					if(inbound_turn_movement_size==1)
					{
						((_Movement_Interface*)inbound_links_container[0])->movement_supply<float>(outbound_link->link_supply<float>());
					}
					else
					{
						///simple rule as Daganzo --- need to work on this later
						int num_lanes_outbound_link = outbound_link->num_lanes<int>();
						int link_type_outbound_link = outbound_link->link_type<int>();
						int num_merge_lanes = 0;

						int num_through_turns = 0;

						for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
						{
							inbound_movement=(_Movement_Interface*)(*inbound_itr);

							inbound_link=inbound_movement->inbound_link<_Link_Interface*>();

							int num_lanes_inbound_link = inbound_link->num_lanes<int>();
							int link_type_inbound_link = inbound_link->link_type<int>();
							
							if (link_type_inbound_link != link_type_outbound_link)
							{
								if (num_merge_lanes<num_lanes_inbound_link)
								{
									num_merge_lanes = num_lanes_inbound_link;
								}
							}
							if (inbound_movement->movement_type<Types::Turn_Movement_Type_Keys>() == Types::Turn_Movement_Type_Keys::THROUGH_TURN)
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

							inbound_link=inbound_movement->inbound_link<_Link_Interface*>();

							int link_type_inbound_link = inbound_link->link_type<int>();
							float merge_ratio = 0;

							if(link_type_inbound_link == link_type_outbound_link && inbound_movement->movement_type<Types::Turn_Movement_Type_Keys>() == Types::Turn_Movement_Type_Keys::THROUGH_TURN)
							{
								merge_ratio = major_approach_merge_ratio;
							}
							else
							{
								merge_ratio = minor_approach_merge_ratio;
							}

							inbound_movement->movement_supply<float>(merge_ratio*outbound_link->link_supply<float>());
							//PRINT("\t\t\t" << "for inbound link " << inbound_link->uuid<int>() << " is: " << inbound_movement->movement_supply<float>());
						}
						///simple rule as Daganzo
					}
				}
			}

			feature void turn_movement_flow_calculation()
			{

				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype,outbound_inbound_movements,_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Outbound_Inbound_Movements_Interface,ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Outbound_Inbound_Movements_Interface_type::outbound_link_reference, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Outbound_Inbound_Movements_Interface_type::inbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);
				define_simple_container_interface(Polaris_Random_Access_Sequence_Prototype, _Movement_Interface_type::cached_turn_movement_cumulative_shifted_arrived_vehicles_array, _Int_Container_Interface, int, ComponentType);


				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->outbound_link_reference<_Link_Interface*>();

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;

					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->inbound_movements<_Movements_Container_Interface&>();
					_Movements_Container_Interface::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<_Link_Interface*>();

						int t_minus_fftt = -1;

						int link_fftt_cached_simulation_interval = inbound_link->link_fftt_cached_simulation_interval_size<int>();

						if (current_simulation_interval_index >= link_fftt_cached_simulation_interval)
						{
							t_minus_fftt = (current_simulation_interval_index - link_fftt_cached_simulation_interval) % link_fftt_cached_simulation_interval;			
						}

						if (t_minus_fftt > -1)
						{
							int cached_turn_movement_cumulative_shifted_arrived_vehicles=(inbound_movement->cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>())[t_minus_fftt];
							inbound_movement->turn_movement_cumulative_shifted_arrived_vehicles<int>(cached_turn_movement_cumulative_shifted_arrived_vehicles);
						}
						else
						{
							inbound_movement->turn_movement_cumulative_shifted_arrived_vehicles<int>(0);
						}
						
						int movement_demand = inbound_movement->turn_movement_cumulative_shifted_arrived_vehicles<int>() - inbound_movement->turn_movement_cumulative_vehicles<int>();
						
						inbound_movement->movement_demand<float>(movement_demand);

						if(inbound_movement->movement_demand<float>() < 0.0) 
						{
							inbound_movement->movement_demand<float>(0);
						}

						float turn_movement_demand = inbound_movement->movement_demand<float>();
						float turn_movement_capacity = inbound_movement->movement_capacity<float>();
						float turn_movement_supply =  inbound_movement->movement_supply<float>();
						float turn_movement_flow = (float) min(min((double)turn_movement_demand,(double)turn_movement_capacity),(double)turn_movement_supply);
						inbound_movement->movement_flow<float>(turn_movement_flow);
						//PRINT("\t" << iteration << ": turn movement flow: " << turn_movement_flow);
					}
				}
			}

			feature void node_transfer()
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype,outbound_inbound_movements,_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Outbound_Inbound_Movements_Interface,ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Outbound_Inbound_Movements_Interface_type::outbound_link_reference, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Outbound_Inbound_Movements_Interface_type::inbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);
				define_simple_container_interface(Polaris_Random_Access_Sequence_Prototype, _Movement_Interface_type::cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, _Float_Container_Interface, int, ComponentType);
				typedef Vehicle_Prototype<ComponentType::vehicle_type, ComponentType> _Vehicle_Interface;

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();


				// apply transfers of y(a,a''t) for each turn;
				//int arrived_vehicles = this->network_cumulative_arrived_vehicles;
				//int in_network_vehicles = this->network_in_network_vehicles;
				
				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->outbound_link_reference<_Link_Interface*>();

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;

					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					
					//initialization -> move to network state update
					outbound_link->link_upstream_arrived_vehicles<int>(0);
					outbound_link->link_destination_arrived_vehicles<int>(0);

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<_Link_Interface*>();

						inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
						inbound_movement->inbound_link_departed_time_based_experienced_link_turn_travel_delay<float>(0);

						if(outbound_itr==outbound_links_container.begin())
						{
							//initialization
							inbound_link->link_downstream_departed_vehicles<int>(0);
						}

						//determine number of transfered vehicles
						float transfer_flow_turn_movement = inbound_movement->movement_flow<float>();
						
						int num_transfer_vehicles_of_turn_movement = (int)transfer_flow_turn_movement;
						transfer_flow_turn_movement = transfer_flow_turn_movement - num_transfer_vehicles_of_turn_movement;
						
						if(transfer_flow_turn_movement > 0.0)
						{
							double rng = (this->rng_stream<RngStream&>()).RandU01();
							if(rng<=transfer_flow_turn_movement)
							{//partial vehicle, incomplete implementation
								++num_transfer_vehicles_of_turn_movement;
							}
						}

						inbound_movement->movement_transferred<int>(num_transfer_vehicles_of_turn_movement);
						
						if (num_transfer_vehicles_of_turn_movement>0)
						{

							
							//move vehicles for this turn movement
							for (int iv=0;iv<num_transfer_vehicles_of_turn_movement;iv++)
							{
								_Vehicle_Interface* vehicle=inbound_movement->pull_vehicle<_Vehicle_Interface*>();

								//update vehicle state: transfer to next link
								int enter_time=vehicle->current_link_enter_time<int>();
								int delayed_time = (scenario->current_time<int>() - enter_time) - inbound_link->link_fftt<float>();
								int enter_interval_index = vehicle->current_link_enter_interval_index<int>();
								int delayed_interval = current_simulation_interval_index - enter_interval_index;

								//update inbound link state: N(a',L,t)
								inbound_link->link_downstream_cumulative_vehicles<int&>()++;
								inbound_link->link_downstream_departed_vehicles<int&>()++;

								outbound_link->link_upstream_arrived_vehicles<int&>()++;
								outbound_link->link_upstream_cumulative_arrived_vehicles<int&>()++;
								//update turn movement state: N(a',a,t)
								inbound_movement->turn_movement_cumulative_vehicles<int&>()++;

								//update link_turn_travel_delay
								inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float&>()+=delayed_time;

								//find the departed time
								int departed_time_position = -1;
								
								int num_intervals=scenario->num_simulation_intervals_per_assignment_interval<int>();

								if (current_simulation_interval_index>=num_intervals)
								{
									departed_time_position = (current_simulation_interval_index+num_intervals-delayed_interval)%num_intervals;
								}
								else
								{
									departed_time_position = enter_interval_index;
								}

								inbound_movement->cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[departed_time_position]+=delayed_interval*simulation_interval_length;
								
								outbound_link->push_vehicle<_Vehicle_Interface*>(vehicle);
							}
						}

						if (num_transfer_vehicles_of_turn_movement>0)
						{
							float delay=inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>()/((float)num_transfer_vehicles_of_turn_movement);
							inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(delay);
						}
						else
						{
							if(inbound_movement->movement_rule<Types::Turn_Movement_Rule_Keys>() == Types::Turn_Movement_Rule_Keys::PROHIBITED)
							{
								inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(FLT_MAX);
							}
							else
							{
								if(inbound_movement->movement_demand<float>() == 0)
								{//no demand
									inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
								}
								else
								{//no supply for this turn -- need a prediction -- need to work it later
									int t_minus_one = -1;
									if (current_simulation_interval_index>0)
									{
										t_minus_one = (current_simulation_interval_index-1)%scenario->num_simulation_intervals_per_assignment_interval<int>();

										float delay = simulation_interval_length +
											inbound_movement->cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_minus_one];

										inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(delay);
									}
									else
									{
										inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
									}
								}
							}
						}
					}
				}
				
				//network_cumulative_arrived_vehicles=arrived_vehicles;
				//network_in_network_vehicles=in_network_vehicles;
			}

			feature void network_state_update()
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype,outbound_inbound_movements,_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Outbound_Inbound_Movements_Interface,ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Outbound_Inbound_Movements_Interface_type::outbound_link_reference, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Outbound_Inbound_Movements_Interface_type::inbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);
				define_simple_container_interface(Polaris_Random_Access_Sequence_Prototype, _Movement_Interface_type::cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, _Float_Container_Interface, int, ComponentType);
				define_simple_container_interface(Polaris_Random_Access_Sequence_Prototype, _Movement_Interface_type::cached_turn_movement_cumulative_shifted_arrived_vehicles_array, _Int_Container_Interface, int, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();



				//network_data.network_cumulative_arrived_vehicles = this->network_cumulative_arrived_vehicles;
				//network_data.network_cumulative_departed_vehicles = this->network_cumulative_departed_vehicles;
				//network_data.network_cumulative_loaded_vehicles = this->network_cumulative_loaded_vehicles;
				//network_data.network_in_network_vehicles = this->network_in_network_vehicles;


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->outbound_link_reference<_Link_Interface*>();
					
					outbound_link->travel_time<float>(outbound_link->link_fftt<float>());

					int t_fftt = -1;
					int t_bwtt = -1;
					int t_cached_delay = -1;

					if (current_simulation_interval_index>0)
					{
						t_fftt = (current_simulation_interval_index)%outbound_link->link_fftt_cached_simulation_interval_size<int>();
						t_bwtt = (current_simulation_interval_index)%outbound_link->link_bwtt_cached_simulation_interval_size<int>();
						t_cached_delay = (current_simulation_interval_index)%scenario->num_simulation_intervals_per_assignment_interval<int>();
					}
					else
					{
						t_fftt = 0;
						t_bwtt = 0;
						t_cached_delay = 0;
					}

					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;

					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<_Link_Interface*>();

						int t_plus_fftt = -1;

						if (current_simulation_interval_index>0)
						{
							t_plus_fftt = (current_simulation_interval_index) % (inbound_link->link_fftt_cached_simulation_interval_size<int>());
						}
						else
						{
							t_plus_fftt = 0;
						}

						//cached cumulative shifted arrived vehicles
						inbound_movement->cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>()[t_plus_fftt] = inbound_movement->turn_movement_cumulative_arrived_vehicles<int>();
						
						//turn movement delayed time update
						inbound_movement->cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_cached_delay] = inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<int>();
						
						inbound_movement->turn_travel_penalty<float>(inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>());
						
						float turn_travel_penalty = 0.0;

						for (int t_cached_time=0;t_cached_time<scenario->num_simulation_intervals_per_assignment_interval<int>();t_cached_time++)
						{
							turn_travel_penalty += inbound_movement->cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[t_cached_time];
						}

						turn_travel_penalty = (float) ( turn_travel_penalty/((float)scenario->num_simulation_intervals_per_assignment_interval<int>()) );
						
						inbound_movement->forward_link_turn_travel_time<float>(outbound_link->travel_time<float>()+inbound_movement->turn_travel_penalty<float>());
					}
				}
			}
			
			feature void Initialize()
			{
				schedule_event_local(ComponentType,Newells_Conditional,Compute_Step_Flow,0,NULLTYPE);
			}

			declare_feature_conditional(Newells_Conditional)
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype,outbound_inbound_movements,_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Outbound_Inbound_Movements_Interface,ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Outbound_Inbound_Movements_Interface_type::outbound_link_reference, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Outbound_Inbound_Movements_Interface_type::inbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;

				

				ComponentType* _pthis = (ComponentType*)_this;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;
					
				//PRINT("\n" << iteration << "." << sub_iteration << ":\t" << "visiting intersection: " << _this_ptr->uuid<int>());
				
				Revision intersection_current_revision=_pthis->object_current_revision();

				
				typedef Link_Components::Types::Link_Simulation_Status link_simulation_status_type;


				if(intersection_current_revision.iteration!=iteration)
				{
					//first visit this iteration, update status
					_this_ptr->intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::NONE_COMPLETE);
				}

				if(_this_ptr->intersection_simulation_status<Types::Intersection_Simulation_Status>()==Types::Intersection_Simulation_Status::NONE_COMPLETE)
				{
					Revision links_current_revision=_Link_Interface::Component_Type::singleton_reference->type_current_revision();
					//Revision links_current_revision=Execution_Object::allocator_template<link_type>::allocator_reference.type_current_revision();

					if(links_current_revision.iteration==iteration)
					{
						//links visited at least once, link_simulation_status is accurate

						//necessary condition is that all intersection's downstream/upstream links are done

						_Link_Interface* outbound_link;
						_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=_this_ptr->outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
						typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

						bool done=true;

						for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
						{
							outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->outbound_link_reference<_Link_Interface*>();

							_Link_Interface* inbound_link;
							_Movement_Interface* inbound_movement;
							_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->inbound_movements<_Movements_Container_Interface&>();
							
							typename _Movements_Container_Interface::iterator inbound_itr;

							for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
							{
								inbound_movement=(_Movement_Interface*)(*inbound_itr);
								inbound_link=inbound_movement->inbound_link<_Link_Interface*>();

								done=done &&
									(inbound_link->link_simulation_status<link_simulation_status_type>()
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
							response.next=iteration;
						}
						else
						{
							//PRINT("\t" << "Compute_Step_Flow_Supply_Update Not Finished, Return This Iteration");
							response.result=false;
							response.next=iteration;
						}
					}
					else
					{
						//link not visited yet

						//PRINT("\t" << "Link Not Visited, Return This Iteration");
						response.result=false;
						response.next=iteration;
					}
				}
				else if(_this_ptr->intersection_simulation_status<Types::Intersection_Simulation_Status>()==Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE)
				{
					//although not ideal, simply check whether links are completely done this iteration
					Revision link_next_revision=Component_Type::singleton_reference->type_next_check();
					//Revision link_next_revision=Execution_Object::allocator_template<link_type>::allocator_reference.type_next_check();

					if(link_next_revision.iteration>iteration)
					{
						//PRINT("\t" << "Run Network_State_Update, Return Next Iteration");
						_pthis->Swap_Event((Event)&Intersection_Prototype::Network_State_Update<NULLTYPE>);
						response.result=true;
						response.next=iteration + _this_ptr->scenario_reference<_Scenario_Interface*>()->simulation_interval_length<int>();
					}
					else
					{
						//PRINT("\t" << "Compute_Step_Flow_Link_Moving Not Finished, Return This Iteration");
						response.result=false;
						response.next=iteration;
					}
				}

			}

			declare_feature_event(Compute_Step_Flow)
			{
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;

				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;

				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this_ptr->turn_movement_capacity_update<NULLTYPE>(); 

				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				_this_ptr->turn_movement_supply_allocation<NULLTYPE>();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this_ptr->turn_movement_flow_calculation<NULLTYPE>();

				//step 6: node transfer
				_this_ptr->node_transfer<NULLTYPE>();

				_this_ptr->intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE);

				//PRINT("\t\t" << "COMPUTE_STEP_FLOW_COMPLETE");
			}

			declare_feature_event(Network_State_Update)
			{
				typedef Intersection_Prototype<ComponentType, ComponentType> _Intersection_Interface;

				Intersection_Prototype* _this_ptr=(Intersection_Prototype*)_this;

				//step 9: intersection network state update
				_this_ptr->network_state_update<NULLTYPE>();

				_this_ptr->intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::NETWORK_STATE_UPDATE_COMPLETE);

				//PRINT("\t\t" << "NETWORK_STATE_UPDATE_COMPLETE");
			}
		};
	}
}

using namespace Intersection_Components::Prototypes;