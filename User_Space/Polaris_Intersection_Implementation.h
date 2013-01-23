#pragma once
#include "User_Space_Includes.h"
#include "Link_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Polaris_Link_Implementation.h"
#include "Intersection_Prototype.h"

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

		implementation struct Polaris_Movement_Implementation:public Polaris_Component_Class<Polaris_Movement_Implementation,MasterType,Data_Object,ParentType>
		{
		public:


			member_data(float, turn_travel_penalty, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(float, movement_capacity, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_supply, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_demand, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_flow, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, movement_transferred, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(Intersection_Components::Types::Turn_Movement_Type_Keys, movement_type, none, none);
			member_data(Intersection_Components::Types::Turn_Movement_Rule_Keys, movement_rule, none, none);

			member_data(float, inbound_link_departed_time_based_experienced_link_turn_travel_delay, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, outbound_link_arrived_time_based_experienced_link_turn_travel_delay, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(int, turn_movement_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, turn_movement_cumulative_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, turn_movement_cumulative_shifted_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(float, minimum_merge_rate, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(vector<typename MasterType::routable_movement_type*>, replicas_container, none, none);


		//==================================================================================================================
		/// forward_link_turn_travel_time
		//------------------------------------------------------------------------------------------------------------------
			//define_container_and_value_interface(_Replicas_Container_Interface, _Replica_Interface, replicas_container_type, Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, NULLTYPE);
            typedef Intersection_Components::Prototypes::Movement_Prototype<typename replicas_container_type::unqualified_value_type,NULLTYPE> _Replica_Interface;
            typedef Random_Access_Sequence_Prototype<replicas_container_type,NULLTYPE,_Replica_Interface*> _Replicas_Container_Interface;
			
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType forward_link_turn_travel_time(){return (TargetType)(_forward_link_turn_travel_time);} tag_getter_as_available(forward_link_turn_travel_time);
			template<typename ComponentType, typename CallerType, typename TargetType>
			void forward_link_turn_travel_time(TargetType set_value)
			{
				_forward_link_turn_travel_time = (float)set_value;
				// update replicas
				typename _Replicas_Container_Interface::iterator replica_itr;
				for (replica_itr=_replicas_container.begin(); replica_itr!=_replicas_container.end(); replica_itr++)
				{
					_Replica_Interface* replica = (_Replica_Interface*)(*replica_itr);
					replica->template forward_link_turn_travel_time<float>(_forward_link_turn_travel_time);
				}
			}
			tag_setter_as_available(forward_link_turn_travel_time);

			float _forward_link_turn_travel_time;

			member_container(vector<float>, cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, none, none);
			member_container(vector<float>, cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			member_container(vector<float>, cached_outbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			member_container(vector<int>, cached_turn_movement_cumulative_shifted_arrived_vehicles_array, none, none);

			//typedef deque<typename MasterType::vehicle_type*> vehicles_container_type;
			member_container(deque<typename MasterType::vehicle_type*>, vehicles_container, none, none);
			member_container(deque<typename MasterType::vehicle_type*>, upstream_vehicles_container, none, none);

			member_component(typename MasterType::link_type, inbound_link, none, none);

			member_component(typename MasterType::link_type, outbound_link, none, none);

			//member_component_basic(typename MasterType::DETECTOR_TYPE,detector);

			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(int, merge_priority, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, inbound_link_green_cycle_ratio, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
		};
		


		implementation struct Polaris_Outbound_Inbound_Movements_Implementation:public Polaris_Component_Class<Polaris_Outbound_Inbound_Movements_Implementation,MasterType,Data_Object,ParentType>
		{


			member_component(typename MasterType::link_type, outbound_link_reference, none, none);

			member_container(vector<typename MasterType::movement_type*>, inbound_movements, none, none);

		};
		
		implementation struct Polaris_Inbound_Outbound_Movements_Implementation:public Polaris_Component_Class<Polaris_Inbound_Outbound_Movements_Implementation,MasterType,Data_Object,ParentType>
		{

			member_component(typename MasterType::link_type, inbound_link_reference, none, none);

			member_container(vector<typename MasterType::movement_type*>, outbound_movements, none, none);

		};
		
		implementation struct Polaris_Intersection_Implementation:public Polaris_Component_Class<Polaris_Intersection_Implementation,MasterType,Execution_Object,ParentType>
		{

			

//			member_component(typename MasterType::SIGNAL_TYPE,signal, none, none);

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

			feature_implementation void turn_movement_capacity_update()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;

				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<ComponentType,CallerType,_Outbound_Inbound_Movements_Container_Interface&>();
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
//						inbound_movement->update_capacity();
// move this part to movement update_capacity()
						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						float inbound_link_capacity = inbound_link->template link_capacity<float>(); 
						float green_time = inbound_movement->green_time<float>();
						float green_time_ratio = inbound_movement->green_time<float>()*1.0f/((float)scenario->template simulation_interval_length<int>());
						if(_intersection_type == Types::Intersection_Type_Keys::NO_CONTROL)
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

			feature_implementation void turn_movement_flow_calculation()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Movement_Interface::get_type_of(cached_turn_movement_cumulative_shifted_arrived_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();



				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<ComponentType,CallerType,_Outbound_Inbound_Movements_Container_Interface&>();
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

			feature_implementation void node_transfer()
			{

				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface,  _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				
				typedef Vehicle_Components::Prototypes::Vehicle_Prototype<typename MasterType::vehicle_type, ComponentType> _Vehicle_Interface;
				define_component_interface(_Movement_Plan_Interface, _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);

				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();

				// apply transfers of y(a,a''t) for each turn;
				//int arrived_vehicles = this->template network_cumulative_arrived_vehicles;
				//int in_network_vehicles = this->template network_in_network_vehicles;
				
				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<ComponentType,CallerType,_Outbound_Inbound_Movements_Container_Interface&>();
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
							double rng = _rng_stream.RandU01();
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
								int enter_time=vehicle->template movement_plan<_Movement_Plan_Interface*>()->template get_current_link_enter_time<int>();
								int delayed_time = int((scenario->template current_simulation_time<int>() - enter_time) - inbound_link->template link_fftt<float>());
								int enter_interval_index = enter_time / scenario->simulation_interval_length<int>();
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
			
			feature_implementation void network_state_update()
			{

				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Movement_Interface::get_type_of(cached_turn_movement_cumulative_shifted_arrived_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();


				

				//network_data.network_cumulative_arrived_vehicles = this->template network_cumulative_arrived_vehicles;
				//network_data.network_cumulative_departed_vehicles = this->template network_cumulative_departed_vehicles;
				//network_data.network_cumulative_loaded_vehicles = this->template network_cumulative_loaded_vehicles;
				//network_data.network_in_network_vehicles = this->template network_in_network_vehicles;


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<ComponentType,CallerType,_Outbound_Inbound_Movements_Container_Interface&>();
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

			feature_implementation void accept_vehicle(void* vehicle)
			{

				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, _Movement_Interface::get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, _Link_Interface::get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype,  ComponentType);
				define_component_interface(_Movement_Plan_Interface, _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);				
				//define_component_interface(_Detector_Interface,  _Movement_Interface_type::get_type_of(detector), Signal_Components::Prototypes::Detector_Prototype, ComponentType);
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();

				_Link_Interface* outbound_link;

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

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
							inbound_movement->template vehicles_container<_Vehicles_Container_Interface&>().push_back((_Vehicle_Interface*)vehicle);
							inbound_movement->template turn_movement_cumulative_arrived_vehicles<int&>()++;

							char s[100];
#ifdef WINDOWS
							sprintf_s(s, "stop:%i:%i:%i\n", inbound_link->template uuid<int>(), _uuid, scenario->template current_time<int>());
#else
							sprintf(s, "stop:%i:%i:%i\n", inbound_link->template uuid<int>(), _uuid, scenario->template current_time<int>());
#endif
							std::string s1 = s;
							//scenario->template output<NULLTYPE>(s1);
#ifdef WINDOWS							
							sprintf_s(s, "advanced:%i:%i:%i\n", outbound_link->template uuid<int>(), outbound_link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>(), scenario->template current_time<int>());
#else
							sprintf(s, "advanced:%i:%i:%i\n", outbound_link->template uuid<int>(), outbound_link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>(), scenario->template current_time<int>());
#endif
							std::string s2 = s;
							//scenario->template output<NULLTYPE>(s2);
						}
					}
				}
			}

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

			feature_implementation void supply_allocation_based_on_driving_rule()
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
				_Link_Interface* outbound_link;
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				size_t size = _outbound_inbound_movements.size();
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
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

			feature_implementation void supply_allocation_proportion_to_demand()
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;				
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
				_Link_Interface* outbound_link;
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				size_t size = _outbound_inbound_movements.size();
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
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

			feature_implementation void origin_link_loading()
			{

				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;		
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_simple_container_interface(_Float_Container_Interface, _Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Movement_Interface::get_type_of(cached_turn_movement_cumulative_shifted_arrived_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				define_container_and_value_interface(_Vehicles_Origin_Container_Interface, _Vehicle_Interface,_Link_Interface::get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_interface(_Vehicle_Origin_Queue_Interface, _Link_Interface::get_type_of(link_origin_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();

				_Link_Interface* outbound_link;
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
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
							double rng = _rng_stream.RandU01();
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
								vehicle->template load<Vehicle_Components::Types::Load_To_Origin_Link>();
							
								//update link state
								outbound_link->template link_origin_cumulative_departed_vehicles<int&>()++;
								outbound_link->template link_origin_departed_vehicles<int&>()++;

								outbound_link->template link_origin_arrived_vehicles<int&>()--;

								outbound_link->template push_vehicle<_Vehicle_Interface*>(vehicle);

								scenario->template network_cumulative_departed_vehicles<int&>()++;
								scenario->template network_in_network_vehicles<int&>()++;
							}
						}
					}
				}
			}

			feature_implementation void Initialize()
			{
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;		
				define_component_interface(_Intersection_Control_Interface, get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				if (_outbound_inbound_movements.size() > 0 && _inbound_outbound_movements.size() > 0)
				{
					((_Intersection_Control_Interface*)_intersection_control)->template Initialize<NULLTYPE>();
					load_event(ComponentType,Newells_Conditional,Compute_Step_Flow,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION,NULLTYPE);
				}
			}
			
			declare_feature_conditional(Newells_Conditional)
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				typedef Intersection_Prototype<typename MasterType::intersection_type> _Intersection_Interface;

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
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;

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

			}

			declare_feature_event(Network_State_Update)
			{
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				typedef Intersection_Prototype<typename MasterType::intersection_type> _Intersection_Interface;
				
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;

				//step 9: intersection network state update
				_this_ptr->template network_state_update<NULLTYPE>();

			}
		};
	}

}
