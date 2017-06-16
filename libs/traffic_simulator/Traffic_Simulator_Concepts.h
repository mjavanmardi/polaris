#pragma once

#include "Core.h"


namespace Activity_Location_Components
{
	namespace Concepts
	{
		concept struct Is_Activity_Location_Prototype 
		{
			check_component_accessor_name(has_origin_links, origin_links);
			check_component_accessor_name(has_destination_links, destination_links);
			check_component_accessor_name(has_zone, zone);
			check_component_accessor_name(has_uuid, uuid);
			check_component_accessor_name(has_internal_id, internal_id);
			define_default_check(has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
		};

		concept struct Is_Activity_Location
		{
			check_accessor_name(has_origin_links, origin_links);
			check_accessor_name(has_destination_links, destination_links);
			check_accessor_name(has_zone, zone);
			check_accessor_name(has_uuid, uuid);
			check_accessor_name(has_internal_id, internal_id);

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
			check_component_accessor_name(has_upstream_intersection, upstream_intersection);
			check_component_accessor_name(has_downstream_intersection, downstream_intersection);
			check_component_accessor_name(has_network_reference, network_reference);
			check_component_accessor_name(has_link_type, link_type);
			check_component_accessor_name(has_uuid, uuid);
			check_component_accessor_name(has_internal_id, internal_id);
			define_default_check(has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
		};

		concept struct Is_Basic_Link
		{
			check_accessor_name(has_upstream_intersection, upstream_intersection);
			check_accessor_name(has_downstream_intersection, downstream_intersection);
			check_accessor_name(has_network_reference, network_reference);
			check_accessor_name(has_link_type, link_type);
			check_accessor_name(has_uuid, uuid);
			check_accessor_name(has_internal_id, internal_id);

			check_concept(is_basic_link_prototype, Is_Basic_Link_Prototype, T, V);
			define_sub_check(is_basic_link, has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
			define_default_check(is_basic_link || is_basic_link_prototype);
		};
	}
}

namespace Network_Components
{
	namespace Concepts
	{
		concept struct Is_Basic_Network_Prototype
		{
			check_component_accessor_name(has_intersections, intersections_container);
			check_component_accessor_name(has_links, links_container);
			//%%%RLW check_component_accessor_name(has_read_function, read_network_data);
			define_default_check(has_intersections && has_links /*&& has_read_function*/);
		};

		concept struct Is_Basic_Network
		{
			check_accessor_name(has_intersections, intersections_container);
			check_accessor_name(has_links, links_container);
			//%%%RLW check_accessor_name(has_read_function, read_network_data);

			check_concept(is_basic_network_prototype, Is_Basic_Network_Prototype, T, V);
			define_sub_check(is_basic_network, has_intersections && has_links /*&& has_read_function*/);
			define_default_check(is_basic_network || is_basic_network_prototype);
		};
		
		concept struct Is_Transportation_Network_Prototype
		{
			// TODO: this check fails
			//static const bool value = true;
			check_concept(is_basic_network, Is_Basic_Network_Prototype, T, V);
			check_component_accessor_name(has_turns, turn_movements_container);
			check_component_accessor_name(has_locations, activity_locations_container);
			check_component_accessor_name(has_zones, zones_container);
			define_default_check(is_basic_network && has_turns && has_locations && has_zones);
		};

		concept struct Is_Transportation_Network
		{
			check_concept(is_basic_network, Is_Basic_Network, T, V);
			check_accessor_name(has_turns, turn_movements_container);
			check_accessor_name(has_locations, activity_locations_container);
			check_accessor_name(has_zones, zones_container);
			//define_default_check(is_basic_network && has_turns && has_locations && has_zones);
			check_concept(is_transportation_network_prototype, Is_Transportation_Network_Prototype, T, V);
			define_sub_check(is_transportation_network, is_basic_network && has_turns && has_locations && has_zones);
			define_default_check(is_transportation_network || is_transportation_network_prototype);
		};
		
		concept struct Is_Routable_Network_Prototype
		{
			check_concept(is_basic_network, Is_Basic_Network_Prototype, T, V);
			check_component_accessor_name(has_routable_network, routable_network);
			check_component_accessor_name(has_routable_networks_container, routable_networks_container);
			check_component_accessor_name(has_scan_list, scan_list);
			define_default_check(is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
		};

		concept struct Is_Routable_Network
		{
			check_concept(is_basic_network, Is_Basic_Network, T, V);
			check_accessor_name(has_routable_network, routable_network);
			check_accessor_name(has_routable_networks_container, routable_networks_container);
			check_accessor_name(has_scan_list, scan_list);
			check_concept(is_routable_network_prototype, Is_Routable_Network_Prototype, T, V);
			define_sub_check(is_routable_network, is_basic_network && has_routable_network && has_routable_networks_container && has_scan_list);
			define_default_check(is_routable_network || is_routable_network_prototype);
		};

		concept struct Is_Simulation_Network_Prototype
		{
			check_concept(is_routable_network, Is_Routable_Network_Prototype, T, V);
			check_accessor_name(has_scenario_reference, scenario_reference);
			check_accessor_name(has_max_free_flow_speed, max_free_flow_speed);
			define_default_check(is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Simulation_Network
		{
			check_concept(is_routable_network, Is_Routable_Network, T, V);
			check_accessor_name(has_scenario_reference, scenario_reference);
			check_accessor_name(has_max_free_flow_speed, max_free_flow_speed);
			check_concept(is_simulation_network_prototype, Is_Simulation_Network_Prototype, T, V);
			define_sub_check(is_simulation_networ, is_routable_network && has_scenario_reference && has_max_free_flow_speed);
			define_default_check(is_simulation_networ || is_simulation_network_prototype);
		};		
		
		concept struct Is_Trasportation_Simulation_Network_Prototype
		{
			check_concept(is_transportation_network, Is_Transportation_Network_Prototype, T, V);
			check_concept(is_routable_network, Is_Routable_Network_Prototype, T, V);
			check_component_accessor_name(has_scenario_reference, scenario_reference);
			check_component_accessor_name(has_max_free_flow_speed, max_free_flow_speed);
			define_default_check(is_transportation_network && is_routable_network && has_scenario_reference && has_max_free_flow_speed);
		};

		concept struct Is_Trasportation_Simulation_Network
		{
			check_concept(is_transportation_network, Is_Transportation_Network, T, V);
			check_concept(is_routable_network, Is_Routable_Network, T, V);
			check_accessor_name(has_scenario_reference, scenario_reference);
			check_accessor_name(has_max_free_flow_speed, max_free_flow_speed);

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
			check_component_accessor_name(has_auto_ttime, auto_ttime);
			check_component_accessor_name(has_transit_ttime, transit_ttime);
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
			check_component_accessor_name(has_routable_network, routable_network);
			check_component_accessor_name(has_routable_origin, routable_origin);
			check_component_accessor_name(has_routable_destination, routable_destination);
			check_component_accessor_name(has_network, network);
			check_component_accessor_name(has_vehicle, vehicle);
			define_default_check(has_routable_network && has_routable_origin && has_routable_destination && has_network && has_vehicle );
		};
		concept struct Is_One_To_One_Router
		{
			check_accessor_name(has_routable_network, routable_network);
			check_accessor_name(has_routable_origin, routable_origin);
			check_accessor_name(has_routable_destination, routable_destination);
			check_accessor_name(has_network, network);
			check_accessor_name(has_vehicle, vehicle);
			check_concept(is_one_to_one_prototype, Is_One_To_One_Router_Prototype, T, V);
			define_default_check(is_one_to_one_prototype || (has_routable_network && has_routable_origin && has_routable_destination && has_network && has_vehicle ));
		};

		concept struct Is_One_To_All_Router_Prototype
		{
			check_component_accessor_name(Has_Link_Times_Container, travel_times_to_link_container);
			check_concept(Has_One_To_One_Router, Is_One_To_One_Router_Prototype, T, V);
			define_default_check(Has_One_To_One_Router && Has_Link_Times_Container);
		};
		concept struct Is_One_To_All_Router
		{
			check_accessor_name(Has_Link_Times_Container, travel_times_to_link_container);
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
			check_component_accessor_name(has_percent_to_synthesize_p, percent_to_synthesize);
			check_component_accessor_name(has_ipf_tolerance_p, ipf_tolerance);
			check_component_accessor_name(has_marginal_tolerance_p, marginal_tolerance);
			check_component_accessor_name(has_maximum_iterations_p, maximum_iterations);
			check_component_accessor_name(has_write_marginal_output_p, write_marginal_output);
			check_component_accessor_name(has_write_full_output_p, write_full_output);
			check_component_accessor_name(has_popsyn_control_file_name_p, popsyn_control_file_name);

			check_accessor_name(has_percent_to_synthesize, percent_to_synthesize);
			check_accessor_name(has_ipf_tolerance, ipf_tolerance);
			check_accessor_name(has_marginal_tolerance, marginal_tolerance);
			check_accessor_name(has_maximum_iterations, maximum_iterations);
			check_accessor_name(has_write_marginal_output,write_marginal_output);
			check_accessor_name(has_write_full_output,write_full_output);
			check_accessor_name(has_popsyn_control_file_name, popsyn_control_file_name);
			define_sub_check(Has_Popsyn_Configuration_Data_Prototype, has_percent_to_synthesize_p && has_ipf_tolerance_p && has_marginal_tolerance_p && has_maximum_iterations_p && has_write_marginal_output_p && has_write_full_output_p && has_popsyn_control_file_name_p);
			define_sub_check(Has_Popsyn_Configuration_Data_Component, has_percent_to_synthesize && has_ipf_tolerance && has_marginal_tolerance && has_maximum_iterations && has_write_marginal_output && has_write_full_output && has_popsyn_control_file_name);
			define_default_check(Has_Popsyn_Configuration_Data_Prototype || Has_Popsyn_Configuration_Data_Component);
		};
	}
}


namespace Zone_Components
{
	namespace Concepts
	{
		concept struct Is_Zone_Prototype
		{
			check_component_accessor_name(has_origin_activity_locations, origin_activity_locations);
			check_component_accessor_name(has_destination_activity_locations, destination_activity_locations);
			check_component_accessor_name(has_uuid, uuid);
			check_component_accessor_name(has_internal_id, internal_id);
			define_default_check(has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
		};
		concept struct Is_Zone
		{
			check_accessor_name(has_origin_activity_locations, origin_activity_locations);
			check_accessor_name(has_destination_activity_locations, destination_activity_locations);
			check_accessor_name(has_uuid, uuid);
			check_accessor_name(has_internal_id, internal_id);

			check_concept(is_zone_prototype, Is_Zone_Prototype, T, V);
			define_sub_check(is_zone_component, has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
			define_default_check(is_zone_component || is_zone_prototype);
		};
	}
}