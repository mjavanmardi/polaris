#pragma once
#include "Repository_User.h"
#include "Routable_Intersection_Base.h"
#include "Routable_Link_Base.h"
#include "Polaris_Vechicle_Base.h"

namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{

		struct Routable_Network_Base
		{

			typedef set<pair<float, int>> scan_list_type;
			member_data_basic(scan_list_type, scan_list);
			member_data_basic(float, max_free_flow_speed);
			
			typedef Intersection_Components::Components::Routable_Intersection_Component intersections_elementType;
			typedef Intersection_Components::Interfaces::Intersection_Interface<intersections_elementType, NULLTYPE> intersection_interface;
			typedef vector<intersection_interface*> intersections_type;
			member_data_basic(intersections_type, intersections);

			typedef Link_Components::Components::Routable_Link_Component links_element_type;
			typedef Link_Components::Interfaces::Link_Interface<links_element_type, NULLTYPE> link_interface;
			typedef vector<link_interface*> links_type;
			member_data_basic(links_type, links);

			typedef Link_Components::Interfaces::Link_Interface<Link_Components::Components::Polaris_Link_Component, NULLTYPE> network_link_interface;
			typedef vector<network_link_interface*> reversed_path_container_type; 
			member_data_basic(reversed_path_container_type, reversed_path_container);
		};

		struct Polaris_Routing_Base
		{
			typedef Vehicle_Components::Components::Polaris_Vehicle_Component vehicle_type;
			typedef Vehicle_Components::Interfaces::Vehicle_Interface<vehicle_type, NULLTYPE> vehicle_interface;
			member_data_basic(vehicle_interface*, vehicle);

			typedef Routing_Components::Components::Routable_Network_Component routable_network_type;
			typedef Routing_Components::Interfaces::Routable_Network_Interface<routable_network_type, NULLTYPE> routable_network_interface;
			member_data_basic(routable_network_interface*, routable_network);

			member_data_basic(Routable_Network_Base::link_interface*, original_link);
			member_data_basic(Routable_Network_Base::link_interface*, destination_link);


		};
	}

	namespace Components
	{
		typedef Polaris_Component<Routing_Components::Interfaces::Routable_Network_Interface, Routing_Components::Bases::Routable_Network_Base> Routable_Network_Component;
		typedef Polaris_Component<Routing_Components::Interfaces::Routing_Interface, Routing_Components::Bases::Polaris_Routing_Base> Polaris_Routing_Component;
	}	

}