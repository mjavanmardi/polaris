#pragma once

#include "Polaris_Link_Base.h"
#include "Routing_Interface.h"

namespace Link_Components
{ 
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{
		struct Routable_Link_Base {

			typedef Link_Components::Components::Routable_Link_Component routable_link_type;
			typedef Link_Components::Interfaces::Link_Interface<routable_link_type, NULLTYPE> routable_link_interface;
			typedef vector<routable_link_interface*> routable_links_container_type;
			typedef Link_Components::Interfaces::Link_Interface<Link_Components::Components::Polaris_Link_Component, NULLTYPE>* network_link_reference_type;

			member_data_basic(float, f_cost);
			member_data_basic(float, h_cost);
			member_data_basic(routable_link_interface*, label_pointer);
			member_data_basic(float, label_cost);
			member_data_basic(network_link_reference_type, network_link_reference);

		};
		
	}

	namespace Components
	{
		typedef Polaris_Component<Link_Components::Interfaces::Link_Interface, Link_Components::Bases::Routable_Link_Base> Routable_Link_Component;
	}	

}