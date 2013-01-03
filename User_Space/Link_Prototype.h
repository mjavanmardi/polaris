#pragma once
#include "User_Space_Includes.h"

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};
namespace Network_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Network_Prototype;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Prototype;
	}
	namespace Types
	{
		// temporary solution for linux compatibility
#ifndef WINDOWS
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
};
namespace Vehicle_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Vehicle_Prototype;
	}
};
/*------------------------------------------------*/

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

		
		//prototype struct ::Vehicle_Components::Prototypes::Vehicle_Prototype;
		//prototype struct ::Scenario_Components::Prototypes::Scenario_Prototype;

		//prototype struct Scenario_Prototype;
		//prototype struct ::Intersection_Components::Prototypes::Intersection_Prototype;

		prototype struct Link_Prototype
		{
			tag_as_prototype;

			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);

			feature_accessor(scenario_reference, none, none);
			feature_accessor(network_reference, none, none);

			// inbound_links and outbound_links might belong to Link_Implementation for network construction which we may implement later
			feature_accessor(inbound_turn_movements, none, none);
			feature_accessor(outbound_turn_movements, none, none);
			
			feature_accessor(travel_time, none, none);

			feature_accessor(upstream_intersection, none, none);
			feature_accessor(downstream_intersection, none, none);
			
			feature_accessor(num_lanes, none, none);
			feature_accessor(length, none, none);
			feature_accessor(speed_limit, none, none);
			
			feature_accessor(link_type, none, none);
			
			feature_accessor(num_left_turn_bays, none, none);
			feature_accessor(num_right_turn_bays, none, none);
			
			feature_accessor(left_turn_bay_length, none, none);
			feature_accessor(right_turn_bay_length, none, none);

			feature_accessor(inbound_turn_movement_size, none, none);
			feature_accessor(outbound_turn_movement_size, none, none);


			//link state			
			feature_accessor(link_fftt, none, none);
			feature_accessor(link_bwtt, none, none);
			feature_accessor(link_fftt_cached_simulation_interval_size, none, none);
			feature_accessor(link_bwtt_cached_simulation_interval_size, none, none);
			
			
			//current interval
			feature_accessor(link_capacity, none, none);
			feature_accessor(link_supply, none, none);
			
			feature_accessor(link_upstream_arrived_vehicles, none, none);
			feature_accessor(link_downstream_departed_vehicles, none, none);
			
			feature_accessor(link_origin_arrived_vehicles, none, none);
			feature_accessor(link_origin_departed_vehicles, none, none);
			feature_accessor(link_origin_loaded_vehicles, none, none);
			
			feature_accessor(link_destination_arrived_vehicles, none, none);
			//feature_accessor(link_destination_departed_vehicles);//Not Used


			//cumulative - Mid-Trip
			feature_accessor(link_upstream_cumulative_arrived_vehicles, none, none);
			//feature_accessor(link_downstream_cumulative_departed_vehicles);//Not Used

			feature_accessor(link_shifted_cumulative_arrived_vehicles, none, none);
			//feature_accessor(link_shifted_cumulative_departed_vehicles);//Not Used

			feature_accessor(link_upstream_cumulative_vehicles, none, none);
			feature_accessor(link_downstream_cumulative_vehicles, none, none);
			
			//cumulative - Begin/End-Trip
			feature_accessor(link_origin_cumulative_arrived_vehicles, none, none);
			feature_accessor(link_origin_cumulative_departed_vehicles, none, none);

			feature_accessor(link_destination_cumulative_arrived_vehicles, none, none);
			//feature_accessor(link_destination_cumulative_departed_vehicles);//Not Used

			//cached cumulative state - Mid-Trip
			//feature_accessor(cached_link_upstream_cumulative_arrived_vehicles);//Not Used
			//feature_accessor(cached_link_downstream_cumulative_departed_vehicles);//Not Used

			feature_accessor(cached_link_shifted_cumulative_arrived_vehicles, none, none);
			//feature_accessor(cached_link_shifted_cumulative_departed_vehicles);//Not Used
			
			feature_accessor(cached_link_upstream_cumulative_vehicles_array, none, none);
			feature_accessor(cached_link_downstream_cumulative_vehicles_array, none, none);


			feature_accessor(link_origin_vehicle_current_position, none, none);
			feature_accessor(link_origin_vehicle_array, none, none);
			feature_accessor(link_origin_vehicle_queue, none, none);


			feature_accessor(current_vehicle_queue, none, none);			

			feature_accessor(detector, none, none);
			feature_accessor(lane_groups, none, none);


			feature_accessor(maximum_flow_rate, none, none);
			feature_accessor(free_flow_speed, none, none);
			feature_accessor(backward_wave_speed, none, none);
			feature_accessor(jam_density, none, none);
			feature_accessor(critical_density, none, none);

			//for holding mirror links in routable networks
			feature_accessor(replicas_container, none, none);

			//routing interface
			feature_accessor(f_cost, none, none);
			feature_accessor(h_cost, none, none);
			feature_accessor(label_pointer, none, none);
			feature_accessor(label_cost, none, none);
			feature_accessor(scan_list_status, none, none);
			feature_accessor(network_link_reference, none, none);

			feature_accessor(approach, none, none);

			feature_accessor(link_num_vehicles_in_queue, none, none);

			feature_accessor(link_destination_vehicle_queue, none, none);

			//feature TargetType pull_vehicle(call_requirements(requires_2(ComponentType,CallerType,Is_Same_Entity)))
			//{
			//	PTHIS(ComponentType)->template offer_vehicle<Dispatch<ComponentType>,TargetType>();
			//}

			feature_prototype void push_vehicle(TargetType vehicle, float a_delayed_time)
			{
				accept_vehicle<TargetType>(vehicle, a_delayed_time);
			}

			feature_prototype void p_vehicle(TargetType* veh/*,requires(TargetType,IsUnloaded)*/)
			{
				link_origin_cumulative_arrived_vehicles<int&>()++;
				define_container_and_value_interface(_Link_Origin_Vehicles_Container_Interface, _Vehicle_Interface,get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)veh;
				link_origin_vehicle_array<_Link_Origin_Vehicles_Container_Interface&>().push_back(vehicle);
				veh->template load_to_entry_queue<NULLTYPE>();
			}

			feature_prototype void accept_vehicle(TargetType veh, float a_delayed_time/*,requires(TargetType,IsLoaded)*/)
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicle_Queue_Interface, _Vehicle_Interface, get_type_of(current_vehicle_queue), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_and_value_interface(_Destination_Vehicle_Queue_Interface, _Vehicle_Interface_2, get_type_of(link_destination_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();
				int current_time = scenario->template current_time<int>();


define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);


//if (this->template uuid<int>() == 1 && current_time == 23796)
//{
//	cout << "here" << endl;
//}

				_Vehicle_Interface* vehicle=(_Vehicle_Interface*)veh;

				vehicle->template transfer_to_next_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length,a_delayed_time);

				//update outbound link state: A(a,0,t)
				//link_upstream_cumulative_arrived_vehicles<int&>()++;
				//link_upstream_arrived_vehicles<int&>()++;

				if(this->template internal_id<int>()==(vehicle->template destination_link<_Link_Interface*>())->template internal_id<int>())
				{
					vehicle->template arrive_to_destination_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
					
					//update link state: N_destination(a,t)
					link_destination_cumulative_arrived_vehicles<int&>()++;
					link_destination_arrived_vehicles<int&>()++;
					this->template link_destination_vehicle_queue<_Destination_Vehicle_Queue_Interface&>().push_back(vehicle);
					scenario->template network_cumulative_arrived_vehicles<int&>()++;
					scenario->template network_in_network_vehicles<int&>()--;
					//update network state:
					//arrived_vehicles++;
					//in_network_vehicles--;
				}
				else
				{

					current_vehicle_queue<_Vehicle_Queue_Interface&>().push_back(vehicle);
				}
			}
			
			feature_prototype void link_supply_update()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, get_type_of(cached_link_upstream_cumulative_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();
				int current_time = scenario->template current_time<int>();

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

			feature_prototype void link_moving()
			{
				define_container_and_value_interface(_Vehicle_Queue_Interface, _Vehicle_Interface, get_type_of(current_vehicle_queue), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
int current_simulation_time = scenario->template current_time<int>();
//if (this->template uuid<int>() == 7 && current_simulation_time == 23802)
//{
//	cout << "here" << endl;
//}

				_Intersection_Interface* intersection=downstream_intersection<_Intersection_Interface*>();

				typename _Vehicle_Queue_Interface::iterator vehicle_itr;

				_Vehicle_Queue_Interface& cur_vehicle_queue=current_vehicle_queue<_Vehicle_Queue_Interface&>();

				for(vehicle_itr=cur_vehicle_queue.begin();vehicle_itr!=cur_vehicle_queue.end();vehicle_itr++)
				{
					intersection->template push_vehicle<NULLTYPE>((*vehicle_itr), -1, -1);
				}

				cur_vehicle_queue.clear();
			}

			feature_prototype void origin_link_loading()
			{

				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Origin_Container_Interface, _Vehicle_Interface,get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				define_container_interface(_Vehicle_Origin_Queue_Interface, get_type_of(link_origin_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();
				int current_time = scenario->template current_time<int>();

				//int loaded_vehicles = this->template network_cumulative_loaded_vehicles;
				//int departed_vehicles = this->template network_cumulative_departed_vehicles;
				//int in_network_vehicles = this->template network_in_network_vehicles;
				//int arrived_vehicles = this->template network_cumulative_arrived_vehicles;

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
						
						int departure_interval=vehicle->template departed_simulation_interval_index<int>();
						//if(vehicle->template departed_simulation_interval_index<int>() == current_time)
						//{
							//PRINT(iteration << ": loading traveler");
							link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().push_back(vehicle);
							link_origin_arrived_vehicles<int&>()++;
							link_origin_cumulative_arrived_vehicles<int&>()++;
							//loaded_vehicles++;
							scenario->template network_cumulative_loaded_vehicles<int&>()++;
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
						double rng = intersection->template rng_stream<User_Space::RngStream&>().RandU01();
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
							vehicle->template load_to_origin_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
							
							//update link state
							link_origin_cumulative_departed_vehicles<int&>()++;
							link_origin_departed_vehicles<int&>()++;
							link_origin_arrived_vehicles<int&>()--;

							push_vehicle<_Vehicle_Interface*>(vehicle);

							//update network state
							//departed_vehicles++;
							//in_network_vehicles++;
							scenario->template network_cumulative_departed_vehicles<int&>()++;
							scenario->template network_in_network_vehicles<int&>()++;
							
							// Bo added: the following logic already implemented in push_vehicle. Don't need to do here

							//if(this==vehicle->template destination_link<Link_Interface*>())
							//{
							//	vehicle->template arrive_to_destination_link<NULLTYPE>(current_simulation_interval_index,simulation_interval_length);
							//	
							//	//update link state: N_destination(a,t)
							//	link_destination_cumulative_arrived_vehicles<int&>()++;
							//	link_destination_arrived_vehicles<int&>()++;
							//	
							//	//update network state:
							//	//arrived_vehicles++;
							//	//in_network_vehicles--;
							//	scenario->template network_cumulative_arrived_vehicles<int&>()++;
							//	scenario->template network_in_network_vehicles<int&>()--;
							//}
						}
					}
				}

				//this->template network_cumulative_loaded_vehicles=loaded_vehicles;
				//this->template network_cumulative_departed_vehicles=departed_vehicles;
				//this->template network_in_network_vehicles=in_network_vehicles;
				//this->template network_cumulative_arrived_vehicles=arrived_vehicles;
			}

			feature_prototype void network_state_update()
			{
				// update N(a,a',t)
				// update A(a,0,t) = A(a,0,t-1) + sum{y(a',a,t)|for all a' in BS(a)};
				// update V(a,t+L/vf) = A(a,0,t) + N_origin(a,t)-N_destination(a,t);
				// N(a,L,t) = D(a,L,t) = N(a,L,t-1) + sum{y(a,a',t)|for all a' in FS(a)};
				// N(a,x,t) = min{N(a,0,t-x/vf),N(a,L,t-(L-x)/w)};
				// update y_demand(a,a',t)

				//input states
				
				//this->template link_destination_arrived_vehicles_array;				//y_destination(a,t) at time t
				//this->template link_destination_cumulative_arrived_vehicles_array;		//N_destination(a,t) by time t

				//this->template link_origin_arrived_vehicles_array;						//y_origin_a(a,t) at time t
				//this->template link_origin_departed_vehicles_array;					//y_origin_d(a,t) at time t

				//this->template link_origin_cumulative_arrive_vehicles_array;			//N_origin(a,t) by time t
				//this->template link_origin_cumulative_departed_vehicles_array;		//N_origin(a,t) by time t
				
				//this->template link_downstream_departed_vehicles_array;				//y_departed(a,t) at time t
				//this->template link_upstream_arrived_vehicles_array;					//y_arrived(a,t) at time t

				//this->template link_upstream_cumulative_arrived_vehicles_array;		//A(a,t) by time t
				//this->template link_downstream_cumulative_vehicles_array;				//N(a,L,t) by time t

				//updated states
				//flow
				//link
				//this->template cached_link_downstream_cumulative_vehicles_array;			//[N(a,L,t-bwtt),N(a,L,t-1)] done
				//this->template cached_link_upstream_cumulative_vehicles_array;				//[N(a,0,t-fftt),N(a,0,t-1)] done
				//this->template cached_link_shifted_cumulative_arrived_vehicles_array;		//[V(a,t-fftt),V(a,t-1)]

				//this->template link_upstream_cumulative_vehicles_array;					//N(a,0,t)
				//turn movement
				//this->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array;	//[N(a,a',0,t-fftt),N(a,a',0,t-1)]
				//this->template cached_turn_movement_downstream_cumulative_vehicles_array;		//[N(a,a',L,t-bwtt),N(a,a',L,t-1)]
				//this->template cached_turn_movement_upstream_cumulative_vehicles_array;			//[V(a,a',t-fftt),V(a,a',t-1]

				//time
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, get_type_of(cached_link_upstream_cumulative_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);

				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();
				int current_time = scenario->template current_time<int>();

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
					t_cached_delay = (current_simulation_interval_index)%scenario->template num_simulation_intervals_per_assignment_interval<int>();
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
				
				this->template link_num_vehicles_in_queue<int>(link_shifted_cumulative_arrived_vehicles - this->template link_downstream_cumulative_vehicles<int>());
				
				//network_reference<_Network_Interface*>()->template increment_link_finish_counter<NULLTYPE>();

				//if (this->template uuid<int>() == 16 && scenario->template current_simulation_interval_index<int>()%1 == 0) // the last link visited for current iteration
				//{
				//	printf("time=%d", scenario->template current_simulation_interval_index<int>());
				//	printf("loaded=%d, departed=%d, in_network=%d, arrived=%d",scenario->template network_cumulative_loaded_vehicles<int>(),scenario->template network_cumulative_departed_vehicles<int>(),scenario->template network_in_network_vehicles<int>(),scenario->template network_cumulative_arrived_vehicles<int>());
				//	printf("\n"); 

				//}
			}

			feature_accessor(link_simulation_status, none, none);
			
			feature_prototype void Initialize()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				load_event(ComponentType,Newells_Conditional,Compute_Step_Flow_Supply_Update,scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>()-1,NULLTYPE);
			}

			/*



			ADD EVENT REGISTER TO TYPE!!!



			*/

			declare_feature_conditional(Newells_Conditional)
			{
				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Link_Interface* _this_ptr=(_Link_Interface*)_pthis;
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				long link_current_revision=_pthis->object_current_revision();
				
				if(_sub_iteration == Scenario_Components::Types::Type_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_ITERATION)
				{
					_pthis->Swap_Event((Event)&Link_Prototype::Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=true;
					response.next=_iteration;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING)
				{
					_pthis->Swap_Event((Event)&Link_Prototype::Compute_Step_Flow_Link_Moving<NULLTYPE>);
					response.result=true;
					response.next=_iteration+_this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();
				}
				else
				{
					response.result=false;
					response.next=_iteration;
				}
			}
			
			feature_prototype string getStatus()
			{
				switch(link_simulation_status<Types::Link_Simulation_Status>())
				{
				case Types::Link_Simulation_Status::NONE_COMPLETE:
					return "NONE_COMPLETE";
				case Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE:
					return "COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE";
				case Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE:
					return "COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE";
				}
			}

			declare_feature_event(Compute_Step_Flow_Supply_Update)
			{

				typedef Link_Prototype<ComponentType,ComponentType> _Link_Interface;
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				
				double calculation_time_start = ::get_current_cpu_time_in_seconds();
				
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;
				//step 1: link supply update based on a given traffic flow model
				//_this_ptr->template link_moving<ComponentType>();
				_this_ptr->template link_supply_update<ComponentType>();

				_this_ptr->template link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE);

				double calculation_time_end = ::get_current_cpu_time_in_seconds();
				_this_ptr->template scenario_reference<_Scenario_Interface*>()->template operation_time_in_seconds<double&>() += calculation_time_end - calculation_time_start;
				//PRINT("\t\t" << "COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE");
			}

			declare_feature_event(Compute_Step_Flow_Link_Moving)
			{
				
				typedef Link_Prototype<ComponentType,ComponentType> _Link_Interface;
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				double calculation_time_start = ::get_current_cpu_time_in_seconds();
				
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;

				//step 7: load vehicles to origin links
				//_this_ptr->template origin_link_loading<ComponentType>();
				
				//step 7.5: link moving -- no link moving in Newell's simplified model -- it can be used to determine turn bay curve
				_this_ptr->template link_moving<ComponentType>();

				//step 8: link network state update
				_this_ptr->template network_state_update<ComponentType>();

				_this_ptr->template link_simulation_status<Types::Link_Simulation_Status>(Types::Link_Simulation_Status::COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE);
				
				double calculation_time_end = ::get_current_cpu_time_in_seconds();
				_this_ptr->template scenario_reference<_Scenario_Interface*>()->template operation_time_in_seconds<double&>() += calculation_time_end - calculation_time_start;

				//PRINT("\t\t" << "COMPUTE_STEP_FLOW_LINK_MOVING_COMPLETE");
			}
		};

	}
}

using namespace Link_Components::Prototypes;