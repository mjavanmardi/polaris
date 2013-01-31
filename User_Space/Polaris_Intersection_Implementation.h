#pragma once
#include "User_Space_Includes.h"
#include "Link_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Polaris_Link_Implementation.h"
#include "Intersection_Prototype.h"
#include "Turn_Movement_Prototype.h"

namespace Intersection_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Outbound_Inbound_Movements_Implementation:public Polaris_Component_Class<Polaris_Outbound_Inbound_Movements_Implementation,MasterType,Data_Object,ParentType>
		{
			// pointer to the outbound link
			member_component(typename MasterType::link_type, outbound_link_reference, none, none);
			// container of inbound movements
			member_container(vector<typename MasterType::movement_type*>, inbound_movements, none, none);
			
			// update state of outbound link and inbound movements
			feature_implementation void update_state()
			{
				define_component_interface(_Link_Interface, get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type, ComponentType> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;

				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				((_Link_Interface*)_outbound_link_reference)->template travel_time<float>(((_Link_Interface*)_outbound_link_reference)->template link_fftt<float>());
				int t_fftt = -1;
				int t_bwtt = -1;
				int t_cached_delay = -1;

				if (current_simulation_interval_index>0)
				{
					t_fftt = (current_simulation_interval_index)%((_Link_Interface*)_outbound_link_reference)->template link_fftt_cached_simulation_interval_size<int>();
					t_bwtt = (current_simulation_interval_index)%((_Link_Interface*)_outbound_link_reference)->template link_bwtt_cached_simulation_interval_size<int>();
					t_cached_delay = (current_simulation_interval_index)%((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();
				}
				else
				{
					t_fftt = 0;
					t_bwtt = 0;
					t_cached_delay = 0;
				}
				_Movement_Interface* inbound_movement;
				typename _Movements_Container_Interface::iterator inbound_itr;
				for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
				{
					inbound_movement=(_Movement_Interface*)(*inbound_itr);
					inbound_movement->update_state<int>(t_cached_delay);
				}
			}

			// allocate supply based on driving rule
			feature_implementation void supply_allocation_based_on_driving_rule()
			{
				define_component_interface(_Link_Interface, get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);

				_Movement_Interface* inbound_movement;
				_Link_Interface* inbound_link;
				typename _Movements_Container_Interface::iterator inbound_itr;

				int inbound_turn_movement_size=(int)_inbound_movements.size();
				if(inbound_turn_movement_size >= 1)
				{
					if (inbound_turn_movement_size == 1)
					{
						((_Movement_Interface*)_inbound_movements[0])->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>());
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

						for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
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
							if (total_transfer_demand<=((_Link_Interface*)_outbound_link_reference)->template link_supply<float>())
							{//case 1: total_transfer_demand<= link_supply_array
								for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
								{
									inbound_movement=(_Movement_Interface*)(*inbound_itr);
									inbound_movement->template movement_supply<float>(inbound_movement->template movement_demand<float>());
								}
							}
							else
							{//case 2: total_transfer_demand<= link_supply_array
								if (protected_turn_movement != nullptr)
								{
									if (protected_turn_movement->template movement_demand<float>() >= ((_Link_Interface*)_outbound_link_reference)->link_supply<float>())
									{
										protected_turn_movement->movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->link_supply<float>());
										for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
										{
											inbound_movement=(_Movement_Interface*)(*inbound_itr);
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
										float link_supply_leftover = ((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() - protected_turn_movement->template movement_supply<float>();
								
										for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
										{
											inbound_movement=(_Movement_Interface*)(*inbound_itr);
											if (inbound_movement->template internal_id<int>() != protected_turn_movement->template internal_id<int>())
											{
												inbound_movement->template movement_supply<float>(link_supply_leftover * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
											}
										}
									}
								}
								else
								{
									for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
									{
										inbound_movement=(_Movement_Interface*)(*inbound_itr);
										inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
									}
										
								}
							}
						}
						else
						{
							for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
							{
								inbound_movement=(_Movement_Interface*)(*inbound_itr);
						
								if (inbound_movement->template movement_rule<int>() == ALLOWED)
								{
									inbound_movement->movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() / (num_allowed_movements*1.0f));
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

			// compute total transfer demand
			feature_implementation float compute_total_transfer_demand()
			{
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);

				float total_transfer_demand = 0.0f;
				typename _Movements_Container_Interface::iterator inbound_itr;
				for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
				{
					total_transfer_demand += ((_Movement_Interface*)(*inbound_itr))->template movement_demand<float>();
				}
				return total_transfer_demand;
			}

			// allocate supply proprotional to demand
			feature_implementation void supply_allocation_proportion_to_demand()
			{
				define_component_interface(_Link_Interface, get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);

				_Movement_Interface* inbound_movement;

				typename _Movements_Container_Interface::iterator inbound_itr;

				int inbound_turn_movement_size=(int)_inbound_movements.size();
				if (inbound_turn_movement_size == 1)
				{//only one inbound turn movement - allocate full space of outbound link to inbound turn movement
					inbound_movement = (_Movement_Interface*)(*_inbound_movements.begin());
					inbound_movement->movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>());
				}
				else
				{//divide space based on merging policy
					///propotional to demand
					float total_transfer_demand = compute_total_transfer_demand<ComponentType,CallerType,float>();
					for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						if (total_transfer_demand>0)
						{
							if (total_transfer_demand<=((_Link_Interface*)_outbound_link_reference)->template link_supply<float>())
							{//case 1: total_transfer_demand<= link_supply_array
								inbound_movement->template movement_supply<float>(inbound_movement->template movement_demand<float>());
							}
							else
							{//case 2: total_transfer_demand<= link_supply_array
								inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
							}
						}
						else
						{
							inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->link_supply<float>() / (inbound_turn_movement_size*1.0f));
						}	
					}
				}
			}
		};
	
		implementation struct Polaris_Inbound_Outbound_Movements_Implementation:public Polaris_Component_Class<Polaris_Inbound_Outbound_Movements_Implementation,MasterType,Data_Object,ParentType>
		{
			member_component(typename MasterType::link_type, inbound_link_reference, none, none);
			member_container(vector<typename MasterType::movement_type*>, outbound_movements, none, none);
		};
		
		implementation struct Polaris_Intersection_Implementation:public Polaris_Component_Class<Polaris_Intersection_Implementation,MasterType,Execution_Object,ParentType>
		{
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, z_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(Intersection_Components::Types::Intersection_Type_Keys, intersection_type, none, none);
			member_container(vector<typename MasterType::link_type*>, inbound_links, none, none);
			member_container(vector<typename MasterType::link_type*>, outbound_links, none, none);
			member_container(vector<typename MasterType::outbound_inbound_movements_type*>, outbound_inbound_movements, none, none);
			member_container(vector<typename MasterType::inbound_outbound_movements_type*>, inbound_outbound_movements, none, none);
			member_data(User_Space::RngStream, rng_stream, none, none);
			member_component(typename MasterType::network_type, network_reference, none, none);
			member_component(typename MasterType::intersection_control_type, intersection_control, none, none);

			typedef typename MasterType::vehicle_type vehicle_type;
//			member_component(typename MasterType::SIGNAL_TYPE,signal, none, none);

			// transfer vehicles 
			feature_implementation void node_transfer()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface,  _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				_Link_Interface* outbound_link;
				for(outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					outbound_link->template link_upstream_arrived_vehicles<int>(0.0);
					outbound_link->template link_destination_arrived_vehicles<int>(0.0);
					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);
						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						if(outbound_itr==_outbound_inbound_movements.begin())
						{
							//initialization
							inbound_link->template link_downstream_departed_vehicles<int>(0.0);
						}
						inbound_movement->template transfer_vehicles<User_Space::RngStream&>(_rng_stream);
					}
				}
			}

			// accept a vehicle
			feature_implementation void accept_vehicle(void* vehicle)
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, _Movement_Interface::get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);				

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				_Link_Interface* outbound_link;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
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
						_Movement_Plan_Interface* mp = ((_Vehicle_Interface*)vehicle)->template movement_plan<_Movement_Plan_Interface*>();
						if(mp->template next_link<_Link_Interface*>()==outbound_link && mp->template current_link<_Link_Interface*>()==inbound_link)
						{
							//_Detector_Interface* detector;
							//detector = inbound_movement->template detector<_Detector_Interface*>();
							//if (detector != NULL) detector->template detect_vehicle<int>();
							inbound_movement->template accept_vehicle<void*>(vehicle);
						}
					}
				}
			}

			// allocate supply among turn movements
			feature_implementation void turn_movement_supply_allocation()
			{
				define_component_interface(_Intersection_Control_Interface, type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_component_interface(_Control_Plan_Interface, _Intersection_Control_Interface::get_type_of(current_control_plan), Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);

				Node_Type_Keys control_type = ((_Intersection_Control_Interface*)_intersection_control)->template current_control_plan<_Control_Plan_Interface*>()->template control_type<Node_Type_Keys>();
				switch(control_type)
				{
					case NO_CONTROL:
						supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
						break;
					case YIELD_SIGN:
						supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
						break;
					case ALL_WAY_STOP_SIGN:
						supply_allocation_proportion_to_demand<ComponentType,CallerType,TargetType>();
						break;
					case TWO_WAY_STOP_SIGN:
						supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
						break;
					case PRE_TIMED_SIGNAL_CONTROL:
						supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
						break;
					case ACTUATED_SIGNAL_CONTROL:
						supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
						break;
					case ADAPTIVE_SIGNAL_CONTROL:
						supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
						break;
				}
			}

			// allocate supply based on driving rule
			feature_implementation void supply_allocation_based_on_driving_rule()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_based_on_driving_rule<NULLTYPE>();
				}
			}

			// allocate supply proportional to demand
			feature_implementation void supply_allocation_proportion_to_demand()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_proportion_to_demand<NULLTYPE>();
				}
			}

			// load vehicles to their origin link
			feature_implementation void origin_link_loading()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);

				_Link_Interface* outbound_link;
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					outbound_link->origin_link_loading<User_Space::RngStream&>(_rng_stream);
				}
			}

			feature_implementation void initialize_features(void* network)
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Movements_Container_Interface, _Inbound_Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;

				_network_reference = (typename MasterType::network_type*)network;
				unsigned long seed = ((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>()+_internal_id+1;
				_rng_stream.SetSeed(seed);

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=_outbound_inbound_movements.begin();outbound_inbound_movements_itr!=_outbound_inbound_movements.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						((_Inbound_Movement_Interface*)(*inbound_movement_itr))->initialize_features<NULLTYPE>();
						//// put this in movement
						//inbound_movement->template movement_capacity<float>(0.0);
						//inbound_movement->template movement_demand<float>(0.0);
						//inbound_movement->template movement_supply<float>(0.0);
						//inbound_movement->template movement_flow<float>(0.0);
						//inbound_movement->template movement_transferred<int>(0.0);
						//inbound_movement->template turn_movement_cumulative_vehicles<int>(0.0);
						//inbound_movement->template turn_movement_cumulative_arrived_vehicles<int>(0.0);
						//inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>(0.0);
						//	
						////inbound_movement->template vehicles_container<_Vehicles_Queue_Interface&>().clear();
						//	
						//inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0.0);
						//inbound_movement->template inbound_link_departed_time_based_experienced_link_turn_travel_delay<float>(0.0);
						//inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().clear();
						//inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().clear();
						//inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().resize(((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());
						//inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().resize(((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());
						//int j;

						//for (j=0;j<scenario_reference<ComponentType,CallerType,_Scenario_Interface*>()->template num_simulation_intervals_per_assignment_interval<int>();j++)
						//{
						//	inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[j] = 0.0;
						//	inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[j] = 0.0;
						//}
						//			
						//_Link_Interface* inbound_link = inbound_movement->template inbound_link<_Link_Interface*>();
						//inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>().clear();
						//inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>().resize(inbound_link->template link_fftt_cached_simulation_interval_size<int>());
						//for (j=0;j<inbound_link->template link_fftt_cached_simulation_interval_size<int>();j++)
						//{
						//	inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>()[j] = 0; 
						//}

						//if (inbound_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>() == Turn_Movement_Components::Types::THROUGH_TURN)
						//{
						//	inbound_movement->template minimum_merge_rate<float>(1.0);
						//}
						//else
						//{
						//	inbound_movement->template minimum_merge_rate<float>(0.2);
						//}
					}
				}
				}


			feature_implementation void Initialize()
			{
				define_component_interface(_Intersection_Control_Interface, type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				if (_outbound_inbound_movements.size() > 0 && _inbound_outbound_movements.size() > 0)
				{
					((_Intersection_Control_Interface*)_intersection_control)->template Initialize<NULLTYPE>();
					int start_iteration = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1;
					load_event(ComponentType,Newells_Conditional,Compute_Step_Flow,start_iteration,Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION,NULLTYPE);
				}
			}
			
			declare_feature_conditional(Newells_Conditional)
			{
				typedef Intersection_Prototype<typename MasterType::intersection_type> _Intersection_Interface;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				ComponentType* _pthis = (ComponentType*)_this;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;
				
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION)
				{
					((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Compute_Step_Flow<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION)
				{

					((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Network_State_Update<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION;
				} 
				else
				{
					assert(false);
					cout << "Should never reach here in intersection conditional!" << endl;
				}
			}

			declare_feature_event(Compute_Step_Flow)
			{
				typedef Intersection_Prototype<typename MasterType::intersection_type> _Intersection_Interface;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;

				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this_ptr->template turn_movement_capacity_update<NULLTYPE>(); 

				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				_this_ptr->template turn_movement_supply_allocation<NULLTYPE>();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this_ptr->template turn_movement_flow_calculation<NULLTYPE>();

				//step 5: node transfer
				_this_ptr->template node_transfer<NULLTYPE>();

				//step 6: origin link loading
				_this_ptr->template origin_link_loading<NULLTYPE>();

			}

			declare_feature_event(Network_State_Update)
			{
				typedef Intersection_Prototype<typename MasterType::intersection_type> _Intersection_Interface;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;
				//step 9: intersection network state update
				_this_ptr->template network_state_update<NULLTYPE>();
			}
		};
	}
}
