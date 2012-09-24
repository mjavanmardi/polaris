#pragma once
#include "Repository_User_Includes.h"
#include "Scenario_Interface.h"

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
	
	namespace Interfaces
	{
		template<typename ThisType, typename CallerType>
		struct Turn_Movement_Interface
		{
			facet_accessor(inbound_link);
			facet_accessor(outbound_link);
			facet_accessor(turn_movement_type);
			facet_accessor(turn_movement_rule);
			facet_accessor(turn_travel_penalty);
			facet_accessor(forward_link_turn_travel_time);
			facet_accessor(id);
			facet_accessor(replicas_container);
		};


		template<typename ThisType,typename CallerType>
		struct Link_Interface
		{
			facet_accessor(uuid);
			facet_accessor(id);

			facet_accessor(scenario_reference);
			facet_accessor(network);

			// inbound_links and outbound_links might belong to Link_Base for network construction which we may implement later
			facet_accessor(inbound_turn_movements);
			facet_accessor(outbound_turn_movements);
			
			facet_accessor(travel_time);

			facet_accessor(upstream_intersection);
			facet_accessor(downstream_intersection);
			
			facet_accessor(num_lanes);
			facet_accessor(length);
			facet_accessor(speed_limit);
			
			facet_accessor(link_type);
			
			facet_accessor(num_left_turn_bays);
			facet_accessor(num_right_turn_bays);
			
			facet_accessor(left_turn_bay_length);
			facet_accessor(right_turn_bay_length);

			facet_accessor(inbound_turn_movement_size);
			facet_accessor(outbound_turn_movement_size);


			//link state			
			facet_accessor(link_fftt);
			facet_accessor(link_bwtt);
			facet_accessor(link_fftt_cached_simulation_interval_size);
			facet_accessor(link_bwtt_cached_simulation_interval_size);
			
			
			//current interval
			facet_accessor(link_capacity);
			facet_accessor(link_supply);
			
			facet_accessor(link_upstream_arrived_vehicles);
			facet_accessor(link_downstream_departed_vehicles);
			
			facet_accessor(link_origin_arrived_vehicles);
			facet_accessor(link_origin_departed_vehicles);
			
			facet_accessor(link_destination_arrived_vehicles);
			facet_accessor(link_destination_departed_vehicles);//Not Used


			//cumulative - Mid-Trip
			facet_accessor(link_upstream_cumulative_arrival_vehicles);
			facet_accessor(link_downstream_cumulative_departure_vehicles);//Not Used

			facet_accessor(link_shifted_cumulative_arrival_vehicles);
			facet_accessor(link_shifted_cumulative_departure_vehicles);//Not Used

			facet_accessor(link_upstream_cumulative_vehicles);
			facet_accessor(link_downstream_cumulative_vehicles);
			
			//cumulative - Begin/End-Trip
			facet_accessor(link_origin_cumulative_arrived_vehicles);
			facet_accessor(link_origin_cumulative_departure_vehicles);

			facet_accessor(link_destination_cumulative_arrival_vehicles);
			facet_accessor(link_destination_cumulative_departure_vehicles);//Not Used

			//cached cumulative state - Mid-Trip
			facet_accessor(cached_link_upstream_cumulative_arrival_vehicles);//Not Used
			facet_accessor(cached_link_downstream_cumulative_departure_vehicles);//Not Used

			facet_accessor(cached_link_shifted_cumulative_arrival_vehicles);
			facet_accessor(cached_link_shifted_cumulative_departure_vehicles);//Not Used
			
			facet_accessor(cached_link_upstream_cumulative_vehicles_array);
			facet_accessor(cached_link_downstream_cumulative_vehicles_array);


			facet_accessor(link_origin_vehicle_current_position);
			facet_accessor(link_origin_vehicle_array);
			facet_accessor(link_origin_vehicle_queue);


			facet_accessor(current_vehicle_queue);			

			facet_accessor(detector);
			facet_accessor(lane_groups);


			facet_accessor(maximum_flow_rate);
			facet_accessor(free_flow_speed);
			facet_accessor(backward_wave_speed);
			facet_accessor(jam_density);
			facet_accessor(critical_density);

			//for holding mirror links in routable networks
			facet_accessor(replicas_container);

			//routing interface
			facet_accessor(f_cost);
			facet_accessor(h_cost);
			facet_accessor(label_pointer);
			facet_accessor(label_cost);
			facet_accessor(scan_list_status);
			facet_accessor(network_link_reference);

			//facet TargetType pull_vehicle(call_requirements(requires_2(ThisType,CallerType,Is_Same_Entity)))
			//{
			//	PTHIS(ThisType)->offer_vehicle<Dispatch<ThisType>,TargetType>();
			//}

			facet void push_vehicle(TargetType vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			facet void p_vehicle(TargetType* veh/*,requires(TargetType,IsUnloaded)*/)
			{
				link_origin_cumulative_arrived_vehicles<int&>()++;
				typedef typename ThisType::link_origin_vehicle_array_type LinkOriginVehiclesType;
				link_origin_vehicle_array<LinkOriginVehiclesType>().push_back(veh);
				veh->load_to_entry_queue<NULLTYPE>();
			}

			facet void accept_vehicle(TargetType veh/*,requires(TargetType,IsLoaded)*/)
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::current_vehicle_queue_element_type VehicleType;
				typedef typename Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;
				typedef typename ThisType::current_vehicle_queue_type VehicleQueueType;


				Vehicle_Interface* vehicle=(Vehicle_Interface*)veh;

				vehicle->transfer_to_next_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length,current_time);

				//update outbound link state: A(a,0,t)
				link_upstream_cumulative_arrival_vehicles<int&>()++;
				link_upstream_arrived_vehicles<int&>()++;

				if(this==vehicle->destination_link<Link_Interface*>())
				{
					vehicle->arrive_to_destination_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
					
					//update link state: N_destination(a,t)
					link_destination_cumulative_arrival_vehicles<int&>()++;
					link_destination_arrived_vehicles<int&>()++;
					
					//update network state:
					//arrived_vehicles++;
					//in_network_vehicles--;
				}
				else
				{
					current_vehicle_queue<VehicleQueueType&>().push_back(vehicle);
				}
			}
			
			facet void link_supply_update()
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

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
						link_upstream_cumulative_vehicles_by_t_minus_one = cached_link_upstream_cumulative_vehicles_array<int*>()[t_minus_one_fftt];
					}
					
					//N(a,L(a),t-bwtt)
					if (current_simulation_interval_index >= (link_bwtt_cached_simulation_interval_size<int>()) )
					{
						t_minus_bwtt = (current_simulation_interval_index + 1 - link_bwtt_cached_simulation_interval_size<int>())%(link_bwtt_cached_simulation_interval_size<int>());
						link_downstream_cumulative_vehicles_by_t_minus_bwtt = cached_link_upstream_cumulative_vehicles_array<int*>()[t_minus_bwtt];
					}
					
					//supply(a,t) = Kj(a,t)*L(a)*nlanes(a,t) + N(a,L(a),t-bwtt) -N(a,0,t-1) = backward wave propogation in link
					link_available_spaces = num_vehicles_under_jam_density_on_inbound_link + link_downstream_cumulative_vehicles_by_t_minus_bwtt - link_upstream_cumulative_vehicles_by_t_minus_one;
					link_supply<float>(max(0.0,(double)link_available_spaces));

					float current_link_capacity = 0.0;
					current_link_capacity =  (float) (simulation_interval_length * num_lanes<int>() * maximum_flow_rate<float>()/3600.0);
					link_capacity<float>(current_link_capacity);
				}


				PRINT("\t\t" << "link N has supply " << link_supply<float>());
			}

			facet void link_moving()
			{
				typedef typename ThisType::current_vehicle_queue_type VehicleQueueType;
				typedef typename ThisType::current_vehicle_queue_element_type VehicleType;
				typedef typename Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;

				
				typedef typename ThisType::intersection_type IntersectionType;
				typedef typename Intersection_Components::Interfaces::Intersection_Interface<IntersectionType,ThisType> Intersection_Interface;

				Intersection_Interface* intersection=downstream_intersection<Intersection_Interface*>();

				typename VehicleQueueType::iterator vehicle_itr;

				VehicleQueueType& cur_vehicle_queue=current_vehicle_queue<VehicleQueueType&>();

				for(vehicle_itr=cur_vehicle_queue.begin();vehicle_itr!=cur_vehicle_queue.end();vehicle_itr++)
				{
					intersection->push_vehicle<NULLTYPE>((*vehicle_itr));
				}

				cur_vehicle_queue.clear();
			}

			facet void origin_link_loading()
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

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
				
				typedef typename ThisType::current_vehicle_queue_type VehicleQueueType;
				typedef typename ThisType::current_vehicle_queue_element_type VehicleType;
				typedef Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;
				typedef typename ThisType::link_origin_vehicle_array_type VehicleOriginContainerType;
				typedef typename ThisType::link_origin_vehicle_queue_type VehicleOriginQueueType;

				Vehicle_Interface* vehicle;

				//arrived vehicles at current interval
				int current_position = link_origin_vehicle_current_position<int>();
				
				if(current_position<(int)link_origin_vehicle_array<VehicleOriginContainerType&>().size())
				{
					for(int iv=current_position;iv<(int)link_origin_vehicle_array<VehicleOriginContainerType&>().size();iv++)
					{
						//update current position
						link_origin_vehicle_current_position<int&>()++;

						vehicle=(Vehicle_Interface*)link_origin_vehicle_array<VehicleOriginContainerType&>()[iv];

						if(vehicle->departure_simulation_interval_index<int>() == current_simulation_interval_index)
						{
							link_origin_vehicle_queue<VehicleOriginQueueType&>().push_back(vehicle);
							link_origin_arrived_vehicles<int&>()++;
							link_origin_cumulative_arrived_vehicles<int&>()++;
							//loaded_vehicles++;
						}
						else
						{
							break;
						}
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
						if(rand()%2==0)
						{//partial vehicle, incomplete implementation
							++num_link_origin_departed_vehicles_allowed;
						}
					}

					if(num_link_origin_departed_vehicles_allowed>0)
					{//
						int num_departed_vehicles = min(link_origin_arrived_vehicles<int>(),num_link_origin_departed_vehicles_allowed);
						for (int iv=0;iv<num_departed_vehicles;iv++)
						{//
							vehicle=(Vehicle_Interface*)link_origin_vehicle_queue<VehicleOriginQueueType&>().front();
							link_origin_vehicle_queue<VehicleOriginQueueType&>().pop_front();

							//update vehicle state
							vehicle->load_to_origin_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
							
							//update link state
							link_origin_cumulative_departure_vehicles<int&>()++;
							link_origin_departed_vehicles<int&>()++;
							link_origin_arrived_vehicles<int&>()--;

							push_vehicle<Vehicle_Interface*>(vehicle);
							
							//update network state
							//departed_vehicles++;
							//in_network_vehicles++;

							if(this==vehicle->destination_link<Link_Interface*>())
							{
								vehicle->arrive_to_destination_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
								
								//update link state: N_destination(a,t)
								link_destination_cumulative_arrival_vehicles<int&>()++;
								link_destination_arrived_vehicles<int&>()++;
								
								//update network state:
								//arrived_vehicles++;
								//in_network_vehicles--;
							}
							else
							{
								current_vehicle_queue<VehicleQueueType&>().push_back(vehicle);
							}
						}
					}
				}

				//this->network_cumulative_loaded_vehicles=loaded_vehicles;
				//this->network_cumulative_departed_vehicles=departed_vehicles;
				//this->network_in_network_vehicles=in_network_vehicles;
				//this->network_cumulative_arrived_vehicles=arrived_vehicles;
			}

			facet void network_state_update()
			{
				// update N(a,a',t)
				// update A(a,0,t) = A(a,0,t-1) + sum{y(a',a,t)|for all a' in BS(a)};
				// update V(a,t+L/vf) = A(a,0,t) + N_origin(a,t)-N_destination(a,t);
				// N(a,L,t) = D(a,L,t) = N(a,L,t-1) + sum{y(a,a',t)|for all a' in FS(a)};
				// N(a,x,t) = min{N(a,0,t-x/vf),N(a,L,t-(L-x)/w)};
				// update y_demand(a,a',t)

				//input states
				
				//this->link_destination_arrived_vehicles_array;				//y_destination(a,t) at time t
				//this->link_destination_cumulative_arrival_vehicles_array;		//N_destination(a,t) by time t

				//this->link_origin_arrived_vehicles_array;						//y_origin_a(a,t) at time t
				//this->link_origin_departed_vehicles_array;					//y_origin_d(a,t) at time t

				//this->link_origin_cumulative_arrive_vehicles_array;			//N_origin(a,t) by time t
				//this->link_origin_cumulative_departure_vehicles_array;		//N_origin(a,t) by time t
				
				//this->link_downstream_departed_vehicles_array;				//y_departed(a,t) at time t
				//this->link_upstream_arrived_vehicles_array;					//y_arrived(a,t) at time t

				//this->link_upstream_cumulative_arrival_vehicles_array;		//A(a,t) by time t
				//this->link_downstream_cumulative_vehicles_array;				//N(a,L,t) by time t

				//updated states
				//flow
				//link
				//this->cached_link_downstream_cumulative_vehicles_array;			//[N(a,L,t-bwtt),N(a,L,t-1)] done
				//this->cached_link_upstream_cumulative_vehicles_array;				//[N(a,0,t-fftt),N(a,0,t-1)] done
				//this->cached_link_shifted_cumulative_arrival_vehicles_array;		//[V(a,t-fftt),V(a,t-1)]

				//this->link_upstream_cumulative_vehicles_array;					//N(a,0,t)
				//turn movement
				//this->cached_turn_movement_cumulative_shifted_arrival_vehicles_array;	//[N(a,a',0,t-fftt),N(a,a',0,t-1)]
				//this->cached_turn_movement_downstream_cumulative_vehicles_array;		//[N(a,a',L,t-bwtt),N(a,a',L,t-1)]
				//this->cached_turn_movement_upstream_cumulative_vehicles_array;			//[V(a,a',t-fftt),V(a,a',t-1]

				//time
				typedef typename ThisType::scenario_type ScenarioType;				
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

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

				int upstream_cumulative_departure_vehicles = 0;
				upstream_cumulative_departure_vehicles = 
					link_upstream_cumulative_arrival_vehicles<int>() + 
					link_origin_cumulative_departure_vehicles<int>() - 
					link_destination_cumulative_arrival_vehicles<int>();

				upstream_cumulative_departure_vehicles = max(0,upstream_cumulative_departure_vehicles);

				if (t_minus_bwtt>-1)
				{
					int jam_vehicles = (int) (num_lanes<int>() * length<float>() * jam_density<float>());

					int cached=cached_link_downstream_cumulative_vehicles_array<int*>()[t_minus_bwtt]+jam_vehicles;
					link_upstream_cumulative_vehicles<int>(min(upstream_cumulative_departure_vehicles,cached));
				}
				else
				{
					link_upstream_cumulative_vehicles<int>(upstream_cumulative_departure_vehicles);
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

				cached_link_upstream_cumulative_vehicles_array<int*>()[t_fftt] = link_upstream_cumulative_vehicles<int>();
				cached_link_downstream_cumulative_vehicles_array<int*>()[t_bwtt] = link_downstream_cumulative_vehicles<int>();

				//network data
				int t_minus_fftt = -1;
				int link_shifted_cumulative_arrive_vehicles = 0;

				int cached_fftt=link_fftt_cached_simulation_interval_size<int>();

				if((current_simulation_interval_index+1) >= cached_fftt)
				{
					t_minus_fftt = (current_simulation_interval_index + 1 - cached_fftt) % cached_fftt;
					link_shifted_cumulative_arrive_vehicles = cached_link_upstream_cumulative_vehicles_array<int*>()[t_minus_fftt];
				}
				else
				{
					link_shifted_cumulative_arrive_vehicles = 0;
				}
				
				//network_data.link_upstream_cumulative_arrival_curve_array[outbound_link_index] = this->link_upstream_cumulative_arrival_vehicles_array[outbound_link_index];
				//network_data.link_upstream_cumulative_depature_curve_array[outbound_link_index] = this->link_upstream_cumulative_vehicles_array[outbound_link_index];
				//network_data.link_downstream_cumulative_arrival_curve_array[outbound_link_index] = link_shifted_cumulative_arrive_vehicles;
				//network_data.link_downstream_cumulative_depature_curve_array[outbound_link_index] = this->link_downstream_cumulative_vehicles_array[outbound_link_index];
				//network_data.link_origin_cumulative_depature_curve_array[outbound_link_index] = this->link_origin_cumulative_departure_vehicles_array[outbound_link_index];
				//network_data.link_destination_cumulative_arrival_curve_array[outbound_link_index] = this->link_destination_cumulative_arrival_vehicles_array[outbound_link_index];
			}

			facet_accessor(link_simulation_status);
			
			facet void Initialize()
			{
				schedule_event_local(ThisType,Newells_Conditional,Compute_Step_Flow_Supply_Update,0,NULLTYPE);
			}

			/*



			ADD EVENT REGISTER TO TYPE!!!



			*/

			declare_facet_conditional(Newells_Conditional)
			{
				Link_Interface* _this=(Link_Interface*)pthis;
				
				Revision link_current_revision=pthis->object_current_revision();
				
				PRINT("\n" << iteration << "." << sub_iteration << ":\t" << "visiting link: " << _this->uuid<int>());

				if(link_current_revision.iteration!=iteration)
				{
					//first visit this iteration, update status

					_this->link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::NONE_COMPLETE);
				}


				if(sub_iteration==0)
				{
					PRINT("\t" << "Compute_Route Not Finished, Return This Iteration");

					pthis->Swap_Event((Event)&Link_Interface::Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=false;
					response.next=iteration;
				}
				else if(_this->link_simulation_status<Types::Link_Simulation_Status>()==Types::Link_Simulation_Status::NONE_COMPLETE)
				{
					PRINT("\t" << "Run Compute_Step_Flow_Supply_Update, Return This Iteration");

					pthis->Swap_Event((Event)&Link_Interface::Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=true;
					response.next=iteration;
				}
				else if(_this->link_simulation_status<Types::Link_Simulation_Status>()==Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE)
				{
					typedef typename ThisType::intersection_type intersection_type;
					typedef Intersection_Components::Interfaces::Intersection_Interface<intersection_type,ThisType> Intersection_Interface;
					typedef Intersection_Components::Types::Intersection_Simulation_Status intersection_simulation_status_type;

					Revision intersection_current_revision=Execution_Object::allocator_template<intersection_type>::allocator_reference.type_current_revision();

					Intersection_Interface* upstream=_this->upstream_intersection<Intersection_Interface*>();
					Intersection_Interface* downstream=_this->downstream_intersection<Intersection_Interface*>();

					if(upstream->intersection_simulation_status<intersection_simulation_status_type>()
						==intersection_simulation_status_type::COMPUTE_STEP_FLOW_COMPLETE
						&&
						downstream->intersection_simulation_status<intersection_simulation_status_type>()
						==intersection_simulation_status_type::COMPUTE_STEP_FLOW_COMPLETE)
					{
						//upstream and downstream intersections check out, ready for "phase 2: Compute_Step_Flow_Link_Moving"
						
						PRINT("\t" << "Run Compute_Step_Flow_Link_Moving, Return Next Iteration");

						pthis->Swap_Event((Event)&Link_Interface::Compute_Step_Flow_Link_Moving<NULLTYPE>);
						response.result=true;
						response.next=iteration+1;
					}
					else
					{
						//either upstream and/or downstream intersection is not done, come back later
						
						PRINT("\t" << "Compute_Step_Flow Not Finished, Return This Iteration");

						response.result=false;
						response.next=iteration;
					}
				}
			}

			declare_facet_event(Compute_Step_Flow_Supply_Update)
			{
				Link_Interface* _this=(Link_Interface*)pthis;

				//step 1: link supply update based on a given traffic flow model
				_this->link_supply_update<ThisType>();

				_this->link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE);

				PRINT("\t\t" << "COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE");
			}

			declare_facet_event(Compute_Step_Flow_Link_Moving)
			{
				Link_Interface* _this=(Link_Interface*)pthis;

				//step 7: load vehicles to origin links
				_this->origin_link_loading<ThisType>();
				
				//step 7.5: link moving -- no link moving in Newell's simplified model -- it can be used to determine turn bay curve
				_this->link_moving<ThisType>();

				//step 8: link network state update
				_this->network_state_update<ThisType>();

				_this->link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE);

				PRINT("\t\t" << "COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE");
			}
		};

	}
}