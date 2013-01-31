#pragma once


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
			// feature_accessor(link_destination_cumulative_arrived_vehicles, requires(check_2(CallerType,ComponentType,is_same)), requires(check_2(CallerType,ComponentType,is_same)));
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
			feature_accessor(link_shifted_cumulative_arrived_vehicles, none, none);
			feature_accessor(link_upstream_cumulative_vehicles, none, none);
			feature_accessor(link_downstream_cumulative_vehicles, none, none);
			
			//cumulative - Begin/End-Trip
			feature_accessor(link_origin_cumulative_arrived_vehicles, none, none);
			feature_accessor(link_origin_cumulative_departed_vehicles, none, none);
			feature_accessor(cached_link_shifted_cumulative_arrived_vehicles, none, none);
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

			feature_prototype void push_vehicle(TargetType vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			feature_prototype void accept_vehicle(TargetType vehicle)
			{
				this_component()->accept_vehicle<ComponentType,CallerType,TargetType>(vehicle);
			}

			feature_prototype void link_supply_update()
			{
				this_component()->link_supply_update<ComponentType,CallerType,TargetType>();
			}
			
			feature_prototype void link_moving()
			{
				this_component()->link_moving<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void origin_link_loading(TargetType param)
			{
				this_component()->origin_link_loading<ComponentType,CallerType,TargetType>(param);
			}

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

			feature_prototype void initialize_features(TargetType param)
			{
				this_component()->initialize_features<ComponentType,CallerType,TargetType>(param);
			}
		};
	}
}

using namespace Link_Components::Prototypes;