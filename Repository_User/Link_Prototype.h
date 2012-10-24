#pragma once
#include "Repository_User_Includes.h"
#include "Scenario_Prototype.h"

namespace Link_Components
{
	namespace Types
	{
		enum Link_Type_Keys
		{
			FREEWAY=0,
			ON_RAMP,
			OFF_RAMP,
			EXPRESSWAY,
			ARTERIAL,
		};
		
		enum Link_Simulation_Status
		{
			NONE_COMPLETE,
			COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE,
			COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		//template<typename ComponentType, typename CallerType>
		//struct Turn_Movement_Interface
		//{
		//	feature_accessor(inbound_link);
		//	feature_accessor(outbound_link);
		//	feature_accessor(turn_movement_type);
		//	feature_accessor(turn_movement_rule);
		//	feature_accessor(turn_travel_penalty);
		//	feature_accessor(forward_link_turn_travel_time);
		//	feature_accessor(id);
		//	feature_accessor(replicas_container);
		//};


		template<typename ComponentType,typename CallerType>
		struct Link_Prototype
		{
			typedef ComponentType Component_Type;

			feature_accessor(uuid);
			feature_accessor(id);

			feature_accessor(scenario_reference);
			feature_accessor(network);

			// inbound_links and outbound_links might belong to Link_Implementation for network construction which we may implement later
			feature_accessor(inbound_turn_movements);
			feature_accessor(outbound_turn_movements);
			
			feature_accessor(travel_time);

			feature_accessor(upstream_intersection);
			feature_accessor(downstream_intersection);
			
			feature_accessor(num_lanes);
			feature_accessor(length);
			feature_accessor(speed_limit);
			
			feature_accessor(link_type);
			
			feature_accessor(num_left_turn_bays);
			feature_accessor(num_right_turn_bays);
			
			feature_accessor(left_turn_bay_length);
			feature_accessor(right_turn_bay_length);

			feature_accessor(inbound_turn_movement_size);
			feature_accessor(outbound_turn_movement_size);


			//link state			
			feature_accessor(link_fftt);
			feature_accessor(link_bwtt);
			feature_accessor(link_fftt_cached_simulation_interval_size);
			feature_accessor(link_bwtt_cached_simulation_interval_size);
			
			
			//current interval
			feature_accessor(link_capacity);
			feature_accessor(link_supply);
			
			feature_accessor(link_upstream_arrived_vehicles);
			feature_accessor(link_downstream_departed_vehicles);
			
			feature_accessor(link_origin_arrived_vehicles);
			feature_accessor(link_origin_departed_vehicles);
			
			feature_accessor(link_destination_arrived_vehicles);
			//feature_accessor(link_destination_departed_vehicles);//Not Used


			//cumulative - Mid-Trip
			feature_accessor(link_upstream_cumulative_arrived_vehicles);
			//feature_accessor(link_downstream_cumulative_departed_vehicles);//Not Used

			feature_accessor(link_shifted_cumulative_arrived_vehicles);
			//feature_accessor(link_shifted_cumulative_departed_vehicles);//Not Used

			feature_accessor(link_upstream_cumulative_vehicles);
			feature_accessor(link_downstream_cumulative_vehicles);
			
			//cumulative - Begin/End-Trip
			feature_accessor(link_origin_cumulative_arrived_vehicles);
			feature_accessor(link_origin_cumulative_departed_vehicles);

			feature_accessor(link_destination_cumulative_arrived_vehicles);
			//feature_accessor(link_destination_cumulative_departed_vehicles);//Not Used

			//cached cumulative state - Mid-Trip
			//feature_accessor(cached_link_upstream_cumulative_arrived_vehicles);//Not Used
			//feature_accessor(cached_link_downstream_cumulative_departed_vehicles);//Not Used

			feature_accessor(cached_link_shifted_cumulative_arrived_vehicles);
			//feature_accessor(cached_link_shifted_cumulative_departed_vehicles);//Not Used
			
			feature_accessor(cached_link_upstream_cumulative_vehicles_array);
			feature_accessor(cached_link_downstream_cumulative_vehicles_array);


			feature_accessor(link_origin_vehicle_current_position);
			feature_accessor(link_origin_vehicle_array);
			feature_accessor(link_origin_vehicle_queue);


			feature_accessor(current_vehicle_queue);			

			feature_accessor(detector);
			feature_accessor(lane_groups);


			feature_accessor(maximum_flow_rate);
			feature_accessor(free_flow_speed);
			feature_accessor(backward_wave_speed);
			feature_accessor(jam_density);
			feature_accessor(critical_density);

			//for holding mirror links in routable networks
			feature_accessor(replicas_container);

			//routing interface
			feature_accessor(f_cost);
			feature_accessor(h_cost);
			feature_accessor(label_pointer);
			feature_accessor(label_cost);
			feature_accessor(scan_list_status);
			feature_accessor(network_link_reference);

			//feature TargetType pull_vehicle(call_requirements(requires_2(ComponentType,CallerType,Is_Same_Entity)))
			//{
			//	PTHIS(ComponentType)->offer_vehicle<Dispatch<ComponentType>,TargetType>();
			//}

			feature void push_vehicle(TargetType vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			feature void p_vehicle(TargetType* veh/*,requires(TargetType,IsUnloaded)*/)
			{
				link_origin_cumulative_arrived_vehicles<int&>()++;
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, link_origin_vehicle_array, _Link_Origin_Vehicles_Container_Interface, Vehicle_Prototype, _Vehicle_Interface, ComponentType);
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)veh;
				link_origin_vehicle_array<_Link_Origin_Vehicles_Container_Interface&>().push_back(vehicle);
				veh->load_to_entry_queue<NULLTYPE>();
			}

			feature void accept_vehicle(TargetType veh/*,requires(TargetType,IsLoaded)*/)
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, current_vehicle_queue, _Vehicle_Queue_Interface, Vehicle_Prototype, _Vehicle_Interface, ComponentType);
				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();




				_Vehicle_Interface* vehicle=(_Vehicle_Interface*)veh;

				vehicle->transfer_to_next_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length,current_time);

				//update outbound link state: A(a,0,t)
				//link_upstream_cumulative_arrived_vehicles<int&>()++;
				//link_upstream_arrived_vehicles<int&>()++;

				if(this->uuid<int>()==(vehicle->destination_link<_Link_Interface*>())->uuid<int>())
				{
					vehicle->arrive_to_destination_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
					
					//update link state: N_destination(a,t)
					link_destination_cumulative_arrived_vehicles<int&>()++;
					link_destination_arrived_vehicles<int&>()++;
					
					scenario->network_cumulative_arrived_vehicles<int&>()++;
					scenario->network_in_network_vehicles<int&>()--;
					//update network state:
					//arrived_vehicles++;
					//in_network_vehicles--;
				}
				else
				{
					current_vehicle_queue<_Vehicle_Queue_Interface&>().push_back(vehicle);
				}
			}
			
			feature void link_supply_update()
			{
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_simple_container_interface_local(Polaris_Random_Access_Sequence_Prototype, cached_link_upstream_cumulative_vehicles_array, _Int_Container_Interface, int, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				//Newell's model
				if(true)
				{
					//int outbound_turn_movement_size = outbound_turn_movement_size<int>();
					
					//jam density * length * num_lanes = Kj(a,t)*L(a)*nlanes(a,t)
					
					float num_vehicles_under_jam_density_on_inbound_link = num_lanes<float>() * jam_density<float>() * length<float>()/5280.0;

					int t_minus_one_fftt = -1;
					int link_upstream_cumulative_vehicles_by_t_minus_one = 0;	
					
					int t_minus_bwtt = -1;
					int link_downstream_cumulative_vehicles_by_t_minus_bwtt = 0;	

					float link_available_spaces = -1;

					//N(a,0,t-1)
					if (current_simulation_interval_index>0)
					{
						t_minus_one_fftt = ((current_simulation_interval_index-1))%(link_fftt_cached_simulation_interval_size<int>());
						link_upstream_cumulative_vehicles_by_t_minus_one = cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>()[t_minus_one_fftt];
					}
					
					//N(a,L(a),t-bwtt)
					if (current_simulation_interval_index >= (link_bwtt_cached_simulation_interval_size<int>()) )
					{
						t_minus_bwtt = (current_simulation_interval_index + 1 - link_bwtt_cached_simulation_interval_size<int>())%(link_bwtt_cached_simulation_interval_size<int>());
						link_downstream_cumulative_vehicles_by_t_minus_bwtt = cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>()[t_minus_bwtt];
					}
					
					//supply(a,t) = Kj(a,t)*L(a)*nlanes(a,t) + N(a,L(a),t-bwtt) -N(a,0,t-1) = backward wave propogation in link
					link_available_spaces = num_vehicles_under_jam_density_on_inbound_link + link_downstream_cumulative_vehicles_by_t_minus_bwtt - link_upstream_cumulative_vehicles_by_t_minus_one;
					link_supply<float>(max(0.0,(double)link_available_spaces));

					float current_link_capacity = 0.0;
					current_link_capacity =  (float) (simulation_interval_length * num_lanes<int>() * maximum_flow_rate<float>()/3600.0);
					link_capacity<float>(current_link_capacity);
				}


				//PRINT("\t" << "link " << uuid<int>() << " has supply " << link_supply<float>());
			}

			feature void link_moving()
			{
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, current_vehicle_queue, _Vehicle_Queue_Interface, Vehicle_Prototype, _Vehicle_Interface, ComponentType);
				define_component_interface_local(_Intersection_Interface, Intersection_Prototype, upstream_intersection, ComponentType);


				_Intersection_Interface* intersection=downstream_intersection<_Intersection_Interface*>();

				typename _Vehicle_Queue_Interface::iterator vehicle_itr;

				_Vehicle_Queue_Interface& cur_vehicle_queue=current_vehicle_queue<_Vehicle_Queue_Interface&>();

				for(vehicle_itr=cur_vehicle_queue.begin();vehicle_itr!=cur_vehicle_queue.end();vehicle_itr++)
				{
					intersection->push_vehicle<NULLTYPE>((*vehicle_itr));
				}

				cur_vehicle_queue.clear();
			}

			feature void origin_link_loading()
			{

				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_component_interface_local(_Intersection_Interface, Intersection_Prototype, upstream_intersection, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, link_origin_vehicle_array, _Vehicles_Origin_Container_Interface, Vehicle_Prototype, _Vehicle_Interface, ComponentType);

				define_container_interface_local(Polaris_Back_Insertion_Sequence_Prototype, link_origin_vehicle_queue, _Vehicle_Origin_Queue_Interface, _Vehicle_Interface, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				//int loaded_vehicles = this->network_cumulative_loaded_vehicles;
				//int departed_vehicles = this->network_cumulative_departed_vehicles;
				//int in_network_vehicles = this->network_in_network_vehicles;
				//int arrived_vehicles = this->network_cumulative_arrived_vehicles;

				// N_origin(a,t) = N_origin(a,t-1) + y_origin(a,t)
				// update N(a,0,t) = D(a,0,t) = min{A(a,0,t)+N_origin(a,t)-N_destination(a,t),N(a,L,t-L/w)};
				// update A(a,0,t) = A(a,0,t-1) + sum{y(a',a,t)|for all a' in BS(a)};
				// update V(a,t+L/vf) = A(a,0,t) + N_origin(a,t)-N_destination(a,t);
				// N(a,L,t) = D(a,L,t) = N(a,L,t-1) + sum{y(a,a',t)|for all a' in FS(a)};
				// N(a,x,t) = min{N(a,0,t-x/vf),N(a,L,t-(L-x)/w)};
				// update y_demand(a,a',t)
				


				_Vehicle_Interface* vehicle;

				//arrived vehicles at current interval
				int current_position = link_origin_vehicle_current_position<int>();

				if(current_position<(int)link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>().size())
				{
					for(int iv=current_position;iv<(int)link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>().size();iv++)
					{
						//update current position
						link_origin_vehicle_current_position<int&>()++;

						vehicle=(_Vehicle_Interface*)link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>()[iv];
						
						int departure_interval=vehicle->departed_simulation_interval_index<int>();
						//if(vehicle->departed_simulation_interval_index<int>() == current_time)
						//{
							//PRINT(iteration << ": loading traveler");
							link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().push_back(vehicle);
							link_origin_arrived_vehicles<int&>()++;
							link_origin_cumulative_arrived_vehicles<int&>()++;
							//loaded_vehicles++;
							scenario->network_cumulative_loaded_vehicles<int&>()++;
						//}
						//else
						//{
						//	break;
						//}
					}
				}

				//have demand
				if(link_origin_arrived_vehicles<int>()>0)
				{
					//check available spaces
					float link_available_spaces = link_supply<float>() - link_upstream_arrived_vehicles<float>();
					int num_link_origin_departed_vehicles_allowed = (int) link_available_spaces;
					float link_origin_departed_flow_allowed = (float) (link_available_spaces - num_link_origin_departed_vehicles_allowed);
					
					if (link_origin_departed_flow_allowed>0.0)
					{//partial vehicle
						_Intersection_Interface* intersection = upstream_intersection<_Intersection_Interface*>();
						double rng = intersection->rng_stream<RngStream&>().RandU01();
						if(rng<=link_origin_departed_flow_allowed)
						{//partial vehicle, incomplete implementation
							++num_link_origin_departed_vehicles_allowed;
						}
					}

					if(num_link_origin_departed_vehicles_allowed>0)
					{//
						int num_departed_vehicles = min(link_origin_arrived_vehicles<int>(),num_link_origin_departed_vehicles_allowed);
						for (int iv=0;iv<num_departed_vehicles;iv++)
						{//
							vehicle=(_Vehicle_Interface*)link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().front();
							link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().pop_front();

							//update vehicle state
							vehicle->load_to_origin_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
							
							//update link state
							link_origin_cumulative_departed_vehicles<int&>()++;
							link_origin_departed_vehicles<int&>()++;
							link_origin_arrived_vehicles<int&>()--;

							push_vehicle<_Vehicle_Interface*>(vehicle);

							//update network state
							//departed_vehicles++;
							//in_network_vehicles++;
							scenario->network_cumulative_departed_vehicles<int&>()++;
							scenario->network_in_network_vehicles<int&>()++;
							
							// Bo added: the following logic already implemented in push_vehicle. Don't need to do here

							//if(this==vehicle->destination_link<Link_Interface*>())
							//{
							//	vehicle->arrive_to_destination_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
							//	
							//	//update link state: N_destination(a,t)
							//	link_destination_cumulative_arrived_vehicles<int&>()++;
							//	link_destination_arrived_vehicles<int&>()++;
							//	
							//	//update network state:
							//	//arrived_vehicles++;
							//	//in_network_vehicles--;
							//	scenario->network_cumulative_arrived_vehicles<int&>()++;
							//	scenario->network_in_network_vehicles<int&>()--;
							//}
						}
					}
				}

				//this->network_cumulative_loaded_vehicles=loaded_vehicles;
				//this->network_cumulative_departed_vehicles=departed_vehicles;
				//this->network_in_network_vehicles=in_network_vehicles;
				//this->network_cumulative_arrived_vehicles=arrived_vehicles;
			}

			feature void network_state_update()
			{
				// update N(a,a',t)
				// update A(a,0,t) = A(a,0,t-1) + sum{y(a',a,t)|for all a' in BS(a)};
				// update V(a,t+L/vf) = A(a,0,t) + N_origin(a,t)-N_destination(a,t);
				// N(a,L,t) = D(a,L,t) = N(a,L,t-1) + sum{y(a,a',t)|for all a' in FS(a)};
				// N(a,x,t) = min{N(a,0,t-x/vf),N(a,L,t-(L-x)/w)};
				// update y_demand(a,a',t)

				//input states
				
				//this->link_destination_arrived_vehicles_array;				//y_destination(a,t) at time t
				//this->link_destination_cumulative_arrived_vehicles_array;		//N_destination(a,t) by time t

				//this->link_origin_arrived_vehicles_array;						//y_origin_a(a,t) at time t
				//this->link_origin_departed_vehicles_array;					//y_origin_d(a,t) at time t

				//this->link_origin_cumulative_arrive_vehicles_array;			//N_origin(a,t) by time t
				//this->link_origin_cumulative_departed_vehicles_array;		//N_origin(a,t) by time t
				
				//this->link_downstream_departed_vehicles_array;				//y_departed(a,t) at time t
				//this->link_upstream_arrived_vehicles_array;					//y_arrived(a,t) at time t

				//this->link_upstream_cumulative_arrived_vehicles_array;		//A(a,t) by time t
				//this->link_downstream_cumulative_vehicles_array;				//N(a,L,t) by time t

				//updated states
				//flow
				//link
				//this->cached_link_downstream_cumulative_vehicles_array;			//[N(a,L,t-bwtt),N(a,L,t-1)] done
				//this->cached_link_upstream_cumulative_vehicles_array;				//[N(a,0,t-fftt),N(a,0,t-1)] done
				//this->cached_link_shifted_cumulative_arrived_vehicles_array;		//[V(a,t-fftt),V(a,t-1)]

				//this->link_upstream_cumulative_vehicles_array;					//N(a,0,t)
				//turn movement
				//this->cached_turn_movement_cumulative_shifted_arrived_vehicles_array;	//[N(a,a',0,t-fftt),N(a,a',0,t-1)]
				//this->cached_turn_movement_downstream_cumulative_vehicles_array;		//[N(a,a',L,t-bwtt),N(a,a',L,t-1)]
				//this->cached_turn_movement_upstream_cumulative_vehicles_array;			//[V(a,a',t-fftt),V(a,a',t-1]

				//time
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_simple_container_interface_local(Polaris_Random_Access_Sequence_Prototype, cached_link_upstream_cumulative_vehicles_array, _Int_Container_Interface, int, ComponentType);
				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				//Vehicle_Interface* vehicle;

				//calculate upstream cumulative vehicles using the three detector method
				int t_minus_bwtt=-1;			
				
				int bwtt_cached_simulation_interval_size=link_bwtt_cached_simulation_interval_size<int>();
				int fftt_cached_simulation_interval_size=link_fftt_cached_simulation_interval_size<int>();
				
				if(current_simulation_interval_index >= bwtt_cached_simulation_interval_size)
				{
					t_minus_bwtt = (current_simulation_interval_index + 1- bwtt_cached_simulation_interval_size)%bwtt_cached_simulation_interval_size;
				}

				int upstream_cumulative_departed_vehicles = 0;
				upstream_cumulative_departed_vehicles = 
					link_upstream_cumulative_arrived_vehicles<int>() + 
					link_origin_cumulative_departed_vehicles<int>() - 
					link_destination_cumulative_arrived_vehicles<int>();

				upstream_cumulative_departed_vehicles = max(0,upstream_cumulative_departed_vehicles);

				if (t_minus_bwtt>-1)
				{
					int jam_vehicles = (int) (num_lanes<int>() * length<float>() * jam_density<float>());

					int cached=cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>()[t_minus_bwtt]+jam_vehicles;
					link_upstream_cumulative_vehicles<int>(min(upstream_cumulative_departed_vehicles,cached));
				}
				else
				{
					link_upstream_cumulative_vehicles<int>(upstream_cumulative_departed_vehicles);
				}

				int t_fftt = -1;
				int t_bwtt = -1;
				int t_cached_delay = -1;

				if (current_simulation_interval_index>0)
				{
					t_fftt = (current_simulation_interval_index)%fftt_cached_simulation_interval_size;
					t_bwtt = (current_simulation_interval_index)%bwtt_cached_simulation_interval_size;
					t_cached_delay = (current_simulation_interval_index)%scenario->num_simulation_intervals_per_assignment_interval<int>();
				}
				else
				{
					t_fftt = 0;
					t_bwtt = 0;
					t_cached_delay = 0;
				}

				cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>()[t_fftt] = link_upstream_cumulative_vehicles<int>();
				cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>()[t_bwtt] = link_downstream_cumulative_vehicles<int>();

				//network data
				int t_minus_fftt = -1;
				int link_shifted_cumulative_arrived_vehicles = 0;

				int cached_fftt=link_fftt_cached_simulation_interval_size<int>();

				if((current_simulation_interval_index+1) >= cached_fftt)
				{
					t_minus_fftt = (current_simulation_interval_index + 1 - cached_fftt) % cached_fftt;
					link_shifted_cumulative_arrived_vehicles = cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>()[t_minus_fftt];
				}
				else
				{
					link_shifted_cumulative_arrived_vehicles = 0;
				}
				if (this->uuid<int>() == 5 && scenario->current_simulation_interval_index<int>()%1 == 0) // the last link visited for current iteration
				{
					printf("time=%d", scenario->current_simulation_interval_index<int>());
					printf("loaded=%d, departed=%d, in_network=%d, arrived=%d",scenario->network_cumulative_loaded_vehicles<int>(),scenario->network_cumulative_departed_vehicles<int>(),scenario->network_in_network_vehicles<int>(),scenario->network_cumulative_arrived_vehicles<int>());
					printf("\n"); 

				}
				//network_data.link_upstream_cumulative_arrived_curve_array[outbound_link_index] = this->link_upstream_cumulative_arrived_vehicles_array[outbound_link_index];
				//network_data.link_upstream_cumulative_depature_curve_array[outbound_link_index] = this->link_upstream_cumulative_vehicles_array[outbound_link_index];
				//network_data.link_downstream_cumulative_arrived_curve_array[outbound_link_index] = link_shifted_cumulative_arrive_vehicles;
				//network_data.link_downstream_cumulative_depature_curve_array[outbound_link_index] = this->link_downstream_cumulative_vehicles_array[outbound_link_index];
				//network_data.link_origin_cumulative_depature_curve_array[outbound_link_index] = this->link_origin_cumulative_departed_vehicles_array[outbound_link_index];
				//network_data.link_destination_cumulative_arrived_curve_array[outbound_link_index] = this->link_destination_cumulative_arrived_vehicles_array[outbound_link_index];
			}

			feature_accessor(link_simulation_status);
			
			feature void Initialize()
			{
				schedule_event_local(ComponentType,Newells_Conditional,Compute_Step_Flow_Supply_Update,0,NULLTYPE);
			}

			/*



			ADD EVENT REGISTER TO TYPE!!!



			*/

			declare_feature_conditional(Newells_Conditional)
			{
				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Link_Interface* _this_ptr=(_Link_Interface*)_pthis;
				define_component_interface_local(_Scenario_Interface, Scenario_Prototype, scenario_reference, ComponentType);
				define_component_interface_local(_Intersection_Interface, Intersection_Prototype, upstream_intersection, ComponentType);
				Revision link_current_revision=_pthis->object_current_revision();
				
				//PRINT("\n" << iteration << "." << sub_iteration << ":\t" << "visiting link: " << _this_ptr->uuid<int>());

				if(link_current_revision.iteration!=iteration)
				{
					//first visit this iteration, update status

					_this_ptr->link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::NONE_COMPLETE);
				}


				if(sub_iteration==0)
				{
					//PRINT("\t" << "Compute_Route Not Finished, Return This Iteration");

					_pthis->Swap_Event((Event)&Link_Prototype::Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=false;
					response.next=iteration;
				}
				else if(_this_ptr->link_simulation_status<Types::Link_Simulation_Status>()==Types::Link_Simulation_Status::NONE_COMPLETE)
				{
					//PRINT("\t" << "Run Compute_Step_Flow_Supply_Update, Return This Iteration");

					_pthis->Swap_Event((Event)&Link_Prototype::Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=true;
					response.next=iteration;
				}
				else if(_this_ptr->link_simulation_status<Types::Link_Simulation_Status>()==Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE)
				{

					typedef Intersection_Components::Types::Intersection_Simulation_Status intersection_simulation_status_type;

					Revision intersection_current_revision=_Intersection_Interface::Component_Type::singleton_reference->type_current_revision();
					//Revision intersection_current_revision=Execution_Object::allocator_template<_Intersection_Interface_type>::allocator_reference.type_current_revision();

					_Intersection_Interface* upstream=_this_ptr->upstream_intersection<_Intersection_Interface*>();
					_Intersection_Interface* downstream=_this_ptr->downstream_intersection<_Intersection_Interface*>();

					if(upstream->intersection_simulation_status<intersection_simulation_status_type>()
						==intersection_simulation_status_type::COMPUTE_STEP_FLOW_COMPLETE
						&&
						downstream->intersection_simulation_status<intersection_simulation_status_type>()
						==intersection_simulation_status_type::COMPUTE_STEP_FLOW_COMPLETE)
					{
						//upstream and downstream intersections check out, ready for "phase 2: Compute_Step_Flow_Link_Moving"
						
						//PRINT("\t" << "Run Compute_Step_Flow_Link_Moving, Return Next Iteration");

						_pthis->Swap_Event((Event)&Link_Prototype::Compute_Step_Flow_Link_Moving<NULLTYPE>);
						response.result=true;
						response.next=iteration+_this_ptr->scenario_reference<_Scenario_Interface*>()->simulation_interval_length<int>();
					}
					else
					{
						//either upstream and/or downstream intersection is not done, come back later

						//PRINT("\t" << "Compute_Step_Flow Not Finished, Return This Iteration");

						response.result=false;
						response.next=iteration;
					}
				}
			}

			declare_feature_event(Compute_Step_Flow_Supply_Update)
			{
				typedef Link_Prototype<ComponentType,ComponentType> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;
				//step 1: link supply update based on a given traffic flow model
				_this_ptr->link_supply_update<ComponentType>();

				_this_ptr->link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE);
				//PRINT("\t\t" << "COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE");
			}

			declare_feature_event(Compute_Step_Flow_Link_Moving)
			{
				typedef Link_Prototype<ComponentType,ComponentType> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;

				//step 7: load vehicles to origin links
				_this_ptr->origin_link_loading<ComponentType>();
				
				//step 7.5: link moving -- no link moving in Newell's simplified model -- it can be used to determine turn bay curve
				_this_ptr->link_moving<ComponentType>();

				//step 8: link network state update
				_this_ptr->network_state_update<ComponentType>();

				_this_ptr->link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE);
				//PRINT("\t\t" << "COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE");
			}
		};

	}
}

using namespace Link_Components::Prototypes;