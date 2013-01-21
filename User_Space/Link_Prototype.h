#pragma once
#include "User_Space_Includes.h"
#include "Vehicle_Prototype.h"

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
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Link_Prototype
		{
			tag_as_prototype;
			
			//==================================================================================================================
			/// basic link
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(network_reference, none, none);
			feature_accessor(upstream_intersection, none, none);
			feature_accessor(downstream_intersection, none, none);
			feature_accessor(length, none, none);
			feature_accessor(link_type, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// transportation link
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(speed_limit, none, none);
			feature_accessor(num_lanes, none, none);
			feature_accessor(travel_time, none, none);
			feature_accessor(inbound_turn_movements, none, none);
			feature_accessor(outbound_turn_movements, none, none);
			feature_accessor(num_left_turn_bays, none, none);
			feature_accessor(num_right_turn_bays, none, none);
			feature_accessor(left_turn_bay_length, none, none);
			feature_accessor(right_turn_bay_length, none, none);
			feature_accessor(approach, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// link with control
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(detector, none, none);
			feature_accessor(lane_groups, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// routable link
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(f_cost, none, none);
			feature_accessor(h_cost, none, none);
			feature_accessor(label_pointer, none, none);
			feature_accessor(label_cost, none, none);
			feature_accessor(scan_list_status, none, none);
			feature_accessor(network_link_reference, none, none);
			feature_accessor(replicas_container, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// simulation link
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(scenario_reference, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// queueing link
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(link_destination_cumulative_arrived_vehicles, none, none);
			feature_accessor(link_destination_arrived_vehicles, none, none);
			feature_accessor(link_destination_vehicle_queue, none, none);
			feature_accessor(current_vehicle_queue, none, none);	

			feature_accessor(link_fftt, none, none);
			feature_accessor(link_bwtt, none, none);
			feature_accessor(link_fftt_cached_simulation_interval_size, none, none);
			feature_accessor(link_bwtt_cached_simulation_interval_size, none, none);
			
			feature_accessor(link_capacity, none, none);
			feature_accessor(link_supply, none, none);
			
			feature_accessor(link_upstream_arrived_vehicles, none, none);
			feature_accessor(link_downstream_departed_vehicles, none, none);
			
			feature_accessor(link_origin_arrived_vehicles, none, none);
			feature_accessor(link_origin_departed_vehicles, none, none);
			feature_accessor(link_origin_loaded_vehicles, none, none);
			


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
		
			feature_accessor(maximum_flow_rate, none, none);
			feature_accessor(free_flow_speed, none, none);
			feature_accessor(backward_wave_speed, none, none);
			feature_accessor(jam_density, none, none);
			feature_accessor(critical_density, none, none);
			feature_accessor(link_num_vehicles_in_queue, none, none);

			//------------------------------------------------------------------------------------------------------------------

			feature_prototype void push_vehicle(TargetType vehicle, float a_delayed_time)
			{
				accept_vehicle<TargetType>(vehicle, a_delayed_time);
			}

			feature_prototype void load_vehicle(TargetType* veh/*,requires(TargetType,IsUnloaded)*/)
			{
				link_origin_cumulative_arrived_vehicles<int&>()++;
				define_container_and_value_interface(_Link_Origin_Vehicles_Container_Interface, _Vehicle_Interface,get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)veh;
				link_origin_vehicle_array<_Link_Origin_Vehicles_Container_Interface&>().push_back(vehicle);
				vehicle->template load<Vehicle_Components::Types::Load_To_Entry_Queue>();
			}

			feature_prototype void accept_vehicle(TargetType veh, float a_delayed_time/*,requires(TargetType,IsLoaded)*/)
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicle_Queue_Interface, _Vehicle_Interface, get_type_of(current_vehicle_queue), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_and_value_interface(_Destination_Vehicle_Queue_Interface, _Vehicle_Interface_2, get_type_of(link_destination_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);				

				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();
				_Vehicle_Interface* vehicle=(_Vehicle_Interface*)veh;
				_Movement_Plan_Interface* mp = vehicle->movement_plan<_Movement_Plan_Interface*>();
				mp->template transfer_to_next_link<NULLTYPE>(a_delayed_time);

				if(this->template internal_id<int>()==(mp->template destination<_Link_Interface*>())->template internal_id<int>())
				{
					vehicle->template unload<NULLTYPE>();
					link_destination_cumulative_arrived_vehicles<int&>()++;
					link_destination_arrived_vehicles<int&>()++;
					this->template link_destination_vehicle_queue<_Destination_Vehicle_Queue_Interface&>().push_back(vehicle);
					scenario->template network_cumulative_arrived_vehicles<int&>()++;
					scenario->template network_in_network_vehicles<int&>()--;
				}
				else
				{
					current_vehicle_queue<_Vehicle_Queue_Interface&>().push_back(vehicle);
				}
			}
			
			feature_prototype void link_supply_update()
			{
				this_component()->link_supply_update<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void link_moving()
			{
				define_container_and_value_interface(_Vehicle_Queue_Interface, _Vehicle_Interface, get_type_of(current_vehicle_queue), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);

				_Intersection_Interface* intersection=downstream_intersection<_Intersection_Interface*>();

				typename _Vehicle_Queue_Interface::iterator vehicle_itr;

				_Vehicle_Queue_Interface& cur_vehicle_queue=current_vehicle_queue<_Vehicle_Queue_Interface&>();

				for(vehicle_itr=cur_vehicle_queue.begin();vehicle_itr!=cur_vehicle_queue.end();vehicle_itr++)
				{
					intersection->template push_vehicle<NULLTYPE>((*vehicle_itr), -1, -1);
				}

				cur_vehicle_queue.clear();
			}

			//feature_prototype void origin_link_loading()
			//{

			//	define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			//	define_component_interface(_Intersection_Interface, get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			//	define_container_and_value_interface(_Vehicles_Origin_Container_Interface, _Vehicle_Interface,get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

			//	define_container_interface(_Vehicle_Origin_Queue_Interface, get_type_of(link_origin_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

			//	_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

			//	int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
			//	int simulation_interval_length = scenario->template simulation_interval_length<int>();


			//	//int loaded_vehicles = this->template network_cumulative_loaded_vehicles;
			//	//int departed_vehicles = this->template network_cumulative_departed_vehicles;
			//	//int in_network_vehicles = this->template network_in_network_vehicles;
			//	//int arrived_vehicles = this->template network_cumulative_arrived_vehicles;

			//	// N_origin(a,t) = N_origin(a,t-1) + y_origin(a,t)
			//	// update N(a,0,t) = D(a,0,t) = min{A(a,0,t)+N_origin(a,t)-N_destination(a,t),N(a,L,t-L/w)};
			//	// update A(a,0,t) = A(a,0,t-1) + sum{y(a',a,t)|for all a' in BS(a)};
			//	// update V(a,t+L/vf) = A(a,0,t) + N_origin(a,t)-N_destination(a,t);
			//	// N(a,L,t) = D(a,L,t) = N(a,L,t-1) + sum{y(a,a',t)|for all a' in FS(a)};
			//	// N(a,x,t) = min{N(a,0,t-x/vf),N(a,L,t-(L-x)/w)};
			//	// update y_demand(a,a',t)
			//	


			//	_Vehicle_Interface* vehicle;
			//		outbound_link->template link_origin_departed_vehicles<int>(0.0);
			//		outbound_link->template link_origin_loaded_vehicles<int>(0.0);
			//	//arrived vehicles at current interval
			//	int current_position = link_origin_vehicle_current_position<int>();
			//	if(current_position<(int)link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>().size())
			//	{
			//		for(int iv=current_position;iv<(int)link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>().size();iv++)
			//		{
			//			//update current position
			//			link_origin_vehicle_current_position<int&>()++;

			//			vehicle=(_Vehicle_Interface*)link_origin_vehicle_array<_Vehicles_Origin_Container_Interface&>()[iv];
			//			
			//			link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().push_back(vehicle);
			//			link_origin_arrived_vehicles<int&>()++;
			//			link_origin_loaded_vehicles<int&>()++;
			//			link_origin_cumulative_arrived_vehicles<int&>()++;
			//			scenario->template network_cumulative_loaded_vehicles<int&>()++;

			//		}
			//	}

			//	//have demand
			//	if(link_origin_arrived_vehicles<int>()>0)
			//	{
			//		//check available spaces
			//		float link_available_spaces = link_supply<float>() - link_upstream_arrived_vehicles<float>();
			//		int num_link_origin_departed_vehicles_allowed = (int) link_available_spaces;
			//		float link_origin_departed_flow_allowed = (float) (link_available_spaces - num_link_origin_departed_vehicles_allowed);
			//		
			//		if (link_origin_departed_flow_allowed>0.0)
			//		{//partial vehicle
			//			_Intersection_Interface* intersection = upstream_intersection<_Intersection_Interface*>();
			//			double rng = intersection->template rng_stream<User_Space::RngStream&>().RandU01();
			//			if(rng<=link_origin_departed_flow_allowed)
			//			{//partial vehicle, incomplete implementation
			//				++num_link_origin_departed_vehicles_allowed;
			//			}
			//		}

			//		if(num_link_origin_departed_vehicles_allowed>0)
			//		{//
			//			int num_departed_vehicles = min(link_origin_arrived_vehicles<int>(),num_link_origin_departed_vehicles_allowed);
			//			for (int iv=0;iv<num_departed_vehicles;iv++)
			//			{//
			//				vehicle=(_Vehicle_Interface*)link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().front();
			//				link_origin_vehicle_queue<_Vehicle_Origin_Queue_Interface&>().pop_front();

			//				//update vehicle state
			//				vehicle->template load<Load_To_Origin_Link>();
			//				
			//				//update link state
			//				link_origin_cumulative_departed_vehicles<int&>()++;
			//				link_origin_departed_vehicles<int&>()++;
			//				link_origin_arrived_vehicles<int&>()--;

			//				push_vehicle<_Vehicle_Interface*>(vehicle, 0.0);

			//				scenario->template network_cumulative_departed_vehicles<int&>()++;
			//				scenario->template network_in_network_vehicles<int&>()++;

			//			}
			//		}
			//	}

			//}

			feature_prototype void network_state_update()
			{
				this_component()->network_state_update<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void Initialize()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				load_event(ComponentType,Newells_Conditional,Compute_Step_Flow_Supply_Update,scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION,NULLTYPE);
			}

			declare_feature_conditional(Newells_Conditional)
			{
				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Link_Interface* _this_ptr=(_Link_Interface*)_pthis;
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION)
				{
					_pthis->Swap_Event((Event)&Link_Prototype::Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration;
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION)
				{
					_pthis->Swap_Event((Event)&Link_Prototype::Compute_Step_Flow_Link_Moving<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration+_this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION;
				}
				else
				{
					assert(false);
					cout << "Should never reach here in link conditional!" << endl;
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

				double calculation_time_end = ::get_current_cpu_time_in_seconds();
				_this_ptr->template scenario_reference<_Scenario_Interface*>()->template operation_time_in_seconds<double&>() += calculation_time_end - calculation_time_start;
			}

			declare_feature_event(Compute_Step_Flow_Link_Moving)
			{
				
				typedef Link_Prototype<ComponentType,ComponentType> _Link_Interface;
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;

				//step 7: load vehicles to origin links
				//_this_ptr->template origin_link_loading<ComponentType>();
				
				//step 7.5: link moving -- no link moving in Newell's simplified model -- it can be used to determine turn bay curve
				_this_ptr->template link_moving<ComponentType>();

				//step 8: link network state update
				_this_ptr->template network_state_update<ComponentType>();
			}
		};

	}
}

using namespace Link_Components::Prototypes;