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

			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// queueing link
			//------------------------------------------------------------------------------------------------------------------

			// fast way to check if any external entities are trying to access this feature
			//feature_accessor(link_destination_cumulative_arrived_vehicles, requires(check_2(CallerType,ComponentType,is_same)), requires(check_2(CallerType,ComponentType,is_same)));
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

			feature_prototype void push_vehicle(TargetType vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			feature_prototype void accept_vehicle(TargetType vehicle)
			{
				this_component()->accept_vehicle<ComponentType,CallerType,TargetType>(vehicle);
			}

			feature_prototype typename TargetType::ReturnType link_supply_update(typename TargetType::ParamType supply_container,typename TargetType::Param2Type capacity_container)
			{
				this_component()->link_supply_update<ComponentType,CallerType,TargetType>(supply_container, capacity_container);
			}
			
			feature_prototype void link_moving()
			{
				this_component()->link_moving<ComponentType,CallerType,TargetType>();
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
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void reset_routable_link()
			{
				this_component()->reset_routable_link<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void construct_routable_from_regular(TargetType regular_link)
			{
				this_component()->construct_routable_from_regular<ComponentType,CallerType,TargetType>(regular_link);
			}
		};

	}
}

using namespace Link_Components::Prototypes;