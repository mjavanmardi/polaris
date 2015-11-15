#pragma once

#include "Core.h"


namespace Activity_Location_Components
{
	namespace Concepts
	{
		concept struct Is_Activity_Location_Prototype 
		{
			check_template_method_name(has_origin_links, Component_Type::template origin_links);
			check_template_method_name(has_destination_links, Component_Type::template destination_links);
			check_template_method_name(has_zone, Component_Type::template zone);
			check_template_method_name(has_uuid, Component_Type::template uuid);
			check_template_method_name(has_internal_id, Component_Type::template internal_id);
			define_default_check(has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
		};

		concept struct Is_Activity_Location
		{
			check_template_method_name(has_origin_links, template origin_links);
			check_template_method_name(has_destination_links, template destination_links);
			check_template_method_name(has_zone, template zone);
			check_template_method_name(has_uuid, template uuid);
			check_template_method_name(has_internal_id, template internal_id);

			check_concept(is_activity_location_prototype, Is_Activity_Location_Prototype, T, V);
			define_sub_check(is_activity_location, has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
			define_default_check(is_activity_location || is_activity_location_prototype);
		};
	}
}

namespace Link_Components
{
	namespace Concepts
	{
		concept struct Is_Basic_Link_Prototype
		{
			check_template_method_name(has_upstream_intersection, Component_Type::template upstream_intersection);
			check_template_method_name(has_downstream_intersection, Component_Type::template downstream_intersection);
			check_template_method_name(has_network_reference, Component_Type::template network_reference);
			check_template_method_name(has_link_type, Component_Type::template link_type);
			check_template_method_name(has_uuid, Component_Type::template uuid);
			check_template_method_name(has_internal_id, Component_Type::template internal_id);
			define_default_check(has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
		};

		concept struct Is_Basic_Link
		{
			check_template_method_name(has_upstream_intersection, template upstream_intersection);
			check_template_method_name(has_downstream_intersection, template downstream_intersection);
			check_template_method_name(has_network_reference, template network_reference);
			check_template_method_name(has_link_type, template link_type);
			check_template_method_name(has_uuid, template uuid);
			check_template_method_name(has_internal_id, template internal_id);

			check_concept(is_basic_link_prototype, Is_Basic_Link_Prototype, T, V);
			define_sub_check(is_basic_link, has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
			define_default_check(is_basic_link || is_basic_link_prototype);
		};
	}
}

namespace Movement_Plan_Components
{
	namespace Concepts
	{	
		concept struct Is_Movement_Plan_Prototype
		{
			check_template_method_name(has_trajectory, Component_Type::template trajectory_container);
			check_template_method_name(has_origin, Component_Type::template origin);
			check_template_method_name(has_destination, Component_Type::template destination);
			check_template_method_name(has_departed_time, Component_Type::template departed_time);
			check_template_method_name(has_arrived_time, Component_Type::template arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};

		concept struct Is_Movement_Plan
		{
			check_template_method_name(has_trajectory, template trajectory_container);
			check_template_method_name(has_origin, template origin);
			check_template_method_name(has_destination, template destination);
			check_template_method_name(has_departed_time, template departed_time);
			check_template_method_name(has_arrived_time, template arrived_time);
			check_concept(is_prototype, Is_Movement_Plan_Prototype, T, V);
			define_default_check(is_prototype || (has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time));
		};

		concept struct Is_Movement_Record_Prototype
		{
			check_template_method_name(has_trajectory, Component_Type::template trajectory_container);
			define_default_check(has_trajectory);
		};
	}
}

namespace Network_Event_Components
{
	namespace Concepts
	{
		concept struct Is_Weather_Event_Prototype
		{
			check_template_method_name(has_type,Component_Type::template weather_type);
			check_template_method_name(has_precipitation_depth,Component_Type::template precipitation_depth);
			check_template_method_name(has_visibility,Component_Type::template visibility);
			check_template_method_name(has_temperature,Component_Type::template temperature);
			check_template_method_name(has_wind_speed,Component_Type::template wind_speed);
			define_default_check(has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
		};
		concept struct Is_Weather_Event
		{
			check_template_method_name(has_type,template weather_type);
			check_template_method_name(has_precipitation_depth,template precipitation_depth);
			check_template_method_name(has_visibility,template visibility);
			check_template_method_name(has_temperature,template temperature);
			check_template_method_name(has_wind_speed,template wind_speed);
			check_concept(is_prototype, Is_Weather_Event_Prototype, T, V);
			define_sub_check(is_component, has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
			define_default_check(is_prototype || is_component);
		};
		concept struct Is_Accident_Event_Prototype
		{
			check_template_method_name(has_type,Component_Type::template accident_type);
			check_template_method_name(has_severity,Component_Type::template severity);
			check_template_method_name(has_lanes,Component_Type::template lanes);
			define_default_check(has_type & has_severity & has_lanes);
		};
		concept struct Is_Accident_Event
		{
			check_template_method_name(has_type,template accident_type);
			check_template_method_name(has_severity,template severity);
			check_template_method_name(has_lanes,template lanes);
			check_concept(is_prototype, Is_Accident_Event_Prototype, T, V);
			define_sub_check(is_component, has_type & has_severity & has_lanes);
			define_default_check(is_prototype || is_component);
		};
	}
}

namespace Network_Components
{
	namespace Concepts
	{
		concept struct Is_Basic_Network_Prototype
		{
			check_template_method_name(has_intersections, Component_Type::template intersections_container);
			check_template_method_name(has_links, Component_Type::template links_container);
			check_template_method_name(has_read_function, Component_Type::template read_network_data);
			define_default_check(has_intersections && has_links && has_read_function);
		};

		concept struct Is_Basic_Network
		{
			check_template_method_name(has_intersections, template intersections_container);
			check_template_method_name(has_links, template links_container);
			check_template_method_name(has_read_function, template read_network_data);

			check_concept(is_basic_network_prototype, Is_Basic_Network_Prototype, T, V);
			define_sub_check(is_basic_network, has_intersections && has_links && has_read_function);
			define_default_check(is_basic_network || is_basic_network_prototype);
		};
		
		concept struct Is_Transportation_Network_Prototype
		{
			check_concept(is_basic_network, Is_Basic_Network_Prototype, T, V);
			check_template_method_name(has_turns, Component_Type::template turn_movements_container);
			check_template_method_name(has_locations, Component_Type::template activity_locations_container);
			check_template_method_name(has_zones, Component_Type::template zones_container);
			define_default_check(is_basic_network && has_turns && has_locations && has_zones);
		};

		concept struct Is_Transportation_Network
		{
			check_concept(is_basic_network, Is_Basic_Network, T, V);
			check_template_method_name(has_turns, template turn_movements_container);
			check_template_method_name(has_locations, template activity_locations_container);
			check_template_method_name(has_zones, template zones_container);
			//define_default_check(is_basic_network && has_turns && has_locations && has_zones);
			check_concept(is_transportation_network_prototype, Is_Transportation_Network_Prototype, T, V);
			define_sub_check(is_transportation_network, is_basic_network && has_turns && has_locations && has_zones);
			define_default_check(is_transportation_network || is_transportation_network_prototype);
		};
		
		concept struct Is_Routable_Network_Prototype
		{
			check_concept(is_basic_network, Is_Basic_Network_Prototype, T, V);
			check_template_method_name(has_routable_network, Component_Type::template routable_network);
			check_template_method_name(has_routable_networks_container, Component_Type::template routable_networks_container);
			check_template_method_name(has_scan_list, Component_Type::template scan_list);
			define_default_check(is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
		};

		concept struct Is_Routable_Network
		{
			check_concept(is_basic_network, Is_Basic_Network, T, V);
			check_template_method_name(has_routable_network, template routable_network);
			check_template_method_name(has_routable_networks_container, template routable_networks_container);
			check_template_method_name(has_scan_list, template scan_list);
			check_concept(is_routable_network_prototype, Is_Routable_Network_Prototype, T, V);
			define_sub_check(is_routable_network, is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
			define_default_check(is_routable_network || is_routable_network_prototype);
		};

		concept struct Is_Simulation_Network_Prototype
		{
			check_concept(is_routable_network, Is_Routable_Network_Prototype, T, V);
			check_template_method_name(has_scenario_reference, template scenario_reference);
			check_template_method_name(has_max_free_flow_speed, template max_free_flow_speed);
			define_default_check(is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Simulation_Network
		{
			check_concept(is_routable_network, Is_Routable_Network, T, V);
			check_template_method_name(has_scenario_reference, template scenario_reference);
			check_template_method_name(has_max_free_flow_speed, template max_free_flow_speed);
			check_concept(is_simulation_network_prototype, Is_Simulation_Network_Prototype, T, V);
			define_sub_check(is_simulation_networ, is_routable_network && has_scenario_reference && has_max_free_flow_speed);
			define_default_check(is_simulation_networ || is_simulation_network_prototype);
		};		
		
		concept struct Is_Trasportation_Simulation_Network_Prototype
		{
			check_concept(is_transportation_network, Is_Transportation_Network_Prototype, T, V);
			check_concept(is_routable_network, Is_Routable_Network_Prototype, T, V);
			check_template_method_name(has_scenario_reference, Component_Type::template scenario_reference);
			check_template_method_name(has_max_free_flow_speed, Component_Type::template max_free_flow_speed);
			define_default_check(is_transportation_network && is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Trasportation_Simulation_Network
		{
			check_concept(is_transportation_network, Is_Transportation_Network, T, V);
			check_concept(is_routable_network, Is_Routable_Network, T, V);
			check_template_method_name(has_scenario_reference, template scenario_reference);
			check_template_method_name(has_max_free_flow_speed, template max_free_flow_speed);

			check_concept(is_transportation_simulation_network_prototype, Is_Trasportation_Simulation_Network_Prototype, T, V);
			define_sub_check(is_transportation_simulation_network, is_transportation_network && is_routable_network && has_scenario_reference && has_max_free_flow_speed);
			define_default_check(is_transportation_simulation_network || is_transportation_simulation_network_prototype);
		};
	}
}

namespace Network_Skimming_Components
{
	namespace Concepts
	{
		concept struct Is_LOS_Prototype
		{
			check_template_method_name(has_auto_ttime, Component_Type::template auto_ttime);
			check_template_method_name(has_transit_ttime, Component_Type::template transit_ttime);
			define_default_check(has_auto_ttime && has_transit_ttime);
		};
	} 
}

namespace Routing_Components
{
	namespace Concepts
	{
		concept struct Is_One_To_One_Router_Prototype
		{
			check_template_method_name(has_routable_network, ComponentType::template routable_network);
			check_template_method_name(has_routable_origin, ComponentType::template routable_origin);
			check_template_method_name(has_routable_destination, ComponentType::template routable_destination);
			check_template_method_name(has_network, ComponentType::template network);
			check_template_method_name(has_vehicle, ComponentType::template vehicle);
			define_default_check(has_routable_network && has_routable_origin && has_routable_destination && has_network && has_vehicle );
		};
		concept struct Is_One_To_One_Router
		{
			check_template_method_name(has_routable_network, template routable_network);
			check_template_method_name(has_routable_origin, template routable_origin);
			check_template_method_name(has_routable_destination, template routable_destination);
			check_template_method_name(has_network, template network);
			check_template_method_name(has_vehicle, template vehicle);
			check_concept(is_one_to_one_prototype, Is_One_To_One_Router_Prototype, T, V);
			define_default_check(is_one_to_one_prototype || (has_routable_network && has_routable_origin && has_routable_destination && has_network && has_vehicle ));
		};

		concept struct Is_One_To_All_Router_Prototype
		{
			check_template_method_name(Has_Link_Times_Container, ComponentType::template travel_times_to_link_container);
			check_concept(Has_One_To_One_Router, Is_One_To_One_Router_Prototype, T, V);
			define_default_check(Has_One_To_One_Router && Has_Link_Times_Container);
		};
		concept struct Is_One_To_All_Router
		{
			check_template_method_name(Has_Link_Times_Container, template travel_times_to_link_container);
			check_concept(Has_One_To_One_Router, Is_One_To_One_Router, T, V);
			check_concept(is_one_to_all_prototype, Is_One_To_All_Router_Prototype, T, V);
			define_default_check(is_one_to_all_prototype || (Has_One_To_One_Router && Has_Link_Times_Container));
		};
	}
}

namespace Scenario_Components
{
	namespace Concepts
	{
		concept struct Has_Popsyn_Configuration_Data
		{
			check_template_method_name(has_percent_to_synthesize_p, Component_Type::template percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance_p, Component_Type::template ipf_tolerance);
			check_template_method_name(has_marginal_tolerance_p, Component_Type::template marginal_tolerance);
			check_template_method_name(has_maximum_iterations_p, Component_Type::template maximum_iterations);
			check_template_method_name(has_write_marginal_output_p, Component_Type::template write_marginal_output);
			check_template_method_name(has_write_full_output_p, Component_Type::template write_full_output);
			check_template_method_name(has_popsyn_control_file_name_p, Component_Type::template popsyn_control_file_name);

			check_template_method_name(has_percent_to_synthesize, template percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance, template ipf_tolerance);
			check_template_method_name(has_marginal_tolerance, template marginal_tolerance);
			check_template_method_name(has_maximum_iterations, template maximum_iterations);
			check_template_method_name(has_write_marginal_output,template write_marginal_output);
			check_template_method_name(has_write_full_output,template write_full_output);
			check_template_method_name(has_popsyn_control_file_name,template popsyn_control_file_name);
			define_sub_check(Has_Popsyn_Configuration_Data_Prototype, has_percent_to_synthesize_p && has_ipf_tolerance_p && has_marginal_tolerance_p && has_maximum_iterations_p && has_write_marginal_output_p && has_write_full_output_p && has_popsyn_control_file_name_p);
			define_sub_check(Has_Popsyn_Configuration_Data_Component, has_percent_to_synthesize && has_ipf_tolerance && has_marginal_tolerance && has_maximum_iterations && has_write_marginal_output && has_write_full_output && has_popsyn_control_file_name);
			define_default_check(Has_Popsyn_Configuration_Data_Prototype || Has_Popsyn_Configuration_Data_Component);
		};
	}
}

namespace Traveler_Components
{
	namespace Concepts
	{
		concept struct Is_Traveler_Prototype
		{
			check_template_method_name(has_router,ComponentType::template router);
			check_template_method_name(has_vehicle,ComponentType::template vehicle);
			define_default_check(has_router && has_vehicle);
		};
		concept struct Is_Traveler
		{
			check_template_method_name(has_router,template router);
			check_template_method_name(has_vehicle,template vehicle);
			check_concept(is_prototype, Is_Traveler_Prototype, T, V);
			define_default_check(is_prototype || (has_router && has_vehicle));
		};
	}
}

namespace Zone_Components
{
	namespace Concepts
	{
		concept struct Is_Zone_Prototype
		{
			check_template_method_name(has_origin_activity_locations, Component_Type::template origin_activity_locations);
			check_template_method_name(has_destination_activity_locations, Component_Type::template destination_activity_locations);
			check_template_method_name(has_uuid, Component_Type::template uuid);
			check_template_method_name(has_internal_id, Component_Type::template internal_id);
			define_default_check(has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
		};
		concept struct Is_Zone
		{
			check_template_method_name(has_origin_activity_locations, template origin_activity_locations);
			check_template_method_name(has_destination_activity_locations, template destination_activity_locations);
			check_template_method_name(has_uuid, template uuid);
			check_template_method_name(has_internal_id, template internal_id);

			check_concept(is_zone_prototype, Is_Zone_Prototype, T, V);
			define_sub_check(is_zone_component, has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
			define_default_check(is_zone_component || is_zone_prototype);
		};
	}
}