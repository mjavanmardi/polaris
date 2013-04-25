#pragma once
#include "User_Space_Includes.h"

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
			LIGHT_RAILWAY,
			HEAVY_RAILWAY,
			WALKWAY,
			BIKEWAY,
			WATERWAY,
		};
	}

	namespace Concepts
	{
		concept struct Is_Basic_Link_Prototype
		{
			check_getter(has_upstream_intersection, Component_Type::upstream_intersection);
			check_getter(has_downstream_intersection, Component_Type::downstream_intersection);
			check_getter(has_network_reference, Component_Type::network_reference);
			check_getter(has_link_type, Component_Type::link_type);  
			check_getter(has_uuid, Component_Type::uuid); 
			check_getter(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
		};
		concept struct Is_Basic_Link
		{
			check_getter(has_upstream_intersection, upstream_intersection);
			check_getter(has_downstream_intersection, downstream_intersection);
			check_getter(has_network_reference, network_reference);
			check_getter(has_link_type, link_type); 
			check_getter(has_uuid, uuid); 
			check_getter(has_internal_id, internal_id); 

			check_concept(is_basic_link_prototype, Is_Basic_Link_Prototype);
			define_sub_check(is_basic_link, has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
			define_default_check(is_basic_link || is_basic_link_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Link_Prototype ADD_DEBUG_INFO
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
			feature_accessor(activity_locations, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// transportation link
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(speed_limit, none, none);
			feature_accessor(num_lanes, none, none);
			feature_accessor(travel_time, none, none);
			feature_accessor(realtime_travel_time, none, none);
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
			feature_accessor(scan_list_position, none, none);
			feature_accessor(reset_list_status, none, none);
			feature_accessor(network_link_reference, none, none);
			feature_accessor(replicas_container, none, none);
			feature_accessor(realtime_replicas_container, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// simulation link
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(link_vmt, none, none);
			feature_accessor(link_vht, none, none);
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
			feature_accessor(original_free_flow_speed, none, none);
			feature_accessor(original_maximum_flow_rate, none, none);
			
			// event-aware link
			feature_accessor(event_affected, none, none);
			feature_accessor(advisory_radio, none, none);
			feature_accessor(depot, none, none);
			feature_accessor(variable_word_sign, none, none);
			feature_accessor(variable_speed_sign, none, none);

			feature_accessor(weather_event_to_process, none, none);
			feature_accessor(current_weather_event, none, none);
			feature_accessor(accident_event_to_process, none, none);
			feature_accessor(current_accident_event, none, none);
			
			// ITS-enabled link
			feature_accessor(advisory_radio_events, none, none);
			
			feature_prototype void Push_ITS(TargetType its)
			{
				this_component()->template Accept_ITS<ComponentType,CallerType,TargetType>(its);
			}

			//visualized link
			feature_accessor(displayed_line, none, none);

			feature_prototype void push_vehicle(TargetType vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			feature_prototype void accept_vehicle(TargetType vehicle)
			{
				this_component()->template accept_vehicle<ComponentType,CallerType,TargetType>(vehicle);
			}

			feature_prototype void link_supply_update()
			{
				this_component()->template link_supply_update<ComponentType,CallerType,TargetType>();
			}
			
			feature_prototype void link_moving()
			{
				this_component()->template link_moving<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void origin_link_loading(TargetType param)
			{
				this_component()->template origin_link_loading<ComponentType,CallerType,TargetType>(param);
			}

			feature_prototype void network_state_update()
			{
				this_component()->template network_state_update<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void Initialize()
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void reset_routable_link()
			{
				this_component()->template reset_routable_link<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void construct_routable_from_regular(TargetType regular_link)
			{
				this_component()->template construct_routable_from_regular<ComponentType,CallerType,TargetType>(regular_link);
			}

			feature_prototype void construct_realtime_routable_from_regular(TargetType regular_link)
			{
				this_component()->template construct_realtime_routable_from_regular<ComponentType,CallerType,TargetType>(regular_link);
			}

			feature_prototype void initialize_features(TargetType param)
			{
				this_component()->template initialize_features<ComponentType,CallerType,TargetType>(param);
			}

			feature_prototype void calculate_moe_for_simulation_interval()
			{
				this_component()->template calculate_moe_for_simulation_interval<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void calculate_moe_for_assignment_interval()
			{
				this_component()->template calculate_moe_for_assignment_interval<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void update_vmt_vht()
			{
				this_component()->template update_vmt_vht<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void visualize_moe()
			{
				this_component()->template visualize_moe<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void configure_displayed_line()
			{
				this_component()->template configure_displayed_line<ComponentType,CallerType,TargetType>();
			}
			
			feature_prototype void handle_events()
			{
				this_component()->template handle_events<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void get_events_from_vms()
			{
				this_component()->template get_events_from_vms<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void get_events_from_har()
			{
				this_component()->template get_events_from_har<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void get_link_moe(int& start_time, int& end_time, int& volume, float& speed, float& density)
			{
				this_component()->template get_link_moe<ComponentType,CallerType,TargetType>(start_time, end_time, volume, speed, density);
			}
		};
	}
}

using namespace Link_Components::Prototypes;