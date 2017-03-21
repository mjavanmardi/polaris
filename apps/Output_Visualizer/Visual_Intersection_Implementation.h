#pragma once
#include "Intersection_Prototype.h"
#include "Turn_Movement_Prototype.h"
#include "Scenario_Prototype.h"
#include "Intersection_Implementation.h"


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

		implementation struct Visual_Intersection_Implementation:public Polaris_Component<MasterType,INHERIT(Visual_Intersection_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Visual_Intersection_Implementation),Data_Object>::ComponentType ComponentType;

			m_data(int, area_type, NONE, NONE);


			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);
			m_data(float, x_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, y_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, z_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(Intersection_Components::Types::Intersection_Type_Keys, intersection_type, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, inbound_links, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, outbound_links, NONE, NONE);
			m_container(std::vector<typename MasterType::outbound_inbound_movements_type*>, outbound_inbound_movements, NONE, NONE);
			m_container(std::vector<typename MasterType::inbound_outbound_movements_type*>, inbound_outbound_movements, NONE, NONE);
			//m_data(RNG_Components::RngStream, rng_stream, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::network_type, network_reference, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::intersection_control_type, intersection_control, NONE, NONE);

			typedef typename MasterType::vehicle_type vehicle_type;
//			member_component(typename MasterType::SIGNAL_TYPE,signal, none, none);
			
			
			typedef Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
			typedef Random_Access_Sequence<type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

			typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Inbound_Movement_Interface;
			typedef Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Inbound_Movement_Interface*> _Inbound_Movements_Container_Interface;


			typedef Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer<typename  type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
			typedef Random_Access_Sequence<type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;

			typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Outbound_Movement_Interface;
			typedef Random_Access_Sequence< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Outbound_Movement_Interface*> _Outbound_Movements_Container_Interface;



			typedef Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Outbound_Movement_Interface::get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
			typedef Back_Insertion_Sequence< typename _Outbound_Movement_Interface::get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

			typedef Link_Components::Prototypes::Link< typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference)> _Link_Interface;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
			typedef Intersection_Control_Components::Prototypes::Intersection_Control<type_of(intersection_control)> _Intersection_Control_Interface;
			typedef Intersection_Control_Components::Prototypes::Control_Plan< typename _Intersection_Control_Interface::get_type_of(current_control_plan)> _Control_Plan_Interface;
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;

			typedef typename MasterType::outbound_inbound_movements_type _outbound_inbound_movements_component_type;
			typedef typename MasterType::inbound_outbound_movements_type _inbound_outbound_movements_component_type;
			typedef typename MasterType::link_type _link_component_type;
			typedef typename MasterType::turn_movement_type _movement_component_type;
			// transfer vehicles 
			template<typename TargetType> void node_transfer()
			{
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				_Link_Interface* outbound_link;
				for(outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Inbound_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Inbound_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_itr;
					outbound_link->template link_upstream_arrived_vehicles<int>(0.0);
					outbound_link->template link_destination_arrived_vehicles<int>(0.0);
					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Inbound_Movement_Interface*)(*inbound_itr);
						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						if(outbound_itr==_outbound_inbound_movements.begin())
						{
							//initialization
							inbound_link->template link_downstream_departed_vehicles<int>(0.0);
						}
						inbound_movement->template transfer_vehicles<RNG_Components::RngStream&>();
					}
				}
			}

			// accept a vehicle
			template<typename TargetType> void accept_vehicle(void* vehicle)
			{

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				_Link_Interface* outbound_link;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Inbound_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Inbound_Movements_Container_Interface& inbound_movements = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_itr;
					for(inbound_itr=inbound_movements.begin();inbound_itr!=inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(_Inbound_Movement_Interface*)(*inbound_itr);
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
			template<typename TargetType> void turn_movement_supply_allocation()
			{
				Types::Intersection_Type_Keys control_type = ((_Intersection_Control_Interface*)_intersection_control)->template current_control_plan<_Control_Plan_Interface*>()->template control_type<Types::Intersection_Type_Keys>();
				switch(control_type)
				{
					case Types::NO_CONTROL:
						//supply_allocation_proportion_to_link<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::YIELD_SIGN:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::ALL_WAY_STOP_SIGN:
						//supply_allocation_proportion_to_demand<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::TWO_WAY_STOP_SIGN:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::PRE_TIMED_SIGNAL_CONTROL:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::ACTUATED_SIGNAL_CONTROL:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::ADAPTIVE_SIGNAL_CONTROL:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
				}
			}

			// allocate supply based on driving rule
			template<typename TargetType> void supply_allocation_based_on_driving_rule()
			{
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_based_on_driving_rule<NULLTYPE>();
				}
			}

			// allocate supply proportional to demand
			template<typename TargetType> void supply_allocation_proportion_to_demand()
			{
				//typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				//typedef  Random_Access_Sequence< type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_proportion_to_demand<NULLTYPE>();
				}
			}

			// allocate supply proportional to number of links
			template<typename TargetType> void supply_allocation_proportion_to_link()
			{
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_proportion_to_link<NULLTYPE>();
				}
			}

			// allocate supply proportional to number of lanes
			template<typename TargetType> void supply_allocation_proportion_to_lane()
			{
				//typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				//typedef  Random_Access_Sequence< type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_proportion_to_lane<NULLTYPE>();
				}
			}

			template<typename TargetType> void supply_allocation_based_on_merging_model()
			{
				switch (((_Scenario_Interface*)_global_scenario)->template merging_mode<int>())
				{
				case Scenario_Components::Types::Merging_Mode_Keys::DRIVING_RULE:
					supply_allocation_based_on_driving_rule<TargetType>();
					break;
				case Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_DEMAND:
					supply_allocation_proportion_to_demand<TargetType>();
					break;
				case Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_LINK:
					supply_allocation_proportion_to_link<TargetType>();
					break;
				case Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_LANE:
					supply_allocation_proportion_to_lane<TargetType>();
					break;
				default:
					supply_allocation_based_on_driving_rule<TargetType>();
					break;
				}
			};

			template<typename TargetType> void link_capacity_allocation()
			{
				//typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				//typedef  Random_Access_Sequence< type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_itr;
				for (inbound_itr=_inbound_outbound_movements.begin(); inbound_itr!=_inbound_outbound_movements.end(); inbound_itr++)
				{
					((_Inbound_Outbound_Movements_Interface*)(*inbound_itr))->template link_capacity_allocation<NULLTYPE>();
				}
			}

			// load vehicles to their origin link
			template<typename TargetType> void origin_link_loading()
			{
				_Link_Interface* outbound_link;
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					outbound_link->template origin_link_loading<RNG_Components::RngStream&>();
				}
			}

			template<typename TargetType> void initialize_features(void* network)
			{
				_network_reference = (network_reference_type)network;
				//unsigned long seed = ((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>()+_internal_id+1;
				unsigned long seed = abs(std::sin(((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>() + (float)_internal_id + 1)*(float)INT_MAX);
				//unsigned long seed = 1;
				//_rng_stream.SetSeed(seed);

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=_outbound_inbound_movements.begin();outbound_inbound_movements_itr!=_outbound_inbound_movements.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						((_Inbound_Movement_Interface*)(*inbound_movement_itr))->template initialize_features<NULLTYPE>();
					}
				}
			}

			template<typename TargetType> void Initialize()
			{

				//if (_outbound_inbound_movements.size() > 0 && _inbound_outbound_movements.size() > 0)
				//{
				//	((_Intersection_Control_Interface*)_intersection_control)->template Initialize<NULLTYPE>();
				//	int start_iteration = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1;
				//	Load_Event<ComponentType>(&ComponentType::Newells_Conditional,start_iteration,Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION);
			}
			
			template<typename TargetType> void calculate_moe_for_simulation_interval()
			{

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=_outbound_inbound_movements.begin();outbound_inbound_movements_itr!=_outbound_inbound_movements.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);
					
					_Link_Interface* outbound_link=outbound_inbound_movements->template outbound_link_reference<_Link_Interface*>();
					_link_component_type* outbound_link_component = (_link_component_type*)((_outbound_inbound_movements_component_type*)(*outbound_inbound_movements_itr))->_outbound_link_reference;
					outbound_link_component->link_moe_data.link_queue_length += outbound_link_component->_link_num_vehicles_in_queue;
					outbound_link_component->realtime_link_moe_data.link_in_volume = 0.0f;
					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						((_Inbound_Movement_Interface*)(*inbound_movement_itr))->template calculate_moe_for_simulation_interval_from_outbound_link<NULLTYPE>();
					}

				}

				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_Link_Interface* inbound_link=inbound_outbound_movements->template inbound_link_reference<_Link_Interface*>();
					_link_component_type* inbound_link_component = (_link_component_type*)((_inbound_outbound_movements_component_type*)(*inbound_outbound_movements_itr))->_inbound_link_reference;

					float avg_turn_penalty = 0.0f;
					int allowed_movement_size = 0;
					int num_vehicles_in_link = 0;

					inbound_link_component->realtime_link_moe_data.link_out_volume = 0.0f;

					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template calculate_moe_for_simulation_interval_from_inbound_link<NULLTYPE>();
						_movement_component_type* outbound_movement_component = (_movement_component_type*)(*outbound_movement_itr);
						if (outbound_movement_component->_turn_travel_penalty < 0.01f*INFINITY_FLOAT)
						{
							avg_turn_penalty += ((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>();
							allowed_movement_size++;
						}
						num_vehicles_in_link += int(((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>().size());
					}

					inbound_link_component->link_moe_data.link_density += ((float)num_vehicles_in_link / (inbound_link_component->_length / 5280.0f)) / (float)inbound_link_component->_num_lanes;
					inbound_link_component->realtime_link_moe_data.num_vehicles_in_link = num_vehicles_in_link;
					if (allowed_movement_size>0)
					{
						avg_turn_penalty /= float(allowed_movement_size);
					}
					else
					{
						avg_turn_penalty = 0.0f;
					}
					
					//realtime moe
					//link travel delay in minutes
					inbound_link_component->realtime_link_moe_data.link_travel_delay = avg_turn_penalty / 60.0f;
					//link travel time
					inbound_link_component->realtime_link_moe_data.link_travel_time = (inbound_link_component->_link_fftt + avg_turn_penalty) /60.0f;
					inbound_link->calculate_moe_for_simulation_interval<NULLTYPE>();
				}
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval()
			{

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=_outbound_inbound_movements.begin();outbound_inbound_movements_itr!=_outbound_inbound_movements.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);
					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						((_Inbound_Movement_Interface*)(*inbound_movement_itr))->template calculate_moe_for_assignment_interval_from_outbound_link<NULLTYPE>();
					}
				}

				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_Link_Interface* inbound_link=inbound_outbound_movements->template inbound_link_reference<_Link_Interface*>();
					_link_component_type* inbound_link_component = (_link_component_type*)((_inbound_outbound_movements_component_type*)(*inbound_outbound_movements_itr))->_inbound_link_reference;
				
					float avg_turn_penalty = 0.0f;
					float avg_turn_penalty_standard_deviation = 0.0f;
					int allowed_movement_size = 0;
					int num_vehicles_in_link = 0;

					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						_movement_component_type* outbound_movement_component = (_movement_component_type*)(*outbound_movement_itr);
						((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template calculate_moe_for_assignment_interval_from_inbound_link<NULLTYPE>();
						if (outbound_movement_component->_turn_travel_penalty < 0.01f*INFINITY_FLOAT)
						{
							allowed_movement_size++;
							avg_turn_penalty += outbound_movement_component->movement_moe_data.turn_penalty;
							avg_turn_penalty_standard_deviation += outbound_movement_component->movement_moe_data.turn_penalty_standard_deviation;
						}
					}

					if (allowed_movement_size > 0)
					{
						avg_turn_penalty /= allowed_movement_size;
						avg_turn_penalty_standard_deviation /= allowed_movement_size;
					}
					else
					{
						avg_turn_penalty = 0;
						avg_turn_penalty_standard_deviation = 0;
					}
					inbound_link_component->link_moe_data.link_travel_delay = avg_turn_penalty;
					inbound_link_component->link_moe_data.link_travel_delay_standard_deviation = avg_turn_penalty_standard_deviation;
					inbound_link->calculate_moe_for_assignment_interval<NULLTYPE>();
				}
			}
			
			template<typename TargetType> void update_vehicle_locations()
			{
				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_link_component_type* inbound_link_component = (_link_component_type*)((_inbound_outbound_movements_component_type*)(*inbound_outbound_movements_itr))->_inbound_link_reference;
					float link_speed = inbound_link_component->realtime_link_moe_data.link_speed;

					float travel_distance = (link_speed * 5280.0f / 3600.0f) * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>();
					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						//num_vehicles_in_link += int(((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>().size());
						typename _Vehicles_Container_Interface::iterator vehicle_itr;
						_Vehicles_Container_Interface& vehicles = ((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>();

						for (vehicle_itr=vehicles.begin();vehicle_itr!=vehicles.end();vehicle_itr++)
						{
							float current_distance = ((_Vehicle_Interface*)(*vehicle_itr))->template distance_to_stop_bar<float>();
							float new_distance = max(0.0f,(current_distance - travel_distance));
							//((_Vehicle_Interface*)(*vehicle_itr))->template distance_to_stop_bar<float>(new_distance); 
							((_Vehicle_Interface*)(*vehicle_itr))->template local_speed<float>(link_speed);
						}
					}
				}
			}

			template<typename TargetType> void update_in_network_vehicle_vht()
			{
				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_link_component_type* inbound_link_component = ((_inbound_outbound_movements_component_type*)(*inbound_outbound_movements_itr))->_inbound_link_reference;
					float link_speed = inbound_link_component->realtime_link_moe_data.link_speed;

					float travel_distance = (link_speed * 5280.0f / 3600.0f) * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>();
					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						//num_vehicles_in_link += int(((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>().size());
						typename _Vehicles_Container_Interface::iterator vehicle_itr;
						_Vehicles_Container_Interface& vehicles = ((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>();

						for (vehicle_itr=vehicles.begin();vehicle_itr!=vehicles.end();vehicle_itr++)
						{
							((_Vehicle_Interface*)(*vehicle_itr))->template update_vht<NT>();
						}
					}
				}
			}

			static void Newells_Conditional(ComponentType* _this,Event_Response& response)
			{
				typename MasterType::intersection_type* _pthis = (typename MasterType::intersection_type*)_this;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;
				
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION)
				{
					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Compute_Step_Flow<NULLTYPE>);
					_pthis->Compute_Step_Flow();
					//response.result=true;
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_ORIGIN_LINK_LOADING_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_ORIGIN_LINK_LOADING_SUB_ITERATION)
				{
					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Origin_Loading_Step<NULLTYPE>);
					_pthis->Origin_Loading_Step();
					//response.result=true;
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION;
				}
				//else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION)
				//{
				//	//TODO:BIG_CHANGE!
				//	//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Network_State_Update<NULLTYPE>);
				//	_pthis->Network_State_Update();
				//	//response.result=true;
				//	response.next._iteration=iteration();
				//	response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION;
				//} 
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION)
				{
					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&ComponentType::template Intersection_REALTIME_MOE_Update<NULLTYPE>);
					_pthis->Intersection_REALTIME_MOE_Update();
					//response.result=true;
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_MOE_COMPUTATION_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_MOE_COMPUTATION_SUB_ITERATION)
				{

					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Intersection_MOE_Update<NULLTYPE>);
					_pthis->Intersection_MOE_Update();
					//response.result=true;
					response.next._iteration=iteration() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION;
				}
				else
				{
					assert(false);
					cout << "Should never reach here in intersection conditional!" << endl;
				}
			}

			//declare_event(Compute_Step_Flow)
			void Compute_Step_Flow()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;
				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this_ptr->template turn_movement_capacity_update<NULLTYPE>(); 
				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				
				_this_ptr->template turn_movement_demand_calculation<NULLTYPE>();
			}

			//declare_event(Origin_Loading_Step)
			void Origin_Loading_Step()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;

				_this_ptr->template turn_movement_supply_allocation<NULLTYPE>();
				
				_this_ptr->template link_capacity_allocation<NULLTYPE>();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this_ptr->template turn_movement_flow_calculation<NULLTYPE>();
				//step 5: node transfer
				_this_ptr->template node_transfer<NULLTYPE>();

				//step 6: origin link loading
				//_this_ptr->template origin_link_loading<NULLTYPE>();
			}

			//declare_event(Network_State_Update)
			void Network_State_Update()
			{
				//typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;

				//step 9: intersection network state update
				_this_ptr->template network_state_update<NULLTYPE>();
			}

			//declare_event(Intersection_REALTIME_MOE_Update)
			void Intersection_REALTIME_MOE_Update()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;
				//step 10: intersection realtime MOE update

				if (((_Scenario_Interface*)_global_scenario)->template calculate_realtime_moe<bool>())
				{
					_this_ptr->template calculate_moe_for_simulation_interval<NULLTYPE>();
				}
			}

			//declare_event(Intersection_MOE_Update)
			void Intersection_MOE_Update()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;
				//step 11: intersection MOE update
				if(((((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					_this_ptr->template calculate_moe_for_assignment_interval<NULLTYPE>();
				}
			}
		};
	}
}

