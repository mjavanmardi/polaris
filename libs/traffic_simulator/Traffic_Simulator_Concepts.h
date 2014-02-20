#pragma once

#include "Core.h"


namespace Activity_Location_Components
{
	namespace Concepts
	{
		concept struct Is_Activity_Location_Prototype 
		{
			check_template_method_name(has_origin_links, Component_Type::origin_links);
			check_template_method_name(has_destination_links, Component_Type::destination_links);
			check_template_method_name(has_zone, Component_Type::zone); 
			check_template_method_name(has_uuid, Component_Type::uuid); 
			check_template_method_name(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
		};

		concept struct Is_Activity_Location
		{
			check_template_method_name(has_origin_links, origin_links);
			check_template_method_name(has_destination_links, destination_links);
			check_template_method_name(has_zone, zone); 
			check_template_method_name(has_uuid, uuid); 
			check_template_method_name(has_internal_id, internal_id); 

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
			check_template_method_name(has_upstream_intersection, Component_Type::upstream_intersection);
			check_template_method_name(has_downstream_intersection, Component_Type::downstream_intersection);
			check_template_method_name(has_network_reference, Component_Type::network_reference);
			check_template_method_name(has_link_type, Component_Type::link_type);  
			check_template_method_name(has_uuid, Component_Type::uuid); 
			check_template_method_name(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
		};

		concept struct Is_Basic_Link
		{
			check_template_method_name(has_upstream_intersection, upstream_intersection);
			check_template_method_name(has_downstream_intersection, downstream_intersection);
			check_template_method_name(has_network_reference, network_reference);
			check_template_method_name(has_link_type, link_type); 
			check_template_method_name(has_uuid, uuid); 
			check_template_method_name(has_internal_id, internal_id); 

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
			check_template_method_name(has_trajectory, Component_Type::trajectory_container);
			check_template_method_name(has_origin, Component_Type::origin);
			check_template_method_name(has_destination, Component_Type::destination);
			check_template_method_name(has_departed_time, Component_Type::departed_time);
			check_template_method_name(has_arrived_time, Component_Type::arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};

		concept struct Is_Movement_Plan
		{
			check_template_method_name(has_trajectory, trajectory_container);
			check_template_method_name(has_origin, origin);
			check_template_method_name(has_destination, destination);
			check_template_method_name(has_departed_time, departed_time);
			check_template_method_name(has_arrived_time, arrived_time);
			check_concept(is_prototype, Is_Movement_Plan_Prototype, T, V);
			define_default_check(is_prototype || (has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time));
		};

		concept struct Is_Movement_Record_Prototype
		{
			check_template_method_name(has_trajectory, Component_Type::trajectory_container);
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
			check_template_method_name(has_type,Component_Type::weather_type);
			check_template_method_name(has_precipitation_depth,Component_Type::precipitation_depth);
			check_template_method_name(has_visibility,Component_Type::visibility);
			check_template_method_name(has_temperature,Component_Type::temperature);
			check_template_method_name(has_wind_speed,Component_Type::wind_speed);
			define_default_check(has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
		};
		concept struct Is_Weather_Event
		{
			check_template_method_name(has_type,weather_type);
			check_template_method_name(has_precipitation_depth,precipitation_depth);
			check_template_method_name(has_visibility,visibility);
			check_template_method_name(has_temperature,temperature);
			check_template_method_name(has_wind_speed,wind_speed);
			check_concept(is_prototype, Is_Weather_Event_Prototype, T, V);
			define_sub_check(is_component, has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
			define_default_check(is_prototype || is_component);
		};
		concept struct Is_Accident_Event_Prototype
		{
			check_template_method_name(has_type,Component_Type::accident_type);
			check_template_method_name(has_severity,Component_Type::severity);
			check_template_method_name(has_lanes,Component_Type::lanes);
			define_default_check(has_type & has_severity & has_lanes);
		};
		concept struct Is_Accident_Event
		{
			check_template_method_name(has_type,accident_type);
			check_template_method_name(has_severity,severity);
			check_template_method_name(has_lanes,lanes);
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
			check_template_method_name(has_intersections, Component_Type::intersections_container);
			check_template_method_name(has_links, Component_Type::links_container);
			check_template_method_name(has_read_function, Component_Type::read_network_data); 
			define_default_check(has_intersections && has_links && has_read_function);
		};

		concept struct Is_Basic_Network
		{
			check_template_method_name(has_intersections, intersections_container);
			check_template_method_name(has_links, links_container);
			check_template_method_name(has_read_function, read_network_data); 

			check_concept(is_basic_network_prototype, Is_Basic_Network_Prototype, T, V);
			define_sub_check(is_basic_network, has_intersections && has_links && has_read_function);
			define_default_check(is_basic_network || is_basic_network_prototype);
		};
		
		concept struct Is_Transportation_Network_Prototype
		{
			check_concept(is_basic_network, Is_Basic_Network_Prototype, T, V);
			check_template_method_name(has_turns, Component_Type::turn_movements_container);
			check_template_method_name(has_locations, Component_Type::activity_locations_container);
			check_template_method_name(has_zones, Component_Type::zones_container); 
			define_default_check(is_basic_network && has_turns && has_locations && has_zones);
		};

		concept struct Is_Transportation_Network
		{
			check_concept(is_basic_network, Is_Basic_Network, T, V);
			check_template_method_name(has_turns, turn_movements_container);
			check_template_method_name(has_locations, activity_locations_container);
			check_template_method_name(has_zones, zones_container); 
			//define_default_check(is_basic_network && has_turns && has_locations && has_zones);
			check_concept(is_transportation_network_prototype, Is_Transportation_Network_Prototype, T, V);
			define_sub_check(is_transportation_network, is_basic_network && has_turns && has_locations && has_zones);
			define_default_check(is_transportation_network || is_transportation_network_prototype);
		};
		
		concept struct Is_Routable_Network_Prototype
		{
			check_concept(is_basic_network, Is_Basic_Network_Prototype, T, V);
			check_template_method_name(has_routable_network, Component_Type::routable_network);
			check_template_method_name(has_routable_networks_container, Component_Type::routable_networks_container);
			check_template_method_name(has_scan_list, Component_Type::scan_list); 
			define_default_check(is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
		};

		concept struct Is_Routable_Network
		{
			check_concept(is_basic_network, Is_Basic_Network, T, V);
			check_template_method_name(has_routable_network, routable_network);
			check_template_method_name(has_routable_networks_container, routable_networks_container);
			check_template_method_name(has_scan_list, scan_list); 
			check_concept(is_routable_network_prototype, Is_Routable_Network_Prototype, T, V);
			define_sub_check(is_routable_network, is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
			define_default_check(is_routable_network || is_routable_network_prototype);
		};

		concept struct Is_Simulation_Network_Prototype
		{
			check_concept(is_routable_network, Is_Routable_Network_Prototype, T, V);
			check_template_method_name(has_scenario_reference, scenario_reference);
			check_template_method_name(has_max_free_flow_speed, max_free_flow_speed);
			define_default_check(is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Simulation_Network
		{
			check_concept(is_routable_network, Is_Routable_Network, T, V);
			check_template_method_name(has_scenario_reference, scenario_reference);
			check_template_method_name(has_max_free_flow_speed, max_free_flow_speed);
			check_concept(is_simulation_network_prototype, Is_Simulation_Network_Prototype, T, V);
			define_sub_check(is_simulation_networ, is_routable_network && has_scenario_reference && has_max_free_flow_speed);
			define_default_check(is_simulation_networ || is_simulation_network_prototype);
		};		
		
		concept struct Is_Trasportation_Simulation_Network_Prototype
		{
			check_concept(is_transportation_network, Is_Transportation_Network_Prototype, T, V);
			check_concept(is_routable_network, Is_Routable_Network_Prototype, T, V);
			check_template_method_name(has_scenario_reference, Component_Type::scenario_reference);
			check_template_method_name(has_max_free_flow_speed, Component_Type::max_free_flow_speed);
			define_default_check(is_transportation_network && is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Trasportation_Simulation_Network
		{
			check_concept(is_transportation_network, Is_Transportation_Network, T, V);
			check_concept(is_routable_network, Is_Routable_Network, T, V);
			check_template_method_name(has_scenario_reference, scenario_reference);
			check_template_method_name(has_max_free_flow_speed, max_free_flow_speed);

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
			check_template_method_name(has_auto_ttime, Component_Type::auto_ttime);
			check_template_method_name(has_transit_ttime, Component_Type::transit_ttime);
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
			check_template_method_name(has_routable_network, ComponentType::routable_network);
			check_template_method_name(has_routable_origin, ComponentType::routable_origin);
			check_template_method_name(has_routable_destination, ComponentType::routable_destination);
			check_template_method_name(has_network, ComponentType::network);
			check_template_method_name(has_vehicle, ComponentType::vehicle);
			define_default_check(has_routable_network && has_routable_origin && has_routable_destination && has_network && has_vehicle );
		};
		concept struct Is_One_To_One_Router
		{
			check_template_method_name(has_routable_network, routable_network);
			check_template_method_name(has_routable_origin, routable_origin);
			check_template_method_name(has_routable_destination, routable_destination);
			check_template_method_name(has_network, network);
			check_template_method_name(has_vehicle, vehicle);
			check_concept(is_one_to_one_prototype, Is_One_To_One_Router_Prototype, T, V);
			define_default_check(is_one_to_one_prototype || (has_routable_network && has_routable_origin && has_routable_destination && has_network && has_vehicle ));
		};

		concept struct Is_One_To_All_Router_Prototype
		{
			check_template_method_name(Has_Link_Times_Container, ComponentType::travel_times_to_link_container);
			check_concept(Has_One_To_One_Router, Is_One_To_One_Router_Prototype, T, V);
			define_default_check(Has_One_To_One_Router && Has_Link_Times_Container);
		};
		concept struct Is_One_To_All_Router
		{
			check_template_method_name(Has_Link_Times_Container, travel_times_to_link_container);
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
			check_template_method_name(has_percent_to_synthesize_p, Component_Type::percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance_p, Component_Type::ipf_tolerance);
			check_template_method_name(has_marginal_tolerance_p, Component_Type::marginal_tolerance);
			check_template_method_name(has_maximum_iterations_p, Component_Type::maximum_iterations);
			check_template_method_name(has_write_marginal_output_p, Component_Type::write_marginal_output);
			check_template_method_name(has_write_full_output_p, Component_Type::write_full_output);
			check_template_method_name(has_popsyn_control_file_name_p, Component_Type::popsyn_control_file_name);

			check_template_method_name(has_percent_to_synthesize, percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance, ipf_tolerance);
			check_template_method_name(has_marginal_tolerance, marginal_tolerance);
			check_template_method_name(has_maximum_iterations, maximum_iterations);
			check_template_method_name(has_write_marginal_output,write_marginal_output);
			check_template_method_name(has_write_full_output,write_full_output);
			check_template_method_name(has_popsyn_control_file_name,popsyn_control_file_name);
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
			check_template_method_name(has_router,ComponentType::router);
			check_template_method_name(has_vehicle,ComponentType::vehicle);
			define_default_check(has_router && has_vehicle);
		};
		concept struct Is_Traveler
		{
			check_template_method_name(has_router,router);
			check_template_method_name(has_vehicle,vehicle);
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
			check_template_method_name(has_origin_activity_locations, Component_Type::origin_activity_locations);
			check_template_method_name(has_destination_activity_locations, Component_Type::destination_activity_locations);
			check_template_method_name(has_uuid, Component_Type::uuid); 
			check_template_method_name(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
		};
		concept struct Is_Zone
		{
			check_template_method_name(has_origin_activity_locations, origin_activity_locations);
			check_template_method_name(has_destination_activity_locations, destination_activity_locations);
			check_template_method_name(has_uuid, uuid); 
			check_template_method_name(has_internal_id, internal_id); 

			check_concept(is_zone_prototype, Is_Zone_Prototype, T, V);
			define_sub_check(is_zone_component, has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
			define_default_check(is_zone_component || is_zone_prototype);
		};
	}
}